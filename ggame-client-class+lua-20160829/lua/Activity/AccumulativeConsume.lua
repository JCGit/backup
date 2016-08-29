----------------------------------------------------------------------------------
--[[
	FILE:			AccumulativeConsume.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			hgs
	CREATED:		2014-09-02
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local NodeHelper = require("NodeHelper");
local thisPageName = "AccumulativeConsume"

local opcodes = {
	ACC_CONSUME_INFO_C 		= HP_pb.ACC_CONSUME_INFO_C,
	ACC_CONSUME_INFO_S		= HP_pb.ACC_CONSUME_INFO_S,
	GET_ACC_CONSUME_AWARD_C	= HP_pb.GET_ACC_CONSUME_AWARD_C,
	GET_ACC_CONSUME_AWARD_S	= HP_pb.GET_ACC_CONSUME_AWARD_S
};
local option = {
	ccbiFile = "Act_RechargeConsumptionPage.ccbi",
	handlerMap = {
		onReturnButton	= "onBack",
		onRecharge		= "onRecharge",
		onHelp			= "onHelp"
	},
	opcode = opcodes
};

--活动基本信息，结构基本与协议pb Message相同
local thisActivityInfo = {
	id				= 6,
	remainTime 		= 0,
	hasConsumed 	= 0,
	rewardCfg		= {},
	rewardIds 		= {},
	gotIds			= {}
};
thisActivityInfo.timerName = "Activity_" .. thisActivityInfo.id;
thisActivityInfo.itemCfg = ActivityConfig[thisActivityInfo.id]["items"] or {};
thisActivityInfo.rewardIds = common:table_keys(thisActivityInfo.itemCfg);
table.sort(thisActivityInfo.rewardIds);

----------------- local data -----------------
local AccumulativeConsumeBase = {}


local ConsumeItem = {
	ccbiFile = "Act_RechargeConsumptionContent.ccbi",
}

function ConsumeItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ConsumeItem.onRefreshItemView(container);
	elseif eventName == "onRewardBtn" then
		ConsumeItem.onReceiveReward(container);
	elseif eventName:sub(1, 7) == "onFrame" then
		ConsumeItem.showItemInfo(container, eventName);
	end		
end

function ConsumeItem.onRefreshItemView(container)
	local index = container:getItemDate().mID;
	local itemCfgId = thisActivityInfo.rewardIds[index];
	local rewardItem = thisActivityInfo.itemCfg[itemCfgId];
	if rewardItem == nil then return; end
	
	local cfg = thisActivityInfo.rewardCfg[itemCfgId];
	if cfg == nil then
		cfg = ConfigManager.getRewardById(rewardItem.r);
		thisActivityInfo.rewardCfg[itemCfgId] = cfg;
	end
	local amount = rewardItem.n;
	NodeHelper:fillRewardItem(container, cfg);	
	NodeHelper:setStringForLabel(container, {
		mConsumptionGoldReward 	= common:getLanguageString("@AccumulativeConsumeItem", amount),
		mReceiveNum				= ""
	});
	local canReceive = amount <= thisActivityInfo.hasConsumed and not thisActivityInfo.gotIds[itemCfgId];
	NodeHelper:setMenuItemEnabled(container, "mRewardBtn", canReceive);	
end

function ConsumeItem.onReceiveReward(container)
	local index = container:getItemDate().mID;
	local itemCfgId = thisActivityInfo.rewardIds[index];
	local rewardItem = thisActivityInfo.itemCfg[itemCfgId];
	if rewardItem == nil then return; end
	
	local msg = Activity_pb.HPGetAccConsumeAward();
	msg.awardCfgId = tonumber(itemCfgId);
	common:sendPacket(opcodes.GET_ACC_CONSUME_AWARD_C, msg);
end

function ConsumeItem.showItemInfo(container, eventName)
	local index = container:getItemDate().mID;
	local itemCfgId = thisActivityInfo.rewardIds[index];
	local cfg = thisActivityInfo.rewardCfg[itemCfgId];
	local rewardIndex = tonumber(eventName:sub(8));
	GameUtil:showTip(container:getVarNode('mFrame' .. rewardIndex), cfg[rewardIndex]);
end
-----------------------------------------------
--AccumulativeConsumeBase页面中的事件处理
----------------------------------------------
function AccumulativeConsumeBase:onEnter(container)
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	NodeHelper:initScrollView(container, "mContent", 3);
	if container.mScrollView ~=nil then
		container:autoAdjustResizeScrollview(container.mScrollView);
	end	
	self:clearPage(container);
	self:registerPacket(container);
	self:getActivityInfo(container);
end

function AccumulativeConsumeBase:onExecute(container)
	self:onTimer(container);
end

function AccumulativeConsumeBase:onExit(container)
	TimeCalculator:getInstance():removeTimeCalcultor(thisActivityInfo.timerName);
	NodeHelper:deleteScrollView(container);
	self:removePacket(container);
end
----------------------------------------------------------------
function AccumulativeConsumeBase:onTimer(container)
	local timerName = thisActivityInfo.timerName;
	if not TimeCalculator:getInstance():hasKey(timerName) then return; end

	local remainTime = TimeCalculator:getInstance():getTimeLeft(timerName);
	if remainTime + 1 > thisActivityInfo.remainTime then
		return;
	end

	thisActivityInfo.remainTime = math.max(remainTime, 0);
	local timeStr = common:second2DateString(thisActivityInfo.remainTime, false);
	NodeHelper:setStringForLabel(container, {mActivityDaysNum = timeStr});
end

function AccumulativeConsumeBase:clearPage(container)
	NodeHelper:setStringForLabel(container, {
		mActivityDaysNum	= "",
		mConsumptionGoldNum	= "",
	});
end

function AccumulativeConsumeBase:getActivityInfo(container)
	local msg = Activity_pb.HPAccConsumeInfo();
	common:sendPacket(opcodes.ACC_CONSUME_INFO_C, msg);
end

function AccumulativeConsumeBase:refreshPage(container)
	local consumeStr = thisActivityInfo.hasConsumed;	-- .. common:getLanguageString("@Gold");
	NodeHelper:setStringForLabel(container, {mConsumptionGoldNum = consumeStr});
	if thisActivityInfo.remainTime > 0 and not TimeCalculator:getInstance():hasKey(timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(thisActivityInfo.timerName, thisActivityInfo.remainTime);
	end
	self:rebuildAllItem(container)
end
----------------scrollview-------------------------
function AccumulativeConsumeBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function AccumulativeConsumeBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function AccumulativeConsumeBase:buildItem(container)
	NodeHelper:buildScrollView(container, #thisActivityInfo.rewardIds, ConsumeItem.ccbiFile, ConsumeItem.onFunction);
end
	
----------------click event------------------------	
function AccumulativeConsumeBase:onClose(container)
	PageManager.popPage(thisPageName);
end	

function AccumulativeConsumeBase:onRecharge(container)
	PageManager.pushPage("RechargePage");
end

function AccumulativeConsumeBase:onBack()
	PageManager.changePage("ActivityPage");
end

--回包处理
function AccumulativeConsumeBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.ACC_CONSUME_INFO_S then
		local msg = Activity_pb.HPAccConsumeInfoRet();
		msg:ParseFromString(msgBuff);
		
		thisActivityInfo.hasConsumed = msg.accConsumeGold;
		thisActivityInfo.remainTime = msg.surplusTime;
		thisActivityInfo.gotIds = {};
		for _, id in ipairs(msg.gotAwardCfgId) do
			thisActivityInfo.gotIds[id] = true;
		end
		self:refreshPage(container);
		return;
	end

    if opcode == opcodes.GET_ACC_CONSUME_AWARD_S then
		local msg = Activity_pb.HPGetAccConsumeAwardRet();
		msg:ParseFromString(msgBuff);
		
		thisActivityInfo.gotIds[msg.gotAwardCfgId] = true;
		thisActivityInfo.remainTime = msg.surplusTime;
		self:refreshPage(container);
		ActivityInfo:decreaseReward(thisActivityInfo.id);
	end
end

function AccumulativeConsumeBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function AccumulativeConsumeBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
AccumulativeConsume = CommonPage.newSub(AccumulativeConsumeBase, thisPageName, option);

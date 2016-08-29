----------------------------------------------------------------------------------
--[[
	FILE:			ContinueRecharge.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			hgs
	CREATED:		2014-08-23
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local NodeHelper = require("NodeHelper");
local thisPageName = "ContinueRecharge"

local opcodes = {
	CONTINUE_RECHARGE_INFO_C 		= HP_pb.CONTINUE_RECHARGE_INFO_C,
	CONTINUE_RECHARGE_INFO_S		= HP_pb.CONTINUE_RECHARGE_INFO_S,
	GET_CONTINUE_RECHARGE_AWARD_C	= HP_pb.GET_CONTINUE_RECHARGE_AWARD_C,
	GET_CONTINUE_RECHARGE_AWARD_S	= HP_pb.GET_CONTINUE_RECHARGE_AWARD_S
};
local option = {
	ccbiFile = "Act_RechargeRebatePage.ccbi",
	handlerMap = {
		onReturnButton	= "onBack",
		onRecharge		= "onRecharge",
		onHelp			= "onHelp"
	},
	opcode = opcodes
};

--活动基本信息，结构基本与协议pb Message相同
local thisActivityInfo = {
	id				= 3,
	remainTime 		= 0,
	hasRecharged 	= 0,
	rewardCfg		= {},
	rewardIds 		= {}
};
thisActivityInfo.timerName = "Activity_" .. thisActivityInfo.id;
thisActivityInfo.itemCfg = ActivityConfig[thisActivityInfo.id]["items"] or {};

----------------- local data -----------------
local ContinueRechargeBase = {}


local RechargeItem = {
	ccbiFile = "Act_RechargeRebateContent.ccbi",
}

function RechargeItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		RechargeItem.onRefreshItemView(container);
	elseif eventName == "onRewardBtn" then
		RechargeItem.onReceiveReward(container);
	elseif eventName:sub(1, 7) == "onFrame" then
		RechargeItem.showItemInfo(container, eventName);	
	end		
end

function RechargeItem.onRefreshItemView(container)
	local index = container:getItemDate().mID;
	local itemCfgId = thisActivityInfo.rewardIds[index];
	local rewardItem = thisActivityInfo.itemCfg[itemCfgId];
	if rewardItem == nil then return; end
	
	local cfg = thisActivityInfo.rewardCfg[itemCfgId];
	if cfg == nil then
		cfg = ConfigManager.getRewardById(rewardItem.r);
		thisActivityInfo.rewardCfg[itemCfgId] = cfg;
	end
	local day = rewardItem.d;
	NodeHelper:fillRewardItem(container, cfg);	
	NodeHelper:setStringForLabel(container, {mRechargeRebateName = common:getLanguageString("@ContinueRechargeItem", day)});
	local canReceive = day <= thisActivityInfo.hasRecharged;
	NodeHelper:setMenuItemEnabled(container, "mRewardBtn", canReceive);
end

function RechargeItem.onReceiveReward(container)
	local index = container:getItemDate().mID;
	local itemCfgId = thisActivityInfo.rewardIds[index];
	local rewardItem = thisActivityInfo.itemCfg[itemCfgId];
	if rewardItem == nil then return; end
	
	local msg = Activity_pb.HPGetContinueRechargeAward();
	msg.awardCfgId = tonumber(itemCfgId);
	common:sendPacket(opcodes.GET_CONTINUE_RECHARGE_AWARD_C, msg);
end

function RechargeItem.showItemInfo(container, eventName)
	local index = container:getItemDate().mID;
	local itemCfgId = thisActivityInfo.rewardIds[index];
	local cfg = thisActivityInfo.rewardCfg[itemCfgId];
	local rewardIndex = tonumber(eventName:sub(8));
	GameUtil:showTip(container:getVarNode('mFrame' .. rewardIndex), common:table_merge(cfg[rewardIndex],{buyTip=true,hideBuyNum=true}));
end
-----------------------------------------------
--ContinueRechargeBase页面中的事件处理
----------------------------------------------
function ContinueRechargeBase:onEnter(container)
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

function ContinueRechargeBase:onExecute(container)
	self:onTimer(container);
end

function ContinueRechargeBase:onExit(container)
	TimeCalculator:getInstance():removeTimeCalcultor(thisActivityInfo.timerName);
	NodeHelper:deleteScrollView(container);
	self:removePacket(container);
end
----------------------------------------------------------------
function ContinueRechargeBase:onTimer(container)
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

function ContinueRechargeBase:clearPage(container)
	NodeHelper:setStringForLabel(container, {
		mActivityDaysNum	= "",
		mRechargeDaysNum	= ""
	});
end

function ContinueRechargeBase:getActivityInfo(container)
	local msg = Activity_pb.HPContinueRechargeInfo();
	common:sendPacket(opcodes.CONTINUE_RECHARGE_INFO_C, msg);
end

function ContinueRechargeBase:refreshPage(container)
	local dayStr = thisActivityInfo.hasRecharged .. common:getLanguageString("@Days");
	NodeHelper:setStringForLabel(container, {mRechargeDaysNum = dayStr});
	if thisActivityInfo.remainTime > 0 and not TimeCalculator:getInstance():hasKey(timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(thisActivityInfo.timerName, thisActivityInfo.remainTime);
	end
	self:rebuildAllItem(container)
end
----------------scrollview-------------------------
function ContinueRechargeBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function ContinueRechargeBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function ContinueRechargeBase:buildItem(container)
	NodeHelper:buildScrollView(container, #thisActivityInfo.rewardIds, RechargeItem.ccbiFile, RechargeItem.onFunction);
end
	
----------------click event------------------------	
function ContinueRechargeBase:onClose(container)
	PageManager.popPage(thisPageName);
end	

function ContinueRechargeBase:onRecharge(container)
	PageManager.pushPage("RechargePage");
end

function ContinueRechargeBase:onBack()
	PageManager.changePage("ActivityPage");
end

--回包处理
function ContinueRechargeBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.CONTINUE_RECHARGE_INFO_S then
		local msg = Activity_pb.HPContinueRechargeInfoRet();
		msg:ParseFromString(msgBuff);
		
		thisActivityInfo.hasRecharged = msg.continueRechargedays;
		thisActivityInfo.remainTime = msg.surplusTime;
		local rewardIds = common:table_keys(thisActivityInfo.itemCfg);
		table.sort(rewardIds);
		for _, id in ipairs(msg.gotAwardCfgId) do
			rewardIds = common:table_removeFromArray(rewardIds, id);
		end
		thisActivityInfo.rewardIds = rewardIds;
		self:refreshPage(container);
		return;
	end

    if opcode == opcodes.GET_CONTINUE_RECHARGE_AWARD_S then
		local msg = Activity_pb.HPGetContinueRechargeAwardRet();
		msg:ParseFromString(msgBuff);
		
		thisActivityInfo.rewardIds = common:table_removeFromArray(thisActivityInfo.rewardIds, msg.gotAwardCfgId);
		thisActivityInfo.remainTime = msg.surplusTime;
		self:refreshPage(container);
		ActivityInfo:decreaseReward(thisActivityInfo.id);
	end
end

function ContinueRechargeBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function ContinueRechargeBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ContinueRecharge = CommonPage.newSub(ContinueRechargeBase, thisPageName, option);

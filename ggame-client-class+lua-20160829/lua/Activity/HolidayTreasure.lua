----------------------------------------------------------------------------------
--[[
	FILE:			HolidayTreasure.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			hgs
	CREATED:		2014-09-24
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local NodeHelper = require("NodeHelper");
local thisPageName = "HolidayTreasure"

local COUNT_TREASURE_MAX = 10;

local opcodes = {
	HOLIDAY_TREASURE_C	= HP_pb.HOLIDAY_TREASURE_C,
	HOLIDAY_TREASURE_S	= HP_pb.HOLIDAY_TREASURE_S
};

local option = {
	ccbiFile = "Act_HolidayTreasurePage.ccbi",
	handlerMap = {
		onReturnButton		= "onBack",
		onHelp				= "onHelp",
		onLuxuryChest		= "showLuxuryTreasure",
		onCelebrateChest	= "showCelebrateTreasure",
		onViewBackpack		= "viewPackage",
		onGoBattle			= "goBattle",
		onGoShop			= "goShop"
	},
	opcode = opcodes
};

for i = 1, COUNT_TREASURE_MAX do
	option.handlerMap[string.format("onRewardFrame%02d", i)] = "showTreasure"
end

local TreasureId = {
	Luxury 		= 1,
	Celebrate 	= 2
};

--活动基本信息，结构基本与协议pb Message相同
local thisActivityInfo = {
	id				= 10,
	treasureId		= TreasureId.Luxury,
	remainTime 		= 0,
	rewardCfg		= {}
};
thisActivityInfo.timerName = "Activity_" .. thisActivityInfo.id;
thisActivityInfo.itemCfg = ActivityConfig[thisActivityInfo.id]["items"] or {};

----------------- local data -----------------
local HolidayTreasureBase = {};

-----------------------------------------------
--HolidayTreasureBase页面中的事件处理
----------------------------------------------
function HolidayTreasureBase:onEnter(container)
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	self:clearPage(container);
	self:registerPacket(container);
	self:getActivityInfo(container);
	--container:registerMessage(MSG_MAINFRAME_REFRESH);
	--self:showLuxuryTreasure(container);
	self:showTreasurePackage(container);
end

function HolidayTreasureBase:onExecute(container)
	self:onTimer(container);
end

function HolidayTreasureBase:onExit(container)
	TimeCalculator:getInstance():removeTimeCalcultor(thisActivityInfo.timerName);
	self:removePacket(container);
	--container:removeMessage(MSG_MAINFRAME_REFRESH);
end
----------------------------------------------------------------
function HolidayTreasureBase:onTimer(container)
	local timerName = thisActivityInfo.timerName;
	if not TimeCalculator:getInstance():hasKey(timerName) then return; end

	local remainTime = TimeCalculator:getInstance():getTimeLeft(timerName);
	if remainTime + 1 > thisActivityInfo.remainTime then
		return;
	end

	thisActivityInfo.remainTime = math.max(remainTime, 0);
	local timeStr = common:second2DateString(thisActivityInfo.remainTime, false);
	NodeHelper:setStringForLabel(container, {mCD = timeStr});
end

function HolidayTreasureBase:clearPage(container)
	NodeHelper:setStringForLabel(container, {
		mCD	= ""
	});
end

function HolidayTreasureBase:getActivityInfo(container)
	local msg = Activity_pb.HPHolidayTreasure();
	common:sendPacket(opcodes.HOLIDAY_TREASURE_C, msg);
end

function HolidayTreasureBase:refreshPage(container)
	if thisActivityInfo.remainTime > 0 and not TimeCalculator:getInstance():hasKey(timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(thisActivityInfo.timerName, thisActivityInfo.remainTime);
	end
end

function HolidayTreasureBase:showMenuSelected(container)
	local menu2TreasureId = {
		mLuxuryChest 	= TreasureId.Luxury,
		mCelebrateChest	= TreasureId.Celebrate
	};
	local selectedTb = {};
	for name, treasureId in pairs(menu2TreasureId) do
		selectedTb[name] = treasureId == thisActivityInfo.treasureId;
	end
	NodeHelper:setMenuItemSelected(container, selectedTb);
end
---------------------------------------------------
function HolidayTreasureBase:showTreasurePackage(container)
	local nodesVisible = {};
	local lb2Str = {};
	local sprite2Img = {};
	local img2Quality = {};
	
	for _, id in pairs(TreasureId) do
		local rewardId = thisActivityInfo.itemCfg[id];
		local i = 1 + (id - 1) * 5;
		nodesVisible[string.format('mRewardPicNode%02d', i)] = rewardId ~= nil;
		if rewardId then
			local rewardCfg = ConfigManager.getRewardById(rewardId);
			local resInfo = ResManagerForLua:getResInfoByTypeAndId(rewardCfg[1].type, rewardCfg[1].itemId, rewardCfg[1].count);
			sprite2Img[string.format('mRewardPic%02d', i)] = resInfo.icon;
			img2Quality[string.format('mRewardFrame%02d', i)] = resInfo.quality;
		end
	end
	NodeHelper:setNodesVisible(container, nodesVisible);
	NodeHelper:setSpriteImage(container, sprite2Img);
	NodeHelper:setQualityFrames(container, img2Quality);
end

function HolidayTreasureBase:showTreasureOptions(container)
	local rewardIds = thisActivityInfo.itemCfg[thisActivityInfo.treasureId];
	
	local nodesVisible = {};
	local lb2Str = {};
	local sprite2Img = {};
	local img2Quality = {};
	for i = 1, COUNT_TREASURE_MAX do
		local rewardId = rewardIds[i];
		nodesVisible[string.format('mRewardPicNode%02d', i)] = rewardId ~= nil;
		if rewardId then
			local rewardCfg = ConfigManager.getRewardById(rewardId);
			local resInfo = ResManagerForLua:getResInfoByTypeAndId(rewardCfg[1].type, rewardCfg[1].itemId, rewardCfg[1].count);
			sprite2Img[string.format('mRewardPic%02d', i)] = resInfo.icon;
			img2Quality[string.format('mRewardFrame%02d', i)] = resInfo.quality;
		end
	end
	NodeHelper:setNodesVisible(container, nodesVisible);
	NodeHelper:setSpriteImage(container, sprite2Img);
	NodeHelper:setQualityFrames(container, img2Quality);
end

----------------click event------------------------	
function HolidayTreasureBase:onClose(container)
	PageManager.popPage(thisPageName);
end	

function HolidayTreasureBase:onHelp(container)
	--PageManager.showHelp(GameConfig.HelpKey.HELP_HOLIDYTREASURE);
end

function HolidayTreasureBase:onBack()
	PageManager.changePage("ActivityPage");
end

function HolidayTreasureBase:viewPackage()
	PackagePage_showItems();
end

function HolidayTreasureBase:goBattle()
	PageManager.showFightPage();
end

function HolidayTreasureBase:goShop()
	PageManager.changePage("MarketPage");
end

function HolidayTreasureBase:showLuxuryTreasure(container)
	thisActivityInfo.treasureId = TreasureId.Luxury;
	self:showTreasureOptions(container);
	self:showMenuSelected(container);
end

function HolidayTreasureBase:showCelebrateTreasure(container)
	thisActivityInfo.treasureId = TreasureId.Celebrate;
	self:showTreasureOptions(container);
	self:showMenuSelected(container);
end

function HolidayTreasureBase:showTreasure(container, eventName)
	local index = tonumber(eventName:sub(-2));
	local rewardIndex = math.ceil(index / 5);
	local rewardId = thisActivityInfo.itemCfg[rewardIndex];
	local rewardCfg = ConfigManager.getRewardById(rewardId);
	GameUtil:showTip(container:getVarNode(string.format('mRewardFrame%02d', index)), rewardCfg[1]);
end

function HolidayTreasureBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();

	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:showExchangeItem(container);
		end
	end
end

--回包处理
function HolidayTreasureBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.HOLIDAY_TREASURE_S then
		local msg = Activity_pb.HPHolidayTreasureRet();
		msg:ParseFromString(msgBuff);
		
		thisActivityInfo.remainTime = msg.leftTimes;

		self:refreshPage(container);
		return;
	end
end

function HolidayTreasureBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function HolidayTreasureBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
HolidayTreasure = CommonPage.newSub(HolidayTreasureBase, thisPageName, option);
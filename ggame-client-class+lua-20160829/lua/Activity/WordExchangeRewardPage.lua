----------------------------------------------------------------------------------
--[[
	FILE:			WordExchangeRewardPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
------------local variable for system api--------------------------------------
local ceil = math.ceil;
--------------------------------------------------------------------------------
local HP_pb = require("HP_pb");
local UserInfo = require("UserInfo");
local Const_pb = require("Const_pb");

local thisPageName = "WordExchangeRewardPage";

local option = {
	ccbiFile = "Act_TextExchangeRewardPopUp.ccbi",
	handlerMap = {
		onClose			= "onClose"
	}
};

local WordExchangeRewardPageBase = {}

local NodeHelper = require("NodeHelper");
local ItemManager = require("ItemManager");
local UserItemManager = require("UserItemManager");
local ItemOprHelper = require("ItemOprHelper");

local thisItemId = 10002;
local thisFinDays = 0;
local thisRewardId = 0;
--------------------------------------------------------------
local RewardItem = {
	ccbiFile = "Act_TextExchangeRewardContent.ccbi"
};

function RewardItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		RewardItem.onRefreshItemView(container);
	elseif eventName == "onExchange" then
		RewardItem.onExchange(container);
	end
end

function RewardItem.onExchange(container)
	local contentId = container:getItemDate().mID;
	
	local cfg = GameConfig.WordExchangeReward[contentId];
	local nextCfg = GameConfig.WordExchangeReward[contentId + 1];
	local rewardCfg = ConfigManager.getRewardById(cfg.r);
	
	local rewardStr = ResManagerForLua:getResStr(rewardCfg);
	local nextStr = "";
	if nextCfg then
		local gap = nextCfg.d - cfg.d;
		nextStr = common:getLanguageString("@ExchangeLater", gap);
	end
	
	local title = common:getLanguageString("@WordExchangeTitle");
	local msg = common:getLanguageString("@WordExchangeMsg", rewardStr, nextStr);
	
	PageManager.showConfirm(title, msg, function(isSure)
		if isSure then
			ItemOprHelper:useItem(thisItemId);
		end
	end);
end

function RewardItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;
	local cfg = GameConfig.WordExchangeReward[contentId];
	local rewardCfg = ConfigManager.getRewardById(cfg.r);
	
	local dayStr = "";
	if contentId == #GameConfig.WordExchangeReward and contentId > 1 then
		local preCfg = GameConfig.WordExchangeReward[contentId - 1];
		dayStr = common:getLanguageString("@DaysAfter", preCfg.d);
	else
		dayStr = common:getLanguageString("@OrderedDay", cfg.d)
	end
	local lb2Str = {
		mDay 			= dayStr,
		mExchangeReward = ResManagerForLua:getResStr(rewardCfg)
	};
	NodeHelper:setStringForLabel(container, lb2Str);
	local btnVisible = thisFinDays > 0;
	NodeHelper:setNodesVisible(container, {mExchangeNode = contentId == thisRewardId});
	
	local color = common:getColorFromConfig(contentId == thisRewardId and "Own" or "Grey");
	NodeHelper:setColor3BForLabel(container, {
		mDay			 = color,
		mExchangeReward	 = color
	});
end	
----------------------------------------------------------------------------------
	
-----------------------------------------------
--WordExchangeRewardPageBase页面中的事件处理
----------------------------------------------
function WordExchangeRewardPageBase:onEnter(container)
	NodeHelper:initScrollView(container, "mContent", 4);
	
	self:refreshPage(container);
	container:registerPacket(HP_pb.ITEM_USE_S);
end

function WordExchangeRewardPageBase:onExit(container)
	NodeHelper:deleteScrollView(container);
	container:removePacket(HP_pb.ITEM_USE_S);
	self:clearCache();
end
----------------------------------------------------------------
function WordExchangeRewardPageBase:refreshPage(container)
	self:getRewardId();
	self:showConsume(container);
	self:showExplain(container);
	self:rebuildAllItem(container);
end

function WordExchangeRewardPageBase:getRewardId()
	thisFinDays = UserInfo.stateInfo.gongceCompleteDays or 0;
	thisRewardId = 0;
	if thisFinDays > 0 then
		for id, cfg in ipairs(GameConfig.WordExchangeReward) do
			if thisFinDays >= cfg.d then
				thisRewardId = id;
			end
		end
	end
end

function WordExchangeRewardPageBase:showConsume(container)
	local UserItemManager = require("UserItemManager");
	local lb2Str, picMap, nodesVisible = {}, {}, {};
	for i, itemId in ipairs(GameConfig.WordExchangeConsume) do
		local resInfo = ResManagerForLua:getResInfoByMainTypeAndId(Const_pb.TOOL, itemId, 1);
		local ownCount = UserItemManager:getCountByItemId(itemId) or 0;
		lb2Str[string.format("mTextNum%02d", i)] = "x" .. ownCount;
		picMap[string.format("mTextPic%02d", i)] = resInfo.icon;
		nodesVisible[string.format("mTextPicBG%02d", i)] = ownCount < 1;
	end
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, picMap);
	NodeHelper:setNodesVisible(container, nodesVisible);
end

function WordExchangeRewardPageBase:showExplain(container)
	local explainTex = "";
	if thisFinDays > 0 then
		explainTex = common:getLanguageString("@WordExchangeExplain", thisFinDays);
	else
		explainTex = common:getLanguageString("@WordExchangeNotFin");
	end
	NodeHelper:setStringForLabel(container, {mExplain = explainTex});
end

function WordExchangeRewardPageBase:clearCache()

end

----------------scrollview-------------------------
function WordExchangeRewardPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	if #GameConfig.WordExchangeReward > 0 then
		self:buildItem(container);
	end
end

function WordExchangeRewardPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function WordExchangeRewardPageBase:buildItem(container)
	local size = #GameConfig.WordExchangeReward;
	NodeHelper:buildScrollView(container, size, RewardItem.ccbiFile, RewardItem.onFunction);
end
	
----------------click event------------------------
function WordExchangeRewardPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function WordExchangeRewardPageBase:onConfirm(container)
	if PageInfo.callback then
		PageInfo.callback(PageInfo.selectedId);
	end
	self:onClose();
end

function WordExchangeRewardPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == HP_pb.ITEM_USE_S then
		self:refreshPage(container);
		return;
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
WordExchangeRewardPage = CommonPage.newSub(WordExchangeRewardPageBase, thisPageName, option)

function WordExchangeRewardPage_setItemId(itemId)
	thisItemId = itemId;
end	
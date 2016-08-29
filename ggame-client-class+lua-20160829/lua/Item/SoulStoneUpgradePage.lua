----------------------------------------------------------------------------------
--[[
	FILE:			SoulStoneUpgradePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	进阶之书升级页面（魂石）
	AUTHOR:			zhaolu
	CREATED:		2014-11-10
--]]
----------------------------------------------------------------------------------
local Const_pb 		= require("Const_pb");
local HP_pb			= require("HP_pb");
local ItemOpr_pb		= require("ItemOpr_pb");
local UserInfo = require("UserInfo");
--------------------------------------------------------------------------------
local thisPageName 	= "SoulStoneUpgradePage";
local thisItemId	= 0;
local SoulStoneIds = {
	90002,
	90003,
	90004,
	90005,
	90006,
}

local opcodes = {
	ITEM_USE_S	= HP_pb.ITEM_USE_S
};

local option = {
	ccbiFile = "MercenarySynthesisBookPopUp.ccbi",
	handlerMap = {
		onSynthesis		 		= "onUpgrade",
		onHelp					= "onHelp",
		onClose					= "onClose"
	},
};
for i=1, #SoulStoneIds do
	option.handlerMap["onHand" .. i] = "onBookHand"
end

local SoulStoneUpgradeBase = {};
local NodeHelper = require("NodeHelper");
local ItemOprHelper = require("ItemOprHelper");
local ItemManager = require("ItemManager");
local UserItemManager = require("UserItemManager");
-----------------------------------------------
--SoulStoneUpgradeBase页面中的事件处理
----------------------------------------------
function SoulStoneUpgradeBase:onEnter(container)
	self:registerPacket(container);
	self:refreshPage(container);
end

function SoulStoneUpgradeBase:onExit(container)
	self:removePacket(container);
end
----------------------------------------------------------------
function SoulStoneUpgradeBase:refreshPage(container)
	thisItemId = ItemManager:getNowSelectItem()
	self:showSoulStoneInfo(container);
	self:showUpgradeInfo(container);
end

function SoulStoneUpgradeBase:showSoulStoneInfo(container)
	local lb2Str = {}
	local sprite2Img = {}
	local scaleMap = {}
	local handMap = {}
	local lb2StrColor = {}
	for i=1, #SoulStoneIds do
		local name	= ItemManager:getNameById(SoulStoneIds[i]);
		lb2Str["mNumber" .. i] = ""
		lb2Str["mName" .. i] = name
		lb2StrColor["mName" .. i] = ItemManager:getQualityById(SoulStoneIds[i])
		sprite2Img["mPic"..i] = ItemManager:getIconById(SoulStoneIds[i])
		scaleMap["mPic"..i] = 1.0
		handMap["mHand"..i]	= ItemManager:getQualityById(SoulStoneIds[i])
		
		NodeHelper:setNodeVisible(container:getVarNode("mTexBG"..i),((thisItemId%10)-1 == i))
	end
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, handMap);
	NodeHelper:setQualityBMFontLabels(container,lb2StrColor)
end

function SoulStoneUpgradeBase:showUpgradeInfo(container)
	-- 是否是最后一本书
	NodeHelper:setNodesVisible(container, {mCanUpgradeBookNode = (thisItemId%10~=6)})
	NodeHelper:setNodesVisible(container, {mHighestBookExplain = (thisItemId%10==6)})
	NodeHelper:setMenuEnabled(container:getVarMenuItemImage("mSoulStoneUpgradeUpBtn"),(thisItemId%10~=6))

	local userItem = UserItemManager:getUserItemByItemId(thisItemId)
	local levelUpCost = ItemManager:getLevelUpCost(thisItemId);
	local costMax = ItemManager:getLevelUpCostMax(thisItemId);
	costMax = math.max(1, costMax);
	local costTb = {};
	local lineNumber = 1
	for _, costCfg in ipairs(levelUpCost) do
		local resInfo = ResManagerForLua:getResInfoByTypeAndId(costCfg.type, costCfg.id, costCfg.count);
		if resInfo ~= nil then
			local ownNum = 0;
			if resInfo.mainType == Const_pb.PLAYER_ATTR and costCfg.id == Const_pb.COIN then
				UserInfo.syncPlayerInfo();
				ownNum = UserInfo.playerInfo.coin;
			elseif resInfo.mainType == Const_pb.PLAYER_ATTR and costCfg.id == Const_pb.GOLD then
				UserInfo.syncPlayerInfo();
				ownNum = UserInfo.playerInfo.gold;
			else
				ownNum = UserItemManager:getCountByItemId(costCfg.id);
			end
			if lineNumber == 1 then
				NodeHelper:setStringForLabel(container ,{
					mDiamondsNum = resInfo.name .. common:getLanguageString("@CurrentOwnInfo", costCfg.count, ownNum)
				})
			elseif lineNumber == 2 then
				NodeHelper:setStringForLabel(container, {
					mSynthesisBook 				= resInfo.name,
 					mSynthesisBookNum  			= common:getLanguageString("@CurrentOwnInfo", costCfg.count, ownNum)
				})
				NodeHelper:setQualityBMFontLabels(container,{mSynthesisBook = ItemManager:getQualityById(thisItemId)})
			end
			lineNumber = lineNumber + 1
		end
	end
	
	NodeHelper:setStringForLabel(container, {
		mTitle  				= common:getLanguageString("@SoulStoneUpgradeTitle"),
	})
	NodeHelper:setNodesVisible(container,{mGold = (thisItemId%10~=6)})
end
	
----------------click event------------------------
function SoulStoneUpgradeBase:onBookHand( container, eventName )
	local index = tonumber(eventName:sub(-1))
	ItemManager:setNowSelectItem(SoulStoneIds[index])
	self:refreshPage(container)
end

function SoulStoneUpgradeBase:onUpgrade(container)
	ItemOprHelper:useItem(thisItemId);
end

function SoulStoneUpgradeBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_GEM_UPGRADE);
end	

function SoulStoneUpgradeBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--回包处理
function SoulStoneUpgradeBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.ITEM_USE_S then
		-- local msg = ItemOpr_pb.HPItemUseRet();
		-- msg:ParseFromString(msgBuff);
		local tipKey = "@SoulStoneUpgradeSuccess";
		local colorKey = "COLOR_GREEN";

		common:popString(common:getLanguageString(tipKey), colorKey);
		self:refreshPage(container);
	end
end

function SoulStoneUpgradeBase:registerPacket(container)
	container:registerPacket(opcodes.ITEM_USE_S)
end

function SoulStoneUpgradeBase:removePacket(container)
	container:removePacket(opcodes.ITEM_USE_S)
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
SoulStoneUpgradePage = CommonPage.newSub(SoulStoneUpgradeBase, thisPageName, option);


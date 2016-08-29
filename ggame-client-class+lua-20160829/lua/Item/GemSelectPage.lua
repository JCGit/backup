----------------------------------------------------------------------------------
--[[
	FILE:			GemSelectPage.lua
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
local Const_pb = require("Const_pb");

local thisPageName = "GemSelectPage";

local option = {
	ccbiFile = "EquipmentCameoIncrustationChoosePopUp.ccbi",
	handlerMap = {
		onHelp	= "onHelp",
		onClose	= "onClose"
	}
};

local GemSelectPageBase = {}

local NodeHelper = require("NodeHelper");
local ItemManager = require("ItemManager");
local UserItemManager = require("UserItemManager");
local EquipOprHelper = require("EquipOprHelper");

local PageInfo = {
	equipId = 0,
	pos = 1,
	gemQuality = {},
	optionIds = {}
};

local GEM_COUNT_PER_LINE = 5;
--------------------------------------------------------------
local GemItemLine = {
	ccbiFile = "EquipmentCameoIncrustationChooseContent.ccbi",
	itemCcbi = "GoodsItem.ccbi"
};

function GemItemLine.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		GemItemLine.onRefreshItemView(container);
	end
end

local function GemItemOnFunction(eventName, container)
	if eventName == "onHand" then
		local gemId = container:getTag();
		local quality = ItemManager:getQualityById(gemId);
		if common:table_hasValue(PageInfo.gemQuality, quality) then
			MessageBoxPage:Msg_Box_Lan("@SameQualityGemSelected");
		else
			EquipOprHelper:embedEquip(PageInfo.equipId, PageInfo.pos, gemId);
			PageManager.popPage(thisPageName);
		end
	end
end

function GemItemLine.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;
	local baseIndex = (contentId - 1) * GEM_COUNT_PER_LINE;

	for i = 1, GEM_COUNT_PER_LINE do
		local nodeContainer = container:getVarNode(string.format("mPosition%d", i));
		NodeHelper:setNodeVisible(nodeContainer, false);
		nodeContainer:removeAllChildren();
		
		local index = baseIndex + i;
		local gemId = PageInfo.optionIds[index];
		if gemId then
			itemNode = GemItemLine.newGemItem(gemId);
			nodeContainer:addChild(itemNode);
			NodeHelper:setNodeVisible(nodeContainer, true);
		end
	end
end

function GemItemLine.newGemItem(gemId)
	local itemNode = ScriptContentBase:create(GemItemLine.itemCcbi, gemId);
	itemNode:registerFunctionHandler(GemItemOnFunction);
	
	local userItem = UserItemManager:getUserItemByItemId(gemId);
	local resInfo = ResManagerForLua:getResInfoByMainTypeAndId(Const_pb.TOOL, userItem.itemId, userItem.count);
	local lb2Str = {
		mName 	= resInfo.name,
		mNumber	= "x" .. userItem.count
	};
	NodeHelper:setStringForLabel(itemNode, lb2Str);
	NodeHelper:setSpriteImage(itemNode, {mPic = resInfo.icon});
	NodeHelper:setQualityFrames(itemNode, {mHand = resInfo.quality});

	itemNode:release();
	return itemNode;
end	
----------------------------------------------------------------------------------
	
-----------------------------------------------
--GemSelectPageBase页面中的事件处理
----------------------------------------------
function GemSelectPageBase:onEnter(container)
	NodeHelper:setStringForLabel(container, {mExplain = common:getLanguageString("@SelectGemToEmbed")});
	self:setOptionIds();
	
	NodeHelper:initScrollView(container, "mContent", 3);
	
	self:rebuildAllItem(container);
end

function GemSelectPageBase:onExit(container)
	NodeHelper:deleteScrollView(container);
	PageInfo.gemQuality = {};
end
----------------------------------------------------------------
function GemSelectPageBase:setOptionIds()
	PageInfo.optionIds = UserItemManager:getItemIdsByType(Const_pb.GEM);
end

----------------scrollview-------------------------
function GemSelectPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	if #PageInfo.optionIds > 0 then
		self:buildItem(container);
	end
end

function GemSelectPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function GemSelectPageBase:buildItem(container)
	local size = math.ceil(#PageInfo.optionIds / GEM_COUNT_PER_LINE);
	NodeHelper:buildScrollView(container, size, GemItemLine.ccbiFile, GemItemLine.onFunction);
end
	
----------------click event------------------------
function GemSelectPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
GemSelectPage = CommonPage.newSub(GemSelectPageBase, thisPageName, option)

function GemSelectPage_setEquipIdAndPos(equipId, pos, gemQuality)
	PageInfo.equipId 	= equipId;
	PageInfo.pos	 	= pos;
	PageInfo.gemQuality = gemQuality;
end	
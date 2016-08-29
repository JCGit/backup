----------------------------------------------------------------------------------
--[[
	FILE:			ItemSelectPage.lua
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

local thisPageName = "ItemSelectPage";

local option = {
	ccbiFile = "Act_Mid-autumnPropChoicePopUp.ccbi",
	handlerMap = {
		onHelp			= "onHelp",
		onClose			= "onClose",
		onConfirmation	= "onConfirm"
	}
};

local ItemSelectPageBase = {}

local NodeHelper = require("NodeHelper");
local ItemManager = require("ItemManager");
local UserItemManager = require("UserItemManager");
local EquipOprHelper = require("EquipOprHelper");

local PageInfo = {
	optionIds = {},
	selectedId	= nil,
	callback = nil
};

--------------------------------------------------------------
local ItemContent = {
	ccbiFile = "Act_Mid-autumnPropChoiceContent.ccbi"
};

function ItemContent.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ItemContent.onRefreshItemView(container);
	elseif eventName == "onChoiceBtn" then
		ItemContent.OnSelect(container);
	end
end

function ItemContent.OnSelect(container)
	local contentId = container:getItemDate().mID;
	local itemId = PageInfo.optionIds[contentId];
	
	if PageInfo.selectedId == nil or PageInfo.selectedId == itemId then
		NodeHelper:setNodesVisible(container, {mSelected = PageInfo.selectedId == nil});
		PageInfo.selectedId = PageInfo.selectedId == nil and itemId or nil;
	else
		PageInfo.selectedId = itemId;
		PageManager.refreshPage(thisPageName);
	end
end

function ItemContent.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;
	local itemId = PageInfo.optionIds[contentId];

	local userItem = UserItemManager:getUserItemByItemId(itemId);
	local resInfo = ResManagerForLua:getResInfoByMainTypeAndId(Const_pb.TOOL, userItem.itemId, userItem.count);
	local lb2Str = {
		mName 		= resInfo.name,
		mNumber		= "x" .. resInfo.count,
	--	mGoodsName	= resInfo.name,
		mGoodsTex	= common:stringAutoReturn(resInfo.describe, 11)
	};
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, {mPic = resInfo.icon});
	NodeHelper:setQualityFrames(container, {mHand = resInfo.quality});
	NodeHelper:setNodesVisible(container, {mSelected = PageInfo.selectedId == itemId});
end	
----------------------------------------------------------------------------------
	
-----------------------------------------------
--ItemSelectPageBase页面中的事件处理
----------------------------------------------
function ItemSelectPageBase:onEnter(container)
	NodeHelper:initScrollView(container, "mContent", 3);
	
	table.sort(PageInfo.optionIds);
	self:rebuildAllItem(container);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
end

function ItemSelectPageBase:onExit(container)
	NodeHelper:deleteScrollView(container);
	container:removeMessage(MSG_MAINFRAME_REFRESH);
	self:clearCache();
end
----------------------------------------------------------------
function ItemSelectPageBase:clearCache()
	PageInfo.optionIds = {};
	PageInfo.selectedId = nil;
	PageInfo.callback = nil;
end

----------------scrollview-------------------------
function ItemSelectPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	if #PageInfo.optionIds > 0 then
		self:buildItem(container);
	end
end

function ItemSelectPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function ItemSelectPageBase:buildItem(container)
	local size = #PageInfo.optionIds;
	NodeHelper:buildScrollView(container, size, ItemContent.ccbiFile, ItemContent.onFunction);
end
	
----------------click event------------------------
function ItemSelectPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function ItemSelectPageBase:onConfirm(container)
	if PageInfo.callback then
		PageInfo.callback(PageInfo.selectedId);
	end
	self:onClose();
end

function ItemSelectPageBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();

	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:rebuildAllItem(container);
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ItemSelectPage = CommonPage.newSub(ItemSelectPageBase, thisPageName, option)

function ItemSelectPage_setOptions(options, selectedId, callback)
	PageInfo.optionIds 	= options;
	PageInfo.selectedId	= selectedId;
	PageInfo.callback 	= callback;
end	
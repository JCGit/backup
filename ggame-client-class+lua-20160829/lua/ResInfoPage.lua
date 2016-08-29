----------------------------------------------------------------------------------
--[[
	FILE:			ResInfoPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-09-02
--]]
----------------------------------------------------------------------------------
local thisPageName 	= "ResInfoPage";
local itemInfo = {};
local NodeHelper = require("NodeHelper");
local option = {
	ccbiFile = "BackpackGoodsInfoPopUp.ccbi",
	handlerMap = {
		onButtonMiddle 	= "onClose",
		onClose			= "onClose"
	}
};

local ResInfoPageBase = {};
-----------------------------------------------
--ResInfoPageBase页面中的事件处理
----------------------------------------------
function ResInfoPageBase:onEnter(container)
	self:refreshPage(container);
end
----------------------------------------------------------------

function ResInfoPageBase:refreshPage(container)
	self:showItemInfo(container);
	self:showButtons(container);
end

function ResInfoPageBase:showItemInfo(container)
	local resInfo = ResManagerForLua:getResInfoByTypeAndId(itemInfo.type, itemInfo.itemId, itemInfo.count);
	local lb2Str = {
		mName 			= resInfo.name,
		mNumber			= "",
		mGoodsInfoTex 	= common:stringAutoReturn(resInfo.describe or "", GameConfig.LineWidth.ItemDescribe)
	};
	local sprite2Img = {
		mPic = resInfo.icon
	};
	local itemImg2Qulity = {
		mHand = resInfo.quality
	};
	local scaleMap = {mPic = 1.0};	
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
end


function ResInfoPageBase:showButtons(container)
	local btnVisible = {
		mButtonMiddleNode	= true,
		mButtonDoubleNode	= false
	};
	NodeHelper:setNodesVisible(container, btnVisible);
	NodeHelper:setStringForLabel(container, {mButtonMiddle = common:getLanguageString("@Cancel")});
end
----------------click event------------------------
function ResInfoPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ResInfoPage = CommonPage.newSub(ResInfoPageBase, thisPageName, option);

function ResInfoPage_setItemInfo(type, itemId, count)
	itemInfo = {
		type = type,
		itemId = itemId,
		count = count or 1
	};
end
----------------------------------------------------------------------------------
--[[
	FILE:			ExchangeBetaWord.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-09-24
--]]
----------------------------------------------------------------------------------
------------local variable for system api--------------------------------------
--------------------------------------------------------------------------------
local thisPageName = "ExchangeBetaWord";
local NodeHelper = require("NodeHelper");
local HP_pb = require('HP_pb');

local opcodes = {
	ITEM_INFO_SYNC_S = HP_pb.ITEM_INFO_SYNC_S
};
local option = {
	ccbiFile = "Act_NewWelfarePage.ccbi",
	handlerMap = {
		onClose			= "onClose",
		onReturnButton	= "onBack",
		onViewBackpack	= "viewPackage",
		onCDKExchange	= "exchangeCDK"
	},
	opcode = opcodes
};

local ExchangeBetaWordBase = {}
--------------------------------------------------------------
-----------------------------------------------
--ExchangeBetaWordBase页面中的事件处理
----------------------------------------------
function ExchangeBetaWordBase:onEnter(container)
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	self:refreshPage(container);
	self:registerPacket(container);
end

function ExchangeBetaWordBase:onExit(container)
	self:removePacket(container);
end
----------------------------------------------------------------
function ExchangeBetaWordBase:refreshPage(container)
	self:showCDKInfo(container);
	self:showConsume(container);
end

function ExchangeBetaWordBase:showConsume(container)
	local UserItemManager = require("UserItemManager");
	local lb2Str, picMap, nodesVisible = {}, {}, {};
	for i, itemId in ipairs(GameConfig.WordExchangeConsume) do
		local resInfo = ResManagerForLua:getResInfoByMainTypeAndId(Const_pb.TOOL, itemId, 1);
		local ownCount = UserItemManager:getCountByItemId(itemId) or 0;
		lb2Str[string.format("mTextNum%02d", i)] = ownCount .. '/1';
		picMap[string.format("mTextPic%02d", i)] = resInfo.icon;
		nodesVisible[string.format("mTextPicBG%02d", i)] = ownCount < 1;
	end
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, picMap);
	NodeHelper:setNodesVisible(container, nodesVisible);
end

function ExchangeBetaWordBase:showCDKInfo(container)
	local str = common:fillHtmlStr('QMGJ_CDK');
	NodeHelper:addHtmlLable(container:getVarNode('mCDKRewardTex'), str, GameConfig.Tag.HtmlLable);
end

----------------click event------------------------
function ExchangeBetaWordBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function ExchangeBetaWordBase:onBack()
	PageManager.changePage("ActivityPage");
end

function ExchangeBetaWordBase:viewPackage()
	PackagePage_showItems();
end

function ExchangeBetaWordBase:exchangeCDK()
	PageManager.pushPage("CDKeyPage");
end

-------------------------------------------------------------------------
--回包处理
function ExchangeBetaWordBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()

	if opcode == opcodes.ITEM_INFO_SYNC_S then
		self:showConsume(container);
		return
	end
end

function ExchangeBetaWordBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function ExchangeBetaWordBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ExchangeBetaWord = CommonPage.newSub(ExchangeBetaWordBase, thisPageName, option);
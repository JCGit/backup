----------------------------------------------------------------------------------
--[[
	FILE:			ConfirmUseItemPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-09-03
--]]
----------------------------------------------------------------------------------

local option = {
	ccbiFile = "GeneralDecisionPopUp4.ccbi",
	handlerMap = {
		onCancel	= "onNo",
		onUse 		= "onYes"
	}
};

local thisPageName = "ConfirmUseItemPage";
local CommonPage = require("CommonPage");
local ConfirmUseItemPage = CommonPage.new("ConfirmUseItemPage", option);
local decisionTitle = "";
local decisionMsg = "";
local decisionCB = nil;

local NodeHelper = require("NodeHelper");
----------------------------------------------------------------------------------
--ConfirmUseItemPage页面中的事件处理
----------------------------------------------
function ConfirmUseItemPage.onEnter(container)
	NodeHelper:setStringForLabel(container, {
		mTitle 			= decisionTitle,
		mDecisionTex 	= common:stringAutoReturn(decisionMsg, 20)		--20: char per line
	});
end

function ConfirmUseItemPage.onExit(container)
	ConfirmUseItemPage_setDecision("", "", nil);
end

function ConfirmUseItemPage.onNo(container)
	if decisionCB then
		decisionCB(false);
	end
	PageManager.popPage(thisPageName)
end

function ConfirmUseItemPage.onYes(container)
	if decisionCB then
		decisionCB(true);
	end
	PageManager.popPage(thisPageName)
end	

-------------------------------------------------------------------------------
function ConfirmUseItemPage_setDecision(title, msg, callback)
	decisionTitle	= title;
	decisionMsg 	= msg;
	decisionCB		= callback;
end
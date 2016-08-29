----------------------------------------------------------------------------------
--[[
	FILE:			DecisionPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	恶魔果实：采摘活动期间介绍弹框
	AUTHOR:			hgs
	CREATED:		2014-02-10
--]]
----------------------------------------------------------------------------------

local option = {
	ccbiFile = "GeneralDecisionPopUp.ccbi",
	handlerMap = {
		onCancel		= "onNo",
		onConfirmation 	= "onYes"
	}
};

local thisPageName = "DecisionPage";
local CommonPage = require("CommonPage");
local DecisionPage = CommonPage.new("DecisionPage", option);
local decisionTitle = "";
local decisionMsg = "";
local decisionCB = nil;
local autoClose = true;

local NodeHelper = require("NodeHelper");
----------------------------------------------------------------------------------
--DecisionPage页面中的事件处理
----------------------------------------------
function DecisionPage.onEnter(container)
	DecisionPage.refreshPage(container);
	container:registerMessage(MSG_MAINFRAME_PUSHPAGE);
end

function DecisionPage.onExit(container)
	DecisionPage_setDecision("", "", nil);
	container:removeMessage(MSG_MAINFRAME_PUSHPAGE);
end

function DecisionPage.refreshPage(container)
	NodeHelper:setStringForLabel(container, {
		mTitle 			= decisionTitle,
		mDecisionTex 	= common:stringAutoReturn(decisionMsg, 20)		--20: char per line
	});
end

function DecisionPage.onNo(container)
	if decisionCB then
		decisionCB(false);
	end
	PageManager.popPage(thisPageName)
end

function DecisionPage.onYes(container)
	if decisionCB then
		decisionCB(true);
	end
	if autoClose then
		PageManager.popPage(thisPageName)
	end
end	

function DecisionPage.onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();
	if typeId == MSG_MAINFRAME_PUSHPAGE then
		local pageName = MsgMainFramePushPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			DecisionPage.refreshPage(container);
		end
	end
end

-------------------------------------------------------------------------------
function DecisionPage_setDecision(title, msg, callback, auto)
	decisionTitle	= title;
	decisionMsg 	= msg;
	decisionCB		= callback;
	autoClose		= (auto or auto == nil) and true or false;
end
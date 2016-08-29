----------------------------------------------------------------------------------
--[[
	FILE:			NoticePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	恶魔果实：采摘活动期间介绍弹框
	AUTHOR:			hgs
	CREATED:		2014-02-10
--]]
----------------------------------------------------------------------------------

local option = {
	ccbiFile = "GeneralDecisionPopUp2.ccbi",
	handlerMap = {
		onConfirmation = "onClose"
	}
};

local thisPageName = "NoticePage";
local CommonPage = require("CommonPage");
local NoticePage = CommonPage.new("NoticePage", option);
local noticeTitle = "";
local noticeMsg = "";

local NodeHelper = require("NodeHelper");
----------------------------------------------------------------------------------
--NoticePage页面中的事件处理
----------------------------------------------
function NoticePage.onEnter(container)
	NodeHelper:setStringForLabel(container, {
		mTitle 			= noticeTitle,
		mDecisionTex 	= common:stringAutoReturn(noticeMsg, 20)		--20: char per line
	});
end

function NoticePage.onClose(container)
	PageManager.popPage(thisPageName)
end	

-------------------------------------------------------------------------------
function NoticePage_setNotice(title, msg)
	noticeTitle = title;
	noticeMsg = msg;
end

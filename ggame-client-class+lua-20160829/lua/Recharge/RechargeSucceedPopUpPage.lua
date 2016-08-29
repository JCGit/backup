----------------------------------------------------------------------------------
--[[
	FILE:			RechargeSucceedPopUpPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	充值成功弹窗
	AUTHOR:			zhaolu
	CREATED:		2014-9-10
--]]
----------------------------------------------------------------------------------
local NodeHelper = require("NodeHelper")
local thisPageName = 'RechargeSucceedPopUpPage'
local RechargePopBase = {}

local option = {
	ccbiFile = "A_Recharge.ccbi",
	handlerMap = {
	}
}

function RechargePopBase.onAnimationDone(container)
	PageManager.popPage(thisPageName)
end

local CommonPage = require('CommonPage')
local RechargeSucceedPopUpPage= CommonPage.newSub(RechargePopBase, thisPageName, option)

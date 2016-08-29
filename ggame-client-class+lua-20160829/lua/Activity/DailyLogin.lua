----------------------------------------------------------------------------------
--[[
	FILE:			DailyLogin.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	开服活动
	AUTHOR:			zhaolu
	CREATED:		2014-10-10
--]]
----------------------------------------------------------------------------------
local thisPageName = 'DailyLogin'
local DailyLoginBase = {}

local option = {
	ccbiFile = "Act_NoviceActivitiesPage.ccbi",
	handlerMap = {
		onReturnButton	= "onBack",
	}
}

function DailyLoginBase:onEnter(container)
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
end

function DailyLoginBase:onBack(container)
	PageManager.changePage("ActivityPage")
end

local CommonPage = require('CommonPage')
local DailyLogin= CommonPage.newSub(DailyLoginBase, thisPageName, option)

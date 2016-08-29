----------------------------------------------------------------------------------
--[[
FILE:			EliteMapPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	精英副本
AUTHOR:			zhenhui
CREATED:		2014-10-16
--]]
----------------------------------------------------------------------------------
local Battle_pb = require "Battle_pb"
local Const_pb = require "Const_pb"
local UserInfo = require("UserInfo");

local thisPageName = "EliteMapPage"
local NodeHelper = require("NodeHelper");
local EliteMapManager = require("EliteMapManager")
local EliteMapCfg = ConfigManager.getEliteMapCfg();

local option = {
	ccbiFile = "EliteMapPage.ccbi",
	handlerMap = {
		onReturn = "onReturn",
		onHelp = "onHelp",
		onWarriorCareer = "onWarriorCareer",
		onHunterCareer = "onHunterCareer",
		onMasterCareer = "onMasterCareer"
	}
}

local EliteMapPageBase = {}

----------------------------------------------------------------------------------

-----------------------------------------------
--EliteMapPageBase页面中的事件处理
----------------------------------------------
function EliteMapPageBase:onEnter(container)
	
	self:refreshPage(container);		
end


function EliteMapPageBase:onExecute(container)
	
end

function EliteMapPageBase:onExit(container)
		
end
----------------------------------------------------------------

function EliteMapPageBase:refreshPage(container)
	if UserInfo.roleInfo.prof == 1 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = true,
			mHunterCareerBG01 = false,
			mMasterCareerBG01 = false,			
			mWarriorCareerPromptNode = false,
			mWarriorCareerSurplusNode = true,
			mHunterCareerPromptNode = true,
			mHunterCareerSurplusNode = false,
			mMasterCareerPromptNode = true,
			mMasterCareerSurplusNode = false,			
		})
		NodeHelper:setStringForLabel(container,{
			mWarriorCareerSurplusNum = tostring(UserInfo.stateInfo.eliteFightTimes)
		})
	elseif UserInfo.roleInfo.prof == 2 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = false,
			mHunterCareerBG01 = true,
			mMasterCareerBG01 = false,			
			mWarriorCareerPromptNode = true,
			mWarriorCareerSurplusNode = false,
			mHunterCareerPromptNode = false,
			mHunterCareerSurplusNode = true,
			mMasterCareerPromptNode = true,
			mMasterCareerSurplusNode = false,	
		})
		NodeHelper:setStringForLabel(container,{
			mHunterCareerSurplusNum = tostring(UserInfo.stateInfo.eliteFightTimes)
		})
	elseif UserInfo.roleInfo.prof == 3 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = false,
			mHunterCareerBG01 = false,
			mMasterCareerBG01 = true,			
			mWarriorCareerPromptNode = true,
			mWarriorCareerSurplusNode = false,
			mHunterCareerPromptNode = true,
			mHunterCareerSurplusNode = false,
			mMasterCareerPromptNode = false,
			mMasterCareerSurplusNode = true,	
		})
		NodeHelper:setStringForLabel(container,{
			mMasterCareerSurplusNum = tostring(UserInfo.stateInfo.eliteFightTimes)
		})
	end
	
end

----------------click event------------------------
function EliteMapPageBase:onReturn(container)
	PageManager.showFightPage()
end

function EliteMapPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_ELITEMAP)
end

function EliteMapPageBase:onWarriorCareer(container)
	if UserInfo.roleInfo.prof == 1 then 
		EliteMapManager:setCurCareerIndex(EliteMapManager.WarriorCareer)
		PageManager.changePage("EliteMapInfoPage")
	else 
		MessageBoxPage:Msg_Box_Lan("@EliteMapOnlyWarriorAllow")
	end
end

function EliteMapPageBase:onHunterCareer(container)
	if UserInfo.roleInfo.prof == 2 then 
		EliteMapManager:setCurCareerIndex(EliteMapManager.HunterCareer)
		PageManager.changePage("EliteMapInfoPage")
	else 
		MessageBoxPage:Msg_Box_Lan("@EliteMapOnlyHunterAllow")
	end		
end

function EliteMapPageBase:onMasterCareer(container)
	if UserInfo.roleInfo.prof == 3 then 
		EliteMapManager:setCurCareerIndex(EliteMapManager.MasterCareer)
		PageManager.changePage("EliteMapInfoPage")
	else 
		MessageBoxPage:Msg_Box_Lan("@EliteMapOnlyMasterAllow")
	end				
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local EliteMapPage = CommonPage.newSub(EliteMapPageBase, thisPageName, option);
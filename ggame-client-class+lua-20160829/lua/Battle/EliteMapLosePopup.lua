local Battle_pb =  require "Battle_pb"
local Const_pb = require "Const_pb"
local UserInfo = require("UserInfo");

local thisPageName = "EliteMapLosePopup"
local NodeHelper = require("NodeHelper");
local EliteMapManager = require("EliteMapManager")
local EliteMapCfg = ConfigManager.getEliteMapCfg();

local curMapId = 0

local option = {
	ccbiFile = "EliteMapBattleLosePopUp.ccbi",
	handlerMap = {
		onEquipmentStrengthen = "onEquipmentStrengthen",
		onMercenaryCulture = "onMercenaryCulture",
		onClose = "onClose",
		onConfirm = "onConfirm",
		onSkillAdjust = "onSkillAdjust"
	}
}	

local EliteMapLosePopup = {}

----------------------------------------------------------------------------------

-----------------------------------------------
--EliteMapWinPopup页面中的事件处理
----------------------------------------------
function EliteMapLosePopup:onEquipmentStrengthen(container)
	PageManager.changePage("EquipmentPage")
end

function EliteMapLosePopup:onMercenaryCulture(container)
	PageManager.changePage("MercenaryPage")
end

function EliteMapLosePopup:onSkillAdjust(container)
	PageManager.changePage("SkillPage")
end

function EliteMapLosePopup:onEnter(container)
	
	self:refreshPage(container);		
end


function EliteMapLosePopup:onExecute(container)
	
end

function EliteMapLosePopup:onExit(container)
	
end
----------------------------------------------------------------

function EliteMapLosePopup:refreshPage(container)
	
	if UserInfo.roleInfo.prof == 1 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = true,
			mHunterCareerBG01 = false,
			mMasterCareerBG01 = false,
		})
	elseif UserInfo.roleInfo.prof == 2 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = false,
			mHunterCareerBG01 = true,
			mMasterCareerBG01 = false,
		})
	elseif UserInfo.roleInfo.prof == 3 then 
		NodeHelper:setNodesVisible(container,{
			mWarriorCareerBG01 = false,
			mHunterCareerBG01 = false,
			mMasterCareerBG01 = true,
		})
	end
end

----------------click event------------------------
function EliteMapLosePopup:onClose(container)
	PageManager.popPage(thisPageName)
end

function EliteMapLosePopup:onConfirm(container)
	if curMapId~=nil and curMapId>0 then	
		local EliteMapManager = require("EliteMapManager")
		EliteMapManager.lastFightMapId = curMapId
	end
	PageManager.popPage(thisPageName)
	PageManager.changePage("EliteMapInfoPage")
end

function EliteMapLosePopup_setMapId(mapId)
	curMapId = mapId
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local EliteMapPage = CommonPage.newSub(EliteMapLosePopup, thisPageName, option);
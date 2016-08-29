local Battle_pb =  require "Battle_pb"
local Const_pb = require "Const_pb"
local UserInfo = require("UserInfo");

local thisPageName = "EliteMapWinPopup"
local NodeHelper = require("NodeHelper");
local EliteMapManager = require("EliteMapManager")
local EliteMapCfg = ConfigManager.getEliteMapCfg();

local option = {
	ccbiFile = "EliteMapBattleWinPopUp.ccbi",
	handlerMap = {
		onClose = "onClose",
		onConfirm = "onConfirm"
	}
}

local curAward = {}
local curMapId = 0
local EliteMapWinPopup = {}

----------------------------------------------------------------------------------

-----------------------------------------------
--EliteMapWinPopup页面中的事件处理
----------------------------------------------
function EliteMapWinPopup:onEnter(container)
	
	self:refreshPage(container);		
end


function EliteMapWinPopup:onExecute(container)
	
end

function EliteMapWinPopup:onExit(container)
	
end


function EliteMapWinPopup.onFunctionExt(eventName, container)
	if eventName:sub(1, 12) == "onRewardFeet" then
		
		local firstDigit = eventName:sub(13) 
		local index = 0
		if firstDigit == "0" then
			index = tonumber(eventName:sub(14))
		else
			index = tonumber(eventName:sub(13,14))
		end
		CCLuaLog("EliteMapRewardPopup:onFunctionExt -- index is "..index)
		EliteMapWinPopup:onFeetById(container,index)
		
	end	
end


function EliteMapWinPopup:onFeetById(container,index)
	local thisResList = EliteMapManager:getAwardItems(curAward)
	local resSize = #thisResList
	if index>resSize then
		return 
	else
		local resCfg = thisResList[index];	
		if resCfg then
			GameUtil:showTip(container:getVarNode(string.format("mRewardFeet%02d", index)), resCfg);
		end
	end
end
----------------------------------------------------------------

function EliteMapWinPopup:refreshPage(container)
		
	CCLuaLog(tostring(curAward))
	local thisResList = EliteMapManager:getAwardItems(curAward)
	local resSize = #thisResList
	
	for i=1,12 do
		local itemNode =string.format("mRewardNode%02d", i);		
		local flag = (i<=resSize)
		local ccbNode = container:getVarNode(itemNode)
		if ccbNode then
			ccbNode:setVisible(flag)
		end
	end
	 
	local index = 1 
	local lb2Str, quaMap, picMap,countMap = {}, {}, {},{};
	for _,value in pairs(thisResList) do
		
		local resCfg = value;		
		local resInfo = ResManagerForLua:getResInfoByTypeAndId(resCfg.type, resCfg.itemId, resCfg.count);				
		lb2Str[string.format("mRewardName%02d", index)]	= resInfo.name
		quaMap[string.format("mRewardFeet%02d", index)] = resInfo.quality;	
		picMap[string.format("mRewardPic%02d", index)] = resInfo.icon
		countMap[string.format("mRewardNum%02d", index)] = "x"..resInfo.count
		index = index + 1 
	end		
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setStringForLabel(container, countMap);
	NodeHelper:setSpriteImage(container, picMap);
	NodeHelper:setQualityFrames(container, quaMap);		
	NodeHelper:setStringForLabel(container,{
		mGetExpNum = tostring(curAward.exp),
		mGetGoldNum = tostring(curAward.coin),
	})

	
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
function EliteMapWinPopup:onClose(container)
	PageManager.popPage(thisPageName)
end

function EliteMapWinPopup:onConfirm(container)
	if curMapId~=nil and curMapId>0 then	
		local EliteMapManager = require("EliteMapManager")
		EliteMapManager.lastFightMapId = curMapId
	end
	PageManager.popPage(thisPageName)
	PageManager.changePage("EliteMapInfoPage")
end

function EliteMapWinPopup_setAward(award,mapId)
	curAward = award
	curMapId = mapId or 0
end


-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local EliteMapPage = CommonPage.newSub(EliteMapWinPopup, thisPageName, option,EliteMapWinPopup.onFunctionExt);
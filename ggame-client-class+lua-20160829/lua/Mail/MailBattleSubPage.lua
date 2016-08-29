----------------------------------------------------------------------------------
--[[
	FILE:			MailBattleSubPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	邮件:战报类型邮件，轮数具体信息页面
	AUTHOR:			Zhen Zhen
	CREATED:		2014-08-18
--]]
----------------------------------------------------------------------------------
--require "ExploreEnergyCore_pb"
local UserInfo = require("UserInfo");
local thisPageName = "MailBattleSubPage"
local NodeHelper = require("NodeHelper");
local opcodes = {
}
--页面基本信息
local option = {
	ccbiFile = "RegimentWarResultPopUp.ccbi",
	handlerMap = {
		onConfirmation		= "onClose"
	},
	opcode = opcodes
};

local MailBattleSubPageBase = {}
local vsList = {};
local teamInfo = {};

--------------------------------------------------------------
local MailBattleSubItem = {
	ccbiFile = "RegimentWarResultContent.ccbi"
};

function MailBattleSubItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		MailBattleSubItem.onRefreshItemView(container)
	elseif eventName == "onViewEnemy" then
		MailBattleSubItem.onViewEnemy( container )
	elseif eventName == "onViewAlly" then
		MailBattleSubItem.onViewAlly( container )	
	end
end

--刷新content信息
function MailBattleSubItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;

	local vsInfo = vsList[contentId];

	local leftPlayerId = vsInfo.leftPlayer.playId;
	local leftPlayerName = vsInfo.leftPlayer.name;
	local leftPlayerKillCount = vsInfo.leftPlayer.killCount;
	local leftPlayerRecord = vsInfo.leftPlayer.record;

	local rightPlayerId = vsInfo.rightPlayer.playId;
	local rightPlayerName = vsInfo.rightPlayer.name;
	local rightPlayerKillCount = vsInfo.rightPlayer.killCount;
	local rightPlayerRecord = vsInfo.rightPlayer.record;

	local leftKillStr = common:getLanguageString("@BattleKill", leftPlayerKillCount);
	local rightKillStr = common:getLanguageString("@BattleKill", rightPlayerKillCount);

	if leftPlayerKillCount <= 0 then
		leftKillStr = common:getLanguageString("@BattleNoKill");
	end

	if rightPlayerKillCount <= 0 then
		rightKillStr = common:getLanguageString("@BattleNoKill");
	end

	local leftPlayerStr = common:getLanguageString("@BattleScore", leftPlayerRecord);
	local rightPlayerStr = common:getLanguageString("@BattleScore", rightPlayerRecord);
    leftPlayerStr = GameMaths:stringAutoReturnForLua( leftPlayerStr , GameConfig.Default.MailBattleTeamNameCount , 0 ) 
	local vsStr = 
	{
		mAllyName = leftPlayerName,
		mAllyKills = leftKillStr,
		mAllyScore = leftPlayerStr,
		mEnemyName = rightPlayerName,
		mEnemyKills = rightKillStr,
		mEnemyScore = rightPlayerStr
	}

	NodeHelper:setStringForLabel(container, vsStr);

	--set color
	local colorMap = {};
	local layerMap = {};
	if UserInfo.playerInfo.playerId == leftPlayerId  then
		colorMap = 
		{
			mAllyName = GameConfig.ColorMap.COLOR_YELLOW,
			mEnemyName = GameConfig.ColorMap.COLOR_WHITE
		}

		layerMap = 
		{
			mPlayer = true
		}
	elseif UserInfo.playerInfo.playerId == rightPlayerId then
		colorMap = 
		{
			mAllyName = GameConfig.ColorMap.COLOR_WHITE,
			mEnemyName = GameConfig.ColorMap.COLOR_YELLOW
		}
		layerMap = 
		{
			mPlayer = false
		}
	else
		layerMap = 
		{
			mPlayer = false
		}
	end

	NodeHelper:setColorForLabel( container, colorMap )
	NodeHelper:setNodesVisible(container, layerMap)
end

function MailBattleSubItem.onViewAlly( container )
	local contentId = container:getItemDate().mID

	local vsInfo = vsList[contentId]

	local leftPlayerId = vsInfo.leftPlayer.playId
	
	PageManager.viewPlayerInfo(leftPlayerId, true)
end

function MailBattleSubItem.onViewEnemy( container )
	local contentId = container:getItemDate().mID

	local vsInfo = vsList[contentId]

	local rightPlayerId = vsInfo.rightPlayer.playId
	
	PageManager.viewPlayerInfo(rightPlayerId, true)
end
----------------------------------------------------------------------------------
	
-----------------------------------------------
--MailBattleSubPage页面中的事件处理
----------------------------------------------
function MailBattleSubPage_setDataList( dataList, teamList )
	vsList = {};
	vsList = common:deepCopy(dataList);
	teamInfo = {};
	teamInfo = common:deepCopy(teamList);
end


function MailBattleSubPageBase:onEnter(container)
	NodeHelper:initScrollView(container, "mContent", 6);
	
	self:refreshPage(container);
	self:rebuildAllItem(container);
end

--刷新页面基本信息
function MailBattleSubPageBase:refreshPage( container )
	local winStr = "";
	if teamInfo.isWin then
		winStr = common:getLanguageString("@Win");
	else
		winStr = common:getLanguageString("@Failed");
	end

	local basicInfo = 
	{
		mTitle = common:getLanguageString("@BattleRoundResult", teamInfo.roundId),
		mAllyTeam = "[ "..teamInfo.leftTeamName.." ]",
		mEnemyTeam = "[ "..teamInfo.rightTeamName.." ]",
		mWinOrLose = winStr
	}

	NodeHelper:setStringForLabel(container, basicInfo);

	local colorMap = {};
	if teamInfo.isWin then
		colorMap = {
			mAllyTeam = GameConfig.ColorMap.COLOR_GREEN,
			mEnemyTeam = GameConfig.ColorMap.COLOR_RED,
			mWinOrLose = GameConfig.ColorMap.COLOR_GREEN
		}
	else
		colorMap = {
			mAllyTeam = GameConfig.ColorMap.COLOR_RED,
			mEnemyTeam = GameConfig.ColorMap.COLOR_GREEN,
			mWinOrLose = GameConfig.ColorMap.COLOR_RED
		}
	end

	NodeHelper:setColorForLabel(container, colorMap);
end

function MailBattleSubPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function MailBattleSubPageBase:onExecute(container)
end

function MailBattleSubPageBase:onExit(container)
	NodeHelper:deleteScrollView(container);
end
----------------------------------------------------------------

----------------scrollview-------------------------
function MailBattleSubPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function MailBattleSubPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function MailBattleSubPageBase:buildItem(container)
	local maxSize = table.maxn(vsList);
	NodeHelper:buildScrollView(container, maxSize, MailBattleSubItem.ccbiFile, MailBattleSubItem.onFunction);
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local MailBattleSubPage = CommonPage.newSub(MailBattleSubPageBase, thisPageName, option);
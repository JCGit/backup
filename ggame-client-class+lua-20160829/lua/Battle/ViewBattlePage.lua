----------------------------------------------------------------------------------
--[[
FILE:			ViewBattlePage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	精英副本
AUTHOR:			zhenhui
CREATED:		2014-10-16
--]]
----------------------------------------------------------------------------------
require "Battle_pb"
require "Const_pb"

local thisPageName = "ViewBattlePage"
local NodeHelper = require("NodeHelper");

local option = {
	ccbiFile = "BattlePlaybackPopUp.ccbi",
	handlerMap = {
		onReturn = "onReturn",	
	}
}

ViewFightState = {
FightRequired = 1;			--请求战斗
BeforeFightStart = 2;		--战斗准备。人物准备和战斗开始动画播放
Fighting = 3;				--战斗序列
FightEnd = 4;				--战斗结束
Waiting =  5;				--等待状态
}

local ViewBattlePageBase = {}

----------------------------------------------------------------------------------

-----------------------------------------------
--ViewBattlePageBase页面中的事件处理
----------------------------------------------
function ViewBattlePageBase:onEnter(container)
	self:refreshPage(container);		
end


function ViewBattlePageBase:onExecute(container)
	
end

function ViewBattlePageBase:onExit(container)
		
end
----------------------------------------------------------------

function ViewBattlePageBase:refreshPage(container)
	
end

----------------click event------------------------
function ViewBattlePageBase:onReturn(container)
	PageManager.showFightPage()
end

function ViewBattlePageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_ViewBattle)
end	

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local ViewBattlePage = CommonPage.newSub(ViewBattlePageBase, thisPageName, option);
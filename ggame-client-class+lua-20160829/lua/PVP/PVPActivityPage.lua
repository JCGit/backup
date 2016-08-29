----------------------------------------------------------------------------------
--[[
FILE:			PVPActivityPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:
AUTHOR:			zhenhui
CREATED:		2014-09-22
--]]
----------------------------------------------------------------------------------

local option = {
ccbiFile = "ManyPeopleArenaPage.ccbi",
handlerMap = {
	onMainRegimentWar	= "onMainRegimentWar",
	onCampWar 		= "onCampWar",
	onHelp 				= "onHelp"
}
};
local UserInfo = require("UserInfo");
local thisPageName = "PVPActivityPage";
local CommonPage = require("CommonPage");
local PVPActivityPage = CommonPage.new("PVPActivityPage", option);
local decisionTitle = "";
local decisionMsg = "";
local decisionCB = nil;

local NodeHelper = require("NodeHelper");
----------------------------------------------------------------------------------
--PVPActivityPage页面中的事件处理
----------------------------------------------
function PVPActivityPage.onEnter(container)
	--刷新红点
end

function PVPActivityPage.onExecute(container)
	PVPActivityPage.refreshRegimentRedPoint(container)
	PVPActivityPage.refreshCampWarRedPoint(container)
end

function PVPActivityPage.isShowPVPActivityRedPoint()
	local CampWarManager = require("CampWarManager")
	if CampWarManager.isShowCampRedPoint() == true or NoticePointState.REGINMENTWAR_POINT == true  then
		return true
	else
		return false
	end
end

function PVPActivityPage.refreshCampWarRedPoint(container)
	local CampWarManager = require("CampWarManager")
	local flag = CampWarManager.isShowCampRedPoint()
	NodeHelper:setNodesVisible(container, 
	{
		mCampWarPoint = flag
	});
	
end

function PVPActivityPage.refreshRegimentRedPoint(container)
	NodeHelper:setNodesVisible(container, 
	{
		mRegimentWarPoint = NoticePointState.REGINMENTWAR_POINT
	});
end


function PVPActivityPage.onExit(container)
	
end

function PVPActivityPage.onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_PvpActivity)
end

function PVPActivityPage.onMainRegimentWar(container)
	if UserInfo.roleInfo.fight < GameConfig.TeamBattleLimit then
		MessageBoxPage:Msg_Box_Lan("@TeamBattleLimit")
		return
	end
	PageManager.changePage("RegimentWarPage");
end

function PVPActivityPage.onCampWar(container)
	local CampWarManager = require("CampWarManager")
	CampWarManager.EnterPageByState()
end

return PVPActivityPage;
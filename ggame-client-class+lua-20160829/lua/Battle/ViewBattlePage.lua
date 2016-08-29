----------------------------------------------------------------------------------
--[[
FILE:			ViewBattlePage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	��Ӣ����
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
FightRequired = 1;			--����ս��
BeforeFightStart = 2;		--ս��׼��������׼����ս����ʼ��������
Fighting = 3;				--ս������
FightEnd = 4;				--ս������
Waiting =  5;				--�ȴ�״̬
}

local ViewBattlePageBase = {}

----------------------------------------------------------------------------------

-----------------------------------------------
--ViewBattlePageBaseҳ���е��¼�����
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
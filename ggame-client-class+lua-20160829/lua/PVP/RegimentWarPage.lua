----------------------------------------------------------------------------------
--[[
	FILE:			RegimentWarPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	团战主页面
	AUTHOR:			zhaozhen
	CREATED:		2014-08-10
--]]
----------------------------------------------------------------------------------
--require "ExploreEnergyCore_pb"

local thisPageName = "RegimentWarPage";
local HP = require("HP_pb");
local TeamBattle_pb = require("TeamBattle_pb");
registerScriptPage('RegimentWarControlPage')
local UserInfo = require("UserInfo");

local opcodes = {
	JOIN_TEAM_C = HP.JOIN_TEAM_C,
	UNDO_SIGN_UP_C = HP.UNDO_SIGN_UP_C,
	CREATE_TEAM_C = HP.CREATE_TEAM_C
};

local globalOpcodes = {
	TEAM_BATTLE_INFO_C = HP.TEAM_BATTLE_INFO_C,
	TEAM_BATTLE_SYNC_S = HP.TEAM_BATTLE_SYNC_S
}

--[[
local OPCODE_GET_USER_BASIC_INFORET_S = 4
local OPCODE_USER_ENERGYCORE_INFO_RET_S = 462
local OPCODE_REMOVE_USER_ENERGYCORE_RET_S = 464

USER_TOOLS = 61001;
ENERGYCORE_TYPE = 81;
--]]

local option = {
	ccbiFile = "RegimentWarPage.ccbi",
	handlerMap = {
		onJoinRanks			= "joinTeam",
		onControlTeam = "controlTeam",
		onCancleTeam = "cancleTeam",
		onCreateTeam = "createTeam",
		onHelp = "onHelp"
	},
	opcode = opcodes
};

local RegimentWarPageBase = {};

local NodeHelper = require("NodeHelper");

--团战状态，-1准备状态前，结束状态后。0准备状态，大于1第n轮战斗进行中
local BATTLE_STATE = 
{
	STATE_NORMAL = -1,
	STATE_READY = 0
}
--页面上半部分UI,战斗状态
local TEAM_STATE = 
{
	STATE_ENTER = 2,
	STATE_NO_ENTER = 1
}

--是不是队长
local PLAYER_STATE =
{
	STATE_CAPTAIN = 2,
	STATE_NORMAL = 1
}

PageInfo = 
{		BattleId = 0,
		BattleState = BATTLE_STATE.STATE_NORMAL,
		CDTimeKey = 'RegimentWarCD',
		BattleLeftTime = 0,
		isEnterBattle = TEAM_STATE.STATE_NO_ENTER,
		isCaptain = PLAYER_STATE.STATE_NORMAL
}

local TimerFunc = {};

-----------------------------------------------

--------------注销账号后，清空页面信息--------------------
function RESETINFO_TEAMBATTLE()
	--PageInfo = {};
	PageInfo = 
	{	BattleId = 0,
		BattleState = BATTLE_STATE.STATE_NORMAL,
		CDTimeKey = 'RegimentWarCD',
		BattleLeftTime = 0,
		isEnterBattle = TEAM_STATE.STATE_NO_ENTER,
		isCaptain = PLAYER_STATE.STATE_NORMAL
	}
end

function RegimentWarPageBase:onEnter(container)
	self:serAllUINotVisible( container )
	self:registerPacket(container);
	self:registerGlobalPacket( container );
	self:sendMsgForBattleInfo( container );
	--container:registerMessage(MSG_SEVERINFO_UPDATE)
	--self:refreshPage(container);
	self:refreshPage(container);
end

function RegimentWarPageBase:onExecute(container)
	if PageInfo.BattleState == BATTLE_STATE.STATE_READY then
		TimerFunc.updateCD(container)
	end
end

--准备状态倒计时
function TimerFunc.updateCD( container )

	local cdString = '00:00:00'
	if TimeCalculator:getInstance():hasKey(PageInfo.CDTimeKey) then
		local timeleft = TimeCalculator:getInstance():getTimeLeft(PageInfo.CDTimeKey)
		if timeleft > 0 then
			 cdString = GameMaths:formatSecondsToTime(timeleft)
		end
	end
	NodeHelper:setStringForLabel(container, { mCD = cdString})
end

function RegimentWarPageBase:onExit(container)
	self:removePacket(container)
	--container:removeMessage(MSG_SEVERINFO_UPDATE)	
end
----------------------------------------------------------------

function RegimentWarPageBase:refreshPage(container)
	self:showWarInfo(container);
	self:showTeamInfo(container);
end

--战斗状态
function RegimentWarPageBase:showWarInfo(container)
	local lb2Str = {	
		mRegimentState = common:getLanguageString("@BattleING"),
		mExplainTex = common:getLanguageString("@BattleEnd")
	};

	local warInfoVisible = 
	{
		mExplainTex = false,
		mCD = false
	}

	if PageInfo.BattleState == BATTLE_STATE.STATE_NORMAL then
		mExplainTex = common:getLanguageString("BattleEnd");
		warInfoVisible.mExplainTex = true;
		warInfoVisible.mCD = false;
	elseif PageInfo.BattleState == BATTLE_STATE.STATE_READY then
		if PageInfo.isEnterBattle == TEAM_STATE.STATE_ENTER then
			lb2Str.mRegimentState = common:getLanguageString("@BattleNextJoin");
		else 
			lb2Str.mRegimentState = common:getLanguageString("@BattleNextJoinNo");
		end
		warInfoVisible.mExplainTex = false;
		warInfoVisible.mCD = true;

		if PageInfo.BattleLeftTime > 0 then
			TimeCalculator:getInstance():createTimeCalcultor(PageInfo.CDTimeKey, PageInfo.BattleLeftTime)
		end
	elseif PageInfo.BattleState > 0 then
		lb2Str.mExplainTex = common:getLanguageString("@BattleINGRound", tonumber(PageInfo.BattleState));
		warInfoVisible.mExplainTex = true;
		warInfoVisible.mCD = false;
	end

	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setNodesVisible(container, warInfoVisible);
end

--人物状态
function RegimentWarPageBase:showTeamInfo(container)

	NodeHelper:setStringForLabel(container, { mCD = ""});
	local teamInfoVisible = 
	{
		mCreateTeamNode = false,
		mCancleTeamNode = false,
		mControlTeamNode = false,
		mJoinRanksNode = false
	}

	if PageInfo.BattleState == BATTLE_STATE.STATE_READY then
		if PageInfo.isEnterBattle == TEAM_STATE.STATE_ENTER then
			if PageInfo.isCaptain == PLAYER_STATE.STATE_CAPTAIN then
				teamInfoVisible.mCancleTeamNode = false;
				teamInfoVisible.mControlTeamNode = true;
			else
				teamInfoVisible.mCancleTeamNode = true;
				teamInfoVisible.mControlTeamNode = false;
			end
			teamInfoVisible.mCreateTeamNode = false;
			teamInfoVisible.mJoinRanksNode = false;
		else 
			teamInfoVisible.mCreateTeamNode = true;
			teamInfoVisible.mJoinRanksNode = true;
			teamInfoVisible.mCancleTeamNode = false;
			teamInfoVisible.mControlTeamNode = false;
		end
	end

	NodeHelper:setNodesVisible(container, teamInfoVisible);

end

--UI重置为不显示
function RegimentWarPageBase:serAllUINotVisible( container )
	local teamInfoVisible = 
	{
		mCreateTeamNode = false,--创建队伍node
		mCancleTeamNode = false,--取消创建队伍node
		mControlTeamNode = false,--管理队伍node
		mJoinRanksNode = false--
	}

	NodeHelper:setNodesVisible(container, teamInfoVisible);

	local warInfoVisible = 
	{
		mExplainTex = false,
		mCD = false
	}

	NodeHelper:setNodesVisible(container, warInfoVisible);
end

----------------click event------------------------
function RegimentWarPageBase.notifyMainPageNews()
	-- 通知主页面去掉红点
	local message = MsgMainFrameGetNewInfo:new()
	message.type = GameConfig.NewPointType.TYPE_RegimentWar_NEW_CLOSE
	MessageManager:getInstance():sendMessageForScript(message)
end

function RegimentWarPageBase:joinTeam(container)

	self:sendMsgForJoinTeam( container );
	self:notifyMainPageNews()
end

function RegimentWarPageBase:controlTeam(container)

	PageManager.changePage('RegimentWarControlPage')
end

function RegimentWarPageBase:cancleTeam(container)

	self:sendMsgForOutTeam( container );
end

function RegimentWarPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_REGIMENTWAR)
end

function RegimentWarPageBase:createTeam(container)
	local createTeamFunc = function ( isOk )
		if isOk and UserInfo.isGoldEnough(GameConfig.Cost.CreateRegimentTeam) then
			self:sendMsgForCreateTeam(container);
			self:notifyMainPageNews()
		end
	end

	local title = common:getLanguageString("@CreateTeam");
	local msg = common:getLanguageString("@CreateTeamContent", GameConfig.Cost.CreateRegimentTeam);

	PageManager.showConfirm(title, msg, createTeamFunc);

end


function RegimentWarPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == globalOpcodes.TEAM_BATTLE_SYNC_S then
		local msg = TeamBattle_pb.HPTeamBattleSyncS()
		msg:ParseFromString(msgBuff)
		self:onReceiveBattleInfo(container, msg)
		return
	end


end

function RegimentWarPageBase:onReceiveBattleInfo( container, msg )

	PageInfo = {	
		BattleId = msg.stageId,
		BattleState = msg.battleState,
		CDTimeKey = 'RegimentWarCD',
		BattleLeftTime = msg.period,
		isEnterBattle = msg.isJoin,
		isCaptain = msg.isCaptain
	}
	if msg.battleState ~= 0 then
		self:notifyMainPageNews()
	end
	self:refreshPage(container);
end



function RegimentWarPageBase:sendMsgForBattleInfo( container )
	local msg = TeamBattle_pb.HPTeamBattleInfo()
	local pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(globalOpcodes.TEAM_BATTLE_INFO_C, pb_data, #pb_data, true)
end

function RegimentWarPageBase:sendMsgForJoinTeam( container )
	local msg = TeamBattle_pb.HPJoinTeam();
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(opcodes.JOIN_TEAM_C, pb_data, #pb_data, false);
end

function RegimentWarPageBase:sendMsgForOutTeam( container )
	local msg = TeamBattle_pb.HPUndoSignUp();
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(opcodes.UNDO_SIGN_UP_C, pb_data, #pb_data, false);
end

function RegimentWarPageBase:sendMsgForCreateTeam( container )
	local msg = TeamBattle_pb.HPCreateTeam();
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(opcodes.CREATE_TEAM_C, pb_data, #pb_data, false);
end



function RegimentWarPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function RegimentWarPageBase:registerGlobalPacket( container )
	for key, opcode in pairs(globalOpcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function RegimentWarPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local RegimentWarPage = CommonPage.newSub(RegimentWarPageBase, thisPageName, option);
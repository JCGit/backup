----------------------------------------------------------------------------------
--[[
	FILE:			RegimentWarControlPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	团战队长管理队伍页面
	AUTHOR:			zhaozhen
	CREATED:		2014-08-04
--]]
----------------------------------------------------------------------------------
--require "ExploreEnergyCore_pb"

local thisPageName = "RegimentWarControlPage"
local UserInfo = require("UserInfo");
local HP = require("HP_pb");
local NodeHelper = require("NodeHelper");
local ResManagerForLua = require("ResManagerForLua");
local Const = require('Const_pb');
local TeamBattle_pb = require("TeamBattle_pb");

------------local variable for system global api--------------------------------------
local tostring = tostring;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
local MAX_KICK_COUNT = 10;

local RegimentWarControlBase = {};

local roleConfig = ConfigManager.getRoleCfg();
local skillCfg = ConfigManager:getSkillCfg()

local RegimentWarControlItem = 
{
	ccbiFile = "RegimentWarTeamContent.ccbi",
};

local opcodes = 
{
	REFRESH_MEMBER_C = HP.REFRESH_MEMBER_C,
	KICK_MEMBER_C = HP.KICK_MEMBER_C
}

local globalOpcodes = {
	MANAGE_MEMBER_C = HP.MANAGE_MEMBER_C,
	TEAM_MEMBER_SYNC_S = HP.TEAM_MEMBER_SYNC_S
}


local option = {
	ccbiFile = "RegimentWarTeamPage.ccbi",
	handlerMap = {
		onJoinRanks = "sendMsgForRefreshList",
		onHelp = "onHelp"
	},
	opcode = opcodes
};

local TeamList = {};

local MainContainer = nil;

local PLAYER_STATE =
{
	STATE_CAPTAIN = 2,
	STATE_NORMAL = 1
}

local TeamBattleKickCfg = ConfigManager.getTeamBattleKickCfg();

--------------------------------------------------------------


function RegimentWarControlItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		RegimentWarControlItem.onRefreshItemView(container);
	elseif eventName == "onKick" then
		RegimentWarControlItem.kickPlayer(container)
	elseif eventName == "onHead" then
		RegimentWarControlItem.viewPlayer(container)
	end
end

--查看指定队员装备信息
function RegimentWarControlItem.viewPlayer( container )
	local contentId = container:getItemDate().mID;
	local memberInfo = TeamList[contentId];

	if memberInfo ~= nil then
		PageManager.viewPlayerInfo(itemInfo.playerId, true);
	end
end

--刷新content
function RegimentWarControlItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;
	local memberInfo = TeamList[contentId];

	if memberInfo ~= nil then

		-- label string
		local fightValueStr = common:getLanguageString("@TeamFightValue",memberInfo.fightValue);

		local teamStr = 
		{
			mFightingCapacity = fightValueStr,
			mName = memberInfo.playerName
		}

		NodeHelper:setStringForLabel(container, teamStr);

		-- visible
		local leaderNode = {};
		if memberInfo.isCaptain == PLAYER_STATE.STATE_CAPTAIN then
			leaderNode = {
				mLeaderNode = true,
				mBtnNode = false
			}
		else
			leaderNode = {
				mLeaderNode = false,
				mBtnNode = true
			}
		end

		NodeHelper:setNodesVisible(container, leaderNode);

		-- image player
		local headPic = roleConfig[memberInfo.roleCfgId].icon;
		NodeHelper:setSpriteImage(container, {mHeroPic = headPic}, {mHeroPic = 1.0});

		-- image skill
		local maxSize = table.maxn(memberInfo.skillCfgId);
		if maxSize > 4 then maxSize = 4 end

		local skillVisible = 
		{
			mSkill1 = false,
			mSkill2 = false,
			mSkill3 = false,
			mSkill4 = false
		}

		NodeHelper:setNodesVisible(container, skillVisible);

		for i = 1, maxSize, 1 do
			local skillId = memberInfo.skillCfgId[i];
			if skillCfg[skillId] ~= nil then
				local skillPic = skillCfg[skillId]["icon"];
				local skillIndex = "mSkill"..i;
				NodeHelper:setNodesVisible(container, {[skillIndex] = true});
				NodeHelper:setSpriteImage(container, {[skillIndex] = skillPic});
			end
		end
	end
end

function RegimentWarControlItem.sendMsgForKickPlayer( container, playerId )
	local msg = TeamBattle_pb.HPKickMember();
	msg.kickPlayerId = playerId;
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(opcodes.KICK_MEMBER_C, pb_data, #pb_data, false);
end

--踢人
function RegimentWarControlItem.kickPlayer( container )
	local contentId = container:getItemDate().mID;
	local memberInfo = TeamList[contentId];

	if memberInfo ~= nil then
		local kickObj;
		if TeamList.kickCount <= MAX_KICK_COUNT then
			kickObj = TeamBattleKickCfg[TeamList.kickCount];
		else
			kickObj = TeamBattleKickCfg[MAX_KICK_COUNT];
		end

		local goldCost = kickObj.goldCost;
		if kickObj ~= nil then
			local kickPlayerFunc = function ( isOk )
				if isOk and UserInfo.isGoldEnough(goldCost) then
					RegimentWarControlItem.sendMsgForKickPlayer( MainC0ontainer, memberInfo.playerId );
				end
			end
			
			local title = common:getLanguageString("@Kick");
			local msg = common:getLanguageString("@KickPlayer",goldCost);

			PageManager.showConfirm(title, msg, kickPlayerFunc);
		end
	end
end



----------------------------------------------

function RegimentWarControlBase:onEnter(container)
	NodeHelper:initScrollView(container, "mContent", 3);
	if container.mScrollView ~= nil then
		container:autoAdjustResizeScrollview(container.mScrollView);
	end
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	
	self:registerPacket(container)
	self:registerGlobalPacket( container );

	self:sendMsgForTeamList( container);
	--container:registerMessage(MSG_SEVERINFO_UPDATE)
	MainContainer = container;

	--self:refreshPage(container);
	--self:rebuildAllItem(container);
end


function RegimentWarControlBase:refreshPage( container )
	local teamName = common:getLanguageString("@TeamName");
	local title = common:getLanguageString("@TeamEnd");

	local fightValue = self:getSumFightValue(container);
	local playerCount = self:getPlayerCount( container );
	local fightValueStr = common:getLanguageString("@TeamBattleCount", fightValue,playerCount);

	local leaderTitle = common:getLanguageString("@TeamLeaderTitle");
	local leaderContent = common:getLanguageString("@TeamLeaderContent");

	local teamStr = 
	{
		mTeamName = teamName,
		mExplain = title,
		mTotalFighting = fightValueStr
	}

	NodeHelper:setStringForLabel(container, teamStr);

end

--获得队伍总战力
function RegimentWarControlBase:getSumFightValue( container )
	local sum = 0;

	local maxSize = table.maxn(TeamList);

	for i=1,maxSize,1 do
		local memberInfo = TeamList[i];

		sum = sum + memberInfo.fightValue;
	end

	return sum;
end

function RegimentWarControlBase:getPlayerCount( container )
	return table.maxn(TeamList);
end

function RegimentWarControlBase:onExecute(container)
end

function RegimentWarControlBase:onExit(container)
	self:removePacket(container)
	--container:removeMessage(MSG_SEVERINFO_UPDATE)	
	NodeHelper:deleteScrollView(container);
end
---------------------------------------------------------------

----------------scrollview-------------------------
function RegimentWarControlBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function RegimentWarControlBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function RegimentWarControlBase:buildItem(container)

	local contentsSize = table.maxn(TeamList);
	NodeHelper:buildScrollView(container, contentsSize, RegimentWarControlItem.ccbiFile, RegimentWarControlItem.onFunction);
end
-------------------------------------------------------------------------
function RegimentWarControlBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function RegimentWarControlBase:registerGlobalPacket( container )
	for key, opcode in pairs(globalOpcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function RegimentWarControlBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode);
		end
	end
end

function RegimentWarControlBase:sendMsgForTeamList( container)

	local msg = TeamBattle_pb.HPManageMember();
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(globalOpcodes.MANAGE_MEMBER_C, pb_data, #pb_data, true);
end

function RegimentWarControlBase:sendMsgForRefreshList( container )
	local msg = TeamBattle_pb.HPRefreshMember();
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(opcodes.REFRESH_MEMBER_C, pb_data, #pb_data, false);
end	

function RegimentWarControlBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == globalOpcodes.TEAM_MEMBER_SYNC_S then
		local msg = TeamBattle_pb.HPTeamMemberSyncS()
		msg:ParseFromString(msgBuff)
		self:onReceiveTeamList(container, msg)
		return
	end
end

--得到队伍所有队员list
function RegimentWarControlBase:onReceiveTeamList( container, msg )
	TeamList = {};

	local memberSize = table.maxn(msg.memberInfo);

	for i = 1,memberSize,1 do
		local memberInfo = msg.memberInfo[i];
		TeamList[i] = {};
		TeamList[i].roleCfgId = memberInfo.roleCfgId;
		TeamList[i].playerId = memberInfo.playerId;
		TeamList[i].playerName = memberInfo.playerName;
		TeamList[i].fightValue = memberInfo.fightValue;

		local skillSize = table.maxn(memberInfo.skillCfgId)

		TeamList[i].skillCfgId = {};
		for j = 1, skillSize,1 do

			TeamList[i].skillCfgId[j] = memberInfo.skillCfgId[j];
		end

		TeamList[i].isCaptain = memberInfo.isCaptain;

	end
	
	table.sort(TeamList,
	function (e1, e2)
		if not e2 then return true end
		if not e1 then return false end
		if e1.isCaptain == 2 then
			return true
		elseif e2.isCaptain == 2 then	
			return false
		else
			return e1.fightValue < e2.fightValue
		end
		
	end
	)
	
	TeamList.kickCount = msg.kickCount;

	self:refreshPage(container);
	self:rebuildAllItem(container);
end

function RegimentWarControlBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_REGIMENTWAR_TEAM);
end

-------------------------------------------------------------------------


local CommonPage = require("CommonPage");
local RegimentWarControlPage = CommonPage.newSub(RegimentWarControlBase, thisPageName, option);


--[[
function RegimentWarControlBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_SEVERINFO_UPDATE then
		local opcode = MsgSeverInfoUpdate:getTrueType(message).opcode
		if opcode == OPCODE_GET_USER_BASIC_INFORET_S then
			self:refreshTitle(container)
		elseif opcode == OPCODE_REMOVE_USER_ENERGYCORE_RET_S or opcode == OPCODE_USER_ENERGYCORE_INFO_RET_S then
			pageInfo.needRefresh = true
		end
	elseif typeId == MSG_MAINFRAME_POPPAGE then
		local pageName = MsgMainFramePopPage:getTrueType(message).pageName
		if pageName == "EnergyCoreUpgradePage" and pageInfo.needRefresh then
			self:rebuildAllItem(container)
		end
	elseif typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			local pageType = common:getBlackBoardVariable("ToMarketPageType", true)
			self:switchPageType(container, tonumber(pageType))
		end
	end
end
--]]

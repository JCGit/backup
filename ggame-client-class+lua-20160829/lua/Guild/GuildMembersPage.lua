----------------------------------------------------------------------------------
--[[
	FILE:			GuildMembersPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	公会成员列表
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------
registerScriptPage('GuildMemberPopupPage')

local UserInfo = require("UserInfo");
local alliance = require('Alliance_pb')
local hp = require('HP_pb')
local NodeHelper = require("NodeHelper")
local GuildData = require("GuildData")
local GuildDataManager = require("GuildDataManager")
local thisPageName = 'GuildMembersPage'
local GuildMemberBase = {}

local option = {
	ccbiFile = "GuildMembersPopUp.ccbi",
	handlerMap = {
		onClose 		= 'onClose',
		onQuitGuild 	= 'onQuitAlliance',
		onContendArray 	= 'onContendArray',
		onConfirmation  = 'onManage'
	}
}

-- personal alliance info
local myInfo = {}

local PositionType = {
	Leader = 2,
	ViceLeader = 1,
	Normal = 0,
}

function GuildMemberBase:onEnter(container)
	container:registerMessage(MSG_MAINFRAME_REFRESH)	
	myInfo = GuildData.MyAllianceInfo.myInfo

	self:registerPackets(container)
	NodeHelper:initScrollView(container, 'mContent', 10)
	self:refreshPage(container)
	GuildDataManager:getMemberList(container)
end

function GuildMemberBase:onExit(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH);
	self:removePackets(container)
	NodeHelper:deleteScrollView(container)
end

function GuildMemberBase:refreshPage(container)
	-- titles
	local lb2Str = {
		mLevelNum 			= ' 0',
		mGuildNumbar 		= '0/0'
	}
	local info = GuildData.allianceInfo.commonInfo
	if info then
		lb2Str.mLevelNum  		= ' ' .. tostring(info.level)
		lb2Str.mGuildNumbar 	= tostring(info.currentPop) .. '/' .. tostring(info.maxPop)
	end
	NodeHelper:setStringForLabel(container, lb2Str)
	
	-- scrollview
	if GuildData.memberList then
		self:rebuildAllItem(container)
	end
end

function GuildMemberBase:onQuitAlliance(container)
	-- the leader
	if GuildData.MyAllianceInfo.myInfo 
		and GuildData.MyAllianceInfo.myInfo.postion == PositionType.Leader 
		and GuildData.allianceInfo.commonInfo
		and GuildData.allianceInfo.commonInfo.currentPop > 1 then
		-- if there is other people in alliance
		MessageBoxPage:Msg_Box('@GuildYouAreNotAlone')
		return 
	end

	PageManager.showConfirm(common:getLanguageString('@GuildQuitAlliance'),
							common:getLanguageString('@GuildQuitConfirm'),
							function (confirm)
								if confirm then
									GuildMemberBase:doQuitAlliance(container)
								end
							end
							)
end

function GuildMemberBase:doQuitAlliance(container)
	if GuildData.MyAllianceInfo.myInfo == nil then 
		MessageBoxPage:Msg_Box('@GuildQuitNoThisPerson')
		self:onClose(container)
		return 
	end
	local msg = alliance.HPAllianceOperC()
	msg.operType = 4 -- see GuildMemberPopupPage OperType.QuitAlliance
	msg.targetId = 0
	local pb = msg:SerializeToString()
	container:sendPakcet(hp.ALLIANCE_OPER_C, pb, #pb, false)
	self:requestBasicInfo(container)
	self:onClose(container)
end

-- 请求公会基本信息和个人公会信息
function GuildMemberBase:requestBasicInfo(container)
	local msg = alliance.HPAllianceEnterC()
	local pb = msg:SerializeToString()
	container:sendPakcet(hp.ALLIANCE_ENTER_C, pb, #pb, false)
end

function GuildMemberBase:onContendArray(container)
	if myInfo.postion then
		if myInfo.postion ~= PositionType.Leader then
			MessageBoxPage:Msg_Box('@GuildOnlyLeaderCanDo')
			return 
		else
			MessageBoxPage:Msg_Box('@CommingSoon')
		end
	else
		MessageBoxPage:Msg_Box('@GuildDataError')
	end
end

function GuildMemberBase:onManage(container)
	if myInfo.postion then
		if myInfo.postion ~= PositionType.Leader then
			MessageBoxPage:Msg_Box('@GuildOnlyLeaderCanDo')
			return 
		else
			self:onClose(container)
			PageManager.pushPage('GuildManagePage')
		end
	else
		MessageBoxPage:Msg_Box('@GuildDataError')
	end
end

function GuildMemberBase:onClose(container)
	PageManager.popPage(thisPageName)
end
----------------scrollview item-------------------------
local MemberItem = {
	ccbiFile = 'GuildMemberContent.ccbi'
}

function MemberItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		MemberItem.onRefreshItemView(container)
	elseif eventName == "onGuildMember" then
		MemberItem.showMemberInfo(container)
	end
end

function MemberItem.onRefreshItemView(container)
	local lb2Str  = {
		mLevelNum 		= common:getLanguageString('@LevelDesc', 0),
		mFightingNum 	= 0,
		mGuildName 		= '',
		mPositionName 	= '',
	}
	local lb2StrTtf = {
		mName = '',
	}
	local index = container:getItemDate().mID
	if GuildData.memberList then 
		local info = GuildData.memberList[index]
		if info then
			lb2Str.mLevelNum 		= common:getLanguageString('@LevelDesc', info.level)
			lb2Str.mFightingNum 	= info.battlePoint
			lb2Str.mGuildName 		= info.contribution
			lb2Str.mPositionName 	= common:getLanguageString('@GuildPosition' .. tostring(info.postion))
			lb2StrTtf.mName 		= info.name
		end
	end
	NodeHelper:setStringForTTFLabel(container, lb2StrTtf)
	NodeHelper:setStringForLabel(container, lb2Str)
end	

function MemberItem.showMemberInfo(container)
	local index = container:getItemDate().mID
	if GuildData.memberList then 
		local info = GuildData.memberList[index]
		if info then
			GuildMemberPop.setMemberInfo(info)
			local myPosition = GuildData.MyAllianceInfo.myInfo.postion
			-- 如果自己是团长则弹出以前的弹框，如果是副团长或普通成员则弹出另一个人的详细信息
			if PositionType.Leader == myPosition then
				-- 如果是自己则弹出详细信息窗口
				if UserInfo.playerInfo.playerId == info.id then
					PageManager.viewPlayerInfo(info.id, true)
				else
					PageManager.pushPage('GuildMemberPopupPage')
				end
			else
				PageManager.viewPlayerInfo(info.id, true)
			end
		else
			MessageBoxPage:Msg_Box('@GuildDataError')
		end
	end
end

----------------scrollview-------------------------
function GuildMemberBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function GuildMemberBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function GuildMemberBase:buildItem(container)
	NodeHelper:buildScrollView(container, #GuildData.memberList, MemberItem.ccbiFile, MemberItem.onFunction);
end

---------------------------- packet function ------------------------------------
function GuildMemberBase:registerPackets(container)
	container:registerPacket(hp.ALLIANCE_MEMBER_S)
end

function GuildMemberBase:removePackets(container)
	container:removePacket(hp.ALLIANCE_MEMBER_S)
end

function GuildMemberBase:onReceiveMembers(container, msg)
	GuildDataManager:setGuildMemberList(msg.memberList)
end

function GuildMemberBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == hp.ALLIANCE_MEMBER_S then
		local msg = alliance.HPAllianceMemberS()
		msg:ParseFromString(msgBuff)
		self:onReceiveMembers(container, msg)
		self:refreshPage(container)
		return
	end
end

function GuildMemberBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			-- request new member info
			GuildDataManager:getMemberList(container)
		end
	end
end

local CommonPage = require('CommonPage')
local GuildMembersPage = CommonPage.newSub(GuildMemberBase, thisPageName, option)

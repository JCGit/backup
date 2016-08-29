----------------------------------------------------------------------------------
--[[
	FILE:			GuildManagePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------
registerScriptPage('GuildSetAnnouncementPage')
registerScriptPage('GuildSetRestrictionPage')

local alliance = require('Alliance_pb')
local hp = require('HP_pb')
local NodeHelper = require("NodeHelper")
local GuildData = require("GuildData")
local thisPageName = 'GuildManagePage'
local GuildManageBase = {}

local option = {
	ccbiFile = "GuildAdministrationPopUp.ccbi",
	handlerMap = {
		onClose 		= 'onClose',
	}
}

-- 会长、副会长、普通成员
local PositionType = {
	Leader = 2,
	ViceLeader = 1,
	Normal = 0,
}

local manageItems = {
	{
		-- 设置战力限制
		itemType = 1,
	},

	{
		-- 设置公告
		itemType = 2,
	},
}

function GuildManageBase:onEnter(container)
	self:registerPackets(container)
	NodeHelper:initScrollView(container, 'mContent', 1)
	self:refreshPage(container)
end

function GuildManageBase:onExit(container)
	self:removePackets(container)
	NodeHelper:deleteScrollView(container)
end

function GuildManageBase:refreshPage(container)
	-- scrollview
	if manageItems then
		self:rebuildAllItem(container)
	end
end
----------------scrollview item-------------------------
local ManageItem = {
	ccbiFile = 'GuildAdministrationContent.ccbi'
}

function ManageItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ManageItem.onRefreshItemView(container)
	elseif eventName == "onSetUp" then
		ManageItem.setJoinCondition(container)
	elseif eventName == "onEdit" then
		ManageItem.setAnnouncement(container)
	end
end

function ManageItem.onRefreshItemView(container)
	local index = container:getItemDate().mID
	local info = manageItems[index]
	if not info then return end

	if 1 == info.itemType then
		-- set condition
		NodeHelper:setNodeVisible(container:getVarNode('mGuildConditionsNode'), true)
		NodeHelper:setNodeVisible(container:getVarNode('mGuildAnnouncementNode'), false)
		local joinLimit = GuildData.allianceInfo.commonInfo and GuildData.allianceInfo.commonInfo.battleLimit or 0
		NodeHelper:setStringForLabel(container, { mConditionsExplain = common:getLanguageString('@GuildJoinCondition', joinLimit) })
	elseif 2 == info.itemType then
		-- set announcement
		NodeHelper:setNodeVisible(container:getVarNode('mGuildConditionsNode'), false)
		NodeHelper:setNodeVisible(container:getVarNode('mGuildAnnouncementNode'), true)
		local announcement = GuildData.allianceInfo.commonInfo and GuildData.allianceInfo.commonInfo.annoucement or ''
		-- 如果公告太长，取前15个字
		local length = GameMaths:calculateStringCharacters(announcement)
		if length > 13 then
			announcement = GameMaths:getStringSubCharacters(announcement, 1, 20) .. '...'
			announcement = GameMaths:stringAutoReturnForLua(announcement, 13, 0)
		end
		NodeHelper:setStringForLabel(container, { mAnnouncementExplain = announcement })
	else
		-- nothing
	end
end	

function ManageItem.setJoinCondition(container)
	-- not the leader
	if GuildData.MyAllianceInfo.myInfo and GuildData.MyAllianceInfo.myInfo.postion ~= PositionType.Leader then
		MessageBoxPage:Msg_Box('@GuildOnlyLeaderCanDo')
		return 
	end
	
	GuildSetRestrictionCallback = function (newBattlePoint)
		local msg = alliance.HPAllianceJoinSetC()
		msg.battlePoint = newBattlePoint
		local pb = msg:SerializeToString()
		PacketManager:getInstance():sendPakcet(hp.ALLIANCE_JOINSET_C, pb, #pb, false)
	end
	PageManager.pushPage('GuildSetRestrictionPage')
end

function ManageItem.setAnnouncement(container)
	-- not the leader
	if GuildData.MyAllianceInfo.myInfo and GuildData.MyAllianceInfo.myInfo.postion ~= PositionType.Leader then
		MessageBoxPage:Msg_Box('@GuildOnlyLeaderCanDo')
		return 
	end
	
	GuildSetAnnouncementCallback = function (newAnnouncement)
		local msg = alliance.HPAllianceNoticeC()
		msg.notice = newAnnouncement
		local pb = msg:SerializeToString()
		PacketManager:getInstance():sendPakcet(hp.ALLIANCE_NOTICE_C, pb, #pb, false)
	end
	PageManager.pushPage('GuildSetAnnouncementPage')
end
----------------scrollview-------------------------
function GuildManageBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function GuildManageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function GuildManageBase:buildItem(container)
	NodeHelper:buildScrollView(container, #manageItems, ManageItem.ccbiFile, ManageItem.onFunction);
end

---------------------------- packet function ------------------------------------
function GuildManageBase:registerPackets(container)
	container:registerPacket(hp.ALLIANCE_CREATE_S)
end

function GuildManageBase:removePackets(container)
	container:removePacket(hp.ALLIANCE_CREATE_S)
end

function GuildManageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == hp.ALLIANCE_CREATE_S then
		local msg = alliance.HPAllianceInfoS()
		msg:ParseFromString(msgBuff)
		GuildData.allianceInfo.commonInfo = msg
		self:refreshPage(container)
		return
	end
end

function GuildManageBase:onClose(container)
	PageManager.popPage(thisPageName)
end

local CommonPage = require('CommonPage')
local GuildManagePage = CommonPage.newSub(GuildManageBase, thisPageName, option)

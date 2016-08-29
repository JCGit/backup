----------------------------------------------------------------------------------
--[[
	FILE:			GuildRankingPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	公会排行界面
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------
require 'common'

local alliance = require('Alliance_pb')
local hp = require('HP_pb')
local NodeHelper = require("NodeHelper")
local GuildData = require("GuildData")
local GuildDataManager = require("GuildDataManager")

local thisPageName = 'GuildRankingPage'
local GuildRankingBase = {}

local option = {
	ccbiFile = "GuildRankingPopUp.ccbi",
	handlerMap = {
		onCancel 		= 'onClose',
		onClose 		= 'onClose',
	}
}

function GuildRankingBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
end

function GuildRankingBase:onEnter(container)
	self:registerPackets(container)
	NodeHelper:initScrollView(container, 'mContent', 10)
	self:refreshPage(container)
	GuildDataManager:requestRankingList(container)
end

function GuildRankingBase:onExit(container)
	self:removePackets(container)
	NodeHelper:deleteScrollView(container)
end

function GuildRankingBase:refreshPage(container)
	self:rebuildAllItem(container)
end

function GuildRankingBase:onClose(container)
	PageManager.popPage(thisPageName)
end

----------------scrollview item-------------------------
local RankListItem = {
	ccbiFile = 'GuildRankingContent.ccbi'
}

function RankListItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		RankListItem.onRefreshItemView(container)
	end
end

function RankListItem.onRefreshItemView(container)
	local index = container:getItemDate().mID
	local info = GuildData.rankingList[index]
	if not info then return end
	local lb2Str = {
		mRanking 		= index,
		mID 			= info.id,
		mGuildLv 		= common:getLanguageString('@LevelDesc', info.level),
		mGuildName 		= info.name,
		mLeadersName 	= info.handName,
	}
	NodeHelper:setStringForLabel(container, lb2Str)
end	

----------------scrollview-------------------------
function GuildRankingBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function GuildRankingBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function GuildRankingBase:buildItem(container)
	NodeHelper:buildScrollView(container, #GuildData.rankingList, RankListItem.ccbiFile, RankListItem.onFunction);
end

 ------------------ packet function -----------------------------------
function GuildRankingBase:registerPackets(container)
	container:registerPacket(hp.ALLIANCE_RANKING_S)
end

function GuildRankingBase:removePackets(container)
	container:removePacket(hp.ALLIANCE_RANKING_S)
end

function GuildRankingBase:onReceiveRankingList(container, msg)
	if msg.showTag then
		GuildDataManager:setRankInfo(msg.rankings)
	else
		MessageBoxPage:Msg_Box('@GuildNoRankList')
		GuildDataManager:setRankInfo({})
	end
end

function GuildRankingBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == hp.ALLIANCE_RANKING_S then
		-- alliance enter
		local msg = alliance.HPAllianceRankingS()
		msg:ParseFromString(msgBuff)
		self:onReceiveRankingList(container, msg)
		self:refreshPage(container)
		return
	end
end

local CommonPage = require('CommonPage')
local GuildRankingPage = CommonPage.newSub(GuildRankingBase, thisPageName, option)

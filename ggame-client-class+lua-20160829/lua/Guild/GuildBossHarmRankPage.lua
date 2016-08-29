----------------------------------------------------------------------------------
--[[
	FILE:			GuildBossHarmRankPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	公会baoss伤害排行榜
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------
local GuildDataManager = require "GuildDataManager"


local alliance = require('Alliance_pb')
local hp = require('HP_pb')
local NodeHelper = require("NodeHelper")
local GuildData = require("GuildData")

local thisPageName = 'GuildBossHarmRankPage'
local GuildBossHarmBase = {}

local option = {
	ccbiFile = "GuildBossRankingPopUp.ccbi",
	handlerMap = {
		onClose 		= 'onClose',
		onCloseBtn 		= 'onClose',
		onQuitGuild 	= 'onQuitAlliance',
		onContendArray 	= 'onContendArray',
		onConfirmation  = 'onManage'
	}
}

local harmRankList = { }

GuildBossHarmVar = {
	setHarmList = function (harmList)
		harmRankList = harmList
	end
}

function GuildBossHarmBase:onEnter(container)
	self:registerPackets(container)
	NodeHelper:initScrollView(container, 'mContent', 10)
	self:refreshPage(container)
	GuildDataManager:getHarmRank(container)
end

function GuildBossHarmBase:onExit(container)
	self:removePackets(container)
	NodeHelper:deleteScrollView(container)
end

function GuildBossHarmBase:refreshPage(container)
	-- titles
	local lb2Str = {
		mRanking 			= ' :0',
		mDamage 			= ' :0'
	}

	local myRank = self:getMyRank()
	if myRank ~= 0 then
		lb2Str.mRanking  		= ': ' .. tostring(myRank)
	else
		lb2Str.mRanking  		= ': ' .. common:getLanguageString('@GuildHarmListNoMe')
	end

	local myHarm = self:getMyHarm()
	if myHarm ~= 0 then
		lb2Str.mDamage  		= ': ' .. tostring(myHarm)
	end

	NodeHelper:setStringForLabel(container, lb2Str)
	
	-- scrollview
	if harmRankList then
		self:rebuildAllItem(container)
	end
end

function GuildBossHarmBase:onClose(container)
	PageManager.popPage(thisPageName)
end
----------------scrollview item-------------------------
local HarmRankItem = {
	ccbiFile = 'GuildBossRankingContent.ccbi'
}

function HarmRankItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		HarmRankItem.onRefreshItemView(container)
	end
end

function HarmRankItem.onRefreshItemView(container)
	local index = container:getItemDate().mID
	local info = harmRankList[index]
	local lb2Str  = {
		mRanking 		= info and info.sort or 0,
		mID 			= info and info.name or '',
		mLv 			= info and ('LV' .. tostring(info.level)) or 0,
		mDamage 		= info and info.harmValue or 0,
		mPercentage 	= info and (tostring(info.harmPercent) .. '%') or '0%'
	}
	local lb2StrTtf = {
		mID = info and info.name or ''
	}
	NodeHelper:setStringForTTFLabel(container, lb2StrTtf)
	NodeHelper:setStringForLabel(container, lb2Str)
end	

----------------scrollview-------------------------
function GuildBossHarmBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function GuildBossHarmBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function GuildBossHarmBase:buildItem(container)
	NodeHelper:buildScrollView(container, #harmRankList, HarmRankItem.ccbiFile, HarmRankItem.onFunction);
end

---------------------------- packet function ------------------------------------
-- 本人的伤害排行要客户端自己到收到的harmRankList里面找
-- 我的排名
function GuildBossHarmBase:getMyRank(container)
	for i=1, #harmRankList do
		if harmRankList[i].id == GuildData.MyAllianceInfo.myInfo.id then
			return harmRankList[i].sort
		end
	end
	return 0
end

-- 我的伤害值
function GuildBossHarmBase:getMyHarm(container)
	for i=1, #harmRankList do
		if harmRankList[i].id == GuildData.MyAllianceInfo.myInfo.id then
			return harmRankList[i].harmValue
		end
	end
	return 0
end

function GuildBossHarmBase:registerPackets(container)
	container:registerPacket(hp.ALLIANCE_HARMSORT_S)
end

function GuildBossHarmBase:removePackets(container)
	container:removePacket(hp.ALLIANCE_HARMSORT_S)
end

function GuildBossHarmBase:onReceiveHarmRank(container, msg)
	-- showTag 是协议填充字段，用来验证harms是否为空
	if msg.showTag then
		harmRankList = msg.harms
	else
		MessageBoxPage:Msg_Box('@GuildNoHarmList')
		harmRankList = {}
	end
end

function GuildBossHarmBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == hp.ALLIANCE_HARMSORT_S then
		local msg = alliance.HPAllianceHarmSortS()
		msg:ParseFromString(msgBuff)
		self:onReceiveHarmRank(container, msg)
		self:refreshPage(container)
		return
	end
end
----------------------------------------------------------------
local CommonPage = require('CommonPage')
GuildBossHarmRankPage = CommonPage.newSub(GuildBossHarmBase, thisPageName, option)

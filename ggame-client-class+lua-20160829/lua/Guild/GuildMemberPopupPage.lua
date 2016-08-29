----------------------------------------------------------------------------------
--[[
	FILE:			GuildMemberPopupPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	公会成员操作界面
	AUTHOR:			sunyj
	CREATED:		2014-8-4
--]]
----------------------------------------------------------------------------------

local alliance = require('Alliance_pb')
local hp = require('HP_pb')
local NodeHelper = require("NodeHelper")
local GuildData = require("GuildData")
local thisPageName = 'GuildMemberPopupPage'
local GuildMemberPopBase = {}

local memberInfo = {}

GuildMemberPop = {
	setMemberInfo = function (info)
		memberInfo = info
	end
}

local PositionType = {
	Leader = 2,
	ViceLeader = 1,
	Normal = 0,
}

-- 公会操作
local OperType = {
	ChangeLeader = 1, 				-- 转让公会
	ChangeViceLeader = 2, 			-- 提升为副会长
	JoinAlliance = 3, 				-- 加入公会
	QuitAlliance = 4, 				-- 退出公会
	DemoteViceLeader = 5, 			-- 解除副会长
	KickOut = 6, 					-- 提出公会
}

local option = {
	ccbiFile = "GuildMemberOperation.ccbi",
	handlerMap = {
		onNormalClose 			= 'onClose',
		onClose 				= 'onClose',
		onPromoteClose 			= 'onClose',
		onDemoteClose 			= 'onClose',

		-- demote node
		onViewDetail 			= 'onViewDetail',
		onDemote 				= 'onDemote',
		onPromote 				= 'onPromote',
		onDemoteKick			= 'onKickOut',
		onPromoteKick			= 'onKickOut',
	}
}

function GuildMemberPopBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
end

function GuildMemberPopBase:onEnter(container)
	self:refreshPage(container)
end

function GuildMemberPopBase:onExit(container)
end

function GuildMemberPopBase:refreshPage(container)

	if not memberInfo.postion then
		return
	end
	local myPosition = GuildData.MyAllianceInfo.myInfo.postion
	local hisPosition = memberInfo.postion
	-- i am leader
	if PositionType.Leader == myPosition then
		if hisPosition == PositionType.Leader then
			self:showNormalNode(container)
		elseif hisPosition == PositionType.ViceLeader then
			self:showDemoteNode(container)
		elseif hisPosition == PositionType.Normal then
			self:showPromoteNode(container)
		else
			self:showNormalNode(container)
		end
	elseif PositionType.ViceLeader == myPosition then
		-- i am vice leader
		self:showNormalNode(container)
	elseif PositionType.Normal == myPosition then
		-- i am normal
		self:showNormalNode(container)
	end
	local RoleManager = require("RoleManager");
	local icon = RoleManager:getIconById(memberInfo.profession)
	NodeHelper:setSpriteImage(container, {mPic = icon})

	lb2Str = {
		mLv 				= memberInfo.level and memberInfo.level or 0,
		mPosition 			= memberInfo.postion and common:getLanguageString('@GuildPosition' .. tostring(memberInfo.postion)) or '',
		mFightingCapacity   = memberInfo.battlePoint and memberInfo.battlePoint or 0,
		mContribution 		= memberInfo.contribution and memberInfo.contribution or 0,
	}
	NodeHelper:setStringForLabel(container, lb2Str) 

	lb2StrTtf = {
		mName = memberInfo.name and memberInfo.name or ''
	}
	NodeHelper:setStringForTTFLabel(container, lb2StrTtf)
end

function GuildMemberPopBase:showNormalNode(container)
	NodeHelper:setNodeVisible(container:getVarNode('mNormalNode'), true)
	NodeHelper:setNodeVisible(container:getVarNode('mDemoteNode'), false)
	NodeHelper:setNodeVisible(container:getVarNode('onPromoteNode'), false)
end

function GuildMemberPopBase:showPromoteNode(container)
	NodeHelper:setNodeVisible(container:getVarNode('mNormalNode'), false)
	NodeHelper:setNodeVisible(container:getVarNode('mDemoteNode'), false)
	NodeHelper:setNodeVisible(container:getVarNode('onPromoteNode'), true)
end

function GuildMemberPopBase:showDemoteNode(container)
	NodeHelper:setNodeVisible(container:getVarNode('mNormalNode'), false)
	NodeHelper:setNodeVisible(container:getVarNode('mDemoteNode'), true)
	NodeHelper:setNodeVisible(container:getVarNode('onPromoteNode'), false)
end

-- 提升副会长
function GuildMemberPopBase:onPromote(container)
	if GuildData.MyAllianceInfo.myInfo == nil or memberInfo.id == nil then 
		MessageBoxPage:Msg_Box('@GuildDataError')
		return
	end

	local myPosition = GuildData.MyAllianceInfo.myInfo.postion
	if myPosition == PositionType.Leader then
		self:doOperation(container, OperType.ChangeViceLeader, memberInfo.id)
	else
		MessageBoxPage:Msg_Box('@GuildOnlyLeaderCanDo')
	end
end

-- 解除副会长
function GuildMemberPopBase:onDemote(container)
	if GuildData.MyAllianceInfo.myInfo == nil or memberInfo.id == nil then 
		MessageBoxPage:Msg_Box('@GuildDataError')
		return
	end

	local myPosition = GuildData.MyAllianceInfo.myInfo.postion
	if myPosition == PositionType.Leader then
		self:doOperation(container, OperType.DemoteViceLeader, memberInfo.id)
	else
		MessageBoxPage:Msg_Box('@GuildOnlyLeaderCanDo')
	end
end

-- 踢出公会
function GuildMemberPopBase:onKickOut(container)
	if GuildData.MyAllianceInfo.myInfo == nil or memberInfo == nil or memberInfo.id == nil or memberInfo.postion == nil then 
		MessageBoxPage:Msg_Box('@GuildDataError')
		return
	end

	local hisPosition = memberInfo.postion
	local myPosition = GuildData.MyAllianceInfo.myInfo.postion
	if myPosition == PositionType.Leader then
		if PositionType.ViceLeader == hisPosition then
			--self:doGiveOutAlliance(container, OperType.KickOut, memberInfo.id)
			self:doOperation(container, OperType.ChangeLeader, memberInfo.id)
		else
			self:doOperation(container, OperType.KickOut, memberInfo.id)
		end
	else
		MessageBoxPage:Msg_Box('@GuildOnlyLeaderCanDo')
	end
end

-- 查看人物详情
function GuildMemberPopBase:onViewDetail( container )
	PageManager.viewPlayerInfo(memberInfo.id, true)
	PageManager.popPage(thisPageName)
end

function GuildMemberPopBase:doOperation(container, operType, targetId)	
	-- 如果是会长踢人则弹窗确认
	if operType == OperType.KickOut then
		local title = common:getLanguageString('@GuildMemberKickOutTitle')
		local kickNeedCost = VaribleManager:getInstance():getSetting("kickNeedCost")
		local message = common:getLanguageString('@GuildMemberKickOutMessage', tostring(kickNeedCost))
		PageManager.showConfirm(title, message,
		   function (agree)
			   if agree then
			    	GuildMemberPopBase.sendKickOut(container, operType, targetId)
			   end
		   end
		)
	else
		GuildMemberPopBase.sendKickOut(container, operType, targetId)
	end
end

function GuildMemberPopBase.sendKickOut(container, operType, targetId)
	targetId = targetId or 0
	local msg = alliance.HPAllianceOperC()
	msg.operType = operType
	msg.targetId = targetId
	local pb = msg:SerializeToString()
	container:sendPakcet(hp.ALLIANCE_OPER_C, pb, #pb, false)
	PageManager.popPage(thisPageName)
	-- notify refresh members page
	local msg = MsgMainFrameRefreshPage:new()
	msg.pageName = 'GuildMembersPage'
	MessageManager:getInstance():sendMessageForScript(msg)
end

function GuildMemberPopBase:onClose(container)
	PageManager.popPage(thisPageName)
end

local CommonPage = require('CommonPage')
local GuildMemberPopupPage = CommonPage.newSub(GuildMemberPopBase, thisPageName, option)

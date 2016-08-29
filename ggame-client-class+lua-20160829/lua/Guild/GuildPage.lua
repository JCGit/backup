----------------------------------------------------------------------------------
--[[
	FILE:			GuildPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	公会主页面
	AUTHOR:			sunyj
	CREATED:		2014-8-4

	you know: 		Guild == Alliance
--]]
----------------------------------------------------------------------------------

registerScriptPage('GuildSearchPopPage')
registerScriptPage('GuildRankingPage')
registerScriptPage('GuildCreatePage')
registerScriptPage('GuildShopPage')
registerScriptPage('GuildMembersPage')
registerScriptPage('GuildManagePage')
registerScriptPage('GuildBossHarmRankPage')
registerScriptPage('GuildOpenBossConfirmPage')

local alliance = require('Alliance_pb')
local hp = require('HP_pb')
local player = require('Player_pb')
local NodeHelper = require("NodeHelper")

local GuildDataManager = require("GuildDataManager")
local GuildData = require("GuildData")
local UserInfo = require("UserInfo");
local firstEnter = true

local option = {
	-- when i don't have a alliance
	ccbiFile = "GuildPage.ccbi",
	-- when i have a alliance
	ccbiWithAlliance = 'GuildPartakePage.ccbi',
	handlerMap = {
		-- basic event
		luaInit = "onInit",
		luaLoad = "onLoad",
		luaUnLoad = "onUnload",
		luaExecute = "onExecute",
		luaEnter = "onEnter",
		luaExit = "onExit",
		luaOnAnimationDone = "onAnimationDone",
		luaReceivePacket = "onReceivePacket",
		luaGameMessage = "onReceiveMessage",

		-- ccbi: GuildPage.ccbi ：未加入公会，搜索、创建、加入。
		onRefreshList 			= 'refreshJoinList',
		onSearchGuild 			= 'onSearchGuild',
		onEstablishGuild 		= 'onCreateGuild',
		onGuildContend 			= 'onGuildBattle',
		onRanking 				= 'onGuildRanking',
		onHelp 					= 'onHelp',

		-- ccbi: GuildPartakePage.ccbi ：已加入公会，签到、boss、成员、管理。
		onSignIn				= "onSignIn",
		onIntrusion				= "onIntrude",
		onContributionExchange	= "exchangeContribution",
		onMembers				= "showMembers",
		onManage				= "onManage",
		onRank					= "onRank",
		onAutoFight 			= "onAutoFight",
	},
	bossHander = {
		onOpenBossIntrusion				= "openBoss",
		onInspireIntrusion 				= 'onInspire',
		onContributionRankingIntrusion 	= 'onContributionRanking',
		onAttributeOpen 				= "onAttributeOpen",
	}
}

-----------------------------------------------
--GuildPage页面中的事件处理
----------------------------------------------
function GuildData.GuildPage.onFunction(eventName, container)
	local funcName = option.handlerMap[eventName]
	if funcName then
		if not GuildData.GuildPage[funcName] then
			GuildData.GuildPage[funcName] = function(container) end
		end
		GuildData.GuildPage[funcName](container, eventName)
	else
		CCLuaLog('In GuildData.GuildPage: unknown function name: ' .. eventName)
	end
end

function GuildData.GuildPage.onLoad(container)
	-- 主节点
    GuildData.mainContainer = container

	-- '有公会节点'
	GuildData.allianceContainer = ScriptContentBase:create(option.ccbiWithAlliance)
	GuildData.allianceContainer:registerFunctionHandler(GuildData.GuildPage.onFunction)
	-- 'boss节点', 嵌入在'有公会节点'
	local bossNode = GuildData.allianceContainer:getVarNode('mPartakeGuildBossIntrusionItem')
	if bossNode then
		GuildData.bossContainer = ScriptContentBase:create('GuildBossIntrusionItem.ccbi')

		-- boss飘血动画节点, 嵌入在'boss节点'
		GuildData.bossHitContainer = ScriptContentBase:create('BattleNormalNum.ccbi')
		local bossAniNode = GuildData.bossContainer:getVarNode('mPersonHitNumberNode')
		bossAniNode:addChild(GuildData.bossHitContainer)
		GuildData.bossHitContainer:release();

		GuildData.bossContainer:registerFunctionHandler(GuildData.BossPage.onFunction)
		bossNode:addChild(GuildData.bossContainer)
		GuildData.bossContainer:release();
	end
	container:addChild(GuildData.allianceContainer)
	GuildData.allianceContainer:release();

	-- '无公会节点'
	GuildData.joinListContainer = ScriptContentBase:create(option.ccbiFile)
	GuildData.joinListContainer:registerFunctionHandler(GuildData.GuildPage.onFunction)
	
	NodeHelper:initScrollView(GuildData.joinListContainer, 'mContent', 10)

	-- -------------------- 适配 --------------------------------
	if GuildData.joinListContainer.mScrollView then
		GuildData.mainContainer:autoAdjustResizeScrollview(GuildData.joinListContainer.mScrollView)
	end		
	
	local mScale9Sprite1 = GuildData.joinListContainer:getVarScale9Sprite("mScale9Sprite1")
	if mScale9Sprite1 then
		GuildData.mainContainer:autoAdjustResizeScale9Sprite( mScale9Sprite1 )
	end
	
	local mScale9Sprite2 = GuildData.joinListContainer:getVarScale9Sprite("mScale9Sprite2")
	if mScale9Sprite2 then
		GuildData.mainContainer:autoAdjustResizeScale9Sprite( mScale9Sprite2 )
	end
	
	local mScale9Sprite3 = GuildData.joinListContainer:getVarScale9Sprite("mScale9Sprite3")
	if mScale9Sprite3 then
		GuildData.mainContainer:autoAdjustResizeScale9Sprite( mScale9Sprite3 )
	end
	-- -------------------- 适配 --------------------------------

	container:addChild(GuildData.joinListContainer)
	GuildData.joinListContainer:release();

	-- 根据是否有公会来控制显隐
	if GuildData.MyAllianceInfo then
		GuildData.joinListContainer:setVisible(not GuildData.MyAllianceInfo.hasAlliance)
		GuildData.allianceContainer:setVisible(not (not GuildData.MyAllianceInfo.hasAlliance))
	else
		GuildData.joinListContainer:setVisible(true)
		GuildData.allianceContainer:setVisible(false)
	end
end

function GuildData.GuildPage.onEnter(container)
	if GuildData.MyAllianceInfo.hasAlliance then
		GuildData.BossPage.onAttributeOpen(GuildData.bossContainer)
	end
	GuildDataManager:registerPackets()
	GuildDataManager:registerMessages()
	UserInfo.sync() --为了判断vip等级是否过3，自动战斗是否显示
	GuildData.GuildPage.refreshPage()

	-- request basic info
	GuildDataManager:requestBasicInfo()
end

function GuildData.GuildPage.onExecute(container)
	if GuildData.BossPage 
		and GuildData.allianceInfo.commonInfo 
		and (GuildData.allianceInfo.commonInfo.bossState == GuildData.BossPage.BossCanInspire) then
		-- 更新boss击退倒计时
		GuildData.BossPage.updateCD(GuildData.bossContainer)
	end

	-- 进入活动片刻后，预加载商店、成员列表、boss排行、公会排行等数据
	local dt = GamePrecedure:getInstance():getFrameTime() * 1000
	GuildData.enterPageTime = GuildData.enterPageTime + dt

	GuildDataManager:requestData(GuildData.enterPageTime)
end

function GuildData.GuildPage.onExit(container)
    nowRefreshPageNum = 1
	GuildDataManager:notifyMainPageRedPoint()
	GuildDataManager:removePackets()
	GuildDataManager:removeMessages()
	if GuildData.joinListContainer then
		NodeHelper:deleteScrollView(GuildData.joinListContainer)
	end
end
--------------------------------- boss page --------------------------------
function GuildData.BossPage.onFunction(eventName, container)
	local funcName = option.bossHander[eventName]
	if funcName then
		GuildData.BossPage[funcName](container, eventName)
	else
		CCLuaLog('unknown eventName: ' .. tostring(eventName))
	end
end

function GuildData.BossPage.openBoss(container, eventName)
	GuildDataManager:openBoss(container, eventName)
end

function GuildData.BossPage.refreshPage(container)
	if not container then return end

	-- titles
	local lb2Str = {
		mBossIntrusionLevel 	= common:getLanguageString('@BossLevelName', 0, ''),
		mBossIntrusionExpNum = 0
	}

	local info = GuildData.allianceInfo.commonInfo
	if info then
		local cfg = GuildDataManager:getBossCfgByBossId(info.bossId)
		if cfg then
			lb2Str.mBossIntrusionLevel 	= common:getLanguageString('@BossLevelName', cfg.level, cfg.bossName)
			lb2Str.mBossIntrusionExpNum = cfg.bossExp
			lb2Str.mBossVitalityNum 	= info.curBossVitality .. '/' .. info.openBossVitality -- 开启boss需要消耗的元气值
		end
	end
	NodeHelper:setStringForLabel(container, lb2Str)

	-- content
	if not info then
		GuildData.BossPage.showOpenBossView(container)
	elseif info.bossState == GuildData.BossPage.BossNotOpen then
		-- not open
		GuildData.BossPage.showOpenBossView(container)
	elseif info.bossState == GuildData.BossPage.BossCanJoin then
		-- battle
		GuildData.BossPage.showBossJoinView(container)
	elseif info.bossState == GuildData.BossPage.BossCanInspire then
		-- can inspire
		GuildData.BossPage.showBossBattleView(container)
	end
end

local attributeOpenState = false
function GuildData.BossPage.onHideTipHandler()
	if attributeOpenState == true then 
		attributeOpenState = false
		GuildData.bossContainer:getVarMenuItemImage("mAttributeBtn"):unselected()
		GuildData.bossContainer:getVarMenuItemImage("mAttributeBtn"):setEnabled(true)
	end
end
function GuildData.BossPage.onTip()
	GameUtil:showTip(GuildData.bossContainer:getVarNode("mAttributeBtn"),GuildData.vitalityCfg,GuildData.BossPage.onHideTipHandler)
end
function GuildData.BossPage.onAttributeOpen()
	if attributeOpenState == false then 
		GuildData.BossPage.onTip()
		GuildData.bossContainer:getVarMenuItemImage("mAttributeBtn"):selected()

		attributeOpenState = not attributeOpenState
		GuildData.bossContainer:getVarMenuItemImage("mAttributeBtn"):setEnabled(false)
	end
end

-- 显示‘开启boss’界面
function GuildData.BossPage.showOpenBossView(container)
	-- 第一次进入页面显示tip，以后不显示
	if firstEnter == true then
		firstEnter = false
		GuildData.BossPage.onAttributeOpen(GuildData.bossContainer)
	end

	NodeHelper:setNodeVisible(container:getVarNode('mOpenBossNode'), true)
	NodeHelper:setNodeVisible(container:getVarNode('mBossOpenNoticeNode'), true)
	NodeHelper:setNodeVisible(container:getVarNode('mBossIntrusionBattle'), false)
	NodeHelper:setNodeVisible(container:getVarNode('mCDIntrusionNode'), false)
	local leftCount = GuildData.allianceInfo.commonInfo and GuildData.allianceInfo.commonInfo.bossFunRemSize or 0
	NodeHelper:setStringForLabel(container, { mOpenBossIntrusion = common:getLanguageString('@OpenBoss', leftCount)})
end

-- 显示‘加入战斗’界面
function GuildData.BossPage.showBossJoinView(container)
	NodeHelper:setNodeVisible(container:getVarNode('mOpenBossNode'), true)
	NodeHelper:setNodeVisible(container:getVarNode('mBossOpenNoticeNode'), false)
	NodeHelper:setNodeVisible(container:getVarNode('mBossIntrusionBattle'), false)
	NodeHelper:setNodeVisible(container:getVarNode('mCDIntrusionNode'), false)
	NodeHelper:setStringForLabel(container, { mOpenBossIntrusion = common:getLanguageString('@GuildBossJoin')})
end

-- 显示‘战斗’界面
function GuildData.BossPage.showBossBattleView(container)
	NodeHelper:setNodeVisible(container:getVarNode('mOpenBossNode'), false)
	NodeHelper:setNodeVisible(container:getVarNode('mBossOpenNoticeNode'), false)
	NodeHelper:setNodeVisible(container:getVarNode('mBossIntrusionBattle'), true)
	NodeHelper:setNodeVisible(container:getVarNode('mCDIntrusionNode'), true)
	local info = GuildData.allianceInfo.commonInfo
	local lb2Str = { }
	local totalBlood = 0
	if info then
		local cfg = GuildDataManager:getBossCfgByBossId(info.bossId)
		if cfg then
			totalBlood = cfg.bossBlood
		end
		lb2Str.mBossIntrusionHpNum = tostring(GuildData.BossPage.bossBloodLeft) .. '/' .. tostring(totalBlood)
		lb2Str.mInspireIntrusionNum = common:getLanguageString('@GuildBossInspireRatio', info.bossPropAdd) 
	else
		lb2Str.mBossIntrusionHpNum = '0/0'
		lb2Str.mInspireIntrusionNum = common:getLanguageString('@GuildBossInspireRatio', info.bossPropAdd) 
	end
	-- inspire desc
	lb2Str.mEncouragePromptTex = common:getLanguageString('@GuildInspirePreview', GuildData.BossPage.InspirePercent, GuildData.BossPage.InspireCost)

	NodeHelper:setStringForLabel(container, lb2Str)

	-- progress bar
	local scale = 0.0
	if totalBlood ~= 0 then
		scale = GuildData.BossPage.bossBloodLeft / totalBlood * 1.09
		if scale < 0 then scale = 0.0 end
	end

	local expBar = container:getVarScale9Sprite('mIntrusionExp')
	if expBar then
		expBar:setScaleX(scale)
	end
end

function GuildData.BossPage.updateCD(container)
	if not container then return end

	local cdString = '00:00:00'
	if TimeCalculator:getInstance():hasKey(GuildData.BossPage.CDTimeKey) then
		local timeleft = TimeCalculator:getInstance():getTimeLeft(GuildData.BossPage.CDTimeKey)
		if timeleft > 0 then
			 cdString = GameMaths:formatSecondsToTime(timeleft)
		 else
			 -- boss 倒计时结束，判断打没打死
			 TimeCalculator:getInstance():removeTimeCalcultor(GuildData.BossPage.CDTimeKey)
			 GuildDataManager:requestBasicInfo()
		end
	end
	NodeHelper:setStringForLabel(container, { mCD = cdString})
end

-- 鼓舞
function GuildData.BossPage.onInspire(container, eventName)
	if UserInfo.isGoldEnough(GuildData.BossPage.InspireCost) then
		GuildDataManager:doInspire()
	end					
end

function GuildData.BossPage.onContributionRanking(container, eventName)
	PageManager.pushPage('GuildBossHarmRankPage')
end
--------------------------------- end boss page --------------------------------

--------------------------ui function--------------------------------------

-- view when you have an alliance
function GuildData.GuildPage.showAllianceView()
	GuildData.joinListContainer:setVisible(false)
	GuildData.allianceContainer:setVisible(true)

	-- alliance info
	GuildData.GuildPage.showAllianceInfo()

	-- refresh boss
	GuildData.BossPage.refreshPage(GuildData.bossContainer)
end

-- view when you don't have an alliance
function GuildData.GuildPage.showJoinListView()
	GuildData.joinListContainer:setVisible(true)
	GuildData.allianceContainer:setVisible(false)

	if GuildData.joinListContainer.mScrollView 
		and GuildData.joinListContainer.m_pScrollViewFacade 
		and GuildData.allianceInfo.joinList then
		--显示刷新页数
		NodeHelper:setStringForLabel(GuildData.joinListContainer, 
			{mPageNum =common:getLanguageString("@GuildRecommendListPage") .. GuildData.allianceInfo.curPage .. '/' .. GuildData.allianceInfo.maxPage})

		GuildData.GuildPage.rebuildAllItem()
	end
end

function GuildData.GuildPage.refreshPage()
	if GuildData.MyAllianceInfo.hasAlliance then
		-- if i have a alliance
		GuildData.GuildPage.showAllianceView()
	else
		-- i don't belong to any alliance
		GuildData.GuildPage.showJoinListView()
	end
end

function GuildData.GuildPage.showAllianceInfo(container)
	local lb2Str = {
		mPartakeLV					= common:getLanguageString("@GuildLevelName", 0, ''),
		mPartakeGuildID				= common:getLanguageString("@GuildID", 'NO'),
		mNumberPeople				= 'NO / NO',
		mPartakeGuildExp			= 'NO / NO',
		mPartakeGuildAnnouncements	= common:getLanguageString('@GuildAnnoucementDefault'),
		mPartakeNumberPeople 		= 0,
	}

	-- exp bar zoom scale
	local scale = 0.0
	local info = GuildData.allianceInfo.commonInfo
	if info then
		lb2Str.mPartakeLV				= common:getLanguageString("@GuildLevelName", info.level, info.name)
		lb2Str.mPartakeGuildID 			= common:getLanguageString("@GuildID", info.id)
		lb2Str.mPartakeNumberPeople		= info.currentPop .. ' / ' .. info.maxPop
		lb2Str.mPartakeGuildExp			= info.currentExp .. ' / ' .. info.nextExp

		if info.nextExp ~= 0 then
			scale = info.currentExp / info.nextExp
		end

		if info.annoucement and common:trim(info.annoucement) ~= '' then
			-- 如果公告太长，取前20个字
			local length = GameMaths:calculateStringCharacters(info.annoucement)
			if length > 20 then
				lb2Str.mPartakeGuildAnnouncements = GameMaths:getStringSubCharacters(info.annoucement, 1, 20) .. '...'
			else
				lb2Str.mPartakeGuildAnnouncements = info.annoucement
			end
		end
	end
	NodeHelper:setStringForLabel(GuildData.allianceContainer, lb2Str)

	-- 自动战斗,如果是vip3以下隐藏
	NodeHelper:setNodeVisible(GuildData.allianceContainer:getVarNode("mAutoFightNode"),UserInfo.playerInfo.vipLevel>=3)
	NodeHelper:setNodeVisible(GuildData.allianceContainer:getVarSprite("mAutoFightSprite"), GuildData.MyAllianceInfo.myInfo.autoFight==1)
	-- exp bar
	local expBar = GuildData.allianceContainer:getVarScale9Sprite('mPartakeExp')
	if expBar then
		expBar:setScaleX(scale)
	end
end

----------------scrollview item of 可加入公会列表 -------------------------
local JoinListItem = {
	ccbiFile = 'GuildRecommendContent.ccbi',
}

function JoinListItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		JoinListItem.onRefreshItemView(container)
	elseif eventName == "onPartake" then
		JoinListItem.joinAlliance(container)
	end
end

function JoinListItem.onRefreshItemView(container)
	local index = container:getItemDate().mID
	local info = GuildData.allianceInfo.joinList[index]
	if not info then return end
	local lb2Str = {
		mGuildLv 		= common:getLanguageString("@GuildLevel", info.level),
		mGuildName 		= info.name,
		mGuildNum		= info.currnetPop .. ' / ' .. info.maxPoj
	}
	NodeHelper:setStringForLabel(container, lb2Str)
end	

-- 加入公会
function JoinListItem.joinAlliance(container)
	local index = container:getItemDate().mID
	local info = GuildData.allianceInfo.joinList[index]
	if not info then return end

	GuildDataManager:sendJoinAlliancePacket(info.id)
end

----------------scrollview-------------------------
function GuildData.GuildPage.rebuildAllItem()
	GuildData.GuildPage.clearAllItem(GuildData.joinListContainer)
	GuildData.GuildPage.buildItem()
end

function GuildData.GuildPage.clearAllItem()
	NodeHelper:clearScrollView(GuildData.joinListContainer)
end

function GuildData.GuildPage.buildItem()
	NodeHelper:buildScrollView(GuildData.joinListContainer, #GuildData.allianceInfo.joinList, JoinListItem.ccbiFile, JoinListItem.onFunction);
end

----------------click event------------------------
function GuildData.GuildPage.onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_ALLIANCE)
end

function GuildData.GuildPage.refreshJoinList(container)
	GuildDataManager:sendRefreshGuildListPacket()
end

-- 搜索公会
function GuildData.GuildPage.onSearchGuild(container)
	PageManager.pushPage('GuildSearchPopPage')
end

-- 创建公会
function GuildData.GuildPage.onCreateGuild(container)
	UserInfo.sync()
	if UserInfo.roleInfo.level < GuildData.CreateAllianceOpenLevel then
		MessageBoxPage:Msg_Box(common:getLanguageString('@GuildCreateLevel', GuildData.CreateAllianceOpenLevel))
		return
	end
	PageManager.pushPage('GuildCreatePage')
end

function GuildData.GuildPage.onGuildBattle(container)
	MessageBoxPage:Msg_Box('@CommingSoon')
end

function GuildData.GuildPage.onGuildRanking(container)
	PageManager.pushPage('GuildRankingPage')
end

-- 签到
function GuildData.GuildPage.onSignIn(container)
	GuildDataManager:onSignIn()
end

-- 下面一排按钮里面的boss入侵，显示‘boss伤害排行榜'
function GuildData.GuildPage.onIntrude(container)
	local bossState = GuildData.allianceInfo.commonInfo.bossState or GuildData.BossPage.BossCanInspire
	if GuildData.BossPage.BossNotOpen == bossState then
		MessageBoxPage:Msg_Box('@GuildBossWaitToOpen')
	elseif GuildData.BossPage.BossCanJoin == bossState then
		MessageBoxPage:Msg_Box('@GuildBossPleaseJoin')
	elseif GuildData.BossPage.BossCanInspire == bossState then
		PageManager.pushPage('GuildBossHarmRankPage')
	end
end

-- 贡献兑换
function GuildData.GuildPage.exchangeContribution(container)
	-- listening this packet in the pop page
	GuildDataManager:removeOnePacket(hp.ALLIANCE_ENTER_S)
	PageManager.pushPage('GuildShopPage')
end

function GuildData.GuildPage.showMembers(container)
	PageManager.pushPage('GuildMembersPage')
end

function GuildData.GuildPage.onManage(container, eventName)
	-- listening this packet in the pop page
	GuildDataManager:removeOnePacket(hp.ALLIANCE_CREATE_S)
	PageManager.pushPage('GuildManagePage')
end

-- 自动战斗
function GuildData.GuildPage.onAutoFight(container)
	-- 如果是开启状态，点击取消勾选
	if GuildData.MyAllianceInfo.myInfo.autoFight==1 then
		GuildData.GuildPage.sendAutoFightPacket(container)
	else
		local autoFightCost = VaribleManager:getInstance():getSetting("autoAllianceFightCost")
		local title = common:getLanguageString('@AllianceAutoFightTitle')
	 	local message = common:getLanguageString('@AllianceAutoFightDesc', autoFightCost)
	 	PageManager.showConfirm(title, message,
	   		function (agree)
			    if agree and UserInfo.isGoldEnough(autoFightCost) then
			    	 GuildData.GuildPage.sendAutoFightPacket(container)
			    end
		   	end
	  	)
	 end
end
function GuildData.GuildPage.sendAutoFightPacket(container)
	common:sendEmptyPacket(HP_pb.ALLIANCE_AUTO_FIGHT_C,false);
end

function GuildData.GuildPage.removeMessages(container)
	GuildData.mainContainer:removeMessage(MSG_MAINFRAME_POPPAGE)
	GuildData.mainContainer:removeMessage(MSG_MAINFRAME_REFRESH)
end

--继承此类的活动如果同时开，消息监听不能同时存在,通过tag来区分
function GuildData.GuildPage.onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_POPPAGE then
		local pageName = MsgMainFramePopPage:getTrueType(message).pageName
		if pageName == "GuildShopPage" then 
			GuildData.mainContainer:registerPacket(hp.ALLIANCE_ENTER_S)
		elseif pageName == 'GuildManagePage' then
			GuildData.mainContainer:registerPacket(hp.ALLIANCE_CREATE_S)
			GuildData.GuildPage.refreshPage()
		elseif pageName == 'GuildOpenBossConfirmPage' then
			GuildData.mainContainer:registerPacket(hp.ALLIANCE_CREATE_S)
		end
	elseif typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == 'GuildPage' then
			-- boss opened by leader
			-- request new alliance info and refresh page
			GuildDataManager:requestBasicInfo()
		elseif pageName == 'GuildPage_Refresh_Right_Now' then
			GuildData.GuildPage.refreshPage()
		elseif pageName == "GuildPage_Refresh_GuildData.BossPage" then
			GuildData.BossPage.refreshPage(GuildData.bossContainer)
		end
	end
end


function GuildData.GuildPage.onReceiveAllianceEnterInfo(container, msg)
	-- check if need change interface(change ccbi)
	GuildData.MyAllianceInfo = msg
	
	-- request joinlist
	if not GuildData.MyAllianceInfo.hasAlliance then
		GuildDataManager:getGuildListPacket()
	end

	-- receive this packet from sign in packet
	if GuildData.allianceInfo.signInFlag then
		GuildData.allianceInfo.signInFlag = false
		MessageBoxPage:Msg_Box('@GuildSignInSuccess')
	end
end

function GuildData.GuildPage.doBossOperation(container, operType)
	local msg = alliance.HPAllianceBossFunOpenC()
	msg.operType = operType
	local pb = msg:SerializeToString()
	GuildData.mainContainer:sendPakcet(hp.ALLIANCE_BOSSFUNOPEN_C, pb, #pb, true)
end

function GuildData.GuildPage.onReceiveBossHarm(container, msg)
	GuildData.BossPage.bossBloodLeft = tonumber(GuildData.BossPage.bossBloodLeft - msg.value)
	local harm = common:getLanguageString('@GuildBossHarmValue', tostring(msg.value))
	--MessageBoxPage:Msg_Box(harm)
	if GuildData.bossHitContainer then
		NodeHelper:setStringForLabel(GuildData.bossHitContainer, { mNumLabel = harm })
		GuildData.bossHitContainer:runAnimation('showNum')
	end

	if GuildData.BossPage.bossBloodLeft <= 0 then
		-- if boss is over, reset page
		GuildDataManager:requestBasicInfo()
	end
end

-- create result
function GuildData.GuildPage.onReceiveAllianceInfo(container, msg)
	GuildData.allianceInfo.commonInfo = msg
	
	-- adjust blood left
	if msg:HasField('bossHp') then
		GuildData.BossPage.bossBloodLeft = msg.bossHp
	end

	-- 校正boss倒计时
	if msg:HasField('bossTime') then
		local bossTime = tonumber(msg.bossTime) and tonumber(msg.bossTime) or 600
		TimeCalculator:getInstance():createTimeCalcultor(GuildData.BossPage.CDTimeKey, bossTime)
		if bossTime <= 0 and (msg.bossState ~= GuildData.BossPage.BossNotOpen) then
			-- boss is over, reset page
			GuildDataManager:requestBasicInfo()
		end
	end

	if GuildData.BossPage.bossJoinFlag then
		-- 收到了加入战斗的回包
		GuildData.BossPage.bossJoinFlag = false
		local bossTime = tonumber(msg.bossTime) and tonumber(msg.bossTime) or 600
		TimeCalculator:getInstance():createTimeCalcultor(GuildData.BossPage.CDTimeKey, bossTime)
	end
end

function GuildData.GuildPage.requestRankingList()
	GuildDataManager:requestRankingList()
end

function GuildData.GuildPage.onReceiveRankingList(msg)
	GuildData.rankInfoInited = true
	GuildDataManager:removeOnePacket(hp.ALLIANCE_RANKING_S)
	if msg.showTag then
		GuildDataManager:setRankInfo(msg.rankings)
	else
		GuildDataManager:setRankInfo({})
	end
end

function GuildData.GuildPage.onReceiveMembers(msg)
	GuildData.memberInfoInited = true
	GuildDataManager:removeOnePacket(hp.ALLIANCE_MEMBER_S)
	GuildDataManager:setGuildMemberList(msg.memberList)
end

function GuildData.GuildPage.onReceiveShopList(msg)
	GuildData.shopInfoInited = true
	GuildDataManager:removeOnePacket(hp.ALLIANCE_SHOP_S)
	GuildDataManager:setGuildMemberList(msg.items)
end

function GuildData.GuildPage.onReceiveHarmRank(msg)
	GuildData.bossRankInited = true
	GuildDataManager:removeOnePacket(hp.ALLIANCE_HARMSORT_S)
	if msg.showTag then
		GuildDataManager:setHarmRank(msg.harms)
	else
		GuildDataManager:setHarmRank({})
	end
end

function GuildData.GuildPage.onReceivePacket(container)
	local opcode = GuildData.mainContainer:getRecPacketOpcode()
	local msgBuff = GuildData.mainContainer:getRecPacketBuffer()

	if opcode == hp.ALLIANCE_ENTER_S then
		-- alliance enter
		local msg = alliance.HPAllianceEnterS()
		msg:ParseFromString(msgBuff)
		GuildData.GuildPage.onReceiveAllianceEnterInfo(container, msg)
		GuildData.GuildPage.refreshPage()
		return
	end

	if opcode == hp.ALLIANCE_CREATE_S then
		-- create alliance
		local msg = alliance.HPAllianceInfoS()
		msg:ParseFromString(msgBuff)
		GuildData.BossPage.onAttributeOpen(GuildData.bossContainer)
		GuildData.GuildPage.onReceiveAllianceInfo(container, msg)
		GuildData.GuildPage.refreshPage()
		return
	end

	if opcode == hp.ALLIANCE_HARMSORT_S then
		local msg = alliance.HPAllianceHarmSortS()
		msg:ParseFromString(msgBuff)
		GuildData.GuildPage.onReceiveHarmRank(msg)
		return
	end

	if opcode == hp.ALLIANCE_SHOP_S then
		-- alliance enter
		local msg = alliance.HPAllianceShopS()
		msg:ParseFromString(msgBuff)
		GuildData.GuildPage.onReceiveShopList(msg)
		return
	end

	if opcode == hp.ALLIANCE_JOIN_LIST_S then
		-- alliance join list.
		local msg = alliance.HPAllianceJoinListS()
		msg:ParseFromString(msgBuff)
		GuildDataManager:onReceiveJoinList(msg)
		GuildData.GuildPage.refreshPage()
		return
	end

	if opcode == hp.ALLIANCE_BOSSHARM_S then
		-- alliance join list.
		local msg = alliance.HPAllianceBossHarmS()
		msg:ParseFromString(msgBuff)
		GuildData.GuildPage.onReceiveBossHarm(container, msg)
		GuildData.BossPage.refreshPage(GuildData.bossContainer)
		return
	end

	if opcode == hp.ALLIANCE_RANKING_S then
		local msg = alliance.HPAllianceRankingS()
		msg:ParseFromString(msgBuff)
		GuildData.GuildPage.onReceiveRankingList(msg)
		return
	end

	if opcode == hp.ALLIANCE_MEMBER_S then
		local msg = alliance.HPAllianceMemberS()
		msg:ParseFromString(msgBuff)
		GuildData.GuildPage.onReceiveMembers(msg)
		return
	end
end

function luaCreat_GuildPage(container)
	container:registerFunctionHandler(GuildData.GuildPage.onFunction)
end	


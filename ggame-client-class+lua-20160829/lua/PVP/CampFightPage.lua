----------------------------------------------------------------------------------
--[[
	FILE:			CampFightPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	阵营站战斗页面
	AUTHOR:			zhenhui
	CREATED:		2014-09-18
--]]
----------------------------------------------------------------------------------


local thisPageName = "CampFightPage";
local HP = require("HP_pb");
local CampWar_pb = require("CampWar_pb");
local UserInfo = require("UserInfo");
local m_chatPageInMove = false

local opcodes = {
	CAMPWAR_INFO_SYNC_S = HP.CAMPWAR_INFO_SYNC_S,
	CAMPWAR_STATE_SYNC_S = HP.CAMPWAR_STATE_SYNC_S
};	

local option = {
	ccbiFile = "CampWarBattlePage.ccbi",
	handlerMap = {
		onInspire = "onInspire",
		onMyInformation = "onMyInformation",
		onAllReport = "onAllReport",
		onShowNo1   = "onShowNo1",
		onShowNo2   = "onShowNo2",
		onShowNo3   = "onShowNo3",
		onHelp   = "onHelp",
		onCampWarMyInforBtn   = "onCampWarMyInforBtn",
		onPersonalReport = "onPersonalReport"
	},
	opcode = opcodes
};

local CampFightPageBase = {};

local NodeHelper = require("NodeHelper");

local CampWarManager = require("CampWarManager")

local CampFightleftTimeKey = 'CampLeftCD'

local TimerFunc = {};

local m_showPersonalFlag = false

local reportList = {}

local reportGapTime = 0

-----------------------------------------------

function CampFightPageBase:onEnter(container)
	self:registerPacket(container);					
	--在数据回来之前，用原有的数据做缓存，不至于显示英文
	reportList = {}
	CampWarManager.campReportList = {}
	--self:refreshPage(container);
	CampFightPageBase:onRequestInfo(container)
end	

function CampFightPageBase:onRequestInfo(container)
	common:sendEmptyPacket(HP.CAMPWAR_INFO_C);
end

function CampFightPageBase:onExecute(container)
	TimerFunc.updateCD(container)	
	self:getMsgDataAndInsert(container)
	self:isShowtimeClose(container)
end

function CampFightPageBase:isShowtimeClose(container)
	if CampWarManager.state == CampWar_pb.CAMP_WAR_CLOSED then
		CampWarManager.EnterPageByState()
	end
end

function CampFightPageBase:getMsgDataAndInsert(container)
	local dt = GamePrecedure:getInstance():getFrameTime() * 1000;
	reportGapTime = reportGapTime + dt
	if reportGapTime > GameConfig.CampWar.GapSecond	then
		local totalReportSize = #CampWarManager.campReportList
		if totalReportSize >0 then
			oneMsg = table.remove(CampWarManager.campReportList,1);
			if #reportList > GameConfig.CampWar.reportMax then
				table.remove(reportList,1)
			end
			table.insert(reportList,oneMsg)
			CampFightPageBase:refreshReportPanel(container)
		end
		reportGapTime = 0
	end
end

function CampFightPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_CAMP)
end


function CampFightPageBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile);
	container.scrollview=container:getVarScrollView("mContent");
	if container.scrollview == nil then
		CCLuaLog("error in scrollView")
	end
	container:autoAdjustResizeScrollview(container.scrollview);	
	container.scrollViewRootNode=container.scrollview:getContainer();
	local mScale9Sprite1 = container:getVarScale9Sprite("mScale9Sprite1")
	if mScale9Sprite1 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite1 )
	end
	local mScale9Sprite2 = container:getVarScale9Sprite("mScale9Sprite2")
	if mScale9Sprite2 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite2 )
	end
	
	local layer = container.scrollview:getParent():getChildByTag(51001);
	
	if not layer then
		layer = CCLayer:create();
		layer:setTag(51001);
		container.scrollview:getParent():addChild(layer);
		layer:setContentSize(container.scrollview:getViewSize());
		layer:setPosition(container.scrollview:getPosition())
		layer:registerScriptTouchHandler(CampFightPageBase.onTouchCampLayer,false,0,false);
		layer:setTouchEnabled(true);
	end
	layer:setVisible(true);
	
end

function CampFightPageBase.onTouchCampLayer(eventName,pTouch )
	if eventName == "began" then
		local point = pTouch:getLocationInView();
		point = CCDirector:sharedDirector():convertToGL(point);
		m_BegainY = point.y;
	elseif eventName == "moved" then
		m_chatPageInMove = true
	elseif eventName == "ended" then
		m_chatPageInMove = false	
	elseif eventName == "cancelled" then
		
	end
end


--准备状态倒计时
function TimerFunc.updateCD( container )
	local cdString = '00:30:00'
	if CampWarManager.state == CampWar_pb.CAMP_WAR_FIGHT  then 
		if TimeCalculator:getInstance():hasKey(CampFightleftTimeKey) then
			local timeleft = TimeCalculator:getInstance():getTimeLeft(CampFightleftTimeKey)
			if timeleft > 0 then
				 cdString = GameMaths:formatSecondsToTime(timeleft)
			else
				CCLuaLog("TimerFunc.updateCD -- timeleft <0 ");
			end			
		end		
	elseif	CampWarManager.state == CampWar_pb.CAMP_WAR_SHOW_RESULT  then 
		cdString = '00:00:00'
	end
	NodeHelper:setStringForLabel(container, { mCD = cdString})		
end

function CampFightPageBase:onExit(container)
	self:removePacket(container)	
	TimeCalculator:getInstance():removeTimeCalcultor(CampFightleftTimeKey);		
end
----------------------------------------------------------------

function CampFightPageBase:refreshPage(container)
	if CampWarManager.m_curCampFightInfo == nil or CampWarManager.m_curCampFightInfo.personalState == nil then
		return 
	end
	CampFightPageBase:refreshUpperPanel(container)
	CampFightPageBase:refreshRankPanel(container)	
	CampFightPageBase:refreshSetting(container)
	CampFightPageBase:refreshFinishNode(container)
	CampFightPageBase:refreshPersonalStatic(container)
end

function CampFightPageBase:refreshFinishNode(container)
	if CampWarManager.state == CampWar_pb.CAMP_WAR_SHOW_RESULT  then 
		NodeHelper:setNodesVisible(
		container,{	mHasFinishNode = true	}	)
	else
		NodeHelper:setNodesVisible(
		container,{	mHasFinishNode = false	}	)
	end
	
end

function CampFightPageBase:refreshUpperPanel(container)
	--refresh cd
	if CampWarManager.state == CampWar_pb.CAMP_WAR_FIGHT  then 
		if CampWarManager.m_curCampFightInfo.campRemainTime > 0 and not TimeCalculator:getInstance():hasKey(CampFightleftTimeKey) then
			TimeCalculator:getInstance():createTimeCalcultor(CampFightleftTimeKey, CampWarManager.m_curCampFightInfo.campRemainTime);
		end	
	end		
	--refresh in which camp, blue or red 1 表示左边的蓝色阵营，2表示右边红色阵营
	if CampWarManager.m_curCampFightInfo.personalState.campId == 1 then
		NodeHelper:setNodesVisible(
		container,{
			mBlueCampNode = true,
			mRedCampNode = false
		}
		)
	else
		NodeHelper:setNodesVisible(
		container,{
			mBlueCampNode = false,
			mRedCampNode = true
		}
		)
	end		
	local blueCampPoint = CampWarManager.getCurrentPointByCampId(1)
	local redCampPoint = CampWarManager.getCurrentPointByCampId(2)
	local hpStr = tostring(CampWarManager.m_curCampFightInfo.personalState.curRemainBlood)..'/'..tostring(CampWarManager.m_curCampFightInfo.personalState.curMaxBlood)
	local bonusStr = "+"..tostring(CampWarManager.m_curCampFightInfo.personalState.bonuses).."%"
	NodeHelper:setStringForLabel(container,{
		mHpNum = hpStr,
		mInspireNum = bonusStr,
		mBlueCamp = tostring(blueCampPoint),
		mRedCamp = tostring(redCampPoint)
	})		
	--	血条显示
	local curHp = container:getVarScale9Sprite("mHp")	
	local hpPercent = CampWarManager.m_curCampFightInfo.personalState.curRemainBlood / CampWarManager.m_curCampFightInfo.personalState.curMaxBlood
	hpPercent = math.min(hpPercent,1.0)
	hpPercent = math.max(hpPercent,0.0)
	curHp:setScaleX(hpPercent)
	
	--进度条显示	
	local campWarHp = container:getVarScale9Sprite("mCampWarHp")
	CCLuaLog("blueCampPoint = "..blueCampPoint.."&redCampPoint = "..redCampPoint)
	local percent = 0.5
	if blueCampPoint == 0 and redCampPoint == 0 then
		percent = 0.5
	elseif blueCampPoint == 0 and redCampPoint >0 then
		percent = 0.0
	elseif blueCampPoint >0 and redCampPoint == 0 then
		percent = 1.0
	else
		percent = blueCampPoint / (blueCampPoint + redCampPoint )
	end
	percent = percent * 2
	percent = math.min(percent,2.0)
	percent = math.max(percent,0.0)
	campWarHp:setScaleX(percent)
	
	
end

function CampFightPageBase:refreshPersonalStatic(container)
	
	if m_showPersonalFlag == true then
		NodeHelper:setNodesVisible(container,{mMyInforNode = true})
	else
		NodeHelper:setNodesVisible(container,{mMyInforNode = false})
	end
	
	local ConfigManager = require("ConfigManager")
	local winStreakCfg = ConfigManager.getCampWarWinStreak()
	NodeHelper:setStringForLabel(container,{
		mLevelNum = tostring(UserInfo.roleInfo.level)
	})
	local curWinStreak = CampWarManager.m_curCampFightInfo.personalState.curWinStreak
	if curWinStreak > 0 and winStreakCfg[curWinStreak] ~=nil and winStreakCfg ~=nil then
		NodeHelper:setStringForLabel(container,{
			mBasicEarningsPrestigeNum = tostring(GameConfig.CampWar.basicRep),
			mBasicEarningsGoldNum = tostring(GameConfig.CampWar.basicCoin * UserInfo.roleInfo.level),	
			mContinuousKillPrestigeNum = tostring(winStreakCfg[curWinStreak].winReputation),
			mContinuousKillGoldNum =  tostring(winStreakCfg[curWinStreak].winCoinsRatio * UserInfo.roleInfo.level),
			mHighestNum = tostring(CampWarManager.m_curCampFightInfo.personalState.maxWinStreak),
			mVictoryNum = tostring(CampWarManager.m_curCampFightInfo.personalState.totalWin),
			mReputationNum = tostring(CampWarManager.m_curCampFightInfo.personalState.totalReputation),
			mGoinNum = tostring(CampWarManager.m_curCampFightInfo.personalState.totalCoins),
			mNowNum = tostring(curWinStreak),
			mFailureNum = tostring(CampWarManager.m_curCampFightInfo.personalState.totalLose)
		})
	end
	
end

function CampFightPageBase:refreshSetting(container)
	NodeHelper:setNodesVisible(
		container,{
			mAllChoice = CampWarManager.m_showAllReport,
			mPersonalChoice = not CampWarManager.m_showAllReport
		}
		)
end

function CampFightPageBase:refreshRankPanel(container)
	local rankSize = #CampWarManager.m_curCampFightInfo.rankInfo
	if #CampWarManager.m_curCampFightInfo.rankInfo == 0 then
		NodeHelper:setNodesVisible(
		container,{
			mCampPrepareNode   = true,
			mCampRankNode    = false
		}
		)
	else	
		NodeHelper:setNodesVisible(
			container,{
				mCampPrepareNode   = false,
				mCampRankNode    = true
			}
			)	
		--判断排行个数，有可能只有两个或者一个
		local RoleManager = require("RoleManager");
		for i=1,3 do
			local nodeStr = "mNo"..tostring(i).."Node"
			if i<=rankSize then			
				NodeHelper:setNodesVisible(
				container,{
					nodeStr   = true
				}
				)
				--基本信息	
				local killNumStr = "mNo"..tostring(i).."KillNum" 
				local nameStr = "mNo"..tostring(i).."Name" 
				local occuStr = "mNo"..tostring(i).."Occupation" 
				local labelMap = {}
				labelMap[killNumStr] = tostring(CampWarManager.getKillRankInfoByRank(i).multiKillQty)
				labelMap[nameStr] = CampWarManager.getKillRankInfoByRank(i).playerName
				local spriteMap = {}
				spriteMap[occuStr] = RoleManager:getOccupationIconById(CampWarManager.getKillRankInfoByRank(i).roleItemId)
				if CampWarManager.getKillRankInfoByRank(i) ~=nil then
					NodeHelper:setStringForLabel(container,labelMap)	
					NodeHelper:setSpriteImage(container, spriteMap);
				end						
			else
				NodeHelper:setNodesVisible(
				container,{
					nodeStr   = false
				}
				)
			end								
		end			
	end
	
	
end

function CampFightPageBase:refreshReportPanel(container)
	local reportSize = #reportList
	local currentPos = 0 	
	container.scrollViewRootNode:removeAllChildren();
	for i=reportSize,1,-1 do
		local oneReport = reportList[i]
		if CampWarManager.m_showAllReport == false then
			--如果是只查看个人记录
			if oneReport.reportType == CampWar_pb.PERSONAL_WIN_TYPE or oneReport.reportType == CampWar_pb.PERSONAL_LOSE_TYPE then
				local reportStr = CampWarManager.generateHtmlReport(oneReport)
				reportStr:setPosition(ccp(0,currentPos));
				currentPos = currentPos + reportStr:getContentSize().height;
				container.scrollViewRootNode:addChild(reportStr)
			end				
		else
			local reportStr = CampWarManager.generateHtmlReport(oneReport)
			reportStr:setPosition(ccp(0,currentPos));
			currentPos = currentPos + reportStr:getContentSize().height;
			container.scrollViewRootNode:addChild(reportStr)
		end	
	end	
	
	if m_chatPageInMove then
		return 
	end
	
	local viewHeight = container.scrollview:getViewSize().height	
	container.scrollview:setContentSize(CCSize(640,currentPos))	;		
	if currentPos <viewHeight then
		container.scrollview:setContentOffset(ccp(0,viewHeight-currentPos));
	else
		container.scrollview:setContentOffset(ccp(0,0));
	end		
end	

function CampFightPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == HP.CAMPWAR_INFO_SYNC_S  then
		self:refreshPage(container)
		return
	end		
	
	if opcode == HP.CAMPWAR_STATE_SYNC_S then
		self:refreshPage(container)
		return
	end				
end

function CampFightPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end	

function CampFightPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

function CampFightPageBase:onShowNo1(container)
	local playerId = CampWarManager.getKillRankInfoByRank(1).playerId
	if playerId~=nil and playerId >0 then
		PageManager.viewPlayerInfo(playerId)
	end
end

function CampFightPageBase:onShowNo2(container)
	local playerId = CampWarManager.getKillRankInfoByRank(2).playerId
	if playerId~=nil and playerId >0 then
		PageManager.viewPlayerInfo(playerId)
	end
end

function CampFightPageBase:onShowNo3(container)
	local playerId = CampWarManager.getKillRankInfoByRank(3).playerId
	if playerId~=nil and playerId >0 then
		PageManager.viewPlayerInfo(playerId)
	end
end


function CampFightPageBase:onAllReport(container)
	CampWarManager.m_showAllReport = true
	CampFightPageBase:refreshPage(container)
	CampFightPageBase:refreshReportPanel(container)
end

function CampFightPageBase:onPersonalReport(container)
	CampWarManager.m_showAllReport = false
	CampFightPageBase:refreshPage(container)
	CampFightPageBase:refreshReportPanel(container)
end

function CampFightPageBase:onInspire(container)
	
	local title = Language:getInstance():getString("@CampWarInspriteTitle")	
	local message = Language:getInstance():getString("@CampWarFightInspirationHint")	
	PageManager.showConfirm(title,message, function(isSure)
		if isSure and UserInfo.isGoldEnough(GameConfig.Cost.CampInspire) then
			common:sendEmptyPacket(HP.CAMPWAR_INSPIRE_C,false);
		end
	end);
	
end

function CampFightPageBase:onMyInformation(container)
	--查看个人信息
	m_showPersonalFlag = not m_showPersonalFlag
	CampFightPageBase:refreshPersonalStatic(container)
end


function CampFightPageBase:onCampWarMyInforBtn(container)
	m_showPersonalFlag = false
	CampFightPageBase:refreshPersonalStatic(container)
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local CampFightPage = CommonPage.newSub(CampFightPageBase, thisPageName, option);
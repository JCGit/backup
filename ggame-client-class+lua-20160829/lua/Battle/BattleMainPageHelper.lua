local Chat_pb = require "Chat_pb"
local HP_pb = require "HP_pb"
local Const_pb = require "Const_pb"
local UserInfo = require("UserInfo");
local ChatManager = require("ChatManager")
--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			BattleMainPageHelper.lua
--  DESCRIPTION:	BattleMainPageHelper
--
--
--  AUTHOR:		Zhen Hui
--  CREATED:	2014/8/3
--  MODIFY2:
--
--  MODIFY1:
--
--------------------------------------------------------------------------------

local UserTypeContent = nil;

local BattleMainPageHelper = {}

local titleManager = require("TitleManager")


BattlePageEnum = {
PAGE_BATTLE = 1,
PAGE_CHAT =2
}

local BattleChatManager = {
chatMaxHeight = 600,
chatMinHeight = 300,

}


local NodeHelper = require("NodeHelper");
local titleManager = require("TitleManager")

local mSurplusSecTime = 0

local mDurationTime = 0.0

local currentChanel = Const_pb.CHAT_WORLD
Flag_Chat = 0
--下一个战斗将挑战谁  TODO
nextChanllengeName = nil
--boss or arena
nextFightIsBoss = true
--战斗页面中“世界聊天”的红点
hasNewChatComing = false
--聊天页面中“新的世界聊天”红点
hasNewWorldChatComing = false
--聊天页面中“新的工会聊天”红点
hasNewMemberChatComing = false

--是否战斗日志页面或者是世界聊天
--true = 战斗日志
--false = 世界聊天
local inBattleOrChat = true

local currentPageIndex = BattlePageEnum.PAGE_BATTLE
--上次战斗统计
local  lastBattleStatic= nil

local accountFlag = false;

chatPageInMove = false

------------------------战斗日志主页面相关---------------

function BattleMainPageHelper:initNode(container)
	--根据聊天SV 的pos,viewSize计算聊天最大、最小的位置
	local chatSVSize = container.scrollview:getViewSize();
	local chatSVWorldPos = container.scrollview:getContainer():getParent():convertToWorldSpaceAR(ccp(container.scrollview:getContainer():getPositionX(),container.scrollview:getContainer():getPositionY()))
	BattleChatManager.chatMaxHeight = chatSVWorldPos.y + chatSVSize.height
	BattleChatManager.chatMinHeight = chatSVWorldPos.y
end

function BattleMainPageHelper:onBattleJournal(container)
	hasNewChatComing = false
	BattleMainPageHelper:showBattlePanel(container, true);
end

function BattleMainPageHelper:onWorldChat(container)
	BattleMainPageHelper:showBattlePanel(container, false);
end

function BattleMainPageHelper:showPrivatePage(container)
	BattleMainPageHelper:showPage(container,BattlePageEnum.PAGE_CHAT,Const_pb.CHAT_PERSONAL)
end

function BattleMainPageHelper:showPage(container,pageIndex,subIndex)
	currentChanel = subIndex == nil and Const_pb.CHAT_WORLD or subIndex
	pageIndex = pageIndex==nil and BattlePageEnum.PAGE_BATTLE or pageIndex
	if pageIndex == BattlePageEnum.PAGE_BATTLE then
		BattleMainPageHelper:showBattlePanel(container, true)
	else
		BattleMainPageHelper:showBattlePanel(container, false)
	end
end

function  BattleMainPageHelper:onViewPlayerInPrivate(container)
	if currentPageIndex == BattlePageEnum.PAGE_CHAT and currentChanel == Const_pb.CHAT_PERSONAL then
		local currentPlayerId = ChatManager.getCurrentChatId()
		if currentPlayerId ==0 then return end
		PageManager.viewPlayerInfo(currentPlayerId, true);
	end
end

function BattleMainPageHelper:showBattlePanel(container, visible)
	inBattleOrChat = visible;
	if inBattleOrChat then
		currentPageIndex = BattlePageEnum.PAGE_BATTLE
	else
		currentPageIndex = BattlePageEnum.PAGE_CHAT
	end
	NodeHelper:setNodeVisible(container:getVarNode("mBattleJournalNode"), visible);
	NodeHelper:setNodeVisible(container:getVarNode("mWordChatNode"), not visible);
	
	--刷新工会聊天按钮按钮
	BattleMainPageHelper:refreshChatPage(container)
end


function BattleMainPageHelper.toPurchaseTimes(flag)
	if flag then
		local message = Battle_pb.HPBuyFastFightTimes()
		if message~=nil then
			message.times = 1;
			local pb_data = message:SerializeToString();
			PacketManager:getInstance():sendPakcet(HP_pb.BUY_FAST_FIGHT_TIMES_C,pb_data,#pb_data,false);
		end
	end
end

function BattleMainPageHelper:onFastBattle(container)
	
	UserInfo.sync()
	local fastFightTime = UserInfo.stateInfo.fastFightTimes
	
	--[[if fastFightTime > 0 then
	--如果有剩余快速战斗次数，直接发战斗协议
	local message = Battle_pb.HPFastBattle()
	if message~=nil then
		--当前挂机地图
		message.mapId = UserInfo.stateInfo.curBattleMap;
		local pb_data = message:SerializeToString();
		PacketManager:getInstance():sendPakcet(HP_pb.BATTLE_FAST_FIGHT_C,pb_data,#pb_data,false);
		return;
	end
end--]]
--如果没有的话，提示购买
local title = Language:getInstance():getString("@FastBuyTimeTitle")
local message = Language:getInstance():getString("@FastBuyTimeMsg")

local vipLevel = UserInfo.playerInfo.vipLevel;
local vipStr = tostring(vipLevel)
UserInfo.syncStateInfo()

--购买次数钻石配置，--buyBossFightPrice = 50,100,100，200
local buyFastFightPrice = "30,50,50,90,90,120,120,150,150"

local  fastTimeTable=Split(buyFastFightPrice,",")
local buyedTime = UserInfo.stateInfo.fastFightBuyTimes
local price = 150
if buyedTime<9 then
	price = tonumber(fastTimeTable[buyedTime+1]);
end
-- 开服活动，第七天快速消耗一半钻石
if GlobalData.diamondRatio~=nil and GlobalData.diamondRatio ~= 0 then
	price = price*GlobalData.diamondRatio
end

if UserInfo.isNewbeeInFastFight() and price > UserInfo.playerInfo.gold then
	local title = common:getLanguageString("@GoldNotEnoughFastFight_Title");
	local msg = common:getLanguageString("@GoldNotEnoughFastFight_Msg");
	PageManager.showConfirm(title, msg, function(isSure)
		if isSure then
			PageManager.pushPage("GiftPage");
		end
	end);
	return;
end

if not UserInfo.isGoldEnough(price) then
	return;
end

local vipCfg = ConfigManager.getVipCfg();
local leftTime = vipCfg[vipLevel]["fastFightTime"] - buyedTime
--local leftTime = fastFightTime
local infoTab = {tostring(price), vipStr,tostring(leftTime) }

PageManager.showConfirm(title,common:getGsubStr(infoTab,message),BattleMainPageHelper.toPurchaseTimes)

end

function BattleMainPageHelper:refreshMapStatics(container,message)
	if message== nil  then
		CCLuaLog("ERROR in BattleMainPageHelper:refreshMapStatics -- message is nil")
		return;
	end
	
	local fightTimes = message.fightTimes
	local averageTime = message.averageTime
	local winRate = message.winRate
	local equipRate = message.equipRate
	local expRate = message.expRate
	local coinRate = message.coinRate
	local equipRatio = message.equipRatio
	local expRatio = message.expRatio
	local coinRatio = message.coinRatio
	
	--经验
	local expFinalStr = ""
	if expRatio ~=nil and expRatio > 1 then
		expFinalStr = common:fill(Language:getInstance():getString("@TotalExpDataRatio"),tostring(math.floor(message.expRate/expRatio)),tostring(expRatio))
	else
		expFinalStr = common:fill(Language:getInstance():getString("@TotalExpData"),tostring(message.expRate))
	end
	
	--装备掉率
	local equipFinalStr = ""
	if equipRatio ~=nil and equipRatio > 1 then
		equipFinalStr = common:fill(Language:getInstance():getString("@LootDataRatio"),tostring(math.floor(message.equipRate/equipRatio)),tostring(equipRatio))
	else
		equipFinalStr = common:fill(Language:getInstance():getString("@LootData"),tostring(message.equipRate))
	end
	
	--金币掉率
	local coinFinalStr = ""
	if coinRatio ~=nil and coinRatio > 1 then
		coinFinalStr = common:fill(Language:getInstance():getString("@TotalCoinDataRatio"),tostring(math.floor(message.coinRate/coinRatio)),tostring(coinRatio))
	else
		coinFinalStr = common:fill(Language:getInstance():getString("@TotalCoinData"),tostring(message.coinRate))
	end
	
	local lb2Str = {
	mBattleAmounts 		= common:fill(Language:getInstance():getString("@BattleAmountsData"),tostring(message.fightTimes)),
	mAverageTime 		= common:fill(Language:getInstance():getString("@AverageTimeData"),tostring(message.averageTime)),
	mWinRate 		= common:fill(Language:getInstance():getString("@WinRateData"),tostring(message.winRate)),
	mLoot 		= equipFinalStr,
	mTotalExp 		= expFinalStr,
	mTotalCoin 		= coinFinalStr
	};
	NodeHelper:setStringForLabel(container, lb2Str);
	
	
	if lastBattleStatic~=nil then
		local lb2Str = {
		mBattleAmounts 		= BattleMainPageHelper:isRedOrGreen(lastBattleStatic.fightTimes,message.fightTimes),
		mAverageTime 		= BattleMainPageHelper:isTimeRedOrGreen(lastBattleStatic.averageTime,message.averageTime),
		mWinRate 		= BattleMainPageHelper:isRedOrGreen(lastBattleStatic.winRate,message.winRate),
		mLoot 		= BattleMainPageHelper:isRedOrGreen(lastBattleStatic.equipRate,message.equipRate),
		mTotalExp 		= BattleMainPageHelper:isRedOrGreen(lastBattleStatic.expRate,message.expRate),
		mTotalCoin 		= BattleMainPageHelper:isRedOrGreen(lastBattleStatic.coinRate,message.coinRate)
		};
		NodeHelper:setColorForLabel(container, lb2Str);
	end
	
	
	--如果winRate 大于95% 弹出提示框
	local hintNode = container:getVarNode("mHintNode")
	if message.winRate > 95 then
		hintNode:setVisible(true)
	else
		hintNode:setVisible(false)
	end
	
	--人物升级至lv #v1#还需#v2#经验，约需要#v3#分钟
	UserInfo.syncRoleInfo();
	local level = UserInfo.roleInfo.level or 1;
	local currentExp = UserInfo.roleInfo.exp
	local roleExpCfg = ConfigManager.getRoleLevelExpCfg()
	local accountLabel = container:getVarLabelBMFont("mBattleAccountTex")
	if accountLabel then
		if level>=GameConfig.LevelLimit.PlayerLevel then
			local strMaxLv = Language:getInstance():getString("@AlreadyLevelMax")
			accountLabel:setString(strMaxLv)
		else
			local nextLevelExp = roleExpCfg[level]["exp"]
			
			local needExp =   nextLevelExp - currentExp
			local expPerHour =  0
			local needTimeStr = ""
			if  expRate ~=nil and expRate>0 then
				expPerHour = expRate
				local needSecond = needExp / expPerHour * 3600
				needTimeStr = common:secondToDateXXYY(needSecond)
			else
				needTimeStr = Language:getInstance():getString("@BattleLevelUpInfiniteTime")
			end
			
			
			local battleLevelExp = common:fill(Language:getInstance():getString("@BattleAccountLevelExp"),tostring(level + 1),
			tostring(needExp),needTimeStr)
			accountLabel:setString(battleLevelExp);
		end
		
	end
	lastBattleStatic = message
end

function BattleMainPageHelper:isRedOrGreen(lastNum,currentNum)
	lastNum = tonumber(lastNum)
	currentNum = tonumber(currentNum)
	local redColor = "255 0 0"
	local greenColor = "0 255 0"
	local whiteColor = "0 0 0"
	if currentNum>lastNum then
		return greenColor
	elseif currentNum<lastNum then
		return redColor
	else
		return whileColor
	end
end

function BattleMainPageHelper:isTimeRedOrGreen(lastNum,currentNum)
	lastNum = tonumber(lastNum)
	currentNum = tonumber(currentNum)
	local redColor = "255 0 0"
	local greenColor = "0 255 0"
	local whiteColor = "0 0 0"
	if currentNum<lastNum then
		return greenColor
	elseif currentNum>lastNum then
		return redColor
	else
		return whileColor
	end
end

function BattleMainPageHelper:onBattleStatistics(container)
	local accountNode = container:getVarNode("mAccountNode")
	if accountFlag == false then
		accountFlag = true
		accountNode:setVisible(true)
	else
		accountFlag = false
		accountNode:setVisible(false)
	end
	
	--第一次进入，刷新地图状态
	if g_firstGotMapStatic then
		BattleMainPageHelper:refreshMapStatics(container,g_mapStaticMsg)
		g_firstGotMapStatic = false
	end
end


--提示胜率已经超过95%,可以打BOSS 了
function BattleMainPageHelper:showNotice(container)
	local title = Language:getInstance():getString("@HintTitle")
	local message = Language:getInstance():getString("@CurrentMapHasReachLimit")
	
	PageManager.showConfirm(title,message,BattleMainPageHelper.showWorldMapDecision)
end

function BattleMainPageHelper.showWorldMapDecision(flag)
	--Todo
	if flag == true then
		PageManager.changePage("MapPage");
	end
end

function BattleMainPageHelper:showWorldMap(container)
	PageManager.changePage("MapPage");
end


------------------------聊天页面相关---------------

function BattleMainPageHelper:onChatContent(container)
	g_chatType = world
	UserTypeContent = nil
	Flag_Chat = 0
	container:registerLibOS()
	libOS:getInstance():showInputbox(false);
end

function BattleMainPageHelper:onInputboxEnter(container)
	--if MainFrame:getInstance():lastPushPageName() == "BattlePage" then
	if Flag_Chat ~= 0 then
		return
	end
	local content = container:getInputboxContent();
	local nameOK = true
	if GameMaths:isStringHasUTF8mb4(content) then
		nameOK = false
	end
	--[[
	if not RestrictedWord:getInstance():isStringOK(content) then
		nameOK = false
	end
	--]]
	if content == "" then
		nameOK = false
	end
	if not nameOK then
		local contentLabel = container:getVarLabelTTF("mChatLabe");
		local label = Language:getInstance():getString("@ChatLabe")
		contentLabel:setString(label)
		MessageBoxPage:Msg_Box("@NameHaveForbbidenChar")
		content = nil
		return
	end
	
	content = RestrictedWord:getInstance():filterWordSentence(content);
	local contentLabel = container:getVarLabelTTF("mChatLabe");
	if contentLabel ~= nil then
		local length = GameMaths:calculateStringCharacters(content);
		if  length > 20 then
			--提示名字字数
			MessageBoxPage:Msg_Box_Lan("@InputWordExceedLimit");
			content = nil
			return;
		end
		UserTypeContent = content;
		local lines = 0;
		local width = 20;
		
		local str = GameMaths:stringAutoReturnForLua(content,width,lines);
		contentLabel:setString(tostring(str));
	end
	--end
	Flag_Chat = 1
	container:removeLibOS()
end

function BattleMainPageHelper:onSendMsg(container)
	if UserInfo.stateInfo.chatClose>0  then
		MessageBoxPage:Msg_Box_Lan("@AlreadyCloseChat")
		return;
	end
	
	if UserTypeContent~=nil and common:trim(UserTypeContent)~="" then
		UserTypeContent = RestrictedWord:getInstance():filterWordSentence(UserTypeContent);
		UserTypeContent = UserTypeContent:gsub("&","*")
		UserTypeContent = UserTypeContent:gsub("#","*")
		UserTypeContent = UserTypeContent:gsub("<","*")
		UserTypeContent = UserTypeContent:gsub(">","*")
		
		if currentChanel == Const_pb.CHAT_PERSONAL then
			local Friend_pb = require("Friend_pb")
			local message = Friend_pb.HPSendMessage()
			if message~=nil then
				message.targetId = ChatManager.curChatPerson.chatUnit.playerId
				message.message = UserTypeContent
				common:sendPacket(HP_pb.MESSAGE_SEND_C, message,false);
				
				--手动插入msgBox 玩家输入的文字
				local friendMsg = Friend_pb.FriendMsg()
				friendMsg.senderId = UserInfo.playerInfo.playerId
				friendMsg.receiveId = ChatManager.curChatPerson.chatUnit.playerId
				friendMsg.senderName = UserInfo.roleInfo.name
				friendMsg.receiveName = ChatManager.curChatPerson.chatUnit.name
				friendMsg.msgType = Const_pb.PLAYER_MSG
				friendMsg.message = UserTypeContent
				friendMsg.titleId = titleManager.myNowTitleId
				ChatManager.insertPrivateMsg(ChatManager.curChatPerson.chatUnit.playerId,nil,friendMsg,false,false)
				PageManager.refreshPage("BattlePage","PrivateChat")
			end
		else
			local message = Chat_pb.HPSendChat();
			if message~=nil then
				CCLuaLog("UserTypeContent"..UserTypeContent.."currentChanel"..currentChanel)
				
				message.chatMsg = UserTypeContent;
				message.chatType = 	currentChanel;
				
				mSurplusSecTime = GameConfig.Default.ChatCoolTime
				mDurationTime = 0.0
				
				local pb_data = message:SerializeToString();
				PacketManager:getInstance():sendPakcet(HP_pb.SEND_CHAT_C,pb_data,#pb_data,false);
				
				
			end
		end
		
		local contentLabel = container:getVarLabelTTF("mChatLabe");
		local label = Language:getInstance():getString("@ChatLabe")
		contentLabel:setString(label)
		UserTypeContent = "";
	else
		MessageBoxPage:Msg_Box_Lan("@PleaseEnterWordFirst")
	end
end

function BattleMainPageHelper:refreshChatMenu(container)
	local channelBtn = container:getVarMenuItemImage("mGuildChannelBtn")
	--如果在军团中，设置为true
	if AllianceOpen then
		channelBtn:setEnabled(true)
	else
		--默认都打开，只是提示不是工会成员
		channelBtn:setEnabled(true)
	end
end


function BattleMainPageHelper:refreshChatPageLabels(container)
	--local channelLabel = container:getVarLabelBMFont("mChatChannel")
	local BattleName = container:getVarLabelTTF("mBattleStatisticsName")
	local LeagueStr = Language:getInstance():getString("@LeagueChat")
	local worldStr = Language:getInstance():getString("@WorldChat")
	
	
	if currentPageIndex == BattlePageEnum.PAGE_BATTLE then
		--显示当前挂机的地图
		local mapCfg = ConfigManager:getMapCfg()
		UserInfo.syncStateInfo()
		currentMapId = UserInfo.stateInfo.curBattleMap
		local mapName = mapCfg[currentMapId]["name"]
		BattleName:setString(mapName);
	else
		if currentChanel == Const_pb.CHAT_WORLD then
			BattleName:setString(worldStr)
		elseif currentChanel == Const_pb.CHAT_ALLIANCE  then
			BattleName:setString(LeagueStr)
		elseif currentChanel == Const_pb.CHAT_PERSONAL  then
			if ChatManager.curChatPerson~=nil and ChatManager.curChatPerson.chatUnit ~= nil then
				BattleName:setString(ChatManager.curChatPerson.chatUnit.name )
			end
		end
	end
	
end


function BattleMainPageHelper:onPersonalChannel(container)
	--1. 如果当前在chat_personal页签
	if currentChanel == Const_pb.CHAT_PERSONAL then	
		local flag,playerId = ChatManager.hasNewMsgInBox()
		if flag then
			--1.1 如果有新的消息过来,跳转到新消息页面
			ChatManager.setCurrentChatPerson(playerId)
			ChatManager.readMsg(playerId)
			BattleMainPageHelper:showPrivatePage(container)
		else
			--1.2 如果没有新消息,直接弹出消息盒子
			BattleMainPageHelper:onChatHistory(false);
		end
	else
		--2. 如果在别的页签中
		local flag,playerId = ChatManager.hasNewMsgInBox()
		if flag then
			--2.1 如果有信息
			ChatManager.setCurrentChatPerson(playerId)
			ChatManager.readMsg(playerId)
			if ChatManager.isOfflineMsg(playerId) then
				--2.1.1 消息是离线的，需要请求
				local Friend_pb = require("Friend_pb")
				local msg = Friend_pb.HPMsgList();
				msg.playerId = playerId			
				common:sendPacket(HP_pb.MESSAGE_LIST_C, msg);
			else
				--2.1.2 消息是在线的，直接进入私聊页面
				BattleMainPageHelper:showPrivatePage(container)
			end				
		else
			--2.2 如果没有新消息
			if ChatManager.getMsgBoxSize() == 0 then
				--2.2.1 如果当前消息盒子是空，弹出最近聊天页面
				BattleMainPageHelper:onChatHistory(true);
			else
				--2.2.2 如果当前消息盒子不为空
				if ChatManager.getCurrentChatId() > 0 then
					--2.2.2.1 如果有已经聊天的对象，切换到私聊页面
					ChatManager.readMsg(ChatManager.getCurrentChatId())
					BattleMainPageHelper:showPrivatePage(container)
				else
					--2.2.2.2 如果没有有已经聊天的对象，弹出消息盒子
					BattleMainPageHelper:onChatHistory(false);
				end					
			end
		end
	end
	
	
end

function BattleMainPageHelper:onGuildChannel(container)
	if currentChanel == Const_pb.CHAT_ALLIANCE then
		return 
	end
	if AllianceOpen then
		currentChanel = Const_pb.CHAT_ALLIANCE
	else
		MessageBoxPage:Msg_Box_Lan("@NotInAllianceMember")
		return
	end
	hasNewMemberChatComing = false
	local channelChatPointNode = container:getVarNode("mGuildChannelPointNode")
	channelChatPointNode:setVisible(false)
	--刷新页面
	BattleMainPageHelper:refreshChatPage(container)
end

function BattleMainPageHelper:onWorldChannel(container)
	if currentChanel == Const_pb.CHAT_WORLD then
		return 
	end
	currentChanel = Const_pb.CHAT_WORLD
	hasNewWorldChatComing = false
	local channelChatPointNode = container:getVarNode("mWorldChannelPointNode")
	channelChatPointNode:setVisible(false)
	--刷新页面
	BattleMainPageHelper:refreshChatPage(container)
end

function BattleMainPageHelper:switchChatChanel(container)
	--切换世界聊天或者工会聊天
	if currentChanel == Const_pb.CHAT_WORLD then
		if AllianceOpen then
			currentChanel = Const_pb.CHAT_ALLIANCE
		else
			MessageBoxPage:Msg_Box_Lan("@NotInAllianceMember")
			return
		end
	else
		currentChanel = Const_pb.CHAT_WORLD
	end
	
end

function BattleMainPageHelper:onBattleSetUp(container)
	PageManager.pushPage("BattleSettingPage");
end

function BattleMainPageHelper:refreshWorldChatPage(container)
	container.scrollViewRootNode:removeAllChildren();
	--local PackageLogicForLua = require("PackageLogicForLua")
	local totalMsgSize = #worldChatList
	if totalMsgSize==0 then
		return
	end
	
	local currentPos = 0
	local chatWriteType = VaribleManager:getInstance():getSetting("chatWriteType")
	if chatWriteType == "0" then
		if FreeTypeConfig~=nil and FreeTypeConfig[4]~=nil and FreeTypeConfig[4].content ~= nil then
		    for i = totalMsgSize,1,-1 do
			local oneMSG =   worldChatList[i]
			if oneMSG~=nil then
				--local htmlTest = ""
				oneMSG.chatMsg = common:checkStringLegal( oneMSG.chatMsg )
				local nameStr = common:checkStringLegal( oneMSG.name )
				if oneMSG:HasField("area") then
					nameStr = nameStr .. "[" .. oneMSG.area .. "]";
				end
				local ttfLabelName = CCLabelTTF:create("","Helvetica",32)
				local ttfLabelContent = CCLabelTTF:create("","Helvetica",32)
				if oneMSG.type == Const_pb.CHAT_BROADCAST then
					ttfLabelName:setHorizontalAlignment(kCCTextAlignmentLeft)
					ttfLabelName:setAnchorPoint(ccp(0,0))
					ttfLabelName:setPosition(ccp(0, currentPos))
					ttfLabelName:setString(common:getLanguageString("@System"))
					local sysColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.system)
					ttfLabelName:setColor(sysColor)
					
					ttfLabelContent:setHorizontalAlignment(kCCTextAlignmentLeft)
					ttfLabelContent:setAnchorPoint(ccp(0,0))
					ttfLabelContent:setPosition(ccp(ttfLabelContent:getContentSize().width, currentPos))
					ttfLabelContent:setString(oneMSG.chatMsg)
					local contentColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.systemContent)
					ttfLabelContent:setColor(contentColor)
					--htmlTest = FreeTypeConfig[3].content;
					--htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v1#",oneMSG.chatMsg)
				else
					ttfLabelName:setHorizontalAlignment(kCCTextAlignmentLeft)
					ttfLabelName:setAnchorPoint(ccp(0,0))
					ttfLabelName:setPosition(ccp(0, currentPos))
					ttfLabelName:setString(nameStr .. ":")
					local nameColor 
					if common:checkPlayerSexByItemId(oneMSG.roleItemId) == true then
						nameColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.nameMan)
					else
						nameColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.nameWoman)
					end
					
					ttfLabelName:setColor(nameColor)
					
					ttfLabelContent:setHorizontalAlignment(kCCTextAlignmentLeft)
					ttfLabelContent:setAnchorPoint(ccp(0,0))
					ttfLabelContent:setPosition(ccp(ttfLabelContent:getContentSize().width, currentPos))
					ttfLabelContent:setString(oneMSG.chatMsg)
					local contentColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.content)
					ttfLabelContent:setColor(contentColor)
					--htmlTest = FreeTypeConfig[4].content;
					--htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v1#",oneMSG.name)
					--htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v2#",oneMSG.chatMsg)
				end
				--local htmlText0 = CCHTMLLabel:createWithString(htmlTest,CCSize(570,200));
				--htmlText0:setPosition(ccp(0,currentPos));
				currentPos = currentPos + ttfLabelContent:getContentSize().height;
				container.scrollViewRootNode:addChild(ttfLabelName)
				container.scrollViewRootNode:addChild(ttfLabelContent)
			end
			end
		end
	else			
		if FreeTypeConfig~=nil and FreeTypeConfig[4]~=nil and FreeTypeConfig[4].content ~= nil then
		for i = totalMsgSize,1,-1 do
			local oneMSG =   worldChatList[i]
			if oneMSG~=nil then			
				local htmlTest = ""
				oneMSG.chatMsg = common:checkStringLegal( oneMSG.chatMsg )
				local nameStr = common:checkStringLegal( oneMSG.name )
				
				if oneMSG.type == Const_pb.CHAT_BROADCAST then
					htmlTest = FreeTypeConfig[3].content;						
					htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v1#",oneMSG.chatMsg)	
				else
					if oneMSG:HasField("playerType") and oneMSG.playerType == 1 then
						htmlTest = FreeTypeConfig[99].content;
					else
						if common:checkPlayerSexByItemId( oneMSG.roleItemId ) == true then
							if titleManager:getTittlePic(oneMSG.titleId) ~= "" then
							    htmlTest = FreeTypeConfig[4].content
						    else
						    	htmlTest = FreeTypeConfig[301].content
						    end
						else
							if titleManager:getTittlePic(oneMSG.titleId) ~= "" then
							    htmlTest = FreeTypeConfig[9].content;
							else
								htmlTest = FreeTypeConfig[302].content
							end
						end
					end
					if titleManager:getTittlePic(oneMSG.titleId) ~= "" then						
					    htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v3#",titleManager:getTittlePic(oneMSG.titleId))
					end
				    if oneMSG:HasField("area") then
						htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v4#",'['..oneMSG.area..']')
					else
						htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v4#","")
					end
					htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v1#",nameStr)
					htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v2#",oneMSG.chatMsg)	
				end						
						
				local htmlText0 = CCHTMLLabel:createWithString(htmlTest,CCSize(570,200));
				--[[	htmlText:registerLuaClickListener(function ()
						MessageBoxPage:Msg_Box("@LevelForArena")
					end)--]]
				htmlText0:setPosition(ccp(0,currentPos));
				currentPos = currentPos + htmlText0:getContentSize().height;
				container.scrollViewRootNode:addChild(htmlText0)
			end
		end
	end
		local viewHeight = container.scrollview:getViewSize().height
		container.scrollview:setContentSize(CCSize(640,currentPos))	;
		if chatPageInMove then
			return
		end
		if currentPos <viewHeight then
			container.scrollview:setContentOffset(ccp(0,viewHeight-currentPos));
		else
			container.scrollview:setContentOffset(ccp(0,0));
		end
		
	end
end

function BattleMainPageHelper:refreshLeagueChatPage(container)
	container.scrollViewRootNode:removeAllChildren();
	--local PackageLogicForLua = require("PackageLogicForLua")
	local totalMsgs = #memberChatList
	if totalMsgs ==0 then
		return
	end
	local currentPos = 0
	local chatWriteType = VaribleManager:getInstance():getSetting("chatWriteType")
	if chatWriteType == "0" then
		if FreeTypeConfig~=nil and FreeTypeConfig[4]~=nil and FreeTypeConfig[4].content ~= nil then
		    for i = totalMsgs,1,-1 do
			local oneMSG =   memberChatList[i]
			if oneMSG~=nil then
				--local htmlTest = ""
				oneMSG.chatMsg = common:checkStringLegal( oneMSG.chatMsg )
				local nameStr = common:checkStringLegal( oneMSG.name )
				if oneMSG:HasField("area") then
					nameStr = nameStr .. "[" .. oneMSG.area .. "]";
				end
				local ttfLabelName = CCLabelTTF:create("","Helvetica",32)
				local ttfLabelContent = CCLabelTTF:create("","Helvetica",32)
				if oneMSG.type == Const_pb.CHAT_BROADCAST then
					ttfLabelName:setHorizontalAlignment(kCCTextAlignmentLeft)
					ttfLabelName:setAnchorPoint(ccp(0,0))
					ttfLabelName:setPosition(ccp(0, currentPos))
					ttfLabelName:setString(common:getLanguageString("@System"))
					local sysColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.system)
					ttfLabelName:setColor(sysColor)
					
					ttfLabelContent:setHorizontalAlignment(kCCTextAlignmentLeft)
					ttfLabelContent:setAnchorPoint(ccp(0,0))
					ttfLabelContent:setPosition(ccp(ttfLabelContent:getContentSize().width, currentPos))
					ttfLabelContent:setString(oneMSG.chatMsg)
					local contentColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.systemContent)
					ttfLabelContent:setColor(contentColor)
					--htmlTest = FreeTypeConfig[3].content;
					--htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v1#",oneMSG.chatMsg)
				else
					ttfLabelName:setHorizontalAlignment(kCCTextAlignmentLeft)
					ttfLabelName:setAnchorPoint(ccp(0,0))
					ttfLabelName:setPosition(ccp(0, currentPos))
					ttfLabelName:setString(nameStr .. ":")
					local nameColor 
					if common:checkPlayerSexByItemId(oneMSG.roleItemId) == true then
						nameColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.nameMan)
					else
						nameColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.nameWoman)
					end
					
					ttfLabelName:setColor(nameColor)
					
					ttfLabelContent:setHorizontalAlignment(kCCTextAlignmentLeft)
					ttfLabelContent:setAnchorPoint(ccp(0,0))
					ttfLabelContent:setPosition(ccp(ttfLabelContent:getContentSize().width, currentPos))
					ttfLabelContent:setString(oneMSG.chatMsg)
					local contentColor = StringConverter:parseColor3B(GameConfig.BattleChatColor.content)
					ttfLabelContent:setColor(contentColor)
					--htmlTest = FreeTypeConfig[4].content;
					--htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v1#",oneMSG.name)
					--htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v2#",oneMSG.chatMsg)
				end
				--local htmlText0 = CCHTMLLabel:createWithString(htmlTest,CCSize(570,200));
				--htmlText0:setPosition(ccp(0,currentPos));
				currentPos = currentPos + ttfLabelContent:getContentSize().height;
				container.scrollViewRootNode:addChild(ttfLabelName)
				container.scrollViewRootNode:addChild(ttfLabelContent)
			end
			end
		end
	else			
		if FreeTypeConfig~=nil and FreeTypeConfig[4]~=nil and FreeTypeConfig[4].content ~= nil then
		for i = totalMsgs,1,-1 do
			local oneMSG =   memberChatList[i]
			if oneMSG~=nil then			
				local htmlTest = ""
				oneMSG.chatMsg = common:checkStringLegal( oneMSG.chatMsg )
				local nameStr = common:checkStringLegal( oneMSG.name )
				-- if oneMSG:HasField("area") then
				-- 	nameStr = nameStr .. "[" .. oneMSG.area .. "]";
				-- end
				if oneMSG.type == Const_pb.CHAT_BROADCAST then
					htmlTest = FreeTypeConfig[3].content;						
					htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v1#",oneMSG.chatMsg)	
				else
					if common:checkPlayerSexByItemId( oneMSG.roleItemId ) == true then
						if titleManager:getTittlePic(oneMSG.titleId) ~= "" then
						    htmlTest = FreeTypeConfig[4].content
					    else
					    	htmlTest = FreeTypeConfig[301].content
					    end
					else
						if titleManager:getTittlePic(oneMSG.titleId) ~= "" then
						    htmlTest = FreeTypeConfig[9].content;
						else
							htmlTest = FreeTypeConfig[302].content
						end
					end
					if titleManager:getTittlePic(oneMSG.titleId) ~= "" then	
					    htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v3#",titleManager:getTittlePic(oneMSG.titleId))
					end
				    if oneMSG:HasField("area") then
						htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v4#",'['..oneMSG.area..']')
					else
						htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v4#","")
					end
					htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v1#",nameStr)
					htmlTest = GameMaths:replaceStringWithCharacterAll(htmlTest,"#v2#",oneMSG.chatMsg)	
				end						
						
				local htmlText0 = CCHTMLLabel:createWithString(htmlTest,CCSize(570,200));
				--[[	htmlText:registerLuaClickListener(function ()
						MessageBoxPage:Msg_Box("@LevelForArena")
					end)--]]
				htmlText0:setPosition(ccp(0,currentPos));
				currentPos = currentPos + htmlText0:getContentSize().height;
				container.scrollViewRootNode:addChild(htmlText0)
			end
		end
	end
	--ttf
	
		
		
		local viewHeight = container.scrollview:getViewSize().height
		
		container.scrollview:setContentSize(CCSize(640,currentPos))	;
		
		if chatPageInMove then
			return
		end
		if currentPos <viewHeight then
			container.scrollview:setContentOffset(ccp(0,viewHeight-currentPos));
		else
			container.scrollview:setContentOffset(ccp(0,0));
		end
		
	end
end

function BattleMainPageHelper:refreshPrivateChatPage(container)
	container.scrollViewRootNode:removeAllChildren();
	if ChatManager.curChatPerson == nil or ChatManager.curChatPerson.msgList == nil then return end
	local totalMsgs = #ChatManager.curChatPerson.msgList
	if totalMsgs ==0 then	return 	end
	--标记为已读
	ChatManager.readMsg(ChatManager.curChatPerson.chatUnit.playerId)
	local currentPos = 0
	if FreeTypeConfig~=nil and FreeTypeConfig[4]~=nil and FreeTypeConfig[4].content ~= nil then
		UserInfo.sync()
		for i = totalMsgs,1,-1 do
			local oneMSG =   ChatManager.curChatPerson.msgList[i]
			if oneMSG~=nil then
				local htmlText = ""
				local roleItemId = 0
				if oneMSG.senderId == UserInfo.playerInfo.playerId then
					roleItemId = UserInfo.roleInfo.itemId
				else
					roleItemId = ChatManager.curChatPerson.chatUnit.roleItemId
				end
                 
				if common:checkPlayerSexByItemId( roleItemId ) == true then
					if titleManager:getTittlePic(oneMSG.titleId) ~= "" then
					    htmlText = FreeTypeConfig[4].content
				    else
				    	htmlText = FreeTypeConfig[301].content
				    end
				else
					if titleManager:getTittlePic(oneMSG.titleId) ~= "" then
					    htmlText = FreeTypeConfig[9].content;
					else
						htmlText = FreeTypeConfig[302].content
					end
				end

				local nameStr = oneMSG.senderName;
				-- if oneMSG:HasField("area") then
				-- 	nameStr = nameStr .. "[" .. oneMSG.area .. "]";
				-- end
				if oneMSG:HasField("area") then
					htmlText = GameMaths:replaceStringWithCharacterAll(htmlText,"#v4#",'['..oneMSG.area..']')
				else
					htmlText = GameMaths:replaceStringWithCharacterAll(htmlText,"#v4#","")
				end
				if titleManager:getTittlePic(oneMSG.titleId) ~= "" then
					htmlText = GameMaths:replaceStringWithCharacterAll(htmlText,"#v3#",titleManager:getTittlePic(oneMSG.titleId))
				end
				htmlText = GameMaths:replaceStringWithCharacterAll(htmlText,"#v1#", nameStr)
				htmlText = GameMaths:replaceStringWithCharacterAll(htmlText,"#v2#",oneMSG.message)
				local htmlText0 = CCHTMLLabel:createWithString(htmlText,CCSize(570,200));
				htmlText0:setPosition(ccp(0,currentPos));
				currentPos = currentPos + htmlText0:getContentSize().height;
				container.scrollViewRootNode:addChild(htmlText0)
			end
		end
		local viewHeight = container.scrollview:getViewSize().height
		
		container.scrollview:setContentSize(CCSize(640,currentPos))	;
		
		if chatPageInMove then
			return
		end
		if currentPos <viewHeight then
			container.scrollview:setContentOffset(ccp(0,viewHeight-currentPos));
		else
			container.scrollview:setContentOffset(ccp(0,0));
		end
		
	end
end

function BattleMainPageHelper:refreshRedPointInChat(container)
	--战斗页面中“世界聊天”的红点
	local chatPointNode = container:getVarNode("mChatPointNode")
	if (hasNewChatComing == true and inBattleOrChat == true) then
		chatPointNode:setVisible(true)
	else
		chatPointNode:setVisible(false)
	end
	
	
	--聊天页面中“新的工会/世界聊天”红点，只有在聊天页面中才显示
	local worldRedPoint = container:getVarNode("mWorldChannelPointNode")
	local guildRedPoint = container:getVarNode("mGuildChannelPointNode")
	local personRedPoint = container:getVarNode("mPersonalChannelPromptNode")	
	if inBattleOrChat == false then
		if currentChanel == Const_pb.CHAT_WORLD then
		    hasNewWorldChatComing = false
			worldRedPoint:setVisible(false)
		else			
			worldRedPoint:setVisible(hasNewWorldChatComing)
		end
		if currentChanel == Const_pb.CHAT_ALLIANCE or AllianceOpen == false then
		    hasNewMemberChatComing = false
			guildRedPoint:setVisible(false)
		else
			guildRedPoint:setVisible(hasNewMemberChatComing )
		end	
		
		if currentChanel == Const_pb.CHAT_PERSONAL then
			local flag,id = ChatManager.hasNewMsgInBox()
			personRedPoint:setVisible(flag)
		else
			local flag = ChatManager.hasNewMsgWithoutCur()
			personRedPoint:setVisible(flag)
		end
		
		
	end
	
	if currentPageIndex == BattlePageEnum.PAGE_CHAT and currentChanel == Const_pb.CHAT_PERSONAL then
		NodeHelper:setNodeVisible(container:getVarNode("mSeeNode"), true);
	else
		NodeHelper:setNodeVisible(container:getVarNode("mSeeNode"), false);
	end
	
end

function BattleMainPageHelper:refreshChatClose(container)
	--如果已经关闭聊天页面，做处理
	local contentLabel = container:getVarLabelTTF("mChatLabe");
	if contentLabel ~= nil then
		if UserInfo.stateInfo.chatClose>0 then
			local label = Language:getInstance():getString("@AlreadyCloseChat")
			contentLabel:setString(label)			
		else			
			local label = Language:getInstance():getString("@ChatLabe")
			contentLabel:setString(label)				
		
		end
	end
end


function BattleMainPageHelper:refreshChatPage(container)
		
	--只有在聊天页面的时候才刷新内容 add by zhenhui 2014/8/26
	if inBattleOrChat == false then
		if currentChanel == Const_pb.CHAT_WORLD then
			BattleMainPageHelper:refreshWorldChatPage(container)
		elseif currentChanel == Const_pb.CHAT_ALLIANCE then
			BattleMainPageHelper:refreshLeagueChatPage(container)
		elseif currentChanel == Const_pb.CHAT_PERSONAL then
			BattleMainPageHelper:refreshPrivateChatPage(container)
		end
	end
	
	--labels
	BattleMainPageHelper:refreshChatPageLabels(container)
	--刷新按钮状态
	BattleMainPageHelper:refreshChatMenu(container)
	--刷新下次战斗名称
	BattleMainPageHelper:refreshNextFightName(container)
	--刷新红点情况
	BattleMainPageHelper:refreshRedPointInChat(container)
end

function BattleMainPageHelper:refreshNextFightName(container)
	--如果下一关有地图
	local nextLabel = container:getVarLabelTTF("mBattleNextName")
	local htmlTag = 10010
	if nextChanllengeName~=nil then	
		RegisterLuaPage("BattleResultPopPage")
		BattleResultPopPage_setBossName(nextChanllengeName)
		nextLabel:setVisible(false)		
		local nextStr = common:fill(FightLogConfig["NextChanllengePerson"].content,nextChanllengeName)
		NodeHelper:addHtmlLable(nextLabel, nextStr, htmlTag,CCSizeMake(600,100));
		--nextLabel:setString(nextStr)
		local BattleFightSceneHelper  = require("BattleFightSceneHelper")
		if nextFightIsBoss then
			BattleFightSceneHelper:ShowNextFightBossAni(container)
		else
			BattleFightSceneHelper:ShowNextFightArenaAni(container)
		end
		
	else
		nextLabel:setVisible(false)
		nextLabel:getParent():removeChildByTag(htmlTag,true)
	end
end

function BattleMainPageHelper:onExecute(container)
	if mSurplusSecTime > 0 then
		mDurationTime = mDurationTime + GamePrecedure:getInstance():getFrameTime();
		if mDurationTime > 1.0 then
			mDurationTime = mDurationTime - 1.0
			mSurplusSecTime = mSurplusSecTime - 1
			BattleMainPageHelper:refreshSendBtnStatus( container )
		end
	end
end

function BattleMainPageHelper:refreshSendBtnStatus( container )
	--Language:getInstance:getString("@MsgSend") .. " " ..
	if mSurplusSecTime > 0 then
		local _str =  mSurplusSecTime .. "s"
		NodeHelper:setStringForTTFLabel(container, { mChatLabe = _str })
		container:getVarMenuItemImage("mMsgSend"):setEnabled(false)
	else
		local _str = Language:getInstance():getString("@ChatLabe")--ChatLabe
		NodeHelper:setStringForTTFLabel(container, { mChatLabe = _str })
		container:getVarMenuItemImage("mMsgSend"):setEnabled(true)
	end
	
end

--聊天页面滑动事件处理
function BattleMainPageHelper.onTouchHistoryLayerHandler(eventName,pTouch )
	if eventName == "began" then
		local point = pTouch:getLocationInView();
		point = CCDirector:sharedDirector():convertToGL(point);
		m_BegainY = point.y;
	elseif eventName == "moved" then
		chatPageInMove = true
	elseif eventName == "ended" then
		chatPageInMove = false
		local point = pTouch:getLocationInView();
		point = CCDirector:sharedDirector():convertToGL(point);
		m_EndY = point.y;
		if m_EndY>BattleChatManager.chatMaxHeight or m_EndY<BattleChatManager.chatMinHeight then
			return
		end
		local dis = math.abs(m_EndY-m_BegainY)
		if dis<50 then
			if currentChanel == Const_pb.CHAT_PERSONAL then
				flag = false
			else
				flag = true
			end
			BattleMainPageHelper:onChatHistory( flag)
		end
	elseif eventName == "cancelled" then
		
	end
end

function BattleMainPageHelper:onChatHistory( isHistory)
	if currentPageIndex ==BattlePageEnum.PAGE_BATTLE  then
		return
	end
	registerScriptPage("BattleChatHistoryPage")
	BattleChatHistoryPage_setTabIsHistory(isHistory)
	PageManager.pushPage("BattleChatHistoryPage")
	
end

function BattleMainPageHelper:resetData(  )
	lastBattleStatic = nil
	nextChanllengeName = nil
	--local PackageLogicForLua = require("PackageLogicForLua")
	memberChatList = {}
	worldChatList = {}
	worldBroadCastList = {}
	Flag_Chat = 0
end

return BattleMainPageHelper;

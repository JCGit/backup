--require "MainSceneOperation_pb"
--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			MainScenePage.lua
--  DESCRIPTION:	MainScenePage page 
--
--
--  AUTHOR:		Zhen Hui
--  CREATED:	2014/7/22
--  MODIFY2:
--
--  MODIFY1:
--
--------------------------------------------------------------------------------
local HP_pb = require("HP_pb");
local Reward_pb = require "Reward_pb"
require "bit"
local UserInfo = require("UserInfo");
local NodeHelper = require("NodeHelper");

local MainScenePageInfo = {}       -- all ui information

local MainSceneInfo = {} -- server data info in the memory

local MainSceneItemContent = {} -- content ui

FreeTypeConfig = {}

local broadCastTime = 0
local curFriendBtnTag = GameConfig.Tag.FriendBtn;

local m_hasClickedHelp = false
local m_isFistEnter = true;
local m_hasClickedFetchGift = false

local opcodes=
{
	PLAYER_INFO_SYNC_S 	= HP_pb.PLAYER_INFO_SYNC_S,
	ROLE_INFO_SYNC_S	= HP_pb.ROLE_INFO_SYNC_S
}


local battleAward = {}
MainScenePageInfo.loginDay = 0 -- 开服活动到达第几天

NoticePointState = 
{
	isChange = true,
	MAIL_POINT_OPEN = false,
	MESSAGE_POINT_OPEN = false,
	ALLIANCE_BOSS_OPEN = false,
	GIFT_NEW_MSG = false,
	ACTIVITY_POINT = false,
	REGINMENTWAR_POINT = false,
	TITLE_CHANGE_MSG = false,
}



function RESETINFO_NOTICE_STATE()
	for key, value in pairs(NoticePointState) do
		NoticePointState[key] = false;
	end
	NoticePointState.isChange = true;
end

function luaCreat_MainScenePage(container)
	CCLuaLog("Z#:luaCreate_MainScenePage!");
	container:registerFunctionHandler(MainScenePageInfo.onFunction);
end

function MainScenePageInfo.onFunction(eventName,container)
	
	if eventName == "luaInit" then
		MainScenePageInfo.onInit(container);
	elseif eventName == "luaLoad" then
		MainScenePageInfo.onLoad(container);
	elseif eventName == "luaEnter" then
		MainScenePageInfo.onEnter(container);
	elseif eventName == "luaExecute" then
		MainScenePageInfo.onExecute(container);
	elseif eventName == "luaExit" then
		MainScenePageInfo.onExit(container);
	elseif eventName == "luaUnLoad" then
		MainScenePageInfo.onUnLoad(container);
	elseif eventName == "luaReceivePacket" then
		MainScenePageInfo.onReceivePacket(container);
	elseif eventName == "luaGameMessage" then
		MainScenePageInfo.onGameMessage(container);
	elseif eventName == "onExplain" then
		MainScenePageInfo.onExplanationButton(container);
	elseif eventName == "onViewReward" then
		MainScenePageInfo.onViewReward(container);
	elseif eventName == "onRefresh" then
		MainScenePageInfo.onRefreshStarButton(container);
	elseif string.find(eventName, "onFace") ~= nil then
		local index = tonumber(string.sub(eventName, -1))
		MainScenePageInfo.onItemButton(container,index)
	elseif eventName == "onContainer" then
		PageManager.changePage("MainSceneContainer");
	elseif eventName == "onExtraction" then
		MainScenePageInfo.getMainScene(container,1)
	elseif eventName == "onExtractionTimes" then
		--MainScenePageInfo.getMainScene(container,10)
		MainScenePageInfo.onFlyToPos(container)
	elseif eventName == "onExchangeFragment" then
		MainScenePageInfo.toExchangePage(container)
	elseif eventName == "onMainArena" then		
		if UserInfo.roleInfo.fight < 1000 then
			MessageBoxPage:Msg_Box_Lan("@ArenaLimit")
			return
		end			
		--登陆比武界面
		PageManager.changePage("ArenaPage");

	elseif eventName == "onMainGift" then
		PageManager.pushPage("GiftPage");
	elseif eventName == "onMainFair" then
		PageManager.changePage("MarketPage");
	elseif eventName == "onMainRefining" then
		PageManager.pushPage("MeltPage");
	elseif eventName == "onManyPeopleArena" then
		PageManager.changePage("PVPActivityPage");
	elseif eventName == "onMainGuild" then
		MainScenePageInfo.goToAlliancePage(container)
	elseif eventName == "onAnnoucement" then
		--GamePrecedure:getInstance():showBulletin();
		PageManager.pushPage("AnnouncePage", true);
		--local str = Language:getInstance():getString("@Announcement")
		--PageManager.showHelp( GameConfig.HelpKey.ANNOUNCEMENT ,str , true )
	--elseif eventName == "onOpinion" then
		--MainScenePageInfo.onFeedback(container);
	elseif eventName == "onActivities" then	
		PageManager.changePage("ActivityPage");
	elseif eventName == "onRecharge" then		
		PageManager.pushPage("RechargePage");
	elseif eventName == "onHelp" then		
		m_hasClickedHelp = true
		PageManager.showHelp(GameConfig.HelpKey.HELP_MAIN)
	elseif eventName == "onMail" then
		PageManager.pushPage("MailPage");
	elseif eventName == "onFriend" then
		PageManager.changePage("FriendPage");
		MainScenePageInfo.switchFriendBtnImg()
	elseif eventName == "onChat" then
		--进入到世界聊天页面
		PageManager.showFightPage();
		PageManager.refreshPage("BattlePage","WorldChat")		
	elseif eventName == "onPackageBtn" then
		MainScenePageInfo.fetchGift(container)
	elseif eventName == "onPersonalConfidence" or eventName == "onMyInfo" then
		PageManager.pushPage("PlayerInfoPage");
	elseif eventName == "onMessageChat" then
		if BlackBoard:getInstance():hasVarible( GameConfig.Default.NewMsgKey ) then
			BlackBoard:getInstance():setVarible( GameConfig.Default.NewMsgKey , false)
		else
			BlackBoard:getInstance():addVarible( GameConfig.Default.NewMsgKey , true )
		end
	    PageManager.pushPage("LeaveMessagePage")
	elseif eventName == "onFightingRanking" then
		PageManager.changePage("ProfessionRankingPage")	
	elseif eventName == "onActivityDailyLogin" then
		PageManager.changePage("DailyLogin")
	end
end

function MainScenePageInfo:setLoginDay( loginDay )
	if loginDay>0 and loginDay<9 then
		MainScenePageInfo.loginDay = loginDay
	else
		MainScenePageInfo.loginDay = 0
	end
end

function MainScenePageInfo.broadcastMessage(container)
	
	local dt = GamePrecedure:getInstance():getFrameTime() * 1000;
	broadCastTime = broadCastTime + dt;
	if broadCastTime>GameConfig.BroadcastLastTime then
		isInAnimation = false
		local castNode = container:getVarNode("mNoticeNode")
		castNode:removeAllChildren()
	end
	--local PackageLogicForLua = require("PackageLogicForLua")
	local size = #worldBroadCastList
	if size>0 then	
		if  isInAnimation == false then
			--get the first msg and remove
			local oneMsg = table.remove(worldBroadCastList,1)
			local castNode = container:getVarNode("mNoticeNode")
			if castNode~=nil then
				castNode:setVisible(true)		
				castNode:removeAllChildren()		
				local castCCB = ScriptContentBase:create("NoticeItem.ccbi");				
				local msgLabel = castCCB:getVarLabelTTF("mNoticeTex")
				msgLabel:setString(oneMsg.chatMsg); 
				castNode:addChild(castCCB)
				castCCB:release();
				castCCB:runAnimation("Notice");		
				isInAnimation = true				
			end
			broadCastTime = 0
		end
	end
end


function MainScenePageInfo.showNewBeeHint(container)	
	if UserInfo.hasFastFight() and UserInfo.hasPassGiftEquip() and UserInfo.guideInfo.Help and m_hasClickedHelp == false then 
		NodeHelper:setNodeVisible(container:getVarNode("mNewbeeHintNode"), true)	
	else
		NodeHelper:setNodeVisible(container:getVarNode("mNewbeeHintNode"), false)	
	end
end

function MainScenePageInfo.runWelcomeAni(container)
	local welAniNode = container:getVarNode("mWelcomeNode")
	if welAniNode~=nil then
		local welAniCCB = ScriptContentBase:create("WelcomeItem.ccbi")
		welAniNode:removeAllChildren()
		welAniNode:addChild(welAniCCB)
		welAniCCB:release();
		welAniCCB:runAnimation("Welcome");
	end
end


--领取奖励
function MainScenePageInfo.fetchGift(container)
	local message = Reward_pb.HPFetchGift();
	if message~=nil then	
		message.giftId = 1;		
		local pb_data = message:SerializeToString();
		PacketManager:getInstance():sendPakcet(HP_pb.FETCH_GIFT_C,pb_data,#pb_data,false);	
		
		m_hasClickedFetchGift = true
		--暂时注释掉播欢迎动画
		--MainScenePageInfo.runWelcomeAni(container)
		local fetchNode = container:getVarNode("mPackageBtnNode")
		fetchNode:setVisible(false)
	end
end

function MainScenePageInfo.refreshFetchGiftBtn(container)
	local needGuide = false;
	if UserInfo.stateInfo.giftStatus ~=nil then
		local fetchNode = container:getVarNode("mPackageBtnNode")
		local giftStatus = UserInfo.stateInfo.giftStatus;
		if giftStatus<0 then
			fetchNode:setVisible(false)
			UserInfo.guideInfo.Gift = false;
			UserInfo.guideInfo.Equip = true;
			return
		end
		
		local midFlag = bit:blshift(1,1)
		local flag = bit:band(giftStatus,midFlag)
		needGuide = flag <= 0;
	
		fetchNode:setVisible(needGuide)	
	end
	NodeHelper:setNodesVisible(container, {mNewbeeHintNode2 = needGuide});
	UserInfo.guideInfo.Gift = needGuide;
	if needGuide == false then
		UserInfo.guideInfo.Equip = true;
	end
end

function MainScenePageInfo.goToAlliancePage(container)
	UserInfo.sync()
	if UserInfo.roleInfo and UserInfo.roleInfo.level >= GameConfig.ALLIANCE_OPEN_LEVEL then
		PageManager.changePage("GuildPage")
	else
		MessageBoxPage:Msg_Box(common:getLanguageString('@AllianceLevelNotReached', GameConfig.ALLIANCE_OPEN_LEVEL))
	end
end

function MainScenePageInfo.onInit(container)
	CCLuaLog("Z#:MainScenePageInfo.onInit!");	
	if FreeTypeConfig == nil or table.maxn(FreeTypeConfig)<=0 then
		local tabel = TableReaderManager:getInstance():getTableReader("FreeTypeFont.txt")
		local count = tabel:getLineCount()-1;
		for i = 1,count do
			local index = tonumber(tabel:getData(i,0))
			if FreeTypeConfig[index] == nil then
				FreeTypeConfig[index] = {}
				FreeTypeConfig[index].id = tonumber(tabel:getData(i, 0))
				FreeTypeConfig[index].content = tabel:getData(i, 1)
			end
		end
	end

end

function MainScenePageInfo.onLoad(container)
	
	CCLuaLog("Z#:MainScenePageInfo.onLoad!");
	--进入页面时，重置当前状态	
	local height = CCDirector:sharedDirector():getWinSize().height
	if height<900 then
		container:loadCcbiFile("MainScene_ipad.ccbi",true);
	else
		container:loadCcbiFile("MainScene.ccbi",true);
	end
	
	if m_isFistEnter then
		PageManager.refreshRedNotice();
		m_isFistEnter = false;
	end
end


function MainScenePageInfo.onEnter(container)
	MainScenePageInfo.showFriendBtnImg(container);
	
	container:registerPacket(HP_pb.PLAYER_AWARD_S)
	
	MainScenePageInfo.refreshPage(container);
	container:registerMessage(MSG_SEVERINFO_UPDATE);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
	MainScenePageInfo.setNoticeNodes( container )	
	--新手指引，暂时只是显示一个NODE节点的
	broadCastTime = 0	
	isInAnimation = false
	--广播消息
	MainScenePageInfo.broadcastMessage(container)		
	
end


function MainScenePageInfo.onExecute(container)
	if NoticePointState.isChange then
		MainScenePageInfo.setNoticeNodes( container )
		NoticePointState.isChange = false;
	end
	
	--控制广播的时间	
	MainScenePageInfo.broadcastMessage(container)			
	
	MainScenePageInfo.showNewBeeHint(container)
	
	MainScenePageInfo.showChatHint(container)
end

function MainScenePageInfo.showChatHint(container)
	local chatPoint = container:getVarNode("mChatPoint")
	if chatPoint~=nil then
		local ChatManager = require("ChatManager")
		if (ChatManager.hasNewMsgWithoutCur() == true) or hasNewMemberChatComing == true then 
			chatPoint:setVisible(true)
		else
			chatPoint:setVisible(false)
		end
	end
end

function MainScenePageInfo.onExit(container)
	CCLuaLog("Z#:MainScenePageInfo.onExit!");
	--MainScenePageInfo.removeAllPacket(container);	
	container:removePacket(HP_pb.PLAYER_AWARD_S)
	container:removeMessage(MSG_SEVERINFO_UPDATE);
	container:removeMessage(MSG_MAINFRAME_REFRESH);
	--container:removeMessage(MSG_GET_NEW_INFO);
	isInAnimation = false
	broadCastTime =0
end

function MainScenePageInfo.onUnLoad(container)
	CCLuaLog("Z#:MainScenePageInfo.onUnLoad!");
end


function MainScenePageInfo.onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()


	if opcode == HP_pb.PLAYER_AWARD_S  then
		local Reward_pb = require("Reward_pb")
		local msg = Reward_pb.HPPlayerReward();
		msg:ParseFromString(msgBuff)	
		if msg:HasField("rewards") then		
			local rewards = msg.rewards.showItems
			-- 专门为领取奖励做特殊处理，收到了五个数量的物品，播放动画 by zhenhui
			if #rewards == 5 and m_hasClickedFetchGift  == true then
				--先播PackageAni，在1.3秒的时候，再显示金钱增加
				container:runAnimation("PackageAni")
				local array = CCArray:create();				
				array:addObject(CCDelayTime:create(2));				
				local functionAction = CCCallFunc:create(function ()
					MainScenePageInfo.refreshPage(container);				
				end)
				array:addObject(functionAction);
				local seq = CCSequence:create(array);				
				container:getVarNode("mLVAniNode"):runAction(seq)
				m_hasClickedFetchGift = false				
			else
				MainScenePageInfo.refreshPage(container);			
			end	
		end
	end		
end


function MainScenePageInfo.setAddAwardData(coin,exp)
	battleAward.coin = coin
	battleAward.exp = exp
end

function MainScenePageInfo.runAddCoinExpAni(container,coin,exp)
	local coinAniNode = container:getVarNode("mLVAniNode")
	if coinAniNode ~=nil and coin>0 and exp >0 then
		local addAniCCB = ScriptContentBase:create("A_LvText")
		
		coinAniNode:removeAllChildren()
		coinAniNode:addChild(addAniCCB)
		addAniCCB:release();
		NodeHelper:setStringForLabel(
			addAniCCB,{
				mExp = common:getLanguageString("@ExpAddAmont",tostring(exp)),
				mCoin = common:getLanguageString("@CoinAddAmont",tostring(coin))
			}
		)			
		addAniCCB:runAnimation("Default Timeline");
	end
end

function MainScenePageInfo.onGameMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_SEVERINFO_UPDATE then
		--[[local opcode = MsgSeverInfoUpdate:getTrueType(message).opcode;
		if common:table_hasValue(opcodes, opcode) then
			MainScenePageInfo.refreshPage(container);
		end--]]
	elseif typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		local extraParam = MsgMainFrameRefreshPage:getTrueType(message).extraParam
		if pageName == "MainScenePage" then
			if extraParam == "runAddCoinAni" then
				MainScenePageInfo.runAddCoinExpAni(container,battleAward.coin ,battleAward.exp)
			end
			if extraParam == "assembly" then
				local coinAniNode = container:getVarNode("mLVAniNode")
				if coinAniNode ~=nil then		
					coinAniNode:removeAllChildren()
				end
			end
			MainScenePageInfo.refreshPage(container);
		end
	end
end

function MainScenePageInfo.setNoticeNodes( container )
	local PVPActivityPage = require("PVPActivityPage")
	local pvpActivityFlag = PVPActivityPage.isShowPVPActivityRedPoint()
	NodeHelper:setNodesVisible(container, 
	{
		mMailPoint = NoticePointState.MAIL_POINT_OPEN,
		mGiftPoint = NoticePointState.GIFT_NEW_MSG,
		mGuildPoint = NoticePointState.ALLIANCE_BOSS_OPEN,
		mActivityPoint = NoticePointState.ACTIVITY_POINT,
		mRegimentWarPoint = NoticePointState.REGINMENTWAR_POINT,
		mMyInfoPoint = NoticePointState.TITLE_CHANGE_MSG,
		mManyPeopleArenaPoint = pvpActivityFlag
			}
		);
end

function MainScenePageInfo.requireUIInformationFromServer(container)
	CCLuaLog("Z#:requireUIInformationFromServer!");	
	local message = UserMainScene_pb.OPMainSceneInfo();	
	if message ~=nil then	
		message.version = 1;
		message.isOnlyUpdate = true
		local pb_data = message:SerializeToString();
		PacketManager:getInstance():sendPakcet(OPCODE_MainScene_INFO_C,pb_data,#pb_data,true);		
	end		
end

function MainScenePageInfo.refreshPage(container)
	MainScenePageInfo.showPlayerInfo(container);	
	MainScenePageInfo.refreshFetchGiftBtn(container)
end

function MainScenePageInfo.showPlayerInfo(container)
	UserInfo.sync();	
	
	local lb2Str = {
		mName				 	= UserInfo.roleInfo.name,
		mCoin 					= UserInfo.playerInfo.coin,
		mGold					= UserInfo.playerInfo.gold,
		mLV						= common:getLanguageString("@MyLevel", UserInfo.roleInfo.level)
	};
	NodeHelper:setStringForLabel(container, lb2Str);
	local RoleManager = require("RoleManager");
	local sprite2Img = {
		mOccupation = RoleManager:getOccupationIconById(UserInfo.roleInfo.itemId),
		mVip		= UserInfo.getVipImage()
	};
	NodeHelper:setSpriteImage(container, sprite2Img);
	
	local currentExp = UserInfo.roleInfo.exp
	local roleExpCfg = ConfigManager.getRoleLevelExpCfg()
	
	local expBar = container:getVarScale9Sprite("mExp")
	if UserInfo.roleInfo.level>=GameConfig.LevelLimit.PlayerLevel then
		expBar:setScaleX(1)
	else
		local nextLevelExp = roleExpCfg[UserInfo.roleInfo.level]["exp"]
		
		local percent = currentExp / nextLevelExp
		expBar:setScaleX(percent)
	end
	
	-- 开服活动，显示当前登录第几天对应的活动
	if MainScenePageInfo.loginDay>0 and MainScenePageInfo.loginDay<8 then
		local confManager = require("ConfigManager")
		local loginCfg = confManager.getLoginCfg()
		NodeHelper:setNodeVisible(container:getVarNode("mActivityDailyLoginNode"),true)
		NodeHelper:setStringForLabel(container,{mActivityDailyLoginLabel = loginCfg[MainScenePageInfo.loginDay].description})
	else
		NodeHelper:setNodeVisible(container:getVarNode("mActivityDailyLoginNode"),false)
	end
end	

function MainScenePageInfo.onFeedback(container)
	local title = common:getLanguageString("@FeedbackTitle");
	local msg 	= common:getLanguageString("@FeedbackMsg");
	
	PageManager.showNotice(title, msg);
end

function MainScenePageInfo.showFriendBtnImg(container)
	NodeHelper:setNormalImages(container, {mFriend = GameConfig.Image["FriendBtn_" .. curFriendBtnTag]});
end

function MainScenePageInfo_resetForAssessbly( )
	--小手，重新设置ccbi按钮
	
	--刷新页面
	PageManager.refreshPage("MainScenePage","assembly")	
end

function MainScenePageInfo.switchFriendBtnImg()
	curFriendBtnTag = curFriendBtnTag % 2 + 1;
end	
	
function MainSceneScene_onMainScenePage()
	PageManager.showPage("MainScenePage");
	return 1;
end

return MainScenePageInfo;


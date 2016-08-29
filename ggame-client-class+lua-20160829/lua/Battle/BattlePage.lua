local Chat_pb = require "Chat_pb"
local Player_pb = require "Player_pb"
local HP_pb = require "HP_pb"
local Battle_pb = require "Battle_pb"
local UserInfo = require("UserInfo");
local ChatManager = require("ChatManager")
local NewbieGuideManager = require("NewbieGuideManager")
--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			BattlePage.lua
--  DESCRIPTION:	BattlePage page 
--
--
--  AUTHOR:		Zhen Hui
--  CREATED:	2014/7/30
--  MODIFY2:
--
--  MODIFY1:
--
--------------------------------------------------------------------------------


local alliance = require('Alliance_pb')
local Shop_pb = require('Shop_pb');
local BattlePageInfo = {}       -- all ui information

local BattleInfo = {} -- server data info in the memory
local MARKET_INIT_INFO_TYPE = 1;

local NodeHelper = require("NodeHelper");
--ս��ҳ������߼�����
BattleMainPageHelper = require "BattleMainPageHelper"

--ս������߼�����
BattleFightHelper = require "BattleFightHelper"

BattleDataHelper = require "BattleDataHelper"


local PackageLogicForLua = require "PackageLogicForLua"
FightLogConfig = {}

local offlinePop = false
gameStartTime = 0
local flagAnnounce = true
local flagGiftPackage = true
local reEnterPage = false
local mSpeakLayer = {}

function luaCreat_BattlePage(container)
	CCLuaLog("Z#:luaCreate_BattlePage!");
	container:registerFunctionHandler(BattlePageInfo.onFunction);
	BattlePageInfo.registerAllPacket(container)
end

function BattlePageInfo.onFunction(eventName,container)
	
	if eventName == "luaInit" then
		BattlePageInfo.onInit(container);
	elseif eventName == "luaLoad" then
		BattlePageInfo.onLoad(container);
	elseif eventName == "luaEnter" then
		BattlePageInfo.onEnter(container);
	elseif eventName == "luaExecute" then
		BattlePageInfo.onExecute(container);
	elseif eventName == "luaExit" then
		BattlePageInfo.onExit(container);
	elseif eventName == "luaUnLoad" then
		BattlePageInfo.onUnLoad(container);
	elseif eventName == "luaReceivePacket" then
		BattlePageInfo.onReceivePacket(container);
	elseif eventName == "luaGameMessage" then
		BattlePageInfo.onGameMessage(container);
	elseif eventName == "luaInputboxEnter" then
		BattleMainPageHelper:onInputboxEnter(container);
	elseif eventName == "onChatContent" then
		BattleMainPageHelper:onChatContent(container)
	elseif eventName == "onWorldMap" then
		BattleMainPageHelper:showWorldMap(container);
	elseif eventName == "onMsgSend" then
		BattleMainPageHelper:onSendMsg(container)
	elseif eventName == "onBattleJournal" then
		BattleMainPageHelper:onBattleJournal(container)
	elseif eventName == "onChat" then
		BattleMainPageHelper:onWorldChat(container)	
	elseif eventName == "onChatSetUp" then
		BattleMainPageHelper:onChatSetUp(container)
	elseif eventName == "onFastBattle" then
		BattleMainPageHelper:onFastBattle(container)
	elseif eventName == "onBattleSetUp" then
		BattleMainPageHelper:onBattleSetUp(container)
	elseif eventName == "onBattleStatistics" then
		BattleMainPageHelper:onBattleStatistics(container)
	elseif eventName == "onSelectCareer" then
		BattleMainPageHelper:showNotice(container)	
	elseif eventName == "onPersonalChannelBtn" then
		BattleMainPageHelper:onPersonalChannel(container)
	elseif eventName == "onGuildChannelBtn" then
		BattleMainPageHelper:onGuildChannel(container)
	elseif eventName == "onWorldChannelBtn" then
		BattleMainPageHelper:onWorldChannel(container)
	elseif eventName == "onAccountOff" then
		BattleMainPageHelper:onBattleStatistics(container)	
	elseif eventName == "onSee" then
		BattleMainPageHelper:onViewPlayerInPrivate(container)	
	elseif eventName == "onEliteMap" then
		if UserInfo.roleInfo.level < GameConfig.ELITEMAP_OPEN_LEVEL then
			MessageBoxPage:Msg_Box_Lan("@EliteMapNotEnoughLevel")
			return 
		end
		local EliteMapManager = require("EliteMapManager")
		EliteMapManager:enterEliteMapInfoByProfId()
		--PageManager.changePage("EliteMapPage")
	end	
end

function BattlePageInfo.initFightLogConfig(container)
	CCLuaLog("Z#:BattlePageInfo.onInit!");	
	if FightLogConfig == nil or table.maxn(FightLogConfig)<=0 then
		local tabel = TableReaderManager:getInstance():getTableReader("FightLogConfig.txt")
		local count = tabel:getLineCount()-1;
		for i = 1,count do
			local index = tabel:getData(i,0)
			if FightLogConfig[index] == nil then
				FightLogConfig[index] = {}
				FightLogConfig[index].id = tabel:getData(i, 0)
				FightLogConfig[index].content = tabel:getData(i, 1)
			end
		end
	end
end


function BattlePageInfo.onInit(container)
	CCLuaLog("Z#:BattlePageInfo.onInit!");	
	
	BattlePageInfo.initFightLogConfig(container)
	
	
end

function BattlePageInfo.onLoad(container)
	
	CCLuaLog("Z#:BattlePageInfo.onLoad!");		
		
	local height = CCDirector:sharedDirector():getWinSize().height
	if height<900 then
		container:loadCcbiFile("BattlePage_ipad.ccbi",true);
	else
		container:loadCcbiFile("BattlePage.ccbi",true);
	end
	--����scrollView
	container.scrollview=container:getVarScrollView("mChatContent");
	container:autoAdjustResizeScrollview(container.scrollview);
	--container.scrollview:setTouchEnabled(false);
	container.scrollViewRootNode=container.scrollview:getContainer();			
	
	--ս��scrollView
	container.battleScrollview=container:getVarScrollView("mBattleContent");
	container:autoAdjustResizeScrollview(container.battleScrollview);
	--container.battleScrollview:setTouchEnabled(false);
	container.battleScrollViewRootNode=container.battleScrollview:getContainer();		
	local mScale9Sprite1 = container:getVarScale9Sprite("mScale9Sprite1")
	if mScale9Sprite1 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite1 )
	end
	local mScale9Sprite2 = container:getVarScale9Sprite("mScale9Sprite2")
	if mScale9Sprite2 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite2 )
	end

	
end

function BattlePageInfo.registerAllPacket(container)
	
	CCLuaLog("Z#:registerAllPacket!");
	container:registerMessage(MSG_MAINFRAME_REFRESH);	
	container:registerPacket(HP_pb.MAP_STATISTICS_SYNC_S)	
	container:registerPacket(HP_pb.BUY_FAST_FIGHT_TIMES_S)	
	container:registerPacket(HP_pb.BATTLE_INFO_SYNC_S)	
	container:registerPacket(HP_pb.PLAYER_AWARD_S)	
	container:registerPacket(HP_pb.NEXT_BATTLE_TYPE_S)	
	container:registerPacket(HP_pb.STATE_INFO_SYNC_S)		
	container:registerPacket(HP_pb.MESSAGE_LIST_S)	
	
	--BUY_FAST_FIGHT_TIMES_S	
end

function BattlePageInfo.removeAllPacket(container)
	CCLuaLog("Z#:removeAllPacket!");
	container:removeMessage(MSG_MAINFRAME_REFRESH)		
	container:removePacket(HP_pb.MAP_STATISTICS_SYNC_S)	
	container:removePacket(HP_pb.BUY_FAST_FIGHT_TIMES_S)	
	container:removePacket(HP_pb.BATTLE_INFO_SYNC_S)	
	container:removePacket(HP_pb.PLAYER_AWARD_S)	
	container:removePacket(HP_pb.NEXT_BATTLE_TYPE_S)	
	container:removePacket(HP_pb.STATE_INFO_SYNC_S)	
	container:removePacket(HP_pb.MESSAGE_LIST_S)
end


function BattlePageInfo.onEnter(container)
	
	container.scrollViewRootNode:removeAllChildren();
	container.battleScrollViewRootNode:removeAllChildren();
	
	BattleMainPageHelper:initNode(container)
	BattleMainPageHelper:showBattlePanel(container,true)
	--��ʼ������
	BattleFightHelper:initSceneNode(container)	
	--����ս������״̬
	BattleFightHelper:EnterState(container,FightState.FightRequired)
	BattleMainPageHelper:refreshChatPage(container)
	
	--����ָ������ʾС��
	BattlePageInfo.showNewBeeHint(container)
	
	--��ȡ��ʷ�����Layer��ע���¼���Ӧ
	mSpeakLayer = container:getVarLayer("mSpeakHistoryLayer");	
	if mSpeakLayer~=nil then	
		mSpeakLayer:setTouchEnabled(true)		
		mSpeakLayer:setContentSize(container.scrollViewRootNode:getContentSize())		
		mSpeakLayer:setPosition(container.scrollViewRootNode:getPosition());
		mSpeakLayer:registerScriptTouchHandler(BattleMainPageHelper.onTouchHistoryLayerHandler,false,0,false);
	end		
end


function BattlePageInfo.sendMsgForDropsInfo( container)
	local msg = Shop_pb.OPShopInfo();
	msg.type = MARKET_INIT_INFO_TYPE;
	msg.itemId = "";

	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(HP_pb.SHOP_C, pb_data, #pb_data, false);
end

function BattlePageInfo.onExecute(container)
	BattleMainPageHelper:onExecute(container)
	
	if reEnterPage then
		container.scrollViewRootNode:removeAllChildren();
		container.battleScrollViewRootNode:removeAllChildren();
		--���ս����������
		local BattleFightSceneHelper = require("BattleFightSceneHelper")
		BattleFightSceneHelper:resetAllCharacter(container)
		--ע������������״̬
		BattlePageInfo.showNewBeeHint(container)
		--����״̬Ϊ����ս��״̬
		BattleFightHelper:initSceneNode(container)
		BattleFightHelper:EnterState(container,FightState.FightRequired)
		
		reEnterPage = false		
	end
	
	local dt = GamePrecedure:getInstance():getFrameTime() * 1000;
	gameStartTime = gameStartTime + dt;
	if offlinePop == false then
		
		--0.8��֮�󵯳����߽����
		if gameStartTime>800 and BattleDataHelper.BattleOfflineStatics ~= nil  then
			if  BattleDataHelper.BattleOfflineStatics.result ~=nil and BattleDataHelper.BattleOfflineStatics.result.useTime ~= nil then
				PageManager.pushPage("OfflineAccountPage");	
				offlinePop = true
			end			
		end			
	end
	
	
	-- Gift Package
	-- 1100
	if flagGiftPackage then
		if gameStartTime > 1100 then
			-- GiftListReceived defined in GiftPage.lua, default [false]
			if GiftListReceived then
				-- ��ִ��һ��
				flagGiftPackage = false

				-- ��ȡ������Ϣ������ս��빫�������
				if UserInfo.roleInfo.level >= GameConfig.ALLIANCE_OPEN_LEVEL then
					local msg = alliance.HPAllianceEnterC()
					local pb = msg:SerializeToString()
					PacketManager:getInstance():sendPakcet(HP_pb.ALLIANCE_ENTER_C, pb, #pb, false)
				end
				
				-- init market info
				BattlePageInfo.sendMsgForDropsInfo( container)

				-- �п���ȡ�����������
				if hasUngetReward() then
					PageManager.pushPage("GiftPage")	
				end
			end
		end			
	end

	--push ����
	--1400
	if flagAnnounce then
		if gameStartTime > 1400 then
		--[[			
			local str = Language:getInstance():getString("@Announcement")
			PageManager.showHelp( GameConfig.HelpKey.ANNOUNCEMENT ,str , true )
		--]]
			flagAnnounce = false
		end			
	end
	
	
	--�����Ƿ���ʾnewbee hint
	BattlePageInfo.showNewBeeHint(container)
	--ս��״̬update
	BattleFightHelper:UpdateState(container);	
	
	--������update
	PackageLogicForLua.Update();
	
end


function BattlePageInfo.showNewBeeHint(container)
	local pageName =  MainFrame:getInstance():getCurShowPageName() 
	local needGuidEquip = not UserInfo.hasEquipInRole() and UserInfo.guideInfo.Equip and UserEquipManager:countEquipAll() > 0 and ( UserInfo.roleInfo.level ~= 2 or pageName ~= "EquipmentPage" );
	MainFrame:getInstance():setChildVisible("mNewbeeHintNode2", needGuidEquip);
	
	--NodeHelper:setNodesVisible(container, {mNewbeeHintNode2 = UserInfo.isNewbeeInFastFight()});
	
	local isNewbeeFight = UserInfo.isNewbeeInFightBoss()	
	local hintNode = container:getVarNode("mNewbeeHintNode")
	if hintNode == nil then
		return 
	end
	if isNewbeeFight == false or NewBeeFightBossDone == true then	
		hintNode:setVisible(false)
		MainFrame:getInstance():setChildVisible("mNewbeeHintNode1", false);
		return 
	end
	

	if isNewbeeFight then
		hintNode:setVisible(true)
	else
		hintNode:setVisible(false)
	end		
	local pageName =  MainFrame:getInstance():getCurShowPageName() 
	--�����ս��ҳ�棬�����ǵ�ͼҳ��Ҳ����ʾ
	if pageName:len() ==0 or pageName == "MapPage" or not UserInfo.hasPassGiftEquip() then	
		MainFrame:getInstance():setChildVisible("mNewbeeHintNode1", false);
	else
		MainFrame:getInstance():setChildVisible("mNewbeeHintNode1", isNewbeeFight);
	end
end


function BattlePageInfo.onAnimationDone(container)
	BattleFightHelper:onAnimationDone(container);
end

function BattlePageInfo.onExit(container)
	CCLuaLog("Z#:BattlePageInfo.onExit!");
	BattlePageInfo.removeAllPacket(container);	
	container.scrollViewRootNode:removeAllChildren();	
	
end

function BattlePageInfo.onUnLoad(container)
	CCLuaLog("Z#:BattlePageInfo.onUnLoad!");
end


function BattlePageInfo.onReceivePacket(container)
	
	--ҳ��ˢ������߼�
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	--ս��ͳ���������
	if opcode == HP_pb.MAP_STATISTICS_SYNC_S then
		local msg = Battle_pb.HPMapStatisticsSync()
		msg:ParseFromString(msgBuff)
		--����յ��ذ���ˢ��ͳ��������
		BattleMainPageHelper:refreshMapStatics(container,msg)		
		g_firstGotMapStatic = false
		return
	end		
	
	--����ս���ذ�
	if opcode == HP_pb.BUY_FAST_FIGHT_TIMES_S then
		local msg = Battle_pb.HPBuyFastFightTimesRet()
		msg:ParseFromString(msgBuff)
		
		UserInfo.stateInfo.fastFightTimes = msg.fastFightTimes
		UserInfo.stateInfo.fastFightBuyTimes = msg.fastFightBuyTimes
		CCLuaLog("recieved fast time return "..UserInfo.stateInfo.fastFightTimes..UserInfo.stateInfo.fastFightBuyTimes);
		--����յ��ذ���ˢ��ͳ��������
		--MessageBoxPage:Msg_Box_Lan("@BuyFastFightTimeSuccess")		
		return
	end		
	
	--����
	if opcode == HP_pb.PLAYER_AWARD_S then
		PackageLogicForLua.onReceivePlayerAward(msgBuff)
	end		
	
	-- �³�ս������ͬ��
	if opcode == HP_pb.NEXT_BATTLE_TYPE_S then
		BattleDataHelper:onReceiveNextBattleType(msgBuff)
	end	
	
	-- player state receive pos 2, make sure player state has been recieved by zhenhui 
	if opcode == HP_pb.STATE_INFO_SYNC_S then
		local msg = Player_pb.HPPlayerStateSync();		
		msg:ParseFromString(msgBuff)
		CCLuaLog("@onReceivePlayerStates -- ");
		if msg~=nil then
			UserInfo.stateInfo = msg;			
		end
	end	
		
	-- ˽����ȡ������Ϣ�ذ�
	if opcode == HP_pb.MESSAGE_LIST_S then
		local msg = Friend_pb.HPMsgListInfo();		
		msg:ParseFromString(msgBuff)
		if msg~=nil then
			for i=1,#msg.friendMsgs do
				local oneData = msg.friendMsgs[i]
				--offline msg push
				if oneData~=nil then
					ChatManager.insertPrivateMsg(oneData.senderId,nil,oneData,false)
				end
			end
			if #msg.friendMsgs>0 then
				BattleMainPageHelper:showPrivatePage(container)
			end
		else
			CCLuaLog("@onReceiveOfflineMessageBox -- error in data");
		end
	end	
end



function BattlePageInfo.onGameMessage(container)
	CCLuaLog("@BattlePageInfo.onGameMessage -- begin!");
	
	if container:getMessage():getTypeId() == MSG_MAINFRAME_REFRESH then	
		--�ж��ǲ��Ǳ�ҳ��
		local msg = MsgMainFrameRefreshPage:getTrueType(container:getMessage());	
		local pageName=	msg.pageName
		if pageName=="BattlePage" then
			local extraParam = msg.extraParam
			if extraParam == "PrivateChat" then			
				BattleMainPageHelper:showPrivatePage(container)	
			elseif extraParam == "WorldChat" then			
				BattleMainPageHelper:showPage(container,BattlePageEnum.PAGE_CHAT,Const_pb.CHAT_WORLD)				
			elseif extraParam == "ChatCloseSwitch" then				
				BattleMainPageHelper:refreshChatClose(container)
			elseif extraParam == "Battle" then
				BattleMainPageHelper:showBattlePanel(container, true);
			elseif extraParam == "Boss" then
				NewbieGuideManager.guide(GameConfig.NewbieGuide.FirstFightBoss)
			elseif extraParam == "ArenaOrBoss" then
				--ֱ������С��ģʽ�Ĳ�ս�����������ȴ�״̬
				BattleMainPageHelper:refreshChatPage(container)
				BattleFightHelper:fightBossOrArena(container)			
			else
				BattleMainPageHelper:refreshChatPage(container)
			end				
		end
	end
	
end

function MainFrame_onBattlePageBtn()
	PageManager.refreshPage("BattlePage", "Battle");
	NewbieGuideManager.guide(GameConfig.NewbieGuide.FirstFight)
	return 0;
end

function BattlePage_ResetForAssembly()
	gameStartTime = 0
	local BattleDataHelper = require("BattleDataHelper")
	BattleDataHelper.BattleOfflineStatics = nil
	offlinePop = false
	--flagAnnounce = true
	flagGiftPackage = true	
	MainScenePageInfo_resetForAssessbly()	
end


--Ϊ�ص�½���������ù���
function BattlePage_Reset()
	BattleDataHelper:resetData()
	BattleMainPageHelper:resetData()
	BattleFightHelper:resetData()	
	NewBeeFightBossDone = false
	reEnterPage = true		
	beforeFightBattleTotalColdTime = 0
	--������أ�ֻ���л��˺�ʱ��������״̬
	flagAnnounce = true
	offlinePop = false
end

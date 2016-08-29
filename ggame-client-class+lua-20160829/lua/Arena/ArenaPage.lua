----------------------------------------------------------------------------------
--[[
	FILE:			ArenaPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	竞技场
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
require "Arena_pb"
require "HonorShop_pb"
require "HP_pb"
local UserInfo = require("UserInfo");
registerScriptPage("LeaveMessageDetailPage")
local ArenaData = require "ArenaData"
local viewPlayerInfoPage = "ViewPlayerInfoPage";
local titleManager = require("TitleManager")
registerScriptPage(viewPlayerInfoPage);

local thisPageName = "ArenaPage"
--[[
local opcodes = {
	--竞技场对手信息
	ARENA_DEFENDER_LIST_C = 7001,
	
	
	
	ARENA_DEFENDER_LIST_S = 7002,
	--换一批竞技场对手
	REPLACE_DEFENDER_LIST_C = 7003,
	REPLACE_DEFENDER_LIST_S = 7004,
	--购买今日次数
	BUY_CHALLENGE_TIMES_C = 7005,
	BUY_CHALLENGE_TIMES_S = 7006,
	--竞技场排行榜
	ARENA_RANKING_LIST_C = 7007,
	ARENA_RANKING_LIST_S = 7008,
	--挑战对手
	CHALLENGE_DEFENDER_C = 7009,
	CHALLENGE_DEFENDER_S = 7010
}
]]--
local ResManagerForlua = require("ResManagerForLua");
local option = {
	ccbiFile = "ArenaPage.ccbi",
	handlerMap = {
		onArena					= "showArena",
		onRanking				= "showRank",
		onHonorExchange 		= "showExchange",
		onPurchaseTimes			= "purchaseTimes",
		onReplacement			= "changeOpponet",
		onExchangeRefreshBtn 	= "refreshExchange",
		onPrestigeCreateBtn		= "buildGodlyEquip",
		onMyMessageChat			= "onMyMessageChat",
		onArenaRecord			= "onArenaRecord",
		onHelp					= "onHelp"
	},
	--opcode = opcodes
};

local MonsterCfg = {}

local ArenaPageBase = {}

local NodeHelper = require("NodeHelper");
local TodoStr = "99";
local TodoImage = "UI/MainScene/UI/u_ico000.png";
local TodoPoster = "person/poster_char_Boss.png";
local TodoTrue = true;
local GameConfig = require("GameConfig");
local ITEM_COUNT_PER_LINE = 5;

local PageType = {
	Arena 	= 1,
	Ranking = 2,
	Exchange = 3
}

local PlayerType = {
	NPC = 1,
	Player = 2
}

local roleConfig = {}

local isGotoBattle = false;

--------------------------------------------------------------
local ArenaItem = {
	ccbiFile = "ArenaContent.ccbi"
};

local ArenaInfo = {
	pageType = PageType.Arena,
	SelfInfo = {},
	DefendersInfos = {},
	itemChangeName = "",
	rankingInfo = {},
	exchangeInfo = {}, 				-- 兑换的物品信息
	refreshCostHonor = 0 			-- 此次兑换需要消耗的荣誉值
}	

local chanllengeContainer

ArenaBuyTimesInitCost = ""

local function toPurchaseTimes( boo )
	if boo then
		local msg = Arena_pb.HPBuyChallengeTimes()
		pb_data = msg:SerializeToString()
		PacketManager:getInstance():sendPakcet(HP_pb.BUY_CHALLENGE_TIMES_C, pb_data, #pb_data, true)
	end		
end

local function setRankReward ( node , str ,boo)
	
	local strName = FreeTypeConfig[62].content
	local infoTab = {}	
	
	--local posX = nameNode:getPositionX()
	--local posY = nameNode:getPositionY()
	--NodeHelper:setCCHTMLLabel( nameNode,CCSize(500,200),ccp(posX,posY),common:getGsubStr( infoTab , strName ) )
	
    
	local tab = Split(str,",")
	
	local rewardStr = "" 
	for k,v in ipairs( tab ) do
		
		local reward = ResManagerForlua:getResInfoByTypeAndId( tonumber(Split(v,"_")[1]),tonumber(Split(v,"_")[2]),tonumber(Split(v,"_")[3]) )
		--rewardStr = rewardStr .. reward.name .. "*" .. reward.count .. ""
		
		table.insert( infoTab , reward.name  )
		table.insert( infoTab , reward.count  )
	end
	if node ~= nil then
		node:setAnchorPoint(ccp(0, 1))
	end
	
	--local lab = NodeHelper:setCCHTMLLabelDefaultPos( node , CCSize(400,200) , common:getGsubStr( infoTab , strName )  )
	if boo == nil or boo ==false then
		local lab = NodeHelper:setCCHTMLLabelDefaultPos( node , CCSize(600,200) , common:getGsubStr( infoTab , strName )  )
	    lab:setAnchorPoint(ccp(0, 1))
	else
	local lab = NodeHelper:setCCHTMLLabelDefaultPos( node , CCSize(600,200) , common:getGsubStr( infoTab , strName )  )
	    lab:setAnchorPoint(ccp(0, 0.5))
	end
end

function ArenaItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ArenaItem.onRefreshItemView(container);
	elseif eventName == "onHand" then
		ArenaItem.showOpponentInfo(container);
	elseif eventName == "mDekaron" then
		ArenaItem.doChanllenge(container)
	elseif eventName == "onLeaveMessage" then
		ArenaItem.onLeaveMessage( container )
	end
end

function ArenaItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID
	local itemInfo = ArenaInfo.DefendersInfos[contentId]
    setRankReward( container:getVarLabelBMFont("mReward") , itemInfo.rankAwardsStr )	
	local lb2Str = {
		mLv 			= "Lv." .. itemInfo.level,
		--mArenaName		= itemInfo.name,
		mRankingNum		= itemInfo.rank,
		mFightingNum	= itemInfo.fightValue
		--mReward		= rewardStr
	};
	if itemInfo:HasField("allianceName") and itemInfo:HasField("allianceId") then
		lb2Str.mGuildName      = itemInfo.allianceName.."("..itemInfo.allianceId..")" 
    else
        lb2Str.mGuildName      = ""
	end
	NodeHelper:setStringForLabel(container, lb2Str);
	local headPic = ""
	if itemInfo.identityType == 2 then
		headPic = roleConfig[itemInfo.cfgItemId]["icon"]
	else
		headPic = MonsterCfg[itemInfo.cfgItemId]["icon"]
	end

	NodeHelper:setSpriteImage(container, {mPic = headPic});
	
	if itemInfo:HasField("roleItemId") then
	    local merPic = roleConfig[itemInfo.roleItemId]["icon"]
	    NodeHelper:setSpriteImage(container, {mMerPic = merPic});
	end

	local strPlayType = ""
	if itemInfo.identityType == PlayerType.NPC then
		strPlayType = Language:getInstance():getString("@ArenaPlayType")
	end
	
	local strName = FreeTypeConfig[16].content
	local infoTab = { itemInfo.name ,roleConfig[itemInfo.prof%100].name,strPlayType}	
	
	local nameNode = container:getVarLabelTTF("mArenaName")

	-- 称号
	nameNode:setString( itemInfo.name .. "[" .. roleConfig[itemInfo.prof%100].name .. "]" .. strPlayType)
	local fontSize = VaribleManager:getInstance():getSetting("arenaItemTittleSize")
	titleManager:setBMFontLabelTittle(nameNode,itemInfo.titleId,fontSize,true)
	
	-- local playerTitleCfg = ConfigManager.getPlayerTitleCfg()
	-- if itemInfo.titleId~=0 then
	-- 	local playerTitleType = playerTitleCfg[itemInfo.titleId].type
	-- 	local size = CCSizeMake(GameConfig.LineWidth.PlayerTitle, 100);
	-- 	local htmlLab = common:setPlayerTitle(nameNode,"ArenaPlayerTitle" .. "_" .. playerTitleType,itemInfo.titleId,size)
	-- 	if htmlLab ~= nil then
	-- 		htmlLab:setPosition(ccp(nameNode:getContentSize().width+nameNode:getPositionX(),nameNode:getPositionY()))
	-- 	end
	-- end
end	

--Õ¹Ê¾ÃèÊöÐÅÏ¢
function ArenaItem.showOpponentInfo(container)
	local contentId = container:getItemDate().mID
	local itemInfo = ArenaInfo.DefendersInfos[contentId]
	
	if itemInfo.identityType == PlayerType.NPC then
		MessageBoxPage:Msg_Box("@CheckNPCErro")
		return
	end
	PageManager.viewPlayerInfo(itemInfo.playerId, true);
	--ViewPlayerInfoPage_setPlayerId(itemInfo.playerId ,true)
	--PageManager.pushPage(viewPlayerInfoPage);
end

function ArenaItem.doChanllenge(container)
    
	if ArenaInfo.SelfInfo.surplusChallengeTimes <= 0 then
	    chanllengeContainer = container
		local title = Language:getInstance():getString("@ArenaPurchaseTimesTitle")	
		local message = Language:getInstance():getString("@DoChanllengeArenaPurchaseTimesMsg")
		local infoTab = { ArenaBuyTimesInitCost }
		PageManager.showConfirm(title,common:getGsubStr( infoTab , message ), function(isSure)
			if isSure and UserInfo.isGoldEnough(ArenaBuyTimesInitCost) then
				toPurchaseTimes(true);
			end
		end);
		return
	end		
	
	local contentId = container:getItemDate().mID
	local itemInfo = ArenaInfo.DefendersInfos[contentId]
	
	if itemInfo.rank > ArenaInfo.SelfInfo.rank then
		MessageBoxPage:Msg_Box_Lan("@ArenaLowPlayer")
		return
	end
	local msg = Arena_pb.HPChallengeDefender()	
	if itemInfo.identityType == 1 then
		msg.monsterId = itemInfo.cfgItemId
	else
		msg.monsterId = 0
	end
	msg.defendeRank = itemInfo.rank
	pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.CHALLENGE_DEFENDER_C, pb_data , #pb_data, false)
	ArenaInfo.itemChangeName = itemInfo.name
    isGotoBattle = true
end

function ArenaItem.onLeaveMessage(container)	
	
	local contentId = container:getItemDate().mID
	local itemInfo = ArenaInfo.DefendersInfos[contentId]
	LeaveMsgDetail_setPlayId( itemInfo.playerId , itemInfo.name )
	PageManager.pushPage("LeaveMessageDetailPage")
	
end 

----------------------------------------------------------------------------------
local ArenaRankPageContent = {}

function ArenaRankPageContent.onFunction(eventName,container)
	if eventName == "luaRefreshItemView" then
		ArenaRankPageContent.onRefreshItemView(container)
	elseif eventName == "onHand" then
		ArenaRankPageContent.showOpponentInfo(container)
	end
end	

function ArenaRankPageContent.showOpponentInfo(container)
	local contentId = container:getItemDate().mID
	local itemInfo = ArenaInfo.rankingInfo [contentId]
	
	if itemInfo.identityType == PlayerType.NPC then
		MessageBoxPage:Msg_Box("@CheckNPCErro")
		return
	end
	PageManager.viewPlayerInfo(itemInfo.playerId, true);
end

function ArenaRankPageContent.onRefreshItemView(container)
	local contentId = container:getItemDate().mID
	local itemInfo = ArenaInfo.rankingInfo[contentId]
	
	local signature = ""
	if itemInfo:HasField("signature") then
		signature = itemInfo.signature
	end
	local lb2Str = {
		mLv 			= "Lv." .. itemInfo.level,
		--mArenaName		= itemInfo.name,
		mRankingNum		= itemInfo.rank,
		mFightingNum	= itemInfo.fightValue,
		mPersonalSignature		=  signature
	};
	if itemInfo:HasField("allianceName") and itemInfo:HasField("allianceId") then
		lb2Str.mGuildName      = itemInfo.allianceName.."("..itemInfo.allianceId..")" 
    else
        lb2Str.mGuildName      = ""
	end
	NodeHelper:setStringForLabel(container, lb2Str)

	local picSprite =  container:getVarSprite("mTrophy0" .. itemInfo.rank)
	NodeHelper:setNodeVisible( picSprite , true )
		
	local bg = container:getVarNode("mTrophyBG0" .. itemInfo.rank)
	
	if bg == nil then
		bg = container:getVarNode("mTrophyBG0" .. 4)
	else
		container:getVarNode("mTrophyBG0" .. 4):setVisible(false)			
	end	
	NodeHelper:setNodeVisible( bg , true )
	local headPic = ""
	if itemInfo.identityType == 2 then
		headPic = roleConfig[itemInfo.cfgItemId].icon
	else
		headPic = MonsterCfg[itemInfo.cfgItemId].icon
	end
	
	
	NodeHelper:setSpriteImage(container, {mPic = headPic});
	
	
	if itemInfo:HasField("roleItemId") then
	    local merPic = roleConfig[itemInfo.roleItemId]["icon"]
	    NodeHelper:setSpriteImage(container, {mMerPic = merPic});
	end
	
	local strPlayType = ""
	if itemInfo.identityType == PlayerType.NPC then
		strPlayType = Language:getInstance():getString("@ArenaPlayType")
	end
	
	local strName = FreeTypeConfig[16].content
	local infoTab = { itemInfo.name ,roleConfig[itemInfo.prof%100].name,strPlayType}	
	
	local nameNode = container:getVarLabelTTF("mArenaName")

	nameNode:setString( itemInfo.name .. "[" .. roleConfig[itemInfo.prof%100].name .. "]" .. strPlayType)
	-- 称号
	local fontSize = VaribleManager:getInstance():getSetting("arenaRankTittleSize")
	titleManager:setBMFontLabelTittle(nameNode,itemInfo.titleId,fontSize,true)
end		
	
	
-----------------------------------------------
--ArenaPageBase页面中的事件处理
----------------------------------------------
function ArenaPageBase:onEnter(container)
	--container:registerMessage(MSG_SEVERINFO_UPDATE)
	CCLuaLog("ArenaPage:--------------onEnter")
	MonsterCfg = ConfigManager.getMonsterCfg()
	ArenaInfo.pageType = PageType.Arena
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	container.scrollviewArenaContent=container:getVarScrollView("mArenaContent");
	if container.scrollviewArenaContent~=nil then
		container:autoAdjustResizeScrollview(container.scrollviewArenaContent);
	end		
	
	local mScale9Sprite1 = container:getVarScale9Sprite("mScale9Sprite1")
	if mScale9Sprite1 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite1 )
	end

	container.scrollviewRankingContent=container:getVarScrollView("mRankingContent");
	if container.scrollviewRankingContent~=nil then
		container:autoAdjustResizeScrollview(container.scrollviewRankingContent);
	end		
	
	local mScale9Sprite2 = container:getVarScale9Sprite("mScale9Sprite2")
	if mScale9Sprite2 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite2 )
	end

	container.scrollviewExchangeContent=container:getVarScrollView("mExchangeContent");
	if container.scrollviewExchangeContent~=nil then
		container:autoAdjustResizeScrollview(container.scrollviewExchangeContent);
	end		
	
	local mScale9Sprite3 = container:getVarScale9Sprite("mScale9Sprite3")
	if mScale9Sprite3 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite3 )
	end

	UserInfo.sync()
	roleConfig = ConfigManager.getRoleCfg()	
	--[[
	if ArenaBuyTimesInitCost == "" then
		ArenaBuyTimesInitCost = GameConfig.Default.ArenaBuyTimes
	end
	]]--
	self:registerPacket( container )
	--NodeHelper:initScrollView(container, "mContent", 3)
	self:initArenaScrollView(container)
	self:initRankingScrollView(container)
	self:initExchangeScrollView(container)
	
	self:selecetTab( container )
	
	if ArenaInfo.pageType == PageType.Arena then
	    self:getInfo( container )
	elseif ArenaInfo.pageType == PageType.Ranking then
	    self:showRank(container)
    elseif ArenaInfo.pageType == PageType.Exchange then
    	self:getExchangeInfo(container)
	end
	
	
	
	self:initPage( container )
	
end


function ArenaPageBase:initPage( container )
    local rankingNum = ""
    local rewardStr = ""
    if ArenaDataPageInfo ~= nil then
        rankingNum = ArenaDataPageInfo.self.rank
		setRankReward( container:getVarLabelBMFont("mReward") , ArenaInfo.SelfInfo.rankAwardsStr ,true)
        --rewardStr = self:setRankReward( ArenaInfo.SelfInfo.rankAwardsStr )
    end

	local lb2Str = {
		mArenaName				= "Lv." .. UserInfo.roleInfo.level .. " " .. UserInfo.roleInfo.name,
		mRankingNum				= rankingNum,
		mFightingNum			= UserInfo.roleInfo.fight,
		--mReward				= rewardStr,
		mRemainingChallengesNum = ArenaInfo.SelfInfo.surplusChallengeTimes
	}
	NodeHelper:setStringForLabel(container, lb2Str)

	-- 称号
	local nameNode = container:getVarLabelTTF("mArenaName")
	local fontSize = VaribleManager:getInstance():getSetting("arenaMyTittleSize")
	titleManager:setBMFontLabelTittle(nameNode,titleManager.myNowTitleId,fontSize)

	local headPic = roleConfig[UserInfo.roleInfo.itemId]["icon"]

	NodeHelper:setSpriteImage(container, {mArenaPic = headPic})
end

function ArenaPageBase:initArenaScrollView(container)
	container.mScrollViewArena = container:getVarScrollView("mArenaContent");
	container.mScrollViewRootNodeArena = container.mScrollViewArena:getContainer();
	container.m_pScrollViewFacadeArena = CCReViScrollViewFacade:new_local(container.mScrollViewArena);
	container.m_pScrollViewFacadeArena:init(3, 3);
end

function ArenaPageBase:initRankingScrollView( container )
	container.mScrollViewRanking = container:getVarScrollView("mRankingContent");
	container.mScrollViewRootNodeRanking = container.mScrollViewRanking:getContainer();
	container.m_pScrollViewFacadeRanking = CCReViScrollViewFacade:new_local(container.mScrollViewRanking);
	container.m_pScrollViewFacadeRanking:init(7, 3);
end

function ArenaPageBase:initExchangeScrollView( container )
	container.mScrollViewExchange = container:getVarScrollView("mExchangeContent")
	container.mScrollViewRootNodeExchange = container.mScrollViewExchange:getContainer()
	container.m_pScrollViewFacadeExchange = CCReViScrollViewFacade:new_local(container.mScrollViewExchange)
	container.m_pScrollViewFacadeExchange:init(3, 3)
end

function ArenaPageBase:onExecute(container)
	if  isGotoBattle then
		--ÓÉ·þÎñÆ÷´«¹ýÀ´£¬ÔÝÊ±²»ÓÃ¿Í»§¶Ë×Ô¼º×ö
	   --[[ nextChanllengeName = ArenaInfo.itemChangeName
		nextFightIsBoss = false--]]
	    PageManager.showFightPage("BattlePage")
		--PageManager.refreshPage("BattlePage");	
	    isGotoBattle = false
	    ArenaInfo.itemChangeName = ""
	end
end

function ArenaPageBase:onExit(container)
	self:removePacket(container)
	--NodeHelper:deleteScrollView(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)	
	self:deleteScrollViewArena(container)
	self:deleteScrollViewRanking(container)
	self:deleteScrollViewExchange(container)
end

----------------------------------------------------------------

function ArenaPageBase:deleteScrollViewArena( container )
	self:clearAllItem(container);
	if container.m_pScrollViewFacadeArena then
		container.m_pScrollViewFacadeArena :delete();
		container.m_pScrollViewFacadeAren = nil;
	end
	container.mScrollViewRootNodeArena = nil;
	container.mScrollViewArena  = nil;
end

function ArenaPageBase:deleteScrollViewRanking( container )
	self:clearAllRankingItem(container);
	if container.m_pScrollViewFacadeRanking then
		container.m_pScrollViewFacadeRanking :delete();
		container.m_pScrollViewFacadeRanking = nil;
	end
	container.mScrollViewRootNodeRanking = nil;
	container.mScrollViewRanking  = nil;
end

function ArenaPageBase:deleteScrollViewExchange( container )
	self:clearAllExchangeItem(container);
	if container.m_pScrollViewFacadeExchange then
		container.m_pScrollViewFacadeExchange:delete();
		container.m_pScrollViewFacadeExchange = nil;
	end
	container.m_pScrollViewFacadeExchange = nil;
	container.mScrollViewExchange  = nil;
end

function ArenaPageBase:refreshPage(container)
	self:showPlayerInfo(container)
end

function ArenaPageBase:showPlayerInfo(container)
	
	--local rewardStr = self:setRankReward( ArenaInfo.SelfInfo.rankAwardsStr )
	setRankReward( container:getVarLabelBMFont("mReward") , ArenaInfo.SelfInfo.rankAwardsStr ,true)	
	local lb2Str = {
		mArenaName				= "Lv." .. ArenaInfo.SelfInfo.level .. " " .. ArenaInfo.SelfInfo.name,
		mRankingNum				= ArenaInfo.SelfInfo.rank,
		mFightingNum			= ArenaInfo.SelfInfo.fightValue,
		--mReward				= rewardStr,
		mNowHonorNum		 	= UserInfo.playerInfo.honorValue,
		mNowPrestigeNum 		= UserInfo.playerInfo.reputationValue,
		mRemainingChallengesNum = ArenaInfo.SelfInfo.surplusChallengeTimes
	}
	NodeHelper:setStringForLabel(container, lb2Str)

	-- 称号
	local nameNode = container:getVarLabelTTF("mArenaName")
	local fontSize = VaribleManager:getInstance():getSetting("arenaMyTittleSize")
	titleManager:setBMFontLabelTittle(nameNode,titleManager.myNowTitleId,fontSize)

	local headPic = roleConfig[ArenaInfo.SelfInfo.cfgItemId]["icon"]
	
	NodeHelper:setSpriteImage(container, {mArenaPic = headPic})
end		

function ArenaPageBase:selecetTab( container )
	if ArenaInfo.pageType == PageType.Arena then
		container:getVarMenuItemImage("mArena"):selected()
		container:getVarMenuItemImage("mRanking"):unselected()
		container:getVarMenuItemImage("mHonorExchange"):unselected()
		
		container:getVarScrollView( "mArenaContent" ):setVisible(true)
		container:getVarNode( "mArenaContentNode" ):setVisible(true)
		container:getVarScrollView( "mRankingContent" ):setVisible(false)
		container:getVarNode( "mRankingContentNode" ):setVisible(false)
		container:getVarScrollView( "mExchangeContent" ):setVisible(false)
		container:getVarNode( "mExchangeContentNode" ):setVisible(false)
		
		container:getVarNode("mArenaBtnNode"):setVisible(true)
		container:getVarNode("mExchangeRefreshNode"):setVisible(false)
	elseif ArenaInfo.pageType == PageType.Ranking then
		container:getVarMenuItemImage("mArena"):unselected()
		container:getVarMenuItemImage("mRanking"):selected()
		container:getVarMenuItemImage("mHonorExchange"):unselected()
		
		container:getVarScrollView( "mArenaContent" ):setVisible(false)
		container:getVarNode( "mArenaContentNode" ):setVisible(false)
		container:getVarScrollView( "mRankingContent" ):setVisible(true)
		container:getVarNode( "mRankingContentNode" ):setVisible(true)
		container:getVarScrollView( "mExchangeContent" ):setVisible(false)
		container:getVarNode( "mExchangeContentNode" ):setVisible(false)
		
		container:getVarNode("mArenaBtnNode"):setVisible(false)
		container:getVarNode("mExchangeRefreshNode"):setVisible(false)
	elseif ArenaInfo.pageType == PageType.Exchange then
		container:getVarMenuItemImage("mArena"):unselected()
		container:getVarMenuItemImage("mRanking"):unselected()
		container:getVarMenuItemImage("mHonorExchange"):selected()
		
		container:getVarScrollView( "mArenaContent" ):setVisible(false)
		container:getVarNode( "mArenaContentNode" ):setVisible(false)
		container:getVarScrollView( "mRankingContent" ):setVisible(false)
		container:getVarNode( "mRankingContentNode" ):setVisible(false)
		container:getVarScrollView( "mExchangeContent" ):setVisible(true)
		container:getVarNode( "mExchangeContentNode" ):setVisible(true)
		
		container:getVarNode("mArenaBtnNode"):setVisible(false)
		container:getVarNode("mExchangeRefreshNode"):setVisible(true)
	end
	NodeHelper:setNodesVisible(container, {mPrestigeCreateNode = ArenaInfo.pageType == PageType.Exchange});
end

function ArenaPageBase:getInfo( container )
    --[[
	local msg = Arena_pb.HPArenaDefenderList()
	msg.playerId = UserInfo.playerInfo.playerId
	pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.ARENA_DEFENDER_LIST_C, pb_data, #pb_data, true)
	]]--
	CCLuaLog("ArenaPage:--------------getInfo")
	--local info = ArenaDataPageInfo
	CCLuaLog("ArenaPage:--getInfo--getHPArenaDefenderListSyncSForLua")
	if ArenaDataPageInfo == nil or ArenaDataPageInfo.self == nil then
		container:registerPacket(HP_pb.ARENA_DEFENDER_LIST_SYNC_S)
		local msg = Arena_pb.HPArenaDefenderList()
		msg.playerId = UserInfo.playerInfo.playerId
		pb_data = msg:SerializeToString()
		PacketManager:getInstance():sendPakcet(HP_pb.ARENA_DEFENDER_LIST_C, pb_data, #pb_data, true)
	else
		self:onReceiveArenaInfo(container, ArenaDataPageInfo)
	end
	
	
end

----------------scrollview-------------------------
function ArenaPageBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function ArenaPageBase:clearAllItem(container)
	if container.m_pScrollViewFacadeArena then
		container.m_pScrollViewFacadeArena:clearAllItems();
	end
	if container.mScrollViewRootNodeArena then
		container.mScrollViewRootNodeArena:removeAllChildren();
	end
end

function ArenaPageBase:buildItem(container)
	--NodeHelper:buildScrollView(container, #ArenaInfo.DefendersInfos, "ArenaContent.ccbi", ArenaItem.onFunction);
	local iMaxNode = container.m_pScrollViewFacadeArena:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0
	
	table.sort(ArenaInfo.DefendersInfos,
		function (e1, e2)
		    if not e2 then return true end
		    if not e1 then return false end
		
		    return e1.rank < e2.rank
	    end
    )
	
	for i = #ArenaInfo.DefendersInfos, 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create("ArenaContent.ccbi")
			pItem.id = iCount
			pItem:registerFunctionHandler(ArenaItem.onFunction)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end

			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacadeArena:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacadeArena:addItem(pItemData)
		end
		iCount = iCount + 1
	end

	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount);
	container.mScrollViewArena:setContentSize(size)
	container.mScrollViewArena:setContentOffset(ccp(0, container.mScrollViewArena:getViewSize().height - container.mScrollViewArena:getContentSize().height * container.mScrollViewArena:getScaleY()))
	container.m_pScrollViewFacadeArena:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollViewArena:forceRecaculateChildren();
end
	
----------------click event------------------------
function ArenaPageBase:showArena(container)
    if ArenaInfo.pageType == PageType.Arena then
        self:selecetTab( container )
        return
    end
	ArenaInfo.pageType = PageType.Arena
	self:selecetTab( container )
	self:getInfo(container)
end

function ArenaPageBase:showRank(container)
    --[[
    if ArenaInfo.pageType == PageType.Ranking then
        self:selecetTab( container )
        return
    end
    ]]--
	if ArenaRankCacheInfo ~= nil and #ArenaRankCacheInfo.rankInfo ~= 0  then        		
		self:onReceiveRankInfo(container , ArenaRankCacheInfo )
	end
	
	ArenaInfo.pageType = PageType.Ranking
	self:selecetTab( container )
	--PageManager.pushPage("ArenaRankPage")
	self:getRankInfo(container)
end

function ArenaPageBase:showExchange(container)
	if ArenaInfo.pageType == PageType.Exchange then
        self:selecetTab( container )
        return
    end
	ArenaInfo.pageType = PageType.Exchange
	self:selecetTab( container )
	self:getExchangeInfo(container)
end

function ArenaPageBase:refreshExchange( container )
	-- 自己的荣誉值不足刷新消耗荣誉值
	if UserInfo.playerInfo.honorValue < ArenaInfo.refreshCostHonor then
		MessageBoxPage:Msg_Box_Lan("@HonorExchangeHonorValueNotEnough")
		return
	end
	local title = common:getLanguageString('@HonorExchangeRefreshTitle')
	local message = common:getLanguageString('@HonorExchangeRefresh', ArenaInfo.refreshCostHonor)
	PageManager.showConfirm(title, message,
	   function (agree)
		   if agree then
		    	ArenaPageBase.sendRefresh()
		   end
	   end
	)
end

function ArenaPageBase:buildGodlyEquip(container)
	PageManager.pushPage("GodlyEquipBuildPage");
end

function ArenaPageBase.sendRefresh()
	common:sendEmptyPacket(HP_pb.HONOR_SHOP_REFRESH_C)
end

function ArenaPageBase:purchaseTimes(container)
	local title = Language:getInstance():getString("@ArenaPurchaseTimesTitle")	
	local message = Language:getInstance():getString("@ArenaPurchaseTimesMsg")
    local infoTab = { ArenaBuyTimesInitCost }
	PageManager.showConfirm(title,common:getGsubStr( infoTab , message ), function(isSure)
		if isSure and UserInfo.isGoldEnough(ArenaBuyTimesInitCost) then
			toPurchaseTimes(true);
		end
	end);
end

function ArenaPageBase:changeOpponet(container)
	local msg = Arena_pb.HPReplaceDefenderList()
	msg.playerId = UserInfo.playerInfo.playerId
	pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.REPLACE_DEFENDER_LIST_C, pb_data , #pb_data, true)	
end	

--»Ø°ü´¦Àí

function ArenaPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
    
	if opcode == HP_pb.ARENA_DEFENDER_LIST_SYNC_S then
		container:removePacket(HP_pb.ARENA_DEFENDER_LIST_SYNC_S)
		local msg = Arena_pb.HPArenaDefenderListSyncS()
		msg:ParseFromString(msgBuff)	
		self:onReceiveArenaInfo(container, msg)
		return
	end		
	
	if opcode == HP_pb.REPLACE_DEFENDER_LIST_S then
		local msg = Arena_pb.HPReplaceDefenderListRet()
		msg:ParseFromString(msgBuff)
		self:onReplaceDefenders( container , msg )
		return
	end

	if opcode == HP_pb.BUY_CHALLENGE_TIMES_S then
		local msg = Arena_pb.HPBuyChallengeTimesRet()	
		msg:ParseFromString(msgBuff)	
		self:buyTimesRet(container, msg)
		ArenaItem.doChanllenge(chanllengeContainer)
		return
	end
	
	if container:getRecPacketOpcode() == HP_pb.ARENA_RANKING_LIST_S then
		local msg = Arena_pb.HPArenaRankingListRet()
		local msgBuff = container:getRecPacketBuffer()
		msg:ParseFromString(msgBuff)
		ArenaRankCacheInfo = msg
		self:onReceiveRankInfo(container, msg)
		return
	end	

	if opcode == HP_pb.HONOR_SHOP_LIST_S or opcode == HP_pb.HONOR_SHOP_REFRESH_S then
		local msg = HonorShop_pb.HPHonorShopListRet()
		msg:ParseFromString(msgBuff)
		self:onReceiveExchangeInfo(container, msg)
		return
	end

	if opcode == HP_pb.HONOR_SHOP_BUY_S then
		-- alliance enter
		local msg = HonorShop_pb.HPHonorBuyRet()
		msg:ParseFromString(msgBuff)
		self:onReceiveExchangeBuyRet(container, msg)
		return
	end
	--[[
	if opcode == HP_pb.CHALLENGE_DEFENDER_S then
		local msg = Arena_pb.HPChallengeDefenderRet()
		msg:ParseFromString(msgBuff)
		--self:onUpdateMergeDataRet(container, msg)
		PageManager.showFightPage( "BattlePage" )
		return
	end
	]]--
end

function ArenaPageBase:onReceiveArenaInfo( container , msg )
	ArenaInfo.SelfInfo = msg.self
	for k = 1 ,#msg.defender, 1 do
		ArenaInfo.DefendersInfos[k] = msg.defender[k]
	end
	table.sort(ArenaInfo.DefendersInfos,
	function (e1, e2)
		if not e2 then return true end
		if not e1 then return false end
		
		return e1.rank < e2.rank
	end
	)
	ArenaBuyTimesInitCost = msg.self.nextBuyPrice
	self:refreshPage(container)
	self:rebuildAllItem(container)

end

function ArenaPageBase:onReceiveRankInfo(container, msg)
	if msg == nil or #msg.rankInfo == 0 then return end
	ArenaInfo.rankingInfo = msg.rankInfo
	ArenaInfo.SelfInfo = msg.self
	ArenaBuyTimesInitCost = msg.self.nextBuyPrice
	self:refreshPage(container)
	self:rebuildAllRankingItem(container)
end

function ArenaPageBase:rebuildAllRankingItem(container)
	self:clearAllRankingItem(container);
	self:buildRankingItem(container);
end

function ArenaPageBase:clearAllRankingItem(container)
	if container.m_pScrollViewFacadeRanking then
		container.m_pScrollViewFacadeRanking:clearAllItems();
	end
	if container.mScrollViewRootNodeRanking then
		container.mScrollViewRootNodeRanking:removeAllChildren();
	end
end

function ArenaPageBase:buildRankingItem(container)
	--NodeHelper:buildScrollView(container, #Arena.rankingInfo, "ArenaRankingContent", ArenaRankPageContent.onFunction);
	local iMaxNode = container.m_pScrollViewFacadeRanking:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0

	for i=#ArenaInfo.rankingInfo, 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create("ArenaRankingContent.ccbi")
			pItem.id = iCount
			pItem:registerFunctionHandler(ArenaRankPageContent.onFunction)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end

			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacadeRanking:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacadeRanking:addItem(pItemData)
		end
		iCount = iCount + 1
	end

	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount)
	container.mScrollViewRanking:setContentSize(size)
	container.mScrollViewRanking:setContentOffset(ccp(0, container.mScrollViewRanking:getViewSize().height - container.mScrollViewRanking:getContentSize().height * container.mScrollViewRanking:getScaleY()))
	container.m_pScrollViewFacadeRanking:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollViewRanking:forceRecaculateChildren();


end

--------------------------------------------------------------

--------------------scrollview item-------------------------------
local ShopItem = {
	ccbiFile = 'ArenaHonorShopContentItem.ccbi',
}

local ShopItemSub = {
	ccbiFile = 'ArenaHonorShopContent.ccbi',
}

function ShopItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ShopItem.onRefreshItemView(container)
	end
end

function ShopItemSub.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ShopItemSub.onRefreshItemView(container)
	elseif eventName == 'onbuy' then
		ShopItemSub.buy(container)
	elseif eventName == 'onHonorShopHand' then
		ShopItemSub.showTip(container)
	end
end

function ShopItemSub.onRefreshItemView(container)
	MessageBoxPage:Msg_Box('@refresh sub shop item')
end

function ShopItemSub.buy(container)
	local itemIndex = container:getTag()
	local info = ArenaInfo.exchangeInfo[itemIndex]
	if not info then
		MessageBoxPage:Msg_Box('@GuildBuyItemEmpty')
		return 
	end
	-- 自己的荣誉值不足物品消耗荣誉值
	if UserInfo.playerInfo.honorValue < info.costHonor then
		MessageBoxPage:Msg_Box_Lan("@HonorExchangeHonorValueNotEnough")
		return
	end
	local msg = HonorShop_pb.HPHonorBuy()
	msg.id = tostring(info.id)
	common:sendPacket(HP_pb.HONOR_SHOP_BUY_C,msg,false)
end

function ShopItemSub.showTip(container)
	local itemIndex = container:getTag()
	local item = ArenaInfo.exchangeInfo[itemIndex]
	if item == nil then return; end
	local stepLevel = EquipManager:getEquipStepById(item.itemId)

	GameUtil:showTip(container:getVarNode('mHonorShopHand'), {
		type 		= item.itemType, 
		itemId 		= item.itemId,
		buyTip		= true,
		starEquip	= stepLevel == GameConfig.ShowStepStar
	});
end

function ShopItem.onRefreshItemView(container)
	local shopItemId = container:getItemDate().mID
	local subContent
	local contentContainer
	for i=1, 3 do

		local subItemIndex = (shopItemId - 1) * 3 + i

		if subItemIndex > #ArenaInfo.exchangeInfo then return end

		local shopInfo = ArenaInfo.exchangeInfo[subItemIndex]
		if not shopInfo then return end

		local resInfo = ResManagerForLua:getResInfoByTypeAndId(shopInfo.itemType, shopInfo.itemId)
		if not resInfo then return end

		subContent = container:getVarNode('mPosition' .. tostring(i))

		if subContent then
			--clear old subItems
			subContent:removeAllChildren();
			
			-- create subItem
			contentContainer = ScriptContentBase:create(ShopItemSub.ccbiFile, subItemIndex)	
			contentContainer:registerFunctionHandler(ShopItemSub.onFunction)

			-- set sub item's view
			local lb2Str = {
				--mNumber 			= shopInfo.remTime,
				mHonorShopName 		= resInfo.name,
				mHonorShopNum 		= shopInfo.costHonor .. common:getLanguageString('@Honor')
			}
			NodeHelper:setStringForLabel(contentContainer, lb2Str)

			--image
			NodeHelper:setSpriteImage(contentContainer,{mHonorShopPic = resInfo.icon})

			-- add subItem into item
			subContent:addChild(contentContainer)
			contentContainer:release()
		end
	end
end	
----------------scrollview item end-------------------------------


function ArenaPageBase:rebuildAllExchangeItem(container)
	self:clearAllExchangeItem(container);
	self:buildExchangeItem(container);
end

function ArenaPageBase:clearAllExchangeItem(container)
	if container.m_pScrollViewFacadeExchange then
		container.m_pScrollViewFacadeExchange:clearAllItems();
	end
	if container.mScrollViewRootNodeExchange then
		container.mScrollViewRootNodeExchange:removeAllChildren();
	end
end
function ArenaPageBase:buildExchangeItem(container)
	local iMaxNode = container.m_pScrollViewFacadeExchange:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0

	for i=math.ceil(#ArenaInfo.exchangeInfo/3), 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)

		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create(ShopItem.ccbiFile)
			pItem.id = iCount
			pItem:registerFunctionHandler(ShopItem.onFunction)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end

			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacadeExchange:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacadeExchange:addItem(pItemData)
		end
		iCount = iCount + 1
	end

	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount)
	container.mScrollViewExchange:setContentSize(size)
	container.mScrollViewExchange:setContentOffset(ccp(0, container.mScrollViewExchange:getViewSize().height - container.mScrollViewExchange:getContentSize().height * container.mScrollViewExchange:getScaleY()))
	container.m_pScrollViewFacadeExchange:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollViewExchange:forceRecaculateChildren();
end
function ArenaPageBase:getExchangeInfo(container)
	common:sendEmptyPacket(HP_pb.HONOR_SHOP_LIST_C)
end
function ArenaPageBase:onReceiveExchangeInfo(container, msg)
	if msg == nil or #msg.shopItems == 0 then return end
	ArenaInfo.exchangeInfo = msg.shopItems
	ArenaInfo.refreshCostHonor = msg.refreshCostHonor
	self:refreshPage(container)
	self:rebuildAllExchangeItem(container)
end
function ArenaPageBase:onReceiveExchangeBuyRet(container, msg)
	if msg == nil or #ArenaInfo.exchangeInfo ==0 then return end
	for i=#ArenaInfo.exchangeInfo,1,-1 do
		if ArenaInfo.exchangeInfo[i].id == msg.id then
			table.remove(ArenaInfo.exchangeInfo,i)
		end
	end
	self:refreshPage(container)
	self:rebuildAllExchangeItem(container)
end
--------------------------------------------------------------

function ArenaPageBase:onReplaceDefenders( container , msg )
	for k = 1 ,#msg.defender, 1 do
		ArenaInfo.DefendersInfos[k] = msg.defender[k]
		ArenaDataPageInfo.defender[k] = msg.defender[k]
	end
	 
	self:rebuildAllItem(container)
end

function ArenaPageBase:buyTimesRet(container , msg )
	ArenaInfo.SelfInfo.surplusChallengeTimes = msg.surplusChallengeTimes
	container:getVarLabelBMFont("mRemainingChallengesNum"):setString(ArenaInfo.SelfInfo.surplusChallengeTimes)  
	ArenaInfo.SelfInfo.nextBuyPrice = msg.nextBuyPrice
	ArenaBuyTimesInitCost = msg.nextBuyPrice
	ArenaDataPageInfo.self.surplusChallengeTimes = msg.surplusChallengeTimes
end	

function ArenaPageBase:registerPacket(container)
	
	container:registerPacket(HP_pb.REPLACE_DEFENDER_LIST_S)
	container:registerPacket(HP_pb.BUY_CHALLENGE_TIMES_S)
	container:registerPacket(HP_pb.ARENA_RANKING_LIST_S)	
	container:registerPacket(HP_pb.ARENA_RANKING_LIST_S)

	container:registerPacket(HP_pb.HONOR_SHOP_LIST_S)
	container:registerPacket(HP_pb.HONOR_SHOP_BUY_S)
	container:registerPacket(HP_pb.HONOR_SHOP_REFRESH_S)
	--container:registerPacket(HP_pb.CHALLENGE_DEFENDER_S)
end

function ArenaPageBase:removePacket(container)
    
	container:removePacket(HP_pb.REPLACE_DEFENDER_LIST_S)
	container:removePacket(HP_pb.BUY_CHALLENGE_TIMES_S)
	container:removePacket(HP_pb.ARENA_RANKING_LIST_S)
	container:removePacket(HP_pb.ARENA_RANKING_LIST_S)

	container:removePacket(HP_pb.HONOR_SHOP_LIST_S)
	container:removePacket(HP_pb.HONOR_SHOP_BUY_S)
	container:removePacket(HP_pb.HONOR_SHOP_REFRESH_S)
	--container:removePacket(HP_pb.CHALLENGE_DEFENDER_S)
end

function ArenaPageBase:getRankInfo( container )
	local msg = Arena_pb.HPArenaRankingList()
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(HP_pb.ARENA_RANKING_LIST_C, pb_data, #pb_data, true)
end

function ArenaPageBase:onArenaRecord( container )
	PageManager.pushPage("ArenaRecordPage")
end

function ArenaPageBase:onMyMessageChat( container )
	PageManager.pushPage("LeaveMessagePage")
end

function ArenaPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			self:onReceiveArenaInfo(container, ArenaDataPageInfo)
		end
	end
end

function ArenaPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_PVP)
end

ArenaRankCacheInfo = Arena_pb.HPArenaRankingListRet()

function ArenaPage_Reset()
    ArenaRankCacheInfo = nil
    ArenaDataPageInfo = nil
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ArenaPage = CommonPage.newSub(ArenaPageBase, thisPageName, option);

require "HP_pb"
require "ProfRank_pb"
local UserInfo = require("UserInfo");
local thisPageName = "ProfessionRankingPage"
local NodeHelper = require("NodeHelper");
local option = {
	ccbiFile = "FightingRankingPage.ccbi",
	handlerMap = {
		onWarrior				= "onWarrior",
		onHunter				= "onHunter",
		onMaster				= "onMaster",
		onHelp					= "onHelp"
	},
	--opcode = opcodes
}
local ProfessionRankingPageBase = {}
local ProfessionRankingPageContent = {}
local titleManager = require("TitleManager")

local MonsterCfg = {}
local roleConfig = {}
local ProfessionType = {
	WARRIOR = 1,
	HUNTER = 2,
	MASTER = 3
}

local PageInfo = {
	curProType = ProfessionType.WARRIOR,
	selfRank = "--",
	rankInfos = {},
	viewHolder = {}
	
}

local isFirstEnter = {
	[1] = true,
	[2] = true,
	[3] = true
}


ProfessionRankingCacheInfo = {
	[1] = ProfRank_pb.HPProfRankingListRet(),
	[2] = ProfRank_pb.HPProfRankingListRet(),
	[3] = ProfRank_pb.HPProfRankingListRet()
}		
----------------------------------------------------

function ProfessionRankingPageContent.onFunction( eventName ,container )
	if eventName == "luaRefreshItemView" then
		ProfessionRankingPageContent.onRefreshItemView(container)
	elseif eventName == "onHand" then
		ProfessionRankingPageContent.onHand( container )
	end
end

function ProfessionRankingPageContent.onRefreshItemView( container ) 
	local contentId = container:getItemDate().mID
	local itemInfo = PageInfo.rankInfos[contentId]
	
	local signature = ""
	--if itemInfo:HasField("signature") == true then
	signature = itemInfo.signature
	--end
	
	local picSprite =  container:getVarSprite("mTrophy0" .. itemInfo.rank)
	if picSprite then		
		picSprite:setVisible(true)
	else
		for i = 1,3 do
			container:getVarSprite("mTrophy0" .. i):setVisible(false)
		end
	end
		
	local bg = container:getVarNode("mTrophyBG0" .. itemInfo.rank)
	
	if bg == nil then
		bg = container:getVarNode("mTrophyBG0" .. 4)
	else
		container:getVarNode("mTrophyBG0" .. 4):setVisible(false)			
	end	
	NodeHelper:setNodeVisible( bg , true )	
	
	
	local lb2Str = {
		mLv 			= "Lv." .. itemInfo.level,
		mName			= itemInfo.name,
		mRankingNum		= itemInfo.rank,
		mFightingNum	= itemInfo.fightValue,
		mPersonalSignature		=  signature
	}
	if itemInfo:HasField("allianceName") and itemInfo:HasField("allianceId") then
		lb2Str.mGuildName      = itemInfo.allianceName.."("..itemInfo.allianceId..")" 
    else
        lb2Str.mGuildName      = ""
	end
	NodeHelper:setStringForLabel(container, lb2Str)	
	NodeHelper:setSpriteImage(container, {mPic = roleConfig[itemInfo.cfgItemId].icon})
	
	if itemInfo:HasField("roleItemId") then
	    local merPic = roleConfig[itemInfo.roleItemId]["icon"]
	    NodeHelper:setSpriteImage(container, {mMerPic = merPic});
	end
	
	
	--称号
	local nameNode = container:getVarLabelTTF("mName")
	local fontSize = VaribleManager:getInstance():getSetting("proRankTittleSize")
	titleManager:setBMFontLabelTittle(nameNode,itemInfo.title,fontSize,true)
end

function ProfessionRankingPageContent.onHand( container )
	local contentId = container:getItemDate().mID
	local itemInfo = PageInfo.rankInfos[contentId]
	
	PageManager.viewPlayerInfo(itemInfo.playerId, true)
end

function ProfessionRankingPageBase:onEnter( container )
	for i = 1,3 do
		isFirstEnter[i] = true
	end
	
	self:initPage( container )
	self:selectTab( container )
	self:refreshPage( container )	
	self:getPageInfo( container )
	
end

function ProfessionRankingPageBase:initPage( container )
	self:registerPacket( container )
	UserInfo.sync()
	roleConfig = ConfigManager.getRoleCfg()
	NodeHelper:initScrollView( container,"mRankingContent",7)
	container.scrollview=container:getVarScrollView("mRankingContent");
	if container.scrollview~=nil then
		container:autoAdjustResizeScrollview(container.scrollview);
	end
	
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	--self:rebuildAllItem(container);
end

function ProfessionRankingPageBase:refreshPage( container )
	local lb2Str = {
		mName					= "Lv." .. UserInfo.roleInfo.level .. " " .. UserInfo.roleInfo.name,
		mRankingNum				= PageInfo.selfRank,
		mFightingNum			= UserInfo.roleInfo.fight
	}

	NodeHelper:setStringForLabel(container, lb2Str)
	NodeHelper:setSpriteImage(container, {mPic = roleConfig[UserInfo.roleInfo.itemId].icon});

	--称号
	local nameNode = container:getVarLabelTTF("mName")
	local fontSize = VaribleManager:getInstance():getSetting("proRankMyTittleSize")
	titleManager:setBMFontLabelTittle(nameNode,titleManager.myNowTitleId,fontSize)
end

function ProfessionRankingPageBase:getPageInfo( container )
	if ProfessionRankingCacheInfo[PageInfo.curProType] ~= nil and #ProfessionRankingCacheInfo[PageInfo.curProType].rankInfo ~= 0 then
		self:onReceiveRankingInfo(container, ProfessionRankingCacheInfo[PageInfo.curProType])
		
		if isFirstEnter[PageInfo.curProType] == false then
			return
		end
	end	
	
	local msg = ProfRank_pb.HPProfRankingList()
	msg.profType = PageInfo.curProType
	local pb_data = msg:SerializeToString()
	
	PacketManager:getInstance():sendPakcet(HP_pb.PROF_RANK_LIST_C, pb_data, #pb_data, true)
end

function ProfessionRankingPageBase:onExecute( container )
	
end

function ProfessionRankingPageBase:onExit( container )
	self:removePacket( container )
	NodeHelper:deleteScrollView(container)
end

function ProfessionRankingPageBase:onWarrior( container )
	if PageInfo.curProType == ProfessionType.WARRIOR then
		self:selectTab(container)
		return
	end
	PageInfo.curProType = ProfessionType.WARRIOR
	self:selectTab(container)
	self:refreshPage( container )
	self:getPageInfo(container)	
	--self:rebuildAllItem(container)
end

function ProfessionRankingPageBase:onHunter( container )
	if PageInfo.curProType == ProfessionType.HUNTER then
		self:selectTab(container)
		return
	end
	PageInfo.curProType = ProfessionType.HUNTER
	self:selectTab(container)
	self:refreshPage( container )
	self:getPageInfo(container)	
	--self:rebuildAllItem(container)
end

function ProfessionRankingPageBase:onMaster( container )
	if PageInfo.curProType == ProfessionType.MASTER then
		self:selectTab(container)
		return
	end
	PageInfo.curProType = ProfessionType.MASTER
	self:selectTab(container)
	self:refreshPage( container )
	self:getPageInfo(container)	
	--self:rebuildAllItem(container)
end

function ProfessionRankingPageBase:selectTab( container )
	local labMenuSelect = {
		mWarrior = (PageInfo.curProType == ProfessionType.WARRIOR),
		mHunter  = (PageInfo.curProType == ProfessionType.HUNTER),
		mMaster  = (PageInfo.curProType == ProfessionType.MASTER)
	}
	
	NodeHelper:setMenuItemSelected( container , labMenuSelect )
	--[[
	NodeHelper:setMenuItemEnabled( container , "mWarrior" , PageInfo.curProType ~= ProfessionType.WARRIOR )
	NodeHelper:setMenuItemEnabled( container , "mHunter" , PageInfo.curProType ~= ProfessionType.HUNTER )
	NodeHelper:setMenuItemEnabled( container , "mMaster" , PageInfo.curProType ~= ProfessionType.MASTER )
	]]--
end

function ProfessionRankingPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_FIGHT_NUM)
end

function ProfessionRankingPageBase:onReceivePacket( container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
    
	if opcode == HP_pb.PROF_RANK_LIST_S then
		local msg = ProfRank_pb.HPProfRankingListRet()
		msg:ParseFromString(msgBuff)	
		ProfessionRankingCacheInfo[PageInfo.curProType] = msg
		isFirstEnter[PageInfo.curProType] = false
		self:onReceiveRankingInfo(container, msg)
		return
	end
end

function ProfessionRankingPageBase:onReceiveRankingInfo( container , msg )
	PageInfo.rankInfos = msg.rankInfo
	table.sort(PageInfo.rankInfos , function ( p1 , p2)
		if not p2 then return true end
		if not p1 then return false end
		
		return p1.rank > p2.rank
	end)
	
	if msg:HasField("selfRank") then
		if msg.selfRank == 0 then
            PageInfo.selfRank = common:getLanguageString("@NotInRanking")
        else
		    PageInfo.selfRank = msg.selfRank
        end
	else
		PageInfo.selfRank = "--"
	end
	local rank = ""
    if PageInfo.selfRank == 0 then
        rank = common:getLanguageString("@NotInRanking")
    else
		rank = PageInfo.selfRank
    end
	container:getVarLabelBMFont("mRankingNum"):setString( rank )
	
	self:rebuildAllItem( container )
	
end

function ProfessionRankingPageBase:rebuildAllItem( container )
	self:clearAllItem(container)
	self:buildItem(container)
end

function ProfessionRankingPageBase:clearAllItem( container )
	NodeHelper:clearScrollView(container)
end

function ProfessionRankingPageBase:buildItem( container )
	NodeHelper:buildScrollViewWithCache(container, #PageInfo.rankInfos, "FightingRankingContent", ProfessionRankingPageContent.onFunction )
end

function ProfessionRankingPageBase:registerPacket( container )
	container:registerPacket(HP_pb.PROF_RANK_LIST_S)
end

function ProfessionRankingPageBase:removePacket( container )
	container:removePacket(HP_pb.PROF_RANK_LIST_S)
end



function ProfessionRankingPage_reset()
	ProfessionRankingCacheInfo = {}
end
----------------------------------------------------
local CommonPage = require("CommonPage")
ProfessionRankingPage = CommonPage.newSub(ProfessionRankingPageBase, thisPageName, option)

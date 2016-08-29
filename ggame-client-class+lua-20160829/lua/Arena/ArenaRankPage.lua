require "Arena_pb"
require "HP_pb"
require "CommonPage"
----------------------------------------------------------------------------------
--[[
	FILE:			ArenaRankPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2013-10-03
	
	MODIFY 1:		hgs at 2013-12-13
					add notes
--]]
----------------------------------------------------------------------------------
local MonsterCfg = {}
local roleConfig = {}
local PageInfo = {
	
}
local NodeHelper = require("NodeHelper");
local PlayerType = {
	NPC = 1,
	Player = 2
}
local ArenaRankPage = {}
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
	local itemInfo = PageInfo[contentId]
	
	if itemInfo.identityType == PlayerType.NPC then
		MessageBoxPage:Msg_Box("@CheckNPCErro")
		return
	end
	PageManager.viewPlayerInfo(itemInfo.playerId);
end

function ArenaRankPageContent.onRefreshItemView(container)
	local contentId = container:getItemDate().mID
	local itemInfo = PageInfo[contentId]
	
	local signature = ""
	if itemInfo:HasField("signature") then
		signature = itemInfo[signature]
	end
	local lb2Str = {
		mLv 			= "Lv." .. itemInfo.level,
		--mArenaName		= itemInfo.name,
		mRankingNum		= itemInfo.rank,
		mFightingNum	= itemInfo.fightValue,
		mPersonalSignature		=  signature
	};
	NodeHelper:setStringForLabel(container, lb2Str)

	local picSprite =  container:getVarSprite("mTrophy0" .. itemInfo.rank)
	NodeHelper:setNodeVisible( picSprite , true )
	local headPic = ""
	if itemInfo.identityType == 2 then
		headPic = roleConfig[itemInfo.cfgItemId].icon
	else
		headPic = MonsterCfg[itemInfo.cfgItemId].icon
	end
	
	
	NodeHelper:setSpriteImage(container, {mPic = headPic});
	
	local strPlayType = ""
	if itemInfo.identityType == PlayerType.NPC then
		strPlayType = Language:getInstance():getString("@ArenaPlayType")
	end
	
	local strName = FreeTypeConfig[16].content
	local infoTab = { itemInfo.name ,roleConfig[itemInfo.prof%100].name,strPlayType}	
	
	local nameNode = container:getVarLabelTTF("mArenaName")
	
	--local posX = nameNode:getPositionX()
	--local posY = nameNode:getPositionY()
	--NodeHelper:setCCHTMLLabel( nameNode,CCSize(500,200),ccp(posX,posY),common:getGsubStr( infoTab , strName ) )
	nameNode:setString( itemInfo.name .. roleConfig[itemInfo.prof%100].name .. strPlayType)
	--NodeHelper:setCCHTMLLabelDefaultPos( nameNode , CCSize(500,200) , common:getGsubStr( infoTab , strName )  )
end	

----------------------------------------------------------------------------------
--ArenaRankPage页面中的事件处理
----------------------------------------------

function ArenaRankPage.onFunction(eventName,container)
	if eventName == "luaInit" then
		ArenaRankPage.onInit(container)
	elseif eventName == "luaEnter" then
		ArenaRankPage.onEnter(container)
	elseif eventName == "luaExit" then
		ArenaRankPage.onExit(container)
	elseif eventName == "luaExecute" then
		ArenaRankPage.onExecute(container)
	elseif eventName == "luaLoad" then
		ArenaRankPage.onLoad(container)
	elseif eventName == "luaUnLoad" then
		ArenaRankPage.onUnLoad(container)
	elseif eventName == "luaGameMessage" then
		ArenaRankPage.onGameMessage(container)
	elseif eventName == "luaReceivePacket" then
		ArenaRankPage.onReceivePacket(container)
	elseif eventName =="onClose" then
    	ArenaRankPage.onClose(container)
    elseif eventName =="onRankBtn" then
    	ArenaRankPage.onUpRank(container)
	end
end

function ArenaRankPage.onInit(container)
    MonsterCfg = ConfigManager.getMonsterCfg()
    roleConfig = ConfigManager.getRoleCfg()
end

function ArenaRankPage.onAnimationDone(container)
end

function ArenaRankPage.onLoad(container)
    container:loadCcbiFile("ArenaRankingPopUp.ccbi")
    ArenaRankPage.onInitPage(container)
end

function ArenaRankPage.onInitPage(container)
    NodeHelper:initScrollView(container, "mContent", 7)
end


function ArenaRankPage.onEnter(container)
    --ArenaRankPage.onInitPage(container)	
	ArenaRankPage.registerPacket(container)
	ArenaRankPage.getRankInfo(container)
end

--发包获取活动信息
function ArenaRankPage.getRankInfo(container)
	local msg = Arena_pb.HPArenaRankingList()
	local pb_data = msg:SerializeToString();
	PacketManager:getInstance():sendPakcet(HP_pb.ARENA_RANKING_LIST_C, pb_data, #pb_data, true);
end

function ArenaRankPage.onReceivePacket(container)
    if container:getRecPacketOpcode() == HP_pb.ARENA_RANKING_LIST_S then
		local msg = Arena_pb.HPArenaRankingListRet()
		local msgBuff = container:getRecPacketBuffer()
		msg:ParseFromString(msgBuff)
		ArenaRankPage.onReceiveRankInfo(container, msg)
		return
	end	
end		

function ArenaRankPage.onReceiveRankInfo(container, msg)
	if msg == nil or #msg.rankInfo == 0 then return end
	PageInfo = msg.rankInfo
	ArenaRankPage.rebuildAllItem(container)
end

function ArenaRankPage.rebuildAllItem(container)
	ArenaRankPage.clearAllItem(container);
	ArenaRankPage.buildItem(container);
end

function ArenaRankPage.clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function ArenaRankPage.buildItem(container)
	NodeHelper:buildScrollView(container, #PageInfo, "ArenaRankingContent", ArenaRankPageContent.onFunction);
end
function ArenaRankPage.onExecute(container)
	
end	


function ArenaRankPage.onExit(container)
	ArenaRankPage.removePacket(container)
	NodeHelper:deleteScrollView(container)
end

function ArenaRankPage.registerPacket(container)
	container:registerPacket(HP_pb.ARENA_RANKING_LIST_S)
end

function ArenaRankPage.removePacket(container)
	container:removePacket(HP_pb.ARENA_RANKING_LIST_S)	
end

function ArenaRankPage.onUnLoad(container)
	CCLuaLog("ArenaRankPage.onUnLoad");
end

function ArenaRankPage.onClose(container)
    local msg = MsgMainFramePopPage:new()
	msg.pageName = "ArenaRankPage"
	
	MessageManager:getInstance():sendMessageForScript(msg)
end

function luaCreat_ArenaRankPage(container)
	CCLuaLog("OnCreat_ArenaRankPage")
	container:registerFunctionHandler(ArenaRankPage.onFunction)
end	


----------------------------------------------------------------------------------
--[[
FILE:			FriendPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	好友页面
AUTHOR:			zhenhui
CREATED:		2014-08-6
--]]
----------------------------------------------------------------------------------
require "Friend_pb"
require "Const_pb"
require "HP_pb"
local GameConfig = require "GameConfig"
local thisPageName = "FriendPage"
local UserInfo = require("UserInfo");


local option = {
	ccbiFile = "FriendPage.ccbi",
	handlerMap = {
		onSearchFriend = "onSearchFriend",
		onFriendRecommend = "onFriendRecommend",
		onHelp = "onHelp"
	},
	opcodes = {
		FRIEND_LIST_S = HP_pb.FRIEND_LIST_S
	}
}

local FriendPageBase = {}

local NodeHelper = require("NodeHelper");

local m_tFriendList = {}


--------------------------------------------------------------
local FriendItem = {
ccbiFile 	= "FriendContent.ccbi"
}


function FriendItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		FriendItem.onRefreshItemView(container)
	elseif eventName == "onViewDetail" then
		FriendItem.onViewDetail(container)	
	end
end

function FriendItem.isInTable(tabel,id)
	for _,v in ipairs(tabel) do
		if v == id then
			return true;
		end
	end
	return false;
end


function FriendItem.removeItemFromTable(tabel,id)
	for k,v in ipairs(tabel) do
		if v == id then
			table.remove(tabel,k);
			break
		end
	end
end


function FriendItem.onRefreshItemView(container)
	local FriendId = tonumber(container:getItemDate().mID)
	local friendInfo = m_tFriendList[FriendId]
	if friendInfo== nil or friendInfo.name == nil then
		CCLuaLog("ERROR in Friend id, not found in m_tFriendList"..FriendId);
		return;
	end		
	
	local flag = (friendInfo.arenaRank == -1) 
	--local arenaRank =flag and "--" or tostring(friendInfo.arenaRank)
	local arenaRank = tostring(FriendId)
	local RoleManager = require("RoleManager");
	local lb2Str = {
	mName 		= friendInfo.name,
	mLevelNum 		= friendInfo.level,
	mOccupation		= RoleManager:getOccupationById(friendInfo.roleId),
	mFightingNum 	= friendInfo.fightValue,
	mId 			= arenaRank,
	--mArenaRankingNum 	= arenaRank -- 取消竞技排名，改成orderId，2014年9月11日 zhaolu
	};
	NodeHelper:setStringForLabel(container, lb2Str);	
	
	local currentColor =  UserInfo.playerInfo.playerId == friendInfo.playerId and GameConfig.ColorMap.COLOR_FRIEND_OWN or GameConfig.ColorMap.COLOR_FRIEND_OTHER
	local lb2Color = {
	mName 		= currentColor,
	mLevelNum 		=currentColor,
	mOccupation		= currentColor,
	mFightingNum 		= currentColor,
	mId 	= currentColor
	};
	NodeHelper:setColorForLabel(container, lb2Color);	
		
		
end		


function FriendItem.onViewDetail(container)
	local FriendId = tonumber(container:getItemDate().mID)
	local friendInfo = m_tFriendList[FriendId]
	if friendInfo== nil or friendInfo.name == nil then
		CCLuaLog("ERROR in Friend id, not found in m_tFriendList"..FriendId);
		return;
	end
	PageManager.viewPlayerInfo(friendInfo.playerId, true);
end



----------------------------------------------------------------------------------

-----------------------------------------------
--FriendPageBase页面中的事件处理
----------------------------------------------
function FriendPageBase:onLoad(container)
	container:loadCcbiFile(option.ccbiFile)
	container.scrollview=container:getVarScrollView("mContent");
	if container.scrollview~=nil then
		container:autoAdjustResizeScrollview(container.scrollview);
	end		
	
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite1")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	
	local mScale9Sprite2 = container:getVarScale9Sprite("mScale9Sprite2")
	if mScale9Sprite2 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite2 )
	end
	
	local mScale9Sprite3 = container:getVarScale9Sprite("mScale9Sprite3")
	if mScale9Sprite3 ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite3 )
	end
end

function FriendPageBase:onEnter(container)
	self:registerPacket(container)	
	
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	
	NodeHelper:initScrollView(container, "mContent", 10);
	
	
	self:onRequestData(container)
	--self:refreshPage(container);		
		
end

function FriendPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_FRIEND)
end

function FriendPageBase:onRequestData(container)
	common:sendEmptyPacket(HP_pb.FRIEND_LIST_C, true)
end

function FriendPageBase:onExecute(container)
	
end	

function FriendPageBase:onExit(container)
	self:removePacket(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	NodeHelper:deleteScrollView(container);	
end
----------------------------------------------------------------

function FriendPageBase:refreshPage(container)
	local friendSize = #m_tFriendList
	local hasNoFriend = container:getVarNode("mFriendAddPromptNode")
	if hasNoFriend==nil then
		return 
	end
	if friendSize<=1 then
		hasNoFriend:setVisible(true)
	else
		hasNoFriend:setVisible(false)
	end
	friendSize = math.max(0,friendSize-1)
	local lb2Str = {
		mFriendLimitNum = common:getLanguageString('@FriendUpperLimit',tostring(friendSize),'30')	
	}
	NodeHelper:setStringForLabel(container, lb2Str)
	local descriptionLabel = container:getVarLabelBMFont("mDescriptionLabel")
	-- 好友下面的描述
	if descriptionLabel ~= nil then
		local autoReturnSize = VaribleManager:getInstance():getSetting("friendDescriptionReturnSize")
		local myRank = nil
		table.foreachi(m_tFriendList, function(i,v)
			if v.playerId == UserInfo.playerInfo.playerId then
				myRank = i
			end
		end)
		if friendSize == 0 then 
            descriptionLabel:setString("")
            return
        end
		-- 好友数量小于10的时候
		if friendSize < 10 then
			if myRank ~= nil then
				local descriptionStr = common:getLanguageString('@MyRankInFriendBattle', myRank)
				descriptionStr = common:stringAutoReturn(descriptionStr, autoReturnSize)
				descriptionLabel:setString(tostring(descriptionStr))
			end
		else
			local descriptionStr = nil
			if myRank ~= nil then
				-- 排名在20%内
				local rankStr = common:getLanguageString('@MyRankInFriendBattle', myRank)
				if myRank <= friendSize*0.2 then
					descriptionStr = rankStr..common:getLanguageString('@MyRankInFriendBattleHigh')
				-- 排名在20%-60%
				elseif myRank <= friendSize*0.6 and myRank > friendSize*0.2 then
					descriptionStr = rankStr..common:getLanguageString('@MyRankInFriendBattleMiddle')
				-- 排名在60%以上
				elseif myRank > friendSize*0.6 then
					descriptionStr = rankStr..common:getLanguageString('@MyRankInFriendBattleLow')
				end
				descriptionStr = common:stringAutoReturn(descriptionStr, autoReturnSize)
				descriptionLabel:setString(tostring(descriptionStr))
			end
		end
	end
end


----------------scrollview-------------------------
function FriendPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function FriendPageBase:clearAllItem(container)
	container.m_pScrollViewFacade:clearAllItems();
	container.mScrollViewRootNode:removeAllChildren();
end

function FriendPageBase:buildItem(container)
	
	--All the friend's list
	local friendSize = #m_tFriendList;
	
	
	local ccbiFile = FriendItem.ccbiFile
	
	--if friendSize ==1 return 
	if friendSize <= 1 or ccbiFile == nil or ccbiFile == ''then return end
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0	
	--totolSize = 6
	for i=friendSize, 1,-1 do
		local pItemData = CCReViSvItemData:new_local()		
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)
		
		if iCount < iMaxNode then
			local pItem = ScriptContentBase:create(ccbiFile)			
			pItem.id = iCount
			pItem:registerFunctionHandler(FriendItem.onFunction)
			if fOneItemHeight < pItem:getContentSize().height then
				fOneItemHeight = pItem:getContentSize().height
			end
			
			if fOneItemWidth < pItem:getContentSize().width then
				fOneItemWidth = pItem:getContentSize().width
			end
			container.m_pScrollViewFacade:addItem(pItemData, pItem.__CCReViSvItemNodeFacade__)
		else
			container.m_pScrollViewFacade:addItem(pItemData)
		end
		iCount = iCount + 1
	end
	
	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount)
	container.mScrollView:setContentSize(size)	
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))	
	
	container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
end

----------------click event------------------------
function FriendPageBase:onCancel(container)
	--PageManager.popPage(thisPageName);
end

function FriendPageBase:onSearchFriend(container)
	PageManager.pushPage("FriendSearchPopPage");
end

function FriendPageBase:onFriendRecommend(container)
	PageManager.pushPage("FriendRecommendPage")
end

--回包处理

function FriendPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == HP_pb.FRIEND_LIST_S then
		local msg = Friend_pb.HPFriendListRet()
		msg:ParseFromString(msgBuff)
		--如果收到回包，刷新Friend页面
		if msg ~= nil then
			m_tFriendList = msg.friendItem
			table.sort(m_tFriendList,
			function ( e1, e2 )
				if not e2 then return true end
				if not e1 then return false end				
				if e1.arenaRank == -1 and e2.arenaRank>0 then return false end
				if e2.arenaRank == -1 and e1.arenaRank>0 then return true end
				
				if e1.arenaRank < e2.arenaRank then
					return true
				elseif e1.arenaRank > e2.arenaRank then
					return false
				else 
					if e1.fightValue > e2.fightValue then
						return true
					else
						return false
					end
				end
			end
			);
			self:rebuildAllItem(container);
			self:refreshPage(container)
		end			
	end
	
end


--继承此类的活动如果同时开，消息监听不能同时存在,通过tag来区分
function FriendPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			FriendPageBase:onRequestData(container)
		end
	end
end

function FriendPageBase:registerPacket(container)
	for key, opcode in pairs(option.opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function FriendPageBase:removePacket(container)
	for key, opcode in pairs(option.opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local FriendPage = CommonPage.newSub(FriendPageBase, thisPageName, option);
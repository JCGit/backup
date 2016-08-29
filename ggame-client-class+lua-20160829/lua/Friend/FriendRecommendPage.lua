----------------------------------------------------------------------------------
--[[
	FILE:			FriendRecommendPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	附近好友弹出页面
	AUTHOR:			zhaolu
	CREATED:		2014-9-11
--]]
----------------------------------------------------------------------------------

local NodeHelper = require("NodeHelper")
local hp = require('HP_pb')
local UserInfo = require("UserInfo");
local FriendRecommend_pb = require('FriendRecommend_pb')
local Friend_pb = require('Friend_pb')
local tittleManager = require("TitleManager")
local thisPageName = 'FriendRecommendPage'
local FriendRecommendBase = {}
local roleCfg = {} -- 好友角色头像
local friendsList = {} -- 6个好友列表
local addedIdTable = {} --添加过的好友id列表

local option = {
	ccbiFile = "FriendRecommendPopUp.ccbi",
	handlerMap = {
		onRefresh 		= 'onRefresh',
		onClose 		= 'onClose'
	},
}

function FriendRecommendBase:onEnter(container)
	addedIdTable = {}
	roleCfg = ConfigManager.getRoleCfg()	
	self:registerPacket(container)
	NodeHelper:initScrollView(container, 'mContent', 3)
	self:getFriendList(container)
end

function FriendRecommendBase:onExit(container)
	self:removePacket(container)
	NodeHelper:deleteScrollView(container)
end

-- 刷新弹出的ccbi的标题 和好友列表
function FriendRecommendBase:refreshPage(container)
	-- title 
	-- local lb2Str = {
	-- 	mTitle = "Lv." .. UserInfo.roleInfo.level .. " " .. UserInfo.roleInfo.name
	-- }
	-- NodeHelper:setStringForLabel(container, lb2Str)
	-- scrollview
	-- 是否搜索到玩家
	if #friendsList ~= 0 then
		self:rebuildAllItem(container)
	else
		MessageBoxPage:Msg_Box_Lan("@FriendRecommendNoPlayer")
	end
end

function FriendRecommendBase:onRefresh(container)
	self:getFriendList(container)
end

function FriendRecommendBase:onClose(container)
	-- 刷新好友页面
	local msg = MsgMainFrameRefreshPage:new()
	msg.pageName = 'FriendPage'
	MessageManager:getInstance():sendMessageForScript(msg)

	PageManager.popPage(thisPageName)
end

----------------scrollview item-------------------------
local FriendItem = {
	ccbiFile = 'FriendRecommendContentItem.ccbi'
}

local FriendItemSub = {
	ccbiFile = 'FriendRecommendContent.ccbi',
}

function FriendItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		FriendItem.onRefreshItemView(container)
	end
end

function FriendItemSub.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		FriendItemSub.onRefreshItemView(container)
	elseif eventName == 'onAddFriend' then
		FriendItemSub.addFriend(container)
	elseif eventName == 'onHand' then
		FriendItemSub.onHand(container)
	end
end

function FriendItemSub.onRefreshItemView(container)
end

function FriendItemSub.addFriend(container)
	local itemIndex = container:getTag()
	local info = friendsList[itemIndex]
	if not info then
		MessageBoxPage:Msg_Box('@FriendRecommendNoAddPlayer')
		return 
	end
	local msg = Friend_pb.HPFriendAdd()
	msg.targetId = info.playerId
	common:sendPacket(HP_pb.FRIEND_ADD_C, msg)
end

function FriendItemSub.onHand(container)
	local itemIndex = container:getTag()
	local info = friendsList[itemIndex]
	PageManager.viewPlayerInfo(info.playerId, true);
end

function FriendItem.onRefreshItemView(container)
	local FriendItemId = container:getItemDate().mID
	local subContent
	local contentContainer
	for i=1, 2 do

		local subItemIndex = (FriendItemId - 1) * 2 + i

		if subItemIndex > #friendsList then return end

		local friendInfo = friendsList[subItemIndex]
		if not friendInfo then return end

		subContent = container:getVarNode('mPeopleNode' .. '0' .. tostring(i))

		if subContent then
			--clear old subItems
			subContent:removeAllChildren();
			
			-- create subItem
			contentContainer = ScriptContentBase:create(FriendItemSub.ccbiFile, subItemIndex)	
			contentContainer:registerFunctionHandler(FriendItemSub.onFunction)

			-- set sub item's view
			local lb2Str = {
				mLevel 				= "Lv." .. friendInfo.level,
				mName 				= friendInfo.name,
				mFightValue 		= friendInfo.fightValue,
			}
			NodeHelper:setStringForLabel(contentContainer, lb2Str)

			--image
			NodeHelper:setSpriteImage(contentContainer,{mPic = roleCfg[friendInfo.roleId]["icon"]})

			-- 称号
			local titleNode = contentContainer:getVarLabelBMFont("mPlayerTitle")
			tittleManager:setLabelTitle(titleNode,friendInfo.titleId)
				  
			--if added btn hide
			table.foreachi(addedIdTable, function(i, v)
				if v == friendInfo.playerId then
					NodeHelper:setNodeVisible(contentContainer:getVarNode("mAddFriendNode"),false)
				end
			end)

			-- add subItem into item
			subContent:addChild(contentContainer)
			contentContainer:release()
		end
	end
end	

----------------scrollview-------------------------
function FriendRecommendBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function FriendRecommendBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function FriendRecommendBase:buildItem(container)
	local needItemCount = math.ceil(#friendsList / 2)
	NodeHelper:buildScrollView(container, needItemCount, FriendItem.ccbiFile, FriendItem.onFunction);
end

---------------------------- packet function ------------------------------------
function FriendRecommendBase:registerPacket(container)
	container:registerPacket(HP_pb.RECOMMEND_FRIEND_LIST_S)
	container:registerPacket(HP_pb.FRIEND_ADD_S)
end

function FriendRecommendBase:removePacket(container)
	container:removePacket(HP_pb.RECOMMEND_FRIEND_LIST_S)
	container:removePacket(HP_pb.FRIEND_ADD_S)
end

function FriendRecommendBase:getFriendList(container)
	common:sendEmptyPacket(HP_pb.RECOMMEND_FRIEND_LIST_C)
end

function FriendRecommendBase:onReceivefriendsList(container, msg)
	friendsList = msg.friendRecommendItem
end

function FriendRecommendBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == hp.RECOMMEND_FRIEND_LIST_S then
		local msg = FriendRecommend_pb.HPFriendRecommendRet()
		msg:ParseFromString(msgBuff)
		addedIdTable = {}	-- 刷新和进入页面的时候已加好友列表为空
		self:onReceivefriendsList(container, msg)
		self:refreshPage(container)
		return
	end

	if opcode == hp.FRIEND_ADD_S then
		MessageBoxPage:Msg_Box_Lan("@AddFriendSuccess")
		local msg = Friend_pb.HPFriendAddRet()
		msg:ParseFromString(msgBuff)
		if msg:HasField("targetId") then
			table.insert(addedIdTable, msg.targetId)
		end
		self:refreshPage(container)
		return
	end
end

local CommonPage = require('CommonPage')
local FriendRecommendPage = CommonPage.newSub(FriendRecommendBase, thisPageName, option)

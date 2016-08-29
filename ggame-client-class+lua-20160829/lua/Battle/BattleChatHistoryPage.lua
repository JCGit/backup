require "Chat_pb"

local thisPageName = "BattleChatHistoryPage"
local UserInfo = require("UserInfo");
local option = {
ccbiFile   = "SpeakHistoryPopUp.ccbi",
handlerMap = {
onClose 			= "onClose",
onBattleChatTitle	= "onBattleChatTitle",
onMessageBox		= "onMessageBox"
}
}
local ChatManager = require("ChatManager")
local BattleChatHistoryPageBase = {}
--------------------------------------------------------------------
local viewPlayerInfoPage = "ViewPlayerInfoPage";
local NodeHelper = require("NodeHelper");
local PageInfo = {
ONE_LINE_COUNT = 5,
personList = {},
MsgBoxPersonList = {}
}

local PAGE_ENUM = {
CHAT_HISTORY = 1,
CHAT_MSGBOX = 2
}

local m_pageIndex = PAGE_ENUM.CHAT_HISTORY

local personFlagList = {}

local roleConfig = {}
local BattleChatHistoryPageItem = {}

function BattleChatHistoryPageItem.onFunction( eventName, container )
	if eventName == "luaRefreshItemView" then
		if m_pageIndex == PAGE_ENUM.CHAT_HISTORY then
			BattleChatHistoryPageItem.onRefreshItemView(container)
		else
			BattleChatHistoryPageItem.onRefreshItemViewForMsgBox(container)
		end
		
	end
end

function BattleChatHistoryPageItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID
	local baseIndex = (contentId - 1) * PageInfo.ONE_LINE_COUNT
	
	for i = 1, PageInfo.ONE_LINE_COUNT do
		local nodeContainer = container:getVarNode("mPositionNode" .. i)
		NodeHelper:setNodeVisible(nodeContainer, false)
		nodeContainer:removeAllChildren()
		
		local itemNode = nil
		local index = baseIndex + i
		
		if index <= #PageInfo.personList then
			itemNode = BattleChatHistoryPageItem.newLineItem(index)
		end
		
		if itemNode then
			nodeContainer:addChild(itemNode)
			NodeHelper:setNodeVisible(nodeContainer, true)
		end
	end
	
end

function BattleChatHistoryPageItem.newLineItem(index)
	--index = index - 1
	local itemNode = ScriptContentBase:create("HeadItem.ccbi", index)
	itemNode:registerFunctionHandler(BattleChatHistoryPageItem.HeadItemFunction)
	
	
	local  itemInfo = PageInfo.personList[index]
	
	itemNode:getVarLabelBMFont("mLv"):setString( "Lv." .. itemInfo.level )
	itemNode:getVarLabelTTF("mName"):setString( itemInfo.name )
	
	local headPic = roleConfig[itemInfo.roleItemId]["icon"]
	
	NodeHelper:setSpriteImage(itemNode, {mPic = headPic})
	itemNode:release()
	return itemNode
end

function BattleChatHistoryPageItem.HeadItemFunction(eventName, container)
	if eventName == "onHand" then
		local index = container:getTag()		
		local itemInfo = PageInfo.personList[index]
		PageManager.viewPlayerInfo(itemInfo.playerId, true);
		--世界聊天点头像不弹出页面
		--PageManager.popPage(thisPageName);		
	end
end

function BattleChatHistoryPageItem.onRefreshItemViewForMsgBox(container)
	local contentId = container:getItemDate().mID
	local baseIndex = (contentId - 1) * PageInfo.ONE_LINE_COUNT
	
	for i = 1, PageInfo.ONE_LINE_COUNT do
		local nodeContainer = container:getVarNode("mPositionNode" .. i)
		NodeHelper:setNodeVisible(nodeContainer, false)
		nodeContainer:removeAllChildren()
		
		local itemNode = nil
		local index = baseIndex + i
		
		if index <= #PageInfo.MsgBoxPersonList then
			itemNode = BattleChatHistoryPageItem.newLineItemForMsgBox(index)
		end
		
		if itemNode then
			nodeContainer:addChild(itemNode)
			NodeHelper:setNodeVisible(nodeContainer, true)
		end
	end
	
end

function BattleChatHistoryPageItem.newLineItemForMsgBox(index)
	--index = index - 1
	local itemNode = ScriptContentBase:create("HeadItem.ccbi", index)
	itemNode:registerFunctionHandler(BattleChatHistoryPageItem.HeadItemFunctionForMsgBox)
	
	
	local  itemInfo = PageInfo.MsgBoxPersonList[index]
	
	itemNode:getVarLabelBMFont("mLv"):setString( "Lv." .. itemInfo.level )
	itemNode:getVarLabelTTF("mName"):setString( itemInfo.name )
	
	local headPic = roleConfig[itemInfo.roleItemId]["icon"]	
	NodeHelper:setSpriteImage(itemNode, {mPic = headPic})
	
	local redPoint = itemNode:getVarSprite("mPoint")
	if redPoint~=nil then
		if ChatManager.hasNewMsgInBoxById(itemInfo.playerId) == true then
			redPoint:setVisible(true)
		else
			redPoint:setVisible(false)
		end
	end
	
	itemNode:release()
	return itemNode
end

function BattleChatHistoryPageItem.HeadItemFunctionForMsgBox(eventName, container)
	if eventName == "onHand" then
		local index = container:getTag()
		
		local itemInfo = PageInfo.MsgBoxPersonList[index]
		
		--标记为已读
		ChatManager.readMsg(itemInfo.playerId)		
		--设置当前的ChatPerson
		ChatManager.setCurrentChatPerson(itemInfo.playerId)	
		if itemInfo.isOffline then
			local Friend_pb = require("Friend_pb")
			local msg = Friend_pb.HPMsgList();
			msg.playerId = itemInfo.playerId			
			common:sendPacket(HP_pb.MESSAGE_LIST_C, msg);
			PageManager.popPage(thisPageName);
			return 
		end
		PageManager.popPage(thisPageName);		
		PageManager.showFightPage();
		PageManager.refreshPage("BattlePage","PrivateChat")
	end
end

function BattleChatHistoryPageBase:onEnter( container )
	NodeHelper:initScrollView(container, "mContent", 6)
	roleConfig = ConfigManager.getRoleCfg()
	UserInfo.sync()
	if m_pageIndex == PAGE_ENUM.CHAT_HISTORY then
		container:getVarMenuItemImage("mMessageBox"):unselected()
		container:getVarMenuItemImage("mBattleChatTitle"):selected()
	else
		container:getVarMenuItemImage("mMessageBox"):selected()
		container:getVarMenuItemImage("mBattleChatTitle"):unselected()
	end
	self:initChatInfo()
	self:rebuildAllItem( container )
	self:refreshPage( container )
end

function BattleChatHistoryPageBase:onMessageBox( container )
	BattleChatHistoryPageBase:onSwitchTab( container ,PAGE_ENUM.CHAT_MSGBOX)	
end

function BattleChatHistoryPageBase:onBattleChatTitle( container )
	BattleChatHistoryPageBase:onSwitchTab( container ,PAGE_ENUM.CHAT_HISTORY)	
end

function BattleChatHistoryPageBase:onSwitchTab( container ,newPageIndex)
	if newPageIndex == PAGE_ENUM.CHAT_HISTORY then
		container:getVarMenuItemImage("mMessageBox"):unselected()
		container:getVarMenuItemImage("mBattleChatTitle"):selected()
	else
		container:getVarMenuItemImage("mMessageBox"):selected()
		container:getVarMenuItemImage("mBattleChatTitle"):unselected()
	end
	
	if m_pageIndex == newPageIndex then
		return;
	end
	
	m_pageIndex = newPageIndex

	self:initChatInfo()
	self:rebuildAllItem( container )
	self:refreshPage( container )
end

function BattleChatHistoryPageBase:refreshPage( container )
	local mTitle = Language:getInstance():getString("@MessageBoxTitle")
	container:getVarLabelBMFont("mTitle"):setString( mTitle )
	--[[if m_pageIndex == PAGE_ENUM.CHAT_HISTORY then
		local mTitle = Language:getInstance():getString("@BattleChatTitle")
		container:getVarLabelBMFont("mTitle"):setString( mTitle )
	else
		local mTitle = Language:getInstance():getString("@MessageBox")
		container:getVarLabelBMFont("mTitle"):setString( mTitle )
	end--]]
	
	
end

function BattleChatHistoryPageBase:initChatInfo()
	--local PackageLogicForLua = require("PackageLogicForLua")
	PageInfo.personList = {}
	PageInfo.MsgBoxPersonList = {}
	personFlagList = {}
	if m_pageIndex == PAGE_ENUM.CHAT_HISTORY then
		local chatList = worldChatList
		local totalNum = #chatList;
		for i = totalNum ,1,-1  do
			local itemInfo = chatList[i]
			if itemInfo:HasField("playerId") and itemInfo.playerId>0 then
				if itemInfo.playerId ~= UserInfo.playerInfo.playerId then
					if personFlagList[itemInfo.playerId]==nil then
						personFlagList[itemInfo.playerId] = true
						table.insert(PageInfo.personList,itemInfo)
					end
					
				end
			end
		end
	else
		local msgBoxList = ChatManager.msgBoxList
		for k,v in pairs(ChatManager.msgBoxList) do	
			local chatUnit = v.chatUnit
			local itemInfo = {}
			itemInfo.level = chatUnit.level
			itemInfo.name = chatUnit.name
			itemInfo.playerId = chatUnit.playerId
			itemInfo.roleItemId = chatUnit.roleItemId
			itemInfo.hasNewMsg = v.hasNewMsg
			itemInfo.isOffline = v.isOffline
			table.insert(PageInfo.MsgBoxPersonList,itemInfo)
		end	
	end
	
end

function BattleChatHistoryPageBase:onExecute( container )
	
end


function BattleChatHistoryPageBase:onExit( container )
	NodeHelper:deleteScrollView( container )
	PageInfo.personList = {}
	personFlagList = {}
	PageInfo.MsgBoxPersonList = {}
end

function BattleChatHistoryPageBase:onClose( container )
	PageManager.popPage(thisPageName)
end

function BattleChatHistoryPageBase:rebuildAllItem( container )
	self:clearAllItem(container)
	self:buildItem(container)
end

function BattleChatHistoryPageBase:clearAllItem( container )
	NodeHelper:clearScrollView(container)
end

function BattleChatHistoryPageBase:buildItem( container )
	local size  =0 
	if m_pageIndex == PAGE_ENUM.CHAT_HISTORY then
		size = math.ceil(#PageInfo.personList / PageInfo.ONE_LINE_COUNT)
	else
		size = math.ceil(#PageInfo.MsgBoxPersonList/ PageInfo.ONE_LINE_COUNT)
	end
	
	NodeHelper:buildScrollView(container, size, "SpeakHistoryItem.ccbi", BattleChatHistoryPageItem.onFunction);
end


function BattleChatHistoryPage_setTabIsHistory(isHistory)
	if isHistory then
		m_pageIndex = PAGE_ENUM.CHAT_HISTORY
	else
		m_pageIndex = PAGE_ENUM.CHAT_MSGBOX
	end
end



--------------------------------------------------------------------
local CommonPage = require("CommonPage")
BattleChatHistoryPage = CommonPage.newSub(BattleChatHistoryPageBase, thisPageName, option);
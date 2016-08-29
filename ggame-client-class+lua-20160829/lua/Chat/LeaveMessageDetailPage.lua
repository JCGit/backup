local Msg_pb = require "Msg_pb"
local UserInfo = require("UserInfo");
local NodeHelper = require("NodeHelper");
local thisPageName = "LeaveMessageDetailPage"

local option = {
	ccbiFile   = "MessageChatPopUp.ccbi",
	handlerMap = {
		onClose 		= "onClose",
		onChat			= "onChat",
		onHelp			= "onHelp",
		onConfirm		= "onConfirm"				
	}
}
----------------------------------------------------
local LeaveMessageDetailPageBase = {}

local LeaveMessageDetailItem = {}

local roleConfig = {}

local PageInfo = {
	playerId = 0,
	playerName = "",
	nameInput = "",
	msgList = {}

}

local mSurplusSecTime = 0

local mDurationTime = 0.0

function LeaveMessageDetailItem.onFunction( eventName , container )
	if eventName == "luaRefreshItemView" then
		LeaveMessageDetailItem.onRefreshItemView( container )		
	end
end

function LeaveMessageDetailItem.onRefreshItemView( container )
	
	local contentId = container:getItemDate().mID
	local itemInfo = PageInfo.msgList[contentId]
	
	local msg = GameMaths:stringAutoReturnForLua( itemInfo.content , GameConfig.Default.LeaveMsgDetailLenCount , 0 )
	local labMsg = container:getVarLabelTTF("mMessage")
	labMsg:setString( msg )
	local labBack = container:getVarScale9Sprite("mScale9Sprite") 
	local x = labBack:getContentSize().width
	local y = labBack:getContentSize().height
	if labBack:getContentSize().width < labMsg:getContentSize().width then	
		x = labMsg:getContentSize().width + 10
	end
	
	if labBack:getContentSize().height < labMsg:getContentSize().height then
		y = labMsg:getContentSize().height + 10
	end
	
	local size = CCSizeMake(x, y )
	labBack:setContentSize(size)
	
	if container:getContentSize().height < y then
		container:setContentSize( CCSizeMake(container:getContentSize().width , y) )
	end
	
	
	local picPath = roleConfig[itemInfo.roleId]["icon"]
	
	container:getVarSprite("mHeroPic"):setTexture( picPath )
	
end

function LeaveMessageDetailPageBase:onEnter( container )
	container:getVarLabelTTF("mName"):setString( PageInfo.playerName )
	UserInfo.sync()
	roleConfig = ConfigManager.getRoleCfg()
	
	NodeHelper:initScrollView(container, "mContent", 10)
	if LeaveMessageDetailPageCacheInfo ~= nil then
        self:onReceivePageInfo(container, LeaveMessageDetailPageCacheInfo)
    end
	self:registerPacket( container )
	self:getPageInfo( container )
    local _str = Language:getInstance():getString("@InputNull")	
    NodeHelper:setStringForTTFLabel(container, { mChat = _str })
    
end

function LeaveMessageDetailPageBase:getPageInfo( container )
	local msg = Msg_pb.HPSeePlayerMsg()
	msg.playerId = PageInfo.playerId
	local pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.SEE_PLAYER_MSG_C, pb_data , #pb_data, true)
end

function LeaveMessageDetailPageBase:onExecute( container )

	if mSurplusSecTime > 0 then
		mDurationTime = mDurationTime + GamePrecedure:getInstance():getFrameTime();
		if mDurationTime > 1.0 then
			mDurationTime = mDurationTime - 1.0
			mSurplusSecTime = mSurplusSecTime - 1
			self:refreshSendBtnStatus( container )
		end
    end
end

function LeaveMessageDetailPageBase:onExit( container )
	self:removePacket(container)
	NodeHelper:deleteScrollView(container)
end

function LeaveMessageDetailPageBase:onChat( container )
    container:registerLibOS()
	Flag_Chat = 1
	libOS:getInstance():showInputbox(false)
end

function LeaveMessageDetailPageBase:onInputboxEnter(container)
	local content = container:getInputboxContent()
	-- todo 检查name合法性
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
	    MessageBoxPage:Msg_Box(common:getLanguageString("@NameHaveForbbidenChar"))
	    content = nil
	    return
	end
	
	content = RestrictedWord:getInstance():filterWordSentence(content);
	if GameMaths:calculateStringCharacters(content) > GameConfig.Default.LeaveMsgCount then
		MessageBoxPage:Msg_Box(common:getLanguageString("@LeaveMsgNum", GameConfig.Default.LeaveMsgCount))
		content = nil
		return
	end

	PageInfo.nameInput = content
	local _str = content
	if GameMaths:calculateStringCharacters(content) > GameConfig.Default.LeaveMsgConfirmCount then
	    _str = GameMaths:stringCutWidthLen( _str , GameConfig.Default.LeaveMsgConfirmCount )
	end
	NodeHelper:setStringForTTFLabel(container, { mChat = _str })
end

function LeaveMessageDetailPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_LEAVEMSG_DETAIL)
end

function LeaveMessageDetailPageBase:onClose( container )
    PageManager.popPage(thisPageName)
end

function LeaveMessageDetailPageBase:onConfirm( container )
	if PageInfo.nameInput == nil or PageInfo.nameInput == "" then
	    MessageBoxPage:Msg_Box_Lan("@InputNull");
	    return
	end
	mSurplusSecTime = GameConfig.Default.ChatCoolTime
	mDurationTime = 0.0
	
	local msg = Msg_pb.HPSendMsgToOthers()
	msg.recvPlayerId = PageInfo.playerId
	msg.content = PageInfo.nameInput
	local pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.SEND_PLAYER_MSG_C, pb_data , #pb_data, false)
	
	local msg = Msg_pb.MsgInfo()
	msg.msgId = #PageInfo.msgList + 1
	msg.sourcePlayerId = UserInfo.playerInfo.playerId
	msg.content = PageInfo.nameInput
	msg.roleId = UserInfo.roleInfo.itemId
	msg.msgTime = "0"
	
	table.insert(PageInfo.msgList , msg)
	--[[
	table.sort(PageInfo.msgList,
	function (e1, e2)
		if not e2 then return true end
		if not e1 then return false end
		
		return e1.msgId < e2.msgId
	end
	)
	]]--
	self:rebuildAllItem( container )
	
	container:getVarLabelTTF("mChat"):setString("")
	PageInfo.nameInput = ""
end

function LeaveMessageDetailPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
    
	if opcode == HP_pb.SEE_PLAYER_MSG_S then
		local msg = Msg_pb.HPSeePlayerMsgRet()
		msg:ParseFromString(msgBuff)
		LeaveMessageDetailPageCacheInfo = msg
		self:onReceivePageInfo(container, msg)
	elseif opcode == HP_pb.NEW_MSG_SYNC_S then
		local msg = Msg_pb.HPNewMsgSyncS()
		msg:ParseFromString(msgBuff)
		self:onReceviceNewMsg( container , msg )
    end	
end

function LeaveMessageDetailPageBase:onReceviceNewMsg( container , msg )
	
	if msg.oneMsg.sourcePlayerId == PageInfo.playerId then
		table.insert(PageInfo.msgList, msg.oneMsg)
		--[[
		table.sort(PageInfo.msgList,
		function (e1, e2)
			if not e2 then return true end
			if not e1 then return false end
		
			return e1.msgId < e2.msgId
		end
		)
		]]--
		self:rebuildAllItem(container)
	end
	
end

function LeaveMessageDetailPageBase:onReceivePageInfo( container , msg )
	PageInfo.msgList = msg.msg
	
	table.sort(PageInfo.msgList,
	function (e1, e2)
		if not e2 then return true end
		if not e1 then return false end
		
		return e1.msgId < e2.msgId
	end
	)
	self:rebuildAllItem(container)
end

function LeaveMessageDetailPageBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function LeaveMessageDetailPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function LeaveMessageDetailPageBase:buildItem(container)
	
	local iMaxNode = container.m_pScrollViewFacade:getMaxDynamicControledItemViewsNum()
	local iCount = 0
	local fOneItemHeight = 0
	local fOneItemWidth = 0

	for i=#PageInfo.msgList, 1, -1 do
		local pItemData = CCReViSvItemData:new_local()
		pItemData.mID = i
		pItemData.m_iIdx = i
		pItemData.m_ptPosition = ccp(0, fOneItemHeight * iCount)

		if iCount < iMaxNode then
			local pItem
			if PageInfo.msgList[i].sourcePlayerId == UserInfo.playerInfo.playerId then
				pItem = ScriptContentBase:create("MessageChatRightContent.ccbi")
			else
				pItem = ScriptContentBase:create("MessageChatLeftContent.ccbi")
			end
			
			pItem:registerFunctionHandler(LeaveMessageDetailItem.onFunction)
			
	
			
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

	local size = CCSizeMake(fOneItemWidth, fOneItemHeight * iCount )
	container.mScrollView:setContentSize(size)
	local sizeHeight = 0
	
    if container.mScrollView:getViewSize().height > container.mScrollView:getContentSize().height * container.mScrollView:getScaleY() then
        sizeHeight = container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()
    end
    
    
	
    --container.mScrollView:setContentOffset(ccp(0, ))
    container.mScrollView:setContentOffset( ccp(0, sizeHeight) )	
    container.m_pScrollViewFacade:setDynamicItemsStartPosition(iCount - 1);
	container.mScrollView:forceRecaculateChildren();
	ScriptMathToLua:setSwallowsTouches(container.mScrollView)
	
end

function LeaveMessageDetailPageBase:refreshItemView( container ,contentId )
	local itemInfo = PageInfo.msgList[contentId]
	
	local msg = GameMaths:stringAutoReturnForLua( itemInfo.content , GameConfig.Default.LeaveMsgDetailLenCount , 0 )
	local labMsg = container:getVarLabelTTF("mMessage")
	labMsg:setString( msg )
	local labBack = container:getVarScale9Sprite("mScale9Sprite") 
	local x = labBack:getContentSize().width
	local y = labBack:getContentSize().height
	if labBack:getContentSize().width < labMsg:getContentSize().width then	
		x = labMsg:getContentSize().width + 10
	end
	
	if labBack:getContentSize().height < labMsg:getContentSize().height then
		y = labMsg:getContentSize().height + 10
	end
	
	local size = CCSizeMake(x, y )
	labBack:setContentSize(size)
	
	local picPath = roleConfig[itemInfo.roleId]["icon"]
	
	container:getVarSprite("mHeroPic"):setTexture( picPath )	
	
	return y
	
end

function LeaveMessageDetailPageBase:refreshSendBtnStatus( container )
	--Language:getInstance:getString("@MsgSend") .. " " ..
	if mSurplusSecTime > 0 then	
		local _str =  mSurplusSecTime .. "s"
		NodeHelper:setStringForTTFLabel(container, { mChat = _str })
		container:getVarMenuItemImage("mChatBtn"):setEnabled(false)		
	else
		local _str = Language:getInstance():getString("@InputNull")
		NodeHelper:setStringForTTFLabel(container, { mChat = _str })
		container:getVarMenuItemImage("mChatBtn"):setEnabled(true)		
	end
	
end

function LeaveMessageDetailPageBase:registerPacket(container)
	container:registerPacket(HP_pb.SEE_PLAYER_MSG_S)
	container:registerPacket(HP_pb.NEW_MSG_SYNC_S)
end

function LeaveMessageDetailPageBase:removePacket(container)
	container:removePacket(HP_pb.SEE_PLAYER_MSG_S)
	container:registerPacket(HP_pb.NEW_MSG_SYNC_S)
end

function LeaveMsgDetail_setPlayId( playerId , playerName)
	PageInfo.playerId = playerId
	PageInfo.playerName = playerName
end
LeaveMessageDetailPageCacheInfo = nil

function LeaveMessageDetailPage_reset()
    LeaveMessageDetailPageCacheInfo = nil
	PageInfo.nameInput = nil
end
-----------------------------------------------------
local CommonPage = require("CommonPage")
LeaveMessagePage = CommonPage.newSub(LeaveMessageDetailPageBase, thisPageName, option);
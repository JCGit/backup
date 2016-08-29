require "Msg_pb"
registerScriptPage("LeaveMessageDetailPage")
local thisPageName = "LeaveMessagePage"
local NodeHelper = require("NodeHelper");
local option = {
	ccbiFile = "MessageBoardPopUp.ccbi",
	handlerMap = {
		onClose 		= "onClose",
		onDeleteAll		= "onDeleteAll",
		onHelp			= "onHelp"				
	}
}

local LeaveMessagePageBase = {}


---------------------------------------------------------------------------------------

local PageType = {
	All = 1,
	Unread = 2	
}

local isDeleteItem = false

local PageInfo = {
	pageType = PageType.All,
	messageList = {},
	container = nil
}

local LeaveMessageItem = {
	--ccbiFile = "MessageBoardContent.ccbi"
}

local roleConfig = {}

function LeaveMessageItem.onFunction( eventName , container )
	if eventName == "luaRefreshItemView" then
		LeaveMessageItem.onRefreshItemView( container )
	elseif eventName == "onChat" then
		LeaveMessageItem.onChat( container )
	elseif eventName == "onDelete" then
		LeaveMessageItem.onDelete( container )
	end
end

function LeaveMessageItem.onRefreshItemView( container )
	local contentId = container:getItemDate().mID
	local itemInfo = PageInfo.messageList[contentId]
    local lines = 0
	local newMessage =GameMaths:stringAutoReturnForLua( itemInfo.lastMsg , GameConfig.Default.LeaveMessageLenCount , lines )
	local tab = Split(newMessage , "\n")
	local str = ""
	if #tab >  GameConfig.Default.LeaveMessageLen then
	    for i = 1 ,GameConfig.Default.LeaveMessageLen,1 do
	        str = str .. tab[i] .. "\n"
	    end
	    str = string.sub(str , 1 ,-8) .. "..."
	else 
	    str = newMessage
	end
	
	
	container:getVarLabelTTF("mName"):setString( itemInfo.playerName )
	container:getVarLabelTTF("mMessage"):setString( str )
	
	--container:getVarSprite("mFlagUnread"):setVisible( itemInfo.hasNewMsg )

	local picPath = roleConfig[itemInfo.roleId]["icon"]
	
	container:getVarSprite("mHeroPic"):setTexture( picPath )
	
end

function LeaveMessageItem.onChat( container )
	local contentId = container:getItemDate().mID
	local itemInfo = PageInfo.messageList[contentId]
	LeaveMsgDetail_setPlayId( itemInfo.playerId , itemInfo.playerName )
	PageManager.pushPage("LeaveMessageDetailPage")
	
end

function LeaveMessageItem.onDelete( container )
	local contentId = container:getItemDate().mID
	local itemInfo = PageInfo.messageList[contentId]
	table.remove(PageInfo.messageList , contentId)
	
	--PageManager.refreshPage(thisPageName)
	isDeleteItem = true
	
	local msg = Msg_pb.HPDeleteOnePlayerMsg()
	msg.playerId = itemInfo.playerId
	local pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.DEL_ONE_PLAYER_MSGS_C, pb_data , #pb_data, false)
	
end


function LeaveMessagePageBase:onEnter( container )
	--self:setTab( container )
	NodeHelper:initScrollView(container, "mContent", 6)
	if LeaveMessagePageCacheInfo ~= nil then
        self:onReceviceMessage(container, LeaveMessagePageCacheInfo)
    end
	self:registerPacket( container )	
	self:getAllMessage( container )
	roleConfig = ConfigManager.getRoleCfg()
end

--[[
function LeaveMessagePageBase:setTab( container )
	if PageInfo.pageType == PageType.All then
		container:getVarMenuItemImage("mAll"):selected()
		container:getVarMenuItemImage("mReadOrNot"):unselected()
	else
		container:getVarMenuItemImage("mAll"):unselected()
		container:getVarMenuItemImage("mReadOrNot"):selected()
	end
end
]]--

function LeaveMessagePageBase:getAllMessage( container )
	local msg = Msg_pb.HPMsgPlayerList()
	pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.MSG_PLAYER_LIST_C, pb_data , #pb_data, true)
end

function LeaveMessagePageBase:rebuildAllItem(container)
	self:clearAllItem(container)
	self:buildItem(container)
end

function LeaveMessagePageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container)
end

function LeaveMessagePageBase:buildItem(container)
	NodeHelper:buildScrollView(container, #PageInfo.messageList, "MessageBoardContent.ccbi", LeaveMessageItem.onFunction);
end


function LeaveMessagePageBase:onExecute( container )
    if isDeleteItem then
        self:rebuildAllItem( container )
        isDeleteItem = false
    end
end

function LeaveMessagePageBase:onExit( container )
	self:removePacket(container)
	NodeHelper:deleteScrollView(container)
	PageInfo.messageList = nil
	isDeleteItem = false
end

function LeaveMessagePageBase:onClose( container )
    PageManager.popPage(thisPageName)
end

function LeaveMessagePageBase:onDeleteAll( container )
	PageInfo.messageList = {}
	LeaveMessagePageCacheInfo = {}
	self:rebuildAllItem(container)
	
	local msg = Msg_pb.HPDeleteAllMsg()
	local pb_data = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.DEL_ALL_PLAYER_MSGS_C, pb_data , #pb_data, false)
	
end

function LeaveMessagePageBase:onHelp( container )
	--Help_SetHelpConfig(GameConfig.HelpKey.HELP_ARENA)
	--PageManager.pushPage("HelpPage")
	PageManager.showHelp(GameConfig.HelpKey.HELP_LEAVEMSG)
end

function LeaveMessagePageBase:onReceivePacket( container )
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == HP_pb.MSG_PLAYER_LIST_S then
		local msg = Msg_pb.HPMsgPlayerListRet()
		msg:ParseFromString(msgBuff)
		LeaveMessagePageCacheInfo = msg
		self:onReceviceMessage( container , msg )
	
	end
end

function LeaveMessagePageBase.onGameMessage(container)
	if container:getMessage():getTypeId() == MSG_MAINFRAME_REFRESH then	
		--判断是不是本页面
		local msg = MsgMainFrameRefreshPage:getTrueType(container:getMessage());	
		if msg.pageName == thisPageName then
			self:rebuildAllItem( container )
		end
	end
end	

function LeaveMessagePageBase:onReceviceMessage( container , msg )
	PageInfo.messageList = {}
	PageInfo.messageList = msg.itemInfo
	
	table.sort(PageInfo.messageList,
	function (e1, e2)
		if not e2 then return true end
		if not e1 then return false end
		
		return e1.lastMsgTime > e2.lastMsgTime
	end
	)
	
	self:rebuildAllItem( container )
	
end

function LeaveMessagePageBase:registerPacket(container)
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	container:registerPacket(HP_pb.MSG_PLAYER_LIST_S)
end

function LeaveMessagePageBase:removePacket(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	container:removePacket(HP_pb.MSG_PLAYER_LIST_S)
end

LeaveMessagePageCacheInfo = nil

function LeaveMessagePage_reset(container)
	LeaveMessagePageCacheInfo = nil
end
---------------------------------------------------------
local CommonPage = require("CommonPage")
local LeaveMessagePage = CommonPage.newSub(LeaveMessagePageBase, thisPageName, option)
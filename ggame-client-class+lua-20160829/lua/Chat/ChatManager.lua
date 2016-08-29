local ChatManager = {}

--消息盒子list
--msgBoxList
--key is player id, value is structure:
	--[[	
	{
		bool isOffline,
		FriendMsg chatMsg,	limit in 50 msg
		MsgBoxUnit chatUnit,
		bool hasNewMsg 
	}
	message MsgBoxUnit
{
	required int32  playerId = 1;
	required string name = 2;
	required int32  level = 3;
	required int32  roleItemId = 4;
}
//消息基本结构
message FriendMsg 
{
	required int32 senderId = 1;
	required int32 receiveId = 2;
	required string senderName = 3;
	required string receiveName = 4;
	required FriendChatMsgType msgType = 5;
	required string message = 6;
}
	--]]
ChatManager.msgBoxList = {}

ChatManager.shieldPlayerList = {}

ChatManager.curChatPerson = {}
--playerId is the unique key 
--chatUnit is the player info 
--chatMsg is the detailed chat msg
--isOffline is distinguish for the offline msg and online msg

--use case 1, add into msg box only, no msg and offline is true,used in recieve offline msg box info
--ChatManager.insertPrivateMsg(playerid,chatUnit,nil,true)

--use case 2, add into msg box only, no msg and offline is false, used in ViewPlayerInfoPage
--ChatManager.insertPrivateMsg(playerid,chatUnit,nil,false)

--use case 3, add unit and msg into msg box , used in recieve push msg online
--ChatManager.insertPrivateMsg(playerid,chatUnit,chatMsg,false)

--use case 4, send msglistinfo_c, got the HPMsgListInfo, add into msgBoxList withOut charUnit
--ChatManager.insertPrivateMsg(playerid,nil,chatMsg,false)

--use case 5, send one private, manually add into msgBoxList withOut charUnit
--ChatManager.insertPrivateMsg(ChatManager.curChatPerson.chatUnit.playerId,nil,friendMsg,false,false)

function ChatManager.insertPrivateMsg(playerId,chatUnit,chatMsg,isOffline,hasNewMsg)
	local chatPersonSize = GameConfig.MsgBoxMaxSize	
	local currentBoxSize = common:table_count(ChatManager.msgBoxList)
	if  currentBoxSize > chatPersonSize then
		table.remove(ChatManager.msgBoxList,1)
	end		
	
	hasNewMsg = hasNewMsg == nil and true or false
	local key = playerId
	
	--[[	if ChatManager.isInShield(playerId) then
			return 
		end--]]
	local maxSize = GameConfig.ChatMsgMaxSize;
	if ChatManager.msgBoxList[key] == nil then
		--if is nil
		local value = {}
		if chatUnit==nil then
			CCLuaLog("Error in ChatManager.insertPrivateMsg chatUnit == nil")
			return 
		end
		value.chatUnit = chatUnit
		value.msgList = {}	
		value.isOffline = isOffline
		value.hasNewMsg = hasNewMsg
		if chatMsg ~= nil then 
			table.insert(value.msgList,chatMsg);		
		end
		
		ChatManager.msgBoxList[key] = value
	else
		--if has data in it, judge currentSize is exceed limit
		local value = ChatManager.msgBoxList[key] 
		value.isOffline = isOffline	
		if value.chatUnit == nil or playerId ~= value.chatUnit.playerId then
			CCLuaLog("Error in ChatManager.insertPrivateMsg, value.chatUnit == nil or playerId ~= value.chatUnit.playerId")
			return 
		end
		value.hasNewMsg = hasNewMsg
		if chatMsg ~= nil then 
			if #value.msgList > maxSize then
				table.remove(value.msgList,1)
			end				
			table.insert(value.msgList,chatMsg);		
		end	
	end
	
end	

function ChatManager.getMsgBoxSize()
	local currentBoxSize = common:table_count(ChatManager.msgBoxList)
	return currentBoxSize
end

function ChatManager.getCurrentChatId()
	local playerId = ChatManager.curChatPerson.chatUnit == nil and 0 or ChatManager.curChatPerson.chatUnit.playerId
	return playerId
end

function ChatManager.setCurrentChatPerson(playerId)
	local msgBox = ChatManager.msgBoxList[playerId]
	if msgBox~=nil then
		ChatManager.curChatPerson = msgBox
	else
		CCLuaLog("ERROR in setCurrentChatPerson playerId"..playerId)
	end
	
end

---------offline related---------------

function ChatManager.isOfflineMsg(playerId)
	local msgBox = ChatManager.msgBoxList[playerId]
	if msgBox~=nil then
		local flag = false
		if msgBox.isOffline == nil then
			flag = false
		else 
			flag = msgBox.isOffline
		end
		return flag
	else
		return false
	end
end

---------new msg related---------------

function ChatManager.readMsg(playerId)
	local msgBox = ChatManager.msgBoxList[playerId]
	if msgBox~=nil then
		msgBox.hasNewMsg = false
	end
end

function ChatManager.hasNewMsgInBoxById(playerId)
	local msgBox = ChatManager.msgBoxList[playerId]
	if msgBox~=nil and msgBox.hasNewMsg ~=nil then
		local currentPlayerId = ChatManager.curChatPerson.chatUnit == nil and 0 or ChatManager.curChatPerson.chatUnit.playerId
		if playerId ~= currentPlayerId and msgBox.hasNewMsg == true then
			return true
		else
			return false
		end			
	else
		return false
	end
end

function ChatManager.hasNewMsgInBox()
	if ChatManager.msgBoxList == nil  then return false end
	for k,v in pairs(ChatManager.msgBoxList) do		
		local currentPlayerId = ChatManager.curChatPerson.chatUnit == nil and 0 or ChatManager.curChatPerson.chatUnit.playerId
		local id = v.chatUnit.playerId 
		if v.hasNewMsg == true and id ~= currentPlayerId then
			return true,id
		end
	end	
	return false,0
end

function ChatManager.hasNewMsgWithoutCur()
	if ChatManager.msgBoxList == nil  then return false end
	for k,v in pairs(ChatManager.msgBoxList) do			
		if v.hasNewMsg == true then
			return true
		end
	end	
	return false
end


---------shield related---------------

function ChatManager.addShieldList(playerId)
	ChatManager.shieldPlayerList[playerId] = true
end

function ChatManager.removeShieldList(playerId)
	ChatManager.shieldPlayerList[playerId] = false
end

function ChatManager.recieveShieldList(list)
	ChatManager.shieldPlayerList = {}
	if #list <=0 then
		return 
	end
	for i=1,#list do
		local playerId = list[i]
		ChatManager.shieldPlayerList[playerId] = true
	end				
end

function ChatManager.isInShield(playerId)
	
	if playerId == nil then return false end
	if ChatManager.shieldPlayerList == nil or common:table_count(ChatManager.shieldPlayerList) == 0 then return false end
	if ChatManager.shieldPlayerList[playerId] ~=nil then
		if ChatManager.shieldPlayerList[playerId] ==true then
			return true
		else
			return false
		end
	else
		return false
	end
end

function ChatManager_reset()
	ChatManager.shieldPlayerList = {}
	ChatManager.msgBoxList = {}
	ChatManager.curChatPerson = {}
end
--[[
function ChatManager_little_reset()
    ChatManager.shieldPlayerList = {}
	ChatManager.msgBoxList = {}
	ChatManager.curChatPerson = {}
end
]]--
return ChatManager;
----------------------------------------------------------------------------------
--[[
	FILE:			ListenMessage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	添加全局监听消息
	AUTHOR:			hgs
	CREATED:		2014-08-03
--]]
----------------------------------------------------------------------------------
local HP_pb = require("HP_pb");
local Const_pb = require("Const_pb");
local UserInfo = require("UserInfo");

function onReceivePacket(opcode)
	if opcode == HP_pb.PLAYER_INFO_SYNC_S then
		UserInfo.syncPlayerInfo();
		if PageManger then
			PageManger.refreshPage('MainScenePage');
		end
	elseif opcode == HP_pb.ROLE_INFO_SYNC_S then
		UserInfo.sync();
		--UserEquipManager:check();
	end
end

function onRefreshNoticePoint(nType)
	if nType == Const_pb.NEW_MAIL then
		--邮件红点开启
		NoticePointState.MAIL_POINT_OPEN = true;
	elseif nType == GameConfig.NewPointType.TYPE_MAIL_CLOSE then
		--邮件红点关闭
		NoticePointState.MAIL_POINT_OPEN = false;
	-- 礼包红点
	elseif nType == Const_pb.GIFT_NEW_MSG then
		NoticePointState.GIFT_NEW_MSG = true
	elseif nType == GameConfig.NewPointType.TYPE_GIFT_NEW_CLOSE then
		NoticePointState.GIFT_NEW_MSG = false
	-- 公会boss开启红点
	elseif nType == Const_pb.ALLIANCE_BOSS_OPEN then
		NoticePointState.ALLIANCE_BOSS_OPEN = true
	elseif nType == GameConfig.NewPointType.TYPE_ALLIANCE_NEW_CLOSE then
		NoticePointState.ALLIANCE_BOSS_OPEN = false
	elseif nType == Const_pb.NEW_MSG then
	
	elseif nType == GameConfig.NewPointType.TYPE_CHAT_MESSAGE_CLOSE then
	
	elseif nType == Const_pb.TEAM_BATTLE_SIGNUP then	
		NoticePointState.REGINMENTWAR_POINT = true
	elseif nType == GameConfig.NewPointType.TYPE_RegimentWar_NEW_CLOSE then
		NoticePointState.REGINMENTWAR_POINT = false
	end

	NoticePointState.isChange = true;
end

--消息处理
function onListenMsg(eventName, gameMsg)
	local typeId = gameMsg:getTypeId();
	if typeId == MSG_SEVERINFO_UPDATE then
		local opcode = MsgSeverInfoUpdate:getTrueType(gameMsg).opcode;
		onReceivePacket(opcode);
	elseif typeId == MSG_GET_NEW_INFO then
		local nType = MsgMainFrameGetNewInfo:getTrueType(gameMsg).type;
		onRefreshNoticePoint(nType);
		if nType == Const_pb.ALLIANCE_BOSS_OPEN then
			local msg = MsgMainFrameRefreshPage:new()
			msg.pageName = 'GuildPage'
			MessageManager:getInstance():sendMessageForScript(msg)
		end
	elseif typeId == MSG_MAINFRAME_PUSHPAGE then
		GameUtil:hideTip();
	end
end

msgHandler = MessageScriptHandler:new(onListenMsg);

function validateAndRegisterMsgHandler()
	CCLuaLog("validateAndRegisterMsgHandler()");
	msgHandler:registerFunctionHandler(onListenMsg)
end

MessageManager:getInstance():regisiterMessageHandler(MSG_SEVERINFO_UPDATE, msgHandler);
MessageManager:getInstance():regisiterMessageHandler(MSG_GET_NEW_INFO, msgHandler);
MessageManager:getInstance():regisiterMessageHandler(MSG_MAINFRAME_PUSHPAGE, msgHandler);

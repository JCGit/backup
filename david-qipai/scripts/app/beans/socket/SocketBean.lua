local SocketService = require("app.core.socket.SocketService")
local PacketReader = require("app.core.socket.PacketReader")
local Packet = require("app.core.socket.Packet")
local PacketType = require("app.core.socket.PacketType")
local SocketEvent = require("app.core.socket.SocketEvent")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local PlayStatus = require("app.beans.table.model.PlayStatus")
local SocketBeanEvent = import(".event.SocketBeanEvent")
local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local BroadcastBeanEvent = require("app.beans.broadcast.event.BroadcastBeanEvent")
local Protocol = import(".model.Protocol")
local SocketBean = class("SocketBean")
function SocketBean:ctor()
  self._reader = PacketReader.new()
  self._socket = SocketService.new("PokDengSocket", self._reader)
  self._gameLink = 0
  self._gameHash = 0
  self._broadcastLink = 0
  self._broadcastHash = 0
  self:addEvents()
  if DEBUG_RECORD_MODE then
    g.record_socketService = self._socket
  end
end
function SocketBean:addEvents()
  g.eventCenter:addEventListener(SocketBeanEvent.EVT_CONNECT, handler(self, self._onConnect))
  g.eventCenter:addEventListener(SocketBeanEvent.EVT_DISCONNECT, handler(self, self._onDisconnect))
  g.eventCenter:addEventListener(SocketBeanEvent.EVT_SEND, handler(self, self._onSend))
  g.eventCenter:addEventListener(SocketBeanEvent.EVT_CONNECT_GAME, handler(self, self._onConnectGame))
  g.eventCenter:addEventListener(SocketBeanEvent.EVT_CONNECT_BROADCAST, handler(self, self._onConnectBroadcast))
  g.eventCenter:addEventListener(SocketBeanEvent.EVT_CLOSE_GAME, handler(self, self._onCloseGame))
  self._socket:addEventListener(SocketEvent.EVT_CONNECT_SUCCESS, handler(self, self._onConnectSuccess))
  self._socket:addEventListener(SocketEvent.EVT_CONNECT_FAIL, handler(self, self._onConnectFail))
  self._socket:addEventListener(SocketEvent.EVT_CLOSED, handler(self, self._onClosed))
  self._socket:addEventListener(SocketEvent.EVT_CLOSE, handler(self, self._onClose))
  self._socket:addEventListener(SocketEvent.EVT_ERROR, handler(self, self._onError))
  self._socket:addEventListener(SocketEvent.EVT_PACKET_RECEIVED, handler(self, self._onPacketReceived))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_LOGIN, handler(self, self._requestLogin))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_LOGOUT, handler(self, self._requestLogout))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_SIT, handler(self, self._requestSit))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_BET, handler(self, self._requestBet))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_DEAL, handler(self, self._requestDeal))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_SPEAK, handler(self, self._requestSpeak))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_CHALLENGE, handler(self, self._requestChallenge))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_STAND, handler(self, self._requestStand))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_AUTO_BUYIN, handler(self, self._requestAutoBuyIn))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_CANCEL_AUTO_BUYIN, handler(self, self._requestCancelAutoBuyIn))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_SEND_EMOJI, handler(self, self._requestSendEmoji))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_SEND_CHAT, handler(self, self._requestSendChat))
  g.eventCenter:addEventListener(TableBeanEvent.EVT_CLI_REQUEST_SEND_CHIP, handler(self, self._requestSendChip))
  g.eventCenter:addEventListener(BroadcastBeanEvent.EVT_CLI_LOGIN, handler(self, self._requestLoginBroadcast))
  g.eventCenter:addEventListener(BroadcastBeanEvent.EVT_CLI_LOGOUT, handler(self, self._requestLogoutBroadcast))
  g.eventCenter:addEventListener(BroadcastBeanEvent.EVT_CLI_TRANSMIT_MULTICAST, handler(self, self._requestTransmitMulticast))
  g.eventCenter:addEventListener(BroadcastBeanEvent.EVT_CLI_TRANSMIT_BROADCAST, handler(self, self._requestTransmitBroadcast))
end
function SocketBean:removeEvents()
end
function SocketBean:_requestLoginBroadcast(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_LOGIN_BROADCAST)
    packet:setLink(self._broadcastLink)
    packet:writeUnsignedInt(data.uid)
    packet:writeString(data.loginKey)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_LOGIN_BROADCAST))
  end
end
function SocketBean:_requestLogoutBroadcast(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_LOGOUT_BROADCAST)
    packet:setLink(self._broadcastLink)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_LOGOUT_BROADCAST))
  end
end
function SocketBean:_requestTransmitMulticast(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_TRANSMIT_MULTICAST)
    packet:setLink(self._broadcastLink)
    packet:writeUnsignedInt(data.uid)
    packet:writeString(data.userName)
    packet:writeUnsignedShort(data.userCount)
    for i = 1, data.userCount do
      packet:writeUnsignedInt(data.toUids[i])
    end
    packet:writeString(data.message)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_TRANSMIT_MULTICAST))
  end
end
function SocketBean:_requestTransmitBroadcast(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_TRANSMIT_BROADCAST)
    packet:setLink(self._broadcastLink)
    packet:writeUnsignedInt(data.uid)
    packet:writeString(data.userName)
    packet:writeString(data.message)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_TRANSMIT_BROADCAST))
  end
end
function SocketBean:_requestLogin(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_REQUEST_LOGIN)
    packet:setLink(self._gameLink)
    packet:writeUnsignedInt(data.uid)
    packet:writeString(data.loginKey)
    packet:writeUnsignedInt(data.tid)
    packet:writeByte(data.isCreate)
    packet:writeString(data.sPic)
    packet:writeUnsignedShort(data.giftId)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_LOGIN))
  end
end
function SocketBean:_requestLogout(evt)
  local packet = Packet.new(PacketType.WRITE)
  packet:setCommand(Protocol.CLI_REQUEST_LOGOUT)
  packet:setLink(self._gameLink)
  self._socket:send(packet)
  logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_LOGOUT))
end
function SocketBean:_requestSit(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_REQUEST_SIT)
    packet:setLink(self._gameLink)
    packet:writeUnsignedByte(data.seatId)
    packet:writeUnsignedInt64(data.buyInChips)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_SIT))
  end
end
function SocketBean:_requestBet(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_REQUEST_BET)
    packet:setLink(self._gameLink)
    packet:writeUnsignedInt64(data.bet)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_BET))
  end
end
function SocketBean:_requestDeal(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_REQUEST_DEAL)
    packet:setLink(self._gameLink)
    packet:writeUnsignedByte(data.direction)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_DEAL))
  end
end
function SocketBean:_requestSpeak(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_REQUEST_SPEAK)
    packet:setLink(self._gameLink)
    packet:writeUnsignedByte(data.operation)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_SPEAK))
  end
end
function SocketBean:_requestChallenge(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_REQUEST_CHALLENGE)
    packet:setLink(self._gameLink)
    packet:writeUnsignedByte(data.seatId)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_CHALLENGE))
  end
end
function SocketBean:_requestStand(evt)
  local packet = Packet.new(PacketType.WRITE)
  packet:setCommand(Protocol.CLI_REQUEST_STAND)
  packet:setLink(self._gameLink)
  self._socket:send(packet)
  logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_STAND))
end
function SocketBean:_requestAutoBuyIn(evt)
  local data = evt.data
  if data ~= nil then
    local packet = Packet.new(PacketType.WRITE)
    packet:setCommand(Protocol.CLI_REQUEST_AUTO_BUYIN)
    packet:setLink(self._gameLink)
    packet:writeUnsignedInt64(data.buyInChips)
    self._socket:send(packet)
    logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_AUTO_BUYIN))
  end
end
function SocketBean:_requestCancelAutoBuyIn(evt)
  local packet = Packet.new(PacketType.WRITE)
  packet:setCommand(Protocol.CLI_REQUEST_CANCEL_AUTO_BUYIN)
  packet:setLink(self._gameLink)
  self._socket:send(packet)
  logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_CANCEL_AUTO_BUYIN))
end
function SocketBean:_requestSendEmoji(evt)
  local data = evt.data
  local packet = Packet.new(PacketType.WRITE)
  packet:setCommand(Protocol.CLI_REQUEST_SEND_EMOJI)
  packet:setLink(self._gameLink)
  packet:writeUnsignedByte(data.emojiType)
  packet:writeUnsignedByte(data.emojiIndex)
  self._socket:send(packet)
  logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_SEND_EMOJI))
end
function SocketBean:_requestSendChat(evt)
  local data = evt.data
  local packet = Packet.new(PacketType.WRITE)
  packet:setCommand(Protocol.CLI_REQUEST_SEND_CHAT)
  packet:setLink(self._gameLink)
  packet:writeString(data.chatMessage)
  self._socket:send(packet)
  logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_SEND_CHAT))
end
function SocketBean:_requestSendChip(evt)
  local data = evt.data
  local packet = Packet.new(PacketType.WRITE)
  packet:setCommand(Protocol.CLI_REQUEST_SEND_CHIP)
  packet:setLink(self._gameLink)
  packet:writeUnsignedInt(data.targetUid)
  packet:writeUnsignedInt64(data.sendChips)
  self._socket:send(packet)
  logger:debug("Send,command=" .. tostring(Protocol.CLI_REQUEST_SEND_CHIP))
end
function SocketBean:_onConnect(evt)
  local data = evt.data
  if data ~= nil then
    if data.host and data.host ~= "" and data.port and data.port > 0 and data.port < 65536 then
      if self._socket:isConnected() then
        logger:debug("Allready connected.")
        g.eventCenter:dispatchEvent({
          name = SocketBeanEvent.EVT_CONNECT_SUCCESS
        })
      else
        self._socket:connect(data.host, data.port, false)
      end
    else
      logger:debug("Illegal host or port")
    end
  end
end
function SocketBean:_onDisconnect(evt)
  logger:debug("SocketBean:_onDisconnect")
  if self._socket and self._socket:isConnected() then
    self._socket:disconnect()
  end
  g.eventCenter:dispatchEvent({
    name = SocketBeanEvent.EVT_CLOSED
  })
end
function SocketBean:_onConnectGame(evt)
  self._gameHash = math.random(1, 2147483647)
  while self._gameHash == self._broadcastHash do
    self._gameHash = math.random(1, 2147483647)
  end
  self:_connectBackend(evt.data, self._gameHash)
end
function SocketBean:_onConnectBroadcast(evt)
  self._broadcastHash = math.random(1, 2147483647)
  while self._gameHash == self._broadcastHash do
    self._broadcastHash = math.random(1, 2147483647)
  end
  self:_connectBackend(evt.data, self._broadcastHash)
end
function SocketBean:_onCloseGame(evt)
  self:_closeBackend(self._gameLink)
  self._gameLink = 0
  self._gameHash = 0
end
function SocketBean:_closeBackend(link)
  local packet = Packet.new(PacketType.WRITE)
  packet:setCommand(Protocol.CLI_CLOSE_BACKEND)
  packet:setLink(link)
  self._socket:send(packet)
  logger:debug("Send,command=" .. tostring(Protocol.CLI_CLOSE_BACKEND))
end
function SocketBean:_connectBackend(data, hash)
  if data ~= nil then
    if data.host and data.host ~= "" and data.port and data.port > 0 and data.port < 65536 then
      local packet = Packet.new(PacketType.WRITE)
      packet:setCommand(Protocol.CLI_CONNECT_BACKEND)
      packet:writeString(data.host)
      packet:writeUnsignedShort(data.port)
      math.newrandomseed()
      packet:writeInt(hash)
      self._socket:send(packet)
      logger:debug("Send,command=" .. tostring(Protocol.CLI_CONNECT_BACKEND))
    else
      logger:debug("Illegal host or port")
    end
  end
end
function SocketBean:_onSend(evt)
  logger:debug("Send")
  local packet = evt.data
  if packet ~= nil then
    self._socket:send(packet)
  end
end
function SocketBean:_onConnectSuccess(evt)
  logger:debug("ConnectSuccess")
  g.eventCenter:dispatchEvent({
    name = SocketBeanEvent.EVT_CONNECT_SUCCESS
  })
  if self._heartbeatHandle == nil then
    self._heartbeat = 0
    self._sequence = g.storage:getInt(g.vars.storageKeys.proxyHeartbeatSeq, 0) + 1
    self._heartbeatHandle = scheduler.scheduleGlobal(handler(self, self.requestHeartbeat), 20)
  end
end
function SocketBean:requestHeartbeat()
  local packet = Packet.new(PacketType.WRITE)
  packet:setCommand(Protocol.CLI_HEART_BEAT)
  packet:writeInt(self._sequence)
  packet:writeUnsignedInt64(math.floor(g.util.functions.getTime() * 1000))
  self._socket:send(packet)
  self._sequence = self._sequence + 1
  self._heartbeat = self._heartbeat + 1
  g.storage:setInt(g.vars.storageKeys.proxyHeartbeatSeq, self._sequence)
  if not self._hasLogPing and self._heartbeat >= 2 then
    g.dataLogger:logPing(20000)
  end
end
function SocketBean:_onConnectFail(evt)
  logger:debug("ConnectFail")
  g.eventCenter:dispatchEvent({
    name = SocketBeanEvent.EVT_CONNECT_FAILURE
  })
end
function SocketBean:_onClosed(evt)
  logger:debug("ConnectClosed")
  g.eventCenter:dispatchEvent({
    name = SocketBeanEvent.EVT_CLOSED
  })
end
function SocketBean:_onClose(evt)
end
function SocketBean:_onError(evt)
end
function SocketBean:_onPacketReceived(evt)
  local packet = evt.data
  if packet ~= nil then
    local command = packet:getCommand()
    logger:debug("PacketReceived,command=" .. tostring(command))
    if DEBUG_RECORD_MODE and not g.record_isNowReplaying then
      if command == Protocol.SVR_LOGIN_SUCCESS then
        local tableDir = g.util.functions.getTime()
        local currentRoundDirName = device.writablePath .. "tablelist" .. "/" .. tableDir
        if not io.exists(device.writablePath .. "tablelist") then
          lfs.mkdir(device.writablePath .. "tablelist")
        end
        if not io.exists(currentRoundDirName) then
          lfs.mkdir(currentRoundDirName)
        end
        g.record_table_dirName = currentRoundDirName
      end
      if g.record_savingPacketData and g.record_table_dirName then
        local fileName = g.util.functions.getTime()
        io.writefile(g.record_table_dirName .. "/" .. fileName, g.record_savingPacketData)
        g.record_savingPacketData = nil
      end
    end
    if not DEBUG_RECORD_MODE or g.record_isNowReplaying then
    end
    if command == Protocol.SVR_LOGIN_SUCCESS then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_LOGIN_SUCCESS,
        data = self:readLoginSuccess(packet)
      })
    elseif command == Protocol.SVR_LOGIN_FAIL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_LOGIN_FAIL,
        data = self:readLoginFail(packet)
      })
    elseif command == Protocol.SVR_LOGOUT_SUCCESS then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_LOGOUT_SUCCESS,
        data = self:readLogoutSuccess(packet)
      })
      self:_onCloseGame(nil)
    elseif command == Protocol.BROADCAST_SIT then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_SIT,
        data = self:readBroadcastSit(packet)
      })
    elseif command == Protocol.SVR_SIT_FAIL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_SIT_FAIL,
        data = self:readSitFail(packet)
      })
    elseif command == Protocol.BROADCAST_GAME_START then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_GAME_START,
        data = self:readGameStart(packet)
      })
    elseif command == Protocol.BROADCAST_BET_TURN then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_BET_TURN,
        data = self:readBetTurn(packet)
      })
    elseif command == Protocol.BROADCAST_BET then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_BET,
        data = self:readBroadcastBet(packet)
      })
    elseif command == Protocol.SVR_BET_FAIL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_BET_FAIL,
        data = self:readBetFail(packet)
      })
    elseif command == Protocol.BROADCAST_BANKER_DEAL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_BANKER_DEAL,
        data = self:readBankerDeal(packet)
      })
    elseif command == Protocol.BROADCAST_DEAL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_DEAL,
        data = self:readBroadcastDeal(packet)
      })
    elseif command == Protocol.SVR_DEAL_FAIL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_DEAL_FAIL,
        data = self:readDealFail(packet)
      })
    elseif command == Protocol.BROADCAST_POK then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_POK,
        data = self:readBroadcastPok(packet)
      })
    elseif command == Protocol.BROADCAST_SPEAK_TURN then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_SPEAK_TURN,
        data = self:readSpeakTurn(packet)
      })
    elseif command == Protocol.BROADCAST_SPEAK then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_SPEAK,
        data = self:readBroadcastSpeak(packet)
      })
    elseif command == Protocol.SVR_SPEAK_FAIL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_SPEAK_FAIL,
        data = self:readSpeakFail(packet)
      })
    elseif command == Protocol.BROADCAST_BANKER_TURN then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_BANKER_TURN,
        data = self:readBankerTurn(packet)
      })
    elseif command == Protocol.BROADCAST_CHALLENGE then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_CHALLENGE,
        data = self:readBroadcastChallenge(packet)
      })
    elseif command == Protocol.SVR_CHALLENGE_FAIL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_CHALLENGE_FAIL,
        data = self:readChallengeFail(packet)
      })
    elseif command == Protocol.BROADCAST_GAME_OVER then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_GAME_OVER,
        data = self:readGameOver(packet)
      })
    elseif command == Protocol.BROADCAST_STAND then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_STAND,
        data = self:readBroadcastStand(packet)
      })
    elseif command == Protocol.SVR_STAND_FAIL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_STAND_FAIL,
        data = self:readStandFail(packet)
      })
    elseif command == Protocol.BROADCAST_EMOJI then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_EMOJI,
        data = self:readBroadcastEmoji(packet)
      })
    elseif command == Protocol.BROADCAST_CHAT then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_CHAT,
        data = self:readBroadcastChat(packet)
      })
    elseif command == Protocol.BROADCAST_INTERACTION_EQUIP then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_INTER_EQUIP,
        data = self:readBroadcastInteractionEquip(packet)
      })
    elseif command == Protocol.BROADCAST_ADD_FRIEND then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_ADD_FRIEND,
        data = self:readBroadcastAddFriend(packet)
      })
    elseif command == Protocol.BROADCAST_SENDCHIP_SUCCESS then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_SENDCHIP_SUCCESS,
        data = self:readBroadcastSendChipSuccess(packet)
      })
    elseif command == Protocol.SVR_SENDCHIP_FAIL then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_SENDCHIP_FAIL,
        data = self:readSVRSendChipFail(packet)
      })
    elseif command == Protocol.SVR_AUTO_BUYIN_SUCCESS then
    elseif command == Protocol.SVR_AUTO_BUYIN_FAIL then
    elseif command == Protocol.SVR_CANCEL_AUTO_BUYIN_SUCCESS then
    elseif command == Protocol.SVR_CANCEL_AUTO_BUYIN_FAIL then
    elseif command == Protocol.BROADCAST_BANKER_ESCAPE then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_BANKER_ESCAPE,
        data = self:readBankerEscape(packet)
      })
    elseif command == Protocol.SVR_CONNECT_BACKEND_SUCCESS then
      local link = packet:readUnsignedByte()
      local hash = packet:readInt()
      if hash == self._gameHash then
        self._gameLink = link
        g.eventCenter:dispatchEvent({
          name = SocketBeanEvent.EVT_CONNECT_GAME_SUCCESS
        })
      elseif hash == self._broadcastHash then
        self._broadcastLink = link
        g.eventCenter:dispatchEvent({
          name = SocketBeanEvent.EVT_CONNECT_BROADCAST_SUCCESS
        })
      end
    elseif command == Protocol.SVR_LOGIN_BROADCAST_SUCCESS then
      g.eventCenter:dispatchEvent({
        name = BroadcastBeanEvent.EVT_SVR_LOGIN_SUCCESS
      })
    elseif command == Protocol.SVR_LOGIN_BROADCAST_FAIL then
      g.eventCenter:dispatchEvent({
        name = BroadcastBeanEvent.EVT_SVR_LOGIN_FAIL,
        data = self:readLoginBroadcastFail(packet)
      })
    elseif command == Protocol.SVR_LOGOUT_BROADCAST_SUCCESS then
      g.eventCenter:dispatchEvent({
        name = BroadcastBeanEvent.EVT_SVR_LOGOUT_SUCCESS,
        data = self:readLogoutBroadcastSuccess(packet)
      })
    elseif command == Protocol.SVR_TRANSMIT_MULTICAST then
      g.eventCenter:dispatchEvent({
        name = BroadcastBeanEvent.EVT_SVR_TRANSMIT_MULTICAST,
        data = self:readTransmitMulticast(packet)
      })
    elseif command == Protocol.SVR_TRANSMIT_BROADCAST then
      g.eventCenter:dispatchEvent({
        name = BroadcastBeanEvent.EVT_SVR_TRANSMIT_BROADCAST,
        data = self:readTransmitBroadcast(packet)
      })
    elseif command == Protocol.SVR_SYSTEM_MULTICAST then
      local data = self:readSystemMulticast(packet)
      g.eventCenter:dispatchEvent({
        name = BroadcastBeanEvent.EVT_SVR_SYSTEM_MULTICAST,
        data = data
      })
    elseif command == Protocol.SVR_SYSTEM_BROADCAST then
      g.eventCenter:dispatchEvent({
        name = BroadcastBeanEvent.EVT_SVR_SYSTEM_BROADCAST,
        data = self:readSystemBroadcast(packet)
      })
    elseif command == Protocol.BROADCAST_SEND_GIFT then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_SEND_GIFT,
        data = self:readSendGift(packet)
      })
    elseif command == Protocol.BROADCAST_APPLY_GIFT then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_APPLY_GIFT,
        data = self:readApplyGift(packet)
      })
    elseif command == Protocol.SVR_USER_BANKRUPT then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_SVR_USER_BANKRUPT,
        data = self:readUserBankrupt(packet)
      })
    elseif command == Protocol.BROADCAST_SERVER_UPDATE then
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_BROADCAST_SERVER_UPDATE,
        data = self:readServerUpdate(packet)
      })
    elseif command == Protocol.SVR_HEART_BEAT and not self._hasLogPing then
      self._hasLogPing = true
      local sequence = packet:readInt()
      if sequence and sequence + 1 >= self._sequence then
        local timestamp = packet:readUnsignedInt64()
        local now = math.ceil(g.util.functions.getTime() * 1000)
        g.dataLogger:logPing(now - timestamp)
      else
        g.dataLogger:logPing(20000)
      end
    end
  end
end
function SocketBean:readLoginSuccess(packet)
  logger:debug("Protocol.SVR_LOGIN_SUCCESS")
  local p = {}
  p.tid = packet:readUnsignedInt()
  p.tableType = packet:readUnsignedByte()
  p.tableLevel = packet:readUnsignedByte()
  p.tableName = packet:readString()
  p.tableFlags = packet:readUnsignedInt()
  p.betTime = packet:readUnsignedByte()
  p.speakTime = packet:readUnsignedByte()
  p.seatCount = packet:readUnsignedByte()
  p.minBuyIn = packet:readUnsignedInt64()
  p.maxBuyIn = packet:readUnsignedInt64()
  p.minBet = packet:readUnsignedInt64()
  p.maxBet = packet:readUnsignedInt64()
  p.bankerSeatId = packet:readByte()
  p.playerCount = packet:readUnsignedByte()
  p.players = {}
  for i = 1, p.playerCount do
    p.players[i] = {}
    p.players[i].uid = packet:readUnsignedInt()
    p.players[i].gender = packet:readUnsignedByte()
    p.players[i].vip = packet:readUnsignedInt()
    p.players[i].exp = packet:readUnsignedInt()
    p.players[i].winCount = packet:readUnsignedInt()
    p.players[i].loseCount = packet:readUnsignedInt()
    p.players[i].drawCount = packet:readUnsignedInt()
    p.players[i].escapeCount = packet:readUnsignedInt()
    p.players[i].userName = packet:readString()
    p.players[i].seatChips = packet:readUnsignedInt64()
    p.players[i].userChips = packet:readUnsignedInt64()
    p.players[i].sPic = packet:readString()
    p.players[i].giftId = packet:readUnsignedShort()
    p.players[i].seatId = packet:readUnsignedByte()
    p.players[i].playStatus = packet:readUnsignedByte()
    if p.players[i].playStatus ~= PlayStatus.NON_PLAY then
      p.players[i].bet = packet:readUnsignedInt64()
      p.players[i].cardCount = packet:readUnsignedByte()
      p.players[i].isFlipedCard = packet:readByte()
      if p.players[i].isFlipedCard == 1 then
        p.players[i].cards = {}
        for j = 1, p.players[i].cardCount do
          p.players[i].cards[j] = packet:readUnsignedByte()
        end
        p.players[i].cardType = packet:readUnsignedByte()
        p.players[i].taem = packet:readUnsignedByte()
        p.players[i].deng = packet:readUnsignedByte()
      end
    end
  end
  p.userChips = packet:readUnsignedInt64()
  p.cardCount = packet:readUnsignedByte()
  if p.cardCount > 0 then
    p.cards = {}
    for i = 1, p.cardCount do
      p.cards[i] = packet:readUnsignedByte()
    end
    p.cardType = packet:readUnsignedByte()
    p.taem = packet:readUnsignedByte()
    p.deng = packet:readUnsignedByte()
    p.chooseType = packet:readUnsignedByte()
  end
  return p
end
function SocketBean:readLoginFail(packet)
  logger:debug("Protocol.SVR_LOGIN_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  logger:debug("errorCode=" .. tostring(p.errorCode))
  return p
end
function SocketBean:readLogoutSuccess(packet)
  logger:debug("Protocol.SVR_LOGOUT_SUCCESS")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readBroadcastSit(packet)
  logger:debug("Protocol.BROADCAST_SIT")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.gender = packet:readUnsignedByte()
  p.vip = packet:readUnsignedInt()
  p.exp = packet:readUnsignedInt()
  p.winCount = packet:readUnsignedInt()
  p.loseCount = packet:readUnsignedInt()
  p.drawCount = packet:readUnsignedInt()
  p.escapeCount = packet:readUnsignedInt()
  p.userName = packet:readString()
  p.sPic = packet:readString()
  p.giftId = packet:readUnsignedShort()
  p.buyInChips = packet:readUnsignedInt64()
  p.userChips = packet:readUnsignedInt64()
  return p
end
function SocketBean:readSitFail(packet)
  logger:debug("Protocol.SVR_SIT_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readGameStart(packet)
  logger:debug("Protocol.BROADCAST_GAME_START")
  local p = {}
  p.bankerSeatId = packet:readByte()
  p.playerCount = packet:readUnsignedByte()
  p.players = {}
  for i = 1, p.playerCount do
    p.players[i] = {}
    p.players[i].uid = packet:readUnsignedInt()
    p.players[i].seatId = packet:readUnsignedByte()
    p.players[i].seatChips = packet:readUnsignedInt64()
    p.players[i].userChips = packet:readUnsignedInt64()
  end
  p.handId = packet:readUnsignedInt()
  return p
end
function SocketBean:readBetTurn(packet)
  logger:debug("Protocol.BROADCAST_BET_TURN")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.maxBet = packet:readUnsignedInt64()
  p.minBet = packet:readUnsignedInt64()
  logger:debug("Protocol.BROADCAST_BET_TURN,maxBet=" .. tostring(p.maxBet))
  return p
end
function SocketBean:readBroadcastBet(packet)
  logger:debug("Protocol.BROADCAST_BET")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.bet = packet:readUnsignedInt64()
  return p
end
function SocketBean:readBetFail(packet)
  logger:debug("Protocol.SVR_BET_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readBankerDeal(packet)
  logger:debug("Protocol.BROADCAST_BANKER_DEAL")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  return p
end
function SocketBean:readBroadcastDeal(packet)
  logger:debug("Protocol.BROADCAST_DEAL")
  local p = {}
  p.direction = packet:readUnsignedByte()
  p.playerCount = packet:readUnsignedByte()
  p.players = {}
  for i = 1, p.playerCount do
    p.players[i] = {}
    p.players[i].uid = packet:readUnsignedInt()
    p.players[i].seatId = packet:readUnsignedByte()
  end
  p.isImPlayer = packet:readUnsignedByte()
  if p.isImPlayer == 1 then
    p.card0 = packet:readUnsignedByte()
    p.card1 = packet:readUnsignedByte()
    p.cardType = packet:readUnsignedByte()
    p.taem = packet:readUnsignedByte()
    p.deng = packet:readUnsignedByte()
  end
  return p
end
function SocketBean:readDealFail(packet)
  logger:debug("Protocol.SVR_DEAL_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readBroadcastPok(packet)
  logger:debug("Protocol.BROADCAST_POK")
  local p = {}
  p.pokCount = packet:readUnsignedByte()
  p.players = {}
  for i = 1, p.pokCount do
    p.players[i] = {}
    p.players[i].uid = packet:readUnsignedInt()
    p.players[i].seatId = packet:readUnsignedByte()
    p.players[i].card0 = packet:readUnsignedByte()
    p.players[i].card1 = packet:readUnsignedByte()
    p.players[i].cardType = packet:readUnsignedByte()
    p.players[i].taem = packet:readUnsignedByte()
    p.players[i].deng = packet:readUnsignedByte()
  end
  return p
end
function SocketBean:readSpeakTurn(packet)
  logger:debug("Protocol.BROADCAST_SPEAK_TURN")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.chooseType = packet:readUnsignedByte()
  return p
end
function SocketBean:readBroadcastSpeak(packet)
  logger:debug("Protocol.BROADCAST_SPEAK")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.operation = packet:readUnsignedByte()
  p.card = packet:readUnsignedByte()
  p.cardType = packet:readUnsignedByte()
  p.taem = packet:readUnsignedByte()
  p.deng = packet:readUnsignedByte()
  return p
end
function SocketBean:readSpeakFail(packet)
  logger:debug("Protocol.SVR_SPEAK_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readBankerTurn(packet)
  logger:debug("Protocol.BROADCAST_BANKER_TURN")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.isHitting = packet:readByte()
  p.playerCount = packet:readUnsignedByte()
  p.players = {}
  for i = 1, p.playerCount do
    p.players[i] = {}
    p.players[i].uid = packet:readUnsignedInt()
    p.players[i].seatId = packet:readUnsignedByte()
  end
  return p
end
function SocketBean:readBroadcastChallenge(packet)
  logger:debug("Protocol.BROADCAST_CHALLENGE")
  local p = {}
  p.targetUid = packet:readUnsignedInt()
  p.targetSeatId = packet:readUnsignedByte()
  p.cardCount = packet:readUnsignedByte()
  p.cards = {}
  for i = 1, p.playerCount do
    p.cards[i] = packet:readUnsignedByte()
  end
  p.cardType = packet:readUnsignedByte()
  p.taem = packet:readUnsignedByte()
  p.deng = packet:readUnsignedByte()
  p.result = packet:readByte()
  p.targetChips = packet:readUnsignedInt64()
  p.targetSeatChips = packet:readUnsignedInt64()
  p.targetUserChips = packet:readUnsignedInt64()
  p.bankerChips = packet:readUnsignedInt64()
  p.bankerSeatChips = packet:readUnsignedInt64()
  p.bankerUserChips = packet:readUnsignedInt64()
  return p
end
function SocketBean:readChallengeFail(packet)
  logger:debug("Protocol.SVR_CHALLENGE_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readGameOver(packet)
  logger:debug("Protocol.BROADCAST_GAME_OVER")
  local p = {}
  p.playerCount = packet:readUnsignedByte()
  p.players = {}
  for i = 1, p.playerCount do
    p.players[i] = {}
    p.players[i].uid = packet:readUnsignedInt()
    p.players[i].seatId = packet:readUnsignedByte()
    p.players[i].cardCount = packet:readUnsignedByte()
    p.players[i].cards = {}
    for j = 1, p.players[i].cardCount do
      p.players[i].cards[j] = packet:readUnsignedByte()
    end
    p.players[i].cardType = packet:readUnsignedByte()
    p.players[i].taem = packet:readUnsignedByte()
    p.players[i].deng = packet:readUnsignedByte()
    p.players[i].result = packet:readByte()
    p.players[i].deltaChips = packet:readUnsignedInt64()
    p.players[i].seatChips = packet:readUnsignedInt64()
    p.players[i].userChips = packet:readUnsignedInt64()
  end
  p.feeChips = packet:readUnsignedInt64()
  return p
end
function SocketBean:readBankerEscape(packet)
  logger:debug("Protocol.BROADCAST_BANKER_ESCAPE")
  local p = {}
  p.playerCount = packet:readUnsignedByte()
  p.players = {}
  for i = 1, p.playerCount do
    p.players[i] = {}
    p.players[i].uid = packet:readUnsignedInt()
    p.players[i].seatId = packet:readUnsignedByte()
    p.players[i].deltaChips = packet:readUnsignedInt64()
    p.players[i].seatChips = packet:readUnsignedInt64()
    p.players[i].userChips = packet:readUnsignedInt64()
  end
  p.feeChips = packet:readUnsignedInt64()
  return p
end
function SocketBean:readBroadcastStand(packet)
  logger:debug("Protocol.BROADCAST_STAND")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.reason = packet:readShort()
  p.userChips = packet:readUnsignedInt64()
  return p
end
function SocketBean:readStandFail(packet)
  logger:debug("Protocol.SVR_STAND_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readBroadcastEmoji(packet)
  logger:debug("Protocol.BROADCAST_EMOJI")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.emojiType = packet:readUnsignedByte()
  p.emojiIndex = packet:readUnsignedByte()
  return p
end
function SocketBean:readBroadcastChat(packet)
  logger:debug("Protocol.BROADCAST_CHAT")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.userName = packet:readString()
  p.sPic = packet:readString()
  p.chatMessage = packet:readString()
  return p
end
function SocketBean:readBroadcastInteractionEquip(packet)
  logger:debug("Protocol.BROADCAST_INTERACTION_EQUIP")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.targetUid = packet:readUnsignedInt()
  p.targetSeatId = packet:readUnsignedByte()
  p.prop = packet:readUnsignedByte()
  return p
end
function SocketBean:readBroadcastAddFriend(packet)
  logger:debug("Protocol.BROADCAST_ADD_FRIEND")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.targetUid = packet:readUnsignedInt()
  return p
end
function SocketBean:readBroadcastSendChipSuccess(packet)
  logger:debug("Protocol.BROADCAST_SENDCHIP_SUCCESS")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.seatChips = packet:readUnsignedInt64()
  p.targetUid = packet:readUnsignedInt()
  p.targetSeatId = packet:readUnsignedByte()
  p.targetSeatChips = packet:readUnsignedInt64()
  p.sendChips = packet:readUnsignedInt64()
  return p
end
function SocketBean:readSVRSendChipFail(packet)
  logger:debug("Protocol.SVR_SENDCHIP_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readTransmitMulticast(packet)
  logger:debug("Protocol.SVR_TRANSMIT_MULTICAST")
  local p = {}
  p.fromUid = packet:readUnsignedInt()
  p.fromUserName = packet:readString()
  p.message = packet:readString()
  logger:debug(p.message)
  return p
end
function SocketBean:readTransmitBroadcast(packet)
  logger:debug("Protocol.SVR_TRANSMIT_BROADCAST")
  local p = {}
  p.fromUid = packet:readUnsignedInt()
  p.fromUserName = packet:readString()
  p.message = packet:readString()
  logger:debug(p.message)
  return p
end
function SocketBean:readSystemMulticast(packet)
  logger:debug("Protocol.SVR_SYSTEM_MULTICAST")
  local p = {}
  local messageData = packet:readString()
  p.message = json.decode(messageData)
  local messageType = tonumber(p.message.type)
  if p.message.type ~= nil then
    if messageType == 0 then
      if p.message.info.chips then
        g.vars.user.chips = tonumber(p.message.info.chips) or 0
      end
      if p.message.info.deltaChips then
        p.message.info.deltaChips = tonumber(p.message.info.deltaChips)
        if 0 < p.message.info.deltaChips then
          g.dataLogger:logChipsProduce(math.abs(p.message.info.deltaChips), p.message.info.reason)
        else
          g.dataLogger:logChipsConsume(math.abs(p.message.info.deltaChips), p.message.info.reason)
        end
      end
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_USER_CHIPS_CHANGE
      })
    elseif messageType == 1 then
      g.vars.user.exp = p.message.info.exp
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_USER_EXP_CHANGE
      })
    elseif messageType == 2 then
      g.vars.user.name = p.message.info.userName
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_USER_NAME_CHANGE
      })
    elseif messageType == 3 then
      g.vars.user.gender = p.message.info.gender
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_USER_GENDER_CHANGE
      })
    elseif messageType == 4 then
      g.vars.user.sPic = p.message.info.sPic
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_USER_PIC_CHANGE
      })
    elseif messageType == 5 then
    elseif messageType == 6 then
      g.vars.user.magic = p.message.info.magic
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_USER_MAGIC_CHANGE
      })
    elseif messageType == 7 then
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_USER_LEVELUP,
        data = p
      })
    elseif messageType == 8 then
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_PAYMENT_DELIVERED,
        data = p
      })
      g.dataLogger:logPayment(p.message.info.channel, p.message.info.orderId, p.message.info.paymentId, p.message.info.pay, p.message.info.currency)
    elseif messageType == 9 then
      g.ui.manager.tip:top(p.message.info.message)
      if p.message.info.activityID then
        g.activityCenter:enableActivityRedDotState(tonumber(p.message.info.activityID))
      end
    end
  end
  return p
end
function SocketBean:readSystemBroadcast(packet)
  logger:debug("Protocol.SVR_SYSTEM_BROADCAST")
  local p = {}
  local messageData = packet:readString()
  p.message = json.decode(messageData)
  local messageType = tonumber(p.message.type)
  if messageType == 0 then
    local delayCallback = function()
      g.systemControlInfo:reportNewDay()
      g.jewelBoxInfo:progress()
    end
    scheduler.performWithDelayGlobal(delayCallback, tonumber(g.vars.user.uid or 0) % 100)
  elseif messageType == 1 then
    g.eventCenter:dispatchEvent({
      name = TableBeanEvent.EVT_BROADCAST_SERVER_STOP,
      data = p
    })
  elseif messageType == 2 then
    g.ui.manager.tip:top(p.message.info.message)
  end
  return p
end
function SocketBean:readLoginBroadcastFail(packet)
  logger:debug("Protocol.SVR_LOGIN_BROADCAST_FAIL")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readLogoutBroadcastSuccess(packet)
  logger:debug("Protocol.SVR_LOGOUT_BROADCAST_SUCCESS")
  local p = {}
  p.errorCode = packet:readShort()
  return p
end
function SocketBean:readSendGift(packet)
  logger:debug("Protocol.BROADCAST_SEND_GIFT")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.giftId = packet:readUnsignedShort()
  p.playerCount = packet:readUnsignedByte()
  p.players = {}
  for i = 1, p.playerCount do
    p.players[i] = {}
    p.players[i].uid = packet:readUnsignedInt()
    p.players[i].seatId = packet:readUnsignedByte()
  end
  return p
end
function SocketBean:readApplyGift(packet)
  logger:debug("Protocol.BROADCAST_APPLY_GIFT")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.seatId = packet:readUnsignedByte()
  p.giftId = packet:readUnsignedShort()
  return p
end
function SocketBean:readUserBankrupt(packet)
  logger:debug("Protocol.SVR_USER_BANKRUPT")
  local p = {}
  p.uid = packet:readUnsignedInt()
  p.bankruptTimes = packet:readUnsignedInt()
  p.supportChips = packet:readUnsignedInt64()
  p.userChips = packet:readUnsignedInt64()
  return p
end
function SocketBean:readServerUpdate(packet)
  logger:debug("Protocol.BROADCAST_SERVER_UPDATE")
  local p = {}
  p.reason = packet:readShort()
  return p
end
return SocketBean

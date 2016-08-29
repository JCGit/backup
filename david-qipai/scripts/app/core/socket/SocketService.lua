cc.utils = require("framework.cc.utils.init")
cc.net = require("framework.cc.net.init")
local Packet = import(".Packet")
local PacketType = import(".PacketType")
local SocketEvent = import(".SocketEvent")
local SocketService = class("SocketService")
function SocketService:ctor(name, reader)
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
  self._name = name
  self._reader = reader
end
function SocketService:setReader(value)
  self._reader = value
end
function SocketService:connect(host, port, retryConnectWhenFailure)
  self:disconnect()
  if not self._socket then
    self._socket = cc.net.SocketTCP.new(host, port, retryConnectWhenFailure or false)
    self._socket.socketId_ = SOCKET_ID
    self._socket:addEventListener(cc.net.SocketTCP.EVENT_CONNECTED, handler(self, self.onConnected))
    self._socket:addEventListener(cc.net.SocketTCP.EVENT_CLOSE, handler(self, self.onClose))
    self._socket:addEventListener(cc.net.SocketTCP.EVENT_CLOSED, handler(self, self.onClosed))
    self._socket:addEventListener(cc.net.SocketTCP.EVENT_CONNECT_FAILURE, handler(self, self.onConnectFailure))
    self._socket:addEventListener(cc.net.SocketTCP.EVENT_DATA, handler(self, self.onData))
  end
  self._socket:setName(self._name):connect()
end
function SocketService:send(value)
  if self._socket and self._socket.isConnected and value and value:isValid() then
    self._socket:send(value:getByteArray():getPack())
  end
end
function SocketService:sendRaw(value)
  if self._socket and self._socket.isConnected and value then
    self._socket:send(value:getPack())
  end
end
function SocketService:disconnect()
  if self._socket and self._socket.isConnected then
    local socket = self._socket
    self._socket = nil
    socket:removeAllEventListeners()
    socket:disconnect()
  end
end
function SocketService:isConnected()
  return self._socket and self._socket.isConnected
end
function SocketService:onConnected(evt)
  self:dispatchEvent({
    name = SocketEvent.EVT_CONNECT_SUCCESS
  })
end
function SocketService:onClose(evt)
  self:dispatchEvent({
    name = SocketEvent.EVT_CLOSE
  })
end
function SocketService:onClosed(evt)
  self:dispatchEvent({
    name = SocketEvent.EVT_CLOSED
  })
end
function SocketService:onConnectFailure(evt)
  self:dispatchEvent({
    name = SocketEvent.EVT_CONNECT_FAIL
  })
end
function SocketService:onData(evt)
  if DEBUG_RECORD_MODE then
    g.record_savingPacketData = evt.data
  end
  if DEBUG_RECORD_MODE and g.record_isNowReplaying and g.record_isConnect_ready then
    if not evt.isReplayPacket then
      print("on data block.....")
      return
    end
    if evt.roundId ~= g.record_roundID then
      print("on data block.....")
      return
    end
  end
  if DEBUG_RECORD_MODE then
    print("on data not block.....")
    print("---------data file --------")
    print(evt.fileName)
  end
  local byteArray = cc.utils.ByteArray.new(cc.utils.ByteArray.ENDIAN_BIG)
  byteArray:writeBuf(evt.data)
  byteArray:setPos(1)
  local packets = self._reader:read(byteArray)
  local length = table.maxn(packets)
  for i = 1, length do
    self:dispatchEvent({
      name = SocketEvent.EVT_PACKET_RECEIVED,
      data = packets[i]
    })
  end
end
return SocketService

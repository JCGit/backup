local PacketType = import(".PacketType")
local PacketConst = import(".PacketConst")
local Packet = class("Packet")
function Packet:ctor(packetType)
  self:setPacketType(packetType)
  self._command = 0
  self._signature = PacketConst.SIGNATURE
  self._version = 1
  self._link = 0
  self._bodyLen = 0
  self._isValid = true
end
function Packet:setPacketType(value)
  value = value or PacketType.WRITE
  if value == PacketType.WRITE and not self._byteArray then
    self._byteArray = cc.utils.ByteArray.new(cc.utils.ByteArray.ENDIAN_BIG)
    self:writeInt(0)
    self:writeInt(0)
    self:writeInt(0)
    self:writeInt(0)
  end
  self._packetType = value
end
function Packet:getPacketType()
  return self._packetType
end
function Packet:setCommand(value)
  self._command = value
end
function Packet:getCommand()
  return self._command
end
function Packet:setSignature(value)
  self._signature = value
end
function Packet:getSignature()
  return self._signature
end
function Packet:setVersion(value)
  self._version = value
end
function Packet:getVersion()
  return self._version
end
function Packet:getBodyLen()
  return self._bodyLen
end
function Packet:setLink(value)
  self._link = value
end
function Packet:getLink()
  return self._link
end
function Packet:isValid()
  return self._isValid
end
function Packet:setByteArray(value)
  if value:getAvailable() >= PacketConst.HEAD_LEN then
    self._byteArray = value
    self:setCommand(self._byteArray:setPos(PacketConst.SIGNATURE_LEN + 1):readUShort())
    self:setVersion(self._byteArray:readUByte())
    self:setLink(self._byteArray:readUByte())
    self._bodyLen = self._byteArray:readUShort()
    self._byteArray:setPos(PacketConst.HEAD_LEN + 1)
    if self._bodyLen + PacketConst.HEAD_LEN > self._byteArray:getLen() then
      self._isValid = false
    end
  else
    self._isValid = false
  end
  return self
end
function Packet:getByteArray()
  self._byteArray:setPos(1):writeStringBytes(PacketConst.SIGNATURE)
  self._byteArray:writeUShort(self:getCommand())
  self._byteArray:writeUByte(self:getVersion())
  self._byteArray:writeUByte(self:getLink())
  self._byteArray:writeUShort(self._byteArray:getLen() - PacketConst.HEAD_LEN)
  self._byteArray:setPos(self._byteArray:getLen() + 1)
  return self._byteArray
end
function Packet:readByte()
  if self._byteArray and self._byteArray:getAvailable() > 0 then
    return self._byteArray:readByte()
  end
  return nil
end
function Packet:writeByte(value)
  if self._byteArray then
    self._byteArray:writeByte(value)
  end
  return self
end
function Packet:readUnsignedByte()
  if self._byteArray and self._byteArray:getAvailable() > 0 then
    return self._byteArray:readUByte()
  end
  return nil
end
function Packet:writeUnsignedByte(value)
  if self._byteArray then
    self._byteArray:writeUByte(value)
  end
  return self
end
function Packet:readShort()
  if self._byteArray and self._byteArray:getAvailable() >= 2 then
    return self._byteArray:readShort()
  end
  return nil
end
function Packet:writeShort(value)
  if self._byteArray then
    self._byteArray:writeShort(value)
  end
  return self
end
function Packet:readUnsignedShort()
  if self._byteArray and self._byteArray:getAvailable() >= 2 then
    return self._byteArray:readUShort()
  end
  return nil
end
function Packet:writeUnsignedShort(value)
  if self._byteArray then
    self._byteArray:writeUShort(value)
  end
  return self
end
function Packet:readInt()
  if self._byteArray and self._byteArray:getAvailable() >= 4 then
    return self._byteArray:readInt()
  end
  return nil
end
function Packet:writeInt(value)
  if self._byteArray then
    self._byteArray:writeInt(value)
  end
  return self
end
function Packet:readUnsignedInt()
  if self._byteArray and self._byteArray:getAvailable() >= 4 then
    return self._byteArray:readUInt()
  end
  return nil
end
function Packet:writeUnsignedInt(value)
  if self._byteArray then
    self._byteArray:writeUInt(value)
  end
  return self
end
function Packet:readInt64()
  if self._byteArray and self._byteArray:getAvailable() >= 8 then
    local high = buf:readInt()
    local low = buf:readUInt()
    return high * 4294967296 + low
  end
  return nil
end
function Packet:writeInt64(value)
  if self._byteArray then
    value = tonumber(value) or 0
    local low = value % 4294967296
    local high = value / 4294967296
    self._byteArray:writeInt(high)
    self._byteArray:writeUInt(low)
  end
  return self
end
function Packet:readUnsignedInt64()
  if self._byteArray and self._byteArray:getAvailable() >= 8 then
    local high = self._byteArray:readUInt()
    local low = self._byteArray:readUInt()
    return high * 4294967296 + low
  end
  return nil
end
function Packet:writeUnsignedInt64(value)
  if self._byteArray then
    value = tonumber(value) or 0
    local low = value % 4294967296
    local high = value / 4294967296
    self._byteArray:writeUInt(high)
    self._byteArray:writeUInt(low)
  end
  return self
end
function Packet:readString()
  if self._byteArray and self._byteArray:getAvailable() >= 2 then
    local length = self._byteArray:readUShort()
    if length <= self._byteArray:getAvailable() then
      local str = self._byteArray:readStringBytes(length - 1)
      self._byteArray:readStringBytes(1)
      return str
    end
  end
  return nil
end
function Packet:writeString(value)
  if self._byteArray then
    value = tostring(value) or 0
    self._byteArray:writeUShort(string.len(value) + 1)
    self._byteArray:writeStringBytes(value)
    self._byteArray:writeByte(0)
  end
  return self
end
return Packet

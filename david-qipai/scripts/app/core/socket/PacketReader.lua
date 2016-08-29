local PacketConst = import(".PacketConst")
local Packet = import(".Packet")
local PacketType = import(".PacketType")
local PacketReader = class("PacketReader")
function PacketReader:ctor()
end
function PacketReader:reset()
  self:clearByteArray()
end
function PacketReader:clearBuffer()
  self._byteArray = nil
end
function PacketReader:appendBuffer(byteArray)
  if not self._byteArray then
    self._byteArray = cc.utils.ByteArray.new(cc.utils.ByteArray.ENDIAN_BIG)
  end
  self._byteArray:setPos(self._byteArray:getLen() + 1)
  local available = byteArray:getLen()
  for i = 1, available do
    self._byteArray:writeRawByte(byteArray:readRawByte())
  end
end
function PacketReader:read(byteArray)
    self:appendBuffer(byteArray)
    local packets = {}
    local available = self._byteArray:getLen()
    local pos = 1
    self._byteArray:setPos(pos)
    local dataString = self._byteArray:readStringBytes(available)
    while true do
        local isHeadCompleted = available >= PacketConst.HEAD_LEN
        if isHeadCompleted then
            local signature = string.find(dataString, PacketConst.SIGNATURE, pos)
            if signature ~= nil then
                pos = signature
            else
                self._byteArray = cc.utils.ByteArray.new(cc.utils.ByteArray.ENDIAN_BIG)
                available = 0
                pos = 0
                break
            end
            local bodyLen = self._byteArray:setPos(pos + 6):readUShort()
            self._byteArray:setPos(pos)
            if available >= PacketConst.HEAD_LEN + bodyLen then
                local packet = Packet.new(PacketType.READ)
                local buf = cc.utils.ByteArray.new(cc.utils.ByteArray.ENDIAN_BIG)
                for i = 1, PacketConst.HEAD_LEN + bodyLen do
                    buf:writeRawByte(self._byteArray:readRawByte())
                end
                buf:setPos(1)
                packet:setByteArray(buf)
                if packet:isValid() then
                    packets[table.maxn(packets) + 1] = packet
                end
                available = available - PacketConst.HEAD_LEN - bodyLen
                pos = pos + PacketConst.HEAD_LEN + bodyLen
                break
            end
        else
            break
        end
    end
    if pos > 1 then
        local buf = cc.utils.ByteArray.new(cc.utils.ByteArray.ENDIAN_BIG)
        for i = pos, available do
            buf:writeRawByte(self._byteArray:readRawByte())
        end
        self._byteArray = buf
    end
    return packets
end
return PacketReader

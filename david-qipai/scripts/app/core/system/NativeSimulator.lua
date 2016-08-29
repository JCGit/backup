local NativeSimulator = {}
local mtable = {
  __index = function(table, key)
    if NativeSimulator[key] then
      return NativeSimulator[key]
    else
      return function(...)
        local params = {
          ...
        }
        for i, v in ipairs(params) do
          params[i] = tostring(v)
        end
      end
    end
  end,
  __newindex = function(table, key, value)
    error("invalid set data to NativeSimulator")
  end
}
function NativeSimulator:getFixedWidthText(font, size, text, width)
  return text
end
function NativeSimulator:getDeviceKey()
  return crypto.encodeBase64("AB:E6:CD:D6:CF:C8" .. "_device1")
end
function NativeSimulator:pickImage(callback)
  callback("error")
end
function NativeSimulator:getChannelId()
  return "Windows"
end
function NativeSimulator:getVersion()
  return "1.0.0.0"
end
return setmetatable({}, mtable)

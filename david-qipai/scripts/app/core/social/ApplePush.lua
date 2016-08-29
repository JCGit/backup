local ApplePush = class("ApplePush")
function ApplePush:ctor()
end
function ApplePush:register(uid, callback)
  local ok, pushToken = luaoc.callStaticMethod("NativeiPhone", "getPushToken")
  if ok and callback and pushToken and pushToken ~= "" then
    pushToken = string.gsub(pushToken, " ", "")
    pushToken = string.gsub(pushToken, "<", "")
    pushToken = string.gsub(pushToken, ">", "")
    callback(true, pushToken)
  else
    callback(false)
  end
end
return ApplePush

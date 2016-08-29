local XinGePushAndroid = class("XinGePushAndroid")
function XinGePushAndroid:ctor()
  self:_call("setRegisterCallback", {
    handler(self, self._registerCallback)
  }, "(I)V")
end
function XinGePushAndroid:register(uid, callback)
  self._callback = callback
  self:_call("register", {uid}, "(Ljava/lang/String;)V")
end
function XinGePushAndroid:_registerCallback(jsonString)
  local jsonData = json.decode(jsonString)
  local eventName = jsonData.eventName
  local isSuccess = jsonData.isSuccess
  local data = jsonData.data
  if eventName == "PUSH_REGISTERED" and self._callback then
    self._callback(isSuccess, data)
  end
end
function XinGePushAndroid:_call(javaMethodName, javaParams, javaMethodSig)
  if device.platform == "android" then
    local isSuccess, ret = luaj.callStaticMethod("com/veragame/push/xg/XgPushInterface", javaMethodName, javaParams, javaMethodSig)
    if not isSuccess then
      if ret == -1 then
        print("call %s failed, -1 不支持的参数类型或返回值类型", javaMethodName)
      elseif ret == -2 then
        print("call %s failed, -2 无效的签名", javaMethodName)
      elseif ret == -3 then
        print("call %s failed, -3 没有找到指定的方法", javaMethodName)
      elseif ret == -4 then
        print("call %s failed, -4 Java 方法执行时抛出了异常", javaMethodName)
      elseif ret == -5 then
        print("call %s failed, -5 Java 虚拟机出错", javaMethodName)
      elseif ret == -6 then
        print("call %s failed, -6 Java 虚拟机出错", javaMethodName)
      end
    end
    return isSuccess, ret
  else
    print("call %s failed, not in android platform", javaMethodName)
    return false, nil
  end
end
return XinGePushAndroid

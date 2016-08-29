local NativeAndroid = class("NativeAndroid")
function NativeAndroid:ctor()
end
function NativeAndroid:call(nativeClassName, nativeMethodName, nativeParams, nativeMethodSig)
  if device.platform == "android" then
    local isSuccess, ret = luaj.callStaticMethod(nativeClassName, nativeMethodName, nativeParams, nativeMethodSig)
    if not isSuccess then
      if ret == -1 then
        print("call %s failed, -1 不支持的参数类型或返回值类型", nativeMethodName)
      elseif ret == -2 then
        print("call %s failed, -2 无效的签名", nativeMethodName)
      elseif ret == -3 then
        print("call %s failed, -3 没有找到指定的方法", nativeMethodName)
      elseif ret == -4 then
        print("call %s failed, -4 Native 方法执行时抛出了异常", nativeMethodName)
      elseif ret == -5 then
        print("call %s failed, -5 Dalvik 虚拟机出错", nativeMethodName)
      elseif ret == -6 then
        print("call %s failed, -6 Dalvik 虚拟机出错", nativeMethodName)
      end
    end
    return isSuccess, ret
  else
    print("call %s failed, not in android platform", nativeMethodName)
    return false, nil
  end
end
function NativeAndroid:getMacAddress()
  local isSuccess, ret = self:call("com/veragame/base/util/NetworkUtil", "getMacAddress", {}, "()Ljava/lang/String;")
  if isSuccess then
    return ret
  end
  return nil
end
function NativeAndroid:getDistributionId()
  local isSuccess, ret = self:call("com/veragame/base/util/DistributionUtil", "getDistributionId", {}, "()Ljava/lang/String;")
  if isSuccess then
    return ret or "Google Play"
  end
  return "Google Play"
end
function NativeAndroid:vibrate(time)
  self:call("com/veragame/base/util/VibrateUtil", "vibrate", {time}, "(I)V")
end
function NativeAndroid:getVersion()
  local isSuccess, ret = self:call("com/veragame/base/util/AppInfoUtil", "getVersion", {}, "()Ljava/lang/String;")
  if isSuccess then
    return ret or ""
  end
  return "1.0.0.0"
end
function NativeAndroid:getFixedWidthText(font, size, text, width)
  local isSuccess, ret = self:call("com/veragame/base/util/TextUtil", "getFixedWidthText", {
    font or "",
    size or 20,
    text or "",
    width or device.display.widthInPixels
  }, "(Ljava/lang/String;ILjava/lang/String;I)Ljava/lang/String;")
  if isSuccess then
    return ret or ""
  end
  return ""
end
function NativeAndroid:getHeadPic(callback)
  self:call("com/veragame/base/util/PicUtil", "getHeadPic", {
    function(result)
      if callback then
        if result == "nosdcard" then
          callback(false, "nosdcard")
        elseif result == "error" then
          callback(false, "error")
        else
          callback(true, result)
        end
      end
    end
  }, "(I)V")
end
function NativeAndroid:getFeedbackPic(callback)
  self:call("com/veragame/base/util/PicUtil", "getFeedbackPic", {
    function(result)
      if callback then
        if result == "nosdcard" then
          callback(false, "nosdcard")
        elseif result == "error" then
          callback(false, "error")
        else
          callback(true, result)
        end
      end
    end
  }, "(I)V")
end
function NativeAndroid:showSMSView(content)
  self:call("com/veragame/core/utils/ShowSMSViewFunction", "apply", {content}, "(Ljava/lang/String;)V")
end
function NativeAndroid:showEmailView(subject, content)
  self:call("com/veragame/core/utils/ShowEmailViewFunction", "apply", {subject, content}, "(Ljava/lang/String;Ljava/lang/String;)V")
end
function NativeAndroid:getDeviceKey()
  return crypto.encodeBase64(self:getMacAddress() .. "_veragame")
end
return NativeAndroid

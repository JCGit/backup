local NativeiPhone = class("NativeiPhone")
function NativeiPhone:ctor()
end
function NativeiPhone:vibrate()
  CCNative:vibrate()
end
function NativeiPhone:getFixedWidthText(fontName, fontSize, text, fixedWidth)
  local ok, fixedString = luaoc.callStaticMethod("NativeiPhone", "getFixedWidthText", {
    text = text,
    fixedWidth = fixedWidth,
    fontName = fontName,
    fontSize = fontSize
  })
  if ok then
    return fixedString
  else
    return text
  end
end
function NativeiPhone:getHeadPic(callback)
  luaoc.callStaticMethod("NativeiPhone", "getHeadPic", {
    getPicCallback = function(imagePath)
      if callback then
        if imagePath and imagePath ~= "" then
          callback(true, imagePath)
        else
          callback(false, imagePath)
        end
      end
    end
  })
end
function NativeiPhone:buyProduct(productId, successCallback, failCallback)
  luaoc.callStaticMethod("NativeiPhone", "buyProduct", {
    productId = productId,
    iapCallback = function(resultCode, info)
      if resultCode == 0 then
        successCallback(resultCode, info)
      else
        failCallback(resultCode, info)
      end
    end
  })
end
function NativeiPhone:bindXingeAcount(uid)
  luaoc.callStaticMethod("NativeiPhone", "bindXingeAcount", {uid = uid})
end
function NativeiPhone:getChannelId()
  return "AppStore"
end
function NativeiPhone:getDeviceKey()
  local openUDID = g.storage:getString("OPEN_UDID")
  if not openUDID or openUDID == "" then
    openUDID = device.getOpenUDID()
    g.storage:setString("OPEN_UDID", openUDID)
    g.storage:flush()
  end
  return crypto.encodeBase64(openUDID .. "_veragame")
end
function NativeiPhone:getVersion()
  local ok, nativeVersion = luaoc.callStaticMethod("NativeiPhone", "getVersion", nil)
  if ok then
    return nativeVersion
  else
    return "1.0.0.0"
  end
end
function NativeiPhone:showSMSView(content)
  luaoc.callStaticMethod("NativeiPhone", "showSMSView", {
    content = content,
    cannotCallback = handler(self, self.cannotShowSMSView_)
  })
end
function NativeiPhone:cannotShowSMSView_()
  g.ui.Dialog.new({
    messageText = "CANNOT_SEND_SMS",
    secondBtnText = "CONFIRM"
  }):show()
end
function NativeiPhone:showEmailView(subject, content)
  luaoc.callStaticMethod("NativeiPhone", "showMAILView", {
    subject = subject,
    content = content,
    cannotCallback = handler(self, self.cannotShowMAILView_)
  })
end
function NativeiPhone:cannotShowMAILView_()
  g.ui.Dialog.new({
    messageText = "CANNOT_SEND_MAIL",
    firstBtnText = "CANCEL",
    secondBtnText = "CONFIRM",
    callback = function(type)
      if type == g.ui.Dialog.SECOND_BTN_CLICK then
        CCNative:openURL("mailto:")
      end
    end
  }):show()
end
return NativeiPhone

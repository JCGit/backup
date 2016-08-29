local FacebookAndroid = class("FacebookAndroid")
function FacebookAndroid:ctor()
  self._loginResultHandler = handler(self, self._onLoginResult)
  self._unauthorizedFriendsResultHandler = handler(self, self._onUnauthorizedFriendsResult)
  self._sendInvitesResultHandler = handler(self, self._onSendInvitesResult)
  self._shareFeedResultHandler = handler(self, self._onShareFeedResult)
  self._getRequestIdHandler = handler(self, self._onGetRequestIdResult)
  self:interfaceCall("setLoginCallback", {
    self._loginResultHandler
  }, "(I)V")
  self:interfaceCall("setGetUnauthorizedFriendsCallback", {
    self._unauthorizedFriendsResultHandler
  }, "(I)V")
  self:interfaceCall("setSendInvitesCallback", {
    self._sendInvitesResultHandler
  }, "(I)V")
  self:interfaceCall("setShareFeedCallback", {
    self._shareFeedResultHandler
  }, "(I)V")
  self:interfaceCall("setGetRequestIdCallback", {
    self._getRequestIdHandler
  }, "(I)V")
end
function FacebookAndroid:login(callback)
  self._loginCallback = callback
  self:interfaceCall("login", {}, "()V")
end
function FacebookAndroid:logout()
  self:interfaceCall("logout", {}, "()V")
end
function FacebookAndroid:getUnauthorizedFriends(callback)
  self._getUnauthorizedFriendsCallback = callback
  self:interfaceCall("getUnauthorizedFriends", {}, "()V")
end
function FacebookAndroid:sendInvites(inviteHash, toID, title, message, callback)
  self._sendInvitesCallback = callback
  self:interfaceCall("sendInvites", {
    inviteHash,
    toID,
    title,
    message
  }, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")
end
function FacebookAndroid:shareFeed(params, callback)
  self._shareFeedCallback = callback
  self:interfaceCall("shareFeed", {
    json.encode(params)
  }, "(Ljava/lang/String;)V")
end
function FacebookAndroid:updateAppRequest()
  self._updateInviteRetryTimes = 3
  self:interfaceCall("getRequestId", {}, "()V")
end
function FacebookAndroid:_onGetRequestIdResult(result)
  local isSuccess = result ~= "canceled" and result ~= "failed"
  if isSuccess then
    result = json.decode(result)
    if result and result.requestData and result.requestId then
      local requestData = {}
      requestData.uid = g.vars.user.uid
      requestData.payload = result.requestData
      requestData.requestId = result.requestId
      g.httpRequest:postMethod(g.requestParamsFactory:createRequest("invite", "acceptInvite", requestData), function(data)
        local retData = json.decode(data)
        if retData and retData.ret and retData.ret == 0 then
          self:interfaceCall("deleteRequestId", {
            result.requestId
          }, "(Ljava/lang/String;)V")
          g.dataLogger:logInviteInstalled("facebook")
        end
      end, function()
        if self._updateInviteRetryTimes > 0 then
          self:_onGetRequestIdResult(result)
          self._updateInviteRetryTimes = self._updateInviteRetryTimes - 1
        end
      end)
    end
  end
end
function FacebookAndroid:_onShareFeedResult(result)
  local isSuccess = result ~= "canceled" and result ~= "failed"
  if self._shareFeedCallback then
    self._shareFeedCallback(isSuccess, result)
  end
  if isSuccess then
    local ret = json.decode(result)
    g.dataLogger:logFeedSent(ret.type)
  end
end
function FacebookAndroid:_onSendInvitesResult(result)
  local isSuccess = result ~= "canceled" and result ~= "failed"
  if isSuccess then
    result = json.decode(result)
    g.dataLogger:logInviteSent("facebook")
  end
  if self._sendInvitesCallback then
    self._sendInvitesCallback(isSuccess, result)
  end
end
function FacebookAndroid:_onUnauthorizedFriendsResult(result)
  local isSuccess = result ~= "canceled" and result ~= "failed"
  if isSuccess then
    result = json.decode(result)
  end
  if self._getUnauthorizedFriendsCallback then
    self._getUnauthorizedFriendsCallback(isSuccess, result)
  end
end
function FacebookAndroid:_onLoginResult(result)
  local isSuccess = result ~= "canceled" and result ~= "failed"
  print("facebook login result=", result)
  if self._loginCallback then
    self._loginCallback(isSuccess, result)
  end
end
function FacebookAndroid:interfaceCall(methodName, params, methodSig)
  if device.platform == "android" then
    local isSuccess, ret = luaj.callStaticMethod("com/veragame/social/facebook/FacebookInterface", methodName, params, methodSig)
    if not isSuccess then
      if ret == -1 then
        print("call %s failed, -1 不支持的参数类型或返回值类型", methodName)
      elseif ret == -2 then
        print("call %s failed, -2 无效的签名", methodName)
      elseif ret == -3 then
        print("call %s failed, -3 没有找到指定的方法", methodName)
      elseif ret == -4 then
        print("call %s failed, -4 Java 方法执行时抛出了异常", methodName)
      elseif ret == -5 then
        print("call %s failed, -5 Java 虚拟机出错", methodName)
      elseif ret == -6 then
        print("call %s failed, -6 Java 虚拟机出错", methodName)
      end
    end
    return isSuccess, ret
  else
    return false, nil
  end
end
return FacebookAndroid

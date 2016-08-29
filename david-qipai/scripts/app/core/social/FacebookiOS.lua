local FacebookiOS = class("FacebookiOS")
function FacebookiOS:ctor()
  luaoc.callStaticMethod("FacebookiOS", "initFaceBookSession")
end
function FacebookiOS:login(loginCallback)
  luaoc.callStaticMethod("FacebookiOS", "login", {
    loginCallbackId = function(isSuccess, accessToken)
      if loginCallback then
        loginCallback(isSuccess, accessToken)
      end
    end
  })
end
function FacebookiOS:logout()
  luaoc.callStaticMethod("FacebookiOS", "logout")
end
function FacebookiOS:getUnauthorizedFriends(callback)
  self.getInvitableFriendsCallback_ = callback
  luaoc.callStaticMethod("FacebookiOS", "getUnauthorizedFriends", {
    getUnauthorizedFriendsCallback = function(unauthorizedFriends)
      local success = unauthorizedFriends and type(unauthorizedFriends) == "table"
      if callback ~= nil then
        callback(success, unauthorizedFriends)
      end
    end
  })
end
function FacebookiOS:sendInvites(data, toIds, title, message, callback)
  self.sendInvitesCallback_ = callback
  luaoc.callStaticMethod("FacebookiOS", "sendInvites", {
    sendInvitesCallback = function(result)
      if result then
        local success = result and result.requestId ~= ""
        if callback ~= nil then
          callback(success, result)
        end
      end
    end,
    data = data,
    toIds = toIds,
    title = title,
    message = message
  })
end
function FacebookiOS:shareFeed(params, callback)
  if type(params) == "table" then
    luaoc.callStaticMethod("FacebookiOS", "shareFeed", {
      params = params,
      shareFeedCallback = function(result)
        if callback then
          local success = result ~= "failed" and result ~= "canceled"
          callback(success, result)
        end
      end
    })
  end
end
function FacebookiOS:updateAppRequest()
  luaoc.callStaticMethod("FacebookiOS", "getRequestId", {
    updateAppRequestCallback = handler(self, self._onGetRequestIdResult)
  })
  self._updateInviteRetryTimes = 3
end
function FacebookiOS:_onGetRequestIdResult(result)
  local isSuccess = result ~= "canceled" and result ~= "failed" and result ~= nil
  if isSuccess and result and result.requestData and result.requestId then
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.payload = result.requestData
    requestData.requestId = result.requestId
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("invite", "acceptInvite", requestData), function(data)
      local retData = json.decode(data)
      if retData and retData.ret and retData.ret == 0 then
        luaoc.callStaticMethod("FacebookiOS", "deleteRequestId", {
          requestId = result.requestId
        })
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
return FacebookiOS

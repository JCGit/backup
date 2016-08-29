local MessageInfo = class("MessageInfo")
function MessageInfo:ctor()
  self._messageInfoCacheData = nil
end
function MessageInfo:getCacheData(successCallback, failCallback)
  if self._messageInfoCacheData then
    successCallback(self._messageInfoCacheData)
  else
    self:refreshData(successCallback, failCallback)
  end
end
function MessageInfo:refreshData(successCallback, failCallback)
  if not self._isMessageListRequesting then
    self._isMessageListRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("messages", "list", requestData), function(data)
      logger:debug(data)
      self._isMessageListRequesting = false
      local retData = json.decode(data)
      if retData ~= nil then
        self._messageInfoCacheData = retData.info
      end
      if successCallback ~= nil then
        successCallback(self._messageInfoCacheData)
      end
    end, function()
      self._isMessageListRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function MessageInfo:markReadMsg(messageId)
end
function MessageInfo:deleteMessage(messageId, successCallback, failCallback)
  if not self._isDeleteRequesting then
    self._isDeleteRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.messageId = messageId
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("messages", "delete", requestData), function(data)
      for k, array in pairs(self._messageInfoCacheData) do
        for i, v in ipairs(array) do
          if v.messageId == messageId then
            table.remove(array, i)
            break
          end
        end
      end
      self._isDeleteRequesting = false
      local retData = json.decode(data)
      if tonumber(retData.ret) == 0 then
        if successCallback then
          successCallback(retData.info)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isDeleteRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function MessageInfo:receivePackage(messageId, successCallback, failCallback)
  if not self._isReceiveRequesting then
    self._isReceiveRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.messageId = messageId
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("messages", "receive", requestData), function(data)
      logger:debug(data)
      local retData = json.decode(data)
      self._isReceiveRequesting = false
      if tonumber(retData.ret) == 0 then
        if successCallback then
          successCallback()
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isReceiveRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function MessageInfo:acceptFriend(messageId, successCallback, failCallback)
  if not self._isAcceptRequesting then
    self._isAcceptRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.messageId = messageId
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("messages", "accept", requestData), function(data)
      local retData = json.decode(data)
      self._isAcceptRequesting = false
      if tonumber(retData.ret) == 0 then
        if successCallback then
          successCallback(retData.info)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isAcceptRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
return MessageInfo

local FriendsInfo = class("FriendsInfo")
function FriendsInfo:ctor()
  self._friendsInfoCacheData = nil
end
function FriendsInfo:getCacheData(successCallback, failCallback)
  if self._friendsInfoCacheData then
    successCallback(clone(self._friendsInfoCacheData))
  else
    return self:refreshData(successCallback, failCallback)
  end
end
function FriendsInfo:refreshData(successCallback, failCallback)
  if not self._isMyFriendListRequesting then
    self._isMyFriendListRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    local requestId = g.httpRequest:postMethod(g.requestParamsFactory:createRequest("friends", "list", requestData), function(data)
      logger:debug(data)
      self._isMyFriendListRequesting = false
      local retData = json.decode(data)
      if retData ~= nil then
        self._friendsInfoCacheData = retData.info
        if self._friendsInfoCacheData ~= nil then
          g.dataLogger:logFriendDistribute(#self._friendsInfoCacheData)
        end
      end
      if successCallback ~= nil then
        successCallback(clone(self._friendsInfoCacheData))
      end
    end, function()
      self._isMyFriendListRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
    return requestId
  end
end
function FriendsInfo:applyFriend(toUid, successCallback, failCallback)
  if not self._applyFriendRequesting then
    self._applyFriendRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.toUid = toUid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("friends", "apply", requestData), function(data)
      self._applyFriendRequesting = false
      if successCallback ~= nil then
        successCallback(json.decode(data))
      end
    end, function()
      self._applyFriendRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function FriendsInfo:delFriend(friendUid, successCallback, failCallback)
  if not self._delFriendRequesting then
    self._delFriendRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.friendUid = friendUid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("friends", "delete", requestData), function(data)
      self._delFriendRequesting = false
      if successCallback ~= nil then
        for i, v in ipairs(self._friendsInfoCacheData) do
          if v.uid == friendUid then
            table.remove(self._friendsInfoCacheData, i)
            break
          end
        end
        successCallback(json.decode(data))
      end
    end, function()
      self._delFriendRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function FriendsInfo:acceptFriend(sendUid, exp, name, sPic, chips, vip)
  if not self._acceptFriendRequesting then
    self._acceptFriendRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.sendUid = sendUid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("friends", "accept", requestData), function(data)
      self._acceptFriendRequesting = false
      local userData = {
        exp = exp,
        uid = sendUid,
        name = name,
        sPic = sPic,
        chips = chips,
        vip = vip
      }
      table.insert(self._friendsInfoCacheData, userData)
      if successCallback ~= nil then
        successCallback(json.decode(data))
      end
    end, function()
      self._acceptFriendRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function FriendsInfo:isMyFriends(uid)
  for i, friend in ipairs(self._friendsInfoCacheData) do
    if friend.uid == uid and uid ~= g.vars.user.uid then
      return true
    end
  end
  return false
end
function FriendsInfo:clearData()
  self._friendsInfoCacheData = nil
end
function FriendsInfo:searchUserRank(uid, successCallback, failCallback)
  if not self._searchRankRequesting then
    self._searchRankRequesting = true
    local requestData = {}
    requestData.targetUid = uid
    local requestId = g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "seatProfile", requestData), function(data)
      self._searchRankRequesting = false
      if data ~= nil then
        local retData = json.decode(data)
        if retData ~= nil and successCallback ~= nil then
          successCallback(retData)
          return
        end
      end
      if failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._searchRankRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
    return requestId
  end
end
return FriendsInfo

local GlobalVars = class("GlobalVars")
local TableUIEvent = require("app.module.table.view.TableUIEvent")
function GlobalVars:ctor()
  self.user = {}
  self.shop = {}
  self.bgScale = 1
  self.table = {}
  self.table.isPlayNow = false
  self.storageKeys = {}
  self.storageKeys.lastFacebookInviteReward = "storageKeys.lastFacebookInviteReward"
  self.storageKeys.facebookInvitedFriends = "storageKeys.facebookInvitedFriends2"
  self.storageKeys.volume = "storageKeys.volume"
  self.storageKeys.volumeEnable = "storageKeys.volumeEnable"
  self.storageKeys.bgm = "storageKeys.bgm"
  self.storageKeys.bgmEnable = "storageKeys.bgmEnable"
  self.storageKeys.autoBuy = "storageKeys.autoBuy"
  self.storageKeys.autoSit = "storageKeys.autoSit"
  self.storageKeys.shock = "storageKeys.shock"
  self.storageKeys.giftJson = "storageKeys.giftJson"
  self.storageKeys.lastGiftJson = "storageKeys.lastGiftJson"
  self.storageKeys.propJson = "storageKeys.propJson"
  self.storageKeys.lastPropJson = "storageKeys.lastPropJson"
  self.storageKeys.paymentsJson = "storageKeys.paymentsJson"
  self.storageKeys.lastPaymentsJson = "storageKeys.lastPaymentsJson"
  self.storageKeys.loginType = "storageKeys.loginType"
  self.storageKeys.proxyHeartbeatSeq = "storageKeys.proxyHeartbeatSeq"
  self.friend = {}
  self.friend.facebookInviteSelected = 0
  self.enterTableParams = nil
  self.isDuplicatedLogin = false
  self.isServerUpdateLogout = false
  self.isServerUStopLogout = false
  self.isWebP = false
end
function GlobalVars:syncChips(successCallback)
  if not self._isSyncRequesting then
    self._isSyncRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "synchronize", requestData), function(data)
      logger:debug(data)
      self._isSyncRequesting = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 and successCallback ~= nil then
        successCallback(retData.info)
      else
      end
    end, function()
      self._isSyncRequesting = false
    end)
  end
end
function GlobalVars:getLoginRewardCount()
  local loginRewards = self.user.loginRewards
  if loginRewards ~= nil then
    local loginReward = tonumber(loginRewards.loginReward or 0)
    local signReward = tonumber(loginRewards.signReward or 0)
    local vipReward = tonumber(loginRewards.vipReward or 0)
    local facebookReward = tonumber(loginRewards.facebookReward or 0)
    return loginReward + signReward + vipReward + facebookReward
  else
    return 0
  end
end
function GlobalVars:getImInBounsTime()
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.ImInBounsTime
  })
  return self.isIn or false
end
function GlobalVars:setImInCurrentGame(isIn)
  self.isIn = isIn
end
return GlobalVars

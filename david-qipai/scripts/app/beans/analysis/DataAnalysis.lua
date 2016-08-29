local DataAnalysis = class("DataAnalysis")
local DCAgent = require("app.dataeye.DCAgent")
local DCAccount = require("app.dataeye.DCAccount")
local DCEvent = require("app.dataeye.DCEvent")
local DCCardsGame = require("app.dataeye.DCCardsGame")
local DCVirtualCurrency = require("app.dataeye.DCVirtualCurrency")
local HttpConst = require("app.core.http.HttpConst")
function DataAnalysis:startService()
  if not self.serviceStarted and HttpConst.REQUEST_BASE == "https://d1ludf2n61l5sd.cloudfront.net/" then
    self.serviceStarted = true
    DCAgent.setReportMode(DC_AFTER_LOGIN)
    DCAgent.setDebugMode(true)
    if launchInfo ~= nil and launchInfo.version ~= nil then
      DCAgent.setVersion(launchInfo.version)
    else
      DCAgent.setVersion(g.native:getVersion())
    end
    DCAgent.onStart("7CD1624F5389F17F13C475D7092B55B2", g.native:getChannelId())
  end
end
function DataAnalysis:loginWithUid(accountId)
  if self.serviceStarted then
    DCAccount.login(accountId)
  end
end
function DataAnalysis:bindUserLevel(level)
  if self.serviceStarted then
    DCAccount.setLevel(tonumber(level or 1))
    local level = "等级" .. tostring(level or 1)
    self:logEvent("用户等级", level, true, false, true, true, false, false)
    self:logEvent("每日活跃用户数", nil, true, true, true, false)
    self:logUserChipsDistribute()
  end
end
local chipsTag = {
  "0 - 499",
  "500 - 3999",
  "4000 - 9999",
  "10000 - 99999",
  "100000 - 999999",
  "1000000 - 9999999",
  "10000000 - 99999999",
  ">100000000"
}
function DataAnalysis:logUserChipsDistribute()
  if self.serviceStarted then
    local labelStr
    local chips = tonumber(g.vars.user.chips or 0) + tonumber(g.vars.user.seatChips or 0)
    if chips <= 499 then
      labelStr = "0 - 499"
    elseif chips <= 3999 then
      labelStr = "500 - 3999"
    elseif chips <= 9999 then
      labelStr = "4000 - 9999"
    elseif chips <= 99999 then
      labelStr = "10000 - 99999"
    elseif chips <= 999999 then
      labelStr = "100000 - 999999"
    elseif chips <= 9999999 then
      labelStr = "1000000 - 9999999"
    elseif chips <= 99999999 then
      labelStr = "10000000 - 99999999"
    else
      labelStr = ">100000000"
    end
    self:logEvent("每日活跃用户筹码分布", labelStr, true, false, true, true, false, false)
    for i = 1, #chipsTag do
      if chipsTag[i] ~= labelStr then
        DCAccount.removeTag("总用户筹码分布", chipsTag[i])
      end
    end
    DCAccount.addTag("总用户筹码分布", labelStr)
  end
end
function DataAnalysis:logout()
  if self.serviceStarted then
    DCAccount.logout()
  end
end
function DataAnalysis:logAllLevel(level, maxLevel)
  if self.serviceStarted then
    for i = 1, maxLevel do
      if i ~= level then
        DCAccount.removeTag("总用户等级分布", "等级" .. level)
      end
    end
    DCAccount.addTag("总用户等级分布", "等级" .. level)
  end
end
function DataAnalysis:logFriendDistribute(friendCount)
  if self.serviceStarted then
    local labelStr
    if friendCount <= 0 then
      labelStr = "0"
    elseif friendCount <= 9 then
      labelStr = "1 - 9"
    elseif friendCount <= 29 then
      labelStr = "10 - 29"
    elseif friendCount <= 99 then
      labelStr = "30 - 99"
    else
      labelStr = ">=100"
    end
    self:logEvent("每日活跃用户好友数分布", labelStr, true, false, true, true, false, false)
  end
end
function DataAnalysis:logEvent(eventId, eventLabel, eventIdAsKey, eventLabelAsKey, userIdAsValue, dateAsValue, eventIdAsValue, eventLableAsValue)
  if self.serviceStarted then
    local currentTime = g.util.functions.getTime()
    local systemDate = os.date("*t", currentTime)
    local todayString = systemDate.year .. "/" .. systemDate.month .. "/" .. systemDate.day
    local hasSend = false
    local dataStorageKey = "DataAnalysis"
    if eventId ~= nil and eventIdAsKey then
      dataStorageKey = dataStorageKey .. "_" .. eventId
    end
    if eventLabel ~= nil and eventLabelAsKey then
      dataStorageKey = dataStorageKey .. "_" .. eventLabel
    end
    local dataStorageValue = "DataAnalysis"
    if userIdAsValue then
      dataStorageValue = dataStorageValue .. "_" .. g.vars.user.uid
    end
    if dateAsValue then
      dataStorageValue = dataStorageValue .. "_" .. todayString
    end
    if eventId and eventIdAsValue ~= nil then
      dataStorageValue = dataStorageValue .. "_" .. eventId
    end
    if eventLabel and eventLableAsValue ~= nil then
      dataStorageValue = dataStorageValue .. "_" .. eventLabel
    end
    if dataStorageKey ~= "DataAnalysis" then
      local preSavedVal = g.storage:getString(dataStorageKey, "")
      if preSavedVal == dataStorageValue then
        hasSend = true
      end
    end
    if not hasSend and self.serviceStarted then
      DCEvent.onEvent(eventId, eventLabel)
      if dataStorageValue ~= "DataAnalysis" then
        g.storage:setString(dataStorageKey, dataStorageValue)
        g.storage:flush()
      end
    end
  end
end
function DataAnalysis:logChipsProduce(chips, reason)
  if self.serviceStarted then
    reason = reason or "未知"
    DCCardsGame.gain("筹码生产", reason, "筹码", chips, g.vars.user.chips)
  end
end
function DataAnalysis:logChipsConsume(chips, reason)
  if self.serviceStarted then
    reason = reason or "未知"
    DCCardsGame.lost("筹码消耗", reason, "筹码", chips, g.vars.user.chips)
  end
end
function DataAnalysis:logPayment(channel, orderId, paymentId, amount, currency)
  if self.serviceStarted then
    DCVirtualCurrency.paymentSuccess(orderId, paymentId, amount, currency, channel)
  end
end
function DataAnalysis:logPing(delay)
  if self.serviceStarted then
    local labelStr
    if delay <= 99 then
      labelStr = "0 - 99ms"
    elseif delay <= 299 then
      labelStr = "100 - 299ms"
    elseif delay <= 799 then
      labelStr = "300 - 799ms"
    elseif delay <= 1500 then
      labelStr = "800 - 1500ms"
    else
      labelStr = "NA"
    end
    self:logEvent("每日活跃用户Ping分布", labelStr, true, false, true, true, false, false)
  end
end
function DataAnalysis:logInviteInstalled(type)
  if self.serviceStarted then
    self:logEvent("邀请安装数", type, true, false, true, true, false, false)
  end
end
function DataAnalysis:logInviteSent(type)
  if self.serviceStarted then
    self:logEvent("邀请发送数", type, true, false, true, true, false, false)
  end
end
function DataAnalysis:logFeedSent(type)
  if self.serviceStarted then
    self:logEvent("Feed发送数", type, true, false, true, true, false, false)
  end
end
function DataAnalysis:logPlayCount(roomId, roomType, seatChips, userChips)
  if self.serviceStarted then
    self:logEvent("每日玩牌总局数")
    self:logEvent("每日玩牌用户数", nil, true, false, true, true, false, false)
  end
end
function DataAnalysis:logPHPApiTime(apiName, duration)
  if self.serviceStarted then
    local label = ""
    if duration < 0.2 then
      label = "<0.2s"
    elseif duration < 0.8 then
      label = "0.2-0.8s"
    elseif duration <= 3 then
      label = "0.8-3s"
    elseif duration > 3 then
      label = ">3s"
    end
    self:logEvent("接口访问时间", label)
    local phpApiName = apiName or "unknow api"
    self:logEvent("接口具体访问分布时间", phpApiName .. ":" .. label)
  end
end
function DataAnalysis:logPHPApiCount(apiName)
  local phpApiName = apiName or "unknow api"
  self:logEvent("接口具体访问分布次数", phpApiName)
end
return DataAnalysis

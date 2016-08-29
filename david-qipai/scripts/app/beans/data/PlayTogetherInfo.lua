local PlayTogetherInfo = class("PlayTogetherInfo")
PlayTogetherInfo.ACTIVITY_OVER = 1
PlayTogetherInfo.ACTIVITY_ING_NOTAPPLY_NOT_COUNT = 2
PlayTogetherInfo.ACTIVITY_ING_NOTAPPLY_IN_COUNT = 3
PlayTogetherInfo.ACTIVITY_ING_APPLY_NOT_COUNT = 4
PlayTogetherInfo.ACTIVITY_ING_APPLY_IN_COUNT = 5
function PlayTogetherInfo:ctor()
  self.dataModel = {}
  local function timerCallback(countDownSec)
    local needReSync = false
    if self.dataModel.startTimeOffset ~= nil then
      self.dataModel.startTimeOffset = self.dataModel.startTimeOffset - 1
      if self.dataModel.startTimeOffset == 30 and self.dataModel.startTimeOffset == 0 then
        needReSync = true
      end
    end
    if self.dataModel.endTimeOffset ~= nil then
      self.dataModel.endTimeOffset = self.dataModel.endTimeOffset - 1
      if self.dataModel.endTimeOffset == 30 and self.dataModel.endTimeOffset == 0 then
        needReSync = true
      end
    end
    if self.dataModel.activityTimeStart ~= nil then
      self.dataModel.activityTimeStart = self.dataModel.activityTimeStart - 1
      if self.dataModel.activityTimeStart == 30 and self.dataModel.activityTimeStart == 0 then
        needReSync = true
      end
    end
    if self.dataModel.activityTimeEnd ~= nil then
      self.dataModel.activityTimeEnd = self.dataModel.activityTimeEnd - 1
      if self.dataModel.activityTimeEnd == 30 and self.dataModel.activityTimeEnd == 0 then
        needReSync = true
      end
    end
    if needReSync then
      self:playTogetherProgress()
    end
    if self.progressTickCountCallback ~= nil then
      self.progressTickCountCallback()
    end
  end
  self.timerHandler = scheduler.scheduleGlobal(timerCallback, 1)
end
function PlayTogetherInfo:playTogetherApply(successCallback, failCallback)
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("gather_play_activity", "apply", requestData), function(data)
    logger:debug(data)
    local retData = json.decode(data)
    if retData ~= nil and tonumber(retData.ret) == 0 then
      if successCallback ~= nil then
        self.dataModel.applyStatus = 1
        successCallback(retData)
      end
    elseif failCallback ~= nil then
      failCallback()
    end
  end, function()
    if failCallback ~= nil then
      failCallback()
    end
  end)
end
function PlayTogetherInfo:playTogetherProgress()
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("gather_play_activity", "progress", requestData), function(data)
    logger:debug(data)
    local retData = json.decode(data)
    if retData ~= nil and tonumber(retData.ret) == 0 then
      local retInfo = retData.info
      if retInfo then
        self.dataModel = retInfo
      end
      if self.progressSuccessbackUIRefresh ~= nil then
        self.progressSuccessbackUIRefresh(retData)
      end
    elseif self.progressFailbackUIRefresh ~= nil then
      self.progressFailbackUIRefresh()
    end
  end, function()
    if self.progressFailbackUIRefresh ~= nil then
      self.progressFailbackUIRefresh()
    end
  end)
end
function PlayTogetherInfo:playTogetherReward(stage, successCallback, failCallback)
  local requestData = {}
  requestData.uid = g.vars.user.uid
  requestData.stage = stage
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("gather_play_activity", "getReward", requestData), function(data)
    logger:debug(data)
    local retData = json.decode(data)
    if retData ~= nil and tonumber(retData.ret) == 0 then
      if successCallback ~= nil then
        successCallback(retData)
      end
    elseif failCallback ~= nil then
      failCallback()
    end
  end, function()
    if failCallback ~= nil then
      failCallback()
    end
  end)
end
function PlayTogetherInfo:playTogetherReport()
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("gather_play_activity", "reportPlay", requestData), function(data)
  end, function()
  end)
end
function PlayTogetherInfo:getActivityState()
  if g.playTogetherInfo.dataModel.startTimeOffset == nil and g.playTogetherInfo.dataModel.endTimeOffset == nil then
    return PlayTogetherInfo.ACTIVITY_OVER
  elseif tonumber(g.playTogetherInfo.dataModel.applyStatus or 0) == 0 and g.playTogetherInfo.dataModel.startTimeOffset > 0 then
    return PlayTogetherInfo.ACTIVITY_ING_NOTAPPLY_NOT_COUNT
  elseif tonumber(g.playTogetherInfo.dataModel.applyStatus or 0) == 0 and g.playTogetherInfo.dataModel.startTimeOffset <= 0 then
    return PlayTogetherInfo.ACTIVITY_ING_NOTAPPLY_IN_COUNT
  elseif tonumber(g.playTogetherInfo.dataModel.applyStatus or 0) == 1 and g.playTogetherInfo.dataModel.startTimeOffset > 0 then
    return PlayTogetherInfo.ACTIVITY_ING_APPLY_NOT_COUNT
  elseif tonumber(g.playTogetherInfo.dataModel.applyStatus or 0) == 1 and g.playTogetherInfo.dataModel.startTimeOffset <= 0 then
    return PlayTogetherInfo.ACTIVITY_ING_APPLY_IN_COUNT
  end
end
return PlayTogetherInfo

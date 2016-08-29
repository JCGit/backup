local JewelBoxInfo = class("JewelBoxInfo")
JewelBoxInfo.NO_ACTIVITY = 1
JewelBoxInfo.ACTIVITY_NOT_START = 2
JewelBoxInfo.ACTIVITY_IN_COUNT = 3
JewelBoxInfo.ACTIVITY_REWARD = 4
function JewelBoxInfo:ctor()
  self.dataModel = {}
  self._boxState = JewelBoxInfo.NO_ACTIVITY
end
function JewelBoxInfo:clearData()
  self.dataModel = {}
  self.noActivity = false
end
function JewelBoxInfo:progress(successCallback, failCallback)
  if self._isRequestingProgress then
    return
  end
  self._isRequestingProgress = true
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("jewel_box_activity", "progress", requestData), function(data)
    self._isRequestingProgress = false
    print("!!!!!!!!!!!!!progress!!!!!!!!!!!!!!!!")
    logger:debug(data)
    local retData = json.decode(data)
    if retData ~= nil and (retData.ret == -2 or retData.ret == -5 or retData.ret == -7 or retData.ret == -15) then
      self.noActivity = true
    elseif retData.ret == 0 then
      self.noActivity = false
      self.dataModel = retData.info
      if successCallback ~= nil then
        successCallback(retData)
      end
    else
      self.noActivity = true
      if failCallback ~= nil then
        failCallback()
      end
    end
  end, function()
    if failCallback ~= nil then
      failCallback()
    end
  end)
end
function JewelBoxInfo:reward(successCallback, failCallback)
  local requestData = {}
  requestData.uid = g.vars.user.uid
  logger:debug(g.util.inspect(requestData))
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("jewel_box_activity", "getReward", requestData), function(data)
    logger:debug(data)
    local retData = json.decode(data)
    if retData ~= nil and tonumber(retData.ret) == 0 then
      if successCallback ~= nil then
        successCallback(retData)
      end
    elseif failCallback ~= nil then
      failCallback(tonumber(retData.ret or 0))
    end
  end, function()
    if failCallback ~= nil then
      failCallback()
    end
  end)
end
function JewelBoxInfo:getBoxState()
  local boxState = JewelBoxInfo.NO_ACTIVITY
  if self.noActivity == true then
    boxState = JewelBoxInfo.NO_ACTIVITY
  elseif tonumber(self.dataModel.Status or 0) == 1 then
    boxState = JewelBoxInfo.ACTIVITY_REWARD
  elseif self.dataModel.leftTime == nil then
    boxState = JewelBoxInfo.NO_ACTIVITY
  elseif 0 <= tonumber(self.dataModel.leftTime or 0) then
    if g.vars:getImInBounsTime() then
      boxState = JewelBoxInfo.ACTIVITY_IN_COUNT
    else
      boxState = JewelBoxInfo.ACTIVITY_NOT_START
    end
  end
  return boxState
end
return JewelBoxInfo

local InviteInfo = class("InviteInfo")
function InviteInfo:ctor()
end
function InviteInfo:activityApply(successCallback, failCallback)
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("invite_activity", "apply", requestData), function(data)
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
function InviteInfo:activityProgress(successCallback, failCallback)
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("invite_activity", "progress", requestData), function(data)
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
function InviteInfo:activityReward(stage, successCallback, failCallback)
  local requestData = {}
  requestData.uid = g.vars.user.uid
  requestData.stage = stage
  logger:debug(g.util.inspect(requestData))
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("invite_activity", "reward", requestData), function(data)
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
return InviteInfo

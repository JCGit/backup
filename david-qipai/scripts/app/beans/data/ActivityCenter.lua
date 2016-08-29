local ActivityCenter = class("ActivityCenter")
local PlayTogetherInfo = require("app.beans.data.PlayTogetherInfo")
function ActivityCenter:ctor()
  self.activityList = nil
end
function ActivityCenter:requestActiviteList()
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("center_activity", "list", requestData), function(data)
    local retData = json.decode(data)
    if retData ~= nil and tonumber(retData.ret) == 0 then
      self.activityList = retData.info
      for i, activity in ipairs(self.activityList) do
        local activityID = tonumber(activity.activityID or 0)
        if activityID == 8 then
          g.playTogetherInfo = PlayTogetherInfo.new()
          g.playTogetherInfo:playTogetherProgress()
        end
      end
    else
    end
  end, function()
  end)
end
function ActivityCenter:getActivityNeedShowRedDot(activityID)
  for i, activity in ipairs(self.activityList) do
    if tonumber(activity.activityID or 0) == tonumber(activityID) then
      if tonumber(activity.rewardStatus) == 1 then
        return true
      else
        return false
      end
    end
  end
  return false
end
function ActivityCenter:setActivityRedDotReaded(activityID)
  for i, item in ipairs(self.activityList) do
    if tonumber(item.activityID or 0) == tonumber(activityID) then
      item.rewardStatus = 0
      return
    end
  end
end
function ActivityCenter:enableActivityRedDotState(activityID)
  for i, item in ipairs(self.activityList) do
    if tonumber(item.activityID or 0) == tonumber(activityID) then
      item.rewardStatus = 1
      return
    end
  end
end
return ActivityCenter

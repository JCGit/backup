local SystemControlInfo = class("SystemControlInfo")
function SystemControlInfo:ctor()
end
function SystemControlInfo:reportNewDay()
  if not self._isRequstingReportNewDay then
    self._isRequstingReportNewDay = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "resetDay", requestData), function(data)
      self._isRequstingReportNewDay = false
    end, function()
      self._isRequstingReportNewDay = false
    end)
  end
end
return SystemControlInfo

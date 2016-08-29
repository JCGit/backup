local ChartsInfo = class("ChartsInfo")
ChartsInfo.CHIPS_TYPE = 0
ChartsInfo.WIN_TYPE = 1
ChartsInfo.INCREASE_TYPE = 2
function ChartsInfo:ctor()
  self._chartsInfoCacheData = {}
  self._chartsInfoCacheData.chipsRankData = nil
  self._chartsInfoCacheData.winRankData = nil
  self._chartsInfoCacheData.increaseRankData = nil
end
function ChartsInfo:getCacheData(requestType, successCallback, failCallback)
  local cacheData
  if requestType == ChartsInfo.CHIPS_TYPE then
    cacheData = self._chartsInfoCacheData.chipsRankData
  elseif requestType == ChartsInfo.WIN_TYPE then
    cacheData = self._chartsInfoCacheData.winRankData
  elseif requestType == ChartsInfo.INCREASE_TYPE then
    cacheData = self._chartsInfoCacheData.increaseRankData
  end
  if cacheData ~= nil then
    successCallback(requestType, cacheData)
  else
    self:refreshData(requestType, successCallback, failCallback)
  end
end
function ChartsInfo:refreshData(requestType, successCallback, failCallback)
  if not self._isChartListRequesting then
    self._isChartListRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.chartsType = tostring(requestType)
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("charts", "chip", requestData), function(data)
      self._isChartListRequesting = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        if requestType == ChartsInfo.CHIPS_TYPE then
          self._chartsInfoCacheData.chipsRankData = retData.info
        elseif requestType == ChartsInfo.WIN_TYPE then
          self._chartsInfoCacheData.winRankData = retData.info
        elseif requestType == ChartsInfo.INCREASE_TYPE then
          self._chartsInfoCacheData.increaseRankData = retData.info
        end
        if successCallback ~= nil then
          successCallback(requestType, retData.info)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isChartListRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function ChartsInfo:clearData()
  self._chartsInfoCacheData = {}
end
return ChartsInfo

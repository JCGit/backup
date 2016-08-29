local RouletteInfo = class("RouletteInfo")
function RouletteInfo:ctor()
  self._rouletteInfoCacheData = nil
  self.rouletteConfig = nil
end
function RouletteInfo:getConfig(successCallback, failCallback)
  if not self._isRouletteRequesting1 then
    self._isRouletteRequesting1 = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("roulettes", "config", requestData), function(data)
      logger:debug(data)
      self._isRouletteRequesting1 = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        if successCallback ~= nil then
          self.rouletteConfig = retData.info
          successCallback(retData.info)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isRouletteRequesting1 = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function RouletteInfo:getChances(successCallback, failCallback)
  if not self._isRouletteRequesting2 then
    self._isRouletteRequesting2 = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("roulettes", "get", requestData), function(data)
      logger:debug(data)
      self._isRouletteRequesting2 = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        self._rouletteInfoCacheData = retData.info
        if successCallback ~= nil then
          successCallback(self._rouletteInfoCacheData)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isRouletteRequesting2 = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function RouletteInfo:playRoulette(successCallback, failCallback)
  if not self._isRouletteRequesting3 then
    self._isRouletteRequesting3 = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("roulettes", "spin", requestData), function(data)
      logger:debug(data)
      self._isRouletteRequesting3 = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        self._rouletteInfoCacheData.spin = retData.info.spin
        if successCallback ~= nil then
          successCallback(retData.info)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isRouletteRequesting3 = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
return RouletteInfo

local TablesInfo = class("TablesInfo")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
local SocketBeanEvent = require("app.beans.socket.event.SocketBeanEvent")
function TablesInfo:ctor()
  self._tablesInfoCacheData = nil
end
function TablesInfo:getCacheData(successCallback, failCallback)
  local request
  if self._tablesInfoCacheData then
    successCallback(self._tablesInfoCacheData)
  else
    request = self:refreshData(successCallback, failCallback)
  end
  return request
end
function TablesInfo:refreshData(successCallback, failCallback)
  local request
  if not self._isTableListRequesting then
    if not io.exists(device.writablePath .. "upd") then
      lfs.mkdir(device.writablePath .. "upd")
    end
    do
      local cacheFileName = device.writablePath .. "upd" .. "/" .. "TableListCache"
      self._isTableListRequesting = true
      local requestData = {}
      requestData.uid = g.vars.user.uid
      request = g.httpRequest:postMethod(g.requestParamsFactory:createRequest("tables", "lobby", requestData), function(data)
        logger:debug(data)
        self._isTableListRequesting = false
        local retData = json.decode(data)
        if retData ~= nil and tonumber(retData.ret) == 0 then
          self._tablesInfoCacheData = retData.info
          if successCallback ~= nil then
            successCallback(self._tablesInfoCacheData)
            io.writefile(cacheFileName, data)
          end
        elseif failCallback ~= nil then
          failCallback()
        end
      end, function()
        self._isTableListRequesting = false
        if failCallback ~= nil then
          failCallback()
        end
      end)
      local file = io.open(cacheFileName, "rb")
      if file ~= nil then
        local data = file:read("*all")
        if data ~= nil then
          local retData = json.decode(data)
          if retData ~= nil and tonumber(retData.ret) == 0 then
            self._tablesInfoCacheData = retData.info
            if successCallback ~= nil then
              successCallback(self._tablesInfoCacheData)
            end
          end
        end
      end
    end
  end
  return request
end
function TablesInfo:getPlayTableWithClassId(classId, successCallback, failCallback)
  self:getPlayTable(classId, nil, successCallback, failCallback)
end
function TablesInfo:getPlayTableWithTid(tid, successCallback, failCallback)
  self:getPlayTable(nil, tid, successCallback, failCallback)
end
function TablesInfo:getPlayTable(classId, tid, successCallback, failCallback)
  showSceneLoading(g.lang:getContent("lobby.tableLoading"))
  if not self._isGetTableRequesting then
    self._isGetTableRequesting = true
    do
      local action
      local requestData = {}
      requestData.uid = g.vars.user.uid
      if tid ~= nil then
        requestData.tid = tid
        action = "get"
      else
        requestData.classId = classId
        action = "play"
      end
      logger:debug(g.util.inspect(requestData))
      g.httpRequest:postMethod(g.requestParamsFactory:createRequest("tables", action, requestData), function(data)
        logger:debug(g.util.inspect(data))
        self._isGetTableRequesting = false
        local retData = json.decode(data)
        if retData ~= nil and tonumber(retData.ret) == 0 then
          local tableDetailInfo = retData.info
          g.eventCenter:dispatchEvent({
            name = LobbyBeanEvent.EVT_ENTER_TABLE,
            data = {
              tid = tableDetailInfo.tid,
              ip = tableDetailInfo.ip,
              port = tonumber(tableDetailInfo.port),
              isCreate = 0
            }
          })
          if successCallback ~= nil then
            successCallback(tableDetailInfo)
          end
          if tableDetailInfo.clientIp ~= g.vars.user.clientIp then
            g.eventCenter:dispatchEvent({
              name = SocketBeanEvent.EVT_DISCONNECT
            })
            g.vars.user.clientIp = tableDetailInfo.clientIp
            g.dataLogger:logEvent("关键监测指标", "tableIP不一致")
          else
            g.dataLogger:logEvent("关键监测指标", "tableIP一致")
          end
        else
          if tonumber(retData.ret) == -1 then
            g.eventCenter:dispatchEvent({
              name = LobbyBeanEvent.EVT_USERINFO_EXPIRE
            })
          elseif tonumber(retData.ret) == -2 then
            if action == "get" then
              g.ui.manager.tip:top(g.lang:getContent("table.loginTableFailSearchNotFound"))
            else
              g.ui.manager.tip:top(g.lang:getContent("table.loginTableFailNotFoundTable"))
            end
          elseif tonumber(retData.ret) == -8 then
            g.ui.manager.tip:top(g.lang:getContent("table.loginTableFailNotEnoughMoney"))
          else
            g.ui.manager.tip:top(g.lang:getContent("table.loginTableFail"))
          end
          hideSceneLoading()
          if failCallback ~= nil then
            failCallback()
          end
        end
      end, function()
        self._isGetTableRequesting = false
        g.ui.manager.tip:top(g.lang:getContent("table.loginTableFail"))
        hideSceneLoading()
        if failCallback ~= nil then
          failCallback()
        end
      end)
    end
  end
end
function TablesInfo:getTotalOnline(successCallback, failCallback)
  self._isTotalRequesting = true
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("tables", "getTotalOnline", requestData), function(data)
    logger:debug(data)
    self._isTotalRequesting = false
    local retData = json.decode(data)
    if retData ~= nil and tonumber(retData.ret) == 0 then
      if successCallback ~= nil then
        successCallback(retData)
      end
    elseif failCallback ~= nil then
      failCallback()
    end
  end, function()
    self._isTotalRequesting = false
    if failCallback ~= nil then
      failCallback()
    end
  end)
end
return TablesInfo

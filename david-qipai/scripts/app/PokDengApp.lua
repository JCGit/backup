require("config")
require("framework.init")
require("app.thirdparty.logger.logger")
require("app.init")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
local SocketBeanEvent = require("app.beans.socket.event.SocketBeanEvent")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local BroadcastBeanEvent = require("app.beans.broadcast.event.BroadcastBeanEvent")
local TableUIEvent = require("app.module.table.view.TableUIEvent")
local SocketBean = require("app.beans.socket.SocketBean")
local DataBean = require("app.beans.data.DataBean")
local TableScene = require("app.scenes.TableScene")
local LobbyScene = require("app.scenes.LobbyScene")
local HttpConst = require("app.core.http.HttpConst")
local PokDengApp = class("PokDengApp", cc.mvc.AppBase)
function PokDengApp:ctor(hasRestart)
  PokDengApp.super.ctor(self)
  g.app = self
end
function PokDengApp:run()
  CCFileUtils:sharedFileUtils():addSearchPath(device.writablePath .. "upd/")
  CCFileUtils:sharedFileUtils():addSearchPath("res/")
  if device.platform == "ios" then
    g.dataLogger:startService()
  elseif device.platform == "android" and HttpConst.REQUEST_BASE == "https://d1ludf2n61l5sd.cloudfront.net" then
    g.dataLogger.serviceStarted = true
  end
  self._socketBean = SocketBean.new()
  self._dataBean = DataBean.new()
  self:enterScene("LobbyScene")
  self:addEvent()
end
function PokDengApp:addEvent()
  self._connectSuccessHandle = g.eventCenter:addEventListener(SocketBeanEvent.EVT_CONNECT_SUCCESS, handler(self, self.onProxyConnectSuccess))
  self._connectFailHandle = g.eventCenter:addEventListener(SocketBeanEvent.EVT_CONNECT_FAILURE, handler(self, self.onProxyConnectFail))
  self._connectCloseHandle = g.eventCenter:addEventListener(SocketBeanEvent.EVT_CLOSED, handler(self, self.onProxyConnectClose))
  self._connectGameSuccessHandle = g.eventCenter:addEventListener(SocketBeanEvent.EVT_CONNECT_GAME_SUCCESS, handler(self, self.onConnectGameSuccess))
  self._connectBroadcastSuccessHandle = g.eventCenter:addEventListener(SocketBeanEvent.EVT_CONNECT_BROADCAST_SUCCESS, handler(self, self.onConnectBroadcastSuccess))
  self._enterTableHandle = g.eventCenter:addEventListener(LobbyBeanEvent.EVT_ENTER_TABLE, handler(self, self.onEnterTable))
  self._loginGameSuccessHandle = g.eventCenter:addEventListener(LobbyBeanEvent.EVT_LOGIN_SUCCESS, handler(self, self.onLoginSuccess))
end
function PokDengApp:removeEvent()
end
function PokDengApp:onLoginSuccess(evt)
  logger:debug("PokDengApp:onLoginSuccess(evt)")
  g.vars.user.isChangeHeadPicPermited = false
  if g.vars.user.loginType == "device" then
    g.vars.user.isChangeHeadPicPermited = true
  end
  g.eventCenter:dispatchEvent({
    name = SocketBeanEvent.EVT_CONNECT,
    data = {
      host = g.vars.user.proxy.ip,
      port = g.vars.user.proxy.port
    }
  })
end
function PokDengApp:onProxyConnectSuccess(evt)
  logger:debug("PokDengApp:onProxyConnectSuccess(evt)")
  g.vars.user.proxy.connected = true
  if g.vars.user then
    g.eventCenter:dispatchEvent({
      name = SocketBeanEvent.EVT_CONNECT_BROADCAST,
      data = {
        host = g.vars.user.broadcast.ip,
        port = g.vars.user.broadcast.port
      }
    })
  end
  if g.vars.enterTableParams ~= nil then
    g.eventCenter:dispatchEvent({
      name = SocketBeanEvent.EVT_CONNECT_GAME,
      data = {
        host = g.vars.enterTableParams.ip,
        port = g.vars.enterTableParams.port
      }
    })
  end
  if g.vars.table.loginRequest ~= nil then
    g.eventCenter:dispatchEvent({
      name = SocketBeanEvent.EVT_CONNECT_GAME,
      data = {
        host = g.vars.table.loginRequest.ip,
        port = g.vars.table.loginRequest.port
      }
    })
  end
end
function PokDengApp:onConnectGameSuccess(evt)
  logger:debug("PokDengApp:onConnectGameSuccess(evt)")
  if DEBUG_RECORD_MODE then
    g.record_isConnect_ready = true
  end
  if g.vars.enterTableParams ~= nil then
    self:loginTable()
  elseif g.vars.table.loginResponse ~= nil then
    g.eventCenter:dispatchEvent({
      name = TableUIEvent.Player.RECONNECT_REQUEST
    })
  end
end
function PokDengApp:onConnectBroadcastSuccess(evt)
  logger:debug("PokDengApp:onConnectBroadcastSuccess(evt)")
  g.eventCenter:dispatchEvent({
    name = BroadcastBeanEvent.EVT_CLI_LOGIN,
    data = {
      uid = g.vars.user.uid,
      loginKey = g.vars.user.loginKey
    }
  })
end
function PokDengApp:onProxyConnectFail(evt)
  logger:debug("PokDengApp:onProxyConnectFail(evt)")
  g.vars.user.proxy.connected = false
  g.eventCenter:dispatchEvent({
    name = SocketBeanEvent.EVT_CONNECT,
    data = {
      host = g.vars.user.proxy.ip,
      port = g.vars.user.proxy.port
    }
  })
end
function PokDengApp:onProxyConnectClose(evt)
  logger:debug("PokDengApp:onProxyConnectClose(evt)")
  if g.vars.user.uid ~= nil then
    g.vars.user.proxy.connected = false
    g.eventCenter:dispatchEvent({
      name = SocketBeanEvent.EVT_CONNECT,
      data = {
        host = g.vars.user.proxy.ip,
        port = g.vars.user.proxy.port
      }
    })
  end
end
function PokDengApp:onEnterTable(evt)
  logger:debug("PokDengApp:onEnterTable(evt)")
  g.vars.user.proxy = g.vars.user.proxy or {}
  g.vars.table = g.vars.table or {}
  g.vars.enterTableParams = evt.data
  if g.vars.user.proxy.connected then
    g.eventCenter:dispatchEvent({
      name = SocketBeanEvent.EVT_CONNECT_GAME,
      data = {
        host = g.vars.enterTableParams.ip,
        port = tonumber(g.vars.enterTableParams.port)
      }
    })
  end
  self._isLoadedTableResource = false
  self._isLoginTable = false
  self._loadedTableResourceCount = 0
  CCTextureCache:sharedTextureCache():addImageAsync("pd_table_bg.jpg", handler(self, self.onTableResourceLoaded))
end
function PokDengApp:loginTable()
  local data = {
    uid = g.vars.user.uid,
    loginKey = g.vars.user.loginKey,
    ip = g.vars.enterTableParams.ip,
    port = g.vars.enterTableParams.port,
    tid = g.vars.enterTableParams.tid,
    isCreate = g.vars.enterTableParams.isCreate or 0,
    sPic = g.vars.user.sPic,
    giftId = g.vars.user.giftId
  }

  self._loginSuccessHandle = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_LOGIN_SUCCESS, handler(self, self.onTableLoginSuccess))
  self._loginFailHandle = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_LOGIN_FAIL, handler(self, self.onTableLoginFail))
  g.vars.table.loginRequest = data
  logger:debug(g.util.inspect(data))
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_LOGIN,
    data = data
  })
end
function PokDengApp:onTableResourceLoaded()
  logger:debug("PokDengApp:onTableResourceLoaded()")
  self._loadedTableResourceCount = self._loadedTableResourceCount + 1
  if self._loadedTableResourceCount == 1 then
    loadTexture("pd_table_texture.plist", "pd_table_texture.png", handler(self, self.onTableResourceLoaded))
  elseif self._loadedTableResourceCount == 2 then
    if self._isLoginTable then
      self:enterScene("TableScene", {}, "fade", 0.5)
    end
    self._isLoadedTableResource = true
  end
end
function PokDengApp:onTableLoginSuccess(evt)
  logger:debug("PokDengApp:onTableLoginSuccess(evt)")
  g.vars.table.loginResponse = evt.data
  g.vars.enterTableParams = nil
  if self._isLoadedTableResource then
    self:enterScene("TableScene", {}, "fade", 0.5)
  end
  g.eventCenter:removeEventListener(self._loginSuccessHandle)
  g.eventCenter:removeEventListener(self._loginFailHandle)
  if self._logoutHandle == nil then
    self._logoutHandle = g.eventCenter:addEventListener(LobbyBeanEvent.EVT_QUIT_TO_LOBBY, handler(self, self.onRequestLogout))
  end
  self._isLoginTable = true
end
function PokDengApp:onTableLoginFail(evt)
  logger:debug("PokDengApp:onTableLoginFail(evt)")
  hideSceneLoading()
  local packetModel = evt.data
  if packetModel.errorCode == 1 then
    g.ui.manager.tip:top(g.lang:getContent("table.loginTableFail"))
  elseif packetModel.errorCode == 2 then
    g.ui.manager.tip:top(g.lang:getContent("table.loginTableFailSearchNotFound"))
  elseif packetModel.errorCode == 19 then
    g.eventCenter:dispatchEvent({
      name = LobbyBeanEvent.EVT_USERINFO_EXPIRE
    })
  else
    g.ui.manager.tip:top(g.lang:getContent("table.loginTableFail"))
  end
  g.eventCenter:removeEventListener(self._loginSuccessHandle)
  g.eventCenter:removeEventListener(self._loginFailHandle)
end
function PokDengApp:onRequestLogout(evt)
  logger:debug("PokDengApp:onRequestLogout(evt)")
  g.vars.enterTableParams = nil
  g.vars.table.loginRequest = nil
  g.vars.table.loginResponse = nil
  self:enterScene("LobbyScene", {}, "fade", 0.5)
  if DEBUG_MEMORY_STATUS then
    local blockToDispatchCards = function()
      g.memoryManager:snapshot()
    end
    scheduler.performWithDelayGlobal(blockToDispatchCards, 2)
  end
end
function PokDengApp:onEnterBackground()
  self:dispatchEvent({
    name = cc.mvc.AppBase.APP_ENTER_BACKGROUND_EVENT
  })
  logger:debug("PokDengApp:onEnterBackground")
end
function PokDengApp:onEnterForeground()
  self:dispatchEvent({
    name = cc.mvc.AppBase.APP_ENTER_FOREGROUND_EVENT
  })
  logger:debug("PokDengApp:onEnterForeground")
end
return PokDengApp

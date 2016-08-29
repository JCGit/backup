local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local BroadcastBeanEvent = require("app.beans.broadcast.event.BroadcastBeanEvent")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local LevelUpView = require("app.beans.levelup.view.LevelUpView")
local LobbyScene = require("app.scenes.LobbyScene")
local DataBean = class("DataBean")
function DataBean:ctor()
  self:addEvent()
end
function DataBean:addEvent()
  self._logoutBroadcastSuccessHandle = g.eventCenter:addEventListener(BroadcastBeanEvent.EVT_SVR_LOGOUT_SUCCESS, handler(self, self.onLogoutBroadcastSuccess))
  self._logoutTableSuccessHandle = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_LOGOUT_SUCCESS, handler(self, self.onLogoutTableSuccess))
end
function DataBean:onLogoutBroadcastSuccess(evt)
  local packet = evt.data
  if packet.errorCode == 3 then
    g.vars.isDuplicatedLogin = true
  end
end
function DataBean:onLogoutTableSuccess(evt)
  local packet = evt.data
  if packet.errorCode == 4 then
    g.vars.isServerUpdateLogout = true
  elseif packet.errorCode == 5 then
    g.vars.isServerUStopLogout = true
  end
end
function DataBean:onLevelUp(evt)
  local level = tonumber(evt.data.message.info.lv)
  if level >= 3 then
    LevelUpView.new(tonumber(level), LobbyScene.SHOW_MODE):show()
  end
end
function DataBean:removeEvent()
  if self._levelupHandle ~= nil then
    g.eventCenter:removeEventListener(self._levelupHandle)
    self._levelupHandle = nil
  end
  if self._logoutBroadcastSuccessHandle ~= nil then
    g.eventCenter:removeEventListener(self._logoutBroadcastSuccessHandle)
    self._logoutBroadcastSuccessHandle = nil
  end
  if self._logoutTableSuccessHandle ~= nil then
    g.eventCenter:removeEventListener(self._logoutTableSuccessHandle)
    self._logoutTableSuccessHandle = nil
  end
end
return DataBean

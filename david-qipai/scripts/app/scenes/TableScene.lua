local SocketEvent = require("app.core.socket.SocketEvent")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local TableUIManager = require("app.module.table.view.TableUIManager")
local TableUIEventCenter = require("app.module.table.controller.TableUIEventCenter")
local TableUIEvent = require("app.module.table.view.TableUIEvent")
local PlayStatus = require("app.beans.table.model.PlayStatus")
local Judge = require("app.module.table.model.Judge")
local ViceJudge = require("app.module.table.model.ViceJudge")
local Player = require("app.module.table.model.Player")
local PlayerConstants = require("app.module.table.model.PlayerConstants")
local TableScene = class("TableScene", function()
  return display.newScene("TableScene")
end)
function TableScene:ctor()
  if DEBUG_MEMORY_STATUS then
    logger:debug("Object-Life:" .. self.__cname .. ": ctor")
    g.memoryManager:registerObject(self, "TableScene")
  end
  self._tableUIManager = TableUIManager.new(self)
  self._judge = Judge.new()
  self._viceJudge = ViceJudge.new()
  self._tableUIEventCenter = TableUIEventCenter.new(self._judge, self._tableUIManager)
  self._judge:unicastEnterTableSuccessInfo(g.vars.table.loginResponse)
  local isAutoSit = g.storage:getBool(g.vars.storageKeys.autoSit)
  if not self._judge:getIsImPlayer() and (isAutoSit and not g.vars.table.isPlayNow or g.vars.table.isPlayNow) then
    local seat = self._tableUIManager:getEmptySeat()
    if seat ~= nil then
      logger:debug("TableScene:ctor():EVT_CLI_REQUEST_SIT")
      g.eventCenter:dispatchEvent({
        name = TableBeanEvent.EVT_CLI_REQUEST_SIT,
        data = {
          seatId = seat:getSeatIdForServer(),
          buyInChips = g.vars.table.loginResponse.maxBuyIn / 2
        }
      })
      local isAutoBuy = g.storage:getBool(g.vars.storageKeys.autoBuy)
      if isAutoBuy then
        g.eventCenter:dispatchEvent({
          name = TableBeanEvent.EVT_CLI_REQUEST_AUTO_BUYIN,
          data = {
            seatId = seat:getSeatIdForServer(),
            buyInChips = g.vars.table.loginResponse.maxBuyIn / 2
          }
        })
      end
    end
  end
  if device.platform == "android" then
    self.touchLayer = display.newLayer()
    self.touchLayer:addNodeEventListener(cc.KEYPAD_EVENT, function(event)
      if self._tableUIManager ~= nil and self._tableUIManager.isRotating then
        return
      end
      if event.key == "back" then
        if not g.ui.manager.popup:removeTop() then
          local player = self._judge:getPlayerByUID(g.vars.user.uid)
          if not self._judge:getIsImPlayer() or player == nil then
            g.eventCenter:dispatchEvent({
              name = TableUIEvent.Player.QUIT_TABLE_REQUEST
            })
          elseif player:getPlayStatus() == PlayStatus.NON_PLAY or player:getPlayStatus() == PlayStatus.WAIT_FOR_GAMESTART or player:getPlayStatus() == PlayStatus.LOSE or player:getPlayStatus() == PlayStatus.WIN or player:getPlayStatus() == PlayStatus.DRAW or player:getPlayStatus() == PlayStatus.STANDAY then
            g.eventCenter:dispatchEvent({
              name = TableUIEvent.Player.QUIT_TABLE_REQUEST
            })
          else
            device.showAlert(g.lang:getContent("table.returnToLobby"), g.lang:getContent("table.returnToLobbyTip"), {
              g.lang:getContent("common.cancel"),
              g.lang:getContent("common.confirm")
            }, function(event)
              if event.buttonIndex == 2 then
                g.eventCenter:dispatchEvent({
                  name = TableUIEvent.Player.STAND_UP_REQUEST
                })
              end
            end)
          end
        end
      elseif event.key == "menu" then
      end
    end)
    self.touchLayer:setKeypadEnabled(true)
    self:addChild(self.touchLayer)
  end
end
function TableScene:onEnter()
  logger:debug("Object-Life: TableScene:onEnter()")
end
function TableScene:onExit()
  logger:debug("Object-Life: TableScene OnExit")
  self._tableUIManager:dispose()
  self._tableUIManager = nil
  self._judge:dispose()
  self._judge = nil
  self._viceJudge:dispose()
  self._viceJudge = nil
  self._tableUIEventCenter:dispose()
  self._tableUIEventCenter = nil
  g.vars.table.isPlayNow = false
end
return TableScene

local LobbyScene = class("LobbyScene", function()
  return display.newScene("LobbyScene")
end)
local LobbyConfig = require("app.beans.lobby.view.LobbyConfig")
local UpdateSubView = require("app.beans.lobby.view.UpdateSubView")
local LoginSubView = require("app.beans.lobby.view.LoginSubView")
local TableListTableBG = require("app.beans.lobby.view.TableListTableBG")
local LobbyMainSubView = require("app.beans.lobby.view.LobbyMainSubView")
local TableListView = require("app.beans.lobby.view.TableListView")
local Girl = require("app.beans.lobby.view.Girl")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local BroadcastBeanEvent = require("app.beans.broadcast.event.BroadcastBeanEvent")
local LevelUpView = require("app.beans.levelup.view.LevelUpView")
LobbyScene.UPDATE_MODE = 0
LobbyScene.LOBBY_MODE = 1
LobbyScene.TABLES_MODE = 2
LobbyScene.LOGIN_MODE = 3
LobbyScene.SHOW_MODE = LobbyScene.UPDATE_MODE
LobbyScene.BG_Z_ORDER = 1
LobbyScene.UPDATE_Z_ORDER = 2
LobbyScene.LOGIN_Z_ORDER = 3
LobbyScene.HALL_RIGHT_OP_Z_ORDER = 4
LobbyScene.GIRL_Z_ORDER = 5
LobbyScene.HALL_Z_ORDER = 6
LobbyScene.TABLELIST_Z_ORDER = 7
function LobbyScene:ctor()
  if DEBUG_MEMORY_STATUS then
    logger:debug("Object-Life:" .. self.__cname .. ": ctor")
    g.memoryManager:registerObject(self, "LobbyScene")
  end
end
function LobbyScene:onEnter()
  print("LobbyScene:onEnter")
  self:showBasicView()
  if LobbyScene.SHOW_MODE == LobbyScene.UPDATE_MODE then
    self:showUpdateView()
  elseif LobbyScene.SHOW_MODE == LobbyScene.LOGIN_MODE then
    self:showLoginSubView()
  elseif LobbyScene.SHOW_MODE == LobbyScene.LOBBY_MODE then
    self._girl:pos(display.cx + 40, display.cy - 23)
    self._girl:scale(g.vars.bgScale * 0.8)
    self:showMainLobbyView(false)
  elseif LobbyScene.SHOW_MODE == LobbyScene.TABLES_MODE then
    self._girl:pos(display.cx + 40, display.cy - 23)
    self._girl:scale(g.vars.bgScale * 0.8)
    self:showTableListView()
  end
  if device.platform == "android" then
    self.touchLayer = display.newLayer()
    self.touchLayer:addNodeEventListener(cc.KEYPAD_EVENT, function(event)
      if event.key == "back" and g.obj.sceneLoading == nil and not g.ui.manager.popup:removeTop() then
        if LobbyScene.SHOW_MODE == LobbyScene.LOBBY_MODE then
          device.showAlert(g.lang:getContent("lobby.logoutTitle"), g.lang:getContent("lobby.logoutTip"), {
            g.lang:getContent("common.cancel"),
            g.lang:getContent("lobby.logout")
          }, function(event)
            if event.buttonIndex == 2 then
              self:userLogoutActionHandler()
            end
          end)
        elseif LobbyScene.SHOW_MODE == LobbyScene.TABLES_MODE then
          self:showMainLobbyView(true)
          self._tableListView:playHideAnim()
        elseif LobbyScene.SHOW_MODE == LobbyScene.UPDATE_MODE then
          device.showAlert(g.lang:getContent("lobby.stopUpdateTitle"), g.lang:getContent("lobby.stopUpdateTip"), {
            g.lang:getContent("common.cancel"),
            g.lang:getContent("lobby.stopUpdate")
          }, function(event)
            if event.buttonIndex == 2 then
              CCDirector:sharedDirector():endToLua()
            end
          end)
        else
          device.showAlert(g.lang:getContent("lobby.quitTitle"), g.lang:getContent("lobby.quitTip"), {
            g.lang:getContent("common.cancel"),
            g.lang:getContent("lobby.quit")
          }, function(event)
            if event.buttonIndex == 2 then
              g.app:exit()
            end
          end)
        end
      end
    end)
    self.touchLayer:setKeypadEnabled(true)
    self:addChild(self.touchLayer)
  end
  g.audio:playMusic("DEFAULT", true)
  self._logoutBroadcastSuccessHandle = g.eventCenter:addEventListener(BroadcastBeanEvent.EVT_SVR_LOGOUT_SUCCESS, handler(self, self.onLogoutBroadcastSuccess))
  self._serverStopHandle = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_SERVER_STOP, handler(self, self.onServerStop))
  self._userInfoExpireHandle = g.eventCenter:addEventListener(LobbyBeanEvent.EVT_USERINFO_EXPIRE, handler(self, self.onUserInfoExpireHandler))
end
function LobbyScene:onEnterTransitionFinish()
  if g.vars.isDuplicatedLogin then
    self:performWithDelay(handler(self, self.showForceLogoutDialog), 1)
  end
  if g.vars.user.uid ~= nil then
    if g.vars.isServerUpdateLogout then
      g.vars.isServerUpdateLogout = false
      self:performWithDelay(handler(self, self.showServerUpdateDialog), 1)
    elseif g.vars.isServerUStopLogout then
      self:performWithDelay(handler(self, self.showServerStopDialog), 1)
    else
      self:performWithDelay(handler(self, self.getLevelUpInfo), 1)
    end
  end
end
function LobbyScene:getLevelUpInfo()
  g.levelInfo:getLevelUpInfo(handler(self, function(self, data)
    if data and data.lv >= 3 then
      LevelUpView.new(data.lv, LobbyScene.SHOW_MODE):show()
    end
  end))
end
function LobbyScene:showForceLogoutDialog()
  g.ui.Dialog.new({
    contentText = g.lang:getContent("common.loginDublicated"),
    confirmButtonText = g.lang:getContent("lobby.logout"),
    dialogType = g.ui.DialogType.ALERT,
    callback = handler(self, function(self, sender)
      self:userLogoutActionHandler()
    end)
  }):show(true, true, false, true)
end
function LobbyScene:onUserInfoExpireHandler()
  g.ui.Dialog.new({
    contentText = g.lang:getContent("table.loginTableFailWithExpire"),
    confirmButtonText = g.lang:getContent("lobby.logout"),
    dialogType = g.ui.DialogType.ALERT,
    callback = handler(self, function(self, sender)
      self:userLogoutActionHandler()
    end)
  }):show(true, true, false, true)
end
function LobbyScene:showServerUpdateDialog()
  g.ui.Dialog.new({
    contentText = g.lang:getContent("lobby.serverUpdate"),
    confirmButtonText = g.lang:getContent("lobby.logout"),
    dialogType = g.ui.DialogType.ALERT,
    hasCloseButton = true,
    callback = handler(self, function(self, sender)
      if sender == g.ui.DialogConst.CONFIRM_BUTTON_CLICK then
        g.vars.table.isPlayNow = true
        g.tablesInfo:getPlayTableWithTid(nil, nil, nil)
      end
    end)
  }):show(true, true, false, true)
end
function LobbyScene:showServerStopDialog()
  g.ui.Dialog.new({
    contentText = g.lang:getContent("lobby.serverStop"),
    confirmButtonText = g.lang:getContent("lobby.logout"),
    dialogType = g.ui.DialogType.ALERT,
    callback = handler(self, function(self, sender)
      self:userLogoutActionHandler()
    end)
  }):show(true, true, false, true)
end
function LobbyScene:onLogoutBroadcastSuccess(evt)
  local packet = evt.data
  if packet.errorCode == 3 then
    self:performWithDelay(handler(self, self.showForceLogoutDialog), 1)
  end
end
function LobbyScene:onServerStop(evt)
  self:showServerStopDialog()
end
function LobbyScene:onExit()
  g.audio:stopMusic(false)
  if self._logoutBroadcastSuccessHandle ~= nil then
    g.eventCenter:removeEventListener(self._logoutBroadcastSuccessHandle)
    self._logoutBroadcastSuccessHandle = nil
  end
  if self._serverStopHandle ~= nil then
    g.eventCenter:removeEventListener(self._serverStopHandle)
    self._serverStopHandle = nil
  end
  if self._userInfoExpireHandle then
    g.eventCenter:removeEventListener(self._userInfoExpireHandle)
    self._userInfoExpireHandle = nil
  end
end
function LobbyScene:showBasicView()
  display.newSprite("pd_mainlobby_background.jpg"):scale(g.vars.bgScale):pos(display.cx, display.cy):addTo(self, LobbyScene.BG_Z_ORDER)
  self._girl = Girl.new():pos(display.cx / 2, display.cy - 23):addTo(self, LobbyScene.GIRL_Z_ORDER):scale(g.vars.bgScale)
  self._rightMainOperView = display.newNode():addTo(self, LobbyScene.HALL_RIGHT_OP_Z_ORDER):pos(display.cx, display.cy)
end
function LobbyScene:showUpdateView()
  loadTexture("pd_login_texture.plist", "pd_login_texture.png")
  self._updateSubView = UpdateSubView.new():pos(display.cx, display.cy):addTo(self, LobbyScene.UPDATE_Z_ORDER)
  self._updateSubView:startUpdateService(handler(self, self.enterAppDirectoryBlock), handler(self, self.restartAppBlock))
  g.vars.currentViewName = LobbyConfig.UPDATE
end
function LobbyScene:enterAppDirectoryBlock()
  local function completeCallback()
    if self._updateSubView ~= nil then
      self._updateSubView:removeFromParent()
      self._updateSubView = nil
    end
  end
  self:showLoginSubView(completeCallback, 1)
end
function LobbyScene:restartAppBlock()
  self._updateSubView:removeFromParent()
  package.loaded.appentry = nil
  require("appentry")
end
function LobbyScene:showLoginSubView(completeCallback, animationMode)
  self:_loadImageResource()
  loadTexture("pd_login_texture.plist", "pd_login_texture.png")
  self._girl:moveTo(LobbyConfig.SHOW_LOGIN_ANIMATION_TIME, display.cx / 2, display.cy - 23)
  local showMainLobbyBlock = handler(self, self.showMainLobbyView)
  self._loginSubView = LoginSubView.new(showMainLobbyBlock):pos(display.cx, display.cy):addTo(self, LobbyScene.LOGIN_Z_ORDER)
  self._loginSubView:playShowLoginSubViewAnimation(completeCallback, animationMode)
  LobbyScene.SHOW_MODE = LobbyScene.LOGIN_MODE
end
function LobbyScene:showMainLobbyView(isFromRoomList)
  LobbyScene.SHOW_MODE = LobbyScene.LOBBY_MODE
  local showTableListViewBlock = handler(self, self.showTableListView)
  local showLoginViewBlock = handler(self, self.showLoginSubView)
  local logoutHandler = handler(self, self.userLogoutActionHandler)
  self._girl:scaleTo(LobbyConfig.ANIMATION_TIME, g.vars.bgScale):moveTo(LobbyConfig.ANIMATION_TIME, display.cx + 40, display.cy - 23)
  self:_createTableIfNotExist()
  self._pokerTable:stopAllActions()
  local animTime = 0
  local baseDelayTime = 0
  self.tableListTableBG:setScaleX(1.5)
  if isFromRoomList then
    animTime = LobbyConfig.HIDE_TABLELIST_ANIMATION_TIME
    baseDelayTime = animTime
    self._pokerTable:pos(display.cx, display.height)
    self.tableListTableBG:disabledHighLight()
    transition.execute(self._pokerTable, CCMoveTo:create(animTime, CCPoint(display.cx, self.tablebarSpriteHeight)), {
      onComplete = function()
        self.tableListTableBG:enabledHighLight()
      end
    })
  else
    animTime = LobbyConfig.ANIMATION_TIME
    baseDelayTime = animTime
    self._pokerTable:pos(display.cx, -200)
    self.tableListTableBG:disabledHighLight()
    transition.execute(self._pokerTable, CCMoveTo:create(animTime, CCPoint(display.cx, self.tablebarSpriteHeight)), {
      onComplete = function()
        self.tableListTableBG:enabledHighLight()
      end
    })
  end
  self._lobbyMainSubView = LobbyMainSubView.new(showTableListViewBlock, showLoginViewBlock, logoutHandler, isFromRoomList, self._rightMainOperView):pos(display.cx, display.cy):addTo(self, LobbyScene.HALL_Z_ORDER)
  self._lobbyMainSubView:playShowAnim(animTime, baseDelayTime)
  function self._lobbyMainSubView.showFreeChipCallback()
    local sequence = transition.sequence({
      CCMoveTo:create(0.3, ccp(0, -140)),
      CCMoveTo:create(0.3, ccp(0, 0))
    })
    transition.execute(self._tableNode, sequence)
  end
  function self._lobbyMainSubView.hideFreeChipCallback()
    local sequence = transition.sequence({
      CCMoveTo:create(0.3, ccp(0, -140)),
      CCMoveTo:create(0.3, ccp(0, 0))
    })
    self._tableNode:stopAllActions()
    transition.execute(self._tableNode, sequence)
  end
end
function LobbyScene:showTableListView(roomType)
  if LobbyScene.SHOW_MODE == LobbyScene.UPDATE_MODE or LobbyScene.SHOW_MODE == LobbyScene.LOGIN_MODE then
    return
  end
  LobbyScene.SHOW_MODE = LobbyScene.TABLES_MODE
  if self._lobbyMainSubView then
    self._lobbyMainSubView:playHideLobbyMainSubViewAnimation()
    self._lobbyMainSubView = nil
  end
  local showMainLobbyBlock = handler(self, self.hideRoomListView)
  self._tableListView = TableListView.new(showMainLobbyBlock):pos(display.cx, display.cy):addTo(self, LobbyScene.TABLELIST_Z_ORDER)
  self._girl:scaleTo(LobbyConfig.ANIMATION_TIME, g.vars.bgScale * 0.8)
  self._tableListView:playShowAnim()
  self:_createTableIfNotExist()
  local animTime = LobbyConfig.SHOWUP_TABLELIST_ANIMATION_TIME
  self.tableListTableBG:disabledHighLight()
  transition.moveTo(self._pokerTable, {
    time = animTime,
    y = display.height,
    easing = "BACKOUT"
  })
  self.tableListTableBG:setScaleX(1)
end
function LobbyScene:hideRoomListView()
  self:showMainLobbyView(true)
end
function LobbyScene:userLogoutActionHandler()
  g.ui.manager.popup:removeAllPopup()
  if LobbyScene.SHOW_MODE == LobbyScene.TABLES_MODE and self._tableListView and self._tableListView:getParent() then
    self._tableListView:removeFromParent()
  end
  if self._girl then
    self._girl:scale(g.vars.bgScale):pos(display.cx + 40, display.cy - 23)
  end
  self:showLoginSubView()
  if self._lobbyMainSubView then
    self._lobbyMainSubView:playHideLobbyMainSubViewAnimation()
    self._lobbyMainSubView = nil
  end
  print("hide _pokerTable")
  local animTime = LobbyConfig.HIDE_LOBBY_ANIMATION_TIME
  self.tableListTableBG:disabledHighLight()
  transition.execute(self._pokerTable, CCMoveTo:create(animTime, CCPoint(0, -display.cy)), {
    onComplete = function()
      self.tableListTableBG:enabledHighLight()
    end
  })
  g.storage:setString(g.vars.storageKeys.loginType, "")
  g.vars = require("app.beans.data.GlobalVars").new()
  g.eventCenter:dispatchEvent({
    name = BroadcastBeanEvent.EVT_CLI_LOGOUT,
    data = {
      uid = g.vars.user.uid,
      loginKey = g.vars.user.loginKey
    }
  })
end
function LobbyScene:_loadImageResource(completeCallback)
  loadTexture("pd_mainlobby_texture.plist", "pd_mainlobby_texture.png")
  loadTexture("pd_common_texture.plist", "pd_common_texture.png")
end
function LobbyScene:_createTableIfNotExist()
  if self._tableNode == nil then
    self._tableNode = display.newNode():addTo(self, LobbyScene.HALL_Z_ORDER)
    local bgScale = g.vars.bgScale
    self._pokerTable = display.newNode():addTo(self._tableNode):scale(bgScale)
    self._pokerTable:pos(display.cx, display.cy)
    self.tablebarSpriteHeight = 102
    self.tableListTableBG = TableListTableBG.new():addTo(self._pokerTable)
    self.tableSpriteHeight = self.tableListTableBG:getTableHeight()
    self.tableListTableBG:pos(0, -self.tableSpriteHeight * 0.5)
  end
end
return LobbyScene

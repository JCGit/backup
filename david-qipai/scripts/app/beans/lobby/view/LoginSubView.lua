local LobbyConfig = import(".LobbyConfig")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
local LoginTable = import(".LoginTable")
local LoginClickView = import(".LoginClickView")
local LogoView = import(".LogoView")
local LoginSubView = class("LoginSubView", function()
  return display.newNode()
end)
function LoginSubView:ctor(showMainLobbyBlock)
  self.showMainLobbyBlock = showMainLobbyBlock
  self:setNodeEventEnabled(true)
  self._flyPokerBatchNode = display.newNode():pos(-display.cx - 300, 0):addTo(self)
  self.dotPoint1 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode):scale(0.5)
  self.dotPoint2 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode)
  self.dotPoint3 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode)
  self.dotPoint4 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode)
  self.dotPoint5 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode):scale(0.4)
  self.dotPoint6 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode):scale(0.5)
  self.dotPoint7 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode):scale(0.5)
  self.dotPoint8 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode):scale(0.5)
  self.dotPoint9 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode):scale(0.5)
  self.dotPoint10 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode):scale(0.5)
  self.dotPoint11 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._flyPokerBatchNode):scale(0.5)
  self:_onEnterFrame()
  self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self._onEnterFrame))
  self:scheduleUpdate()
  self._tableNode = display.newNode():addTo(self)
  self._loginTable = LoginTable.new():addTo(self._tableNode, 0)
  local tableWidth = self._loginTable:getTableWidth()
  if tableWidth < display.width * 0.75 then
    self._visibleTableWidth = tableWidth
  else
    self._visibleTableWidth = display.width * 0.75
  end
  self._loginTable:setVisibleWidth(self._visibleTableWidth)
  self._loginTable:pos(display.width * 0.5 + tableWidth * 0.5 - self._visibleTableWidth, 0)
  self._logoBatchNode = LogoView.new():pos(LobbyConfig.LOGO_POS_X, LobbyConfig.LOGO_NORMAL_POS_Y):addTo(self._tableNode)
  local fbHandler = clickHandler(self, self._onFacebookLoginClick)
  local deviceHandler = clickHandler(self, self._onGuestLoginClick)
  self.loginClickView = LoginClickView.new(fbHandler, deviceHandler):addTo(self._tableNode):pos(LoginClickView.POS_X, LoginClickView.POS_Y)
  if launchInfo.ret == -12 then
    loadTexture("pd_common_texture.plist", "pd_common_texture.png")
    g.ui.Dialog.new({
      contentText = g.lang:getContent("common.serverStop"),
      confirmButtonText = g.lang:getContent("common.confirm"),
      dialogType = g.ui.DialogType.ALERT,
      callback = handler(self, function(self, sender)
      end)
    }):show(true, true, false, true)
  end
  self._copyrightLabel = ui.newTTFLabel({
    text = g.lang:getContent("lobby.copyright"),
    color = ccc3(0, 109, 50),
    size = 18,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.RIGHT_BOTTOM, display.cx - 4, -(display.cy - 4)):addTo(self)
  if DEBUG_MEMORY_STATUS then
    self.guest1TestButton = self:createTestButton(1)
    self.guest1TestButton:setTouchSwallowEnabled(false)
    self.guest2TestButton = self:createTestButton(2)
    self.guest2TestButton:setTouchSwallowEnabled(false)
    self.guest3TestButton = self:createTestButton(3)
    self.guest3TestButton:setTouchSwallowEnabled(false)
    self.guest4TestButton = self:createTestButton(4)
    self.guest4TestButton:setTouchSwallowEnabled(false)
  end
end
function LoginSubView:playShowLoginSubViewAnimation(completeCallback, animationMode)
  local animTime = LobbyConfig.SHOW_LOGIN_ANIMATION_TIME
  if animationMode ~= nil and animationMode == 1 then
    local function animationCompleteCallback()
      completeCallback()
      if launchInfo.ret ~= -12 then
        local loginType = g.storage:getString(g.vars.storageKeys.loginType, "")
        if loginType == "device" then
          self:_onGuestLoginClick()
        elseif loginType == "facebook" then
          self:_onFacebookLoginClick()
        end
      end
    end
    transition.moveTo(self._flyPokerBatchNode, {
      time = animTime,
      x = -display.cx
    })
    self._logoBatchNode:pos(LobbyConfig.LOGO_POS_X, LobbyConfig.LOGO_LOADING_POS_Y)
    transition.moveTo(self._logoBatchNode, {
      time = animTime,
      y = LobbyConfig.LOGO_NORMAL_POS_Y,
      onComplete = animationCompleteCallback
    })
    self.loginClickView:pos(display.cx + 3 * LoginClickView.POS_X, LoginClickView.POS_Y)
    transition.moveTo(self.loginClickView, {
      x = LoginClickView.POS_X,
      time = animTime
    })
  else
    transition.fadeIn(self._copyrightLabel, {time = animTime})
    transition.moveTo(self._flyPokerBatchNode, {
      time = animTime,
      x = -display.cx
    })
    self._tableNode:setPositionX(display.cx * 2)
    transition.moveTo(self._tableNode, {
      time = animTime,
      x = 0,
      onComplete = completeCallback
    })
  end
end
function LoginSubView:_onFacebookLoginClick()
  self:playLoginRequestAnimation()
  if g.vars.user.uid == nil then
    g.facebook:login(function(isSuccess, data)
      if isSuccess then
        self:_facebookLoginWithAccessToken(data)
      else
        g.ui.manager.tip:top(g.lang:getContent("common.fbLoginFail"))
        self:playLoginFailAnimation()
      end
    end)
  else
    g.eventCenter:dispatchEvent({
      name = LobbyBeanEvent.EVT_LOGIN_SUCCESS,
      data = {
        ret = 0,
        info = g.vars.user
      }
    })
  end
end
function LoginSubView:_facebookLoginWithAccessToken(accessToken)
  function successCallback(jsonData)
    self:_playLoginSuccessHideAnimation()
    g.eventCenter:dispatchEvent({
      name = LobbyBeanEvent.EVT_LOGIN_SUCCESS,
      data = jsonData
    })
    g.facebook:updateAppRequest()
  end
  function loginFail(data)
    if data and data.ret == -12 then
      self:_showServerStopDialog()
    else
      g.ui.manager.tip:top(g.lang:getContent("common.requestDataFail"))
    end
    self:playLoginFailAnimation()
  end
  g.loginInfo:requestLogin({loginType = "facebook", accessToken = accessToken}, successCallback, loginFail)
end
function LoginSubView:_showServerStopDialog()
  g.ui.Dialog.new({
    contentText = g.lang:getContent("common.serverStop"),
    confirmButtonText = g.lang:getContent("common.confirm"),
    dialogType = g.ui.DialogType.ALERT,
    callback = handler(self, function(self, sender)
    end)
  }):show(true, true, false, true)
end
function LoginSubView:_onGuestLoginClick()
  self:playLoginRequestAnimation(2)
  self:requestLogin()
end
function LoginSubView:playLoginRequestAnimation(loginType)
  self._loginTable:showLoginLoading()
  local animTime = LobbyConfig.ANIMATION_TIME
  self._logoBatchNode:stopAllActions()
  self.loginClickView:stopAllActions()
  transition.moveTo(self._logoBatchNode, {
    time = animTime,
    y = -LobbyConfig.LOGO_LOADING_POS_Y
  })
  transition.moveTo(self.loginClickView, {
    x = display.cx + 3 * LoginClickView.POS_X,
    time = animTime
  })
end
function LoginSubView:playLoginFailAnimation()
  self._loginTable:hideLoginLoading()
  local animTime = LobbyConfig.ANIMATION_TIME
  self._logoBatchNode:stopAllActions()
  self.loginClickView:stopAllActions()
  transition.moveTo(self._logoBatchNode, {
    time = animTime,
    y = LobbyConfig.LOGO_NORMAL_POS_Y
  })
  transition.moveTo(self.loginClickView, {
    x = LoginClickView.POS_X,
    time = animTime
  })
end
function LoginSubView:_playLoginSuccessHideAnimation()
  function hideAnimationComplte()
    self.showMainLobbyBlock()
    self:removeFromParent()
  end
  function flyCompleteCallback()
    local animTime = LobbyConfig.HIDE_LOGIN_ANIMATION_TIME
    transition.fadeOut(self._copyrightLabel, {time = animTime})
    transition.moveTo(self._flyPokerBatchNode, {
      time = animTime,
      x = -display.cx - 300
    })
    transition.moveTo(self._tableNode, {
      time = animTime,
      x = self._visibleTableWidth,
      onComplete = hideAnimationComplte
    })
  end
  self._loginTable:chipsFlyAwayAnimation(flyCompleteCallback)
end
function LoginSubView:onCleanup()
  if self._dotsSchedulerHandle then
    scheduler.unscheduleGlobal(self._dotsSchedulerHandle)
    self._dotsSchedulerHandle = nil
  end
end
function LoginSubView:requestLogin()
  print("GuestLoginTestClick")
  function successCallback(jsonData)
    self:_playLoginSuccessHideAnimation()
    g.eventCenter:dispatchEvent({
      name = LobbyBeanEvent.EVT_LOGIN_SUCCESS,
      data = jsonData
    })
  end
  function loginFail(data)
    if data and data.ret == -12 then
      self:_showServerStopDialog()
    else
      g.ui.manager.tip:top(g.lang:getContent("common.requestDataFail"))
    end
    self:playLoginFailAnimation()
  end
  g.loginInfo:requestLogin({loginType = "device"}, successCallback, loginFail)
end
function LoginSubView:createTestButton(index)
  local buttonWidth = 100
  local buttonHeight = 50
  return cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):addTo(self._tableNode):setButtonSize(buttonWidth, buttonHeight):pos(buttonWidth * (index - 2) * 1.5, buttonHeight * 4):onButtonClicked(handler(self, self._onGuestLoginTestClick)):setButtonLabel("normal", ui.newTTFLabel({
    text = "Device" .. tostring(index * 4),
    size = 26,
    color = ccc3(214, 255, 239),
    align = ui.TEXT_ALIGN_CENTER
  }))
end
function LoginSubView:_onGuestLoginTestClick(event)
  local button = event.target
  local lableStr = button:getButtonLabel():getString()
  g.loginTempDevice = lableStr
  self:_onGuestLoginClick()
end
function LoginSubView:_onEnterFrame()
  if self.nextY1 == nil or self.nextY1 == 0 then
    self.nextX1 = 0
    self.nextV1 = 1
    self.nextY1 = 0
    self.nextScale1 = 1
    self.nextOpacity1 = 255
  end
  self.nextX1, self.nextV1 = self:calculate(self.nextX1, self.nextV1, 0.02)
  self.nextY1 = self.nextY1 + 0.6
  if self.nextY1 >= 840 then
    self.nextY1 = 0
  end
  self.dotPoint1:setPositionX(self.nextX1 + 60)
  self.dotPoint1:setPositionY(self.nextY1 - 180)
  if self.nextY2 == nil or self.nextY2 == 0 then
    self.nextX2 = 0
    self.nextV2 = 1.6
    self.nextY2 = 0
    self.nextScale2 = 1
    self.nextOpacity2 = 255
  end
  self.nextX2, self.nextV2 = self:calculate(self.nextX2, self.nextV2, 0.04)
  self.nextY2 = self.nextY2 + 0.8
  if 840 <= self.nextY2 then
    self.nextY2 = 0
  elseif self.nextY2 > 200 then
    if self.nextY2 < 480 then
      self.nextScale2 = self.nextScale2 + 2.0E-4
    else
      self.nextScale2 = self.nextScale2 - 0.0016
      self.nextOpacity2 = self.nextOpacity2 - 0.1
    end
  else
    self.nextScale2 = self.nextScale2 - 0.001
  end
  self.dotPoint2:scale(self.nextScale2)
  self.dotPoint2:setPositionX(self.nextX2 + 60)
  self.dotPoint2:setPositionY(self.nextY2 - 480)
  self.dotPoint2:setOpacity(self.nextOpacity2)
  if self.nextY3 == nil or self.nextY3 == 0 then
    self.nextX3 = 0
    self.nextV3 = 2
    self.nextY3 = 0
    self.nextScale3 = 1
    self.nextOpacity3 = 255
  end
  self.nextX3, self.nextV3 = self:calculate(self.nextX3, self.nextV3, 0.05)
  self.nextY3 = self.nextY3 + 0.8
  if 480 <= self.nextY3 then
    self.nextY3 = 0
  elseif self.nextY3 > 160 then
    if 200 > self.nextY3 then
      self.nextScale3 = self.nextScale3 + 0.005
    else
      self.nextScale3 = self.nextScale3 - 0.002
      self.nextOpacity3 = self.nextOpacity3 - 0.2
    end
  else
    self.nextScale3 = self.nextScale3 - 0.001
  end
  self.dotPoint3:scale(self.nextScale3)
  self.dotPoint3:setPositionX(self.nextX3 + 60)
  self.dotPoint3:setPositionY(self.nextY3 - 200)
  self.dotPoint3:setOpacity(self.nextOpacity3)
  if self.nextY4 == nil or self.nextY4 == 0 then
    self.nextX4 = 0
    self.nextV4 = 1.5
    self.nextY4 = 0
    self.nextScale4 = 1
    self.nextOpacity4 = 255
  end
  self.nextX4, self.nextV4 = self:calculate(self.nextX4, self.nextV4, 0.05)
  self.nextY4 = self.nextY4 + 0.7
  if self.nextY4 >= 440 then
    self.nextY4 = 0
  elseif 60 < self.nextY4 then
    if 200 > self.nextY4 then
      self.nextScale4 = self.nextScale4 + 0.001
    else
      self.nextScale4 = self.nextScale4 - 5.0E-4
      self.nextOpacity4 = self.nextOpacity4 - 0.15
    end
  else
    self.nextScale4 = self.nextScale4 - 0.004
  end
  self.dotPoint4:scale(self.nextScale4)
  self.dotPoint4:setPositionX(self.nextX4 + 60)
  self.dotPoint4:setPositionY(self.nextY4 - 200)
  self.dotPoint4:setOpacity(self.nextOpacity4)
  if self.nextY5 == nil or self.nextY5 == 0 then
    self.nextX5 = 0
    self.nextV5 = 1.5
    self.nextY5 = 0
    self.nextScale5 = 1
    self.nextOpacity5 = 255
  end
  self.nextX5, self.nextV5 = self:calculate(self.nextX5, self.nextV5, 0.05)
  self.nextY5 = self.nextY5 + 0.7
  if self.nextY5 >= 240 then
    self.nextY5 = 0
  elseif self.nextY5 > 80 then
    if 160 > self.nextY5 then
      self.nextScale5 = self.nextScale5 + 5.0E-4
    else
      self.nextScale5 = self.nextScale5 - 0.001
      self.nextOpacity5 = self.nextOpacity5 - 0.7
    end
  else
    self.nextScale5 = self.nextScale5 - 0.015
  end
  self.dotPoint5:scale(self.nextScale5)
  self.dotPoint5:setPositionX(self.nextX5 + 260)
  self.dotPoint5:setPositionY(self.nextY5 + 100)
  self.dotPoint5:setOpacity(self.nextOpacity5)
  if self.nextY6 == nil or self.nextY6 == 0 then
    self.nextX6 = 0
    self.nextV6 = 0.8
    self.nextY6 = 0
    self.nextScale6 = 1
    self.nextOpacity6 = 255
  end
  self.nextY6 = self.nextY6 + 0.8
  if self.nextY6 >= 600 then
    self.nextY6 = 0
    self.nextOpacity6 = 255
  elseif self.nextY6 > 300 then
    if self.nextY6 > 500 then
      self.nextScale6 = self.nextScale6 - 0.002
      self.nextOpacity6 = self.nextOpacity6 - 1
    else
      self.nextScale6 = self.nextScale6 + 0.002
    end
  else
    self.nextScale6 = self.nextScale6 - 0.0015
  end
  self.dotPoint6:scale(self.nextScale6)
  self.dotPoint6:setPositionX(self.nextX6 + 140)
  self.dotPoint6:setPositionY(self.nextY6 - 360)
  self.dotPoint6:setOpacity(self.nextOpacity6)
  if self.nextY7 == nil or self.nextY7 == 0 then
    self.nextX7 = 0
    self.nextV7 = 0.8
    self.nextY7 = 0
    self.nextScale7 = 1
    self.nextOpacity7 = 255
  end
  self.nextY7 = self.nextY7 + 0.8
  if 300 <= self.nextY7 then
    self.nextY7 = 0
    self.nextOpacity7 = 255
  elseif 100 < self.nextY7 then
    if 200 < self.nextY7 then
      self.nextScale7 = self.nextScale7 - 4.0E-4
      self.nextOpacity7 = self.nextOpacity7 - 1
    else
      self.nextScale7 = self.nextScale7 + 4.0E-4
    end
  else
    self.nextScale7 = self.nextScale7 - 0.0015
  end
  self.dotPoint7:scale(self.nextScale7)
  self.dotPoint7:setPositionX(self.nextX7 + 240)
  self.dotPoint7:setPositionY(self.nextY7)
  self.dotPoint7:setOpacity(self.nextOpacity7)
  if self.nextY8 == nil or self.nextY8 == 0 then
    self.nextX8 = 0
    self.nextV8 = 0.8
    self.nextY8 = 0
    self.nextScale8 = 1
    self.nextOpacity8 = 255
  end
  self.nextY8 = self.nextY8 + 0.6
  if 300 <= self.nextY8 then
    self.nextY8 = 0
    self.nextOpacity8 = 255
  elseif self.nextY8 > 50 then
    if 160 < self.nextY8 then
      self.nextScale8 = self.nextScale8 - 2.0E-4
      self.nextOpacity8 = self.nextOpacity8 - 1
    else
      self.nextScale8 = self.nextScale8 + 2.0E-4
    end
  else
    self.nextScale8 = self.nextScale8 - 0.006
  end
  self.dotPoint8:scale(self.nextScale8)
  self.dotPoint8:setPositionX(self.nextX8 + 320)
  self.dotPoint8:setPositionY(self.nextY8 + 40)
  self.dotPoint8:setOpacity(self.nextOpacity8)
  if self.nextY9 == nil or 600 <= self.nextY9 then
    self.nextY9 = 0
    self.nextX9 = 0
  end
  self.nextX9 = self.nextX9 - 1
  self.nextY9 = self.nextY9 + 1
  self.dotPoint9:setPositionX(self.nextX9 + 320)
  self.dotPoint9:setPositionY(self.nextY9 + 40)
  if self.nextY10 == nil or 200 <= self.nextY10 then
    self.nextY10 = 0
    self.nextX10 = 0
  end
  self.nextX10 = self.nextX10 - 0.5
  self.nextY10 = self.nextY10 + 1
  self.dotPoint10:setPositionX(self.nextX10 + 320)
  self.dotPoint10:setPositionY(self.nextY10 + 180)
  if self.nextY11 == nil or 300 <= self.nextY11 then
    self.nextY11 = 0
    self.nextX11 = 0
  end
  self.nextX11 = self.nextX11 - 1.2
  self.nextY11 = self.nextY11 + 1
  self.dotPoint11:setPositionX(self.nextX11 + 300)
  self.dotPoint11:setPositionY(self.nextY11 + 120)
end
function LoginSubView:calculate(x, v, k)
  local t = -1
  if x < 0 then
    t = 1
  elseif x == 0 then
    if v > 0 then
      t = -1
    else
      t = 1
    end
  end
  local nextV = v + k * t
  local nextX = x + (nextV + v) * 1 * 0.5
  return nextX, nextV
end
return LoginSubView

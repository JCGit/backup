local LoginClickView = class("LoginClickView", function()
  return display.newNode()
end)
LoginClickView.POS_X = display.cx - 380 * g.scaleWidth * 0.5 - 84
LoginClickView.POS_Y = 176 - 264 * g.scaleHeight
local LOGIN_BUTTON_WIDTH = 380 * g.scaleWidth
local LOGIN_BUTTON_HEIGHT = 96
local LOGIN_BUTTON_GAP = 34
local PANEL_WIDTH = LOGIN_BUTTON_WIDTH + LOGIN_BUTTON_GAP * 2
local PANEL_HEIGHT = LOGIN_BUTTON_HEIGHT * 2 + LOGIN_BUTTON_GAP * 3
function LoginClickView:ctor(fbHandler, deviceHandler)
  self._buttonNode = display.newNode():addTo(self)
  local btnCenterPosY = LOGIN_BUTTON_HEIGHT * 0.5 + LOGIN_BUTTON_GAP * 0.5
  self._facebookBonusBackground = display.newSprite("#login_fb_reward_icon.png"):pos(LOGIN_BUTTON_WIDTH * 0.5 - 50, 28):hide()
  self.fbBtn = cc.ui.UIPushButton.new({
    normal = "#login_fb_icon_normal.png",
    pressed = "#login_fb_icon_pressed.png"
  }, {scale9 = true}):setButtonSize(LOGIN_BUTTON_WIDTH, LOGIN_BUTTON_HEIGHT):pos(0, LOGIN_BUTTON_HEIGHT * 0.5 + LOGIN_BUTTON_GAP * 0.5 + 1):addTo(self._buttonNode):add(self._facebookBonusBackground)
  if fbHandler then
    self.fbBtn:onButtonClicked(fbHandler)
  end
  ui.newTTFLabel({
    text = g.lang:getContent("lobby.facebookLogin"),
    color = g.font.color.LIGHT,
    size = 32,
    align = ui.TEXT_ALIGN_LEFT
  }):pos(24, btnCenterPosY):addTo(self._buttonNode)
  display.newSprite("#login_fb_icon.png"):pos(0 - LOGIN_BUTTON_WIDTH * 0.5 + 60, btnCenterPosY):addTo(self._buttonNode)
  local btnCenterPosY = -LOGIN_BUTTON_HEIGHT * 0.5 - LOGIN_BUTTON_GAP * 0.5
  self.guestBtn = cc.ui.UIPushButton.new({
    normal = "#login_guest_icon_normal.png",
    pressed = "#login_guest_icon_pressed.png"
  }, {scale9 = true}):setButtonSize(LOGIN_BUTTON_WIDTH, LOGIN_BUTTON_HEIGHT):pos(0, -LOGIN_BUTTON_HEIGHT * 0.5 - LOGIN_BUTTON_GAP * 0.5 + 1):addTo(self._buttonNode)
  if deviceHandler then
    self.guestBtn:onButtonClicked(deviceHandler)
  end
  ui.newTTFLabel({
    text = g.lang:getContent("lobby.deviceLogin"),
    color = g.font.color.LIGHT,
    size = 32,
    align = ui.TEXT_ALIGN_LEFT
  }):pos(24, btnCenterPosY):addTo(self._buttonNode)
  display.newSprite("#login_guest_icon.png"):pos(0 - LOGIN_BUTTON_WIDTH * 0.5 + 60, btnCenterPosY):addTo(self._buttonNode)
end
return LoginClickView

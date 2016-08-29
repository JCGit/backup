local PanelConst = import(".PanelConst")
local Panel = class("Panel", function()
  return display.newNode()
end)
function Panel:ctor(width, height, params)
  self._width = width
  self._height = height
  local bgTexture = "#common_standard_bluebg_02.png"
  if params and params.background then
    bgTexture = params.background
  end
  self._background = display.newScale9Sprite(bgTexture, 0, 0, CCSize(self._width, self._height)):addTo(self)
  self._background:setTouchEnabled(true)
  self._background:setTouchSwallowEnabled(true)
  self._backgroundHightLight = display.newSprite("#common_popup_bg_highlight.png")
  self._backgroundHightLight:align(display.TOP_CENTER, 0, self._height * 0.5 - 5)
  self._backgroundHightLight:addTo(self):hide()
  self._backgroundHightLight:scale(4)
end
function Panel:enableBGHightLight()
  self._backgroundHightLight:show()
end
function Panel:setBGHightLightOpacity(opacity)
  self._backgroundHightLight:opacity(opacity)
end
function Panel:addCloseButton(normalBtnSp, pressedBtnSp)
  if not self._closeButton then
    self._closeTransparentButton = cc.ui.UIPushButton.new({
      normal = "#transparent.png",
      pressed = "#transparent.png"
    }, {scale9 = true}):addTo(self):onButtonClicked(clickHandler(self, self.onClose))
    self._closeTransparentButton:setButtonSize(90, 90)
    self._closeTransparentButton:pos(self._width * 0.5, self._height * 0.5)
    normalBtnSp = normalBtnSp or "#common_black_close_btn_normal.png"
    pressedBtnSp = pressedBtnSp or "#common_black_close_btn_pressed.png"
    self._closeButton = cc.ui.UIPushButton.new({normal = normalBtnSp, pressed = pressedBtnSp}):pos(self._width * 0.5, self._height * 0.5):onButtonClicked(handler(self, self.onClose)):addTo(self)
  end
end
function Panel:removeCloseButton()
  if not self._closeButton then
    self._closeButton:removeFromParent()
    self._closeTransparentButton:removeFromParent()
  end
end
function Panel:show(isCover, isCenter, closeWhenClickCover, useShowAnimation)
  g.ui.manager.popup:addPopup(self, isCover ~= false, isCenter ~= false, closeWhenClickCover ~= false, useShowAnimation ~= false)
  return self
end
function Panel:hide()
  g.ui.manager.popup:removePopup(self)
  return self
end
function Panel:onClose()
  g.audio:playSound("POPUP_CLOSE")
  self:hide()
end
function Panel:_showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():addTo(self)
  end
end
function Panel:_hideMiniLoading()
  if self._miniLoading then
    self._miniLoading:removeFromParent()
    self._miniLoading = nil
  end
end
function Panel:getWidth()
  return self._width
end
function Panel:getHeight()
  return self._height
end
return Panel

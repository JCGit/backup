local ControlButton = class("ControlButton", function()
  return display.newNode()
end)
ControlButton.TYPE_NORMAL = "TYPE_NORMAL"
ControlButton.TYPE_CHECK = "TYPE_CHECK"
ControlButton.BUTTON_WIDTH = 180
ControlButton.BUTTON_HEIGHT = 74
ControlButton.QUICK_BUTTON_WIDTH = 136
function ControlButton:ctor(buttonType)
  self._checked = false
  self._pressed = false
  self._enabled = true
  self._buttonType = buttonType
  self._changeCallback = nil
  self._changeCallbackTarget = nil
  self._touchCallback = nil
  self._touchCallbackTarget = nil
  local buttonWidth = ControlButton.BUTTON_WIDTH
  local buttonHeight = ControlButton.BUTTON_HEIGHT
  self._statusImages = {}
  self._statusImages.NORMAL_UP = display.newScale9Sprite("#table_opr_check_up.png"):size(buttonWidth, buttonHeight):addTo(self)
  self._statusImages.NORMAL_DOWN = display.newScale9Sprite("#table_opr_check_down.png"):size(buttonWidth, buttonHeight):addTo(self)
  self._statusImages.CHECK_UP = display.newScale9Sprite("#table_opr_check_up.png"):size(buttonWidth, buttonHeight):addTo(self)
  self._statusImages.CHECK_DOWN = display.newScale9Sprite("#table_opr_check_down.png"):size(buttonWidth, buttonHeight):addTo(self)
  self._statusImages.CHECK_SELECTED = display.newScale9Sprite("#table_opr_check_selected.png"):size(buttonWidth, buttonHeight):addTo(self)
  self._checkBg = display.newScale9Sprite("#pd_common_check_bg.png", buttonWidth * -0.5 + 38, 0, cc.size(54, 54)):addTo(self)
  self._checkIcon = display.newSprite("#pd_common_check_big_icon.png"):pos(buttonWidth * -0.5 + 38, 0):addTo(self)
  self._label = ui.newTTFLabel({
    text = "",
    size = 28,
    align = ui.TEXT_ALIGN_CENTER,
    color = g.font.color.WHITE
  }):addTo(self)
  self:setTouchEnabled(true)
  self:setTouchSwallowEnabled(true)
  self:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self.onTouch))
end
function ControlButton:onTouch(evt)
  if self._enabled then
    local name, x, y, prevX, prevY = evt.name, evt.x, evt.y, evt.prevX, evt.prevY
    local isTouchInBounding = self:getCascadeBoundingBox():containsPoint(CCPoint(x, y))
    if name == "began" then
      self._pressed = true
    elseif name == "moved" then
      self._pressed = true
    elseif (name == "ended" or name == "cancelled") and self._pressed then
      self._pressed = false
      if isTouchInBounding then
        if self._buttonType == ControlButton.TYPE_CHECK then
          self._checked = not self._checked
          if self._changeCallback ~= nil then
            self._changeCallback(self._changeCallbackTarget, evt)
          end
        end
        if self._touchCallback ~= nil then
          self._touchCallback(self._touchCallbackTarget, evt)
        end
        self:render()
        return false
      end
    end
    self:render()
    return true
  end
end
function ControlButton:setTouchCallback(target, value)
  self._touchCallbackTarget = target
  self._touchCallback = value
end
function ControlButton:getTouchCallback()
  return self._touchCallback
end
function ControlButton:setChangeCallback(target, value)
  self._changeCallbackTarget = target
  self._changeCallback = value
end
function ControlButton:getChangeCallback()
  return self._changeCallback
end
function ControlButton:disable()
  self._enabled = false
  self._pressed = false
  self:render()
end
function ControlButton:enable()
  self._enabled = true
  self:render()
end
function ControlButton:setText(value)
  self._label:setString(value)
end
function ControlButton:getText()
  if self._label ~= nil then
    return self._label:getString()
  else
    return nil
  end
end
function ControlButton:getChecked()
  return self._checked
end
function ControlButton:setChecked(value)
  self._checked = value
  self:render()
end
function ControlButton:getButtonType()
  return self._buttonType
end
function ControlButton:setButtonType(value)
  self._buttonType = value
  self:render()
end
function ControlButton:render()
  if self._buttonType == ControlButton.TYPE_NORMAL then
    self._checkBg:hide()
    self._checkIcon:hide()
    self._label:pos(0, 0)
  elseif self._buttonType == ControlButton.TYPE_CHECK then
    self._checkBg:show()
    if self._checked then
      self._checkIcon:show()
    else
      self._checkIcon:hide()
    end
    self._label:pos(32, 0)
  end
  if self._enabled then
    if self._buttonType == ControlButton.TYPE_NORMAL then
      if self._pressed then
        self:changeStatusImage("NORMAL_DOWN")
        self._label:setColor(g.font.color.WHITE)
      else
        self:changeStatusImage("NORMAL_UP")
        self._label:setColor(g.font.color.WHITE)
      end
    elseif self._buttonType == ControlButton.TYPE_CHECK then
      if self._pressed then
        self:changeStatusImage("CHECK_DOWN")
        self._label:setColor(g.font.color.WHITE)
      elseif self._checked then
        self:changeStatusImage("CHECK_SELECTED")
        self._label:setColor(g.font.color.WHITE)
      else
        self:changeStatusImage("CHECK_UP")
        self._label:setColor(g.font.color.WHITE)
      end
    end
  else
    self:changeStatusImage("CHECK_DOWN")
    self._label:setColor(ccc3(131, 136, 145))
  end
end
function ControlButton:changeStatusImage(status)
  for k, v in pairs(self._statusImages) do
    if k == status then
      v:show()
    else
      v:hide()
    end
  end
end
return ControlButton

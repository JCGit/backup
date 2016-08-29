local RaisePanel = class("RaisePanel", function()
  return display.newNode()
end)
RaisePanel.WIDTH = 150
RaisePanel.HEIGHT = 450
RaisePanel.PROGRESS_HEIGHT = 408
function RaisePanel:ctor(onRaiseChangeHandler)
  self.onRaiseChangeHandler = onRaiseChangeHandler
  local labelWidth = RaisePanel.WIDTH - 16 - 12
  self._backgroundLeft = display.newSprite("#table_increaseBetLeft_bg.png", -RaisePanel.WIDTH * 0.25 + 1, 0)
  self._backgroundLeft:addTo(self)
  self._backgroundLeft:setTouchEnabled(true)
  self._backgroundLeft:setTouchSwallowEnabled(true)
  self._backgroundRight = display.newSprite("#table_increaseBetLeft_bg.png", RaisePanel.WIDTH * 0.25 - 1, 0)
  self._backgroundRight:addTo(self)
  self._backgroundRight:setScaleX(-1)
  self._backgroundRight:setTouchEnabled(true)
  self._backgroundRight:setTouchSwallowEnabled(true)
  self._sliderBarPoint = display.newSprite("#table_increaseBet_point.png")
  self._sliderBarPoint:setRotation(90)
  self._sliderBarPoint:setTouchEnabled(true)
  self._sliderBarPoint:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onSliderBarPointTouch))
  self._sliderBarPoint:addTo(self, 1)
  self.progress = display.newSprite("#table_increaseBet_slider.png")
  local innerBGHeight = self.progress:getContentSize().height + 10
  self.progressRealHeight = innerBGHeight - self._sliderBarPoint:getContentSize().height
  self.progressInnerBG = display.newScale9Sprite("#table_increaseBet_inner_bg.png"):addTo(self)
  self.progressInnerBG:setContentSize(CCSize(self.progressInnerBG:getContentSize().width, self.progress:getContentSize().height + 10))
  self.imgClip = CCClippingNode:create():addTo(self)
  self.imgClip:pos(0, 0)
  self.stencil = display.newScale9Sprite("#table_increaseBet_slider_stencil.png", 0, 0, cc.size(32, self.progress:getContentSize().height))
  self.imgClip:setAlphaThreshold(0)
  self.imgClip:setStencil(self.stencil)
  self.progress:addTo(self.imgClip)
  RaisePanel.BOUNDARY_TOP = innerBGHeight * 0.5 - self._sliderBarPoint:getContentSize().height * 0.5
  RaisePanel.BOUNDARY_BOTTOM = -innerBGHeight * 0.5 + self._sliderBarPoint:getContentSize().height * 0.5
  RaisePanel.BOUNDARY_HEIGHT = RaisePanel.BOUNDARY_TOP - RaisePanel.BOUNDARY_BOTTOM
  self._range = 0
  self._min = 0
  self._max = 0
  self:setSliderPercentValue(0)
end
function RaisePanel:showPanel()
  self:setSliderPercentValue(0)
  return self:show()
end
function RaisePanel:hidePanel()
  self:setSliderPercentValue(0)
  return self:hide()
end
function RaisePanel:setMin(value)
  self._min = value
  if self._max ~= nil then
    self._range = self._max - self._min
  end
  return self
end
function RaisePanel:setMax(value)
  self._max = value
  if self._min ~= nil then
    self._range = self._max - self._min
  end
  return self
end
function RaisePanel:setValue(value)
  if self._range and self._range > 0 then
    self:setSliderPercentValue(value / self._range)
  else
    self:setSliderPercentValue(0)
  end
  return self
end
function RaisePanel:getValue()
  return math.round(self:getSliderPercentValue() * self._range + self._min)
end
function RaisePanel:setSliderPercentValue(value)
  assert(value >= 0 and value <= 1, "slider value must be between 0 and 1")
  self:_onSliderPercentValueChange(value, true)
  self._sliderBarPoint:setPositionY(RaisePanel.BOUNDARY_BOTTOM + RaisePanel.BOUNDARY_HEIGHT * value)
  return self
end
function RaisePanel:getSliderPercentValue()
  return (self._sliderBarPoint:getPositionY() - RaisePanel.BOUNDARY_BOTTOM) / RaisePanel.BOUNDARY_HEIGHT
end
function RaisePanel:_onSliderPercentValueChange(value, forceUpdate, needSound)
  if self._percentValue ~= value or forceUpdate then
    self._percentValue = value
    if value == 1 then
    else
    end
    self._previousValue = self._currentValue
    self._currentValue = self:getValue()
    local currentTime = g.util.functions.getTime()
    local prevTime = self._lastRaiseSliderGearTickPlayTime or 0
    if needSound and self._previousValue ~= self._currentValue and currentTime - prevTime > 0.05 then
      self._lastRaiseSliderGearTickPlayTime = currentTime
    end
    if self.onRaiseChangeHandler then
      self.onRaiseChangeHandler(self._currentValue)
    end
    self.stencil:setPositionY(self.progressRealHeight * (value - 1))
  end
end
function RaisePanel:_onSliderBarPointTouch(evt)
  local name, x, y, prevX, prevY = evt.name, evt.x, evt.y, evt.prevX, evt.prevY
  local isTouchInSprite = self._sliderBarPoint:getCascadeBoundingBox():containsPoint(CCPoint(x, y))
  if name == "began" then
    if isTouchInSprite then
      self._isSliderBarTouching = true
      self._sliderBarTouchBeginY = y
      self._sliderBarBeginY = self._sliderBarPoint:getPositionY()
      return true
    else
      return false
    end
  elseif not self._isSliderBarTouching then
    return false
  elseif name == "moved" then
    local movedY = y - self._sliderBarTouchBeginY
    local toY = self._sliderBarBeginY + movedY
    if toY >= RaisePanel.BOUNDARY_TOP then
      toY = RaisePanel.BOUNDARY_TOP
    elseif toY <= RaisePanel.BOUNDARY_BOTTOM then
      toY = RaisePanel.BOUNDARY_BOTTOM
    end
    self._sliderBarPoint:setPositionY(toY)
    local value = (toY - RaisePanel.BOUNDARY_BOTTOM) / RaisePanel.BOUNDARY_HEIGHT
    self:_onSliderPercentValueChange(value, false, true)
  elseif name == "ended" or name == "cancelled" then
    self._isSliderBarTouching = false
  end
  return true
end
return RaisePanel

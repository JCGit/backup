local StarView = class("StarView", function()
  return display.newNode()
end)
function StarView:ctor(image)
  self:setCascadeOpacityEnabled(true)
  self:setNodeEventEnabled(false)
  self._image = display.newSprite(image):addTo(self)
end
function StarView:play(delay, min, max, blink)
  self:performWithDelay(handler(self, function(self)
    self:runAction(CCRepeatForever:create(transition.sequence({
      CCScaleTo:create(blink, min),
      CCScaleTo:create(blink, max)
    })))
  end), delay)
  return self
end
function StarView:stop()
  self:stopAllActions()
  return self
end
return StarView

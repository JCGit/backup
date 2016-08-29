local TableUIConfig = import(".TableUIConfig")
local SearchLight = class("SearchLight", function()
  return display.newNode()
end)
function SearchLight:ctor()
  self._light = display.newSprite("#table_light.png")
  self._light:setAnchorPoint(CCPoint(0.5, 1))
  self._lightSourcePoint = ccp(TableUIConfig.LIGHT_SOURCE.x, TableUIConfig.LIGHT_SOURCE.y)
  self._light:pos(self._lightSourcePoint.x, self._lightSourcePoint.y)
  self._light:setScaleX(1.5)
  self._light:addTo(self)
  self._defaultLightHeight = self._light:getContentSize().height
  self._isWorking = false
  self.restoreSeatId = nil
end
function SearchLight:stopAllAnimation()
  transition.stopTarget(self._light)
end
function SearchLight:show()
  self._isShow = true
  self._light:show()
end
function SearchLight:hide()
  self._isShow = false
  self._light:hide()
end
function SearchLight:isVisible()
  return self._isShow
end
function SearchLight:fadeIn()
  self._light:opacity(0)
  transition.fadeTo(self._light, {
    opacity = 255,
    time = 0.3,
    onComplete = function()
      self:show()
    end
  })
end
function SearchLight:fadeOut()
  transition.fadeTo(self._light, {
    opacity = 0,
    time = 0.3,
    onComplete = function()
      self:hide()
    end
  })
end
function SearchLight:turnTo(seatId, foucusPoint, animation)
  self:setRestoreSeatId(seatId)
  local foucusPointX = foucusPoint.x
  local foucusPointY = foucusPoint.y
  local angleOffset = math.radian2angle(math.atan((foucusPointX - self._lightSourcePoint.x) / (foucusPointY - self._lightSourcePoint.y)))
  if foucusPointX < self._lightSourcePoint.x then
    if angleOffset > 0 then
      angleOffset = angleOffset - 180
    end
  elseif angleOffset <= 0 then
    angleOffset = angleOffset + 180
  end
  local seatLightScale = ((foucusPointX - self._lightSourcePoint.x) ^ 2 + (foucusPointY - self._lightSourcePoint.y) ^ 2) ^ 0.5 / self._defaultLightHeight
  self:_setLightRotation(animation, angleOffset, seatLightScale)
end
local toRange360 = function(rotation)
  if rotation >= 360 then
    while true do
      rotation = rotation - 360
      if rotation < 360 then
        return rotation
      end
    end
  else
    while true do
      if rotation < 0 then
        rotation = rotation + 360
        if rotation >= 0 then
          return rotation
        end
      end
    end
  end
  return rotation
end
function SearchLight:_setLightRotation(animation, rotation, scale)
  if animation then
    local curRot = self._light:getRotation()
    local dstRot = toRange360(rotation - 180)
    if 180 < math.abs(dstRot - curRot) then
      if curRot < 180 then
        dstRot = dstRot - 360
      else
        dstRot = dstRot + 360
      end
    end
    transition.scaleTo(self._light, {scaleY = scale, time = 0.5})
    transition.rotateTo(self._light, {rotate = dstRot, time = 0.5})
  else
    self._light:setRotation(rotation - 180)
    self._light:setScaleY(scale)
  end
end
function SearchLight:setRestoreSeatId(seatId)
  self.restoreSeatId = seatId
end
function SearchLight:getRestoreSeatId()
  return self.restoreSeatId
end
function SearchLight:clearData()
  self:setRestoreSeatId(nil)
  self:setIsWorking(false)
end
function SearchLight:getIsWorking()
  return self._isWorking
end
function SearchLight:setIsWorking(isWorking)
  self._isWorking = isWorking
end
return SearchLight

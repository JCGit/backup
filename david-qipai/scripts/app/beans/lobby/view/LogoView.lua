local LogoView = class("LogoView", function()
  return display.newNode()
end)
function LogoView:ctor()
  self._content = display.newNode():addTo(self):scale(g.scaleWidth)
  display.newSprite("#login_logo_image_bg.png"):addTo(self._content)
  self.dotPoint1 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._content):scale(0.5)
  self.dotPoint2 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._content)
  self.dotPoint3 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._content)
  self.dotPoint4 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._content)
  self.dotPoint5 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._content):scale(0.3)
  self.dotPoint6 = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self._content):scale(0.4)
  self:_onEnterFrame()
  self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self._onEnterFrame))
  self:scheduleUpdate()
end
function LogoView:_onEnterFrame()
  if self.nextY1 == nil or self.nextY1 == 0 then
    self.nextX1 = 0
    self.nextV1 = -1
    self.nextY1 = 0
  end
  self.nextX1, self.nextV1 = self:calculate(self.nextX1, self.nextV1, 0.05)
  self.nextY1 = self.nextY1 + 1
  if self.nextY1 >= 180 then
    self.nextY1 = 0
    self.dotPoint1:setOpacity(255)
  elseif self.nextY1 > 120 then
    self.dotPoint1:setOpacity(255 - (self.nextY1 - 120))
  end
  self.dotPoint1:setPositionX(self.nextX1 - 160)
  self.dotPoint1:setPositionY(self.nextY1 - 30)
  if self.nextY2 == nil or self.nextY2 == 0 then
    self.nextX2 = 0
    self.nextV2 = -1.5
    self.nextY2 = 0
    self.nextScale2 = 1
    self.nextOpacity2 = 255
  end
  self.nextX2, self.nextV2 = self:calculate(self.nextX2, self.nextV2, 0.05)
  self.nextY2 = self.nextY2 + 1.5
  if 180 <= self.nextY2 then
    self.nextY2 = 0
  elseif self.nextY2 > 100 then
    if 120 > self.nextY2 then
      self.dotPoint2:scale(0.16666666666666666 + (self.nextY2 - 100) / 20 * 100 / 120)
      self.nextScale2 = self.nextScale2 + 0.02
    else
      self.nextScale2 = self.nextScale2 - 0.001
      self.nextOpacity2 = self.nextOpacity2 - 4
    end
  else
    self.nextScale2 = self.nextScale2 - 0.005
  end
  self.dotPoint2:scale(self.nextScale2)
  self.dotPoint2:setPositionX(self.nextX2 - 180)
  self.dotPoint2:setPositionY(self.nextY2 - 20)
  self.dotPoint2:setOpacity(self.nextOpacity2)
  if self.nextY3 == nil or self.nextY3 == 0 then
    self.nextX3 = 0
    self.nextV3 = -1.8
    self.nextY3 = 0
    self.nextScale3 = 1
    self.nextOpacity3 = 255
  end
  self.nextX3, self.nextV3 = self:calculate(self.nextX3, self.nextV3, 0.06)
  self.nextY3 = self.nextY3 + 1.5
  if self.nextY3 >= 140 then
    self.nextY3 = 0
    self.dotPoint3:setOpacity(255)
  elseif self.nextY3 > 80 then
    if 120 > self.nextY3 then
      self.nextScale3 = self.nextScale3 + 0.02
    else
      self.nextScale3 = self.nextScale3 - 0.001
      self.nextOpacity3 = self.nextOpacity3 - 10
    end
  else
    self.nextScale3 = self.nextScale3 - 0.01
  end
  self.dotPoint3:scale(self.nextScale3)
  self.dotPoint3:setPositionX(self.nextX3 - 160)
  self.dotPoint3:setPositionY(self.nextY3)
  self.dotPoint3:setOpacity(self.nextOpacity3)
  if self.nextY4 == nil or self.nextY4 == 0 then
    self.nextX4 = 0
    self.nextV4 = 1.5
    self.nextY4 = 0
    self.nextScale4 = 1
    self.nextOpacity4 = 255
  end
  self.nextX4, self.nextV4 = self:calculate(self.nextX4, self.nextV4, 0.05)
  self.nextY4 = self.nextY4 + 2
  if 140 <= self.nextY4 then
    self.nextY4 = 0
  elseif self.nextY4 > 60 then
    if 100 > self.nextY4 then
      self.nextScale4 = self.nextScale4 + 0.015
    else
      self.nextScale4 = self.nextScale4 - 0.001
      self.nextOpacity4 = self.nextOpacity4 - 10
    end
  else
    self.nextScale4 = self.nextScale4 - 0.02
  end
  self.dotPoint4:scale(self.nextScale4)
  self.dotPoint4:setPositionX(self.nextX4 - 160)
  self.dotPoint4:setPositionY(self.nextY4)
  self.dotPoint4:setOpacity(self.nextOpacity4)
  if self.nextY5 == nil or self.nextY5 == 0 then
    self.nextY5 = 0
    self.nextX5 = 0
    self.nextV5 = 1
    self.nextOpacity5 = 255
  end
  self.nextX5, self.nextV5 = self:calculate(self.nextX5, self.nextV5, 0.1)
  self.nextY5 = self.nextY5 + 1
  if self.nextY5 >= 260 then
    self.nextY5 = 0
  elseif self.nextY5 > 130 then
    self.nextOpacity5 = self.nextOpacity5 - 3
  end
  self.dotPoint5:setPositionX(self.nextX5 - 130)
  self.dotPoint5:setPositionY(self.nextY5 - 40)
  self.dotPoint5:setOpacity(self.nextOpacity5)
  if self.nextY6 == nil or self.nextY6 == 0 then
    self.nextY6 = 0
    self.nextX6 = 0
    self.nextV6 = 0.5
  end
  self.nextX6, self.nextV6 = self:calculate(self.nextX6, self.nextV6, 0.01)
  self.nextY6 = self.nextY6 + 1
  if 100 <= self.nextY6 then
    self.nextY6 = 0
    self.dotPoint6:setOpacity(255)
  elseif 80 < self.nextY6 then
    self.dotPoint6:setOpacity(255 - (self.nextY6 - 80))
  end
  self.dotPoint6:setPositionX(self.nextX6 - 150)
  self.dotPoint6:setPositionY(self.nextY6)
end
function LogoView:calculate(x, v, k)
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
function LogoView:playDotAnimation1()
  for i = 1, 1 do
    do
      local posX = 70
      local posY = 80
      local dotPoint
      local dotNode = display.newNode():addTo(self.leftIcon):pos(0, 0)
      if i % 2 == 0 then
        dotPoint = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(dotNode):pos(posX, posY)
      else
        dotPoint = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(dotNode):pos(posX, posY)
      end
      transition.moveTo(dotNode, {time = 0.5, y = 80})
      transition.moveTo(dotNode, {
        time = 0.5,
        delay = 0.5,
        y = 200,
        onComplete = function()
          dotNode:setPositionY(0)
        end
      })
    end
  end
end
function LogoView:playDotAnimation()
  for i = 1, 6 do
    do
      local posX = 100 + (i - 3) * 20 + math.random(0, 10)
      local posY = 50 + math.random(0, 50)
      local dotPoint
      if i % 2 == 0 then
        dotPoint = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self.leftIcon):pos(posX, posY)
      else
        dotPoint = display.newSprite("#login_logo_image_blade_dot_1.png"):addTo(self.leftIcon):pos(posX, posY)
      end
      dotPoint:setOpacity(0)
      local dealyTime = math.random(0, 100) / 100
      local part1Time = 0.4 + math.random(0, 10) / 100
      local part2Time = 0.4 + math.random(0, 10) / 100
      dotPoint:runAction(CCRepeatForever:create(transition.sequence({
        CCDelayTime:create(dealyTime),
        cc.FadeTo:create((part1Time + part2Time) / 2, 255),
        cc.FadeTo:create((part1Time + part2Time) / 2, 0)
      })))
      local initAction = CCCallFunc:create(function()
        dotPoint:pos(posX, posY)
      end)
      local fromPoint = ccp(posX, posY)
      local toPoint = ccp(posX, posY + 80)
      local distance = cc.PointDistance(fromPoint, toPoint)
      local bconfig1 = cc.BezierConfig()
      bconfig1.endPosition = toPoint
      bconfig1.controlPoint_1 = ccp((fromPoint.x + toPoint.x) * 0.5 - distance, (fromPoint.y + toPoint.y) * 0.5)
      bconfig1.controlPoint_2 = toPoint
      local fromPoint2 = ccp(posX, posY + 80)
      local toPoint2 = ccp(posX, posY + 160)
      local bconfig2 = cc.BezierConfig()
      bconfig2.endPosition = toPoint2
      bconfig2.controlPoint_1 = ccp((fromPoint2.x + toPoint2.x) * 0.5 + distance, (fromPoint2.y + toPoint2.y) * 0.5)
      bconfig2.controlPoint_2 = toPoint2
      dotPoint:runAction(CCRepeatForever:create(transition.sequence({
        initAction,
        CCDelayTime:create(dealyTime),
        CCBezierTo:create(part1Time, bconfig1),
        CCBezierTo:create(part2Time, bconfig2)
      })))
    end
  end
end
return LogoView

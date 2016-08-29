local socket = require("socket")
local SeatClock = class("SeatClock", function()
  return cc.RenderTexture:create(display.width, display.height)
end)
SeatClock.ERASER_WIDTH = 7
function SeatClock:ctor()
  self.isSelf = false
  self.clockWidth = 0
  self.clockHeight = 0
  self._totalTime = 0
  self._startTime = 0
  self._endTime = 0
  self._curTime = 0
end
function SeatClock:dispose()
  self:clearData()
end
function SeatClock:_timePoint(specTime, endTime, totalTime)
  local leftTime = endTime - specTime
  local goneTime = totalTime - leftTime
  local goneDistance = (self.clockWidth + self.clockHeight) * 2 * goneTime / totalTime
  return self:_endPointForDistance(goneDistance)
end
function SeatClock:_endPointForDistance(goneDistance)
  if goneDistance < self.clockWidth then
    return ccp(-self.clockWidth / 2 + goneDistance, self.clockHeight / 2)
  elseif goneDistance < self.clockWidth + self.clockHeight then
    return ccp(self.clockWidth / 2, -goneDistance + self.clockWidth + self.clockHeight / 2)
  elseif goneDistance < self.clockWidth + self.clockHeight + self.clockWidth then
    return ccp(self.clockWidth * 3 / 2 + self.clockHeight - goneDistance, -self.clockHeight / 2)
  else
    return ccp(-self.clockWidth / 2, goneDistance - (self.clockWidth * 2 + self.clockHeight * 3 / 2))
  end
end
function SeatClock:_getDistanceWithEndPoint(endDrawPoint)
  local distance = 0
  if endDrawPoint.y == self.clockHeight / 2 then
    distance = endDrawPoint.x + self.clockWidth / 2
    return distance, 1, distance / self.clockWidth
  elseif endDrawPoint.x == self.clockWidth / 2 then
    distance = self.clockWidth + self.clockHeight / 2 - endDrawPoint.y
    return distance, 2, (distance - self.clockWidth) / self.clockHeight
  elseif endDrawPoint.y == -self.clockHeight / 2 then
    distance = self.clockWidth + self.clockHeight + self.clockWidth / 2 - endDrawPoint.x
    return distance, 3, (distance - self.clockWidth - self.clockHeight) / self.clockWidth
  elseif endDrawPoint.x == -self.clockWidth / 2 then
    distance = self.clockWidth * 2 + self.clockHeight + self.clockHeight / 2 + endDrawPoint.y
    return distance, 4, (distance - self.clockWidth - self.clockHeight * 2) / self.clockHeight
  end
  logger:debug("异常")
  logger:debug("endpoint x :" .. endDrawPoint.x .. "  y :" .. endDrawPoint.y)
  return 0, 1, 0
end
function SeatClock:_refreshPoints(startDrawPoint, endDrawPoint)
  local points = {}
  local lastGoneDistance = self:_getDistanceWithEndPoint(startDrawPoint)
  local endGoneDistance, lineNumber, percent = self:_getDistanceWithEndPoint(endDrawPoint)
  local nextStep = lastGoneDistance + SeatClock.ERASER_WIDTH
  while endGoneDistance > nextStep do
    local curPoint = self:_endPointForDistance(nextStep)
    table.insert(points, curPoint)
    nextStep = nextStep + SeatClock.ERASER_WIDTH
  end
  table.insert(points, endDrawPoint)
  return points, lineNumber, percent
end
function SeatClock:_refreshFrame()
  if self._curTime > self._endTime then
    self:clearData()
    return
  end
  self._curTime = socket.gettime()
  if self._endTime - self._curTime < self._totalTime * 0.25 and not self.hasTap then
    self.hasTap = true
    if self.isSelf then
      g.audio:playSound("TAP_TABLE")
    end
  end
  self._erasePoint = self:_timePoint(self._curTime, self._endTime, self._totalTime)
  local points, lineNumber, percent = self:_refreshPoints(self._lastDrawPoint, self._erasePoint)
  for i, point in ipairs(points) do
    if lineNumber == 1 then
      self._rgbRed = 0
      self._rgbGreen = 255
      self:getSprite():setColor(ccc3(self._rgbRed, self._rgbGreen, 0))
    elseif lineNumber == 2 then
      self._rgbRed = 255 * percent
      self._rgbGreen = 255
      self:getSprite():setColor(ccc3(self._rgbRed, self._rgbGreen, 0))
    elseif lineNumber == 3 then
      self._rgbRed = 255
      self._rgbGreen = 255 * (1 - percent)
      self:getSprite():setColor(ccc3(self._rgbRed, self._rgbGreen, 0))
    elseif lineNumber == 4 then
      self._rgbRed = 255
      self._rgbGreen = 0
      self:getSprite():setColor(ccc3(self._rgbRed, self._rgbGreen, 0))
    end
    self._eraser:drawDot(point, SeatClock.ERASER_WIDTH, ccc4f(0, 0, 0, 0))
  end
  self:begin()
  self._eraser:visit()
  self:endToLua()
  self._lastDrawPoint = self._erasePoint
end
function SeatClock:startCount(totalTime)
  self:show()
  self._totalTime = totalTime
  self._curTime = socket.gettime()
  self._startTime = self._curTime
  self._endTime = self._startTime + self._totalTime
  self._lastDrawPoint = ccp(0, 0)
  self._rgbRed = 0
  self._rgbGreen = 255
  if self.sp == nil then
    self.sp = display.newScale9Sprite("#table_timer_canvas.png", 0, 0, cc.size(116, 166))
    self.sp:retain()
    self.clockWidth = self.sp:getContentSize().width
    self.clockHeight = self.sp:getContentSize().height
  end
  self:begin()
  self.sp:pos(display.cx, display.cy):visit()
  self:endToLua()
  self._lastDrawPoint = ccp(-self.clockWidth / 2, self.clockHeight / 2)
  self:getSprite():setColor(ccc3(self._rgbRed, self._rgbGreen, 0))
  self:getSprite():getTexture():setAntiAliasTexParameters()
  if self._eraser == nil then
    self._eraser = CCDrawNode:create():pos(display.cx, display.cy)
    self._eraser:retain()
  end
  local blendFunc = ccBlendFunc:new()
  blendFunc.src = GL_ONE
  blendFunc.dst = GL_ZERO
  self._eraser:setBlendFunc(blendFunc)
  self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self._refreshFrame))
  self:scheduleUpdate()
end
function SeatClock:clearData()
  self:hide()
  self.hasTap = false
  if self._eraser then
    self._eraser:release()
    self._eraser = nil
  end
  if self.sp then
    self.sp:release()
    self.sp = nil
  end
  self:unscheduleUpdate()
  self:removeAllNodeEventListeners()
end
function SeatClock:onEnter()
end
function SeatClock:onExit()
  self:clearData()
end
return SeatClock

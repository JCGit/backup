local ScrollPanel = class("ScrollPanel", function()
  return display.newNode()
end)
ScrollPanel.DIRECTION_VERTICAL = 1
ScrollPanel.DIRECTION_HORIZONTAL = 2
ScrollPanel.DIRECTION_UP = 3
ScrollPanel.DIRECTION_DOWN = 4
ScrollPanel.DIRECTION_LEFT = 5
ScrollPanel.DIRECTION_RIGHT = 6
ScrollPanel.OUT_OF_TOP = 7
ScrollPanel.OUT_OF_BOTTOM = 8
ScrollPanel.INSIDE = 9
ScrollPanel.SCROLL_NORMAL = 10
ScrollPanel.SCROLL_REVERSE = 11
ScrollPanel.COUNT_STAY = 12
ScrollPanel.SPEED_ADJUST_RATIO = 50
ScrollPanel.MAX_STAY_TIME = 6
ScrollPanel.EASE_RATIO = 0.92
ScrollPanel.FASTEST_SPEED = 50
ScrollPanel.SLOWEST_SPEED = 1
ScrollPanel.EVENT_SCROLL_BEGIN = "ScrollPanel.EVENT_SCROLL_BEGIN"
ScrollPanel.EVENT_SCROLL_END = "ScrollPanel.EVENT_SCROLL_END"
ScrollPanel.EVENT_SCROLLING = "ScrollPanel.EVENT_SCROLLING"
function ScrollPanel:ctor(params)
  self:setNodeEventEnabled(true)
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
  self:setCascadeOpacityEnabled(true)
  self._clippingNode = cc.ClippingRegionNode:create():addTo(self, 2)
  self._clippingNode:setCascadeOpacityEnabled(true)
  self._isAutoHideInactive = true
  if params and params.visibleRect then
    self:setVisibleRect(params.visibleRect)
  end
  if params and params.visibleRect and params.background then
    local bgMargin = params.bgMargin or cc.size(0, 0)
    self.bgSprite = display.newScale9Sprite(params.background, 0, 0, cc.size(self._visibleRect.width + bgMargin.width, self._visibleRect.height + bgMargin.height)):addTo(self, 1)
  end
  if params and params.scrollLength then
    self:setScrollLength(params.scrollLength)
  end
  if params and params.direction then
    self:setDirection(params.direction)
  else
    self:setDirection(ScrollPanel.DIRECTION_VERTICAL)
  end
  if params and params.scrollContent then
    self:setScrollContent(params.scrollContent)
  end
  if params and params.scrollBar then
    self:setScrollBar(params.scrollBar)
  end
  self._currentPlace = 0
  self._bottomBoundary = 0
  self._outStatus = ScrollPanel.INSIDE
end
function ScrollPanel:setBackground(params)
  if params and params.background and self.bgSprite == nil then
    local bgMargin = params.bgMargin or cc.size(0, 0)
    self.bgSprite = display.newScale9Sprite(params.background, 0, 0, cc.size(self._visibleRect.width + bgMargin.width, self._visibleRect.height + bgMargin.height)):addTo(self, 1)
  end
end
function ScrollPanel:setVisibleRect(rect)
  self._clippingNode:setClippingRegion(rect)
  self._visibleRect = rect
  return self
end
function ScrollPanel:setScrollLength(length)
  self._scrollLength = length
  if self._scrollLength < 0 then
    self._scrollLength = 0
  end
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
    self._topBoundary = self._scrollLength
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    self._topBoundary = -self._scrollLength
  end
  return self
end
function ScrollPanel:setScrollContent(content)
  if self._scrollContent then
    if self._scrollContent == content then
      self._scrollContent:resetCascadeBoundingBox()
      self._scrollContent:removeAllNodeEventListeners()
    else
      self._scrollContent:removeFromParent()
    end
  end
  self._scrollContent = content
  if not self._scrollContent:getParent() then
    self._clippingNode:addChild(self._scrollContent)
  end
  local contentSize = self._scrollContent:getContentSize() or CCSize()
  local cascadeRect = self._scrollContent:getCascadeBoundingBox() or CCRect()
  local isContentSizeValid = contentSize.height > 0
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
    self._srcContentPlace = (self._visibleRect.height - (isContentSizeValid and contentSize.height or cascadeRect.height)) * 0.5
    self._scrollContent:pos(0, self._srcContentPlace)
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    self._srcContentPlace = ((isContentSizeValid and contentSize.width or cascadeRect.width) - self._visibleRect.width) * 0.5
    self._scrollContent:pos(self._srcContentPlace, 0)
  end
  self._scrollContent:setTouchEnabled(true)
  self._scrollContent:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onTouch))
  self._scrollContent:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self._onEnterFrame))
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
    self:setScrollLength((isContentSizeValid and contentSize.height or cascadeRect.height) - self._visibleRect.height)
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    self:setScrollLength((isContentSizeValid and contentSize.width or cascadeRect.height) - self._visibleRect.width)
  end
  return self
end
function ScrollPanel:setScrollBar(scrollBar)
  if self._scrollBar and self._scrollBar ~= scrollBar then
    self._scrollBar:removeFromParent()
  end
  self._scrollBar = scrollBar
  self._scrollBar:opacity(0)
  if not self._scrollBar:getParent() then
    self._clippingNode:addChild(self._scrollBar, 1)
  end
  if not self._srcScrollBarSize then
    self._srcScrollBarSize = self._scrollBar:getContentSize()
  end
  local barScale = 1
  local scrollBarSize = CCSize(self._srcScrollBarSize.width, self._srcScrollBarSize.height)
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
    if self._scrollLength and 0 < self._scrollLength then
      barScale = self._visibleRect.height / self._srcScrollBarSize.height / (self._scrollLength / self._visibleRect.height + 1)
      if barScale > 1 then
        scrollBarSize.height = self._srcScrollBarSize.height * barScale
      end
      self._scrollBar:setContentSize(scrollBarSize)
      self._scrollBarScrollLength = self._visibleRect.height - scrollBarSize.height
      self._scrollBar:pos((self._visibleRect.width - scrollBarSize.width) * 0.5 - 2, self._scrollBarScrollLength * 0.5)
      self._scrollBar:show()
    else
      self._scrollBar:hide()
    end
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    if self._scrollLength and 0 < self._scrollLength then
      barScale = self._visibleRect.width / self._srcScrollBarSize.width / (self._scrollLength / self._visibleRect.width + 1)
      if barScale > 1 then
        scrollBarSize.width = self._srcScrollBarSize.width * barScale
      end
      self._scrollBar:setContentSize(scrollBarSize)
      self._scrollBarScrollLength = self._visibleRect.width - scrollBarSize.width
      self._scrollBar:pos(-self._scrollBarScrollLength * 0.5, -(self._visibleRect.height - scrollBarSize.height) * 0.5 + 2)
      self._scrollBar:show()
    else
      self._scrollBar:hide()
    end
  end
end
function ScrollPanel:_onTouch(evt)
  local name, currentX, currentY, prevX, prevY = evt.name, evt.x, evt.y, evt.prevX, evt.prevY
  if name == "began" and not self:_isTouchInVisibleRect(evt) then
    return false
  end
  if name == "began" then
    self._srcTouchX = currentX
    self._srcTouchY = currentY
    self._srcTime = g.util.functions.getTime()
    self._srcPlace = self._currentPlace
    self._scrollFlag = ScrollPanel.COUNT_STAY
    self._stayTime = 0
    self._stayPlace = self._currentPlace + 1
    self._scrollContent:unscheduleUpdate()
    self._scrollContent:scheduleUpdate()
    if self._scrollBar:isVisible() then
      self._scrollBar:opacity(255)
      self._scrollBar:stopAllActions()
    end
    self:dispatchEvent({
      name = ScrollPanel.EVENT_SCROLL_BEGIN
    })
    return true
  elseif name == "moved" then
    self._destTouchX = currentX
    self._destTouchY = currentY
    if self._direction == ScrollPanel.DIRECTION_VERTICAL then
      self._currentPlace = self._srcPlace + self._destTouchY - self._srcTouchY
    elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
      self._currentPlace = self._srcPlace + self._destTouchX - self._srcTouchX
    end
    self:_checkContentIsOut()
    if self._outStatus ~= ScrollPanel.INSIDE then
      if self._direction == ScrollPanel.DIRECTION_VERTICAL then
        if self._outStatus == ScrollPanel.OUT_OF_TOP then
          self._currentPlace = self._topBoundary + (self._currentPlace - self._topBoundary) * 0.25
        elseif self._outStatus == ScrollPanel.OUT_OF_BOTTOM then
          self._currentPlace = self._bottomBoundary - (self._bottomBoundary - self._currentPlace) * 0.25
        end
      elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
        if self._outStatus == ScrollPanel.OUT_OF_TOP then
          self._currentPlace = self._topBoundary - (self._topBoundary - self._currentPlace) * 0.25
        elseif self._outStatus == ScrollPanel.OUT_OF_BOTTOM then
          self._currentPlace = self._bottomBoundary + (self._currentPlace - self._bottomBoundary) * 0.25
        end
      end
    end
    if self._currentPlace ~= self._srcPlace then
      self:_startScroll()
    end
  elseif name == "ended" or name == "cancelled" then
    self._scrollContent:unscheduleUpdate()
    self._destTouchX = currentX
    self._destTouchY = currentY
    self._destTime = g.util.functions.getTime()
    local dragTime = self._destTime - self._srcTime
    local dragDistance = 0
    if self._direction == ScrollPanel.DIRECTION_VERTICAL then
      dragDistance = self._destTouchY - self._srcTouchY
      self._speed = dragDistance / (dragTime * 1000) * ScrollPanel.SPEED_ADJUST_RATIO
      if 0 < self._speed then
        self._scrollDirection = ScrollPanel.DIRECTION_UP
      elseif 0 > self._speed then
        self._scrollDirection = ScrollPanel.DIRECTION_DOWN
      end
      self._speed = math.abs(self._speed)
    elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
      dragDistance = self._destTouchX - self._srcTouchX
      self._speed = dragDistance / (dragTime * 1000) * ScrollPanel.SPEED_ADJUST_RATIO
      if 0 < self._speed then
        self._scrollDirection = ScrollPanel.DIRECTION_RIGHT
      elseif 0 > self._speed then
        self._scrollDirection = ScrollPanel.DIRECTION_LEFT
      end
      self._speed = math.abs(self._speed)
    end
    self:_checkContentIsOut()
    if self._outStatus ~= ScrollPanel.INSIDE then
      self:_startBack()
    elseif self._stayTime <= ScrollPanel.MAX_STAY_TIME and self._speed ~= 0 then
      self:_fixSpeed()
      self._scrollFlag = ScrollPanel.SCROLL_NORMAL
      self._scrollContent:scheduleUpdate()
    else
      self:_startScroll()
      self:_fadeOutScrollBar()
    end
  end
end
function ScrollPanel:_onEnterFrame(dt)
  if self._scrollFlag == ScrollPanel.COUNT_STAY then
    self:_countStay()
  elseif self._scrollFlag == ScrollPanel.SCROLL_NORMAL then
    self:_scrollNormal()
  elseif self._scrollFlag == ScrollPanel.SCROLL_REVERSE then
    self:_scrollReverse()
  end
end
function ScrollPanel:_isTouchInVisibleRect(event)
  local visibleRect = self:convertToWorldSpace(ccp(self._visibleRect.origin.x, self._visibleRect.origin.y))
  visibleRect.width = self._visibleRect.width
  visibleRect.height = self._visibleRect.height
  return cc.rectContainsPoint(visibleRect, ccp(event.x, event.y))
end
function ScrollPanel:_checkContentIsOut()
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
    if self._currentPlace > self._topBoundary then
      self._outStatus = ScrollPanel.OUT_OF_TOP
    elseif self._currentPlace < self._bottomBoundary then
      self._outStatus = ScrollPanel.OUT_OF_BOTTOM
    else
      self._outStatus = ScrollPanel.INSIDE
    end
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    if self._currentPlace < self._topBoundary then
      self._outStatus = ScrollPanel.OUT_OF_TOP
    elseif self._currentPlace > self._bottomBoundary then
      self._outStatus = ScrollPanel.OUT_OF_BOTTOM
    else
      self._outStatus = ScrollPanel.INSIDE
    end
  end
end
function ScrollPanel:_startScroll()
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
    self._scrollContent:setPositionY(self._currentPlace + self._srcContentPlace)
    if self._scrollBar:isVisible() then
      local posY = self._scrollBarScrollLength * 0.5 - self._currentPlace / self._scrollLength * self._scrollBarScrollLength
      if posY < -self._scrollBarScrollLength * 0.5 then
        posY = -self._scrollBarScrollLength * 0.5
      elseif posY > self._scrollBarScrollLength * 0.5 then
        posY = self._scrollBarScrollLength * 0.5
      end
      self._scrollBar:setPositionY(posY)
    end
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    self._scrollContent:setPositionX(self._currentPlace + self._srcContentPlace)
    if self._scrollBar:isVisible() then
      local posX = -self._scrollBarScrollLength * 0.5 - self._currentPlace / self._scrollLength * self._scrollBarScrollLength
      if posX < -self._scrollBarScrollLength * 0.5 then
        posX = -self._scrollBarScrollLength * 0.5
      elseif posX > self._scrollBarScrollLength * 0.5 then
        posX = self._scrollBarScrollLength * 0.5
      end
      self._scrollBar:setPositionX(posX)
    end
  end
  if self._isAutoHideInactive then
    self:onScrolling()
  end
  self:dispatchEvent({
    name = ScrollPanel.EVENT_SCROLLING
  })
end
function ScrollPanel:_startBack()
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
    if self._outStatus == ScrollPanel.OUT_OF_TOP then
      self._speed = (self._currentPlace - self._topBoundary) / 8
      self._scrollDirection = ScrollPanel.DIRECTION_DOWN
    elseif self._outStatus == ScrollPanel.OUT_OF_BOTTOM then
      self._speed = (self._bottomBoundary - self._currentPlace) / 8
      self._scrollDirection = ScrollPanel.DIRECTION_UP
    end
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    if self._outStatus == ScrollPanel.OUT_OF_TOP then
      self._speed = (self._topBoundary - self._currentPlace) / 8
      self._scrollDirection = ScrollPanel.DIRECTION_RIGHT
    elseif self._outStatus == ScrollPanel.OUT_OF_BOTTOM then
      self._speed = (self._currentPlace - self._bottomBoundary) / 8
      self._scrollDirection = ScrollPanel.DIRECTION_LEFT
    end
  end
  self._scrollContent:scheduleUpdate()
  self._scrollFlag = ScrollPanel.SCROLL_REVERSE
end
function ScrollPanel:_fixSpeed()
  if self._speed < ScrollPanel.SLOWEST_SPEED then
    self._speed = ScrollPanel.SLOWEST_SPEED
  elseif self._speed > ScrollPanel.FASTEST_SPEED then
    self._speed = ScrollPanel.FASTEST_SPEED
  end
end
function ScrollPanel:_fadeOutScrollBar()
  self:dispatchEvent({
    name = ScrollPanel.EVENT_SCROLL_END
  })
  if self._scrollBar:isVisible() then
    self._scrollBar:fadeOut(0.8)
  end
end
function ScrollPanel:hideScrollBar()
  if self._scrollBar then
    self._scrollBar:hide()
  end
  return self
end
function ScrollPanel:scrollTo(place)
  self._currentPlace = self._currentPlace + place
  self:_fixCurrentPlace()
  self:_startScroll()
  return self
end
function ScrollPanel:update()
  if self._scrollContent then
    self:setScrollContent(self._scrollContent)
  end
  if self._scrollBar then
    self:setScrollBar(self._scrollBar)
  end
  self._currentPlace = 0
  self:_startScroll()
  self:setScrollContentTouchRect()
end
function ScrollPanel:getCurrentPlace()
  return self._currentPlace
end
function ScrollPanel:setScrollContentTouchRect()
  self._visibleRectOriginPoint = self:convertToWorldSpace(ccp(self._visibleRect.x, self._visibleRect.y))
  self._scrollContent:setCascadeBoundingBox(CCRect(self._visibleRectOriginPoint.x, self._visibleRectOriginPoint.y, self._visibleRect.width, self._visibleRect.height))
end
function ScrollPanel:onEnter()
  self:setScrollContentTouchRect()
end
function ScrollPanel:onScrolling()
end
function ScrollPanel:setAutoHideInactive(isAutoHideInactive)
  self._isAutoHideInactive = isAutoHideInactive
end
function ScrollPanel:_changePlace()
  if self._scrollDirection == ScrollPanel.DIRECTION_UP then
    self._currentPlace = self._currentPlace + self._speed
  elseif self._scrollDirection == ScrollPanel.DIRECTION_DOWN then
    self._currentPlace = self._currentPlace - self._speed
  elseif self._scrollDirection == ScrollPanel.DIRECTION_LEFT then
    self._currentPlace = self._currentPlace - self._speed
  elseif self._scrollDirection == ScrollPanel.DIRECTION_RIGHT then
    self._currentPlace = self._currentPlace + self._speed
  end
end
function ScrollPanel:_fixCurrentPlace()
  if self._direction == ScrollPanel.DIRECTION_VERTICAL and self._currentPlace >= self._topBoundary or self._direction == ScrollPanel.DIRECTION_HORIZONTAL and self._currentPlace <= self._topBoundary then
    self._currentPlace = self._topBoundary
  end
  if self._direction == ScrollPanel.DIRECTION_VERTICAL and self._currentPlace <= self._bottomBoundary or self._direction == ScrollPanel.DIRECTION_HORIZONTAL and self._currentPlace >= self._bottomBoundary then
    self._currentPlace = self._bottomBoundary
  end
end
function ScrollPanel:_slowDown()
  if self._speed * ScrollPanel.EASE_RATIO > ScrollPanel.SLOWEST_SPEED then
    self._speed = self._speed * ScrollPanel.EASE_RATIO
  else
    self._speed = ScrollPanel.SLOWEST_SPEED
  end
end
function ScrollPanel:_countStay()
  if self._stayPlace == self._currentPlace then
    self._stayTime = self._stayTime + 1
  else
    self._stayPlace = self._currentPlace
  end
end
function ScrollPanel:_scrollReverse()
  if self._outStatus == ScrollPanel.OUT_OF_TOP then
    self:_changePlace()
    self:_slowDown()
    if self._direction == ScrollPanel.DIRECTION_VERTICAL and self._currentPlace <= self._topBoundary or self._direction == ScrollPanel.DIRECTION_HORIZONTAL and self._currentPlace >= self._topBoundary then
      self._currentPlace = self._topBoundary
      self._scrollContent:unscheduleUpdate()
      self:_fadeOutScrollBar()
    end
    self:_startScroll()
  elseif self._outStatus == ScrollPanel.OUT_OF_BOTTOM then
    self:_changePlace()
    self:_slowDown()
    if self._direction == ScrollPanel.DIRECTION_VERTICAL and self._currentPlace >= self._bottomBoundary or self._direction == ScrollPanel.DIRECTION_HORIZONTAL and self._currentPlace <= self._bottomBoundary then
      self._currentPlace = self._bottomBoundary
      self._scrollContent:unscheduleUpdate()
      self:_fadeOutScrollBar()
    end
    self:_startScroll()
  else
    self._scrollContent:unscheduleUpdate()
    self:_fadeOutScrollBar()
  end
end
function ScrollPanel:_scrollNormal()
  self:_changePlace()
  self:_startScroll()
  self:_checkContentIsOut()
  if self._outStatus ~= ScrollPanel.INSIDE then
    self._speed = self._speed * 0.4
  end
  if self._speed <= ScrollPanel.SLOWEST_SPEED then
    self._scrollContent:unscheduleUpdate()
    if self._outStatus ~= ScrollPanel.INSIDE then
      self:_startBack()
    else
      self:_fadeOutScrollBar()
    end
  else
    self:_slowDown()
  end
end
function ScrollPanel:showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():addTo(self._clippingNode)
  end
end
function ScrollPanel:hideMiniLoading()
  if self._miniLoading then
    self._miniLoading:removeFromParent()
    self._miniLoading = nil
  end
end
function ScrollPanel:setDirection(direction)
  self._direction = direction
  return self
end
return ScrollPanel

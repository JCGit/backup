local PopupManager = class("PopupManager")
function PopupManager:ctor()
  self._stack = {}
  self._container = display.newNode()
  self._container:retain()
  self._container:setNodeEventEnabled(true)
  self._container.onCleanup = handler(self, function(param)
    if param._cover then
      param._cover:removeFromParent()
      param._cover = nil
    end
    for k, v in pairs(param._stack) do
      if v.popup then
        v.popup:removeFromParent()
      end
      param._stack[k] = nil
    end
    self._zOrder = 2
  end)
  self._zOrder = 2
end
function PopupManager:_onCoverClick()
  local value = self._stack[table.maxn(self._stack)]
  if value and value.popup and value.closeWhenClickCover then
    self:removePopup(value.popup)
  end
end
function PopupManager:addPopup(popup, isCover, isCenter, closeWhenClickCover, useShowAnimation)
  if isCover == nil then
    isCover = true
  end
  if isCenter == nil then
    isCenter = true
  end
  if not isCover then
    closeWhenClickCover = false
  elseif closeWhenClickCover == nil then
    closeWhenClickCover = true
  end
  if isCover and not self._cover then
    self._cover = display.newScale9Sprite("#panel_cover.png", 0, 0, CCSize(display.width, display.height)):pos(display.cx, display.cy):addTo(self._container)
    self._cover:setTouchEnabled(true)
    self._cover:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onCoverClick))
  end
  if isCenter then
    popup:pos(display.cx, display.cy)
  end
  if self:hasPopup(popup) then
    self:removePopup(popup)
  end
  table.insert(self._stack, {
    popup = popup,
    closeWhenClickCover = closeWhenClickCover,
    isCover = isCover
  })
  if useShowAnimation ~= false then
    local posX, posY = popup:getPosition()
    popup:pos(posX, display.cy + popup:getHeight() + posY)
    if popup.onShow then
      transition.moveTo(popup, {
        time = 0.5,
        easing = "backout",
        y = posY,
        onComplete = function()
          popup:onShow()
        end
      })
    else
      transition.moveTo(popup, {
        time = 0.5,
        easing = "backout",
        y = posY
      })
    end
    g.audio:playSound("POPUP_OPEN")
  end
  popup:addTo(self._container, self._zOrder)
  self._zOrder = self._zOrder + 2
  if not self._container:getParent() then
    self._container:addTo(cc.Director:sharedDirector():getRunningScene(), 1024)
  end
  if isCover then
    self._cover:setZOrder(popup:getZOrder() - 1)
  end
  if popup.onShowPopup then
    popup:onShowPopup()
  end
end
function PopupManager:removePopup(popup)
  if popup then
    local function removePopupFn()
      popup:removeFromParent()
      self._zOrder = self._zOrder - 2
      local bool, index = self:hasPopup(popup)
      table.remove(self._stack, index)
      if table.maxn(self._stack) == 0 then
        self._container:removeFromParent()
      else
        local needCover = false
        for k, v in pairs(self._stack) do
          if v.isCover then
            needCover = true
            self._cover:setZOrder(v.popup:getZOrder() - 1)
            break
          end
        end
        if not needCover then
          self._cover:removeFromParent()
          self._cover = nil
        end
      end
    end
    if popup.onPopupRemove then
      popup:onPopupRemove(removePopupFn)
    else
      removePopupFn()
    end
  end
end
function PopupManager:removeAllPopup()
  self._container:removeFromParent()
end
function PopupManager:hasPopup(popup)
  for k, v in ipairs(self._stack) do
    if v.popup == popup then
      return true, k
    end
  end
  return false, 0
end
function PopupManager:isOnTop(popup)
  if self._stack[#self._stack].popup == popup then
    return true
  else
    return false
  end
end
function PopupManager:removeTop()
  if #self._stack > 0 then
    local v = self._stack[#self._stack]
    self:removePopup(v.popup)
    return true
  end
  return false
end
return PopupManager

local ControlPanelTip = class("ControlPanelTip", function()
  return display.newNode()
end)
function ControlPanelTip:ctor()
  self.width = display.width * 0.35
  self._background = display.newScale9Sprite("#common_standard_greybg_01.png", 0, 0, cc.size(self.width, 72)):addTo(self)
  self._tipContents = g.lang:getContent("table.controlTips")
  self._currentIndex = 1
  self._indexSequence = g.util.functions.randomSequence(1, table.maxn(self._tipContents))
  self._isPlaying = false
  self._playInterval = nil
  self._clippingNode = CCClippingNode:create():addTo(self)
  local drawNode = display.newDrawNode()
  local w = self.width - 4
  local h = 68
  local vertex = {
    {
      -0.5 * w,
      -0.5 * h
    },
    {
      -0.5 * w,
      0.5 * h
    },
    {
      0.5 * w,
      0.5 * h
    },
    {
      0.5 * w,
      -0.5 * h
    }
  }
  local color = ccc4f(255, 0, 0, 255)
  drawNode:drawPolygon(vertex, color, 1, color)
  self._clippingNode:setStencil(drawNode)
  self._label = ui.newTTFLabel({
    size = 24,
    color = ccc3(43, 86, 134),
    text = self._tipContents[self._currentIndex],
    dimensions = cc.size(self.width - 8, 72),
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(self._clippingNode)
end
function ControlPanelTip:play()
  if not self._isPlaying then
    self._isPlaying = true
    if self._playInterval ~= nil then
      scheduler.unscheduleGlobal(self._playInterval)
      self._playInterval = nil
    end
    self._playInterval = scheduler.scheduleGlobal(handler(self, self.playNextTip), 30)
  end
  return self
end
function ControlPanelTip:stop()
  if self._isPlaying then
    self._isPlaying = false
    if self._playInterval ~= nil then
      scheduler.unscheduleGlobal(self._playInterval)
      self._playInterval = nil
    end
  end
end
function ControlPanelTip:playNextTip()
  if self._isPlaying then
    self._currentIndex = self._currentIndex + 1
    if self._currentIndex > table.maxn(self._tipContents) then
      self._currentIndex = 1
    end
    do
      local nextLabel = ui.newTTFLabel({
        size = 24,
        color = ccc3(43, 86, 134),
        text = self._tipContents[self._currentIndex],
        dimensions = cc.size(self.width - 8, 72),
        align = ui.TEXT_ALIGN_CENTER
      }):pos(0, -72):addTo(self._clippingNode)
      transition.moveTo(nextLabel, {
        time = 0.5,
        easing = "out",
        y = 0
      })
      transition.moveBy(self._label, {
        time = 0.5,
        easing = "out",
        y = 72,
        onComplete = function()
          self._label:removeFromParent()
          self._label = nextLabel
        end
      })
    end
  end
end
function ControlPanelTip:playContent(value)
end
return ControlPanelTip

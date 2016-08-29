local PokersStateTip = class("PokersStateTip", function()
  return display.newNode()
end)
PokersStateTip.WIDTH = display.width * 0.35
PokersStateTip.HEIGHT = 66
function PokersStateTip:ctor()
  self.width = PokersStateTip.WIDTH
  self._background = display.newScale9Sprite("#common_standard_greybg_01.png", 0, 0, cc.size(self.width, PokersStateTip.HEIGHT)):addTo(self)
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
    text = "",
    dimensions = cc.size(self.width - 8, PokersStateTip.HEIGHT),
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(self._clippingNode)
end
function PokersStateTip:playNextTip(tipContent)
  if self.nextLabel ~= nil then
    self.nextLabel:removeFromParent()
  end
  self.nextLabel = ui.newTTFLabel({
    size = 24,
    color = ccc3(43, 86, 134),
    text = tipContent,
    dimensions = cc.size(self.width - 8, PokersStateTip.HEIGHT),
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, -PokersStateTip.HEIGHT):addTo(self._clippingNode)
  transition.moveTo(self.nextLabel, {
    time = 0.5,
    easing = "out",
    y = 0
  })
  if self._label ~= nil then
    transition.moveBy(self._label, {
      time = 0.5,
      easing = "out",
      y = PokersStateTip.HEIGHT,
      onComplete = function()
        self._label:removeFromParent()
        self._label = self.nextLabel
        self.nextLabel = nil
      end
    })
  end
end
return PokersStateTip

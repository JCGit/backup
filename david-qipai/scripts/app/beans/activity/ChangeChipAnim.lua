local LEFT = 0
local BOTTOM = 0
local ChangeChipAnim = class("ChangeChipAnim", function()
  return display.newNode()
end)
function ChangeChipAnim:ctor(chips)
  self._chips = chips
  self._chipChangeAnimation = display.newSprite("#add_chips_bg.png"):pos(LEFT, BOTTOM):addTo(self)
  self._chipChangeLabel = ui.newTTFLabel({
    text = "+" .. string.formatnumberthousands(self._chips),
    color = ccc3(244, 205, 86),
    size = 32,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(LEFT, BOTTOM):addTo(self)
  transition.moveTo(self._chipChangeLabel, {
    time = 1,
    x = LEFT,
    y = BOTTOM + 60,
    rotation = 120,
    onComplete = handler(self, function(obj)
      self._chipChangeAnimation:opacity(0)
      self._chipChangeLabel:opacity(0)
    end)
  })
  self:performWithDelay(function()
    self._chipChangeAnimation:opacity(0)
  end, 10)
end
return ChangeChipAnim

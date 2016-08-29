local ProgressItem = class("ProgressItem", function()
  return display.newNode()
end)
local DayItem = import(".DayItem")
ProgressItem.WIDTH = 106
function ProgressItem:ctor()
end
function ProgressItem:setData(value)
  if self._data ~= value then
    local str = string.gsub(g.lang:getContent("dailybonus.days"), "{1}", value.index)
    self._titleLabel = ui.newTTFLabel({
      text = str,
      color = g.font.color.LIGHT,
      size = 20,
      align = ui.TEXT_ALIGN_CENTER
    }):pos(0, 30):addTo(self)
    self._image = display.newSprite("#common_chips_set_icon_" .. DayItem.PIC[value.index] .. ".png"):pos(0, -8):addTo(self)
    self._image:scale(66 / self._image:getContentSize().width)
    str = string.gsub(g.lang:getContent("dailybonus.rewardName"), "{chips}", string.formatnumberthousands(value.chips))
    self._nameLabel = ui.newTTFLabel({
      text = str,
      color = g.font.color.GOLDEN,
      size = 20,
      align = ui.TEXT_ALIGN_CENTER
    }):pos(0, -45):addTo(self)
    self._data = value
  end
end
return ProgressItem

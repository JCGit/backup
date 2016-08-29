local Component = import("..base.Component")
local ScrollPanel = import(".ScrollPanel")
local ScrollBar = class("ScrollBar", Component)
function ScrollBar:ctor(direction)
  ScrollBar.super.ctor(self)
  if direction == ScrollPanel.DIRECTION_VERTICAL then
    self._bar = display.newScale9Sprite("#vertical_scroll_bar.png", 0, 0, CCSize(8, 24))
  elseif direction == ScrollPanel.DIRECTION_HORIZONTAL then
    self._bar = display.newScale9Sprite("#horizontal_scroll_bar.png", 0, 0, CCSize(24, 8))
  else
    self._bar = display.newSprite()
  end
  self:addChild(self._bar)
end
return ScrollBar

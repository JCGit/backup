local DayItem = class("DayItem", function()
  return display.newNode()
end)
DayItem.WIDTH = 210
function DayItem:ctor()
  self:setCascadeOpacityEnabled(true)
end
function DayItem:setData(value)
  if self._data ~= value then
    local daysLabel = g.lang:getContent("registerReward.daysLabel")
    if value.isSigned then
      self._background = display.newScale9Sprite("#common_invitefriends_board_small_unavailable.png", 0, 0, cc.size(DayItem.WIDTH, 256)):addTo(self)
      self._descLabel = ui.newTTFLabel({
        text = value.desc,
        color = ccc3(106, 57, 6),
        size = 22,
        align = ui.TEXT_ALIGN_LEFT,
        valign = ui.TEXT_VALIGN_TOP,
        dimensions = cc.size(160, 90)
      }):pos(0, -72):addTo(self)
    else
      self._background = display.newScale9Sprite("#common_invitefriends_board_small_available.png", 0, 0, cc.size(DayItem.WIDTH, 258)):addTo(self)
      self._descLabel = ui.newTTFLabel({
        text = value.desc,
        color = ccc3(106, 57, 6),
        size = 22,
        align = ui.TEXT_ALIGN_LEFT,
        valign = ui.TEXT_VALIGN_TOP,
        dimensions = cc.size(160, 90)
      }):pos(0, -72):addTo(self)
    end
    self._titleLabel = display.newSprite("#register_item_title_" .. value.index .. ".png"):pos(0, 97):addTo(self)
    self._pic = display.newSprite("#register_box.png"):pos(4, 25):addTo(self)
    if value.isSigned then
      self._signBg = display.newScale9Sprite("#register_sign_bg.png", 0, 0, cc.size(DayItem.WIDTH, 258)):addTo(self)
    end
    self._data = value
  end
  return self
end
return DayItem

local DayItem = class("DayItem", function()
  return display.newNode()
end)
DayItem.WIDTH = 106
DayItem.PIC = {
  "01",
  "02",
  "03",
  "04",
  "05",
  "06",
  "08"
}
function DayItem:ctor()
  self:setCascadeOpacityEnabled(true)
end
function DayItem:setData(value)
  if self._data ~= value then
    self._background = display.newScale9Sprite("#dailybonus_unsign_bg.png", 0, 0, CCSize(DayItem.WIDTH, 154)):addTo(self)
    self._titleBg = display.newScale9Sprite("#dailybonus_tab_bg_selected.png", 0, 0, CCSize(100, 28)):pos(0, 60):addTo(self)
    local str = string.gsub(g.lang:getContent("dailybonus.days"), "{1}", value.index)
    self._titleLabel = ui.newTTFLabel({
      text = str,
      color = g.font.color.LIGHT,
      size = 22,
      align = ui.TEXT_ALIGN_CENTER
    }):pos(0, 60):addTo(self)
    self._image = display.newSprite("#common_chips_set_icon_" .. DayItem.PIC[value.index] .. ".png"):pos(0, -8):addTo(self, 8)
    self._image:scale(80 / self._image:getContentSize().width)
    str = string.gsub(g.lang:getContent("dailybonus.rewardName"), "{chips}", string.formatnumberthousands(value.chips))
    self._nameLabel = ui.newTTFLabel({
      text = str,
      color = g.font.color.GOLDEN,
      size = 20,
      align = ui.TEXT_ALIGN_CENTER
    }):pos(0, -60):addTo(self, 8)
    self._data = value
    if value.status == 2 then
      self:opacity(128)
    elseif value.status == 1 then
      local signedPic = display.newSprite("#dailybonus_sign_icon.png"):pos(0, 0):addTo(self, 16)
      self:opacity(255)
    else
      local picBg = display.newSprite("#dailybonus_highlight.png"):pos(0, -15):addTo(self, 0)
      self:opacity(255)
    end
  end
end
return DayItem

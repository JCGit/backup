local GiftListItem = class("GiftListItem", g.ui.ListItem)
local giftGroup = {}
local GiftItem = import(".GiftItem")
function GiftListItem:ctor()
  self:setNodeEventEnabled(true)
  GiftListItem.super.ctor(self, 708, 120)
end
function GiftListItem:createItems(count)
  self._giftItems = {}
  self._checkButtons = {}
  for i = 1, count do
    self._giftItems[i] = GiftItem.new()
    self._giftItems[i]:pos(136 * (i - 1), 0):addTo(self)
    self._checkButtons[i] = self._giftItems[i].checkButton
  end
end
function GiftListItem:setData(value, checkButtonGroup)
  if self._giftItems == nil then
    self:createItems(5)
  end
  if self._data ~= value then
    for i = 1, table.maxn(self._giftItems) do
      if value[i] then
        checkButtonGroup:addButton(self._checkButtons[i]:show(), value[i].giftId)
        self._giftItems[i]:setData(value[i])
      else
        self._giftItems[i]:hide()
      end
    end
  end
  self._data = value
  return self
end
return GiftListItem

local PropItem = import(".PropItem")
local GiftListItem = import(".GiftListItem")
local PropListItem = class("PropListItem", GiftListItem)
function PropListItem:ctor()
  self:setNodeEventEnabled(true)
  PropListItem.super.ctor(self)
  self:setContentSize(cc.size(708, 220))
end
function PropListItem:createItems(count)
  self._giftItems = {}
  self._checkButtons = {}
  for i = 1, count do
    self._giftItems[i] = PropItem.new()
    self._giftItems[i]:pos(219 * (i - 1) + 135, -5):addTo(self)
    self._checkButtons[i] = self._giftItems[i].checkButton
  end
end
function PropListItem:setData(value, checkButtonGroup)
  if self._giftItems == nil then
    self:createItems(3)
  end
  PropListItem.super.setData(self, value, checkButtonGroup)
end
return PropListItem

local ChipsItem = import(".ChipsItem")
local GiftListItem = require("app.beans.giftshop.view.GiftListItem")
local ChipsListItem = class("ChipsListItem", GiftListItem)
ChipsListItem.GAP_X = 0
ChipsListItem.MARGIN_X = 0
function ChipsListItem:ctor()
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
  self:setNodeEventEnabled(true)
  ChipsListItem.super.ctor(self)
  self:setContentSize(cc.size(708, 200))
end
function ChipsListItem:createItems(count)
  self._giftItems = {}
  self._checkButtons = {}
  for i = 1, count do
    self._giftItems[i] = ChipsItem.new()
    self._giftItems[i]:pos((ChipsItem.WIDTH + ChipsListItem.GAP_X) * (i - 1) + ChipsListItem.MARGIN_X, -5):addTo(self)
    self._giftItems[i]:addEventListener("ITEM_EVENT", handler(self, self._onItemEvent))
  end
end
function ChipsListItem:_onItemEvent(evt)
  self:dispatchEvent(evt)
end
function ChipsListItem:setData(value, checkButtonGroup)
  if self._giftItems == nil then
    self:createItems(3)
  end
  if self._data ~= value then
    for i = 1, table.maxn(self._giftItems) do
      if value[i] then
        self._giftItems[i]:setData(value[i])
      else
        self._giftItems[i]:hide()
      end
    end
  end
  self._data = value
  return self
end
return ChipsListItem

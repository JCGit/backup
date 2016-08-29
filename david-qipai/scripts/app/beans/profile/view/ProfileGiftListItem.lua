local GiftListItem = require("app.beans.giftshop.view.GiftListItem")
local ProfileGiftListItem = class("ProfileGiftListItem", GiftListItem)
local ProfileGiftItem = import(".ProfileGiftItem")
function ProfileGiftListItem:ctor()
  ProfileGiftListItem.super.ctor(self)
end
function ProfileGiftListItem:createItems(count)
  self._giftItems = {}
  self._checkButtons = {}
  for i = 1, count do
    self._giftItems[i] = ProfileGiftItem.new()
    self._giftItems[i]:pos(136 * (i - 1), 0):addTo(self)
    self._checkButtons[i] = self._giftItems[i].checkButton
  end
end
return ProfileGiftListItem

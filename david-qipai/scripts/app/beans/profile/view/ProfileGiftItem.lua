local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local GiftItem = require("app.beans.giftshop.view.GiftItem")
local ProfileGiftItem = class("ProfileGiftItem", GiftItem)
function ProfileGiftItem:ctor()
  ProfileGiftItem.super.ctor(self)
end
function ProfileGiftItem:onChange(evt)
  if evt.target:isButtonSelected() then
    g.eventCenter:dispatchEvent({
      name = DataBeanEvent.EVT_USER_GIFT_PREVIEW,
      data = self._data.giftId
    })
  end
end
return ProfileGiftItem

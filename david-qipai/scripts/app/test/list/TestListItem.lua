local TestListItem = class("TestListItem", g.ui.ListItem)
TestListItem.ITEM_WIDTH = 200
TestListItem.ITEM_HEIGHT = 50
function TestListItem:ctor()
  TestListItem.super.ctor(self, 200, 50)
  self._background = display.newScale9Sprite("#common_button_blue_up.png", 0, 0, CCSize(TestListItem.ITEM_WIDTH, TestListItem.ITEM_HEIGHT))
  self:addChild(self._background)
  self._textField = ui.newTTFLabel({
    text = "",
    size = 26,
    color = ccc3(214, 255, 239),
    align = ui.TEXT_ALIGN_CENTER
  })
  self:addChild(self._textField)
end
function TestListItem:setIndex(index)
  TestListItem.super.setIndex(self, index)
  self._textField:setString("Item :" .. tostring(index))
  return self
end
return TestListItem

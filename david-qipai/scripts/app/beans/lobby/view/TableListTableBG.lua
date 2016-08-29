local TableListTableBG = class("TableListTableBG", function()
  return display.newNode()
end)
function TableListTableBG:ctor()
  self.tableUpperLeft = display.newSprite("#mainlobby_table_upper_left.png"):addTo(self)
  self.tableUpperRight = display.newSprite("#mainlobby_table_upper_left.png"):addTo(self)
  self.tableUpperRight:setScaleX(-1)
  self.tableLowerLeft = display.newSprite("pd_mainlobby_table_lower_left.jpg"):addTo(self)
  self.tableLowerRight = display.newSprite("pd_mainlobby_table_lower_left.jpg"):addTo(self)
  self.tableLowerRight:setScaleX(-1)
  self.tableLight = display.newSprite("#mainlobby_table_light.png"):scale(4):addTo(self)
  self.upperHeight = self.tableUpperLeft:getContentSize().height
  self.upperLeftWidth = self.tableUpperLeft:getContentSize().width
  self.lowerHeight = self.tableLowerLeft:getContentSize().height
  self.lowerLeftWidth = self.tableLowerLeft:getContentSize().width
  self.tableUpperLeft:setAnchorPoint(ccp(1, 0))
  self.tableUpperLeft:pos(1, (self.upperHeight + self.lowerHeight) * 0.5 - self.upperHeight)
  self.tableUpperRight:setAnchorPoint(ccp(1, 0))
  self.tableUpperRight:pos(-1, (self.upperHeight + self.lowerHeight) * 0.5 - self.upperHeight)
  self.tableLight:setAnchorPoint(ccp(0.5, 0))
  self.tableLight:pos(0, (self.upperHeight + self.lowerHeight) * 0.5 - self.upperHeight - 17)
  self.tableLowerLeft:setAnchorPoint(ccp(1, 1))
  self.tableLowerLeft:pos(1, (self.upperHeight + self.lowerHeight) * 0.5 - self.upperHeight + 1)
  self.tableLowerRight:setAnchorPoint(ccp(1, 1))
  self.tableLowerRight:pos(-1, (self.upperHeight + self.lowerHeight) * 0.5 - self.upperHeight + 1)
end
function TableListTableBG:getTableHeight()
  return self.upperHeight + self.lowerHeight
end
function TableListTableBG:enabledHighLight()
  self.tableLight:show()
end
function TableListTableBG:disabledHighLight()
  self.tableLight:hide()
end
return TableListTableBG

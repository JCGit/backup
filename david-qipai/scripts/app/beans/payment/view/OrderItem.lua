local OrderItem = class("OrderItem", g.ui.ListItem)
OrderItem.WIDTH = 100
OrderItem.HEIGHT = 20
OrderItem.PADDING_LEFT = 10
OrderItem.PADDING_RIGHT = 2
local ROW_GAP = 2
function OrderItem:ctor()
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
  OrderItem.super.ctor(self, OrderItem.WIDTH, OrderItem.HEIGHT + ROW_GAP)
  self._indexLabel = ui.newTTFLabel({
    text = "",
    size = 28,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT
  })
  self._indexLabel:pos(OrderItem.PADDING_LEFT + 27, (OrderItem.HEIGHT + ROW_GAP) * 0.5)
  self._indexLabel:addTo(self)
  display.newScale9Sprite("#pay_item_bg.png", OrderItem.WIDTH * 0.5 + (OrderItem.PADDING_LEFT - OrderItem.PADDING_RIGHT) * 0.5 - 3, (OrderItem.HEIGHT + ROW_GAP) * 0.5, cc.size(OrderItem.WIDTH - OrderItem.PADDING_LEFT - OrderItem.PADDING_RIGHT, OrderItem.HEIGHT)):addTo(self)
  display.newSprite("#mainlobby_white_chips_icon.png"):pos(OrderItem.PADDING_LEFT + 135, (OrderItem.HEIGHT + ROW_GAP) * 0.5):addTo(self)
  self._nameLabel = ui.newTTFLabel({
    text = "",
    size = 28,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT
  })
  self._nameLabel:setAnchorPoint(ccp(0, 0.5))
  self._nameLabel:pos(OrderItem.PADDING_LEFT + 155, (OrderItem.HEIGHT + ROW_GAP) * 0.5)
  self._nameLabel:addTo(self)
  self._priceLabel = ui.newTTFLabel({
    text = "",
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_RIGHT
  })
  self._priceLabel:setAnchorPoint(ccp(0, 0.5))
  self._priceLabel:pos(OrderItem.WIDTH * 0.7, (OrderItem.HEIGHT + ROW_GAP) * 0.5)
  self._priceLabel:addTo(self)
  self._statusLabel = ui.newTTFLabel({
    text = "",
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_RIGHT
  })
  self._statusLabel:setAnchorPoint(ccp(0, 0.5))
  self._statusLabel:pos(OrderItem.WIDTH * 0.88, (OrderItem.HEIGHT + ROW_GAP) * 0.5)
  self._statusLabel:addTo(self)
  self._timeLabel = ui.newTTFLabel({
    text = "",
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_RIGHT
  })
  self._timeLabel:setAnchorPoint(ccp(0, 0.5))
  self._timeLabel:pos(OrderItem.WIDTH * 0.35, (OrderItem.HEIGHT + ROW_GAP) * 0.5)
  self._timeLabel:addTo(self)
end
function OrderItem:onDataSet(isModified, data)
  if isModified then
    local text
    if self._productPic then
      self._productPic:removeFromParent()
      self._productPic = nil
    end
    self._productPic = display.newSprite(data.pic)
    if self._indexLabel then
      self._indexLabel:setString(self._index)
    end
    if self._productPic then
      self._productPic:pos(88, (OrderItem.HEIGHT + ROW_GAP) * 0.5)
      self._productPic:addTo(self)
    end
    if self._nameLabel then
      self._nameLabel:setString(g.util.functions.formatBigInt(tonumber(data.chips) + tonumber(data.chipsMore)))
    end
    if self._priceLabel then
      text = string.gsub(g.lang:getContent("payment.priceFormat"), "{pay}", tonumber(data.pay / 100))
      self._priceLabel:setString(text)
    end
    if self._statusLabel then
      self._statusLabel:setString(g.lang:getContent("payment.orderStatusLabel")[tonumber(data.status) + 1])
    end
    if self._timeLabel then
      text = os.date("%m/%d/%Y %H:%M:%S", tonumber(data.payTime) / 1000)
      self._timeLabel:setString(text)
    end
  end
end
return OrderItem

local ChipsItem = class("ChipsItem", g.ui.ListItem)
ChipsItem.WIDTH = 0
ChipsItem.HEIGHT = 0
ChipsItem.PADDING_LEFT = 10
ChipsItem.PADDING_RIGHT = 2
local ROW_GAP = 2
function ChipsItem:ctor()
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
  ChipsItem.super.ctor(self, ChipsItem.WIDTH, ChipsItem.HEIGHT + ROW_GAP)
  self._bgX = 0
  self._bgY = (ChipsItem.HEIGHT + ROW_GAP) * 0.5
  self._background = display.newScale9Sprite("#pay_item_bg.png", self._bgX, self._bgY, cc.size(ChipsItem.WIDTH - ChipsItem.PADDING_LEFT - ChipsItem.PADDING_RIGHT, ChipsItem.HEIGHT)):addTo(self)
  self._hot = display.newSprite("#common_hot_01.png"):pos(ChipsItem.PADDING_LEFT + 28, ChipsItem.HEIGHT + ROW_GAP * 0.5 - 28):addTo(self, 1)
  self._new = display.newSprite("#common_new_01.png"):pos(ChipsItem.PADDING_LEFT + 28, ChipsItem.HEIGHT + ROW_GAP * 0.5 - 28):addTo(self, 1):hide()
  self._discountBg = display.newSprite("#pay_discount_bg.png"):pos(self._bgX - ChipsItem.WIDTH * 0.5 + 43, self._bgY + ChipsItem.HEIGHT * 0.5 - 40):hide():addTo(self)
  self._discountLabel = ui.newTTFLabel({
    text = "",
    size = 18,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT
  }):rotation(-45):pos(28, 56):hide():addTo(self._discountBg)
  self._priceLabel = ui.newTTFLabel({
    text = "",
    size = 20,
    color = ccc3(39, 144, 213),
    align = ui.TEXT_ALIGN_LEFT
  })
  self._priceLabel:pos(self._bgX, self._bgY - 75)
  self._priceLabel:addTo(self)
  self._buyButton = cc.ui.UIPushButton.new({
    normal = "#common_flat_green_btn_up.png",
    pressed = "#common_flat_green_btn_down.png"
  }, {scale9 = true})
  self._buyButton:setButtonLabel(ui.newTTFLabel({
    text = "",
    size = 28,
    color = ccc3(255, 255, 255),
    align = ui.TEXT_ALIGN_CENTER
  }))
  self._buyButton:onButtonClicked(function(evt)
    local now = g.util.functions.getTime()
    if not ChipsItem.buyButtonLastClickTime or math.abs(now - ChipsItem.buyButtonLastClickTime) > 2 then
      ChipsItem.buyButtonLastClickTime = now
      g.audio:playSound("CLICK_BUTTON")
      self:dispatchEvent({
        name = "ITEM_EVENT",
        type = "PURCHASE",
        data = self._data
      })
    end
  end)
  self._buyButton:setButtonSize(ChipsItem.WIDTH, 56)
  self._buyButton:pos(self._bgX, self._bgY - 30)
  self._buyButton:addTo(self)
  self._chipsBg = display.newScale9Sprite("#pay_chips_bg.png", self._bgX + 2, self._bgY - 34, cc.size(ChipsItem.WIDTH, 60)):hide():addTo(self)
  self._chipsLabel = ui.newTTFLabel({
    text = "",
    size = 28,
    color = ccc3(255, 255, 255),
    align = ui.TEXT_ALIGN_CENTER
  }):pos(self._bgX, self._bgY - 32):hide():addTo(self)
end
function ChipsItem:onDataSet(isModified, data)
  if isModified then
    local text
    if data.discount ~= 1 then
      self._hot:hide()
      self._new:hide()
    elseif data.tag == "hot" then
      self._hot:show()
      self._new:hide()
    elseif data.tag == "new" then
      self._hot:hide()
      self._new:show()
    else
      self._hot:hide()
      self._new:hide()
    end
    self._priceLabel:setString(data.priceLabel or "")
    if g.vars.user.firstPayEnabled and g.vars.user.firstPayEnabled == 1 then
      data.chipsMore = 1000
    end
    if data.chipsMore and tonumber(data.chipsMore) > 0 and (data.paymentId == 1 or data.paymentId == 10 or data.paymentId == 13 or data.paymentId == 19) then
      self._discountBg:show()
      text = string.gsub(g.lang:getContent("payment.promotionLabel"), "{more}", data.chipsMore)
      self._discountLabel:setString(text)
      self._discountLabel:show()
    end
    if self._productPic then
      self._productPic:removeFromParent()
      self._productPic = nil
    end
    self._productPic = display.newSprite(data.pic)
    if self._productPic then
      self._productPic:pos(self._bgX, self._bgY + 50)
      self._productPic:addTo(self)
    end
    text = string.gsub(g.lang:getContent("payment.chipsFormat"), "{chips}", g.util.functions.formatBigInt(data.chips))
    self._buyButton:setButtonLabelString(text)
    self._chipsLabel:setString(text)
    if data.buyButtonEnabled ~= false then
      self._buyButton:show()
    else
      self._buyButton:hide()
      self._chipsBg:show()
      self._chipsLabel:show()
    end
  end
end
return ChipsItem

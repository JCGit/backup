local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local GiftItem = class("GiftItem", function()
  return display.newNode()
end)
local giftGroup = {}
local HEIGHT = 120
function GiftItem:ctor()
  self:setNodeEventEnabled(true)
  local posY = HEIGHT * 0.5
  self._hotIcon = display.newSprite("#common_hot_01.png"):pos(40, posY + 22):addTo(self):hide()
  self._newIcon = display.newSprite("#common_new_01.png"):pos(40, posY + 22):addTo(self):hide()
  self.checkButton = cc.ui.UICheckBoxButton.new({
    off = "#common_standard_greybg_07.png",
    on = "#common_standard_bluebg_07.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    text = "",
    size = 24,
    color = ccc3(178, 220, 255),
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabelOffset(0, -36):setButtonLabelAlignment(display.CENTER):pos(80, posY):onButtonStateChanged(handler(self, self.onChange)):setButtonSize(131, 102):addTo(self):hide()
  self.checkButton:setTouchSwallowEnabled(false)
  self._priceBg = display.newScale9Sprite("#pd_gift_price_bg.png", 80, 25, CCSize(130, 34)):addTo(self)
  self._image = g.ui.UIImageView.new():pos(80, posY + 17):addTo(self):hide()
end
function GiftItem:setData(value)
  if self._data ~= value then
    self._data = value
    if value.pic then
      self._image:setImage(value.pic, {
        size = cc.size(60, 60)
      })
      self._image:show()
    end
    if value.chips then
      self._priceNode = display.newNode():addTo(self)
      self._chipsIcon = display.newSprite("#mainlobby_white_chips_icon.png"):pos(0, 0):align(display.LEFT_CENTER):addTo(self._priceNode)
      self._priceLabel = ui.newTTFLabel({
        text = string.formatnumberthousands(value.chips),
        color = g.font.color.LIGHT,
        size = 26,
        align = ui.TEXT_ALIGN_CENTER
      }):align(display.LEFT_CENTER):pos(self._chipsIcon:getContentSize().width + 5, 0):addTo(self._priceNode)
      self._priceNode:pos(-(self._chipsIcon:getContentSize().width + 5 + self._priceLabel:getContentSize().width) * 0.5 + 80, 25)
      if value.mark then
        if value.mark == "1" then
          self._hotIcon:show()
        elseif value.mark == "2" then
          self._newIcon:show()
        else
          self._hotIcon:hide()
          self._newIcon:hide()
        end
      end
    else
      local t = tonumber(value.expire) - g.util.functions.getTime()
      self._priceLabel = ui.newTTFLabel({
        text = "",
        color = g.font.color.LIGHT,
        size = 20,
        align = ui.TEXT_ALIGN_CENTER
      }):pos(80, 25):addTo(self)
      if t > 0 then
        local day = string.gsub(g.lang:getContent("shop.leftExpire"), "{day}", math.ceil(t / 86400))
        self._priceLabel:setString(day)
      else
        self._priceLabel:setString(g.lang:getContent("shop.expired"))
      end
    end
    if value.giftId then
      self.checkButton.giftId = value.giftId
    end
    if value.positionId then
      self.checkButton.positionId = value.positionId
    end
  end
  return self
end
function GiftItem:onChange(evt)
  if evt.target:isButtonSelected() then
    g.vars.shop.currentGiftId = self._data.giftId
  end
end
return GiftItem

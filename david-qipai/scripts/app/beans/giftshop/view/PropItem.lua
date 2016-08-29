local PropItem = class("PropItem", function()
  return display.newNode()
end)
local WIDTH = 193
local HEIGHT = 221
function PropItem:ctor()
  self.checkButton = cc.ui.UICheckBoxButton.new({
    off = "#common_standard_bluebg_08.png",
    on = "#common_standard_bluebg_08.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    text = "",
    size = 24,
    color = ccc3(178, 220, 255),
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabelOffset(0, -36):setButtonLabelAlignment(display.CENTER):pos(0, HEIGHT * 0.5 - 7):onButtonStateChanged(handler(self, self.onChange)):setButtonSize(193, 221):addTo(self):hide()
  self.checkButton:setTouchSwallowEnabled(false)
end
function PropItem:setData(value)
  if self._data ~= value then
    local posY = HEIGHT * 0.5 - 7
    if self._background == nil then
      self._background = display.newScale9Sprite("#common_standard_bluebg_07.png", 0, 0, CCSize(WIDTH, HEIGHT)):pos(0, posY):addTo(self):hide()
    end
    if self._image == nil then
      self._image = display.newSprite(value.pic):pos(0, posY + 36):addTo(self)
      self._image:scale(100 / self._image:getContentSize().width)
    end
    if self._nameBg == nil then
      self._nameBg = display.newScale9Sprite("#common_standard_greybg_05.png", 0, 0, CCSize(193, 32)):pos(0, posY - 40):addTo(self)
    end
    if self._nameLabel == nil then
      self._nameLabel = ui.newTTFLabel({
        text = value.name,
        color = g.font.color.LIGHT,
        size = 24,
        align = ui.TEXT_ALIGN_CENTER
      }):pos(0, posY - 40):addTo(self)
    end
    if value.chips == nil then
      if self._button == nil then
        self._button = cc.ui.UIPushButton.new({
          normal = "#common_green_btn_up.png",
          pressed = "#common_green_btn_down.png"
        }, {scale9 = true}):setButtonSize(185, 45):setButtonLabel("normal", ui.newTTFLabel({
          text = value.buttonText,
          color = ccc3(50, 88, 0),
          size = 26,
          align = ui.TEXT_ALIGN_CENTER
        })):pos(0, posY - 82):onButtonClicked(clickHandler(self, self._onButtonClick)):addTo(self)
      end
      self.checkButton:show()
    elseif self._priceNode == nil then
      self._priceNode = display.newNode():addTo(self)
      self._chipsIcon = display.newSprite("#mainlobby_chips_icon.png"):pos(0, 0):align(display.LEFT_CENTER):addTo(self._priceNode)
      self._priceLabel = ui.newTTFLabel({
        text = string.formatnumberthousands(value.chips),
        color = g.font.color.GOLDEN,
        size = 24,
        align = ui.TEXT_ALIGN_CENTER
      }):align(display.LEFT_CENTER):pos(self._chipsIcon:getContentSize().width + 5, 0):addTo(self._priceNode)
      self._priceNode:pos(-(self._chipsIcon:getContentSize().width + 5 + self._priceLabel:getContentSize().width) * 0.5, posY - 82)
    end
    self._data = value
  end
end
function PropItem:setName(value)
  self._nameLabel:setString(value)
end
function PropItem:_onButtonClick(evt)
  if self._data ~= nil and self._data.callback ~= nil then
    self._data.callback()
  end
end
function PropItem:onChange(evt)
  if self._data.chips ~= nil then
    if evt.target:isButtonSelected() then
      g.vars.shop.currentPropId = self._data.propId
      if self._background then
        self._background:show()
      end
    elseif self._background then
      self._background:hide()
    end
  end
end
return PropItem

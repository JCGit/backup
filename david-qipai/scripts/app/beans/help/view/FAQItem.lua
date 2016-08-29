local FAQItem = class("FAQItem", g.ui.ListItem)
FAQItem.WIDTH = 0
FAQItem.HEIGHT = 64
FAQItem.ANSWER_SIZE = 20
FAQItem.ANSWER_COLOR = ccc3(96, 168, 255)
function FAQItem:ctor()
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
  FAQItem.super.ctor(self, FAQItem.WIDTH, FAQItem.HEIGHT)
  self.contentWidth = FAQItem.WIDTH - 24
  local contentH = FAQItem.HEIGHT
  self._isFold = true
  self._touchButton = cc.ui.UIPushButton.new("#common_transparent_skin.png", {scale9 = true})
  self._touchButton:setTouchSwallowEnabled(false)
  self._touchButton:onButtonPressed(function(evt)
    self._buttonPressedY = evt.y
    self._isButtonClickCanceled = false
  end)
  self._touchButton:onButtonRelease(function(evt)
    if math.abs(evt.y - self._buttonPressedY) > 10 then
      self._isButtonClickCanceled = true
    end
  end)
  self._touchButton:onButtonClicked(clickHandler(self, function(self, evt)
    if not self._isButtonClickCanceled and self:getParent():getCascadeBoundingBox():containsPoint(ccp(evt.x, evt.y)) then
      self:foldContent()
    end
  end))
  self._touchButton:setButtonSize(FAQItem.WIDTH, FAQItem.HEIGHT)
  self._touchButton:pos(FAQItem.WIDTH * 0.5, FAQItem.HEIGHT * 0.5)
  self._touchButton:addTo(self)
  self._contentContainer = display.newNode():addTo(self):pos(FAQItem.WIDTH * 0.5, FAQItem.HEIGHT * 0.5)
  self._increaseHeight = 0
  self._bottomPanel = cc.ClippingNode:create():addTo(self._contentContainer)
  self._bottomPanel:setContentSize(cc.size(self.contentWidth, 1))
  self._stencil = display.newRect(FAQItem.WIDTH - 8, FAQItem.HEIGHT * 0.5, {
    fill = true,
    color = ccc4f(1, 1, 1, 1)
  })
  self._stencil:setAnchorPoint(ccp(0.5, 0))
  self._stencil:pos(-1, -FAQItem.HEIGHT * 0.5)
  self._bottomPanel:setStencil(self._stencil)
  self._topPanel = display.newNode():addTo(self._contentContainer)
  self.bottomHeight = 60
  self._bottomBg = display.newScale9Sprite("#common_standard_greybg_08.png"):addTo(self._bottomPanel):size(self.contentWidth - 8, self.bottomHeight):pos(-1, 0):hide()
  self._topBg = display.newScale9Sprite("#common_standard_greybg_03.png")
  self._topBg:setContentSize(CCSize(self.contentWidth, FAQItem.HEIGHT))
  self._topBg:addTo(self._topPanel)
  self._topBg:pos(-1, 0)
  local foldIconMarginLeft = 28
  self._foldIcon = cc.ui.UIPushButton.new({
    normal = "#common_small_blue_arrow_up.png",
    pressed = "#common_small_blue_arrow_down.png"
  }, {scale9 = false}):pos(FAQItem.WIDTH * 0.5 - foldIconMarginLeft - 10, 0):addTo(self._topPanel)
  self.titleMarginLeft = foldIconMarginLeft + self._foldIcon:getContentSize().width + 12
  self._titleLabel = ui.newTTFLabel({
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT
  }):pos(FAQItem.WIDTH * -0.5 + self.titleMarginLeft, 0):addTo(self._topPanel)
  self._titleLabel:setAnchorPoint(ccp(0, 0.5))
  local answerLabelPadding = 60
  local params = {
    size = FAQItem.ANSWER_SIZE,
    color = FAQItem.ANSWER_COLOR,
    align = ui.TEXT_ALIGN_LEFT,
    valign = ui.TEXT_VALIGN_TOP,
    dimensions = cc.size(self.contentWidth - answerLabelPadding, 0)
  }
  self.answerLabel = ui.newTTFLabel(params)
  self.answerLabel:addTo(self._bottomPanel)
  self.answerLabel:hide()
  self.answerLabel:setAnchorPoint(ccp(0, 0.5))
  self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self._onEnterFrame))
end
function FAQItem:_onTouch(evt)
  local name, x, y, prevX, prevY = evt.name, evt.x, evt.y, evt.prevX, evt.prevY
  local isTouchInSprite = self:getCascadeBoundingBox():containsPoint(CCPoint(x, y))
  if name == "began" then
    if isTouchInSprite then
      self._beginY = evt.y
      self._isClickCanceled = false
      self._isTouching = true
      return true
    else
      return false
    end
  elseif not self._isTouching then
    return false
  elseif name == "moved" then
    if math.abs(evt.y - self._beginY) > 10 then
      self._isClickCanceled = true
    end
  elseif name == "ended" or name == "cancelled" then
    self._isTouching = false
    if not self._isClickCanceled and isTouchInSprite and self:getParent():getParent():getCascadeBoundingBox():containsPoint(ccp(evt.x, evt.y)) then
      self:foldContent()
    end
  end
  return true
end
function FAQItem:_onEnterFrame()
  local bottomHeight = self._bottomBg:getContentSize().height
  local dest, direction
  if self._isFold then
    dest = 0
    direction = -1
  else
    dest = bottomHeight
    direction = 1
  end
  if self._increaseHeight == dest then
    self:unscheduleUpdate()
  else
    self._increaseHeight = self._increaseHeight + direction * math.max(1, math.abs(self._increaseHeight - dest) * 0.08)
    if direction > 0 and dest < self._increaseHeight or direction < 0 and dest > self._increaseHeight then
      self._increaseHeight = dest
    end
  end
  self._foldIcon:rotation(90 * (self._increaseHeight / bottomHeight))
  local contentHeight = FAQItem.HEIGHT + self._increaseHeight
  self._bottomPanel:setPositionY(-self._increaseHeight - 12)
  self:setContentSize(cc.size(FAQItem.WIDTH, FAQItem.HEIGHT + self._increaseHeight))
  self._contentContainer:setPositionY(FAQItem.HEIGHT * 0.5 + self._increaseHeight)
  self._touchButton:setButtonSize(FAQItem.WIDTH, FAQItem.HEIGHT + self._increaseHeight)
  self._touchButton:setPositionY(contentHeight * 0.5)
  self._stencil:setScaleY((FAQItem.HEIGHT * 0.2 + self._increaseHeight) / (FAQItem.HEIGHT * 0.5))
  self:dispatchEvent({name = "RESIZE"})
end
function FAQItem:foldContent()
  if self._isFold then
    self._isFold = false
  else
    self._isFold = true
  end
  self:unscheduleUpdate()
  self:scheduleUpdate()
  if not self.isInitialized then
    self._bottomBg:show()
    self.isInitialized = true
    self:createItem(self._data)
  end
end
function FAQItem:onDataSet(isModified, data)
  if isModified then
    self._data = data
    self._titleLabel:setString(self._data.question)
  end
end
local answerLabelMarginBottom = 17
local linePadding = 26
function FAQItem:createItem(data)
  self.answerLabel:show()
  for i, element in ipairs(self._data.answer) do
    if element.type == "text" then
      self.answerLabel:setString(element.content)
    elseif element.type == "image" then
      local iconImage = display.newSprite(element.content)
      local contentSize = iconImage:getContentSize()
      iconImage:scale(35 / contentSize.width):pos(element.x, element.y):addTo(self._bottomPanel)
    end
  end
  local answerLabelMarginLeft = self.titleMarginLeft
  local answerLabelSize = self.answerLabel:getContentSize()
  local h = linePadding + answerLabelSize.height
  self._bottomBg:size(self.contentWidth - 8, h)
  self._bottomBg:setPositionY(h * 0.5 - FAQItem.HEIGHT * 0.5 + 16)
  self.answerLabel:setPosition(ccp(-FAQItem.WIDTH / 2 + answerLabelMarginLeft, h * 0.5 - FAQItem.HEIGHT * 0.5 + answerLabelMarginBottom))
end
return FAQItem

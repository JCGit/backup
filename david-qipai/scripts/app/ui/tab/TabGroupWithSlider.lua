local TabGroupWithSlider = class("TabGroupWithSlider", function()
  return display.newNode()
end)
TabGroupWithSlider.HORIZONTAL = 1
TabGroupWithSlider.VERTICAL = 2
local subTabSelectedColor = ccc3(255, 255, 255)
local subTabDefaultColor = ccc3(202, 202, 202)
local subTabSelectedSize = 22
local subTabDefaultSize = 22
function TabGroupWithSlider:ctor(images, texts, textConfigs, scale9, dragable, direction)
  self._textConfigs = {
    selectedText = {color = subTabSelectedColor, size = subTabSelectedSize},
    defaltText = {color = subTabDefaultColor, size = subTabDefaultSize}
  }
  self._scale9 = scale9 or false
  self._dragable = dragable or false
  self._direction = direction or TabGroupWithSlider.HORIZONTAL
  if self._scale9 then
    if type(images.background) == "string" then
      self._background = display.newScale9Sprite(images.background):addTo(self)
    else
      self._background = images.background:addTo(self)
    end
    self._slider = display.newScale9Sprite(images.slider):addTo(self)
  else
    if type(images.background) == "string" then
      self._background = display.newSprite(images.background):addTo(self)
    else
      self._background = images.background:addTo(self)
    end
    self._slider = display.newSprite(images.slider):addTo(self)
  end
  if self._dragable then
    self._slider:setTouchEnabled(true)
    self._slider:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onSliderTouch))
  end
  self._subButtons = {}
  self._labels = {}
  self._dividers = {}
  self._buttonCount = #texts
  local buttonSize
  if self._direction == TabGroupWithSlider.HORIZONTAL then
    buttonSize = CCSize(self._background:getContentSize().width / self._buttonCount, self._background:getContentSize().height)
  else
    buttonSize = CCSize(self._background:getContentSize().width, self._background:getContentSize().height / self._buttonCount)
  end
  for i = 1, self._buttonCount do
    self._subButtons[i] = cc.ui.UIPushButton.new({
      normal = "#common_transparent_skin.png"
    }, {scale9 = true}):setButtonSize(buttonSize.width, buttonSize.height):addTo(self):onButtonClicked(clickHandler(self, self._onButtonClick))
    self._subButtons[i]:setTouchSwallowEnabled(false)
    if self._direction == TabGroupWithSlider.HORIZONTAL then
      self._subButtons[i]:pos((i - 0.5 - self._buttonCount * 0.5) * buttonSize.width, 0)
    else
      self._subButtons[i]:pos(0, (self._buttonCount * 0.5 - i + 0.5) * buttonSize.height)
    end
    self._labels[i] = ui.newTTFLabel({
      text = texts[i],
      color = self._textConfigs.defaltText.color,
      size = self._textConfigs.defaltText.size,
      align = ui.TEXT_ALIGN_CENTER
    }):addTo(self)
    if self._direction == TabGroupWithSlider.HORIZONTAL then
      self._labels[i]:pos((i - 0.5 - self._buttonCount * 0.5) * buttonSize.width, 0)
    else
      self._labels[i]:pos(0, (self._buttonCount * 0.5 - i + 0.5) * buttonSize.height)
    end
  end
  self._selectedTab = 1
end
function TabGroupWithSlider:_onSliderTouch(evt)
  local name, currentX, currentY = evt.name, evt.x, evt.y
  if name == "began" then
    self._originalPosY = currentY
    self._originalPosX = currentX
    return true
  elseif name == "ended" then
    if self._direction == TabGroupWithSlider.HORIZONTAL then
      if currentX - self._originalPosX > 20 and self._selectedTab + 1 <= self._buttonCount then
        self:tabOn(self._selectedTab + 1, true)
      elseif self._originalPosX - currentX > 20 and 1 <= self._selectedTab - 1 then
        self:tabOn(self._selectedTab - 1, true)
      end
    elseif self._originalPosY - currentY > 20 and self._selectedTab + 1 <= self._buttonCount then
      self:tabOn(self._selectedTab + 1, true)
    elseif currentY - self._originalPosY > 20 and 1 <= self._selectedTab - 1 then
      self:tabOn(self._selectedTab - 1, true)
    end
  end
end
function TabGroupWithSlider:setTabGroupSize(width, height, offsetWidth, offsetHeight)
  assert(self._scale9, "TabGroupWithSlider:setTabGroupSize() - can't change size for non-scale9 tab bar")
  self._background:setContentSize(CCSize(width, height))
  local buttonSize
  if self._direction == TabGroupWithSlider.HORIZONTAL then
    self._slider:setContentSize(CCSize(width / self._buttonCount + (offsetWidth or 0), height + (offsetHeight or 0)))
    buttonSize = CCSize(self._background:getContentSize().width / self._buttonCount, self._background:getContentSize().height)
    self._slider:setPositionX((0.5 - self._buttonCount * 0.5) * buttonSize.width)
    self._labels[1]:setColor(self._textConfigs.selectedText.color)
  else
    self._slider:setContentSize(CCSize(width + (offsetWidth or 0), height / self._buttonCount + (offsetHeight or 0)))
    buttonSize = CCSize(self._background:getContentSize().width, self._background:getContentSize().height / self._buttonCount)
    self._slider:setPositionY((self._buttonCount * 0.5 - 1 + 0.5) * buttonSize.height)
    self._labels[1]:setColor(self._textConfigs.selectedText.color)
  end
  for i = 1, self._buttonCount do
    self._subButtons[i]:setButtonSize(buttonSize.width, buttonSize.height)
    if self._direction == TabGroupWithSlider.HORIZONTAL then
      self._subButtons[i]:pos((i - 0.5 - self._buttonCount * 0.5) * buttonSize.width, 0)
      self._labels[i]:pos((i - 0.5 - self._buttonCount * 0.5) * buttonSize.width, 0)
    else
      self._subButtons[i]:pos(0, (self._buttonCount * 0.5 - i + 0.5) * buttonSize.height)
      self._labels[i]:pos(0, (self._buttonCount * 0.5 - i + 0.5) * buttonSize.height)
    end
  end
  return self
end
function TabGroupWithSlider:tabOn(tab, isAnimation)
  self._labels[self._selectedTab]:setColor(self._textConfigs.defaltText.color)
  if tab >= 1 and tab <= self._buttonCount then
    if not isAnimation then
      self._selectedTab = tab
      self._labels[self._selectedTab]:setColor(self._textConfigs.selectedText.color)
      self._slider:stopAllActions()
      if self._direction == TabGroupWithSlider.HORIZONTAL then
        self._slider:setPositionX((tab - 0.5 - self._buttonCount * 0.5) * self._background:getContentSize().width / self._buttonCount)
      else
        self._slider:setPositionY((self._buttonCount * 0.5 - tab + 0.5) * self._background:getContentSize().height / self._buttonCount)
      end
      if self._callback then
        self._callback(self._selectedTab)
      end
    else
      local moveToParams = {
        time = 0.2,
        onComplete = handler(self, function(obj)
          obj._selectedTab = tab
          obj._labels[obj._selectedTab]:setColor(self._textConfigs.selectedText.color)
          if obj._callback then
            obj._callback(obj._selectedTab)
          end
        end)
      }
      if self._direction == TabGroupWithSlider.HORIZONTAL then
        moveToParams.x = (tab - 0.5 - self._buttonCount * 0.5) * self._background:getContentSize().width / self._buttonCount
      else
        moveToParams.y = (self._buttonCount * 0.5 - tab + 0.5) * self._background:getContentSize().height / self._buttonCount
      end
      self._slider:stopAllActions()
      transition.moveTo(self._slider, moveToParams)
    end
  end
  return self
end
function TabGroupWithSlider:_onButtonClick(event)
  local buttonId = table.keyof(self._subButtons, event.target) + 0
  if buttonId ~= self._selectedTab and buttonId >= 1 and buttonId <= self._buttonCount then
    self:tabOn(buttonId, true)
  end
end
function TabGroupWithSlider:bindTabClickHandler(callback)
  assert(type(callback) == "function", "callback should be a function")
  self._callback = callback
  return self
end
return TabGroupWithSlider

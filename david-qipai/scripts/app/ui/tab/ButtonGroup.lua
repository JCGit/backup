local ButtonGroup = class("ButtonGroup", function()
  return display.newNode()
end)
ButtonGroup.TAB_GROUP_HEIGHT = 64
local WHITE = ccc3(255, 255, 255)
local CHECKED_COLOR = ccc3(39, 144, 213)
local UNCHECKED_COLOR = ccc3(202, 202, 202)
function ButtonGroup:ctor(params)
  self._bgWidth = params.windowWidth * (params.scale or 0.85)
  self._iconTexture = params.iconTexture
  local yOffset = params.yOffset or -5
  local container = display.newNode():addTo(self):pos(0, yOffset)
  display.newScale9Sprite("#popup_tab_bar_bg.png", 0, 0, CCSize(self._bgWidth, ButtonGroup.TAB_GROUP_HEIGHT)):addTo(container)
  display.newScale9Sprite("#popup_tab_bar_unselected_bg.png", 0, 0, CCSize(self._bgWidth - 8, ButtonGroup.TAB_GROUP_HEIGHT - 4)):addTo(container)
  self._checkedBg = display.newScale9Sprite("#popup_tab_bar_left_selected_bg.png"):pos(-self._bgWidth * 0.25 + 2, 1):size(self._bgWidth * 0.5 - 4, 56):addTo(container)
  self._subButtons = {}
  self._buttonIconBgs = {}
  self._buttonTexts = params.buttonTexts
  for i = 1, #params.buttonTexts do
    self._subButtons[i] = cc.ui.UIPushButton.new({
      normal = params.iconTexture[i][1]
    }, {scale9 = true}):setButtonSize(self._bgWidth / #params.buttonTexts, ButtonGroup.TAB_GROUP_HEIGHT):setButtonLabel("normal", ui.newTTFLabel({
      text = self._buttonTexts[i],
      color = UNCHECKED_COLOR,
      size = 28,
      align = ui.TEXT_ALIGN_CENTER
    })):setButtonLabel("pressed", ui.newTTFLabel({
      text = self._buttonTexts[i],
      color = CHECKED_COLOR,
      size = 28,
      align = ui.TEXT_ALIGN_CENTER
    })):setButtonLabel("disabled", ui.newTTFLabel({
      text = self._buttonTexts[i],
      color = UNCHECKED_COLOR,
      size = 28,
      align = ui.TEXT_ALIGN_CENTER
    })):setButtonImage("normal", params.iconTexture[i][2]):setButtonImage("pressed", params.iconTexture[i][1]):setButtonImage("disabled", params.iconTexture[i][3]):pos(self._bgWidth * -0.5 + (i - 0.5) * (self._bgWidth / #params.buttonTexts), 1):addTo(container):onButtonClicked(clickHandler(self, self._onButtonClick))
    if i > 1 then
      display.newSprite("#popup_tab_bar_divider.png"):addTo(container):setPositionX(self._bgWidth * -0.5 + (i - 1) * (self._bgWidth / #params.buttonTexts))
    end
  end
  self._checkedTab = 1
  self:tabOn(self._checkedTab)
end
function ButtonGroup:_onButtonClick(evt)
  local buttonId = table.keyof(self._subButtons, evt.target) + 0
  if buttonId ~= self._checkedTab then
    self:tabOn(buttonId)
  end
end
function ButtonGroup:tabOn(buttonId)
  local padding = 0
  for i, v in ipairs(self._subButtons) do
    local button = self._subButtons[i]
    local label = button:getButtonLabel()
    if i == buttonId then
      label:setColor(CHECKED_COLOR)
      button:setButtonImage("normal", self._iconTexture[i][1])
      button:setButtonImage("pressed", self._iconTexture[i][1])
      button:setButtonImage("disabled", self._iconTexture[i][3])
    else
      label:setColor(UNCHECKED_COLOR)
      button:setButtonImage("normal", self._iconTexture[i][2])
      button:setButtonImage("pressed", self._iconTexture[i][1])
      button:setButtonImage("disabled", self._iconTexture[i][3])
    end
  end
  self._checkedTab = buttonId
  if self._callback then
    self._callback(self._checkedTab)
  end
end
function ButtonGroup:bindTabClickHandler(callback)
  assert(type(callback) == "function", "callback should be a function")
  self._callback = callback
  return self
end
function ButtonGroup:setButtonTitles(titles)
  for i, button in ipairs(self._subButtons) do
    button:setButtonLabelString("normal", titles[i])
    button:setButtonLabelString("pressed", titles[i])
    button:setButtonLabelString("disabled", titles[i])
  end
end
function ButtonGroup:setButtonDisable(i)
  local button = self._subButtons[i]
  button:setButtonEnabled(false)
end
return ButtonGroup

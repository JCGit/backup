local TabGroup = class("TabGroup", function()
  return display.newNode()
end)
TabGroup.TAB_GROUP_HEIGHT = 54
TabGroup.TAB_GROUP_BTN_HEIGHT = TabGroup.TAB_GROUP_HEIGHT - 8
local WHITE = ccc3(255, 255, 255)
local CHECKED_COLOR = ccc3(39, 144, 213)
local UNCHECKED_COLOR = ccc3(139, 184, 220)
function TabGroup:ctor(params)
  self._bgWidth = params.windowWidth * (params.scale or 0.85)
  self._bgHeight = params.windowHeight or TabGroup.TAB_GROUP_HEIGHT
  self._tabBtnSize = params.tabBtnSize
  self._iconTexture = params.iconTexture
  self._itemStateTexture = params.itemStateTexture
  local yOffset = params.yOffset or -5
  local container = display.newNode():addTo(self):pos(0, yOffset)
  if params.tabBG then
    display.newScale9Sprite(params.tabBG, 0, 0, CCSize(self._bgWidth, self._bgHeight)):addTo(container)
  else
    display.newScale9Sprite("#common_standard_greybg_04.png", 0, 0, CCSize(self._bgWidth, self._bgHeight)):addTo(container)
  end
  self._checkedBg = display.newScale9Sprite("#common_tab_bar_left_selected_bg.png"):pos(-self._bgWidth * 0.25 + 2, 1):size(self._bgWidth * 0.5 - 4, TabGroup.TAB_GROUP_BTN_HEIGHT):addTo(container)
  self._unselectedBg = {}
  self._subButtons = {}
  self._buttonIcons = {}
  self._buttonIconBgs = {}
  self._buttonTexts = params.buttonTexts
  for i = 1, #params.buttonTexts do
    self._unselectedBg[i] = display.newScale9Sprite("#common_tab_bar_icon_right_unselected.png"):size(self._bgWidth * 0.5 - 4, TabGroup.TAB_GROUP_BTN_HEIGHT):addTo(container)
    if params.iconTexture then
      self._buttonIcons[i] = display.newSprite(params.iconTexture[i][1]):pos(params.iconOffsetX, 0)
    end
    self._subButtons[i] = cc.ui.UIPushButton.new({
      normal = "#common_transparent_skin.png"
    }, {scale9 = true}):setButtonLabel("normal", ui.newTTFLabel({
      text = self._buttonTexts[i],
      color = CHECKED_COLOR,
      size = 28,
      align = ui.TEXT_ALIGN_CENTER
    })):pos(self._bgWidth * -0.5 + (i - 0.5) * (self._bgWidth / #params.buttonTexts), 1):addTo(container):onButtonClicked(clickHandler(self, self._onButtonClick))
    if self._tabBtnSize then
      self._subButtons[i]:setButtonSize(self._tabBtnSize.width, self._tabBtnSize.height)
    else
      self._subButtons[i]:setButtonSize(self._bgWidth / #params.buttonTexts, self._bgHeight)
    end
    if params.iconTexture then
      self._subButtons[i]:setButtonLabelOffset(self._buttonIcons[i]:getContentSize().width * 0.5, 0):add(self._buttonIcons[i])
    end
    if params.iconTexture then
      self._buttonIcons[i]:setPositionX(-0.5 * self._subButtons[i]:getButtonLabel("normal"):getContentSize().width - params.iconOffsetX)
    end
    if i > 1 then
    end
  end
  self._checkedTab = 1
  self:tabOn(self._checkedTab)
end
function TabGroup:_onButtonClick(evt)
  local buttonId = table.keyof(self._subButtons, evt.target) + 0
  if buttonId ~= self._checkedTab then
    self:tabOn(buttonId)
  end
end
function TabGroup:tabOn(buttonId)
  local padding = 0
  for i, v in ipairs(self._subButtons) do
    local button = self._subButtons[i]
    local icon = self._buttonIcons[i]
    local iconBg = self._buttonIconBgs[i]
    local label = button:getButtonLabel()
    local unSelectedBg = self._unselectedBg[i]
    if i == buttonId then
      label:setColor(WHITE)
      unSelectedBg:hide()
      if icon then
        icon:setDisplayFrame(display.newSpriteFrame(string.gsub(self._iconTexture[i][1], "#", "")))
      end
      if i == 1 then
        if self._itemStateTexture and self._itemStateTexture[i] then
          self._checkedBg:setSpriteFrame(display.newSpriteFrame(self._itemStateTexture[i][1]))
        else
          self._checkedBg:setSpriteFrame(display.newSpriteFrame("common_tab_bar_left_selected_bg.png"))
        end
        self._checkedBg:pos(self._bgWidth * -0.5 + (buttonId - 0.5) * self._bgWidth / #self._subButtons + 2, 1)
      elseif i == #self._subButtons then
        if self._itemStateTexture and self._itemStateTexture[i] then
          self._checkedBg:setSpriteFrame(display.newSpriteFrame(self._itemStateTexture[i][1]))
        else
          self._checkedBg:setSpriteFrame(display.newSpriteFrame("common_tab_bar_right_selected_bg.png"))
        end
        self._checkedBg:pos(self._bgWidth * -0.5 + (buttonId - 0.5) * self._bgWidth / #self._subButtons - 4, 1)
      else
        if self._itemStateTexture and self._itemStateTexture[i] then
          self._checkedBg:setSpriteFrame(display.newSpriteFrame(self._itemStateTexture[i][1]))
        else
          self._checkedBg:setSpriteFrame(display.newSpriteFrame("common_tab_bar_middle_selected_bg.png"))
        end
        self._checkedBg:pos(self._bgWidth * -0.5 + (buttonId - 0.5) * self._bgWidth / #self._subButtons, 1)
      end
      if self._tabBtnSize then
        self._checkedBg:setContentSize(self._tabBtnSize.width, self._tabBtnSize.height)
      else
        self._checkedBg:setContentSize(self._bgWidth / #self._subButtons, TabGroup.TAB_GROUP_BTN_HEIGHT)
      end
    else
      label:setColor(UNCHECKED_COLOR)
      if icon then
        icon:setDisplayFrame(display.newSpriteFrame(string.gsub(self._iconTexture[i][2], "#", "")))
      end
      unSelectedBg:show()
      if i == 1 then
        if self._itemStateTexture and self._itemStateTexture[i] then
          unSelectedBg:setSpriteFrame(display.newSpriteFrame(self._itemStateTexture[i][2]))
        else
          unSelectedBg:setSpriteFrame(display.newSpriteFrame("common_tab_bar_icon_left_unselected.png"))
        end
        unSelectedBg:setContentSize(self._bgWidth / #self._subButtons - 4, TabGroup.TAB_GROUP_BTN_HEIGHT)
        unSelectedBg:pos(self._bgWidth * -0.5 + (i - 0.5) * self._bgWidth / #self._subButtons + 2, 1)
      elseif i == #self._subButtons then
        if self._itemStateTexture and self._itemStateTexture[i] then
          unSelectedBg:setSpriteFrame(display.newSpriteFrame(self._itemStateTexture[i][2]))
        else
          unSelectedBg:setSpriteFrame(display.newSpriteFrame("common_tab_bar_icon_right_unselected.png"))
        end
        unSelectedBg:setContentSize(self._bgWidth / #self._subButtons - 4, TabGroup.TAB_GROUP_BTN_HEIGHT)
        unSelectedBg:pos(self._bgWidth * -0.5 + (i - 0.5) * self._bgWidth / #self._subButtons - 2, 1)
      else
        if self._itemStateTexture and self._itemStateTexture[i] then
          unSelectedBg:setSpriteFrame(display.newSpriteFrame(self._itemStateTexture[i][2]))
        else
          unSelectedBg:setSpriteFrame(display.newSpriteFrame("common_tab_bar_icon_middle_unselected.png"))
        end
        unSelectedBg:setContentSize(self._bgWidth / #self._subButtons, TabGroup.TAB_GROUP_BTN_HEIGHT)
        unSelectedBg:pos(self._bgWidth * -0.5 + (i - 0.5) * self._bgWidth / #self._subButtons, 1)
      end
      if self._tabBtnSize then
        unSelectedBg:setContentSize(self._tabBtnSize.width, self._tabBtnSize.height)
      else
        unSelectedBg:setContentSize(self._bgWidth / #self._subButtons, TabGroup.TAB_GROUP_BTN_HEIGHT)
      end
    end
  end
  self._checkedTab = buttonId
  if self._callback then
    self._callback(self._checkedTab)
  end
end
function TabGroup:bindTabClickHandler(callback)
  assert(type(callback) == "function", "callback should be a function")
  self._callback = callback
  return self
end
function TabGroup:setButtonsTitle(titles)
  for i, button in ipairs(self._subButtons) do
    button:setButtonLabelString("normal", titles[i])
  end
end
function TabGroup:setButtonDisableState(i)
  local button = self._subButtons[i]
  local icon = self._buttonIcons[i]
  local iconBg = self._buttonIconBgs[i]
  local label = button:getButtonLabel()
  label:setColor(UNCHECKED_COLOR)
  if icon then
    icon:setDisplayFrame(display.newSpriteFrame(string.gsub(self._iconTexture[i][3], "#", "")))
  end
end
return TabGroup

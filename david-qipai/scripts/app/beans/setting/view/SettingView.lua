local SettingAboutView = import(".SettingAboutView")
local Panel = require("app.ui.window.Panel")
local SettingView = class("SettingView", Panel)
SettingView.PADDING = 16
SettingView.WIDTH = 750
SettingView.HEIGHT = 480
local labelTitleSize = 26
SettingView.labelContentSize = 26
SettingView.labelTitleColor = ccc3(39, 144, 213)
SettingView.labelContentColor = ccc3(202, 202, 202)
SettingView.contentWidth = 700
SettingView.contentHeight = 62
function SettingView:ctor(logoutCallback)
  SettingView.super.ctor(self, SettingView.WIDTH, SettingView.HEIGHT)
  if logoutCallback ~= nil then
    function managerLogoutCallback()
      if logoutCallback then
        logoutCallback()
        self:onClose()
      end
    end
    self.managerLogoutCallback = managerLogoutCallback
  end
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  self:setNodeEventEnabled(true)
  self:initialize()
end
function SettingView:initialize()
  local size = cc.size(720, 480)
  local titlePadding = 50
  local contentPadding = 12
  local contentMarginTop = 12
  local bottomMargin = 12
  local totalHeight = 0
  local leftOriginX = titlePadding
  local topOriginY = size.height / 2 - 40
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  self:addCloseButton()
  local titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.title"),
    color = g.font.color.WHITE,
    size = 28,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, SettingView.HEIGHT * 0.5 - 30):addTo(self)
  local divider = display.newScale9Sprite("#common_divider.png", 0, SettingView.HEIGHT * 0.5 - 50, cc.size(320, 2)):addTo(self)
  local scrollContent = display.newNode()
  local container = display.newNode():addTo(scrollContent)
  totalHeight = totalHeight + titlePadding
  topOriginY = topOriginY - titlePadding
  local nameLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.userName"),
    size = SettingView.labelContentSize,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_CENTER
  })
  nameLabel:setAnchorPoint(ccp(0, 0.5))
  nameLabel:addTo(container)
  nameLabel:pos(contentPadding, topOriginY)
  local nameBg = display.newScale9Sprite("#common_standard_greybg_04.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight + 12, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  nameBg:addTo(container)
  local nameTextLabel = ui.newTTFLabel({
    text = g.vars.user.name,
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  nameTextLabel:addTo(nameBg)
  nameTextLabel:setAnchorPoint(ccp(0, 0.5))
  nameTextLabel:pos(contentPadding, SettingView.contentHeight / 2)
  if g.table == nil or #g.table == 0 then
    local logOutBtn = cc.ui.UIPushButton.new({
      normal = "#common_flat_red_btn_up.png",
      pressed = "#common_flat_red_btn_down.png"
    }, {scale9 = true}):setButtonSize(166, 52):setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("setting.logout"),
      size = labelTitleSize,
      color = ccc3(199, 229, 255),
      align = ui.TEXT_ALIGN_LEFT
    })):addTo(nameBg):pos(SettingView.contentWidth - 83 - 8, SettingView.contentHeight / 2):onButtonClicked(clickHandler(self, self._onLogoutAccountClick))
  end
  totalHeight = totalHeight + SettingView.contentHeight + contentMarginTop
  topOriginY = topOriginY - SettingView.contentHeight - contentMarginTop
  local systemLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.system"),
    size = SettingView.labelContentSize,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_CENTER
  })
  systemLabel:setAnchorPoint(ccp(0, 0.5))
  systemLabel:addTo(container)
  systemLabel:pos(contentPadding, topOriginY - SettingView.contentHeight * 0.5 - 9)
  local soundContent = display.newScale9Sprite("#common_standard_greybg_05.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight * 1.5, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  soundContent:addTo(container)
  local soundLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.sound"),
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  soundLabel:addTo(soundContent)
  soundLabel:setAnchorPoint(ccp(0, 0.5))
  soundLabel:pos(contentPadding, SettingView.contentHeight * 0.5)
  self._soundSliderScale = 0.9
  local soundSliderScale = self._soundSliderScale
  self._soundWidth = 520
  local soundWidth = self._soundWidth / soundSliderScale + 50
  local soundHeight = 18 / soundSliderScale
  local fgSoundHeight = 14 / soundSliderScale
  local soundPosX = SettingView.contentWidth - 590
  local soundProgressBg = display.newScale9Sprite("#common_grey_progress_bg.png"):align(display.LEFT_CENTER, soundPosX + 6, SettingView.contentHeight * 0.5):size(soundWidth - 4, soundHeight):addTo(soundContent):scale(soundSliderScale)
  self.soundProgressFg = display.newScale9Sprite("#common_grey_progress_fill.png"):align(display.LEFT_CENTER, soundPosX + 8, SettingView.contentHeight * 0.5):size(0, fgSoundHeight):addTo(soundContent):scale(soundSliderScale):hide()
  self.soundSlider = cc.ui.UISlider.new(display.LEFT_TO_RIGHT, {
    bar = "#transparent.png",
    button = "#pd_switch_slider.png"
  }, {scale9 = true}):onSliderValueChanged(handler(self, self.soundValueChangeListener)):setSliderSize(soundWidth, soundHeight):align(display.LEFT_BOTTOM, soundPosX, SettingView.contentHeight * 0.5):addTo(soundContent):onSliderRelease(handler(self, self._sliderUpdateSoundValue)):scale(soundSliderScale)
  self.soundSlider:setAnchorPoint(ccp(0, 0.5))
  if g.storage:getBool(g.vars.storageKeys.volumeEnable, true) then
    local volume = g.storage:getInt(g.vars.storageKeys.volume)
    self.soundSlider:setSliderValue(volume)
  else
    self.soundSlider:setSliderValue(0)
  end
  local soundScale = 0.9
  local xPosPadding = soundLabel:getPositionX() + soundLabel:getContentSize().width + 25
  self.silenceButton = cc.ui.UIPushButton.new({
    normal = "#pd_silience_btn_up.png",
    pressed = "#pd_silience_btn_down.png"
  }):pos(xPosPadding, SettingView.contentHeight * 0.5):addTo(soundContent):onButtonClicked(clickHandler(self, function(evt)
    self:_onSilenceBtnClick()
  end)):scale(soundScale)
  self.voiceButton = cc.ui.UIPushButton.new({
    normal = "#pd_voice_btn_up.png",
    pressed = "#pd_voice_btn_down.png"
  }):pos(xPosPadding, SettingView.contentHeight * 0.5):addTo(soundContent):onButtonClicked(clickHandler(self, function(evt)
    self:_onSilenceBtnClick()
  end)):scale(soundScale)
  local vibrateContent = display.newScale9Sprite("#common_standard_greybg_05.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight * 3 + 29, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  vibrateContent:addTo(container)
  local vibrate = ui.newTTFLabel({
    text = g.lang:getContent("setting.vibrate"),
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  vibrate:addTo(vibrateContent)
  vibrate:setAnchorPoint(ccp(0, 0.5))
  vibrate:pos(contentPadding, SettingView.contentHeight / 2)
  local switchPadding = 165
  local switchScale = 0.9
  local isShock = g.storage:getBool(g.vars.storageKeys.shock)
  self.vibrateSwitch = cc.ui.UICheckBoxButton.new({
    on = "#common_switch_on.png",
    off = "#common_switch_off.png"
  }):onButtonStateChanged(handler(self, self.vibrateChangeListener)):align(display.LEFT_CENTER, SettingView.contentWidth - switchPadding, SettingView.contentHeight / 2):addTo(vibrateContent):setButtonSelected(isShock):scale(switchScale)
  totalHeight = totalHeight + SettingView.contentHeight * 2 + contentMarginTop
  topOriginY = topOriginY - SettingView.contentHeight * 2 - contentMarginTop
  local autoOptContent = display.newScale9Sprite("#common_standard_greybg_05.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight * 1.5 + 8, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  autoOptContent:addTo(container)
  local autoSit = ui.newTTFLabel({
    text = g.lang:getContent("setting.autoSit"),
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  autoSit:addTo(autoOptContent)
  autoSit:setAnchorPoint(ccp(0, 0.5))
  autoSit:pos(contentPadding, SettingView.contentHeight / 2)
  local isAutoSit = g.storage:getBool(g.vars.storageKeys.autoSit)
  self.autoSitSwitch = cc.ui.UICheckBoxButton.new({
    on = "#common_switch_on.png",
    off = "#common_switch_off.png"
  }):onButtonStateChanged(handler(self, self.autoSitChangeListener)):align(display.LEFT_CENTER, SettingView.contentWidth - switchPadding, SettingView.contentHeight / 2):addTo(autoOptContent):setButtonSelected(isAutoSit):scale(switchScale)
  local autoBuyinContent = display.newScale9Sprite("#common_standard_greybg_05.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight * 2.5 + 6, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  autoBuyinContent:addTo(container)
  local autoBuyin = ui.newTTFLabel({
    text = g.lang:getContent("setting.autoBuy"),
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  autoBuyin:addTo(autoBuyinContent)
  autoBuyin:setAnchorPoint(ccp(0, 0.5))
  autoBuyin:pos(contentPadding, SettingView.contentHeight / 2)
  local isBuyin = g.storage:getBool(g.vars.storageKeys.autoBuy)
  self.autobuySwitch = cc.ui.UICheckBoxButton.new({
    on = "#common_switch_on.png",
    off = "#common_switch_off.png"
  }):onButtonStateChanged(handler(self, self.autoBuyinChangeListener)):align(display.LEFT_CENTER, SettingView.contentWidth - switchPadding, SettingView.contentHeight / 2):addTo(autoBuyinContent):setButtonSelected(isBuyin):scale(switchScale)
  totalHeight = totalHeight + SettingView.contentHeight * 2 + contentMarginTop
  topOriginY = topOriginY - SettingView.contentHeight * 2 - contentMarginTop
  local otherLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.other"),
    size = SettingView.labelContentSize,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_CENTER
  })
  otherLabel:setAnchorPoint(ccp(0, 0.5))
  otherLabel:addTo(container)
  otherLabel:pos(contentPadding, topOriginY - SettingView.contentHeight - 9)
  local arrowPadding = 45
  local fansContent = display.newScale9Sprite("#common_standard_greybg_05.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight * 2, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  fansContent:addTo(container)
  local fansBtn = cc.ui.UIPushButton.new({
    normal = "#common_transparent_skin.png",
    pressed = "#common_transparent_skin.png"
  }, {scale9 = true}):setButtonSize(SettingView.contentWidth, SettingView.contentHeight):addTo(fansContent):pos(0, SettingView.contentHeight / 2)
  fansBtn:setTouchSwallowEnabled(false)
  fansBtn:setAnchorPoint(ccp(0, 0.5))
  local fansLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.fans"),
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  fansLabel:addTo(fansContent)
  fansLabel:setAnchorPoint(ccp(0, 0.5))
  fansLabel:pos(contentPadding, SettingView.contentHeight / 2)
  local fansArrow = display.newSprite("#common_small_blue_arrow_up.png")
  fansArrow:addTo(fansContent)
  fansArrow:pos(SettingView.contentWidth - arrowPadding, SettingView.contentHeight / 2)
  fansArrow:setAnchorPoint(ccp(0, 0.5))
  local fansArrowPress = display.newSprite("#common_small_blue_arrow_down.png"):addTo(fansArrow):align(display.LEFT_BOTTOM, 0, 0):hide()
  self:buttonTouchHandler(fansBtn, clickHandler(self, self.fansOnClick), fansArrowPress)
  local versionContent = display.newScale9Sprite("#common_standard_greybg_05.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight * 3 - 2, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  versionContent:addTo(container)
  local checkVersionBtn = cc.ui.UIPushButton.new({
    normal = "#common_transparent_skin.png",
    pressed = "#common_transparent_skin.png"
  }, {scale9 = true}):setButtonSize(SettingView.contentWidth, SettingView.contentHeight):addTo(versionContent):pos(0, SettingView.contentHeight + SettingView.contentHeight / 2)
  checkVersionBtn:setTouchSwallowEnabled(false)
  checkVersionBtn:setAnchorPoint(ccp(0, 0.5))
  local checkVersionLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.version"),
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  checkVersionLabel:addTo(versionContent)
  checkVersionLabel:setAnchorPoint(ccp(0, 0.5))
  checkVersionLabel:pos(contentPadding, SettingView.contentHeight / 2)
  local checkVersionArrow = display.newSprite("#common_small_blue_arrow_up.png")
  checkVersionArrow:pos(SettingView.contentWidth - arrowPadding, SettingView.contentHeight / 2)
  checkVersionArrow:setAnchorPoint(ccp(0, 0.5))
  local checkVersionArrowPress = display.newSprite("#common_small_blue_arrow_down.png"):addTo(checkVersionArrow):align(display.LEFT_BOTTOM, 0, 0):hide()
  local currentVersionPadding = 15
  local currentVersion = launchInfo.version
  local currentVersionLabel = ui.newTTFLabel({
    text = currentVersion,
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  currentVersionLabel:addTo(versionContent)
  currentVersionLabel:setAnchorPoint(ccp(0, 0.5))
  currentVersionPadding = currentVersionPadding + currentVersionLabel:getContentSize().width + 15
  currentVersionLabel:pos(SettingView.contentWidth - currentVersionPadding, SettingView.contentHeight / 2)
  local gradeContent = display.newScale9Sprite("#common_standard_greybg_05.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight * 4 - 4, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  gradeContent:addTo(container)
  local appStoreGradeBtn = cc.ui.UIPushButton.new({
    normal = "#common_transparent_skin.png",
    pressed = "#common_transparent_skin.png"
  }, {scale9 = true}):setButtonSize(SettingView.contentWidth, SettingView.contentHeight):addTo(gradeContent):pos(0, SettingView.contentHeight / 2)
  appStoreGradeBtn:setTouchSwallowEnabled(false)
  appStoreGradeBtn:setAnchorPoint(ccp(0, 0.5))
  local appStoreLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.grade"),
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  appStoreLabel:addTo(gradeContent)
  appStoreLabel:setAnchorPoint(ccp(0, 0.5))
  appStoreLabel:pos(contentPadding, SettingView.contentHeight / 2)
  local appStoreArrow = display.newSprite("#common_small_blue_arrow_up.png")
  appStoreArrow:addTo(gradeContent)
  appStoreArrow:pos(SettingView.contentWidth - arrowPadding, SettingView.contentHeight / 2)
  appStoreArrow:setAnchorPoint(ccp(0, 0.5))
  local appStoreArrowPress = display.newSprite("#common_small_blue_arrow_down.png"):addTo(appStoreArrow):align(display.LEFT_BOTTOM, 0, 0):hide()
  self:buttonTouchHandler(appStoreGradeBtn, clickHandler(self, self.appStoreOnClick), appStoreArrowPress)
  local aboutContent = display.newScale9Sprite("#common_standard_greybg_05.png", SettingView.contentWidth / 2, topOriginY - SettingView.contentHeight * 5 - 6, CCSize(SettingView.contentWidth, SettingView.contentHeight))
  aboutContent:addTo(container)
  local aboutBtn = cc.ui.UIPushButton.new({
    normal = "#common_transparent_skin.png",
    pressed = "#common_transparent_skin.png"
  }, {scale9 = true}):setButtonSize(SettingView.contentWidth, SettingView.contentHeight):addTo(aboutContent):pos(0, SettingView.contentHeight / 2)
  aboutBtn:setTouchSwallowEnabled(false)
  aboutBtn:setAnchorPoint(ccp(0, 0.5))
  local aboutLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.about"),
    size = SettingView.labelContentSize,
    color = SettingView.labelContentColor,
    align = ui.TEXT_ALIGN_CENTER
  })
  aboutLabel:addTo(aboutContent)
  aboutLabel:setAnchorPoint(ccp(0, 0.5))
  aboutLabel:pos(contentPadding, SettingView.contentHeight / 2)
  local aboutArrow = display.newSprite("#common_small_blue_arrow_up.png")
  aboutArrow:addTo(aboutContent)
  aboutArrow:pos(SettingView.contentWidth - arrowPadding, SettingView.contentHeight / 2)
  aboutArrow:setAnchorPoint(ccp(0, 0.5))
  local aboutArrowPress = display.newSprite("#common_small_blue_arrow_down.png"):addTo(aboutArrow):align(display.LEFT_BOTTOM, 0, 0):hide()
  self:buttonTouchHandler(aboutBtn, clickHandler(self, self.aboutOnClick), aboutArrowPress)
  totalHeight = totalHeight + SettingView.contentHeight + contentMarginTop + 23
  container:pos(-SettingView.contentWidth / 2, totalHeight / 2 - SettingView.contentHeight / 2 - 26)
  local w, h = SettingView.contentWidth, size.height - bottomMargin - g.ui.TabGroup.TAB_GROUP_HEIGHT - contentMarginTop
  self._scrollPanel = g.ui.ScrollPanel.new({
    visibleRect = cc.rect(-0.5 * w, -0.5 * h, w, h),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }):addTo(self):pos(0, -(size.height / 2 - bottomMargin - h / 2))
  self._scrollPanel:setScrollContent(scrollContent)
end
function SettingView:buttonTouchHandler(button, clickCallback, arrowIcon)
  button:onButtonPressed(handler(self, function(self, evt)
    self._btnPressedY = evt.y
    self._btnClickCanceled = false
    arrowIcon:show()
  end))
  button:onButtonRelease(function(evt)
    if math.abs(evt.y - self._btnPressedY) > 10 then
      self._btnClickCanceled = true
    end
    arrowIcon:hide()
  end)
  button:onButtonClicked(clickHandler(self, function(self, evt)
    if not self._btnClickCanceled and self:getParent():getParent():getCascadeBoundingBox():containsPoint(ccp(evt.x, evt.y)) then
      clickCallback()
    end
  end))
end
function SettingView:_onLogoutAccountClick()
  if self.managerLogoutCallback then
    self.managerLogoutCallback()
  end
end
function SettingView:vibrateChangeListener(event)
  if self.canSound then
    clickSoundImmediately()
    if event.target:isButtonSelected() then
      g.storage:setBool(g.vars.storageKeys.shock, true)
      if self.canSound then
        g.native:vibrate(500)
      end
    else
      g.storage:setBool(g.vars.storageKeys.shock, false)
    end
    g.storage:flush()
  end
end
function SettingView:autoSitChangeListener(event)
  if self.canSound then
    clickSoundImmediately()
    if event.target:isButtonSelected() then
      g.storage:setBool(g.vars.storageKeys.autoSit, true)
    else
      g.storage:setBool(g.vars.storageKeys.autoSit, false)
    end
    g.storage:flush()
  end
end
function SettingView:autoBuyinChangeListener(event)
  if self.canSound then
    clickSoundImmediately()
    if event.target:isButtonSelected() then
      g.storage:setBool(g.vars.storageKeys.autoBuy, true)
    else
      g.storage:setBool(g.vars.storageKeys.autoBuy, false)
    end
    g.storage:flush()
  end
end
function SettingView:soundValueChangeListener(event)
  if event.value == 0 then
    self.soundProgressFg:hide()
  else
    self.soundProgressFg:show()
    self.soundProgressFg:size((self._soundWidth - 8) / self._soundSliderScale * event.value / 100, 14)
  end
  self.soundValue = event.value
  self.prevValue_ = self.curValue_
  self.curValue_ = self.soundValue
  local curTime = g.util.functions.getTime()
  local prevTime = self._lastRaiseSliderGearTickPlayTime or 0
  if self.prevValue_ ~= self.curValue_ and curTime - prevTime > 0.05 then
    self._lastRaiseSliderGearTickPlayTime = curTime
    if self.canSound then
      g.audio:playSound("GEAR_TICK")
    end
  end
end
function SettingView:_sliderUpdateSoundValue()
  if self.soundValue then
    g.storage:setInt(g.vars.storageKeys.volume, self.soundValue)
    g.storage:setInt(g.vars.storageKeys.bgm, self.soundValue)
    if self.soudValue == 0 then
      g.storage:setBool(g.vars.storageKeys.volumeEnable, false)
      g.storage:setBool(g.vars.storageKeys.bgmEnable, false)
    else
      g.storage:setBool(g.vars.storageKeys.volumeEnable, true)
      g.storage:setBool(g.vars.storageKeys.bgmEnable, true)
    end
    g.storage:flush()
    g.audio:updateVolume()
    g.audio:playSound("CLICK_BUTTON")
  end
end
function SettingView:_onSilenceBtnClick()
  if g.storage:getBool(g.vars.storageKeys.volumeEnable) then
    g.storage:setBool(g.vars.storageKeys.volumeEnable, false)
    g.storage:setBool(g.vars.storageKeys.bgmEnable, false)
    self.soundSlider:setSliderValue(0)
    self.voiceButton:hide()
    self.silenceButton:show()
  else
    g.storage:setBool(g.vars.storageKeys.volumeEnable, true)
    g.storage:setBool(g.vars.storageKeys.bgmEnable, true)
    if g.storage:getInt(g.vars.storageKeys.volume) == 0 then
      g.storage:setInt(g.vars.storageKeys.volume, 50)
    end
    local volume = g.storage:getInt(g.vars.storageKeys.volume)
    self.soundSlider:setSliderValue(volume)
    self.voiceButton:show()
    self.silenceButton:hide()
  end
  g.storage:flush()
  g.audio:updateVolume()
end
function SettingView:appStoreOnClick()
  local url
  if device.platform == "android" then
    url = g.lang:getContent("common.googlePlayUrl")
  end
  if url then
    device.openURL(url)
  end
end
function SettingView:aboutOnClick()
  SettingAboutView.new():showPanel()
end
function SettingView:fansOnClick()
  device.openURL(g.lang:getContent("common.fansPageUrl"))
end
function SettingView:onShow()
  self._scrollPanel:update()
  self.canSound = true
end
function SettingView:_showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():pos(0, self.listPosY_):addTo(self)
  end
end
function SettingView:_hideMiniLoading()
  if self._miniLoading then
    self._miniLoading:removeFromParent()
    self._miniLoading = nil
  end
end
function SettingView:onExit()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  g.settingsInfo:saveSettingOnline()
end
return SettingView

local FeedbackView = class("FeedbackView", g.ui.Panel)
local FeedbackItem = import(".FeedbackItem")
FeedbackView.feedbackInfo = {
  uid = "",
  tid = "",
  model = ""
}
FeedbackView.WIDTH = 750
FeedbackView.HEIGHT = 480
function FeedbackView:ctor(defaultTab)
  FeedbackView.super.ctor(self, FeedbackView.WIDTH, FeedbackView.HEIGHT)
  self:setNodeEventEnabled(true)
  self._defaultTab = defaultTab or 1
  self:initialize()
end
function FeedbackView:initialize()
  loadTexture("pd_dailybonus_texture.plist", "pd_dailybonus_texture.png")
  local contentWidth = FeedbackView.WIDTH
  local contentHeight = FeedbackView.HEIGHT
  local upContentHeight = 370
  local subTabItemWidth = 150
  local subTabItemHeight = 44
  local subTabItemWidthOffset = -6
  local subTabItemHeightOffset = -6
  local contentPadding = 16
  local titlePadding = 120
  self:addCloseButton()
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  local titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("feedback.title"),
    color = g.font.color.WHITE,
    size = 28,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, FeedbackView.HEIGHT * 0.5 - 30):addTo(self)
  local divider = display.newScale9Sprite("#common_divider.png", 0, FeedbackView.HEIGHT * 0.5 - 50, cc.size(320, 2)):addTo(self)
  self._subTabGroup = g.ui.TabGroupWithSlider.new({
    background = "#common_standard_greybg_04.png",
    slider = "#common_standard_bluebg_05.png"
  }, g.lang:getContent("feedback.subTabGroupLabels"), {
    selectedText = {
      color = g.font.color.LIGHT,
      size = 26
    },
    defaltText = {
      color = ccc3(139, 184, 220),
      size = 26
    }
  }, true, true):addTo(self):setTabGroupSize(subTabItemWidth * 4, subTabItemHeight, subTabItemWidthOffset, subTabItemHeightOffset):bindTabClickHandler(handler(self, self.onTabChange))
  self.currentTab = 1
  local subTabBarMarginTop = 8
  local subTabBarSize = self._subTabGroup._background:getContentSize()
  local tabHeight = g.ui.TabGroup.TAB_GROUP_HEIGHT
  self._subTabGroup:pos(0, FeedbackView.HEIGHT / 2 - tabHeight - subTabBarSize.height / 2 - subTabBarMarginTop)
  self._inputNode = display.newNode():addTo(self):hide()
  local inputWidth = 566
  local inputHeight = upContentHeight - 30
  local inputContentSize = 24
  local inputContentColor = ccc3(202, 202, 202)
  self.inputEditBox = ui.newEditBox({
    image = "#common_standard_greybg_03.png",
    size = CCSize(inputWidth, inputHeight),
    x = -(contentWidth / 2 - contentPadding - inputWidth / 2),
    y = contentHeight / 2 - titlePadding - inputHeight / 2,
    listener = handler(self, self._onContentEdit)
  }):addTo(self._inputNode)
  self.inputEditBox:setTouchSwallowEnabled(false)
  self.inputEditBox:setFontColor(inputContentColor)
  self.inputEditBox:setPlaceholderFontColor(inputContentColor)
  self.inputEditBox:setFont(ui.DEFAULT_TTF_FONT, inputContentSize)
  self.inputEditBox:setPlaceholderFont(ui.DEFAULT_TTF_FONT, inputContentSize)
  self.inputEditBox:setReturnType(kKeyboardReturnTypeDone)
  self.showContent = ui.newTTFLabel({
    text = g.lang:getContent("feedback.feedbackHint"),
    size = inputContentSize,
    color = inputContentColor,
    align = ui.TEXT_ALIGN_LEFT,
    valign = ui.TEXT_VALIGN_TOP,
    dimensions = CCSize(inputWidth - 30, inputHeight - 30)
  }):addTo(self._inputNode):pos(-(contentWidth / 2 - contentPadding - inputWidth / 2) + 16, contentHeight / 2 - titlePadding - inputHeight / 2):size(inputWidth, inputHeight)
  self._uploadIcon = display.newSprite("#feedback_upload_icon.png"):align(display.CENTER, 3, -1)
  self.uploadPicBtnWidth = 145
  self.uploadPicBtnHeight = 105
  display.newScale9Sprite("#common_standard_greybg_03.png", contentWidth / 2 - contentPadding - self.uploadPicBtnWidth / 2, contentHeight / 2 - titlePadding - self.uploadPicBtnHeight / 2, cc.size(self.uploadPicBtnWidth, self.uploadPicBtnHeight)):addTo(self._inputNode)
  self.uploadPicBtn = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#common_standard_greybg_04.png"
  }, {scale9 = true}):addTo(self._inputNode):setButtonSize(self.uploadPicBtnWidth, self.uploadPicBtnHeight):pos(contentWidth / 2 - contentPadding - self.uploadPicBtnWidth / 2, contentHeight / 2 - titlePadding - self.uploadPicBtnHeight / 2):onButtonClicked(clickHandler(self, self._onPicUploaded)):add(self._uploadIcon)
  self.uploadPicBtn:setTouchSwallowEnabled(false)
  local sendButtonHeight = 57
  self.sendButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):addTo(self._inputNode):setButtonSize(self.uploadPicBtnWidth, sendButtonHeight):pos(contentWidth / 2 - self.uploadPicBtnWidth / 2 - contentPadding, contentHeight / 2 - self.uploadPicBtnHeight - titlePadding - sendButtonHeight / 2 - 4):onButtonClicked(clickHandler(self, self._onSent)):setButtonLabel("normal", ui.newTTFLabel({
    text = g.lang:getContent("feedback.sendButtonLabel"),
    size = 28,
    color = g.font.color.GREEN,
    align = ui.TEXT_ALIGN_CENTER
  }))
  self.sendButton:setTouchSwallowEnabled(false)
  self._feedbackNode = display.newNode():addTo(self):hide()
  FeedbackItem.WIDTH = contentWidth
  local feedbackListMarginTop, feedbackListMarginBottom = 12, 4
  local feedbackListHeight = contentHeight - 200 - feedbackListMarginTop - feedbackListMarginBottom + 84
  self.feedbackList = g.ui.ListPanel.new({
    visibleRect = cc.rect(-0.5 * contentWidth, -0.5 * feedbackListHeight, contentWidth, feedbackListHeight),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }, FeedbackItem):addTo(self._feedbackNode):pos(0, -feedbackListMarginTop - 42)
  self._subTabGroup:tabOn(self._defaultTab)
end
function FeedbackView:onTabChange(selectedTab)
  self:_hideMiniLoading()
  if selectedTab == 1 then
    self._inputNode:show()
    self._feedbackNode:hide()
  else
    self._inputNode:hide()
    self._feedbackNode:show()
    self:requestData()
  end
  self._selectedTab = selectedTab
end
function FeedbackView:_showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():pos(0, -50):addTo(self)
  end
end
function FeedbackView:_hideMiniLoading()
  if self._miniLoading then
    self._miniLoading:removeFromParent()
    self._miniLoading = nil
  end
end
function FeedbackView:requestData()
  self:_showMiniLoading()
  local function successCallback(feedbackList)
    self:_hideMiniLoading()
    if #feedbackList == 0 then
      self.feedbackList:setVisibleRect(cc.rect(-0.5 * (FeedbackView.WIDTH - 34), -172, FeedbackView.WIDTH - 34, 344))
      self.feedbackList:setBackground({
        background = "#common_standard_greybg_03.png"
      })
      self.feedbackList:showNoDataHintView(g.lang:getContent("feedback.noFeedbackTip"))
    end
    self.feedbackList:setData(feedbackList)
    self.feedbackList:update()
  end
  local function failCallback()
    self.feedbackList:setVisibleRect(cc.rect(-0.5 * (FeedbackView.WIDTH - 34), -172, FeedbackView.WIDTH - 34, 344))
    self.feedbackList:setBackground({
      background = "#common_standard_greybg_03.png"
    })
    self.feedbackList:showNoDataHintView(g.lang:getContent("feedback.noFeedbackTip"))
    self:_hideMiniLoading()
  end
  g.feedbackInfo:requestFeedbackListData(successCallback, failCallback)
end
function FeedbackView:onShow()
  self.feedbackList:setScrollContentTouchRect()
end
function FeedbackView:onPopupRemove(callback)
  if self.uploadPic then
    CCTextureCache:sharedTextureCache():removeTexture(self.uploadPic:getTexture())
    self.uploadPic:removeFromParent()
    self.uploadPic = nil
  end
  callback()
end
function FeedbackView:_onPicUploaded()
  local function successCallback(result)
    self.picFilePath = result
    if self.uploadPic then
      CCTextureCache:sharedTextureCache():removeTexture(self.uploadPic:getTexture())
      self.uploadPic:removeFromParent()
      self.uploadPic = nil
    end
    local setImageSize = function(width, height, sprite)
      local sX = width / sprite:getContentSize().width
      local sY = height / sprite:getContentSize().height
      local scale = math.min(sX, sY)
      sprite:scale(scale * 0.9)
    end
    self.uploadPic = display.newSprite(self.picFilePath):addTo(self.uploadPicBtn)
    self._uploadIcon:hide()
    setImageSize(self.uploadPicBtnWidth, self.uploadPicBtnHeight, self.uploadPic)
  end
  local failCallback = function()
  end
  g.feedbackInfo:getPicitureFromAlbum(successCallback, failCallback)
end
function FeedbackView:submitFeedbackSuccessCallback()
  self.showContent:setString(g.lang:getContent("feedback.feedbackHint"))
  g.ui.manager.tip:top(g.lang:getContent("feedback.feedbackSucces"))
  self._uploadIcon:show()
  self:requestData()
end
function FeedbackView:submitFeedbackFailCallback()
end
function FeedbackView:_onSent(evt)
  local displayingText = self.showContent:getString()
  if displayingText == g.lang:getContent("feedback.feedbackHint") or displayingText == "" then
    g.ui.manager.tip:top(g.lang:getContent("feedback.inputEmpty"))
    return
  end
  local function uploadFinishCallback(imgURL)
    g.feedbackInfo:submitFeedbackContent("", self.showContent:getString(), imgURL, handler(self, self.submitFeedbackSuccessCallback), handler(self, self.submitFeedbackFailCallback))
  end
  self:uploadPicture(uploadFinishCallback)
end
function FeedbackView:uploadPicture(uploadFinishCallback)
  if self.picFilePath == nil then
    uploadFinishCallback()
    return
  end
  local function successCallback(imgURL)
    uploadFinishCallback(imgURL)
    self:cleanImageContent()
    self.sendButton:setButtonEnabled(true)
  end
  local failCallback = function()
  end
  g.feedbackInfo:uploadPic(self.picFilePath, successCallback, failCallback)
end
function FeedbackView:cleanImageContent()
  if self.uploadPic then
    CCTextureCache:sharedTextureCache():removeTexture(self.uploadPic:getTexture())
    self.uploadPic:removeFromParent()
    self.uploadPic = nil
  end
end
function FeedbackView:_onContentEdit(event, editbox)
  if event == "began" then
    local displayingText = self.showContent:getString()
    if displayingText == g.lang:getContent("feedback.feedbackHint") then
      self.inputEditBox:setText("")
    else
      self.inputEditBox:setText(displayingText)
    end
    self.showContent:setString("")
  elseif event == "changed" then
  elseif event == "ended" then
    local text = editbox:getText()
    if text == "" then
      text = g.lang:getContent("feedback.feedbackHint")
    end
    self.showContent:setString(text)
    editbox:setText("")
  elseif event == "return" then
  end
end
return FeedbackView

local ExchangeRewardsPopup = class("ExchangeRewardsPopup", g.ui.Panel)
local CommonRewardChipAnimation = require("app.beans.activity.CommonRewardChipAnimation")
local ChangeChipAnim = require("app.beans.activity.ChangeChipAnim")
ExchangeRewardsPopup.WIDTH = 800
ExchangeRewardsPopup.HEIGHT = 460
function ExchangeRewardsPopup:ctor()
  ExchangeRewardsPopup.super.ctor(self, ExchangeRewardsPopup.WIDTH, ExchangeRewardsPopup.HEIGHT)
  loadTexture("pd_dailybonus_texture.plist", "pd_dailybonus_texture.png")
  self:setNodeEventEnabled(true)
  self:enableBGHightLight()
  local TOP = self._height * 0.5
  local BOTTOM = -self._height * 0.5
  local LEFT = -self._width * 0.5
  local RIGHT = self._width * 0.5
  self._panelTitle = ui.newTTFLabel({
    text = g.lang:getContent("exchangAward.title"),
    size = 30,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, TOP - 30):addTo(self)
  local divider = display.newScale9Sprite("#common_divider.png", 0, TOP - 52, cc.size(320, 2)):addTo(self)
  self.inputCode = ui.newEditBox({
    size = CCSize(520, 62),
    align = ui.TEXT_ALIGN_CENTER,
    image = "#exchangecode_inputbg.png",
    imagePressed = "#exchangecode_inputbg.png",
    listener = handler(self, self.onCodeEdit_)
  }):pos(LEFT + 280, TOP - 110)
  self.inputCode:setFontName(ui.DEFAULT_TTF_FONT)
  self.inputCode:setFontSize(30)
  self.inputCode:setFontColor(g.font.color.LIGHT)
  self.inputCode:setPlaceholderFontName(ui.DEFAULT_TTF_FONT)
  self.inputCode:setPlaceholderFontSize(30)
  self.inputCode:setPlaceholderFontColor(g.font.color.GREY)
  self.inputCode:setPlaceHolder(g.lang:getContent("exchangAward.inputCode"))
  self.inputCode:setReturnType(kKeyboardReturnTypeDone)
  self.inputCode:setInputMode(kEditBoxInputModeNumeric)
  self.inputCode:addTo(self)
  self._exchangeBtn = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png"
  }, {scale9 = true}):setButtonSize(220, 62):setButtonLabel(ui.newTTFLabel({
    text = g.lang:getContent("exchangAward.exhang"),
    size = 24,
    color = ccc3(50, 88, 0),
    align = ui.TEXT_ALIGN_CENTER
  })):onButtonClicked(clickHandler(self, self._onSubmitClicked)):pos(277, TOP - 110):addTo(self)
  self._contentBG = display.newScale9Sprite("#common_standard_greybg_03.png", 0, 0, CCSize(ExchangeRewardsPopup.WIDTH - 32, 230)):pos(0, TOP - 270):addTo(self)
  self._leftExchangeReward = display.newSprite("#exchangecode_rewards.png"):pos(120, 110):addTo(self._contentBG)
  self._chipsIcon = display.newSprite("#common_chips_set_icon_04.png"):pos(54, 44):addTo(self._contentBG)
  self._exchangeDesc = ui.newTTFLabel({
    text = "",
    size = 22,
    color = g.font.color.GREY,
    align = ui.TEXT_ALIGN_LEFT,
    valign = ui.TEXT_VALIGN_TOP,
    dimensions = cc.size(460, 200)
  }):addTo(self._contentBG)
  self._exchangeDesc:setAnchorPoint(ccp(0, 1))
  self._exchangeDesc:pos(260, self._contentBG:getContentSize().height - 10)
  self._exchangeDesc:setString(g.lang:getContent("exchangAward.desc"))
  self._followFBBtn = cc.ui.UIPushButton.new({
    normal = "#common_blue_btn_up.png",
    pressed = "#common_blue_btn_down.png"
  }, {scale9 = true}):setButtonSize(250, 60):setButtonLabel(ui.newTTFLabel({
    text = g.lang:getContent("exchangAward.follow"),
    size = 24,
    color = ccc3(0, 98, 115),
    align = ui.TEXT_ALIGN_CENTER
  })):onButtonClicked(clickHandler(self, self.fansOnClick)):pos(0, BOTTOM + 43):addTo(self)
  self:addCloseButton()
end
function ExchangeRewardsPopup:showPanel()
  self:show()
end
function ExchangeRewardsPopup:_onSubmitClicked()
  local function successCallback(retData)
    self:_hideMiniLoading()
    if retData.info and retData.info[1] then
      do
        local infoData = retData.info[1]
        if infoData.type == "chips" then
          local textString = string.gsub(g.lang:getContent("exchangAward.chipsAwarded"), "{1}", infoData.quantity)
          g.ui.manager.tip:top(textString)
          self._totalReward = infoData.quantity
          self:playRewardAnim()
        elseif infoData.type == "magic" then
          self:onClose()
          local textString = string.gsub(g.lang:getContent("exchangAward.magicAwarded"), "{1}", infoData.quantity)
          g.ui.manager.tip:top(textString)
        elseif infoData.type == "gift" then
          self:onClose()
          function loadGiftDataComplete()
            local gift = g.giftInfo:getGiftById(infoData.content)
            if gift ~= nil then
              local giftName = gift.name
              local textString = string.gsub(g.lang:getContent("exchangAward.giftAwarded"), "{1}", giftName)
              g.ui.manager.tip:top(textString)
            end
          end
          g.giftInfo:loadGiftData(loadGiftDataComplete)
        end
      end
    end
  end
  local function failCallback(errorNo)
    self:_hideMiniLoading()
    if tonumber(errorNo or 0) == -6 then
      g.ui.manager.tip:top(g.lang:getContent("exchangAward.errorUsed"))
    elseif tonumber(errorNo or 0) == -7 then
      g.ui.manager.tip:top(g.lang:getContent("exchangAward.errorExpire"))
    elseif tonumber(errorNo or 0) == -12 then
      g.ui.manager.tip:top(g.lang:getContent("exchangAward.errorFail"))
    else
      g.ui.manager.tip:top(g.lang:getContent("exchangAward.errorFail"))
    end
  end
  local exchangeID = string.trim(self._exchangeCodeString or "")
  if exchangeID == nil or exchangeID == "" then
    g.ui.manager.tip:top(g.lang:getContent("exchangAward.errorEmpty"))
  else
    self:_showMiniLoading()
    g.giftInfo:submitExchageCode(exchangeID, successCallback, failCallback)
  end
end
function ExchangeRewardsPopup:playRewardAnim()
  local srcPtX, srcPtY
  srcPtX = -200
  srcPtY = 100
  self._animation = CommonRewardChipAnimation.new(ccp(srcPtX, srcPtY), ccp(display.c_right - 160, display.c_bottom + 20)):addTo(self)
  g.audio:playSound("REWARD_GET")
  self:performWithDelay(handler(self, function(self)
    self._changeChipAnim = ChangeChipAnim.new(self._totalReward):addTo(self:getParent(), 16):pos(display.right - 160, 10)
  end), 0.8)
  self:performWithDelay(handler(self, function(self)
    if self._animation and self._animation:getParent() then
      self._animation:removeFromParent()
      self._changeChipAnim:removeFromParent()
      self:onClose()
    end
  end), 1.5)
end
function ExchangeRewardsPopup:onCodeEdit_(event)
  if event == "began" then
  elseif event == "changed" then
    self._exchangeCodeString = self.inputCode:getText()
    print(self._exchangeCodeString)
  elseif event == "ended" then
  elseif event == "return" then
    print(self._exchangeCodeString)
  end
end
function ExchangeRewardsPopup:fansOnClick()
  device.openURL(g.lang:getContent("common.fansPageUrl"))
end
function ExchangeRewardsPopup:onEnter()
end
function ExchangeRewardsPopup:onExit()
end
return ExchangeRewardsPopup

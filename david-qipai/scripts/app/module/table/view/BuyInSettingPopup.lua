local Panel = require("app.ui.window.Panel")
local BuyInSettingPopup = class("BuyInSettingPopup", Panel)
local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local TableUIEvent = require("app.module.table.view.TableUIEvent")
local ControlButton = import(".ControlButton")
local WIDTH = 714
local HEIGHT = 468
local TOP = HEIGHT * 0.5
local BOTTOM = HEIGHT * -0.5
local LEFT = -0.5 * WIDTH
local RIGHT = 0.5 * WIDTH
BuyInSettingPopup.PROGRESS_WIDTH = 600
BuyInSettingPopup.PROGRESS_HEIGHT = 34
function BuyInSettingPopup:ctor(params)
  BuyInSettingPopup.super.ctor(self, WIDTH, HEIGHT)
  self:setNodeEventEnabled(true)
  self._params = params
  self._seatId = params.seatId
  self._minBuyIn = params.minBuyIn
  self._maxBuyIn = params.maxBuyIn
  self._range = self._maxBuyIn - self._minBuyIn
  self._step = math.ceil(self._range / 10)
  self._myChipsPercent = ((g.vars.user and g.vars.user.chips or 0) - self._minBuyIn) / self._range
  self._middlePercent = (self._maxBuyIn * 0.5 - self._minBuyIn) / self._range
  self:addCloseButton()
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  self._title = ui.newTTFLabel({
    size = 28,
    text = g.lang:getContent("table.buyinTitle"),
    color = ccc3(211, 234, 255)
  }):pos(0, TOP - 28):addTo(self)
  display.newScale9Sprite("#common_divider_01.png", 0, 0, CCSize(326, 2)):pos(0, TOP - 50):addTo(self)
  local userChipsString = string.gsub(g.lang:getContent("table.buyinAccoutLabel"), "{1}", g.util.functions.formatBigInt(g.vars.user.chips))
  self._accountChipsText = ui.newTTFLabel({
    text = userChipsString,
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT
  })
  self._accountChipsText:setAnchorPoint(ccp(0, 0.5))
  self._accountChipsText:pos(LEFT + 40, TOP - 94)
  self._accountChipsText:addTo(self)
  display.newScale9Sprite("#common_divider_01.png", 0, 0, CCSize(640, 2)):pos(0, TOP - 110):addTo(self)
  self._buyInChipsBG = display.newScale9Sprite("#table_buyIn_buyInChipsBG.png", 0, TOP - 160, cc.size(640, 64)):addTo(self)
  self._currentBuyInText = ui.newTTFLabel({
    size = 44,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(self._buyInChipsBG):pos(self._buyInChipsBG:getContentSize().width * 0.5, self._buyInChipsBG:getContentSize().height * 0.5)
  self._sliderBarPoint = display.newSprite("#table_buyIn_sliderPoint.png")
  self._sliderBarPoint:setTouchEnabled(true)
  self._sliderBarPoint:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onSliderBarPointTouch))
  self._sliderBarPoint:addTo(self, 1)
  self.progressRealWidth = BuyInSettingPopup.PROGRESS_WIDTH - self._sliderBarPoint:getContentSize().width
  display.newScale9Sprite("#table_buyIn_sliderBG.png", 0, 0, CCSize(BuyInSettingPopup.PROGRESS_WIDTH, BuyInSettingPopup.PROGRESS_HEIGHT)):addTo(self)
  self.imgClip = CCClippingNode:create():addTo(self)
  self.imgClip:pos(0, 0)
  self.stencil = display.newScale9Sprite("#table_buyIn_sliderFill.png", 0, 0, cc.size(BuyInSettingPopup.PROGRESS_WIDTH - 16, 26))
  self.imgClip:setAlphaThreshold(0)
  self.imgClip:setStencil(self.stencil)
  self.progress = display.newScale9Sprite("#table_buyIn_sliderFill.png", 0, 0, cc.size(BuyInSettingPopup.PROGRESS_WIDTH - 16, 26))
  self.progress:addTo(self.imgClip)
  self._thumbLeft = self.progressRealWidth * -0.5
  self._thumbRight = self.progressRealWidth * 0.5
  local minBuyInString = string.gsub(g.lang:getContent("table.buyinMinLabel"), "{1}", "$" .. g.util.functions.formatBigInt(params.minBuyIn))
  local maxBuyInString = string.gsub(g.lang:getContent("table.buyinMaxLabel"), "{1}", "$" .. g.util.functions.formatBigInt(params.maxBuyIn))
  self._minLabel = ui.newTTFLabel({
    text = minBuyInString,
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT
  })
  self._minLabel:setAnchorPoint(ccp(0, 0.5))
  self._minLabel:pos(LEFT + 40, TOP - 286)
  self._minLabel:addTo(self)
  self._maxLabel = ui.newTTFLabel({
    text = maxBuyInString,
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_RIGHT
  })
  self._maxLabel:setAnchorPoint(ccp(1, 0.5))
  self._maxLabel:pos(RIGHT - 40, TOP - 286)
  self._maxLabel:addTo(self)
  display.newScale9Sprite("#common_divider_01.png", 0, 0, CCSize(640, 2)):pos(0, TOP - 304):addTo(self)
  self._autoBuyInCheckBoxBg = display.newSprite("#pd_common_check_bg.png", LEFT + 60, TOP - 346):addTo(self)
  self._autoBuyInCheckBoxIcon = display.newSprite("#pd_common_check_icon.png"):addTo(self)
  self._autoBuyInLabel = ui.newTTFLabel({
    size = 24,
    color = ccc3(255, 250, 50),
    text = g.lang:getContent("table.buyinAutoLabel")
  }):addTo(self)
  self._isAutoBuyIn = params.isAutoBuyIn
  self._autoBuyInCheckBoxIcon:setVisible(self._isAutoBuyIn)
  local sizeCheckBox = self._autoBuyInCheckBoxBg:getContentSize()
  local sizeLabel = self._autoBuyInLabel:getContentSize()
  local checkBoxWidth = sizeCheckBox.width + 8 + sizeLabel.width
  local checkBoxHeight = math.max(sizeCheckBox.height, sizeLabel.height) + 16
  self._autoBuyInLabel:align(display.LEFT_CENTER, LEFT + 90, self._autoBuyInCheckBoxBg:getPositionY())
  self._autoBuyInCheckBoxIcon:pos(self._autoBuyInCheckBoxBg:getPositionX(), self._autoBuyInCheckBoxBg:getPositionY())
  cc.ui.UIPushButton.new("#transparent.png", {scale9 = true}):setButtonSize(checkBoxWidth + 40, checkBoxHeight + 20):onButtonPressed(function()
  end):onButtonRelease(function()
  end):onButtonClicked(clickHandler(self, function()
    self._isAutoBuyIn = not self._isAutoBuyIn
    self._autoBuyInCheckBoxIcon:setVisible(self._isAutoBuyIn)
  end)):pos(self._autoBuyInLabel:getPositionX() + sizeLabel.width * 0.5 - 40, self._autoBuyInLabel:getPositionY()):addTo(self)
  self._buyInButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    size = 24,
    text = g.lang:getContent("table.buyinButton"),
    color = g.font.color.DARK
  })):setButtonSize(200, 64):onButtonClicked(clickHandler(self, function()
    self:hidePanel()
    local preSaveAutoBuy = g.storage:getBool(g.vars.storageKeys.autoBuy)
    if preSaveAutoBuy ~= self._isAutoBuyIn then
      g.storage:setBool(g.vars.storageKeys.autoBuy, self._isAutoBuyIn or false)
      g.settingsInfo:saveSettingOnline()
    end
    g.eventCenter:dispatchEvent({
      name = TableUIEvent.Player.BUYIN_REQUEST,
      data = {
        seatId = self._seatId,
        buyInChips = self._currentBuyInChips,
        isAutoBuyIn = self._isAutoBuyIn
      }
    })
  end)):pos(0, TOP - 414):addTo(self)
  self:_onSliderPercentValueChange(math.min(self._middlePercent, self._myChipsPercent), true, false)
end
function BuyInSettingPopup:_onSliderBarPointTouch(evt)
  local name, x, y, prevX, prevY = evt.name, evt.x, evt.y, evt.prevX, evt.prevY
  local isTouchInside = self._sliderBarPoint:getCascadeBoundingBox():containsPoint(CCPoint(x, y))
  if name == "began" then
    if isTouchInside then
      self._isThumbTouching = true
      self._thumbTouchBeginX = x
      self._thumbBeginX = self._sliderBarPoint:getPositionX()
      self:unscheduleUpdate()
      return true
    else
      return false
    end
  elseif not self._isThumbTouching then
    return false
  elseif name == "moved" then
    local movedX = x - self._thumbTouchBeginX
    local toX = self._thumbBeginX + movedX
    if toX >= self._thumbRight then
      toX = self._thumbRight
    elseif toX <= self._thumbLeft then
      toX = self._thumbLeft
    end
    local val = (toX - self._thumbLeft) / self.progressRealWidth
    self:_onSliderPercentValueChange(val, false, true)
  elseif name == "ended" or name == "cancelled" then
    self._isThumbTouching = false
    self._posPercent = (self._sliderBarPoint:getPositionX() - self._thumbLeft) / self.progressRealWidth
    if self._posPercent > self._myChipsPercent then
      transition.moveTo(self.progress, {
        x = (self._myChipsPercent - 1) * self.progressRealWidth,
        time = 0.3
      })
      transition.moveTo(self._sliderBarPoint, {
        x = self._thumbLeft + self.progressRealWidth * self._myChipsPercent,
        time = 0.3,
        onComplete = function()
          if self._currentBuyInChips > (g.vars.user and tonumber(g.vars.user.chips) or 0) then
            self._currentBuyInText:setColor(ccc3(255, 131, 11))
          else
            self._currentBuyInText:setColor(g.font.color.LIGHT)
          end
          self._currentBuyInText:setString("$" .. self._currentBuyInChips)
        end
      })
    end
  end
  return true
end
function BuyInSettingPopup:_onSliderPercentValueChange(newVal, forceUpdate, needSound)
  if self._percentValue ~= newVal or forceUpdate then
    local currentTime = g.util.functions.getTime()
    local prevTime = self._lastRaiseSliderGearTickPlayTime or 0
    local moneyVal = math.round(self._minBuyIn + self._range * newVal)
    if moneyVal > (g.vars.user and tonumber(g.vars.user.chips) or 0) then
      self._currentBuyInText:setColor(ccc3(255, 131, 11))
    else
      self._currentBuyInText:setColor(g.font.color.LIGHT)
    end
    self._currentBuyInText:setString("$" .. moneyVal)
    self._sliderBarPoint:setPositionX(self._thumbLeft + self.progressRealWidth * newVal)
    self.progress:setPositionX((newVal - 1) * self.progressRealWidth)
    newVal = math.max(0, math.min(self._myChipsPercent, newVal, 1))
    self._prevValue = self._currentBuyInChips
    self._currentBuyInChips = math.round(self._minBuyIn + self._range * newVal)
    if needSound and self._prevValue ~= self._currentBuyInChips and currentTime - prevTime > 0.05 then
      self._lastRaiseSliderGearTickPlayTime = currentTime
    end
    self._percentValue = newVal
  end
end
function BuyInSettingPopup:showPanel()
  self:show(true, true, true, true)
end
function BuyInSettingPopup:hidePanel()
  self:hide()
end
function BuyInSettingPopup:onEnter()
  if not self._userChipsChangeHandle then
    self._userChipsChangeHandle = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_CHIPS_CHANGE, handler(self, self._onUserChipsChange))
  end
end
function BuyInSettingPopup:onExit()
  if self._userChipsChangeHandle then
    g.eventCenter:removeEventListener(self._userChipsChangeHandle)
    self._userChipsChangeHandle = nil
  end
  self._accountChipsText:setOpacity(255)
end
function BuyInSettingPopup:_onUserChipsChange(evt)
  local chips = g.vars.user.chips
  self._accountChipsText:setString(g.util.functions.formatBigInt(chips or 0))
  self._myChipsPercent = math.max(0, ((chips or 0) - self._minBuyIn) / self._range)
  self:_onSliderPercentValueChange(math.min(self._middlePercent, self._myChipsPercent), true, false)
end
return BuyInSettingPopup

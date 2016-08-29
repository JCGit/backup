local ControlButton = import(".ControlButton")
local PlayStatus = require("app.beans.table.model.PlayStatus")
local TableUIEvent = import(".TableUIEvent")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local PokersStateTip = import(".PokersStateTip")
local PaymentView = require("app.beans.payment.view.PaymentView")
local RaisePanel = import(".RaisePanel")
local ControlPanel = class("ControlPanel", function()
  return display.newNode()
end)
ControlPanel.BUTTON_GAP = 22
ControlPanel.Button_Y_GAP = 8
function ControlPanel:ctor(didStatusWillChangeCallback)
  self.didStatusWillChangeCallback = didStatusWillChangeCallback
  self.seats = {}
  self.seatCount = 9
  self._leftButton = ControlButton.new(ControlButton.TYPE_NORMAL):pos(display.width - ControlPanel.BUTTON_GAP * 2 - ControlButton.BUTTON_WIDTH * 1.5, -80):addTo(self):hide()
  self._rightButton = ControlButton.new(ControlButton.TYPE_NORMAL):pos(display.width - ControlPanel.BUTTON_GAP - ControlButton.BUTTON_WIDTH * 0.5, -80):addTo(self):hide()
  self._quickBetNode = display.newNode():addTo(self):hide()
  self._quickBetLeftBtn = cc.ui.UIPushButton.new({
    normal = "#table_opr_quick_bet_btn_up.png",
    pressed = "#table_opr_quick_bet_btn_down.png",
    disabled = "#table_opr_quick_bet_btn_down.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    size = 30,
    color = g.font.color.WHITE,
    text = ""
  })):setButtonSize(ControlButton.QUICK_BUTTON_WIDTH, ControlButton.BUTTON_HEIGHT):pos(ControlPanel.Button_Y_GAP + ControlButton.QUICK_BUTTON_WIDTH * 0.5, ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP):addTo(self._quickBetNode):onButtonClicked(clickHandler(self, self.quickBetLeftClick))
  self._quickBetMiddleBtn = cc.ui.UIPushButton.new({
    normal = "#table_opr_quick_bet_btn_up.png",
    pressed = "#table_opr_quick_bet_btn_down.png",
    disabled = "#table_opr_quick_bet_btn_down.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    size = 30,
    color = g.font.color.WHITE,
    text = ""
  })):setButtonSize(ControlButton.QUICK_BUTTON_WIDTH, ControlButton.BUTTON_HEIGHT):pos(ControlPanel.Button_Y_GAP + ControlButton.QUICK_BUTTON_WIDTH * 1.5 + ControlPanel.BUTTON_GAP, ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP):addTo(self._quickBetNode):onButtonClicked(clickHandler(self, self.quickBetMiddleClick))
  self._quickBetRightBtn = cc.ui.UIPushButton.new({
    normal = "#table_opr_quick_bet_btn_up.png",
    pressed = "#table_opr_quick_bet_btn_down.png",
    disabled = "#table_opr_quick_bet_btn_down.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    size = 30,
    color = g.font.color.WHITE,
    text = ""
  })):setButtonSize(ControlButton.QUICK_BUTTON_WIDTH, ControlButton.BUTTON_HEIGHT):pos(ControlPanel.Button_Y_GAP + ControlButton.QUICK_BUTTON_WIDTH * 2.5 + ControlPanel.BUTTON_GAP * 2, ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP):addTo(self._quickBetNode):onButtonClicked(clickHandler(self, self.quickBetRightClick))
  self._playStatus = PlayStatus.NON_PLAY
  self._bet = 0
  self._minBet = 0
  self._maxBet = 0
  self._raisePanel = RaisePanel.new(handler(self, self._onRaiseChange))
  self._raisePanel:pos(display.width - ControlButton.BUTTON_WIDTH * 0.5 - ControlPanel.BUTTON_GAP, RaisePanel.HEIGHT * 0.5 + ControlButton.BUTTON_HEIGHT + ControlPanel.Button_Y_GAP):addTo(self):hidePanel()
  self._raiseBtn = cc.ui.UIPushButton.new({
    normal = "#table_opr_raise_bet_btn_normal.png",
    pressed = "#table_opr_raise_bet_btn_pressed.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    size = 30,
    color = g.font.color.WHITE,
    text = ""
  })):setButtonSize(ControlButton.BUTTON_WIDTH, ControlButton.BUTTON_HEIGHT):pos(display.width - ControlButton.BUTTON_WIDTH * 0.5 - ControlPanel.BUTTON_GAP, ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP):addTo(self):onButtonClicked(clickHandler(self, self.onRaiseSureBtnClick)):hide()
end
function ControlPanel:showQuickBetArea()
  if g.vars.table.gamblers ~= nil then
    for i, player in ipairs(g.vars.table.gamblers) do
      if player:getIsSelf() then
        self.ownPlayer = player
        break
      end
    end
  end
  self._quickBetNode:show()
  self.isHidingQuickBetArea = false
  transition.stopTarget(self._quickBetNode)
  self._quickBetNode:setPositionY(-ControlButton.BUTTON_HEIGHT - ControlPanel.Button_Y_GAP)
  transition.moveTo(self._quickBetNode, {
    time = 0.3,
    easing = "out",
    y = 0
  })
  if self._bet == self._minBet then
    self._quickBetLeftValue = self._bet * 2
    self._quickBetMidValue = self._bet * 5
    self._quickBetRightValue = self._bet * 10
  elseif self._bet <= self._minBet * 2 then
    self._quickBetLeftValue = self._minBet
    self._quickBetMidValue = self._bet * 2
    self._quickBetRightValue = self._bet * 5
  else
    self._quickBetLeftValue = self._minBet
    self._quickBetMidValue = math.ceil(self._bet * 0.5)
    self._quickBetRightValue = self._bet * 2
  end
  local leftBtnText = string.gsub(g.lang:getContent("table.betWithChips"), "{chips}", g.util.functions.formatBigInt(self._quickBetLeftValue, 2))
  self._quickBetLeftBtn:setButtonLabel(ui.newTTFLabel({
    size = 24,
    color = g.font.color.WHITE,
    text = leftBtnText
  }))
  if self._quickBetLeftValue > self._maxBet then
    self._quickBetLeftBtn:setButtonEnabled(false)
  else
    self._quickBetLeftBtn:setButtonEnabled(true)
  end
  local midBtnText = string.gsub(g.lang:getContent("table.betWithChips"), "{chips}", g.util.functions.formatBigInt(self._quickBetMidValue, 2))
  self._quickBetMiddleBtn:setButtonLabel(ui.newTTFLabel({
    size = 24,
    color = g.font.color.WHITE,
    text = midBtnText
  }))
  if self._quickBetMidValue > self._maxBet then
    self._quickBetMiddleBtn:setButtonEnabled(false)
  else
    self._quickBetMiddleBtn:setButtonEnabled(true)
  end
  local rightBtnText
  if self._quickBetRightValue >= self._maxBet then
    rightBtnText = g.lang:getContent("table.allIn")
    self._quickBetRightValue = self._maxBet
  else
    rightBtnText = string.gsub(g.lang:getContent("table.betWithChips"), "{chips}", g.util.functions.formatBigInt(self._quickBetRightValue, 2))
  end
  self._quickBetRightBtn:setButtonLabel(ui.newTTFLabel({
    size = 24,
    color = g.font.color.WHITE,
    text = rightBtnText
  }))
  if self._quickBetRightValue > self._maxBet then
    self._quickBetRightBtn:setButtonEnabled(false)
  else
    self._quickBetRightBtn:setButtonEnabled(true)
  end
end
function ControlPanel:hideQuickBetArea()
  if not self._quickBetNode:isVisible() or self.isHidingQuickBetArea then
    return
  end
  self.isHidingQuickBetArea = true
  transition.stopTarget(self._quickBetNode)
  if self._quickBetNode:getPositionY() ~= -ControlButton.BUTTON_HEIGHT - ControlPanel.Button_Y_GAP then
    self._quickBetNode:setPositionY(0)
    transition.moveTo(self._quickBetNode, {
      time = 0.3,
      easing = "out",
      y = -ControlButton.BUTTON_HEIGHT - ControlPanel.Button_Y_GAP,
      onComplete = function()
        self._quickBetNode:hide()
        self.isHidingQuickBetArea = false
      end
    })
  end
end
function ControlPanel:quickBetLeftClick()
  self._bet = self._quickBetLeftValue
  self:_onBetTouch()
end
function ControlPanel:quickBetMiddleClick()
  self._bet = self._quickBetMidValue
  self:_onBetTouch()
end
function ControlPanel:quickBetRightClick()
  self._bet = self._quickBetRightValue
  self:_onBetTouch()
end
function ControlPanel:setPlayStatus(value, player)
  if self.didStatusWillChangeCallback then
    self.didStatusWillChangeCallback(self._playStatus, value)
  end
  self._playStatus = value
  self:hideRaisePanelAnimation()
  if value == PlayStatus.NON_PLAY then
    self:hideButtonsByTween()
    self:uncheck()
  elseif value == PlayStatus.STAND then
    self:hideQuickBetArea()
    self._bet = self._minBet
    self._leftButton:disable()
    self._rightButton:enable()
    self._rightButton:show()
    self:showButtonsByTween()
    self._leftButton:hide()
    if self._rightButton:getChecked() then
      self:performWithDelay(function()
        self:_onSitDownTouch(nil)
      end, 0.2)
    elseif g.vars.table.loginResponse ~= nil and #g.vars.table.gamblers == g.vars.table.loginResponse.seatCount then
      self._rightButton:setText(g.lang:getContent("table.sitdown"))
      self._rightButton:setButtonType(ControlButton.TYPE_CHECK)
      self._rightButton:setTouchCallback(self, nil)
      self._rightButton:setChangeCallback(self, nil)
    else
      self._rightButton:setText(g.lang:getContent("table.sitdown"))
      self._rightButton:setButtonType(ControlButton.TYPE_NORMAL)
      self._rightButton:setTouchCallback(self, self._onSitDownTouch)
      self._rightButton:setChangeCallback(self, nil)
    end
  elseif value == PlayStatus.WAIT_FOR_BET then
    self._leftButton:disable()
    self._rightButton:enable()
    self._rightButton:show()
    self:uncheck()
    self:showButtonsByTween()
    self._leftButton:hide()
    self._rightButton:setText(g.lang:getContent("table.bet"))
    self._rightButton:setButtonType(ControlButton.TYPE_CHECK)
    self._rightButton:setTouchCallback(self, nil)
    self._rightButton:setChangeCallback(self, self._onBetChange)
  elseif value == PlayStatus.BETTING then
    self:showQuickBetArea()
    self:showButtonsByTween()
    self._leftButton:enable()
    self._rightButton:enable()
    self._leftButton:show()
    self._rightButton:show()
    if self._rightButton:getChecked() then
      self:performWithDelay(function()
        self:_onBetTouch(nil)
      end, 0.2)
    end
    self:uncheck()
    if self.ownPlayer ~= nil and self._bet > self.ownPlayer:getSeatChips() then
      self._bet = self.ownPlayer:getSeatChips()
    end
    if self._bet > self._maxBet then
      local betInfo = g.lang:getContent("table.bet") .. g.util.functions.formatBigInt(self._maxBet, 2)
      self._leftButton:setText(betInfo)
    elseif self._bet > self._minBet then
      local betInfo = g.lang:getContent("table.bet") .. g.util.functions.formatBigInt(self._bet, 2)
      self._leftButton:setText(betInfo)
    else
      self._leftButton:setText(g.lang:getContent("table.bet"))
    end
    self._leftButton:setButtonType(ControlButton.TYPE_NORMAL)
    self._leftButton:setTouchCallback(self, self._onBetTouch)
    self._leftButton:setChangeCallback(self, nil)
    self._rightButton:setText(g.lang:getContent("table.raise"))
    self._rightButton:setButtonType(ControlButton.TYPE_NORMAL)
    self._rightButton:setTouchCallback(self, self._onRaiseTouch)
    self._rightButton:setChangeCallback(self, nil)
  elseif value == PlayStatus.BETTED then
    self:hideQuickBetArea()
    self._leftButton:enable()
    self._rightButton:enable()
    self._leftButton:show()
    self._rightButton:show()
    self:uncheck()
    self:showButtonsByTween()
    self._leftButton:setText(g.lang:getContent("table.hitting"))
    self._leftButton:setButtonType(ControlButton.TYPE_CHECK)
    self._leftButton:setTouchCallback(self, nil)
    self._leftButton:setChangeCallback(self, self._onHitChange)
    self._rightButton:setText(g.lang:getContent("table.staying"))
    self._rightButton:setButtonType(ControlButton.TYPE_CHECK)
    self._rightButton:setTouchCallback(self, nil)
    self._rightButton:setChangeCallback(self, self._onStayChange)
  elseif value == PlayStatus.SPEAKING then
    self._leftButton:enable()
    self._rightButton:enable()
    self._leftButton:show()
    self._rightButton:show()
    self:showButtonsByTween()
    if player:getChooseType() == 2 then
      if self._leftButton:getChecked() then
        self:performWithDelay(function()
          self:_onHitTouch(nil)
        end, 0.2)
        self._leftButton:disable()
        self._rightButton:disable()
      else
        self._leftButton:enable()
        self._rightButton:disable()
      end
    elseif self._leftButton:getChecked() then
      self:performWithDelay(function()
        self:_onHitTouch(nil)
      end, 0.2)
      self._leftButton:disable()
      self._rightButton:disable()
    elseif self._rightButton:getChecked() then
      self:performWithDelay(function()
        self:_onStayTouch(nil)
      end, 0.2)
      self._leftButton:disable()
      self._rightButton:disable()
    else
      self._leftButton:enable()
      self._rightButton:enable()
    end
    self:uncheck()
    self._leftButton:setText(g.lang:getContent("table.hitting"))
    self._leftButton:setButtonType(ControlButton.TYPE_NORMAL)
    self._leftButton:setTouchCallback(self, self._onHitTouch)
    self._leftButton:setChangeCallback(self, nil)
    self._rightButton:setText(g.lang:getContent("table.staying"))
    self._rightButton:setButtonType(ControlButton.TYPE_NORMAL)
    self._rightButton:setTouchCallback(self, self._onStayTouch)
    self._rightButton:setChangeCallback(self, nil)
  elseif value == PlayStatus.WAIT_FOR_GAMESTART or value == PlayStatus.SPOKEN then
    self:hideQuickBetArea()
    self:showButtonsByTween()
    if value == PlayStatus.WAIT_FOR_GAMESTART then
    elseif value == PlayStatus.SPOKEN then
    end
    if self._rightButton:getText() == nil or self._rightButton:getText() == "" then
      self:hideButtonsByTween()
    end
    self:uncheck()
    self._leftButton:disable()
    self._rightButton:disable()
    self._leftButton:setButtonType(ControlButton.TYPE_NORMAL)
    self._rightButton:setButtonType(ControlButton.TYPE_NORMAL)
  else
    self:hideQuickBetArea()
    self:showButtonsByTween()
    if self._rightButton:getText() == nil or self._rightButton:getText() == "" then
      self:hideButtonsByTween()
    end
    self:uncheck()
    self._leftButton:disable()
    self._rightButton:disable()
    self._leftButton:setButtonType(ControlButton.TYPE_NORMAL)
    self._rightButton:setButtonType(ControlButton.TYPE_NORMAL)
  end
end
function ControlPanel:showTipsByTween()
  self._pokersStateTip:show()
  transition.stopTarget(self._pokersStateTip)
  if self._pokersStateTip:getPositionY() ~= ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP then
    self._pokersStateTip:setPositionY(-80)
    transition.moveTo(self._pokersStateTip, {
      time = 0.3,
      easing = "out",
      y = ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP
    })
  end
end
function ControlPanel:updatePokersTipsInfo(tipsText)
  if tipsText ~= self.preTips then
    self._pokersStateTip:playNextTip(tipsText)
    self.preTips = tipsText
  end
end
function ControlPanel:hideTipsByTween()
  transition.stopTarget(self._pokersStateTip)
  if self._pokersStateTip:getPositionY() ~= -80 then
    self._pokersStateTip:setPositionY(ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP)
    transition.moveTo(self._pokersStateTip, {
      time = 0.3,
      easing = "out",
      y = -80,
      onComplete = function()
        self._pokersStateTip:hide()
      end
    })
  end
end
function ControlPanel:showButtonsByTween()
  transition.stopTarget(self._leftButton)
  transition.stopTarget(self._rightButton)
  self._leftButton:show()
  transition.moveTo(self._leftButton, {
    time = 0.3,
    easing = "out",
    y = ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP,
    onComplete = function(...)
      local positionX, positionY = self._leftButton:getPosition()
    end
  })
  self._rightButton:show()
  transition.moveTo(self._rightButton, {
    time = 0.3,
    easing = "out",
    y = ControlButton.BUTTON_HEIGHT * 0.5 + ControlPanel.Button_Y_GAP
  })
end
function ControlPanel:hideButtonsByTween()
  transition.stopTarget(self._leftButton)
  transition.stopTarget(self._rightButton)
  transition.moveTo(self._leftButton, {
    time = 0.3,
    easing = "out",
    y = -80,
    onComplete = function()
      self._leftButton:hide()
    end
  })
  transition.moveTo(self._rightButton, {
    time = 0.3,
    easing = "out",
    y = -80,
    onComplete = function()
      self._rightButton:hide()
    end
  })
end
function ControlPanel:showRaisePanelAnimation()
  self._raisePanel:showPanel()
  self._raiseBtn:show()
  self._raiseBtn:setButtonLabel(ui.newTTFLabel({
    size = 30,
    color = g.font.color.WHITE,
    text = g.util.functions.formatBigInt(self._minBet, 2)
  }))
end
function ControlPanel:hideRaisePanelAnimation()
  self._raisePanel:hidePanel()
  self._raiseBtn:hide()
end
function ControlPanel:onBackgroundClicked()
  self._raisePanel:hidePanel()
  self._raiseBtn:hide()
end
function ControlPanel:getPlayStatus(value)
  return self._playStatus
end
function ControlPanel:uncheck()
  if self._leftButton:getButtonType() == ControlButton.TYPE_CHECK then
    self._leftButton:setChecked(false)
  end
  if self._rightButton:getButtonType() == ControlButton.TYPE_CHECK then
    self._rightButton:setChecked(false)
  end
end
function ControlPanel:setBet(value)
  self._bet = value
end
function ControlPanel:getBet()
  return self._bet
end
function ControlPanel:setMinBet(value)
  self._minBet = value
  self._raisePanel:setMin(value)
end
function ControlPanel:setMaxBet(value)
  self._maxBet = value
  self._raisePanel:setMax(value)
end
function ControlPanel:_onHitTouch(evt)
  local data = {operation = 1}
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.SPEAKCONTENT_SEND_REQUEST,
    data = data
  })
  self._leftButton:disable()
  self._rightButton:disable()
end
function ControlPanel:_onHitChange(evt)
  self._rightButton:setChecked(false)
end
function ControlPanel:_onStayTouch(evt)
  local data = {operation = 0}
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.SPEAKCONTENT_SEND_REQUEST,
    data = data
  })
  self._leftButton:disable()
  self._rightButton:disable()
end
function ControlPanel:_onStayChange(evt)
  self._leftButton:setChecked(false)
end
function ControlPanel:_onSitDownTouch(evt)
  for i, seat in ipairs(self.seats) do
    local seatIsAvailable = true
    if self.seatCount == 5 and (i == 2 or i == 4 or i == 6 or i == 8) then
      seatIsAvailable = false
    end
    if seat:isEmpty() and seatIsAvailable then
      if g.vars.table.loginResponse.minBuyIn > tonumber(g.vars.user.chips or 0) then
        g.ui.Dialog.new({
          contentText = g.lang:getContent("table.buyinNotEnough"),
          confirmButtonText = g.lang:getContent("shop.buyLabel"),
          dialogType = g.ui.DialogType.NORMAL,
          callback = function(sender)
            if sender == g.ui.DialogConst.CONFIRM_BUTTON_CLICK then
              PaymentView.new():show()
            end
          end
        }):show()
        return
      else
        logger:debug("ControlPanel:EVT_CLI_REQUEST_SIT")
        g.eventCenter:dispatchEvent({
          name = TableBeanEvent.EVT_CLI_REQUEST_SIT,
          data = {
            seatId = seat:getSeatIdForServer(),
            buyInChips = math.ceil(g.vars.table.loginResponse.maxBuyIn / 2)
          }
        })
        local isAutoBuy = g.storage:getBool(g.vars.storageKeys.autoBuy)
        if isAutoBuy then
          g.eventCenter:dispatchEvent({
            name = TableBeanEvent.EVT_CLI_REQUEST_AUTO_BUYIN,
            data = {
              seatId = seat:getSeatIdForServer(),
              buyInChips = math.ceil(g.vars.table.loginResponse.maxBuyIn / 2)
            }
          })
        end
        return
      end
    end
  end
end
function ControlPanel:disableBetButtons()
  self._leftButton:disable()
  self._rightButton:disable()
  self:hideQuickBetArea()
  self:hideRaisePanelAnimation()
end
function ControlPanel:_onBetTouch(evt)
  if self._bet > self._maxBet then
    self._bet = self._maxBet
  end
  local data = {
    bet = self._bet
  }
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.BET_SEND_REQUEST,
    data = data
  })
  self:disableBetButtons()
end
function ControlPanel:_onBetChange(evt)
  self._leftButton:setChecked(false)
end
function ControlPanel:onRaiseSureBtnClick()
  self._bet = self._raisePanel:getValue()
  self:_onBetTouch()
end
function ControlPanel:_onRaiseTouch(evt)
  self:showRaisePanelAnimation()
end
function ControlPanel:_onRaiseChange(newValue)
  if self._raiseBtn then
    self._raiseBtn:setButtonLabel(ui.newTTFLabel({
      size = 30,
      color = g.font.color.WHITE,
      text = g.util.functions.formatBigInt(newValue, 2)
    }))
  end
end
return ControlPanel

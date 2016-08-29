local ChipsItem = import(".ChipsItem")
local ChipsListItem = import(".ChipsListItem")
local OrderItem = import(".OrderItem")
local PayPluginManager = require("app.beans.payment.controller.PayPluginManager")
local GiftListPanel = require("app.beans.giftshop.view.GiftListPanel")
local PayChannelOrder = require("app.beans.payment.model.PayChannelOrder")
local PaymentView = class("PaymentView", g.ui.Panel)
local WIDTH, HEIGHT = 860, 542
local CONTENT_WIDTH, CONTENT_HEIGHT = 828, 428
local PAYCHANNEL_WIDTH = 180
function PaymentView:ctor(tab)
  PaymentView.super.ctor(self, WIDTH, HEIGHT)
  self._defaultTab = tab or 1
  self:setNodeEventEnabled(true)
  self:addCloseButton()
  self._pluginManager = PayPluginManager.new()
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  self:_initTabGroup()
end
function PaymentView:onEnter()
  if PaymentView.super.onEnter then
    PaymentView.super.onEnter(self)
  end
end
function PaymentView:onExit()
  if PaymentView.super.onExit then
    PaymentView.super.onExit(self)
  end
  if self._closeCallback then
    self._closeCallback()
    self._closeCallback = nil
  end
end
function PaymentView:proccessPaymentData(jsonData)
  local payChannelAvailable = {}
  for channel, config in pairs(jsonData) do
    if self._pluginManager:isPluginEnabled(channel) then
      payChannelAvailable[#payChannelAvailable + 1] = config
    end
  end
  if not self._isInitialized then
    self._pluginManager:initialize(payChannelAvailable)
    self:initialize(payChannelAvailable)
    self._isInitialized = true
  end
end
function PaymentView:show(closeCallback)
  self._closeCallback = closeCallback
  PaymentView.super.show(self, true, true, true, true)
end
function PaymentView:onShow()
  self:tabOn(self._defaultTab)
  self._defaultTab = nil
end
function PaymentView:_updateTouchRect()
  if self._payTypeSelectPanel then
    self._payTypeSelectPanel:setScrollContentTouchRect()
  end
  if self._chipsPanel then
    self._chipsPanel:setScrollContentTouchRect()
  end
  if self._ordersPanel then
    self._ordersPanel:setScrollContentTouchRect()
  end
end
function PaymentView:initialize(payConfig)
  self._payConfig = payConfig
  self._selectedPayChannel = payConfig[1]
  if payConfig and #payConfig > 1 then
    CONTENT_WIDTH = 678
    ChipsItem.PADDING_LEFT = 10
    self:_initPayConfig(payConfig)
    ChipsItem.WIDTH = 180
    ChipsListItem.GAP_X = 40
    ChipsListItem.MARGIN_X = 135
  else
    CONTENT_WIDTH = 828
    ChipsItem.PADDING_LEFT = 2
    PAYCHANNEL_WIDTH = 0
    ChipsItem.WIDTH = 225
    ChipsListItem.GAP_X = 25
    ChipsListItem.MARGIN_X = 105
  end
  ChipsItem.HEIGHT = 200
  OrderItem.WIDTH = 828
  OrderItem.HEIGHT = 77
end
function PaymentView:_initTabGroup()
  self._tabGroup = g.ui.TabGroup.new({
    windowWidth = 720,
    iconOffsetX = 4,
    iconTexture = {
      {
        "#pay_chips_icon_selected.png",
        "#pay_chips_icon_unselected.png"
      },
      {
        "#pay_order_icon_selected.png",
        "#pay_order_icon_unselected.png"
      }
    },
    buttonTexts = g.lang:getContent("payment.tabGroupLabels")
  }):pos(0, HEIGHT * 0.5 - g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5 - 16):addTo(self, 10)
  self._tabGroup:bindTabClickHandler(handler(self, self.onTabChange))
end
function PaymentView:_initPayConfig(payConfig)
  local contentNode = display.newNode()
  self._payChannelButtonGroup = g.ui.CheckButtonGroup.new()
  local fromY = 58 * (#payConfig - 1) * 0.5
  local newOrderConfig = {}
  for index = 1, #PayChannelOrder do
    for i, config in ipairs(payConfig) do
      if config.channel == PayChannelOrder[index] then
        newOrderConfig[#newOrderConfig + 1] = config
        display.newScale9Sprite("#pay_channel_logo_bg_normal.png", 4, fromY, cc.size(134, 46)):addTo(contentNode)
        local button = cc.ui.UICheckBoxButton.new({
          on = "#pay_channel_logo_bg_selected.png",
          off = "#transparent.png"
        }, {scale9 = true}):setButtonSize(178, 58):pos(0, fromY):addTo(contentNode)
        button:setTouchSwallowEnabled(false)
        self._payChannelButtonGroup:addButton(button)
        local path = CCFileUtils:sharedFileUtils():fullPathForFilename("pay_channel_logo_" .. config.channel .. ".png")
        if io.exists(path) then
          display.newSprite("pay_channel_logo_" .. config.channel .. ".png"):pos(5, fromY):addTo(contentNode)
        else
          local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("pay_channel_logo_" .. config.channel .. ".png")
          if frame then
            display.newSprite(frame):pos(5, fromY):addTo(contentNode)
          else
          end
        end
        fromY = fromY - 58
      end
    end
  end
  if #newOrderConfig > 0 then
    self._payConfig = newOrderConfig
  end
  self._payTypeSelectPanel = g.ui.ScrollPanel.new({
    visibleRect = cc.rect(-0.5 * PAYCHANNEL_WIDTH, -0.5 * CONTENT_HEIGHT, PAYCHANNEL_WIDTH, CONTENT_HEIGHT),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollContent = contentNode,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }):addTo(self, 10):pos(WIDTH * 0.5 - PAYCHANNEL_WIDTH * 0.5 - 2, HEIGHT * -0.5 + CONTENT_HEIGHT * 0.5 + 22)
  self._payChannelButtonGroup:getButtonAtIndex(1):setButtonSelected(true)
  self._payChannelButtonGroup:onButtonSelectChanged(handler(self, self._onPayTypeButtonGroupChange))
end
function PaymentView:tabOn(tab)
  self._tabGroup:tabOn(tab)
end
function PaymentView:getSelectedTab()
  return self._selectedTab or 1
end
function PaymentView:getSelectedPayChannel()
  return self._selectedPayChannel or self._payConfig[1]
end
function PaymentView:onTabChange(selectedTab)
  if self._selectedTab ~= selectedTab then
    self._selectedTab = selectedTab
    if self._input1 then
      self._input1:hide()
    end
    if self._input2 then
      self._input2:hide()
    end
    if self._submitButton then
      self._submitButton:hide()
    end
    self._pages = self._pages or {}
    for key, page in pairs(self._pages) do
      page:hide()
    end
    local page = self._pages[selectedTab]
    if not page then
      if selectedTab == 1 then
        page = self:_initChipsPage()
      elseif selectedTab == 2 then
        page = self:_initOrderPage()
      end
      page:addTo(self)
      self._pages[selectedTab] = page
    end
    page:show()
    if selectedTab == 1 then
      if self._payTypeSelectPanel then
        self._payTypeSelectPanel:show()
      end
      do
        local paymentData = g.paymentInfo:getPaymentData()
        if paymentData == nil then
          self._chipsPanel:showMiniLoading()
          g.paymentInfo:loadPaymentData(handler(self, function(self)
            self._chipsPanel:hideMiniLoading()
            paymentData = g.paymentInfo:getPaymentData()
            self:proccessPaymentData(paymentData)
            self._pages[selectedTab]:removeFromParent()
            self._pages[selectedTab] = self:_initChipsPage()
            self._pages[selectedTab]:addTo(self)
            self:_loadProductList()
          end))
        else
          self:proccessPaymentData(paymentData)
          self:_loadProductList()
        end
      end
    elseif selectedTab == 2 then
      if self._payTypeSelectPanel then
        self._payTypeSelectPanel:hide()
      end
      self:_loadOrderList()
    end
  end
end
function PaymentView:_onPayTypeButtonGroupChange(evt)
  self._selectedPayChannel = self._payConfig[evt.selected] or self._payConfig[1]
  self:_loadProductList()
end
function PaymentView:_loadProductList()
  local plugin = self._pluginManager:getPlugin(self:getSelectedPayChannel().channel)
  plugin:loadChipsList(handler(self, self._loadChipsListResult))
end
function PaymentView:_loadOrderList()
  self._ordersPanel:showMiniLoading()
  g.paymentInfo:loadOrderData(handler(self, function(self)
    local orderData = g.paymentInfo:getOrderData()
    self:setOrderList(orderData)
    self._ordersPanel:hideMiniLoading()
  end), handler(self, function(self)
    self:setOrderList(nil)
    self._ordersPanel:hideMiniLoading()
  end))
end
function PaymentView:setChipsList(config, isComplete, data)
  self._chipsPanel:hideNoDataHintView()
  self:_showInputIfNeeded()
  if config.id == self:getSelectedPayChannel().id then
    if not self._pages[1] then
      return
    end
    local groupData = g.giftInfo:groupGiftData(data, 3)
    if isComplete then
      self._chipsPanel:hideMiniLoading()
      if type(data) == "string" then
        self._chipsPanel:setData(nil)
      else
        self._chipsPanel:setData(nil)
        self._chipsPanel:setData(groupData)
        self:_updateTouchRect()
      end
    else
      self._chipsPanel:setData(nil)
      self._chipsPanel:showMiniLoading()
    end
  end
end
function PaymentView:_showInputIfNeeded()
  if not self._input1 then
    self._input1 = ui.newEditBox({
      image = "#pay_input_bg.png",
      size = CCSize(CONTENT_WIDTH - 160, 50)
    }):addTo(self, 100):hide()
    self._input1:setFontName(ui.DEFAULT_TTF_FONT)
    self._input1:setFontSize(24)
    self._input1:setFontColor(ccc3(183, 200, 212))
    self._input1:setPlaceholderFontName(ui.DEFAULT_TTF_FONT)
    self._input1:setPlaceholderFontSize(24)
    self._input1:setPlaceholderFontColor(ccc3(183, 200, 212))
  end
  if not self._input2 then
    self._input2 = ui.newEditBox({
      image = "#common_input_bg_up.png",
      imagePressed = "#common_input_bg_down.png",
      size = CCSize(CONTENT_WIDTH - 210, 44)
    }):addTo(self, 100):hide()
    self._input2:setFontName(ui.DEFAULT_TTF_FONT)
    self._input2:setFontSize(24)
    self._input2:setFontColor(ccc3(183, 200, 212))
    self._input2:setPlaceholderFontName(ui.DEFAULT_TTF_FONT)
    self._input2:setPlaceholderFontSize(24)
    self._input2:setPlaceholderFontColor(ccc3(183, 200, 212))
  end
  if not self._submitButton then
    self._submitButton = cc.ui.UIPushButton.new({
      normal = "#pay_topup_button_up.png",
      pressed = "#pay_topup_button_down.png"
    }, {scale9 = true}):setButtonLabel("normal", ui.newTTFLabel({
      size = 28,
      text = g.lang:getContent("payment.cardSubmitCharge"),
      align = ui.TEXT_ALIGN_CENTER
    })):onButtonClicked(clickHandler(self, self._onSubmitClick)):addTo(self, 100):hide()
  end
  self._input1:setMaxLength(0)
  self._input1:setInputMode(kEditBoxInputModeSingleLine)
  self._input1:setInputFlag(kEditBoxInputFlagSensitive)
  self._input1:setReturnType(kKeyboardReturnTypeDone)
  self._input1:setPlaceHolder("")
  self._input1:setText("")
  self._input2:setMaxLength(0)
  self._input2:setInputMode(kEditBoxInputModeSingleLine)
  self._input2:setInputFlag(kEditBoxInputFlagSensitive)
  self._input2:setReturnType(kKeyboardReturnTypeDone)
  self._input2:setPlaceHolder("")
  self._input2:setText("")
  local selectedPayType = self:getSelectedPayChannel()
  self:prepareEditBox(selectedPayType, self._input1, self._input2, self._submitButton)
  local inputHeight = 0
  if selectedPayType.inputType == "singleLine" then
    inputHeight = 60
    self._input1:pos(WIDTH * 0.5 - CONTENT_WIDTH + (CONTENT_WIDTH - 160) * 0.5 - 157, -HEIGHT + CONTENT_HEIGHT - 107)
    self._submitButton:setButtonSize(164, 50):pos(WIDTH * 0.5 - 248, -HEIGHT + CONTENT_HEIGHT - 107)
    self._input1:show()
    self._input2:hide()
    self._submitButton:show()
  elseif selectedPayType.inputType == "twoLine" then
    inputHeight = 120
    self._input1:pos(WIDTH * 0.5 - CONTENT_WIDTH + (CONTENT_WIDTH - 210) * 0.5, HEIGHT * -0.5 + CONTENT_HEIGHT)
    self._input2:pos(WIDTH * 0.5 - CONTENT_WIDTH + (CONTENT_WIDTH - 210) * 0.5, HEIGHT * -0.5 + CONTENT_HEIGHT - 50)
    self._submitButton:setButtonSize(176, 100):pos(WIDTH * 0.5 - 110, HEIGHT * -0.5 + CONTENT_HEIGHT - 26)
    self._input1:show()
    self._input2:show()
    self._submitButton:show()
  else
    self._input1:hide()
    self._input2:hide()
    self._submitButton:hide()
  end
  if self._chipsPanel then
    self._chipsPanel:setVisibleRect(cc.rect(-0.5 * (CONTENT_WIDTH - 4), -0.5 * (CONTENT_HEIGHT - inputHeight - 8), CONTENT_WIDTH - 4, CONTENT_HEIGHT - inputHeight - 8))
    if #self._payConfig > 1 then
      self._chipsPanel:pos(WIDTH * 0.5 - CONTENT_WIDTH * 0.5 - PAYCHANNEL_WIDTH + 16, HEIGHT * -0.5 + CONTENT_HEIGHT * 0.5 + 22 + inputHeight * 0.5)
      self._background:pos(WIDTH * 0.5 - CONTENT_WIDTH * 0.5 - PAYCHANNEL_WIDTH + 16, HEIGHT * -0.5 + CONTENT_HEIGHT * 0.5 + 22)
    else
      self._chipsPanel:pos(WIDTH * 0.5 - CONTENT_WIDTH * 0.5 - PAYCHANNEL_WIDTH - 16, HEIGHT * -0.5 + CONTENT_HEIGHT * 0.5 + 22 + inputHeight * 0.5)
      self._background:pos(WIDTH * 0.5 - CONTENT_WIDTH * 0.5 - PAYCHANNEL_WIDTH - 16, HEIGHT * -0.5 + CONTENT_HEIGHT * 0.5 + 22)
    end
  end
end
function PaymentView:prepareEditBox(payChannel, input1, input2, submitButton)
  local plugin = self._pluginManager:getPlugin(payChannel.channel)
  plugin:prepareEditBox(input1, input2, submitButton)
end
function PaymentView:_onSubmitClick()
  self._chipsPanel:showMiniLoading()
  self:onInputCardInfo(self:getSelectedPayChannel(), self._input1, self._input2, self._submitButton)
end
function PaymentView:onInputCardInfo(payChannel, input1, input2, submitButton)
  local plugin = self._pluginManager:getPlugin(payChannel.channel)
  plugin:onInputCardInfo(input1, input2, submitButton, handler(self, self._payResult))
end
function PaymentView:_loadChipsListResult(payChannel, isComplete, data)
  self:setChipsList(payChannel, isComplete, data)
end
function PaymentView:_payResult(isSuccess, result)
  if self._chipsPanel then
    self._chipsPanel:hideMiniLoading()
  end
  if succ then
    g.paymentInfo.isNeedRefresh = true
    self:_loadOrderList()
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "synchronize", requestData), function(data)
    end, function()
    end)
  end
end
function PaymentView:setOrderList(data)
  if not self._pages[2] then
    return
  end
  self._ordersPanel:hideMiniLoading()
  if data and #data > 0 then
    self._ordersPanel:setData(data)
    self:_updateTouchRect()
  else
    self._ordersPanel:setData(nil)
  end
end
function PaymentView:_initChipsPage()
  local page = display.newNode()
  local pageX, pageY = WIDTH * 0.5 - CONTENT_WIDTH * 0.5 - 16, HEIGHT * -0.5 + CONTENT_HEIGHT * 0.5 + 22
  self._background = display.newScale9Sprite("#common_standard_greybg_03.png", pageX, pageY, cc.size(CONTENT_WIDTH - 4, CONTENT_HEIGHT)):addTo(page)
  self._chipsPanel = GiftListPanel.new({
    bgMargin = cc.size(0, 8),
    visibleRect = cc.rect(-0.5 * (CONTENT_WIDTH - 4), -0.5 * (CONTENT_HEIGHT - 4), CONTENT_WIDTH - 8, CONTENT_HEIGHT),
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL
  }, ChipsListItem):pos(pageX, pageY):addTo(page)
  self._chipsPanel:addEventListener("ITEM_EVENT", handler(self, self._onItemEvent))
  self._chipsPanel:setNoDataHintText(g.lang:getContent("common.badNetwork"))
  return page
end
function PaymentView:_initOrderPage()
  local page = display.newNode()
  local pageX, pageY = WIDTH * 0.5 - CONTENT_WIDTH * 0.5 - 16, HEIGHT * -0.5 + CONTENT_HEIGHT * 0.5 + 22
  self._ordersPanel = g.ui.ListPanel.new({
    bgMargin = cc.size(0, 12),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-412, -0.5 * (CONTENT_HEIGHT - 4), 824, CONTENT_HEIGHT),
    direction = g.ui.ListPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }, OrderItem):pos(pageX, pageY):addTo(page)
  if #self._payConfig > 1 then
    self._ordersPanel:pos(pageX - 73, pageY)
  end
  self._ordersPanel:setNoDataHintText(g.lang:getContent("payment.noOrderTip"))
  return page
end
function PaymentView:_onItemEvent(evt)
  if evt.type == "GOTO_TAB" then
    self:tabOn(evt.tab or 1)
  elseif evt.type == "PURCHASE" then
    self:purchase(self:getSelectedPayChannel(), evt.data)
  end
end
function PaymentView:purchase(payChannel, paymentData)
  if device.platform == "ios" then
    local successCallback = function(resultCode, receipt)
      print("PaymentView iap  successCallback!!!!!!!!!!")
    end
    local failCallback = function(resultCode, errorInfo)
      print("errorCode:", resultCode)
      print("errorInfo:", errorInfo)
      print("PaymentView iap  failCallback!!!!!!!!!!")
    end
    local productId = paymentData.goodsId
    g.native:buyProduct(productId, successCallback, failCallback)
  else
    self._chipsPanel:showMiniLoading()
    local plugin = self._pluginManager:getPlugin(payChannel.channel)
    plugin:purchase(paymentData, handler(self, self._payResult))
  end
end
return PaymentView

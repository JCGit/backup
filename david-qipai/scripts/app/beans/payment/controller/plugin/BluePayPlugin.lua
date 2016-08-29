local PayChannePlugin = require("app.beans.payment.controller.plugin.PayChannePlugin")
local PayChannel = require("app.beans.payment.model.PayChannel")
local BluePayPlugin = class("BluePayPlugin", PayChannePlugin)
function BluePayPlugin:ctor()
  BluePayPlugin.super.ctor(self, "BluePayPlugin")
  if device.platform == "android" then
    self._nativeCaller = self:getNativeCaller("com/veragame/payment/bluepay/BluePayInterface")
  else
    function self._nativeCaller(method, param, sig)
      if method == "initialize" then
        self:_onSDKInitCallback("success")
      elseif method == "isInitialized" then
        return true
      end
    end
  end
end
function BluePayPlugin:initialize(config)
  if config and (config.channel == PayChannel.BLUE_PAY_TRUE_MONEY or config.channel == PayChannel.BLUE_PAY_12_CALL) then
    config.inputType = "singleLine"
  end
  self._config = config
  self._isActive = true
  self._isPurchasing = false
  local isSuccess, result = self._nativeCaller("isInitialized", {}, "()Z")
  if isSuccess and result then
    BluePayPlugin.isInitialized = true
  end
  if not BluePayPlugin.isInitialized and not BluePayPlugin.isInitializing then
    BluePayPlugin.isInitializing = true
    self._nativeCaller("setInitCallback", {
      handler(self, self._onSDKInitCallback)
    }, "(I)V")
    self._nativeCaller("initialize", {}, "()V")
  end
  self._products = self:parseConfig(config, function(category, json, product)
    product.priceLabel = string.format("%dTHB", product.price / 100)
    product.priceDollar = "THB"
    if self._config.channel == PayChannel.BLUE_PAY_TRUE_MONEY or self._config.channel == PayChannel.BLUE_PAY_12_CALL then
      product.buyButtonEnabled = false
    end
  end)
  self:_loadProcess()
end
function BluePayPlugin:autoDispose()
  self._isActive = false
  self._isLoadChipsRequested = false
  self._loadChipsCallback = nil
  self._purchaseCallback = nil
  self._nativeCaller("delayDispose", {60}, "(I)V")
end
function BluePayPlugin:loadChipsList(callback)
  self._loadChipsCallback = callback
  self._isLoadChipsRequested = true
  self:_loadProcess()
end
function BluePayPlugin:_loadProcess()
  if self._isLoadChipsRequested then
    self:updateDiscount(self._products, self._config)
    self:_doCallback(1, true, self._products.chips)
  else
    self:_doCallback(3, false)
  end
  if self._isLoadChipsRequested then
    self:updateDiscount(self._products, self._config)
    self:_doCallback(1, true, self._products.chips)
  else
    self:_doCallback(3, false)
  end
end
function BluePayPlugin:_doCallback(flag, isComplete, data)
  if self._isLoadChipsRequested and self._loadChipsCallback and (flag == 1 or flag == 3) then
    self._loadChipsCallback(self._config, isComplete, data)
  end
end
function BluePayPlugin:purchase(paymentData, callback, cardNo)
  if self._isPurchasing then
    g.ui.manager.tip:top(g.lang:getContent("payment.payingTip"))
    return
  end
  local isSuccess, result = self._nativeCaller("isInitialized", {}, "()Z")
  if isSuccess and result then
    BluePayPlugin.isInitialized = true
  end
  if BluePayPlugin.isInitialized then
    self._isPurchasing = true
    self._nativeCaller("setPayCallback", {
      handler(self, self._onPayCallback)
    }, "(I)V")
    self._purchaseCallback = callback
    local orderId
    local uid = tostring(g.vars.user.uid)
    if paymentData.channel == PayChannel.BLUE_PAY then
      orderId = uid .. "-" .. paymentData.paymentId .. "-" .. string.format("%x", math.floor(g.util.functions.getTime()))
      self._nativeCaller("payBySMS", {
        paymentData.paymentId,
        uid,
        paymentData.name,
        paymentData.price,
        orderId
      }, "(ILjava/lang/String;Ljava/lang/String;ILjava/lang/String;)V")
    elseif paymentData.channel == PayChannel.BLUE_PAY_TRUE_MONEY then
      orderId = uid .. "-" .. string.format("%x", math.floor(g.util.functions.getTime()))
      self._nativeCaller("payByTrueMoney", {
        paymentData.paymentId,
        uid,
        paymentData.name,
        cardNo,
        orderId
      }, "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")
    elseif paymentData.channel == PayChannel.BLUE_PAY_12_CALL then
      orderId = uid .. "-" .. string.format("%x", math.floor(g.util.functions.getTime()))
      self._nativeCaller("payBy12Call", {
        paymentData.paymentId,
        uid,
        paymentData.name,
        cardNo,
        orderId
      }, "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")
    end
  elseif BluePayPlugin.isInitializing then
    if not self._purchaseData then
      self._purchaseData = {}
      self._purchaseData.paymentData = paymentData
      self._purchaseData.callback = callback
      self._purchaseData.cardNo = cardNo
    end
  else
    BluePayPlugin.isInitializing = true
    self._nativeCaller("setInitCallback", {
      handler(self, self._onSDKInitCallback)
    }, "(I)V")
    self._nativeCaller("initialize", {}, "()V")
    self._purchaseData = {}
    self._purchaseData.paymentData = paymentData
    self._purchaseData.callback = callback
    self._purchaseData.cardNo = cardNo
  end
end
function BluePayPlugin:onInputCardInfo(input1, input2, submitButton, callback)
  local serial_no, pin_no
  local cardInput1 = input1:getText()
  local cardInput2 = input2:getText()
  cardInput1 = cardInput1 and string.trim(cardInput1) or nil
  cardInput2 = cardInput2 and string.trim(cardInput2) or nil
  if cardInput1 == "" then
    cardInput1 = nil
  end
  if cardInput2 == "" then
    cardInput2 = nil
  end
  if self._config.channel == PayChannel.BLUE_PAY_TRUE_MONEY then
    if not cardInput1 or string.len(cardInput1) ~= 20 then
      g.ui.manager.tip:top("กรุณากรอกข้อมูลที่ถูกต้องของรหัสเติมเงิน")
      callback(false)
      return
    end
    pin_no = cardInput1
  elseif self._config.channel == PayChannel.BLUE_PAY_12_CALL then
    if not cardInput1 or string.len(cardInput1) ~= 20 then
      g.ui.manager.tip:top("กรุณากรอกข้อมูลที่ถูกต้องของรหัสเติมเงิน")
      callback(false)
      return
    end
    pin_no = cardInput1
  end
  self:purchase(self._config.chips[1], callback, pin_no)
end
function BluePayPlugin:prepareEditBox(input1, input2, submitButton)
  if self._config.channel == PayChannel.BLUE_PAY_TRUE_MONEY then
    input1:setInputMode(kEditBoxInputModeNumeric)
    input1:setMaxLength(20)
    input1:setPlaceHolder("กรุณากรอกรหัสเติมเงินตรงนี้ค่ะ")
  elseif self._config.channel == PayChannel.BLUE_PAY_12_CALL then
    input1:setInputMode(kEditBoxInputModeNumeric)
    input1:setMaxLength(20)
    input1:setPlaceHolder("กรุณากรอกรหัสเติมเงินตรงนี้ค่ะ")
  end
end
function BluePayPlugin:_onSDKInitCallback(jsonString)
  if jsonString == "success" then
    BluePayPlugin.isInitialized = true
    if self._purchaseData then
      self:purchase(self._purchaseData.paymentData, self._purchaseData.callback, self._purchaseData.cardNo)
      self._purchaseData = nil
    end
  else
    g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
  end
  BluePayPlugin.isInitializing = false
end
function BluePayPlugin:_onPayCallback(result)
  self._isPurchasing = false
  local jsonData = json.decode(result)
  local isSuccess = jsonData and jsonData.ret == 0
  if isSuccess then
    g.ui.manager.tip:top(g.lang:getContent("payment.paySuccess"))
    self._purchaseCallback(true)
  else
    if self._purchaseCallback then
      self._purchaseCallback(false)
    end
    if jsonData.ret == 504 then
      g.ui.manager.tip:top(g.lang:getContent("payment.payFailDailyExceeded"))
    elseif jsonData.ret == 505 then
      g.ui.manager.tip:top(g.lang:getContent("payment.payFailMonthlyExceeded"))
    elseif jsonData.ret == 507 then
      g.ui.manager.tip:top(g.lang:getContent("payment.payFailTooOffen"))
    elseif jsonData.ret == 508 then
      g.ui.manager.tip:top(g.lang:getContent("payment.payFailDublicated"))
    elseif jsonData.ret == 509 then
      g.ui.manager.tip:top(g.lang:getContent("payment.payFailWrongCode"))
    elseif jsonData.ret == 510 then
      g.ui.manager.tip:top(g.lang:getContent("payment.payFailExpired"))
    elseif jsonData.ret == 511 then
      g.ui.manager.tip:top(g.lang:getContent("payment.payFailTooMuchWrong"))
    elseif jsonData.ret == 601 then
      g.ui.manager.tip:top(g.lang:getContent("payment.payFailInsufficientBalance"))
    else
      g.ui.manager.tip:top(g.lang:getContent("payment.payFail"))
    end
  end
end
return BluePayPlugin

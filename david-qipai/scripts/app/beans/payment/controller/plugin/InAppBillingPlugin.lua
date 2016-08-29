local PayChannePlugin = require("app.beans.payment.controller.plugin.PayChannePlugin")
local InAppBillingPlugin = class("InAppBillingPlugin", PayChannePlugin)
function InAppBillingPlugin:ctor()
  InAppBillingPlugin.super.ctor(self, "InAppBillingPlugin")
  if device.platform == "android" then
    self.nativeCaller = self:getNativeCaller("com/veragame/payment/iab/IabInterface")
    self.nativeCaller("setSetupCallback", {
      handler(self, self._onSetupCallback)
    }, "(I)V")
    self.nativeCaller("setLoadProductsCallback", {
      handler(self, self._onLoadProductsCallback)
    }, "(I)V")
    self.nativeCaller("setPurchaseCallback", {
      handler(self, self._onPurchaseCallback)
    }, "(I)V")
    self.nativeCaller("setDeliverCallback", {
      handler(self, self.doDeliver)
    }, "(I)V")
    self.nativeCaller("setConsumeCallback", {
      handler(self, self.onConsumeCallback)
    }, "(I)V")
  else
    function self.nativeCaller(method, param, sig)
      if method == "setup" then
        scheduler.performWithDelayGlobal(function()
          self:_onSetupCallback("true")
        end, 1)
      elseif method == "purchase" then
        scheduler.performWithDelayGlobal(function()
          self:_onPurchaseCallback("{\"sku\":\"com.joyours.pokdeng.fakepid\", \"originalJson\":\"{}\", \"signature\":\"fakesignature\"}")
        end, 1)
      elseif method == "loadProducts" then
        scheduler.performWithDelayGlobal(function()
          self:_onLoadProductsCallback(" [{\"sku\":\"com.joyours.pokdeng.vn.chip6m\",\"price\":\"21.000 đ\"}] ")
        end, 1)
      end
    end
  end
end
function InAppBillingPlugin:initialize(config)
  self._config = config
  self._isActive = true
  local isSuccess, result = self.nativeCaller("isSetup", {}, "()Z")
  if isSuccess then
    self._isSetup = result
  end
  isSuccess, result = self.nativeCaller("isSupported", {}, "()Z")
  if isSuccess then
    self._isSupported = result
  end
  if not self._isSetup then
    self._isSetuping = true
    self.nativeCaller("setup", {}, "()V")
  end
  if not self._products then
    logger:debug("remote config is loading..")
    self._products = self:parseConfig(config, function(category, json, product)
    end)
    self._isProductPriceLoaded = false
    self:_loadProcess()
  end
end
function InAppBillingPlugin:autoDispose()
  self._isActive = false
  self._isProductPriceLoaded = false
  self._isProductRequesting = false
  self._isLoadChipsRequested = false
  self._isLoadPropRequested = false
  self._loadChipsCallback = nil
  self._loadPropCallback = nil
  self._purchaseCallback = nil
  self.nativeCaller("delayDispose", {60}, "(I)V")
end
function InAppBillingPlugin:loadChipsList(callback)
  self._loadChipsCallback = callback
  self._isLoadChipsRequested = true
  self:_loadProcess()
end
function InAppBillingPlugin:loadPropList(callback)
  self._loadPropCallback = callback
  self._isLoadPropRequested = true
  self:_loadProcess()
end
function InAppBillingPlugin:purchase(paymentData, callback)
  self._purchaseCallback = callback
  local orderId = ""
  local uid = tostring(g.vars.user.uid) or ""
  local channel = paymentData.channel or ""
  local goodsId = paymentData.goodsId or ""
  local paymentId = paymentData.paymentId or ""
  self.nativeCaller("purchase", {
    orderId,
    paymentId,
    goodsId,
    uid,
    channel
  }, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")
end
function InAppBillingPlugin:_loadProcess()
  if not self._products then
    logger:debug("remote config is loading..")
    self:cacheConfig(self._config.configURL, handler(self, self._onConfigLoaded))
  end
  if self._isSetup then
    if self._isSupported then
      if self._isLoadChipsRequested or self._isLoadPropRequested then
        if self._products then
          if self._isProductPriceLoaded then
            self:updateDiscount(self._products, self._config)
            self:_doCallback(1, true, self._products.chips)
            self:_doCallback(2, true, self._products.props)
          elseif not self._isProductRequesting then
            self._isProductRequesting = true
            local joinedSkuList = table.concat(self._products.skus, ",")
            logger:debug("start loading price...")
            self:_doCallback(3, false)
            self.nativeCaller("loadProducts", {joinedSkuList}, "(Ljava/lang/String;)V")
          else
            self:_doCallback(3, false)
          end
        else
          self:_doCallback(3, false)
        end
      end
    else
      logger:debug("iab not supported")
      self:_doCallback(3, false, g.lang:getContent("payment.notSupportPay"))
    end
  elseif self._isSetuping then
    logger:debug("setuping ...")
    self:_doCallback(3, false)
  else
    self._isSetuping = true
    logger:debug("start setup..")
    self:_doCallback(3, false)
    self.nativeCaller("setup", {}, "()V")
  end
end
function InAppBillingPlugin:_onSetupCallback(isSupported)
  logger:debug("setup complete.")
  self._isSetuping = false
  self._isSetup = true
  self._isSupported = isSupported == "true"
  logger:debug("isSupported raw:", isSupported)
  self:_loadProcess()
end
function InAppBillingPlugin:_onLoadProductsCallback(jsonString)
  logger:debug("price load complete -> " .. jsonString)
  local isSuccess = jsonString ~= "fail"
  self._isProductRequesting = false
  if isSuccess then
    local products = json.decode(jsonString)
    if products then
      for i, prd in ipairs(products) do
        if self._products.chips then
          for j, chip in ipairs(self._products.chips) do
            if prd.sku == chip.paymentId then
              chip.priceLabel = prd.price
              if prd.priceNum and prd.priceDollar then
                chip.priceNum = prd.priceNum
                chip.priceDollar = prd.priceDollar
              end
            end
          end
        end
      end
      self._isProductPriceLoaded = true
      self:_loadProcess()
      return
    end
  end
  self:_doCallback(3, false, g.lang:getContent("common.badNetwork"))
end
function InAppBillingPlugin:_onPurchaseCallback(jsonString)
  logger:debug("purchase complete -> ", jsonString)
  local isSuccess = string.sub(jsonString, 1, 4) ~= "fail"
  if isSuccess then
    g.ui.manager.tip:top(g.lang:getContent("payment.paySuccess"))
    local json = json.decode(jsonString)
    self:deliver(json.sku, json.originalJson, json.signature, true)
  elseif string.sub(jsonString, 6) == "canceled" then
    g.ui.manager.tip:top(g.lang:getContent("payment.payCancel"))
    if self._purchaseCallback then
      self._purchaseCallback(false, "canceled")
    end
  else
    g.ui.manager.tip:top(g.lang:getContent("payment.payFail"))
    if self._purchaseCallback then
      self._purchaseCallback(false, "error")
    end
  end
end
function InAppBillingPlugin:doDeliver(jsonString)
  logger:debug("doDeliver ", jsonString)
  local json = json.decode(jsonString)
  self:deliver(json.sku, json.originalJson, json.signature, false)
end
function InAppBillingPlugin:onConsumeCallback(jsonString)
  logger:debug("onConsumeCallback", jsonString)
end
function InAppBillingPlugin:_doCallback(flag, isComplete, data)
  if self._isLoadChipsRequested and self._loadChipsCallback and (flag == 1 or flag == 3) then
    self._loadChipsCallback(self._config, isComplete, data)
  end
  if self._isLoadPropRequested and self._loadPropCallback and (flag == 2 or flag == 3) then
    self._loadPropCallback(self._config, isComplete, data)
  end
end
function InAppBillingPlugin:_onConfigLoaded(succ, content)
  if succ then
    logger:debug("remote config file loaded.")
    self._products = self:parseConfig(content, function(category, json, product)
    end)
    self._isProductPriceLoaded = false
    self:_loadProcess()
  else
    logger:debug("remote config file load failed.")
    self:_doCallback(3, true, g.lang:getContent("common.badNetwork"))
  end
end
function InAppBillingPlugin:deliver(sku, receipt, signature, isTip)
  local retryTimes = 6
  local deliverFunc
  function deliverFunc()
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.receipt = receipt
    requestData.signature = signature
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("pay", "deliver", requestData), function(data)
      logger:debug("Iab deliver-" .. data)
      local jsonData = json.decode(data)
      if jsonData then
        if jsonData.ret == 0 then
          self.nativeCaller("consume", {sku}, "(Ljava/lang/String;)V")
          logger:debug("deliver isSuccess, consume it")
          if isTip then
            g.ui.manager.tip:top(g.lang:getContent("payment.deliverSuccess"))
          end
          if self._purchaseCallback then
            self._purchaseCallback(true)
          end
        elseif jsonData.ret == -6 then
          self.nativeCaller("consume", {sku}, "(Ljava/lang/String;)V")
          if self._purchaseCallback then
            self._purchaseCallback(false)
          end
        elseif jsonData.ret == -5 then
          self.nativeCaller("consume", {sku}, "(Ljava/lang/String;)V")
          if self._purchaseCallback then
            self._purchaseCallback(false)
          end
        end
      elseif self._purchaseCallback then
        self._purchaseCallback(false)
      end
    end, function()
      retryTimes = retryTimes - 1
      if retryTimes > 0 then
        scheduler.performWithDelayGlobal(function()
          deliverFunc()
        end, 10)
      elseif isTip then
        g.ui.manager.tip:top(g.lang:getContent("payment.deliverFail"))
        if self._purchaseCallback then
          self._purchaseCallback(false, "error")
        end
      end
    end)
  end
  deliverFunc()
end
return InAppBillingPlugin

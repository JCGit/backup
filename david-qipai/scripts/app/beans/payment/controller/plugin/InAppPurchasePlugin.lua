local PayChannePlugin = require("app.beans.payment.controller.plugin.PayChannePlugin")
local InAppPurchasePlugin = class("InAppPurchasePlugin", PayChannePlugin)
function InAppPurchasePlugin:ctor()
  InAppPurchasePlugin.super.ctor(self, "InAppPurchasePlugin")
  self._appStore = AppStore.new()
  self._appStore:addEventListener(AppStore.LOAD_PRODUCTS_FINISHED, handler(self, self.loadProductFinished_))
  self._appStore:addEventListener(AppStore.TRANSACTION_PURCHASED, handler(self, self._transactionPurchased))
  self._appStore:addEventListener(AppStore.TRANSACTION_RESTORED, handler(self, self._transactionRestored))
  self._appStore:addEventListener(AppStore.TRANSACTION_FAILED, handler(self, self._transactionFailed))
  self._appStore:addEventListener(AppStore.TRANSACTION_UNKNOWN_ERROR, handler(self, self._transactionUnkownError))
end
function InAppPurchasePlugin:initialize(config)
  self._isActive = true
  self._config = config
  self._isSupported = self._appStore:canMakePurchases()
  if not self._products then
    logger:debug("remote config is loading..")
    self:cacheConfig(self._config.configURL, handler(self, self._onConfigLoaded))
  end
  self._appStore:restore()
end
function InAppPurchasePlugin:autoDispose()
  self._isActive = false
  self._isLoadChipsRequested = false
  self._isLoadPropRequested = false
  self._loadChipsCallback = nil
  self._loadPropCallback = nil
  self._purchaseCallback = nil
  self._isProductPriceLoaded = false
  self._isProductRequesting = false
end
function InAppPurchasePlugin:loadChipsList(callback)
  self._loadChipsCallback = callback
  self._isLoadChipsRequested = true
  self:_loadProcess()
end
function InAppPurchasePlugin:loadPropList(callback)
  self._loadPropCallback = callback
  self._isLoadPropRequested = true
  self:_loadProcess()
end
function InAppPurchasePlugin:_loadProcess()
  if not self._isSupported then
    logger:debug("iap not supported")
    self:_doCallback(3, true, g.lang:getContent("payment.notSupportPay"))
  else
    if not self._products then
      logger:debug("remote config is loading..")
      self:cacheConfig(self._config.configURL, handler(self, self._onConfigLoaded))
    end
    if self._isLoadChipsRequested or self._isLoadPropRequested then
      if self._products then
        if self._isProductPriceLoaded then
          self:updateDiscount(self._products, self._config)
          self:_doCallback(1, true, self._products.chips)
          self:_doCallback(2, true, self._products.props)
        elseif not self._isProductRequesting then
          self._isProductRequesting = true
          logger:debug("start loading price...")
          self:_doCallback(3, false)
          self._appStore:loadProducts(self._products.skus)
        else
          self:_doCallback(3, false)
        end
      else
        self:_doCallback(3, false)
      end
    else
      self:_doCallback(3, false)
    end
  end
end
function InAppPurchasePlugin:_doCallback(flag, isComplete, data)
  if self._isLoadChipsRequested and self._loadChipsCallback and (flag == 1 or flag == 3) then
    self._loadChipsCallback(self._config, isComplete, data)
  end
  if self._isLoadPropRequested and self._loadPropCallback and (flag == 2 or flag == 3) then
    self._loadPropCallback(self._config, isComplete, data)
  end
end
function InAppPurchasePlugin:_onConfigLoaded(succ, content)
  if succ then
    logger:debug("remote config file loaded.")
    self._products = self:parseConfig(content, function(category, json, product)
    end)
    self:_loadProcess()
  else
    logger:debug("remote config file load failed.")
    self:_doCallback(3, true, g.lang:getContent("common.badNetwork"))
  end
end
function InAppPurchasePlugin:makePurchase(pid, callback)
  self._purchaseCallback = callback
  self._appStore:purchaseProduct(pid)
end
function InAppPurchasePlugin:loadProductFinished_(evt)
  self._isProductRequesting = false
  local getPriceLabel = function(prd)
    return luaoc.callStaticMethod("NativeiPhone", "getPriceLabel", {
      priceLocale = prd.priceLocale,
      price = prd.price
    })
  end
  if evt.products and #evt.products > 0 then
    for i, prd in ipairs(evt.products) do
      if self._products.chips then
        for j, chip in ipairs(self._products.chips) do
          if prd.productIdentifier == chip.pid then
            local isSuccess, price = getPriceLabel(prd)
            if isSuccess then
              chip.priceLabel = price
            else
              chip.priceLabel = prd.price
            end
            chip.priceNum = prd.price
          end
        end
      end
      if self._products.props then
        for j, prop in ipairs(self._products.props) do
          if prd.productIdentifier == prop.pid then
            local isSuccess, price = getPriceLabel(prd)
            if isSuccess then
              prop.priceLabel = price
            else
              prop.priceLabel = prd.price
            end
            prop.priceNum = prd.price
          end
        end
      end
    end
    self._isProductPriceLoaded = true
    self:_loadProcess()
    return
  end
  self:_doCallback(3, true, g.lang:getContent("common.badNetwork"))
end
function InAppPurchasePlugin:_transactionPurchased(evt)
  g.ui.manager.tip:top(g.lang:getContent("payment.paySuccess"))
  self:delivery(evt.transaction, true)
end
function InAppPurchasePlugin:_transactionRestored(evt)
  self:delivery(evt.transaction, false)
end
function InAppPurchasePlugin:_transactionFailed(evt)
  g.ui.manager.tip:top(g.lang:getContent("payment.payFail"))
  if self._purchaseCallback then
    self._purchaseCallback(false, "ERROR")
  end
end
function InAppPurchasePlugin:_transactionUnkownError(evt)
  g.ui.manager.tip:top(g.lang:getContent("payment.payFail"))
  if self._purchaseCallback then
    self._purchaseCallback(false, "ERROR")
  end
end
function InAppPurchasePlugin:delivery(transaction, isTip)
  local retryTimes = 6
  local deliveryFunc
  function deliveryFunc()
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.receipt = crypto.encodeBase64(transaction.receipt)
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("pay", "deliver", requestData), function(data)
      local jsonData = json.decode(ret)
      if jsonData and tonumber(jsonData.ret) == 0 then
        logger:debug("dilivery success, consume it")
        self._appStore:finishTransaction(transaction)
        if isTip then
          g.ui.manager.tip:top(g.lang:getContent("payment.deliverSuccess"))
          if self._purchaseCallback then
            self._purchaseCallback(true)
          end
        end
      else
        logger:debug("delivery failed => " .. ret)
        retryTimes = retryTimes - 1
        if retryTimes > 0 then
          scheduler.performWithDelayGlobal(function()
            deliveryFunc()
          end, 10)
        elseif isTip then
          g.ui.manager.tip:top(g.lang:getContent("payment.deliverFail"))
          if self._purchaseCallback then
            self._purchaseCallback(false, "error")
          end
        end
      end
    end, function()
      retryTimes = retryTimes - 1
      if retryTimes > 0 then
        scheduler.performWithDelayGlobal(function()
          deliveryFunc()
        end, 10)
      elseif isTip then
        g.ui.manager.tip:top(g.lang:getContent("payment.deliverFail"))
        if self._purchaseCallback then
          self._purchaseCallback(false, "error")
        end
      end
    end)
  end
  deliveryFunc()
end
return InAppPurchasePlugin

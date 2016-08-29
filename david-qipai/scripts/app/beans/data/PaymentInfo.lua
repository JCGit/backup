local PaymentInfo = class("PaymentInfo")
local PAYMENT = {}
function PaymentInfo:ctor()
  self._isPaymentRequesting = false
  self.isNeedRefresh = true
end
function PaymentInfo:getPaymentById(paymentId)
  return PAYMENT[tonumber(paymentId)]
end
local loadPaymentData
function PaymentInfo:loadPaymentData(success, fail)
  if not self._paymentData then
    if not self._isPaymentRequesting then
      do
        local retryTimes = 3
        function loadPaymentData()
          if not io.exists(device.writablePath .. "upd") then
            lfs.mkdir(device.writablePath .. "upd")
          end
          local cacheFileName = device.writablePath .. "upd" .. "/" .. "paymentsCache"
          local file = io.open(cacheFileName, "rb")
          if file ~= nil then
            local data = file:read("*all")
            print(data)
            if data ~= nil then
              local jsonData = json.decode(data)
              if jsonData ~= nil and jsonData.paymentsJson == g.vars.user.paymentsJson then
                self._paymentData = jsonData.paymentData
                self:processPaymentData()
                self._isPaymentRequesting = false
                success()
                return
              end
            end
          end
          local requestData = {}
          g.httpRequest:getMethod(g.requestParamsFactory:createGETRequest(g.vars.user.cdn .. "/" .. g.vars.user.paymentsJson, requestData, "PaymentInfo/url"), function(data)
            local jsonData = json.decode(data)
            self._paymentData = jsonData
            if not io.exists(device.writablePath .. "upd") then
              lfs.mkdir(device.writablePath .. "upd")
            end
            local cacheFileName = device.writablePath .. "upd" .. "/" .. "paymentsCache"
            local jsonData = {}
            jsonData.paymentsJson = g.vars.user.paymentsJson
            jsonData.paymentData = self._paymentData
            io.writefile(cacheFileName, json.encode(jsonData))
            self:processPaymentData()
            success()
          end, function()
            retryTimes = retryTimes - 1
            if retryTimes > 0 then
              loadPaymentData()
            else
              g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
            end
          end)
        end
        loadPaymentData()
      end
    end
  else
    success()
  end
end
function PaymentInfo:processPaymentData()
  if self._paymentData then
    for key, value in pairs(self._paymentData) do
      if value.chips then
        for i = 1, #value.chips do
          PAYMENT[tonumber(value.chips[i].paymentId)] = value.chips[i]
        end
      end
    end
  end
end
function PaymentInfo:getPaymentData()
  return self._paymentData
end
function PaymentInfo:loadOrderData(success, fail)
  if self._isOrderRequesting then
    return
  end
  if self._orderData and not self.isNeedRefresh then
    success()
  elseif self._isOrdersLoading then
  else
    self._isOrdersLoading = true
    self:loadPaymentData(handler(self, function(self)
      local requestData = {}
      requestData.uid = g.vars.user.uid
      requestData.channel = g.vars.user.channel
      g.httpRequest:postMethod(g.requestParamsFactory:createRequest("pay", "history", requestData), function(data)
        self._isOrdersLoading = false
        local jsonData = json.decode(data)
        self._orderData = jsonData.info
        for i = 1, #self._orderData do
          local paymentData = g.paymentInfo:getPaymentById(tonumber(self._orderData[i].paymentId))
          if paymentData then
            self._orderData[i].pic = paymentData.pic
            self._orderData[i].name = paymentData.name
          end
        end
        success()
      end, function()
        self._isOrdersLoading = false
        fail()
      end)
    end), handler(self, function(self)
      fail()
    end))
  end
end
function PaymentInfo:getOrderData()
  return self._orderData
end
function PaymentInfo:loadDiscountData(success, fail)
  if self._isDiscountRequesting then
    return
  end
  if self._isDiscountRequesting then
  else
    self._isDiscountRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("pay", "discount", requestData), function(data)
      self._isDiscountRequesting = false
      local jsonData = json.decode(data)
      success(jsonData)
    end, function()
      self._isDiscountRequesting = false
      fail()
    end)
  end
end
return PaymentInfo

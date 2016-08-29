local PayChannePlugin = class("PayChannePlugin")
local DEFAULT_CACHE_DIR = device.writablePath .. "cache" .. device.directorySeparator .. "remotedata" .. device.directorySeparator
g.util.functions.mkdir(DEFAULT_CACHE_DIR)
function PayChannePlugin:ctor(name)
end
function PayChannePlugin:initialize(config)
end
function PayChannePlugin:autoDispose()
end
function PayChannePlugin:loadChipsList(callback)
end
function PayChannePlugin:loadPropList(callback)
end
function PayChannePlugin:makePurchase(paymentId, callback)
end
function PayChannePlugin:prepareEditBox(input1, input2, submitButton)
end
function PayChannePlugin:onInputCardInfo(input1, input2, submitButton, callback)
end
function PayChannePlugin:getNativeCaller(nativeClassName)
  return function(nativeMethodName, nativeParams, nativeMethodSig)
    if device.platform == "android" then
      local isSuccess, result = luaj.callStaticMethod(nativeClassName, nativeMethodName, nativeParams, nativeMethodSig)
      if not isSuccess then
        if result == -1 then
          print("call %s failed, -1 不支持的参数类型或返回值类型", nativeMethodName)
        elseif result == -2 then
          print("call %s failed, -2 无效的签名", nativeMethodName)
        elseif result == -3 then
          print("call %s failed, -3 没有找到指定的方法", nativeMethodName)
        elseif result == -4 then
          print("call %s failed, -4 Java 方法执行时抛出了异常", nativeMethodName)
        elseif result == -5 then
          print("call %s failed, -5 Java 虚拟机出错", nativeMethodName)
        elseif result == -6 then
          print("call %s failed, -6 Java 虚拟机出错", nativeMethodName)
        end
      end
      return isSuccess, result
    else
      print("call %s failed, not in android platform", nativeMethodName)
      return false, nil
    end
  end
end
function PayChannePlugin:cacheConfig(url, callback)
  self._cacheConfigCallback = callback
  if not self._isCachingConfig then
    self._isCachingConfig = true
    do
      local filename = crypto.md5(url)
      local filepath = DEFAULT_CACHE_DIR .. filename
      if io.exists(filepath) then
        local content = io.readfile(filepath)
        if self._cacheConfigCallback then
          self._cacheConfigCallback(true, content)
          self._cacheConfigCallback = nil
        end
        self._isCachingConfig = false
      else
        do
          local loadContent
          local retryTimes = 3
          function loadContent()
            local requestData = {}
            requestData.uid = g.vars.user.uid
            g.httpRequest:getMethod(g.requestParamsFactory:createGETRequest(url, requestData, "PayChannePlugin/url"), function(data)
              if data then
                io.writefile(filepath, data, "w+")
                if self._cacheConfigCallback then
                  self._cacheConfigCallback(true, data)
                  self._cacheConfigCallback = nil
                end
                self._isCachingConfig = false
              else
                retryTimes = retryTimes - 1
                if retryTimes > 0 then
                  loadContent()
                else
                  if self._cacheConfigCallback then
                    self._cacheConfigCallback(false)
                    self._cacheConfigCallback = nil
                  end
                  self._isCachingConfig = false
                end
              end
            end, function()
              retryTimes = retryTimes - 1
              if retryTimes > 0 then
                loadContent()
              else
                if self._cacheConfigCallback then
                  self._cacheConfigCallback(false)
                  self._cacheConfigCallback = nil
                end
                self._isCachingConfig = false
              end
            end)
          end
          loadContent()
        end
      end
    end
  end
end
function PayChannePlugin:parsePrice(priceString)
  local s, e = string.find(priceString, "%d")
  local partDollar, partNumber, partNumberLen
  if s <= 1 then
    local lastNumIdx = 1
    while true do
      local st, ed = string.find(priceString, "%d", lastNumIdx + 1)
      if ed then
        lastNumIdx = ed
      else
        partDollar = string.sub(priceString, lastNumIdx + 1)
        partNumber = string.sub(priceString, 1, lastNumIdx)
        partNumberLen = string.len(partNumber)
        break
      end
    end
  else
    partDollar = string.sub(priceString, 1, s - 1)
    partNumber = string.sub(priceString, s)
    partNumberLen = string.len(partNumber)
  end
  local price = 0
  local split, dot = "", ""
  local s1, e1 = string.find(partNumber, "%priceString")
  if s1 then
    local firstP = string.sub(partNumber, s1, e1)
    local s2, e2 = string.find(partNumber, "%priceString", s1 + 1)
    if s2 then
      local secondP = string.sub(partNumber, s2, e2)
      if firstP == secondP then
        split = firstP
        local str = string.gsub(partNumber, "%" .. firstP, "")
        local sdb, sde = string.find(str, "%priceString")
        if sdb then
          dot = string.sub(str, sdb, sde)
          str = string.gsub(str, "%" .. dot, ".")
        end
        price = tonumber(str)
      else
        split = firstP
        dot = secondP
        local str = string.gsub(partNumber, "%" .. split, "")
        str = string.gsub(str, "%" .. dot, ".")
        price = tonumber(str)
      end
    elseif string.sub(partNumber, 1, s1 - 1) == "0" then
      dot = firstP
      local str = string.gsub(partNumber, "%" .. firstP, ".")
      price = tonumber(str)
    elseif partNumberLen == e1 + 3 then
      split = firstP
      local str = string.gsub(partNumber, "%" .. firstP, "")
      price = tonumber(str)
    elseif partNumberLen <= e1 + 2 then
      dot = firstP
      local str = string.gsub(partNumber, "%" .. firstP, ".")
      price = tonumber(str)
    elseif firstP == "," then
      split = firstP
      local str = string.gsub(partNumber, "%" .. firstP, "")
      price = tonumber(str)
    elseif firstP == "." then
      dot = firstP
      local str = string.gsub(partNumber, "%" .. firstP, ".")
      price = tonumber(str)
    else
      split = firstP
      local str = string.gsub(partNumber, "%" .. firstP, "")
      price = tonumber(str)
    end
  else
    price = tonumber(partNumber)
  end
  return partDollar, price, split, dot
end
function PayChannePlugin:parseConfig(config, itemCallback)
  local result = {}
  result.skus = {}
  if config.chips then
    local chips = {}
    result.chips = chips
    for i = 1, #config.chips do
      local chip = config.chips[i]
      local product = {}
      product.paymentId = chip.paymentId
      product.goodsId = chip.goodsId
      product.price = chip.price
      product.pic = chip.pic
      product.name = chip.name
      product.chips = chip.chips
      product.chipsMore = chip.chipsMore
      product.status = chip.status
      product.channel = config.channel
      if itemCallback then
        itemCallback("chips", chip, product)
      end
      table.insert(chips, product)
      table.insert(result.skus, product.goodsId)
    end
  end
  return result
end
function PayChannePlugin:updateDiscount(products, payChannelConfig)
  if not products or not payChannelConfig then
    return
  end
  local itemDiscount = payChannelConfig.discount or {}
  local chipDiscount = payChannelConfig.chipDiscount or 1
  local coinDiscount = payChannelConfig.coinDiscount or 1
  if products.chips then
    for i, chip in ipairs(products.chips) do
      local byIdDiscount = itemDiscount[chip.paymentId] and itemDiscount[chip.paymentId].dis
      local byIdDiscountExpire = itemDiscount[chip.paymentId] and itemDiscount[chip.paymentId].expire
      chip.discount = byIdDiscount or chipDiscount
      chip.discountExpire = byIdDiscountExpire
      if not chip.priceLabel then
        chip.priceLabel = string.gsub(g.lang:getContent("payment.priceFormat"), "{pay}", chip.price / 100)
      end
      local partDollar, price
      if chip.price and chip.priceDollar then
        partDollar = chip.priceDollar
        price = chip.price
      elseif not chip.price then
        partDollar, price = self:parsePrice(chip.priceLabel)
        chip.price = price
        chip.priceDollar = partDollar
      else
        price = chip.price
        chip.priceDollar = self:parsePrice(chip.priceLabel)
      end
      if chip.discount ~= 1 then
        chip.rate = chip.chips * chip.discount / price
        chip.chipsOff = chip.chips * chip.discount
      else
        chip.rate = chip.chips / price
      end
      chip.rate = tonumber(string.format("%.2f", chip.rate))
    end
  end
end
return PayChannePlugin

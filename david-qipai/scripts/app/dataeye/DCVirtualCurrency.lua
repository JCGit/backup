DCVirtualCurrency = {}
function DCVirtualCurrency.paymentSuccess(orderId, currencyAmount, currencyType, paymentType)
  DCLuaVirtualCurrency:paymentSuccess(orderId, currencyAmount, currencyType, paymentType)
end
function DCVirtualCurrency.paymentSuccessInLevel(orderId, currencyAmount, currencyType, paymentType, levelId)
  DCLuaVirtualCurrency:paymentSuccessInLevel(orderId, currencyAmount, currencyType, paymentType, levelId)
end
return DCVirtualCurrency

DCItem = {}
function DCItem.buy(itemId, itemType, itemCount, virtualCurrency, currencyType, consumePoint)
  DCLuaItem:buy(itemId, itemType, itemCount, virtualCurrency, currencyType, consumePoint)
end
function DCItem.buyInLevel(itemId, itemType, itemCount, virtualCurrency, currencyType, consumePoint, levelId)
  DCLuaItem:buyInLevel(itemId, itemType, itemCount, virtualCurrency, currencyType, consumePoint, levelId)
end
function DCItem.get(itemId, itemType, itemCount, reason)
  DCLuaItem:get(itemId, itemType, itemCount, reason)
end
function DCItem.getInLevel(itemId, itemType, itemCount, reason, levelId)
  DCLuaItem:getInLevel(itemId, itemType, itemCount, reason, levelId)
end
function DCItem.consume(itemId, itemType, itemCount, reason)
  DCLuaItem:consume(itemId, itemType, itemCount, reason)
end
function DCItem.consumeInLevel(itemId, itemType, itemCount, reason, levelId)
  DCLuaItem:consumeInLevel(itemId, itemType, itemCount, reason, levelId)
end
return DCItem

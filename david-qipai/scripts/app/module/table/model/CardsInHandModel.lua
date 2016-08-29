local CardsInHandModel = class("CardsInHandModel")
CardsInHandModel.UNKNOWN_VALUE = 0
function CardsInHandModel:ctor()
  self._cardCount = 0
  self._cards = {}
  self._cardType = -1
  self._taem = -1
  self._deng = -1
end
function CardsInHandModel:setCards(cards)
  self._cards = cards
end
function CardsInHandModel:getCards()
  return self._cards
end
function CardsInHandModel:addCardWithValue(cardValue, cardType, taem, deng)
  self:setCardType(cardType)
  self:setTaem(taem)
  self:setDeng(deng)
  table.insert(self._cards, cardValue)
end
function CardsInHandModel:addCardInHandWithoutValue()
  table.insert(self._cards, CardsInHandModel.UNKNOWN_VALUE)
end
function CardsInHandModel:setCardType(cardType)
  self._cardType = cardType
end
function CardsInHandModel:getCardCount()
  return table.maxn(self._cards)
end
function CardsInHandModel:getCardType()
  return self._cardType
end
function CardsInHandModel:setTaem(taem)
  self._taem = taem
end
function CardsInHandModel:getTaem()
  return self._taem
end
function CardsInHandModel:setDeng(deng)
  self._deng = deng
end
function CardsInHandModel:getDeng()
  return self._deng
end
return CardsInHandModel

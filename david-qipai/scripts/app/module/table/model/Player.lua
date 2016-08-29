local Player = class("Player")
local PlayerConstants = import(".PlayerConstants")
local CardsInHandModel = import(".CardsInHandModel")
function Player:ctor(user)
  self.uid = ""
  self._userName = ""
  self._sPic = ""
  self._gender = 0
  self.seatId = -1
  self._giftId = -1
  self._buyInChips = 2000
  self._seatChips = -1
  self._userChips = -1
  self._deltaChips = 0
  self._feeChips = 0
  self._playStatus = -1
  self._bet = -1
  self._isFlipedCard = 0
  self._pokState = 0
  self._cardsInHandModel = CardsInHandModel.new()
  self._chooseType = 1
  self._role = PlayerConstants.PlayerRole.VISITER
  self._state = ""
  self._isSelf = false
  self._isBanker = false
  self._minBet = 100
  self._maxBet = 1000
  self._defaultBet = 20
end
function Player:requestEnterTable(judge)
  judge:asyncRequestEnterTable(self)
end
function Player:reqeustAsBanker(judge)
  logger:debug("Player:用户" .. self.uid .. " 申请成为庄家")
  judge:asyncRequestBankerCampaign(self)
end
function Player:setUid(uid)
  self.uid = uid
end
function Player:getUid()
  return self.uid
end
function Player:setUserName(userName)
  self._userName = userName
end
function Player:getUserName()
  return self._userName
end
function Player:setSPic(sPic)
  self._sPic = sPic
end
function Player:getSPic()
  return self._sPic
end
function Player:setGender(gender)
  self._gender = tonumber(gender)
end
function Player:getGender()
  return self._gender
end
function Player:setSeatId(seatId)
  self.seatId = seatId
end
function Player:getSeatId()
  return self.seatId
end
function Player:getPokState()
  return self._pokState
end
function Player:setPokState(pok)
  self._pokState = pok
end
function Player:getSeatIdForServer()
  return self.seatId - 1
end
function Player:setSeatIdFromServer(seatId)
  self.seatId = seatId + 1
end
function Player:setGiftId(giftId)
  self._giftId = giftId
end
function Player:getGiftId()
  return self._giftId
end
function Player:setBuyInChips(buyInChips)
  self._buyInChips = buyInChips
end
function Player:getBuyInChips()
  return self._buyInChips
end
function Player:setIsAutoBuyIn(isAutoBuyIn)
  self._isAutoBuyIn = isAutoBuyIn
end
function Player:getIsAutoBuyIn()
  return self._isAutoBuyIn
end
function Player:setSeatChips(seatChips)
  self._seatChips = seatChips
end
function Player:getSeatChips()
  return self._seatChips
end
function Player:setUserChips(userChips)
  self._userChips = userChips
end
function Player:getUserChips()
  return self._userChips
end
function Player:setDeltaChips(deltaChips)
  self._deltaChips = deltaChips
end
function Player:getDeltaChips()
  return self._deltaChips
end
function Player:setFeeChips(feeChips)
  self._feeChips = feeChips
end
function Player:getFeeChips()
  return self._feeChips
end
function Player:setPlayStatus(playStatus)
  self._playStatus = playStatus
end
function Player:getPlayStatus()
  return self._playStatus
end
function Player:setChooseType(chooseType)
  self._chooseType = chooseType
end
function Player:getChooseType()
  return self._chooseType
end
function Player:setBet(bet)
  self._bet = bet
end
function Player:getBet()
  return self._bet
end
function Player:getCardCount()
  return self._cardsInHandModel:getCardCount()
end
function Player:setIsFlipedCard(isFlipedCard)
  self._isFlipedCard = isFlipedCard
end
function Player:getIsFlipedCard()
  if self._isFlipedCard == nil then
    return 0
  end
  return self._isFlipedCard
end
function Player:isCardValueVisible()
  local cards = self._cardsInHandModel:getCards()
  for i, cardValue in ipairs(cards) do
    if cardValue ~= CardsInHandModel.UNKNOWN_VALUE then
      return true
    end
  end
  return false
end
function Player:setCardsInHandModel(cardsInHandModel)
  self._cardsInHandModel = cardsInHandModel
end
function Player:getCardsInHandModel()
  return self._cardsInHandModel
end
function Player:getCardsInHandModelDetailDescription()
  local detailString = ""
  if self._cardsInHandModel ~= nil then
    local cardType = self._cardsInHandModel:getCardType()
    local taem = self._cardsInHandModel:getTaem()
    local deng = self._cardsInHandModel:getDeng()
    if cardType == 90 then
      detailString = g.lang:getContent("table.cardTypePok")
    elseif cardType == 80 then
      detailString = g.lang:getContent("table.cardTypeStraihtFlush")
    elseif cardType == 70 then
      detailString = g.lang:getContent("table.cardTypeTong")
    elseif cardType == 60 then
      detailString = g.lang:getContent("table.cardTypeSamLueang")
    elseif cardType == 50 then
      detailString = g.lang:getContent("table.cardTypeStraiht")
    elseif cardType == 40 then
    end
    local valueString = string.gsub(g.lang:getContent("table.cardTaem"), "{taem}", tostring(taem))
    detailString = detailString .. " " .. valueString
    if deng > 1 then
      local dengString = string.gsub(g.lang:getContent("table.cardDeng"), "{deng}", tostring(deng))
      detailString = detailString .. " " .. dengString
    end
  end
  return detailString
end
function Player:getCardsTypeDescription()
  local detailString = ""
  if self._cardsInHandModel ~= nil then
    local cardType = self._cardsInHandModel:getCardType()
    local taem = self._cardsInHandModel:getTaem()
    local deng = self._cardsInHandModel:getDeng()
    if cardType == 90 then
      detailString = g.lang:getContent("table.cardTypePok")
    elseif cardType == 80 then
      detailString = g.lang:getContent("table.cardTypeStraihtFlush")
    elseif cardType == 70 then
      detailString = g.lang:getContent("table.cardTypeTong")
    elseif cardType == 60 then
      detailString = g.lang:getContent("table.cardTypeSamLueang")
    elseif cardType == 50 then
      detailString = g.lang:getContent("table.cardTypeStraiht")
    elseif cardType == 40 then
      local valueString = string.gsub(g.lang:getContent("table.cardTaem"), "{taem}", tostring(taem))
      detailString = detailString .. " " .. valueString
    end
  end
  return detailString
end
function Player:addCardInHandWithValue(cardValue, cardType, taem, deng)
  return self._cardsInHandModel:addCardWithValue(cardValue, cardType, taem, deng)
end
function Player:addCardInHandWithoutValue()
  self._cardsInHandModel:addCardInHandWithoutValue()
end
function Player:clearHandData()
  local cardsInHandModel = CardsInHandModel.new()
  self:setCardsInHandModel(cardsInHandModel)
  self:setPokState(0)
end
function Player:setPlayerRole(roleType)
  local preRole = self:getPlayerRole()
  self._role = roleType
  if roleType ~= PlayerConstants.PlayerRole.VISITER then
  end
end
function Player:getPlayerRole()
  return self._role
end
function Player:setPlayerState(state)
  self._state = state
end
function Player:getPlayerState()
  return self._state
end
function Player:setIsSelf(isSelf)
  self._isSelf = isSelf
end
function Player:getIsSelf()
  return self._isSelf
end
function Player:setIsBanker(isBanker)
  self._isBanker = isBanker
end
function Player:getIsBanker()
  return self._isBanker
end
function Player:setMinBet(minBet)
  self._minBet = minBet
end
function Player:getMinBet()
  return self._minBet
end
function Player:setMaxBet(maxBet)
  self._maxBet = maxBet
end
function Player:getMaxBet()
  return self._maxBet
end
function Player:setDefaultBet(defaultBet)
  self._defaultBet = defaultBet
end
function Player:getDefaultBet()
  return self._defaultBet
end
return Player

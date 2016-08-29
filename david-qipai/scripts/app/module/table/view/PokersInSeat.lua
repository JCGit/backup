local SingleHandPoker = import(".SingleHandPoker")
local PokerConfigure = import(".PokerConfigure")
local CardsInHandModel = require("app.module.table.model.CardsInHandModel")
local PlayStatus = require("app.beans.table.model.PlayStatus")
local PokersInSeat = class("PokersInSeat", function()
  return display.newNode()
end)
PokersInSeat.ANIMATE_TIME = 0.1
function PokersInSeat:ctor()
  self.pokers = {}
  self.totalRounds = 0
  self.singleHandPokers = display.newNode():addTo(self)
  for i = 1, 3 do
    local singleHandPoker = SingleHandPoker.new():pos(0, 0):addTo(self.singleHandPokers):hide()
    self.pokers[i] = singleHandPoker
  end
  self._cardTypeBackground = display.newScale9Sprite("#table_card_type_bg.png", 0, 0, CCSize(156, 36)):addTo(self, 10, 10):hide()
  self._cardTypeLabel = ui.newTTFLabel({
    size = 22,
    align = ui.TEXT_ALIGN_CENTER,
    color = g.font.color.GOLDEN
  }):addTo(self, 11, 11):hide()
  self._containNode = display.newNode():addTo(self, 12, 12)
  self._pokAnimationNode = display.newNode():addTo(self._containNode, 13, 13)
  self._dengNode = display.newNode():addTo(self._containNode, 14, 14):hide()
end
function PokersInSeat:showDengIcon(deng)
  local needAnimation = false
  if self._dengNode ~= nil then
    self._dengNode:removeAllChildren()
  end
  if not self._dengNode:isVisible() then
    self._dengNode:show()
    needAnimation = true
  end
  if deng < 2 then
    return
  end
  self._dengImageView = display.newSprite("#table_deng_bg.png", 0, 0):addTo(self._dengNode)
  if deng == 2 then
    self._dengX = display.newSprite("#table_deng_value_x2.png", 0, 0):addTo(self._dengNode)
  elseif deng == 3 then
    self._dengX = display.newSprite("#table_deng_value_x3.png", 0, 0):addTo(self._dengNode)
  elseif deng == 5 then
    self._dengX = display.newSprite("#table_deng_value_x5.png", 0, 0):addTo(self._dengNode)
  end
  if needAnimation then
    self._isShowDengAnimation = true
    self._dengNode:scale(0)
    self._dengNode:scaleTo(0.5, 1)
  end
end
function PokersInSeat:bindData(player)
  self._player = player
  local cards = player._cardsInHandModel:getCards()
  self.deng = player:getCardsInHandModel():getDeng()
  self.totalRounds = player:getCardCount()
  for roundNumber, pokerValue in ipairs(cards) do
    if pokerValue ~= CardsInHandModel.UNKNOWN_VALUE then
      local singleHandPoker = self.pokers[roundNumber]
      singleHandPoker:bindData(pokerValue)
    end
  end
  local addOffset = 0
  if self._player:getCardCount() == 3 then
    addOffset = 12
  end
  if player:getIsSelf() then
    local isSelf = true
    local isSeatSubView = true
    local seatOffsetPoint = PokerConfigure:pokerSeatOffset(5, isSelf)
    self._cardTypeBackground:size(108, 36)
    self._cardTypeBackground:pos(seatOffsetPoint.x, seatOffsetPoint.y - 60)
    self._cardTypeLabel:pos(seatOffsetPoint.x, seatOffsetPoint.y - 60)
    self._dengNode:pos(seatOffsetPoint.x + 50 + addOffset, seatOffsetPoint.y + 40 - addOffset)
  else
    self._cardTypeBackground:pos(0, -60)
    self._cardTypeLabel:pos(0, -60)
    if player:getPokState() == 8 or player:getPokState() == 9 then
      self._dengNode:pos(75 + addOffset, 75 - addOffset)
    else
      self._dengNode:pos(50 + addOffset, 40 - addOffset)
    end
    self._cardTypeBackground:size(108, 36)
  end
  self:updatePokTypePostion(self.virtualSeatId)
end
function PokersInSeat:clearData()
  self._player = nil
  self.virtualSeatId = nil
  self.totalRounds = 0
  for i, singleHandPoker in ipairs(self.pokers) do
    singleHandPoker:stopAllActions()
    singleHandPoker:hidePokerInfo()
    singleHandPoker:hide()
    singleHandPoker:clearData()
  end
  self:cleanPokAnimation()
  self._cardTypeBackground:hide()
  self._cardTypeLabel:hide()
  self._dengNode:removeAllChildren()
  self._dengNode:hide()
end
function PokersInSeat:showNthPokerInSeat(index, pokerCount, isSelf)
  local singleHandPoker = self.pokers[index]
  singleHandPoker:show()
  self:setPokerInSeatLayout(index, pokerCount, isSelf)
end
function PokersInSeat:showPokersInSeatInTurnOverState()
  for i, singleHandPoker in ipairs(self.pokers) do
    if i <= self.totalRounds then
      singleHandPoker:showPokerInfo()
    end
  end
end
function PokersInSeat:showPokersInSeatInCoverState()
  for i, singleHandPoker in ipairs(self.pokers) do
    if i <= self.totalRounds then
      singleHandPoker:hidePokerInfo()
    end
  end
end
function PokersInSeat:showPokersInSeatCardType(cardTypeString)
  if not self._cardTypeLabel:isVisible() then
    self._cardTypeLabel:scale(0):scaleTo(0.3, 1)
    self._cardTypeBackground:scale(0):scaleTo(0.3, 1)
  end
  self._cardTypeBackground:show()
  self._cardTypeLabel:show()
  self._cardTypeLabel:setString(cardTypeString)
  self:showDengIcon(tonumber(self.deng or 0))
end
function PokersInSeat:hidePokersInSeatCardType()
  self._cardTypeBackground:hide()
  self._cardTypeLabel:hide()
end
function PokersInSeat:setPokerInSeatLayout(roundNumber, totalRounds, isSelf)
  local singleHandPoker = self.pokers[roundNumber]
  local endPoint, pokerRotateAngle
  if isSelf then
    local isSeatSubView = true
    endPoint, pokerRotateAngle = PokerConfigure:getPokerHoldingPosition(roundNumber, totalRounds, 0, 5, isSelf, isSeatSubView)
  else
    endPoint, pokerRotateAngle = PokerConfigure:getPokerComparePosition(roundNumber, totalRounds, isSelf)
  end
  singleHandPoker:stopAllActions()
  singleHandPoker:rotation(pokerRotateAngle)
  singleHandPoker:pos(endPoint.x, endPoint.y)
end
function PokersInSeat:updatePokerInSeatLayoutAnimate(index, totalRounds, isSelf, animate, turnOverPokersCompleteCallback)
  local singleHandPoker = self.pokers[index]
  local endPoint, pokerRotateAngle
  if isSelf then
    local isSeatSubView = true
    endPoint, pokerRotateAngle = PokerConfigure:getPokerHoldingPosition(index, totalRounds, 0, 5, isSelf, isSeatSubView)
  else
    endPoint, pokerRotateAngle = PokerConfigure:getPokerComparePosition(index, totalRounds, isSelf)
  end
  singleHandPoker:stopAllActions()
  if animate then
    singleHandPoker:rotateTo(PokersInSeat.ANIMATE_TIME, pokerRotateAngle)
    transition.execute(singleHandPoker, cc.MoveTo:create(PokersInSeat.ANIMATE_TIME, ccp(endPoint.x, endPoint.y)), {
      delay = 0,
      onComplete = function()
        if turnOverPokersCompleteCallback ~= nil then
          turnOverPokersCompleteCallback()
        end
      end
    })
  else
    singleHandPoker:rotation(pokerRotateAngle)
    singleHandPoker:pos(endPoint.x, endPoint.y)
  end
end
function PokersInSeat:playIncreasePokerInSeatAnimation(roundNumber, isSelf)
  for i = 1, roundNumber do
    local singleHandPoker = self.pokers[i]
    singleHandPoker:show()
    if i == roundNumber then
      local animate = false
      self:updatePokerInSeatLayoutAnimate(i, roundNumber, isSelf, animate)
    else
      local animate = true
      self:updatePokerInSeatLayoutAnimate(i, roundNumber, isSelf, animate)
    end
  end
end
function PokersInSeat:playTurnOverNthPokersInSeatAnimation(Nth, turnOverPokersCompleteCallback)
  if Nth <= self.totalRounds then
    local singleHandPoker = self.pokers[Nth]
    if not singleHandPoker:getPokerIsTurnOvered() then
      singleHandPoker:turnOverPoker(turnOverPokersCompleteCallback)
    end
  end
end
function PokersInSeat:playTurnOverPokersInSeatAnimation(turnOverPokersCompleteCallback)
  print(self.totalRounds)
  for i, singleHandPoker in ipairs(self.pokers) do
    if i <= self.totalRounds then
      if not singleHandPoker:getPokerIsTurnOvered() then
        if i == self.totalRounds then
          singleHandPoker:turnOverPoker(turnOverPokersCompleteCallback)
        else
          singleHandPoker:turnOverPoker()
        end
      else
        if i == self.totalRounds then
          turnOverPokersCompleteCallback()
        end
        print("bug............")
      end
    end
  end
end
function PokersInSeat:playPokAnimation(animate)
  self:cleanPokAnimation()
  local pokPoint
  if self._player == nil then
    return
  end
  if self._player:getIsSelf() then
    pokPoint = ccp(50, 70)
  else
    pokPoint = ccp(50, 50)
  end
  self._pokTypeStarIcons = {}
  for i = 1, 5 do
    local star = display.newSprite("#table_pok_star.png", pokPoint.x, pokPoint.y):addTo(self._pokAnimationNode)
    table.insert(self._pokTypeStarIcons, star)
  end
  local taem = 0
  if self._player ~= nil then
    taem = self._player:getCardsInHandModel():getTaem()
  end
  if tonumber(taem) == 8 then
    self._pokTypeIcon = display.newSprite("#table_pok8_icon.png", pokPoint.x, pokPoint.y)
  else
    self._pokTypeIcon = display.newSprite("#table_pok9_icon.png", pokPoint.x, pokPoint.y)
  end
  self._pokTypeIcon:scale(1.8)
  self._pokTypeIcon:addTo(self._pokAnimationNode)
  transition.scaleTo(self._pokTypeIcon, {scale = 2, time = 0.05})
  transition.scaleTo(self._pokTypeIcon, {
    scale = 1,
    time = 0.2,
    delay = 0.15,
    easing = "IN"
  })
  for i, star in ipairs(self._pokTypeStarIcons) do
    do
      local endPoint
      if i == 1 then
        endPoint = ccp(star:getPositionX() - 30, star:getPositionY() + 50)
      elseif i == 2 then
        endPoint = ccp(star:getPositionX() - 80, star:getPositionY() + 20)
      elseif i == 3 then
        endPoint = ccp(star:getPositionX() - 60, star:getPositionY() - 60)
      elseif i == 4 then
        endPoint = ccp(star:getPositionX() + 60, star:getPositionY() - 30)
      elseif i == 5 then
        endPoint = ccp(star:getPositionX() + 65, star:getPositionY() + 30)
      end
      transition.execute(star, CCMoveTo:create(0.3, endPoint), {
        delay = 0.4,
        easing = "backout",
        onComplete = function()
          star:removeFromParent()
        end
      })
      transition.rotateTo(star, {
        rotate = 90,
        time = 0.3,
        delay = 0.4
      })
      transition.fadeOut(star, {time = 0.2, delay = 0.4})
    end
  end
end
function PokersInSeat:cleanPokAnimation()
  self._pokAnimationNode:removeAllChildren()
  self._pokTypeIcon = nil
  self._pokTypeStarIcons = {}
end
function PokersInSeat:updatePokTypePostion(virtualSeatId)
  if tonumber(virtualSeatId or 0) == 3 and self._player ~= nil and (self._pokTypeIcon ~= nil or self._player:getPokState() == 8 or self._player:getPokState() == 9) then
    if tonumber(self.deng or 0) > 1 then
      self._containNode:pos(-15, 0)
      return
    end
  end
  self._containNode:pos(0, 0)
end
function PokersInSeat:setPokerScale(scale)
  for i, singleHandPoker in ipairs(self.pokers) do
    singleHandPoker:scale(scale)
  end
end
return PokersInSeat

local PokersInSeat = import(".PokersInSeat")
local PokerConfigure = import(".PokerConfigure")
local SmallPokersInSeat = class("SmallPokersInSeat", function()
  return display.newNode()
end)
SmallPokersInSeat.ANIMATION_TIME = 0.1
function SmallPokersInSeat:ctor()
  self.pokers = {}
  self.totalRounds = 0
  for i = 1, 3 do
    local singleHandPoker = display.newSprite("#card_back_small_bg.png"):addTo(self):hide()
    self.pokers[i] = singleHandPoker
  end
end
function SmallPokersInSeat:clearData()
  self.totalRounds = 0
end
function SmallPokersInSeat:showNthSmallPokerInSeat(index, pokerCount, seatId, virtualSeatId, isSelf)
  if index == 0 then
    return
  end
  local singleHandPoker = self.pokers[index]
  singleHandPoker:show()
  self:setSmallPokerInSeatLayout(index, pokerCount, seatId, virtualSeatId, isSelf)
end
function SmallPokersInSeat:hideSmallPokersInSeat()
  for i = 1, 3 do
    local singleHandPoker = self.pokers[i]
    singleHandPoker:hide()
  end
end
function SmallPokersInSeat:setSmallPokerInSeatLayout(index, totalRounds, seatId, virtualSeatId, isSelf)
  self.totalRounds = totalRounds
  local singleHandPoker = self.pokers[index]
  local isSeatSubView = true
  local endPoint, pokerRotateAngle = PokerConfigure:getPokerHoldingPosition(index, totalRounds, seatId, virtualSeatId, isSelf, isSeatSubView)
  singleHandPoker:stopAllActions()
  singleHandPoker:rotation(pokerRotateAngle)
  singleHandPoker:pos(endPoint.x, endPoint.y)
end
function SmallPokersInSeat:_updateSmallPokerInSeatLayoutAnimate(index, totalRounds, seatId, virtualSeatId, isSelf, animate, updateSmallPokerInSeatCompleteCallback)
  local singleHandPoker = self.pokers[index]
  local isSeatSubView = true
  local endPoint, pokerRotateAngle = PokerConfigure:getPokerHoldingPosition(index, totalRounds, seatId, virtualSeatId, isSelf, isSeatSubView)
  singleHandPoker:stopAllActions()
  if animate then
    singleHandPoker:rotateTo(SmallPokersInSeat.ANIMATION_TIME, pokerRotateAngle)
    transition.execute(singleHandPoker, cc.MoveTo:create(SmallPokersInSeat.ANIMATION_TIME, ccp(endPoint.x, endPoint.y)), {
      onComplete = function()
        if updateSmallPokerInSeatCompleteCallback ~= nil then
          updateSmallPokerInSeatCompleteCallback()
        end
      end
    })
  else
    singleHandPoker:rotation(pokerRotateAngle)
    singleHandPoker:pos(endPoint.x, endPoint.y)
  end
end
function SmallPokersInSeat:playIncreaseSmallPokerInSeatAnimation(roundNumber, seatId, virtualSeatId, isSelf, increaseSmallPokerInSeatCompleteCallback)
  self.totalRounds = roundNumber
  for i = 1, roundNumber do
    local singleHandPoker = self.pokers[i]
    singleHandPoker:show()
    if i == roundNumber then
      local animate = false
      self:_updateSmallPokerInSeatLayoutAnimate(i, roundNumber, seatId, virtualSeatId, isSelf, false)
    else
      local animate = true
      if roundNumber - 1 == i then
        self:_updateSmallPokerInSeatLayoutAnimate(i, roundNumber, seatId, virtualSeatId, isSelf, animate, increaseSmallPokerInSeatCompleteCallback)
      else
        self:_updateSmallPokerInSeatLayoutAnimate(i, roundNumber, seatId, virtualSeatId, isSelf, animate)
      end
    end
  end
end
function SmallPokersInSeat:playSmallPokersInSeatToSeatCenterAnimation(smallPokersMovedCompleteCallback)
  local centerPointX = 0
  for i = 1, self.totalRounds do
    local singleHandPoker = self.pokers[i]
    centerPointX = centerPointX + singleHandPoker:getPositionX()
  end
  if 0 < self.totalRounds then
    centerPointX = centerPointX / self.totalRounds
  end
  for i, singleHandPoker in ipairs(self.pokers) do
    if i <= self.totalRounds then
      local xPoint = singleHandPoker:getPositionX()
      local yPoint = singleHandPoker:getPositionY()
      singleHandPoker:stopAllActions()
      transition.execute(singleHandPoker, cc.MoveTo:create(0.3, ccp(xPoint - centerPointX, 0)), {
        onComplete = function()
          if i == self.totalRounds and smallPokersMovedCompleteCallback then
            smallPokersMovedCompleteCallback()
          end
        end
      })
    end
  end
end
return SmallPokersInSeat

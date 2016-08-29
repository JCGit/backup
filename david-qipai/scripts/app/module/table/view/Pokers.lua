local TableUIConfig = import(".TableUIConfig")
local PokerConfigure = import(".PokerConfigure")
local PlayStatus = require("app.beans.table.model.PlayStatus")
local Pokers = class("Pokers", function()
  return display.newNode()
end)
Pokers.POKERS_MAX_COUNT = 3
Pokers.POKER_FORWARD_TIME = 0.05
Pokers.POKER_FLY_TIME = 0.5
Pokers.DISPATCH_POKER_DURATION = Pokers.POKER_FORWARD_TIME + Pokers.POKER_FLY_TIME
Pokers.POKER_GAP_DURATION = 0.1
Pokers.POKER_SELF_SCALE = 2
Pokers.TOTALSEATS = 9
function Pokers:ctor(staticPokerNode)
  self._staticPokerNode = staticPokerNode
  self.staticPokers = {}
  self.allPokers = {}
  self.allPokersCallback = {}
  self:initPokers()
end
function Pokers:initPokers()
  local texture = "pd_card_texture.png"
  if g.vars.isWebP then
    texture = "pd_card_texture.webp"
  end
  if self._staticPokerNode ~= nil then
    self._handPokersBatchNode = display.newBatchNode(texture):addTo(self._staticPokerNode)
  else
    self._handPokersBatchNode = display.newBatchNode(texture):addTo(self)
  end
  local pokerPosition = ccp(TableUIConfig.DEALER_POKERS_CENTER.x, TableUIConfig.DEALER_POKERS_CENTER.y)
  for i = 1, 5 do
    pokerPosition = ccp(pokerPosition.x, pokerPosition.y + 1)
    self.staticPokers[i] = display.newSprite("#card_back_small_bg.png"):pos(pokerPosition.x, pokerPosition.y):rotation(180):addTo(self._handPokersBatchNode)
  end
  for i = 1, Pokers.POKERS_MAX_COUNT do
    self.allPokers[i] = {}
    self.allPokersCallback[i] = {}
    for j = 1, Pokers.TOTALSEATS do
      local singleHandPoker = display.newSprite("#card_back_small_bg.png")
      self.allPokers[i][j] = singleHandPoker:pos(pokerPosition.x, pokerPosition.y):addTo(self):rotation(180):hide()
    end
  end
end
function Pokers:_resetPokersPosition(basePoint, animate)
  local pokerPosition = ccp(basePoint.x, basePoint.y)
  TableUIConfig.DEALER_POKERS_CENTER.x = basePoint.x
  TableUIConfig.DEALER_POKERS_CENTER.y = basePoint.y
  for i = 1, 5 do
    pokerPosition = ccp(pokerPosition.x, pokerPosition.y + 1)
    local poker = self.staticPokers[i]
    poker:stopAllActions()
    if animate then
      transition.execute(poker, cc.MoveTo:create(TableUIConfig.DEALER_MOVE_STEP_TIME, pokerPosition), {easing = "sineOut", delay = 0.1})
    else
      poker:pos(pokerPosition.x, pokerPosition.y)
    end
  end
  for i = 1, Pokers.POKERS_MAX_COUNT do
    for j = 1, Pokers.TOTALSEATS do
      local singleHandPoker = self.allPokers[i][j]
      singleHandPoker:pos(pokerPosition.x + 200, pokerPosition.y):hide()
    end
  end
end
function Pokers:dealCards(totalRounds, seatIds, virtualSeatIds, mySeatId, dealCardCallback)
  local delay = 0
  local totalDealCardsCount = totalRounds * #seatIds
  for roundNumber = 1, totalRounds do
    for i, seatId in ipairs(seatIds) do
      local isFinalCard = false
      totalDealCardsCount = totalDealCardsCount - 1
      if totalDealCardsCount == 0 then
        isFinalCard = true
      end
      local isSelf = seatId == mySeatId
      virtualSeatId = virtualSeatIds[i]
      self:dealCard(roundNumber, totalRounds, seatId, virtualSeatId, delay, isSelf, dealCardCallback, isFinalCard)
      delay = delay + Pokers.POKER_GAP_DURATION
    end
  end
end
function Pokers:dealCard(roundNumber, totalRounds, seatId, virtualSeatId, delay, isSelf, dealCardCallback, isFinalCard)
  self.allPokersCallback[roundNumber][seatId + 1] = dealCardCallback
  local singleHandPoker = self.allPokers[roundNumber][seatId]
  singleHandPoker.dispatched = true
  local isSeatSubView = false
  local endPoint, pokerRotateAngle = PokerConfigure:getPokerHoldingPosition(roundNumber, totalRounds, seatId, virtualSeatId, isSelf, isSeatSubView)
  local delayTime = delay
  local delayPerform = cc.DelayTime:create(delayTime)
  local initStartPoint = cc.MoveTo:create(0, ccp(TableUIConfig.DEALER_POKERS_CENTER.x, TableUIConfig.DEALER_POKERS_CENTER.y))
  local showPokerAction = CCCallFunc:create(function()
    g.audio:playSound("DEAL_CARD")
    singleHandPoker:show()
  end)
  local distance = cc.PointDistance(TableUIConfig.DEALER_POKERS_CENTER, endPoint)
  local timeRatio = distance / 500
  if timeRatio > 0.75 then
    timeRatio = 0.75
  elseif timeRatio < 0.35 then
    timeRatio = 0.35
  end
  local moveForwardAction = cc.MoveTo:create(Pokers.POKER_FORWARD_TIME, ccp(TableUIConfig.DEALER_POKERS_CENTER.x, TableUIConfig.DEALER_POKERS_CENTER.y - 20))
  local array = cc.Array:create()
  local rotateAction = cc.RotateTo:create(Pokers.POKER_FLY_TIME * timeRatio, pokerRotateAngle)
  array:addObject(rotateAction)
  if isSelf then
    local scaleAction = cc.ScaleTo:create(Pokers.POKER_FLY_TIME * timeRatio, Pokers.POKER_SELF_SCALE)
    array:addObject(scaleAction)
  end
  local pokerFlyAction = cc.MoveTo:create(Pokers.POKER_FLY_TIME * timeRatio, ccp(endPoint.x, endPoint.y))
  array:addObject(pokerFlyAction)
  local actionFly = cc.Spawn:create(array)
  local pokerFlyEndAction = CCCallFunc:create(function()
    if dealCardCallback then
      dealCardCallback(roundNumber, seatId, singleHandPoker, isFinalCard)
      self.allPokersCallback[roundNumber][seatId + 1] = nil
    end
  end)
  local actArray = cc.Array:create()
  actArray:addObject(delayPerform)
  actArray:addObject(initStartPoint)
  actArray:addObject(showPokerAction)
  actArray:addObject(moveForwardAction)
  actArray:addObject(actionFly)
  actArray:addObject(pokerFlyEndAction)
  local actionSequence = cc.Sequence:create(actArray)
  singleHandPoker:stopAllActions()
  local action = singleHandPoker:runAction(actionSequence)
  g.animationsManager:addActions(singleHandPoker, action)
end
function Pokers:clearData()
  local pokerPosition = ccp(TableUIConfig.DEALER_POKERS_CENTER.x, TableUIConfig.DEALER_POKERS_CENTER.y + 5)
  for i = 1, Pokers.POKERS_MAX_COUNT do
    for j = 1, Pokers.TOTALSEATS do
      local singleHandPoker = self.allPokers[i][j]
      singleHandPoker:pos(pokerPosition.x, pokerPosition.y):rotation(180):scale(1):hide()
    end
  end
end
function Pokers:hidePokers()
  local callbackCount = 0
  for i = 1, Pokers.POKERS_MAX_COUNT do
    for j = 1, Pokers.TOTALSEATS do
      local callbackHandler = self.allPokersCallback[i][j]
      if callbackHandler then
        callbackCount = callbackCount + 1
      end
    end
  end
  for i = 1, Pokers.POKERS_MAX_COUNT do
    for j = 1, Pokers.TOTALSEATS do
      local singleHandPoker = self.allPokers[i][j]
      local callbackHandler = self.allPokersCallback[i][j]
      if callbackHandler then
        callbackCount = callbackCount - 1
        local isFinalCard = callbackCount == 0
        callbackHandler(i, j - 1, singleHandPoker, isFinalCard)
        self.allPokersCallback[i][j] = nil
      end
      singleHandPoker:stopAllActions()
      singleHandPoker:hide()
      singleHandPoker:rotation(180)
      singleHandPoker:scale(1)
    end
  end
end
function Pokers:showPokersAtVirtualSeat(virtualSeatId, animate)
  local seatCenterPosition = TableUIConfig.STATICPOKER[virtualSeatId]
  local basePoint = ccp(seatCenterPosition.x, seatCenterPosition.y)
  self:_resetPokersPosition(basePoint, animate)
end
return Pokers

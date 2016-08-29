local PokerConfigure = class("PokerConfigure")
local TableUIConfig = import(".TableUIConfig")
PokerConfigure.LARGE_SELF_SCALE = 0.85
PokerConfigure.LARGE_OTHER_SCALE = 0.8
PokerConfigure.SMALL_SCALE = 0.3
PokerConfigure.POKER_SMALL_RADIUS = 100
PokerConfigure.POKER_LARGE_OTHER_TWO_RADIUS = 120
PokerConfigure.POKER_LARGE_OTHER_THREE_RADIUS = 130
PokerConfigure.OTHER_TWO_POKER_ANGLE = 15
PokerConfigure.OTHER_THREE_POKER_ANGLE = 13
PokerConfigure.POKER_LARGE_SELF_TWO_RADIUS = 120
PokerConfigure.POKER_LARGE_SELF_THREE_RADIUS = 130
PokerConfigure.SELF_TWO_POKER_ANGLE = 14
PokerConfigure.SELF_THREE_POKER_ANGLE = 13
PokerConfigure.POKER_LARGE_RADIUS = 150
PokerConfigure.POKER_MID_RADIUS = 100
PokerConfigure.TWO_POKER_ANGLE = 15
PokerConfigure.THREE_POKER_ANGLE = 13
function PokerConfigure:getPokerHoldingPosition(roundNumber, totalRounds, seatId, virtualSeatId, isSelf, isSeatSubView)
  local offsetAngle
  local baseRadius = 0
  if isSelf then
    if totalRounds == 2 then
      offsetAngle = PokerConfigure.SELF_TWO_POKER_ANGLE
      baseRadius = PokerConfigure.POKER_LARGE_SELF_TWO_RADIUS * PokerConfigure.LARGE_SELF_SCALE
    else
      offsetAngle = PokerConfigure.SELF_THREE_POKER_ANGLE
      baseRadius = PokerConfigure.POKER_LARGE_SELF_THREE_RADIUS * PokerConfigure.LARGE_SELF_SCALE
    end
  else
    if totalRounds == 2 then
      offsetAngle = PokerConfigure.TWO_POKER_ANGLE
    else
      offsetAngle = PokerConfigure.THREE_POKER_ANGLE
    end
    baseRadius = PokerConfigure.POKER_SMALL_RADIUS * PokerConfigure.SMALL_SCALE
  end
  local pokerRotateAngle = -(totalRounds - 1) * offsetAngle / 2 + offsetAngle * (roundNumber - 1)
  local xOffset = math.sin(math.rad(pokerRotateAngle)) * baseRadius
  local yOffset = math.cos(math.rad(pokerRotateAngle)) * baseRadius - baseRadius
  local endPoint
  if isSeatSubView then
    endPoint = ccp(xOffset, yOffset)
  else
    endPoint = ccp(TableUIConfig.SEAT[virtualSeatId].x + xOffset, TableUIConfig.SEAT[virtualSeatId].y + yOffset)
  end
  local seatOffsetPoint = PokerConfigure:pokerSeatOffset(virtualSeatId, isSelf)
  endPoint = ccp(endPoint.x + seatOffsetPoint.x, endPoint.y + seatOffsetPoint.y)
  return endPoint, pokerRotateAngle
end
function PokerConfigure:pokerSeatOffset(virtualSeatId, isSelf)
  local seatXOffset, seatYOffset
  if virtualSeatId > 5 then
    seatXOffset = -35
    seatYOffset = -30
  elseif isSelf then
    seatXOffset = 100
    seatYOffset = 0
  else
    seatXOffset = 35
    seatYOffset = -30
  end
  return ccp(seatXOffset, seatYOffset)
end
function PokerConfigure:getPokerComparePosition(roundNumber, totalRounds, isSelf)
  local offsetAngle
  local baseRadius = 0
  if isSelf then
  elseif totalRounds == 2 then
    offsetAngle = PokerConfigure.OTHER_TWO_POKER_ANGLE
    baseRadius = PokerConfigure.POKER_LARGE_OTHER_TWO_RADIUS * PokerConfigure.LARGE_OTHER_SCALE
  else
    offsetAngle = PokerConfigure.OTHER_THREE_POKER_ANGLE
    baseRadius = PokerConfigure.POKER_LARGE_OTHER_THREE_RADIUS * PokerConfigure.LARGE_OTHER_SCALE
  end
  local pokerRotateAngle = -(totalRounds - 1) * offsetAngle / 2 + offsetAngle * (roundNumber - 1)
  local xOffset = math.sin(math.rad(pokerRotateAngle)) * baseRadius
  local yOffset = math.cos(math.rad(pokerRotateAngle)) * baseRadius - baseRadius
  local endPoint
  endPoint = ccp(xOffset, yOffset)
  return endPoint, pokerRotateAngle
end
return PokerConfigure

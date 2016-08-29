local TableUIConfig = import(".TableUIConfig")
local ChipsSpot = class("ChipsSpot", function()
  return display.newNode()
end)
ChipsSpot.MOVE_DELAY_DURATION = 0.06
ChipsSpot.MOVE_FROM_SEAT_DURATION = 0.35
ChipsSpot.CHIPS_PER_STACK = 5
ChipsSpot.SEND_CHIP_DURATION = 2
ChipsSpot.chipsConfig = {
  1,
  2,
  5,
  10,
  20,
  50,
  100,
  200,
  500,
  1000,
  2000,
  5000,
  10000,
  20000,
  50000,
  100000,
  200000,
  500000,
  1000000,
  2000000,
  5000000,
  10000000,
  20000000,
  50000000,
  100000000,
  200000000,
  500000000,
  1000000000,
  2000000000,
  5000000000,
  10000000000,
  20000000000,
  50000000000,
  100000000000,
  200000000000,
  500000000000,
  1000000000000,
  2000000000000,
  5000000000000
}
function ChipsSpot:ctor(totalChips, chipsSpotPoint)
  self._seatId = 0
  self.chipsNode1 = display.newNode():addTo(self)
  self.chipsNode2 = display.newNode():addTo(self)
  self.chipsNode3 = display.newNode():addTo(self)
  self.chipsSpotPoint = chipsSpotPoint or ccp(0, 0)
  self:setTotalChips(totalChips)
  self.chipStackCount = 0
  self.chipsInfoNode = display.newNode():addTo(self)
  self._chipTextLabel = ui.newTTFLabel({
    text = "999.9M",
    size = 20,
    align = ui.TEXT_ALIGN_CENTER,
    color = ccc3(255, 204, 0)
  }):pos(0, 0):addTo(self.chipsInfoNode)
  self._chipTextBG = display.newScale9Sprite("#table_chip_text_bg.png", 0, 0, cc.size(70, 20)):addTo(self.chipsInfoNode)
  self:setChipsLabelVisible(false)
  if self._totalChips then
    self.chipSprites = self:exchangeChipsToSprites(self._totalChips)
    self._chipSpritesOffset = {}
    self:_createChipsSpot(self.chipsSpotPoint)
    self:updateChipsLabelPosition()
  end
end
function ChipsSpot:setSeatId(seatId)
  self._seatId = seatId
end
function ChipsSpot:getSeatId()
  return self._seatId
end
function ChipsSpot:getTotalChips()
  return self._totalChips
end
function ChipsSpot:setTotalChips(totalChips)
  self._totalChips = tonumber(totalChips or 0)
end
function ChipsSpot:bindData(totalChips)
  self:setTotalChips(totalChips)
  self:clearAllSprites()
  self.chipSprites = self:exchangeChipsToSprites(self._totalChips)
  self:_createChipsSpot()
end
function ChipsSpot:_createChipsSpot()
  local totalChipsCount = #self.chipSprites
  self.chipStackCount = math.ceil(totalChipsCount / ChipsSpot.CHIPS_PER_STACK)
  if self._totalChips > 0 then
    self._chipTextLabel:setString(g.util.functions.formatBigInt(self._totalChips))
    self._chipTextBG:setContentSize(cc.size(self._chipTextLabel:getContentSize().width + 10, 20))
  end
  local baseOffSet = 25
  for i, chipSprite in ipairs(self.chipSprites) do
    local stackNumber = math.ceil(i / ChipsSpot.CHIPS_PER_STACK)
    local chipsOrder = i - (stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK
    if self.chipStackCount == 1 then
      local spriteOffset = ccp(0, chipsOrder * TableUIConfig.WAGER_GAP)
      self._chipSpritesOffset[i] = spriteOffset
      chipSprite:pos(self.chipsSpotPoint.x + spriteOffset.x, self.chipsSpotPoint.y + spriteOffset.y):addTo(self.chipsNode1)
    elseif self.chipStackCount == 2 then
      if stackNumber == 1 then
        local spriteOffset = ccp(-baseOffSet * 0.5, chipsOrder * TableUIConfig.WAGER_GAP)
        self._chipSpritesOffset[i] = spriteOffset
        chipSprite:pos(self.chipsSpotPoint.x + spriteOffset.x, self.chipsSpotPoint.y + spriteOffset.y):addTo(self.chipsNode1)
      elseif stackNumber == 2 then
        local spriteOffset = ccp(baseOffSet * 0.5, chipsOrder * TableUIConfig.WAGER_GAP)
        self._chipSpritesOffset[i] = spriteOffset
        chipSprite:pos(self.chipsSpotPoint.x + spriteOffset.x, self.chipsSpotPoint.y + spriteOffset.y):addTo(self.chipsNode2)
      end
    elseif self.chipStackCount == 3 then
      if stackNumber == 1 then
        local spriteOffset = ccp(-baseOffSet * 0.5, baseOffSet * 0.5 + chipsOrder * TableUIConfig.WAGER_GAP)
        self._chipSpritesOffset[i] = spriteOffset
        chipSprite:pos(self.chipsSpotPoint.x + spriteOffset.x, self.chipsSpotPoint.y + spriteOffset.y):addTo(self.chipsNode1)
      elseif stackNumber == 2 then
        local spriteOffset = ccp(baseOffSet * 0.5, baseOffSet * 0.5 + chipsOrder * TableUIConfig.WAGER_GAP)
        self._chipSpritesOffset[i] = spriteOffset
        chipSprite:pos(self.chipsSpotPoint.x + spriteOffset.x, self.chipsSpotPoint.y + spriteOffset.y):addTo(self.chipsNode2)
      elseif stackNumber == 3 then
        local spriteOffset = ccp(0, -baseOffSet * 0.25 + chipsOrder * TableUIConfig.WAGER_GAP)
        self._chipSpritesOffset[i] = spriteOffset
        chipSprite:pos(self.chipsSpotPoint.x + spriteOffset.x, self.chipsSpotPoint.y + spriteOffset.y):addTo(self.chipsNode3)
      end
    end
  end
end
function ChipsSpot:updateChipsLabelPosition()
  if self.chipStackCount == 1 then
    self.chipsInfoNode:pos(self.chipsSpotPoint.x, self.chipsSpotPoint.y - 22)
  elseif self.chipStackCount == 2 then
    self.chipsInfoNode:pos(self.chipsSpotPoint.x, self.chipsSpotPoint.y - 22)
  elseif self.chipStackCount == 3 then
    self.chipsInfoNode:pos(self.chipsSpotPoint.x, self.chipsSpotPoint.y - 28)
  end
end
function ChipsSpot:_getFinalChipsLabelPostionWithChipsSpotPoint(chipsSpotPoint)
  if self.chipStackCount == 1 then
    return ccp(chipsSpotPoint.x, chipsSpotPoint.y - 22)
  elseif self.chipStackCount == 2 then
    return ccp(chipsSpotPoint.x, chipsSpotPoint.y - 22)
  elseif self.chipStackCount == 3 then
    return ccp(chipsSpotPoint.x, chipsSpotPoint.y - 28)
  end
end
function ChipsSpot:setChipsLabelVisible(visible)
  if visible then
    self.chipsInfoNode:show()
  else
    self.chipsInfoNode:hide()
  end
end
function ChipsSpot:clearData()
  self._totalChips = 0
  self:clearAllSprites()
end
function ChipsSpot:clearAllSprites()
  self.chipsNode1:removeAllChildren()
  self.chipsNode2:removeAllChildren()
  self.chipsNode3:removeAllChildren()
  self.chipSprites = {}
  self._chipSpritesOffset = {}
  self:setChipsLabelVisible(false)
end
function ChipsSpot:showForVirtualSeatId(virtualSeatId)
  self:clearAllSprites()
  self.chipsSpotPoint = TableUIConfig.WAGERVIEW[virtualSeatId]
  if self:getTotalChips() > 0 then
    self.chipSprites = self:exchangeChipsToSprites(self._totalChips)
    self:_createChipsSpot(self.chipsSpotPoint)
    self:updateChipsLabelPosition()
    self:setChipsLabelVisible(true)
  else
    self:setChipsLabelVisible(false)
  end
end
function ChipsSpot:playChipsBetAnimation(startPoint, endPoint, completeBlock)
  local totalChipsCount = table.maxn(self.chipSprites)
  for i, chipSprite in ipairs(self.chipSprites) do
    do
      local spriteOffset = self._chipSpritesOffset[i]
      chipSprite:pos(startPoint.x + spriteOffset.x, startPoint.y + spriteOffset.y):opacity(0)
      transition.execute(chipSprite, cc.FadeTo:create(ChipsSpot.MOVE_FROM_SEAT_DURATION, 255), {
        delay = (i - 1) * ChipsSpot.MOVE_DELAY_DURATION
      })
      transition.execute(chipSprite, cc.MoveTo:create(ChipsSpot.MOVE_FROM_SEAT_DURATION, ccp(endPoint.x + spriteOffset.x, endPoint.y + spriteOffset.y)), {
        easing = "sineOut",
        delay = (i - 1) * ChipsSpot.MOVE_DELAY_DURATION,
        onComplete = function()
          if i == totalChipsCount then
            self.chipsSpotPoint = endPoint
            self:updateChipsLabelPosition()
            self:setChipsLabelVisible(true)
            if completeBlock then
              completeBlock()
            end
          end
        end
      })
    end
  end
end
function ChipsSpot:movingChipsSpotToCenterPostion(chipsStayTime, endPoint, moveCompleteCallback)
  self.movingChipsSpotToCenterPostionCallback = moveCompleteCallback
  self:setChipsLabelVisible(false)
  local totalChipsCount = table.maxn(self.chipSprites)
  local totalStackCount = math.ceil(totalChipsCount / ChipsSpot.CHIPS_PER_STACK)
  for i, chipSprite in ipairs(self.chipSprites) do
    do
      local spriteOffset = self._chipSpritesOffset[i]
      local stackNumber = math.ceil(i / ChipsSpot.CHIPS_PER_STACK)
      local chipsOrderInStack = i - (stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK
      local currentStackTopN = ChipsSpot.CHIPS_PER_STACK
      if stackNumber == totalStackCount then
        currentStackTopN = totalChipsCount - (stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK
      end
      chipSprite:stopAllActions()
      transition.execute(chipSprite, cc.MoveTo:create(ChipsSpot.MOVE_FROM_SEAT_DURATION, ccp(endPoint.x + spriteOffset.x, endPoint.y + spriteOffset.y)), {
        easing = "sineOut",
        delay = chipsStayTime + ((stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK + currentStackTopN - chipsOrderInStack) * ChipsSpot.MOVE_DELAY_DURATION,
        onComplete = function()
          if i == (totalStackCount - 1) * ChipsSpot.CHIPS_PER_STACK + 1 then
            self.chipsSpotPoint = endPoint
            if moveCompleteCallback then
              moveCompleteCallback()
              self.movingChipsSpotToCenterPostionCallback = nil
            end
          end
        end
      })
      transition.fadeTo(chipSprite, {
        opacity = 200,
        time = 0,
        delay = chipsStayTime + ((stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK + ChipsSpot.CHIPS_PER_STACK - chipsOrderInStack) * ChipsSpot.MOVE_DELAY_DURATION
      })
    end
  end
end
function ChipsSpot:movingWinChipsSpotToUser(chipsStayTime, endPoint, moveCompleteCallback)
  self.movingWinChipsSpotToUserCallback = moveCompleteCallback
  self:setChipsLabelVisible(false)
  local totalChipsCount = table.maxn(self.chipSprites)
  local totalStackCount = math.ceil(totalChipsCount / ChipsSpot.CHIPS_PER_STACK)
  for i, chipSprite in ipairs(self.chipSprites) do
    do
      local spriteOffset = self._chipSpritesOffset[i]
      local stackNumber = math.ceil(i / ChipsSpot.CHIPS_PER_STACK)
      local chipsOrderInStack = i - (stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK
      local currentStackTopN = ChipsSpot.CHIPS_PER_STACK
      if stackNumber == totalStackCount then
        currentStackTopN = totalChipsCount - (stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK
      end
      chipSprite:stopAllActions()
      transition.execute(chipSprite, cc.MoveTo:create(ChipsSpot.MOVE_FROM_SEAT_DURATION, endPoint), {
        easing = "sineOut",
        delay = chipsStayTime + ((stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK + currentStackTopN - chipsOrderInStack) * ChipsSpot.MOVE_DELAY_DURATION,
        onComplete = function()
          chipSprite:hide()
          if i == (totalStackCount - 1) * ChipsSpot.CHIPS_PER_STACK + 1 then
            self.chipsSpotPoint = endPoint
            if moveCompleteCallback then
              moveCompleteCallback()
              self.movingWinChipsSpotToUserCallback = nil
            end
          end
        end
      })
      transition.fadeTo(chipSprite, {
        opacity = 200,
        time = 0,
        delay = chipsStayTime + ((stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK + ChipsSpot.CHIPS_PER_STACK - chipsOrderInStack) * ChipsSpot.MOVE_DELAY_DURATION
      })
    end
  end
end
function ChipsSpot:playChipsStacksMoveAnimation(startPoint, endPoint, completeBlock)
  local totalChipsCount = table.maxn(self.chipSprites)
  for i, chipSprite in ipairs(self.chipSprites) do
    do
      local spriteOffset = self._chipSpritesOffset[i]
      local chipsOrderInStack = i % ChipsSpot.CHIPS_PER_STACK
      if chipsOrderInStack == 0 then
        chipsOrderInStack = ChipsSpot.CHIPS_PER_STACK
      end
      chipSprite:pos(startPoint.x + spriteOffset.x, startPoint.y + spriteOffset.y):opacity(0)
      transition.execute(chipSprite, cc.FadeTo:create(ChipsSpot.MOVE_FROM_SEAT_DURATION, 255))
      transition.execute(chipSprite, cc.MoveTo:create(ChipsSpot.MOVE_FROM_SEAT_DURATION, ccp(endPoint.x + spriteOffset.x, endPoint.y + spriteOffset.y)), {
        delay = chipsOrderInStack * ChipsSpot.MOVE_DELAY_DURATION,
        easing = "sineOut",
        onComplete = function()
          if i == totalChipsCount then
            self.chipsSpotPoint = endPoint
            self:updateChipsLabelPosition()
            self:setChipsLabelVisible(true)
            if completeBlock then
              completeBlock()
            end
          end
        end
      })
    end
  end
end
function ChipsSpot:exchangeChipsToSprites(chipsValue)
  local chipsValueInt = tonumber(chipsValue or 0)
  if chipsValueInt <= 0 then
    return {}
  end
  local chipsSprite = {}
  local totalUnit = #ChipsSpot.chipsConfig
  for i, unit in ipairs(ChipsSpot.chipsConfig) do
    unit = ChipsSpot.chipsConfig[totalUnit - i + 1]
    while chipsValueInt >= unit do
      local chipName = "#table_chip_type_" .. i % 11 + 1 .. ".png"
      table.insert(chipsSprite, display.newSprite(chipName))
      chipsValueInt = chipsValueInt - unit
      if #chipsSprite == 15 then
        return chipsSprite
      end
    end
  end
  return chipsSprite
end
function ChipsSpot:sendingChipsSpotToUser(chipsStayTime, endPoint, moveCompleteCallback)
  self:setChipsLabelVisible(true)
  local totalChipsCount = table.maxn(self.chipSprites)
  local totalStackCount = math.ceil(totalChipsCount / ChipsSpot.CHIPS_PER_STACK)
  for i, chipSprite in ipairs(self.chipSprites) do
    do
      local spriteOffset = self._chipSpritesOffset[i]
      local stackNumber = math.ceil(i / ChipsSpot.CHIPS_PER_STACK)
      local chipsOrderInStack = i - (stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK
      local currentStackTopN = ChipsSpot.CHIPS_PER_STACK
      if stackNumber == totalStackCount then
        currentStackTopN = totalChipsCount - (stackNumber - 1) * ChipsSpot.CHIPS_PER_STACK
      end
      chipSprite:stopAllActions()
      transition.execute(chipSprite, cc.MoveTo:create(ChipsSpot.SEND_CHIP_DURATION, ccp(endPoint.x + spriteOffset.x, endPoint.y + spriteOffset.y)), {
        easing = "exponentialInOut",
        onComplete = function()
          chipSprite:hide()
          if i == totalChipsCount then
            self.chipsSpotPoint = endPoint
            if moveCompleteCallback then
              moveCompleteCallback()
            end
          end
        end
      })
    end
  end
  local finalChipsInfoPoint = self:_getFinalChipsLabelPostionWithChipsSpotPoint(endPoint)
  transition.execute(self.chipsInfoNode, cc.MoveTo:create(ChipsSpot.SEND_CHIP_DURATION, finalChipsInfoPoint), {
    easing = "exponentialInOut",
    onComplete = function()
    end
  })
end
return ChipsSpot

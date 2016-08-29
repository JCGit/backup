local POKER_TYPE_DIAMOND = 1
local POKER_TYPE_CLUB = 2
local POKER_TYPE_HEART = 3
local POKER_TYPE_SPADE = 4
local getValue = function(pokerValue)
  return pokerValue % 256
end
local getPokerType = function(pokerValue)
  return math.floor(pokerValue / 256)
end
local SingleHandPoker = class("SingleHandPoker", function()
  return display.newNode()
end)
function SingleHandPoker:ctor()
  self.dispatched = false
  self.pokerIsTurnOvered = false
  self:createPlaceHolder()
  self:hidePokerInfo()
end
function SingleHandPoker:getDispatched()
  return self.dispatched
end
function SingleHandPoker:setDispatched(dispatched)
  self.dispatched = dispatched
end
function SingleHandPoker:getPokerIsTurnOvered()
  return self.pokerIsTurnOvered
end
function SingleHandPoker:setPokerIsTurnOvered(pokerIsTurnOvered)
  self.pokerIsTurnOvered = pokerIsTurnOvered
end
function SingleHandPoker:getSpriteHeight()
  return self._pokerCover:getContentSize().height
end
function SingleHandPoker:createPlaceHolder()
  self._pokerCover = display.newSprite("#card_back_bg.png"):pos(0, -4):addTo(self):hide()
  self._pokerCover:setScaleX(-1)
  local texture = "pd_card_texture.png"
  if g.vars.isWebP then
    texture = "pd_card_texture.webp"
  end
  self._pokerContentBatch = display.newBatchNode(texture):pos(0, 0):addTo(self):hide()
  self._pokerContentBG = display.newSprite("#card_front_bg.png"):pos(0, -4):addTo(self._pokerContentBatch)
  self._pokerContentBigType = display.newSprite("#card_variety_big_red_diamond.png"):pos(10, -14):addTo(self._pokerContentBatch)
  self._pokerContentSmallType = display.newSprite("#card_variety_small_red_diamond.png"):pos(-24, 4):addTo(self._pokerContentBatch)
  self._pokerContentNumber = display.newSprite("#card_value_big_red_1.png"):pos(-24, 32):addTo(self._pokerContentBatch)
end
function SingleHandPoker:bindData(pokerValue)
  if pokerValue == nil then
    pokerValue = 26
  end
  self.cardValue_ = g.util.functions.getPokerValue(pokerValue)
  self.cardVariety_ = g.util.functions.getPokerSuit(pokerValue)
  if self.cardVariety_ == POKER_TYPE_DIAMOND then
    self._pokerContentSmallType:setDisplayFrame(display.newSpriteFrame("card_variety_small_red_diamond.png"))
    self._pokerContentNumber:setDisplayFrame(display.newSpriteFrame("card_value_big_red_" .. self.cardValue_ .. ".png"))
    if self.cardValue_ > 10 and self.cardValue_ < 14 then
      self._pokerContentBigType:setDisplayFrame(display.newSpriteFrame("card_variety_big_red_diamond_" .. self.cardValue_ .. ".png"))
      self._pokerContentBigType:pos(5, -12)
    else
      self._pokerContentBigType:setDisplayFrame(display.newSpriteFrame("card_variety_big_red_diamond.png"))
      self._pokerContentBigType:pos(10, -14)
    end
  elseif self.cardVariety_ == POKER_TYPE_HEART then
    self._pokerContentSmallType:setDisplayFrame(display.newSpriteFrame("card_variety_small_red_heart.png"))
    self._pokerContentNumber:setDisplayFrame(display.newSpriteFrame("card_value_big_red_" .. self.cardValue_ .. ".png"))
    if self.cardValue_ > 10 and self.cardValue_ < 14 then
      self._pokerContentBigType:setDisplayFrame(display.newSpriteFrame("card_variety_big_red_heart_" .. self.cardValue_ .. ".png"))
      self._pokerContentBigType:pos(5, -12)
    else
      self._pokerContentBigType:setDisplayFrame(display.newSpriteFrame("card_variety_big_red_heart.png"))
      self._pokerContentBigType:pos(10, -14)
    end
  elseif self.cardVariety_ == POKER_TYPE_CLUB then
    self._pokerContentSmallType:setDisplayFrame(display.newSpriteFrame("card_variety_small_black_club.png"))
    self._pokerContentNumber:setDisplayFrame(display.newSpriteFrame("card_value_big_black_" .. self.cardValue_ .. ".png"))
    if self.cardValue_ > 10 and self.cardValue_ < 14 then
      self._pokerContentBigType:setDisplayFrame(display.newSpriteFrame("card_variety_big_black_club_" .. self.cardValue_ .. ".png"))
      self._pokerContentBigType:pos(5, -12)
    else
      self._pokerContentBigType:setDisplayFrame(display.newSpriteFrame("card_variety_big_black_club.png"))
      self._pokerContentBigType:pos(10, -14)
    end
  elseif self.cardVariety_ == POKER_TYPE_SPADE then
    self._pokerContentSmallType:setDisplayFrame(display.newSpriteFrame("card_variety_small_black_spade.png"))
    self._pokerContentNumber:setDisplayFrame(display.newSpriteFrame("card_value_big_black_" .. self.cardValue_ .. ".png"))
    if self.cardValue_ > 10 and self.cardValue_ < 14 then
      self._pokerContentBigType:setDisplayFrame(display.newSpriteFrame("card_variety_big_black_spade_" .. self.cardValue_ .. ".png"))
      self._pokerContentBigType:pos(5, -12)
    else
      self._pokerContentBigType:setDisplayFrame(display.newSpriteFrame("card_variety_big_black_spade.png"))
      self._pokerContentBigType:pos(10, -14)
    end
  end
end
function SingleHandPoker:showPokerInfo()
  self.pokerIsTurnOvered = true
  self._pokerCover:hide()
  self._pokerContentBatch:show()
end
function SingleHandPoker:hidePokerInfo()
  self.pokerIsTurnOvered = false
  self._pokerCover:show()
  self._pokerContentBatch:hide()
end
function SingleHandPoker:turnOverPoker(turnOverPokerCompleteCallback)
  self._turnOverPokerCompleteCallback = turnOverPokerCompleteCallback
  if self.pokerIsTurnOvered then
    if self._turnOverPokerCompleteCallback ~= nil then
      self._turnOverPokerCompleteCallback()
      self._turnOverPokerCompleteCallback = nil
    end
    return
  end
  self.pokerIsTurnOvered = true
  local delayPerform = cc.DelayTime:create(0.2)
  local orbitAction1 = cc.OrbitCamera:create(0.25, 1, 0, 0, 50, 0, 0)
  local showPokerInfoAction = CCCallFunc:create(function()
    self:showPokerInfo()
  end)
  local orbitAction2 = cc.OrbitCamera:create(0.25, 1, 0, -90, 90, 0, 0)
  local turnOverEndAction = CCCallFunc:create(function()
    if self._turnOverPokerCompleteCallback then
      self._turnOverPokerCompleteCallback()
      self._turnOverPokerCompleteCallback = nil
    end
  end)
  local array = cc.Array:create()
  array:addObject(delayPerform)
  array:addObject(orbitAction1)
  array:addObject(showPokerInfoAction)
  array:addObject(orbitAction2)
  array:addObject(turnOverEndAction)
  local actionSequence = cc.Sequence:create(array)
  local action = self:runAction(actionSequence)
  g.animationsManager:addActions(self, action)
end
function SingleHandPoker:clearData()
  if self._turnOverPokerCompleteCallback then
    self._turnOverPokerCompleteCallback()
    self._turnOverPokerCompleteCallback = nil
  end
  self:setPokerIsTurnOvered(false)
  self:setDispatched(false)
  self.cardValue_ = nil
  self.cardVariety_ = nil
end
return SingleHandPoker

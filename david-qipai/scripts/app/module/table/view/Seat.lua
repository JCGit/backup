local SeatConstants = import(".SeatConstants")
local TableUIEvent = import(".TableUIEvent")
local PlayStatus = require("app.beans.table.model.PlayStatus")
local BuyInSettingPopup = import(".BuyInSettingPopup")
local OtherUserProfileView = import(".OtherUserProfileView")
local ProfileView = require("app.beans.profile.view.ProfileView")
local SeatClock = import(".SeatClock")
local UIImageView = require("app.ui.imageview.UIImageView")
local PokersInSeat = import(".PokersInSeat")
local SmallPokersInSeat = import(".SmallPokersInSeat")
local PokerConfigure = import(".PokerConfigure")
local SmallWinView = import(".SmallWinView")
local GiftShopView = require("app.beans.giftshop.view.GiftShopView")
local Seat = class("Seat", function()
  return display.newNode()
end)
Seat.WIDTH = 110
Seat.HEIGHT = 156
Seat.AVATAR_HEIGHT = 85
function Seat:ctor(seatId)
  self.seatWidth = Seat.WIDTH
  self.seatHeight = Seat.HEIGHT
  self.avatarHeight = Seat.AVATAR_HEIGHT
  self:setNodeEventEnabled(true)
  self._player = nil
  self._seatState = SeatConstants.SeateState.AVAILABLE
  self._seatId = seatId
  self._virtualSeatId = self._seatId
  self:initSeat()
  self._seatChips = 0
  self._helpSprite = display.newSprite()
  self._helpSprite:addTo(self)
  self._helpSprite:retain()
  self._seatChipsEnterFrameHandle = nil
  self._giftId = 0
  self:scale(0.9)
end
function Seat:onEnter()
end
function Seat:onExit()
  self:clearData()
  if self._helpSprite then
    self._helpSprite:release()
    self._helpSprite = nil
  end
end
function Seat:initSeat()
  self._background = display.newScale9Sprite("#table_seat_bg.png", 0, 0, CCSize(self.seatWidth, self.seatHeight)):addTo(self, 2)
  self._background:setTouchEnabled(true)
  self._background:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event, x, y, prevX, prevY)
    if event.name == "began" then
      if self:isEmpty() then
        self:_clickSeatToSitDown()
      else
        self:_clickSeatToShowProfileInfo()
      end
    end
    return true
  end)
  self._seatAvailableImage = display.newSprite("#table_seat_available.png"):pos(0, -10)
  display.newSprite("#table_seat_available_text.png"):pos(self._seatAvailableImage:getContentSize().width * 0.5, 75):addTo(self._seatAvailableImage)
  self._avatarImageView = UIImageView.new():pos(0, self.avatarHeight)
  self._grayCover = display.newRect(100, 100, {
    fill = true,
    color = ccc4f(0, 0, 0, 0.6)
  }):addTo(self._avatarImageView, 3, 3):hide()
  self._avatarClippingNode = cc.ClippingNode:create()
  local stencil = display.newDrawNode()
  local points = {
    {
      -self.avatarHeight / 2,
      -self.avatarHeight / 2
    },
    {
      -self.avatarHeight / 2,
      self.avatarHeight / 2
    },
    {
      self.avatarHeight / 2,
      self.avatarHeight / 2
    },
    {
      self.avatarHeight / 2,
      -self.avatarHeight / 2
    }
  }
  local colors = ccc4f(255, 0, 0, 255)
  stencil:drawPolygon(points, colors, 1, colors)
  self._avatarClippingNode:setStencil(stencil)
  self._avatarClippingNode:addChild(self._seatAvailableImage, 1, 1)
  self._avatarClippingNode:addChild(self._avatarImageView, 2, 2)
  self._avatarClippingNode:addTo(self, 4, 4)
  self._seatChipsLabelBG = display.newScale9Sprite("#table_seat_chip_bg.png", 0, -Seat.HEIGHT * 0.5 + 17, cc.size(Seat.WIDTH, 34))
  self._seatChipsLabelBG:addTo(self, 5, 5):hide()
  self._seatChipsLabel = ui.newTTFLabel({
    text = "",
    size = 24,
    align = ui.TEXT_ALIGN_CENTER,
    color = ccc3(255, 209, 0)
  }):pos(0, -Seat.HEIGHT * 0.5 + 17):addTo(self, 6, 6)
  self._playStatusBar = ui.newTTFLabel({
    text = "",
    size = 24,
    align = ui.TEXT_ALIGN_CENTER,
    color = ccc3(255, 209, 0)
  }):pos(0, Seat.HEIGHT * 0.5 - 20):addTo(self, 7, 7)
  if g.vars.user.giftShop == 1 then
    self._giftButton = cc.ui.UIPushButton.new({
      normal = "#gift_icon_up.png",
      pressed = "#gift_icon_down.png"
    }):scale(1):addTo(self, 9, 9):hide():onButtonClicked(clickHandler(self, self._onGiftClick))
    self:_updatGiftPositon()
  end
  self._smallPokersInSeat = SmallPokersInSeat.new():addTo(self, 10, 10)
  self._pokersInSeat = PokersInSeat.new():addTo(self, 11, 11)
  self.seatClock = SeatClock.new():addTo(self):show()
  self._seatWinView = SmallWinView.new():addTo(self, 14, 14):hide():pos(0, -16)
end
function Seat:_onGiftClick(evt)
  local toUsers = {}
  local selfSeatId = -1
  for i, player in ipairs(g.vars.table.gamblers) do
    if not player:getIsSelf() then
      toUser = {}
      toUser.uid = player.uid
      toUser.seatId = player:getSeatIdForServer()
      table.insert(toUsers, toUser)
    else
      selfSeatId = player:getSeatIdForServer()
    end
  end
  local sourceUser = {
    uid = self._player:getUid(),
    seatId = self._player:getSeatIdForServer()
  }
  GiftShopView.new(1):show(true, sourceUser, toUsers, selfSeatId)
end
function Seat:bindData(player, notChangeSeatChipsLabel)
  self:show()
  self._player = player
  local avatarUrl = player and player:getSPic()
  if player:getGender() == 0 then
    self._avatarImageView:setImage(avatarUrl, {
      defaultImageName = "#common_male_avatar.png",
      size = cc.size(100, 100)
    })
  else
    self._avatarImageView:setImage(avatarUrl, {
      defaultImageName = "#common_female_avatar.png",
      size = cc.size(100, 100)
    })
  end
  self._seatChipsLabel:show()
  if not notChangeSeatChipsLabel then
    self:setSeatChips(player:getSeatChips())
  end
  self._playStatusBar:show()
  if player:getPlayStatus() == PlayStatus.NON_PLAY then
    self:showGrayCover(true)
  elseif player:getPlayStatus() == PlayStatus.WAIT_FOR_GAMESTART then
    self:showGrayCover(false)
  elseif player:getPlayStatus() == PlayStatus.BETTED then
  elseif player:getPlayStatus() == PlayStatus.WAIT_FOR_BET then
    self:showGrayCover(false)
  elseif player:getPlayStatus() == PlayStatus.BETTING then
  elseif player:getPlayStatus() == PlayStatus.LOSE then
  elseif player:getPlayStatus() == PlayStatus.WIN then
  elseif player:getPlayStatus() == PlayStatus.DRAW then
  elseif player:getPlayStatus() == PlayStatus.POK then
  end
  self:setPlayStatusString(player:getUserName())
  self._pokersInSeat:bindData(player)
  self._pokersInSeat.virtualSeatId = self:getVirtualSeatId()
  if player:getIsSelf() then
    self._pokersInSeat:setPokerScale(PokerConfigure.LARGE_SELF_SCALE)
  else
    self._pokersInSeat:setPokerScale(PokerConfigure.LARGE_OTHER_SCALE)
  end
  local giftId = player:getGiftId()
  self:setGiftId(giftId)
end
function Seat:setGiftId(giftId)
  if g.vars.user.giftShop == 1 and giftId > 0 and self._giftId ~= giftId then
    self._giftButton:setButtonImage("normal", "#gift_icon_up.png")
    self._giftButton:setButtonImage("pressed", "#gift_icon_down.png")
    self._giftButton:scale(1)
    g.giftInfo:loadGiftData(handler(self, function()
      local gift = g.giftInfo:getGiftById(giftId)
      if gift then
        self._giftImage = g.ui.UIImageView.new()
        self._giftImage:addTo(self):hide()
        self._giftImage:setImage(gift.pic, {
          success = handler(self, self._onGiftLoaded)
        })
      end
    end))
  end
end
function Seat:_onGiftLoaded()
  if self._giftImage.sprite then
    local texture = self._giftImage.sprite:getTexture()
    self._giftButton:setButtonImage("normal", texture)
    self._giftButton:setButtonImage("pressed", texture)
    self._giftButton:scale(48 / self._giftImage.imageSize.width)
    self._giftImage:removeFromParent()
  end
end
function Seat:setPlayStatusString(value)
  self._playStatusBar:setString(g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 24, value, 100))
end
function Seat:showCountDown(leftSeconds)
  self.seatClock:clearData()
  self.seatClock.isSelf = self._player:getIsSelf()
  self.seatClock:startCount(leftSeconds)
end
function Seat:showGrayCover(visible)
  if visible then
    self._grayCover:show()
  else
    self._grayCover:hide()
  end
end
function Seat:clearData()
  self:clearHandData()
  self:stopSeatChipsAnimation()
  self._player = nil
  self._seatChips = 0
  self._giftId = 0
  self.seatClock:clearData()
  self._playStatusBar:hide()
  self._seatChipsLabel:hide()
  self._pokersInSeat:clearData()
end
function Seat:clearHandData()
  self.seatClock:clearData()
  self._pokersInSeat:clearData()
  self._smallPokersInSeat:clearData()
  self:hideSmallPokersInSeat()
  self:hideSeatWinAnimation()
end
function Seat:resetSeatClock()
  self.seatClock:clearData()
end
function Seat:_clickSeatToSitDown()
  if self._player == nil then
    self:_requestSitDown()
  end
end
function Seat:_clickSeatToShowProfileInfo()
  if self._player:getIsSelf() then
    local profileView = ProfileView.new()
    profileView:show()
  else
    local otherUserProfileView = OtherUserProfileView.new(self._player)
    otherUserProfileView:showPanel()
  end
end
function Seat:_requestSitDown()
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.SIT_DOWN_REQUEST,
    data = {
      seatId = self._seatId
    }
  })
end
function Seat:sitDownAnimation(animation)
  self:cancelSitOrStandAnimation()
  if animation then
    transition.moveTo(self._avatarImageView:pos(0, self.avatarHeight):show(), {
      time = 0.5,
      easing = "backOut",
      x = 0,
      y = 0
    })
    transition.moveTo(self._seatAvailableImage:pos(0, -10):show(), {
      time = 0.5,
      easing = "backOut",
      x = 0,
      y = -self.avatarHeight,
      onComplete = function()
        self._seatAvailableImage:hide()
        if self._player:getIsSelf() then
          transition.scaleTo(self, {scale = 1, time = 0.2})
        end
      end
    })
  else
    self._avatarImageView:pos(0, 0)
    self._seatAvailableImage:pos(0, -self.avatarHeight)
    if self._player:getIsSelf() then
      self:scale(1)
    else
      self:scale(0.9)
    end
  end
  if g.vars.user.giftShop == 1 then
    self._giftButton:show()
  end
  self._seatChipsLabelBG:show()
end
function Seat:standUpAnimation()
  self:cancelSitOrStandAnimation()
  transition.moveTo(self._avatarImageView:pos(0, 0):show(), {
    time = 0.5,
    easing = "backOut",
    x = 0,
    y = self.avatarHeight + 10
  })
  transition.moveTo(self._seatAvailableImage:pos(0, -self.avatarHeight):show(), {
    time = 0.5,
    easing = "backOut",
    x = 0,
    y = -10,
    onComplete = function()
      self._avatarImageView:hide()
      if self:getScale() ~= 0.9 then
        transition.scaleTo(self, {scale = 0.9, time = 0.2})
      end
    end
  })
  if g.vars.user.giftShop == 1 then
    self._giftButton:hide()
  end
  self._seatChipsLabelBG:hide()
end
function Seat:cancelSitOrStandAnimation()
  if self._avatarImageView then
    self._avatarImageView:stopAllActions()
  end
  if self._seatAvailableImage then
    self._seatAvailableImage:stopAllActions()
  end
end
function Seat:showSeatWinAnimation(seatWinCompleteCallback)
  self._seatWinView:scale(0.1)
  self._seatWinView:show()
  transition.scaleTo(self._seatWinView, {
    time = 0.3,
    scaleX = 1,
    scaleY = 1,
    easing = "BACKOUT"
  })
end
function Seat:hideSeatWinAnimation()
  self._seatWinView:hide()
end
function Seat:setSeatChips(value)
  if self._seatChips ~= value then
    self:stopSeatChipsAnimation()
    self._helpSprite:pos(self._seatChips, 0)
    self._seatChips = value
    self:playSeatChipsAnimation()
  end
end
function Seat:playSeatChipsAnimation()
  transition.moveTo(self._helpSprite, {
    x = self._seatChips,
    time = 0.25,
    easeing = "backOut",
    onComplete = function()
      self:stopSeatChipsAnimation()
      self._seatChipsLabel:setString(g.util.functions.formatBigInt(self._seatChips))
    end
  })
  if self._seatChipsEnterFrameHandle then
    self._seatChipsEnterFrameHandle = scheduler.scheduleUpdateGlobal(handler(self, self.seatChipsAnimationEnterFrame))
  end
end
function Seat:stopSeatChipsAnimation()
  transition.stopTarget(self._helpSprite)
  if self._seatChipsEnterFrameHandle ~= nil then
    scheduler.unscheduleGlobal(self._seatChipsEnterFrameHandle)
    self._seatChipsEnterFrameHandle = nil
  end
end
function Seat:seatChipsAnimationEnterFrame()
  print("~~~~~~~~~~~~~~ testing data")
  self._seatChipsLabel:setString(g.util.functions.formatBigInt(math.ceil(self._helpSprite:getPositionX())))
end
function Seat:playPokAnimation(animate)
  self._pokersInSeat:playPokAnimation(animate)
end
function Seat:_updatePokTypePositon()
  self._pokersInSeat:updatePokTypePostion(self:getVirtualSeatId())
end
function Seat:setSeatPokersToBeInvisible()
  self._pokersInSeat:clearData()
end
function Seat:showNthPokerInSeat(roundNumber, totalRounds)
  local isSelf = self:isSelfSeat()
  self._pokersInSeat:showNthPokerInSeat(roundNumber, totalRounds, isSelf)
end
function Seat:showPokersInSeat(roundNumber)
  for i = 1, roundNumber do
    self:showNthPokerInSeat(i, roundNumber)
  end
end
function Seat:showPokersInSeatInTurnOverState()
  self._pokersInSeat:showPokersInSeatInTurnOverState()
end
function Seat:showPokersInSeatInCoverState()
  self._pokersInSeat:showPokersInSeatInCoverState()
end
function Seat:hideSmallPokersInSeat()
  self._smallPokersInSeat:hideSmallPokersInSeat()
end
function Seat:showNthSmallPokerInSeat(roundNumber, totalRounds)
  if self._player == nil then
    return
  end
  local seatId = self:getSeatId()
  local virtualSeatId = self:getVirtualSeatId()
  local isSelf = self:isSelfSeat()
  self._smallPokersInSeat:showNthSmallPokerInSeat(roundNumber, totalRounds, seatId, virtualSeatId, isSelf)
end
function Seat:showSmallPokersInSeat(roundNumber)
  for i = 1, roundNumber do
    self:showNthSmallPokerInSeat(i, roundNumber)
  end
end
function Seat:showPokersInSeatCardType()
  local cardsInHandModel = self._player:getCardsInHandModel()
  local cardType = cardsInHandModel:getCardType()
  if cardType ~= nil then
  end
  if self._player ~= nil then
    self._pokersInSeat:showPokersInSeatCardType(self._player:getCardsTypeDescription())
  end
end
function Seat:playIncreasePokerInSeatAnimation(roundNumber)
  local isSelf = self:isSelfSeat()
  self._pokersInSeat:playIncreasePokerInSeatAnimation(roundNumber, isSelf)
end
function Seat:playIncreaseSmallPokerInSeatAnimation(roundNumber, increaseSmallPokerInSeatCompleteCallback)
  local seatId = self:getSeatId()
  local virtualSeatId = self:getVirtualSeatId()
  local isSelf = self:isSelfSeat()
  self._smallPokersInSeat:playIncreaseSmallPokerInSeatAnimation(roundNumber, seatId, virtualSeatId, isSelf, increaseSmallPokerInSeatCompleteCallback)
end
function Seat:playTurnOverNthPokersInSeatAnimation(Nth, turnOverPokersCompleteCallback)
  self._pokersInSeat:playTurnOverNthPokersInSeatAnimation(Nth, turnOverPokersCompleteCallback)
end
function Seat:playTurnOverPokersInSeatAnimation(turnOverPokersCompleteCallback)
  self._pokersInSeat:playTurnOverPokersInSeatAnimation(turnOverPokersCompleteCallback)
end
function Seat:playSmallPokersInSeatToSeatCenterAnimation(smallPokersMovedCompleteCallback)
  self._smallPokersInSeat:playSmallPokersInSeatToSeatCenterAnimation(smallPokersMovedCompleteCallback)
end
function Seat:getSeatId()
  return self._seatId
end
function Seat:getSeatIdForServer()
  return self._seatId - 1
end
function Seat:getVirtualSeatId()
  return self._virtualSeatId
end
function Seat:setVirtualSeatId(virtualSeatId)
  self._virtualSeatId = virtualSeatId
  self:_updatGiftPositon()
  self:_updatePokTypePositon()
end
function Seat:_updatGiftPositon()
  if g.vars.user.giftShop == 1 then
    if self._virtualSeatId == 1 or self._virtualSeatId == 2 or self._virtualSeatId == 3 or self._virtualSeatId == 5 then
      self._giftButton:pos(-55, 0)
    else
      self._giftButton:pos(55, 0)
    end
  end
end
function Seat:isSelfSeat()
  if self._player ~= nil then
    return self._player:getIsSelf()
  else
    return false
  end
end
function Seat:isEmpty()
  if self._player == nil then
    return true
  else
    return false
  end
end
return Seat

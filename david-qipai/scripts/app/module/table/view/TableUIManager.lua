local Seat = import(".Seat")
local ChipsSpot = import(".ChipsSpot")
local SearchLight = import(".SearchLight")
local TableUIConfig = import(".TableUIConfig")
local TableUIEvent = import(".TableUIEvent")
local Pokers = import(".Pokers")
local ChipsSpot = import(".ChipsSpot")
local BigWinView = import(".BigWinView")
local Dealer = import(".Dealer")
local JewelBox = import(".JewelBox")
local ChatMessageBubble = import(".ChatMessageBubble")
local UserBankruptPopup = require("app.module.table.view.UserBankruptPopup")
local MagicEmoji = import(".MagicEmoji")
local ControlPanel = import(".ControlPanel")
local BetClock = import(".BetClock")
local BuyInSettingPopup = import(".BuyInSettingPopup")
local PlayStatus = require("app.beans.table.model.PlayStatus")
local TableMessageList = require("app.module.table.controller.TableMessageList")
local TableAnimationManager = require("app.module.table.controller.TableAnimationManager")
local ViceJudge = require("app.module.table.model.ViceJudge")
local ChatPanel = import(".ChatPanel")
local PaymentView = require("app.beans.payment.view.PaymentView")
local FriendListView = require("app.beans.friend.view.FriendListView")
local TableUIManager = class("TableUIManager")
TableUIManager.SEAT_MOVE_STEP_TIME = 0.1
function TableUIManager:ctor(scene)
  if DEBUG_MEMORY_STATUS then
    logger:debug("Object-Life:" .. self.__cname .. ": ctor")
    g.memoryManager:registerObject(self, "TableUIManager")
  end
  self._dealerVirtualSeatId = -1
  self._seatCount = 9
  self._tableScene = scene
  self:_createPlaceHolder()
  self:_drawNodesContent()
  self._clearTabelTimer = nil
  self._tableAnimationManager = TableAnimationManager.new()
end
function TableUIManager:_createPlaceHolder()
  self.nodes = {}
  self.nodes.backgroundNode = display.newNode():addTo(self._tableScene)
  self.nodes.dealerNode = display.newNode():addTo(self._tableScene)
  self.nodes.staticPokersNode = display.newNode():addTo(self._tableScene)
  self.nodes.seatNode = display.newNode():addTo(self._tableScene)
  self.nodes.wagerNode = display.newNode():addTo(self._tableScene)
  self.nodes.pokersNode = display.newNode():addTo(self._tableScene)
  self.nodes.lightNode = display.newNode():addTo(self._tableScene)
  self.nodes.operateNode = display.newNode():addTo(self._tableScene)
  self.nodes.animationNode = display.newNode():addTo(self._tableScene)
  self.nodes.animationPersistNode = display.newNode():addTo(self._tableScene)
  self.nodes.backgroundNode:setTouchEnabled(true)
  self.nodes.backgroundNode:setTouchSwallowEnabled(true)
  self.nodes.backgroundNode:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onBackgroundTouch))
end
function TableUIManager:_onBackgroundTouch(evt)
  self._controlPanel:hideRaisePanelAnimation()
end
function TableUIManager:_drawNodesContent()
  local carpet = display.newSprite("pd_table_bg.jpg"):pos(TableUIConfig.CARPET_CENTER.x, TableUIConfig.CARPET_CENTER.y):addTo(self.nodes.backgroundNode)
  local carpetWidth = carpet:getContentSize().width
  if carpetWidth < display.width and carpetWidth > 0 then
    carpet:setScale(display.width / carpetWidth)
  end
  loadTexture("pd_table_texture.plist", "pd_table_texture.png")
  loadTexture("pd_card_texture.plist", "pd_card_texture.png")
  local tableSpriteLeft = display.newSprite("#table_left.png")
  tableSpriteLeft:setAnchorPoint(ccp(1, 0.5))
  tableSpriteLeft:pos(display.cx + 1 - TableUIConfig.TABLE_EXTENT_LENGTH * 0.5, TableUIConfig.TABLE_CENTER.y):addTo(self.nodes.backgroundNode)
  local tableSpriteRight = display.newSprite("#table_left.png")
  local tableHeight = tableSpriteRight:getContentSize().height
  tableSpriteRight:setAnchorPoint(ccp(1, 0.5))
  tableSpriteRight:setScaleX(-1)
  tableSpriteRight:pos(display.cx - 1 + TableUIConfig.TABLE_EXTENT_LENGTH * 0.5, TableUIConfig.TABLE_CENTER.y):addTo(self.nodes.backgroundNode)
  local tableSpriteMiddle = display.newScale9Sprite("#table_middle.png")
  local middleTableHeight = tableSpriteMiddle:getContentSize().height
  tableSpriteMiddle:pos(display.cx, TableUIConfig.TABLE_CENTER.y - 4)
  tableSpriteMiddle:setContentSize(cc.size(TableUIConfig.TABLE_EXTENT_LENGTH, middleTableHeight))
  tableSpriteMiddle:addTo(self.nodes.backgroundNode)
  self.tableInfo = ui.newTTFLabel({
    size = 20,
    text = "",
    color = g.font.color.GREY
  }):pos(TableUIConfig.TABLEINFO_CENTER.x, TableUIConfig.TABLEINFO_CENTER.y - 35):addTo(self.nodes.backgroundNode)
  local dealer = display.newSprite("#pd_table_dealer_girl.png"):pos(TableUIConfig.DEALER_CENTER.x, TableUIConfig.DEALER_CENTER.y + 22):addTo(self.nodes.dealerNode)
  self._tableDealerIcon = display.newSprite("#pd_table_dealer.png"):addTo(self.nodes.dealerNode):hide()
  self.seats = {}
  for i, point in ipairs(TableUIConfig.SEAT) do
    local seat = Seat.new(i):pos(point.x, point.y):addTo(self.nodes.seatNode, 9 - i)
    table.insert(self.seats, seat)
  end
  self.chipsSpots = {}
  for i, point in ipairs(TableUIConfig.WAGERVIEW) do
    local chipsSpot = ChipsSpot.new():addTo(self.nodes.wagerNode)
    chipsSpot:setSeatId(i)
    table.insert(self.chipsSpots, chipsSpot)
  end
  self._pokers = Pokers.new(self.nodes.staticPokersNode):addTo(self.nodes.pokersNode)
  self._searchLight = SearchLight.new()
  self._searchLight:addTo(self.nodes.lightNode):hide()
  self._returnToLobbyTransParentButton = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#transparent.png"
  }, {scale9 = true}):addTo(self.nodes.operateNode):onButtonClicked(clickHandler(self, self._onReturnBackToLobbyClick))
  self._returnToLobbyTransParentButton:setButtonSize(90, 90)
  self._returnToLobbyTransParentButton:pos(62, display.height - 42)
  self._returnToLobbyButton = cc.ui.UIPushButton.new({
    normal = "#common_back_normal.png",
    pressed = "#common_back_pressed.png"
  }):onButtonClicked(clickHandler(self, self._onReturnBackToLobbyClick)):addTo(self.nodes.operateNode)
  self._returnToLobbyButton:pos(62, display.height - 42)
  self._standUpTransparentButton = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#transparent.png"
  }, {scale9 = true}):addTo(self.nodes.operateNode):onButtonClicked(clickHandler(self, self._onStandupClick))
  self._standUpTransparentButton:setButtonSize(90, 90)
  self._standUpTransparentButton:pos(148, display.height - 42)
  self._standUpButton = cc.ui.UIPushButton.new({
    normal = "#table_standup_normal.png",
    pressed = "#table_standup_pressed.png"
  }):onButtonClicked(clickHandler(self, self._onStandupClick)):addTo(self.nodes.operateNode):hide()
  self._standUpButton:pos(148, display.height - 42)
  self._addFriendsTransparentButton = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#transparent.png"
  }, {scale9 = true}):addTo(self.nodes.operateNode):onButtonClicked(clickHandler(self, self._onAddFriendsClick))
  self._addFriendsTransparentButton:setButtonSize(90, 90)
  self._addFriendsTransparentButton:pos(display.width - 62 - 86, display.height - 42)
  self._addFriendsButton = cc.ui.UIPushButton.new({
    normal = "#table_invite_friend_up.png",
    pressed = "#table_invite_friend_down.png"
  }):onButtonClicked(clickHandler(self, self._onAddFriendsClick)):addTo(self.nodes.operateNode)
  self._addFriendsButton:pos(display.width - 62 - 86, display.height - 42)
  self._rechargeTransparentButton = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#transparent.png"
  }, {scale9 = true}):addTo(self.nodes.operateNode):onButtonClicked(clickHandler(self, self.onPayButtonClick))
  self._rechargeTransparentButton:setButtonSize(90, 90)
  self._rechargeTransparentButton:pos(display.width - 62, display.height - 42)
  self._rechargeButton = cc.ui.UIPushButton.new({
    normal = "#common_recharge_normal.png",
    pressed = "#common_recharge_pressed.png"
  }, {scale9 = false}):onButtonClicked(clickHandler(self, self.onPayButtonClick)):addTo(self.nodes.operateNode)
  self._rechargeButton:pos(display.width - 62, display.height - 42)
  self._sendChipToDealerSprite = display.newSprite("#table_send_girl_chip_icon.png"):addTo(self.nodes.operateNode):pos(display.cx - 68, display.height - 60)
  self._sendChipToDealerSprite:setTouchEnabled(true)
  self._sendChipToDealerSprite:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._sendGirlChipsOnTouch))
  self._chatBtn = cc.ui.UIPushButton.new({
    normal = "#table_chat_btn_normal.png",
    pressed = "#table_chat_btn_pressed.png"
  }):onButtonClicked(clickHandler(self, self._onChatBtnClick)):pos(55, 40):addTo(self.nodes.operateNode)
  self._controlPanel = ControlPanel.new(handler(self, self.controlPanelStateWillChange)):addTo(self.nodes.operateNode)
  self._controlPanel.seats = self.seats
  self._controlPanel:setPlayStatus(PlayStatus.NON_PLAY)
  self._betClock = BetClock.new():addTo(self.nodes.operateNode)
  self._betClock:setCascadeOpacityEnabled(true)
  self._betClock:setOpacity(0)
end
function TableUIManager:controlPanelStateWillChange(fromState, toState)
  if toState == PlayStatus.BETTING then
    self._chatBtn:hide()
  else
    self._chatBtn:show()
  end
end
function TableUIManager:_sendGirlChipsOnTouch(evt)
  self._touchInSprite = self._sendChipToDealerSprite:getCascadeBoundingBox():containsPoint(CCPoint(evt.x, evt.y))
  if evt.name == "began" then
    self._sendChipToDealerSprite:scaleTo(0.05, 0.9)
    self._clickCancel = false
    return true
  elseif evt.name == "moved" then
    if not self._touchInSprite and not self._clickCancel then
      self._sendChipToDealerSprite:scaleTo(0.05, 1)
      self._clickCancel = true
    end
  elseif (evt.name == "ended" or name == "cancelled") and not self._clickCancel then
    clickSoundImmediately()
    self._sendChipToDealerSprite:scaleTo(0.05, 1)
    self:_onSendChipToDealerClick()
  end
end
function TableUIManager:getSeatBySeatID(seatId)
  for i, seat in ipairs(self.seats) do
    local currentSeatID = seat:getSeatId()
    if currentSeatID == seatId then
      return seat
    end
  end
end
function TableUIManager:getSeatByVirtualSeatID(virtualSeatId)
  for i, seat in ipairs(self.seats) do
    local currentVirtualSeatID = seat:getVirtualSeatId()
    if currentVirtualSeatID == virtualSeatId then
      return seat
    end
  end
end
function TableUIManager:getEmptySeat()
  for i, seat in ipairs(self.seats) do
    if seat:isEmpty() then
      if self._seatCount == 5 then
        local seatId = seat:getSeatId()
        if seatId ~= 2 and seatId ~= 4 and seatId ~= 6 and seatId ~= 8 then
          return seat
        end
      else
        return seat
      end
    end
  end
  return nil
end
function TableUIManager:getChipsSpotBySeatID(seatId)
  for i, chipsSpot in ipairs(self.chipsSpots) do
    if chipsSpot:getSeatId() == seatId then
      return chipsSpot
    end
  end
end
function TableUIManager:responseToEnterBroadcast(judge)
  local tableInfoText = ""
  tableInfoText = g.lang:getContent("lobby.tableType")[tonumber(judge._tableLevel or 1)]
  tableInfoText = tableInfoText .. string.gsub(g.lang:getContent("table.palyerCountLabel"), "{count}", judge:getSeatCount())
  tableInfoText = tableInfoText .. " : "
  tableInfoText = tableInfoText .. (judge._tableName or "")
  tableInfoText = tableInfoText .. " : "
  tableInfoText = tableInfoText .. "$" .. g.util.functions.formatBigInt(judge._minBet)
  self.tableInfo:setString(tableInfoText)
  self._seatCount = judge:getSeatCount()
  self._controlPanel.seatCount = self._seatCount
  local mySeatId
  for i, player in ipairs(judge.gamblers) do
    local seatId = player:getSeatId()
    local seat = self:getSeatBySeatID(seatId)
    seat:bindData(player)
    seat:sitDownAnimation(false)
    if player:getIsSelf() then
      mySeatId = player:getSeatId()
    end
    local virtualSeatId = seat:getVirtualSeatId()
    if judge:getBankerSeatId() == seatId then
      self._dealerVirtualSeatId = virtualSeatId
      self:showTableDealerIconAtVirtualSeatId(virtualSeatId, false)
    end
    local chipsSpot = self:getChipsSpotBySeatID(seatId)
    chipsSpot:bindData(player:getBet())
    chipsSpot:showForVirtualSeatId(virtualSeatId)
    if player:getIsFlipedCard() == 1 then
      seat:showPokersInSeat(player:getCardCount())
      seat:showPokersInSeatInTurnOverState()
      seat:showPokersInSeatCardType()
    elseif player:getIsSelf() and player:getCardCount() > 0 then
      seat:showPokersInSeat(player:getCardCount())
      seat:showPokersInSeatInTurnOverState()
      seat:showPokersInSeatCardType()
    else
      seat:showSmallPokersInSeat(player:getCardCount())
    end
    if player:getPlayStatus() == PlayStatus.POK then
      seat:playPokAnimation(false)
    end
  end
  if self._dealerVirtualSeatId == -1 then
    self:showTableDealerIconAtVirtualSeatId(10, false)
  end
  local isSitDown = judge:getIsImPlayer()
  self:onResponseToSelfSitOrStandup(isSitDown, mySeatId, judge)
  if isSitDown then
    local player = judge:getPlayerBySeatId(mySeatId)
    self:onResponseToPlayStatus(player)
  end
  self._controlPanel:setBet(g.vars.table.loginResponse.minBet)
  self._controlPanel:setMinBet(g.vars.table.loginResponse.minBet)
  self._controlPanel:setMaxBet(g.vars.table.loginResponse.maxBet)
end
function TableUIManager:onResponseToSitDown(player)
  local seatId = player:getSeatId()
  local seat = self:getSeatBySeatID(seatId)
  seat:bindData(player)
  seat:sitDownAnimation(true)
  if player:getIsSelf() then
    g.audio:playSound("SIT_DOWN")
    self._controlPanel:setPlayStatus(player:getPlayStatus())
  end
end
function TableUIManager:onResponseToSitDownFail(errorCode)
  if errorCode == 1 then
    g.ui.manager.tip:top(g.lang:getContent("table.sitdownFailSystemError"))
  elseif errorCode == 2 then
    g.ui.manager.tip:top(g.lang:getContent("table.sitdownFailSeatIdError"))
  elseif errorCode == 5 then
    g.ui.manager.tip:top(g.lang:getContent("table.sitdownFailNotLogin"))
  elseif errorCode == 6 then
    g.ui.manager.tip:top(g.lang:getContent("table.sitdownFailChipsNotEnough"))
    g.ui.Dialog.new({
      contentText = g.lang:getContent("table.buyinNotEnough"),
      confirmButtonText = g.lang:getContent("shop.buyLabel"),
      dialogType = g.ui.DialogType.NORMAL,
      callback = function(sender)
        if sender == g.ui.DialogConst.CONFIRM_BUTTON_CLICK then
          PaymentView.new():show()
        end
      end
    }):show()
  elseif errorCode == 7 then
    g.ui.manager.tip:top(g.lang:getContent("table.sitdownFailExceedBuyIn"))
  elseif errorCode == 9 then
  elseif errorCode == 16 then
    g.ui.manager.tip:top(g.lang:getContent("table.sitdownFailOtherSit"))
  elseif errorCode == 18 then
    g.ui.manager.tip:top(g.lang:getContent("table.sitdownFailSameIP"))
  end
end
function TableUIManager:onResponseToSelfSitOrStandup(isSitDown, seatId, judge)
  local function seatsRotateComplete()
    if isSitDown then
      self:_showSitAnchor()
    end
  end
  if isSitDown then
    self._standUpButton:show()
    self._standUpTransparentButton:show()
    self:hideEmptySeats()
    self:seatsRotate(seatId, seatsRotateComplete)
  else
    if judge:getBankerSeatId() == -1 or judge:getBankerSeatId() == nil then
      self._dealerVirtualSeatId = -1
    end
    self:_hideSitAnchor()
    self._standUpButton:hide()
    self._standUpTransparentButton:hide()
    self:showEmptySeat()
    self:resetRotateSeatsNoraml()
    self._controlPanel:setPlayStatus(PlayStatus.STAND)
  end
  if judge:getSeatCount() == 5 then
    for i, seat in ipairs(self.seats) do
      if i % 2 == 0 then
        seat:hide()
      end
    end
  end
end
function TableUIManager:onResponseToOtherSitOrStandup(isSitDown, seatId, judge)
  if not judge:getIsImPlayer() then
    self._controlPanel:setPlayStatus(PlayStatus.STAND)
  end
  if judge:getBankerSeatId() == -1 or judge:getBankerSeatId() == nil then
    self._dealerVirtualSeatId = -1
  end
end
function TableUIManager:onResponseToPlayStatus(player)
  self._controlPanel:setPlayStatus(player:getPlayStatus(), player)
end
function TableUIManager:_showSitAnchor()
  self:_hideSitAnchor()
  local seatPosition = TableUIConfig.SEAT[5]
  local startPoint = ccp(seatPosition.x, seatPosition.y + 140)
  local endPoint = ccp(seatPosition.x, seatPosition.y + 110)
  self.arrow = display.newSprite("#table_self_seat_arrow.png")
  self.arrow:setPosition(startPoint)
  self.arrow:addTo(self.nodes.animationNode)
  transition.execute(self.arrow, CCRepeat:create(transition.sequence({
    CCMoveTo:create(0, startPoint),
    CCMoveTo:create(0.5, endPoint),
    CCMoveTo:create(0.2, startPoint)
  }), 3.5), {
    onComplete = function()
      self.arrow:removeFromParent()
      self.arrow = nil
    end
  })
end
function TableUIManager:_hideSitAnchor()
  if self.arrow then
    self.arrow:stopAllActions()
    self.arrow:removeFromParent()
    self.arrow = nil
  end
end
function TableUIManager:responseToGameStartBroadcast(judge)
  self:clearTable()
  local seat = self:getSeatBySeatID(judge:getBankerSeatId())
  self._dealerVirtualSeatId = seat:getVirtualSeatId()
  local ImBanker = false
  for i, player in ipairs(judge.gamblers) do
    if player:getPlayStatus() == PlayStatus.WAIT_FOR_BET then
      local seatId = player:getSeatId()
      local seat = self:getSeatBySeatID(seatId)
      seat:bindData(player)
    end
    if player:getIsSelf() and judge:getBankerSeatId() == player:getSeatId() then
      ImBanker = true
    end
  end
  local ImPlayer = judge:getIsImPlayer()
  self:removeClearTableTimer()
  self:clearTable()
  if not ImPlayer or ImBanker then
    if not ImPlayer then
    else
      self._controlPanel:hideButtonsByTween()
    end
    self._betClock:startCountDown(judge:getBetTime())
  else
    self._controlPanel:setPlayStatus(PlayStatus.WAIT_FOR_BET)
  end
  self._dealerVirtualSeatId = seat:getVirtualSeatId()
  self:showTableDealerIconAtVirtualSeatId(seat:getVirtualSeatId(), true)
end
function TableUIManager:showClockOnSeat(seatId, leftSeconds)
  local seat = self:getSeatBySeatID(seatId)
  leftSeconds = leftSeconds - 0.2
  seat:showCountDown(leftSeconds)
end
function TableUIManager:playerBetTurn(player, leftSeconds)
  local seatId = player:getSeatId()
  if not self._searchLight:getIsWorking() then
    self._searchLight:show()
    self:playSearchLightOnSeatAnimation(seatId, false)
    self._searchLight:setIsWorking(true)
  else
    self:playSearchLightOnSeatAnimation(seatId, true)
  end
  self:showClockOnSeat(seatId, leftSeconds)
  if player:getIsSelf() then
    self._controlPanel:setMinBet(player:getMinBet())
    self._controlPanel:setMaxBet(player:getMaxBet())
    self._controlPanel:setPlayStatus(PlayStatus.BETTING)
    self._betClock:hideImediatly()
  end
end
function TableUIManager:playerBetted(player)
  local seatId = player:getSeatId()
  local seat = self:getSeatBySeatID(seatId)
  local virtualSeatId = seat:getVirtualSeatId()
  seat:bindData(player)
  seat.seatClock:clearData()
  local chipsSpot = self:getChipsSpotBySeatID(seatId)
  chipsSpot:bindData(player:getBet())
  local wagerCenterPosition = TableUIConfig.WAGERVIEW[virtualSeatId]
  local seatCenterPosition = TableUIConfig.SEAT[virtualSeatId]
  chipsSpot:playChipsStacksMoveAnimation(seatCenterPosition, wagerCenterPosition)
  g.audio:playSound("BET")
  if player:getIsSelf() then
    self._controlPanel:disableBetButtons()
    self._chatBtn:show()
  end
end
function TableUIManager:hideEmptySeats()
  for i, seat in ipairs(self.seats) do
    if seat:isEmpty() then
      seat:hide()
    end
  end
end
function TableUIManager:showEmptySeat()
  for i, seat in ipairs(self.seats) do
    if seat:isEmpty() then
      seat:show()
    end
  end
end
function TableUIManager:clearSeatData(seatId, isSelf)
  local seat = self:getSeatBySeatID(seatId)
  seat:clearData()
  local virtualSeatId = seat:getVirtualSeatId()
  local chipsSpot = self:getChipsSpotBySeatID(seatId)
  chipsSpot:clearData()
  if isSelf then
    self._controlPanel:setPlayStatus(PlayStatus.STAND)
  end
end
function TableUIManager:dispatchPokers(judge)
  logger:debug("TableUIManager:dispatchPokers")
  logger:debug("立即隐藏下注倒计时")
  self._betClock:hideWithAnimation()
  self._searchLight:stopAllAnimation()
  self._searchLight:fadeOut()
  local seatIds = {}
  local virtualSeatIds = {}
  local mySeatId, bankerSeatId
  for i, player in ipairs(judge.gamblers) do
    if player:getPlayStatus() == PlayStatus.BETTED or player:getIsBanker() then
      local seatId = player:getSeatId()
      if player:getIsSelf() then
        mySeatId = seatId
      end
      if player:getIsBanker() then
        bankerSeatId = seatId
      end
      table.insert(seatIds, seatId)
      local seat = self:getSeatBySeatID(seatId)
      seat:bindData(player)
      local data1 = {
        eventName = TableMessageList.BROADCAST_POK,
        plus = 2
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data1
      })
      local data2 = {
        eventName = TableMessageList.BROADCAST_SPEAK_TURN,
        plus = 2
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data2
      })
      local data3 = {
        eventName = TableMessageList.BROADCAST_SPEAK,
        plus = 2
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data3
      })
      local data4 = {
        eventName = TableMessageList.BROADCAST_RESULT,
        plus = 2
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data4
      })
    end
  end
  local sortFunc = function(a, b)
    return a < b
  end
  table.sort(seatIds, sortFunc)
  local dispatchOrders = {}
  local j = 1
  for i, seatId in ipairs(seatIds) do
    if bankerSeatId >= seatId then
      table.insert(dispatchOrders, seatId)
    else
      table.insert(dispatchOrders, j, seatId)
      j = j + 1
    end
  end
  seatIds = dispatchOrders
  for i, seatId in ipairs(seatIds) do
    local seat = self:getSeatBySeatID(seatId)
    table.insert(virtualSeatIds, seat:getVirtualSeatId())
  end
  local dealCardCount = 2
  local totoalCard = dealCardCount * #seatIds
  local function dealCardCallback(roundNumber, seatId, singleHandPoker, isFinalCard)
    singleHandPoker:hide()
    if isFinalCard then
      self._betClock:hideImediatly()
    end
    local seat = self:getSeatBySeatID(seatId)
    if mySeatId ~= nil and mySeatId == seatId then
      local function turnOverPokersCompleteCallback()
        totoalCard = totoalCard - 1
        if totoalCard == 0 then
          local isPlayInCurrentGame = false
          if judge:getImInCurrentGame() then
            self._controlPanel:setPlayStatus(PlayStatus.BETTED)
          end
          seat:showPokersInSeatCardType()
        end
        local data1 = {
          eventName = TableMessageList.BROADCAST_POK,
          plus = -1
        }
        g.eventCenter:dispatchEvent({
          name = TableMessageList.DISPATCH_EVENT,
          data = data1
        })
        local data2 = {
          eventName = TableMessageList.BROADCAST_SPEAK_TURN,
          plus = -1
        }
        g.eventCenter:dispatchEvent({
          name = TableMessageList.DISPATCH_EVENT,
          data = data2
        })
        local data3 = {
          eventName = TableMessageList.BROADCAST_SPEAK,
          plus = -1
        }
        g.eventCenter:dispatchEvent({
          name = TableMessageList.DISPATCH_EVENT,
          data = data3
        })
        local data4 = {
          eventName = TableMessageList.BROADCAST_RESULT,
          plus = -1
        }
        g.eventCenter:dispatchEvent({
          name = TableMessageList.DISPATCH_EVENT,
          data = data4
        })
      end
      if seat:isSelfSeat() then
        seat:showNthPokerInSeat(roundNumber, dealCardCount)
        seat:playTurnOverNthPokersInSeatAnimation(roundNumber, turnOverPokersCompleteCallback)
      else
        turnOverPokersCompleteCallback()
      end
    else
      totoalCard = totoalCard - 1
      if totoalCard == 0 and judge:getImInCurrentGame() then
        self._controlPanel:setPlayStatus(PlayStatus.BETTED)
      end
      seat:showNthSmallPokerInSeat(roundNumber, dealCardCount)
      local data1 = {
        eventName = TableMessageList.BROADCAST_POK,
        plus = -1
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data1
      })
      local data2 = {
        eventName = TableMessageList.BROADCAST_SPEAK_TURN,
        plus = -1
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data2
      })
      local data3 = {
        eventName = TableMessageList.BROADCAST_SPEAK,
        plus = -1
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data3
      })
      local data4 = {
        eventName = TableMessageList.BROADCAST_RESULT,
        plus = -1
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data4
      })
    end
  end
  self._pokers:dealCards(dealCardCount, seatIds, virtualSeatIds, mySeatId, dealCardCallback)
end
function TableUIManager:pok(judge)
  self._searchLight:stopAllAnimation()
  self._searchLight:fadeOut()
  local hasPlayPokSound = false
  for i, player in ipairs(judge.gamblers) do
    if player:getPlayStatus() == PlayStatus.POK then
      do
        local seatId = player:getSeatId()
        local seat = self:getSeatBySeatID(seatId)
        seat:bindData(player)
        local data = {
          eventName = TableMessageList.BROADCAST_RESULT,
          plus = 1
        }
        g.eventCenter:dispatchEvent({
          name = TableMessageList.DISPATCH_EVENT,
          data = data
        })
        local function turnOverPokersCompleteCallback()
          local data = {
            eventName = TableMessageList.BROADCAST_RESULT,
            plus = -1
          }
          g.eventCenter:dispatchEvent({
            name = TableMessageList.DISPATCH_EVENT,
            data = data
          })
          seat:showPokersInSeatCardType()
          seat:playPokAnimation(true)
          if not hasPlayPokSound then
            g.audio:playSound("POK")
            hasPlayPokSound = true
          end
        end
        local function smallPokersMovedCompleteCallback()
          seat:hideSmallPokersInSeat()
          seat:showPokersInSeat(player:getCardCount())
          seat:playTurnOverPokersInSeatAnimation(turnOverPokersCompleteCallback)
        end
        if player:getIsSelf() then
          self._controlPanel:setPlayStatus(PlayStatus.POK)
          turnOverPokersCompleteCallback()
        else
          seat:playSmallPokersInSeatToSeatCenterAnimation(smallPokersMovedCompleteCallback)
        end
      end
    end
  end
end
function TableUIManager:speakTurn(player, leftSeconds)
  local seatId = player:getSeatId()
  self:showClockOnSeat(seatId, leftSeconds)
  if not self._searchLight:getIsWorking() then
    self._searchLight:show()
    self:playSearchLightOnSeatAnimation(seatId, false)
    self._searchLight:setIsWorking(true)
  else
    self._searchLight:show()
    self:playSearchLightOnSeatAnimation(seatId, true)
  end
  if player:getIsSelf() then
    self._controlPanel:setPlayStatus(PlayStatus.SPEAKING, player)
  end
end
function TableUIManager:speakContent(player, operation)
  local seat = self:getSeatBySeatID(player:getSeatId())
  seat.seatClock:clearData()
  if operation == 1 then
    do
      local cards = player._cardsInHandModel:getCards()
      local virtualSeatId = seat:getVirtualSeatId()
      local seatId = player:getSeatId()
      local isSelf = player:getIsSelf()
      local seat = self:getSeatBySeatID(seatId)
      seat:bindData(player)
      local data = {
        eventName = TableMessageList.BROADCAST_RESULT,
        plus = 1
      }
      g.eventCenter:dispatchEvent({
        name = TableMessageList.DISPATCH_EVENT,
        data = data
      })
      local function speakFinshedCallback()
        if isSelf then
        end
        local data = {
          eventName = TableMessageList.BROADCAST_RESULT,
          plus = -1
        }
        g.eventCenter:dispatchEvent({
          name = TableMessageList.DISPATCH_EVENT,
          data = data
        })
      end
      local function dealCardCallback(roundNumber, seatId, singleHandPoker)
        singleHandPoker:hide()
        if seat:isSelfSeat() then
          local function turnOverPokersCompleteCallback()
            seat:showPokersInSeatCardType()
            speakFinshedCallback()
          end
          seat:playIncreasePokerInSeatAnimation(roundNumber)
          seat:playTurnOverNthPokersInSeatAnimation(roundNumber, turnOverPokersCompleteCallback)
        else
          seat:playIncreaseSmallPokerInSeatAnimation(roundNumber, speakFinshedCallback)
        end
      end
      local totalRounds = player:getCardCount()
      if player:getIsSelf() then
        self._pokers:dealCard(3, totalRounds, seatId, virtualSeatId, 0, isSelf, dealCardCallback)
      else
        self._pokers:dealCard(3, totalRounds, seatId, virtualSeatId, 0, isSelf, dealCardCallback)
      end
    end
  end
  if player:getIsSelf() then
    self._controlPanel:setPlayStatus(PlayStatus.SPOKEN)
  end
end
function TableUIManager:showGameResult(judge)
  self._searchLight:stopAllAnimation()
  self._searchLight:fadeOut()
  self._searchLight:clearData()
  local isBankerWinAll = true
  local flipCount = 0
  local totalPlayerCount = 0
  for i, player in ipairs(judge.gamblers) do
    local status = player:getPlayStatus()
    if status == PlayStatus.LOSE or status == PlayStatus.WIN or status == PlayStatus.DRAW then
      flipCount = flipCount + 1
      totalPlayerCount = totalPlayerCount + 1
    end
    if not player:getIsBanker() and (status == PlayStatus.WIN or status == PlayStatus.DRAW) then
      isBankerWinAll = false
    end
  end
  local function chipsMoveToUsersCompleteCallback()
    self:hideWin(true)
    for i, player in ipairs(judge.gamblers) do
      local seatId = player:getSeatId()
      local seat = self:getSeatBySeatID(seatId)
      seat:setSeatChips(player:getSeatChips())
    end
  end
  for i, player in ipairs(judge.gamblers) do
    do
      local status = player:getPlayStatus()
      if status == PlayStatus.LOSE or status == PlayStatus.WIN or status == PlayStatus.DRAW then
        do
          local seatId = player:getSeatId()
          local seat = self:getSeatBySeatID(seatId)
          local virtualSeatId = seat:getVirtualSeatId()
          seat:bindData(player, true)
          local isSelf = player:getIsSelf()
          if isSelf then
            if player:getPlayStatus() == PlayStatus.WIN then
              local playBigWinSound = false
              if player:getIsBanker() and totalPlayerCount > 4 and isBankerWinAll then
                playBigWinSound = true
              end
              if tonumber(player:getCardsInHandModel():getDeng() or 0) > 2 then
                playBigWinSound = true
              end
              self:playSelfWinViewAnimation(playBigWinSound)
            end
            flipCount = flipCount - 1
            if flipCount == 0 then
              self:showCollectChipsSpotsAnimation(judge, chipsMoveToUsersCompleteCallback)
            end
          else
            do
              local function turnOverPokersCompleteCallback()
                seat:showPokersInSeatCardType()
                if player:getPlayStatus() == PlayStatus.WIN then
                  seat:showSeatWinAnimation()
                end
                flipCount = flipCount - 1
                if flipCount == 0 then
                  self:showCollectChipsSpotsAnimation(judge, chipsMoveToUsersCompleteCallback)
                end
              end
              local function smallPokersMovedCompleteCallback()
                seat:hideSmallPokersInSeat()
                seat:showPokersInSeat(player:getCardCount())
                seat:playTurnOverPokersInSeatAnimation(turnOverPokersCompleteCallback)
              end
              if player:getPokState() ~= 0 then
                turnOverPokersCompleteCallback()
              else
                seat:playSmallPokersInSeatToSeatCenterAnimation(smallPokersMovedCompleteCallback)
              end
            end
          end
        end
      end
      if player:getIsSelf() then
        self._controlPanel:setPlayStatus(status)
      end
    end
  end
  self:removeClearTableTimer()
  self:addClearTableTimer()
end
function TableUIManager:showCollectChipsSpotsAnimation(judge, animationCompleteCallback)
  local needToPayMore = false
  local noOneLose = true
  local totalChips = 0
  local winerCount = 0
  local loserCount = 0
  local drawCount = 0
  local function chipsCreateComplete()
    self:_sendChipsSpotsAnimation(judge, totalChips, animationCompleteCallback)
  end
  local function chipsIncreaseCompleteCallback()
    self:_playMovingChipsToChipsSpot(judge, loserCount, winerCount, drawCount, chipsCreateComplete)
  end
  local increaseChipsCount = 0
  for i, player in ipairs(judge.gamblers) do
    totalChips = totalChips + player:getBet()
    if player:getPlayStatus() == PlayStatus.LOSE then
      noOneLose = false
      do
        local bet = player:getBet()
        local deltaChips = player:getDeltaChips()
        totalChips = totalChips + deltaChips
        local payMore = deltaChips - bet
        if payMore > 0 then
          increaseChipsCount = increaseChipsCount + 1
          totalChips = totalChips + payMore
          needToPayMore = true
          do
            local seatId = player:getSeatId()
            local seat = self:getSeatBySeatID(seatId)
            local virtualSeatId = seat:getVirtualSeatId()
            local chipsSpot = self:getChipsSpotBySeatID(seatId)
            local function increaseChipsCompleteCallback(increaseChipsSpot)
              increaseChipsSpot:removeFromParent()
              chipsSpot:bindData(deltaChips)
              chipsSpot:showForVirtualSeatId(virtualSeatId)
              increaseChipsCount = increaseChipsCount - 1
              if increaseChipsCount == 0 then
                chipsIncreaseCompleteCallback()
              end
            end
            local wagerCenterPosition = TableUIConfig.WAGERVIEW[virtualSeatId]
            local seatCenterPosition = TableUIConfig.SEAT[virtualSeatId]
            local increaseChipsSpot = ChipsSpot.new(payMore):addTo(self.nodes.wagerNode)
            increaseChipsSpot:playChipsStacksMoveAnimation(seatCenterPosition, wagerCenterPosition, handler(increaseChipsSpot, increaseChipsCompleteCallback))
          end
        end
        loserCount = loserCount + 1
      end
    elseif player:getPlayStatus() == PlayStatus.WIN then
      winerCount = winerCount + 1
      totalChips = totalChips + player:getBet()
    elseif player:getPlayStatus() == PlayStatus.DRAW then
      drawCount = drawCount + 1
    end
  end
  if not needToPayMore then
    self:_playMovingChipsToChipsSpot(judge, loserCount, winerCount, drawCount, chipsCreateComplete)
  end
  if noOneLose then
    chipsCreateComplete()
  end
end
function TableUIManager:_playMovingChipsToChipsSpot(judge, loserCount, winerCount, drawCount, completeCallback)
  local moveLoserChipsToCenterAction = CCCallFunc:create(function()
    local chipsMovedCount = 0
    for i, player in ipairs(judge.gamblers) do
      if player:getPlayStatus() == PlayStatus.LOSE or player:getPlayStatus() == PlayStatus.WIN then
        do
          local seatId = player:getSeatId()
          local chipsSpot = self:getChipsSpotBySeatID(seatId)
          local function moveCompleteCallback()
            chipsMovedCount = chipsMovedCount + 1
            chipsSpot:clearData()
            if chipsMovedCount == winerCount + loserCount then
              completeCallback()
            end
          end
          local collectChipsPoint = TableUIConfig.COLLECT_CHIP_POINT
          if 0 < chipsSpot:getTotalChips() then
            chipsSpot:movingChipsSpotToCenterPostion(0.1, collectChipsPoint, moveCompleteCallback)
          else
            moveCompleteCallback()
          end
        end
      end
    end
  end)
  local action = self.nodes.wagerNode:runAction(moveLoserChipsToCenterAction)
  g.animationsManager:addActions(self.nodes.wagerNode, action)
end
function TableUIManager:_sendChipsSpotsAnimation(judge, totalChips, sendChipsCompleteCallback)
  local totalChipsSpot = 0
  local chipsStayTime = 0.4
  for i, player in ipairs(judge.gamblers) do
    if player:getPlayStatus() == PlayStatus.WIN then
      totalChipsSpot = totalChipsSpot + 1
      do
        local collectChipsPoint = TableUIConfig.COLLECT_CHIP_POINT
        local chipsSpot = ChipsSpot.new(totalChips, collectChipsPoint):addTo(self.nodes.wagerNode)
        local function moveCompleteCallback()
          totalChipsSpot = totalChipsSpot - 1
          chipsSpot:removeFromParent()
          if totalChipsSpot == 0 and sendChipsCompleteCallback then
            sendChipsCompleteCallback()
          end
        end
        local seatId = player:getSeatId()
        local seat = self:getSeatBySeatID(seatId)
        local virtualSeatId = seat:getVirtualSeatId()
        local seatCenterPosition = TableUIConfig.SEAT[virtualSeatId]
        chipsSpot:movingWinChipsSpotToUser(chipsStayTime, seatCenterPosition, moveCompleteCallback)
      end
    elseif player:getPlayStatus() == PlayStatus.DRAW then
      totalChipsSpot = totalChipsSpot + 1
      do
        local chipsSpot = self:getChipsSpotBySeatID(player:getSeatId())
        local function moveCompleteCallback()
          totalChipsSpot = totalChipsSpot - 1
          chipsSpot:clearData()
          if totalChipsSpot == 0 and sendChipsCompleteCallback then
            sendChipsCompleteCallback()
          end
        end
        local seatId = player:getSeatId()
        local seat = self:getSeatBySeatID(seatId)
        local virtualSeatId = seat:getVirtualSeatId()
        local seatCenterPosition = TableUIConfig.SEAT[virtualSeatId]
        chipsSpot:movingWinChipsSpotToUser(chipsStayTime, seatCenterPosition, moveCompleteCallback)
      end
    end
  end
end
function TableUIManager:showBankerEscapeResult(judge)
  logger:debug("隐藏下注倒计时")
  self._betClock:hideWithAnimation()
  self._searchLight:stopAllAnimation()
  self._searchLight:fadeOut()
  self._searchLight:clearData()
  local playerIn = false
  for i, player in ipairs(judge.gamblers) do
    local status = player:getPlayStatus()
    if status == PlayStatus.LOSE or status == PlayStatus.WIN or status == PlayStatus.DRAW then
      playerIn = true
      local seatId = player:getSeatId()
      local seat = self:getSeatBySeatID(seatId)
      seat:resetSeatClock()
      local isSelf = player:getIsSelf()
      if isSelf then
        if player:getPlayStatus() == PlayStatus.WIN then
          local playBigWinSound = tonumber(player:getCardsInHandModel():getDeng() or 0) > 2
          self:playSelfWinViewAnimation(playBigWinSound)
        end
      else
        seat:showSeatWinAnimation()
      end
    end
    if player:getIsSelf() then
      self._controlPanel:setPlayStatus(status)
    end
  end
  local function chipsMoveToUsersCompleteCallback()
    self:hideWin(true)
    for i, player in ipairs(judge.gamblers) do
      local seatId = player:getSeatId()
      local seat = self:getSeatBySeatID(seatId)
      seat:setSeatChips(player:getSeatChips())
    end
  end
  if playerIn then
    self:showCollectChipsSpotsAnimation(judge, chipsMoveToUsersCompleteCallback)
  end
  self:removeClearTableTimer()
  self:addClearTableTimer()
end
function TableUIManager:showBankrupt(packetModel)
  UserBankruptPopup.new():showPanel(packetModel.bankruptTimes, packetModel.supportChips)
end
function TableUIManager:playEmojiAnimation(seatId, emojiType, emojiIndex)
  local seat = self:getSeatBySeatID(seatId)
  ChatPanel:playEmojiAnimation(seat, emojiType, emojiIndex)
end
function TableUIManager:receiveChatMessage(seatId, userName, sPic, message)
  self.chatMessages = self.chatMessages or {}
  local messageItem = {
    userName = userName,
    sPic = sPic,
    message = message
  }
  table.insert(self.chatMessages, messageItem)
  if self._chatPanel ~= nil then
    self._chatPanel:setMessageList(self.chatMessages)
  end
  if seatId ~= nil then
    self:showChatMsg(seatId, message)
  end
end
function TableUIManager:playMagicEmojiAnimation(magicId, startSeatId, endSeatId)
  local startSeat = self:getSeatBySeatID(startSeatId)
  local endSeat = self:getSeatBySeatID(endSeatId)
  local startX, startY = startSeat:getPosition()
  local startPoint = ccp(startX, startY)
  local endX, endY = endSeat:getPosition()
  local endPoint = ccp(endX, endY)
  if self._magicEmoji == nil then
    self._magicEmoji = MagicEmoji.new(self.nodes.animationPersistNode)
  end
  self._magicEmoji:playMagicAnimation(startPoint, endPoint, magicId)
end
function TableUIManager:playSendChipSuccessAnimation(startSeatId, endSeatId, sendChips, player, startUserSeatChip, endUserSeatChip)
  if player:getIsSelf() then
    scheduler.performWithDelayGlobal(function()
      g.audio:playSound("MOVE_CHIP")
    end, 1)
  end
  self:playSendWagerAnimation(startSeatId, endSeatId, sendChips, player:getUserName(), startUserSeatChip, endUserSeatChip)
end
function TableUIManager:playSendWagerAnimation(startSeatId, endSeatId, sendChips, userName, startUserSeatChip, endUserSeatChip)
  local startSeat = self:getSeatBySeatID(startSeatId)
  local startX, startY = startSeat:getPosition()
  local startPoint = ccp(startX, startY)
  local endPoint, completeCallback
  startSeat:setSeatChips(startUserSeatChip)
  if endSeatId == 255 then
    endPoint = TableUIConfig.DEALER_CENTER
    function completeCallback()
      local message = string.gsub(g.lang:getContent("table.thanksForSendChip"), "{1}", userName)
      self:showChatMsg(endSeatId, message)
    end
  else
    local endSeat = self:getSeatBySeatID(endSeatId)
    local endX, endY = endSeat:getPosition()
    endPoint = ccp(endX, endY)
    endSeat:setSeatChips(endUserSeatChip)
  end
  local chipsSpot = ChipsSpot.new(sendChips, startPoint):addTo(self.nodes.animationPersistNode)
  local function sendChipsCompleteCallback()
    chipsSpot:removeFromParent()
    if completeCallback then
      completeCallback()
    end
  end
  chipsSpot:sendingChipsSpotToUser(0, endPoint, sendChipsCompleteCallback)
end
function TableUIManager:playerApplyGift(seatId, giftId)
  local seat = self:getSeatBySeatID(seatId)
  if seat then
    seat:setGiftId(giftId)
  end
end
function TableUIManager:playerSendGift(fromSeatId, toSeatId, giftId)
  local fromSeat = self:getSeatBySeatID(fromSeatId)
  local toSeat = self:getSeatBySeatID(toSeatId)
  if fromSeat and toSeat then
    local fromX, fromY = fromSeat:getPosition()
    local toX, toY = toSeat:getPosition()
    fromX, fromY = self:_getGiftButtonPosition(fromSeat:getVirtualSeatId(), fromX, fromY)
    toX, toY = self:_getGiftButtonPosition(toSeat:getVirtualSeatId(), toX, toY)
    function completeCallback()
      if not toSeat:isEmpty() then
        toSeat:setGiftId(giftId)
      end
    end
    self:playSendGiftAnimation(ccp(fromX, fromY), ccp(toX, toY), giftId, completeCallback)
  end
end
function TableUIManager:_getGiftButtonPosition(virtualSeatId, x, y)
  if virtualSeatId == 1 or virtualSeatId == 2 or virtualSeatId == 3 or virtualSeatId == 5 then
    return x - 55, y
  else
    return x + 55, y
  end
  return x, y
end
function TableUIManager:playSendGiftAnimation(fromPoint, toPoint, giftId, completeCallback)
  g.giftInfo:loadGiftData(handler(self, function(self)
    local gift = g.giftInfo:getGiftById(giftId)
    if gift then
      do
        local giftImage = g.ui.UIImageView.new()
        giftImage:setImage(gift.pic, {
          size = cc.size(48, 48),
          success = handler(self, function(self)
            self._tableAnimationManager:playSendGiftAnimation(self.nodes.animationPersistNode, fromPoint, toPoint, giftImage, completeCallback)
          end)
        })
      end
    end
  end))
end
function TableUIManager:playAddFriendAnimation(startVirtualSeatId, endVirtualSeatId)
  local fromPoint = ccp(0, display.width)
  if startVirtualSeatId ~= 0 then
    fromPoint = TableUIConfig.SEAT[startVirtualSeatId]
  end
  local toPoint = TableUIConfig.SEAT[endVirtualSeatId]
  local messageSprite = display.newSprite("#table_addFriendMessage_icon.png")
  self._tableAnimationManager:playSendGiftAnimation(self.nodes.animationPersistNode, fromPoint, toPoint, messageSprite, nil)
end
function TableUIManager:showChatMsg(seatId, message)
  if not self._chatBubbles then
    self._chatBubbles = {}
  end
  local bubble
  local positionId = 255
  if seatId ~= 255 then
    local seat = self:getSeatBySeatID(seatId)
    positionId = seat:getVirtualSeatId()
    local p = ccp(0, 0)
    if p then
      local px, py
      if positionId >= 1 and positionId <= 3 then
        bubble = ChatMessageBubble.new(message, ChatMessageBubble.DIRECTION_RIGHT)
        px = p.x + 8
      else
        bubble = ChatMessageBubble.new(message, ChatMessageBubble.DIRECTION_LEFT)
        px = p.x - 8
      end
      if positionId == 1 or positionId == 9 then
        py = p.y - 16
      elseif positionId == 2 or positionId == 8 then
        py = p.y + 4
      elseif positionId == 3 or positionId == 7 then
        py = p.y + 40
      else
        py = p.y + 32
      end
      local seatPosition = TableUIConfig.SEAT[positionId]
      bubble:show(self.nodes.animationPersistNode, seatPosition.x + px, seatPosition.y + py)
    end
  else
    bubble = ChatMessageBubble.new(message, ChatMessageBubble.DIRECTION_LEFT)
    for i, oldBubble in ipairs(self._chatBubbles) do
      if oldBubble:getPositionId() == positionId then
        return
      end
    end
    bubble:show(self.nodes.animationPersistNode, TableUIConfig.DEALER_CENTER.x + 40, TableUIConfig.DEALER_CENTER.y - 25)
  end
  if bubble then
    bubble:setPositionId(positionId)
    for i, oldBubble in ipairs(self._chatBubbles) do
      if oldBubble:getPositionId() == positionId then
        oldBubble:stopAllActions()
        oldBubble:removeFromParent()
        table.removebyvalue(self._chatBubbles, oldBubble, true)
        break
      end
    end
    table.insert(self._chatBubbles, bubble)
    bubble:runAction(transition.sequence({
      CCDelayTime:create(5),
      CCCallFunc:create(function()
        table.removebyvalue(self._chatBubbles, bubble, true)
        bubble:removeFromParent()
      end)
    }))
  end
end
function TableUIManager:addClearTableTimer()
  self._clearTabelTimer = scheduler.performWithDelayGlobal(handler(self, self.onClearTableTimer), 4)
end
function TableUIManager:removeClearTableTimer()
  if self._clearTabelTimer ~= nil then
    scheduler.unscheduleGlobal(self._clearTabelTimer)
  end
  self._clearTabelTimer = nil
end
function TableUIManager:onClearTableTimer()
  self._clearTabelTimer = nil
  self:clearTable()
end
function TableUIManager:clearTable()
  logger:debug("____________________clear table time ___________")
  g.actionsManager:cancelAllActionObject()
  g.animationsManager:cancelAllAnimations()
  self._betClock:hideImediatly()
  self._tableAnimationManager:cancelAllAnimation()
  for i, player in ipairs(g.vars.table.gamblers) do
    player:clearHandData()
    player:setBet(0)
    player:setChooseType(1)
    if player:getIsSelf() then
    end
  end
  for i, seat in ipairs(self.seats) do
    local seatId = seat:getSeatId()
    seat:clearHandData()
    local chipsSpot = self:getChipsSpotBySeatID(seatId)
    chipsSpot:clearData()
  end
  self._chatBubbles = {}
  self:hideWin(false)
  self.nodes.animationNode:removeAllChildren()
  self.arrow = nil
  self._pokers:clearData()
  self._searchLight:clearData()
end
function TableUIManager:onResponseToCleanTableLastSnapshot(judge)
  print(" TableUIManager:onResponseToCleanTableLastSnapshot")
  self:clearTable()
  for i, player in ipairs(judge.gamblers) do
    local seatId = player:getSeatId()
    local seat = self:getSeatBySeatID(seatId)
    seat:standUpAnimation()
    self:clearSeatData(seatId, player:getIsSelf())
    print("clear seat data :", player:getUid())
  end
  self:showEmptySeat()
end
function TableUIManager:onResponseServerUpdate(judge)
  g.ui.manager.tip:top(g.lang:getContent("table.serverUpdate"))
end
function TableUIManager:onResponseServerStop(judge)
  g.ui.manager.tip:top(g.lang:getContent("table.serverStop"))
end
function TableUIManager:_onStandupClick()
  if self._tableUIManager ~= nil and self._tableUIManager.isRotating then
    return
  end
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.STAND_UP_REQUEST
  })
end
function TableUIManager:_onChatBtnClick()
  self._chatPanel = ChatPanel.new(self.chatMessages):showPanel():pos(ChatPanel.PANEL_WIDTH * 0.5 + 5, ChatPanel.PANNEL_HEIGHT * 0.5 + 5)
  function self._chatPanel.exitCallback()
    self._chatPanel = nil
  end
end
function TableUIManager:_onTableRecordClick()
end
function TableUIManager:_onAddFriendsClick()
  local selectedTab = 2
  local friendListView = FriendListView.new(selectedTab)
  friendListView:showPanel()
end
function TableUIManager:onPayButtonClick()
  PaymentView.new():show()
end
function TableUIManager:_onMenuClick()
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.QUIT_TABLE_REQUEST
  })
end
function TableUIManager:_onReturnBackToLobbyClick()
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.QUIT_TABLE_REQUEST
  })
end
function TableUIManager:_onSendChipToDealerClick()
  if g.vars.user.seatId then
    ViceJudge:asyncRequestSendChip("1", tonumber(g.vars.table.loginResponse.minBet) * 0.1)
  else
    g.ui.manager.tip:top(g.lang:getContent("table.sendChipNotInSeat"))
  end
end
function TableUIManager:resetRotateSeatsNoraml(seatsRotateComplete)
  local totalSeats = table.maxn(self.seats)
  local centerSeatId = math.ceil(totalSeats / 2)
  self:seatsRotate(centerSeatId, seatsRotateComplete)
end
function TableUIManager:seatsRotate(seatId, seatsRotateComplete)
  local rotateClockwise = true
  local totalSeats = table.maxn(self.seats)
  local centerSeatId = math.ceil(totalSeats / 2)
  local seat = self:getSeatBySeatID(seatId)
  local targetSeatCurVirtualSeatId = seat:getVirtualSeatId()
  if targetSeatCurVirtualSeatId == centerSeatId then
    if seatsRotateComplete then
      seatsRotateComplete()
    end
    return
  end
  if centerSeatId > targetSeatCurVirtualSeatId then
    rotateClockwise = true
  elseif centerSeatId < targetSeatCurVirtualSeatId then
    rotateClockwise = false
  else
    return
  end
  self._pokers:hidePokers()
  self.nodes.wagerNode:hide()
  local isVisibleBeforeRotate = self._searchLight:isVisible()
  self._searchLight:stopAllAnimation()
  self._searchLight:fadeOut()
  if self._dealerVirtualSeatId ~= -1 then
    self:hideTableDealerIcon()
  end
  local virtualSeatIdOffset = targetSeatCurVirtualSeatId - centerSeatId
  local totalSeats = table.maxn(self.seats)
  self.isRotating = true
  local function rotateComplete()
    self:updatePokersPosition()
    self:updateAllChipsSpotsPosition()
    if self._dealerVirtualSeatId ~= -1 then
      self:showTableDealerIconAtVirtualSeatId(self._dealerVirtualSeatId, false)
    end
    if isVisibleBeforeRotate then
      local restoreSeatId = self._searchLight:getRestoreSeatId()
      if restoreSeatId ~= nil then
        self:playSearchLightOnSeatAnimation(restoreSeatId, false)
      end
    end
    if seatsRotateComplete then
      seatsRotateComplete()
    end
    self.isRotating = false
  end
  local bankerNewVirtualSeatId = -1
  for i, seat in ipairs(self.seats) do
    local currentVirtualSeatID = seat:getVirtualSeatId()
    local finalVirtualSeatId = currentVirtualSeatID - virtualSeatIdOffset
    if finalVirtualSeatId <= 0 then
      finalVirtualSeatId = finalVirtualSeatId + totalSeats
      if self._seatCount == 5 then
        finalVirtualSeatId = finalVirtualSeatId + 1
      end
    elseif totalSeats < finalVirtualSeatId then
      finalVirtualSeatId = finalVirtualSeatId - totalSeats
      if self._seatCount == 5 then
        finalVirtualSeatId = finalVirtualSeatId - 1
      end
    end
    if i == totalSeats then
      self:_seatRotate(seat, currentVirtualSeatID, finalVirtualSeatId, rotateClockwise, rotateComplete)
    else
      self:_seatRotate(seat, currentVirtualSeatID, finalVirtualSeatId, rotateClockwise)
    end
    seat:setVirtualSeatId(finalVirtualSeatId)
    if bankerNewVirtualSeatId == -1 and self._dealerVirtualSeatId == currentVirtualSeatID then
      bankerNewVirtualSeatId = finalVirtualSeatId
    end
  end
  self._dealerVirtualSeatId = bankerNewVirtualSeatId
end
function TableUIManager:_seatRotate(seat, startVirtualSeatId, endVirtualSeatId, rotateClockwise, rotateComplete)
  local keyPositions = {}
  local totalSeats = table.maxn(self.seats)
  local routes = {}
  table.insert(routes, startVirtualSeatId)
  local is5PersonLimitTable = self._seatCount == 5
  if startVirtualSeatId < endVirtualSeatId then
    if rotateClockwise then
      for i = startVirtualSeatId + 1, endVirtualSeatId do
        table.insert(keyPositions, TableUIConfig.SEAT[i])
        table.insert(routes, i)
      end
    else
      for i = endVirtualSeatId, startVirtualSeatId - 1 + totalSeats do
        local index = startVirtualSeatId - 1 + totalSeats - (i - endVirtualSeatId)
        if totalSeats < index then
          table.insert(routes, index - totalSeats)
          table.insert(keyPositions, TableUIConfig.SEAT[index - totalSeats])
        else
          table.insert(routes, index)
          table.insert(keyPositions, TableUIConfig.SEAT[index])
        end
      end
    end
  elseif rotateClockwise then
    for i = startVirtualSeatId + 1, endVirtualSeatId + totalSeats do
      if i > totalSeats then
        table.insert(routes, i - totalSeats)
        table.insert(keyPositions, TableUIConfig.SEAT[i - totalSeats])
      else
        table.insert(routes, i)
        table.insert(keyPositions, TableUIConfig.SEAT[i])
      end
    end
  else
    for i = endVirtualSeatId + 1, startVirtualSeatId do
      table.insert(routes, startVirtualSeatId - (i - endVirtualSeatId))
      table.insert(keyPositions, TableUIConfig.SEAT[startVirtualSeatId - (i - endVirtualSeatId)])
    end
  end
  local delay = 0
  for i, keyPoint in ipairs(keyPositions) do
    do
      local routerInfo = routes[i]
      local moveStepAnimationTime = TableUIManager.SEAT_MOVE_STEP_TIME
      if is5PersonLimitTable and (routerInfo == 1 or routerInfo == 9) and i < #routes then
        local nextRouterInfo = routes[i + 1]
        if nextRouterInfo == 1 or nextRouterInfo == 9 then
          moveStepAnimationTime = moveStepAnimationTime * 2
        end
      end
      transition.execute(seat, cc.MoveTo:create(moveStepAnimationTime, keyPoint), {
        delay = delay,
        onComplete = function()
          if rotateComplete and i == table.maxn(keyPositions) then
            rotateComplete()
          end
        end
      })
      delay = delay + moveStepAnimationTime
    end
  end
end
function TableUIManager:updatePokersPosition()
  local totalSeats = table.maxn(self.seats)
  for seatId = 1, totalSeats do
    local seat = self:getSeatBySeatID(seatId)
    if not seat:isEmpty() then
      local player = seat._player
      if not player:isCardValueVisible() then
        seat:showSmallPokersInSeat(player:getCardCount())
      end
    end
  end
end
function TableUIManager:updateAllChipsSpotsPosition()
  local totalSeats = table.maxn(self.seats)
  for seatId = 1, totalSeats do
    local seat = self:getSeatBySeatID(seatId)
    local virtualSeatId = seat:getVirtualSeatId()
    local chipsSpot = self:getChipsSpotBySeatID(seatId)
    chipsSpot:showForVirtualSeatId(virtualSeatId)
    if chipsSpot.movingChipsSpotToCenterPostionCallback then
      chipsSpot.movingChipsSpotToCenterPostionCallback()
      chipsSpot.movingChipsSpotToCenterPostionCallback = nil
    end
    if chipsSpot.movingWinChipsSpotToUserCallback then
      chipsSpot.movingWinChipsSpotToUserCallback()
      chipsSpot.movingWinChipsSpotToUserCallback = nil
    end
  end
  self.nodes.wagerNode:show()
end
function TableUIManager:showBuyinSettingPopup(seatId)
  local params = {}
  params.minBuyIn = g.vars.table.loginResponse.minBuyIn
  params.maxBuyIn = g.vars.table.loginResponse.maxBuyIn
  params.seatId = seatId
  params.isAutoBuyIn = g.storage:getBool(g.vars.storageKeys.autoBuy)
  if params.minBuyIn > tonumber(g.vars.user.chips or 0) then
    g.ui.Dialog.new({
      contentText = g.lang:getContent("table.buyinNotEnough"),
      confirmButtonText = g.lang:getContent("shop.buyLabel"),
      dialogType = g.ui.DialogType.NORMAL,
      callback = function(sender)
        if sender == g.ui.DialogConst.CONFIRM_BUTTON_CLICK then
          PaymentView.new():show()
        end
      end
    }):show()
  else
    self._buyinSettingPopup = BuyInSettingPopup.new(params)
    self._buyinSettingPopup:showPanel()
  end
end
function TableUIManager:hideBuyinSettingPopup()
  if self._buyinSettingPopup ~= nil then
    self._buyinSettingPopup:removeFromParent()
    self._buyinSettingPopup = nil
  end
end
function TableUIManager:playSearchLightOnSeatAnimation(seatId, animate)
  self._searchLight:stopAllAnimation()
  self._searchLight:fadeIn()
  local seat = self:getSeatBySeatID(seatId)
  local x, y = seat:getPosition()
  local virtualSeatId = seat:getVirtualSeatId()
  local seatPosition = TableUIConfig.SEAT[virtualSeatId]
  self._searchLight:turnTo(seatId, ccp(seatPosition.x, seatPosition.y), animate)
end
function TableUIManager:playSelfWinViewAnimation(playBigWinSound)
  if playBigWinSound then
    g.audio:playSound("BIG_WIN")
  else
    g.audio:playSound("SMALL_WIN")
  end
  self.bigWinView = BigWinView.new():pos(display.cx, display.cy)
  self.bigWinView:setCascadeOpacityEnabled(true)
  self.bigWinView:addTo(self.nodes.animationNode)
end
function TableUIManager:hideWin(animate)
  if self.bigWinView ~= nil and animate then
    self.bigWinView:stopAllActions()
    transition.fadeOut(self.bigWinView, {
      opacity = 0,
      time = 1,
      onComplete = function()
        self.bigWinView:stop()
        self.bigWinView:removeFromParent()
        self.bigWinView = nil
      end
    })
  elseif self.bigWinView ~= nil and not animate then
    self.bigWinView:stop()
    self.bigWinView:stopAllActions()
    self.bigWinView:removeFromParent()
    self.bigWinView = nil
  end
end
function TableUIManager:showTableDealerIconAtVirtualSeatId(virtualSeatId, animate)
  self._pokers:show()
  self._tableDealerIcon:show()
  self._pokers:showPokersAtVirtualSeat(virtualSeatId, animate)
  self.nodes.staticPokersNode:show()
  local staticPokerPostion = TableUIConfig.STATICPOKER[virtualSeatId]
  local dealerPosition = ccp(staticPokerPostion.x + TableUIConfig.DEALER_OFFSET.x, staticPokerPostion.y + TableUIConfig.DEALER_OFFSET.y)
  self._tableDealerIcon:stopAllActions()
  if animate then
    transition.execute(self._tableDealerIcon, cc.MoveTo:create(TableUIConfig.DEALER_MOVE_STEP_TIME, dealerPosition), {easing = "sineInOut"})
  else
    self._tableDealerIcon:pos(dealerPosition.x, dealerPosition.y):show()
  end
end
function TableUIManager:hideTableDealerIcon()
  self._tableDealerIcon:hide()
  self._pokers:hide()
  self.nodes.staticPokersNode:hide()
end
function TableUIManager:dispose()
  self._chatPanel = nil
  if self._magicEmoji then
    self._magicEmoji:dispose()
    self._magicEmoji = nil
  end
  for k, node in pairs(self.nodes) do
    node:removeFromParent()
  end
  self._tableScene = nil
  self:removeClearTableTimer()
end
return TableUIManager

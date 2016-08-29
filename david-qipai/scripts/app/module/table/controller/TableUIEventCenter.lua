local TableUIEvent = require("app.module.table.view.TableUIEvent")
local Player = require("app.module.table.model.Player")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local TableUIEventCenter = class("TableUIEventCenter")
function TableUIEventCenter:ctor(judge, tableUIManager)
  self._judge = judge
  self._tableUIManager = tableUIManager
  self._eventHandlers = {}
  local eventCenter = g.eventCenter
  local eventHandler1 = eventCenter:addEventListener(TableUIEvent.Player.ENTER_SUCCESS, handler(self, self.notificationEnterTableSuccessMessage))
  table.insert(self._eventHandlers, eventHandler1)
  local eventHandler2 = eventCenter:addEventListener(TableUIEvent.Player.QUIT_TABLE_REQUEST, handler(self, self.notificationRequestQiutTable))
  table.insert(self._eventHandlers, eventHandler2)
  local eventHandler3 = eventCenter:addEventListener(TableUIEvent.Player.SIT_DOWN_REQUEST, handler(self, self.notificationRequestSeat))
  table.insert(self._eventHandlers, eventHandler3)
  local eventHandler3_1 = eventCenter:addEventListener(TableUIEvent.Player.SIT_DOWN_FAIL, handler(self, self.notificationRequestSeatFail))
  table.insert(self._eventHandlers, eventHandler3_1)
  local eventHandler4 = eventCenter:addEventListener(TableUIEvent.Player.BUYIN_REQUEST, handler(self, self.notificationRequestBuyIn))
  table.insert(self._eventHandlers, eventHandler4)
  local eventHandler5 = eventCenter:addEventListener(TableUIEvent.Player.STAND_UP_REQUEST, handler(self, self.notificationRequestStandUp))
  table.insert(self._eventHandlers, eventHandler5)
  local eventHandler6 = eventCenter:addEventListener(TableUIEvent.Player.SIT_DOWN_BROADCAST, handler(self, self.notificationSitDownMessage))
  table.insert(self._eventHandlers, eventHandler6)
  local eventHandler7 = eventCenter:addEventListener(TableUIEvent.Player.BETTING_BROADCAST, handler(self, self.notificationToBetMessage))
  table.insert(self._eventHandlers, eventHandler7)
  local eventHandler8 = eventCenter:addEventListener(TableUIEvent.Player.BET_SEND_REQUEST, handler(self, self.notificationRequestSendBet))
  table.insert(self._eventHandlers, eventHandler8)
  local eventHandler9 = eventCenter:addEventListener(TableUIEvent.Player.BETTED_BROADCAST, handler(self, self.notificationBetInfo))
  table.insert(self._eventHandlers, eventHandler9)
  local eventHandler10 = eventCenter:addEventListener(TableUIEvent.Player.GAMESTART_BROADCAST, handler(self, self.notificationGameStart))
  table.insert(self._eventHandlers, eventHandler10)
  local eventHandler11 = eventCenter:addEventListener(TableUIEvent.Player.STAND_UP_BROADCAST, handler(self, self.notificationStandUpMessage))
  table.insert(self._eventHandlers, eventHandler11)
  local eventHandler12 = eventCenter:addEventListener(TableUIEvent.Player.DISPATCH_CARDS_BROADCAST, handler(self, self.notificationDispatchCards))
  table.insert(self._eventHandlers, eventHandler12)
  local eventHandler13 = eventCenter:addEventListener(TableUIEvent.Player.POK_BROADCAST, handler(self, self.notificationPOK))
  table.insert(self._eventHandlers, eventHandler13)
  local eventHandler14 = eventCenter:addEventListener(TableUIEvent.Player.SPEAK_TURN_BROADCAST, handler(self, self.notificationSpeakTurn))
  table.insert(self._eventHandlers, eventHandler14)
  local eventHandler15 = eventCenter:addEventListener(TableUIEvent.Player.SPEAKCONTENT_SEND_REQUEST, handler(self, self.notificationRequestSpeak))
  table.insert(self._eventHandlers, eventHandler15)
  local eventHandler16 = eventCenter:addEventListener(TableUIEvent.Player.SPEAK_CONTENT_BROADCAST, handler(self, self.notificationSpeakContent))
  table.insert(self._eventHandlers, eventHandler16)
  local eventHandler17 = eventCenter:addEventListener(TableUIEvent.Player.RESULT_BROADCAST, handler(self, self.notificationResult))
  table.insert(self._eventHandlers, eventHandler17)
  local eventHandler18 = eventCenter:addEventListener(TableUIEvent.Player.BANKER_ESCAPE_RESULT_BROADCAST, handler(self, self.notificationBankerEscapeResult))
  table.insert(self._eventHandlers, eventHandler18)
  local eventHandler19 = eventCenter:addEventListener(TableUIEvent.Player.BANKRUPT, handler(self, self.notificationUserBankrupt))
  table.insert(self._eventHandlers, eventHandler19)
  local eventHandler20 = eventCenter:addEventListener(TableUIEvent.Player.EMOJI_BROADCAST, handler(self, self.notificationPlayerEmoji))
  table.insert(self._eventHandlers, eventHandler20)
  local eventHandler21 = eventCenter:addEventListener(TableUIEvent.Player.CHAT_BROADCAST, handler(self, self.notificationPlayerChat))
  table.insert(self._eventHandlers, eventHandler21)
  local eventHandler22 = eventCenter:addEventListener(TableUIEvent.Player.MAGIC_EMOJI_BROADCAST, handler(self, self.notificationPlayerMagicEmoji))
  table.insert(self._eventHandlers, eventHandler22)
  local eventHandler23 = eventCenter:addEventListener(TableUIEvent.Player.SENDCHIP_SUCCESS_BROADCAST, handler(self, self.notificationPlayerSendChipSuccess))
  table.insert(self._eventHandlers, eventHandler23)
  local eventHandler24 = eventCenter:addEventListener(TableUIEvent.Player.APLLY_GIFT, handler(self, self.notificationPlayerApplyGift))
  table.insert(self._eventHandlers, eventHandler24)
  local eventHandler25 = eventCenter:addEventListener(TableUIEvent.Player.SEND_GIFT, handler(self, self.notificationPlayerSendGift))
  table.insert(self._eventHandlers, eventHandler25)
  local eventHandler26 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_CHIPS_CHANGE, handler(self, self.notificationUserInfoUpdate))
  table.insert(self._eventHandlers, eventHandler26)
  local eventHandler27 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_PIC_CHANGE, handler(self, self.notificationUserInfoUpdate))
  table.insert(self._eventHandlers, eventHandler27)
  local eventHandler28 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_NAME_CHANGE, handler(self, self.notificationUserInfoUpdate))
  table.insert(self._eventHandlers, eventHandler28)
  local eventHandler30 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_GENDER_CHANGE, handler(self, self.notificationUserInfoUpdate))
  table.insert(self._eventHandlers, eventHandler30)
  local eventHandler31 = g.eventCenter:addEventListener(TableUIEvent.Player.ADD_FRIEND_BROADCAST, handler(self, self.notificationAddFriend))
  table.insert(self._eventHandlers, eventHandler31)
  local eventHandler32 = g.eventCenter:addEventListener(TableUIEvent.Player.RECONNECT_REQUEST, handler(self, self.notificationReconnectRequest))
  table.insert(self._eventHandlers, eventHandler32)
  local eventHandler33 = g.eventCenter:addEventListener(TableUIEvent.Player.CLEAN_TABLE, handler(self, self.notificationCleanTable))
  table.insert(self._eventHandlers, eventHandler33)
  local eventHandler34 = g.eventCenter:addEventListener(TableUIEvent.Player.SERVER_UPDATE, handler(self, self.notificationServerUpdate))
  table.insert(self._eventHandlers, eventHandler34)
  local eventHandler35 = g.eventCenter:addEventListener(TableUIEvent.Player.SERVER_STOP, handler(self, self.notificationServerStop))
  table.insert(self._eventHandlers, eventHandler35)
  local eventHandler36 = eventCenter:addEventListener(TableUIEvent.Player.ImInBounsTime, handler(self, self.refreshIsInBounsTime))
  table.insert(self._eventHandlers, eventHandler36)
end
function TableUIEventCenter:notificationEnterTableSuccessMessage()
  self._tableUIManager:responseToEnterBroadcast(self._judge)
end
function TableUIEventCenter:notificationRequestQiutTable()
  self._judge:asyncRequestQuitTable()
end
function TableUIEventCenter:notificationRequestSeat(event)
  local data = event.data
  local seatId = data.seatId
  self._tableUIManager:showBuyinSettingPopup(seatId)
end
function TableUIEventCenter:notificationRequestSeatFail(event)
  local data = event.data
  local errorCode = data.errorCode
  self._tableUIManager:onResponseToSitDownFail(errorCode)
end
function TableUIEventCenter:notificationRequestBuyIn(event)
  local player = Player.new()
  local data = event.data
  player.uid = g.vars.user.uid
  player:setSeatId(data.seatId)
  player:setBuyInChips(data.buyInChips)
  player:setIsAutoBuyIn(data.isAutoBuyIn)
  player:setIsSelf(true)
  self._judge:asyncRequestSitDown(player)
end
function TableUIEventCenter:notificationRequestStandUp()
  self._judge:asyncRequestStandUp()
end
function TableUIEventCenter:notificationSitDownMessage(event)
  local player = event.data.player
  self._tableUIManager:onResponseToSitDown(player)
  local isSitDown = true
  if player:getIsSelf() then
    self._tableUIManager:onResponseToSelfSitOrStandup(isSitDown, player:getSeatId(), self._judge)
  else
    self._tableUIManager:onResponseToOtherSitOrStandup(isSitDown, player:getSeatId(), self._judge)
  end
end
function TableUIEventCenter:notificationToBetMessage(event)
  local player = event.data.player
  self._tableUIManager:playerBetTurn(player, self._judge:getBetTime())
end
function TableUIEventCenter:notificationRequestSendBet(event)
  local bet = event.data.bet
  self._judge:asyncRequestBet(bet)
end
function TableUIEventCenter:notificationBetInfo(event)
  local player = event.data.player
  self._tableUIManager:playerBetted(player)
end
function TableUIEventCenter:notificationGameStart()
  self._tableUIManager:responseToGameStartBroadcast(self._judge)
end
function TableUIEventCenter:notificationStandUpMessage(event)
  local player = event.data.player
  local seatId = player.seatId
  local seat = self._tableUIManager:getSeatBySeatID(seatId)
  seat:standUpAnimation()
  self._tableUIManager:clearSeatData(seatId, player:getIsSelf())
  local isSitDown = false
  if player:getIsSelf() then
    self._tableUIManager:onResponseToSelfSitOrStandup(isSitDown, nil, self._judge)
  else
    self._tableUIManager:onResponseToOtherSitOrStandup(isSitDown, nil, self._judge)
  end
  if self._judge:getIsImPlayer() then
    seat:hide()
  end
  if player:getIsBanker() then
    self._tableUIManager:showTableDealerIconAtVirtualSeatId(10, true)
  end
end
function TableUIEventCenter:notificationDispatchCards(event)
  logger:debug("TableUIEventCenter:notificationDispatchCards")
  self._tableUIManager:dispatchPokers(self._judge)
end
function TableUIEventCenter:notificationPOK(event)
  logger:debug("TableUIEventCenter:notificationPOK")
  self._tableUIManager:pok(self._judge)
end
function TableUIEventCenter:notificationSpeakTurn(event)
  logger:debug("TableUIEventCenter:notificationSpeakTurn")
  local player = event.data.player
  self._tableUIManager:speakTurn(player, self._judge:getSpeakTime())
end
function TableUIEventCenter:notificationRequestSpeak(event)
  logger:debug("TableUIEventCenter:notificationRequestSpeak")
  local operation = event.data.operation
  self._judge:asyncRequestPostSpeakContent(operation)
end
function TableUIEventCenter:notificationSpeakContent(event)
  logger:debug("TableUIEventCenter:notificationSpeakContent")
  local player = event.data.player
  local operation = event.data.operation
  self._tableUIManager:speakContent(player, operation)
end
function TableUIEventCenter:notificationResult(event)
  self._tableUIManager:showGameResult(self._judge)
end
function TableUIEventCenter:notificationBankerEscapeResult(event)
  self._tableUIManager:showBankerEscapeResult(self._judge)
end
function TableUIEventCenter:notificationUserBankrupt(event)
  local packetModel = event.data
  self._tableUIManager:showBankrupt(packetModel)
end
function TableUIEventCenter:notificationPlayerEmoji(event)
  local data = event.data
  local uid = data.uid
  local seatIdFromServer = data.seatId
  local emojiType = data.emojiType
  local emojiIndex = data.emojiIndex
  local seatId = seatIdFromServer + 1
  self._tableUIManager:playEmojiAnimation(seatId, emojiType, emojiIndex)
end
function TableUIEventCenter:notificationPlayerChat(event)
  local data = event.data
  local userName = data.userName
  local uid = data.uid
  local sPic = data.sPic
  local chatMessage = data.chatMessage
  local player = self._judge:getPlayerByUID(uid)
  local seatId
  if player ~= nil then
    seatId = player:getSeatId()
  end
  self._tableUIManager:receiveChatMessage(seatId, userName, sPic, chatMessage)
end
function TableUIEventCenter:notificationPlayerMagicEmoji(event)
  local data = event.data
  self._tableUIManager:playMagicEmojiAnimation(data.prop, data.seatId, data.targetSeatId)
end
function TableUIEventCenter:notificationPlayerSendChipSuccess(event)
  local data = event.data
  local sendChips = data.sendChips
  local seatIdFromServer = data.seatId
  local targetSeatIdFromServer = data.targetSeatId
  local startUserSeatChip = data.seatChips
  local endUserSeatChip = data.targetSeatChips
  local seatId = seatIdFromServer + 1
  local player = self._judge:getPlayerBySeatId(seatId)
  if data.targetSeatId == 255 then
    self._tableUIManager:playSendChipSuccessAnimation(seatId, targetSeatIdFromServer, sendChips, player, startUserSeatChip, endUserSeatChip)
  else
    local targetSeatId = targetSeatIdFromServer + 1
    self._tableUIManager:playSendChipSuccessAnimation(seatId, targetSeatId, sendChips, player, startUserSeatChip, endUserSeatChip)
  end
end
function TableUIEventCenter:notificationPlayerApplyGift(event)
  local data = event.data
  local uid = data.uid
  local seatId = data.seatId + 1
  local giftId = data.giftId
  if data.seatId ~= -1 then
    local player = self._judge:getPlayerBySeatId(seatId)
    if player:getUid() == uid and player:getGiftId() ~= giftId then
      self._tableUIManager:playerApplyGift(seatId, giftId)
      player:setGiftId(giftId)
    end
  end
end
function TableUIEventCenter:notificationPlayerSendGift(event)
  local data = event.data
  local uid = data.uid
  local seatId = data.seatId + 1
  local giftId = data.giftId
  if data.seatId ~= -1 then
    local playerCount = data.playerCount
    if playerCount > 0 then
      for i = 1, playerCount do
        local player = self._judge:getPlayerBySeatId(data.players[i].seatId + 1)
        if player:getUid() == data.players[i].uid then
          self._tableUIManager:playerSendGift(seatId, data.players[i].seatId + 1, giftId)
          if data.players[i].uid == g.vars.user.uid then
            g.vars.user.giftId = giftId
          end
          player:setGiftId(giftId)
        end
      end
    end
  end
end
function TableUIEventCenter:notificationUserInfoUpdate(message)
  for i, player in ipairs(self._judge.gamblers) do
    if player:getIsSelf() then
      player:setUserName(g.vars.user.name)
      player:setSPic(g.vars.user.sPic)
      player:setUserChips(g.vars.user.chips)
      player:setGender(g.vars.user.gender)
      local seat = self._tableUIManager:getSeatBySeatID(player:getSeatId())
      seat:bindData(player)
      break
    end
  end
end
function TableUIEventCenter:notificationAddFriend(event)
  local data = event.data
  if data ~= nil then
    local uid = data.uid
    local targetUid = data.targetUid
    local player = self._judge:getPlayerByUID(uid)
    local targetPlayer = self._judge:getPlayerByUID(targetUid)
    local startVirtualSeatId = 0
    if player ~= nil then
      local seatId = player:getSeatId()
      local seat = self._tableUIManager:getSeatBySeatID(seatId)
      startVirtualSeatId = seat:getVirtualSeatId()
    end
    local endVirtualSeatId = 0
    if targetPlayer ~= nil then
      local seatId = targetPlayer:getSeatId()
      local seat = self._tableUIManager:getSeatBySeatID(seatId)
      endVirtualSeatId = seat:getVirtualSeatId()
    end
    if endVirtualSeatId ~= 0 then
      self._tableUIManager:playAddFriendAnimation(startVirtualSeatId, endVirtualSeatId)
    end
  end
end
function TableUIEventCenter:notificationReconnectRequest()
  self._judge:asyncRequestReConnectTable()
end
function TableUIEventCenter:notificationCleanTable()
  self._tableUIManager:onResponseToCleanTableLastSnapshot(self._judge)
end
function TableUIEventCenter:notificationServerUpdate()
  self._tableUIManager:onResponseServerUpdate(self._judge)
end
function TableUIEventCenter:notificationServerStop()
  self._tableUIManager:onResponseServerStop(self._judge)
end
function TableUIEventCenter:refreshIsInBounsTime()
  local isImInCurrentGame = self._judge:getIsBounsTimeValid()
  g.vars:setImInCurrentGame(isImInCurrentGame)
  print("update state successsssssss !!!!!!!!!!!")
end
function TableUIEventCenter:dispose()
  for i, handler in ipairs(self._eventHandlers) do
    g.eventCenter:removeEventListener(handler)
    handler = nil
  end
  self._eventHandlers = nil
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.ENTER_SUCCESS)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.QUIT_TABLE_REQUEST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.SIT_DOWN_REQUEST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.BUYIN_REQUEST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.STAND_UP_REQUEST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.SIT_DOWN_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.BETTING_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.BET_SEND_REQUEST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.BETTED_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.GAMESTART_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.STAND_UP_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.DISPATCH_CARDS_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.POK_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.SPEAK_TURN_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.SPEAKCONTENT_SEND_REQUEST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.SPEAK_CONTENT_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.RESULT_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.BANKER_ESCAPE_RESULT_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.EMOJI_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.CHAT_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.MAGIC_EMOJI_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.SENDCHIP_SUCCESS_BROADCAST)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.APLLY_GIFT)
  g.eventCenter:removeEventListenersByEvent(TableUIEvent.Player.SEND_GIFT)
  self._judge = nil
  self._tableUIManager = nil
end
return TableUIEventCenter

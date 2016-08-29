local TableUIEvent = require("app.module.table.view.TableUIEvent")
local PlayStatus = require("app.beans.table.model.PlayStatus")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
local PlayerConstants = import(".PlayerConstants")
local CardsInHandModel = import(".CardsInHandModel")
local Player = import(".Player")
local TableMessageList = require("app.module.table.controller.TableMessageList")
local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local PlayTogetherInfo = require("app.beans.data.PlayTogetherInfo")
local TestDataModule = require("app.test.TestDataModule")
local Judge = class("Judge")
function Judge:ctor()
  if DEBUG_MEMORY_STATUS then
    logger:debug("Object-Life:" .. self.__cname .. ": ctor")
    g.memoryManager:registerObject(self, "Judge")
  end
  self.isImPlayer = false
  self._handId = -1
  self._tid = -1
  self._tableType = -1
  self._tableLevel = -1
  self._tableName = ""
  self._minBet = 0
  self._timeout = -1
  self._seatCount = -1
  self._bankerSeatId = -1
  self.gamblers = {}
  g.vars.table.gamblers = self.gamblers
  self._playerCount = -1
  self._playingCount = -1
  self._direction = 0
  self._tableMessageList = TableMessageList.new()
  self._judgeHandlers = {}
  local eventHandler2 = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_LOGOUT_SUCCESS, handler(self, self.unicastQuitTableSuccessInfo))
  table.insert(self._judgeHandlers, eventHandler2)
  local eventHandler3 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_SIT, handler(self, self.broadcastSitDownSuccessMessage))
  table.insert(self._judgeHandlers, eventHandler3)
  local eventHandler4 = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_SIT_FAIL, handler(self, self.unicastSitDownFailMessage))
  table.insert(self._judgeHandlers, eventHandler4)
  local eventHandler5 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_GAME_START, handler(self, self.broadcastGameStart))
  table.insert(self._judgeHandlers, eventHandler5)
  local eventHandler6 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_BET_TURN, handler(self, self.broadcastBetOrder))
  table.insert(self._judgeHandlers, eventHandler6)
  local eventHandler7 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_BET, handler(self, self.broadcastBetInfo))
  table.insert(self._judgeHandlers, eventHandler7)
  local eventHandler8 = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_BET_FAIL, handler(self, self.unicastBetFail))
  table.insert(self._judgeHandlers, eventHandler8)
  local eventHandler9 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_DEAL, handler(self, self.broadcastDispatchCards))
  table.insert(self._judgeHandlers, eventHandler9)
  local eventHandler10 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_POK, handler(self, self.broadcastPok))
  table.insert(self._judgeHandlers, eventHandler10)
  local eventHandler11 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_SPEAK_TURN, handler(self, self.broadcastSpeakTurn))
  table.insert(self._judgeHandlers, eventHandler11)
  local eventHandler12 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_SPEAK, handler(self, self.broadcastSpeakContent))
  table.insert(self._judgeHandlers, eventHandler12)
  local eventHandler13 = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_SPEAK_FAIL, handler(self, self.unicastSpeakFail))
  table.insert(self._judgeHandlers, eventHandler13)
  local eventHandler14 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_GAME_OVER, handler(self, self.broadcastResult))
  table.insert(self._judgeHandlers, eventHandler14)
  local eventHandler15 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_BANKER_ESCAPE, handler(self, self.broadcastBankerEscape))
  table.insert(self._judgeHandlers, eventHandler15)
  local eventHandler16 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_STAND, handler(self, self.broadcastStandUp))
  table.insert(self._judgeHandlers, eventHandler16)
  local eventHandler17 = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_STAND_FAIL, handler(self, self.unicastStandUpFail))
  table.insert(self._judgeHandlers, eventHandler17)
  local eventHandler18 = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_USER_BANKRUPT, handler(self, self.singlecastUserBankRupt))
  table.insert(self._judgeHandlers, eventHandler18)
  local eventHandler19 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_SERVER_UPDATE, handler(self, self.unicastServerUpdate))
  table.insert(self._judgeHandlers, eventHandler19)
  local eventHandler20 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_SERVER_STOP, handler(self, self.unicastServerStop))
  table.insert(self._judgeHandlers, eventHandler20)
  local eventHandler21 = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_LOGOUT_SUCCESS, handler(self, self.unicastQuitTableSuccessInfo))
  table.insert(self._judgeHandlers, eventHandler21)
end
function Judge:getSeatCount()
  return self._seatCount
end
function Judge:getPlayerByUID(UID)
  for i, player in ipairs(self.gamblers) do
    if player.uid == UID then
      return player
    end
  end
end
function Judge:getPlayerBySeatId(seatId)
  for i, player in ipairs(self.gamblers) do
    if player:getSeatId() == seatId then
      return player
    end
  end
end
function Judge:getBetTime()
  return self._betTime
end
function Judge:getSpeakTime()
  return self._speakTime
end
function Judge:asyncRequestEnterTable()
end
function Judge:unicastReconnectTableSuccessInfo(evt)
  g.eventCenter:removeEventListener(self._reloginSuccessHandle)
  g.eventCenter:removeEventListener(self._reloginFailHandle)
  self._reloginSuccessHandle = nil
  self._reloginFailHandle = nil
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.CLEAN_TABLE
  })
  self:unicastEnterTableSuccessInfo(evt.data)
end
function Judge:unicastEnterTableSuccessInfo(data)
  hideSceneLoading()
  g.audio:playSound("SCENE_SWITCH")
  logger:debug("Judge-TestServer---unicastEnterTableSuccessInfo2")
  local packetModel
  if DEBUG_FAKE_JSON then
    local fakeData = TestDataModule:readFile("S_2_unicastEnterTableSuccessInfo")
    packetModel = json.decode(fakeData)
  else
    packetModel = data
  end
  logger:debug(g.util.inspect(packetModel))
  self._tid = packetModel.tid
  self._tableType = packetModel.tableType
  self._tableLevel = packetModel.tableLevel
  self._tableName = packetModel.tableName
  self._betTime = packetModel.betTime
  self._speakTime = packetModel.speakTime
  self._seatCount = packetModel.seatCount
  local bankerSeatIdInServer = packetModel.bankerSeatId
  self:setBankerSeatIdFromServer(bankerSeatIdInServer)
  self._minBet = packetModel.minBet
  self._playerCount = packetModel.playerCount
  g.vars.user.chips = packetModel.userChips
  self.isImPlayer = false
  self.gamblers = {}
  g.vars.table.gamblers = self.gamblers
  if self._playerCount > 0 then
    for i = 1, self._playerCount do
      local player
      player = self:getPlayerByUID(packetModel.players[i].uid)
      if player == nil then
        player = Player.new()
      end
      player:setUid(packetModel.players[i].uid)
      if g.vars.user.uid == player:getUid() then
        player:setIsSelf(true)
        self.isImPlayer = true
        local seatIdFromServer = packetModel.players[i].seatId
        g.vars.user.seatId = seatIdFromServer + 1
      else
        player:setIsSelf(false)
      end
      player:setUserName(packetModel.players[i].userName)
      player:setSeatChips(packetModel.players[i].seatChips)
      player:setGender(packetModel.players[i].gender)
      player:setUserChips(packetModel.players[i].userChips)
      player:setSPic(packetModel.players[i].sPic)
      player:setGiftId(packetModel.players[i].giftId)
      player:setSeatIdFromServer(packetModel.players[i].seatId)
      player:setPlayStatus(packetModel.players[i].playStatus)
      if player:getSeatId() == self:getBankerSeatId() then
        player:setIsBanker(true)
      else
        player:setIsBanker(false)
      end
      if player:getPlayStatus() ~= PlayStatus.NON_PLAY then
        player:setBet(packetModel.players[i].bet)
        player:setIsFlipedCard(packetModel.players[i].isFlipedCard)
        if player:getIsFlipedCard() == 1 then
          local cardsInHandModel = CardsInHandModel.new()
          cardsInHandModel:setCards(packetModel.players[i].cards)
          cardsInHandModel:setCardType(packetModel.players[i].cardType)
          cardsInHandModel:setTaem(packetModel.players[i].taem)
          cardsInHandModel:setDeng(packetModel.players[i].deng)
          player:setCardsInHandModel(cardsInHandModel)
        elseif player:getIsSelf() and 0 < tonumber(packetModel.cardCount) then
          local cardsInHandModel = CardsInHandModel.new()
          cardsInHandModel:setCards(packetModel.cards)
          cardsInHandModel:setCardType(packetModel.cardType)
          cardsInHandModel:setTaem(packetModel.taem)
          cardsInHandModel:setDeng(packetModel.deng)
          player:setCardsInHandModel(cardsInHandModel)
          player:setChooseType(packetModel.chooseType)
        else
          for i = 1, packetModel.players[i].cardCount do
            player:addCardInHandWithoutValue()
          end
        end
      end
      table.insert(self.gamblers, player)
    end
  end
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.ENTER_SUCCESS
  })
end
function Judge:unicastEnterTableFailInfo(evt)
  logger:debug("Judge-TestServer:unicastEnterTableFailInfo")
  g.eventCenter:removeEventListener(self._reloginSuccessHandle)
  g.eventCenter:removeEventListener(self._reloginFailHandle)
  self._reloginSuccessHandle = nil
  self._reloginFailHandle = nil
  local packetModel = evt.data
  if packetModel.errorCode == 1 then
    g.ui.manager.tip:top(g.lang:getContent("table.loginTableFail"))
  elseif packetModel.errorCode == 2 then
    g.ui.manager.tip:top(g.lang:getContent("table.loginTableFailSearchNotFound"))
  elseif packetModel.errorCode == 19 then
    g.eventCenter:dispatchEvent({
      name = LobbyBeanEvent.EVT_USERINFO_EXPIRE
    })
  elseif packetModel.errorCode == 22 or packetModel.errorCode == 23 then
    g.ui.manager.tip:top(g.lang:getContent("common.serverStop"))
  else
    g.ui.manager.tip:top(g.lang:getContent("table.loginTableFail"))
  end
end
function Judge:asyncRequestQuitTable()
  logger:debug("Judge-TestServer:asyncRequestQuitTable")
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_LOGOUT
  })
  g.eventCenter:dispatchEvent({
    name = LobbyBeanEvent.EVT_QUIT_TO_LOBBY
  })
end
function Judge:unicastQuitTableSuccessInfo(evt)
  logger:debug("Judge-TestServer:unicastQuitTableSuccessInfo")
  local packetModel = evt.data
  if tonumber(packetModel.reason) == 0 then
    return
  elseif tonumber(packetModel.reason) == 1 then
    local delayCallback = function()
      g.ui.manager.tip:top(g.lang:getContent("table.logoutKicked"))
    end
    scheduler.performWithDelayGlobal(delayCallback, 1.5)
  elseif tonumber(packetModel.reason) == 2 then
    local delayCallback = function()
      g.ui.manager.tip:top(g.lang:getContent("table.logoutTimeOut"))
    end
    scheduler.performWithDelayGlobal(delayCallback, 1.5)
  elseif tonumber(packetModel.reason) == 3 then
    local delayCallback = function()
      g.ui.manager.tip:top(g.lang:getContent("table.logoutSystemError"))
    end
    scheduler.performWithDelayGlobal(delayCallback, 1.5)
  end
  g.eventCenter:dispatchEvent({
    name = LobbyBeanEvent.EVT_QUIT_TO_LOBBY
  })
end
function Judge:asyncRequestSitDown(player)
  logger:debug("Judge-TestServer:asyncRequestSitDown player:" .. player.uid)
  local data = {
    seatId = player:getSeatIdForServer(),
    buyInChips = player:getBuyInChips()
  }
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_SIT,
    data = data
  })
  if player:getIsAutoBuyIn() then
    g.eventCenter:dispatchEvent({
      name = TableBeanEvent.EVT_CLI_REQUEST_AUTO_BUYIN,
      data = data
    })
  end
end
function Judge:broadcastSitDownSuccessMessage(evt)
  logger:debug("Judge-TestServer:broadcastSitDownSuccessMessage")
  local packetModel = evt.data
  logger:debug(g.util.inspect(packetModel))
  if packetModel ~= nil then
    local player
    player = self:getPlayerByUID(packetModel.uid)
    if player == nil then
      player = Player.new()
    end
    player:setUid(packetModel.uid)
    player:setGender(packetModel.gender)
    player:setSeatIdFromServer(packetModel.seatId)
    player:setUserName(packetModel.userName)
    player:setSPic(packetModel.sPic)
    player:setGiftId(packetModel.giftId)
    player:setBuyInChips(packetModel.buyInChips)
    player:setUserChips(packetModel.userChips)
    player:setPlayStatus(PlayStatus.NON_PLAY)
    player:setSeatChips(packetModel.buyInChips)
    if player:getUid() == g.vars.user.uid then
      player:setIsSelf(true)
      player:getIsSelf()
      self.isImPlayer = true
      g.vars.user.seatId = player:getSeatId()
      g.vars.user.chips = packetModel.userChips
    end
    player:setPlayerRole(PlayerConstants.PlayerRole.GAMBLER)
    player:setPlayerState(PlayerConstants.GamblerState.UNSTART)
    table.insert(self.gamblers, player)
    g.eventCenter:dispatchEvent({
      name = TableUIEvent.Player.SIT_DOWN_BROADCAST,
      data = {player = player}
    })
  end
end
function Judge:unicastSitDownFailMessage(evt)
  logger:debug("Judge-TestServer:unicastSitDownFailMessage")
  local packetModel = evt.data
  local errorCode = packetModel.errorCode
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.SIT_DOWN_FAIL,
    data = {errorCode = errorCode}
  })
end
function Judge:broadcastGameStart(evt)
  self._tableMessageList:clearData()
  local packetModel = evt.data
  logger:debug("Judge-TestServer:broadcastGameStart")
  logger:debug(packetModel)
  local bankerSeatIdInServer = packetModel.bankerSeatId
  self._handId = packetModel.handId
  self:setBankerSeatIdFromServer(bankerSeatIdInServer)
  local playingCount = packetModel.playerCount
  for i = 1, playingCount do
    local playerSeatId = packetModel.players[i].seatId
    local playerUID = packetModel.players[i].uid
    local player = self:getPlayerByUID(playerUID)
    if player == nil then
      player = Player.new()
      table.insert(self.gamblers, player)
    end
    if bankerSeatIdInServer == playerSeatId then
      player:setIsBanker(true)
    else
      player:setIsBanker(false)
    end
    player:clearHandData()
    player:setPlayStatus(PlayStatus.WAIT_FOR_BET)
    player:setSeatChips(packetModel.players[i].seatChips)
    if player:getIsSelf() then
      g.vars.user.chips = packetModel.players[i].userChips
      g.tasksInfo:reportTaskForPlayCard()
      g.dataLogger:logPlayCount(self._tid, self._tableType, player:getSeatChips(), player:getUserChips())
      if g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_APPLY_IN_COUNT then
        g.playTogetherInfo:playTogetherReport()
        print("&&&&&&&&&&&&&&&&&&playtogether :count play +1 ")
      end
    end
  end
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.GAMESTART_BROADCAST
  })
end
function Judge:broadcastBetOrder(evt)
  logger:debug("Judge-TestServer:broadcastBetOrder")
  local packetModel = evt.data
  local uid = packetModel.uid
  local seatId = packetModel.seatId
  local player = self:getPlayerByUID(uid)
  player:setPlayStatus(PlayStatus.BETTING)
  player:setMinBet(packetModel.minBet)
  player:setMaxBet(packetModel.maxBet)
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.BETTING_BROADCAST,
    data = {player = player}
  })
end
function Judge:asyncRequestBet(bet)
  logger:debug("Judge-TestServer:asyncRequestBet")
  local data = {bet = bet}
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_BET,
    data = data
  })
end
function Judge:broadcastBetInfo(evt)
  logger:debug("Judge-TestServer:broadcastBetInfo")
  local packetModel = evt.data
  local playerUID = packetModel.uid
  local serverSeatId = packetModel.seatId
  local bet = packetModel.bet
  local player = self:getPlayerByUID(playerUID)
  if player then
    player:setBet(bet)
    player:setSeatChips(player:getSeatChips() - bet)
    player:setPlayStatus(PlayStatus.BETTED)
    g.eventCenter:dispatchEvent({
      name = TableUIEvent.Player.BETTED_BROADCAST,
      data = {player = player}
    })
  end
end
function Judge:unicastBetFail()
  logger:debug("Judge-TestServer:unicastBetFail")
end
function Judge:broadcastDispatchCards(evt)
  logger:debug("Judge-TestServer:broadcastDispatchCards")
  local packetModel
  if DEBUG_FAKE_JSON then
    local fakeData = TestDataModule:readFile("S_2_broadcastDispatchCards")
    packetModel = json.decode(fakeData)
  else
    packetModel = evt.data
  end
  self._direction = packetModel.direction
  self._playingCount = packetModel.playerCount
  local playingCount = packetModel.playerCount
  for i = 1, playingCount do
    local playerSeatId = packetModel.players[i].seatId
    local playerUID = packetModel.players[i].uid
    local player = self:getPlayerByUID(playerUID)
    if not player:getIsBanker() then
      player:setPlayStatus(PlayStatus.BETTED)
    end
    player:addCardInHandWithoutValue()
    player:addCardInHandWithoutValue()
  end
  local isImPlayer = packetModel.isImPlayer
  if isImPlayer ~= 0 then
    local ownUID = g.vars.user.uid
    local player = self:getPlayerByUID(ownUID)
    if player ~= nil then
      local cardsInHandModel = CardsInHandModel.new()
      local cards = {
        packetModel.card0,
        packetModel.card1
      }
      cardsInHandModel:setCards(cards)
      cardsInHandModel:setCardType(packetModel.cardType)
      cardsInHandModel:setTaem(packetModel.taem)
      cardsInHandModel:setDeng(packetModel.deng)
      player:setCardsInHandModel(cardsInHandModel)
    end
  end
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.DISPATCH_CARDS_BROADCAST
  })
end
function Judge:broadcastPok(evt)
  logger:debug("Judge-TestServer:broadcastPok")
  local data = {
    eventName = TableMessageList.BROADCAST_RESULT,
    plus = 1
  }
  g.eventCenter:dispatchEvent({
    name = TableMessageList.DISPATCH_EVENT,
    data = data
  })
  local event = self._tableMessageList:getEvent(TableMessageList.BROADCAST_POK)
  if event == nil or event.eventCount == 0 then
    self:dispatchBroadcastPok(evt)
  else
    event.eventExcuteObj = self
    event.eventExcuteMethod = self.dispatchBroadcastPok
    event.eventExcuteArguments = evt
  end
end
function Judge:dispatchBroadcastPok(evt)
  local packetModel = evt.data
  local pokCount = packetModel.pokCount
  logger:debug(g.util.inspect(packetModel))
  for i = 1, pokCount do
    local playerSeatId = packetModel.players[i].seatId
    local playerUID = packetModel.players[i].uid
    local player = self:getPlayerByUID(playerUID)
    if player then
      player:setSeatIdFromServer(playerSeatId)
      player:setUid(playerUID)
      player:setPlayStatus(PlayStatus.POK)
      if player ~= nil then
        local cardsInHandModel = CardsInHandModel.new()
        local cards = {
          packetModel.players[i].card0,
          packetModel.players[i].card1
        }
        cardsInHandModel:setCards(cards)
        cardsInHandModel:setCardType(packetModel.players[i].cardType)
        cardsInHandModel:setTaem(packetModel.players[i].taem)
        cardsInHandModel:setDeng(packetModel.players[i].deng)
        player:setCardsInHandModel(cardsInHandModel)
        player:setPokState(packetModel.players[i].taem)
      end
      if player:getIsSelf() then
        g.tasksInfo:reportTaskForPok()
      end
    end
  end
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.POK_BROADCAST
  })
  local data = {
    eventName = TableMessageList.BROADCAST_RESULT,
    plus = -1
  }
  g.eventCenter:dispatchEvent({
    name = TableMessageList.DISPATCH_EVENT,
    data = data
  })
end
function Judge:broadcastSpeakTurn(evt)
  logger:debug("Judge-TestServer:broadcastSpeakTurn")
  local event = self._tableMessageList:getEvent(TableMessageList.BROADCAST_SPEAK_TURN)
  if event == nil or event.eventCount == 0 then
    self:dispatchBroadcastSpeakTurn(evt)
  else
    event.eventExcuteObj = self
    event.eventExcuteMethod = self.dispatchBroadcastSpeakTurn
    event.eventExcuteArguments = evt
  end
end
function Judge:dispatchBroadcastSpeakTurn(evt)
  logger:debug("Judge-TestServer:dispatchBroadcastSpeakTurn")
  local packetModel = evt.data
  local seatId = packetModel.seatId
  local uid = packetModel.uid
  local player = self:getPlayerByUID(uid)
  if player == nil then
    return
  end
  player:setChooseType(packetModel.chooseType)
  if player ~= nil then
    g.eventCenter:dispatchEvent({
      name = TableUIEvent.Player.SPEAK_TURN_BROADCAST,
      data = {player = player}
    })
  end
end
function Judge:asyncRequestPostSpeakContent(operation)
  logger:debug("Judge-TestServer:asyncRequestPostSpeakContent")
  local data = {operation = operation}
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_SPEAK,
    data = data
  })
end
function Judge:broadcastSpeakContent(evt)
  logger:debug("Judge-TestServer:broadcastSpeakContent")
  local event = self._tableMessageList:getEvent(TableMessageList.BROADCAST_SPEAK)
  if event == nil or event.eventCount == 0 then
    self:dispatchBroadcastSpeakContent(evt)
  else
    event.eventExcuteObj = self
    event.eventExcuteMethod = self.dispatchBroadcastSpeakContent
    event.eventExcuteArguments = evt
  end
end
function Judge:dispatchBroadcastSpeakContent(evt)
  local packetModel
  logger:debug("Judge-TestServer:dispatchBroadcastSpeakContent")
  if DEBUG_FAKE_JSON then
    local fakeData = TestDataModule:readFile("S_2_broadcastSpeakContent1")
    packetModel = json.decode(fakeData)
  else
    packetModel = evt.data
  end
  logger:debug(g.util.inspect(packetModel))
  local seatId = packetModel.seatId
  local uid = packetModel.uid
  local operation = packetModel.operation
  local card = packetModel.card
  local cardType = packetModel.cardType
  local taem = packetModel.taem
  local deng = packetModel.deng
  local player = self:getPlayerByUID(uid)
  if player ~= nil then
    if card ~= nil and card > 0 then
      player:addCardInHandWithValue(card, cardType, taem, deng)
    else
      player:addCardInHandWithoutValue()
    end
    g.eventCenter:dispatchEvent({
      name = TableUIEvent.Player.SPEAK_CONTENT_BROADCAST,
      data = {player = player, operation = operation}
    })
  end
end
function Judge:unicastSpeakFail(evt)
  logger:debug("Judge-TestServer:unicastSpeakFail")
end
function Judge:broadcastResult(evt)
  logger:debug("Judge-TestServer:broadcastResult----------")
  local broadCastEvent = self._tableMessageList:getEvent(TableMessageList.BROADCAST_RESULT)
  if broadCastEvent == nil or broadCastEvent.eventCount == 0 then
    self:disPatchBroadcastResult(evt)
  else
    broadCastEvent.eventExcuteObj = self
    broadCastEvent.eventExcuteMethod = self.disPatchBroadcastResult
    broadCastEvent.eventExcuteArguments = evt
  end
end
function Judge:disPatchBroadcastResult(evt)
  logger:debug("Judge-TestServer:disPatchBroadcastResult---------")
  local packetModel
  if DEBUG_FAKE_JSON then
    local fakeData = TestDataModule:readFile("S_2_broadcastResult")
    packetModel = json.decode(fakeData)
  else
    packetModel = evt.data
  end
  logger:debug(g.util.inspect(packetModel))
  local playerCount = packetModel.playerCount
  for i = 1, playerCount do
    local playerSeatIdInServer = packetModel.players[i].seatId
    local playerUID = packetModel.players[i].uid
    local player = self:getPlayerByUID(playerUID)
    if player ~= nil then
      local cardCount = packetModel.players[i].cardCount
      local cards = {}
      for i, v in ipairs(packetModel.players[i].cards) do
        table.insert(cards, v)
      end
      local cardsInHandModel = CardsInHandModel.new()
      cardsInHandModel:setCards(cards)
      cardsInHandModel:setCardType(packetModel.players[i].cardType)
      cardsInHandModel:setTaem(packetModel.players[i].taem)
      cardsInHandModel:setDeng(packetModel.players[i].deng)
      player:setCardsInHandModel(cardsInHandModel)
      local result = packetModel.players[i].result
      if result == 0 then
        player:setPlayStatus(PlayStatus.DRAW)
      elseif result == 1 then
        if self:getBankerSeatIdForServer() == playerSeatIdInServer then
          player:setPlayStatus(PlayStatus.LOSE)
        else
          player:setPlayStatus(PlayStatus.WIN)
        end
      elseif self:getBankerSeatIdForServer() == playerSeatIdInServer then
        player:setPlayStatus(PlayStatus.WIN)
      else
        player:setPlayStatus(PlayStatus.LOSE)
      end
      player:setDeltaChips(packetModel.players[i].deltaChips)
      player:setSeatChips(packetModel.players[i].seatChips)
      player:setUserChips(packetModel.players[i].userChips)
      if player:getIsSelf() then
        player:setFeeChips(packetModel.feeChips)
        g.vars.user.chips = packetModel.players[i].userChips
        if player:getPlayStatus() == PlayStatus.WIN then
          g.tasksInfo:reportTaskForWin()
        end
      end
    end
  end
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.RESULT_BROADCAST
  })
end
function Judge:broadcastBankerEscape(evt)
  logger:debug("Judge-TestServer:broadcastBankerEscape----------")
  local broadCastEvent = self._tableMessageList:getEvent(TableMessageList.BROADCAST_RESULT)
  if broadCastEvent == nil or broadCastEvent.eventCount == 0 then
    self:dispatchBroadcastBankerEscape(evt)
  else
    broadCastEvent.eventExcuteObj = self
    broadCastEvent.eventExcuteMethod = self.dispatchBroadcastBankerEscape
    broadCastEvent.eventExcuteArguments = evt
  end
end
function Judge:dispatchBroadcastBankerEscape(evt)
  logger:debug("Judge-TestServer:dispatchBroadcastBankerEscape---------")
  local packetModel
  if DEBUG_FAKE_JSON then
    local fakeData = TestDataModule:readFile("broadcastBankerEscape")
    packetModel = json.decode(fakeData)
  else
    packetModel = evt.data
  end
  logger:debug(g.util.inspect(packetModel))
  local playerCount = packetModel.playerCount
  for i = 1, playerCount do
    local playerSeatIdInServer = packetModel.players[i].seatId
    local playerUID = packetModel.players[i].uid
    local player = self:getPlayerByUID(playerUID)
    if player ~= nil then
      player:setPlayStatus(PlayStatus.WIN)
      player:setDeltaChips(packetModel.players[i].deltaChips)
      player:setSeatChips(packetModel.players[i].seatChips)
      player:setUserChips(packetModel.players[i].userChips)
      if player:getIsSelf() then
        player:setFeeChips(packetModel.feeChips)
      end
    end
  end
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.BANKER_ESCAPE_RESULT_BROADCAST
  })
end
function Judge:asyncRequestBankerCampaign(player)
  logger:debug("Judge:向服务器端请求 用户" .. player.uid .. "成为庄家")
  local function successCallback()
    player:setPlayerRole(PlayerConstants.PlayerRole.BANKER)
    self:broadcastBankerInfo(player)
  end
  scheduler.performWithDelayGlobal(successCallback, 1)
end
function Judge:broadcastBankerInfo(banker)
  logger:debug("Judge:收到服务器端通知用户ID " .. banker.uid .. " 的用户成为庄家")
  for i, player in ipairs(self.gamblers) do
    player:setPlayerState(PlayerConstants.GamblerState.UNBET)
    if player.uid == banker.uid then
      player:setPlayerRole(PlayerConstants.PlayerRole.BANKER)
    end
  end
end
function Judge:broadcastBankerDispatchCards()
  logger:debug("Judge: 收到服务器端广播 说轮到庄家发牌")
end
function Judge:asyncRequestDispathCards()
  logger:debug("Judge :庄家向服务器请求发牌")
end
function Judge:unicastDispatchCardFail()
  logger:debug("Judge :服务器广播发牌失败")
end
function Judge:broadcastBankerToSpeak()
  logger:debug("Judge :服务器广播轮到庄家说话")
end
function Judge:asyncRequestPostBankerVSComand(gambler)
  logger:debug("Judge :庄家向服务器请求和 玩家" .. gambler.uid .. " 比牌")
end
function Judge:unicastVSRequestFail()
  logger:debug("Judge :服务器广播请求比牌请求失败")
end
function Judge:broadcastGameFinish()
  logger:debug("Judge :服务器广播本局结束")
end
function Judge:asyncRequestStandUp()
  logger:debug("Judge-TestServer:asyncRequestStandUp")
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_STAND
  })
end
function Judge:broadcastStandUp(evt)
  logger:debug("Judge-TestServer:broadcastStandUp")
  local packetModel = evt.data
  local player = self:getPlayerByUID(packetModel.uid)
  if player == nil then
    return
  end
  if player:getIsSelf() then
    self.isImPlayer = false
    g.vars.user.chips = packetModel.userChips
    g.eventCenter:dispatchEvent({
      name = DataBeanEvent.EVT_USER_CHIPS_CHANGE
    })
    g.vars.user.seatId = nil
  end
  for i, curPlayer in ipairs(self.gamblers) do
    if curPlayer == player then
      if player:getIsBanker() then
        self:setBankerSeatId(-1)
      end
      table.remove(self.gamblers, i)
      break
    end
  end
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.STAND_UP_BROADCAST,
    data = {player = player}
  })
end
function Judge:unicastStandUpFail()
  logger:debug("Judge-TestServer:unicastStandUpFail")
end
function Judge:singlecastUserBankRupt(evt)
  local packetModel = evt.data
  logger:debug(g.util.inspect(packetModel))
  g.vars.user.chips = packetModel.userChips
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.BANKRUPT,
    data = packetModel
  })
end
function Judge:unicastServerUpdate(evt)
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.SERVER_UPDATE,
    data = evt.data
  })
end
function Judge:unicastServerStop(evt)
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.SERVER_STOP,
    data = evt.data
  })
end
function Judge:asyncRequestReConnectTable()
  local data = {
    uid = g.vars.user.uid,
    loginKey = g.vars.user.loginKey,
    ip = g.vars.table.loginRequest.ip,
    port = g.vars.table.loginRequest.port,
    tid = self._tid,
    isCreate = 0,
    sPic = g.vars.user.sPic,
    giftId = g.vars.user.giftId
  }
  self._reloginSuccessHandle = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_LOGIN_SUCCESS, handler(self, self.unicastReconnectTableSuccessInfo))
  self._reloginFailHandle = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_LOGIN_FAIL, handler(self, self.unicastEnterTableFailInfo))
  g.vars.table.loginRequest = data
  logger:debug(g.util.inspect(data))
  showSceneLoading(g.lang:getContent("lobby.tableLoading"))
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_LOGIN,
    data = data
  })
end
function Judge:setBankerSeatIdFromServer(seatId)
  self._bankerSeatId = seatId + 1
end
function Judge:getBankerSeatIdForServer()
  return self._bankerSeatId - 1
end
function Judge:setBankerSeatId(seatId)
  self._bankerSeatId = seatId
end
function Judge:getBankerSeatId()
  return self._bankerSeatId
end
function Judge:getIsImPlayer()
  return self.isImPlayer
end
function Judge:getImInCurrentGame()
  for i, player in ipairs(self.gamblers) do
    if player:getPlayStatus() ~= PlayStatus.NON_PLAY and player:getPlayStatus() ~= PlayStatus.WAIT_FOR_GAMESTART and player:getIsSelf() then
      return true
    end
  end
  return false
end
function Judge:getIsBounsTimeValid()
  for i, player in ipairs(self.gamblers) do
    if player:getIsSelf() and #self.gamblers > 1 then
      return true
    end
  end
  return false
end
function Judge:dispose()
  logger:debug("Object-Life:Judge: dispose")
  self._tableMessageList:dispose()
  self._tableMessageList = nil
  for i, handler in ipairs(self._judgeHandlers) do
    g.eventCenter:removeEventListener(handler)
    handler = nil
  end
  self._judgeHandlers = nil
  g.vars.user.seatId = nil
end
return Judge

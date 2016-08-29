local TableUIEvent = require("app.module.table.view.TableUIEvent")
local TableBeanEvent = require("app.beans.table.event.TableBeanEvent")
local ViceJudge = class("ViceJudge")
function ViceJudge:ctor()
  if DEBUG_MEMORY_STATUS then
    logger:debug("Object-Life:" .. self.__cname .. ": ctor")
    g.memoryManager:registerObject(self, "ViceJudge")
  end
  self._viceJudgeHandlers = {}
  local eventHandler1 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_EMOJI, handler(self, self.broadcastEmojiMessage))
  table.insert(self._viceJudgeHandlers, eventHandler1)
  local eventHandler2 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_CHAT, handler(self, self.broadcastChatMessage))
  table.insert(self._viceJudgeHandlers, eventHandler2)
  local eventHandler3 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_INTER_EQUIP, handler(self, self.broadcastMagicEmoji))
  table.insert(self._viceJudgeHandlers, eventHandler3)
  local eventHandler4 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_ADD_FRIEND, handler(self, self.broadcastAddFriend))
  table.insert(self._viceJudgeHandlers, eventHandler4)
  local eventHandler5 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_SENDCHIP_SUCCESS, handler(self, self.broadcastSendChipSuccess))
  table.insert(self._viceJudgeHandlers, eventHandler5)
  local eventHandler6 = g.eventCenter:addEventListener(TableBeanEvent.EVT_SVR_SENDCHIP_FAIL, handler(self, self.broadcastSendChipFail))
  table.insert(self._viceJudgeHandlers, eventHandler6)
  local eventHandler7 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_APPLY_GIFT, handler(self, self.broadcastApplyGift))
  table.insert(self._viceJudgeHandlers, eventHandler7)
  local eventHandler8 = g.eventCenter:addEventListener(TableBeanEvent.EVT_BROADCAST_SEND_GIFT, handler(self, self.broadcastSendGift))
  table.insert(self._viceJudgeHandlers, eventHandler8)
  local successCallback = function()
  end
  scheduler.performWithDelayGlobal(successCallback, 5)
end
function ViceJudge:asyncRequestSendEmoji(emojiType, emojiIndex)
  local data = {emojiType = emojiType, emojiIndex = emojiIndex}
  g.tasksInfo:reportTaskForEmoji()
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_SEND_EMOJI,
    data = data
  })
end
function ViceJudge:broadcastEmojiMessage(evt)
  logger:debug("ViceJudge-TestServer:broadcastEmojiMessage ")
  local packetModel = evt.data
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.EMOJI_BROADCAST,
    data = packetModel
  })
end
function ViceJudge:asyncRequestSendChat(chatMessage)
  logger:debug("ViceJudge-TestServer:asyncRequestSendChat")
  local data = {chatMessage = chatMessage}
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_SEND_CHAT,
    data = data
  })
end
function ViceJudge:broadcastChatMessage(evt)
  logger:debug("ViceJudge-TestServer:broadcastChatMessage")
  local packetModel = evt.data
  logger:debug(g.util.inspect(packetModel))
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.CHAT_BROADCAST,
    data = packetModel
  })
end
function ViceJudge:asyncRequestSendMajicEmoji(targetUid, targetSeatId, itemId, magicId)
  logger:debug("ViceJudge-TestServer:asyncRequestSendMajicEmoji")
  local requestData = {}
  requestData.uid = g.vars.user.uid
  requestData.seatId = g.vars.user.seatId
  requestData.targetUid = targetUid
  requestData.targetSeatId = targetSeatId
  requestData.itemId = itemId
  requestData.magicId = magicId
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("items", "apply", requestData), function(data)
    print(data)
    print("items/apply")
    local jsonData = json.decode(data)
    if jsonData ~= nil and jsonData.ret ~= nil and jsonData.ret == 0 then
      g.tasksInfo:reportTaskForMagicEmoji()
    elseif jsonData ~= nil and jsonData.ret == -11 then
      g.ui.manager.tip:top(g.lang:getContent("magic.noMagicItem"))
    else
      g.ui.manager.tip:top(g.lang:getContent("magic.magicErro"))
    end
  end, function()
    g.ui.manager.tip:top(g.lang:getContent("magic.magicErro"))
  end)
end
function ViceJudge:broadcastMagicEmoji(evt)
  local packetModel = evt.data
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.MAGIC_EMOJI_BROADCAST,
    data = packetModel
  })
end
function ViceJudge:broadcastAddFriend(evt)
  local packetModel = evt.data
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.ADD_FRIEND_BROADCAST,
    data = packetModel
  })
end
function ViceJudge:asyncRequestSendChip(targetUid, sendChips)
  local data = {targetUid = targetUid, sendChips = sendChips}
  g.eventCenter:dispatchEvent({
    name = TableBeanEvent.EVT_CLI_REQUEST_SEND_CHIP,
    data = data
  })
end
function ViceJudge:broadcastSendChipSuccess(evt)
  local packetModel = evt.data
  logger:debug("broadcastSendChipSuccess")
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.SENDCHIP_SUCCESS_BROADCAST,
    data = packetModel
  })
end
function ViceJudge:broadcastSendChipFail(evt)
  local packetModel = evt.data
  local errorCode = packetModel.errorCode
  if errorCode == 6 then
    g.ui.manager.tip:top(g.lang:getContent("table.sendChipNotEnough"))
  elseif errorCode == 17 then
    g.ui.manager.tip:top(g.lang:getContent("table.sendChipTooOften"))
  end
end
function ViceJudge:broadcastApplyGift(evt)
  local packetModel = evt.data
  logger:debug("broadcastApplyGift")
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.APLLY_GIFT,
    data = packetModel
  })
end
function ViceJudge:broadcastSendGift(evt)
  local packetModel = evt.data
  logger:debug("broadcastSendGift")
  g.eventCenter:dispatchEvent({
    name = TableUIEvent.Player.SEND_GIFT,
    data = packetModel
  })
end
function ViceJudge:dispose()
  logger:debug("Object-Life:ViceJudge: dispose")
  for i, handler in ipairs(self._viceJudgeHandlers) do
    g.eventCenter:removeEventListener(handler)
    handler = nil
  end
  self._viceJudgeHandlers = nil
end
return ViceJudge

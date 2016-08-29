local TableMessageList = class("TableMessageList")
local TableMessageListEventModel = require("app.module.table.model.TableMessageListEventModel")
TableMessageList.DISPATCH_EVENT = "TableMessageList.DISPATCH_EVENT"
TableMessageList.BROADCAST_POK = "BROADCAST_POK"
TableMessageList.BROADCAST_SPEAK_TURN = "BROADCAST_SPEAK_TURN"
TableMessageList.BROADCAST_SPEAK = "BROADCAST_SPEAK"
TableMessageList.BROADCAST_RESULT = "BROADCAST_RESULT"
function TableMessageList:ctor()
  if DEBUG_MEMORY_STATUS then
    logger:debug("Object-Life:" .. self.__cname .. ": ctor")
    g.memoryManager:registerObject(self, "TableMessageList")
  end
  self.events = {}
  self.dispatchEventHandler = g.eventCenter:addEventListener(TableMessageList.DISPATCH_EVENT, handler(self, self.dispatchEvent))
end
function TableMessageList:getEvent(eventName)
  for i, event in ipairs(self.events) do
    if event.eventName == eventName then
      return event
    end
  end
end
function TableMessageList:dispatchEvent(evt)
  local eventName = evt.data.eventName
  local plus = evt.data.plus
  for i, event in ipairs(self.events) do
    if event.eventName == eventName then
      event.eventCount = event.eventCount + plus
      if event.eventCount == 0 then
        if event.eventExcuteObj ~= nil then
          event:execute()
        end
        table.remove(self.events, i)
      end
      return
    end
  end
  local event = self:getEvent(eventName)
  if event == nil then
    event = TableMessageListEventModel.new()
    event.eventName = eventName
    event.eventCount = plus
    table.insert(self.events, event)
  end
end
function TableMessageList:clearData()
  self.events = {}
end
function TableMessageList:dispose()
  g.eventCenter:removeEventListener(self.dispatchEventHandler)
end
return TableMessageList

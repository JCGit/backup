DCEvent = {}
function DCEvent.onEventBeforeLogin(eventId, map, duration)
  DCLuaEvent:onEventBeforeLogin(eventId, map, duration)
end
function DCEvent.onEventCount(eventId, count)
  DCLuaEvent:onEventCount(eventId, count)
end
function DCEvent.onEvent(...)
  DCLuaEvent:onEvent(...)
end
function DCEvent.onEventDuration(...)
  DCLuaEvent:onEventDuration(...)
end
function DCEvent.onEventBegin(...)
  DCLuaEvent:onEventBegin(...)
end
function DCEvent.onEventEnd(...)
  DCLuaEvent:onEventEnd(...)
end
return DCEvent

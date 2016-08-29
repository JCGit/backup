local TableMessageListEventModel = class("TableMessageListEventModel")
function TableMessageListEventModel:ctor()
  self.eventName = ""
  self.eventExcuteObj = nil
  self.eventExcuteMethod = nil
  self.eventExcuteArguments = nil
  self.eventCount = 0
end
function TableMessageListEventModel:execute()
  if self.eventExcuteObj ~= nil and self.eventExcuteMethod ~= nil then
    self.eventExcuteMethod(self.eventExcuteObj, self.eventExcuteArguments)
  end
end
return TableMessageListEventModel

local EventCenter = class("EventCenter")
function EventCenter:ctor()
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
end
return EventCenter

local ComponentStatus = import(".ComponentStatus")
local Component = class("Component", function()
  return display.newNode()
end)
function Component:ctor()
  self:init()
  self:addEvents()
end
function Component:init()
  self.status = ComponentStatus.DEFAULT
end
function Component:update()
end
function Component:dispose()
end
function Component:addEvents()
end
function Component:removeEvents()
end
function Component:setStatus(value)
  if value ~= self.status then
    self.status = value
    self:onStatusChange()
  end
end
function Component:getStatus()
  return self.status
end
function Component:onStatusChange()
end
return Component

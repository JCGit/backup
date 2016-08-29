local ScrollBar = import(".ScrollBar")
local ScrollBarFactory = class("ScrollBarFactory")
function ScrollBarFactory:ctor()
end
function ScrollBarFactory:create(direction)
  return ScrollBar.new(direction)
end
return ScrollBarFactory

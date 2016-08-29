local MiniLoading = class("MiniLoading", function(filename)
  return display.newSprite(filename or "#common_mini_loading.png")
end)
function MiniLoading:ctor(filename)
  self:setNodeEventEnabled(true)
  self:setAnchorPoint(ccp(0.5, 0.5))
end
function MiniLoading:onEnter()
  self:runAction(CCRepeatForever:create(CCRotateBy:create(100, 36000)))
end
function MiniLoading:onExit()
  self:stopAllActions()
end
return MiniLoading

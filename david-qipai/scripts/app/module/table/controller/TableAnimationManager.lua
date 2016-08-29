local TableAnimationManager = class("TableAnimationManager")
function TableAnimationManager:ctor()
  self._sendWagerView = {}
end
function TableAnimationManager:playSendGiftAnimation(container, fromPoint, toPoint, gift, completeCallback)
  gift:addTo(container):pos(fromPoint.x, fromPoint.y)
  transition.moveTo(gift, {
    time = 2,
    easing = "exponentialInOut",
    x = toPoint.x,
    y = toPoint.y,
    onComplete = function()
      gift:removeFromParent()
      if completeCallback ~= nil then
        completeCallback()
      end
    end
  })
end
function TableAnimationManager:cancelAllAnimation()
  while #self._sendWagerView > 0 do
    local sp = table.remove(self._sendWagerView, 1)
    if sp:getParent() then
      sp:removeFromParent()
    end
  end
end
return TableAnimationManager

local BetClock = class("BetClock", function()
  return display.newNode()
end)
BetClock.PRO_WIDTH = 400
BetClock.PRO_HEIGHT = 12
function BetClock:ctor()
  self.progressBG = display.newScale9Sprite("#login_update_progressbar_bg.png")
  self.progressBG:setContentSize(CCSize(BetClock.PRO_WIDTH, BetClock.PRO_HEIGHT))
  self.progressBG:addTo(self)
  self.progressBG:setCascadeOpacityEnabled(true)
  self.progressBG:pos(display.cx, 50)
  self.imgClip = CCClippingNode:create():addTo(self)
  self.imgClip:setCascadeOpacityEnabled(true)
  self.imgClip:pos(display.cx, 50)
  self.stencil = display.newScale9Sprite("#table_bet_countdown_progress.png", -BetClock.PRO_WIDTH, 0, cc.size(BetClock.PRO_WIDTH, BetClock.PRO_HEIGHT))
  self.imgClip:setAlphaThreshold(0)
  self.imgClip:setStencil(self.stencil)
  self.progress = display.newScale9Sprite("#table_bet_countdown_progress.png")
  self.progress:addTo(self.imgClip)
  self.progress:setCascadeOpacityEnabled(true)
  self.progress:setContentSize(CCSize(BetClock.PRO_WIDTH, BetClock.PRO_HEIGHT))
  self.progressLabel = ui.newTTFLabel({
    text = g.lang:getContent("table.betting"),
    size = 17,
    color = g.font.color.GREY,
    align = ui.TEXT_ALIGN_LEFT
  })
  self.progressLabel:align(display.CENTER, display.cx, 25)
  self.progressLabel:addTo(self)
  self.progressLabel:setCascadeOpacityEnabled(true)
end
function BetClock:startCountDown(seconds)
  self.totalSeconds = seconds
  self.leftTime = self.totalSeconds
  self.startShowTime = g.util.functions.getTime()
  self.stencil:stopAllActions()
  self:stopAllActions()
  self.stencil:setPositionX(0)
  transition.fadeIn(self, {time = 1})
  transition.moveTo(self.stencil, {
    time = seconds,
    x = -BetClock.PRO_WIDTH,
    onComplete = function()
      self.startShowTime = nil
    end
  })
  transition.fadeOut(self, {
    time = 1,
    delay = seconds - 1
  })
end
function BetClock:hideImediatly()
  self:stopAllActions()
  self.stencil:stopAllActions()
  self:setOpacity(0)
  self.startShowTime = nil
end
function BetClock:hideWithAnimation()
  if self.startShowTime == nil then
    self:hideImediatly()
    return
  end
  local leftTime = g.util.functions.getTime() - self.startShowTime
  if leftTime > 1 then
    transition.stopTarget(self)
    self.stencil:stopAllActions()
    transition.fadeTo(self, {time = 1})
    self.startShowTime = nil
  end
end
return BetClock

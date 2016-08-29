local Component = import("..base.Component")
local HScrollBar = class("HScrollBar", Component)
function HScrollBar:ctor(params)
  HScrollBar.super.ctor(self)
  self._track = params.track
  self:addChild(self._track)
  self._minusButton = params.minusButton
  self:addChild(self._minusButton)
  self._plusButton = params.plusButton
  self:addChild(self._plusButton)
  self._slider = params.slider
  self:addChild(self._slider)
  self._step = params.step
end
return HScrollBar

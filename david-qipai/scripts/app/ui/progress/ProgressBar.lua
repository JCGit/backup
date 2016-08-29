local ProgressBar = class("ProgressBar", function()
  return display.newNode()
end)
function ProgressBar:ctor(targetBg, progressBg, sizes)
  self._sizes = sizes
  self._targetBg = display.newScale9Sprite(targetBg, 0, 0, CCSize(sizes.bgWidth, sizes.bgHeight)):align(display.LEFT_CENTER, 0, 0):addTo(self)
  self._progressBg = display.newScale9Sprite(progressBg, 0, 0, CCSize(sizes.progressWidth, sizes.progressHeight)):align(display.LEFT_CENTER, (sizes.bgHeight - sizes.progressHeight) * 0.5, 0):addTo(self)
  self._value = 0
  self._maxProgressWidth = sizes.bgWidth - (sizes.bgHeight - sizes.progressHeight)
end
function ProgressBar:setValue(value)
  if value == self._value then
    return self
  end
  if value <= 0 then
    value = 0
  end
  if value >= 1 then
    value = 1
  end
  self._value = value
  if self._value <= self._sizes.progressWidth / self._maxProgressWidth then
    self._progressBg:setContentSize(CCSize(self._sizes.progressWidth, self._sizes.progressHeight))
  else
    self._progressBg:setContentSize(CCSize(self._maxProgressWidth * self._value, self._sizes.progressHeight))
  end
  return self
end
return ProgressBar

local CommonRewardChipAnimation = require("app.beans.activity.CommonRewardChipAnimation")
local ChangeChipAnim = require("app.beans.activity.ChangeChipAnim")
local RouletteWheelView = class("RouletteWheelView", function()
  return display.newNode()
end)
local RouletteSliceView = import(".RouletteSliceView")
local RouletteShareView = import(".RouletteShareView")
function RouletteWheelView:ctor(width, height, playCallback)
  self:setNodeEventEnabled(true)
  self.width, self.height = width, height
  self:setupView()
  self.playCallback = playCallback
end
function RouletteWheelView:setupView()
  self._content = display.newNode():addTo(self):rotation(0)
  display.newSprite("#roulette_wheel.png"):addTo(self._content)
  display.newSprite("#roulette_spin.png"):pos(0, 32):addTo(self)
  self._slices = {}
  local slice
  local degree = 45
  for i = 1, 8 do
    slice = RouletteSliceView.new()
    slice:setDescText(i)
    slice:rotation(degree * (i - 1) + 22.5)
    slice:addTo(self._content):align(display.CENTER_BOTTOM)
    table.insert(self._slices, slice)
  end
  local playBtnLabelX = 0
  local playBtnLabelY = 5
  self._spinButton = cc.ui.UIPushButton.new({
    normal = "#roulette_start_button_up.png",
    pressed = "#roulette_start_button_up.png",
    disabled = "#roulette_start_button_disabled.png"
  }):pos(3, -3):addTo(self):onButtonPressed(function(event)
    self._spinButton:scale(0.9)
  end):onButtonRelease(function(event)
    self._spinButton:scale(1)
  end):onButtonClicked(handler(self, self._onSpinButtonClick)):setButtonLabel(ui.newTTFLabel({
    text = g.lang:getContent("luckyWheel.play"),
    size = 30,
    color = g.font.color.WHITE,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabelOffset(playBtnLabelX, playBtnLabelY)
end
function RouletteWheelView:setItemData(items)
  self._items = items
  for i, v in ipairs(items) do
    local slice = self._slices[i]
    slice:setDescText(v.desc)
    slice:setImageUrl(v.pic)
  end
end
function RouletteWheelView:_onSpinButtonClick()
  self:setPlayButtonEnable(false)
  function successCallback(retData)
    self:playNow(retData)
  end
  function failCallback()
    self:setPlayButtonEnable(true)
  end
  g.rouletteInfo:playRoulette(successCallback, failCallback)
end
function RouletteWheelView:setPlayButtonEnable(enable)
  if enable then
    self._spinButton:getButtonLabel():setColor(g.font.color.WHITE)
  else
    self._spinButton:getButtonLabel():setColor(g.font.color.DARK)
  end
  self._spinButton:setButtonEnabled(enable)
end
function RouletteWheelView:playNow(retData)
  self:setDestDegreeById(retData.index)
  g.audio:playSound("WHEEL_START")
  g.audio:playSound("WHEEL_LOOP")
  self:startRotation(function()
    self:setPlayButtonEnable(true)
    self.playCallback(retData)
    g.audio:playSound("WHEEL_END")
    if retData.type == "chips" then
      function successCallback()
        self:playRewardAnim(retData.quantity)
      end
      g.vars:syncChips(successCallback)
    end
    self:performWithDelay(function()
      g.audio:playSound("WHEEL_WIN")
      RouletteShareView.new(retData.index):show()
    end, 0.5)
  end)
end
function RouletteWheelView:playRewardAnim(deltaChips)
  self._animation = CommonRewardChipAnimation.new(ccp(0, 120), ccp(display.c_right - 330, display.c_bottom + 20)):addTo(self)
  g.audio:playSound("REWARD_GET")
  self:performWithDelay(handler(self, function(self)
    self._changeChipAnim = ChangeChipAnim.new(deltaChips):addTo(self:getParent():getParent(), 16):pos(display.right - 160, 10)
  end), 0.8)
  self:performWithDelay(handler(self, function(self)
    if self._animation and self._animation:getParent() then
      self._animation:removeFromParent()
      self._changeChipAnim:removeFromParent()
    end
  end), 1.5)
end
function RouletteWheelView:findItemById(id)
  return self._items[id + 1]
end
function RouletteWheelView:setDestDegreeById(id)
  math.randomseed(tostring(os.time()):reverse():sub(1, 6))
  local offsetDegree = 5
  local min = 3 + 45 * (7 - id) + offsetDegree
  local max = min - 3 + 45 - offsetDegree * 2
  self._destDegree = math.random(min, max)
end
function RouletteWheelView:startRotation(callback)
  self._animOverCallback = callback
  if self.soundId then
  end
  self:rotationByAccelerate()
end
function RouletteWheelView:rotationByAccelerate()
  self._content:stopAllActions()
  local sequence = transition.sequence({
    CCEaseIn:create(CCRotateBy:create(1, 360), 2.5),
    CCCallFunc:create(function()
      self:rotationByDefault()
    end)
  })
  self._content:runAction(sequence)
end
function RouletteWheelView:rotationByDefault()
  self._content:setRotation(self._destDegree)
  local sequence = transition.sequence({
    CCRotateBy:create(0.5, 360),
    CCCallFunc:create(function()
      self:rotationByDecelerate()
    end)
  })
  self._content:runAction(sequence)
end
function RouletteWheelView:rotationByDecelerate()
  local sequence = transition.sequence({
    CCEaseOut:create(CCRotateBy:create(3, 360), 2.5),
    CCCallFunc:create(function()
      if self.soundId then
        audio.stopSound(self.soundId)
      end
      if self._animOverCallback then
        self._animOverCallback()
      end
    end)
  })
  self._content:runAction(sequence)
end
function RouletteWheelView:onExit()
  self._content:stopAllActions()
  if self.soundId then
    audio.stopSound(self.soundId)
  end
end
return RouletteWheelView

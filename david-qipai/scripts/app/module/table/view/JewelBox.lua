local JewelBox = class("JewelBox", function()
  return display.newNode()
end)
local JewelBoxInfo = require("app.beans.data.JewelBoxInfo")
JewelBox.CLOSE = 1
JewelBox.WAIT_OPEN = 2
JewelBox.OPEN = 3
JewelBox.OPEN_TIME = 1.2
function JewelBox:ctor()
  self.jewelBoxTimeIsworking = false
  self:setNodeEventEnabled(true)
  self.box = display.newSprite("#table_jewelbox_close.png"):addTo(self)
  self.openState = JewelBox.CLOSE
  self.clickBtn = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#transparent.png"
  }, {scale9 = true}):addTo(self):setButtonSize(self.box:getContentSize().width, self.box:getContentSize().height):onButtonClicked(clickHandler(self, self.onBoxClick))
  self.clickBtn:setTouchSwallowEnabled(false)
  self._timeLabel = ui.newTTFLabel({
    text = g.lang:getContent("exchangAward.title"),
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, -self.box:getContentSize().height * 0.5 - 8):addTo(self)
  self._bonusLabel = ui.newTTFLabel({
    text = "",
    size = 24,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, 20):addTo(self)
  self.timerHandler = scheduler.scheduleGlobal(handler(self, self.timerCallback), 1)
  self:hide()
  self:startTiming()
  self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self.onEnterFrame))
  return
end
function JewelBox:timerCallback()
  if self.openState == JewelBox.OPEN then
    if g.jewelBoxInfo.dataModel ~= nil and g.jewelBoxInfo.dataModel.leftTime ~= nil then
      g.jewelBoxInfo.dataModel.leftTime = g.jewelBoxInfo.dataModel.leftTime - 1
    end
    return
  end
  local boxState = g.jewelBoxInfo:getBoxState()
  if boxState == JewelBoxInfo.NO_ACTIVITY then
    self:hide()
  elseif boxState == JewelBoxInfo.ACTIVITY_NOT_START then
    self:show()
    local timeInfo = self:timeFormate(g.jewelBoxInfo.dataModel.leftTime)
    self._timeLabel:setString(timeInfo)
    local chipNumber = 0
    local rewardsInfo = json.decode(g.jewelBoxInfo.dataModel.reward or "{}")
    for k, v in pairs(rewardsInfo) do
      if k == "chips" then
        chipNumber = tonumber(v or 0)
      end
    end
    local str = string.gsub(g.lang:getContent("table.jewelBoxInfo"), "{chips}", g.util.functions.formatBigInt(chipNumber))
    self._bonusLabel:setString(str)
  elseif boxState == JewelBoxInfo.ACTIVITY_IN_COUNT then
    self:show()
    if tonumber(g.jewelBoxInfo.dataModel.leftTime) == 30 or tonumber(g.jewelBoxInfo.dataModel.leftTime) == 0 then
      g.jewelBoxInfo:progress()
    end
    g.jewelBoxInfo.dataModel.leftTime = g.jewelBoxInfo.dataModel.leftTime - 1
    if g.jewelBoxInfo.dataModel.leftTime < 0 then
      g.jewelBoxInfo.dataModel.leftTime = 0
    end
    local timeInfo = self:timeFormate(g.jewelBoxInfo.dataModel.leftTime)
    self._timeLabel:setString(timeInfo)
    local chipNumber = 0
    local rewardsInfo = json.decode(g.jewelBoxInfo.dataModel.reward or "{}")
    for k, v in pairs(rewardsInfo) do
      if k == "chips" then
        chipNumber = tonumber(v or 0)
      end
    end
    local str = string.gsub(g.lang:getContent("table.jewelBoxInfo"), "{chips}", g.util.functions.formatBigInt(chipNumber))
    self._bonusLabel:setString(str)
  elseif boxState == JewelBoxInfo.ACTIVITY_REWARD then
    self._timeLabel:setString("")
    local chipNumber = 0
    local rewardsInfo = json.decode(g.jewelBoxInfo.dataModel.reward or "{}")
    for k, v in pairs(rewardsInfo) do
      if k == "chips" then
        chipNumber = tonumber(v or 0)
      end
    end
    local str = string.gsub(g.lang:getContent("table.jewelBoxInfo"), "{chips}", g.util.functions.formatBigInt(chipNumber))
    self._bonusLabel:setString(str)
    self:startShakeAnimation()
    self:show()
    self:bindOpenState(JewelBox.WAIT_OPEN)
  end
end
function JewelBox:onExit()
  scheduler.unscheduleGlobal(self.timerHandler)
  self:removeAllNodeEventListeners()
  self:stopShakeAnimation()
end
function JewelBox:onBoxClick()
  if self.openState == JewelBox.WAIT_OPEN then
    local function successCallback()
      g.jewelBoxInfo:clearData()
      self:stopShakeAnimation()
      self:bindOpenState(JewelBox.OPEN)
      self:performWithDelay(handler(self, function(self)
        self._bonusLabel:setString("")
        self:bindOpenState(JewelBox.CLOSE)
      end), JewelBox.OPEN_TIME)
      g.jewelBoxInfo:progress()
    end
    local failCallback = function()
      print("update!!!!!!!!!!!!!!!!!!!!!!!!!")
      g.jewelBoxInfo:progress()
    end
    g.jewelBoxInfo:reward(successCallback, failCallback)
  end
end
function JewelBox:bindOpenState(openState)
  self.openState = openState
  if self.openState == JewelBox.CLOSE then
    local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("table_jewelbox_close.png")
    self.box:setDisplayFrame(frame)
  elseif self.openState == JewelBox.WAIT_OPEN then
    local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("table_jewelbox_highlight.png")
    self.box:setDisplayFrame(frame)
  elseif self.openState == JewelBox.OPEN then
    local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName("table_jewelbox_open.png")
    self.box:setDisplayFrame(frame)
  end
end
function JewelBox:startTiming()
  if self.jewelBoxTimeIsworking then
    return
  end
  local retryCount = 3
  self.retryFuncion = handler(self, function()
    retryCount = retryCount - 1
    if retryCount < 0 then
      return
    else
      local function successCallback()
        self.jewelBoxTimeIsworking = true
      end
      local function failCallback()
        if self.retryFuncion then
          self.retryFuncion()
        end
      end
      g.jewelBoxInfo:progress(successCallback, failCallback)
    end
  end)
  self.retryFuncion()
end
function JewelBox:onEnterFrame(dt)
  local posX, posY = self.box:getPosition()
  if posX <= -1 or posX >= 1 then
    posX = 0
    self.box:setPositionX(posX)
  end
  if posY <= -1 or posY >= 1 then
    posY = 0
    self.box:setPositionY(posY)
  end
  posX = posX + math.random(-1, 1)
  posY = posY + math.random(-1, 1)
  self.box:pos(posX, posY)
end
function JewelBox:startShakeAnimation()
  if self._isShaking == false or self._isShaking == nil then
    self._isShaking = true
    self:scheduleUpdate()
  end
end
function JewelBox:stopShakeAnimation()
  if self._isShaking then
    self:unscheduleUpdate()
  end
  self.box:pos(0, 0)
  self._isShaking = false
end
function JewelBox:timeFormate(leftTime)
  leftTime = tonumber(leftTime or 0)
  local timeInfo = ""
  if leftTime > 0 then
    local hour = math.floor(leftTime / 3600)
    local min = math.floor((leftTime - hour * 3600) / 60)
    local sec = math.floor(leftTime - hour * 3600 - min * 60)
    if hour < 10 then
      hour = "0" .. tostring(hour)
    end
    if min < 10 then
      min = "0" .. tostring(min)
    end
    if sec < 10 then
      sec = "0" .. tostring(sec)
    end
    timeInfo = hour .. ":" .. min .. ":" .. sec
  else
    timeInfo = "00:00:00"
  end
  return timeInfo
end
return JewelBox

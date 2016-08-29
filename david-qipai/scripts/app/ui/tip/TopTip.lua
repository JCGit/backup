local TopTip = class("TopTip")
local DEFAULT_STAY_TIME = 3
local X_GAP = 100
local Y_GAP = 0
local TIP_HEIGHT = 72
local LABEL_X_GAP = 16
local ICON_SIZE = 56
local LABEL_ROLL_VELOCITY = 80
local BG_CONTENT_SIZE = CCSize(display.width - X_GAP * 2, TIP_HEIGHT)
local Z_ORDER = 1001
function TopTip:ctor()
  self._container = display.newNode()
  self._container:retain()
  self._container:setNodeEventEnabled(true)
  self._container.onCleanup = handler(self, function(obj)
    if obj._currentData and obj._currentData.image and type(obj._currentData.image) == "userdata" then
      obj._currentData.image:release()
      obj._currentData.image:removeFromParent()
    end
    if obj._delayScheduleHandle then
      scheduler.unscheduleGlobal(obj._delayScheduleHandle)
      obj._delayScheduleHandle = nil
    end
    scheduler.performWithDelayGlobal(function()
      obj:_playNext()
    end, 1)
    print("container removed")
  end)
  self._waitQueue = {}
  self._isPlaying = false
end
function TopTip:play(topTipData)
  assert(type(topTipData) == "table" or type(topTipData) == "string", "topTipData should be a table")
  if not self._tipBg then
    self._tipBg = display.newScale9Sprite("#top_tip_bg.png", 0, 0, BG_CONTENT_SIZE):addTo(self._container)
    self._smallStencil = display.newDrawNode()
    self._smallStencil:drawPolygon({
      {
        -BG_CONTENT_SIZE.width * 0.5 + LABEL_X_GAP * 2 + ICON_SIZE,
        -BG_CONTENT_SIZE.height * 0.5
      },
      {
        -BG_CONTENT_SIZE.width * 0.5 + LABEL_X_GAP * 2 + ICON_SIZE,
        BG_CONTENT_SIZE.height * 0.5
      },
      {
        BG_CONTENT_SIZE.width * 0.5 - LABEL_X_GAP,
        BG_CONTENT_SIZE.height * 0.5
      },
      {
        BG_CONTENT_SIZE.width * 0.5 - LABEL_X_GAP,
        -BG_CONTENT_SIZE.height * 0.5
      }
    })
    self._smallStencil:retain()
    self._bigStencil = display.newDrawNode()
    self._bigStencil:drawPolygon({
      {
        -BG_CONTENT_SIZE.width * 0.5 + LABEL_X_GAP,
        -BG_CONTENT_SIZE.height * 0.5
      },
      {
        -BG_CONTENT_SIZE.width * 0.5 + LABEL_X_GAP,
        BG_CONTENT_SIZE.height * 0.5
      },
      {
        BG_CONTENT_SIZE.width * 0.5 - LABEL_X_GAP,
        BG_CONTENT_SIZE.height * 0.5
      },
      {
        BG_CONTENT_SIZE.width * 0.5 - LABEL_X_GAP,
        -BG_CONTENT_SIZE.height * 0.5
      }
    })
    self._bigStencil:retain()
    self._clipNode = cc.ClippingNode:create():addTo(self._container)
    self._clipNode:setStencil(self._bigStencil)
    self._label = ui.newTTFLabel({
      text = "",
      size = 28,
      align = ui.TEXT_ALIGN_CENTER
    }):addTo(self._clipNode)
  end
  if type(topTipData) == "string" then
    for _, v in pairs(self._waitQueue) do
      if v.text == topTipData then
        return
      end
    end
    table.insert(self._waitQueue, {text = topTipData})
  else
    for _, v in pairs(self._waitQueue) do
      if v.text == topTipData.text then
        return
      end
    end
    if topTipData.image and type(topTipData.image) == "userdata" then
      topTipData.image:retain()
    end
    table.insert(self._waitQueue, topTipData)
  end
  if not self._isPlaying then
    self:_playNext()
  end
end
function TopTip:_playNext()
  if self._waitQueue[1] then
    self._currentData = table.remove(self._waitQueue, 1)
  else
    self._isPlaying = false
    return
  end
  local topTipData = self._currentData
  local scrollTime = 0
  if topTipData.text then
    self._label:setString(topTipData.text)
    local labelWidth = self._label:getContentSize().width
    local startXPos = 0
    if topTipData.image and type(topTipData.image) == "userdata" then
      topTipData.image:pos(LABEL_X_GAP + ICON_SIZE * 0.5 - BG_CONTENT_SIZE.width * 0.5, 0):addTo(self._container)
      self._clipNode:setStencil(self._smallStencil)
      scrollTime = (labelWidth - (BG_CONTENT_SIZE.width - LABEL_X_GAP * 2 - LABEL_X_GAP - ICON_SIZE)) / LABEL_ROLL_VELOCITY
      if scrollTime > 0 then
        startXPos = labelWidth * 0.5 - BG_CONTENT_SIZE.width * 0.5 + LABEL_X_GAP + LABEL_X_GAP + ICON_SIZE
        self._label:pos(startXPos, 0)
        transition.execute(self._label, cc.MoveTo:create(scrollTime, ccp(-startXPos + LABEL_X_GAP + ICON_SIZE, 0)), {delay = 1.5})
      else
        scrollTime = 0
        self._label:pos((LABEL_X_GAP * 2 + ICON_SIZE) * 0.5, 0)
      end
    else
      self._clipNode:setStencil(self._bigStencil)
      scrollTime = (labelWidth - (BG_CONTENT_SIZE.width - LABEL_X_GAP * 2)) / LABEL_ROLL_VELOCITY
      if scrollTime > 0 then
        startXPos = labelWidth * 0.5 - BG_CONTENT_SIZE.width * 0.5 + LABEL_X_GAP
        self._label:pos(startXPos, 0)
        transition.execute(self._label, cc.MoveTo:create(scrollTime, ccp(-startXPos, 0)), {
          delay = DEFAULT_STAY_TIME * 0.5
        })
      else
        scrollTime = 0
        self._label:pos(0, 0)
      end
    end
  end
  self._isPlaying = true
  self._container:pos(display.cx, display.top + TIP_HEIGHT * 0.5):addTo(cc.Director:sharedDirector():getRunningScene(), Z_ORDER):moveTo(0.3, display.cx, display.top - Y_GAP - TIP_HEIGHT * 0.5)
  self._delayScheduleHandle = scheduler.performWithDelayGlobal(handler(self, self._delayCallback), 0.3 + DEFAULT_STAY_TIME + scrollTime)
  local getFrame = display.newSpriteFrame
  if topTipData.messageType == 1000 then
    self._label:setColor(ccc3(255, 174, 112))
    self._tipBg:setSpriteFrame(getFrame("common-da-laba-play-tip-icon.png"))
    self._tipBg:setContentSize(display.width - X_GAP * 2, TIP_HEIGHT)
  else
    self._label:setColor(ccc3(255, 255, 255))
    self._tipBg:setSpriteFrame(getFrame("top_tip_bg.png"))
    self._tipBg:setContentSize(display.width - X_GAP * 2, TIP_HEIGHT)
  end
end
function TopTip:_delayCallback()
  self._delayScheduleHandle = nil
  if self._container:getParent() then
    transition.moveTo(self._container, {
      x = display.cx,
      y = display.top + TIP_HEIGHT * 0.5,
      time = 0.3,
      onComplete = handler(self, self._onHideComplete)
    })
  else
    self._container:pos(display.cx, display.top + TIP_HEIGHT * 0.5)
    self:_onHideComplete()
  end
end
function TopTip:_onHideComplete()
  self._container:removeFromParent()
end
return TopTip

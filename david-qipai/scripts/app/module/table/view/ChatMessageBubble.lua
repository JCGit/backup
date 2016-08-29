local ChatMessageBubble = class("ChatMessageBubble", function()
  return display.newNode()
end)
ChatMessageBubble.DIRECTION_LEFT = 1
ChatMessageBubble.DIRECTION_RIGHT = 2
function ChatMessageBubble:ctor(label, direction)
  if direction == ChatMessageBubble.DIRECTION_LEFT then
    self._backgroundImage = display.newScale9Sprite("#table_chat_msg_right_bubble.png")
  else
    self._backgroundImage = display.newScale9Sprite("#table_chat_msg_right_bubble.png")
    self._backgroundImage:setScaleX(-1)
  end
  self._backgroundImage:setCapInsets(cc.rect(36, 15, 1, 1))
  self._backgroundImage:addTo(self)
  self._chatContentLabel = ui.newTTFLabel({
    text = label,
    size = 22,
    color = ccc3(85, 92, 100),
    align = ui.TEXT_ALIGN_CENTER,
    dimensions = cc.size(152, 68)
  })
  self._chatContentLabel:setAnchorPoint(ccp(0.5, 0.5))
  self._chatContentLabel:addTo(self)
  local lbsize = self._chatContentLabel:getContentSize()
  local bgw = math.max(64, lbsize.width + 16)
  local bgh = math.max(lbsize.height + 30, 55)
  self._backgroundImage:setContentSize(cc.size(bgw, bgh))
  if direction == ChatMessageBubble.DIRECTION_LEFT then
    self._backgroundImage:pos(bgw * 0.5, bgh * 0.5)
    self._chatContentLabel:pos(bgw * 0.5, math.floor((bgh + 11) * 0.5))
  else
    self._backgroundImage:pos(bgw * -0.5, bgh * 0.5)
    self._chatContentLabel:pos(bgw * -0.5, math.floor((bgh + 11) * 0.5))
  end
end
function ChatMessageBubble:show(parent, x, y)
  if self:getParent() then
    self:removeFromParent()
  end
  self:pos(x, y):addTo(parent, 100)
  self:scale(0.3)
  self:scaleTo(0.3, 1)
end
function ChatMessageBubble:setPositionId(positionId)
  self._positionId = positionId
end
function ChatMessageBubble:getPositionId()
  return self._positionId
end
return ChatMessageBubble

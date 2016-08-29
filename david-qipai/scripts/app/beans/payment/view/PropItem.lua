local PropItem = class("PropItem", g.ui.ListItem)
PropItem.WIDTH = 100
PropItem.HEIGHT = 77
PropItem.PADDING_LEFT = 2
PropItem.PADDING_RIGHT = 2
local ROW_GAP = 2
function PropItem:ctor()
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
  PropItem.super.ctor(self, PropItem.WIDTH, PropItem.HEIGHT + ROW_GAP)
  local contentWidth = PropItem.WIDTH - PropItem.PADDING_LEFT - PropItem.PADDING_RIGHT
  local contentHeight = PropItem.HEIGHT
  self._isFolded = true
  self._touchButton = cc.ui.UIPushButton.new("#transparent.png", {scale9 = true})
  self._touchButton:setTouchSwallowEnabled(false)
  self._touchButton:onButtonPressed(function(evt)
    self._buttonPressedY = evt.y
    self._buttonClickCanceled = false
  end)
  self._touchButton:onButtonRelease(function(evt)
    if math.abs(evt.y - self._buttonPressedY) > 10 then
      self._buttonClickCanceled = true
    end
  end)
  self._touchButton:onButtonClicked(clickHandler(self, function(evt)
    if not self._buttonClickCanceled and self:getParent():getParent():getCascadeBoundingBox():containsPoint(ccp(evt.x, evt.y)) then
      g.audio:playSound("CLICK_BUTTON")
      self:foldContent()
    end
  end))
  self._touchButton:setButtonSize(PropItem.WIDTH, PropItem.HEIGHT + ROW_GAP)
  self._touchButton:pos(PropItem.WIDTH * 0.5, (PropItem.HEIGHT + ROW_GAP) * 0.5)
  self._touchButton:addTo(self)
  self._content = display.newNode():addTo(self):pos(PropItem.WIDTH * 0.5 + (PropItem.PADDING_LEFT - PropItem.PADDING_RIGHT) * 0.5, (PropItem.HEIGHT + ROW_GAP) * 0.5)
  self._extraHeight = 0
  self._bottomPanel = cc.ClippingNode:create():addTo(self._content)
  self._bottomPanel:setContentSize(cc.size(contentWidth, 1))
  self._stencil = display.newRect(contentWidth, contentHeight * 0.5, {
    fill = true,
    color = ccc4f(1, 1, 1, 1)
  })
  self._stencil:setAnchorPoint(ccp(0.5, 0))
  self._stencil:setPositionY(-contentHeight * 0.5)
  self._bottomPanel:setStencil(self._stencil)
  self._topPanel = display.newNode():addTo(self._content)
  self._bottomBg = display.newScale9Sprite("#store_item_extended.png"):addTo(self._bottomPanel)
  self._topBg = display.newScale9Sprite("#store_item_background.png")
  self._topBg:setContentSize(CCSize(contentWidth, contentHeight))
  self._topBg:addTo(self._topPanel)
  self._foldIcon = display.newSprite("#store_list_triangle.png", contentWidth * -0.5 + 28, 0):addTo(self._topPanel)
  self._foldIcon:setAnchorPoint(ccp(0.3, 0.5))
  self._title = ui.newTTFLabel({
    size = 28,
    color = ccc3(186, 233, 255),
    align = ui.TEXT_ALIGN_LEFT
  }):pos(contentWidth * -0.5 + 172, 0):addTo(self._topPanel)
  self._title:setAnchorPoint(ccp(0, 0.5))
  self._title:setTouchEnabled(false)
  self._buyButton = cc.ui.UIPushButton.new({
    normal = "#store_buy_btn_up.png",
    pressed = "#store_buy_btn_down.png",
    disabled = "#store_btn_disabled.png"
  }, {scale9 = true})
  self._buyButton:setButtonLabel(ui.newTTFLabel({
    size = 28,
    color = ccc3(255, 255, 255),
    align = ui.TEXT_ALIGN_CENTER
  }))
  self._buyButton:onButtonClicked(clickHandler(self, function(evt)
    local now = g.util.functions.getTime()
    if not PropItem.buyButtonLastClickTime or math.abs(now - PropItem.buyButtonLastClickTime) > 2 then
      PropItem.buyButtonLastClickTime = now
      g.audio:playSound("CLICK_BUTTON")
      self:dispatchEvent({
        name = "ITEM_EVENT",
        type = "PURCHASE",
        data = self._data
      })
    end
  end))
  self._buyButton:setButtonSize(150, PropItem.HEIGHT)
  self._buyButton:pos(contentWidth * 0.5 - 75, 0)
  self._buyButton:addTo(self._topPanel)
  self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, handler(self, self._onEnterFrame))
end
function PropItem:getContentSize()
  return cc.size(PropItem.WIDTH, PropItem.HEIGHT + self._extraHeight)
end
function PropItem:_onTouch(evt)
  local name, x, y, prevX, prevY = evt.name, evt.x, evt.y, evt.prevX, evt.prevY
  local isTouchInSprite = self:getCascadeBoundingBox():containsPoint(CCPoint(x, y))
  if name == "began" then
    if isTouchInSprite then
      self._beginY = evt.y
      self._cancelClick = false
      self._isTouching = true
      return true
    else
      return false
    end
  elseif not self._isTouching then
    return false
  elseif name == "moved" then
    if math.abs(evt.y - self._beginY) > 10 then
      self._cancelClick = true
    end
  elseif name == "ended" or name == "cancelled" then
    self._isTouching = false
    if not self._cancelClick and isTouchInSprite and self:getParent():getParent():getCascadeBoundingBox():containsPoint(ccp(evt.x, evt.y)) then
      self:foldContent()
    end
  end
  return true
end
function PropItem:_onEnterFrame()
  local bottomHeight = self._bottomBg:getContentSize().height
  local dest, direction
  if self._isFolded then
    dest = 0
    direction = -1
  else
    dest = bottomHeight - 8
    direction = 1
  end
  if self._extraHeight == dest then
    self:unscheduleUpdate()
  else
    self._extraHeight = self._extraHeight + direction * math.max(1, math.abs(self._extraHeight - dest) * 0.14)
    if direction > 0 and dest < self._extraHeight or direction < 0 and dest > self._extraHeight then
      self._extraHeight = dest
    end
  end
  self._foldIcon:rotation(90 * (self._extraHeight / (bottomHeight - 8)))
  local contentHeight = PropItem.HEIGHT + self._extraHeight + ROW_GAP
  self._bottomPanel:setPositionY(-self._extraHeight)
  self._content:setPositionY((PropItem.HEIGHT + ROW_GAP) * 0.5 + self._extraHeight)
  self._touchButton:setButtonSize(PropItem.WIDTH, PropItem.HEIGHT + self._extraHeight)
  self._touchButton:setPositionY(contentHeight * 0.5)
  self._stencil:setScaleY((PropItem.HEIGHT * 0.2 + self._extraHeight) / (PropItem.HEIGHT * 0.5))
  self:dispatchEvent({name = "RESIZE"})
end
function PropItem:foldContent()
  if self._isFolded then
    self._isFolded = false
  else
    self._isFolded = true
  end
  self:unscheduleUpdate()
  self:scheduleUpdate()
end
function PropItem:onDataSet(dataChanged, data)
  if dataChanged then
    local contentWidth = PropItem.WIDTH - PropItem.PADDING_LEFT - PropItem.PADDING_RIGHT
    local contentHeight = PropItem.HEIGHT
    local details = string.split(data.detail, "\n")
    local row = 0
    local h = 24
    if self._detailLabels then
      for i, v in ipairs(self._detailLabels) do
        v:removeFromParent()
      end
    end
    self._detailLabels = {}
    for i = 1, #details do
      local detail = details[i]
      if detail and string.trim(detail) and string.trim(detail) ~= "" then
        row = row + 1
        local label = ui.newTTFLabel({
          text = detail,
          size = 20,
          color = ccc3(66, 119, 186),
          align = ui.TEXT_ALIGN_LEFT
        })
        label:setAnchorPoint(ccp(0, 0.5))
        self._detailLabels[row] = label
        h = h + 30
      end
    end
    self._bottomBg:setContentSize(cc.size(contentWidth - 4, h))
    self._bottomBg:setPositionY(h * 0.5 - contentHeight * 0.5 + 6)
    local y = h - PropItem.HEIGHT * 0.5 - 10
    for i, label in ipairs(self._detailLabels) do
      label:pos(-0.5 * contentWidth + 48, y - 15)
      display.newSprite("#store_list_circle.png", -0.5 * contentWidth + 30, y - 15):addTo(self._bottomPanel)
      label:addTo(self._bottomPanel)
      y = y - 30
    end
    self._title:setString(data.name)
    if data.buyButtonEnabled == false then
      self._buyButton:setButtonEnabled(false)
    else
      self._buyButton:setButtonEnabled(true)
    end
    if data.buyButtonLabel then
      self._buyButton:setButtonLabelString(data.buyButtonLabel)
    else
      self._buyButton:setButtonLabelString(data.priceLabel or "")
    end
    if self._product then
      self._product:removeFromParent()
      self._product = nil
    end
    local filename
    if data.propId == "1" then
      filename = "store_prd_prop.png"
    elseif data.propId == "2" then
      filename = "store_prd_broadcast.png"
    end
    if filename then
      local path = CCFileUtils:sharedFileUtils():fullPathForFilename(filename)
      if io.exists(path) then
        self._product = display.newSprite(filename)
      else
        local productFrame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(filename)
        if productFrame then
          self._product = display.newSprite(productFrame)
        end
      end
    end
    if self._product then
      self._product:addTo(self._topPanel):pos(contentWidth * -0.5 + 100, 0)
    end
    if not self._isFolded and data then
      self:foldContent()
    end
  end
end
return PropItem

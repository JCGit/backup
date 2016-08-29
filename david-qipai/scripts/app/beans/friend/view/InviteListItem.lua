local InviteListItem = class("InviteListItem", function()
  return display.newNode()
end)
InviteListItem.ITEM_WIDTH = 333
InviteListItem.ITEM_HEIGHT = 84
function InviteListItem:ctor()
  self:setNodeEventEnabled(true)
  self:setContentSize(260, 100)
end
function InviteListItem:setData(value)
  self._data = value
  return self
end
function InviteListItem:initialize()
  local data = self._data
  self._uncheckedBg = display.newScale9Sprite("#common_standard_bluebg_10.png"):size(InviteListItem.ITEM_WIDTH, InviteListItem.ITEM_HEIGHT):addTo(self):hide()
  self._checkedBg = display.newScale9Sprite("#common_standard_bluebg_09.png"):size(InviteListItem.ITEM_WIDTH, InviteListItem.ITEM_HEIGHT):addTo(self):hide()
  self._checkBoxBg = display.newSprite("#pd_common_check_bg.png"):pos(139, 0):addTo(self)
  self._checkIcon = display.newSprite("#pd_common_check_icon.png"):pos(147, 0):addTo(self):hide()
  self._background = display.newScale9Sprite("#transparent.png"):size(InviteListItem.ITEM_WIDTH, InviteListItem.ITEM_HEIGHT):addTo(self)
  self._headPic = g.ui.AvatarView.new():pos(-125, 2):scale(0.78):addTo(self)
  self._headPic:setImage(data.url, {
    defaultImageName = "#common_male_avatar.png"
  })
  self._background:setTouchSwallowEnabled(false)
  self._background:setTouchEnabled(true)
  self._background:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self.onTouch))
  ui.newTTFLabel({
    text = g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 24, data.name, 190),
    color = g.font.color.WHITE,
    size = 24,
    align = ui.TEXT_ALIGN_LEFT,
    dimensions = cc.size(200, 0)
  }):align(display.LEFT_CENTER, -81, 16):addTo(self)
  ui.newTTFLabel({
    text = "+" .. g.vars.user.rewards.info.facebookInviteReward,
    color = g.font.color.GOLDEN,
    size = 20,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.LEFT_CENTER, -81, -18):addTo(self)
  if self._isSelected then
    self._checkedBg:show()
    self._checkIcon:show()
    self._uncheckedBg:hide()
  else
    self._checkedBg:hide()
    self._checkIcon:hide()
    self._uncheckedBg:show()
  end
end
function InviteListItem:lazyInitialize()
  if not self.initialized then
    self.initialized = true
    self:initialize()
  end
  return self
end
function InviteListItem:onItemInactive()
  if self.initialized then
  end
end
function InviteListItem:onTouch(evt)
  local name, x, y, prevX, prevY = evt.name, evt.x, evt.y, evt.prevX, evt.prevY
  local isTouchInBounding = self:getCascadeBoundingBox():containsPoint(CCPoint(x, y))
  if name == "began" then
    self._ownerBeginPlace = self._owner:getCurrentPlace()
  elseif name == "ended" or name == "cancelled" then
    if isTouchInBounding and math.abs(self._ownerBeginPlace - self._owner:getCurrentPlace()) < 20 then
      if self._isSelected then
        self._isSelected = false
        self._checkedBg:hide()
        self._checkIcon:hide()
        self._uncheckedBg:show()
      elseif g.vars.friend.facebookInviteSelected >= 50 then
        g.ui.manager.tip:top(g.lang:getContent("Ffriend.inviteLimitExceedTip"))
      else
        self._isSelected = true
        self._checkedBg:show()
        self._checkIcon:show()
        self._uncheckedBg:hide()
      end
    end
    self._popup:setSelecteTip()
  end
  return true
end
function InviteListItem:getData()
  return self._data
end
function InviteListItem:setOwner(owner)
  self._owner = owner
  return self
end
function InviteListItem:getOwner()
  return self._owner
end
function InviteListItem:setPopup(popup)
  self._popup = popup
  return self
end
function InviteListItem:getPopup()
  return self._popup
end
function InviteListItem:isSelected()
  return self._isSelected
end
function InviteListItem:setSelected(isSelected)
  self._isSelected = isSelected
  if self.initialized then
    if self._isSelected then
      self._checkedBg:show()
      self._checkIcon:show()
      self._uncheckedBg:hide()
    else
      self._checkedBg:hide()
      self._checkIcon:hide()
      self._uncheckedBg:show()
    end
  end
end
return InviteListItem

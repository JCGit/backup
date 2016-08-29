local GiftListPanel = class("GiftListPanel", g.ui.ListPanel)
function GiftListPanel:ctor(...)
  GiftListPanel.super.ctor(self, ...)
  self._checkButtonGroup = g.ui.CheckButtonGroup.new()
end
function GiftListPanel:selectGiftById(id)
  if id then
    local button = self._checkButtonGroup:getButtonById(id)
    if button then
      button:setButtonSelected(true)
    end
  end
end
function GiftListPanel:selectGiftByIndex(index)
  local button = self._checkButtonGroup:getButtonAtIndex(index)
  if button then
    button:setButtonSelected(true)
  end
end
function GiftListPanel:setData(value, params)
  self._data = value
  self._checkButtonGroup:reset()
  self._checkButtonGroup = g.ui.CheckButtonGroup.new()
  local prevItemCount = self._itemCount or 0
  self._itemCount = self._data and #self._data or 0
  if self._items then
    if prevItemCount > self._itemCount then
      for i = self._itemCount + 1, prevItemCount do
        table.remove(self._items):removeFromParent()
      end
    end
  else
    self._items = {}
  end
  local contentSize = 0
  local itemResizeHandler = handler(self, self._onItemResize)
  local itemEventHandler = handler(self, self._onItemEvent)
  if self._direction == g.ui.ScrollPanel.DIRECTION_VERTICAL then
    for i = 1, self._itemCount do
      if not self._items[i] then
        self._items[i] = self._itemClass.new():addTo(self._content)
        if self._items[i].addEventListener then
          self._items[i]:addEventListener("RESIZE", itemResizeHandler)
          self._items[i]:addEventListener("ITEM_EVENT", itemEventHandler)
        end
      end
      self._items[i]:show()
      self._items[i]:setIndex(i)
      self._items[i]:setData(self._data[i], self._checkButtonGroup, params)
      self._items[i]:setOwner(self)
      contentSize = contentSize + self._items[i]:getContentSize().height
    end
    if self._itemCount > 0 then
      local size = self._items[1]:getContentSize()
      self._items[1]:pos(-size.width * 0.5, contentSize * 0.5 - size.height)
      for i = 2, self._itemCount do
        size = self._items[i]:getContentSize()
        self._items[i]:pos(-size.width * 0.5, self._items[i - 1]:getPositionY() - size.height)
      end
    end
    self._content:setContentSize(cc.size(self._content:getCascadeBoundingBox().width, contentSize))
  elseif self._direction == g.ui.ScrollPanel.DIRECTION_HORIZONTAL then
    for i = 1, self._itemCount do
      if not self._items[i] then
        self._items[i] = self._itemClass.new():addTo(self._content)
      end
      self._items[i]:show()
      self._items[i]:setIndex(i)
      self._items[i]:setData(self._data[i], self._checkButtonGroup, params)
      self._items[i]:setOwner(self)
      contentSize = contentSize + self._items[i]:getContentSize().width
    end
    if self._itemCount > 0 then
      local size = self._items[1]:getContentSize()
      self._items[1]:pos(-contentSize * 0.5, -size.height * 0.5)
      for i = 2, self._itemCount do
        local prevSize = size
        size = self._items[i]:getContentSize()
        self._items[i]:pos(self._items[i - 1]:getPositionX() + prevSize.width, -size.height * 0.5)
      end
    end
    self._content:setContentSize(cc.size(contentSize, self._content:getCascadeBoundingBox().height))
  end
  self:update()
  return self
end
return GiftListPanel

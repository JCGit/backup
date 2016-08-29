local ScrollPanel = import(".ScrollPanel")
local ListPanel = class("ListPanel", ScrollPanel)
function ListPanel:ctor(params, itemClass)
  ListPanel.super.ctor(self, params)
  self:setCascadeOpacityEnabled(true)
  self.noDataHint = g.lang:getContent("common.noDataHint")
  self._content = display.newNode()
  self._content:setCascadeOpacityEnabled(true)
  self:setScrollContent(self._content)
  self:setItemClass(itemClass)
end
function ListPanel:onScrolling()
  if self._items and self._visibleRectOriginPoint then
    if self._direction == ScrollPanel.DIRECTION_VERTICAL then
      for k, item in ipairs(self._items) do
        local tempWorldPoint = self._content:convertToWorldSpace(ccp(item:getPosition()))
        if tempWorldPoint.y > self._visibleRectOriginPoint.y + self._visibleRect.height or tempWorldPoint.y < self._visibleRectOriginPoint.y - item.height then
          item:hide()
          if item.onItemInactive then
            item:onItemInactive()
          end
        else
          item:show()
          if item.lazyInitialize then
            item:lazyInitialize()
          end
        end
      end
    elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
      for k, item in ipairs(self._items) do
        local tempWorldPoint = self._content:convertToWorldSpace(ccp(item:getPosition()))
        if tempWorldPoint.x > self._visibleRectOriginPoint.x + self._visibleRect.width or tempWorldPoint.x < self._visibleRectOriginPoint.x - item.width then
          item:hide()
          if item.onItemInactive then
            item:onItemInactive()
          end
        else
          item:show()
          if item.lazyInitialize then
            item:lazyInitialize()
          end
        end
      end
    end
  end
end
function ListPanel:setData(data)
  self._data = data
  local prevItemCount = self._itemCount or 0
  self._itemCount = self._data and table.maxn(self._data) or 0
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
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
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
      self._items[i]:setData(self._data[i])
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
    self._content:setContentSize(cc.size(self._content:getCascadeBoundingBox().size.width, contentSize))
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    for i = 1, self._itemCount do
      if not self._items[i] then
        self._items[i] = self._itemClass.new():addTo(self._content)
      end
      self._items[i]:show()
      self._items[i]:setIndex(i)
      self._items[i]:setData(self._data[i])
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
  if self._itemCount == 0 then
    self:showNoDataHintView()
  else
    self:hideNoDataHintView()
  end
  self:update()
  return self
end
function ListPanel:showMiniLoading()
  self:hideNoDataHintView()
  ListPanel.super.showMiniLoading(self)
end
function ListPanel:hideMiniLoading()
  ListPanel.super.hideMiniLoading(self)
end
function ListPanel:setNoDataHintText(str)
  self.noDataHint = str
end
function ListPanel:showNoDataHintView(noDataHint, fontSize)
  fontSize = fontSize or 24
  if self._noDataLabel == nil then
    self._noDataLabel = ui.newTTFLabel({
      text = "",
      size = fontSize,
      color = ccc3(139, 184, 220),
      align = ui.TEXT_ALIGN_CENTER
    }):addTo(self)
    self._noDataLabel:setCascadeOpacityEnabled(true)
  end
  self._noDataLabel:setFontSize(fontSize)
  if noDataHint then
    self._noDataLabel:setString(noDataHint)
  else
    self._noDataLabel:setString(self.noDataHint)
  end
  self._noDataLabel:show()
  self._noDataLabel:setOpacity(self:getOpacity())
end
function ListPanel:hideNoDataHintView()
  if self._noDataLabel ~= nil then
    self._noDataLabel:hide()
  end
end
function ListPanel:getData()
  return self._data
end
function ListPanel:_onItemResize()
  local curPlace = self._currentPlace
  local contentSize = 0
  if self._direction == ScrollPanel.DIRECTION_VERTICAL then
    for i = 1, self._itemCount do
      contentSize = contentSize + self._items[i]:getContentSize().height
    end
    local size = self._items[1]:getContentSize()
    self._items[1]:pos(-size.width * 0.5, contentSize * 0.5 - size.height)
    local pX, pY = -size.width * 0.5, contentSize * 0.5 - size.height
    for i = 2, self._itemCount do
      size = self._items[i]:getContentSize()
      pY = pY - size.height
      self._items[i]:pos(pX, pY)
    end
    self._content:setContentSize(cc.size(self._content:getCascadeBoundingBox().width, contentSize))
  elseif self._direction == ScrollPanel.DIRECTION_HORIZONTAL then
    for i = 1, self._itemCount do
      contentSize = contentSize + self._items[i]:getContentSize().width
    end
    self._content:setContentSize(cc.size(contentSize, self._content:getContentSize().height))
    local size = self._items[1]:getContentSize()
    self._items[1]:pos(-contentSize * 0.5, -size.height * 0.5)
    local pX, pY = -contentSize * 0.5, -size.height * 0.5
    for i = 2, self._itemCount do
      size = self._items[i]:getContentSize()
      pX = pX + size.width
      self._items[i]:pos(pX, pY)
    end
    self._content:setContentSize(cc.size(contentSize, self._content:getCascadeBoundingBox().height))
  end
  self:update()
  self:scrollTo(curPlace)
  return self
end
function ListPanel:_onItemEvent(evt)
  self:dispatchEvent(evt)
end
function ListPanel:getListItem(index)
  if self._items then
    return self._items[index]
  end
end
function ListPanel:setItemClass(class)
  self._itemClass = class
  return self
end
return ListPanel

local Component = import("..base.Component")
local ListItem = class("ListItem", Component)
function ListItem:ctor(width, height)
  ListItem.super.ctor(self)
  self:setContentSize(CCSize(width, height))
  self.width = width
  self.height = height
end
function ListItem:setData(data)
  local isModyfied = self._data ~= data
  self._data = data
  if self.onDataSet then
    self:onDataSet(isModyfied, data)
  end
  return self
end
function ListItem:getData()
  return self._data
end
function ListItem:setIndex(index)
  self._index = index
  return self
end
function ListItem:getIndex()
  return self._index
end
function ListItem:setOwner(owner)
  self._owner = owner
  return self
end
function ListItem:getOwner()
  return self._owner
end
return ListItem

local CheckButtonGroup = class("CheckButtonGroup")
CheckButtonGroup.BUTTON_SELECT_CHANGE = "BUTTON_SELECT_CHANGE"
function CheckButtonGroup:ctor()
  cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
  self._buttons = {}
  self._buttonIds = {}
  self._currentSelectedIndex = 0
end
function CheckButtonGroup:addButton(button, id)
  self._buttons[#self._buttons + 1] = button
  if id then
    self._buttonIds[#self._buttonIds + 1] = id
  end
  button:onButtonClicked(clickHandler(self, self._onButtonStateChange))
  button:onButtonStateChanged(handler(self, self._onButtonStateChange))
  return self
end
function CheckButtonGroup:getButtonById(id)
  for i, v in pairs(self._buttonIds) do
    if tostring(v) == tostring(id) then
      return self._buttons[i]
    end
  end
end
function CheckButtonGroup:reset()
  self:removeAllEventListeners()
  while self:getButtonsCount() > 0 do
    self:removeButtonAtIndex(self:getButtonsCount())
  end
end
function CheckButtonGroup:removeButtonAtIndex(index)
  assert(self._buttons[index] ~= nil, "CheckButtonGroup:removeButtonAtIndex() - invalid index")
  local button = self._buttons[index]
  button:removeSelf()
  table.remove(self._buttons, index)
  table.remove(self._buttonIds, index)
  if self._currentSelectedIndex == index then
    self:_updateButtonStatus(nil)
  elseif index < self._currentSelectedIndex then
    self:_updateButtonStatus(self._buttons[self._currentSelectedIndex - 1])
  end
  return self
end
function CheckButtonGroup:getButtonAtIndex(index)
  return self._buttons[index]
end
function CheckButtonGroup:getButtonsCount()
  return #self._buttons
end
function CheckButtonGroup:addButtonSelectChangedEventListener(callback)
  return self:addEventListener(CheckButtonGroup.BUTTON_SELECT_CHANGE, callback)
end
function CheckButtonGroup:onButtonSelectChanged(callback)
  self:addButtonSelectChangedEventListener(callback)
  return self
end
function CheckButtonGroup:_onButtonStateChange(event)
  if event.name == cc.ui.UICheckBoxButton.STATE_CHANGED_EVENT and event.target:isButtonSelected() == false then
    return
  end
  self:_updateButtonStatus(event.target)
end
function CheckButtonGroup:_updateButtonStatus(clickedButton)
  local currentSelectedIndex = 0
  for index, button in ipairs(self._buttons) do
    if button == clickedButton then
      currentSelectedIndex = index
      if not button:isButtonSelected() then
        button:setButtonSelected(true)
      end
    elseif button:isButtonSelected() then
      button:setButtonSelected(false)
    end
  end
  if self._currentSelectedIndex ~= currentSelectedIndex then
    local last = self._currentSelectedIndex
    self._currentSelectedIndex = currentSelectedIndex
    self:dispatchEvent({
      name = CheckButtonGroup.BUTTON_SELECT_CHANGE,
      selected = currentSelectedIndex,
      last = last
    })
  end
end
return CheckButtonGroup

local ActionsManager = class("ActionsManager")
local ActionObj = import(".ActionObj")
function ActionsManager:ctor()
  self.actionObjs = {}
  self._actionId = 0
end
function ActionsManager:addSingleAction(actionOwner, action, actionId)
  local actionObj = self:_getActionObj(actionId)
  if actionObj == nil then
    actionObj = ActionObj.new()
    local actionId = self:_increaseActionId()
    actionObj:setActionId(actionId)
    actionObj:setActionOwner(actionOwner)
    table.insert(self.actionObjs, actionObj)
  end
  local actionsBatch = cc.Array:create()
  local preActions = actionObj:getArrayOfActions()
  if preActions ~= nil then
    for i = 1, preActions:count() do
      actionsBatch:addObject(preActions:objectAtIndex(i - 1))
    end
  end
  if action ~= nil then
    actionsBatch:addObject(action)
  end
  actionObj:setArrayOfActions(actionsBatch)
  return actionObj:getActionId()
end
function ActionsManager:addBatchActions(actionOwner, actions, actionId)
  local actionObj = self:_getActionObj(actionId)
  if actionObj == nil then
    actionObj = ActionObj.new()
    local actionId = self:_increaseActionId()
    actionObj:setActionId(actionId)
    actionObj:setActionOwner(actionOwner)
    table.insert(self.actionObjs, actionObj)
  end
  local actionsBatch = cc.Array:create()
  local preActions = actionObj:getArrayOfActions()
  if preActions ~= nil then
    for i = 1, preActions:count() do
      actionsBatch:addObject(preActions:objectAtIndex(i - 1))
    end
  end
  for i = 1, actions:count() do
    actionsBatch:addObject(actions:objectAtIndex(i - 1))
  end
  actionObj:setArrayOfActions(actionsBatch)
  return actionObj:getActionId()
end
function ActionsManager:runActionByActionId(actionId)
  local actionObj = self:_getActionObj(actionId)
  if actionObj == nil then
    return
  end
  local actionsBatch = cc.Array:create()
  local preActions = actionObj:getArrayOfActions()
  if preActions ~= nil then
    for i = 1, preActions:count() do
      actionsBatch:addObject(preActions:objectAtIndex(i - 1))
    end
  end
  local batchActionsCallback = CCCallFunc:create(function()
    local completeCallback = actionObj:getCompleteCallback()
    if completeCallback ~= nil then
      completeCallback(callbackOwner)
    end
    self:_removeActonObjectByActionId(actionId)
  end)
  actionsBatch:addObject(batchActionsCallback)
  local actionSequence = cc.Sequence:create(actionsBatch)
  local actionOwner = actionObj:getActionOwner()
  local action = actionOwner:runAction(actionSequence)
  actionObj:setAction(action)
end
function ActionsManager:runBatchActions(actionOwner, actions, callbackOwner, completeCallback, stopCallback)
  local actionObj = ActionObj.new()
  local actionId = self:_increaseActionId()
  actionObj:setActionId(actionId)
  actionObj:setActionOwner(actionOwner)
  actionObj:setCallbackOwner(callbackOwner)
  actionObj:setCompleteCallback(completeCallback)
  actionObj:setStopCallback(stopCallback)
  table.insert(self.actionObjs, actionObj)
  local actionsBatch = cc.Array:create()
  for i = 1, actions:count() do
    actionsBatch:addObject(actions:objectAtIndex(i - 1))
  end
  local batchActionsCallback = CCCallFunc:create(function()
    local completeCallback = actionObj:getCompleteCallback()
    if completeCallback ~= nil then
      completeCallback(callbackOwner)
    end
    self:_removeActonObjectByActionId(actionId)
  end)
  actionsBatch:addObject(batchActionsCallback)
  local actionSequence = cc.Sequence:create(actionsBatch)
  local action = actionOwner:runAction(actionSequence)
  actionObj:setAction(action)
  return actionId
end
function ActionsManager:cancelActionObjectByActionId(actionId)
  print("cancelActonObjectByActionId~~~~~~~~~~")
  print(actionId)
  for i, actionObj in ipairs(self.actionObjs) do
    print(actionObj:getActionId())
    if actionObj:getActionId() == actionId then
      local callbackOwner = actionObj:getCallbackOwner()
      local stopCallback = actionObj:getStopCallback()
      local action = actionObj:getAction()
      local actionOwner = actionObj:getActionOwner()
      print("成功取消掉动画.............")
      actionOwner:stopAction(action)
      table.remove(self.actionObjs, i)
      return
    end
  end
end
function ActionsManager:stopActionObjectByActionId(actionId)
  for i, actionObj in ipairs(self.actionObjs) do
    if actionObj:getActionId() == actionId then
      local callbackOwner = actionObj:getCallbackOwner()
      local stopCallback = actionObj:getStopCallback()
      local action = actionObj:getAction()
      local actionOwner = actionObj:getActionOwner()
      actionOwner:stopAction(action)
      if stopCallback ~= nil then
        stopCallback(callbackOwner)
      end
      table.remove(self.actionObjs, i)
      return
    end
  end
end
function ActionsManager:stopAllActionObject()
  local actionIds = {}
  for i, actionObj in ipairs(self.actionObjs) do
    table.insert(actionIds, actionObj:getActionId())
  end
  for i, actionId in ipairs(actionIds) do
    self:stopActionObjectByActionId(actionId)
  end
end
function ActionsManager:cancelAllActionObject()
  local actionIds = {}
  for i, actionObj in ipairs(self.actionObjs) do
    table.insert(actionIds, actionObj:getActionId())
  end
  for i, actionId in ipairs(actionIds) do
    self:cancelActionObjectByActionId(actionId)
  end
end
function ActionsManager:_removeActonObjectByActionId(actionId)
  for i, actionObj in ipairs(self.actionObjs) do
    if actionObj:getActionId() == actionId then
      table.remove(self.actionObjs, i)
      return
    end
  end
end
function ActionsManager:_increaseActionId()
  self._actionId = self._actionId + 1
  return self._actionId
end
function ActionsManager:_getActionObj(actionId)
  for i, actionObj in ipairs(self.actionObjs) do
    if actionObj:getActionId() == actionId then
      return actionObj
    end
  end
end
return ActionsManager

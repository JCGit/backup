local ActionObj = class("ActionObj")
function ActionObj:ctor(action, actionOwner, callbackOwner, completeCallback, stopCallback)
  self.actionId = -1
  self.action = action
  self.actionOwner = actionOwner
  self.callbackOwner = callbackOwner
  self.completeCallback = completeCallback
  self.stopCallback = stopCallback
  self.arrayOfActions = nil
end
function ActionObj:setActionId(actionId)
  self.actionId = actionId
end
function ActionObj:getActionId()
  return self.actionId
end
function ActionObj:setAction(action)
  self.action = action
end
function ActionObj:getAction()
  return self.action
end
function ActionObj:setActionOwner(actionOwner)
  self.actionOwner = actionOwner
end
function ActionObj:getActionOwner()
  return self.actionOwner
end
function ActionObj:setCallbackOwner(callbackOwner)
  self.callbackOwner = callbackOwner
end
function ActionObj:getCallbackOwner()
  return self.callbackOwner
end
function ActionObj:setCompleteCallback(completeCallback)
  self.completeCallback = completeCallback
end
function ActionObj:getCompleteCallback()
  return self.completeCallback
end
function ActionObj:setStopCallback(stopCallback)
  self.stopCallback = stopCallback
end
function ActionObj:getStopCallback()
  return self.stopCallback
end
function ActionObj:setArrayOfActions(arrayOfActions)
  if self.arrayOfActions ~= nil then
    self.arrayOfActions:release()
  end
  self.arrayOfActions = arrayOfActions
  self.arrayOfActions:retain()
end
function ActionObj:getArrayOfActions()
  return self.arrayOfActions
end
return ActionObj

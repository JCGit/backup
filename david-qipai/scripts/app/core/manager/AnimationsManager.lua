local AnimationsManager = class("AnimationsManager")
function AnimationsManager:ctor()
  self.actionsOwner = {}
  self.actions = {}
end
function AnimationsManager:addActions(actionOwner, action)
  actionOwner:retain()
  action:retain()
  table.insert(self.actionsOwner, actionOwner)
  table.insert(self.actions, action)
end
function AnimationsManager:cancelAllAnimations()
  for i, actionOwner in ipairs(self.actionsOwner) do
    local action = self.actions[i]
    if action ~= nil then
      if not action:isDone() then
        print("AnimationsManager:cancel One Action")
        actionOwner:stopAction(action)
      end
      action:release()
    else
      actionOwner:stopAllActions()
    end
    actionOwner:release()
  end
  self.actionsOwner = {}
  self.actions = {}
end
return AnimationsManager

DCTask = {}
function DCTask.begin(taskId, taskType)
  DCLuaTask:begin(taskId, taskType)
end
function DCTask.complete(taskId)
  DCLuaTask:complete(taskId)
end
function DCTask.fail(taskId, reason)
  DCLuaTask:fail(taskId, reason)
end
return DCTask

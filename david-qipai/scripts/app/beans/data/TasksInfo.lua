local TasksInfo = class("TasksInfo")
function TasksInfo:ctor()
  self._tasksInfoCacheData = {}
end
function TasksInfo:refreshData(successCallback, failCallback)
  if not self._isTasksRequesting then
    self._isTasksRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    local requestID = g.httpRequest:postMethod(g.requestParamsFactory:createRequest("tasks", "list", requestData), function(data)
      logger:debug(data)
      self._isTasksRequesting = false
      local retData = json.decode(data)
      if retData ~= nil then
        self._tasksInfoCacheData = retData.info
      end
      if successCallback ~= nil then
        successCallback(self._tasksInfoCacheData)
      end
    end, function()
      self._isTasksRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
    return requestID
  end
end
function TasksInfo:reportTaskForPlayCard()
  if self._tasksInfoCacheData ~= nil then
    for i, task in ipairs(self._tasksInfoCacheData) do
      local taskId = tonumber(task.taskId)
      if taskId >= 10001 and taskId <= 10003 and tonumber(task.status) == 0 then
        self:_reportTask(taskId)
        return
      end
    end
  end
end
function TasksInfo:reportTaskForProPlayCard()
  if self._tasksInfoCacheData ~= nil then
    for i, task in ipairs(self._tasksInfoCacheData) do
      local taskId = tonumber(task.taskId)
      if taskId >= 10101 and taskId <= 10103 and tonumber(task.status) == 0 then
        self:_reportTask(taskId)
        return
      end
    end
  end
end
function TasksInfo:reportTaskForWin()
  if self._tasksInfoCacheData ~= nil then
    for i, task in ipairs(self._tasksInfoCacheData) do
      local taskId = tonumber(task.taskId)
      if taskId >= 10201 and taskId <= 10203 and tonumber(task.status) == 0 then
        self:_reportTask(taskId)
        return
      end
    end
  end
end
function TasksInfo:reportTaskForPok()
  if self._tasksInfoCacheData ~= nil then
    for i, task in ipairs(self._tasksInfoCacheData) do
      local taskId = tonumber(task.taskId)
      if taskId >= 10301 and taskId <= 10302 and tonumber(task.status) == 0 then
        print(taskId)
        self:_reportTask(taskId)
        return
      end
    end
  end
end
function TasksInfo:reportTaskForAddFriend()
  if self._tasksInfoCacheData ~= nil then
    for i, task in ipairs(self._tasksInfoCacheData) do
      local taskId = tonumber(task.taskId)
      if taskId >= 10401 and taskId <= 10404 and tonumber(task.status) == 0 then
        self:_reportTask(taskId)
        return
      end
    end
  end
end
function TasksInfo:reportTaskForMagicEmoji()
  if self._tasksInfoCacheData ~= nil then
    for i, task in ipairs(self._tasksInfoCacheData) do
      local taskId = tonumber(task.taskId)
      if taskId >= 10501 and taskId <= 10502 and tonumber(task.status) == 0 then
        self:_reportTask(taskId)
        return
      end
    end
  end
end
function TasksInfo:reportTaskForEmoji()
  if self._tasksInfoCacheData ~= nil then
    for i, task in ipairs(self._tasksInfoCacheData) do
      local taskId = tonumber(task.taskId)
      if taskId >= 10601 and taskId <= 10602 and tonumber(task.status) == 0 then
        self:_reportTask(taskId)
        return
      end
    end
  end
end
function TasksInfo:_reportTask(taskId, successCallback, failCallback)
  print("reportTask :" .. taskId)
  if not self._isTasksRequesting then
    self._isTasksRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.taskId = taskId
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("tasks", "report", requestData), function(data)
      self._isTasksRequesting = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        if successCallback ~= nil then
          successCallback(retData.info)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isTasksRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
function TasksInfo:getReward(taskId, successCallback, failCallback)
  if not self._isTasksRequesting then
    self._isTasksRequesting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.taskId = taskId
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("tasks", "reward", requestData), function(data)
      logger:debug(data)
      self._isTasksRequesting = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        if successCallback ~= nil then
          successCallback(retData.info)
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isTasksRequesting = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
return TasksInfo

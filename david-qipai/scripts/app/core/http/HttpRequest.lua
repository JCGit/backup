local HttpRequest = class("HttpRequest")
function HttpRequest:ctor()
  self.requests = {}
  self.curRequestKey = 1
end
function _kernelRequest(requestUrl, postParams, isPost, onRequestFinished)
  local request
  if isPost then
    request = network.createHTTPRequest(onRequestFinished, requestUrl, "POST")
    if postParams ~= nil then
      for k, v in pairs(postParams) do
        request:addPOSTValue(k, v)
      end
    end
  else
    request = network.createHTTPRequest(onRequestFinished, requestUrl, "GET")
  end
  request:start()
  return request
end
function HttpRequest:requestMethod(requestParams, isPost, doneCallback, errorCallback, inProgressCallback)
  local url = requestParams.fullURL
  if url == nil then
    url = requestParams.baseUrl
    if url ~= nil then
      url = string.gsub(url, "(.*)/$", "%1")
    end
    local urlPrefix
    if requestParams.urlPrefix ~= nil then
      urlPrefix = string.gsub(requestParams.urlPrefix, "^/(.*)", "%1")
    end
    if url ~= nil and urlPrefix ~= nil then
      url = string.format("%s/%s", url, urlPrefix)
    end
  end
  if not isPost and requestParams.getParams ~= nil then
    local queryParams
    for k, v in pairs(requestParams.getParams) do
      if queryParams == nil then
        queryParams = string.format("%s=%s", k, v)
      else
        queryParams = string.format("%s&%s=%s", queryParams, k, v)
      end
    end
    if url ~= nil then
      queryParams = queryParams or ""
      url = string.format("%s?%s", url, queryParams)
    end
  end
  local phpSendTime = g.util.functions.getTime()
  g.dataLogger:logPHPApiCount(requestParams.dataAnalysisApiKey)
  function onRequestFinished(event)
    local request = event.request
    if event.name == "completed" then
      local phpResonposeTime = g.util.functions.getTime()
      local duration = phpResonposeTime - phpSendTime
      g.dataLogger:logPHPApiTime(requestParams.dataAnalysisApiKey, duration)
      local code = request:getResponseStatusCode()
      if code ~= 200 then
        errorCallback(code, "Completed")
        return
      else
        local response = request:getResponseData()
        local responseLength = request:getResponseDataLength()
        if doneCallback then
          doneCallback(response, responseLength)
        end
        return
      end
    elseif event.name == "inprogress" then
      if inProgressCallback then
        inProgressCallback()
      end
    else
      print(request:getErrorCode(), request:getErrorMessage())
      if errorCallback then
        errorCallback(request:getErrorCode(), request:getErrorMessage())
      end
      return
    end
  end
  local currentRequestId = self.curRequestKey + 1
  self.curRequestKey = currentRequestId
  local request = _kernelRequest(url, requestParams.postParams, isPost, onRequestFinished)
  return request
end
local checkDirOK = function(path)
  require("lfs")
  local oldpath = lfs.currentdir()
  CCLuaLog("old path------> " .. oldpath)
  if lfs.chdir(path) then
    lfs.chdir(oldpath)
    CCLuaLog("path check OK------> " .. path)
    return true
  end
  if lfs.mkdir(path) then
    CCLuaLog("path create OK------> " .. path)
    return true
  end
end
function HttpRequest:getMethod(params, doneCallback, errorCallback)
  return self:requestMethod(params, false, doneCallback, errorCallback, nil)
end
function HttpRequest:postMethod(params, doneCallback, errorCallback)
  return self:requestMethod(params, true, doneCallback, errorCallback, nil)
end
function HttpRequest:downloadMethod(params, downloadDir, fileName, doneCallback, errorCallback, inProgressCallback)
  function downloadSuccessCallback(responseData, responseLength)
    if not checkDirOK(downloadDir) then
      errorCallback()
      return
    else
      local filePath
      if downloadDir ~= nil then
        downloadDir = string.gsub(downloadDir, "(.*)/$", "%1")
      end
      if fileName then
        filePath = downloadDir .. "/" .. fileName
      end
      if filePath ~= nil and responseData ~= nil then
        io.writefile(filePath, responseData)
      end
      if doneCallback ~= nil then
        doneCallback(responseData, responseLength)
      end
    end
  end
  return self:requestMethod(params, false, downloadSuccessCallback, errorCallback, inProgressCallback)
end
function HttpRequest:cancelMethod(requestID)
  if requestID ~= nil then
    local request = self.requests[requestID]
    if request ~= nil then
      request:cancel()
      self.requests[requestID] = nil
      request:release()
    end
  end
end
return HttpRequest

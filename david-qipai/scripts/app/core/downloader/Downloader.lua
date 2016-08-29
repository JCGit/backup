local httpRequest = g.httpRequest
local Downloader = class("Downloader")
function Downloader:ctor()
  self.batchArray = {}
  self.isDownloading = false
  function batchJobsSuccessCallback()
    for i = 1, table.getn(self.batchArray) do
      v = self.batchArray[i]
      if v ~= nil then
        if not self.isDownloading then
          self.isDownloading = true
          self:_exeBatchJobs(v)
        end
        break
      end
    end
  end
  function batchJobsFailedCallback()
    for i = 1, table.getn(self.batchArray) do
      v = self.batchArray[i]
      if v ~= nil then
        if not self.isDownloading then
          self.isDownloading = true
          self:_exeBatchJobs(v)
        end
        break
      end
    end
  end
  self._singleJobSuccessCallback = nil
  self._singleJobFailedCallback = nil
  self._batchJobsSuccessCallback = batchJobsSuccessCallback
  self._batchJobsFailedCallback = batchJobsFailedCallback
end
function Downloader:startBatchJobs(batchJobs)
  print("------在batchArray后面插入新的 batchJobs(批任务)---")
  table.insert(self.batchArray, batchJobs)
  if not self.isDownloading then
    self.isDownloading = true
    local exeBatchJobs
    for i = 1, table.getn(self.batchArray) do
      v = self.batchArray[i]
      if v ~= nil then
        exeBatchJobs = v
        break
      end
    end
    if not exeBatchJobs.isSupportResumeTransfer then
    end
    self:_exeBatchJobs(exeBatchJobs)
  end
end
function Downloader:_exeBatchJobs(exeBatchJobs)
  local downloadSingleTask
  for i, v in ipairs(exeBatchJobs.downloadTasks) do
    if not v.isDownloaded then
      print("-----------current exe-------------------")
      print(v.fileName)
      print("-----------current exe-------------------")
      downloadSingleTask = v
      break
    end
  end
  if downloadSingleTask ~= nil then
    function doneCallback(responseData, responseLength)
      if self._singleJobSuccessCallback ~= nil then
        self._singleJobSuccessCallback(exeBatchJobs, downloadSingleTask, responseData, responseLength)
      end
      downloadSingleTask.isDownloaded = true
      self:_exeBatchJobs(exeBatchJobs)
    end
    function errorCallback(errorCode, errorMsg)
      if self._singleJobFailedCallback ~= nil then
        self._singleJobFailedCallback(exeBatchJobs, downloadSingleTask, errorCode, errorMsg)
      end
      downloadSingleTask.isDownloaded = false
      if exeBatchJobs._alreadyRetryCount == nil then
        exeBatchJobs._alreadyRetryCount = 0
      end
      exeBatchJobs._alreadyRetryCount = exeBatchJobs._alreadyRetryCount + 1
      if exeBatchJobs._alreadyRetryCount < exeBatchJobs.retryCount then
        self:_exeBatchJobs(exeBatchJobs)
      else
        self.isDownloading = false
        for i = 1, table.getn(self.batchArray) do
          v = self.batchArray[i]
          if v == exeBatchJobs then
            table.remove(self.batchArray, i)
            break
          end
        end
        self:_batchJobsFailedCallback()
      end
    end
    self:_exeDetailJob(downloadSingleTask, doneCallback, errorCallback)
  else
    self.isDownloading = false
    for i = 1, table.getn(self.batchArray) do
      v = self.batchArray[i]
      if v == exeBatchJobs then
        table.remove(self.batchArray, i)
        break
      end
    end
    self._batchJobsSuccessCallback()
  end
end
function Downloader:_exeDetailJob(downloadSingleTask, doneCallback, errorCallback)
  local params = {
    fullURL = downloadSingleTask.downloadUrl
  }
  local fileName = downloadSingleTask.fileName
  local downloadDir = device.writablePath .. "upd/"
  local requestID = httpRequest:downloadMethod(params, downloadDir, fileName, doneCallback, errorCallback)
end
return Downloader

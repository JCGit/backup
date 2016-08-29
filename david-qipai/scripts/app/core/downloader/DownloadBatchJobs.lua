local DownloadBatchJobs = class("DownloadBatchJobs")
function DownloadBatchJobs:ctor(params)
  self.downloadDir = params.downloadDir
  self.downloadTasks = params.downloadTasks
  self.isSupportResumeTransfer = true
  self.retryCount = 3
  self._alreadyRetryCount = 0
end
return DownloadBatchJobs

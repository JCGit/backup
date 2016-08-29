local DownloadSingleTask = class("DownloadSingleTask")
function DownloadSingleTask:ctor(params)
  self.downloadUrl = params.downloadUrl
  self.isDownloaded = params.isDownloaded
  self.downloadDir = params.downloadDir
  self.fileName = params.fileName
end
return DownloadSingleTask

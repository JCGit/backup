require("config")
require("framework.init")
require("app.init")
local downloader = g.downloader
local httpRequest = g.httpRequest
local imageManager = g.imageManager
local TestDrive = class("TestDrive", cc.mvc.AppBase)
function TestDrive:ctor()
  TestDrive.super.ctor(self)
end
function TestDrive:testImage()
end
function TestDrive:testNetModule()
  local updateManager = require("app.core.updater.UpdateManager").new()
  function updateFinishedCallback()
    print("------update finished---------")
  end
  function updateProgressCallback(updatedSize, totalUpdateSize)
    print("start----update progress-----")
    print(updatedSize, totalUpdateSize)
    print("end----update progress-----")
  end
  updateManager.updateFinishedCallback = updateFinishedCallback
  updateManager.updateProgressCallback = updateProgressCallback
  updateManager:startUp()
  do return end
  print("[App  TestDrive] testing Network")
  function doneCallback(responseData, responseLength)
    print("[App  TestDrive] successCallback")
    print(responseLength)
  end
  function errorCallback(errorCode, errorMsg)
    print("[App  TestDrive] failedCallback")
    print(errorCode)
    print(errorMsg)
  end
  function inProgressCallback()
    print("[App  TestDrive] inProgressCallback")
  end
  local params = require("app.core.http.RequestParams").new({
    baseUrl = "http://www.baidu.com/",
    urlPrefix = "s",
    getParams = {wd = "d"}
  })
  local downloadDir = device.writablePath .. "upd/"
  g.DebugTool.printMyLog(httpRequest.curRequestKey)
  local requestID = httpRequest:downloadMethod(params, downloadDir, "download.html", doneCallback, errorCallback, nil)
  do return end
  local downloadBatchJobs1 = {
    downloadTasks = {
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "finename1"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "finename2"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "finename3"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "finename4"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "finename5"
      }
    },
    isSupportResumeTransfer = true,
    retryCount = 3
  }
  local downloadBatchJobs2 = {
    downloadTasks = {
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "t_finename1"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "t_finename2"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "t_finename3"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "t_finename4"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "t_finename5"
      }
    },
    isSupportResumeTransfer = true,
    retryCount = 3
  }
  local downloadBatchJobs3 = {
    downloadTasks = {
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "tf_finename1"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "tf_finename2"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "tf_finename3"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "tf_finename4"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "tf_finename5"
      }
    },
    isSupportResumeTransfer = true,
    retryCount = 3
  }
  local downloadBatchJobs4 = {
    downloadTasks = {
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "4t_finename1"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "4t_finename2"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "4t_finename3"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "4t_finename4"
      },
      {
        downloadUrl = "http://d147wns3pm1voh.cloudfront.net/static/nineke/nineke/hotFix/flist",
        fileName = "4t_finename5"
      }
    },
    isSupportResumeTransfer = true,
    retryCount = 3
  }
  downloader:startBatchJobs(downloadBatchJobs1)
  downloader:startBatchJobs(downloadBatchJobs2)
  downloader:startBatchJobs(downloadBatchJobs3)
  downloader:startBatchJobs(downloadBatchJobs4)
end
function showLeaderBoard()
  if device.platform == "ios" then
    local args = {platform = platform}
    local ok, ret = luaoc.callStaticMethod("AppController", "showLeaderBoard", nil)
    if ok then
      print("getIsValid,ret:", ret)
      return ret
    else
      print("shareEvent error code = ", ret)
      return nil
    end
  end
end
function TestDrive:testgamekit()
  print("gamekit testing")
  showLeaderBoard()
end
return TestDrive

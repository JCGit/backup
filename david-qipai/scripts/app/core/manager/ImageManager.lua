local httpRequest = g.httpRequest
local fileManager = g.fileManager
local ImageManager = class("ImageManager")
ImageManager.DEFAULT_TMP_DIR = "cache" .. device.directorySeparator .. "imageCache"
function ImageManager:ctor()
  fileManager:rmdir(ImageManager.DEFAULT_TMP_DIR)
  fileManager:mkdir(ImageManager.DEFAULT_TMP_DIR)
  self._fetchingList = {}
end
function ImageManager:fetchImage(url, successCallback, failedCallback)
  if string.sub(url, 1, 4) ~= "http" then
    logger:debug("URL illegal, url = " .. url)
    return
  end
  local downloadDir = device.writablePath .. ImageManager.DEFAULT_TMP_DIR
  local hashName = crypto.md5(url)
  if self._fetchingList[hashName] ~= nil then
    return
  end
  local relativeFilePath = ImageManager.DEFAULT_TMP_DIR .. device.directorySeparator .. hashName
  local absoluteFilePath = downloadDir .. device.directorySeparator .. hashName
  function doneCallback(responseData, responseLength)
    lfs.touch(absoluteFilePath)
    self._fetchingList[hashName] = nil
    local texture = CCTextureCache:sharedTextureCache():addImage(absoluteFilePath)
    if texture ~= nil then
      if successCallback ~= nil then
        successCallback(CCSprite:createWithTexture(texture))
      end
    elseif failedCallback then
      failedCallback()
    end
  end
  function failCallback()
    self._fetchingList[hashName] = nil
    if failedCallback then
      failedCallback()
    end
  end
  if fileManager:exists(relativeFilePath) then
    lfs.touch(absoluteFilePath)
    local texture = CCTextureCache:sharedTextureCache():addImage(absoluteFilePath)
    if texture ~= nil then
      if successCallback ~= nil then
        successCallback(CCSprite:createWithTexture(texture))
      end
    elseif failedCallback then
      failedCallback()
    end
    return
  end
  local params = require("app.core.http.RequestParams").new({fullURL = url})
  local requestID = httpRequest:downloadMethod(params, downloadDir, hashName, doneCallback, failCallback)
  self._fetchingList[hashName] = requestID
end
function ImageManager:cancelFetchImage(url)
  local hashName = crypto.md5(url)
  local requestID = self._fetchingList[hashName]
  if requestID ~= nil then
    requestID:cancel()
    self._fetchingList[hashName] = nil
  end
end
function ImageManager:clearImageCache()
  local relativeFilePath = ImageManager.DEFAULT_TMP_DIR
  fileManager:rmdir(relativeFilePath)
end
return ImageManager

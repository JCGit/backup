CCLuaLoadChunksFromZIP("res/framework_precompiled.zip")
require("config")
require("framework.init")
local langManager = require("app.core.manager.LangManager").new()
local UpdateManager = class("UpdateManager")
local NEEDUPDATE = true
local server = "http://127.0.0.1:8080/"
local param = "?dev=" .. device.platform
local list_filename = "resourcelist"
local downList = {}
local totalUpdateSize = 0
local updatedSize = 0
local hex = function(s)
  s = string.gsub(s, "(.)", function(x)
    return string.format("%02X", string.byte(x))
  end)
  return s
end
local readFile = function(path)
  local file = io.open(path, "rb")
  if file then
    local content = file:read("*all")
    io.close(file)
    return content
  end
  return nil
end
local removeFile = function(path)
  io.writefile(path, "")
  if device.platform == "windows" then
    os.execute("del " .. string.gsub(path, "/", "\\"))
  else
    os.execute("rm " .. path)
  end
end
local function checkFile(fileName, cryptoCode)
  print("checkFile:", fileName)
  print("cryptoCode:", cryptoCode)
  if not io.exists(fileName) then
    return false
  end
  local data = readFile(fileName)
  if data == nil then
    return false
  end
  if cryptoCode == nil then
    return true
  end
  local ms = crypto.md5file(fileName)
  print("reading file :" .. fileName)
  print("file cryptoCode:", ms)
  if ms == cryptoCode then
    return true
  end
  return false
end
local checkDirOK = function(path)
  require("lfs")
  print("pwd目录设置")
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
function UpdateManager:ctor()
  self.path = device.writablePath .. "upd/"
  self.updateDidStartDownFlistCallback = nil
  self.updateFinishedCallback = nil
  self.updateProgressCallback = nil
  self.fileList = nil
  if io.exists(self.path .. list_filename) then
    self.fileList = dofile(self.path .. list_filename)
  end
  if self.fileList == nil then
    local currentVersion = g.native:getVersion()
    self.fileList = {
      ver = currentVersion,
      stage = {},
      remove = {}
    }
  end
end
function UpdateManager:updateFiles()
  local data = readFile(self.newListFile)
  io.writefile(self.curListFile, data)
  self.fileList = dofile(self.curListFile)
  if self.fileList == nil then
    local enterAppDirectory = true
    self:endProcess(enterAppDirectory)
    return
  end
  print("resourcelist已经更新,移除下载的临时resourcelist")
  print(self.newListFile)
  removeFile(self.newListFile)
  for i, v in ipairs(downList) do
    print(i, v)
    local data = readFile(v)
    local fn = string.sub(v, 1, -5)
    print("fn: ", fn)
    io.writefile(fn, data)
    removeFile(v)
  end
  local enterAppDirectory = false
  self:endProcess(enterAppDirectory)
end
function UpdateManager:reqNextFile()
  self.numFileCheck = self.numFileCheck + 1
  self.curStageFile = self.fileListNew.stage[self.numFileCheck]
  if self.curStageFile and self.curStageFile.name then
    local fn = self.path .. self.curStageFile.name
    if checkFile(fn, self.curStageFile.code) then
      updatedSize = updatedSize + self.curStageFile.size
      if self.updateProgressCallback then
        self.updateProgressCallback(updatedSize, totalUpdateSize)
      end
      self:reqNextFile()
      return
    end
    fn = fn .. ".upd"
    if checkFile(fn, self.curStageFile.code) then
      updatedSize = updatedSize + self.curStageFile.size
      table.insert(downList, fn)
      if self.updateProgressCallback then
        self.updateProgressCallback(updatedSize, totalUpdateSize)
      end
      self:reqNextFile()
      return
    end
    self:requestFromServer(self.curStageFile.name)
    return
  end
  self:updateFiles()
end
function UpdateManager:onEnterFrame(dt)
  if self.dataRecv then
    if self.requesting == list_filename then
      io.writefile(self.newListFile, self.dataRecv)
      self.dataRecv = nil
      self.dataRecvLength = nil
      self.fileListNew = dofile(self.newListFile)
      if self.fileListNew == nil then
        print("新的resourcelist是空的,open error,直接进入app吧")
        local enterAppDirectory = true
        self:endProcess(enterAppDirectory)
        return
      end
      print("新的resourcelist版本号是:")
      CCLuaLog(self.fileListNew.ver)
      if self.fileListNew.ver == self.fileList.ver then
        print("新老resourcelist一模一样,直接进入app吧")
        local enterAppDirectory = true
        self:endProcess(enterAppDirectory)
        return
      end
      totalUpdateSize = 0
      updatedSize = 0
      for i, v in ipairs(self.fileListNew.stage) do
        totalUpdateSize = totalUpdateSize + v.size
      end
      local totalSizeMsg
      if totalUpdateSize > 1048576 then
        totalSizeMsg = langManager:getContent("update.updateSizeMsg") .. string.format("%d MB", totalUpdateSize / 1024 / 1024)
      else
        totalSizeMsg = langManager:getContent("update.updateSizeMsg") .. string.format("%d KB", totalUpdateSize / 1024)
      end
      local netState = network.getInternetConnectionStatus()
      if netState ~= kCCNetworkStatusReachableViaWiFi then
        device.showAlert(langManager:getContent("common.NoWifi"), totalSizeMsg, {
          langManager:getContent("update.updateLater"),
          langManager:getContent("update.updateNow")
        }, function(event)
          print("click alert view :")
          logger:debug(g.util.inspect(event))
          if event.buttonIndex == 2 then
            self.numFileCheck = 0
            self.requesting = "files"
            self:reqNextFile()
          else
            print("取消热更新,直接进入app吧")
            local enterAppDirectory = true
            self:endProcess(enterAppDirectory)
          end
        end)
      else
        self.numFileCheck = 0
        self.requesting = "files"
        self:reqNextFile()
      end
      return
    end
    if self.requesting == "files" then
      local fn = self.path .. self.curStageFile.name .. ".upd"
      if string.find(fn, "/") then
        local arr = string.split(fn, "/")
        arr[#arr] = nil
        g.util.functions.mkdir(table.concat(arr, "/"))
      end
      io.writefile(fn, self.dataRecv)
      updatedSize = updatedSize + self.dataRecvLength
      if self.updateProgressCallback then
        self.updateProgressCallback(updatedSize, totalUpdateSize)
      end
      self.dataRecv = nil
      self.dataRecvLength = nil
      if checkFile(fn, self.curStageFile.code) then
        table.insert(downList, fn)
        self:reqNextFile()
      else
        local enterAppDirectory = true
        self:endProcess(enterAppDirectory)
      end
      return
    end
    return
  end
end
function UpdateManager:hotUpdate()
  if not checkDirOK(self.path) then
    local enterAppDirectory = true
    self:endProcess(enterAppDirectory)
    return
  end
  if self.updateDidStartDownFlistCallback ~= nil then
    self.updateDidStartDownFlistCallback()
  end
  self.curListFile = self.path .. list_filename
  self.fileList = nil
  if io.exists(self.curListFile) then
    self.fileList = dofile(self.curListFile)
  end
  if self.fileList == nil then
    local currentVersion = g.native:getVersion()
    self.fileList = {
      ver = currentVersion,
      stage = {},
      remove = {}
    }
  end
  print("-------------------本地resourcelist数据------------------------")
  print("self.curListFile:" .. self.curListFile)
  logger:debug(g.util.inspect(self.fileList))
  print("---------根据已有配置resourcelist进行更新,拉取线上resourcelist进行比较--------")
  self.requestCount = 0
  self.requesting = list_filename
  self.newListFile = self.curListFile .. ".upd"
  self.dataRecv = nil
  self.dataRecvLength = nil
  self:requestFromServer(self.requesting)
end
function versionCompare(installedVersion, latestVersion)
  print("installedVersion:")
  print(g.util.inspect(installedVersion))
  print("latestVersion:")
  print(g.util.inspect(latestVersion))
  if installedVersion and latestVersion then
    local installedArray = string.split(installedVersion, ".")
    local latestArray = string.split(latestVersion, ".")
    local installedVersionSize = table.getn(installedArray)
    local latestVersionSize = table.getn(latestArray)
    if installedVersionSize ~= 4 and latestVersionSize ~= 4 then
      return -1
    end
    for i = 1, 4 do
      installedArray[i] = tonumber(installedArray[i])
      latestArray[i] = tonumber(latestArray[i])
    end
    if latestArray[1] > installedArray[1] then
      return -1
    elseif latestArray[1] == installedArray[1] then
      if latestArray[2] > installedArray[2] then
        return -1
      elseif latestArray[2] == installedArray[2] then
        if latestArray[3] > installedArray[3] then
          return 1
        elseif latestArray[3] == installedArray[3] then
          if latestArray[4] > installedArray[4] then
            return 1
          else
            return 0
          end
        else
          return 0
        end
      else
        return 0
      end
    else
      return 0
    end
  end
  return 0
end
function UpdateManager:updateRouter(latestVersionNum, installedVersionNum, updateTitle, updateMessage, downloadUrl, forceUpdate)
  local updateState = versionCompare(installedVersionNum, latestVersionNum)
  if updateState == 1 then
    print("update state :" .. "需要进行热更新")
  elseif updateState == -1 then
    print("update state :" .. "需要进行大版本更新")
  else
    print("update state :" .. "已经是最新版本,无需更新")
  end
  if updateState == 1 and not forceUpdate then
    self:hotUpdate()
  elseif updateState == -1 or forceUpdate then
    updateTitle = updateTitle or langManager:getContent("update.updateTitle")
    updateMessage = updateMessage or langManager:getContent("update.updateMessage")
    downloadUrl = downloadUrl or langManager:getContent("update.updateUrl")
    if forceUpdate then
      device.showAlert(updateTitle, updateMessage, {
        langManager:getContent("update.updateNow")
      }, function(event)
        device.openURL(downloadUrl)
        os.exit()
      end)
    else
      device.showAlert(updateTitle, updateMessage, {
        langManager:getContent("update.downloadNow"),
        langManager:getContent("update.downloadLater")
      }, function(event)
        logger:debug(g.util.inspect(event))
        if event.buttonIndex == 1 then
          device.openURL(downloadUrl)
        else
          local enterAppDirectory = true
          self:endProcess(enterAppDirectory)
        end
      end)
    end
  else
    local enterAppDirectory = true
    self:endProcess(enterAppDirectory)
  end
end
function UpdateManager:startUp()
  local localInfo, currentVersion
  if io.exists(self.path .. "resourcelist") then
    localInfo = dofile(self.path .. "resourcelist")
  end
  if localInfo == nil or localInfo.ver == nil then
    print(g.native)
    currentVersion = g.native:getVersion()
  else
    currentVersion = localInfo.ver
  end
  if launchInfo ~= nil and launchInfo.hasLoadUpdateChunck then
    NEEDUPDATE = false
  end
  if not NEEDUPDATE then
    local enterAppDirectory = true
    self:endProcess(enterAppDirectory)
    return
  end
  local requestData = {}
  requestData.device = device.platform
  requestData.version = currentVersion
  print("当前版本:" .. currentVersion)
  local latestVersionInfoUrl = g.requestParamsFactory:assembleRequestURL("update", "get")
  g.httpRequest:getMethod(g.requestParamsFactory:createGETRequest(latestVersionInfoUrl, requestData, "update/get"), function(data)
    print("服务器最新版本信息update/get")
    logger:debug(data)
    local retData = json.decode(data)
    launchInfo = launchInfo or {}
    if retData ~= nil and retData.ret == 0 and retData.info ~= nil then
      launchInfo.ret = retData.ret
      local forceUpdate = false
      if tonumber(retData.info.forceUpdate or 0) == 1 then
        forceUpdate = true
      end
      self.facebookLoginReward = retData.info.facebookLoginReward
      if retData.info.baseUrl then
        server = retData.info.baseUrl .. "/"
      end
      self:updateRouter(retData.info.current, requestData.version, retData.info.title, retData.info.desc, retData.info.updateUrl, forceUpdate)
    else
      local enterAppDirectory = true
      self:endProcess(enterAppDirectory)
    end
  end, function()
    local enterAppDirectory = true
    self:endProcess(enterAppDirectory)
  end)
end
function UpdateManager:endProcess(enterAppDirectory)
  print("-----------------UpdateManager:endProcess!!!!!")
  logger:debug(g.util.inspect(self.fileList))
  local enterDirectly = true
  if self.fileList and self.fileList.stage then
    local checkOK = true
    for i, v in ipairs(self.fileList.stage) do
      if not checkFile(self.path .. v.name, v.code) then
        CCLuaLog("----------------------------------------Check Files Error")
        checkOK = false
        break
      end
    end
    if checkOK then
      for i, v in ipairs(self.fileList.stage) do
        if v.act == "load" then
          if launchInfo ~= nil and launchInfo.hasLoadUpdateChunck then
            print("已经是最新chunk代码~")
            break
          else
            print("以下模块将会被加载...........")
            local zipPath = self.path .. v.name
            print(zipPath)
            CCLuaLoadChunksFromZIP(zipPath)
            enterDirectly = false
          end
        elseif v.act == "texture" then
          local updateTexture = string.split(v.name, ".")
          if #updateTexture > 1 then
            local exist = CCFileUtils:sharedFileUtils():isFileExist("res/" .. updateTexture[1] .. ".plist")
            print("hello exitst ", exist)
            if exist then
              print("以下texture 将会被更新移除:" .. updateTexture[1])
              unloadTexture(updateTexture[1] .. ".plist", updateTexture[1] .. ".png")
            elseif device.platform ~= "ios" and device.platform ~= "android" then
              print("以下texture 将会被更新移除:" .. updateTexture[1])
              unloadTexture(updateTexture[1] .. ".plist", updateTexture[1] .. ".png")
            end
          end
        end
      end
      for i, v in ipairs(self.fileList.remove) do
        removeFile(self.path .. v)
      end
    else
      removeFile(self.curListFile or self.path .. "resourcelist")
    end
  end
  launchInfo = launchInfo or {}
  launchInfo.GLOBLE_DEVICE_PATH = device.writablePath
  launchInfo.facebookLoginReward = launchInfo.facebookLoginReward or self.facebookLoginReward
  if self.fileList ~= nil then
    launchInfo.version = self.fileList.ver or g.native:getVersion()
  else
    launchInfo.version = g.native:getVersion()
  end
  if not enterDirectly then
    launchInfo.hasLoadUpdateChunck = true
  end
  if self.updateFinishedCallback then
    self.updateFinishedCallback(enterDirectly)
  end
end
function UpdateManager:requestFromServer(filename, waittime)
  local url = server .. filename .. param
  self.requestCount = self.requestCount + 1
  local index = self.requestCount
  local request
  if NEEDUPDATE then
    request = network.createHTTPRequest(function(event)
      self:onResponse(event, index)
    end, url, "GET")
  end
  if request then
    request:setTimeout(waittime or 30)
    request:start()
  else
    local enterAppDirectory = true
    self:endProcess(enterAppDirectory)
  end
end
function UpdateManager:onResponse(event, index, dumpResponse)
  local request = event.request
  print("REQUEST %d - event.name = %s", index, event.name)
  if event.name == "completed" then
    if request:getResponseStatusCode() ~= 200 then
      local enterAppDirectory = true
      self:endProcess(enterAppDirectory)
    else
      printf("REQUEST %d - getResponseDataLength() = %d", index, request:getResponseDataLength())
      if dumpResponse then
        printf([[
REQUEST %d - getResponseString() =
%s]], index, request:getResponseString())
      end
      self.dataRecv = request:getResponseData()
      self.dataRecvLength = request:getResponseDataLength()
      self:onEnterFrame()
    end
  elseif event.name == "inprogress" then
  else
    printf("REQUEST %d - getErrorCode() = %d, getErrorMessage() = %s", index, request:getErrorCode(), request:getErrorMessage())
    local enterAppDirectory = true
    self:endProcess(enterAppDirectory)
  end
end
return UpdateManager

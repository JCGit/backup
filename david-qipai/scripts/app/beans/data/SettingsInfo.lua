local SettingsInfo = class("SettingsInfo")
function SettingsInfo:ctor()
end
function SettingsInfo:saveSettingOnline()
  if not self._isSavingSetting then
    self._isSavingSetting = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    local volume = g.storage:getInt(g.vars.storageKeys.volume)
    requestData.volume = volume
    local bgm = g.storage:getInt(g.vars.storageKeys.bgm)
    requestData.bgm = bgm
    local volumeEnable = g.storage:getBool(g.vars.storageKeys.volumeEnable, true)
    if volumeEnable then
      requestData.volumeEnable = 1
    else
      requestData.volumeEnable = 0
    end
    local bgmEnable = g.storage:getBool(g.vars.storageKeys.bgmEnable, true)
    if bgmEnable then
      requestData.bgmEnable = 1
    else
      requestData.bgmEnable = 0
    end
    local isShock = g.storage:getBool(g.vars.storageKeys.shock)
    if isShock then
      requestData.vibrate = 1
    else
      requestData.vibrate = 0
    end
    local isAutoSit = g.storage:getBool(g.vars.storageKeys.autoSit)
    if isAutoSit then
      requestData.autoSit = 1
    else
      requestData.autoSit = 0
    end
    local isBuyin = g.storage:getBool(g.vars.storageKeys.autoBuy)
    if isBuyin then
      requestData.autoBuy = 1
    else
      requestData.autoBuy = 0
    end
    logger:debug(g.util.inspect(requestData))
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("settings", "set", requestData), function(data)
      self._isSavingSetting = false
      local retData = json.decode(data)
      if retData == nil or tonumber(retData.ret) == 0 then
      else
      end
    end, function()
      self._isSavingSetting = false
    end)
  end
end
function SettingsInfo:copyOnlineSetting()
  if not self._isCopySettingRequest then
    self._isCopySettingRequest = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("settings", "get", requestData), function(data)
      self._isCopySettingRequest = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        g.storage:setInt(g.vars.storageKeys.volume, tonumber(retData.info.volume))
        g.storage:setBool(g.vars.storageKeys.volumeEnable, tonumber(retData.info.volumeEnable) ~= 0)
        g.storage:setInt(g.vars.storageKeys.bgm, tonumber(retData.info.bgm))
        g.storage:setBool(g.vars.storageKeys.bgmEnable, tonumber(retData.info.bgmEnable) ~= 0)
        g.storage:setBool(g.vars.storageKeys.shock, tonumber(retData.info.vibrate) ~= 0)
        g.storage:setBool(g.vars.storageKeys.autoSit, tonumber(retData.info.autoSit) ~= 0)
        g.storage:setBool(g.vars.storageKeys.autoBuy, tonumber(retData.info.autoBuy) ~= 0)
        g.storage:flush()
        g.audio:updateVolume()
      else
        local volumeEnable = g.storage:getBool(g.vars.storageKeys.volumeEnable, true)
        g.storage:setBool(g.vars.storageKeys.volumeEnable, volumeEnable)
        local bgmEnable = g.storage:getBool(g.vars.storageKeys.bgmEnable, true)
        g.storage:setBool(g.vars.storageKeys.bgmEnable, bgmEnable)
        g.audio:updateVolume()
      end
    end, function()
      self._isCopySettingRequest = false
      local volumeEnable = g.storage:getBool(g.vars.storageKeys.volumeEnable, true)
      g.storage:setBool(g.vars.storageKeys.volumeEnable, volumeEnable)
      local bgmEnable = g.storage:getBool(g.vars.storageKeys.bgmEnable, true)
      g.storage:setBool(g.vars.storageKeys.bgmEnable, bgmEnable)
      g.audio:updateVolume()
    end)
  end
end
function SettingsInfo:requestQAListData(successCallback, failCallback)
  if not self._isRequstingQAList then
    self._isRequstingQAList = true
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("help", "faq", requestData), function(data)
      self._isRequstingQAList = false
      local retData = json.decode(data)
      if retData ~= nil and tonumber(retData.ret) == 0 then
        if successCallback ~= nil then
          successCallback()
        end
      elseif failCallback ~= nil then
        failCallback()
      end
    end, function()
      self._isRequstingQAList = false
      if failCallback ~= nil then
        failCallback()
      end
    end)
  end
end
return SettingsInfo

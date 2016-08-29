local GiftInfo = class("GiftInfo")
local GIFT = {}
function GiftInfo:ctor()
  self._isGiftRequesting = false
  self._isMyGiftRequesting = false
end
function GiftInfo:getGiftById(giftId)
  return GIFT[tonumber(giftId)]
end
function GiftInfo:create(giftId, name, chips, pic, category, status, duration, order)
  GIFT[giftId] = {}
  GIFT[giftId].giftId = giftId
  GIFT[giftId].name = name
  GIFT[giftId].chips = chips
  GIFT[giftId].pic = pic
  GIFT[giftId].category = category
  GIFT[giftId].status = status
  GIFT[giftId].duration = duration
  GIFT[giftId].order = order
end
function GiftInfo:loadGiftData(success, fail)
  if not self._giftData then
    if not self._isGiftRequesting then
      if not io.exists(device.writablePath .. "upd") then
        lfs.mkdir(device.writablePath .. "upd")
      end
      local cacheFileName = device.writablePath .. "upd" .. "/" .. "giftCache"
      local file = io.open(cacheFileName, "rb")
      if file ~= nil then
        local data = file:read("*all")
        if data ~= nil then
          local jsonData = json.decode(data)
          if jsonData ~= nil and jsonData.giftJson == g.vars.user.giftJson then
            self._giftData = jsonData.giftData
            self:processGiftData()
            self._isGiftRequesting = false
            success()
            return
          end
        end
      end
      self._success = success
      self._fail = fail
      self._isGiftRequesting = true
      local requestData = {}
      g.httpRequest:getMethod(g.requestParamsFactory:createGETRequest(g.vars.user.cdn .. "/" .. g.vars.user.giftJson, requestData, "GiftInfo/GiftData"), handler(self, self.onGiftDataLoaded), function()
        self._isGiftRequesting = false
        if fail then
          fail()
        end
      end)
    end
  else
    success()
  end
end
function GiftInfo:onGiftDataLoaded(data)
  self._isGiftRequesting = false
  self._giftData = json.decode(data)
  if not io.exists(device.writablePath .. "upd") then
    lfs.mkdir(device.writablePath .. "upd")
  end
  local cacheFileName = device.writablePath .. "upd" .. "/" .. "giftCache"
  local jsonData = {}
  jsonData.giftJson = g.vars.user.giftJson
  jsonData.giftData = self._giftData
  io.writefile(cacheFileName, json.encode(jsonData))
  if self._giftData then
    self:processGiftData()
    self._success()
  else
    self._fail()
  end
end
function GiftInfo:processGiftData()
  self._hotGiftData = {}
  self._boutiqueGiftData = {}
  self._festivalGiftData = {}
  self._otherGiftData = {}
  for i = 1, #self._giftData do
    if self._giftData[i].status == "0" then
      if self._giftData[i].category == "0" then
        table.insert(self._hotGiftData, self._giftData[i])
      elseif self._giftData[i].category == "1" then
        table.insert(self._boutiqueGiftData, self._giftData[i])
      elseif self._giftData[i].category == "2" then
        table.insert(self._festivalGiftData, self._giftData[i])
      elseif self._giftData[i].category == "3" then
        table.insert(self._otherGiftData, self._giftData[i])
      end
    end
    self._giftData[i].pic = g.vars.user.cdn .. "/" .. self._giftData[i].pic
    self:create(self._giftData[i].giftId, self._giftData[i].name, self._giftData[i].chips, self._giftData[i].pic, self._giftData[i].category, self._giftData[i].status, self._giftData[i].duration, self._giftData[i].order)
  end
end
function GiftInfo:loadMyGiftData(success)
  if self._isMyGiftRequesting then
    return
  end
  local retry = 3
  local requestData = {}
  requestData.uid = g.vars.user.uid
  local request = handler(self, function()
    self._isMyGiftRequesting = true
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("gifts", "list", requestData), function(data)
      self._isMyGiftRequesting = false
      success(data)
    end, handler(self, function()
      retry = retry - 1
      if retry > 0 then
        request()
      else
        self._isMyGiftRequesting = false
        g.ui.Dialog.new({
          contentText = g.lang:getContent("common.requestDataFail"),
          confirmText = g.lang:getContent("common.retry"),
          dialogType = DialogType.NORMAL,
          callback = function()
            retry = 3
            request()
          end
        }):show()
      end
    end))
  end)
  request()
end
function GiftInfo:groupGiftData(data, limit)
  if data then
    local groupData = {}
    local length = table.maxn(data)
    local left = length
    local group
    while limit <= left do
      group = {}
      for i = 1, limit do
        local index = length - left + 1
        table.insert(group, data[length - left + 1])
        left = left - 1
      end
      table.insert(groupData, group)
    end
    group = {}
    while left > 0 do
      local index = length - left + 1
      table.insert(group, data[length - left + 1])
      left = left - 1
    end
    if #group > 0 then
      table.insert(groupData, group)
    end
    return groupData
  end
  return nil
end
function GiftInfo:getGiftData()
  return self._giftData, self._hotGiftData, self._boutiqueGiftData, self._festivalGiftData, self._otherGiftData
end
function GiftInfo:submitExchageCode(exchangeID, successCallback, failCallback)
  if not self._isSubmitCodeRequesting then
    self._isSubmitCodeRequesting = true
    do
      local requestData = {}
      requestData.uid = g.vars.user.uid
      requestData.exchangeID = exchangeID
      g.httpRequest:postMethod(g.requestParamsFactory:createRequest("exchange_code", "get", requestData), function(data)
        logger:debug(requestData)
        logger:debug(data)
        self._isSubmitCodeRequesting = false
        local retData = json.decode(data)
        local errorNo
        if retData ~= nil then
          errorNo = retData.ret
        end
        if retData ~= nil and tonumber(retData.ret) == 0 then
          if successCallback ~= nil then
            successCallback(retData)
          end
        elseif failCallback ~= nil then
          failCallback(errorNo)
        end
      end, function()
        self._isSubmitCodeRequesting = false
        if failCallback ~= nil then
          failCallback()
        end
      end)
    end
  end
end
function GiftInfo:loadPropData(success, fail)
  if not self._propData then
    if not self._isPropRequesting then
      if not io.exists(device.writablePath .. "upd") then
        lfs.mkdir(device.writablePath .. "upd")
      end
      local cacheFileName = device.writablePath .. "upd" .. "/" .. "propCache"
      local file = io.open(cacheFileName, "rb")
      if file ~= nil then
        local data = file:read("*all")
        if data ~= nil then
          local jsonData = json.decode(data)
          if jsonData ~= nil and jsonData.propJson == g.vars.user.propJson then
            self._propData = jsonData.propData
            self:processPropData()
            self._isPropRequesting = false
            success()
            return
          end
        end
      end
      self._success = success
      self._fail = fail
      self._isPropRequesting = true
      local requestData = {}
      g.httpRequest:getMethod(g.requestParamsFactory:createGETRequest(g.vars.user.cdn .. "/" .. g.vars.user.propJson, requestData, "GiftInfo/propData"), handler(self, self.onPropDataLoaded), function()
        self._isPropRequesting = false
        if not fail then
          fail()
        end
      end)
    end
  else
    success()
  end
end
function GiftInfo:processPropData()
  self._magicData = {}
  self._styleData = {}
  for i = 1, #self._propData do
    if self._propData[i].status == "0" and self._propData[i].itemId == "1" then
      table.insert(self._magicData, self._propData[i])
    end
  end
end
function GiftInfo:onPropDataLoaded(data)
  self._isPropRequesting = false
  self._propData = json.decode(data)
  if not io.exists(device.writablePath .. "upd") then
    lfs.mkdir(device.writablePath .. "upd")
  end
  local cacheFileName = device.writablePath .. "upd" .. "/" .. "propCache"
  local jsonData = {}
  jsonData.propJson = g.vars.user.propJson
  jsonData.propData = self._propData
  io.writefile(cacheFileName, json.encode(jsonData))
  if self._propData then
    self:processPropData()
    self._success()
  else
    self._fail()
  end
end
function GiftInfo:getPropData()
  return self._propData, self._magicData, self._styleData
end
return GiftInfo

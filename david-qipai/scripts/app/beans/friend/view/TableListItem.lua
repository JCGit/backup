local ProfileView = require("app.beans.profile.view.ProfileView")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
local TableListItem = class("TableListItem", g.ui.ListItem)
local SocketEvent = require("app.core.socket.SocketEvent")
TableListItem.AVATAR_LENGTH = 80
function TableListItem:ctor()
  self:setNodeEventEnabled(true)
  TableListItem.super.ctor(self, 708, 103)
end
function TableListItem:initialize()
  local posY = self.height * 0.5
  local background = display.newScale9Sprite("#common_standard_bluebg_06.png", self.width * 0.5, posY, cc.size(675, 94)):addTo(self)
  self._headPic = g.ui.AvatarView.new()
  self._headPic:pos(80, posY):addTo(self):scale(TableListItem.AVATAR_LENGTH / g.ui.AvatarView.DEFAULT_HEIGHT):setImage(nil, {
    defaultImageName = "#common_male_avatar.png"
  })
  self.friendDetailButton = cc.ui.UIPushButton.new({
    normal = "#common_transparent_skin.png",
    pressed = "#common_transparent_skin.png"
  }, {scale9 = true}):setButtonSize(100, 82):onButtonClicked(handler(self, self.onFriendDetailClick)):pos(64, posY):addTo(self)
  self._userNameLable = ui.newTTFLabel({
    text = "",
    color = g.font.color.LIGHT,
    size = 26,
    align = ui.TEXT_ALIGN_RIGHT
  }):align(display.LEFT_CENTER, 139, posY + 20):addTo(self)
  display.newSprite("#mainlobby_chips_icon.png"):align(display.LEFT_CENTER, 350, posY):addTo(self)
  self._chipsLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.GOLDEN,
    size = 26,
    align = ui.TEXT_ALIGN_RIGHT
  }):align(display.LEFT_CENTER, 380, posY):addTo(self)
  self._statusLabel = ui.newTTFLabel({
    text = "",
    color = ccc3(62, 162, 238),
    size = 22,
    align = ui.TEXT_ALIGN_RIGHT
  }):align(display.LEFT_CENTER, 139, posY - 20):addTo(self)
  self._sendButton = cc.ui.UIPushButton.new({
    normal = "#common_orange_btn_up.png",
    pressed = "#common_orange_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonSize(100, 52):setButtonLabel("normal", ui.newTTFLabel({
    text = g.lang:getContent("friend.sendFriendChips"),
    color = ccc3(102, 46, 0),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabel("disabled", ui.newTTFLabel({
    text = g.lang:getContent("friend.sendFriendChips"),
    color = g.font.color.DARK,
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })):pos(525, posY):addTo(self):onButtonClicked(clickHandler(self, self._onSendClick))
  self._sendButton:setTouchSwallowEnabled(false)
  self._followButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonSize(100, 52):setButtonLabel("normal", ui.newTTFLabel({
    text = g.lang:getContent("friend.followFriend"),
    color = ccc3(50, 88, 0),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabel("disabled", ui.newTTFLabel({
    text = g.lang:getContent("friend.followFriend"),
    color = g.font.color.DARK,
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })):pos(630, posY):addTo(self):onButtonClicked(clickHandler(self, self._onTraceClick))
  self._followButton:setTouchSwallowEnabled(false)
end
function TableListItem:lazyInitialize()
  if not self._initialized then
    self._initialized = true
    self:initialize()
  end
  if self._isModyfied then
    self._isModyfied = false
    self:_setData(self._data)
  end
  if self._headPicTrigger and self._data then
    self._headPicTrigger = false
    self._headPic:setImage(self._data.sPic, {
      defaultImageName = "#common_male_avatar.png"
    })
  end
end
function TableListItem:onDataSet(isModyfied, data)
  self._isModyfied = isModyfied
end
function TableListItem:_setData(data)
  self.directoryName = data
  self._chipsLabel:setString(self.directoryName)
  self._chipsLabel:setPositionX(150)
  do return end
  self._headPic:setImage(self._data.sPic, {
    defaultImageName = "#common_male_avatar.png"
  })
  self._userNameLable:setString(data.name)
  self._chipsLabel:setString(g.util.functions.formatBigInt(data.chips))
  if g.vars.user.isFriendGift and g.vars.user.isFriendGift == 0 then
    self._sendButton:hide()
  else
    self._sendButton:show()
    data.send = tonumber(data.send)
    if 0 < data.send then
      self._sendButton:setButtonEnabled(true)
    else
      self._sendButton:setButtonEnabled(false)
      local label = self._sendButton:getButtonLabel()
      label:setColor(g.font.color.DARK)
    end
  end
  if data.ip and data.port and data.tid then
    self._followButton:setButtonEnabled(true)
  else
    self._followButton:setButtonEnabled(false)
    local label = self._followButton:getButtonLabel()
    label:setColor(g.font.color.DARK)
  end
  local descString
  local now = g.util.functions.getTime()
  if tonumber(data.status) == 0 then
    local elapsed = now - data.updateTime
    if elapsed >= 2592000 then
      descString = string.gsub(g.lang:getContent("lobby.playerLoginMonths"), "{month}", math.floor(elapsed / 86400 * 30))
    elseif elapsed >= 86400 then
      descString = string.gsub(g.lang:getContent("lobby.playerLoginDays"), "{day}", math.floor(elapsed / 86400))
    elseif elapsed >= 2400 then
      descString = string.gsub(g.lang:getContent("lobby.playerLoginHours"), "{hour}", math.floor(elapsed / 2400))
    else
      descString = g.lang:getContent("lobby.playerLoginJustnow")
    end
  elseif tonumber(data.status) == 1 then
    descString = g.lang:getContent("lobby.playerIsIdle")
  end
  self._statusLabel:setString(descString)
end
function TableListItem:_onSendClick()
  if not self._isSendingChips then
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.friendUid = self._data.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("friends", "deliverGift", requestData), handler(self, self._onSendLoaded), function()
      g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
    end)
  end
end
function TableListItem:_onSendLoaded(data)
  local jsonData = json.decode(data)
  if jsonData.ret == 0 then
    g.ui.manager.tip:top(string.gsub(g.lang:getContent("friend.sendFriendGiftSuccess"), "{chips}", g.util.functions.formatBigInt(jsonData.info)))
    self._data.send = self._data.send - 1
    if 0 >= self._data.send then
      self._sendButton:setButtonEnabled(false)
    end
  elseif jsonData.ret == -11 then
    g.ui.manager.tip:top(g.lang:getContent("friend.allreadySendChipsTip"))
  end
end
function TableListItem:_onTraceClick()
  g.ui.manager.popup:removeAllPopup()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  g.eventCenter:dispatchEvent({
    name = LobbyBeanEvent.EVT_ENTER_TABLE,
    data = self._data
  })
  showSceneLoading(g.lang:getContent("lobby.tableLoading"))
end
function TableListItem:onFriendDetailClick()
  local tableDirName = device.writablePath .. "tablelist" .. "/" .. self.directoryName
  g.record_roundID = self.directoryName
  local allFiles = {}
  function attrdir(path)
    for file in lfs.dir(path) do
      if file ~= "." and file ~= ".." then
        local f = path .. "/" .. file
        local attr = lfs.attributes(f)
        assert(type(attr) == "table")
        table.insert(allFiles, file)
      end
    end
  end
  attrdir(tableDirName)
  local baseDelay = 5
  if #allFiles > 0 then
    table.sort(allFiles, function(a, b)
      return tonumber(a) < tonumber(b)
    end)
    local baseTime = allFiles[1]
    for i, fileName in ipairs(allFiles) do
      do
        local thisPacketDelayTime = tonumber(fileName) - tonumber(baseTime)
        local file = io.open(tableDirName .. "/" .. fileName, "rb")
        local data = file:read("*all")
        scheduler.performWithDelayGlobal(function()
          local evt = {}
          evt.data = data
          evt.isReplayPacket = true
          evt.roundId = g.record_roundID
          evt.fileName = fileName
          g.record_socketService:onData(evt)
        end, baseDelay + thisPacketDelayTime)
      end
    end
  end
  g.tablesInfo:getPlayTableWithTid(nil, nil, nil)
end
return TableListItem

local LIST_POS_Y = 0
local InviteListView = class("InviteListView", function()
  return display.newNode()
end)
local InviteListItem = import(".InviteListItem")
function InviteListView:ctor(width, height)
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  self._width = width
  self._height = height
  self._background = display.newScale9Sprite("#transparent.png", 0, 0, CCSize(width, height)):addTo(self)
  self._background:setTouchEnabled(true)
  self._background:setTouchSwallowEnabled(true)
  self._panelNode = display.newNode():addTo(self)
  local touchCover = display.newScale9Sprite("#transparent.png", 0, 0, CCSize(width, 80)):pos(0, LIST_POS_Y - self._height * 0.5 - 40):addTo(self)
  touchCover:setTouchEnabled(true)
  touchCover:setTouchSwallowEnabled(true)
  self._buttonNode = display.newNode():addTo(self):hide()
  self._selectAllBg = display.newSprite("#pd_common_check_bg.png"):pos(-66, 0)
  self._selected = display.newSprite("#pd_common_check_icon.png"):pos(-66, 0):hide()
  self._selectAllButton = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#transparent.png"
  }, {scale9 = true}):setButtonLabel("normal", ui.newTTFLabel({
    text = g.lang:getContent("friend.selectAll"),
    color = g.font.color.WHITE,
    size = 22,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabelOffset(0, 0):setButtonSize(180, 60):pos(-self._width * 0.5 + 90 + 30, LIST_POS_Y - self._height * 0.5 - 36):addTo(self._buttonNode, 5, 5):add(self._selectAllBg):add(self._selected):onButtonClicked(clickHandler(self, self.onSelectAllClick))
  self._inviteButtonLabel = ui.newTTFLabel({
    text = g.lang:getContent("friend.sendInvite"),
    color = ccc3(50, 88, 0),
    size = 26,
    align = ui.TEXT_ALIGN_CENTER
  })
  self._inviteButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonSize(200, 55):setButtonLabel("normal", self._inviteButtonLabel):pos(0, LIST_POS_Y - self._height * 0.5 - 36):onButtonClicked(clickHandler(self, self._onInviteButtonClick)):addTo(self._buttonNode)
  display.newSprite("#mainlobby_chips_icon.png"):align(display.LEFT_CENTER, 110, LIST_POS_Y - self._height * 0.5 - 36):addTo(self._buttonNode)
  self._chipsLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.GOLDEN,
    size = 26,
    align = ui.TEXT_ALIGN_RIGHT
  }):align(display.LEFT_CENTER, 145, LIST_POS_Y - self._height * 0.5 - 36):addTo(self._buttonNode)
  self._buttonNode:hide()
  self:createPanelWithHeight(371, -g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5 - 5)
  if g.vars.user.loginType == "facebook" then
    self:_showMiniLoading()
    g.facebook:getUnauthorizedFriends(handler(self, self._onDataLoaded))
  else
    self:_showNoDataTip(g.lang:getContent("lobby.loginFacebookToInvite"))
  end
  self._invitedFriends = self._invitedFriends or {}
  self._invitedFriends.friends = self._invitedFriends.friends or {}
  self._invitedFriends.date = self._invitedFriends.date or os.date("%Y%m%d")
  self._invitedFriends.todayTotal = self._invitedFriends.todayTotal or 0
end
function InviteListView:onSelectAllClick(evt)
  if not self._items or #self._items == 0 then
    self._selected:hide()
    self._selectAllButton:setButtonLabelString("normal", g.lang:getContent("friend.selectAll"))
    return
  end
  local selectedCount = self:getSelectedCount()
  if selectedCount == self:getSelectLimit() or selectedCount == #self._items then
    for key, item in ipairs(self._items) do
      item:setSelected(false)
    end
  else
    local limit = self:getSelectLimit()
    for key, item in ipairs(self._items) do
      if not item:isSelected() then
        selectedCount = selectedCount + 1
        item:setSelected(true)
        if selectedCount == limit then
          break
        end
      end
    end
  end
  self:setSelecteTip()
  self:selectAll()
end
function InviteListView:createPanelWithHeight(height, y)
  self._facebookFriendPanel = g.ui.ScrollPanel.new({
    bgMargin = cc.size(0, 8),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-self._width * 0.5, -self._height * 0.5, self._width, height),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  })
  self._inviteFriendNode = display.newNode()
  self._facebookFriendPanel:setScrollContent(self._inviteFriendNode)
  self._facebookFriendPanel:pos(0, y):addTo(self._panelNode, 2, 2)
end
function InviteListView:removePanel()
  self._inviteFriendNode:removeFromParent()
  self._inviteFriendNode = nil
  self._facebookFriendPanel:removeFromParent()
  self._facebookFriendPanel = nil
end
function InviteListView:selectAll()
  if not self._items or #self._items == 0 then
    self._selected:hide()
    return
  end
  local selectedCount = self:getSelectedCount()
  if selectedCount == self:getSelectLimit() or selectedCount == #self._items then
    self._selected:show()
    self._selectAllButton:setButtonLabelString("normal", g.lang:getContent("friend.unSelectAll"))
  else
    self._selected:hide()
    self._selectAllButton:setButtonLabelString("normal", g.lang:getContent("friend.selectAll"))
  end
end
function InviteListView:getSelectLimit()
  if self._invitedFriends.todayTotal >= g.vars.user.facebookInviteLimit then
    return 0
  else
    local left = g.vars.user.facebookInviteLimit - self._invitedFriends.todayTotal
    if left <= 50 then
      return left
    else
      return 50
    end
  end
end
function InviteListView:_onDataLoaded(isSuccess, facebookFriends)
  self:_hideMiniLoading()
  if isSuccess then
    do
      local facebookInvitedFriends = g.storage:getString(g.vars.storageKeys.facebookInvitedFriends)
      if facebookInvitedFriends ~= "" then
        self._invitedFriends = json.decode(facebookInvitedFriends)
        local now = os.time()
        for friend, timestamp in pairs(self._invitedFriends.friends) do
          local i, length = 1, #facebookFriends
          while i <= length do
            if facebookFriends[i].name == friend and now - timestamp < g.vars.user.facebookInviteExpire then
              table.remove(facebookFriends, i)
              i = i - 1
              length = length - 1
            else
              self._invitedFriends.friends[friend] = nil
            end
            i = i + 1
          end
        end
        local today = os.date("%Y%m%d")
        if self._invitedFriends.date ~= today then
          self._invitedFriends.date = today
          self._invitedFriends.todayTotal = 0
        end
      end
      self._maxFriend = #facebookFriends
      if self._maxFriend >= 300 then
        self._maxFriend = 300
      elseif self._maxFriend == 0 then
        self:_showNoDataTip(g.lang:getContent("lobby.noFriendsToInvite"))
        return
      end
      self._buttonNode:show()
      self:removePanel()
      self:createPanelWithHeight(self._height, LIST_POS_Y)
      local nodeWidth = self._width
      local nodeHeight = math.ceil(self._maxFriend / 2) * (InviteListItem.ITEM_HEIGHT + 3)
      self._inviteFriendNode:setContentSize(CCSize(nodeWidth, nodeHeight))
      self._items = {}
      for i = 1, self._maxFriend do
        self._items[i] = InviteListItem.new():pos(-nodeWidth * 0.5 + 11 + InviteListItem.ITEM_WIDTH * 0.5 + (i + 1) % 2 * (InviteListItem.ITEM_WIDTH + 7), nodeHeight * 0.5 - 4 - InviteListItem.ITEM_HEIGHT * 0.5 - math.floor((i - 1) / 2) * (InviteListItem.ITEM_HEIGHT + 3)):setOwner(self._facebookFriendPanel):setPopup(self):addTo(self._inviteFriendNode)
        self._items[i]:setData(facebookFriends[i])
      end
      local outterSelf = self
      local innerSelf = self._facebookFriendPanel
      function self._facebookFriendPanel.onScrolling()
        if innerSelf._visibleRectOriginPoint and #outterSelf._items > 0 then
          for key, item in ipairs(outterSelf._items) do
            local stagePoint = outterSelf._inviteFriendNode:convertToWorldSpace(ccp(item:getPosition()))
            if stagePoint.y > innerSelf._visibleRectOriginPoint.y + innerSelf._visibleRect.height + item.ITEM_HEIGHT or stagePoint.y < innerSelf._visibleRectOriginPoint.y - item.ITEM_HEIGHT - item.ITEM_HEIGHT then
              item:hide()
              if item.onItemInactive and (stagePoint.y - (innerSelf._visibleRectOriginPoint.y + innerSelf._visibleRect.height) > innerSelf._visibleRect.height or innerSelf._visibleRectOriginPoint.y - item.ITEM_HEIGHT - stagePoint.y > innerSelf._visibleRect.height) then
                item:onItemInactive()
              end
            else
              item:show()
              if item.lazyInitialize then
                item:lazyInitialize()
              end
            end
          end
        end
      end
      self._facebookFriendPanel:update()
      self._facebookFriendPanel:onScrolling()
      self:onSelectAllClick()
    end
  else
    self:_showNoDataTip(g.lang:getContent("lobby.noFriendsToInvite"))
  end
end
function InviteListView:getSelectedCount()
  local selectedCount = 0
  if self._items then
    for key, item in ipairs(self._items) do
      if item:isSelected() then
        selectedCount = selectedCount + 1
      end
    end
  end
  return selectedCount
end
function InviteListView:setSelecteTip()
  local selectedCount = 0
  for key, item in ipairs(self._items) do
    if item:isSelected() then
      selectedCount = selectedCount + 1
    end
  end
  local text = g.lang:getContent("friend.sendInviteTip")
  text = string.gsub(text, "{chips}", string.formatnumberthousands(selectedCount * g.vars.user.rewards.info.facebookInviteReward))
  self._chipsLabel:setString(text)
end
function InviteListView:_onInviteButtonClick()
  if not self._items then
    return
  end
  local toIdTable = {}
  local friendTable = {}
  for key, item in ipairs(self._items) do
    if item:isSelected() then
      table.insert(toIdTable, item:getData().id)
      table.insert(friendTable, item:getData().name)
    end
  end
  if self._invitedFriends.todayTotal >= g.vars.user.facebookInviteLimit then
    g.ui.manager.tip:top(g.lang:getContent("friend.inviteLimitExceeded"))
    return
  elseif self._invitedFriends.todayTotal + #toIdTable > g.vars.user.facebookInviteLimit then
    local text = string.gsub(g.lang:getContent("friend.inviteLimit"), "{friend}", g.vars.user.facebookInviteLimit - self._invitedFriends.todayTotal)
    g.ui.manager.tip:top(text)
    return
  end
  self._toIds = table.concat(toIdTable, ",")
  self._toNames = friendTable
  if self._toIds ~= "" then
    local requestData = {}
    requestData.uid = g.vars.user.uid
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("invite", "hash", requestData), handler(self, self._onInviteHashLoaded), function()
      g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
    end)
  end
end
function InviteListView:_onInviteHashLoaded(data)
  local jsonData = json.decode(data)
  if jsonData.ret and jsonData.ret == 0 then
    jsonData = jsonData.info
    self._inviteHash = "su:" .. jsonData.su .. ";id:" .. jsonData.id .. ";ik:" .. jsonData.ik
  end
  g.facebook:sendInvites(self._inviteHash, self._toIds, g.lang:getContent("friend.inviteSubject"), g.lang:getContent("friend.inviteContent"), handler(self, self.facebookInviteCallback))
end
function InviteListView:facebookInviteCallback(isSuccess, result)
  if isSuccess then
    if self._toNames ~= nil then
      self._invitedFriends.todayTotal = self._invitedFriends.todayTotal + #self._toNames
      local now = os.time()
      for i, friend in ipairs(self._toNames) do
        self._invitedFriends.friends[friend] = now
      end
      g.storage:setString(g.vars.storageKeys.facebookInvitedFriends, json.encode(self._invitedFriends))
      g.storage:flush()
    end
    if result.toIds then
      local length = string.len(result.toIds)
      if length > 0 and string.sub(result.toIds, length, length) == "," then
        result.toIds = string.sub(result.toIds, 1, length - 1)
      end
    end
    local requestData = {}
    requestData.uid = g.vars.user.uid
    requestData.inviteHash = self._inviteHash
    requestData.requestId = result.requestId
    requestData.toIds = result.toIds
    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("invite", "reward", requestData), handler(self, self._onInviteReported), function()
    end)
  end
end
function InviteListView:setHideCallback(callback)
  self._hideCallback = callback
end
function InviteListView:_onInviteReported(data)
  local jsonData = json.decode(data)
  if jsonData and jsonData.ret == 0 and jsonData.info ~= nil then
    local lastFacebookInviteReward = g.storage:getInt(g.vars.storageKeys.lastFacebookInviteReward)
    lastFacebookInviteReward = lastFacebookInviteReward + tonumber(jsonData.info)
    g.storage:setInt(g.vars.storageKeys.lastFacebookInviteReward, lastFacebookInviteReward)
    g.ui.manager.tip:top(string.gsub(g.lang:getContent("friend.sendInviteSuccessTip"), "{chips}", jsonData.info))
  end
  if self._hideCallback then
    self._hideCallback()
  end
end
function InviteListView:_showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():pos(0, LIST_POS_Y):addTo(self, 9, 9)
  end
end
function InviteListView:_hideMiniLoading()
  if self._miniLoading then
    self._miniLoading:removeFromParent()
    self._miniLoading = nil
  end
end
function InviteListView:_showNoDataTip(noDataTip)
  if not self._noDataTip then
    self._noDataTip = ui.newTTFLabel({
      text = noDataTip,
      color = ccc3(139, 184, 220),
      size = 24,
      align = ui.TEXT_ALIGN_CENTER
    }):pos(0, -g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5 - 5):addTo(self, 8, 8)
  end
end
function InviteListView:_hideNoDataTip()
  if self._noDataTip then
    self._noDataTip:removeFromParent()
    self._noDataTip = nil
  end
end
function InviteListView:show()
  g.ui.manager.popup:addPopup(self)
end
function InviteListView:hide()
  g.ui.manager.popup:removePopup(self)
end
function InviteListView:onShow()
  if self._facebookFriendPanel then
    self._facebookFriendPanel:setScrollContentTouchRect()
  end
end
function InviteListView:onExit()
  scheduler.performWithDelayGlobal(function()
    CCTextureCache:sharedTextureCache():removeUnusedTextures()
  end, 0.1)
end
return InviteListView

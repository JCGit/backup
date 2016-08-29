local FriendsWindowItem = import(".FriendsWindowItem")
local FriendBeanEvent = require("app.beans.friend.event.FriendBeanEvent")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
local FriendsWindowSubView = class("FriendsWindowSubView", function()
  return display.newNode()
end)
FriendsWindowSubView.WIDTH = 326
FriendsWindowSubView.HEIGHT = 510
FriendsWindowSubView.BoardXGap = 5
FriendsWindowSubView.BoardYGap = 15
FriendsWindowSubView.First_Tab_Gap = 15
FriendsWindowSubView.TAB_GROUP_HEIGHT = 50
FriendsWindowSubView.LIST_WIDTH = 300
FriendsWindowSubView.LIST_HEIGHT = 420
function FriendsWindowSubView:ctor()
  self:setNodeEventEnabled(true)
  self:setCascadeOpacityEnabled(true)
  self:initialize()
end
function FriendsWindowSubView:onEnter()
  self._refreshData = true
  self._subTabGroupFriends:tabOn(g.vars.user.onlineFriendDefaultTab or 2, false)
  self._deleteFriendHandle = g.eventCenter:addEventListener(FriendBeanEvent.EVT_DELETE_FRIEND, handler(self, self._onFriendDeleted))
  self._inviteSentHandle = g.eventCenter:addEventListener(FriendBeanEvent.EVT_INVITE_SENT, handler(self, self._onInviteSent))
  self._refreshFriendsHandle = g.eventCenter:addEventListener(LobbyBeanEvent.EVT_LOBBY_REFRESH_FRIENDLIST, handler(self, self.onResponseToRefreshFriendsList))
  self._isInvitableLoading = false
end
function FriendsWindowSubView:onExit()
  self:dispose()
  g.eventCenter:removeEventListener(self._deleteFriendHandle)
  g.eventCenter:removeEventListener(self._inviteSentHandle)
  g.eventCenter:removeEventListener(self._refreshFriendsHandle)
end
function FriendsWindowSubView:dispose()
  if self._deleteFriendHandle then
    g.eventCenter:removeEventListener(self._deleteFriendHandle)
    self._deleteFriendHandle = nil
  end
  if self._friendListRequstID ~= nil then
    self._friendListRequstID:cancel()
    self._friendListRequstID = nil
  end
end
function FriendsWindowSubView:_onFriendDeleted(evt)
  if self._selectedTab == 1 then
    self:requestOnlineFriendsData()
  end
end
function FriendsWindowSubView:initialize()
  local pointX = -display.cx + FriendsWindowSubView.WIDTH * 0.5
  local bgPointY = display.cy - FriendsWindowSubView.HEIGHT * 0.5 - FriendsWindowSubView.BoardYGap
  self.bgSprite = display.newScale9Sprite("#common_standard_bluebg_01.png", pointX, bgPointY, cc.size(FriendsWindowSubView.WIDTH, FriendsWindowSubView.HEIGHT)):addTo(self)
  self._subTabGroupFriends = g.ui.TabGroupWithSlider.new({
    background = "#common_standard_greybg_02.png",
    slider = "#common_standard_bluebg_04.png"
  }, g.lang:getContent("lobby.friendsWindow"), {
    selectedText = {
      color = g.font.color.LIGHT,
      size = 26
    },
    defaltText = {
      color = ccc3(139, 184, 220),
      size = 26
    }
  }, true, true)
  self._subTabGroupFriends:addTo(self.bgSprite)
  self._subTabGroupFriends:setTabGroupSize(300, FriendsWindowSubView.TAB_GROUP_HEIGHT, -4, -4)
  self._subTabGroupFriends:pos(FriendsWindowSubView.WIDTH * 0.5, FriendsWindowSubView.HEIGHT - FriendsWindowSubView.TAB_GROUP_HEIGHT * 0.5 - FriendsWindowSubView.First_Tab_Gap)
  self._subTabGroupFriends:bindTabClickHandler(handler(self, self._onTabChange))
  self._subTabGroupFriends:setCascadeOpacityEnabled(true)
  self._listPosY = -g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5
  self._friendListPanel = g.ui.ListPanel.new({
    bgMargin = cc.size(0, 2),
    background = "#common_standard_greybg_01.png",
    visibleRect = cc.rect(-FriendsWindowSubView.LIST_WIDTH * 0.5, -FriendsWindowSubView.LIST_HEIGHT * 0.5, FriendsWindowSubView.LIST_WIDTH, FriendsWindowSubView.LIST_HEIGHT),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }, FriendsWindowItem):pos(FriendsWindowSubView.WIDTH * 0.5, FriendsWindowSubView.LIST_HEIGHT * 0.5 + 15):addTo(self.bgSprite)
  self._friendListPanel:setCascadeOpacityEnabled(true)
end
function FriendsWindowSubView:_onTabChange(selectedTab)
  self._selectedTab = selectedTab
  self:setListData(nil)
  if selectedTab == 1 then
    FriendsWindowItem.ITEM_TYPE = FriendsWindowItem.ONLINE_TYPE
    self._friendListPanel:setNoDataHintText(g.lang:getContent("lobby.noFriendsOnline"))
    self:requestOnlineFriendsData()
  else
    FriendsWindowItem.ITEM_TYPE = FriendsWindowItem.INVITE_TYPE
    if g.vars.user.loginType == "device" then
      self._friendListPanel:showNoDataHintView(g.lang:getContent("lobby.loginFacebookToInvite"), 22)
    else
      self._friendListPanel:setNoDataHintText(g.lang:getContent("lobby.noFriendsToInvite"))
      self:requestUnauthorizedFriendsData()
    end
  end
end
function FriendsWindowSubView:onResponseToRefreshFriendsList()
  if self._selectedTab == 1 then
    self._refreshData = true
    self:requestOnlineFriendsData()
  else
    g.friendsInfo:refreshData()
  end
end
function FriendsWindowSubView:requestOnlineFriendsData()
  local function successCallback(friendsList)
    self._friendListRequstID = nil
    local onlineFriends = {}
    if friendsList ~= nil and #friendsList > 0 then
      for i, v in ipairs(friendsList) do
        if v.status and tonumber(v.status) ~= 0 then
          table.insert(onlineFriends, v)
        end
      end
    end
    if self._friendListPanel ~= nil then
      self._friendListPanel:hideMiniLoading()
      self:setListData(onlineFriends)
    end
  end
  local failCallback = handler(self, function(self)
    self._friendListRequstID = nil
    if self._friendListPanel ~= nil then
      self._friendListPanel:hideMiniLoading()
    end
  end)
  self._friendListPanel:showMiniLoading()
  if self._refreshData then
    self._friendListRequstID = g.friendsInfo:refreshData(successCallback, failCallback)
    self._refreshData = false
  else
    self._friendListRequstID = g.friendsInfo:getCacheData(successCallback, failCallback)
  end
end
function FriendsWindowSubView:requestUnauthorizedFriendsData()
  local callback = handler(self, function(self, isSuccess, friendsList)
    self._friendListPanel:hideMiniLoading()
    if isSuccess then
      local facebookInvitedFriends = g.storage:getString(g.vars.storageKeys.facebookInvitedFriends)
      if facebookInvitedFriends ~= "" then
        self._invitedFriends = json.decode(facebookInvitedFriends)
        local now = os.time()
        for friend, timestamp in pairs(self._invitedFriends.friends) do
          local i, length = 1, #friendsList
          while i <= length do
            if friendsList[i].name == friend and now - timestamp < g.vars.user.facebookInviteExpire then
              table.remove(friendsList, i)
              i = i - 1
              length = length - 1
            else
              self._invitedFriends.friends[friend] = nil
            end
            i = i + 1
          end
        end
      end
      table.sort(friendsList, function(a, b)
        return a == nil
      end)
      self._invitableFriends = friendsList
      self:setListData(friendsList)
    end
  end)
  self._friendListPanel:showMiniLoading()
  if self._invitableFriends == nil then
    if not self._isInvitableLoading then
      g.facebook:getUnauthorizedFriends(callback)
      self._isInvitableLoading = true
    end
  else
    callback(true, self._invitableFriends)
  end
end
function FriendsWindowSubView:_onInviteSent(evt)
  if selectedTab == 2 then
    self:requestUnauthorizedFriendsData()
  end
end
function FriendsWindowSubView:setListData(data)
  self._friendListPanel:setData(data)
  self._friendListPanel:update()
end
function FriendsWindowSubView:reloadData()
  self._subTabGroupFriends:tabOn(g.vars.user.onlineFriendDefaultTab or 2, false)
end
return FriendsWindowSubView

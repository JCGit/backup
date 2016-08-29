local PADDING = 16
local LIST_WIDTH = 695
local LIST_HEIGHT = 371
local INVITE_btn_WIDTH = 200
local INVITE_btn_HEIGHT = 84
local INVITE_btn_GAP = 30
local CONTENT_PADDING = 12
local FriendListItem = import(".FriendListItem")
local InviteListView = import(".InviteListView")
local FriendBeanEvent = require("app.beans.friend.event.FriendBeanEvent")
local FriendListView = class("FriendListView", g.ui.Panel)
function FriendListView:ctor(selectedTab)
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  self:setNodeEventEnabled(true)
  FriendListView.super.ctor(self, 720, 480)
  self:initialize()
  self:addCloseButton()
  self._deleteFriendHandle = g.eventCenter:addEventListener(FriendBeanEvent.EVT_DELETE_FRIEND, handler(self, self._onFriendDeleted))
  local defaultSelectedTab = selectedTab or 1
  self._tabGroup:tabOn(defaultSelectedTab)
end
function FriendListView:_onFriendDeleted(evt)
  g.friendsInfo:getCacheData(handler(self, self._onFriendDataLoaded))
end
function FriendListView:initialize()
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  self._friendNode = display.newNode():addTo(self)
  self._listPosY = -g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5 - 5
  self._friendListPanel = g.ui.ListPanel.new({
    bgMargin = cc.size(0, 8),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-LIST_WIDTH * 0.5, -LIST_HEIGHT * 0.5, LIST_WIDTH, LIST_HEIGHT),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }, FriendListItem):pos(0, self._listPosY):addTo(self._friendNode)
  self._inviteNode = display.newNode():addTo(self)
  self._tabGroup = g.ui.TabGroup.new({
    windowWidth = 720,
    iconOffsetX = 10,
    iconTexture = {
      {
        "#friend_icon_selected.png",
        "#friend_icon_unselected.png"
      },
      {
        "#invite_icon_selected.png",
        "#invite_icon_unselected.png"
      }
    },
    buttonTexts = g.lang:getContent("friend.tabGroupLabels")
  }):pos(0, self._height * 0.5 - g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5 - 16):addTo(self, 10)
  self._tabGroup:bindTabClickHandler(handler(self, self._onTabChange))
end
function FriendListView:_onTabChange(selectedTab)
  if selectedTab == 1 then
    self._friendNode:show()
    self._inviteNode:hide()
  else
    self:_hideMiniLoading()
    self:_hideNoDataTip()
    self._friendNode:hide()
    self._inviteNode:show()
    if self._inviteListView == nil then
      self._inviteListView = InviteListView.new(LIST_WIDTH, LIST_HEIGHT - 60):pos(0, self._listPosY + 30):addTo(self._inviteNode)
      self._inviteListView:setHideCallback(handler(self, self.hide))
    end
  end
  self._selectedTab = selectedTab
  if self._selectedTab == 1 then
    self:_showMiniLoading()
    g.friendsInfo:getCacheData(handler(self, self._onFriendDataLoaded))
  end
end
function FriendListView:_onFriendDataLoaded(friendData)
  self._friendData = friendData
  self:_hideMiniLoading()
  if self._friendData then
    if #self._friendData > 0 then
      self:_hideNoDataTip()
      self:setListData(self._friendData)
    else
      self:_showNoDataTip()
    end
  end
end
function FriendListView:_onFriendDataLoadFail()
  self:_hideMiniLoading()
end
function FriendListView:_showNoDataTip()
  if not self._noDataTip then
    self._noDataTip = ui.newTTFLabel({
      text = g.lang:getContent("friend.noFriendTip"),
      color = ccc3(139, 184, 220),
      size = 24,
      align = ui.TEXT_ALIGN_CENTER
    }):pos(0, self._listPosY):addTo(self)
  end
end
function FriendListView:_hideNoDataTip()
  if self._noDataTip then
    self._noDataTip:removeFromParent()
    self._noDataTip = nil
  end
end
function FriendListView:setListData(data)
  self._friendListPanel:setData(data)
  self._friendListPanel:update()
end
function FriendListView:showPanel()
  self:show(true, true, true, true)
end
function FriendListView:onShow()
  if self._friendListPanel then
    self._friendListPanel:setScrollContentTouchRect()
  end
end
function FriendListView:onPopupRemove(callback)
  if self._deleteFriendHandle then
    g.eventCenter:removeEventListener(self._deleteFriendHandle)
    self._deleteFriendHandle = nil
  end
  callback()
end
function FriendListView:onExit()
  scheduler.performWithDelayGlobal(function()
    CCTextureCache:sharedTextureCache():removeUnusedTextures()
  end, 0.1)
end
return FriendListView

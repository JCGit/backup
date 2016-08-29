local PADDING = 16
local LIST_WIDTH = 695
local LIST_HEIGHT = 371
local INVITE_btn_WIDTH = 200
local INVITE_btn_HEIGHT = 84
local INVITE_btn_GAP = 30
local CONTENT_PADDING = 12
local TableListItem = import(".TableListItem")
local InviteListView = import(".InviteListView")
local FriendBeanEvent = require("app.beans.friend.event.FriendBeanEvent")
local TableRecordList = class("TableRecordList", g.ui.Panel)
function TableRecordList:ctor()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  self:setNodeEventEnabled(true)
  TableRecordList.super.ctor(self, 720, 480)
  self:initialize()
  self:addCloseButton()
  if device.platform == "android" or device.platform == "ios" then
    cc.analytics:start("analytics.UmengAnalytics")
  end
  self._deleteFriendHandle = g.eventCenter:addEventListener(FriendBeanEvent.EVT_DELETE_FRIEND, handler(self, self._onFriendDeleted))
  self._tabGroup:tabOn(1)
end
function TableRecordList:_onFriendDeleted(evt)
  g.friendsInfo:getCacheData(handler(self, self._onFriendDataLoaded))
end
function TableRecordList:initialize()
  self:enableBGHightLight(128)
  self._friendNode = display.newNode():addTo(self)
  self._listPosY = -g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5 - 5
  self._friendListPanel = g.ui.ListPanel.new({
    bgMargin = cc.size(0, 8),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-LIST_WIDTH * 0.5, -LIST_HEIGHT * 0.5, LIST_WIDTH, LIST_HEIGHT),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }, TableListItem):pos(0, self._listPosY):addTo(self._friendNode)
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
function TableRecordList:_onTabChange(selectedTab)
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
    end
  end
  self._selectedTab = selectedTab
  if self._selectedTab == 1 then
    do
      local allTableRecord = {}
      function attrdir(path)
        for file in lfs.dir(path) do
          if file ~= "." and file ~= ".." then
            local f = path .. "/" .. file
            local attr = lfs.attributes(f)
            assert(type(attr) == "table")
            if attr.mode == "directory" then
              table.insert(allTableRecord, file)
            end
          end
        end
      end
      local tableDirName = device.writablePath .. "tablelist"
      attrdir(tableDirName)
      print(allTableRecord)
      self:setListData(allTableRecord)
    end
  end
end
function TableRecordList:_onFriendDataLoaded(friendData)
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
function TableRecordList:_onFriendDataLoadFail()
  self:_hideMiniLoading()
end
function TableRecordList:_onFacebookInviteClick()
  self:hide()
  InviteListView.new():show()
  if device.platform == "android" or device.platform == "ios" then
    cc.analytics:doCommand({
      command = "event",
      args = {
        eventId = "facebook_invite_friends"
      },
      label = "user facebook_invite_friends"
    })
  end
end
function TableRecordList:_onSmsInviteClick()
  g.native:showSMSView(g.lang:getContent("friend.inviteContent"))
  if device.platform == "android" or device.platform == "ios" then
    cc.analytics:doCommand({
      command = "event",
      args = {
        eventId = "sms_invite_friends"
      },
      label = "user sms_invite_friends"
    })
  end
end
function TableRecordList:_onMailInviteClick()
  g.native:showEmailView(g.lang:getContent("friend.inviteSubject"), g.lang:getContent("friend.inviteContent"))
  if device.platform == "android" or device.platform == "ios" then
    cc.analytics:doCommand({
      command = "event",
      args = {
        eventId = "mail_invite_friends"
      },
      label = "user mail_invite_friends"
    })
  end
end
function TableRecordList:_showNoDataTip()
  if not self._noDataTip then
    self._noDataTip = ui.newTTFLabel({
      text = g.lang:getContent("friend.noFriendTip"),
      color = g.font.color.LIGHT,
      size = 24,
      align = ui.TEXT_ALIGN_CENTER
    }):pos(0, self._listPosY):addTo(self)
  end
end
function TableRecordList:_hideNoDataTip()
  if self._noDataTip then
    self._noDataTip:removeFromParent()
    self._noDataTip = nil
  end
end
function TableRecordList:setListData(data)
  self._friendListPanel:setData(data)
  self._friendListPanel:update()
end
function TableRecordList:showPanel()
  self:show(true, true, true, true)
end
function TableRecordList:onShow()
  if DEBUG_RECORD_MODE then
    g.record_isNowReplaying = not g.record_isNowReplaying
  end
  if self._friendListPanel then
    self._friendListPanel:setScrollContentTouchRect()
  end
end
function TableRecordList:onPopupRemove(callback)
  if self._deleteFriendHandle then
    g.eventCenter:removeEventListener(self._deleteFriendHandle)
    self._deleteFriendHandle = nil
  end
  callback()
end
function TableRecordList:onExit()
  scheduler.performWithDelayGlobal(function()
    CCTextureCache:sharedTextureCache():removeUnusedTextures()
  end, 0.1)
end
return TableRecordList

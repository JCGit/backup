local MessagePopup = class("MessagePopup", g.ui.Panel)
local MessageListItem = import(".MessageListItem")
MessagePopup.WIDTH = 800
MessagePopup.HEIGHT = 480
MessagePopup.FIRST_TAB_SPACE = 20
MessagePopup.PADDING = 16
function MessagePopup:ctor()
  MessagePopup.super.ctor(self, MessagePopup.WIDTH, MessagePopup.HEIGHT)
  self:enableBGHightLight()
  self:setupView()
  self:_showMiniLoading()
end
function MessagePopup:exit()
end
function MessagePopup:setupView()
  self._tabGroup = g.ui.TabGroup.new({
    windowWidth = 660,
    iconOffsetX = 0,
    scale = 1,
    iconTexture = {
      {
        "#message_tab_gift_icon_selected.png",
        "#message_tab_gift_icon_unselecte.png"
      },
      {
        "#message_tab_friend_icon_selected.png",
        "#message_tab_friend_icon_unselecte.png"
      },
      {
        "#message_tab_system_icon_selected.png",
        "#message_tab_system_icon_unselecte.png"
      }
    },
    buttonTexts = g.lang:getContent("message.tabTitles")
  }):pos(0, self._height * 0.5 - MessagePopup.FIRST_TAB_SPACE - g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5):addTo(self, 10)
  self._tabGroup:bindTabClickHandler(handler(self, self._onTabPanelChange))
  self._listPosY = -MessagePopup.FIRST_TAB_SPACE - g.ui.TabGroup.TAB_GROUP_HEIGHT
  local listWidth = MessagePopup.WIDTH - 40
  local listHeight = MessagePopup.HEIGHT + self._listPosY - 15 - 15
  MessageListItem.dataChangeHandler = handler(self, self.messageDataChange)
  self._list = g.ui.ListPanel.new({
    bgMargin = cc.size(0, 8),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-listWidth * 0.5, -listHeight * 0.5, listWidth, listHeight),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }, MessageListItem):pos(0, self._listPosY * 0.5):addTo(self)
  self:addCloseButton()
end
function MessagePopup:showPanel()
  self:show(true, true, true, true)
end
function MessagePopup:_onTabPanelChange(selectedTab)
  self.topSelectIndex = selectedTab
  if selectedTab == 1 then
    self._list:setNoDataHintText(g.lang:getContent("message.noPackageMessage"))
    self._list:setData(self._retData.package)
  elseif selectedTab == 2 then
    self._list:setNoDataHintText(g.lang:getContent("message.noFriendsMessage"))
    self._list:setData(self._retData.friend)
  else
    self._list:setNoDataHintText(g.lang:getContent("message.noNoticeMessage"))
    self._list:setData(self._retData.notice)
  end
  self._list:update()
end
function MessagePopup:onShow()
  self:doRequest()
  self._list:setScrollContentTouchRect()
end
function MessagePopup:doRequest()
  g.messageInfo:refreshData(handler(self, self.doRequestSuccess), handler(self, self.doRequestFail))
end
function MessagePopup:doRequestSuccess(retData)
  self._retData = retData or {}
  if self._hideMiniLoading == nil then
    return
  end
  self:_hideMiniLoading()
  self._tabGroup:tabOn(1)
end
function MessagePopup:doRequestFail()
  if self._hideMiniLoading == nil then
    return
  end
  self:_hideMiniLoading()
end
function MessagePopup:messageDataChange()
  self._tabGroup:tabOn(self.topSelectIndex)
end
return MessagePopup

local RankBoardView = class("RankBoardView", g.ui.Panel)
local RankBoardItem = import(".RankBoardItem")
local SUB_TAB_SPACE = 72
RankBoardView.LIST_WIDTH = 690
RankBoardView.LIST_HEIGHT = 306
RankBoardView.FIRST_TAB_SPACE = 20
RankBoardView.TAB_GAP = 12
RankBoardView.SEC_TAB_HEIGHT = 48
function RankBoardView:ctor()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  self:setNodeEventEnabled(true)
  RankBoardView.super.ctor(self, 720, 468)
  self:enableBGHightLight()
  self:_createNodes()
  self:addCloseButton()
end
function RankBoardView:_createNodes()
  self._tabGroup = g.ui.TabGroup.new({
    windowWidth = 600,
    iconOffsetX = 10,
    scale = 1,
    iconTexture = {
      {
        "#common_rank_friend_select_icon.png",
        "#common_rank_friend_unselect_icon.png"
      },
      {
        "#common_rank_all_select_icon.png",
        "#common_rank_all_unselect_icon.png"
      }
    },
    buttonTexts = g.lang:getContent("rank.rankType")
  }):pos(0, self._height * 0.5 - RankBoardView.FIRST_TAB_SPACE - g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5):addTo(self, 10)
  self._tabGroup:bindTabClickHandler(handler(self, self._onTabPanelChange))
  self._listPosY = -(RankBoardView.FIRST_TAB_SPACE + g.ui.TabGroup.TAB_GROUP_HEIGHT + RankBoardView.TAB_GAP + 44) * 0.5
  self._list = g.ui.ListPanel.new({
    bgMargin = cc.size(0, 8),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-RankBoardView.LIST_WIDTH * 0.5, -RankBoardView.LIST_HEIGHT * 0.5, RankBoardView.LIST_WIDTH, RankBoardView.LIST_HEIGHT),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }, RankBoardItem):pos(0, self._listPosY):addTo(self)
  g.chartsInfo:clearData()
  g.friendsInfo:clearData()
  self:showFriendRankView(selectedTab)
end
function RankBoardView:setListData(data)
  if self._friendListData then
    for i, v in ipairs(self._friendListData) do
      v.rankIndex = i
      if self.topSelectIndex == 1 and self.bottomSelectIndex == 1 then
        v.rankType = 1
      elseif self.topSelectIndex == 1 and self.bottomSelectIndex == 2 then
        v.rankType = 2
      end
    end
  end
  if self._chartsInfoCacheData then
    for i, v in ipairs(self._chartsInfoCacheData) do
      v.rankIndex = i
      v.rankType = self.bottomSelectIndex
    end
  end
  self._list:setData(data)
  self._list:update()
end
function RankBoardView:onShow()
  self._tabGroup:tabOn(1)
  self._list:setScrollContentTouchRect()
end
function RankBoardView:showPanel()
  self:show(true, true, true, true)
end
function RankBoardView:onPopupRemove(callback)
  if self._friendListRequstID ~= nil then
    self._friendListRequstID:cancel()
    self._friendListRequstID = nil
  end
  callback()
end
function RankBoardView:_onTabPanelChange(selectedTab)
  self.topSelectIndex = selectedTab
  if selectedTab == 1 then
    self:showFriendRankView(selectedTab)
    self._subTabBarFriend:tabOn(1, false)
  elseif selectedTab == 2 then
    self:showAllRankView(selectedTab)
    self._subTabBarGlobal:tabOn(1, false)
  end
end
function RankBoardView:_onSubTabChange(selectedIndex)
  self._list:hideNoDataHintView()
  self.bottomSelectIndex = selectedIndex
  self:doRequest(self.topSelectIndex, self.bottomSelectIndex)
end
function RankBoardView:showFriendRankView(selectedTab)
  if self._subTabBarFriend == nil then
    self._subTabBarFriend = g.ui.TabGroupWithSlider.new({
      background = "#common_standard_greybg_04.png",
      slider = "#common_standard_bluebg_05.png"
    }, g.lang:getContent("rank.friendRankTypes"), {
      selectedText = {
        color = g.font.color.LIGHT,
        size = 20
      },
      defaltText = {
        color = ccc3(139, 184, 220),
        size = 20
      }
    }, true, true)
    self._subTabBarFriend:addTo(self, 11)
    self._subTabBarFriend:setTabGroupSize(400, RankBoardView.SEC_TAB_HEIGHT, -4, -6)
    self._subTabBarFriend:pos(0, self._height * 0.5 - RankBoardView.FIRST_TAB_SPACE - g.ui.TabGroup.TAB_GROUP_HEIGHT - RankBoardView.TAB_GAP - RankBoardView.SEC_TAB_HEIGHT * 0.5)
    self._subTabBarFriend:bindTabClickHandler(handler(self, self._onSubTabChange))
    self._subTabBarFriend:setVisible(selectedTab == 1)
  end
  if self._subTabBarGlobal then
    self._subTabBarGlobal:hide()
  end
  self._subTabBarFriend:show()
end
function RankBoardView:showAllRankView(selectedTab)
  if self._subTabBarGlobal == nil then
    self._subTabBarGlobal = g.ui.TabGroupWithSlider.new({
      background = "#common_standard_greybg_04.png",
      slider = "#common_standard_bluebg_05.png"
    }, g.lang:getContent("rank.allRankTypes"), {
      selectedText = {
        color = g.font.color.LIGHT,
        size = 20
      },
      defaltText = {
        color = ccc3(139, 184, 220),
        size = 20
      }
    }, true, true)
    self._subTabBarGlobal:addTo(self, 11)
    self._subTabBarGlobal:setTabGroupSize(560, RankBoardView.SEC_TAB_HEIGHT, -4, -6)
    self._subTabBarGlobal:pos(0, self._height * 0.5 - RankBoardView.FIRST_TAB_SPACE - g.ui.TabGroup.TAB_GROUP_HEIGHT - RankBoardView.TAB_GAP - RankBoardView.SEC_TAB_HEIGHT * 0.5)
    self._subTabBarGlobal:bindTabClickHandler(handler(self, self._onSubTabChange))
  end
  if self._subTabBarFriend then
    self._subTabBarFriend:hide()
  end
  self._subTabBarGlobal:show()
end
function RankBoardView:_showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():pos(0, self._listPosY):addTo(self)
  end
end
function RankBoardView:doRequest(topIndex, bottomIndex)
  self:_showMiniLoading()
  if topIndex == 1 then
    self._friendListRequstID = g.friendsInfo:getCacheData(handler(self, self.doRequestMyFriendListDataSuccessCallback), handler(self, function(self)
      self._friendListRequstID = nil
    end))
  else
    g.chartsInfo:getCacheData(bottomIndex - 1, handler(self, self.doRequestChartsDataSuccessCallback), handler(self, self.doRequestChartsDataFailCallback))
  end
end
function RankBoardView:doRequestMyFriendListDataSuccessCallback(retData)
  self._friendListRequstID = nil
  self:_hideMiniLoading()
  if retData then
    self._friendListData = retData
    self.hasAddSelfData = true
    local selfData = {
      exp = g.vars.user.exp,
      name = g.vars.user.name,
      sPic = g.vars.user.sPic,
      chips = g.vars.user.chips,
      vip = g.vars.user.vip,
      gender = g.vars.user.gender,
      continuewin = g.vars.user.win - g.vars.user.lastWeekWin,
      weekIncChips = g.vars.user.weekIncChips
    }
    table.insert(self._friendListData, selfData)
    self:sortMyFriendListInUI()
  else
  end
end
function RankBoardView:sortMyFriendListInUI()
  if self.topSelectIndex == 1 then
    if self.bottomSelectIndex == 1 then
      table.sort(self._friendListData, function(a, b)
        return tonumber(a.chips) > tonumber(b.chips)
      end)
      self:setListData(self._friendListData)
    else
      table.sort(self._friendListData, function(a, b)
        return tonumber(a.exp) > tonumber(b.exp)
      end)
      self:setListData(self._friendListData)
    end
    self:_hideMiniLoading()
  end
end
function RankBoardView:doRequestChartsDataSuccessCallback(requestType, retData)
  self:_hideMiniLoading()
  self._chartsInfoCacheData = retData
  self:setListData(self._chartsInfoCacheData)
end
function RankBoardView:doRequestChartsDataFailCallback()
  self:_hideMiniLoading()
  self._friendListRequstID = nil
end
function RankBoardView:onExit()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
end
return RankBoardView

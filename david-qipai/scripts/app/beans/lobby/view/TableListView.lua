local LobbyConfig = require("app.beans.lobby.view.LobbyConfig")
local TableItemView = import(".TableItemView")
local PaymentView = require("app.beans.payment.view.PaymentView")
local TableListView = class("TableListView", function()
  return display.newNode()
end)
TableListView.PLAYER_LIMIT_SELECTED = 1
TableListView.ROOM_LEVEL_SELECTED = 1
local CHIP_HORIZONTAL_DISTANCE = 220 * g.scaleWidth
local CHIP_VERTICAL_DISTANCE = 220 * g.scaleHeight
TableListView.TOP_BUTTOM_Y_OFFSET = 7
TableListView.TOP_TAB_BAR_Y_OFFSET = 64
TableListView.TOP_TAB_BAR_HEIGHT = 50
TableListView.SPELINE_TOPBAR_GAP = 50
function TableListView:ctor(showMainLobbyBlock)
  self.showMainLobbyBlock = showMainLobbyBlock
  self:setNodeEventEnabled(true)
  self._userOnlineCountLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.LIGHT,
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, display.cy - TableListView.TOP_TAB_BAR_Y_OFFSET - TableListView.TOP_TAB_BAR_HEIGHT - TableListView.SPELINE_TOPBAR_GAP * 0.5):opacity(0):addTo(self)
  self._seperateLine = display.newSprite("#common_tablelist_sepline.png"):pos(0, display.cy - TableListView.TOP_TAB_BAR_Y_OFFSET - TableListView.TOP_TAB_BAR_HEIGHT - TableListView.SPELINE_TOPBAR_GAP):opacity(0):addTo(self)
  self._seperateLine:setScaleX(g.scaleWidth)
  self._topBtnNode = display.newNode():addTo(self)
  self._transparentBack = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#transparent.png"
  }, {scale9 = true}):addTo(self._topBtnNode):onButtonClicked(clickHandler(self, self._onReturnBtnClick))
  self._transparentBack:setButtonSize(90, 90)
  self._transparentBack:pos(-display.cx + 62, display.cy - 42)
  cc.ui.UIPushButton.new({
    normal = "#common_back_normal.png",
    pressed = "#common_back_pressed.png"
  }, {scale9 = false}):pos(-display.cx + 62, display.cy - 42):addTo(self._topBtnNode):onButtonClicked(clickHandler(self, self._onReturnBtnClick))
  self._transparentShop = cc.ui.UIPushButton.new({
    normal = "#transparent.png",
    pressed = "#transparent.png"
  }, {scale9 = true}):addTo(self._topBtnNode):onButtonClicked(clickHandler(self, self._onReturnBtnClick))
  self._transparentShop:setButtonSize(90, 90)
  self._transparentShop:pos(display.cx - 62, display.cy - 42)
  cc.ui.UIPushButton.new({
    normal = "#common_recharge_normal.png",
    pressed = "#common_recharge_pressed.png"
  }, {scale9 = false}):pos(display.cx - 62, display.cy - 42):addTo(self._topBtnNode):onButtonClicked(clickHandler(self, self._onPaymentBtnclick))
  TableListView.ROOM_LEVEL_SELECTED = tonumber(g.vars.user.defaultTableLevel or 1)
  self._topTableLevelTabBar = g.ui.TabGroupWithSlider.new({
    background = "#common_standard_greybg_06.png",
    slider = "#common_standard_bluebg_05.png"
  }, g.lang:getContent("lobby.tableType"), {
    selectedText = {
      color = g.font.color.LIGHT,
      size = 24
    },
    defaltText = {
      color = ccc3(39, 113, 193),
      size = 24
    }
  }, true, true):pos(0, display.cy - TableListView.TOP_TAB_BAR_Y_OFFSET - TableListView.TOP_TAB_BAR_HEIGHT * 0.5):addTo(self._topBtnNode)
  self._topTableLevelTabBar:setTabGroupSize(452, TableListView.TOP_TAB_BAR_HEIGHT, -4, -4)
  self._topTableLevelTabBar:bindTabClickHandler(handler(self, self._onTableLevelTabChange))
  self._tablePersonLimitTab = g.ui.TabGroupWithSlider.new({
    background = "#common_standard_greybg_06.png",
    slider = "#common_person_slider_select.png"
  }, g.lang:getContent("lobby.tablePersonLimit"), {
    selectedText = {
      color = g.font.color.WHITE
    },
    defaltText = {
      color = ccc3(39, 113, 193),
      size = 26
    }
  }, true, true, g.ui.TabGroupWithSlider.VERTICAL):pos(display.cx - 72, -CHIP_VERTICAL_DISTANCE * 0.5 + 50 * g.scaleHeight):addTo(self)
  self._tablePersonLimitTab:setTabGroupSize(50, 240, -4, -4)
  self._tablePersonLimitTab:bindTabClickHandler(handler(self, self._onTablePersonLimitTabChange))
  self._chipButtonsNode = display.newNode():addTo(self)
  self._tablesBtn = {}
  for i = 1, 6 do
    self._tablesBtn[i] = TableItemView.new(i):pos(((i - 1) % 3 - 1) * CHIP_HORIZONTAL_DISTANCE, (math.floor((6 - i) / 3) - 1) * CHIP_VERTICAL_DISTANCE - 81):addTo(self._chipButtonsNode):hide()
  end
  self._topTableLevelTabBar:tabOn(TableListView.ROOM_LEVEL_SELECTED, false)
  self._tablePersonLimitTab:tabOn(TableListView.PLAYER_LIMIT_SELECTED, false)
end
function TableListView:playShowAnim()
  local animTime = LobbyConfig.SHOWUP_TABLELIST_ANIMATION_TIME
  self._topBtnNode:setCascadeOpacityEnabled(true)
  self._topTableLevelTabBar:setCascadeOpacityEnabled(true)
  self._topBtnNode:setOpacity(0)
  transition.fadeIn(self._topBtnNode, {
    opacity = 255,
    time = 0.4,
    delay = animTime
  })
  transition.fadeIn(self._seperateLine, {
    time = animTime,
    opacity = 255,
    delay = animTime
  })
  transition.fadeIn(self._userOnlineCountLabel, {
    time = animTime,
    opacity = 255,
    delay = animTime,
    onComplete = function()
      if self._miniLoading then
        self._miniLoading:show()
      end
      self._pokerDecorate = display.newSprite("#mainlobby_play_icon.png"):pos(-display.cx - 30, -display.cy + 150):rotation(30):opacity(128):scale(1.2):addTo(self)
      transition.moveTo(self._pokerDecorate, {
        time = 0.25,
        x = -display.cx + 50,
        y = -display.cy + 175,
        easing = "SINEOUT"
      })
      transition.rotateTo(self._pokerDecorate, {time = 0.25, rotate = 45})
    end
  })
  for i, tableBtn in ipairs(self._tablesBtn) do
    local delayTime = 0
    if i <= 3 then
      delayTime = delayTime + 0.4
    else
      delayTime = delayTime + 0.5
    end
    transition.moveTo(tableBtn, {
      time = animTime,
      y = (math.floor((6 - i) / 3) - 1) * CHIP_VERTICAL_DISTANCE + 70 * g.scaleHeight,
      delay = delayTime,
      easing = "BACKOUT"
    })
    tableBtn:setCascadeOpacityEnabled(true)
    tableBtn:setOpacity(0)
    transition.fadeIn(tableBtn, {
      opacity = 255,
      time = animTime,
      delay = delayTime
    })
  end
  self._tablePersonLimitTab:setCascadeOpacityEnabled(true)
  self._tablePersonLimitTab:setOpacity(0)
  transition.fadeIn(self._tablePersonLimitTab, {
    opacity = 255,
    time = 0.4,
    delay = animTime
  })
end
function TableListView:playHideAnim()
  self:removeFromParent()
end
function TableListView:_onReturnBtnClick()
  self.showMainLobbyBlock()
  self:playHideAnim()
end
function TableListView:_onSearchClick()
  HallSearchRoomPanel.new(self.controller_):showPanel()
end
function TableListView:_onPaymentBtnclick()
  PaymentView.new():show()
end
function TableListView:_onTableLevelTabChange(selectedTab)
  TableListView.ROOM_LEVEL_SELECTED = selectedTab
  self:doRequest(false)
end
function TableListView:_onTablePersonLimitTabChange(selectedTab)
  TableListView.PLAYER_LIMIT_SELECTED = selectedTab
  self:doRequest(false)
end
function TableListView:onEnter()
  self:doRequest(true)
end
function TableListView:onExit()
  if self.request ~= nil then
    self.request:cancel()
    self.request = nil
  end
end
function TableListView:doRequest(onlyFetchOnlineData)
  self:_showMiniLoading()
  function successCallback(tableListInfo)
    self.request = nil
    if self._hideMiniLoading == nil then
      return
    end
    self:_hideMiniLoading()
    if tableListInfo then
      local totalPlayer = 0
      local tableItemsInfo = tableListInfo.online[TableListView.ROOM_LEVEL_SELECTED][TableListView.PLAYER_LIMIT_SELECTED]
      for i, table in ipairs(self._tablesBtn) do
        if tableItemsInfo and tableItemsInfo[i] then
          local tableInfo = tableItemsInfo[i]
          table:bindData(tableInfo)
          table:show()
          totalPlayer = totalPlayer + tonumber(tableInfo.online or 0)
        else
          table:hide()
        end
      end
      local onlineCount = string.gsub(g.lang:getContent("lobby.tableUserOnLine"), "{1}", g.util.functions.formatBigInt(totalPlayer))
      self._userOnlineCountLabel:setString(onlineCount)
    end
  end
  local failCallback = handler(self, function()
    self.request = nil
    if self._hideMiniLoading == nil then
      return
    end
    self:_hideMiniLoading()
  end)
  if onlyFetchOnlineData then
    self.request = g.tablesInfo:refreshData(successCallback, failCallback)
  else
    self.request = g.tablesInfo:getCacheData(successCallback, failCallback)
  end
end
function TableListView:_showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():addTo(self):hide()
  end
end
function TableListView:_hideMiniLoading()
  if self._miniLoading then
    self._miniLoading:removeFromParent()
    self._miniLoading = nil
  end
end
return TableListView

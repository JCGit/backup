local GiftShopView = class("GiftShopView", g.ui.Panel)
local GiftListItem = import(".GiftListItem")
local PropListItem = import(".PropListItem")
local GiftListPanel = import(".GiftListPanel")
local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local PADDING = 16
local SUB_TAB_SPACE = 72
local VIEW_WIDTH = 720
local VIEW_HEIGHT = 480
local LIST_WIDTH = 698
local LIST_HEIGHT = 244
local BOTTOM, LEFT, RIGHT
function GiftShopView:ctor(tab)
  self:setNodeEventEnabled(true)
  GiftShopView.super.ctor(self, VIEW_WIDTH, VIEW_HEIGHT)
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  BOTTOM = -self._height * 0.5
  LEFT = -self._width * 0.5
  RIGHT = self._width * 0.5
  self:initialize()
  self:addCloseButton()
  if tab == 1 then
    local giftData = g.giftInfo:getGiftData()
    if giftData then
      self._tabGroup:tabOn(tab)
    else
      self._defaultTab = tab
      self._giftShopNode:show()
      self._propShopNode:hide()
    end
  elseif tab == 2 then
    local propData = g.giftInfo:getPropData()
    if propData then
      self._tabGroup:tabOn(tab)
    else
      self._defaultTab = tab
      self._giftShopNode:hide()
      self._propShopNode:show()
    end
  end
end
function GiftShopView:initialize()
  self._tabGroup = g.ui.TabGroup.new({
    windowWidth = 720,
    iconOffsetX = 10,
    iconTexture = {
      {
        "#gift_icon_selected.png",
        "#gift_icon_unselected.png"
      },
      {
        "#prop_icon_selected.png",
        "#prop_icon_unselected.png"
      }
    },
    buttonTexts = g.lang:getContent("shop.tabGroupLabels")
  }):pos(0, self._height * 0.5 - g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5 - 19):addTo(self)
  self._tabGroup:bindTabClickHandler(handler(self, self._onTabPanelChange))
  self._listPosY = -g.ui.TabGroup.TAB_GROUP_HEIGHT * 0.5
  self._giftShopNode = display.newNode():addTo(self):hide()
  self._giftList = GiftListPanel.new({
    bgMargin = cc.size(0, 8),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-LIST_WIDTH * 0.5, -LIST_HEIGHT * 0.5, LIST_WIDTH, LIST_HEIGHT),
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL
  }, GiftListItem):pos(0, self._listPosY):addTo(self._giftShopNode)
  self._buyButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png"
  }, {scale9 = true}):setButtonSize(200, 55):setButtonLabel("normal", ui.newTTFLabel({
    text = g.lang:getContent("shop.buyLabel"),
    size = 26,
    color = ccc3(50, 88, 0),
    align = ui.TEXT_ALIGN_CENTER
  })):pos(0, BOTTOM + 48):onButtonClicked(clickHandler(self, self.onBuyClick)):addTo(self)
  logger:debug("pos(RIGHT - 120, BOTTOM + 48)")
  self._sendButton = cc.ui.UIPushButton.new({
    normal = "#common_blue_btn_up.png",
    pressed = "#common_blue_btn_down.png"
  }, {scale9 = true}):setButtonSize(200, 55):setButtonLabel("normal", ui.newTTFLabel({
    text = g.lang:getContent("shop.sendLabel"),
    size = 24,
    color = ccc3(0, 98, 115),
    align = ui.TEXT_ALIGN_CENTER
  })):pos(-120, BOTTOM + 48):onButtonClicked(clickHandler(self, self._onSendClick)):addTo(self._giftShopNode)
  self._subTabGroupShop = g.ui.TabGroupWithSlider.new({
    background = "#common_standard_greybg_04.png",
    slider = "#common_standard_bluebg_05.png"
  }, g.lang:getContent("shop.subTabGroupLabels"), {
    selectedText = {
      color = g.font.color.LIGHT,
      size = 26
    },
    defaltText = {
      color = ccc3(139, 184, 220),
      size = 26
    }
  }, true, true)
  self._subTabGroupShop:addTo(self._giftShopNode)
  self._subTabGroupShop:setTabGroupSize(640, 44, -4, -6)
  self._subTabGroupShop:pos(0, self._height * 0.5 - g.ui.TabGroup.TAB_GROUP_HEIGHT - SUB_TAB_SPACE * 0.5 - 19)
  self._subTabGroupShop:bindTabClickHandler(handler(self, self._onGiftSubTabChange))
  self._propShopNode = display.newNode():addTo(self):hide()
  self._propList = GiftListPanel.new({
    bgMargin = cc.size(0, 8),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-LIST_WIDTH * 0.5, -LIST_HEIGHT * 0.5, LIST_WIDTH, LIST_HEIGHT),
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL
  }, PropListItem):pos(0, self._listPosY):addTo(self._propShopNode)
  self._subTabGroupProp = g.ui.TabGroupWithSlider.new({
    background = "#common_standard_greybg_04.png",
    slider = "#common_standard_bluebg_05.png"
  }, g.lang:getContent("shop.subTabGroupMineLabels"), {
    selectedText = {
      color = g.font.color.LIGHT,
      size = 26
    },
    defaltText = {
      color = ccc3(139, 184, 220),
      size = 26
    }
  }, true, true)
  self._subTabGroupProp:addTo(self._propShopNode)
  self._subTabGroupProp:setTabGroupSize(560, 44, -4, -6)
  self._subTabGroupProp:pos(0, self._height * 0.5 - g.ui.TabGroup.TAB_GROUP_HEIGHT - SUB_TAB_SPACE * 0.5 - 19)
  self._subTabGroupProp:bindTabClickHandler(handler(self, self._onPropSubTabChange))
end
function GiftShopView:_onTabPanelChange(selectedTab)
  self._mainSelectedTab = selectedTab
  self:_showMiniLoading()
  if selectedTab == 1 then
    self._giftShopNode:show()
    self._propShopNode:hide()
    g.giftInfo:loadGiftData(handler(self, self.showShopGiftView))
  elseif selectedTab == 2 then
    self._giftShopNode:hide()
    self._propShopNode:show()
    g.giftInfo:loadPropData(handler(self, self.showShopPropView))
  end
  if self._isInTable then
    self._sendButton:show()
    self._buyButton:pos(120, BOTTOM + 48)
  else
    self._sendButton:hide()
    self._buyButton:pos(0, BOTTOM + 48)
  end
end
function GiftShopView:showShopGiftView()
  self:_hideMiniLoading()
  if self._subTabGroupShopSelectedTab == nil then
    self._subTabGroupShop:tabOn(1, false)
    self._subTabGroupShopSelectedTab = true
  end
end
function GiftShopView:showShopPropView()
  self:_hideMiniLoading()
  if self._subTabGroupPropSelectedTab == nil then
    self._subTabGroupProp:tabOn(1, false)
    self._subTabGroupPropSelectedTab = true
  end
end
function GiftShopView:_showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():pos(0, self._listPosY):addTo(self)
  end
end
function GiftShopView:_hideMiniLoading()
  if self._miniLoading then
    self._miniLoading:removeFromParent()
    self._miniLoading = nil
  end
end
function GiftShopView:onShow()
  if self._giftList then
    self._giftList:setScrollContentTouchRect()
  end
  if self._defaultTab then
    self._tabGroup:tabOn(self._defaultTab)
  end
end
function GiftShopView:_onPropSubTabChange(selectedTab)
  self._subSelectedTab = selectedTab
  local empty = {}
  self:setPropListData(empty, -1)
  local groupData
  local propData, magicData, styleData = g.giftInfo:getPropData()
  self._propList:hideNoDataHintView()
  if selectedTab == 1 and magicData then
    if #magicData == 0 then
      self._propList:showNoDataHintView(g.lang:getContent("shop.noGiftLabel"))
      self:setPropListData(magicData, -1)
    else
      groupData = g.giftInfo:groupGiftData(magicData, 3)
      self:setPropListData(groupData)
    end
  elseif selectedTab == 2 and styleData then
    if #styleData == 0 then
      self._propList:showNoDataHintView(g.lang:getContent("shop.noGiftLabel"))
      self:setPropListData(styleData, -1)
    else
      groupData = g.giftInfo:groupGiftData(styleData, 3)
      self:setPropListData(groupData)
    end
  else
    self._propList:showNoDataHintView(g.lang:getContent("shop.noGiftLabel"))
  end
end
function GiftShopView:_onGiftSubTabChange(selectedTab)
  self._subSelectedTab = selectedTab
  local empty = {}
  self:setGiftListData(empty, -1)
  local groupData
  local giftData, hotGiftData, boutiqueGiftData, festivalGiftData, otherGiftData = g.giftInfo:getGiftData()
  self._giftList:hideNoDataHintView()
  if selectedTab == 1 and hotGiftData then
    if #hotGiftData == 0 then
      self._giftList:showNoDataHintView(g.lang:getContent("shop.noGiftLabel"))
      self:setGiftListData(hotGiftData, -1)
    else
      groupData = g.giftInfo:groupGiftData(hotGiftData, 5)
      self:setGiftListData(groupData)
    end
  elseif selectedTab == 2 and boutiqueGiftData then
    if #boutiqueGiftData == 0 then
      self._giftList:showNoDataHintView(g.lang:getContent("shop.noGiftLabel"))
      self:setGiftListData(boutiqueGiftData, -1)
    else
      groupData = g.giftInfo:groupGiftData(boutiqueGiftData, 5)
      self:setGiftListData(groupData)
    end
  elseif selectedTab == 3 and festivalGiftData then
    if #festivalGiftData == 0 then
      self._giftList:showNoDataHintView(g.lang:getContent("shop.noGiftLabel"))
      self:setGiftListData(festivalGiftData, -1)
    else
      groupData = g.giftInfo:groupGiftData(festivalGiftData, 5)
      self:setGiftListData(groupData)
    end
  elseif selectedTab == 4 and otherGiftData then
    if #otherGiftData == 0 then
      self._giftList:showNoDataHintView(g.lang:getContent("shop.noGiftLabel"))
      self:setGiftListData(otherGiftData, -1)
    else
      groupData = g.giftInfo:groupGiftData(otherGiftData, 5)
      self:setGiftListData(groupData)
    end
  else
    self._giftList:showNoDataHintView(g.lang:getContent("shop.noGiftLabel"))
  end
end
function GiftShopView:show(isInTable, sourceUser, toUsers, selfSeatId)
  GiftShopView.super.show(self, true, true, true, true)
  self._isInTable = isInTable
  self._sourceUser = sourceUser
  self._toUsers = toUsers
  self._selfSeatId = selfSeatId
  if self._isInTable then
    if self._sourceUser.uid == g.vars.user.uid then
      self._buyButton:setButtonLabel("normal", ui.newTTFLabel({
        text = g.lang:getContent("shop.buyLabel"),
        size = 24,
        color = ccc3(50, 88, 0),
        align = ui.TEXT_ALIGN_CENTER
      }))
    else
      self._buyButton:setButtonLabel("normal", ui.newTTFLabel({
        text = g.lang:getContent("shop.sendLabel"),
        size = 24,
        color = ccc3(50, 88, 0),
        align = ui.TEXT_ALIGN_CENTER
      }))
    end
    self._sendButton:setButtonLabel("normal", ui.newTTFLabel({
      text = string.gsub(g.lang:getContent("shop.sendAllLabel"), "{count}", #toUsers),
      size = 24,
      color = ccc3(0, 98, 115),
      align = ui.TEXT_ALIGN_CENTER
    }))
  end
  if self._isInTable then
    self._sendButton:show()
    self._buyButton:pos(120, BOTTOM + 48)
  else
    self._sendButton:hide()
    self._buyButton:pos(0, BOTTOM + 48)
  end
end
function GiftShopView:hideView()
  g.ui.manager.popup:removePopup(self)
end
function GiftShopView:onPopupRemove(callback)
  callback()
end
function GiftShopView:setPropListData(value, selectedId)
  self._propList:setData(value, nil)
  if selectedId then
    self._propList:selectGiftById(selectedId)
  else
    self._propList:selectGiftByIndex(1)
  end
end
function GiftShopView:setGiftListData(value, selectedId)
  self._giftList:setData(value, nil)
  if selectedId then
    self._giftList:selectGiftById(selectedId)
  else
    self._giftList:selectGiftByIndex(1)
  end
end
function GiftShopView:onBuyClick()
  if self._mainSelectedTab == 1 then
    if self._isInTable then
      if self._sourceUser.uid == g.vars.user.uid then
        self:buyGiftRequest()
      else
        self:sendGiftRequest({
          self._sourceUser
        })
      end
    else
      self:buyGiftRequest()
    end
  else
    self:buyPropRequest()
  end
end
function GiftShopView:buyPropRequest()
  if g.vars.shop.currentPropId == nil then
    return
  end
  local requestData = {}
  requestData.uid = g.vars.user.uid
  requestData.propId = g.vars.shop.currentPropId
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("items", "buy", requestData), function(data)
    local jsonData = json.decode(data)
    if jsonData.ret == 0 then
      g.ui.manager.tip:top(g.lang:getContent("shop.buyGiftSuccessTip"))
      self:hideView()
    else
      g.ui.manager.tip:top(g.lang:getContent("shop.buyGiftFailTip"))
    end
  end, function()
    g.ui.manager.tip:top(g.lang:getContent("shop.buyGiftFailTip"))
  end)
end
function GiftShopView:_onSendClick()
  self:sendGiftRequest(self._toUsers)
end
function GiftShopView:sendGiftRequest(toUsers)
  if g.vars.shop.currentGiftId == nil then
    return
  end
  local requestData = {}
  requestData.uid = g.vars.user.uid
  requestData.giftId = g.vars.shop.currentGiftId
  requestData.toUsers = json.encode(toUsers)
  requestData.quantity = 1
  if self._isInTable then
    requestData.isInTable = tostring(self._isInTable)
    requestData.seatId = self._selfSeatId
  end
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("gifts", "send", requestData), function(data)
    local jsonData = json.decode(data)
    if jsonData.ret == 0 then
      g.ui.manager.tip:top(g.lang:getContent("shop.sendGiftSuccessTip"))
      self:hideView()
    else
      g.ui.manager.tip:top(g.lang:getContent("shop.sendGiftFailTip"))
    end
  end, function()
    g.ui.manager.tip:top(g.lang:getContent("shop.sendGiftFailTip"))
  end)
end
function GiftShopView:buyGiftRequest()
  if g.vars.shop.currentGiftId == nil then
    return
  end
  local requestData = {}
  requestData.uid = g.vars.user.uid
  requestData.giftId = g.vars.shop.currentGiftId
  requestData.quantity = 1
  if self._isInTable then
    requestData.isInTable = tostring(self._isInTable)
    requestData.seatId = self._sourceUser.seatId
  end
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("gifts", "buy", requestData), function(data)
    local jsonData = json.decode(data)
    if jsonData.ret == 0 then
      g.vars.user.giftId = g.vars.shop.currentGiftId
      g.ui.manager.tip:top(g.lang:getContent("shop.buyGiftSuccessTip"))
      g.eventCenter:dispatchEvent({
        name = DataBeanEvent.EVT_USER_BUY_GIFT
      })
      self:hideView()
    else
      g.ui.manager.tip:top(g.lang:getContent("shop.buyGiftFailTip"))
    end
  end, function()
    g.ui.manager.tip:top(g.lang:getContent("shop.buyGiftFailTip"))
  end)
end
return GiftShopView

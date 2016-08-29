local InviteFriendsEventPanel = class("InviteFriendsEventPanel", g.ui.Panel)
local ChangeChipAnim = require("app.beans.activity.ChangeChipAnim")
local InviteItem = import(".InviteItem")
local FriendListView = require("app.beans.friend.view.FriendListView")
InviteFriendsEventPanel.WIDTH = 750
InviteFriendsEventPanel.HEIGHT = 480
InviteFriendsEventPanel.MID_BOARD_WIDTH = 680
InviteFriendsEventPanel.MID_BOARD_HEIGHT = 280
InviteFriendsEventPanel.LEFT = -InviteFriendsEventPanel.WIDTH * 0.5
InviteFriendsEventPanel.TOP = InviteFriendsEventPanel.HEIGHT * 0.5
InviteFriendsEventPanel.RIGHT = InviteFriendsEventPanel.WIDTH * 0.5
InviteFriendsEventPanel.BOTTOM = -InviteFriendsEventPanel.HEIGHT * 0.5
local CommonRewardChipAnimation = require("app.beans.activity.CommonRewardChipAnimation")
function InviteFriendsEventPanel:ctor()
  self._inviteItemDataModel = nil
  loadTexture("pd_invitefriends_texture.plist", "pd_invitefriends_texture.png")
  self._panelBG = display.newScale9Sprite("#common_invitefriends_board.png", 0, 0, cc.size(InviteFriendsEventPanel.WIDTH, InviteFriendsEventPanel.HEIGHT)):addTo(self)
  self._panelBG:setTouchEnabled(true)
  InviteFriendsEventPanel.super.ctor(self, InviteFriendsEventPanel.WIDTH, InviteFriendsEventPanel.HEIGHT)
  self:addCloseButton("#common_invitefriends_close_normal.png", "#common_invitefriends_close_press.png")
  if self._background ~= nil and self._background:getParent() ~= nil then
    self._background:removeFromParent()
  end
  self._titleLogo = display.newSprite("#common_invitefriends_title_bg.png"):pos(0, InviteFriendsEventPanel.TOP - 16):addTo(self)
  self._titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("invite.title"),
    color = g.font.color.GOLDEN,
    size = 30,
    align = ui.TEXT_ALIGN_LEFT
  }):addTo(self._titleLogo):pos(self._titleLogo:getContentSize().width * 0.5, self._titleLogo:getContentSize().height * 0.5 - 6)
  self.boardMid = display.newScale9Sprite("#common_invitefriends_board_mid.png", 0, 28, cc.size(InviteFriendsEventPanel.MID_BOARD_WIDTH, InviteFriendsEventPanel.MID_BOARD_HEIGHT)):addTo(self)
  self._ruleLabel = ui.newTTFLabel({
    text = g.lang:getContent("invite.rule"),
    color = g.font.color.LIGHT,
    size = 20,
    align = ui.TEXT_ALIGN_LEFT
  }):align(display.LEFT_CENTER, -340, -134):addTo(self)
  self._rule1Label = ui.newTTFLabel({
    text = g.lang:getContent("invite.rule1"),
    color = g.font.color.LIGHT,
    size = 20,
    align = ui.TEXT_ALIGN_LEFT,
    dimensions = cc.size(600, 60)
  }):addTo(self)
  local posX = self._ruleLabel:getPositionX() + self._ruleLabel:getContentSize().width
  self._rule1Label:align(display.LEFT_CENTER, posX, -148)
  self._inviteButton = cc.ui.UIPushButton.new({
    normal = "#common_invitefriends_attend_normal.png",
    pressed = "#common_invitefriends_attend_pressed.png"
  }):setButtonLabel(ui.newTTFLabel({
    text = g.lang:getContent("invite.attendActivity"),
    color = g.font.color.LIGHT,
    size = 26,
    align = ui.TEXT_ALIGN_CENTER
  })):pos(0, -204):addTo(self):onButtonClicked(clickHandler(self, self.onInviteButtonHandler))
  self._inviteButton:setTouchSwallowEnabled(false)
  self._inviteButton:hide()
  self:refreshData()
end
function InviteFriendsEventPanel:refreshData()
  self:_showMiniLoading()
  local function successCallback(retData)
    if self._hideMiniLoading == nil then
      return
    end
    self:_hideMiniLoading()
    self._inviteItemDataModel = retData.info
    self:bindData(self._inviteItemDataModel)
  end
  local function failCallback()
    if self._hideMiniLoading == nil then
      return
    end
    self:_hideMiniLoading()
  end
  g.inviteInfo:activityProgress(successCallback, failCallback)
end
function InviteFriendsEventPanel:bindData(inviteItemDataModel)
  self._items = {}
  for i, itemDataModel in ipairs(inviteItemDataModel.progress or {}) do
    local rewardHandler = handler(self, self.onRewardButtonHandler)
    local totalInvite = tonumber(inviteItemDataModel.inviteCount or 0)
    self._items[i] = InviteItem.new(rewardHandler, totalInvite, i):pos(InviteFriendsEventPanel.MID_BOARD_WIDTH * 0.5 - 220 + 220 * (i - 1), InviteFriendsEventPanel.MID_BOARD_HEIGHT * 0.5):addTo(self.boardMid):setData(itemDataModel)
    self._items[i].stage = i
  end
  self._inviteButton:show()
  if tonumber(inviteItemDataModel.applyStatus) == 0 then
    self._inviteButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("invite.attendActivity"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    }))
  elseif tonumber(inviteItemDataModel.applyStatus) == 1 then
    self._inviteButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("invite.inviteMore"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    }))
  end
end
function InviteFriendsEventPanel:showPanel()
  self:show(true, true, true, true)
end
function InviteFriendsEventPanel:onInviteButtonHandler()
  local function successCallback(retData)
    if retData ~= nil then
      g.ui.manager.tip:top(g.lang:getContent("invite.attendSuccess"))
    end
    self:onClose()
    local selectedTab = 2
    local friendListView = FriendListView.new(selectedTab)
    friendListView:showPanel()
  end
  local failCallback = function()
    g.ui.manager.tip:top(g.lang:getContent("common.requestDataFail"))
  end
  if tonumber(self._inviteItemDataModel.applyStatus) == 0 then
    g.inviteInfo:activityApply(successCallback, failCallback)
  else
    successCallback()
  end
end
function InviteFriendsEventPanel:onRewardButtonHandler(stage)
  local function successCallback(retData)
    if retData.info ~= nil then
      self._items[stage]:onResponseStateChage(2)
      local chipQuantity = 0
      local magicQuantity = 0
      for k, v in pairs(retData.info) do
        if k == "chips" then
          chipQuantity = v
          self:playRewardAnim(stage, chipQuantity)
        elseif k == "magic" then
          magicQuantity = v
        elseif k == "gift" then
        end
      end
      if chipQuantity ~= 0 and magicQuantity ~= 0 then
        local textString = string.gsub(g.lang:getContent("invite.rewardChipsAndMagicTips"), "{1}", chipQuantity)
        textString = string.gsub(textString, "{2}", magicQuantity)
        g.ui.manager.tip:top(textString)
      elseif chipQuantity ~= 0 then
        local textString = string.gsub(g.lang:getContent("invite.rewardChipsTips"), "{1}", chipQuantity)
        g.ui.manager.tip:top(textString)
      elseif magicQuantity ~= 0 then
        local textString = string.gsub(g.lang:getContent("invite.rewardMagicTips"), "{2}", magicQuantity)
        g.ui.manager.tip:top(textString)
      end
    end
  end
  local function failCallback(ret)
    if ret == -13 then
      g.ui.manager.tip:top(g.lang:getContent("invite.invitationNoEnough"))
      self._items[stage]:onResponseStateChage(0)
    else
      g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
    end
  end
  g.inviteInfo:activityReward(stage, successCallback, failCallback)
end
function InviteFriendsEventPanel:playRewardAnim(stage, totalChipsReward)
  local srcPtX, srcPtY = self._items[stage]:getPosition()
  srcPtX = srcPtX - 340
  srcPtY = srcPtY - 110
  if self._animation ~= nil then
    return
  end
  self._animation = CommonRewardChipAnimation.new(ccp(srcPtX, srcPtY), ccp(display.c_right - 160, display.c_bottom + 20)):addTo(self)
  g.audio:playSound("REWARD_GET")
  self:performWithDelay(handler(self, function(self)
    self._changeChipAnim = ChangeChipAnim.new(totalChipsReward):addTo(self:getParent(), 16):pos(display.right - 160, 10)
  end), 0.8)
  self:performWithDelay(handler(self, function(self)
    if self._animation and self._animation:getParent() then
      self._animation:removeFromParent()
      self._animation = nil
    end
    if self._changeChipAnim and self._changeChipAnim:getParent() then
      self._changeChipAnim:removeFromParent()
      self._changeChipAnim = nil
    end
  end), 1.5)
end
function InviteFriendsEventPanel:onExit()
  if self._animation and self._animation:getParent() then
    self._animation:removeFromParent()
    self._animation = nil
    self:hide()
  end
  if self._changeChipAnim and self._changeChipAnim:getParent() then
    self._changeChipAnim:removeFromParent()
    self._changeChipAnim = nil
  end
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
end
return InviteFriendsEventPanel

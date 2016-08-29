local UserBankruptPopup = class("UserBankruptPopup", g.ui.Panel)
local PaymentView = require("app.beans.payment.view.PaymentView")
local FriendListView = require("app.beans.friend.view.FriendListView")
UserBankruptPopup.WIDTH = 480
UserBankruptPopup.HEIGHT = 300
function UserBankruptPopup:ctor()
  UserBankruptPopup.super.ctor(self, UserBankruptPopup.WIDTH, UserBankruptPopup.HEIGHT)
  self:setNodeEventEnabled(true)
  local TOP = self._height * 0.5
  local BOTTOM = -self._height * 0.5
  local LEFT = -self._width * 0.5
  local RIGHT = self._width * 0.5
  local TOP_HEIGHT = 64
  self:enableBGHightLight()
  self._titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("bankrupt.bankruptTitle"),
    size = 30,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, TOP - 30):addTo(self)
  self._textAreaBG = display.newScale9Sprite("#common_standard_greybg_03.png", 0, 0, CCSize(UserBankruptPopup.WIDTH - 32, 160)):pos(0, 10):addTo(self)
  self._chipIcon = display.newSprite("#common_chips_set_icon_04.png"):pos(LEFT + 80, 5):addTo(self)
  self._rewardDescLabel = ui.newTTFLabel({
    text = "",
    size = 18,
    color = g.font.color.LIGHT,
    dimensions = CCSize(self._textAreaBG:getContentSize().width - 140, 80),
    align = ui.TEXT_ALIGN_LEFT
  }):pos(LEFT + 90 + 210, 50):addTo(self)
  self._buyChipLabel = ui.newTTFLabel({
    text = "",
    size = 18,
    color = g.font.color.LIGHT,
    dimensions = CCSize(self._textAreaBG:getContentSize().width - 140, 80),
    align = ui.TEXT_ALIGN_LEFT
  }):pos(LEFT + 90 + 210, -30):addTo(self)
  self._payButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png"
  }, {scale9 = true}):setButtonSize(200, 62):pos(0, BOTTOM + 43):addTo(self)
  self:addCloseButton()
end
function UserBankruptPopup:showPanel(times, subsidizeChips)
  self:show()
  if times ~= nil and times > 0 and times < 4 and subsidizeChips ~= nil then
    if times == 1 then
      local promt = string.gsub(g.lang:getContent("bankrupt.firstSupportHint"), "{1}", subsidizeChips)
      self._rewardDescLabel:setString(promt)
      self._buyChipLabel:setString(g.lang:getContent("bankrupt.firstSupportSip"))
    elseif times == 2 then
      local promt = string.gsub(g.lang:getContent("bankrupt.secondSupportHint"), "{1}", subsidizeChips)
      self._rewardDescLabel:setString(promt)
      self._buyChipLabel:setString(g.lang:getContent("bankrupt.secondSupportSip"))
    elseif times == 3 then
      local promt = string.gsub(g.lang:getContent("bankrupt.thirdSupportHint"), "{1}", subsidizeChips)
      self._rewardDescLabel:setString(promt)
      self._buyChipLabel:setString(g.lang:getContent("bankrupt.thirdSupportSip"))
    end
  else
    self._rewardDescLabel:setString(g.lang:getContent("bankrupt.noSupportHint"))
    self._buyChipLabel:setString(g.lang:getContent("bankrupt.buyChipPromt"))
  end
  if g.vars.user.loginType == "facebook" and (times == 1 or times == 2) then
    local descString = string.gsub(g.lang:getContent("lobby.inviteFriendItemDesc"), "{chips}", g.util.functions.formatBigInt(g.vars.user.rewards.info.facebookInviteSuccReward))
    self._payButton:setButtonLabel(ui.newTTFLabel({
      text = descString,
      size = 24,
      color = g.font.color.DARK,
      align = ui.TEXT_ALIGN_CENTER
    }))
    self._payButton:onButtonClicked(handler(self, self.inviteFBFriends))
  else
    self._payButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("shop.buyLabel"),
      size = 24,
      color = g.font.color.DARK,
      align = ui.TEXT_ALIGN_CENTER
    }))
    self._payButton:onButtonClicked(handler(self, self.onPaymentClick))
  end
end
function UserBankruptPopup:onPaymentClick()
  self:onClose()
  PaymentView.new():show()
end
function UserBankruptPopup:inviteFBFriends()
  self:onClose()
  local selectedTab = 2
  local friendListView = FriendListView.new(selectedTab)
  friendListView:showPanel()
end
return UserBankruptPopup

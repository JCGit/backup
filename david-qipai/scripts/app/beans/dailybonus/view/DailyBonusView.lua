local DailyBonusView = class("DailyBonusView", g.ui.Panel)
local DayItem = require("app.beans.dailybonus.view.DayItem")
local ProgressItem = require("app.beans.dailybonus.view.ProgressItem")
local CommonRewardChipAnimation = require("app.beans.activity.CommonRewardChipAnimation")
local ChangeChipAnim = require("app.beans.activity.ChangeChipAnim")
local MessagePopup = require("app.beans.message.MessagePopup")
local VIEW_WIDTH = 855
local VIEW_HEIGHT = 516
function DailyBonusView:ctor()
  self._rewardData = g.vars.user.loginRewards
  DailyBonusView.super.ctor(self, VIEW_WIDTH, VIEW_HEIGHT)
  loadTexture("pd_dailybonus_texture.plist", "pd_dailybonus_texture.png")
  self:addCloseButton()
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  self._background = display.newScale9Sprite("#common_standard_greybg_03.png", 0, 0, CCSize(VIEW_WIDTH - 20, 404)):pos(0, 10):addTo(self)
  self._titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("dailybonus.title"),
    color = g.font.color.WHITE,
    size = 28,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, VIEW_HEIGHT * 0.5 - 29):addTo(self)
  self._progressBar = g.ui.ProgressBar.new("#common_golden_progress_bg.png", "#common_golden_progress_fill.png", {
    bgWidth = 700,
    bgHeight = 20,
    progressWidth = 12,
    progressHeight = 12
  }):pos(-397, 94):addTo(self):setValue(tonumber(self._rewardData.days or 0) / 7)
  local loginReward = tonumber(self._rewardData.loginReward or 0)
  local signReward = tonumber(self._rewardData.signReward or 0)
  local vipReward = tonumber(self._rewardData.vipReward or 0)
  local facebookReward = tonumber(self._rewardData.facebookReward or 0)
  local progressItemData = {}
  progressItemData[1] = 3
  progressItemData[2] = 5
  progressItemData[3] = 7
  for i = 1, #progressItemData do
    local progressItem = ProgressItem.new():pos(-100 + 210 * (i - 1), 108):addTo(self):setData({
      index = progressItemData[i],
      chips = self._rewardData.info.continuous[progressItemData[i]] + loginReward
    })
  end
  self._totalReward = loginReward + signReward + vipReward + facebookReward
  local facebookLabelBg = display.newScale9Sprite("#dailybonus_label_bg.png", 0, 0, CCSize(630, 36)):pos(-82, 184):addTo(self)
  local str = ""
  if facebookReward > 0 then
    str = string.gsub(g.lang:getContent("dailybonus.rewardFacebook"), "{chips}", string.formatnumberthousands(self._totalReward))
    str = string.gsub(str, "{fbChips}", string.formatnumberthousands(facebookReward))
  else
    str = string.gsub(g.lang:getContent("dailybonus.reward"), "{chips}", string.formatnumberthousands(self._totalReward))
    str = string.gsub(str, "{fbChips}", string.formatnumberthousands(g.vars.user.rewards.info.facebookLoginReward))
  end
  local rewardLabel = ui.newTTFLabel({
    text = str,
    color = g.font.color.LIGHT,
    size = 22,
    align = ui.TEXT_ALIGN_CENTER
  })
  rewardLabel:setAnchorPoint(ccp(0, 0.5))
  rewardLabel:pos(-378, 184):addTo(self)
  local signLabelBg = display.newScale9Sprite("#dailybonus_label_bg.png", 0, 0, CCSize(630, 36)):pos(-82, 16):addTo(self)
  local maxSignReward = tonumber(self._rewardData.info.continuous[#self._rewardData.info.continuous])
  str = string.gsub(g.lang:getContent("dailybonus.prompt"), "{chips}", string.formatnumberthousands(maxSignReward + loginReward))
  local signLabel = ui.newTTFLabel({
    text = str,
    color = g.font.color.LIGHT,
    size = 22,
    align = ui.TEXT_ALIGN_CENTER
  })
  signLabel:setAnchorPoint(ccp(0, 0.5))
  signLabel:pos(-378, 16):addTo(self)
  self._dayItems = {}
  for i = 1, 7 do
    local dayItem = DayItem.new():pos(-345 + (DayItem.WIDTH + 9) * (i - 1), -92):addTo(self)
    local itemData = {}
    local x = 1
    local x = 70 + (i - 4) * 138
    local y = 0
    local mass
    itemData.chips = tonumber(self._rewardData.info.continuous[i]) + loginReward
    itemData.index = i
    if i < tonumber(self._rewardData.days or 0) then
      itemData.status = 2
    elseif i == tonumber(self._rewardData.days or 0) then
      itemData.status = 1
    else
      itemData.status = 0
    end
    dayItem:setData(itemData)
    self._dayItems[i] = dayItem
  end
  local shareButtonTextKey
  if g.vars.user.loginType == "device" then
    shareButtonTextKey = "common.confirm"
  else
    shareButtonTextKey = "common.share"
  end
  self._shareButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonSize(200, 52):setButtonLabel(ui.newTTFLabel({
    text = g.lang:getContent(shareButtonTextKey),
    color = ccc3(50, 88, 0),
    size = 26,
    align = ui.TEXT_ALIGN_CENTER
  })):pos(0, -220):addTo(self):onButtonClicked(clickHandler(self, self.onShareButtonClick))
  self._shareButton:setTouchSwallowEnabled(false)
end
function DailyBonusView:onShow()
  self:playRewardAnim()
end
function DailyBonusView:onShareButtonClick()
  if g.vars.user.loginType == "device" then
    self:hide()
    return
  end
  if self._totalReward and self._totalReward > 0 then
    do
      local feedData = clone(g.lang:getContent("feed.dailybonus"))
      feedData.name = string.gsub(feedData.name, "{chips}", string.formatnumberthousands(self._totalReward))
      feedData.picture = g.vars.user.cdn .. feedData.picture
      local str
      g.facebook:shareFeed(feedData, function(success, result)
        if not success then
          self._shareButton:setButtonEnabled(true)
          str = g.lang:getContent("feed.shareFail")
          g.ui.manager.tip:top(str)
        else
          str = g.lang:getContent("feed.shareSucc")
          g.ui.manager.tip:top(str)
        end
      end)
    end
  end
  self:hide()
end
function DailyBonusView:playRewardAnim()
  local srcPtX, srcPtY
  if self._dayItems then
    srcPtX, srcPtY = self._dayItems[tonumber(self._rewardData.days)]:getPosition()
  end
  self._animation = CommonRewardChipAnimation.new(ccp(srcPtX, srcPtY), ccp(display.c_right - 160, display.c_bottom + 20)):addTo(self)
  g.audio:playSound("REWARD_GET")
  self:performWithDelay(handler(self, function(self)
    self._changeChipAnim = ChangeChipAnim.new(self._totalReward):addTo(self:getParent(), 16):pos(display.right - 160, 10)
    g.vars:syncChips()
  end), 0.8)
  self:performWithDelay(handler(self, function(self)
    if self._animation and self._animation:getParent() then
      self._animation:removeFromParent()
    end
    if self._changeChipAnim and self._changeChipAnim:getParent() then
      self._changeChipAnim:removeFromParent()
    end
  end), 1.5)
end
function DailyBonusView:showPanel()
  self:show(true, true, true, true)
end
function DailyBonusView:onPopupRemove(callback)
  g.vars.user.loginRewards = nil
  if g.vars.user.unreadMessage == 2 then
    MessagePopup.new():showPanel()
    g.vars.user.unreadMessage = 0
  end
  callback()
end
return DailyBonusView

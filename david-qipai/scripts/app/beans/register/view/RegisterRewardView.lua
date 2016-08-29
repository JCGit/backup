local RegisterRewardView = class("RegisterRewardView", g.ui.Panel)
local ChangeChipAnim = require("app.beans.activity.ChangeChipAnim")
local DayItem = import(".DayItem")
RegisterRewardView.WIDTH = 745
RegisterRewardView.HEIGHT = 425
RegisterRewardView.LEFT = -RegisterRewardView.WIDTH * 0.5
RegisterRewardView.TOP = RegisterRewardView.HEIGHT * 0.5
RegisterRewardView.RIGHT = RegisterRewardView.WIDTH * 0.5
RegisterRewardView.BOTTOM = -RegisterRewardView.HEIGHT * 0.5
local CommonRewardChipAnimation = require("app.beans.activity.CommonRewardChipAnimation")
function RegisterRewardView:ctor()
  loadTexture("pd_register_texture.plist", "pd_register_texture.png")
  RegisterRewardView.super.ctor(self, RegisterRewardView.WIDTH, RegisterRewardView.HEIGHT, {
    background = "#register_panel_bg.png"
  })
  self._titleBgLeft = display.newSprite("#register_title_bg_left.png"):pos(0, RegisterRewardView.TOP - 37):addTo(self)
  self._titleBgRight = display.newSprite("#register_title_bg_left.png"):pos(0, RegisterRewardView.TOP - 37):addTo(self)
  self._titleBgRight:setScaleX(-1)
  self.titleBgLeftWidth = self._titleBgLeft:getContentSize().width
  self._titleBgLeft:setAnchorPoint(ccp(1, 0))
  self._titleBgLeft:pos(1, RegisterRewardView.TOP - 10)
  self._titleBgRight:setAnchorPoint(ccp(1, 0))
  self._titleBgRight:pos(-1, RegisterRewardView.TOP - 10)
  self._titleDecorate = display.newSprite("#register_title_decorate.png"):pos(0, RegisterRewardView.TOP - 28):addTo(self)
  self._titleLabel = display.newSprite("#register_title_label.png"):pos(0, 0.5 * RegisterRewardView.HEIGHT + 30):addTo(self)
  display.newScale9Sprite("#common_standard_greybg_01.png", 0, 5, cc.size(694, 282)):addTo(self)
  self._rewardGetButton = cc.ui.UIPushButton.new({
    normal = "#register_sign_button_up.png",
    pressed = "#register_sign_button_down.png"
  }, {scale9 = true}):pos(0, RegisterRewardView.BOTTOM + 6):addTo(self):onButtonClicked(clickHandler(self, self.onRewardButtonHandler))
  self._rewardGetButton:setButtonSize(200, 74)
  self.rewardButtonLabel = ui.newTTFLabel({
    text = g.lang:getContent("registerReward.getReward"),
    color = ccc3(169, 11, 26),
    size = 32,
    align = ui.TEXT_ALIGN_CENTER
  })
  self._rewardGetButton:setButtonLabel(self.rewardButtonLabel)
  self._dots = {}
  local xBase = 0
  local xPos = {
    xBase - 170,
    xBase - 149,
    xBase - 110,
    xBase - 67,
    xBase - 31,
    xBase,
    xBase + 31,
    xBase + 67,
    xBase + 110,
    xBase + 149,
    xBase + 170
  }
  local yBase = RegisterRewardView.TOP + 8
  local yPos = {
    yBase,
    yBase + 28,
    yBase + 48,
    yBase + 60,
    yBase + 66,
    yBase + 67,
    yBase + 66,
    yBase + 60,
    yBase + 48,
    yBase + 28,
    yBase
  }
  local frames = display.newFrames("register_title_light_%01d.png", 1, 2, true)
  for i = 1, 11 do
    local anim = display.newAnimation(frames, 0.5)
    self._dots[i] = display.newSprite():pos(xPos[i], yPos[i]):addTo(self)
    self._dots[i]:playAnimationForever(anim, 0.25 + 0.25 * math.pow(-1, i))
  end
  self:bindData()
end
function RegisterRewardView:bindData()
  local registerRewardsDays = tonumber(g.vars.user.registerRewards.days or 0)
  local registerRewardsInfo = g.vars.user.registerRewards.info.continuous
  local facebookReward = 0
  if g.vars.user.loginRewards ~= nil and g.vars.user.loginRewards.facebookReward ~= nil then
    facebookReward = tonumber(g.vars.user.loginRewards.facebookReward)
  end
  self._items = {}
  for i = 1, 3 do
    local totalTextString = ""
    if 0 < tonumber(registerRewardsInfo[i].chips) then
      local topTextString = string.gsub(g.lang:getContent("registerReward.chipsReward"), "{1}", registerRewardsInfo[i].chips)
      totalTextString = totalTextString .. topTextString
    end
    if g.vars.user.registerRewards.FBLoginDay ~= nil and g.vars.user.registerRewards.FBLoginDay[i].sign ~= nil then
      local FBRewardStander = g.vars.user.rewards.info.facebookLoginReward
      if tonumber(g.vars.user.registerRewards.FBLoginDay[i].sign) ~= 0 and FBRewardStander ~= nil then
        local middleTextString = string.gsub(g.lang:getContent("registerReward.chipsFBReward"), "{1}", FBRewardStander)
        totalTextString = totalTextString .. middleTextString
      end
    end
    if 0 < tonumber(registerRewardsInfo[i].magic) then
      local bottomTextString = string.gsub(g.lang:getContent("registerReward.magicReward"), "{1}", registerRewardsInfo[i].magic)
      totalTextString = totalTextString .. bottomTextString
    end
    local data = {}
    if i ~= registerRewardsDays then
      data.isSigned = true
    else
      data.isSigned = false
    end
    data.desc = totalTextString
    data.index = i
    self._items[i] = DayItem.new():pos(-232 + 232 * (i - 1), 8):addTo(self):setData(data)
  end
end
function RegisterRewardView:showPanel()
  self:show(true, true, true, true)
end
function RegisterRewardView:onRewardButtonHandler()
  local function successCallback(registerChipsReward)
    local totalChipsReward = registerChipsReward + g.vars:getLoginRewardCount()
    if totalChipsReward > 0 then
      g.vars:syncChips(handlerHelper(self, self.playRewardAnim, totalChipsReward))
    end
  end
  local function failCallback(ret)
    if ret == -6 then
      g.ui.manager.tip:top(g.lang:getContent("registerReward.rewardsAlreadyGet"))
      self:onClose()
    else
      g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
    end
  end
  g.loginInfo:getRegisterRewards(successCallback, failCallback)
end
function RegisterRewardView:playRewardAnim(totalChipsReward)
  local registerRewardsDays = tonumber(g.vars.user.registerRewards.days or 1)
  local srcPtX, srcPtY = self._items[registerRewardsDays]:getPosition()
  self._animation = CommonRewardChipAnimation.new(ccp(srcPtX, srcPtY), ccp(display.c_right - 160, display.c_bottom + 20)):addTo(self)
  g.audio:playSound("REWARD_GET")
  self:performWithDelay(handler(self, function(self)
    self._changeChipAnim = ChangeChipAnim.new(totalChipsReward):addTo(self:getParent(), 16):pos(display.right - 160, 10)
  end), 0.8)
  self:performWithDelay(handler(self, function(self)
    if self._animation and self._animation:getParent() then
      self._animation:removeFromParent()
      self:hide()
    end
    if self._changeChipAnim and self._changeChipAnim:getParent() then
      self._changeChipAnim:removeFromParent()
    end
  end), 1.5)
end
function RegisterRewardView:onExit()
  if self._animation and self._animation:getParent() then
    self._animation:removeFromParent()
    self:hide()
  end
  if self._changeChipAnim and self._changeChipAnim:getParent() then
    self._changeChipAnim:removeFromParent()
  end
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
end
function RegisterRewardView:onPopupRemove(callback)
  if self._dots then
    for i = 1, #self._dots do
      self._dots[i]:stopAllActions()
    end
  end
  callback()
end
return RegisterRewardView

local CommonRewardChipAnimation = require("app.beans.activity.CommonRewardChipAnimation")
local ChangeChipAnim = require("app.beans.activity.ChangeChipAnim")
local StarView = require("app.beans.levelup.view.StarView")
local LevelUpView = class("LevelUpView", g.ui.Panel)
function LevelUpView:ctor(level, lobbyShowMode)
  self.lobbyShowMode = lobbyShowMode
  self._width = 720
  self._height = 480
  self:setNodeEventEnabled(true)
  if level < 0 then
    level = 1
  elseif level > 99 then
    level = 99
  end
  self._level = tonumber(level)
  self:initialize()
end
function LevelUpView:initialize()
  loadTexture("pd_levelup_texture.plist", "pd_levelup_texture.png")
  local container = display.newNode():addTo(self)
  self._container = container
  local containerPadding = 50
  container:setTouchEnabled(true)
  container:pos(0, containerPadding)
  local bgWidth = 648
  local bgHeight = 276
  local flash = display.newSprite("#common_effect_bg_light.png"):pos(0, -30):scale(2):addTo(container)
  self._levelNode = display.newNode():addTo(container):pos(0, display.cy):hide()
  local lvWidth = 132
  local lvHeight = 106
  local lvMarginLeft = 25
  local lvMarginTop = -15
  display.newSprite("#levelup_lv.png"):addTo(self._levelNode):pos(-bgWidth / 2 + lvWidth + lvMarginLeft, lvMarginTop)
  self:createLevelNumberNode():addTo(self._levelNode):pos(-bgWidth / 2 + lvWidth * 3 / 2 + lvMarginLeft, lvMarginTop)
  local boxWidth = 274
  local boxHeight = 174
  local boxPadding = 15
  self._boxBUtton = cc.ui.UIPushButton.new({
    normal = "#levelup_box_close.png",
    pressed = "#levelup_box_close.png"
  }):onButtonPressed(function(event)
    self._boxBUtton:scale(0.9)
  end):onButtonRelease(function(event)
    self._boxBUtton:scale(1)
  end):addTo(container):onButtonClicked(clickHandler(self, self.onGetButotnClick))
  self._boxBUtton:pos(bgWidth / 2 + boxPadding - boxWidth / 2, display.cy + boxHeight):hide()
  self._boxOpenIcon = display.newSprite("#levelup_box_open.png")
  self._boxOpenIcon:addTo(container):pos(bgWidth / 2 + boxPadding - boxWidth / 2, boxPadding):hide()
  local rewardMarginTop = 60
  self._rewardLabel = ui.newTTFLabel({
    text = "",
    size = rewardTextSize,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(container):hide()
  local rewardLabelSize = self._rewardLabel:getContentSize()
  self._rewardLabel:pos(0, bgHeight / 2 - boxHeight - rewardLabelSize.height / 2 - rewardMarginTop)
  self._stars = {}
  self._starNode = display.newNode():addTo(container)
  self._spots = {}
  local texture = "pd_common_texture.png"
  if g.vars.isWebP then
    texture = "pd_common_texture.webp"
  end
  self._spotsNode = display.newBatchNode(texture):addTo(container)
  for i = 1, 30 do
    local xRatio = math.random() * 2 - 1
    local yRatio = math.random() * 2 - 1
    self._spots[i] = display.newSprite("#common_effect_spot_fade.png"):pos(xRatio * 300, yRatio * 200):scale(math.random()):opacity(math.random() * 255):addTo(self._spotsNode)
  end
  for i = 31, 60 do
    local xRatio = math.random() * 2 - 1
    local yRatio = math.random() * 2 - 1
    self._spots[i] = display.newSprite("#common_effect_spot_solid.png"):pos(xRatio * 300, yRatio * 200):scale(math.random()):opacity(math.random() * 255):addTo(self._spotsNode)
  end
  local btnWidth = 291
  local btnHeight = 59
  local btnMarginTop = 30
  self._getButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png"
  }, {scale9 = true}):setButtonSize(btnWidth, btnHeight):addTo(container):onButtonClicked(clickHandler(self, self.onGetButotnClick)):setButtonLabel(ui.newTTFLabel({
    text = g.lang:getContent("levelup.openLabel"),
    size = btnSize,
    color = ccc3(50, 88, 0),
    align = ui.TEXT_ALIGN_CENTER
  })):pos(0, -96 - rewardLabelSize.height - 29.5 - 30):scale(0):hide()
end
function LevelUpView:onShow()
  self:playShowUpAnimaition()
end
function LevelUpView:playShowUpAnimaition()
  local rewardLabelSize = self._rewardLabel:getContentSize()
  self._boxBUtton:show()
  self._levelNode:show()
  self._getButton:show()
  transition.moveTo(self._boxBUtton, {
    time = 0.3,
    y = 15,
    easing = "BACKOUT"
  })
  transition.moveTo(self._levelNode, {
    time = 0.3,
    delay = 0.2,
    y = -15,
    easing = "BACKOUT"
  })
  transition.scaleTo(self._getButton, {
    time = 0.3,
    delay = 0.4,
    scale = 1,
    easing = "BACKOUT"
  })
  self._boxBUtton:runAction(CCRepeatForever:create(transition.sequence({
    CCRotateTo:create(0.08, 1),
    CCRotateTo:create(0.08, -1)
  })))
end
function LevelUpView:createLevelNumberNode()
  local numberNode = display.newNode()
  local width = 78
  local height = 106
  local num2 = self._level % 10
  local num2Node = display.newSprite("#levelup_" .. num2 .. ".png"):addTo(numberNode)
  num2Node:setAnchorPoint(ccp(0, 0.5))
  if self._level >= 10 then
    local num1 = math.floor(self._level / 10)
    local num1Node = display.newSprite("#levelup_" .. num1 .. ".png"):addTo(numberNode)
    num1Node:setAnchorPoint(ccp(0, 0.5))
    num2Node:pos(width - 25, 0)
  end
  return numberNode
end
function LevelUpView:onGetButotnClick(evt)
  if self._isRewarded then
    self:share()
    self:hide()
  else
    self:_showMiniLoading()
    g.levelInfo:getLevelUpReward(self._level, handler(self, self.onRewardLoaded), handler(self, function(self)
      self:_hideMiniLoading()
    end))
  end
end
function LevelUpView:onRewardLoaded(data)
  self:_hideMiniLoading()
  self._data = data
  self._boxBUtton:hide()
  self._boxOpenIcon:show()
  self:playOpenAnimation()
  local text
  if self._data.magic and self._data.magic > 0 then
    text = string.gsub(g.lang:getContent("levelup.getReward"), "{chips}", data.chips)
    text = string.gsub(text, "{magic}", data.magic)
  else
    text = string.gsub(g.lang:getContent("levelup.getRewardChips"), "{chips}", data.chips)
  end
  self._rewardLabel:show()
  self._rewardLabel:setString(text)
  self._getButton:setButtonLabelString(g.lang:getContent("common.share"))
  self._isRewarded = true
end
function LevelUpView:playOpenAnimation()
  local srcPtX, srcPtY = self._boxOpenIcon:getPosition()
  self._animation = CommonRewardChipAnimation.new(ccp(srcPtX - 13, srcPtY + 50), ccp(display.c_right - 160, display.c_bottom + 20)):addTo(self)
  g.audio:playSound("REWARD_GET")
  if self.lobbyShowMode ~= 2 then
    self:performWithDelay(handler(self, function(self)
      self._changeChipAnim = ChangeChipAnim.new(self._data.chips):addTo(self:getParent(), 16):pos(display.right - 160, 10)
    end), 0.8)
  end
  self:performWithDelay(handler(self, function(self)
    if self._animation and self._animation:getParent() then
      self._animation:removeFromParent()
      self._animation = nil
    end
    if self._changeChipAnim and self._changeChipAnim:getParent() then
      self._changeChipAnim:removeFromParent()
      self._changeChipAnim = nil
    end
    if g.vars.user.loginType == "device" then
      self:hide()
      return
    end
  end), 1.5)
  srcPt = ccp(srcPtX - 13, srcPtY + 50)
  for i = 1, 12 do
    self._stars[i] = StarView.new("#common_star_" .. math.random(1, 3) .. ".png"):pos(srcPt.x, srcPt.y):addTo(self._starNode):play(math.random() / 2, 0.8, 1.6, 0.3)
    transition.moveTo(self._stars[i], {
      time = math.random() + 0.5,
      x = srcPt.x - math.random() * 300 + 150,
      y = srcPt.y + (math.random() * 2 - 1) * 60 - 30
    })
  end
  for i = 1, 8 do
    self._stars[i] = StarView.new("#common_star_" .. math.random(1, 3) .. ".png"):pos(srcPt.x, srcPt.y):addTo(self._starNode):play(math.random() / 2, 0.8, 1.6, 0.3)
    transition.moveTo(self._stars[i], {
      time = math.random() + 0.5,
      x = srcPt.x - math.random() * 400,
      y = srcPt.y + (math.random() * 2 - 1) * 20 - 90
    })
  end
end
function LevelUpView:share()
  local feedData = clone(g.lang:getContent("feed.levelup"))
  feedData.name = string.gsub(feedData.name, "{chips}", string.formatnumberthousands(tonumber(self._data.chips)))
  feedData.name = string.gsub(feedData.name, "{level}", self._data.lv)
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
function LevelUpView:show()
  LevelUpView.super.show(self, true, true, true, true)
  return self
end
function LevelUpView:hide()
  LevelUpView.super.hide(self)
  return self
end
function LevelUpView:onPopupRemove(callback)
  for i = 1, #self._stars do
    self._stars[i]:stop()
  end
  if self._animation and self._animation:getParent() then
    self._animation:removeFromParent()
    self._animation = nil
  end
  if self._changeChipAnim and self._changeChipAnim:getParent() then
    self._changeChipAnim:removeFromParent()
    self._changeChipAnim = nil
  end
  self:getLevelUpInfo()
  unloadTexture("pd_levelup_texture.plist", "pd_levelup_texture.png")
  callback()
end
function LevelUpView:getLevelUpInfo()
  g.levelInfo:getLevelUpInfo(handler(self, function(self, data)
    if data and data.lv >= 3 then
      LevelUpView.new(data.lv, self.lobbyShowMode):show()
    end
  end))
end
return LevelUpView

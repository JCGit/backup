local RouletteShareView = class("RouletteShareView", g.ui.Panel)
RouletteShareView.WIDTH = 500
RouletteShareView.HEIGHT = 262
local shareColor = ccc3(73, 104, 6)
function RouletteShareView:ctor(index)
  local data = g.rouletteInfo.rouletteConfig[index + 1]
  self.data = data
  self:initialize()
end
function RouletteShareView:initialize()
  RouletteShareView.super.ctor(self, RouletteShareView.WIDTH, RouletteShareView.HEIGHT, {
    background = "#common_standard_redbg_01.png"
  })
  local titleBg = display.newSprite("#roulette_share_title_bg.png"):pos(0, RouletteShareView.HEIGHT * 0.5 - 20):addTo(self)
  local titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("luckyWheel.rewardTitle"),
    size = 26,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, RouletteShareView.HEIGHT * 0.5):addTo(self)
  local rewardLabel = ui.newTTFLabel({
    text = string.gsub(g.lang:getContent("luckyWheel.rewardDesc"), "{1}", self.data.desc),
    size = 22,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_CENTER,
    dimensions = CCSize(480, 60)
  }):addTo(self)
  rewardLabel:pos(0, RouletteShareView.HEIGHT * 0.5 - 64)
  local picSprite = display.newSprite(self.data.pic):addTo(self):scale(1.2)
  local picSize = picSprite:getContentSize()
  picSprite:pos(0, 0)
  local shareButtonBottom = 16
  local shareButtonWidth = 240
  local shareButtonHeight = 58
  local shareButtonTextKey
  if g.vars.user.loginType == "device" then
    shareButtonTextKey = "common.confirm"
  else
    shareButtonTextKey = "common.share"
  end
  self.shareButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    text = g.lang:getContent(shareButtonTextKey),
    size = 26,
    color = ccc3(73, 104, 6),
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonSize(shareButtonWidth, shareButtonHeight):addTo(self):pos(0, -RouletteShareView.HEIGHT / 2 + shareButtonHeight / 2 + shareButtonBottom):onButtonClicked(clickHandler(self, self._onShareButtonClick))
end
function RouletteShareView:_onShareButtonClick()
  if g.vars.user.loginType == "device" then
    self:hide()
    return
  end
  self.shareButton:setButtonEnabled(false)
  local feedData = clone(g.lang:getContent("feed.rouletteReward"))
  feedData.name = string.gsub(feedData.name, "{reward}", self.data.desc)
  feedData.picture = g.vars.user.cdn .. feedData.picture
  g.facebook:shareFeed(feedData, function(success, result)
    if not success then
      self.shareButton:setButtonEnabled(true)
      g.ui.manager.tip:top(g.lang:getContent("feed.shareFail"))
    else
      g.ui.manager.tip:top(g.lang:getContent("feed.shareSucc"))
    end
  end)
  self:hide()
end
function RouletteShareView:showPanel()
  self:show(true, true, true, true)
end
return RouletteShareView

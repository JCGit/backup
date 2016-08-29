local RouletteView = class("RouletteView", g.ui.Panel)
local RouletteWheelView = import(".RouletteWheelView")
RouletteView.WIDTH = 800
RouletteView.HEIGHT = 400
local largeSize = 36
local bigSize = 32
local middleSize = 24
local smallSize = 18
function RouletteView:ctor()
  RouletteView.super.ctor(self, RouletteView.WIDTH, RouletteView.HEIGHT)
  loadTexture("pd_roulette_texture.plist", "pd_roulette_texture.png")
  self:setNodeEventEnabled(true)
  self:initialize()
  self:_showMiniLoading()
end
function RouletteView:initialize()
  self:setTouchEnabled(true)
  self._panel = display.newSprite("#roulette_panel.png"):pos(-RouletteView.WIDTH * 0.5 + 242, 0):addTo(self)
  self._panel:setTouchEnabled(true)
  if self._background ~= nil and self._background:getParent() ~= nil then
    self._background:removeFromParent()
  end
  local titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("luckyWheel.title"),
    size = 26,
    color = ccc3(251, 214, 118),
    align = ui.TEXT_ALIGN_CENTER
  }):pos(-205, RouletteView.HEIGHT * 0.5 - 100):addTo(self)
  local wheelViewMarginLeft = 52
  local wheelViewWidth = 360
  local wheelViewHeight = 360
  self._rouletteView = RouletteWheelView.new(wheelViewWidth, wheelViewHeight, handler(self, self.playRouletteCallback)):addTo(self):pos(RouletteView.WIDTH / 2 - wheelViewWidth / 2 - wheelViewMarginLeft, RouletteView.HEIGHT / 2 - wheelViewHeight / 2)
  self._rouletteView:setPlayButtonEnable(false)
  local descLabel1Top = 8
  self.descLabel1 = ui.newTTFLabel({
    text = g.lang:getContent("luckyWheel.desc1"),
    size = smallSize,
    color = g.font.color.WHITE,
    align = ui.TEXT_ALIGN_CENTER,
    valign = ui.TEXT_VALIGN_CENTER,
    dimensions = cc.size(330, 0)
  }):pos(-210, RouletteView.HEIGHT * 0.5 - 163):addTo(self)
  local playCountLabelRight = 432
  local playCountLabelTop = 164
  local playCountLabelWidth = 330
  local playCountLabelHeight = 73
  local playCountPosX = RouletteView.WIDTH / 2 - playCountLabelWidth / 2 - playCountLabelRight
  local playCountPosY = RouletteView.HEIGHT / 2 - playCountLabelHeight / 2 - playCountLabelTop
  local playCountBg = display.newScale9Sprite("#transparent.png"):addTo(self):size(playCountLabelWidth, playCountLabelHeight):pos(playCountPosX, playCountPosY)
  playCountPosY = playCountPosY - playCountLabelHeight / 2
  self.playCountLabelContainer_ = display.newNode():addTo(playCountBg)
  local playCountLabelContainer = self.playCountLabelContainer_
  local playCountLabel1MarginLeft = 0
  local playCountLabel1 = ui.newTTFLabel({
    text = g.lang:getContent("luckyWheel.remainCount"),
    size = middleSize,
    color = g.font.color.WHITE,
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(playCountLabelContainer)
  local playCountLabel1Size = playCountLabel1:getContentSize()
  playCountLabel1:setAnchorPoint(ccp(0, 0))
  playCountLabel1:pos(playCountLabel1MarginLeft, (playCountLabelHeight - playCountLabel1Size.height) / 2)
  local playCountLabelMarginLeft = 8
  self.playCountLabel = ui.newTTFLabel({
    text = "...",
    size = middleSize,
    color = ccc3(251, 102, 0),
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(playCountLabelContainer)
  local playCountLabelSize = self.playCountLabel:getContentSize()
  self.playCountLabel:setAnchorPoint(ccp(0, 0))
  self.playCountLabel:pos(playCountLabel1:getPositionX() + playCountLabel1Size.width + playCountLabelMarginLeft, (playCountLabelHeight - playCountLabelSize.height) / 2)
  local playCountLabel2MarginLeft = 2
  local playCountLabel2 = ui.newTTFLabel({
    text = g.lang:getContent("luckyWheel.unit"),
    size = middleSize,
    color = g.font.color.WHITE,
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(playCountLabelContainer)
  local playCountLabel2Size = playCountLabel2:getContentSize()
  playCountLabel2:setAnchorPoint(ccp(0, 0))
  playCountLabel2:pos(self.playCountLabel:getPositionX() + playCountLabelSize.width + playCountLabel2MarginLeft, (playCountLabelHeight - playCountLabel2Size.height) / 2)
  local playCountLabelLength = playCountLabel1MarginLeft + playCountLabel1Size.width + playCountLabelMarginLeft + playCountLabelSize.width + playCountLabel2MarginLeft + playCountLabel2Size.width
  playCountLabelContainer:setPositionX((playCountLabelWidth - playCountLabelLength) / 2)
  self.descLabel2 = ui.newTTFLabel({
    text = string.gsub(g.lang:getContent("luckyWheel.desc2"), "{1}", "..."),
    size = smallSize,
    color = g.font.color.WHITE,
    align = ui.TEXT_ALIGN_CENTER,
    valign = ui.TEXT_VALIGN_CENTER,
    dimensions = cc.size(playCountLabelWidth, 0)
  }):addTo(self)
  local descLabelSize2 = self.descLabel2:getContentSize()
  playCountPosY = playCountPosY - descLabelSize2.height / 2 - descLabel1Top
  self.descLabel2:pos(playCountPosX - 12, playCountPosY + 16)
  self:getConfig()
end
function RouletteView:getPlayTimes()
  self:_showMiniLoading()
  function successCallback(data)
    if self._hideMiniLoading == nil then
      return
    end
    self:_hideMiniLoading()
    self.playCountLabel:setString(data.spin)
    if tonumber(data.spin) > 0 then
      self._rouletteView:setPlayButtonEnable(true)
    end
    local desLabelStr = string.gsub(g.lang:getContent("luckyWheel.desc2"), "{1}", data.chance)
    self.descLabel2:setString(desLabelStr)
  end
  function failCallback()
    if self._hideMiniLoading == nil then
      return
    end
    self:_hideMiniLoading()
    self.playCountLabel:setString("0")
    g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
  end
  g.rouletteInfo:getChances(successCallback, failCallback)
end
function RouletteView:getConfig()
  function successCallback(data)
    logger:debug(g.util.inspect(data))
    self._rouletteView:setItemData(data)
  end
  function failCallback()
    g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
  end
  g.rouletteInfo:getConfig(successCallback, failCallback)
end
function RouletteView:playRouletteCallback(retData)
  if tonumber(retData.spin) == 0 then
    self._rouletteView:setPlayButtonEnable(false)
  end
  self.playCountLabel:setString(retData.spin)
end
function RouletteView:showPanel()
  self:show(true, true, true, true)
end
function RouletteView:onShow()
  self:getPlayTimes()
end
function RouletteView:onCleanup()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
end
return RouletteView

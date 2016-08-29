local SettingAboutView = class("SettingAboutView", g.ui.Panel)
SettingAboutView.WIDTH = 480
SettingAboutView.HEIGHT = 300
local TOP_HEIGHT = 44
local CONTENT_WIDTH = 455
local CONTENT_HEIGHT = 225
local topTitleSize = 30
local topTitleColor = ccc3(100, 154, 201)
local contentSize = 22
local contentColor = ccc3(202, 202, 202)
local serviceSize = 14
local serviceColor = ccc3(100, 154, 201)
function SettingAboutView:ctor()
  SettingAboutView.super.ctor(self, SettingAboutView.WIDTH, SettingAboutView.HEIGHT)
  local titleMarginTop = 10
  local titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("setting.about"),
    size = topTitleSize,
    color = g.font.color.WHITE,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, SettingAboutView.HEIGHT / 2 - TOP_HEIGHT + TOP_HEIGHT / 2 - titleMarginTop):addTo(self)
  local divider = display.newScale9Sprite("#common_divider.png", 0, SettingAboutView.HEIGHT * 0.5 - 52, cc.size(240, 2)):addTo(self)
  local container = display.newNode():addTo(self)
  local contentMarginTop = 15
  local contentOriginY = SettingAboutView.HEIGHT / 2 - TOP_HEIGHT - contentMarginTop
  display.newScale9Sprite("#common_standard_greybg_03.png", 0, contentOriginY - CONTENT_HEIGHT / 2, cc.size(CONTENT_WIDTH, CONTENT_HEIGHT)):addTo(container)
  local contentLabelMarginTop = 15
  local contentLabelMarginLeft = 35
  local contentLabelPadding = 15
  local label = ui.newTTFLabel({
    text = string.gsub(g.lang:getContent("setting.aboutText"), "{version}", launchInfo.version),
    size = contentSize,
    color = contentColor,
    align = ui.TEXT_ALIGN_LEFT,
    dimensions = cc.size(450, 0)
  }):addTo(container)
  label:setAnchorPoint(ccp(0, 0.5))
  local contentLabelHeight = label:getContentSize().height
  label:pos(-SettingAboutView.WIDTH / 2 + contentLabelMarginLeft, contentOriginY - contentLabelHeight / 2 - contentLabelMarginTop)
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  self:addCloseButton()
end
function SettingAboutView:showPanel()
  self:show(true, true, true, true)
end
return SettingAboutView

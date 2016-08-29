local HelpView = class("HelpView", g.ui.Panel)
HelpView.TAB_FAQ = 1
HelpView.TAB_RULE = 2
HelpView.TAB_LEVEL = 3
local FAQItem = import(".FAQItem")
local RuleSubView = import(".RuleSubView")
local LevelItem = import(".LevelItem")
HelpView.WIDTH = 750
HelpView.HEIGHT = 480
function HelpView:ctor(defaultTab)
  HelpView.super.ctor(self, HelpView.WIDTH, HelpView.HEIGHT)
  self:setNodeEventEnabled(true)
  self.subTabViews = {}
  self._defaultTab = defaultTab or 1
  loadTexture("pd_card_texture.plist", "pd_card_texture.png")
  self:initialize()
end
function HelpView:initialize()
  local subTabItemWidth = 153
  local subTabItemHeight = 44
  local subTabItemWidthOffset = -6
  local subTabItemHeightOffset = -6
  self:addCloseButton()
  self:enableBGHightLight()
  self:setBGHightLightOpacity(192)
  local titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("help.title"),
    color = g.font.color.WHITE,
    size = 28,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, HelpView.HEIGHT * 0.5 - 30):addTo(self)
  local divider = display.newScale9Sprite("#common_divider.png", 0, HelpView.HEIGHT * 0.5 - 50, cc.size(320, 2)):addTo(self)
  self._subTabGroup = g.ui.TabGroupWithSlider.new({
    background = "#common_standard_greybg_04.png",
    slider = "#common_standard_bluebg_05.png"
  }, g.lang:getContent("help.subTabGroupLabels"), {
    selectedText = {
      color = g.font.color.LIGHT,
      size = 26
    },
    defaltText = {
      color = ccc3(139, 184, 220),
      size = 26
    }
  }, true, true):addTo(self, 11):setTabGroupSize(subTabItemWidth * 4, subTabItemHeight, subTabItemWidthOffset, subTabItemHeightOffset):bindTabClickHandler(handler(self, self.onTabChange))
  self.currentTab = 1
  local subTabBarMarginTop = 8
  local subTabBarSize = self._subTabGroup._background:getContentSize()
  local tabHeight = g.ui.TabGroup.TAB_GROUP_HEIGHT
  self._subTabGroup:pos(0, HelpView.HEIGHT / 2 - tabHeight - subTabBarSize.height / 2 - subTabBarMarginTop)
  local contentMarginTop, contentMarginBottom = 12, 12
  self.viewRectWidth, self.viewRectHeight = HelpView.WIDTH, HelpView.HEIGHT - (tabHeight + subTabBarSize.height + contentMarginTop + contentMarginBottom)
  self.container = display.newNode():addTo(self):pos(0, -(HelpView.HEIGHT / 2 - contentMarginBottom - self.viewRectHeight / 2))
  self.bound = cc.rect(-self.viewRectWidth / 2, -self.viewRectHeight / 2, self.viewRectWidth, self.viewRectHeight)
  local topOriginY = 0
  FAQItem.WIDTH = HelpView.WIDTH
  self._subTabGroup:tabOn(self._defaultTab)
end
function HelpView:onTabChange(selectedTab)
  for i, subTabView in ipairs(self.subTabViews) do
    subTabView:hide()
  end
  if selectedTab == HelpView.TAB_FAQ then
    if self.faqListView == nil then
      self.faqListView = g.ui.ListPanel.new({
        visibleRect = self.bound,
        direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
        scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
      }, FAQItem):addTo(self.container)
      table.insert(self.subTabViews, self.faqListView)
      self.faqListView:setAutoHideInactive(false)
    else
      self.faqListView:show()
    end
    self:requestQAData()
  elseif selectedTab == HelpView.TAB_RULE then
    if self._ruleSubView == nil then
      self._ruleSubView = RuleSubView.new():addTo(self.container)
      table.insert(self.subTabViews, self._ruleSubView)
    end
    self._ruleSubView:show()
  elseif selectedTab == HelpView.TAB_LEVEL then
    if self.levelListView == nil then
      self.levelListView = g.ui.ListPanel.new({
        visibleRect = self.bound,
        direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
        scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
      }, LevelItem):addTo(self.container)
      table.insert(self.subTabViews, self.levelListView)
      self.levelListView:setAutoHideInactive(false)
    else
      self.levelListView:show()
    end
    self:requestLevelData()
  end
  self.currentTab = selectedTab
end
function HelpView:requestQAData()
  local faqData = g.lang:getContent("help.faq")
  self.faqListView:setData(faqData)
end
function HelpView:requestRuleData()
end
function HelpView:onShow()
  if self.levelListView then
    self.levelListView:update()
  end
  if self.faqListView then
    self.faqListView:update()
  end
end
function HelpView:onExit()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
end
function HelpView:requestLevelData()
  local levelData = g.lang:getContent("help.level")
  self.levelListView:setData(levelData)
end
return HelpView

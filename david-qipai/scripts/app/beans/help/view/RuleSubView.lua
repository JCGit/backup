local SingleHandPoker = require("app.module.table.view.SingleHandPoker")
local RuleItem = import(".RuleItem")
local RuleSubView = class("RuleSubView", function()
  return display.newNode()
end)
RuleSubView.WIDTH = 728
RuleSubView.HEIGHT = 348
function RuleSubView:ctor()
  self:initialize()
end
function RuleSubView:initialize()
  self._ruleData = g.lang:getContent("help.rule")
  local topMargin = 21
  self._content = display.newNode()
  for i = 1, #self._ruleData.type do
    local ruleItem = RuleItem.new()
    ruleItem:setData(self._ruleData.type[i])
    ruleItem:pos(RuleSubView.WIDTH * -0.5, RuleSubView.HEIGHT * 0.5 - RuleItem.HEIGHT * (i - 2.5) - topMargin):addTo(self._content)
  end
  local divider = display.newScale9Sprite("#common_divider.png", 20, 0, cc.size(320, 2)):rotation(90):addTo(self)
  for i = 1, #self._ruleData.deng do
    local ruleItem = RuleItem.new()
    ruleItem:setData(self._ruleData.deng[i])
    ruleItem:pos(0, RuleSubView.HEIGHT * 0.5 - RuleItem.HEIGHT * (i - 2.5) - topMargin):addTo(self._content)
  end
  self._content:setContentSize(cc.size(RuleItem.WIDTH, RuleItem.HEIGHT * #self._ruleData.deng))
  self._scrollPanel = g.ui.ScrollPanel.new({
    bgMargin = cc.size(0, 8),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-RuleSubView.WIDTH * 0.5, -RuleSubView.HEIGHT * 0.5, RuleSubView.WIDTH, RuleSubView.HEIGHT),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
  }):addTo(self)
  self._scrollPanel:setScrollContent(self._content)
end
function RuleSubView:onEnter()
  self._scrollPanel:update()
end
return RuleSubView

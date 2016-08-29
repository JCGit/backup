local SingleHandPoker = require("app.module.table.view.SingleHandPoker")
local RuleItem = class("RuleItem", g.ui.ListItem)
RuleItem.WIDTH = 350
RuleItem.HEIGHT = 75
function RuleItem:ctor()
  RuleItem.super.ctor(self, RuleItem.WIDTH, RuleItem.HEIGHT)
  self:initialize()
end
function RuleItem:initialize()
end
function RuleItem:onDataSet(isModified, data)
  if isModified then
    self._data = data
    self._pokers = {}
    for i = #data.pokers, 1, -1 do
      local singleHandPoker = SingleHandPoker.new():pos(50 * (3 - (#data.pokers - i)), 0):scale(0.55):addTo(self)
      singleHandPoker:bindData(data.pokers[i])
      singleHandPoker:showPokerInfo()
    end
    self._titleLabel = ui.newTTFLabel({
      text = data.title,
      color = g.font.color.GOLDEN,
      size = 20,
      align = ui.TEXT_ALIGN_CENTER
    }):addTo(self)
    self._titleLabel:setAnchorPoint(ccp(0, 0.5))
    self._titleLabel:pos(185, 20)
    self._descLabel = ui.newTTFLabel({
      text = data.desc,
      color = g.font.color.LIGHT,
      size = 16,
      align = ui.TEXT_ALIGN_CENTER
    }):addTo(self)
    self._descLabel:setAnchorPoint(ccp(0, 0.5))
    self._descLabel:pos(185, -20)
  end
end
return RuleItem

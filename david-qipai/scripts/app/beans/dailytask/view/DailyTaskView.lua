local DailyTaskView = class("DailyTaskView", g.ui.Panel)
local DailyTaskItem = import(".DailyTaskItem")
local CommonRewardChipAnimation = require("app.beans.activity.CommonRewardChipAnimation")
local ChangeChipAnim = require("app.beans.activity.ChangeChipAnim")
DailyTaskView.WIDTH = 720
DailyTaskView.HEIGHT = 474
function DailyTaskView:ctor()
  DailyTaskView.super.ctor(self, DailyTaskView.WIDTH, DailyTaskView.HEIGHT)
  self:initialize()
  self._getRewardHandle = g.eventCenter:addEventListener(DailyTaskItem.EVT_GET_REWARD, handler(self, self.onGetReward))
end
function DailyTaskView:initialize()
  local width, height = DailyTaskView.WIDTH, DailyTaskView.HEIGHT
  loadTexture("pd_dailybonus_texture.plist", "pd_dailybonus_texture.png")
  self:addCloseButton()
  self:enableBGHightLight()
  self:setBGHightLightOpacity(192)
  local titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("dailyTask.title"),
    color = g.font.color.WHITE,
    size = 36,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, DailyTaskView.HEIGHT * 0.5 - 40):addTo(self)
  local divider = display.newScale9Sprite("#common_divider.png", 0, DailyTaskView.HEIGHT * 0.5 - 68, cc.size(320, 2)):addTo(self)
  local contentWidth = 700
  local contentHeight = 377
  self._list = g.ui.ListPanel.new({
    bgMargin = cc.size(8, 0),
    background = "#common_standard_greybg_03.png",
    visibleRect = cc.rect(-0.5 * contentWidth, -0.5 * contentHeight, contentWidth, contentHeight),
    direction = g.ui.ScrollPanel.DIRECTION_HORIZONTAL,
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_HORIZONTAL)
  }, DailyTaskItem):pos(0, -31):addTo(self)
end
function DailyTaskView:onShow()
  self:loadData()
end
function DailyTaskView:onPopupRemove(callback)
  if self._getRewardHandle then
    g.eventCenter:removeEventListener(self._getRewardHandle)
    self._getRewardHandle = nil
  end
  callback()
end
function DailyTaskView:showPanel()
  self:show(true, true, true, true)
end
function DailyTaskView:loadData()
  self:_showMiniLoading()
  g.tasksInfo:refreshData(handler(self, self.loadDataSuccess), handler(self, self.loadDataFail))
end
function DailyTaskView:playRewardAnim(item)
  local srcPtX, srcPtY = item:getPosition()
  local chips = item.task.chips
  self._animation = CommonRewardChipAnimation.new(ccp(srcPtX + 20, srcPtY + 245), ccp(display.c_right - 160, display.c_bottom + 20)):addTo(self)
  g.audio:playSound("REWARD_GET")
  self:performWithDelay(handler(self, function(self)
    self._changeChipAnim = ChangeChipAnim.new(chips):addTo(self:getParent(), 16):pos(display.right - 160, 10)
  end), 0.8)
  self:performWithDelay(handler(self, function(self)
    if self._animation and self._animation:getParent() then
      self._animation:removeFromParent()
      self._changeChipAnim:removeFromParent()
    end
  end), 1.5)
end
function DailyTaskView:onGetReward(evt)
  self:playRewardAnim(evt.data)
end
function DailyTaskView:loadDataSuccess(retData)
  if self._hideMiniLoading == nil then
    return
  end
  self:_hideMiniLoading()
  self._list:setData(retData)
  self._list:update()
end
function DailyTaskView:loadDataFail()
  if self._hideMiniLoading == nil then
    return
  end
  self:_hideMiniLoading()
end
return DailyTaskView

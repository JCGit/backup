local DailyTaskItem = class("DailyTaskItem", g.ui.ListItem)
DailyTaskItem.WIDTH = 240
DailyTaskItem.HEIGHT = 355
DailyTaskItem.UNDER_GOING_STATUS = 0
DailyTaskItem.HAS_REWARD_STATUS = 1
DailyTaskItem.HAD_REWARD_STATUS = 2
DailyTaskItem.EVT_GET_REWARD = "DailyTaskItem.EVT_GET_REWARD"
function DailyTaskItem:ctor()
  self:setNodeEventEnabled(true)
  DailyTaskItem.super.ctor(self, DailyTaskItem.WIDTH, DailyTaskItem.HEIGHT)
  local width, height = DailyTaskItem.WIDTH, DailyTaskItem.HEIGHT
  self._bgX = width * 0.5 + 16
  self._bgY = height * 0.5
  self._background = display.newScale9Sprite("#task_item_bg.png", self._bgX, self._bgY, cc.size(width, height)):addTo(self)
  self.descLabel = ui.newTTFLabel({
    size = 18,
    color = ccc3(139, 184, 220),
    align = ui.TEXT_ALIGN_LEFT,
    dimensions = cc.size(222, 0)
  }):addTo(self)
  self.descLabel:setAnchorPoint(ccp(0, 1))
  self.descLabel:pos(self._bgX - width * 0.5 + 12, self._bgY - 32)
  self.rewardLabel = ui.newTTFLabel({
    size = labelSize,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(self._bgX, self._bgY + 30):addTo(self)
  progressWidth = 228
  self.progress = g.ui.ProgressBar.new("#common_yellow_progress_bg.png", "#common_yellow_progress_fill.png", {
    bgWidth = progressWidth,
    bgHeight = 26,
    progressWidth = 24,
    progressHeight = 20
  }):addTo(self):pos(self._bgX - width * 0.5 + 6, self._bgY - 16):setValue(0)
  self.progressLabel = ui.newTTFLabel({
    size = 16,
    color = g.font.color.WHITE,
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(self.progress):pos(progressWidth / 2, 0)
  local buttonWidth = 200
  local buttonHeight = 50
  self.rewardButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
    text = g.lang:getContent("dailyTask.getAward"),
    size = buttonSize,
    color = ccc3(50, 88, 0),
    align = ui.TEXT_ALIGN_CENTER
  })):onButtonClicked(clickHandler(self, self.onGetReward)):addTo(self):pos(self._bgX, self._bgY - height * 0.5 + buttonHeight * 0.5 + 8):setButtonSize(buttonWidth, buttonHeight)
  self.finishLabel = ui.newTTFLabel({
    text = g.lang:getContent("dailyTask.awarded"),
    size = buttonSize,
    color = g.font.color.GREY,
    align = ui.TEXT_ALIGN_CENTER,
    valign = ui.TEXT_VALIGN_CENTER,
    dimensions = cc.size(buttonWidth, buttonHeight)
  }):addTo(self):pos(self._bgX, self._bgY - 20):hide()
end
function DailyTaskItem:setData(data)
  self.task = data
  local width, height = DailyTaskItem.WIDTH, DailyTaskItem.HEIGHT
  self._icon = display.newSprite(data.pic):pos(self._bgX, self._bgY + 103):addTo(self)
  self.descLabel:setString("- " .. data.desc)
  local text = string.gsub(g.lang:getContent("dailyTask.rewardChips"), "{chips}", string.formatnumberthousands(data.chips))
  self.rewardLabel:setString(text)
  if data.status == DailyTaskItem.UNDER_GOING_STATUS then
    self.rewardButton:setButtonEnabled(false)
    self.rewardButton:getButtonLabel():setColor(g.font.color.DARK)
    self.finishLabel:hide()
    self.progress:setValue(data.progress / data.target)
    self.progressLabel:setString(data.progress .. "/" .. data.target)
  elseif data.status == DailyTaskItem.HAS_REWARD_STATUS then
    self.rewardButton:setButtonEnabled(true)
    self.progressLabel:setString(data.target .. "/" .. data.target)
    self.progress:setValue(1)
    self.finishLabel:hide()
  elseif data.status == DailyTaskItem.HAD_REWARD_STATUS then
    self.rewardButton:setButtonEnabled(false)
    local label = self.rewardButton:getButtonLabel()
    label:setColor(g.font.color.DARK)
    label:setString(g.lang:getContent("dailyTask.awarded"))
    self.finishLabel:show()
    self.progress:hide()
  end
end
function DailyTaskItem:onGetReward()
  function getAwardSuccess()
    self.task.status = DailyTaskItem.HAD_REWARD_STATUS
    self:setData(self.task)
    g.eventCenter:dispatchEvent({
      name = DailyTaskItem.EVT_GET_REWARD,
      data = self
    })
  end
  g.tasksInfo:getReward(self.task.taskId, getAwardSuccess)
end
return DailyTaskItem

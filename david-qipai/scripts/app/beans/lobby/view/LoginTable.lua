local LoginTable = class("LoginTable", function()
  return display.newNode()
end)
function LoginTable:ctor()
  self.tableLeftSprite = display.newSprite("#login_table_left.png"):addTo(self)
  self.tableRightSprite = display.newSprite("pd_login_table_right.jpg"):addTo(self)
  local leftSize = self.tableLeftSprite:getContentSize()
  local rightSize = self.tableRightSprite:getContentSize()
  self.tableWidth = leftSize.width + rightSize.width
  self.tableLeftSprite:setPositionX(-self.tableWidth * 0.5 + leftSize.width * 0.5 + 1)
  self.tableRightSprite:setPositionX(self.tableWidth * 0.5 - rightSize.width * 0.5)
  self.chip1 = g.ui.Chips.new("orange", 3, 60):addTo(self):pos(-self.tableWidth * 0.5 + 100, 30):rotation(15)
  self.chip6 = g.ui.Chips.new("green", 1, 60):addTo(self):pos(-self.tableWidth * 0.5 + 150, 120)
  self.chip2 = g.ui.Chips.new("purple", 2, 60):addTo(self):pos(-self.tableWidth * 0.5 + 110, 90):rotation(-10)
  self.chip4 = g.ui.Chips.new("blue", 2, 60):addTo(self):pos(-self.tableWidth * 0.5 + 120, -display.cy + 46):rotation(60)
  self.chip3 = g.ui.Chips.new("orange", 4, 60):addTo(self):pos(-self.tableWidth * 0.5 + 130, -display.cy + 88):rotation(-15):suitPos(0, 3)
  self.chip5 = g.ui.Chips.new("purple", 3, 60):addTo(self):pos(-self.tableWidth * 0.5 + 190, -display.cy + 96)
  self.chip7 = g.ui.Chips.new("red", 2, 60):addTo(self):pos(-self.tableWidth * 0.5 + 180, -display.cy + 8):rotation(15)
  self.loadingChip = display.newSprite("#common_green_chips_90.png"):addTo(self):pos(-self.tableWidth * 0.5 + 260, -display.cy + 70)
  self.loginLoadingSprite = display.newSprite("#common_chips_spade_90.png"):pos(self.loadingChip:getContentSize().width * 0.5, self.loadingChip:getContentSize().height * 0.5 + 3):addTo(self.loadingChip)
  self._loadingLabel = ui.newTTFLabel({
    text = g.lang:getContent("lobby.loading"),
    align = ui.TEXT_ALIGN_LEFT
  }):align(display.LEFT_CENTER, 0, -display.cy + 30):addTo(self):hide()
  self:scale(g.vars.bgScale)
end
function LoginTable:getTableWidth()
  return self.tableWidth * g.vars.bgScale
end
function LoginTable:setVisibleWidth(visibleWidth)
  self._visibleWidth = visibleWidth
end
function LoginTable:showLoginLoading()
  self.loginLoadingSprite:stopAllActions()
  self.loginLoadingSprite:runAction(CCRepeatForever:create(transition.sequence({
    CCScaleTo:create(0.75, -1, 1),
    CCScaleTo:create(0.75, 1, 1)
  })))
  transition.stopTarget(self.loadingChip)
  transition.moveTo(self.loadingChip, {
    time = 0.5,
    x = self._visibleWidth - 0.5 * self.tableWidth - 60,
    onComplete = function()
      self._loadingLabel:pos(self._visibleWidth - 0.5 * self.tableWidth - 220, -display.cy + 30)
      self._loadingLabel:show()
    end
  })
end
function LoginTable:hideLoginLoading()
  self._loadingLabel:hide()
  self.loginLoadingSprite:stopAllActions()
  self.loginLoadingSprite:scale(1)
  self.loadingChip:stopAllActions()
  transition.moveTo(self.loadingChip, {
    time = 0.5,
    x = -self.tableWidth * 0.5 + 260,
    onComplete = function()
    end
  })
end
function LoginTable:chipsFlyAwayAnimation(flyCompleteCallback)
  transition.stopTarget(self.loadingChip)
  self._loadingLabel:hide()
  transition.moveTo(self.loadingChip, {
    time = 0.1,
    x = display.cx,
    onComplete = function()
    end
  })
  local chipRunTime = 0.2
  transition.moveTo(self.chip5, {
    time = chipRunTime,
    x = display.cx,
    onComplete = function()
    end
  })
  transition.moveTo(self.chip4, {
    time = chipRunTime,
    x = display.cx,
    delay = 0.2,
    onComplete = function()
    end
  })
  transition.moveTo(self.chip3, {
    time = chipRunTime,
    x = display.cx,
    delay = 0.1,
    onComplete = function()
    end
  })
  transition.moveTo(self.chip2, {
    time = chipRunTime,
    x = display.cx,
    delay = 0.2,
    onComplete = function()
    end
  })
  transition.moveTo(self.chip1, {
    time = chipRunTime,
    x = display.cx,
    delay = 0.3,
    onComplete = function()
      flyCompleteCallback()
    end
  })
  transition.moveTo(self.chip6, {
    time = chipRunTime,
    x = display.cx,
    delay = 0.4,
    onComplete = function()
    end
  })
  transition.moveTo(self.chip7, {
    time = chipRunTime,
    x = display.cx,
    delay = 0.4,
    onComplete = function()
    end
  })
end
return LoginTable

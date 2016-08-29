local PlayTogetherItem = class("PlayTogetherItem", function()
  return display.newNode()
end)
PlayTogetherItem.WIDTH = 205
PlayTogetherItem.HEIGHT = 240
function PlayTogetherItem:ctor(rewardHandler, stage, playCount)
  self.stage = stage
  self.playCount = playCount
  self.rewardHandler = rewardHandler
  self:setCascadeOpacityEnabled(true)
  self._backgroundNode = display.newNode():addTo(self)
end
function PlayTogetherItem:setData(value)
  if self._data ~= value then
    self._data = value
    local daysLabel = g.lang:getContent("registerReward.daysLabel")
    self:onResponseStateChage(tonumber(self._data.status))
    display.newSprite("#playtogether_item_title_" .. self.stage .. ".png"):pos(0, 85):addTo(self)
  end
  return self
end
function PlayTogetherItem:onResponseStateChage(status)
  if self._background ~= nil then
    self._background:removeFromParent()
    self._background = nil
  end
  if self._rewardButton ~= nil then
    self._rewardButton:removeFromParent()
    self._rewardButton = nil
  end
  if self._rewardInfoLabel ~= nil then
    self._rewardInfoLabel:removeFromParent()
    self._rewardInfoLabel = nil
  end
  if self._progressLabel ~= nil then
    self._progressLabel:removeFromParent()
    self._progressLabel = nil
  end
  if self.progress ~= nil then
    self.progress:removeFromParent()
    self.progress = nil
  end
  if self.boxSprite ~= nil then
    self.boxSprite:removeFromParent()
  end
  local bgSpriteName
  local completeCount = 0
  local stageCount = 0
  local isNeedRewardLabel = false
  if status == 0 then
    bgSpriteName = "#common_invitefriends_board_small_unavailable.png"
    local progressWidth = 160
    self.progress = g.ui.ProgressBar.new("#common_invite_progress_bg.png", "#common_invite_progress_fill.png", {
      bgWidth = progressWidth,
      bgHeight = 16,
      progressWidth = 24,
      progressHeight = 14
    }):setValue(0)
    self._progressLabel = ui.newTTFLabel({
      size = 18,
      color = ccc3(87, 40, 0),
      align = ui.TEXT_ALIGN_CENTER,
      dimensions = cc.size(222, 80)
    })
    if self.stage == 1 then
      stageCount = 9
      if 0 < self.playCount then
        completeCount = self.playCount
      end
    elseif self.stage == 2 then
      stageCount = 20
      if 0 < self.playCount - 9 then
        completeCount = self.playCount - 9
      end
    elseif self.stage == 3 then
      stageCount = 20
      if 0 < self.playCount - 29 then
        completeCount = self.playCount - 29
      end
    end
    isNeedRewardLabel = true
  elseif status == 1 then
    bgSpriteName = "#common_invitefriends_board_small_available.png"
    self._rewardButton = cc.ui.UIPushButton.new({
      normal = "#common_invitefriends_get_btn_normal.png",
      pressed = "#common_invitefriends_get_btn_pressed.png"
    }):setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("invite.RewardAvailable"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    })):onButtonClicked(clickHandler(self, self.onRewardClick))
  elseif status == 2 then
    bgSpriteName = "#common_invitefriends_board_small_unavailable.png"
    self._rewardButton = cc.ui.UIPushButton.new({
      normal = "#common_invitefriends_already_get_btn.png",
      pressed = "#common_invitefriends_already_get_btn.png"
    }, {scale9 = true}):setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("invite.hasRewarded"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    })):setButtonSize(165, 59)
  end
  if status == 2 then
    if self.stage == 1 then
      self.boxSprite = display.newSprite("#playtogether_iron_box_open.png")
    elseif self.stage == 2 then
      self.boxSprite = display.newSprite("#playtogether_silver_box_open.png")
    elseif self.stage == 3 then
      self.boxSprite = display.newSprite("#playtogether_golden_box_open.png")
    end
  elseif self.stage == 1 then
    self.boxSprite = display.newSprite("#playtogether_iron_box_close.png")
  elseif self.stage == 2 then
    self.boxSprite = display.newSprite("#playtogether_silver_box_close.png")
  elseif self.stage == 3 then
    self.boxSprite = display.newSprite("#playtogether_golden_box_close.png")
  end
  self._background = display.newScale9Sprite(bgSpriteName, 0, 0, cc.size(PlayTogetherItem.WIDTH, PlayTogetherItem.HEIGHT)):addTo(self._backgroundNode)
  if isNeedRewardLabel then
    local rewardInfo = json.decode(self._data.reward or "{}")
    local rewardString = ""
    for k, v in pairs(rewardInfo or {}) do
      if rewardString ~= "" then
        rewardString = rewardString .. "\n"
      end
      if k == "chips" then
        local textString = string.gsub(g.lang:getContent("invite.rewardChips"), "{chips}", v)
        print(textString)
        rewardString = rewardString .. textString
      elseif k == "magic" then
        local textString = string.gsub(g.lang:getContent("invite.rewardMagic"), "{magic}", v)
        rewardString = rewardString .. textString
      end
    end
    self._rewardInfoLabel = ui.newTTFLabel({
      size = 18,
      color = ccc3(87, 40, 0),
      align = ui.TEXT_ALIGN_CENTER,
      dimensions = cc.size(222, 80)
    }):addTo(self._backgroundNode)
    self._rewardInfoLabel:pos(0, -55)
    self._rewardInfoLabel:setString(rewardString)
  end
  if self._rewardButton then
    self._rewardButton:pos(0, -80):addTo(self._backgroundNode)
  end
  self.boxSprite:addTo(self._backgroundNode):pos(0, 15)
  if self.progress then
    self.progress:addTo(self._backgroundNode):pos(-80, -85)
  end
  if self._progressLabel then
    self._progressLabel:addTo(self._backgroundNode)
    self._progressLabel:pos(0, -104)
    self._progressLabel:setString(completeCount .. "/" .. stageCount)
    self.progress:setValue(tonumber(completeCount) / tonumber(stageCount))
  end
end
function PlayTogetherItem:onRewardClick()
  self.rewardHandler(self.stage)
end
return PlayTogetherItem

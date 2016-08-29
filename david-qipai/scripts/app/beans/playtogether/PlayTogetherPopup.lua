local PlayTogetherPopup = class("PlayTogetherPopup", g.ui.Panel)
local ChangeChipAnim = require("app.beans.activity.ChangeChipAnim")
local PlayTogetherItem = import(".PlayTogetherItem")
local PlayTogetherInfo = require("app.beans.data.PlayTogetherInfo")
PlayTogetherPopup.WIDTH = 750
PlayTogetherPopup.HEIGHT = 480
PlayTogetherPopup.MID_BOARD_WIDTH = 680
PlayTogetherPopup.MID_BOARD_HEIGHT = 280
PlayTogetherPopup.LEFT = -PlayTogetherPopup.WIDTH * 0.5
PlayTogetherPopup.TOP = PlayTogetherPopup.HEIGHT * 0.5
PlayTogetherPopup.RIGHT = PlayTogetherPopup.WIDTH * 0.5
PlayTogetherPopup.BOTTOM = -PlayTogetherPopup.HEIGHT * 0.5
local CommonRewardChipAnimation = require("app.beans.activity.CommonRewardChipAnimation")
function PlayTogetherPopup:ctor()
  self:setNodeEventEnabled(true)
  loadTexture("pd_invitefriends_texture.plist", "pd_invitefriends_texture.png")
  loadTexture("pd_playtogether_texture.plist", "pd_playtogether_texture.png")
  self._panelBG = display.newScale9Sprite("#common_invitefriends_board.png", 0, 0, cc.size(PlayTogetherPopup.WIDTH, PlayTogetherPopup.HEIGHT)):addTo(self)
  self._panelBG:setTouchEnabled(true)
  PlayTogetherPopup.super.ctor(self, PlayTogetherPopup.WIDTH, PlayTogetherPopup.HEIGHT)
  self:addCloseButton("#common_invitefriends_close_normal.png", "#common_invitefriends_close_press.png")
  if self._background ~= nil and self._background:getParent() ~= nil then
    self._background:removeFromParent()
  end
  self._titleLogo = display.newSprite("#common_invitefriends_title_bg.png"):pos(0, PlayTogetherPopup.TOP - 16):addTo(self)
  self._titleLabel = ui.newTTFLabel({
    text = g.lang:getContent("playtogether.title"),
    color = g.font.color.GOLDEN,
    size = 30,
    align = ui.TEXT_ALIGN_LEFT
  }):addTo(self._titleLogo):pos(self._titleLogo:getContentSize().width * 0.5, self._titleLogo:getContentSize().height * 0.5 - 6)
  self.boardMid = display.newScale9Sprite("#common_invitefriends_board_mid.png", 0, 28, cc.size(PlayTogetherPopup.MID_BOARD_WIDTH, PlayTogetherPopup.MID_BOARD_HEIGHT)):addTo(self)
  self._playStateLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.LIGHT,
    size = 20,
    align = ui.TEXT_ALIGN_LEFT
  }):align(display.LEFT_CENTER, PlayTogetherPopup.LEFT + 40, -135):addTo(self)
  self._timeCountLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.LIGHT,
    size = 20,
    align = ui.TEXT_ALIGN_LEFT
  }):align(display.LEFT_CENTER, PlayTogetherPopup.LEFT + 40, -190):addTo(self)
  self._inviteButton = cc.ui.UIPushButton.new({
    normal = "#common_invitefriends_attend_normal.png",
    pressed = "#common_invitefriends_attend_pressed.png",
    disabled = "#common_invitefriends_attend_diable.png"
  }, {scale9 = true}):setButtonSize(205, 59):pos(0, -204):addTo(self):onButtonClicked(clickHandler(self, self.onPlayTogetherButtonHandler))
  self._inviteButton:setTouchSwallowEnabled(false)
  self._inviteButton:hide()
  self:refreshData()
end
function PlayTogetherPopup:refreshData()
  self:_showMiniLoading()
  local function successCallback(retData)
    if self._hideMiniLoading == nil then
      return
    end
    self:_hideMiniLoading()
    self:bindData(g.playTogetherInfo.dataModel)
  end
  local function failCallback()
    if self._hideMiniLoading == nil then
      return
    end
    self:_hideMiniLoading()
  end
  local function timeTickCallback()
    if self.updateTimeInfoUI then
      self:updateTimeInfoUI()
    end
  end
  g.playTogetherInfo.progressSuccessbackUIRefresh = successCallback
  g.playTogetherInfo.progressFailbackUIRefresh = failCallback
  g.playTogetherInfo.progressTickCountCallback = timeTickCallback
  g.playTogetherInfo:playTogetherProgress()
end
function PlayTogetherPopup:bindData(inviteItemDataModel)
  local playCount = tonumber(inviteItemDataModel.playCount or 0)
  if self._items == nil then
    self._items = {}
    for i, itemDataModel in ipairs(inviteItemDataModel.progress or {}) do
      local rewardHandler = handler(self, self.onRewardButtonHandler)
      self._items[i] = PlayTogetherItem.new(rewardHandler, i, playCount):pos(PlayTogetherPopup.MID_BOARD_WIDTH * 0.5 - 220 + 220 * (i - 1), PlayTogetherPopup.MID_BOARD_HEIGHT * 0.5):addTo(self.boardMid):setData(itemDataModel)
    end
  end
  self._hasReward = false
  for i, itemDataModel in ipairs(inviteItemDataModel.progress or {}) do
    if tonumber(itemDataModel.status) == 1 then
      self._hasReward = true
    end
  end
  self._inviteButton:show()
  self._inviteButton:setButtonEnabled(true)
  self._playStateLabel:setString("")
  if g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_OVER then
    self._inviteButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("playtogether.activityOver"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    }))
    self._inviteButton:setButtonEnabled(false)
    if self._hasReward then
      local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.activityEnd)
      local textString = string.gsub(g.lang:getContent("playtogether.activityOffInfo"), "{1}", timeInfo)
      self._playStateLabel:setString(textString)
    else
      self._playStateLabel:setString(g.lang:getContent("playtogether.activityOverNextTime"))
    end
  elseif g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_NOTAPPLY_NOT_COUNT then
    self._inviteButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("playtogether.attendActivity"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    }))
    local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.startTimeOffset)
    local textString = string.gsub(g.lang:getContent("playtogether.startTimeInfo"), "{1}", timeInfo)
    self._playStateLabel:setString(textString)
  elseif g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_NOTAPPLY_IN_COUNT then
    self._inviteButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("playtogether.attendActivity"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    }))
    local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.endTimeOffset)
    local textString = string.gsub(g.lang:getContent("playtogether.endTimeInfo"), "{1}", timeInfo)
    self._playStateLabel:setString(textString)
  elseif g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_APPLY_NOT_COUNT then
    self._inviteButton:setButtonEnabled(false)
    self._inviteButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("playtogether.hasAttend"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    }))
    local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.startTimeOffset)
    local textString = string.gsub(g.lang:getContent("playtogether.startTimeInfo"), "{1}", timeInfo)
    self._playStateLabel:setString(textString)
  elseif g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_APPLY_IN_COUNT then
    self._inviteButton:setButtonEnabled(true)
    self._inviteButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("playtogether.playnow"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    }))
    local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.endTimeOffset)
    local textString = string.gsub(g.lang:getContent("playtogether.endTimeInfo"), "{1}", timeInfo)
    self._playStateLabel:setString(textString)
  end
end
function PlayTogetherPopup:showPanel()
  self:show(true, true, true, true)
end
function PlayTogetherPopup:updateTimeInfoUI()
  self._playStateLabel:setString("")
  if g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_OVER then
    self._inviteButton:setButtonLabel(ui.newTTFLabel({
      text = g.lang:getContent("playtogether.activityOver"),
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER
    }))
    if self._hasReward then
      local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.activityEnd)
      local textString = string.gsub(g.lang:getContent("playtogether.activityOffInfo"), "{1}", timeInfo)
      self._playStateLabel:setString(textString)
    else
      self._playStateLabel:setString(g.lang:getContent("playtogether.activityOverNextTime"))
    end
  elseif g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_NOTAPPLY_NOT_COUNT then
    local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.startTimeOffset)
    local textString = string.gsub(g.lang:getContent("playtogether.startTimeInfo"), "{1}", timeInfo)
    self._playStateLabel:setString(textString)
  elseif g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_NOTAPPLY_IN_COUNT then
    local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.endTimeOffset)
    local textString = string.gsub(g.lang:getContent("playtogether.endTimeInfo"), "{1}", timeInfo)
    self._playStateLabel:setString(textString)
  elseif g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_APPLY_NOT_COUNT then
    local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.startTimeOffset)
    local textString = string.gsub(g.lang:getContent("playtogether.startTimeInfo"), "{1}", timeInfo)
    self._playStateLabel:setString(textString)
  elseif g.playTogetherInfo:getActivityState() == PlayTogetherInfo.ACTIVITY_ING_APPLY_IN_COUNT then
    local timeInfo = self:timeFormate(g.playTogetherInfo.dataModel.endTimeOffset)
    local textString = string.gsub(g.lang:getContent("playtogether.endTimeInfo"), "{1}", timeInfo)
    self._playStateLabel:setString(textString)
  end
end
function PlayTogetherPopup:timeFormate(leftTime)
  leftTime = tonumber(leftTime)
  local timeInfo = ""
  if leftTime > 0 then
    local hour = math.floor(leftTime / 3600)
    local min = math.floor((leftTime - hour * 3600) / 60)
    local sec = math.floor(leftTime - hour * 3600 - min * 60)
    if hour < 10 then
      hour = "0" .. tostring(hour)
    end
    if min < 10 then
      min = "0" .. tostring(min)
    end
    if sec < 10 then
      sec = "0" .. tostring(sec)
    end
    timeInfo = hour .. ":" .. min .. ":" .. sec
  else
    timeInfo = "00:00:00"
  end
  return timeInfo
end
function PlayTogetherPopup:onPlayTogetherButtonHandler()
  local function successCallback(retData)
    if retData ~= nil then
      g.ui.manager.tip:top(g.lang:getContent("playtogether.attendSuccess"))
    end
    if tonumber(g.playTogetherInfo.dataModel.startTimeOffset or 0) < 0 then
      self:onClose()
      g.vars.table.isPlayNow = true
      g.tablesInfo:getPlayTableWithTid(nil, nil, nil)
    else
      self:bindData(g.playTogetherInfo.dataModel)
    end
  end
  local failCallback = function()
    g.ui.manager.tip:top(g.lang:getContent("common.requestDataFail"))
  end
  if tonumber(g.playTogetherInfo.dataModel.applyStatus) == 0 then
    g.playTogetherInfo:playTogetherApply(successCallback, failCallback)
  else
    successCallback()
  end
end
function PlayTogetherPopup:onRewardButtonHandler(stage)
  local function successCallback(retData)
    if retData.info ~= nil then
      self._items[stage]:onResponseStateChage(2)
      local chipQuantity = 0
      local magicQuantity = 0
      for k, v in pairs(retData.info) do
        if k == "chips" then
          chipQuantity = v
          self:playRewardAnim(stage, chipQuantity)
        elseif k == "magic" then
          magicQuantity = v
        elseif k == "gift" then
        end
      end
      if chipQuantity ~= 0 and magicQuantity ~= 0 then
        local textString = string.gsub(g.lang:getContent("invite.rewardChipsAndMagicTips"), "{1}", chipQuantity)
        textString = string.gsub(textString, "{2}", magicQuantity)
        g.ui.manager.tip:top(textString)
      elseif chipQuantity ~= 0 then
        local textString = string.gsub(g.lang:getContent("invite.rewardChipsTips"), "{1}", chipQuantity)
        g.ui.manager.tip:top(textString)
      elseif magicQuantity ~= 0 then
        local textString = string.gsub(g.lang:getContent("invite.rewardMagicTips"), "{2}", magicQuantity)
        g.ui.manager.tip:top(textString)
      end
    end
  end
  local function failCallback(ret)
    if ret == -13 then
      g.ui.manager.tip:top(g.lang:getContent("invite.invitationNoEnough"))
      self._items[stage]:onResponseStateChage(0)
    else
      g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
    end
  end
  g.playTogetherInfo:playTogetherReward(stage, successCallback, failCallback)
end
function PlayTogetherPopup:playRewardAnim(stage, totalChipsReward)
  local srcPtX, srcPtY = self._items[stage]:getPosition()
  srcPtX = srcPtX - 340
  srcPtY = srcPtY - 110
  if self._animation ~= nil then
    return
  end
  self._animation = CommonRewardChipAnimation.new(ccp(srcPtX, srcPtY), ccp(display.c_right - 160, display.c_bottom + 20)):addTo(self)
  g.audio:playSound("REWARD_GET")
  self:performWithDelay(handler(self, function(self)
    self._changeChipAnim = ChangeChipAnim.new(totalChipsReward):addTo(self:getParent(), 16):pos(display.right - 160, 10)
  end), 0.8)
  self:performWithDelay(handler(self, function(self)
    if self._animation and self._animation:getParent() then
      self._animation:removeFromParent()
      self._animation = nil
    end
    if self._changeChipAnim and self._changeChipAnim:getParent() then
      self._changeChipAnim:removeFromParent()
      self._changeChipAnim = nil
    end
  end), 1.5)
end
function PlayTogetherPopup:onExit()
  if self._animation and self._animation:getParent() then
    self._animation:removeFromParent()
    self._animation = nil
    self:hide()
  end
  if self._changeChipAnim and self._changeChipAnim:getParent() then
    self._changeChipAnim:removeFromParent()
    self._changeChipAnim = nil
  end
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  g.playTogetherInfo.progressSuccessbackUIRefresh = nil
  g.playTogetherInfo.progressFailbackUIRefresh = nil
  g.playTogetherInfo.progressTickCountCallback = nil
end
return PlayTogetherPopup

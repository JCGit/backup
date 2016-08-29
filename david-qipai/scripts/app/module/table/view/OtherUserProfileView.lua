local ViceJudge = require("app.module.table.model.ViceJudge")
local AvatarView = require("app.ui.imageview.AvatarView")
local PANEL_WIDTH = 594
local PANEL_HEIGHT = 520
local LEFT = -PANEL_WIDTH * 0.5
local TOP = PANEL_HEIGHT * 0.5
local RIGHT = PANEL_WIDTH * 0.5
local BOTTOM = -PANEL_HEIGHT * 0.5
local OtherUserProfileView = class("OtherUserProfileView", g.ui.Panel)
OtherUserProfileView.AVATAR_LENGTH = 80
function OtherUserProfileView:ctor(player)
  OtherUserProfileView.super.ctor(self, PANEL_WIDTH, PANEL_HEIGHT)
  self:setNodeEventEnabled(true)
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  self:addCloseButton()
  self._headPic = AvatarView.new()
  self._headPic:pos(LEFT + OtherUserProfileView.AVATAR_LENGTH * 0.5 + 24, TOP - OtherUserProfileView.AVATAR_LENGTH * 0.5 - 24)
  self._headPic:scale(OtherUserProfileView.AVATAR_LENGTH / AvatarView.DEFAULT_HEIGHT)
  self._headPic:addTo(self)
  self._maleIcon = display.newSprite("#common_male_icon_02.png"):addTo(self):hide()
  self._maleIcon:align(display.LEFT_CENTER, LEFT + 120 - 3, TOP - 45)
  self._femaleIcon = display.newSprite("#common_female_icon_02.png"):addTo(self):hide()
  self._femaleIcon:align(display.LEFT_CENTER, LEFT + 120 - 3, TOP - 45)
  if player:getGender() == 0 then
    self._headPic:setImage(player:getSPic(), {
      defaultImageName = "#common_male_avatar.png"
    })
    self._maleIcon:show()
  else
    self._headPic:setImage(player:getSPic(), {
      defaultImageName = "#common_female_avatar.png"
    })
    self._femaleIcon:show()
  end
  self._userName = ui.newTTFLabel({
    size = 22,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT,
    valign = ui.TEXT_VALIGN_CENTER
  })
  self._userName:setAnchorPoint(ccp(0, 0.5))
  self._userName:pos(LEFT + 160, TOP - 48)
  self._userName:addTo(self)
  self._uidLabel = ui.newTTFLabel({
    size = 20,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_RIGHT
  })
  self._uidLabel:setAnchorPoint(ccp(1, 0.5))
  self._uidLabel:pos(RIGHT - 45, TOP - 48)
  self._uidLabel:addTo(self)
  display.newScale9Sprite("#common_divider_01.png", 0, 0, CCSize(392, 2)):align(display.LEFT_CENTER, LEFT + 160, TOP - 65):addTo(self)
  self._chipIcon = display.newSprite("#mainlobby_chips_icon.png"):addTo(self)
  self._chipIcon:align(display.LEFT_CENTER, LEFT + 120, TOP - 90)
  self._chipLabel = ui.newTTFLabel({
    size = 20,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_LEFT,
    valign = ui.TEXT_VALIGN_CENTER
  })
  self._chipLabel:setAnchorPoint(ccp(0, 0.5))
  self._chipLabel:pos(LEFT + 160, TOP - 90)
  self._chipLabel:addTo(self)
  self._levelProgressBar = g.ui.ProgressBar.new("#common_grey_progress_bg.png", "#common_grey_progress_fill.png", {
    bgWidth = 175,
    bgHeight = 18,
    progressWidth = 12,
    progressHeight = 14
  }):pos(RIGHT - 45 - 175, TOP - 90):addTo(self)
  self._levelLabel = ui.newTTFLabel({
    size = 24,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_LEFT,
    valign = ui.TEXT_VALIGN_CENTER,
    dimensions = CCSize(80, 30)
  })
  self._levelLabel:setAnchorPoint(ccp(0, 0.5))
  self._levelLabel:pos(RIGHT - 45 - 175 - 60, TOP - 90)
  self._levelLabel:addTo(self)
  self._rankLabel = ui.newTTFLabel({
    size = 24,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_LEFT,
    dimensions = CCSize(135, 100)
  })
  self._rankLabel:setAnchorPoint(ccp(0, 0.5))
  self._rankLabel:pos(LEFT + 120, TOP - 142)
  self._rankLabel:addTo(self)
  self._rankLabelNumber = ui.newTTFLabel({
    size = 20,
    text = "",
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT
  })
  self._rankLabelNumber:setAnchorPoint(ccp(0, 0.5))
  self._rankLabelNumber:pos(LEFT + 240, TOP - 142)
  self._rankLabelNumber:addTo(self)
  self._winRateLabel = ui.newTTFLabel({
    size = 24,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_LEFT,
    valign = ui.TEXT_VALIGN_CENTER,
    dimensions = CCSize(135, 100)
  })
  self._winRateLabel:setAnchorPoint(ccp(0, 0.5))
  self._winRateLabel:pos(RIGHT - 45 - 175 - 60, TOP - 142)
  self._winRateLabel:addTo(self)
  self._winRateLabelNumber = ui.newTTFLabel({
    size = 20,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_LEFT
  })
  self._winRateLabelNumber:setAnchorPoint(ccp(0, 0.5))
  self._winRateLabelNumber:pos(LEFT + 430, TOP - 142)
  self._winRateLabelNumber:addTo(self)
  self._isMyFriend = false
  self._addFriendBtn = cc.ui.UIPushButton.new({
    normal = "#common_add_friend_up.png",
    pressed = "#common_add_friend_down.png"
  }):onButtonClicked(clickHandler(self, self._onFriendBtnClick)):pos(LEFT + 60, TOP - 138):setButtonEnabled(false):addTo(self)
  self._sendChipBtn1 = cc.ui.UIPushButton.new("#common_green_chips_90.png"):setButtonLabel(ui.newTTFLabel({
    text = "1",
    size = 28,
    color = ccc3(202, 234, 211)
  })):setButtonLabelOffset(0, 2):onButtonPressed(function(event)
    self._sendChipBtn1:scale(0.9)
  end):onButtonRelease(function(event)
    self._sendChipBtn1:scale(1)
  end):onButtonClicked(function(event)
    self._sendChipBtn1:scale(1)
    self:_onSendChipClicked(1)
  end):pos(LEFT + 70, TOP - 220):addTo(self)
  self._sendChipBtn2 = cc.ui.UIPushButton.new("#common_red_chips_90.png"):setButtonLabel(ui.newTTFLabel({
    text = "10",
    size = 28,
    color = ccc3(202, 234, 211)
  })):setButtonLabelOffset(0, 2):onButtonPressed(function(event)
    self._sendChipBtn2:scale(0.9)
  end):onButtonRelease(function(event)
    self._sendChipBtn2:scale(1)
  end):onButtonClicked(function(event)
    self._sendChipBtn2:scale(1)
    self:_onSendChipClicked(10)
  end):pos(LEFT + 70 + 100, TOP - 220):addTo(self)
  self._sendChipBtn3 = cc.ui.UIPushButton.new("#common_blue_chips_90.png"):setButtonLabel(ui.newTTFLabel({
    text = "100",
    size = 28,
    color = ccc3(253, 229, 228)
  })):setButtonLabelOffset(0, 2):onButtonPressed(function(event)
    self._sendChipBtn3:scale(0.9)
  end):onButtonRelease(function(event)
    self._sendChipBtn3:scale(1)
  end):onButtonClicked(function(event)
    self._sendChipBtn3:scale(1)
    self:_onSendChipClicked(100)
  end):pos(LEFT + 70 + 200, TOP - 220):addTo(self)
  self._sendChipBtn4 = cc.ui.UIPushButton.new("#common_pink_chips_90.png"):setButtonLabel(ui.newTTFLabel({
    text = "1000",
    size = 28,
    color = ccc3(253, 229, 228)
  })):setButtonLabelOffset(0, 2):onButtonPressed(function(event)
    self._sendChipBtn4:scale(0.9)
  end):onButtonRelease(function(event)
    self._sendChipBtn4:scale(1)
  end):onButtonClicked(function(event)
    self._sendChipBtn4:scale(1)
    self:_onSendChipClicked(1000)
  end):pos(LEFT + 70 + 300, TOP - 220):addTo(self)
  self._leftSepLine = display.newSprite("#table_profile_sepline.png"):addTo(self)
  self._leftSepLine:setScaleX(-1)
  self._leftSepLine:align(display.RIGHT_CENTER, LEFT - 20, TOP - 290)
  self._rightSepLine = display.newSprite("#table_profile_sepline.png"):addTo(self)
  self._rightSepLine:align(display.RIGHT_CENTER, -LEFT + 20, TOP - 290)
  local magicString
  if 0 < tonumber(g.vars.user.magic or 0) then
    magicString = string.gsub(g.lang:getContent("magic.magicCount"), "{1}", g.vars.user.magic)
  else
    magicString = string.gsub(g.lang:getContent("magic.magicUseTips"), "{1}", g.util.functions.formatBigInt(g.vars.table.loginResponse.minBet * 0.1))
  end
  self._magicDesLabel = ui.newTTFLabel({
    size = 20,
    text = magicString,
    color = g.font.color.LIGHT,
    align = ui.TEXT_ALIGN_CENTER
  })
  self._magicDesLabel:pos(0, TOP - 290)
  self._magicDesLabel:addTo(self)
  local x, y = LEFT + 85, TOP - 360
  for i = 1, 2 do
    for j = 1, 5 do
      do
        local id = (i - 1) * 5 + j
        local btn = cc.ui.UIPushButton.new({
          normal = "#transparent.png",
          normal = "#table_profile_magic_normal.png",
          pressed = "#table_profile_magic_pressed.png"
        }):onButtonClicked(function()
          self:_onSendMagicEmojiClicked(id)
        end):pos(x, y):addTo(self)
        btn:scale(1.1)
        btn:onButtonPressed(function()
        end)
        btn:onButtonRelease(function()
        end)
        if id == 1 then
          local bombNode = display.newNode()
          display.newSprite("#table_magic_0001.png"):scale(0.9):pos(8, 0):addTo(bombNode)
          btn:setButtonLabel(bombNode)
        elseif id == 2 then
          btn:setButtonLabel(display.newSprite("#table_magic_0002.png"):scale(0.45))
        elseif id == 3 then
          btn:setButtonLabel(display.newSprite("#table_magic_0003.png"):scale(0.4))
        elseif id == 4 then
          btn:setButtonLabel(display.newSprite("#table_magic_0004.png"):scale(0.55))
        elseif id == 5 then
          btn:setButtonLabel(display.newSprite("#table_magic_0005.png"):scale(0.5))
        elseif id == 6 then
          local tomatoNode = display.newNode()
          display.newSprite("#table_magic_0006.png"):scale(0.5):pos(0, -15):addTo(tomatoNode)
          btn:setButtonLabel(tomatoNode)
        elseif id == 7 then
          local waterNode = display.newNode()
          display.newSprite("#table_magic_0007.png"):scale(0.4):pos(-5, -15):addTo(waterNode)
          btn:setButtonLabel(waterNode)
        elseif id == 8 then
          local cockNode = display.newNode()
          display.newSprite("#table_magic_0008.png"):scale(0.45):pos(-5, 8):addTo(cockNode)
          btn:setButtonLabel(cockNode)
        elseif id == 9 then
          btn:setButtonLabel(display.newSprite("#pd_magic_dagger.png"):scale(0.7))
        elseif id == 10 then
          local missileNode = display.newNode()
          display.newSprite("#pd_magic_missile.png"):scale(0.7):pos(0, 10):addTo(missileNode)
          btn:setButtonLabel(missileNode)
        else
          btn:setButtonEnabled(false)
        end
        x = x + 106
      end
    end
    x = LEFT + 85
    y = y - 100
  end
  self:bindData(player)
  self:loadFriendUidList()
  self:loadUserRank(player:getUid())
end
function OtherUserProfileView:showPanel()
  self:show(true, true, true, true)
end
function OtherUserProfileView:hidePanel()
  self:hide()
end
function OtherUserProfileView:bindData(player)
  self._player = player
  self._userName:setString(g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 22, player:getUserName(), 280))
  local uidString = string.gsub(g.lang:getContent("common.uid"), "{1}", tostring(player:getUid()))
  self._uidLabel:setString(uidString)
  self._chipLabel:setString(g.util.functions.formatBigInt(player:getUserChips()))
  self._rankLabel:setString(g.lang:getContent("profile.weekProfitSepLabel"))
  self._winRateLabel:setString(g.lang:getContent("profile.winRateSepLabel"))
end
function OtherUserProfileView:_onSendChipClicked(chips)
  clickSoundImmediately()
  if g.vars.user.seatId then
    ViceJudge:asyncRequestSendChip(self._player:getUid(), chips)
    self:hidePanel()
  else
    g.ui.manager.tip:top(g.lang:getContent("table.sendChipNotInSeat"))
  end
end
function OtherUserProfileView:_onSendMagicEmojiClicked(magicId)
  clickSoundImmediately()
  if g.vars.user.seatId then
    local targetSeatId = self._player:getSeatId()
    local targetUid = self._player:getUid()
    ViceJudge:asyncRequestSendMajicEmoji(targetUid, targetSeatId, 1, magicId)
    self:hidePanel()
  else
    g.ui.manager.tip:top(g.lang:getContent("language.table.sendMagicEmojiNotInSeat"))
  end
end
function OtherUserProfileView:loadFriendUidList()
  function successCallback()
    if self._player then
      local isMyFriend = g.friendsInfo:isMyFriends(self._player:getUid())
      self:setAddFriendStatus(isMyFriend)
    end
  end
  function failCallback()
  end
  g.friendsInfo:refreshData(successCallback, failCallback)
end
function OtherUserProfileView:loadUserRank(uid)
  local function successCallback(retData)
    local info = retData.info
    if info ~= nil then
      self._rankLabelNumber:setString(g.util.functions.formatBigInt(info.profit or 0))
      local win = info.win
      local lose = info.lose
      local draw = info.draw
      local esc = info.esc
      local total = win + lose + draw + esc
      if total == 0 then
        self._winRateLabelNumber:setString("0%")
      else
        local winRate = win / total * 100
        self._winRateLabelNumber:setString(tostring(winRate - winRate % 0.1) .. "%")
      end
      local levelStr = string.gsub(g.lang:getContent("common.level"), "{level}", g.levelInfo:getLevelByExperience(tonumber(info.exp or 0)))
      self._levelLabel:setString(levelStr)
      self._levelProgressBar:setValue(g.levelInfo:getLevelUpProgress(tonumber(g.vars.user.exp)))
    end
    self.rankRequestId = nil
  end
  local function failCallback()
    self.rankRequestId = nil
  end
  if self.rankRequestId == nil then
    self.rankRequestId = g.friendsInfo:searchUserRank(uid, successCallback, failCallback)
  end
end
function OtherUserProfileView:setAddFriendStatus(isMyFriend)
  self._addFriendBtn:setButtonEnabled(true)
  if not isMyFriend then
    self._addFriendBtn:setButtonImage("normal", "#common_add_friend_up.png", true)
    self._addFriendBtn:setButtonImage("pressed", "#common_add_friend_down.png", true)
    self._isMyFriend = false
  else
    self._addFriendBtn:setButtonImage("normal", "#common_delete_friend_up.png", true)
    self._addFriendBtn:setButtonImage("pressed", "#common_delete_friend_down.png", true)
    self._isMyFriend = true
  end
end
function OtherUserProfileView:_onFriendBtnClick(evt)
  if self._isMyFriend then
    self:_onDelFriendClick(evt)
  else
    self:_onAddFriendClick(evt)
  end
end
function OtherUserProfileView:_onAddFriendClick(evt)
  self._addFriendBtn:setButtonEnabled(false)
  function successCallback(jsonData)
    if jsonData and jsonData.ret == 0 then
      self._addFriendBtn:setButtonEnabled(true)
      g.ui.manager.tip:top(g.lang:getContent("friend.addFriendSuccess"))
      self:hidePanel()
    else
      self:setAddFriendStatus(false)
      g.ui.manager.tip:top(g.lang:getContent("friend.addFriendFail"))
    end
  end
  function failCallback()
    self:setAddFriendStatus(false)
    g.ui.manager.tip:top(g.lang:getContent("friend.addFriendFail"))
  end
  g.friendsInfo:applyFriend(self._player:getUid(), successCallback, failCallback)
end
function OtherUserProfileView:_onDelFriendClick(evt)
  self._addFriendBtn:setButtonEnabled(false)
  function successCallback(jsonData)
    if jsonData and jsonData.ret == 0 then
      self:setAddFriendStatus(false)
      g.ui.manager.tip:top(g.lang:getContent("friend.deleteFriendSuccess"))
    else
      self:setAddFriendStatus(true)
      g.ui.manager.tip:top(g.lang:getContent("friend.deleteFriendFail"))
    end
  end
  function failCallback()
    self:setAddFriendStatus(true)
    g.ui.manager.tip:top(g.lang:getContent("friend.deleteFriendFail"))
  end
  g.friendsInfo:delFriend(self._player:getUid(), successCallbak, failCallback)
end
function OtherUserProfileView:onExit()
  if self.rankRequestId ~= nil then
    self.rankRequestId:cancel()
    self.rankRequestId = nil
  end
end
return OtherUserProfileView

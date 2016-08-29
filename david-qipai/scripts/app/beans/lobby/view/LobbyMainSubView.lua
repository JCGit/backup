local AvatarView = require("app.ui.imageview.AvatarView")
local LobbyConfig = require("app.beans.lobby.view.LobbyConfig")
local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local LobbyMainSubView = class("LobbyMainSubView", function()
  return display.newNode()
end)
local FriendsWindowSubView = import(".FriendsWindowSubView")
local ProfileView = require("app.beans.profile.view.ProfileView")
local GiftShopView = require("app.beans.giftshop.view.GiftShopView")
local PaymentView = require("app.beans.payment.view.PaymentView")
local RankBoardView = require("app.beans.rankboard.RankBoardView")
local SettingView = require("app.beans.setting.view.SettingView")
local FriendListView = require("app.beans.friend.view.FriendListView")
local HelpView = require("app.beans.help.view.HelpView")
local FeedbackView = require("app.beans.feedback.view.FeedbackView")
local MessagePopup = require("app.beans.message.MessagePopup")
local DailyTaskView = require("app.beans.dailytask.view.DailyTaskView")
local RouletteView = require("app.beans.roulette.view.RouletteView")
local DailyBonusView = require("app.beans.dailybonus.view.DailyBonusView")
local LevelUpView = require("app.beans.levelup.view.LevelUpView")
local ExchangeRewardsPopup = import("app.beans.activity.ExchangeRewardsPopup")
local InviteFriendsEventPanel = require("app.beans.invitefriends.InviteFriendsEventPanel")
local PlayTogetherPopup = require("app.beans.playtogether.PlayTogetherPopup")
LobbyMainSubView.AVATAR_HEIGHT = 82
local MORE_CHIP_PANEL_HEIGHT = 140
local BRICK_WIDTH = 180
local BRICK_HEIGHT = 220
local BOTTOM_USER_INFO_WIDTH = 300 * g.scaleWidth
local BOTTOM_MAIN_BTN_WIDTH = 140 * g.scaleWidth
local BOTTOM_SUB_BTN_WIDTH = 90 * g.scaleWidth
local BOTTOM_PANEL_HEIGHT = 107
function LobbyMainSubView:ctor(showRommListViewBlock, showLoginViewBlock, logoutHandler, isFromRoomList, rightMainOperView)
  self._isFromRoomList = isFromRoomList
  self.showRommListViewBlock = showRommListViewBlock
  self.showLoginViewBlock = showLoginViewBlock
  self.logoutHandler = logoutHandler
  self.rightMainOperView = rightMainOperView
  self:setNodeEventEnabled(true)
  self:createFriendsWinOperView()
  self:createMainOperView()
  self:createBottomOperView()
  self:addListener()
end
function LobbyMainSubView:createTableView()
end
function LobbyMainSubView:createFriendsWinOperView()
  self._friendsWindowSubView = FriendsWindowSubView.new():addTo(self.rightMainOperView)
end
LobbyMainSubView.ButtonBoardXGap = 16
LobbyMainSubView.ButtonBoardYGap = 36
LobbyMainSubView.MainButtonWidth = 270
LobbyMainSubView.MainButtonHeight = 109
function LobbyMainSubView:createMainOperView()
  self._mainOperNode = display.newNode():addTo(self.rightMainOperView)
  self._startupNode = display.newNode():addTo(self._mainOperNode)
  self._startUpBtnIcon = display.newSprite("#mainlobby_play_icon.png"):pos(-100, 14)
  self._startUpLightNode = display.newNode()
  self._totalOnlineLabel = ui.newTTFLabel({
    text = string.gsub(g.lang:getContent("lobby.totalOnline"), "{player}", "..."),
    color = ccc3(247, 231, 107),
    size = 16,
    align = ui.TEXT_ALIGN_RIGHT,
    dimensions = cc.size(225, 30)
  }):pos(0, -21)
  local pointX = display.cx - LobbyMainSubView.ButtonBoardXGap * g.scaleWidth - LobbyMainSubView.MainButtonWidth / 2
  local pointY = display.cy - LobbyMainSubView.ButtonBoardYGap * g.scaleHeight - LobbyMainSubView.MainButtonHeight / 2
  self._startupBtn = cc.ui.UIPushButton.new({
    normal = "#mainlobby_play_up.png"
  }, {scale9 = false}):onButtonPressed(function(event)
    self._startupBtn:scale(1.1)
  end):onButtonRelease(function(event)
    self._startupBtn:scale(1)
  end):pos(pointX, pointY):add(self._totalOnlineLabel):add(self._startUpLightNode):add(self._startUpBtnIcon):addTo(self._startupNode):onButtonClicked(clickHandler(self, self.onPlayNowClick))
  pointY = pointY - LobbyMainSubView.MainButtonHeight
  self._tableListNode = display.newNode():addTo(self._mainOperNode)
  self._tableListLabel = ui.newTTFLabel({
    text = g.lang:getContent("lobby.tableListLabel"),
    color = ccc3(131, 231, 74),
    size = 16,
    align = ui.TEXT_ALIGN_RIGHT,
    dimensions = cc.size(225, 30)
  }):pos(0, -21)
  self._tableListBtnIcon = display.newSprite("#mainlobby_tablelist_icon.png"):pos(-100, 14)
  self._tableListLightNode = display.newNode()
  self._tableListBtn = cc.ui.UIPushButton.new({
    normal = "#mainlobby_tablelist_up.png"
  }, {scale9 = false}):onButtonPressed(function(event)
    self._tableListBtn:scale(1.1)
  end):onButtonRelease(function(event)
    self._tableListBtn:scale(1)
  end):pos(pointX, pointY):add(self._tableListLabel):add(self._tableListLightNode):add(self._tableListBtnIcon):addTo(self._tableListNode):onButtonClicked(clickHandler(self, self.onTableListClick))
  pointY = pointY - LobbyMainSubView.MainButtonHeight
  self._matchNode = display.newNode():addTo(self._mainOperNode)
  self._matchBtnIcon = display.newSprite("#mainlobby_match_icon.png"):pos(-100, 14)
  self._matchOnlineLabel = ui.newTTFLabel({
    text = string.gsub(g.lang:getContent("lobby.matchOnline"), "{player}", "..."),
    color = ccc3(90, 243, 239),
    size = 16,
    align = ui.TEXT_ALIGN_RIGHT,
    dimensions = cc.size(225, 30)
  }):pos(0, -21)
  self._matchLightNode = display.newNode()
  self._matchBtn = cc.ui.UIPushButton.new({
    normal = "#mainlobby_match_up.png"
  }, {scale9 = false}):onButtonPressed(function(event)
    self._matchBtn:scale(1.1)
  end):onButtonRelease(function(event)
    self._matchBtn:scale(1)
  end):pos(pointX, pointY):add(self._matchOnlineLabel):add(self._matchLightNode):add(self._matchBtnIcon):addTo(self._matchNode):onButtonClicked(clickHandler(self, self.onMatchButtonClick))
  pointY = pointY - LobbyMainSubView.MainButtonHeight
  self._freeNode = display.newNode():addTo(self._mainOperNode)
  self._freeBtnIcon = display.newSprite("#mainlobby_freechips_icon.png"):pos(-100, 14)
  self._freeLabel = ui.newTTFLabel({
    text = g.lang:getContent("lobby.freeLabel"),
    color = ccc3(255, 214, 255),
    size = 16,
    align = ui.TEXT_ALIGN_RIGHT,
    dimensions = cc.size(225, 30)
  }):pos(0, -21)
  self._freeLightNode = display.newNode()
  self._freeBtn = cc.ui.UIPushButton.new({
    normal = "#mainlobby_freechips_up.png"
  }, {scale9 = false}):onButtonPressed(function(event)
    self._freeBtn:scale(1.1)
  end):onButtonRelease(function(event)
    self._freeBtn:scale(1)
  end):pos(pointX, pointY):add(self._freeLabel):add(self._freeLightNode):add(self._freeBtnIcon):addTo(self._freeNode):onButtonClicked(clickHandler(self, self.onFreeChipClick))
  self._oprButtonLightNodes = {
    self._startUpLightNode,
    self._tableListLightNode,
    self._matchLightNode,
    self._freeLightNode
  }
  self._oprButtons = {
    self._startupBtn,
    self._tableListBtn,
    self._matchNode,
    self._freeBtn
  }
  self:schedule(handler(self, self.playOprButtonAnimation), 3)
  g.tablesInfo:getTotalOnline(handler(self, self.onTotalOnlineLoaded))
end
LobbyMainSubView.BottomButtonWidth = 73
LobbyMainSubView.BottomButtonHeight = 70
function LobbyMainSubView:createBottomOperView()
  self._bottomPanelNode = display.newNode():addTo(self, LobbyConfig.BOTTOM_NODE_Z_ORDER)
  local pointX = -display.cx + LobbyMainSubView.BottomButtonWidth * 0.5
  local pointY = -display.cy + LobbyMainSubView.BottomButtonHeight * 0.5
  cc.ui.UIPushButton.new({
    normal = "#mainlobby_setting_normal.png",
    pressed = "#mainlobby_setting_pressed.png"
  }, {scale9 = false}):pos(pointX, pointY):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onSettingButtonClick))
  pointX = display.cx - 320
  local messageButton = cc.ui.UIPushButton.new({
    normal = "#mainlobby_message_normal.png",
    pressed = "#mainlobby_message_pressed.png"
  }, {scale9 = false}):pos(pointX, pointY):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onMessageButtonClick))
  if g.vars.user.unreadMessage > 0 then
    self.messageRedDot = display.newSprite("#common_red_dot.png"):pos(23, 20):addTo(messageButton)
  end
  pointX = pointX - g.scaleWidth * 84
  cc.ui.UIPushButton.new({
    normal = "#mainlobby_friend_normal.png",
    pressed = "#mainlobby_friend_pressed.png"
  }, {scale9 = false}):pos(pointX, pointY):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onFriendBtnClicked))
  pointX = pointX - g.scaleWidth * 84
  local payButton = cc.ui.UIPushButton.new({
    normal = "#mainlobby_recharge_normal.png",
    pressed = "#mainlobby_recharge_pressed.png"
  }, {scale9 = false}):pos(pointX, pointY):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onPayButtonClick))
  if g.vars.user.chipsMore and 0 < g.vars.user.chipsMore then
    self._promotionIcon = display.newSprite("#mainlobby_promotion_icon.png"):pos(-28, 11)
    self._promotionLabel = ui.newTTFLabel({
      text = string.gsub(g.lang:getContent("lobby.promotionLabel"), "{more}", g.vars.user.chipsMore),
      color = g.font.color.LIGHT,
      size = 22,
      align = ui.TEXT_ALIGN_LEFT
    }):pos(40, 37):addTo(self._promotionIcon)
    self._promotionIcon:setAnchorPoint(ccp(0.1, 0))
    payButton:add(self._promotionIcon)
    self._promotionIcon:schedule(handler(self, self.playPromotionAnimation), 5)
  end
  pointX = pointX - g.scaleWidth * 84
  cc.ui.UIPushButton.new({
    normal = "#mainlobby_mall_normal.png",
    pressed = "#mainlobby_mall_pressed.png"
  }, {scale9 = false}):pos(pointX, pointY):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onGiftShopButtonClick))
  pointX = pointX - g.scaleWidth * 85
  cc.ui.UIPushButton.new({
    normal = "#mainlobby_rankboard_normal.png",
    pressed = "#mainlobby_rankboard_pressed.png"
  }, {scale9 = false}):pos(pointX, pointY):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onRankingBtnClicked))
  pointX = pointX - g.scaleWidth * 85
  cc.ui.UIPushButton.new({
    normal = "#mainlobby_feedback_normal.png",
    pressed = "#mainlobby_feedback_pressed.png"
  }, {scale9 = false}):pos(pointX, pointY):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onFeedbackButtonClick))
  pointX = pointX - g.scaleWidth * 85
  cc.ui.UIPushButton.new({
    normal = "#mainlobby_question_normal.png",
    pressed = "#mainlobby_question_pressed.png"
  }, {scale9 = false}):pos(pointX, pointY):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onHelpButtonClick))
  self._userInfoBtn = cc.ui.UIPushButton.new({
    normal = "#common_transparent_skin.png",
    pressed = "#common_transparent_skin.png"
  }, {scale9 = true}):setButtonSize(BOTTOM_USER_INFO_WIDTH, BOTTOM_PANEL_HEIGHT - 2):pos(display.cx - LobbyMainSubView.AVATAR_HEIGHT * 0.5 - 7, -display.cy + LobbyMainSubView.AVATAR_HEIGHT * 0.5 + 5):addTo(self._bottomPanelNode):onButtonClicked(clickHandler(self, self.onUserInfoBtnClicked))
  self._headPic = AvatarView.new():pos(display.cx - LobbyMainSubView.AVATAR_HEIGHT * 0.5 - 7, -display.cy + LobbyMainSubView.AVATAR_HEIGHT * 0.5 + 5):addTo(self._bottomPanelNode, 0):scale(LobbyMainSubView.AVATAR_HEIGHT / AvatarView.DEFAULT_HEIGHT)
  self._userNameLabel = ui.newTTFLabel({
    text = g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 24, g.vars.user.name, 175),
    color = g.font.color.LIGHT,
    size = 24,
    align = ui.TEXT_ALIGN_RIGHT
  }):align(display.RIGHT_CENTER, display.cx - 105, -display.cy + 60):addTo(self._bottomPanelNode)
  display.newSprite("#mainlobby_chips_icon.png"):align(display.RIGHT_CENTER, display.cx - 105, -display.cy + 27):addTo(self._bottomPanelNode)
  self._chipsLabel = ui.newTTFLabel({
    text = string.formatnumberthousands(g.vars.user.chips),
    size = 24,
    color = g.font.color.GOLDEN,
    align = ui.TEXT_ALIGN_RIGHT
  }):align(display.RIGHT_CENTER, display.cx - 135, -display.cy + 25):addTo(self._bottomPanelNode)
  self:updateUserInfo()
end
function LobbyMainSubView:onEnter()
end
function LobbyMainSubView:playShowAnim(animTime, baseDelayTime)
  function animationComplete()
    if g.vars.user.tid ~= nil then
      g.tablesInfo:getPlayTableWithTid(g.vars.user.tid, nil, nil)
      g.vars.user.tid = nil
    else
      g.loginInfo:showRewardView()
    end
  end
  local delayGapTime = 0.05
  local friendsWidowAnimationDuration = LobbyConfig.ANIMATION_TIME
  self._friendsWindowSubView:pos(-5, -display.cy * 0.5)
  transition.moveTo(self._friendsWindowSubView, {
    time = friendsWidowAnimationDuration,
    y = 0,
    delay = baseDelayTime + delayGapTime * 3,
    easing = "BACKOUT",
    onComplete = function()
      self._friendsWindowSubView:reloadData()
    end
  })
  self._friendsWindowSubView:setCascadeOpacityEnabled(true)
  self._friendsWindowSubView:hide()
  transition.fadeOut(self._friendsWindowSubView, {
    opacity = 0,
    time = baseDelayTime + delayGapTime * 3,
    onComplete = function()
      self._friendsWindowSubView:show()
    end
  })
  self._friendsWindowSubView:setOpacity(0)
  transition.fadeIn(self._friendsWindowSubView, {
    opacity = 255,
    time = friendsWidowAnimationDuration,
    delay = baseDelayTime + delayGapTime * 3
  })
  local buttonAnimationDuration = LobbyConfig.ANIMATION_TIME
  local buttonYDistance = display.height - 200
  local pointX = display.cx - LobbyMainSubView.ButtonBoardXGap * g.scaleWidth - LobbyMainSubView.MainButtonWidth / 2
  local pointY = display.cy - LobbyMainSubView.ButtonBoardYGap * g.scaleHeight - LobbyMainSubView.MainButtonHeight / 2
  self._startupBtn:pos(pointX, pointY - buttonYDistance)
  transition.moveTo(self._startupBtn, {
    time = buttonAnimationDuration,
    y = pointY,
    delay = baseDelayTime + delayGapTime,
    easing = "BACKOUT"
  })
  self._startupNode:setCascadeOpacityEnabled(true)
  self._startupNode:setOpacity(0)
  transition.fadeIn(self._startupNode, {
    opacity = 255,
    time = buttonAnimationDuration,
    delay = animTime + delayGapTime
  })
  pointY = pointY - LobbyMainSubView.MainButtonHeight
  self._tableListBtn:pos(pointX, pointY - buttonYDistance)
  transition.moveTo(self._tableListBtn, {
    time = buttonAnimationDuration,
    y = pointY,
    delay = baseDelayTime + delayGapTime * 2,
    easing = "BACKOUT"
  })
  self._tableListNode:setCascadeOpacityEnabled(true)
  self._tableListNode:setOpacity(0)
  transition.fadeIn(self._tableListNode, {
    opacity = 255,
    time = buttonAnimationDuration,
    delay = baseDelayTime + delayGapTime * 2
  })
  pointY = pointY - LobbyMainSubView.MainButtonHeight
  self._matchBtn:pos(pointX, pointY - buttonYDistance)
  transition.moveTo(self._matchBtn, {
    time = buttonAnimationDuration,
    y = pointY,
    delay = baseDelayTime + delayGapTime * 3,
    easing = "BACKOUT"
  })
  self._matchNode:setCascadeOpacityEnabled(true)
  self._matchNode:setOpacity(0)
  transition.fadeIn(self._matchNode, {
    opacity = 255,
    time = buttonAnimationDuration,
    delay = baseDelayTime + delayGapTime * 3
  })
  pointY = pointY - LobbyMainSubView.MainButtonHeight
  self._freeBtn:pos(pointX, pointY - buttonYDistance)
  transition.moveTo(self._freeBtn, {
    time = buttonAnimationDuration,
    y = pointY,
    delay = baseDelayTime + delayGapTime * 4,
    easing = "BACKOUT"
  })
  self._freeNode:setCascadeOpacityEnabled(true)
  self._freeNode:setOpacity(0)
  transition.fadeIn(self._freeNode, {
    opacity = 255,
    time = buttonAnimationDuration,
    delay = baseDelayTime + delayGapTime * 4,
    onComplete = animationComplete
  })
  if self._bottomPanelNode ~= nil then
    self._bottomPanelNode:pos(0, -BOTTOM_PANEL_HEIGHT)
    transition.moveTo(self._bottomPanelNode, {
      time = animTime,
      y = 0,
      delay = baseDelayTime + delayGapTime * 4 + buttonAnimationDuration * 0.5,
      easing = "BACKOUT"
    })
    self._bottomPanelNode:setCascadeOpacityEnabled(true)
    self._bottomPanelNode:setOpacity(0)
    transition.fadeIn(self._bottomPanelNode, {
      opacity = 255,
      time = animTime,
      delay = baseDelayTime + delayGapTime * 4 + buttonAnimationDuration * 0.5
    })
  end
end
function LobbyMainSubView:playPromotionAnimation()
  if self.isPromotionAnimPlaying then
    return
  end
  local sequence = transition.sequence({
    CCRotateTo:create(1, -20),
    CCRotateTo:create(1, 5),
    CCRotateTo:create(1, 0)
  })
  self._promotionIcon:runAction(sequence)
end
function LobbyMainSubView:playOprButtonAnimation()
  if self._oprButtonAnimPlaying then
    return
  end
  local index = math.random(1, 4)
  index = 4
  while index == 3 or index == self._lastAnimButtonIndex do
    index = math.random(1, 4)
  end
  local coverLight = display.newSprite("#mainlobby_button_light_cover.png"):pos(0, 8):opacity(0):addTo(self._oprButtons[index])
  local sequence = transition.sequence({
    CCFadeTo:create(0.2, 255),
    CCFadeTo:create(0.2, 0)
  })
  transition.execute(coverLight, sequence, {
    onComplete = handler(self, function(self)
      if coverLight and coverLight:getParent() then
        coverLight:removeFromParent()
      end
      local topLight = display.newSprite("#mainlobby_button_light_bottom.png"):pos(-75, 49):opacity(0):addTo(self._oprButtonLightNodes[index])
      local bottomLight = display.newSprite("#mainlobby_button_light_bottom.png"):pos(84, -38):opacity(0):addTo(self._oprButtonLightNodes[index])
      local midLightRight = display.newSprite("#mainlobby_button_light_mid_right.png"):pos(-108, 5):opacity(0):addTo(self._oprButtonLightNodes[index])
      local midLightMid = display.newSprite("#mainlobby_button_light_mid_right.png"):pos(-108, 5):opacity(0):addTo(self._oprButtonLightNodes[index])
      local midLightLeft = display.newSprite("#mainlobby_button_light_mid_left.png"):pos(-108, 5):opacity(0):addTo(self._oprButtonLightNodes[index])
      transition.moveTo(topLight, {time = 1, x = 85})
      transition.moveTo(bottomLight, {time = 1, x = -80})
      local fadeSequnce = transition.sequence({
        CCFadeTo:create(0.3, 255),
        CCDelayTime:create(0.4),
        CCFadeTo:create(0.3, 0)
      })
      transition.execute(topLight, fadeSequnce, {
        onComplete = handler(self, function(self)
          if topLight and topLight:getParent() then
            topLight:removeFromParent()
          end
        end)
      })
      fadeSequnce = transition.sequence({
        CCFadeTo:create(0.3, 255),
        CCDelayTime:create(0.4),
        CCFadeTo:create(0.3, 0)
      })
      transition.execute(bottomLight, fadeSequnce, {
        onComplete = handler(self, function(self)
          if bottomLight and bottomLight:getParent() then
            bottomLight:removeFromParent()
          end
        end)
      })
      transition.moveTo(midLightRight, {
        time = 0.6,
        x = 75,
        delay = 0.15
      })
      transition.moveTo(midLightMid, {
        time = 0.6,
        x = 75,
        delay = 0.3
      })
      transition.moveTo(midLightLeft, {
        time = 0.6,
        x = 75,
        delay = 0.45
      })
      local sequence = transition.sequence({
        CCFadeTo:create(0.3, 128),
        CCFadeTo:create(0.3, 0)
      })
      transition.execute(midLightRight, sequence, {
        onComplete = handler(self, function(self)
          if midLightRight and midLightRight:getParent() then
            midLightRight:removeFromParent()
          end
        end),
        delay = 0.15
      })
      sequence = transition.sequence({
        CCFadeTo:create(0.3, 128),
        CCFadeTo:create(0.3, 0)
      })
      transition.execute(midLightMid, sequence, {
        onComplete = handler(self, function(self)
          if midLightMid and midLightMid:getParent() then
            midLightMid:removeFromParent()
          end
        end),
        delay = 0.3
      })
      sequence = transition.sequence({
        CCFadeTo:create(0.3, 128),
        CCFadeTo:create(0.3, 0)
      })
      transition.execute(midLightLeft, sequence, {
        onComplete = handler(self, function(self)
          if midLightLeft and midLightLeft:getParent() then
            midLightLeft:removeFromParent()
          end
          self._oprButtonAnimPlaying = false
        end),
        delay = 0.45
      })
    end)
  })
  self._lastAnimButtonIndex = index
  self._oprButtonAnimPlaying = true
end
function LobbyMainSubView:onTotalOnlineLoaded(data)
  local text = string.gsub(g.lang:getContent("lobby.totalOnline"), "{player}", data.info)
  if self._totalOnlineLabel then
    self._totalOnlineLabel:setString(text)
  end
end
function LobbyMainSubView:playHideLobbyMainSubViewAnimation()
  local animTime = LobbyConfig.HIDE_LOBBY_ANIMATION_TIME
  self._friendsWindowSubView:dispose()
  self._startupNode:fadeOut(animTime)
  self._tableListNode:fadeOut(animTime)
  self._matchNode:fadeOut(animTime)
  self._freeNode:fadeOut(animTime)
  transition.moveTo(self._bottomPanelNode, {
    time = animTime,
    y = -BOTTOM_PANEL_HEIGHT
  })
  self._bottomPanelNode:setCascadeOpacityEnabled(true)
  self._bottomPanelNode:setOpacity(0)
  transition.fadeOut(self._bottomPanelNode, {time = animTime})
  transition.fadeOut(self._friendsWindowSubView, {
    time = animTime,
    onComplete = function()
      self._mainOperNode:removeFromParent()
      self.rightMainOperView:removeAllChildren()
      self:removeFromParent()
    end
  })
end
function LobbyMainSubView:hideFreeChipsNode()
  if self.hideFreeChipCallback then
    self.hideFreeChipCallback()
  end
  self._freeChipsNode:stopAllActions()
  transition.moveTo(self._freeChipsNode, {
    time = 0.3,
    y = -140,
    onComplete = handler(self, function(self)
      transition.moveTo(self._bottomPanelNode, {
        time = 0.5,
        y = 0,
        easing = "BACKOUT",
        delay = 0.3,
        onComplete = handler(self, function(self)
          self._freeChipAnimationPlaying = false
        end)
      })
      self._freeChipsNode:removeFromParent()
      self._freeChipsNode = nil
    end)
  })
  self._freeChipsCover:removeFromParent()
  self._freeChipsCover = nil
  self._freeChipsCoverNode:removeFromParent()
  self._freeChipsCoverNode = nil
  self._freeChipAnimationPlaying = true
end
function LobbyMainSubView:showFreeChipsNode()
  if self.showFreeChipCallback then
    self.showFreeChipCallback()
  end
  transition.moveTo(self._bottomPanelNode, {
    time = 0.3,
    y = -120,
    onComplete = handler(self, function(self)
      transition.moveTo(self._freeChipsNode, {
        time = 0.6,
        y = 0,
        easing = "BACKOUT",
        delay = 0.3,
        onComplete = handler(self, function(self)
          self._freeChipAnimationPlaying = false
        end)
      })
    end)
  })
  self._freeChipAnimationPlaying = true
end
function LobbyMainSubView:onFreeChipClick()
  self:setButtonTemporaryDisable()
  if self._freeChipAnimationPlaying then
    return
  end
  g.dataLogger:logEvent("大厅点击事件", "免费筹码点击")
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  else
    self._freeChipsCoverNode = display.newNode():pos(0, 0):addTo(self, LobbyConfig.FREE_CHIP_COVER_Z_ORDER)
    self._freeChipsCoverNode:setTouchEnabled(true)
    self._freeChipsCover = display.newScale9Sprite("#panel_cover.png", 0, 0, CCSize(display.width, display.height)):addTo(self._freeChipsCoverNode)
    clickHelper(self._freeChipsCover, handler(self, self.onCoverTouch))
    self._freeChipsNode = display.newNode():pos(0, -140):addTo(self, LobbyConfig.FREE_CHIP_NODE_Z_ORDER)
    self._freeChipsNode:setTouchEnabled(true)
    local activityList = g.activityCenter.activityList or {}
    local totalButton = #activityList
    local btnGap = 140
    local posX = -(totalButton - 1) * btnGap / 2
    if totalButton > 0 then
      for i, activity in ipairs(activityList) do
        local activityID = tonumber(activity.activityID or 0)
        local needShowRedDot = g.activityCenter:getActivityNeedShowRedDot(activityID)
        if activityID == 1 then
        elseif activityID == 2 then
          local label = ui.newTTFLabel({
            text = g.lang:getContent("lobby.dailyMisson"),
            color = g.font.color.GOLDEN,
            size = 24,
            align = ui.TEXT_ALIGN_CENTER
          }):pos(0, -54)
          self._dailyMissionBtn = cc.ui.UIPushButton.new({
            normal = "#mainlobby_task_normal.png"
          }, {scale9 = false}):pos(posX, -display.cy + 75):onButtonPressed(function(event)
            self._dailyMissionBtn:scale(1.1)
          end):onButtonRelease(function(event)
            self._dailyMissionBtn:scale(1)
          end):onButtonClicked(clickHandler(self, self.onDailyMissionClick, activityID)):add(label):addTo(self._freeChipsNode)
          if needShowRedDot then
            self.taskRedDot = display.newSprite("#common_red_dot.png"):pos(30, 33):addTo(self._dailyMissionBtn)
          end
        elseif activityID == 3 then
          local label = ui.newTTFLabel({
            text = g.lang:getContent("lobby.roulette"),
            color = g.font.color.GOLDEN,
            size = 24,
            align = ui.TEXT_ALIGN_CENTER
          }):pos(0, -54)
          self._rouletteButton = cc.ui.UIPushButton.new({
            normal = "#mainlobby_roulette_normal.png"
          }, {scale9 = false}):pos(posX, -display.cy + 75):onButtonPressed(function(event)
            self._rouletteButton:scale(1.1)
          end):onButtonRelease(function(event)
            self._rouletteButton:scale(1)
          end):onButtonClicked(clickHandler(self, self.onRouletteButtonClick, activityID)):add(label):addTo(self._freeChipsNode)
          if needShowRedDot then
            self.rouletteRedDot = display.newSprite("#common_red_dot.png"):pos(30, 33):addTo(self._rouletteButton)
          end
        elseif activityID == 4 then
          local label = ui.newTTFLabel({
            text = g.lang:getContent("lobby.exchangeCode"),
            color = g.font.color.GOLDEN,
            size = 24,
            align = ui.TEXT_ALIGN_CENTER
          }):pos(0, -54)
          self._exchangeCodeButton = cc.ui.UIPushButton.new({
            normal = "#mainlobby_exchange_code_normal.png"
          }, {scale9 = false}):pos(posX, -display.cy + 75):onButtonPressed(function(event)
            self._exchangeCodeButton:scale(1.1)
          end):onButtonRelease(function(event)
            self._exchangeCodeButton:scale(1)
          end):onButtonClicked(clickHandler(self, self.onExchangeCodeClick)):add(label):addTo(self._freeChipsNode)
        elseif activityID == 5 then
          local label = ui.newTTFLabel({
            text = g.lang:getContent("lobby.dailyFeed"),
            color = g.font.color.GOLDEN,
            size = 24,
            align = ui.TEXT_ALIGN_CENTER
          }):pos(0, -54)
          self._dailyFeedBtn = cc.ui.UIPushButton.new({
            normal = "#mainlobby_dailyfeed_normal.png"
          }, {scale9 = false}):pos(posX, -display.cy + 75):onButtonPressed(function(event)
            self._dailyFeedBtn:scale(1.1)
          end):onButtonRelease(function(event)
            self._dailyFeedBtn:scale(1)
          end):onButtonClicked(clickHandler(self, self.onDailyFeedClick, activityID)):add(label):addTo(self._freeChipsNode)
          if needShowRedDot then
            self.dailyFeedRedDot = display.newSprite("#common_red_dot.png"):pos(30, 30):addTo(self._dailyFeedBtn)
          end
        elseif activityID == 6 then
          local label = ui.newTTFLabel({
            text = g.lang:getContent("lobby.inviteFriends"),
            color = g.font.color.GOLDEN,
            size = 24,
            align = ui.TEXT_ALIGN_CENTER
          }):pos(0, -54)
          self._commonInviteButton = cc.ui.UIPushButton.new({
            normal = "#mainlobby_invite_common.png"
          }, {scale9 = false}):pos(posX, -display.cy + 75):onButtonPressed(function(event)
            self._commonInviteButton:scale(1.1)
          end):onButtonRelease(function(event)
            self._commonInviteButton:scale(1)
          end):onButtonClicked(clickHandler(self, self.onNormalInviteClick)):add(label):addTo(self._freeChipsNode)
        elseif activityID == 7 then
          local label = ui.newTTFLabel({
            text = g.lang:getContent("lobby.crazyInviteFriends"),
            color = g.font.color.GOLDEN,
            size = 24,
            align = ui.TEXT_ALIGN_CENTER
          }):pos(0, -54)
          self._inviteRewardButton = cc.ui.UIPushButton.new({
            normal = "#mainlobby_invite_normal.png"
          }, {scale9 = false}):pos(posX, -display.cy + 75):onButtonPressed(function(event)
            self._inviteRewardButton:scale(1.1)
          end):onButtonRelease(function(event)
            self._inviteRewardButton:scale(1)
          end):onButtonClicked(clickHandler(self, self.onInviteRewardClick, activityID)):add(label):addTo(self._freeChipsNode)
          if needShowRedDot then
            self.crazyInviteRedDot = display.newSprite("#common_red_dot.png"):pos(30, 30):addTo(self._inviteRewardButton)
          end
        elseif activityID == 8 then
          local label = ui.newTTFLabel({
            text = g.lang:getContent("lobby.playTogether"),
            color = g.font.color.GOLDEN,
            size = 24,
            align = ui.TEXT_ALIGN_CENTER
          }):pos(0, -54)
          self._playTogetherRewardButton = cc.ui.UIPushButton.new({
            normal = "#mainlobby_playtogether_normal.png"
          }, {scale9 = false}):pos(posX, -display.cy + 75):onButtonPressed(function(event)
            self._playTogetherRewardButton:scale(1.1)
          end):onButtonRelease(function(event)
            self._playTogetherRewardButton:scale(1)
          end):onButtonClicked(clickHandler(self, self.onPlayTogetherRewardClick, activityID)):add(label):addTo(self._freeChipsNode)
          if needShowRedDot then
            self.playTogetherRedDot = display.newSprite("#common_red_dot.png"):pos(30, 30):addTo(self._playTogetherRewardButton)
          end
        end
        posX = posX + btnGap
      end
    end
    self:showFreeChipsNode()
  end
end
function LobbyMainSubView:setButtonTemporaryDisable()
  self:performWithDelay(function()
    self._matchBtn:setButtonEnabled(true)
    self._tableListBtn:setButtonEnabled(true)
    self._startupBtn:setButtonEnabled(true)
    self._freeBtn:setButtonEnabled(true)
  end, 1)
  self._matchBtn:setButtonEnabled(false)
  self._tableListBtn:setButtonEnabled(false)
  self._tableListBtn:setButtonEnabled(false)
  self._freeBtn:setButtonEnabled(false)
end
function LobbyMainSubView:onTableListClick()
  self.showRommListViewBlock(1)
  g.dataLogger:logEvent("大厅点击事件", "新手场点击")
  self:setButtonTemporaryDisable()
end
local UserBankruptPopup = require("app.module.table.view.UserBankruptPopup")
local TableRecordList = require("app.beans.friend.view.TableRecordList")
function LobbyMainSubView:onMatchButtonClick()
  g.dataLogger:logEvent("大厅点击事件", "比赛场点击")
  self:setButtonTemporaryDisable()
  g.ui.manager.tip:top(g.lang:getContent("common.commingSoon"))
end
function LobbyMainSubView:onPlayNowClick()
  g.dataLogger:logEvent("大厅点击事件", "立即开始点击")
  g.vars.table.isPlayNow = true
  g.tablesInfo:getPlayTableWithTid(nil, nil, nil)
  self:setButtonTemporaryDisable()
end
function LobbyMainSubView:onCoverTouch(target, evt)
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  end
end
function LobbyMainSubView:onDailyMissionClick(activityID)
  print(activityID)
  print("!!!!!!!!!!!!!!!!!!!!!00000")
  DailyTaskView.new():showPanel()
  if self.taskRedDot then
    self.taskRedDot:removeFromParent()
    self.taskRedDot = nil
    g.activityCenter:setActivityRedDotReaded(activityID)
  end
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  end
end
function LobbyMainSubView:onDailyBonusClick(target, evt)
  DailyBonusView.new():showPanel()
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  end
end
function LobbyMainSubView:onDailyFeedClick(activityID)
  if self.dailyFeedRedDot then
    self.dailyFeedRedDot:removeFromParent()
    g.vars.user.hasDailyFeed = 0
    self.dailyFeedRedDot = nil
    g.activityCenter:setActivityRedDotReaded(activityID)
  end
  if g.vars.user.loginType == "device" then
    g.ui.manager.tip:top(g.lang:getContent("lobby.loginFacebookToInvite"))
    return
  end
  local feedData = clone(g.lang:getContent("feed.dailyFeed"))
  feedData.picture = g.vars.user.cdn .. feedData.picture
  local str
  g.facebook:shareFeed(feedData, function(success, result)
    if not success then
      self._shareButton:setButtonEnabled(true)
      str = g.lang:getContent("feed.shareFail")
      g.ui.manager.tip:top(str)
    else
      str = g.lang:getContent("feed.shareSucc")
      g.ui.manager.tip:top(str)
      local requestData = {}
      local jsonData = json.decode(result)
      requestData.uid = g.vars.user.uid
      requestData.postId = jsonData.postId
      g.httpRequest:postMethod(g.requestParamsFactory:createRequest("daily_feed", "reward", requestData), function(data)
        local jsonData = json.decode(data)
        if jsonData and jsonData.ret == 0 then
          local str = g.lang:getContent("feed.shareSuccReward")
          g.ui.manager.tip:top(str)
        end
      end, function()
      end)
    end
  end)
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  end
end
function LobbyMainSubView:onExchangeCodeClick(target, evt)
  ExchangeRewardsPopup.new():showPanel()
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  end
end
function LobbyMainSubView:onInviteRewardClick(activityID)
  InviteFriendsEventPanel.new():showPanel()
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  end
  if self.crazyInviteRedDot then
    self.crazyInviteRedDot:removeFromParent()
    self.crazyInviteRedDot = nil
    g.activityCenter:setActivityRedDotReaded(activityID)
  end
end
function LobbyMainSubView:onNormalInviteClick()
  local selectedTab = 2
  local friendListView = FriendListView.new(selectedTab)
  friendListView:showPanel()
end
function LobbyMainSubView:onPlayTogetherRewardClick(activityID)
  PlayTogetherPopup.new():showPanel()
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  end
  if self.playTogetherRedDot then
    self.playTogetherRedDot:removeFromParent()
    self.playTogetherRedDot = nil
    g.activityCenter:setActivityRedDotReaded(activityID)
  end
end
function LobbyMainSubView:onRouletteButtonClick(activityID)
  RouletteView.new():showPanel()
  if self.rouletteRedDot then
    self.rouletteRedDot:removeFromParent()
    self.rouletteRedDot = nil
    g.activityCenter:setActivityRedDotReaded(activityID)
  end
  if self._freeChipsCover then
    self:hideFreeChipsNode()
  end
end
function LobbyMainSubView:addListener()
  self._listenerHandlers = self._listenerHandlers or {}
  local handler1 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_CHIPS_CHANGE, handler(self, self.onUserChipsChange))
  table.insert(self._listenerHandlers, handler1)
  local handler2 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_PIC_CHANGE, handler(self, self.onUserPicChange))
  table.insert(self._listenerHandlers, handler2)
  local handler3 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_NAME_CHANGE, handler(self, self.onUserNameChange))
  table.insert(self._listenerHandlers, handler3)
  local handler4 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_EXP_CHANGE, handler(self, self.onUserExpChange))
  table.insert(self._listenerHandlers, handler4)
  local handler5 = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_GENDER_CHANGE, handler(self, self.onUserGenderChange))
  table.insert(self._listenerHandlers, handler5)
  local handler6 = g.eventCenter:addEventListener(DataBeanEvent.EVT_PAYMENT_DELIVERED, handler(self, self.onPaymentDelivered))
  table.insert(self._listenerHandlers, handler6)
end
function LobbyMainSubView:removeListener()
  for i, handler in ipairs(self._listenerHandlers) do
    g.eventCenter:removeEventListener(handler)
  end
end
function LobbyMainSubView:updateUserInfo(evt)
  self:onUserNameChange()
  self:onUserChipsChange()
  self:onUserExpChange()
  self:onUserGenderChange()
  self:onUserPicChange()
end
function LobbyMainSubView:onPaymentDelivered(evt)
  g.ui.manager.tip:top(g.lang:getContent("payment.deliverSuccess"))
  if g.vars.user.firstPayEnabled and g.vars.user.firstPayEnabled == 1 then
    g.paymentInfo:loadDiscountData(handler(self, self.onDiscountLoaded))
    g.vars.user.firstPayEnabled = nil
    g.vars.user.chipsMore = 0
  end
end
function LobbyMainSubView:onDiscountLoaded(data)
  if data and data.ret == 0 then
    if 0 >= data.info then
      self._promotionIcon:stopAllActions()
      self._promotionIcon:removeFromParent()
    else
      g.vars.user.chipsMore = data.info
      local text = string.gsub(g.lang:getContent("lobby.promotionLabel"), "{more}", g.vars.user.chipsMore)
      self._promotionLabel:setString(text)
    end
  end
end
function LobbyMainSubView:onUserNameChange(evt)
  self._userNameLabel:setString(g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 24, g.vars.user.name, 175))
end
function LobbyMainSubView:onUserChipsChange(evt)
  self._chipsLabel:setString(string.formatnumberthousands(g.vars.user.chips))
end
function LobbyMainSubView:onUserExpChange(evt)
  local levelText = string.gsub(g.lang:getContent("common.level"), "{level}", g.levelInfo:getLevelByExperience(tonumber(g.vars.user.exp)))
  if self.levelLabel then
    self.levelLabel:setString(levelText)
  end
end
function LobbyMainSubView:onUserGenderChange(evt)
  if tonumber(g.vars.user.gender) == 0 then
    self._headPic:setImage(g.vars.user.sPic, {
      defaultImageName = "#common_male_avatar.png"
    })
  else
    self._headPic:setImage(g.vars.user.sPic, {
      defaultImageName = "#common_female_avatar.png"
    })
  end
end
function LobbyMainSubView:onUserPicChange(evt)
  if tonumber(g.vars.user.gender) == 0 then
    self._headPic:setImage(g.vars.user.sPic, {
      defaultImageName = "#common_male_avatar.png"
    })
  else
    self._headPic:setImage(g.vars.user.sPic, {
      defaultImageName = "#common_female_avatar.png"
    })
  end
end
function LobbyMainSubView:showMessagePoint(hasNewMessage)
  if hasNewMessage then
    self.newMessagePoint:show()
  else
    g.vars.unreadMessage = 0
  end
end
function LobbyMainSubView:onUserInfoBtnClicked()
  local profileView = ProfileView.new()
  profileView:show()
end
function LobbyMainSubView:onPayButtonClick()
  PaymentView.new():show()
end
function LobbyMainSubView:onGiftShopButtonClick()
  GiftShopView.new(1):show()
end
function LobbyMainSubView:onFriendBtnClicked()
  local friendListView = FriendListView.new()
  friendListView:showPanel()
end
function LobbyMainSubView:onRankingBtnClicked()
  RankBoardView.new():showPanel()
end
function LobbyMainSubView:onMessageButtonClick()
  self:showMessagePoint(false)
  MessagePopup.new():showPanel()
  if self.messageRedDot then
    self.messageRedDot:removeFromParent()
    g.vars.user.unreadMessage = 0
    self.messageRedDot = nil
  end
end
function LobbyMainSubView:onHelpButtonClick()
  HelpView.new():show()
end
function LobbyMainSubView:onSettingButtonClick()
  function logoutCallback()
  end
  SettingView.new(self.logoutHandler):show()
end
function LobbyMainSubView:onFeedbackButtonClick()
  FeedbackView.new():show()
end
function LobbyMainSubView:onCleanup()
  self:removeListener()
end
return LobbyMainSubView

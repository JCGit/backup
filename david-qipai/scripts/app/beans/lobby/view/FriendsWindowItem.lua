local ProfileView = require("app.beans.profile.view.ProfileView")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
local FriendsWindowItem = class("FriendsWindowItem", g.ui.ListItem)
local AvatarView = require("app.ui.imageview.AvatarView")
local FriendListView = require("app.beans.friend.view.FriendListView")
FriendsWindowItem.ONLINE_TYPE = 1
FriendsWindowItem.INVITE_TYPE = 2
FriendsWindowItem.ITEM_TYPE = FriendsWindowItem.ONLINE_TYPE
FriendsWindowItem.WIDTH = 290
FriendsWindowItem.HEIGHT = 92
FriendsWindowItem.TOPGAP = 4
FriendsWindowItem.AVATAR_WIDTH = 74
function FriendsWindowItem:ctor()
  self:setNodeEventEnabled(true)
  FriendsWindowItem.super.ctor(self, FriendsWindowItem.WIDTH, FriendsWindowItem.HEIGHT)
end
function FriendsWindowItem:initialize()
  local posY = (self.height - FriendsWindowItem.TOPGAP) * 0.5
  local bgSpriteX = FriendsWindowItem.WIDTH * 0.5
  local bgSpriteY = (FriendsWindowItem.HEIGHT - FriendsWindowItem.TOPGAP) * 0.5
  local bgSpriteSize = cc.size(FriendsWindowItem.WIDTH, FriendsWindowItem.HEIGHT - FriendsWindowItem.TOPGAP)
  local bgSprite = display.newScale9Sprite("#common_standard_bluebg_03.png", bgSpriteX, bgSpriteY, bgSpriteSize):addTo(self)
  bgSprite:setCascadeOpacityEnabled(true)
  self._headPic = AvatarView.new()
  self._headPic:pos(44, 44)
  self._headPic:addTo(bgSprite)
  self._headPic:scale(FriendsWindowItem.AVATAR_WIDTH / AvatarView.DEFAULT_HEIGHT)
  self._headPic:setCascadeOpacityEnabled(true)
  self.friendDetailButton = cc.ui.UIPushButton.new({
    normal = "#common_transparent_skin.png",
    pressed = "#common_transparent_skin.png"
  }, {scale9 = true}):setButtonSize(100, 82):onButtonClicked(handler(self, self.onFriendDetailClick)):pos(40, 40):addTo(bgSprite)
  self._userNameLable = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.LEFT_CENTER, 95, 60):addTo(bgSprite)
  self._descLabel = ui.newTTFLabel({
    text = "",
    color = ccc3(62, 162, 238),
    size = 20,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.LEFT_CENTER, 95, 22):addTo(bgSprite)
  self._traceButton = cc.ui.UIPushButton.new({
    normal = "#mainlobby_friendsItem_goto.png",
    pressed = "#mainlobby_friendsItem_goto.png",
    disabled = "#mainlobby_friendsItem_goto.png"
  }, {scale9 = false}):pos(252, 45):addTo(bgSprite):onButtonClicked(clickHandler(self, self._onTraceClick))
  self._traceButton:setTouchSwallowEnabled(false)
  self._inviteButton = cc.ui.UIPushButton.new({
    normal = "#mainlobby_friendsItem_add.png",
    pressed = "#mainlobby_friendsItem_add.png",
    disabled = "#mainlobby_friendsItem_add.png"
  }, {scale9 = false}):pos(252, 45):addTo(bgSprite):onButtonClicked(clickHandler(self, self._onInviteClick))
  self._inviteButton:setTouchSwallowEnabled(false)
  self:setCascadeOpacityEnabled(true)
end
function FriendsWindowItem:lazyInitialize()
  if not self._initialized then
    self._initialized = true
    self:initialize()
  end
  if self._isModyfied then
    self._isModyfied = false
    self:_setData(self._data)
  end
  if self._headPicTrigger and self._data then
    self._headPicTrigger = false
    self._headPic:setImage(self._data.sPic, {
      defaultImageName = "#common_male_avatar.png"
    })
  end
end
function FriendsWindowItem:onDataSet(isModyfied, data)
  self._isModyfied = isModyfied
end
function FriendsWindowItem:_setData(data)
  self._data = data
  self._inviteButton:hide()
  self._traceButton:hide()
  local headPicURL
  local descString = ""
  if FriendsWindowItem.ITEM_TYPE == FriendsWindowItem.ONLINE_TYPE then
    headPicURL = self._data.sPic
    if tonumber(data.status) == 1 then
      descString = g.lang:getContent("lobby.playerIsIdle")
      self._traceButton:hide()
    elseif tonumber(data.status) == 2 then
      descString = string.gsub(g.lang:getContent("lobby.playerIsInTable"), "{1}", g.util.functions.formatBigInt(data.minBet))
      self._traceButton:show()
    end
  else
    headPicURL = self._data.url
    if self._data.invited then
      descString = g.lang:getContent("lobby.invited")
      self._inviteButton:hide()
    else
      descString = string.gsub(g.lang:getContent("lobby.inviteFriendItemDesc"), "{chips}", g.util.functions.formatBigInt(g.vars.user.rewards.info.facebookInviteSuccReward))
      self._descLabel:setFontSize(16)
      self._inviteButton:show()
    end
  end
  self._userNameLable:setString(g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 24, data.name, 190))
  if data.gender then
    if data.gender == "0" then
      self._headPic:setImage(headPicURL, {
        defaultImageName = "#common_male_avatar.png"
      })
    else
      self._headPic:setImage(headPicURL, {
        defaultImageName = "#common_female_avatar.png"
      })
    end
  else
    self._headPic:setImage(headPicURL, {
      defaultImageName = "#common_male_avatar.png"
    })
  end
  self._descLabel:setString(descString)
end
function FriendsWindowItem:_onTraceClick()
  g.ui.manager.popup:removeAllPopup()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  g.eventCenter:dispatchEvent({
    name = LobbyBeanEvent.EVT_ENTER_TABLE,
    data = self._data
  })
  showSceneLoading(g.lang:getContent("lobby.tableLoading"))
end
function FriendsWindowItem:onFriendDetailClick()
  if FriendsWindowItem.ITEM_TYPE == FriendsWindowItem.ONLINE_TYPE then
    local friendProfileView = ProfileView.new(self._data.uid)
    friendProfileView:show()
  end
end
function FriendsWindowItem:_onInviteClick()
  FriendListView.new(2):show()
end
return FriendsWindowItem

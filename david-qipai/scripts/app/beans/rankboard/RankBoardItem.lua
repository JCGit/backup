local RankBoardItem = class("RankBoardItem", g.ui.ListItem)
local AvatarView = require("app.ui.imageview.AvatarView")
local ProfileView = require("app.beans.profile.view.ProfileView")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
RankBoardItem.TOPGAP = 8
RankBoardItem.WIDTH = 670
RankBoardItem.HEIGHT = 100
RankBoardItem.AVATAR_LENGTH = 74
function RankBoardItem:ctor()
  self:setNodeEventEnabled(true)
  RankBoardItem.super.ctor(self, RankBoardItem.WIDTH, RankBoardItem.HEIGHT)
  self._isInitialized = false
end
function RankBoardItem:initContent()
  local bgSpriteX = RankBoardItem.WIDTH * 0.5
  local bgSpriteY = (RankBoardItem.HEIGHT - RankBoardItem.TOPGAP) * 0.5
  local bgSpriteSize = cc.size(RankBoardItem.WIDTH, RankBoardItem.HEIGHT - RankBoardItem.TOPGAP)
  local bgSprite = display.newScale9Sprite("#common_standard_bluebg_06.png", bgSpriteX, bgSpriteY, bgSpriteSize):addTo(self)
  self._headPic = AvatarView.new()
  self._headPic:pos(RankBoardItem.AVATAR_LENGTH * 0.5 + 30, RankBoardItem.AVATAR_LENGTH * 0.5 + 10)
  self._headPic:scale(RankBoardItem.AVATAR_LENGTH / AvatarView.DEFAULT_HEIGHT)
  self._headPic:addTo(bgSprite)
  self.friendDetailButton = cc.ui.UIPushButton.new({
    normal = "#common_transparent_skin.png",
    pressed = "#common_transparent_skin.png"
  }, {scale9 = true}):setButtonSize(RankBoardItem.AVATAR_LENGTH, RankBoardItem.AVATAR_LENGTH):onButtonClicked(handler(self, self.onAvatarBtnClick)):addTo(self._headPic)
  self._userName = ui.newTTFLabel({
    text = "",
    color = g.font.color.LIGHT,
    size = 44,
    align = ui.TEXT_ALIGN_RIGHT
  }):align(display.LEFT_CENTER, 132, bgSpriteSize.height - 32):addTo(bgSprite)
  self._chipsIcon = display.newSprite("#mainlobby_chips_icon.png"):pos(147, bgSpriteSize.height - 68):addTo(bgSprite)
  self._rankInfoLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.GOLDEN,
    size = 24,
    align = ui.TEXT_ALIGN_RIGHT
  }):align(display.LEFT_CENTER, 132, bgSpriteSize.height - 70):addTo(bgSprite)
  self._trackBtn = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonSize(152, 52):setButtonLabel("normal", ui.newTTFLabel({
    text = g.lang:getContent("friend.tracePlayer"),
    color = ccc3(39, 72, 5),
    size = 22,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabel("disabled", ui.newTTFLabel({
    text = g.lang:getContent("friend.tracePlayer"),
    color = ccc3(59, 59, 59),
    size = 22,
    align = ui.TEXT_ALIGN_CENTER
  })):pos(560, bgSpriteSize.height * 0.5):addTo(bgSprite):onButtonClicked(clickHandler(self, self._onTraceBtnClick))
  self._trackBtn:setTouchSwallowEnabled(false)
  self._rankIcon = display.newSprite("#common_rank_label_bg.png"):pos(38, bgSpriteSize.height - 9):addTo(bgSprite)
  self._rankLabel = ui.newTTFLabelWithOutline({
    text = "",
    color = g.font.color.WHITE,
    size = 20,
    align = ui.TEXT_ALIGN_CENTER,
    outlineColor = display.COLOR_BLACK,
    outlineWidth = 1
  }):align(display.CENTER, 17, 15):addTo(self._rankIcon)
  self._rankStatusIcon = display.newSprite("#common_rank_up_icon.png"):pos(450, bgSpriteSize.height * 0.5):addTo(bgSprite)
end
function RankBoardItem:_setData(data)
  if data.rankIndex and data.rankIndex <= 3 then
    if data.rankIndex == 1 then
      self._rankIcon:setDisplayFrame(display.newSpriteFrame("common_rank_gold_icon.png"))
    elseif data.rankIndex == 2 then
      self._rankIcon:setDisplayFrame(display.newSpriteFrame("common_rank_sliver_icon.png"))
    elseif data.rankIndex == 3 then
      self._rankIcon:setDisplayFrame(display.newSpriteFrame("common_rank_copper_icon.png"))
    end
    self._rankLabel:hide()
  else
    self._rankIcon:setDisplayFrame(display.newSpriteFrame("common_rank_label_bg.png"))
    self._rankLabel:setString(tostring(data.rankIndex))
    self._rankLabel:show()
  end
  if tonumber(data.gender or 0) == 0 then
    self._headPic:setImage(data.sPic, {
      defaultImageName = "#common_male_avatar.png"
    })
  else
    self._headPic:setImage(data.sPic, {
      defaultImageName = "#common_female_avatar.png"
    })
  end
  self._userName:setString(g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 44, data.name, 350))
  if data.rankType and data.rankType == 1 then
    local chipsDescString = string.formatnumberthousands(data.chips or 0)
    self._rankInfoLabel:setString(chipsDescString)
    self._rankInfoLabel:setPositionX(167)
    self._chipsIcon:show()
  elseif data.rankType and data.rankType == 2 then
    local text = string.gsub(g.lang:getContent("rank.winLabel"), "{win}", data.continuewin or "0")
    self._rankInfoLabel:setString(text)
    self._rankInfoLabel:setPositionX(132)
    self._chipsIcon:hide()
  elseif data.rankType and data.rankType == 3 then
    local text = string.gsub(g.lang:getContent("rank.profitLabel"), "{chips}", string.formatnumberthousands(data.weekIncChips or 0))
    self._rankInfoLabel:setString(text)
    self._rankInfoLabel:setPositionX(132)
    self._chipsIcon:hide()
  end
  if data.stat then
    self._rankStatusIcon:setVisible(true)
    if data.stat == 1 then
      self._rankStatusIcon:setDisplayFrame(display.newSpriteFrame("common_rank_up_icon.png"))
    elseif data.stat == -1 then
      self._rankStatusIcon:setDisplayFrame(display.newSpriteFrame("common_rank_down_icon.png"))
    else
      self._rankStatusIcon:setDisplayFrame(display.newSpriteFrame("common_rank_equal_icon.png"))
    end
  else
    self._rankStatusIcon:setVisible(false)
  end
  if tonumber(data.status) == 2 and data.ip and data.port and data.tid then
    self._trackBtn:setButtonEnabled(true)
    local label = self._trackBtn:getButtonLabel()
    label:setColor(ccc3(39, 72, 5))
  else
    local label = self._trackBtn:getButtonLabel()
    label:setColor(ccc3(59, 59, 59))
    self._trackBtn:setButtonEnabled(false)
  end
end
function RankBoardItem:_onTraceBtnClick()
  g.ui.manager.popup:removeAllPopup()
  CCTextureCache:sharedTextureCache():removeUnusedTextures()
  g.eventCenter:dispatchEvent({
    name = LobbyBeanEvent.EVT_ENTER_TABLE,
    data = self._data
  })
  showSceneLoading(g.lang:getContent("lobby.tableLoading"))
end
function RankBoardItem:onDataSet(isModyfied, data)
  self._isModyfied = isModyfied
end
function RankBoardItem:lazyInitialize()
  if not self._isInitialized then
    self:initContent()
    self._isInitialized = true
  end
  if self._isModyfied then
    self:_setData(self._data)
    self._isModyfied = false
  end
end
function RankBoardItem:onAvatarBtnClick()
  local userProfileView = ProfileView.new(self._data.uid)
  userProfileView:show()
end
return RankBoardItem

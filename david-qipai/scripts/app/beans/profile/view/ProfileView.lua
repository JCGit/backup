local UIImageView = require("app.ui.imageview.UIImageView")
local GiftShopView = require("app.beans.giftshop.view.GiftShopView")
local PropItem = require("app.beans.giftshop.view.PropItem")
local GiftListPanel = require("app.beans.giftshop.view.GiftListPanel")
local ProfileGiftListItem = require("app.beans.profile.view.ProfileGiftListItem")
local FriendBeanEvent = require("app.beans.friend.event.FriendBeanEvent")
local DataBeanEvent = require("app.beans.data.event.DataBeanEvent")
local ProfileView = class("ProfileView", g.ui.Panel)
local PROFILE_WIDTH = 750
local PROFILE_HEIGHT = 480
local HEADPIC_PADDING = 12
function ProfileView:ctor(targetUid)
  if targetUid then
    PROFILE_HEIGHT = 410
    self._targetUid = targetUid
  else
    PROFILE_HEIGHT = 480
  end
  ProfileView.super.ctor(self, PROFILE_WIDTH, PROFILE_HEIGHT)
  self:enableBGHightLight()
  self:setBGHightLightOpacity(128)
  self:addCloseButton()
  self:setNodeEventEnabled(true)
  local headPicPosX = -self._width * 0.5 + 58 + HEADPIC_PADDING
  local headPicPosY = self._height * 0.5 - 58 - HEADPIC_PADDING
  self._headPic = g.ui.AvatarView.new():pos(headPicPosX, headPicPosY):hide():addTo(self):scale(80 / g.ui.AvatarView.DEFAULT_HEIGHT)
  local cameraIcon = display.newSprite("#profile_upload_head.png"):pos(headPicPosX + 25, headPicPosY - 26):addTo(self)
  if self._targetUid ~= nil or not g.vars.user.isChangeHeadPicPermited then
    cameraIcon:hide()
  end
  local uploadURL = g.requestParamsFactory:assembleRequestURL("users", "upload")
  if uploadURL and g.vars.user.isChangeHeadPicPermited and self._targetUid == nil then
    self._uploadPicButton = cc.ui.UIPushButton.new("#transparent.png", {scale9 = true}):setButtonSize(108, 108):onButtonClicked(function()
      g.native:getHeadPic(function(success, result)
        logger:debug("g.native:getHeadPic callback ", success, result)
        if success then
          if g.util.functions.isFileExist(result) then
            g.ui.manager.tip:top(g.lang:getContent("language.profile.uploading"))
            network.uploadFile(function(evt)
              if evt.name == "completed" then
                local request = evt.request
                local code = request:getResponseStatusCode()
                local dataString = request:getResponseString()
                local jsonData = json.decode(dataString)
                if jsonData.ret == 0 and jsonData.info.imgURL then
                  do
                    local imgURL = jsonData.info.imgURL
                    local requestData = {}
                    requestData.bPic = imgURL
                    requestData.mPic = imgURL
                    requestData.sPic = imgURL
                    requestData.uid = g.vars.user.uid
                    requestData.loginKey = g.vars.user.loginKey
                    g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "modify", requestData), function(data)
                      local modifyJsonData = json.decode(data)
                      if modifyJsonData.ret == 0 then
                        g.ui.manager.tip:top(g.lang:getContent("profile.uploadSuccess"))
                        g.vars.user.bPic = imgURL
                        g.vars.user.mPic = imgURL
                        g.vars.user.sPic = imgURL
                        self:updateHeadPic(imgURL .. "?_r=" .. tostring(math.random()))
                        g.eventCenter:dispatchEvent({
                          name = DataBeanEvent.EVT_USER_PIC_CHANGE
                        })
                      else
                        g.ui.manager.tip:top(g.lang:getContent("profile.uploadFail"))
                      end
                    end, function()
                      g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
                    end)
                  end
                else
                  g.ui.manager.tip:top(g.lang:getContent("profile.uploadFail"))
                end
                os.remove(result)
              end
            end, uploadURL, {
              fileFieldName = "upload",
              filePath = result,
              contentType = "Image/jpeg",
              extra = {
                {
                  "loginKey",
                  g.vars.user.loginKey
                },
                {
                  "uid",
                  g.vars.user.uid
                },
                {"path", "head"}
              }
            })
          else
            g.ui.manager.tip:top(g.lang:getContent("profile.uploadImageFail"))
          end
        elseif result == "nosdcard" then
          g.ui.manager.tip:top(g.lang:getContent("profile.uploadNoSDCard"))
        else
          g.ui.manager.tip:top(g.lang:getContent("profile.uploadImageFail"))
        end
      end)
    end):pos(headPicPosX, headPicPosY):addTo(self)
  end
  local genderIconPosX = -self._width * 0.5 + HEADPIC_PADDING + 105
  local genderIconPosY = self._height * 0.5 - HEADPIC_PADDING + 2
  self._genderIconBg = display.newScale9Sprite("#common_standard_greybg_03.png", genderIconPosX + 20, genderIconPosY - 28, CCSize(42, 42)):hide():addTo(self)
  self._genderIcon = display.newSprite("#common_male_icon_01.png")
  self._genderIcon:pos(genderIconPosX + 20, genderIconPosY - 26):addTo(self):hide()
  if g.vars.user.isChangeHeadPicPermited then
    self._genderIconBg:setTouchEnabled(true)
    self._genderIconBg:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onGenderIconTouch))
  end
  self._userNameUnderline = display.newScale9Sprite("#common_divider_01.png", 0, 0, CCSize(326, 2))
  self._userNameUnderline:pos(genderIconPosX + 206, genderIconPosY - 45):addTo(self)
  local labelPosX = genderIconPosX + 28
  local labelPosY = genderIconPosY
  self._giftIcon = cc.ui.UIPushButton.new({
    normal = "#gift_icon_up.png",
    pressed = "#gift_icon_down.png"
  }):scale(1):pos(genderIconPosX - 95, labelPosY - 60):hide():addTo(self):onButtonClicked(clickHandler(self, self._onGiftIconClick))
  self._uidLabel = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 20,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(labelPosX + 76 + 240 + 40 + 90, labelPosY - 32):addTo(self)
  self._userNameEdit = ui.newEditBox({
    listener = handler(self, self._onUserNameEdit),
    size = CCSize(380, 40)
  }):align(display.LEFT_CENTER, labelPosX + 14, labelPosY - 30):addTo(self)
  self._userNameEdit:setFont(ui.DEFAULT_TTF_FONT, 26)
  self._userNameEdit:setPlaceholderFont(ui.DEFAULT_TTF_FONT, 26)
  self._userNameEdit:setMaxLength(20)
  self._userNameEdit:setPlaceholderFontColor(ccc3(199, 229, 255))
  self._userNameEdit:setReturnType(kKeyboardReturnTypeDone)
  self._chipsIcon = display.newSprite("#mainlobby_chips_icon.png"):pos(genderIconPosX + 20, genderIconPosY - 86):hide():addTo(self)
  self._chipsLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.GOLDEN,
    size = 26,
    align = ui.TEXT_ALIGN_RIGHT
  })
  self._chipsLabel:setAnchorPoint(ccp(0, 0.5))
  self._chipsLabel:pos(genderIconPosX + 40, genderIconPosY - 86)
  self._chipsLabel:addTo(self)
  self._levelLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.GOLDEN,
    size = 26,
    align = ui.TEXT_ALIGN_CENTER
  })
  self._levelLabel:pos(genderIconPosX + 300, genderIconPosY - 86)
  self._levelLabel:addTo(self)
  self._expProgressBar = g.ui.ProgressBar.new("#common_grey_progress_bg.png", "#common_grey_progress_fill.png", {
    bgWidth = 240,
    bgHeight = 19,
    progressWidth = 12,
    progressHeight = 14
  }):hide():pos(labelPosX + 300, labelPosY - 86):addTo(self)
  self._expLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.GOLDEN,
    size = 16,
    align = ui.TEXT_ALIGN_CENTER
  })
  self._expLabel:hide()
  self._expLabel:pos(genderIconPosX + 445, genderIconPosY - 69)
  self._expLabel:addTo(self)
  self._userNameEditButton = cc.ui.UIPushButton.new({
    normal = "#common_input_icon_up.png",
    pressed = "#common_input_icon_down.png"
  }):pos(labelPosX + 76 + 240 + 10, labelPosY - 28):hide():addTo(self)
  self._userNameEditButton:setTouchSwallowEnabled(false)
  self._isMyFriend = false
  self._addFriendBtn = cc.ui.UIPushButton.new({
    normal = "#common_add_friend_up.png",
    pressed = "#common_add_friend_down.png"
  }):onButtonClicked(clickHandler(self, self._onFriendBtnClick)):pos(labelPosX + 76 + 240 + 10, genderIconPosY - 28):setButtonEnabled(false):scale(0.65):hide():addTo(self)
  display.newScale9Sprite("#common_divider_01.png", 0, 0, CCSize(PROFILE_WIDTH - 20, 2)):pos(0, labelPosY - 116):addTo(self)
  if self._targetUid == nil then
    self._tabGroup = g.ui.TabGroupWithSlider.new({
      background = "#common_standard_greybg_04.png",
      slider = "#common_standard_bluebg_05.png"
    }, g.lang:getContent("profile.tabGroupLabels"), {
      selectedText = {
        color = g.font.color.LIGHT,
        size = 26
      },
      defaltText = {
        color = ccc3(139, 184, 220),
        size = 26
      }
    }, true, true):pos(0, labelPosY - 151):setTabGroupSize(540, 50, -4, -6):addTo(self)
    self._tabGroup:bindTabClickHandler(handler(self, self._onTabPanelChange))
    self._giftShopButton = cc.ui.UIPushButton.new({
      normal = "#mainlobby_mall_normal.png",
      pressed = "#mainlobby_mall_pressed.png"
    }, {scale9 = false}):pos(315, labelPosY - 151):addTo(self):scale(0.8):onButtonClicked(clickHandler(self, self.onGiftShopButtonClick))
  end
  self._tabGroupPanelBg = display.newScale9Sprite("#common_standard_greybg_03.png", 0, 0, CCSize(PROFILE_WIDTH - 20, 260)):pos(0, genderIconPosY - 318):addTo(self)
  self._infoView = display.newNode():hide():addTo(self)
  local labelPosX = -self._width * 0.5 + HEADPIC_PADDING * 2 + 20
  local labelPosY = self._height * 0.5 - HEADPIC_PADDING * 3 - 80
  self.weekProfitLabel = ui.newTTFLabel({
    text = g.lang:getContent("profile.weekProfitLabel"),
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.weekProfitLabel:align(display.LEFT_CENTER, labelPosX, labelPosY - 116)
  self.weekProfitLabel:addTo(self._infoView)
  local whiteChipsIcon = display.newSprite("#mainlobby_white_chips_icon.png"):align(display.LEFT_CENTER, self.weekProfitLabel:getPositionX() + self.weekProfitLabel:getContentSize().width, labelPosY - 116):addTo(self._infoView)
  self.weekProfitText = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.weekProfitText:align(display.LEFT_CENTER, self.weekProfitLabel:getPositionX() + self.weekProfitLabel:getContentSize().width + 35, labelPosY - 118)
  self.weekProfitText:addTo(self._infoView)
  self.weekProfitRankLabel = ui.newTTFLabel({
    text = g.lang:getContent("profile.weekProfitRankLabel"),
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.LEFT_CENTER, 64, labelPosY - 116):addTo(self._infoView)
  self.weekProfitRankText = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.weekProfitRankText:align(display.LEFT_CENTER, self.weekProfitRankLabel:getPositionX() + self.weekProfitRankLabel:getContentSize().width, labelPosY - 116)
  self.weekProfitRankText:addTo(self._infoView)
  self.weekWinLabel = ui.newTTFLabel({
    text = g.lang:getContent("profile.weekWinLabel"),
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.weekWinLabel:align(display.LEFT_CENTER, labelPosX, labelPosY - 156)
  self.weekWinLabel:addTo(self._infoView)
  self.weekWinText = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.weekWinText:align(display.LEFT_CENTER, self.weekWinLabel:getPositionX() + self.weekWinLabel:getContentSize().width, labelPosY - 156)
  self.weekWinText:addTo(self._infoView)
  self.weekWinRankLabel = ui.newTTFLabel({
    text = g.lang:getContent("profile.weekWinRankLabel"),
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.LEFT_CENTER, 64, labelPosY - 156):addTo(self._infoView)
  self.weekWinRankText = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.weekWinRankText:align(display.LEFT_CENTER, self.weekWinRankLabel:getPositionX() + self.weekWinRankLabel:getContentSize().width, labelPosY - 156)
  self.weekWinRankText:addTo(self._infoView)
  self.historicalChipsLabel = ui.newTTFLabel({
    text = g.lang:getContent("profile.historicalChipsLabel"),
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.historicalChipsLabel:align(display.LEFT_CENTER, labelPosX, labelPosY - 196)
  self.historicalChipsLabel:addTo(self._infoView)
  local whiteChipsIcon = display.newSprite("#mainlobby_white_chips_icon.png"):align(display.LEFT_CENTER, self.historicalChipsLabel:getPositionX() + self.historicalChipsLabel:getContentSize().width, labelPosY - 196):addTo(self._infoView)
  self.historicalChipsText = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.historicalChipsText:align(display.LEFT_CENTER, self.historicalChipsLabel:getPositionX() + self.historicalChipsLabel:getContentSize().width + 35, labelPosY - 198)
  self.historicalChipsText:addTo(self._infoView)
  self._rankingLabel = ui.newTTFLabel({
    text = g.lang:getContent("profile.rankingLabel"),
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.LEFT_CENTER, 64, labelPosY - 196):addTo(self._infoView)
  self._rankingText = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.LEFT_CENTER, self._rankingLabel:getPositionX() + self._rankingLabel:getContentSize().width, labelPosY - 196):addTo(self._infoView)
  self.historicalPotLabel = ui.newTTFLabel({
    text = g.lang:getContent("profile.historicalPotLabel"),
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.historicalPotLabel:align(display.LEFT_CENTER, labelPosX, labelPosY - 236)
  self.historicalPotLabel:addTo(self._infoView)
  local whiteChipsIcon = display.newSprite("#mainlobby_white_chips_icon.png"):align(display.LEFT_CENTER, self.historicalPotLabel:getPositionX() + self.historicalPotLabel:getContentSize().width, labelPosY - 236):addTo(self._infoView)
  self.historicalPotText = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self.historicalPotText:align(display.LEFT_CENTER, self.historicalPotLabel:getPositionX() + self.historicalPotLabel:getContentSize().width + 35, labelPosY - 238)
  self.historicalPotText:addTo(self._infoView)
  self._winRateLabel = ui.newTTFLabel({
    text = g.lang:getContent("profile.winRateLabel"),
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self._winRateLabel:align(display.LEFT_CENTER, labelPosX, labelPosY - 276)
  self._winRateLabel:addTo(self._infoView)
  self._winRateText = ui.newTTFLabel({
    text = "",
    color = ccc3(199, 229, 255),
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self._winRateText:align(display.LEFT_CENTER, self._winRateLabel:getPositionX() + self._winRateLabel:getContentSize().width, labelPosY - 276)
  self._winRateText:addTo(self._infoView)
  self._giftView = display.newNode()
  self._giftView:addTo(self)
  self._giftView:hide()
  self._giftList = GiftListPanel.new({
    visibleRect = cc.rect(-(PROFILE_WIDTH - 20) * 0.5, -125, PROFILE_WIDTH - 20, 250),
    scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL),
    direction = g.ui.ScrollPanel.DIRECTION_VERTICAL
  }, ProfileGiftListItem):pos(2, genderIconPosY - 318):addTo(self._giftView):show()
  self._giftList:setNoDataHintText(g.lang:getContent("profile.noGiftTip"))
  self._propView = display.newNode()
  self._propView:addTo(self)
  self._propView:hide()
  self._noPropTip = ui.newTTFLabel({
    text = g.lang:getContent("profile.noPropTip"),
    size = 24,
    color = ccc3(139, 184, 220),
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, genderIconPosY - 318):hide():addTo(self._propView)
  self._magicItem = PropItem.new():pos(-self._tabGroupPanelBg:getContentSize().width * 0.5 + 12 + 96, labelPosY - 314):addTo(self._propView)
  self._infoView:hide()
  self._propView:hide()
  self._giftView:hide()
  if self._targetUid == nil then
    self:addEvent()
    self._tabGroup:tabOn(1)
    self._bgY = genderIconPosY - 318
  else
    self._infoView:pos(0, 70)
    self._tabGroupPanelBg:pos(0, genderIconPosY - 248)
    self._bgY = genderIconPosY - 248
  end
  self:updateData()
end
function ProfileView:addEvent()
  self._giftChangeHandle = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_GIFT_CHANGE, handler(self, self._onGiftChange))
  self._magicChangeHandle = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_MAGIC_CHANGE, handler(self, self._onMagicChange))
  self._chipsChangeHandle = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_MAGIC_CHANGE, handler(self, self._onChipsChange))
  self._giftPreviewHandle = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_GIFT_PREVIEW, handler(self, self._onGiftChange))
  self._buyGiftHandle = g.eventCenter:addEventListener(DataBeanEvent.EVT_USER_BUY_GIFT, handler(self, self._onBuyGift))
end
function ProfileView:removeEvent()
  g.eventCenter:removeEventListener(self._giftChangeHandle)
  g.eventCenter:removeEventListener(self._magicChangeHandle)
  g.eventCenter:removeEventListener(self._giftPreviewHandle)
  g.eventCenter:removeEventListener(self._chipsChangeHandle)
  g.eventCenter:removeEventListener(self._buyGiftHandle)
end
function ProfileView:_onFriendBtnClick(evt)
  if self._isMyFriend then
    self:_onDelFriendClick(evt)
  else
    self:_onAddFriendClick(evt)
  end
end
function ProfileView:loadFriendUidList()
  function successCallback()
    local isMyFriend = g.friendsInfo:isMyFriends(self._data.uid)
    self:setAddFriendStatus(isMyFriend)
  end
  function failCallback()
  end
  g.friendsInfo:refreshData(successCallback, failCallback)
end
function ProfileView:_onAddFriendClick(evt)
  self._addFriendBtn:setButtonEnabled(false)
  function successCallback(jsonData)
    if jsonData and jsonData.ret == 0 then
      self._addFriendBtn:setButtonEnabled(true)
      g.ui.manager.tip:top(g.lang:getContent("friend.addFriendSuccess"))
    else
      self:setAddFriendStatus(false)
      g.ui.manager.tip:top(g.lang:getContent("friend.addFriendFail"))
    end
  end
  function failCallback()
    self:setAddFriendStatus(false)
    g.ui.manager.tip:top(g.lang:getContent("friend.addFriendFail"))
  end
  g.friendsInfo:applyFriend(self._data.uid, successCallback, failCallback)
end
function ProfileView:_onDelFriendClick(evt)
  self._addFriendBtn:setButtonEnabled(false)
  function successCallback(jsonData)
    if jsonData and jsonData.ret == 0 then
      self:setAddFriendStatus(false)
      g.ui.manager.tip:top(g.lang:getContent("friend.deleteFriendSuccess"))
      g.eventCenter:dispatchEvent({
        name = FriendBeanEvent.EVT_DELETE_FRIEND
      })
    else
      self:setAddFriendStatus(true)
      g.ui.manager.tip:top(g.lang:getContent("friend.deleteFriendFail"))
    end
  end
  function failCallback()
    self:setAddFriendStatus(true)
    g.ui.manager.tip:top(g.lang:getContent("friend.deleteFriendFail"))
  end
  g.friendsInfo:delFriend(self._data.uid, successCallback, failCallback)
end
function ProfileView:setAddFriendStatus(isMyFriend)
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
  self._addFriendBtn:show()
end
function ProfileView:onGiftShopButtonClick()
  if self._selectedTab == 3 then
    GiftShopView.new(2):show()
  else
    GiftShopView.new(1):show()
  end
end
function ProfileView:_onTabPanelChange(selectedTab)
  if selectedTab == 1 then
    self._infoView:show()
    self._propView:hide()
    self._giftView:hide()
  elseif selectedTab == 2 then
    self._infoView:hide()
    self._propView:hide()
    self._giftView:show()
    if self._myGiftData == nil then
      g.giftInfo:loadGiftData(handler(self, self._onGiftDataLoaded))
    end
  elseif selectedTab == 3 then
    self._infoView:hide()
    self._propView:show()
    self._giftView:hide()
  end
  self._selectedTab = selectedTab
end
function ProfileView:_onGiftDataLoaded()
  g.giftInfo:loadMyGiftData(handler(self, self._onMyGiftDataLoaded))
end
function ProfileView:_onMyGiftDataLoaded(data)
  self._giftList:setData({}, nil)
  local giftData = g.giftInfo:getGiftData()
  self._myGiftData = json.decode(data)
  if self._myGiftData.ret == 0 and giftData then
    if 0 < #self._myGiftData.info then
      self._giftList:hideNoDataHintView()
      for i = 1, #self._myGiftData.info do
        local gift = g.giftInfo:getGiftById(self._myGiftData.info[i].giftId)
        if gift then
          self._myGiftData.info[i].pic = gift.pic
          self._myGiftData.info[i].name = gift.name
        end
      end
    else
      self._giftList:showNoDataHintView()
    end
  else
    self._giftList:showNoDataHintView()
  end
  local groupData = g.giftInfo:groupGiftData(self._myGiftData.info, 5)
  self._giftList:setData(groupData, nil)
  local giftId = tonumber(g.vars.user.giftId)
  if giftId ~= nil and giftId > 0 then
    self._giftList:selectGiftById(g.vars.user.giftId)
  else
    self._giftList:selectGiftByIndex(1)
  end
  self._giftList:onScrolling()
end
function ProfileView:updateData()
  if self._targetUid == nil then
    self._data = g.vars.user
    self:updateUserInfo()
  end
end
function ProfileView:updateUserInfo()
  self:_hideMiniLoading()
  local winRate = 0
  self._data.win = tonumber(self._data.win)
  self._data.lose = tonumber(self._data.lose)
  self._data.draw = tonumber(self._data.draw)
  self._data.esc = tonumber(self._data.esc)
  if 0 < self._data.win then
    winRate = math.round(self._data.win / (self._data.win + self._data.lose + self._data.draw + self._data.esc) * 1000) / 10
  end
  self._winRateText:setString(winRate .. "%")
  self._uidLabel:setString("ID:" .. self._data.uid)
  self._userNameEdit:setPlaceHolder(g.native:getFixedWidthText(ui.DEFAULT_TTF_FONT, 26, self._data.name, 280))
  if self._targetUid ~= nil or not g.vars.user.isChangeHeadPicPermited then
    self._userNameEdit:setTouchEnabled(false)
  end
  if self._targetUid ~= nil or not g.vars.user.isChangeHeadPicPermited then
    self._userNameEditButton:hide()
  else
    self._userNameEditButton:show()
  end
  self._giftIcon:show()
  if self._data.giftId and 0 < tonumber(self._data.giftId) then
    self:_loadGift()
  elseif self._targetUid ~= nil then
    self._giftIcon:hide()
  end
  self._chipsIcon:show()
  self._chipsLabel:setString(string.formatnumberthousands(self._data.chips))
  self._expProgressBar:show()
  self._expProgressBar:setValue(g.levelInfo:getLevelUpProgress(tonumber(self._data.exp)))
  self._expLabel:show()
  local ratio, progress, target = g.levelInfo:getLevelUpProgress(tonumber(self._data.exp))
  local expLabelText = string.gsub(g.lang:getContent("profile.expProgress"), "{progress}", tostring(progress))
  expLabelText = string.gsub(expLabelText, "{target}", target)
  self._expLabel:setString(expLabelText)
  local text = string.gsub(g.lang:getContent("common.level"), "{level}", g.levelInfo:getLevelByExperience(tonumber(self._data.exp)))
  self._levelLabel:setString(text)
  self._genderIcon:show()
  self._genderIconBg:show()
  self._penddingGender = self._data.gender
  self:updateGender()
  self._headPic:show()
  self:updateHeadPic(self._data.sPic)
  self.weekProfitText:setString(string.formatnumberthousands(self._data.chips - self._data.lastWeekChip))
  self.weekProfitRankText:setString(string.formatnumberthousands(self._data.weekProfitRank))
  self.weekWinText:setString(string.formatnumberthousands(self._data.win - self._data.lastWeekWin))
  self.weekWinRankText:setString(string.formatnumberthousands(self._data.weekWinRank))
  self.historicalChipsText:setString(string.formatnumberthousands(self._data.maxChips))
  self._rankingText:setString(string.formatnumberthousands(self._data.chipsRank))
  self.historicalPotText:setString(string.formatnumberthousands(self._data.maxWin))
  if self._targetUid == nil then
    local quantity = string.gsub(g.lang:getContent("profile.magicLabel"), "{quantity}", self._data.magic)
    self._magicItem:setData({
      name = quantity,
      buttonText = g.lang:getContent("shop.buyLabel"),
      callback = handler(self, self._onBuyPropClick),
      pic = "#magic_item_icon.png"
    })
    self._magicItem:show()
  end
  if self._targetUid ~= nil then
    self._infoView:show()
  end
end
function ProfileView:updateHeadPic(url)
  if string.len(url) > 5 then
    local imgURL = url
    if string.find(imgURL, "facebook") then
      if string.find(imgURL, "?") then
        imgURL = imgURL .. "&width=100&height=100"
      else
        imgURL = imgURL .. "?width=100&height=100"
      end
    end
    self._headPic:setImage(imgURL, {
      defaultImageName = "#common_male_avatar.png"
    })
  end
end
function ProfileView:onShow()
  local uid
  if self._targetUid == nil then
    uid = g.vars.user.uid
  else
    uid = self._targetUid
  end
  self:_showMiniLoading()
  local requestData = {}
  requestData.targetUid = uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "get", requestData), handler(self, function(self, data)
    local jsonData = json.decode(data)
    if jsonData.ret == 0 then
      self._data = jsonData.info
      self._data.uid = uid
      if uid == g.vars.user.uid then
        self._data.magic = tonumber(g.vars.user.magic)
        g.vars.user.win = tonumber(self._data.win)
        g.vars.user.lose = tonumber(self._data.lose)
        g.vars.user.draw = tonumber(self._data.draw)
        g.vars.user.esc = tonumber(self._data.esc)
        g.vars.user.exp = tonumber(self._data.exp)
        g.vars.user.chips = tonumber(self._data.chips)
        g.vars.user.maxChips = tonumber(self._data.maxChips)
        g.vars.user.weekIncChips = tonumber(self._data.weekIncChips)
        g.vars.user.maxWin = tonumber(self._data.maxWin)
        g.vars.user.chipsRank = tonumber(self._data.chipsRank)
      end
      if self.updateUserInfo then
        self:updateUserInfo()
      end
    else
      g.ui.manager.tip:top(g.lang:getContent("profile.getUserFailTip"))
    end
  end), function()
    g.ui.manager.tip:top(g.lang:getContent("profile.getUserFailTip"))
  end)
  if uid ~= g.vars.user.uid then
    self:loadFriendUidList()
  end
end
function ProfileView:updateGender()
  if self._penddingGender == "0" then
    self._genderIcon:setDisplayFrame(display.newSpriteFrame("common_male_icon_01.png"))
    if string.len(self._data.sPic) <= 5 then
      self._headPic:setImage(nil, {
        defaultImageName = "#common_male_avatar.png"
      })
    end
  else
    self._genderIcon:setDisplayFrame(display.newSpriteFrame("common_female_icon_01.png"))
    if string.len(self._data.sPic) <= 5 then
      self._headPic:setImage(nil, {
        defaultImageName = "#common_female_avatar.png"
      })
    end
  end
end
function ProfileView:_onGenderIconTouch(evt)
  if self._targetUid == nil and evt.name == "began" then
    clickSoundImmediately()
    if self._penddingGender == "1" then
      self._penddingGender = "0"
    else
      self._penddingGender = "1"
    end
    self:updateGender()
  end
end
function ProfileView:_onMagicChange(evt)
  local quantity = string.gsub(g.lang:getContent("profile.magicLabel"), "{quantity}", g.vars.user.magic)
  self._magicItem:setName(quantity)
end
function ProfileView:_onChipsChange(evt)
  self._chipsLabel:setString(string.formatnumberthousands(g.vars.user.chips))
end
function ProfileView:_onGiftChange(evt)
  self:_loadGift(evt.data)
end
function ProfileView:_onBuyGift(evt)
  g.giftInfo:loadMyGiftData(handler(self, self._onMyGiftDataLoaded))
end
function ProfileView:_loadGift(giftId)
  if giftId == nil then
    giftId = tonumber(self._data.giftId)
  end
  giftId = tonumber(giftId)
  self._penddingGift = giftId
  if giftId > 0 then
    g.giftInfo:loadGiftData(handler(self, function()
      local gift = g.giftInfo:getGiftById(giftId)
      if gift then
        do
          local imageView = g.ui.UIImageView.new()
          imageView:setImage(gift.pic, {
            success = handler(self, function()
              local texture = imageView.sprite:getTexture()
              self._giftIcon:setButtonImage("normal", texture)
              self._giftIcon:setButtonImage("pressed", texture)
              self._giftIcon:scale(40 / imageView.imageSize.width)
            end)
          })
        end
      end
    end))
  end
end
function ProfileView:_onGiftIconClick(evt)
  self:_openGiftShop()
end
function ProfileView:_openGiftShop()
  if g.vars.user.giftShop == 1 then
    local isInTable = g.vars.table.loginResponse ~= nil
    if isInTable then
      local toUsers = {}
      local selfSeatId = -1
      for i, player in ipairs(g.vars.table.gamblers) do
        if not player:getIsSelf() then
          toUser = {}
          toUser.uid = player.uid
          toUser.seatId = player:getSeatIdForServer()
          table.insert(toUsers, toUser)
        else
          sourceUser = {
            uid = player:getUid(),
            seatId = player:getSeatIdForServer()
          }
          selfSeatId = player:getSeatIdForServer()
        end
      end
      if not sourceUser then
        sourceUser = {
          uid = g.vars.user.uid,
          seatId = -1
        }
      end
      GiftShopView.new(1):show(isInTable, sourceUser, toUsers, selfSeatId)
    else
      GiftShopView.new(1):show(isInTable, {
        uid = g.vars.user.uid,
        seatId = -1
      })
    end
  end
end
function ProfileView:_onUserNameEdit(evt)
  if evt == "began" then
  elseif evt == "changed" then
    local text = self._userNameEdit:getText()
    self._penddingUserName = string.trim(self._userNameEdit:getText())
  elseif evt == "ended" then
  elseif evt == "return" then
  end
end
function ProfileView:_onBuyPropClick(evt)
  GiftShopView.new(2):show()
end
function ProfileView:onPopupRemove(callback)
  if self._targetUid == nil then
    if self._penddingGender and self._penddingGender ~= g.vars.user.gender or self._penddingUserName and self._penddingUserName ~= g.vars.user.name then
      local requestData = {}
      requestData.gender = self._penddingGender
      requestData.name = self._penddingUserName
      requestData.uid = g.vars.user.uid
      g.vars.user.gender = self._penddingGender
      g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "modify", requestData), function(data)
        g.eventCenter:dispatchEvent({
          name = DataBeanEvent.EVT_USER_NAME_CHANGE
        })
        g.eventCenter:dispatchEvent({
          name = DataBeanEvent.EVT_USER_GENDER_CHANGE
        })
      end, function()
      end)
      if self._penddingGender and self._penddingGender ~= g.vars.user.gender then
        g.vars.user.gender = self._penddingGender
        if string.len(g.vars.user.sPic) <= 10 then
        end
      end
      if self._penddingUserName and self._penddingUserName ~= g.vars.user.name then
        g.vars.user.name = self._penddingUserName
      end
    end
    if self._penddingGift ~= nil and self._penddingGift ~= tonumber(g.vars.user.giftId) then
      g.vars.user.giftId = self._penddingGift
      local requestData = {}
      requestData.uid = g.vars.user.uid
      requestData.giftId = self._penddingGift
      local isInTable = g.vars.table.loginResponse ~= nil
      if isInTable then
        requestData.isInTable = tostring(isInTable)
        local selfSeatId = -1
        for i, player in ipairs(g.vars.table.gamblers) do
          if player:getIsSelf() then
            selfSeatId = player:getSeatIdForServer()
          end
        end
        requestData.seatId = selfSeatId
      end
      g.httpRequest:postMethod(g.requestParamsFactory:createRequest("gifts", "apply", requestData), function(data)
        local jsonData = json.decode(data)
        if jsonData.ret == 0 then
          g.ui.manager.tip:top(g.lang:getContent("shop.setGiftSuccessTip"))
          g.eventCenter:dispatchEvent({
            name = DataBeanEvent.EVT_USER_GIFT_CHANGE
          })
        else
          g.ui.manager.tip:top(g.lang:getContent("shop.setGiftFailTip"))
        end
      end, function()
        g.ui.manager.tip:top(g.lang:getContent("shop.setGiftFailTip"))
      end)
    end
  end
  self:removeEvent()
  callback()
end
function ProfileView:_showMiniLoading()
  if not self._miniLoading then
    self._miniLoading = g.ui.MiniLoading.new():pos(0, self._bgY):addTo(self)
  end
end
function ProfileView:_hideMiniLoading()
  if self._miniLoading then
    self._miniLoading:removeFromParent()
    self._miniLoading = nil
  end
end
return ProfileView

local MessageListItem = class("MessageListItem", g.ui.ListItem)
local AvatarView = require("app.ui.imageview.AvatarView")
local LobbyBeanEvent = require("app.beans.lobby.event.LobbyBeanEvent")
MessageListItem.WIDTH = 750
MessageListItem.HEIGHT = 106
MessageListItem.TOPGAP = 8
MessageListItem.AVATAR_LENGTH = 76
MessageListItem.dataChangeHandler = nil
function MessageListItem:ctor()
  self:setNodeEventEnabled(true)
  MessageListItem.super.ctor(self, MessageListItem.WIDTH, MessageListItem.HEIGHT)
  local bgSpriteX = MessageListItem.WIDTH * 0.5
  local bgSpriteY = (MessageListItem.HEIGHT - MessageListItem.TOPGAP) * 0.5
  local bgSpriteSize = cc.size(MessageListItem.WIDTH - MessageListItem.TOPGAP, MessageListItem.HEIGHT - MessageListItem.TOPGAP)
  local bgSprite = display.newScale9Sprite("#common_standard_bluebg_06.png", bgSpriteX, bgSpriteY, bgSpriteSize):addTo(self)
  local posY = self.height * 0.5
  self._headPic = AvatarView.new()
  self._headPic:pos(MessageListItem.AVATAR_LENGTH * 0.5 + 10, bgSpriteY)
  self._headPic:scale(MessageListItem.AVATAR_LENGTH / AvatarView.DEFAULT_HEIGHT)
  self._headPic:addTo(bgSprite)
  self._contentLabel = ui.newTTFLabel({
    text = "",
    size = 22,
    align = ui.TEXT_ALIGN_LEFT,
    dimensions = CCSize(470, 0)
  }):align(display.LEFT_CENTER, 100, MessageListItem.HEIGHT * 0.5):addTo(bgSprite)
  self._messageTimeLabel = ui.newTTFLabel({
    text = "",
    size = 18,
    align = ui.TEXT_ALIGN_RIGHT,
    dimensions = CCSize(470, 0)
  }):align(display.RIGHT_CENTER, MessageListItem.WIDTH - 20, MessageListItem.HEIGHT - 95):addTo(bgSprite)
  self._deleteButton = cc.ui.UIPushButton.new({
    normal = "#message_delete_icon.png"
  }):onButtonClicked(handler(self, self._onDeleteClick)):pos(MessageListItem.WIDTH - 40, posY):addTo(bgSprite)
  self._readIcon = display.newSprite("#message_read_icon.png"):pos(MessageListItem.WIDTH - 80, posY):addTo(bgSprite):hide()
  self._ignoreIcon = display.newSprite("#message_ignore_icon.png"):pos(MessageListItem.WIDTH - 80, posY):addTo(bgSprite):hide()
  self:setTouchEnabled(true)
  self:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onReadClick))
  self:setTouchSwallowEnabled(false)
  self._agreeButtonLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.GREEN,
    size = 24,
    align = ui.TEXT_ALIGN_CENTER
  })
  self._agreeButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png",
    disabled = "#common_btn_disabled.png"
  }, {scale9 = true}):setButtonSize(110, 70):setButtonLabel("normal", self._agreeButtonLabel):pos(MessageListItem.WIDTH - 120, posY):addTo(bgSprite):onButtonClicked(clickHandler(self, self._onAgreeBtnClick)):hide()
  self._agreeButton:setTouchSwallowEnabled(true)
end
function MessageListItem:setData(data)
  self._data = data
  local fullText = data.content or ""
  self._contentLabel:setString(fullText)
  local timeInterval = tonumber(data.createTime or 0)
  if timeInterval > 0 then
    local timeText = os.date("%m/%d %H:%M", timeInterval)
    self._messageTimeLabel:setString(timeText)
  end
  self._headPic:setImage(data.icon, {
    defaultImageName = "#common_male_avatar.png"
  })
  self._agreeButton:hide()
  self._contentLabel:setColor(ccc3(199, 229, 255))
  self._messageTimeLabel:setColor(self._contentLabel:getColor())
  if tonumber(data.type) == 2 or tonumber(data.type) == 5 then
    if tonumber(data.status) == 5 then
      self._agreeButton:show():setButtonEnabled(true)
      self._agreeButtonLabel:setString(g.lang:getContent("message.agree"))
    elseif tonumber(data.status) == 6 then
      self._agreeButton:hide()
      self._readIcon:show()
      self._ignoreIcon:hide()
    else
      self._agreeButton:hide()
      self._readIcon:hide()
      self._ignoreIcon:show()
    end
  elseif tonumber(data.type) == 4 then
    if tonumber(data.status) == 3 then
      self._agreeButton:show():setButtonEnabled(true)
      self._agreeButtonLabel:setString(g.lang:getContent("message.fetchGift"))
    else
      self._agreeButton:hide()
      self._readIcon:show()
      self._ignoreIcon:hide()
    end
  end
end
function MessageListItem:_onReadClick(event)
  g.messageInfo:markReadMsg(self._data.messageId)
  local messageType = tonumber(self._data.type)
  if messageType == 0 or messageType == 1 or messageType == 3 then
    self._contentLabel:setColor(ccc3(199, 229, 255))
    self._messageTimeLabel:setColor(self._contentLabel:getColor())
    self._data.status = 2
  end
end
function MessageListItem:_onDeleteClick()
  function deleteSuccess()
    MessageListItem.dataChangeHandler()
    g.ui.manager.tip:top(g.lang:getContent("message.deleteSuccessPromt"))
  end
  function deleteFail()
    g.ui.manager.tip:top(g.lang:getContent("message.deleteFailPromt"))
  end
  g.messageInfo:deleteMessage(self._data.messageId, deleteSuccess, deleteFail)
end
function MessageListItem:_onAgreeBtnClick()
  local messageType = tonumber(self._data.type)
  if messageType == 4 then
    function receiveSuccess()
      self._data.status = 4
      MessageListItem.dataChangeHandler()
      g.ui.manager.tip:top(g.lang:getContent("message.receiveSuccessPromt"))
    end
    function receiveFail()
      g.ui.manager.tip:top(g.lang:getContent("message.receiveFailPromt"))
    end
    g.messageInfo:receivePackage(self._data.messageId, receiveSuccess, receiveFail)
  elseif messageType == 2 then
    function accepteSuccess(msg)
      self._data.status = 6
      self._data.content = msg
      MessageListItem.dataChangeHandler()
      g.ui.manager.tip:top(g.lang:getContent("message.acceptSuccessPromt"))
      g.eventCenter:dispatchEvent({
        name = LobbyBeanEvent.EVT_LOBBY_REFRESH_FRIENDLIST
      })
    end
    function accepteFail()
      g.ui.manager.tip:top(g.lang:getContent("message.acceptFailPromt"))
    end
    g.messageInfo:acceptFriend(self._data.messageId, accepteSuccess, accepteFail)
  end
end
return MessageListItem

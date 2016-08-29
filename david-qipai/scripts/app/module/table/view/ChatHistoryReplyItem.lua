local ChatHistoryReplyItem = class("ChatHistoryReplyItem", g.ui.ListItem)
ChatHistoryReplyItem.WIDTH = 0
ChatHistoryReplyItem.HEIGHT = 0
ChatHistoryReplyItem.AVATAR_LENGTH = 50
ChatHistoryReplyItem.TEXT_LEFT_GAP = 60
function ChatHistoryReplyItem:ctor()
  ChatHistoryReplyItem.super.ctor(self, ChatHistoryReplyItem.WIDTH, ChatHistoryReplyItem.HEIGHT)
  self._headPic = g.ui.AvatarView.new()
  self._headPic:pos(30, 0):addTo(self):scale(ChatHistoryReplyItem.AVATAR_LENGTH / g.ui.AvatarView.DEFAULT_HEIGHT):setImage(nil, {
    defaultImageName = "#common_male_avatar.png"
  })
  self._userNameLabel = ui.newTTFLabel({
    text = "",
    size = 20,
    color = ccc3(114, 127, 137),
    align = ui.TEXT_ALIGN_LEFT
  }):align(display.CENTER_LEFT, 60, 15):addTo(self)
  self._contentLabel = ui.newTTFLabel({
    text = "",
    size = 20,
    color = ccc3(114, 127, 137),
    dimensions = cc.size(ChatHistoryReplyItem.WIDTH - ChatHistoryReplyItem.TEXT_LEFT_GAP, 0),
    align = ui.TEXT_ALIGN_LEFT
  }):pos(ChatHistoryReplyItem.WIDTH * 0.5 + ChatHistoryReplyItem.TEXT_LEFT_GAP * 0.5, ChatHistoryReplyItem.HEIGHT * 0.5):addTo(self)
end
function ChatHistoryReplyItem:onDataSet(dataChanged, data)
  if dataChanged then
    if data ~= nil and data.message then
      self._userNameLabel:setString(data.userName)
      self._headPic:setImage(data.sPic, {
        defaultImageName = "#common_male_avatar.png"
      })
      self._contentLabel:setString(data.message)
    else
      self._contentLabel:setString("")
    end
    local csize = self._contentLabel:getContentSize()
    self._contentLabel:pos(ChatHistoryReplyItem.WIDTH * 0.5 + ChatHistoryReplyItem.TEXT_LEFT_GAP * 0.5, csize.height * 0.5)
    local cellHeight = self._contentLabel:getContentSize().height + 30
    self:setContentSize(cc.size(ChatHistoryReplyItem.WIDTH, cellHeight))
    self._headPic:pos(30, cellHeight - 30)
    self._userNameLabel:pos(58, cellHeight - 15)
  end
end
return ChatHistoryReplyItem

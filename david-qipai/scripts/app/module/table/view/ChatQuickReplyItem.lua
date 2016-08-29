local ChatQuickReplyItem = class("ChatQuickReplyItem", g.ui.ListItem)
ChatQuickReplyItem.WIDTH = 0
ChatQuickReplyItem.HEIGHT = 0
ChatQuickReplyItem.ON_ITEM_CLICKED_LISTENER = nil
function ChatQuickReplyItem:ctor()
  ChatQuickReplyItem.super.ctor(self, ChatQuickReplyItem.WIDTH, ChatQuickReplyItem.HEIGHT)
  self._contentBtn = cc.ui.UIPushButton.new({
    normal = "#table_chat_quickreply_normal.png",
    pressed = "#table_chat_quickreply_pressed.png"
  }, {scale9 = true}):setButtonLabel(cc.ui.UILabel.new({
    text = "",
    size = 20,
    color = ccc3(255, 255, 255)
  })):setButtonSize(ChatQuickReplyItem.WIDTH, 64):onButtonPressed(function(evt)
    self.btnPressedY_ = evt.y
    self.btnClickCanceled_ = false
  end):onButtonRelease(function(evt)
    if math.abs(evt.y - self.btnPressedY_) > 5 then
      self.btnClickCanceled_ = true
    end
  end):onButtonClicked(clickHandler(self, function(evt)
    if not self.btnClickCanceled_ and ChatQuickReplyItem.ON_ITEM_CLICKED_LISTENER then
      ChatQuickReplyItem.ON_ITEM_CLICKED_LISTENER(self._data)
    end
  end))
  self._contentBtn:setTouchSwallowEnabled(false)
  self._contentBtn:pos(ChatQuickReplyItem.WIDTH * 0.5, 32)
  self._contentBtn:addTo(self)
end
function ChatQuickReplyItem:onDataSet(dataChanged, data)
  if dataChanged then
    self._data = data
    self._contentBtn:setButtonLabelString(data)
  end
end
return ChatQuickReplyItem

local ChatPanel = class("ChatPanel", g.ui.Panel)
local EmojiView = import(".EmojiView")
local ChatQuickReplyItem = import(".ChatQuickReplyItem")
local ChatHistoryReplyItem = import(".ChatHistoryReplyItem")
local ViceJudge = require("app.module.table.model.ViceJudge")
ChatPanel.PANEL_WIDTH = 400
ChatPanel.PANNEL_HEIGHT = 580
function ChatPanel:ctor(messageList)
  self.chatMessages = messageList
  self:setNodeEventEnabled(true)
  ChatPanel.super.ctor(self, ChatPanel.PANEL_WIDTH, ChatPanel.PANNEL_HEIGHT)
  if self._background ~= nil and self._background:getParent() ~= nil then
    self._background:removeFromParent()
  end
  self._background = display.newScale9Sprite("#common_standard_bluebg_01.png", 0, 0, CCSize(ChatPanel.PANEL_WIDTH, ChatPanel.PANNEL_HEIGHT)):addTo(self)
  self._background:setTouchEnabled(true)
  self._background:setTouchSwallowEnabled(true)
  display.newScale9Sprite("#common_standard_greybg_07.png", 0, -30, cc.size(384, 500)):addTo(self)
  self._tabGroup = g.ui.TabGroup.new({
    scale = 1,
    windowWidth = 500,
    windowHeight = 68,
    iconOffsetX = 0,
    tabBtnSize = cc.size(160, 60),
    iconTexture = {
      {
        "#table_chat_message_his_selected.png",
        "#table_chat_message_his_unselect.png"
      },
      {
        "#table_chat_quick_message_selected.png",
        "#table_chat_quick_message_unselect.png"
      },
      {
        "#table_chat_emotion_selected.png",
        "#table_chat_emotion_unselect.png"
      }
    },
    buttonTexts = {
      "",
      "",
      ""
    },
    tabBG = "#transparent.png",
    itemStateTexture = {
      {
        "table_chat_tabItem_select_bg.png",
        "table_chat_tabItem_unselect_bg.png"
      },
      {
        "table_chat_tabItem_select_bg.png",
        "table_chat_tabItem_unselect_bg.png"
      },
      {
        "table_chat_tabItem_select_bg.png",
        "table_chat_tabItem_unselect_bg.png"
      }
    }
  }):pos(-ChatPanel.PANEL_WIDTH * 0.5 + 36, -30):rotation(-90):addTo(self, 10)
  self._tabGroup:bindTabClickHandler(handler(self, self._onTabChange))
  self._inputBG = display.newScale9Sprite("#table_chat_input_bg.png", -ChatPanel.PANEL_WIDTH * 0.5 + 10 + 133, ChatPanel.PANNEL_HEIGHT * 0.5 - 40, cc.size(266, 50)):addTo(self)
  self._inputChatField = ui.newEditBox({
    listener = handler(self, self._onInputFieldEdit),
    size = CCSize(266, 50)
  }):align(display.LEFT_CENTER, 0, 25):addTo(self._inputBG)
  self._inputChatField:setFont(ui.DEFAULT_TTF_FONT, 26)
  self._inputChatField:setPlaceholderFont(ui.DEFAULT_TTF_FONT, 26)
  self._inputChatField:setMaxLength(40)
  self._inputChatField:setPlaceholderFontColor(ccc3(199, 229, 255))
  self._inputChatField:setReturnType(kKeyboardReturnTypeSend)
  self._inputChatLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.LIGHT,
    size = 26,
    align = ui.TEXT_ALIGN_LEFT,
    dimensions = CCSize(266, 50)
  }):align(display.LEFT_CENTER, 0, 25):addTo(self._inputBG):hide()
  self._sendButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png"
  }, {scale9 = true}):pos(ChatPanel.PANEL_WIDTH * 0.5 - 64, ChatPanel.PANNEL_HEIGHT * 0.5 - 40):setButtonSize(110, 50):setButtonLabel(ui.newTTFLabel({
    color = g.font.color.DARK,
    text = g.lang:getContent("table.chatSend"),
    align = ui.TEXT_ALIGN_CENTER
  })):onButtonClicked(clickHandler(self, self._onSendBtnClick)):addTo(self)
  self._tabGroup:tabOn(3)
end
function ChatPanel:showPanel()
  self:show(true, false, true, false)
  return self
end
function ChatPanel:_onTabChange(selectedTab)
  if selectedTab == 1 then
    if self._chatHistoryReplyList == nil then
      local listW, listH = 316, 494
      ChatHistoryReplyItem.WIDTH = listW
      ChatHistoryReplyItem.HEIGHT = 68
      self._chatHistoryReplyList = g.ui.ListPanel.new({
        visibleRect = cc.rect(-0.5 * listW, -0.5 * listH, listW, listH),
        direction = g.ui.ScrollPanel.DIRECTION_VERTICAL,
        scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL)
      }, ChatHistoryReplyItem)
      self._chatHistoryReplyList:addTo(self):pos(30, -30)
    end
    if self._emojiView then
      self._emojiView:hide()
    end
    if self._chatQuickReplyList then
      self._chatQuickReplyList:hide()
    end
    self._chatHistoryReplyList:show()
    self:setMessageList(self.chatMessages)
  elseif selectedTab == 2 then
    if self._chatQuickReplyList == nil then
      local listW, listH = 316, 494
      self._quickReplyItems = g.lang:getContent("table.chatQuickReplyItems")
      ChatQuickReplyItem.WIDTH = listW
      ChatQuickReplyItem.HEIGHT = 68
      ChatQuickReplyItem.ON_ITEM_CLICKED_LISTENER = clickHandler(self, self._onChatQuickReplyItemClicked)
      self._chatQuickReplyList = g.ui.ListPanel.new({
        visibleRect = cc.rect(-0.5 * listW, -0.5 * listH, listW, listH),
        scrollBar = g.ui.factory.scrollBarFactory.create(g.ui.ScrollPanel.DIRECTION_VERTICAL),
        direction = g.ui.ScrollPanel.DIRECTION_VERTICAL
      }, ChatQuickReplyItem)
      self._chatQuickReplyList:addTo(self):pos(30, -30)
      self._chatQuickReplyList:setData(self._quickReplyItems)
    end
    if self._emojiView then
      self._emojiView:hide()
    end
    self._chatQuickReplyList:show()
    if self._chatHistoryReplyList then
      self._chatHistoryReplyList:hide()
    end
  elseif selectedTab == 3 then
    if self._emojiView == nil then
      self._emojiView = EmojiView.new():addTo(self):pos(-130, -180)
    end
    self._emojiView:show()
    if self._chatQuickReplyList then
      self._chatQuickReplyList:hide()
    end
    if self._chatHistoryReplyList then
      self._chatHistoryReplyList:hide()
    end
  end
end
function ChatPanel:onExit()
  self.exitCallback()
end
function ChatPanel:_onInputFieldEdit(evt)
  if evt == "began" then
    self._inputChatLabel:hide()
  elseif evt == "changed" then
    self._inputContentString = self._inputChatField:getText()
    print("change :", self._inputContentString)
    if device.platform == "ios" then
      self._inputChatLabel:show()
      self._inputChatLabel:setString(self._inputContentString)
    end
  elseif evt == "ended" then
    self._inputChatLabel:hide()
    local text = self._inputChatField:getText()
    self:_onChatQuickReplyItemClicked(self._inputContentString)
    print("ended :", self._inputContentString)
  elseif evt == "return" then
    self._inputChatLabel:hide()
    local text = self._inputChatField:getText()
    print("return :", self._inputContentString)
    self:_onChatQuickReplyItemClicked(self._inputContentString)
  end
end
function ChatPanel:_onSendBtnClick()
  self:_onChatQuickReplyItemClicked(self._inputContentString)
end
function ChatPanel:_onChatQuickReplyItemClicked(msg)
  local _trimed
  if msg ~= nil then
    _trimed = string.trim(msg)
  end
  if msg == nil or msg == "" or _trimed == nil or _trimed == "" then
    g.ui.manager.tip:top(g.lang:getContent("table.sendEmptyMessage"))
  else
    ViceJudge:asyncRequestSendChat(msg)
    scheduler.performWithDelayGlobal(function()
      g.ui.manager.popup:removePopup(self)
    end, 0.1)
  end
end
function ChatPanel:setMessageList(list)
  if self._chatHistoryReplyList ~= nil then
    self._chatHistoryReplyList:setData(list)
    self._chatHistoryReplyList:update()
    self._chatHistoryReplyList:scrollTo(9999999)
  end
end
function ChatPanel:playEmojiAnimation(targetView, emojiType, emojiIndex)
  EmojiView:playEmojiAnimation(targetView, emojiType, emojiIndex)
end
return ChatPanel

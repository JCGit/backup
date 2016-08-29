local Panel = import(".Panel")
local DialogConst = import(".DialogConst")
local DialogType = import(".DialogType")
local Dialog = class("Dialog", Panel)
function Dialog:ctor(params)
  if params ~= nil then
    self._contentText = params.contentText
    self._dialogWidth = params.dialogWidth or DialogConst.DEFAULT_WIDTH
    self._callback = params.callback
    self._cancelButtonText = params.cancelButtonText or g.lang:getContent("common.cancel")
    self._confirmButtonText = params.confirmButtonText or g.lang:getContent("common.confirm")
    self._shareButtonText = params.shareBtnText or g.lang:getContent("common.share")
    self._titleText = params.titleText or g.lang:getContent("common.notice")
    self._hasCloseButton = params.hasCloseButton
    local contentLabel = ui.newTTFLabel({
      text = self._contentText,
      color = g.font.color.LIGHT,
      size = 26,
      align = ui.TEXT_ALIGN_CENTER,
      dimensions = cc.size(self._dialogWidth - 32, 0)
    }):pos(0, (DialogConst.PADDING + DialogConst.BUTTON_HEIGHT - DialogConst.TOP_HEIGHT) * 0.5)
    local dialogHeight = contentLabel:getContentSize().height + DialogConst.PADDING * 3 + DialogConst.BUTTON_HEIGHT + DialogConst.TOP_HEIGHT
    if dialogHeight < DialogConst.DEFAULT_HEIGHT then
      dialogHeight = DialogConst.DEFAULT_HEIGHT
    end
    Dialog.super.ctor(self, self._dialogWidth, dialogHeight)
    if self._hasCloseButton then
      self:addCloseButton()
    end
    display.newScale9Sprite("#common_standard_greybg_03.png", 0, 0, CCSize(self._width - DialogConst.PADDING * 0.5, self._height - DialogConst.PADDING * 0.25 - DialogConst.TOP_HEIGHT)):pos(0, -30):addTo(self)
    ui.newTTFLabel({
      text = self._titleText,
      color = g.font.color.LIGHT,
      size = 30,
      align = ui.TEXT_ALIGN_CENTER
    }):pos(0, self._height * 0.5 - DialogConst.TOP_HEIGHT * 0.5):addTo(self)
    contentLabel:addTo(self)
    local buttonWidth = 0
    local dialogHeight = contentLabel:getContentSize().height + DialogConst.PADDING * 3 + DialogConst.BUTTON_HEIGHT + DialogConst.TOP_HEIGHT
    if params.dialogType == DialogType.NORMAL then
      buttonWidth = (self._dialogWidth - 3 * DialogConst.PADDING) * 0.5
      self:_createConfirmButton(buttonWidth):pos((DialogConst.PADDING + buttonWidth) * 0.5, -dialogHeight * 0.5 + DialogConst.VPADDING)
      self:_createCancelButton(buttonWidth):pos(-(DialogConst.PADDING + buttonWidth) * 0.5, -dialogHeight * 0.5 + DialogConst.VPADDING)
    elseif params.dialogType == DialogType.ALERT then
      buttonWidth = 280
      self:_createConfirmButton(buttonWidth):pos(0, -dialogHeight * 0.5 + DialogConst.VPADDING)
    elseif params.dialogType == DialogType.SHARE then
      buttonWidth = (self._dialogWidth - 3 * DialogConst.PADDING) * 0.5
      self:_createShareButton(buttonWidth):pos((DialogConst.PADDING + buttonWidth) * 0.5, -dialogHeight * 0.5 + DialogConst.VPADDING)
      self:_createCancelButton(buttonWidth):pos(-(DialogConst.PADDING + buttonWidth) * 0.5, -dialogHeight * 0.5 + DialogConst.VPADDING)
    elseif params.dialogType == DialogType.FORCE_SHARE then
      buttonWidth = 280
      self:_createShareButton(buttonWidth):pos(0, -dialogHeight * 0.5 + DialogConst.VPADDING)
    end
  end
end
function Dialog:_createConfirmButton(buttonWidth)
  self._confirmButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png"
  }, {scale9 = true}):addTo(self):onButtonClicked(clickHandler(self, self._onButtonClick)):setButtonLabel("normal", ui.newTTFLabel({
    text = self._confirmButtonText,
    color = g.font.color.LIGHT,
    size = 30,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabel("pressed", ui.newTTFLabel({
    text = self._confirmButtonText,
    color = g.font.color.GREY,
    size = 30,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonSize(buttonWidth, DialogConst.BUTTON_HEIGHT)
  return self._confirmButton
end
function Dialog:_createShareButton(buttonWidth)
  self._shareButton = cc.ui.UIPushButton.new({
    normal = "#common_green_btn_up.png",
    pressed = "#common_green_btn_down.png"
  }, {scale9 = true}):addTo(self):onButtonClicked(clickHandler(self, self._onButtonClick)):setButtonLabel("normal", ui.newTTFLabel({
    text = self._shareButtonText,
    color = g.font.color.LIGHT,
    size = 30,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabel("pressed", ui.newTTFLabel({
    text = self._shareButtonText,
    color = g.font.color.GREY,
    size = 30,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonSize(buttonWidth, DialogConst.BUTTON_HEIGHT)
  return self._shareButton
end
function Dialog:_createCancelButton(buttonWidth)
  self._cancelButton = cc.ui.UIPushButton.new({
    normal = "#common_dark_blue_btn_up.png",
    pressed = "#common_dark_blue_btn_down.png"
  }, {scale9 = true}):addTo(self):onButtonClicked(clickHandler(self, self._onButtonClick)):setButtonLabel("normal", ui.newTTFLabel({
    text = self._cancelButtonText,
    color = g.font.color.LIGHT,
    size = 30,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonLabel("pressed", ui.newTTFLabel({
    text = self._cancelButtonText,
    color = g.font.color.GREY,
    size = 30,
    align = ui.TEXT_ALIGN_CENTER
  })):setButtonSize(buttonWidth, DialogConst.BUTTON_HEIGHT)
  return self._cancelButton
end
function Dialog:_onButtonClick(event)
  if self._callback then
    if event.target == self._confirmButton then
      self._callback(DialogConst.CONFIRM_BUTTON_CLICK)
    elseif event.target == self._cancelButton then
      self._callback(DialogConst.CANCEL_BUTTON_CLICK)
    elseif event.target == self._shareButton then
      self._callback(DialogConst.SHARE_BUTTON_CLICK)
    end
  end
  self._callback = nil
  self:hide()
end
function Dialog:onRemovePopup(removeFunc)
  if self._callback then
    self._callback(DialogConst.CLOSE_BTN_CLICK)
  end
  removeFunc()
end
function Dialog:onClose()
  if self._callback then
    self._callback(DialogConst.CLOSE_BTN_CLICK)
  end
  self._callback = nil
  self:hide()
end
return Dialog

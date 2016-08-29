local TableItemView = class("TableItemView", function()
  return display.newNode()
end)
TableItemView.CHIPS = {
  "#common_blue_chips_145.png",
  "#common_green_chips_145.png",
  "#common_black_chips_145.png",
  "#common_orange_chips_145.png",
  "#common_purple_chips_145.png",
  "#common_red_chips_145.png"
}
function TableItemView:ctor(index)
  local textColor = g.font.color.LIGHT
  self._tableTypeIcon = display.newSprite(TableItemView.CHIPS[index]):addTo(self)
  self._tableTypeIcon:setTouchEnabled(true)
  self._tableTypeIcon:addNodeEventListener(cc.NODE_TOUCH_EVENT, handler(self, self._onTouch))
  self._minBetLabel = ui.newTTFLabel({
    text = "",
    color = textColor,
    size = 36,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, 20):addTo(self)
  ui.newTTFLabel({
    text = g.lang:getContent("lobby.minBet"),
    color = textColor,
    size = 18,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, -6):addTo(self)
  self._tableIconRibbon = display.newSprite("#mainlobby_table_type_ribbon_0" .. index .. ".png"):pos(0, -50):addTo(self):show()
  self._playerCountIcon = display.newSprite("#mainlobby_table_count_icon.png"):align(display.LEFT_CENTER, -20, -34):addTo(self):hide()
  self._playerCountLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.LIGHT,
    size = 18,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.LEFT_CENTER, 0, -34):addTo(self):hide()
  self:setPlayerCount(0)
  local bgPosY = -(self._tableTypeIcon:getContentSize().height * 0.5 + 16)
  display.newScale9Sprite("#common_standard_greybg_06.png", 0, bgPosY, cc.size(148, 32)):addTo(self)
  self._maxBuyInLabel = ui.newTTFLabel({
    text = "",
    color = g.font.color.LIGHT,
    size = 18,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, bgPosY):addTo(self)
end
function TableItemView:_onTouch(evt)
  self._touchInSprite = self._tableTypeIcon:getCascadeBoundingBox():containsPoint(CCPoint(evt.x, evt.y))
  if evt.name == "began" then
    self:scaleTo(0.05, 0.9)
    self._clickCancel = false
    return true
  elseif evt.name == "moved" then
    if not self._touchInSprite and not self._clickCancel then
      self:scaleTo(0.05, 1)
      self._clickCancel = true
    end
  elseif (evt.name == "ended" or name == "cancelled") and not self._clickCancel then
    clickSoundImmediately()
    self:scaleTo(0.05, 1)
    self:onClickTableIcon()
  end
end
function TableItemView:bindData(tableInfo)
  self._tableInfo = tableInfo
  self:setPlayerCount(tonumber(tableInfo.online or 0))
  self._minBetLabel:setString(g.util.functions.formatBigInt(tonumber(tableInfo.minBet)))
  local maxBuyString = string.gsub(g.lang:getContent("lobby.tableMaxBuyIn"), "{1}", g.util.functions.formatBigInt(tonumber(tableInfo.maxBuyin)))
  self._maxBuyInLabel:setString(maxBuyString)
end
function TableItemView:setPlayerCount(count)
  if count >= 0 then
    self._playerCountLabel:setString(string.formatnumberthousands(count))
    local iconWidth = self._playerCountIcon:getContentSize().width
    local labelWidth = self._playerCountLabel:getContentSize().width
    self._playerCountIcon:show():setPositionX(-(iconWidth + labelWidth + 6) * 0.5)
    self._playerCountLabel:show():setPositionX(self._playerCountIcon:getPositionX() + iconWidth + 6)
  end
end
function TableItemView:onClickTableIcon()
  local tip
  if g.vars.user.tableLimit then
    local tableLimit = g.vars.user.tableLimit
    if tonumber(self._tableInfo.classId) <= 1012 then
      if g.vars.user.chips >= tableLimit.primaryLimit then
        tip = string.gsub(g.lang:getContent("lobby.exceedChipsLimit"), "{chips}", g.util.functions.formatBigInt(tableLimit.primaryLimit))
      end
    elseif tonumber(self._tableInfo.classId) <= 1024 then
      if g.vars.user.chips >= tableLimit.mediumLimit then
        tip = string.gsub(g.lang:getContent("lobby.exceedChipsLimit"), "{chips}", g.util.functions.formatBigInt(tableLimit.mediumLimit))
      elseif g.vars.user.chips < tableLimit.mediumRequire then
        tip = string.gsub(g.lang:getContent("lobby.bellowChipsLimit"), "{chips}", g.util.functions.formatBigInt(tableLimit.mediumRequire))
      end
    elseif g.vars.user.chips < tableLimit.seniorRequire then
      tip = string.gsub(g.lang:getContent("lobby.bellowChipsLimit"), "{chips}", g.util.functions.formatBigInt(tableLimit.seniorRequire))
    end
  end
  if tip then
    g.ui.Dialog.new({
      contentText = tip,
      confirmButtonText = g.lang:getContent("lobby.playnow"),
      dialogType = g.ui.DialogType.ALERT,
      hasCloseButton = true,
      callback = handler(self, function(self, sender)
        if sender == g.ui.DialogConst.CONFIRM_BUTTON_CLICK then
          g.vars.table.isPlayNow = true
          g.tablesInfo:getPlayTableWithTid(nil, nil, nil)
        end
      end)
    }):show(true, true, true, true)
    return
  end
  local successCallback = function(dispatchedTableInfo)
  end
  local failCallback = function()
  end
  g.tablesInfo:getPlayTableWithClassId(self._tableInfo.classId, successCallback, failCallback)
end
return TableItemView

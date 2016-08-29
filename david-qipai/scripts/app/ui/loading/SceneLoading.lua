local SceneLoading = class("SceneLoading", function()
  return display.newNode()
end)
function SceneLoading:ctor(tip)
  self:setTouchEnabled(true)
  local transparentCover = display.newSprite("#common_transparent_skin.png"):addTo(self)
  transparentCover:setScaleX(display.width / 4)
  transparentCover:setScaleY(display.height / 4)
  local background = display.newScale9Sprite("#scene_swich_loading_bg.png", 0, 0, cc.size(212, 212)):addTo(self)
  background:setScaleX(display.width / background:getContentSize().width)
  local loadingChip = display.newSprite("#common_green_chips_90.png"):addTo(self):pos(0, 20)
  self._loadingChipSpade = display.newSprite("#common_chips_spade_90.png"):pos(loadingChip:getContentSize().width * 0.5, loadingChip:getContentSize().height * 0.5 + 3):addTo(loadingChip)
  self._loadingChipSpade:runAction(CCRepeatForever:create(transition.sequence({
    CCScaleTo:create(0.75, -1, 1),
    CCScaleTo:create(0.75, 1, 1)
  })))
  ui.newTTFLabel({
    text = tip,
    color = g.font.color.LIGHT,
    size = 28,
    align = ui.TEXT_ALIGN_CENTER
  }):pos(0, -55):addTo(self)
  self:setCascadeOpacityEnabled(true)
end
function SceneLoading:reset()
  if self._loadingChipSpade then
    self._loadingChipSpade:stopAllActions()
    self._loadingChipSpade:setScaleX(1)
  end
end
function SceneLoading:show()
  self:opacity(0)
  transition.fadeTo(self, {opacity = 255, time = 0.2})
  g.ui.manager.popup:addPopup(self, false, true, false, false, false)
end
function SceneLoading:hide()
  g.ui.manager.popup:removePopup(self)
end
return SceneLoading

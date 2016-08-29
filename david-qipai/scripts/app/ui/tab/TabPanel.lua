local Panel = require("app.ui.window.Panel")
local TabPanel = class("TabPanel", function()
  return display.newNode()
end)
TabPanel.WIDTH = 500
TabPanel.HEIGHT = 480
TabPanel.PAGE_WIDTH = TabPanel.WIDTH - 24
TabPanel.PAGE_HEIGHT = TabPanel.HEIGHT - 80
function TabPanel:ctor(tabTitleLeft, tabTitleRight)
  self._background = display.newScale9Sprite("#room_chat_panel_background.png", 0, 0, CCSize(TabPanel.WIDTH, TabPanel.HEIGHT))
  self._background:setCapInsets(CCRect(8, 8, 24, 26))
  self._background:addTo(self)
  self:setTouchEnabled(true)
  self:setTouchSwallowEnabled(true)
  self._tabs = cc.ui.UICheckBoxButtonGroup.new(display.LEFT_TO_RIGHT):addButton(cc.ui.UICheckBoxButton.new({
    on = "#room_chat_tab_btn_selected.png",
    off = "#transparent.png"
  }, {scale9 = true}):setButtonLabel(cc.ui.UILabel.new({
    text = tabTitleLeft,
    size = 28,
    color = ccc3(183, 200, 212)
  })):setButtonLabelAlignment(display.CENTER):onButtonClicked(clickSound()):setButtonSize(236, 60)):addButton(cc.ui.UICheckBoxButton.new({
    on = "#room_chat_tab_btn_selected.png",
    off = "#transparent.png"
  }, {scale9 = true}):setButtonLabel(cc.ui.UILabel.new({
    text = tabTitleRight,
    size = 28,
    color = ccc3(183, 200, 212)
  })):setButtonLabelAlignment(display.CENTER):onButtonClicked(clickSound()):setButtonSize(236, 60)):pos(-TabPanel.WIDTH * 0.5 + 14, TabPanel.HEIGHT * 0.5 - 8 - 60):onButtonSelectChanged(function(evt)
    self:_onTabSelect(evt.selected)
  end):setButtonsLayoutMargin(0, 0, 0, 0):addTo(self)
  self._tabs:getButtonAtIndex(1):setButtonSelected(true)
  self.container_ = display.newNode():pos(0, -30):addTo(self)
  self:pos(-TabPanel.WIDTH * 0.5, TabPanel.HEIGHT * 0.5 + 80 + 8)
end
function TabPanel:showPanel()
  local isCover = true
  local isCenter = false
  local closeWhenClickCover = true
  local useShowAnimation = false
  g.ui.manager.popup:addPopup(self, isCover ~= false, isCenter ~= false, closeWhenClickCover ~= false, useShowAnimation ~= false)
end
function TabPanel:hidePanel()
  self:hide()
end
function TabPanel:onRemovePopup(removeFunc)
  self:stopAllActions()
  transition.moveTo(self, {
    time = 0.3,
    x = -TabPanel.WIDTH * 0.5,
    easing = "OUT",
    onComplete = function()
      removeFunc()
    end
  })
end
function TabPanel:onShowPopup()
  self:stopAllActions()
  transition.moveTo(self, {
    time = 0.3,
    x = TabPanel.WIDTH * 0.5 + 8,
    easing = "OUT",
    onComplete = function()
      if self.onShow then
        self:onShow()
      end
      local cbox = self:getCascadeBoundingBox()
      cbox:setRect(cbox.origin.x, cbox.origin.y, TabPanel.WIDTH, TabPanel.HEIGHT)
      self:setCascadeBoundingBox(cbox)
    end
  })
end
function TabPanel:setPage(index, page)
  if index == 1 then
    if self._page1 then
      self._page1:removeFromParent()
    end
    self._page1 = page:pos(0, -30):addTo(self):hide()
  elseif index == 2 then
    if self._page2 then
      self._page2:removeFromParent()
    end
    self._page2 = page:pos(0, -30):addTo(self):hide()
  end
  self:selectPage(self.selectedIndex_)
end
function TabPanel:selectPage(index)
  if self._page1 and self._page2 then
    if self.selectedIndex_ ~= index then
      if index == 2 then
        self._page1:hide()
        self._page2:show()
        self.selectedIndex_ = 2
      elseif index == 1 then
        self._page1:show()
        self._page2:hide()
        self.selectedIndex_ = 1
      end
    end
  elseif self._page1 then
    self._page1:show()
    self.selectedIndex_ = 1
  elseif self._page2 then
    self._page2:show()
    self.selectedIndex_ = 2
  end
end
function TabPanel:_onTabSelect(index)
  print("selecte ", index)
  self:selectPage(index)
end
return TabPanel

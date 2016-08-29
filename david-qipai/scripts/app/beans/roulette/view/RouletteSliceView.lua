local RouletteSliceView = class("RouletteSliceView", function()
  return display.newNode()
end)
RouletteSliceView.WIDTH = 140
RouletteSliceView.HEIGHT = 175
local textColor = ccc3(255, 254, 236)
local textSize = 18
function RouletteSliceView:ctor()
  self:setNodeEventEnabled(true)
  self:initialize()
end
function RouletteSliceView:initialize()
  self._content = display.newNode():addTo(self)
  self._content:setAnchorPoint(ccp(0.5, 0))
  local descLabelTop = 22
  local descLabelLeft = 70
  self.descLabel = ui.newTTFLabel({
    text = "",
    size = textSize,
    color = textColor,
    align = ui.TEXT_ALIGN_CENTER
  }):addTo(self._content)
  local descLabelSize = self.descLabel:getContentSize()
  self.descLabel:pos(RouletteSliceView.WIDTH / 2 - descLabelLeft, RouletteSliceView.HEIGHT - descLabelTop - descLabelSize.height / 2)
end
function RouletteSliceView:setDescText(text)
  self.descLabel:setString(text)
end
function RouletteSliceView:setImageUrl(url)
  local fileName = url
  local iconMarginBottom = 32
  local iconMarginLeft = -70
  self.icon = display.newSprite(fileName):scale(0.75):addTo(self._content)
  self.icon:pos(RouletteSliceView.WIDTH / 2 + iconMarginLeft, RouletteSliceView.HEIGHT / 2 + iconMarginBottom)
end
function RouletteSliceView:onCleanup()
end
return RouletteSliceView

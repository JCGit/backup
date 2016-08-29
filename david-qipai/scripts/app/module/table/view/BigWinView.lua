local BigWinView = class("BigWinView", function()
  return display.newNode()
end)
local StarView = require("app.beans.levelup.view.StarView")
function BigWinView:ctor()
  self._content = display.newNode():addTo(self):pos(0, 50)
  self._content:setCascadeOpacityEnabled(true)
  local flashLeft = display.newSprite("#common_effect_bg_light.png"):pos(0, -70):scale(2):addTo(self._content)
  local texture = "pd_table_texture.png"
  if g.vars.isWebP then
    texture = "pd_table_texture.webp"
  end
  self._bgBatchNode = display.newBatchNode(texture):addTo(self._content)
  self._bgBatchNode:setCascadeOpacityEnabled(true)
  self._stars = {}
  for i = 1, 30 do
    local xRatio
    if i <= 15 then
      xRatio = math.random()
    else
      xRatio = math.random() - 1
    end
    local yRatio = math.random() * 10 + 4
    self._stars[i] = display.newSprite("#table_pok_star.png"):pos(0, 0):addTo(self._bgBatchNode)
    self._stars[i].xSpeed = xRatio * 10
    self._stars[i].ySpeed = yRatio
  end
  self:scheduleUpdate(handler(self, self._onEnterFrame))
  self._deltaYSpeed = -0.4
  self._ySpeed = 0
  self._starOpacity = 255
  display.newSprite("#table_win_big_cup.png"):addTo(self._content):setCascadeOpacityEnabled(true)
  display.newSprite("#table_win_big_chips.png"):addTo(self._content):pos(-30, -50):setCascadeOpacityEnabled(true)
  display.newSprite("#table_win_big_text.png"):addTo(self._content):pos(0, -80):setCascadeOpacityEnabled(true)
  self.star1 = StarView.new("#common_star_1.png"):addTo(self._content):pos(-140, 30):play(math.random() / 2, 1, 2, 0.3)
  self.star1:setCascadeOpacityEnabled(true)
  self.star2 = StarView.new("#common_star_1.png"):addTo(self._content):pos(-90, 20):play(math.random() / 2, 0.5, 1, 0.3)
  self.star2:setCascadeOpacityEnabled(true)
  self.star3 = StarView.new("#common_star_1.png"):addTo(self._content):pos(75, 50):play(math.random() / 2, 1, 2, 0.3)
  self.star3:setCascadeOpacityEnabled(true)
  self.star4 = StarView.new("#common_star_1.png"):addTo(self._content):pos(35, -10):play(math.random() / 2, 0.8, 1.6, 0.3)
  self.star4:setCascadeOpacityEnabled(true)
  self:playWinAnimation()
  self.star5 = StarView.new("#common_star_2.png"):addTo(self._content):pos(-140, -50):play(math.random() / 2, 0.8, 1.6, 0.3)
  self.star4:setCascadeOpacityEnabled(true)
  self:playWinAnimation()
  self.star6 = StarView.new("#common_star_1.png"):addTo(self._content):pos(-40, -60):play(math.random() / 2, 0.8, 1.6, 0.3)
  self.star4:setCascadeOpacityEnabled(true)
  self:playWinAnimation()
  self.star7 = StarView.new("#common_star_2.png"):addTo(self._content):pos(0, -30):play(math.random() / 2, 0.8, 1.6, 0.3)
  self.star4:setCascadeOpacityEnabled(true)
  self:playWinAnimation()
  self.star8 = StarView.new("#common_star_3.png"):addTo(self._content):pos(185, -40):play(math.random() / 2, 0.8, 1.6, 0.3)
  self.star4:setCascadeOpacityEnabled(true)
  self:playWinAnimation()
  self.star9 = StarView.new("#common_star_2.png"):addTo(self._content):pos(-50, -30):play(math.random() / 2, 0.8, 1.6, 0.3)
  self.star4:setCascadeOpacityEnabled(true)
  self:playWinAnimation()
  self.star10 = StarView.new("#common_star_1.png"):addTo(self._content):pos(-205, -105):play(math.random() / 2, 0.6, 1.2, 0.3)
  self.star4:setCascadeOpacityEnabled(true)
  self:playWinAnimation()
end
function BigWinView:playWinAnimation()
  self._content:scale(0.5)
  self._content:scaleTo(0.3, 1)
end
function BigWinView:_onEnterFrame(evt)
  for i = 1, #self._stars do
    local x, y = self._stars[i]:getPosition()
    self._stars[i]:pos(x + self._stars[i].xSpeed, y + self._ySpeed + self._stars[i].ySpeed)
    local rotate = self._stars[i]:getRotation()
    self._stars[i]:rotation(rotate + 12)
    self._stars[i]:opacity(self._starOpacity)
  end
  self._ySpeed = self._ySpeed + self._deltaYSpeed
  self._starOpacity = self._starOpacity - 3
end
function BigWinView:_onNodeEvent(evt)
  if event.name == "exit" then
    self:stop()
  end
end
function BigWinView:stop()
  self.star1:stop()
  self.star2:stop()
  self.star3:stop()
  self.star4:stop()
  self.star5:stop()
  self.star6:stop()
  self.star7:stop()
  self.star8:stop()
  self.star9:stop()
  self.star10:stop()
  self:unscheduleUpdate()
end
function BigWinView:onExit()
  print("BigWinView:onExit()")
  self:stop()
end
return BigWinView

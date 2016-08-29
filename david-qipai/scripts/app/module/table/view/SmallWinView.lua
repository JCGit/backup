local SmallWinView = class("SmallWinView", function()
  return display.newNode()
end)
function SmallWinView:ctor()
  self.flashPoints = {}
  for i = 1, 3 do
    local flashPoint = display.newSprite("#table_win_flash_point.png"):addTo(self):pos(math.random(0, 80), 0)
    table.insert(self.flashPoints, flashPoint)
  end
  self.star1 = display.newSprite("#table_win_big_star2.png"):addTo(self):pos(-30, 0):scale(0.5)
  self.star3 = display.newSprite("#table_win_big_star2.png"):addTo(self):pos(15, 0)
  self._seatWinText = display.newSprite("#table_win_small_text.png", 0, 0):addTo(self)
  self.star1:runAction(CCRepeatForever:create(cc.Sequence:createWithTwoActions(cc.RotateBy:create(2, math.random(270, 360)), cc.RotateBy:create(2, math.random(270, 360)))))
  self.star3:runAction(CCRepeatForever:create(cc.Sequence:createWithTwoActions(cc.RotateBy:create(2, math.random(270, 360)), cc.RotateBy:create(2, math.random(270, 360)))))
  self.star3:runAction(CCRepeatForever:create(cc.Sequence:createWithTwoActions(cc.FadeTo:create(0.5 + math.random(0.1, 0.3), 0), cc.FadeTo:create(0.5 + math.random(0.1, 0.3), 255))))
  self.star1:runAction(CCRepeatForever:create(cc.Sequence:createWithTwoActions(cc.FadeTo:create(0.5 + math.random(0.1, 0.3), 0), cc.FadeTo:create(0.5 + math.random(0.1, 0.3), 255))))
  for i, flashPoint in ipairs(self.flashPoints) do
    do
      local posX = -30 + math.random(0, 80)
      local posY = 20 + math.random(0, 30)
      local randomDelayTime = math.random(0, 0.8)
      local randomScale = math.random(0.3, 0.5)
      flashPoint:scale(randomScale)
      local moveTime = 2
      local initAction = CCCallFunc:create(function()
        flashPoint:setOpacity(255)
        flashPoint:pos(posX, posY)
      end)
      flashPoint:runAction(CCRepeatForever:create(transition.sequence({
        initAction,
        CCDelayTime:create(randomDelayTime),
        cc.MoveTo:create(moveTime, ccp(posX, math.random(0, 10) - 40))
      })))
      flashPoint:runAction(CCRepeatForever:create(transition.sequence({
        cc.FadeTo:create(moveTime + randomDelayTime, 0),
        initAction
      })))
    end
  end
end
return SmallWinView

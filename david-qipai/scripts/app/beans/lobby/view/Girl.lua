local POKER_GIRL_ZORDER = 2
local Girl = class("Girl", function()
  return display.newNode()
end)
function Girl:ctor()
  self:showWelcomGirl()
end
function Girl:showWelcomGirl()
  loadTexture("pd_girl_texture.plist", "pd_girl_texture.png")
  local texture = "pd_girl_texture.png"
  if g.vars.isWebP then
    texture = "pd_girl_texture.webp"
  end
  self.pokerGirlBatchNode = display.newBatchNode(texture):addTo(self, POKER_GIRL_ZORDER)
  local girl = display.newSprite("#girl.png")
  girl:addTo(self.pokerGirlBatchNode):schedule(handler(self, self.pokerGirlBlink), 5)
  self.pokerGirlBatchNode:pos(0, 0):scale(g.vars.bgScale)
end
function Girl:pokerGirlBlink()
  local girlBlinkSP = display.newSprite("#girl_blink_1.png"):pos(-28, 200):addTo(self.pokerGirlBatchNode)
  girlBlinkSP:performWithDelay(function()
    girlBlinkSP:setDisplayFrame(display.newSpriteFrame("girl_blink_2.png"))
  end, 0.05)
  girlBlinkSP:performWithDelay(function()
    girlBlinkSP:setDisplayFrame(display.newSpriteFrame("girl_blink_1.png"))
  end, 0.15)
  girlBlinkSP:performWithDelay(function()
    girlBlinkSP:removeFromParent()
  end, 0.2)
end
function Girl:moveGirlToCenterAnimate()
  transition.moveTo(self, {
    time = 0.5,
    x = display.cx / 2
  })
end
return Girl

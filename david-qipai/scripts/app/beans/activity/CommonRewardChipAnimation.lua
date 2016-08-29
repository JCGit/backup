local CommonRewardChipAnimation = class("CommonRewardChipAnimation", function()
  return display.newNode()
end)
local DISTENCE_X = 100
local DISTENCE_Y = 100
function CommonRewardChipAnimation:ctor(srcPt, destPt)
  if srcPt == nil then
    srcPt = ccp(0, 0)
  end
  if destPt == nil then
    destPt = ccp(display.c_right - 160, display.c_bottom + 20)
  end
  local texture = "pd_common_texture.png"
  if g.vars.isWebP then
    texture = "pd_common_texture.webp"
  end
  self._chipsBatchNode = display.newBatchNode(texture):addTo(self)
  self.chipTabel_1 = {}
  self.chipTabel_2 = {}
  self.chipTabel_3 = {}
  self.chipTabel_4 = {}
  self.chipTabel_5 = {}
  self.chipTabel_6 = {}
  self.chipTabel_7 = {}
  self._chipsList = {}
  math.randomseed(g.util.functions.getTime())
  local animTime = math.round(6 * math.random() - 0.499999) * 0.1
  local delayAnimTime = 0.2
  for i = 1, 7 do
    self._chipsList[i] = {}
    for j = 1, 2 do
      self._chipsList[i][j] = display.newSprite("#common_reward_chip_0" .. i .. ".png"):pos(srcPt.x, srcPt.y):addTo(self._chipsBatchNode)
      local moveToExplosion = transition.moveTo(self._chipsList[i][j], {
        time = 0.3,
        x = srcPt.x + (math.random() * 2 - 1) * DISTENCE_X,
        y = srcPt.y + (math.random() * 2 - 1) * DISTENCE_Y
      })
      local moveToDest = transition.moveTo(self._chipsList[i][j], {
        time = 0.3 + 0.4 * math.random(),
        x = destPt.x,
        y = destPt.y,
        delay = delayAnimTime,
        rotation = 120,
        onComplete = handler(self._chipsList[i][j], function(chips)
          chips:opacity(0)
        end)
      })
      transition.stopTarget(self._chipsList[i][j])
      self._chipsList[i][j]:runAction(transition.sequence({moveToExplosion, moveToDest}))
      self._chipsList[i][j]:runAction(CCRepeatForever:create(transition.sequence({
        CCRotateTo:create(delayAnimTime, 180),
        CCRotateTo:create(delayAnimTime, 360)
      })))
    end
  end
end
return CommonRewardChipAnimation

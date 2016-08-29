local MagicEmoji = class("MagicEmoji")
MagicEmoji.sourceRef = {
  "pd_magic_bomb",
  "pd_magic_dog",
  "pd_magic_ghost",
  "pd_magic_kiss",
  "pd_magic_rose",
  "pd_magic_tomato",
  "pd_magic_water",
  "pd_magic_cock",
  "pd_magic_dagger",
  "pd_magic_missile"
}
function MagicEmoji:ctor(contentView)
  if DEBUG_MEMORY_STATUS then
    logger:debug("Object-Life:" .. self.__cname .. ": ctor")
    g.memoryManager:registerObject(self, "MagicEmoji")
  end
  self._contentView = contentView
  self._majicEmojiBuffer = {}
  self._magicEmojiLoadingQueue = {}
end
function MagicEmoji:dispose()
  for k, v in pairs(self._majicEmojiBuffer) do
    display.removeAnimationCache("hddjAnim" .. k)
    local resourceName = MagicEmoji.sourceRef[i]
    if resourceName then
      unloadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png")
    end
  end
  self._majicEmojiBuffer = nil
  self._magicEmojiLoadingQueue = nil
  self.isDisposed_ = true
end
function MagicEmoji:playMagicAnimation(fromPoint, toPoint, magicIndex, completeCallback)
  if magicIndex == 1 then
    self:playMagicBombAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 2 then
    self:palyMagicDogAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 3 then
    self:palyMagicGhostAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 4 then
    self:palyMagicKissAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 5 then
    self:palyMagicRoseAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 6 then
    self:palyMagicTomatoAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 7 then
    self:palyMagicWaterAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 8 then
    self:palyMagicCockAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 9 then
    self:palyMagicDangerAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  elseif magicIndex == 10 then
    self:playMagicMissileAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  end
  return
end
MagicEmoji.hddjConfig = {
  [1] = {},
  [2] = {},
  [3] = {},
  [4] = {},
  [5] = {},
  [6] = {},
  [7] = {},
  [8] = {},
  [9] = {
    delay = 0,
    x = 0,
    y = 0,
    iconX = 0,
    iconY = 0
  }
}
function MagicEmoji:playMagicBombAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 1
  local bombTime = 1
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png")
  icon:pos(fromPoint.x, fromPoint.y)
  icon:addTo(self._contentView)
  local distance = cc.PointDistance(fromPoint, toPoint)
  local bconfig = cc.BezierConfig()
  bconfig.endPosition = toPoint
  bconfig.controlPoint_1 = ccp((fromPoint.x + toPoint.x) * 0.5, (fromPoint.y + toPoint.y) * 0.5 + distance * 0.16)
  bconfig.controlPoint_2 = ccp((fromPoint.x + toPoint.x) * 0.5, (fromPoint.y + toPoint.y) * 0.5 + distance * 0.16)
  transition.execute(icon, transition.sequence({
    CCEaseInOut:create(CCBezierTo:create(flyTime, bconfig), 2),
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      icon:removeFromParent()
    end)
  }))
  icon:runAction(transition.sequence({
    CCDelayTime:create(flyTime * 0.65),
    CCCallFunc:create(function()
      g.audio:playMagicEmojiSound("BOMB")
    end)
  }))
  if fromPoint.x < toPoint.x then
    icon:rotateBy(flyTime, 1080)
  else
    icon:rotateBy(flyTime, -1080)
  end
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = {}
    for i = 1, 100 do
      local frameIndex = i
      if i < 10 then
        frameIndex = "000" .. tostring(i)
      elseif i < 100 then
        frameIndex = "00" .. tostring(i)
      elseif i < 100 then
        frameIndex = "0" .. tostring(i)
      end
      local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(resourceName .. "_" .. frameIndex .. ".png")
      if frame then
        table.insert(frames, frame)
      end
    end
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local ani = display.newSprite():pos(toPoint):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, flyTime + 0.1)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * frameNum),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(0.3),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:palyMagicDogAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 1
  local animationTime = 1
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png")
  icon:pos(fromPoint.x, fromPoint.y):scale(0.6)
  if fromPoint.x > toPoint.x then
    icon:setScaleX(-0.6)
  end
  icon:addTo(self._contentView)
  g.audio:playMagicEmojiSound("DOG")
  icon:runAction(transition.sequence({
    CCEaseOut:create(CCMoveTo:create(1, toPoint), flyTime),
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      icon:removeFromParent()
    end)
  }))
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    print(animName .. " texture loaded")
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = display.newFrames(resourceName .. "_%04d.png", 1, frameNum, loop)
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local ani = display.newSprite():pos(toPoint):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, flyTime + 0.1)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * frameNum),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(0.5),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:palyMagicGhostAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 1
  local animationTime = 1
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png")
  icon:pos(fromPoint.x, fromPoint.y)
  icon:addTo(self._contentView)
  g.audio:playMagicEmojiSound("GHOST")
  icon:runAction(transition.sequence({
    CCEaseOut:create(CCMoveTo:create(1, toPoint), flyTime),
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      icon:removeFromParent()
    end)
  }))
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    print(animName .. " texture loaded")
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = display.newFrames(resourceName .. "_%04d.png", 1, frameNum, loop)
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local ani = display.newSprite():pos(toPoint):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, flyTime + 0.1)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * frameNum),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(0.5),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:palyMagicKissAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 1
  local animationTime = 1
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png")
  icon:pos(fromPoint.x, fromPoint.y):scale(0.6)
  icon:addTo(self._contentView)
  g.audio:playMagicEmojiSound("KISS")
  icon:runAction(transition.sequence({
    CCEaseOut:create(CCMoveTo:create(1, toPoint), flyTime),
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      icon:removeFromParent()
    end)
  }))
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    print(animName .. " texture loaded")
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = display.newFrames(resourceName .. "_%04d.png", 1, frameNum, loop)
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local ani = display.newSprite():pos(toPoint):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, flyTime + 0.1)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * frameNum),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(0.6),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:palyMagicRoseAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 1
  local animationTime = 1
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png")
  icon:pos(fromPoint.x, fromPoint.y):scale(0.6)
  icon:addTo(self._contentView)
  icon:runAction(transition.sequence({
    CCEaseOut:create(CCMoveTo:create(1, toPoint), flyTime),
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      icon:removeFromParent()
      g.audio:playMagicEmojiSound("ROSE")
    end)
  }))
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    print(animName .. " texture loaded")
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = {}
    for i = 1, 100 do
      local frameIndex = i
      if i < 10 then
        frameIndex = "000" .. tostring(i)
      elseif i < 100 then
        frameIndex = "00" .. tostring(i)
      elseif i < 100 then
        frameIndex = "0" .. tostring(i)
      end
      local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(resourceName .. "_" .. frameIndex .. ".png")
      if frame then
        table.insert(frames, frame)
      end
    end
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local ani = display.newSprite():pos(toPoint):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, flyTime + 0.1)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * frameNum),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(0.3),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:palyMagicTomatoAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 0.5
  local animationTime = 1
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png")
  icon:pos(fromPoint.x, fromPoint.y):scale(0.6)
  icon:addTo(self._contentView)
  local distance = cc.PointDistance(fromPoint, toPoint)
  local bconfig = cc.BezierConfig()
  bconfig.endPosition = toPoint
  bconfig.controlPoint_1 = ccp((fromPoint.x + toPoint.x) * 0.5, (fromPoint.y + toPoint.y) * 0.5 + distance * 0.16)
  bconfig.controlPoint_2 = ccp((fromPoint.x + toPoint.x) * 0.5, (fromPoint.y + toPoint.y) * 0.5 + distance * 0.16)
  if fromPoint.x < toPoint.x then
  else
  end
  icon:runAction(transition.sequence({
    CCEaseExponentialIn:create(CCBezierTo:create(flyTime, bconfig)),
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      icon:removeFromParent()
      g.audio:playMagicEmojiSound("TOMATO")
    end)
  }))
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    print(animName .. " texture loaded")
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = {}
    for i = 1, 100 do
      local frameIndex = i
      if i < 10 then
        frameIndex = "000" .. tostring(i)
      elseif i < 100 then
        frameIndex = "00" .. tostring(i)
      elseif i < 100 then
        frameIndex = "0" .. tostring(i)
      end
      local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(resourceName .. "_" .. frameIndex .. ".png")
      if frame then
        table.insert(frames, frame)
      end
    end
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local ani = display.newSprite():pos(toPoint):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, flyTime + 0.1)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * frameNum),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(0.6),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:palyMagicWaterAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 1
  local animationTime = 1
  toPoint = ccp(toPoint.x - 50, toPoint.y)
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png"):scale(0.6)
  icon:pos(fromPoint.x, fromPoint.y)
  icon:addTo(self._contentView)
  icon:runAction(transition.sequence({
    CCEaseOut:create(CCMoveTo:create(1, toPoint), flyTime),
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      icon:hide()
    end),
    CCDelayTime:create(0.7),
    CCCallFunc:create(function()
      g.audio:playMagicEmojiSound("WATER")
      icon:removeFromParent()
    end)
  }))
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    print(animName .. " texture loaded")
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = {}
    for i = 1, 100 do
      local frameIndex = i
      if i < 10 then
        frameIndex = "000" .. tostring(i)
      elseif i < 100 then
        frameIndex = "00" .. tostring(i)
      elseif i < 100 then
        frameIndex = "0" .. tostring(i)
      end
      local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(resourceName .. "_" .. frameIndex .. ".png")
      if frame then
        table.insert(frames, frame)
      end
    end
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local config = {}
    config.x = 30
    config.y = 30
    local ani = display.newSprite():scale(config.scale or 1):pos(toPoint.x + (config.x or 0), toPoint.y + (config.y or 0)):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, flyTime + 0.1)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint.x + (config.x or 0), toPoint.y + (config.y or 0)):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * frameNum),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(0.3),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:palyMagicCockAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  toPoint = ccp(toPoint.x + 20, toPoint.y + 20)
  local flyTime = 0.5
  local animationTime = 1
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png")
  icon:pos(fromPoint.x, fromPoint.y):scale(0.6)
  icon:addTo(self._contentView)
  local moveEndPoint = ccp(toPoint.x + 60, toPoint.y)
  icon:runAction(transition.sequence({
    CCEaseOut:create(CCMoveTo:create(flyTime, moveEndPoint), flyTime),
    CCCallFunc:create(function()
      icon:hide()
      g.audio:playMagicEmojiSound("COCK")
      icon:removeFromParent()
    end)
  }))
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    print(animName .. " texture loaded")
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = {}
    for i = 1, 100 do
      local frameIndex = i
      if i < 10 then
        frameIndex = "000" .. tostring(i)
      elseif i < 100 then
        frameIndex = "00" .. tostring(i)
      elseif i < 100 then
        frameIndex = "0" .. tostring(i)
      end
      local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(resourceName .. "_" .. frameIndex .. ".png")
      if frame and i % 5 == 0 then
        table.insert(frames, frame)
      end
    end
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local ani = display.newSprite():pos(toPoint):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, 0)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * #frames),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(0.3),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:palyMagicDangerAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 2
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    print(animName .. " texture loaded")
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local framesArray = dict:objectForKey("frames")
    local frameNum = framesArray:count()
    local frames = {}
    for i = 1, 100 do
      local frameIndex = i
      if i < 10 then
        frameIndex = "000" .. tostring(i)
      elseif i < 100 then
        frameIndex = "00" .. tostring(i)
      elseif i < 100 then
        frameIndex = "0" .. tostring(i)
      end
      local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(resourceName .. "_" .. frameIndex .. ".png")
      if frame then
        table.insert(frames, frame)
      end
    end
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local config = {}
    config.x = 30
    config.y = 30
    local ani = display.newSprite():scale(config.scale or 1):pos(fromPoint.x, fromPoint.y):addTo(self._contentView)
    ani:runAction(transition.sequence({
      CCCallFunc:create(function()
        g.audio:playMagicEmojiSound("DAGGER")
      end),
      CCEaseOut:create(CCMoveTo:create(flyTime * 0.5, toPoint), 1),
      CCDelayTime:create(0.1)
    }))
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, 0.1)
    local bombEffecticon = display.newSprite(frames[frameNum])
    bombEffecticon:pos(toPoint.x + (config.x or 0), toPoint.y + (config.y or 0)):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 + 0.1 * frameNum),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
function MagicEmoji:playMagicMissileAnimation(magicIndex, fromPoint, toPoint, completeCallback)
  local flyTime = 1
  local bombTime = 1
  local resourceName = MagicEmoji.sourceRef[magicIndex]
  local icon = display.newSprite("#" .. resourceName .. ".png")
  icon:pos(fromPoint.x, fromPoint.y)
  icon:addTo(self._contentView)
  local distance = cc.PointDistance(fromPoint, toPoint)
  local bconfig = cc.BezierConfig()
  bconfig.endPosition = toPoint
  local keyPoint = ccp((fromPoint.x + toPoint.x) * 0.5, (fromPoint.y + toPoint.y) * 0.5)
  local keyPoint = ccp((fromPoint.x + keyPoint.x) * 0.5, (fromPoint.y + keyPoint.y) * 0.5 + distance * 0.16)
  bconfig.controlPoint_1 = keyPoint
  bconfig.controlPoint_2 = keyPoint
  local delataValue1 = 0
  if keyPoint.x < fromPoint.x then
    delataValue1 = -90
  elseif keyPoint.x > fromPoint.x then
    delataValue1 = 90
  end
  local rotateValue1 = -math.deg(math.atan((keyPoint.y - fromPoint.y) / (keyPoint.x - fromPoint.x))) + delataValue1
  icon:rotation(rotateValue1)
  transition.execute(icon, transition.sequence({
    CCEaseInOut:create(CCBezierTo:create(flyTime, bconfig), 2),
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      icon:removeFromParent()
    end)
  }))
  local delataValue2 = 0
  if keyPoint.x < toPoint.x then
    delataValue2 = 90
  elseif keyPoint.x > toPoint.x then
    delataValue2 = -90
  end
  local rotateValue2 = -math.deg(math.atan((keyPoint.y - toPoint.y) / (keyPoint.x - toPoint.x))) + delataValue2
  icon:runAction(transition.sequence({
    CCDelayTime:create(0.1),
    CCCallFunc:create(function()
      g.audio:playMagicEmojiSound("MISSILE")
    end),
    CCDelayTime:create(flyTime * 0.5),
    CCCallFunc:create(function()
      icon:rotateTo(0.05, rotateValue2)
    end)
  }))
  local animName = "magicAnimation_" .. magicIndex
  loadTexture("pd_magic/" .. resourceName .. ".plist", "pd_magic/" .. resourceName .. ".png", function()
    local fullPath = cc.FileUtils:getInstance():fullPathForFilename("pd_magic/" .. resourceName .. ".plist")
    local dict = CCDictionary:createWithContentsOfFileThreadSafe(fullPath)
    local frames = {}
    for i = 1, 100 do
      local frameIndex = i
      if i < 10 then
        frameIndex = "000" .. tostring(i)
      elseif i < 100 then
        frameIndex = "00" .. tostring(i)
      elseif i < 100 then
        frameIndex = "0" .. tostring(i)
      end
      local frame = CCSpriteFrameCache:sharedSpriteFrameCache():spriteFrameByName(resourceName .. "_" .. frameIndex .. ".png")
      if frame then
        table.insert(frames, frame)
      end
    end
    local anim = display.getAnimationCache(animName)
    if anim == nil then
      anim = display.newAnimation(frames, 0.1)
      display.setAnimationCache(animName, anim)
    end
    local ani = display.newSprite():pos(toPoint):addTo(self._contentView)
    ani:playAnimationOnce(anim, true, function()
      if completeCallback then
        completeCallback()
      end
    end, flyTime)
    local bombEffecticon = display.newSprite(frames[#frames])
    bombEffecticon:pos(toPoint):addTo(self._contentView):hide()
    transition.execute(bombEffecticon, transition.sequence({
      CCDelayTime:create(flyTime + 0.1 * #frames),
      CCCallFunc:create(function()
        bombEffecticon:show()
      end),
      CCDelayTime:create(1),
      CCCallFunc:create(function()
        bombEffecticon:removeFromParent()
      end)
    }))
  end)
end
return MagicEmoji

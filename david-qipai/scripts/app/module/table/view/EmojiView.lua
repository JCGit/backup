local EmojiView = class("EmojiView", function()
  return display.newNode()
end)
local MagicEmoji = import(".MagicEmoji")
local ViceJudge = require("app.module.table.model.ViceJudge")
function EmojiView:ctor()
  if DEBUG_MEMORY_STATUS then
    logger:debug("Object-Life:" .. self.__cname .. ": ctor")
    g.memoryManager:registerObject(self, "EmojiView")
  end
  loadTexture("pd_face_texture.plist", "pd_face_texture.png")
  local expNum = 20
  local totalItemsPerRow = 4
  local totalRows = math.ceil(expNum / totalItemsPerRow)
  local itemWidth = 80
  self.lvGrid = cc.ui.UIListView.new({
    viewRect = cc.rect(0, 0, totalItemsPerRow * itemWidth, totalRows * itemWidth),
    direction = cc.ui.UIScrollView.DIRECTION_VERTICAL
  }):addTo(self)
  for i = 1, totalRows do
    local item = self.lvGrid:newItem()
    local content
    content = display.newNode()
    for count = 1, totalItemsPerRow do
      do
        local idx = (i - 1) * totalItemsPerRow + count
        if expNum < idx then
          break
        end
        local emotionImage = "#face_" .. idx .. "_0.png"
        local emotionImageButton = cc.ui.UIPushButton.new({
          normal = "#transparent.png",
          pressed = "#transparent.png"
        }, {scale9 = true}):setButtonSize(itemWidth, itemWidth):onButtonPressed(function(event)
        end):onButtonRelease(function(event)
        end):onButtonClicked(clickHandler(self, function(event)
          self:sendEmojiRequest(0, idx)
        end)):align(display.CENTER, itemWidth * count - itemWidth / 2, itemWidth / 2):addTo(content):setTouchSwallowEnabled(false)
        local sprite = display.newSprite(emotionImage):addTo(content):pos(itemWidth * count - itemWidth / 2, itemWidth / 2)
        sprite:setScale(0.6)
      end
    end
    content:setContentSize(totalItemsPerRow * itemWidth, itemWidth)
    item:addContent(content)
    item:setItemSize(totalItemsPerRow * itemWidth, itemWidth)
    self.lvGrid:addItem(item)
  end
  self.lvGrid:reload()
end
function EmojiView:sendEmojiRequest(emojiType, emojiIndex)
  if g.vars.user.seatId then
    ViceJudge:asyncRequestSendEmoji(emojiType, emojiIndex)
    g.ui.manager.popup:removeAllPopup()
  else
    g.ui.manager.tip:top(g.lang:getContent("table.sendEmojiNotInSeat"))
  end
end
function EmojiView:playEmojiAnimation(targetView, emojiType, emojiId)
  local emojiViewOffsetY = 0
  local animName = "expression-" .. emojiId
  local anim = display.getAnimationCache(animName)
  if anim then
    local sp = display.newSprite()
    sp:playAnimationOnce(anim, true)
    sp:addTo(targetView, 100)
    sp:pos(0, emojiViewOffsetY)
  else
    loadTexture("pd_face_texture.plist", "pd_face_texture.png", function()
      local frames = display.newFrames("face_" .. emojiId .. "_%01d.png", 0, 3, false)
      local animation = display.newAnimation(frames, 0.25)
      animation:setLoops(2)
      display.setAnimationCache(animName, animation)
      local sp = display.newSprite()
      sp:playAnimationOnce(animation, true)
      sp:setPosition(0, emojiViewOffsetY)
      sp:addTo(targetView, 100)
    end)
  end
end
return EmojiView

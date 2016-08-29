local UIImageView = class("UIImageView", function()
  return display.newNode()
end)
function UIImageView:ctor()
  self._imageUrl = nil
  return self
end
function UIImageView:setImage(url, params)
  if url ~= nil and url == self._imageUrl then
    return
  elseif url ~= nil and self._imageUrl ~= nil then
    g.imageManager:cancleFetchImage(self._imageUrl)
  end
  self._params = params
  self:clearData()
  function createDefaultImage()
    local defaultImage = display.newSprite(params.defaultImageName):pos(0, 0):addTo(self, 1, 1)
    local imageSize = defaultImage:getContentSize()
    if imageSize.width > imageSize.height then
      defaultImage:scale(self._params.size.width / imageSize.width)
    else
      defaultImage:scale(self._params.size.height / imageSize.height)
    end
  end
  local imageViewSize = self:getContentSize()
  if params.defaultImageName then
    createDefaultImage()
  end
  if url then
    self._url = url
    g.imageManager:fetchImage(url, handler(self, self.successCallback), handler(self, self.failCallback))
  end
end
function UIImageView:successCallback(sprite)
  local imageSize = sprite:getContentSize()
  if self._params and self._params.size then
    if imageSize.width > imageSize.height then
      sprite:scale(self._params.size.width / imageSize.width)
    else
      sprite:scale(self._params.size.height / imageSize.height)
    end
  end
  if self._params then
    imageSize = sprite:getContentSize()
    sprite:pos(0, 0):addTo(self, 1, 2)
    self.imageSize = imageSize
    self.sprite = sprite
  end
  if self._params and self._params.success then
    self._params.success()
  end
end
function UIImageView:failCallback()
  if self._url then
    logger:debug("get sprint  fail -------" .. self._url)
  end
end
function UIImageView:clearData()
  local defaultImageSprite = self:getChildByTag(1)
  if defaultImageSprite then
    defaultImageSprite:removeFromParent()
  end
  local avatarImageSprite = self:getChildByTag(2)
  if avatarImageSprite then
    avatarImageSprite:removeFromParent()
  end
end
return UIImageView

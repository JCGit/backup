local UIImageView = import(".UIImageView")
local AvatarView = class("AvatarView", function()
  return display.newNode()
end)
AvatarView.DEFAULT_HEIGHT = 89
function AvatarView:ctor()
  self.imgClip = CCClippingNode:create():addTo(self)
  self.imgClip:setCascadeOpacityEnabled(true)
  self.stencil = display.newScale9Sprite("#common_avatar_img_clip.png", 0, 0, cc.size(AvatarView.DEFAULT_HEIGHT - 3, AvatarView.DEFAULT_HEIGHT - 3))
  self.imgClip:setStencil(self.stencil)
  self.imgClip:setAlphaThreshold(0)
  self._imageView = UIImageView.new()
  self.imgClip:addChild(self._imageView)
  self._imageView:setCascadeOpacityEnabled(true)
  self.imageFrame = display.newScale9Sprite("#common_avatar_boarder.png", 0, 0, cc.size(AvatarView.DEFAULT_HEIGHT, AvatarView.DEFAULT_HEIGHT)):addTo(self)
  return self
end
function AvatarView:setImage(url, params)
  params = params or {}
  if params and params.size == nil then
    params.size = cc.size(AvatarView.DEFAULT_HEIGHT, AvatarView.DEFAULT_HEIGHT)
  end
  self._imageView:setImage(url, params)
end
return AvatarView

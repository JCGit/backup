local TopTip = import(".TopTip")
local Tip = class("Tip")
function Tip:ctor(width, height)
  self._topTip = TopTip.new()
end
function Tip:top(content)
  self._topTip:play(content)
end
return Tip

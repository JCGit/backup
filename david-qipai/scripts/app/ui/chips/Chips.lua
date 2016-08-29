local Chips = class("Chips", function(filename)
  return display.newNode()
end)
Chips.SUIT = {
  "diamond",
  "club",
  "heart",
  "spade"
}
function Chips:ctor(color, suit, size)
  self._chip = display.newSprite("#common_" .. color .. "_chips_" .. size .. ".png"):addTo(self)
  self._suit = display.newSprite("#common_chips_" .. Chips.SUIT[suit] .. "_" .. size .. ".png"):addTo(self)
end
function Chips:suitRotation(rotation)
  self._suit:rotation(rotation)
  return self
end
function Chips:suitPos(x, y)
  self._suit:pos(x, y)
  return self
end
return Chips

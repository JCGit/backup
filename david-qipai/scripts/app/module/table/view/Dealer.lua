local Dealer = class("Dealer", function()
  return display.newNode()
end)
function Dealer:ctor(dealerId)
  self.dealerId_ = dealerId
  local texture = "pd_table_texture.png"
  if g.vars.isWebP then
    texture = "pd_table_texture.webp"
  end
  self._batchNode = display.newBatchNode(texture):addTo(self)
  self._dealerGirl = display.newSprite("#pd_table_dealer.png"):addTo(self._batchNode)
end
return Dealer

DCCardsGame = {}
function DCCardsGame.play(roomId, id, coinType, loseOrGain, tax, left)
  DCLuaCardsGame:play(roomId, id, coinType, loseOrGain, tax, left)
end
function DCCardsGame.gain(roomId, id, coinType, gain, left)
  DCLuaCardsGame:gain(roomId, id, coinType, gain, left)
end
function DCCardsGame.lost(roomId, id, coinType, lost, left)
  DCLuaCardsGame:lost(roomId, id, coinType, lost, left)
end
return DCCardsGame

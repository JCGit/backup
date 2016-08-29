DCCoin = {}
function DCCoin.setCoinNum(coinNum, coinType)
  DCLuaCoin:setCoinNum(coinNum, coinType)
end
function DCCoin.lost(id, coinType, lost, left)
  DCLuaCoin:lost(id, coinType, lost, left)
end
function DCCoin.lostInLevel(id, coinType, lost, left, levelId)
  DCLuaCoin:lostInLevel(id, coinType, lost, left, levelId)
end
function DCCoin.gain(id, coinType, gain, left)
  DCLuaCoin:gain(id, coinType, gain, left)
end
function DCCoin.gainInLevel(id, coinType, gain, left, levelId)
  DCLuaCoin:gainInLevel(id, coinType, gain, left, levelId)
end
return DCCoin

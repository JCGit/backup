DCLevels = {}
function DCLevels.begin(levelId)
  DCLuaLevels:begin(levelId)
end
function DCLevels.complete(levelId)
  DCLuaLevels:complete(levelId)
end
function DCLevels.fail(levelId, failPoint)
  DCLuaLevels:fail(levelId, failPoint)
end
return DCLevels

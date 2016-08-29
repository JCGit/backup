local LevelInfo = class("LevelInfo")
local LEVEL_TABLE = g.lang:getContent("help.level")
LEVEL_TABLE = LEVEL_TABLE[2].content
function LevelInfo:ctor()
  local lastExperience = 0
  local levelItem
  for i = #LEVEL_TABLE, 1, -1 do
    levelItem = LEVEL_TABLE[i]
    levelItem.needExperience = lastExperience > levelItem.experience and lastExperience - levelItem.experience or 0
    lastExperience = levelItem.experience
  end
end
function LevelInfo:getMaxLevel()
  return #LEVEL_TABLE
end
function LevelInfo:getLevelByExperience(experience)
  local levelItem
  for i = 1, #LEVEL_TABLE do
    levelItem = LEVEL_TABLE[i]
    if levelItem.experience == experience then
      return i
    elseif experience < LEVEL_TABLE[i].experience then
      if i - 1 < 1 then
        return 1
      else
        return i - 1
      end
    end
  end
  return #LEVEL_TABLE
end
function LevelInfo:getTitleByLevel(level)
  local levelItem = LEVEL_TABLE[level]
  if levelItem then
    return levelItem.title
  end
  return ""
end
function LevelInfo:getTitleByExperience(experience)
  local level = self:getLevelByExperience(experience)
  return self:getTitleByLevel(level)
end
function LevelInfo:getLevelUpProgress(experience)
  local level = self:getLevelByExperience(experience)
  local nextLevel = level + 1 <= #LEVEL_TABLE and level + 1 or #LEVEL_TABLE
  if level == nextLevel then
    return 0, 0, 0
  else
    local progress = experience - LEVEL_TABLE[level].experience
    local target = LEVEL_TABLE[level].needExperience
    return progress / target, progress, target
  end
end
function LevelInfo:getLevelUpReward(level, success, fail)
  local requestData = {}
  requestData.level = level
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "getLvReward", requestData), handler(self, function(self, data)
    local jsonData = json.decode(data)
    if jsonData.ret == 0 then
      success(jsonData.info)
    else
      if jsonData.ret == -2 then
        g.ui.manager.tip:top(g.lang:getContent("levelup.levelNotEnough"))
      elseif jsonData.ret == -6 then
        g.ui.manager.tip:top(g.lang:getContent("levelup.confilcated"))
      end
      if fail then
        fail()
      end
    end
  end), function()
    g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
    if fail then
      fail()
    end
  end)
end
function LevelInfo:getLevelUpInfo(success, fail)
  local requestData = {}
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "getLvUp", requestData), handler(self, function(self, data)
    local jsonData = json.decode(data)
    if jsonData.ret == 0 then
      success(jsonData.info)
    elseif fail then
      fail()
    end
  end), function()
    if fail then
      fail()
    end
  end)
end
return LevelInfo

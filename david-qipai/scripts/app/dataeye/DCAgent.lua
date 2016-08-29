DCAgent = {}
function DCAgent.onStart(appId, channelId)
  DCLuaAgent:onStart(appId, channelId)
end
function DCAgent.setDebugMode(mode)
  DCLuaAgent:setDebugMode(mode)
end
function DCAgent.setReportMode(mode)
  DCLuaAgent:setReportMode(mode)
end
function DCAgent.setUploadInterval(interval)
  DCLuaAgent:setUploadInterval(interval)
end
function DCAgent.setVersion(version)
  DCLuaAgent:setVersion(version)
end
function DCAgent.reportError(title, content)
  DCLuaAgent:reportError(title, content)
end
function DCAgent.uploadNow()
  DCLuaAgent:uploadNow()
end
function DCAgent.getUID()
  return DCLuaAgent:getUID()
end
return DCAgent

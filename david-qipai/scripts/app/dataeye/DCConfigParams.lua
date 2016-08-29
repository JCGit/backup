DCConfigParams = {}
function DCConfigParams.update()
  DCLuaConfigParams:update()
end
function DCConfigParams.getParamNumber(key, defaultValue)
  return DCLuaConfigParams:getParameter(key, defaultValue)
end
function DCConfigParams.getParamString(key, defaultValue)
  return DCLuaConfigParams:getParameter(key, defaultValue)
end
function DCConfigParams.getParamBool(key, defaultValue)
  return DCLuaConfigParams:getParameter(key, defaultValue)
end
return DCConfigParams

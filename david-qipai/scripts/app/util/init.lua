local util = {}
util.DebugTool = import(".DebugTool")
util.inspect = import(".inspect")
util.functions = import(".functions")
local SocketBeanEvent = require("app.beans.socket.event.SocketBeanEvent")
function handlerHelper(obj, method, param)
  return function(...)
    if param == nil then
      return method(obj, param, ...)
    else
      return method(obj, param, ...)
    end
  end
end
function clickHandler(obj, method, param)
  return function(...)
    g.audio:playSound("CLICK_BUTTON")
    if param == nil then
      return method(obj, ...)
    else
      return method(obj, param, ...)
    end
  end
end
function clickSound()
  return function(...)
    g.audio:playSound("CLICK_BUTTON")
  end
end
function clickSoundImmediately()
  g.audio:playSound("CLICK_BUTTON")
end
function clickHelper(sprite, handler)
  sprite:setTouchEnabled(true)
  sprite:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
    if event.name == "began" then
      handler(sprite, event)
    end
    return true
  end)
end
function showSceneLoading(text)
  if g.obj.sceneLoading == nil then
    g.obj.sceneLoading = g.ui.SceneLoading.new(text)
    g.obj.sceneLoading:retain()
    g.obj.sceneLoading:performWithDelay(function()
      if g.obj.sceneLoading then
        hideSceneLoading()
        g.ui.manager.tip:top(g.lang:getContent("common.badNetwork"))
        g.eventCenter:dispatchEvent({
          name = SocketBeanEvent.EVT_DISCONNECT
        })
      end
    end, 20)
  end
  g.obj.sceneLoading:show()
end
function hideSceneLoading()
  if g.obj.sceneLoading ~= nil then
    g.obj.sceneLoading:hide()
    g.obj.sceneLoading:release()
    g.obj.sceneLoading = nil
  end
end
function loadTexture(resPlist, resPng, handler)
  if resPlist ~= nil and resPng ~= nil then
    if g.vars.isWebP then
      resPng = string.gsub(resPng, ".png", ".webp")
    end
    display.addSpriteFramesWithFile(resPlist, resPng, handler)
  else
    print("load texture error !")
  end
end
function unloadTexture(resPlist, resPng, handler)
  if resPlist ~= nil and resPng ~= nil then
    if g.vars.isWebP then
      resPng = string.gsub(resPng, ".png", ".webp")
    end
    display.removeSpriteFramesWithFile(resPlist, resPng, handler)
  else
    print("unload texture error !")
  end
end
local exportstring = function(s)
  return string.format("%q", s)
end
function tableSave(tbl, filename)
  local charS, charE = "   ", "\n"
  local file, err = io.open(filename, "wb")
  if err then
    return err
  end
  local tables, lookup = {tbl}, {
    [tbl] = 1
  }
  file:write("return {" .. charE)
  for idx, t in ipairs(tables) do
    file:write("-- Table: {" .. idx .. "}" .. charE)
    file:write("{" .. charE)
    local thandled = {}
    for i, v in ipairs(t) do
      thandled[i] = true
      local stype = type(v)
      if stype == "table" then
        if not lookup[v] then
          table.insert(tables, v)
          lookup[v] = #tables
        end
        file:write(charS .. "{" .. lookup[v] .. "}," .. charE)
      elseif stype == "string" then
        file:write(charS .. exportstring(v) .. "," .. charE)
      elseif stype == "number" then
        file:write(charS .. tostring(v) .. "," .. charE)
      end
    end
    for i, v in pairs(t) do
      if not thandled[i] then
        local str = ""
        local stype = type(i)
        if stype == "table" then
          if not lookup[i] then
            table.insert(tables, i)
            lookup[i] = #tables
          end
          str = charS .. "[{" .. lookup[i] .. "}]="
        elseif stype == "string" then
          str = charS .. "[" .. exportstring(i) .. "]="
        elseif stype == "number" then
          str = charS .. "[" .. tostring(i) .. "]="
        end
        if str ~= "" then
          stype = type(v)
          if stype == "table" then
            if not lookup[v] then
              table.insert(tables, v)
              lookup[v] = #tables
            end
            file:write(str .. "{" .. lookup[v] .. "}," .. charE)
          elseif stype == "string" then
            file:write(str .. exportstring(v) .. "," .. charE)
          elseif stype == "number" then
            file:write(str .. tostring(v) .. "," .. charE)
          end
        end
      end
    end
    file:write("}," .. charE)
  end
  file:write("}")
  file:close()
end
function tableLoad(sfile)
  local ftables, err = loadfile(sfile)
  if err then
    return _, err
  end
  local tables = ftables()
  for idx = 1, #tables do
    local tolinki = {}
    for i, v in pairs(tables[idx]) do
      if type(v) == "table" then
        tables[idx][i] = tables[v[1]]
      end
      if type(i) == "table" and tables[i[1]] then
        table.insert(tolinki, {
          i,
          tables[i[1]]
        })
      end
    end
    for _, v in ipairs(tolinki) do
      tables[idx][v[2]], tables[idx][v[1]] = tables[idx][v[1]], nil
    end
  end
  return tables[1]
end
return util

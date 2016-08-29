local LangManager = class("LangManager")
local language = require("app.language")
function LangManager:getContent(key)
  assert(key ~= nil, "key cannot be nil")
  local realKey = key .. language.distribute
  local indexs = string.split(realKey, ".")
  local langContent
  for i, v in ipairs(indexs) do
    if langContent == nil then
      langContent = language[v]
    else
      langContent = langContent[v]
    end
  end
  if langContent == nil then
    local realKey = key .. "_chs"
    local indexs = string.split(realKey, ".")
    local langContent
    for i, v in ipairs(indexs) do
      if langContent == nil then
        langContent = language[v]
      else
        langContent = langContent[v]
      end
    end
    if langContent == nil then
      print("language." .. key .. " undefined in language.lua")
      return nil
    end
    return langContent
  end
  return langContent
end
return LangManager

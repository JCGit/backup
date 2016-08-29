if g ~= nil then
  g = nil
  print("清除全局变量")
end
local updataStroagePath = launchInfo.GLOBLE_DEVICE_PATH .. "upd/"
local flistData
if io.exists(updataStroagePath .. "resourcelist") then
  flistData = dofile(updataStroagePath .. "resourcelist")
end
local ownModule = {}
for k, v in pairs(package.loaded) do
  local startIndex = string.find(k, "app")
  if startIndex == 1 then
    table.insert(ownModule, k)
  end
end
for i, v in ipairs(ownModule) do
  package.loaded[v] = nil
end
print("####ownModule unloaded!")
require("app.PokDengApp").new():run()

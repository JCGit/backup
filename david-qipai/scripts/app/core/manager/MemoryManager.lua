local MemoryManager = class("MemoryManager")
function MemoryManager:ctor()
  self.objects = {}
  setmetatable(self.objects, {__mode = "k"})
end
function MemoryManager:registerObject(object, nickName)
  object.nickName = nickName
  if nickName == nil then
    self.objects[object] = "nickName"
  else
    self.objects[object] = nickName
  end
end
function MemoryManager:snapshot()
  collectgarbage()
  print("**************MemoryManager SnapShot**********")
  for key, value in pairs(self.objects) do
    print("----------------------Item Start -----------")
    if type(key) == type({}) then
      print(key.__cname .. ":" .. value)
    else
      print(type(key) .. ":" .. value)
    end
    if key.nickName ~= nil then
      print("nickname :" .. key.nickName)
    end
    print("----------------------Item End -----------")
  end
  print("*********************************************")
end
return MemoryManager

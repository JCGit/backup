require("lfs")
local FileSystemManager = class("FileSystemManager")
function FileSystemManager:ctor()
end
function FileSystemManager:exists(relativePath)
  local path = device.writablePath .. relativePath
  return CCFileUtils:sharedFileUtils():isFileExist(path)
end
function FileSystemManager:mkdir(relativePath)
  local path = device.writablePath .. relativePath
  print("mkdir ---")
  if not FileSystemManager:exists(path) then
    local relativePathArray = g.util.functions.split(relativePath, device.directorySeparator)
    local currentDir
    for i, v in ipairs(relativePathArray) do
      currentDir = currentDir or ""
      currentDir = currentDir .. v .. device.directorySeparator
      print(currentDir)
      if not FileSystemManager:exists(currentDir) then
        local currentDirPath = device.writablePath .. currentDir
        local success = lfs.mkdir(currentDirPath)
        if not success then
          return false
        end
      end
    end
  end
  return true
end
function FileSystemManager:rmdir(relativePath)
  local path = device.writablePath .. relativePath
  print("FileSystemManager-rmdir:", path)
  if FileSystemManager:exists(path) then
    do
      local function _rmdir(path)
        local iter, dir_obj = lfs.dir(path)
        while true do
          local dir = iter(dir_obj)
          if dir == nil then
            break
          end
          if dir ~= "." and dir ~= ".." then
            local curDir = path .. dir
            local mode = lfs.attributes(curDir, "mode")
            if mode == "directory" then
              _rmdir(curDir .. "/")
            elseif mode == "file" then
              os.remove(curDir)
            end
          end
        end
        local succ, des = os.remove(path)
        if des then
          print(des)
        end
        return succ
      end
      _rmdir(path)
    end
  end
  return true
end
return FileSystemManager

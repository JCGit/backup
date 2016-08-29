local socket = require("socket")
local functions = {}
function functions.getTime()
  return socket.gettime()
end
function functions.split(fullString, seperator)
  local nFindStartIndex = 1
  local nSplitIndex = 1
  local nSplitArray = {}
  while true do
    local nFindLastIndex, endIndex = string.find(fullString, seperator, nFindStartIndex)
    if not nFindLastIndex then
      nSplitArray[nSplitIndex] = string.sub(fullString, nFindStartIndex, string.len(fullString))
      break
    end
    nSplitArray[nSplitIndex] = string.sub(fullString, nFindStartIndex, nFindLastIndex - 1)
    nFindStartIndex = nFindLastIndex + endIndex - nFindLastIndex + 1
    nSplitIndex = nSplitIndex + 1
  end
  return nSplitArray
end
function functions.print_r(root)
  local cache = {
    [root] = "."
  }
  local function _dump(t, space, name)
    local temp = {}
    for k, v in pairs(t) do
      local key = tostring(k)
      if cache[v] then
        table.insert(temp, "+" .. key .. " {" .. cache[v] .. "}")
      elseif type(v) == "table" then
        local new_key = name .. "." .. key
        cache[v] = new_key
        table.insert(temp, "+" .. key .. _dump(v, space .. (next(t, k) and "|" or " ") .. string.rep(" ", #key), new_key))
      else
        table.insert(temp, "+" .. key .. " [" .. tostring(v) .. "]")
      end
    end
    return table.concat(temp, "\n" .. space)
  end
  print(_dump(root, "", ""))
end
function functions.copy_table(ori_tab)
  if type(ori_tab) ~= "table" then
    return
  end
  local new_tab = {}
  for k, v in pairs(ori_tab) do
    local vtype = type(v)
    if vtype == "table" then
      new_tab[k] = copy_table(v)
    else
      new_tab[k] = v
    end
  end
  return new_tab
end
function functions.deep_copy(object)
  local lookup_table = {}
  local function _copy(object)
    if type(object) ~= "table" then
      return object
    elseif lookup_table[object] then
      return lookup_table[object]
    end
    local new_table = {}
    lookup_table[object] = new_table
    for index, value in pairs(object) do
      new_table[_copy(index)] = _copy(value)
    end
    return setmetatable(new_table, getmetatable(object))
  end
  return _copy(object)
end
function functions.getPokerSuit(card)
  return bit.brshift(card, 4)
end
function functions.getPokerValue(card)
  local t = bit.band(card, 15)
  if t > 13 then
    return t - 13
  end
  return t
end
function functions.formatBigInt(value, floatCount)
  floatCount = floatCount or 3
  local floatFormat = "%." .. floatCount .. "f"
  local length = string.len(tostring(value))
  local value = tonumber(value)
  local result
  if length >= 13 then
    value = value / 1000000000000
    result = string.format(floatFormat, value)
    result = string.sub(result, 1, string.len(result) - 1)
    result = result .. "T"
  elseif length >= 10 then
    value = value / 1000000000
    result = string.format(floatFormat, value)
    result = string.sub(result, 1, string.len(result) - 1)
    result = result .. "B"
  elseif length >= 7 then
    value = value / 1000000
    result = string.format(floatFormat, value)
    result = string.sub(result, 1, string.len(result) - 1)
    result = result .. "M"
  elseif length >= 5 then
    value = value / 1000
    result = string.format(floatFormat, value)
    result = string.sub(result, 1, string.len(result) - 1)
    result = result .. "K"
  else
    return tostring(value)
  end
  while true do
    if string.find(result, "%.") then
      local length = string.len(result)
      local tmp = string.sub(result, length - 1, string.len(result) - 1)
      if tmp == "." then
        result = string.sub(result, 1, length - 2) .. string.sub(result, length)
        break
      else
        tmp = tonumber(tmp)
        if tmp == 0 then
          result = string.sub(result, 1, length - 2) .. string.sub(result, length)
          else
            break
          end
        end
      end
  end
  return result
end
function functions.randomSequence(begin, count)
  local sequence = {}
  for i = begin, count do
    sequence[i] = i
  end
  local rand = {}
  local length = count
  while length > 0 do
    local index = 1
    if length > 1 then
      math.newrandomseed()
      index = math.random(1, length)
    end
    table.insert(rand, table.remove(sequence, index))
    length = table.maxn(sequence)
  end
  return rand
end
function functions.isFileExist(path)
  return path and CCFileUtils:sharedFileUtils():isFileExist(path)
end
function functions.isDirExist(path)
  local success, msg = lfs.chdir(path)
  return success
end
function functions.mkdir(path)
  cc.LuaLog("mkdir " .. path)
  if not functions.isDirExist(path) then
    local prefix = ""
    if string.sub(path, 1, 1) == device.directorySeparator then
      prefix = device.directorySeparator
    end
    local pathInfo = string.split(path, device.directorySeparator)
    local i = 1
    while true do
      if i > #pathInfo then
        break
      end
      local p = string.trim(pathInfo[i] or "")
      if p == "" or p == "." then
        table.remove(pathInfo, i)
      elseif p == ".." then
        if i > 1 then
          table.remove(pathInfo, i)
          table.remove(pathInfo, i - 1)
          i = i - 1
        else
          return false
        end
      else
        i = i + 1
      end
    end
    for i = 1, #pathInfo do
      local curPath = prefix .. table.concat(pathInfo, device.directorySeparator, 1, i) .. device.directorySeparator
      if not functions.isDirExist(curPath) then
        local succ, err = lfs.mkdir(curPath)
        if not succ then
          cc.LuaLog("mkdir " .. path .. " failed, " .. err)
          return false
        end
      else
      end
    end
  end
  cc.LuaLog("done mkdir " .. path)
  return true
end
function functions.rmdir(path)
  cc.LuaLog("rmdir " .. path)
  if functions.isDirExist(path) then
    do
      local function _rmdir(path)
        local iter, dir_obj = lfs.dir(path)
        while true do
          local dir = iter(dir_obj)
          if dir == nil then
            break
          end
          if dir ~= "." and dir ~= ".." then
            local currentDir = path .. dir
            local mode = lfs.attributes(currentDir, "mode")
            if mode == "directory" then
              _rmdir(currentDir .. "/")
            elseif mode == "file" then
              os.remove(currentDir)
            end
          end
        end
        local succ, dest = lfs.rmdir(path)
        if not succ then
          cc.LuaLog("remove dir " .. path .. " failed, " .. dest)
        end
        return succ
      end
      _rmdir(path)
    end
  end
  cc.LuaLog("done rmdir " .. path)
  return true
end
return functions

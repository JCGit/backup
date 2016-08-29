local inspect = {
  _VERSION = "inspect.lua 3.0.0",
  _URL = "http://github.com/kikito/inspect.lua",
  _DESCRIPTION = "human-readable representations of tables",
  _LICENSE = "    MIT LICENSE\n\n    Copyright (c) 2013 Enrique García Cota\n\n    Permission is hereby granted, free of charge, to any person obtaining a\n    copy of this software and associated documentation files (the\n    \"Software\"), to deal in the Software without restriction, including\n    without limitation the rights to use, copy, modify, merge, publish,\n    distribute, sublicense, and/or sell copies of the Software, and to\n    permit persons to whom the Software is furnished to do so, subject to\n    the following conditions:\n\n    The above copyright notice and this permission notice shall be included\n    in all copies or substantial portions of the Software.\n\n    THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS\n    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.\n    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY\n    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,\n    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE\n    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n  "
}
inspect.KEY = setmetatable({}, {
  __tostring = function()
    return "inspect.KEY"
  end
})
inspect.METATABLE = setmetatable({}, {
  __tostring = function()
    return "inspect.METATABLE"
  end
})
local smartQuote = function(str)
  if str:match("\"") and not str:match("'") then
    return "'" .. str .. "'"
  end
  return "\"" .. str:gsub("\"", "\\\"") .. "\""
end
local controlCharsTranslation = {
  ["\a"] = "\\a",
  ["\b"] = "\\b",
  ["\f"] = "\\f",
  ["\n"] = "\\n",
  ["\r"] = "\\r",
  ["\t"] = "\\t",
  ["\v"] = "\\v"
}
local function escape(str)
  local result = str:gsub("\\", "\\\\"):gsub("(%c)", controlCharsTranslation)
  return result
end
local isIdentifier = function(str)
  return type(str) == "string" and str:match("^[_%a][_%a%d]*$")
end
local isSequenceKey = function(k, length)
  return type(k) == "number" and k >= 1 and k <= length and math.floor(k) == k
end
local defaultTypeOrders = {
  ["number"] = 1,
  ["boolean"] = 2,
  ["string"] = 3,
  ["table"] = 4,
  ["function"] = 5,
  ["userdata"] = 6,
  ["thread"] = 7
}
local function sortKeys(a, b)
  local ta, tb = type(a), type(b)
  if ta == tb and (ta == "string" or ta == "number") then
    return a < b
  end
  local dta, dtb = defaultTypeOrders[ta], defaultTypeOrders[tb]
  if dta and dtb then
    return defaultTypeOrders[ta] < defaultTypeOrders[tb]
  elseif dta then
    return true
  elseif dtb then
    return false
  end
  return ta < tb
end
local function getNonSequentialKeys(t)
  local keys, length = {}, #t
  for k, _ in pairs(t) do
    if not isSequenceKey(k, length) then
      table.insert(keys, k)
    end
  end
  table.sort(keys, sortKeys)
  return keys
end
local getToStringResultSafely = function(t, mt)
  local __tostring = type(mt) == "table" and rawget(mt, "__tostring")
  local str, ok
  if type(__tostring) == "function" then
    ok, str = pcall(__tostring, t)
    if not ok or not str then
      str = "error: " .. tostring(str)
    end
  end
  if type(str) == "string" and #str > 0 then
    return str
  end
end
local maxIdsMetaTable = {
  __index = function(self, typeName)
    rawset(self, typeName, 0)
    return 0
  end
}
local idsMetaTable = {
  __index = function(self, typeName)
    local col = setmetatable({}, {__mode = "kv"})
    rawset(self, typeName, col)
    return col
  end
}
local function countTableAppearances(t, tableAppearances)
  tableAppearances = tableAppearances or setmetatable({}, {__mode = "k"})
  if type(t) == "table" then
    if not tableAppearances[t] then
      tableAppearances[t] = 1
      for k, v in pairs(t) do
        countTableAppearances(k, tableAppearances)
        countTableAppearances(v, tableAppearances)
      end
      countTableAppearances(getmetatable(t), tableAppearances)
    else
      tableAppearances[t] = tableAppearances[t] + 1
    end
  end
  return tableAppearances
end
local copySequence = function(s)
  local copy, len = {}, #s
  for i = 1, len do
    copy[i] = s[i]
  end
  return copy, len
end
local function makePath(path, ...)
  local keys = {
    ...
  }
  local newPath, len = copySequence(path)
  for i = 1, #keys do
    newPath[len + i] = keys[i]
  end
  return newPath
end
local function processRecursive(process, item, path)
  if item == nil then
    return nil
  end
  local processed = process(item, path)
  if type(processed) == "table" then
    local processedCopy = {}
    local processedKey
    for k, v in pairs(processed) do
      processedKey = processRecursive(process, k, makePath(path, k, inspect.KEY))
      if processedKey ~= nil then
        processedCopy[processedKey] = processRecursive(process, v, makePath(path, processedKey))
      end
    end
    local mt = processRecursive(process, getmetatable(processed), makePath(path, inspect.METATABLE))
    setmetatable(processedCopy, mt)
    processed = processedCopy
  end
  return processed
end
local Inspector = {}
local Inspector_mt = {__index = Inspector}
function Inspector:puts(...)
  local args = {
    ...
  }
  local buffer = self.buffer
  local len = #buffer
  for i = 1, #args do
    len = len + 1
    buffer[len] = tostring(args[i])
  end
end
function Inspector:down(f)
  self.level = self.level + 1
  f()
  self.level = self.level - 1
end
function Inspector:tabify()
  self:puts(self.newline, string.rep(self.indent, self.level))
end
function Inspector:alreadyVisited(v)
  return self.ids[type(v)][v] ~= nil
end
function Inspector:getId(v)
  local tv = type(v)
  local id = self.ids[tv][v]
  if not id then
    id = self.maxIds[tv] + 1
    self.maxIds[tv] = id
    self.ids[tv][v] = id
  end
  return id
end
function Inspector:putKey(k)
  if isIdentifier(k) then
    return self:puts(k)
  end
  self:puts("[")
  self:putValue(k)
  self:puts("]")
end
function Inspector:putTable(t)
  if t == inspect.KEY or t == inspect.METATABLE then
    self:puts(tostring(t))
  elseif self:alreadyVisited(t) then
    self:puts("<table ", self:getId(t), ">")
  elseif self.level >= self.depth then
    self:puts("{...}")
  else
    if self.tableAppearances[t] > 1 then
      self:puts("<", self:getId(t), ">")
    end
    do
      local nonSequentialKeys = getNonSequentialKeys(t)
      local length = #t
      local mt = getmetatable(t)
      local toStringResult = getToStringResultSafely(t, mt)
      self:puts("{")
      self:down(function()
        if toStringResult then
          self:puts(" -- ", escape(toStringResult))
          if length >= 1 then
            self:tabify()
          end
        end
        local count = 0
        for i = 1, length do
          if count > 0 then
            self:puts(",")
          end
          self:puts(" ")
          self:putValue(t[i])
          count = count + 1
        end
        for _, k in ipairs(nonSequentialKeys) do
          if count > 0 then
            self:puts(",")
          end
          self:tabify()
          self:putKey(k)
          self:puts(" = ")
          self:putValue(t[k])
          count = count + 1
        end
        if mt then
          if count > 0 then
            self:puts(",")
          end
          self:tabify()
          self:puts("<metatable> = ")
          self:putValue(mt)
        end
      end)
      if #nonSequentialKeys > 0 or mt then
        self:tabify()
      elseif length > 0 then
        self:puts(" ")
      end
      self:puts("}")
    end
  end
end
function Inspector:putValue(v)
  local tv = type(v)
  if tv == "string" then
    self:puts(smartQuote(escape(v)))
  elseif tv == "number" or tv == "boolean" or tv == "nil" then
    self:puts(tostring(v))
  elseif tv == "table" then
    self:putTable(v)
  else
    self:puts("<", tv, " ", self:getId(v), ">")
  end
end
function inspect.inspect(root, options)
  options = options or {}
  local depth = options.depth or math.huge
  local newline = options.newline or "\n"
  local indent = options.indent or "  "
  local process = options.process
  if process then
    root = processRecursive(process, root, {})
  end
  local inspector = setmetatable({
    depth = depth,
    buffer = {},
    level = 0,
    ids = setmetatable({}, idsMetaTable),
    maxIds = setmetatable({}, maxIdsMetaTable),
    newline = newline,
    indent = indent,
    tableAppearances = countTableAppearances(root)
  }, Inspector_mt)
  inspector:putValue(root)
  return table.concat(inspector.buffer)
end
setmetatable(inspect, {
  __call = function(_, ...)
    return inspect.inspect(...)
  end
})
return inspect

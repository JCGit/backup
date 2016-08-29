local logging = require("logging")
local openFile = function(self)
  self.file = io.open(self.filename, "a")
  if not self.file then
    return nil, string.format("file `%s' could not be opened for writing", self.filename)
  end
  self.file:setvbuf("line")
  return self.file
end
local function rollOver(self)
  for i = self.maxIndex - 1, 1, -1 do
    os.rename(self.filename .. "." .. i, self.filename .. "." .. i + 1)
  end
  self.file:close()
  self.file = nil
  local _, msg = os.rename(self.filename, self.filename .. "." .. "1")
  if msg then
    return nil, string.format("error %s on log rollover", msg)
  end
  return openFile(self)
end
local function openRollingFileLogger(self)
  if not self.file then
    return openFile(self)
  end
  local filesize = self.file:seek("end", 0)
  if filesize < self.maxSize then
    return self.file
  end
  return rollOver(self)
end
function logging.rolling_file(filename, maxFileSize, maxBackupIndex, logPattern)
  if type(filename) ~= "string" then
    filename = "lualogging.log"
  end
  local obj = {
    filename = filename,
    maxSize = maxFileSize,
    maxIndex = maxBackupIndex or 1
  }
  return logging.new(function(self, level, message)
    local f, msg = openRollingFileLogger(obj)
    if not f then
      return nil, msg
    end
    local s = logging.prepareLogMsg(logPattern, os.date(), level, message)
    f:write(s)
    return true
  end)
end
return logging.rolling_file

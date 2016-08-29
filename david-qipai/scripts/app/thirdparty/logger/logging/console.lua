local logging = import("..logging")
function logging.console(logPattern)
  return logging.new(function(self, level, message)
    print(logging.prepareLogMsg(logPattern, os.date(), level, message))
    return true
  end)
end
return logging.console

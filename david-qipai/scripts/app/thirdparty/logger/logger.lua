local log_console = import(".logging.console")
logger = log_console()
logger:setLevel("DEBUG")
print("Console Logging OK")
return logger

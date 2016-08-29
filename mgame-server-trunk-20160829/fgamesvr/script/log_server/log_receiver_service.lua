local skynet = require "skynet"
require "skynet.manager"	-- import skynet.register

local utils = require "utils"
local logger = require "logger"
local busilog_setting = require "busilog_setting"

skynet.register_protocol {
    name = "business_logger",
    id = 0,
    pack = function (...)
        local t = {...}
        for i,v in ipairs(t) do
            t[i] = tostring(v)
        end
        return table.concat(t," ")
    end,
    unpack = skynet.unpack,
}

local CMD = {}
CMD.init_log = {}

function CMD.init()
    for key ,value in pairs(busilog_setting.log_init_by_receiver) do
        skynet.launch("busilogger", value)
        CMD.init_log[key] = 1
    end
end

function CMD.revc(logtype, msg)
    if not CMD.init_log[logtype] then
        logger.warningf("cannot find bussiness by type=", logtype)
    end
   -- print(busilog_setting.log_init_name[logtype])
    skynet.send(busilog_setting.log_init_name[logtype], "business_logger", msg)
end

skynet.start(function()
    logger.trace ("start log_receiver_service")

    local traceback = debug.traceback
    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
             return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)
end)

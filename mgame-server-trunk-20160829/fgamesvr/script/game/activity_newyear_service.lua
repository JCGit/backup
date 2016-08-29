--元旦活动管理服务
--
--auth : diandian
--date : 2016/02/04
--

local skynet = require "skynet"
local utils = require "utils"

local redis_service


local CMD = {}

function CMD.notify_act_open(act_info_)
end

function CMD.notify_act_update(act_info_)
end

function CMD.notify_act_end(act_info_)
    skynet.call(redis_service, "lua", "del", "new_year_info")
end

local traceback = debug.traceback
skynet.start(function()

    redis_service = skynet.queryservice("redis_service")

    skynet.dispatch("lua", function(_, _, command, ...)
        --logger.trace("command  : " .. command)
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

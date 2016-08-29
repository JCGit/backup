--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/10/13
-- Time: 17:20
-- Desc: È«·þÁÄÌì
--

local skynet = require "skynet"
local logger = require "logger"
local cluster = require "cluster"
local syncserver

local redis_service

local CMD = {}
function CMD.chat2all(data, account_info)
    local cluster_list = skynet.call(syncserver, "lua", "get_all_gamecluster")
    for k, v in ipairs(cluster_list) do
        cluster.call(v, ".chat_service", "chat2all", data, account_info)
    end
end

local traceback = debug.traceback
skynet.start(function()
    logger.trace ("start global_chat_service")

    redis_service = skynet.queryservice("redis_service")
    syncserver = skynet.queryservice("syncserver")

    skynet.dispatch("lua", function(_, _, command, ...)
--        logger.trace("command")
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

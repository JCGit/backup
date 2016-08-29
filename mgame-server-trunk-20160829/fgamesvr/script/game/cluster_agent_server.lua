-- User: Zhenggp
-- Date: 2015/10/15
-- 该service针对于其他节点服调用 必定有返回值
local skynet = require "skynet"
local cluster = require "cluster"

local logger = require "logger"
local proto  = require "proto"
local utils  = require "utils"
local COMMON = require "common"

local service_name_mapping = {}

local CMD = {}

function CMD.agent_forward(address, func, ...)
    return skynet.call(address, "lua", func, ...)
end

function CMD.agent_forward_by_uin(uin, func, ...)
    local agent = skynet.call(".online_mgr", "lua", "get_uin_agent", uin)
    if agent then
       return skynet.call(agent, "lua", func, ...)
    end
end

--调用此方法需要保证servicename为uniqueservice
function CMD.agent_forward_by_servicename(servicename, skynet_func, func, ...)
    local service = service_name_mapping[servicename]
    if not service then
        service  = skynet.queryservice(servicename)
        if service then
          service_name_mapping[servicename] = service
        end
    end

    if service then
       local f = skynet[skynet_func]
       if f then
          return f(service, "lua", func, ...)
       else
          logger.tracef("unknow skynet func_name : %s", skynet_func)
       end
    end
end

skynet.start(function()
    local traceback = debug.traceback
    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s) id : %d", command, skynet.self())
            return skynet.retpack()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s  id : %d", command, ret, skynet.self())
            return skynet.retpack(ret)
        end
        return skynet.retpack(ret)
    end)
end)

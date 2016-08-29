local skynet = require "skynet"
local logger = require "logger"

-- 存放所有玩家的地址信息
local global_players = {}

local CMD = {}

function CMD.register(uin, node, handler)
    local player = {uin,node,handler}
    global_players[uin] = player
end

function CMD.unregister(uin, node, handler)
    global_players[uin] = nil
end

function CMD.query(uin)
    return global_players[uin]
end

skynet.start(function()
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






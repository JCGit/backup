local skynet = require "skynet"
local logger = require "logger"

-- 存放所有房间的地址信息
local global_rooms = {}

local CMD = {}

function CMD.register_room(uin,node,handler)
    local player = {uin,node,handler}
    global_rooms[uin] = player
end

function CMD.unregister_room(uin,node,handler)
    global_rooms[uin] = nil
end

function CMD.query(uin)
    return global_rooms[uin]
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

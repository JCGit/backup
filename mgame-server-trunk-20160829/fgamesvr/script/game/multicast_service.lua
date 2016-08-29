local skynet = require "skynet"
local mc     = require "multicast"
local dc     = require "datacenter"

local logger = require "logger"

local channel, server_event_channel

local CMD = {}

function CMD.publish_announce(kind, data)
    channel:publish(kind, data)
end

function CMD.server_event(...)
    server_event_channel:publish(...)
end

skynet.start(function()
    channel = mc.new()
    dc.set("MCCHANNEL", channel.channel)

    server_event_channel = mc.new()
    dc.set("SERVER_EVENT_CHANNEL", server_event_channel.channel)

    local traceback = debug.traceback
    skynet.dispatch("lua", function(_, _, command, ...)
        local ok, ret = xpcall(CMD[command], traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        return skynet.retpack(ret)
    end)
end)

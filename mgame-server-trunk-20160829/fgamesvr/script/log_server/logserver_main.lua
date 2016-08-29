local skynet = require "skynet"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name
local snax = require "snax"

local server_config = require "server_config"

skynet.start(function()

    skynet.newservice("debug_console", server_config.log_console_port)

    local log_receiver_service = skynet.uniqueservice("log_receiver_service")
    skynet.call(log_receiver_service, "lua", "init")

    skynet.name(".log_receiver_service", log_receiver_service)

    cluster.register("log_receiver_service", log_receiver_service)

    cluster.open "lognode"

end)

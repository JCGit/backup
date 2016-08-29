local skynet = require "skynet"
local skynet = require "skynet.manager" -- import skynet.abort()
local socket = require "socket"
local config = require "server_config"

local logger = require "logger"
local pvp_rank_service

local function exit_handle()
    skynet.call(pvp_rank_service, "lua", "exit_and_save")
end

local function command_loop(id)
    while true do
        local str = socket.readline(id, "\n")
        if str then
            if str == "Quit" then
                logger.error("skynet will exit in a few seconds")
                exit_handle()
                socket.write(id, "Exit ok")
                logger.error("skynet is exiting")
                skynet.sleep(300)
                skynet.abort()
                break
            end
        else
            return socket.close(id)
        end
    end
end

skynet.start(function()

    pvp_rank_service = skynet.queryservice("pvp_rank_service")

    local conf = config.rank_monitor_conf
    local ip = conf.ip or "127.0.0.1" -- 安全起见应该只在本机监听
    local port = tonumber(conf.port) or 7002
    logger.trace("monitor service listen at: ", ip, " port:", tostring(port))
    local id = socket.listen(ip, port)

    socket.start(id, function(id, addr)
        logger.trace("monitor service has connect from ", addr, id)
        socket.start(id)
        skynet.fork(command_loop, id)
    end)

end)

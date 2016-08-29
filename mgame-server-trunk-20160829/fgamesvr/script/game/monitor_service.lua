local skynet = require "skynet"
local skynet = require "skynet.manager" -- import skynet.abort()
local socket = require "socket"
local config = require "server_config"

local logger = require "logger"
local mysql_service
local gamed

local function exit_handle()
    skynet.call(gamed, "lua", "exit")
    print("----------------exit --1")
    skynet.call(mysql_service, "lua", "flush_queue")
    print("----------------exit --2")
end

local function command_loop(id)
    while true do
        local str = socket.readline(id, "\n")
        if str then
            if str == "Quit" then
                logger.error("skynet will exit in a few seconds")
                exit_handle()
                print("---------------1")
                socket.write(id, "Exit ok")
                print("---------------2")
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
    mysql_service     = skynet.queryservice("mysql_service")
    gamed             = skynet.queryservice("gamed")

    local conf = config.monitor_conf
    local ip = conf.ip or "127.0.0.1" -- 安全起见应该只在本机监听
    local port = tonumber(conf.port) or 7001
    logger.trace("monitor service listen at: ", ip, " port:", tostring(port))
    local id = socket.listen(ip, port)

    socket.start(id, function(id, addr)
        logger.trace("monitor service has connect from ", addr, id)
        socket.start(id)
        skynet.fork(command_loop, id)
    end)

end)

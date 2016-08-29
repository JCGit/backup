local skynet = require "skynet"
local skynet = require "skynet.manager" -- import skynet.abort()
local socket = require "socket"
local config = require "server_config"

local logger = require "logger"

-- 需要做退出处理的服务
local score_rank_service
local star_rank_service
local win_rank_service
local king_rank_service


local function exit_handle()
    skynet.call(score_rank_service, "lua", "exit")
    skynet.call(star_rank_service, "lua", "exit")
    skynet.call(win_rank_service, "lua", "exit")
    skynet.call(king_rank_service, "lua", "exit")
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

    score_rank_service = skynet.queryservice("score_rank_service")
    star_rank_service = skynet.queryservice("star_rank_service")
    win_rank_service = skynet.queryservice("win_rank_service")
    king_rank_service = skynet.queryservice("king_rank_service")


    local conf = config.common_monitor
    local ip = conf.ip or "127.0.0.1" -- 安全起见应该只在本机监听
    local port = tonumber(conf.port) or 7606
    logger.trace("common rank monitor service listen at: ", ip, " port:", tostring(port))
    local id = socket.listen(ip, port)

    socket.start(id, function(id, addr)
        logger.trace("common rank monitor service has connect from ", addr, id)
        socket.start(id)
        skynet.fork(command_loop, id)
    end)

end)

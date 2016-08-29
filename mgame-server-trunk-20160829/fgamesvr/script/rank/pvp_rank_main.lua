
local skynet = require "skynet"
local cluster = require "cluster"
local logger = require "logger"

require "skynet.manager" -- import skynet.name

local server_config = require "server_config"

skynet.start(function()
    -- TODO  注意service的注册顺序
    skynet.uniqueservice("debug_console", server_config.rank_console_port)

    skynet.uniqueservice("mysql_service")
    skynet.uniqueservice("redis_service")
    skynet.uniqueservice("plan_service")

    local pvp_rank_service = skynet.uniqueservice("pvp_rank_service")
    skynet.name(".pvp_rank_service", pvp_rank_service)


    skynet.uniqueservice("rank_test_service")
    --skynet.uniqueservice("rank_monitor_service")

    logger.trace("start complete...")
    cluster.open "ranknode1"
end)


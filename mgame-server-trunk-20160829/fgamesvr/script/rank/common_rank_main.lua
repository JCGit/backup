
local skynet = require "skynet"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name

local server_config = require "server_config"
local busilog_setting = require "busilog_setting"

skynet.start(function()
    -- TODO  注意service的注册顺序
    skynet.uniqueservice("debug_console", server_config.common_rank_console_port)

    skynet.uniqueservice("mysql_service")
    skynet.uniqueservice("redis_service")

    local plan = assert(skynet.uniqueservice("plan_service"))
    skynet.name(".plan_service", plan)

    local business_log_service = skynet.uniqueservice("business_log_service")
    skynet.send(business_log_service, "lua", "init", busilog_setting.log_init_by_top_rank)

    local score_rank_service = assert(skynet.uniqueservice("score_rank_service"))
    skynet.name(".score_rank_service", score_rank_service)

    local star_rank_service = assert(skynet.uniqueservice("star_rank_service"))
    skynet.name(".star_rank_service", star_rank_service)    
    
    local win_rank_service = assert(skynet.uniqueservice("win_rank_service"))
    skynet.name(".win_rank_service", win_rank_service)    
 
    local king_rank_service = assert(skynet.uniqueservice("king_rank_service"))
    skynet.name(".king_rank_service", king_rank_service)

    assert(skynet.uniqueservice("common_monitor_service"))

    cluster.open "commonrank"
end)


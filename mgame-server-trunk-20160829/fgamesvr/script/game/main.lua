local skynet = require "skynet"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name

local server_config = require "server_config"

local callback_logger = require "callback_logger"
local busilog_setting = require "busilog_setting"
local utils = require "utils"

skynet.start(function()
    skynet.newservice("debug_console", tonumber(server_config.game_console_port))

    skynet.uniqueservice("mysql_service")
    skynet.uniqueservice("redis_service")

    local plan = skynet.uniqueservice("plan_service")
    skynet.name(".plan_service", plan)

    skynet.uniqueservice("multicast_service")
    local mail_service = skynet.uniqueservice("mail_service")
    skynet.name(".mail_service", mail_service)

    callback_logger.init()

    local business_log_service = skynet.uniqueservice("business_log_service")
    skynet.send(business_log_service, "lua", "init", busilog_setting.log_init_by_gamed)

    local operate_actvity_service = skynet.uniqueservice("operate_activity_service")
    skynet.name(".activitys_service", operate_actvity_service)

    -- gate服务； 接受客户端连接，转发socket消息给agent服务，管理在线agent
    local gamed = skynet.uniqueservice("gamed")
    skynet.call(gamed, "lua", "open", server_config.gamed_conf)
    skynet.name(".gamed", gamed)

    -- 需要在operate_actvity_service init之前启动
    local proxy = assert(skynet.uniqueservice("game_proxy"))
    skynet.name(".proxy", proxy)

    skynet.call(operate_actvity_service, "lua", "init")

    skynet.uniqueservice("rank_service") -- mail redis gamed
    skynet.uniqueservice("rank_service_v2")

    -- pvp匹配服务器；处理发自agent的匹配/取消匹配请求，
    -- 匹配成功（定时器，人数） 创建room
    local pvpserver = skynet.uniqueservice("pvpserver")
    skynet.call(pvpserver, "lua", "open")
    skynet.name(".pvpserver", pvpserver)

    local pvp_room_server = skynet.uniqueservice("pvp_room_server")
    skynet.name(".pvp_room_server",pvp_room_server)
    skynet.call(pvp_room_server, "lua", "open")

    local activitys_service = skynet.uniqueservice("activitys_service")
    skynet.call(activitys_service, "lua", "init")
    skynet.name(".activitys_service", activitys_service)

    local gmservice = skynet.uniqueservice("gmservice")
    skynet.name(".gmservice", gmservice)

    -- 中心服给游戏服通知充值信息
    local callback_service = skynet.uniqueservice("callback_service")
    skynet.name(".callback_service", callback_service)

    local chat_service = skynet.uniqueservice("chat_service")
    skynet.name(".chat_service", chat_service)
    skynet.uniqueservice("monitor_service")

    skynet.name(".online_mgr",skynet.uniqueservice("gamed_online_mgr"))

    --代理转发
    local cluster_agent_server = skynet.uniqueservice("cluster_agent_server")
    skynet.name(".cluster_agent_server", cluster_agent_server)

    local nodename = utils.query_nodename(server_config.gamed_conf.servername)
    cluster.open(nodename)

    skynet.exit()
end)

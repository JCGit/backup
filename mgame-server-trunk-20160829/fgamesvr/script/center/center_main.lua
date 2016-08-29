--
-- Created by IntelliJ IDEA.
-- User: Administrator
-- Date: 2015/8/26
-- Time: 21:19
-- To change this template use File | Settings | File Templates.
--

local skynet = require "skynet"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name

local server_config = require "server_config"
local callback_logger = require "callback_logger"

skynet.start(function()
    -- TODO  注意service的注册顺序
    skynet.uniqueservice("debug_console", server_config.center_console_port)

    callback_logger.init()

    skynet.uniqueservice("mysql_service")
    skynet.uniqueservice("redis_service")
    skynet.uniqueservice("global_cdk_service")

    local syncserver = skynet.uniqueservice("syncserver")
    skynet.name(".syncserver", syncserver)

    local glb_mail = skynet.uniqueservice("global_mailservice")
    skynet.name(".global_mailservice", glb_mail)
    skynet.uniqueservice("global_gmservice")
    skynet.uniqueservice("global_webservice")

    local globalchatservice = skynet.uniqueservice("global_chat_service")
    skynet.name(".globalchatservice", globalchatservice)

    --全局玩家管理服务
    local gpm = skynet.uniqueservice("global_player_manager")
    skynet.name(".g_player_manager", gpm)

    --全局房间管理服务
    local grm =skynet.uniqueservice("global_room_manager")
    skynet.name(".g_room_manager", grm)

    skynet.uniqueservice("platform_callback")

    --充值回调服务
    skynet.uniqueservice("global_rechargeweb")

    local red_packet_mgr_service = skynet.uniqueservice("red_packet_mgr_service")
    skynet.name(".red_packet_mgr_service", red_packet_mgr_service)

    cluster.open "centernode"
end)


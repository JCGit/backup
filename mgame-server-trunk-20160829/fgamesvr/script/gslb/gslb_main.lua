--
-- Created by IntelliJ IDEA.
-- User: Denny
-- Date: 2015/10/7
-- Time: 18:26
-- To change this template use File | Settings | File Templates.
--

local skynet = require "skynet"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name

local server_config = require "server_config"

skynet.start(function()
    print("Glsb Main Server Start")
    skynet.newservice("debug_console", server_config.gslb_console_port)

    skynet.uniqueservice("mysql_service")
    skynet.uniqueservice("redis_service")

    local gslbserver = skynet.uniqueservice("gslbserver")
    skynet.call(gslbserver, "lua", "open", server_config.gslbd_conf)

    skynet.name(".gslbserver", gslbserver)
    cluster.open "gslbnode"
end)






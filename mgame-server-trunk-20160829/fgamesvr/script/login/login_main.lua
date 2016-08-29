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

local utils = require "utils"
local server_config = require "server_config"

skynet.start(function()
	skynet.newservice("debug_console", server_config.login_console_port)

	skynet.uniqueservice("mysql_service")
	skynet.uniqueservice("redis_service")

	--skynet.uniqueservice("platform")
	local loginserver = skynet.uniqueservice("login_server")
	skynet.call(loginserver, "lua", "open", server_config.logind_conf)
	skynet.name(".loginserver", loginserver)

	local nodename = utils.query_nodename(server_config.logind_conf.servername)
	cluster.open(nodename)
end)






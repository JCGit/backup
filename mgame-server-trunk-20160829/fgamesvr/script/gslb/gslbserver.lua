--
-- Created by IntelliJ IDEA.
-- User: Denny
-- Date: 2015/10/7
-- Time: 18:26
-- To change this template use File | Settings | File Templates.
--
--Global Server Load Balance 全局负载均衡服务

local skynet = require "skynet"
local gateserver = require "snax.gateserver"
local netpack = require "netpack"
local socketdriver = require "socketdriver"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name

local logger = require "logger"
local proto = require "proto"
local utils = require "utils"
local COMMON = require "common"
local server_config = require "server_config"
local json = require "cjson"
local setting = require "setting"

local traceback = debug.traceback

local gslbserver = {}
local handler = {}
local logind_list = {}
local logind_map = {}
local gamed_list = {}
local gamed_map = {}
local gamed_hash = {} --用于存放 host -> name 的hash映射
local ban_gamed_map = {}
local hotfix = {}
local dispatch = {} --负载分发算法集合
local balance_basic_seq = 1 --负载均衡配置序列号
local gamedlist_change_flag = false

local redis_service

local function send_package(user_fd, cmdid, pack)
	local len = #pack + 2
	local package = string.pack(">I2>I2", len, cmdid) .. pack

	socketdriver.send(user_fd, package)
end

----------------------------------------------------
function hotfix.set_ban_gamed(gamed_name_list)
	logger.trace("set ban game")
	for k,v in ipairs(gamed_name_list) do
		ban_gamed_map[v] = true
	end
end

function hotfix.clean_ban_gamed()
	ban_gamed_map = {}
end

------------------------------负载均衡相关-----------------------------------
-----------------------初始化操作---------------
local balance_basic
local function load_balance_config()
	--加载负载策略文件，生成相应的负载数据table
	local filePath = "setting/balance.json"
	os.execute("cp -f ../bak/setting/balance.json setting/balance.json")
	local file = assert(io.open(filePath, "r"))
	local data = file:read("a")
	balance_basic = json.decode(data)
	--print("balance_node -----------------")
	--base, gamed1, gamed2, white, white_range
end

local function update_balance_config(config)
	--1. 加载负载策略文件
	logger.debugf("config is [%s]", config)
	local file = assert(io.open("setting/balance.json", "w+"))
	file:write(config)
	file:flush()
	file:close()
	os.execute("cp -f setting/balance.json ../bak/setting/balance.json")
	balance_basic = json.decode(config)
	--base, gamed1, gamed2, white, white_range

	balance_basic_seq = balance_basic_seq + 1

	--logind_map
	--2. 通知其他login服负载策略文件  --Todo: 这里后面要加上重新注册机制
	for k,v in pairs(logind_map) do
		local loginnode = utils.query_nodename(k)
		--print("-------- login load --- ", k, " --- ", loginnode)
		--cluster.call(loginnode, ".loginserver", "update_balance_config", balance_basic)
		xpcall(cluster.call, traceback, loginnode, ".loginserver", "sync_balance_seq", balance_basic_seq)
	end
end

local gray_basic
local function load_gray_config()
	--加载负载策略文件，生成相应的负载数据table
	local filePath = "setting/gray.json"
	os.execute("cp -f ../bak/setting/gray.json setting/gray.json")
	local file = assert(io.open(filePath, "r"))
	local data = file:read("a")
	gray_basic = json.decode(data)
	--base, gamed1, gamed2, white, white_range
end

local function update_gray_config(config)
	--1. 加载灰度策略文件
	logger.debugf("config is [%s]", config)
	local file = assert(io.open("setting/gray.json", "w+"))
	file:write(config)
	file:flush()
	file:close()
	os.execute("cp -f setting/gray.json ../bak/setting/gray.json")
	gray_basic = json.decode(config)
	--base, gamed1, gamed2, white, white_range

	--logind_map
	--这里使用重发机制，重发超过三次就记录错误日志，并不再重发
	for k,v in pairs(logind_map) do
		local loginnode = utils.query_nodename(k)
		local success = false
		local retry_count = 0

		while not success and retry_count < 3 do  --重发超过3次就不再进行重发
			local ok, ret = xpcall(cluster.call, traceback, loginnode, ".loginserver", "sync_gray_config", gray_basic)
			if not ok then
				retry_count = retry_count + 1
			else
				success = true
			end
		end

		if not success then
			logger.errorf("gray basic sync %s fail ", loginnode)  --同步灰度配置超时的，记错误日志
		end
	end
end

local stop_basic
local function load_stop_config()
	--加载负载策略文件，生成相应的负载数据table
	local filePath = "setting/stop.json"
	os.execute("cp -f  ../bak/setting/stop.json setting/stop.json")
	local file = assert(io.open(filePath, "r"))
	local data = file:read("a")
	stop_basic = json.decode(data)
end

local function update_stop_config(config)
	--1. 加载负载策略文件
	logger.debugf("config is [%s]", config)
	local file = assert(io.open("setting/stop.json", "w+"))
	file:write(config)
	file:flush()
	file:close()
	os.execute("cp -f setting/stop.json ../bak/setting/stop.json")
	stop_basic = json.decode(config)

	--logind_map
	--这里使用重发机制，重发超过三次就记录错误日志，并不再重发
	for k,v in pairs(logind_map) do
		local loginnode = utils.query_nodename(k)
		local success = false
		local retry_count = 0

		while not success and retry_count < 3 do  --重发超过3次就不再进行重发
			local ok, ret = xpcall(cluster.call, traceback, loginnode, ".loginserver", "sync_stop_config", stop_basic)
			if not ok then
				retry_count = retry_count + 1
			else
				success = true
			end
		end

		if not success then
			logger.errorf("stop basic sync %s fail ", loginnode)  --同步灰度配置超时的，记错误日志
		end
	end
end

---------------------- 消息处理 -------------------
function handler.message(fd, msg, sz)
end

local heartbeat_overtime = 40  --秒
local heartbeat_check_time = 20 * 100 --毫秒
local function heartbeat_check()
	skynet.timeout(heartbeat_check_time, heartbeat_check)
	local now = math.floor(skynet.time())
	for k, v in pairs(gamed_map) do
		--print(k .. " now:", now , "last:", v.last_alive_time)
		if now - v.last_alive_time > heartbeat_overtime then
			for key, value in ipairs(gamed_list) do
				if value == k then
					table.remove(gamed_list, key)
					gamedlist_change_flag = true
				end
			end
			gamed_map[k] = nil
			--发送邮件报警
			-- 自己修改后面的邮件地址
			os.execute("echo 'gslb gamed server heart time out!! ' | mutt -s \"gslb server error!!\" " .. server_config.mail_conf.mails)
			logger.error(k .. " is not live")
		end
	end

	for k, v in pairs(logind_map) do
		if now - v.last_alive_time > heartbeat_overtime then
			for key, value in ipairs(logind_list) do
				if value == v.addr then
					table.remove(logind_list, key)
				end
			end
			logind_map[k] = nil
			--发送邮件报警
			-- 自己修改后面的邮件地址
			os.execute("echo 'gslb logind server heart time out!! ' | mutt -s \"gslb server error!!\" " .. server_config.mail_conf.mails)
			logger.error(k .. " is not live")
		end
	end
end

local notice_node_time = setting.gslb_setting.notice_node_time
local prev_balance_basic_seq = 0
local function notice_node_info()
	skynet.timeout(notice_node_time, notice_node_info)
	if not logind_list then return end
	if #logind_list ~= 0 then
		for k, v in ipairs(logind_list) do
			local node = utils.query_nodename(v)
			if gamedlist_change_flag then
				--xpcall(cluster.call, traceback, node, ".loginserver", "update_gamed_list", gamed_list)
				xpcall(cluster.call, traceback, node, ".loginserver", "update_gamed_map", gamed_map)
				gamedlist_change_flag = false
			end
			if prev_balance_basic_seq ~= balance_basic_seq then
				xpcall(cluster.call, traceback, node, ".loginserver", "sync_balance_seq", balance_basic_seq)
				prev_balance_basic_seq = balance_basic_seq
			end
		end
	end
end

function handler.open(source, conf)
	redis_service = skynet.queryservice("redis_service")
	load_balance_config()
	load_gray_config()
	load_stop_config()

	skynet.timeout(heartbeat_check_time, heartbeat_check)
	skynet.timeout(20 * 100, notice_node_info)

	return true
end

function handler.connect(fd, addr)
	gateserver.openclient(fd)
end

----------------------心跳处理相关-------------------
local function reg_logind(name, logind, reg_flag)
	logger.trace("reg loginserver name:" .. name .. " logind:" .. logind)

	local instance = logind_map[name]
	if instance then
		if instance.addr ~= logind then
			logind_map[name].addr = logind
			logind_map[name].last_alive_time = math.floor(skynet.time())
		end
	else
		for k, v in ipairs(logind_list) do
			if v == logind then
				table.remove(logind_list, k)
			end
		end

		table.insert(logind_list, name)

		local stat = {addr = logind, last_alive_time = math.floor(skynet.time())}
		logind_map[name] = stat
	end

	logger.trace("login length: " .. #logind_list)
end

local function del_gamed(name)
	if not gamed_map[name] then
		return
	end

	for k,v in ipairs(gamed_list) do
		if name == v then
			table.remove(gamed_list, k)
		end
	end
	gamed_map[name] = nil

	gamedlist_change_flag = true
end

local function reg_gamed(name, gamed)
	if ban_gamed_map[name] then
		if gamed_map[name] then
			del_gamed(name)
		end
		return
	end

	logger.trace("reg gameserver name:" .. name .. " gamed:" .. gamed)

	local instance = gamed_map[name]
	if instance then
		if instance.addr ~= gamed then
			gamed_map[name].addr = gamed
			gamed_map[name].last_alive_time = math.floor(skynet.time())
		end
	else
		for k, v in ipairs(gamed_list) do
			if v == name then
				table.remove(gamed_list, k)
			end
		end

		table.insert(gamed_list, name)

		local stat = {addr = gamed, last_alive_time = math.floor(skynet.time())}
		gamed_map[name] = stat
		gamed_hash[gamed] = name
	end

	gamedlist_change_flag = true

	logger.trace("game legnth: " .. #gamed_list)
end

--集群配置文件clustename.lua 重载
local function cluster_reload()
	logger.trace("clustername reload !!!")
	cluster.reload()

	xpcall(cluster.call, traceback, "centernode", ".syncserver", "cluster_reload")

	if logind_list and #logind_list ~= 0 then
		for k, v in ipairs(logind_list) do
			local node = utils.query_nodename(v)
			print("logind_list ----- ", v)
			xpcall(cluster.call, traceback, node, ".loginserver", "cluster_reload")
		end
	end

	if gamed_list and #gamed_list ~= 0 then
		for k, v in ipairs(gamed_list) do
			local node = utils.query_nodename(v)
			print("gamed_list ----- ", v)
			xpcall(cluster.call, traceback, node, ".gamed", "cluster_reload")
		end
	end
end

---------------------------CMD 处理-------------------------------------
-- 配置logind的时候，需要保障每个logind的名字不一样
local CMD = {}
function CMD.reg_logind(name, logind)
	local reg_flag = true  --标明是第一次启动注册过来的
	reg_logind(name, logind, reg_flag)
end

function CMD.unreg_logind(name, logind)
	logind_map[name] = nil
	for k, v in ipairs(logind_list) do
		if v == logind then
			table.remove(logind_list, k)
		end
	end
end

function CMD.reg_gamed(name, gamed)
	--这个地方也要加上处理
	reg_gamed(name, gamed)
end

function CMD.unreg_gamed(name, gamed)
	gamed_map[name] = nil
	gamed_hash[gamed] = nil
	logger.trace("unreg gamed:" .. gamed)
	for k, v in ipairs(gamed_list) do
		if v == name then
			table.remove(gamed_list, k)
		end
	end

	gamedlist_change_flag = true
end

--获取当前存活的逻辑服
function CMD.get_all_gamed()
	return gamed_list
end

function CMD.get_all_gamed_map()
	return gamed_map
end

function CMD.heart(type, name, host)
	if type == "login" then
		if not logind_map[name] then
			reg_logind(name, host)
		else
			logind_map[name].last_alive_time = math.floor(skynet.time())
		end
	elseif type == "game" then
		if ban_gamed_map[name] then
			if gamed_map[name] then
				del_gamed(name)
			end
			return
		end
		if not gamed_map[name] then
			reg_gamed(name, host)
		else
			gamed_map[name].last_alive_time = math.floor(skynet.time())
		end
	end
end

--获取负载策略
function CMD.get_balance_schema()
	return balance_basic
end

--获取灰度策略
function CMD.get_gray_schema()
	return gray_basic
end

--获取停服策略
function CMD.get_stop_schema()

	return stop_basic
end

--比较策略文件版本号
function CMD.cmp_schema_seq(seq)
	if not balance_basic then return false end

	if seq <= balance_basic.seq then
		return false
	else
		return true
	end
end


function CMD.update_balance( config )
	update_balance_config(config)
end

function CMD.update_gray( config )
	update_gray_config(config)
end

function CMD.update_stop( config )
	update_stop_config(config)
end

--这里消息结构是:type, source, value
function handler.command(cmd, _, ...)
	local f = CMD[cmd]
	if f then
		return f(...)
	else
		logger.trace("gslbserver.lua:handler.command>invalid cmd ", cmd)
	end
end

--注册热更新函数到handler.hotfix节点上
handler.hotfix = hotfix
hotfix.update_schema = update_balance_config
hotfix.update_gray_schema = update_gray_config
hotfix.update_stop_schema = update_stop_config
hotfix.cluster_reload = cluster_reload
handler.dispatch = dispatch

--主启动函数
function gslbserver.start(handler)
	proto.load_proto()
	return gateserver.start(handler)
end

gslbserver.start(handler)

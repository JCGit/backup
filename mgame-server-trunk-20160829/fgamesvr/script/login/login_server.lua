local skynet     = require "skynet"
local netpack    = require "netpack"
local gateserver = require "snax.gateserver"
local cluster    = require "cluster"

local logger        = require "logger"
local proto         = require "proto"
local utils         = require "utils"
local COMMON        = require "common"
local socketdriver  = require "socketdriver"
local server_config = require "server_config"
local json          = require "cjson"

local loginserver = {}
local handler = {}
local platform
local node_name
local host
local balance_schema
local gamed_list
local gamed_map
local traceback = debug.traceback
local cur_balance_seq = 1
local white_update_flag = false
local gray_basic
local stop_basic
local release_basic --测试版本控制 用于版本提审

-- 保存fd -> key的字典
local fd_key_map = {}

local check_service_list = {}
local platform_service_list = {}

local check_service_count = server_config.logind_conf.check_service_count
local platform_service_count = server_config.logind_conf.platform_service_count  --platfrom的个数理论上是要大于login_check_service, http耗时比cluster.call要高

-- 向负载均衡服和中心服发送心跳
local heartbeat_time = 20 * 100
local function send_heartbeat()
    xpcall(cluster.call, traceback,
        "gslbnode", ".gslbserver", "heart", "login", node_name, host)
    skynet.timeout(heartbeat_time, send_heartbeat)
end

local function notice_schema_update(seq)
    local ok, schema = xpcall(cluster.call, traceback, "gslbnode", ".gslbserver", "get_balance_schema")
    cur_balance_seq = seq

    if ok and schema then
        balance_schema = schema
        for k,v in ipairs(check_service_list) do
            skynet.call(v, "lua", "update_balance_config", balance_schema)
        end
    end
end

local index = 1
local function get_check_service_agent()
   if index > check_service_count then
      index = 1
   end
   local agent = check_service_list[index]
   index = index + 1
   return agent
end

local platform_index = 1
local function get_platform_service()
    if platform_index > platform_service_count then
        platform_index = 1
    end
    local platform = platform_service_list[platform_index]
    platform_index = platform_index + 1
    return platform
end

local function init_release_basic()
    local filePath = "setting/release.json"
    local file = assert(io.open(filePath, "r"))
    local data = file:read("a")
    release_basic = json.decode(data)
    -- release_flag, gameserver, game_node , release_version
end

local function update_release_basic()
    local filePath = "setting/test_release.json"
    local file = assert(io.open(filePath, "r"))
    local data = file:read("a")
    release_basic = json.decode(data)
end

----------------------------------  handler接口 -----------------
function handler.message(fd, msg, sz)
    local agent = get_check_service_agent()
    local platform = get_platform_service()
    local bfkey = fd_key_map[fd]
    skynet.call(agent, "lua", "handle_login_check", fd, msg, sz, bfkey, platform)
end

function handler.connect(fd, addr)
    if string.match(addr,"115.159.19.104") and string.match(addr,"115.159.19.105") then
        logger.trace('connect: open client from ', addr)
    end
    gateserver.openclient(fd)
end

function handler.open(source, conf)
    logger.trace("login_server open", check_service_count)
    node_name = conf.servername
    host = conf.host .. ":" .. tostring(conf.port)
    xpcall(cluster.call, traceback,
        "gslbnode", ".gslbserver", "reg_logind", conf.servername, host)

    --发送心跳给负载均衡服
    skynet.timeout(heartbeat_time, send_heartbeat)

    --获取当前gamed节点存活信息
    gamed_map  = cluster.call("gslbnode", ".gslbserver", "get_all_gamed_map")

    --向中心服获取当前负载均衡的策略配置文件，生成相应的逻辑， 先不考虑这么负载的，先就考虑login_server带这段逻辑
    logger.trace(" balance schema  ")
    balance_schema = cluster.call("gslbnode", ".gslbserver", "get_balance_schema")
    if balance_schema then
        --读负载策略配置，解析成schema
--        logger.trace("get balance_schema ")
    end
  
    gray_basic = cluster.call("gslbnode", ".gslbserver", "get_gray_schema")
    if gray_basic then
        utils.var_dump(gray_basic)
    end

    stop_basic = cluster.call("gslbnode", ".gslbserver", "get_stop_schema")

    --读取测试版本控制配置
    init_release_basic()

    local me = skynet.self()
    while #check_service_list < check_service_count do
       local agent = skynet.newservice("login_check_service", me)
       table.insert(check_service_list, agent)
    end

    while #platform_service_list < platform_service_count do
        local platform_service = skynet.newservice("platform")
        table.insert(platform_service_list, platform_service)
    end

    logger.trace("Login Server open finish")
    return true
end

local CMD = {}
function CMD.update_fd_key(fd, key)
    fd_key_map[fd] = key
    return true
end

function CMD.get_schema_data()
    return balance_schema
end

function CMD.get_gray_schema()
    return gray_basic
end

function CMD.get_stop_schema()
    return stop_basic
end

function CMD.get_alive_gamed()
    return gamed_list
end

function CMD.get_alive_gamed_map()
    return gamed_map
end

function CMD.get_release_schema()
    return release_basic
end

function CMD.update_balance_config(balance_data)
    balance_schema = balance_data
   
    for k,v in ipairs(check_service_list) do
        skynet.call(v, "lua", "update_balance_config", balance_schema)
    end
end

function CMD.update_gamed_list(list)
    gamed_list = list
    for k,v in ipairs(check_service_list) do
        skynet.call(v, "lua", "update_gamed_list", gamed_list)
    end
end

function CMD.update_gamed_map(map)
    gamed_map = map

    for k,v in ipairs(check_service_list) do
        skynet.call(v, "lua", "update_gamed_map", gamed_map)
    end
end

--刷新版本发布配置
function CMD.update_release_schema()
    update_release_basic()
    for k,v in ipairs(check_service_list) do
        skynet.call(v, "lua", "update_release_schema", release_basic)
    end
end

-- 负载配置序列号比较
function CMD.sync_balance_seq(seq)
    -- 协调服的seq比当前的seq大表明需要更新当前的seq
    -- 协调服的seq比当前的seq小表明协调服有可能重启过，这时候需要去重新请求负载配置, 现在设置seq小于10(因为gslbserver重启seq总是从1开始)，就更新配置
    if seq > cur_balance_seq or  (seq < cur_balance_seq  and seq < 10) then
        notice_schema_update(seq)
    end
end

-- 同步灰度配置
function CMD.sync_gray_config(gray_data)
    gray_basic = gray_data
    for k, v in ipairs(check_service_list) do
        skynet.call(v, "lua", "sync_gray_config", gray_basic)
    end
end

-- 同步停服配置
function CMD.sync_stop_config(stop_data)
    stop_basic = stop_data
    if not stop_basic then return end

    for k, v in ipairs(check_service_list) do
        skynet.call(v, "lua", "sync_stop_config", stop_basic)
    end
end

-- clustername集群配置更新
function CMD.cluster_reload()
    logger.trace("login server clustername reload !!!")
    cluster.reload()
end

function handler.command(cmd, source, ...)
    local f = CMD[cmd]
    logger.trace("loginserver cmd: ", cmd)
    if f then
        return f(...)
    else
        logger.warningf("unknown message")
    end
end

-- 启动函数
gateserver.start(handler)

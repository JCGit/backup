--
-- User: Denny
-- Date: 2015/8/26
-- Time: 10:24

local skynet       = require "skynet"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name

local logger        = require "logger"
local proto         = require "proto"
local utils         = require "utils"
local COMMON        = require "common"
local server_config = require "server_config"

local gamecluster_list = {}
local gamed_map = {}

local redis_service

local heartbeat_overtime = 40  --秒
local heartbeat_check_time = 20 * 100 --毫秒
local function heartbeat_check()
    local now = math.floor(skynet.time())
    for k, v in pairs(gamed_map) do
        if now - v.last_alive_time > heartbeat_overtime then
            for key, value in ipairs(gamecluster_list) do
                if value == v.cluster then
                    table.remove(gamecluster_list, key)
                end
            end
            gamed_map[k] = nil
            --发送邮件报警
            local fmt = [[
echo 'sync server(%s) time out' | mutt -s "login server error" %s
]]
            os.execute(string.format(fmt, k, server_config.mail_conf.mails))
            logger.error(k, " is not live")
        end
    end

    skynet.timeout(heartbeat_check_time, heartbeat_check)
end

local function reg_gamed(name, gamed)
    logger.trace("reg gameserver name:" .. name .. " gamed:" .. gamed)

    local instance = gamed_map[name]
    if instance then
        if instance.addr ~= gamed then
            for k, v in ipairs(gamecluster_list) do
                if v == instance.cluster then
                    table.remove(gamecluster_list, k)
                    table.insert(gamecluster_list, instance.cluster)
                end
            end
            gamed_map[name].addr = gamed
            gamed_map[name].last_alive_time = math.floor(skynet.time())
        end
    else
        for k, v in ipairs(gamecluster_list) do
            if v == gamed then
                table.remove(gamecluster_list, k)
            end
        end

        local node_name = utils.query_nodename(name)
        table.insert(gamecluster_list, node_name)
  

        local stat = {addr = gamed, last_alive_time = math.floor(skynet.time()), cluster = node_name}
        gamed_map[name] = stat
    end

    logger.trace("gamecluster list legnth: " .. #gamecluster_list)
end

local CMD = {}
function CMD.reg_gamed(name, gamed)
    reg_gamed(name, gamed)
end

function CMD.unreg_gamed(name, gamed)
    gamed_map[name] = nil
    logger.trace("unreg gamed:" .. gamed)
    for k, v in ipairs(gamecluster_list) do
        if v.name == name then
            table.remove(gamecluster_list, k)
        end
    end
end

function CMD.get_all_gamecluster()
    return gamecluster_list
end

-- 获取一个随机的gamed
function CMD.get_random_gamecluster()
    if #gamecluster_list == 0 then
        return nil
    end
    return gamecluster_list[math.random(#gamecluster_list)]
end

-- 玩家上下线登记
-- center的用户在线数据保存在内存中，后续再考虑保存在redis中, 来一个存一个
local online_user = {}
function CMD.report_online(nodename, uin)
    online_user[uin] = nodename
    skynet.call(redis_service, "lua", "hset_obj", "uin_distribute", uin, nodename)
end

function CMD.report_offline(nodename, uin)
    online_user[uin] = nil
    skynet.call(redis_service, "lua", "hset_obj", "uin_distribute", uin, "0")
end

-- 支持离线用户位置查找，主要是针对支付回调，用户可能临时不在线的情况
function CMD.get_location(id)
    local data = skynet.call(redis_service, "lua", "hget_obj", "player_node", id)
    if data then
        local node_name = utils.query_nodename(data.servername)
        return node_name

    end
end

--支持在线用户的位置查找
function CMD.get_user_location(id)
    local data = skynet.call(redis_service, "lua", "hget_obj", "player_node", id)

    if data and data.isonline then
        local node_name = utils.query_nodename(data.servername)
        return node_name

    end
end

function CMD.heart(type, name, host)
    if type == "game" then
        if not gamed_map[name] then
            reg_gamed(name, host)
        else
            gamed_map[name].last_alive_time = math.floor(skynet.time())
        end
    end
end

--集群配置文件clustename.lua 重载
function CMD.cluster_reload()
    logger.trace("centerserver clustername reload !")
    cluster.reload()
end

skynet.start(function()
    redis_service = skynet.queryservice("redis_service")

    skynet.timeout(heartbeat_check_time, heartbeat_check)

    local traceback = debug.traceback

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            return skynet.ret()
        end
        return skynet.retpack(ret)
    end)
end)


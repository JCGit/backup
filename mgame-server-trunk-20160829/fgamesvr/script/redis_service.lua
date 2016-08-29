-- redis_service.lua
-- 数据库接口文件

local skynet = require "skynet"
local logger = require "logger"
local redis = require "redis"
local utils = require "utils"
local server_config = require "server_config"

local mysql_service
local id_db
local CMD = {}

local function mysql_set(table_name, key, value)
    local obj = {
        table_name = table_name,
        key = key,
        value = value,
    }
    skynet.call(mysql_service, "lua", "insert_queue", obj)
end

-- key  : id
-- value: lua obj
function CMD.set_obj(id, obj)
    local obj_str = skynet.packstring(obj)
    id_db:set(id, obj_str)
end

-- key      : id
-- 返回值   : lua obj
function CMD.get_obj(id)
    local obj_str = id_db:get(id)
    if obj_str then
        return skynet.unpack(obj_str)
    else
        return nil
    end
end

-- flag 表示是否从mysql删除 默认是删除的
function CMD.hdel_obj(table, id, flag)
    id_db:hdel(table, id)
    if not flag then
        skynet.call(mysql_service, "lua", "remove_obj", table, id)
    end
end

-- 移除整张表数据 不传表示要删除mysql表里的数据 如果不需要则[flag:true]
function CMD.del(table, flag)
    id_db:del(table)
    if not flag then
        skynet.call(mysql_service, "lua", "truncate_tab", table)
    end
end

-- flag 不传表示要写入mysql 如果不需要写入mysql[flag:true]
function CMD.hset_obj(table, id, obj, flag)
    local obj_str = skynet.packstring(obj)
    id_db:hset(table, id, obj_str)

    if not flag then
        mysql_set(table, id, obj)
    end
end

function CMD.hmset_obj(table, data )
    id_db:hmset(table, data)
end

function CMD.hreset_obj(table, ...)
    id_db:del(table)
    id_db:hmset(table, ...)
end

-- flag flag:true 不查数据库
function CMD.hget_obj(table, id, flag)
    local obj_str = id_db:hget(table, id)
    if obj_str then
        return skynet.unpack(obj_str)
    else
        if flag then
            return nil
        else
            return skynet.call(mysql_service, "lua", "query", table, id)
        end
    end
end

function CMD.hgetall_obj(table)
    local tmp = id_db:hgetall(table)
    local len = math.floor(#tmp / 2)
    local lists = {}
    for i = 1, len do
        lists[i] = skynet.unpack(tmp[i * 2])
    end
    tmp = nil
    return lists
end

-- flag true表示要将key转成number
function CMD.hgetall_all_value(table, flag)
    local tmp = id_db:hgetall(table)
    local len = #tmp / 2
    local lists = {}
    for i = 1, len do
        local key = tmp[i * 2 -1 ]
        if flag then key = tonumber(key) end
        lists[key] = skynet.unpack(tmp[i * 2])
    end
    tmp = nil
    return lists
end

function CMD.hlen(table)
    return id_db:hlen(table)
end

-- ID生成器
function CMD.id_handler(key)
    return id_db:incr(key)
end

-- 新增一组原生redis命令接口 Dec 29, 2015
function CMD.get(key)
    return id_db:get(key)
end

function CMD.set(key, field)
    return id_db:set(key, field)
end

function CMD.hgetall(key)
    return id_db:hgetall(key)
end

function CMD.hget(key, field)
    return id_db:hget(key, field)
end

function CMD.hset(key, field, value)
    return id_db:hset(key, field, value)
end

function CMD.eval(lua_script, numkeys, ...)
    return id_db:eval(lua_script, numkeys, ...)
end

function CMD.rename(key, new_key)
    return id_db:rename(key, new_key)
end

function CMD.exists(key)
    return id_db:exists(key)
end

function CMD.keys(t)
    return id_db:keys(t)
end

function CMD.type(key)
    return id_db:type(key)
end

-- 原生命令接口

local function id_init()
    local id_conf = server_config.id_conf
    for k, v in pairs(id_conf) do
        local id = id_db:get(k)
        if not id then
            id_db:set(k, v)
        end
    end
end

skynet.start(function()
    logger.trace("Redis start...")
    for i,node in pairs(server_config.redis_conf) do
        local db = redis.connect(node)
        if node.is_id_redis then
            id_db = db
        end
    end

    id_init()

    mysql_service = skynet.queryservice("mysql_service")

    local traceback = debug.traceback

    skynet.dispatch("lua", function(_, src, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s src:%d", command, ret, src)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)
end)
--数据打包方式转换服务
--服务器启动会把用lua格式编码的老数据转换成skynet二进制打包
--
--auth:diandian
--date:2016.4.1


local skynet = require "skynet"
local logger = require "logger"
local utils = require "utils"


local redis_service 
local redis_service_lua 

local function trans_hash_key(key)
    local tmp = skynet.call(redis_service_lua, "lua", "hgetall", key)
    local len = #tmp / 2
    local index = 0
    print("trans_hash_key len:", len, key)
    for i = 1, len do
        local k = tmp[i * 2 - 1]
        local dat = utils.unserialize_table(tmp[i * 2])
        skynet.call(redis_service, "lua", "hset_obj", key, k, dat)

        k = nil
        dat = nil
        index = index + 1
        if index >= 10000 then
            print("collectgarbage :", i)
            collectgarbage("collect")
            index = 0
        end
    end
    tmp = nil
    print("trans_hash_key success", key)
end

local function trans_string_key(key)
    local dat = skynet.call(redis_service_lua, "lua", "get_obj", key)
    skynet.call(redis_service, "lua", "set_obj", key, dat)
    print("trans_string_key success", key)
end

local function init()

    local nontrans = {
        account_max_id = 1,
        max_global_mail_id = 1,
        max_single_mail_id = 1,
    }

    local keys = skynet.call(redis_service, "lua", "keys", "*")
    for i, key in pairs(keys) do
        if not nontrans[key] then
            local ktype = skynet.call(redis_service, "lua", "type", key)
            if ktype == "hash" then
                trans_hash_key(key)
            elseif ktype == "string" then
                trans_string_key(key)
            end
        end
        --print(skynet.call(redis_service, "lua", "type", key), key)
    end

    print("trans pack way success:...")
end

local CMD = {}


skynet.start(function()
    logger.trace("trans_pack_service start...")

    redis_service = skynet.queryservice("redis_service")
    redis_service_lua = skynet.uniqueservice("redis_service_lua")

    init()

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

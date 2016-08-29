

local skynet = require "skynet"
local mysql = require "mysql"
local logger = require "logger"
local utils = require "utils"
local server_config = require "server_config"


local db
local redis


local function insert_hash_key(key)
    local tmp = skynet.call(redis, "lua", "hgetall", key)
    local len = #tmp / 2
    local index = 0
    logger.trace("insert hash key len:", len, key)
    for i = 1, len do
        local k = tmp[i * 2 - 1]
        local ret, dat = pcall(skynet.unpack, tmp[i * 2])
        if ret then
            local sql = "replace into " .. key .. " values('" .. k .. "', '" .. utils.serialize_table(dat) .. "');"
            mysql.query(db, sql)
            
            k = nil
            dat = nil
            index = index + 1
            if index >= 10000 then
                logger.trace("collectgarbage :", i)
                collectgarbage("collect")
                index = 0
            end
        end
    end
    logger.trace("insert hash key success", key)
end


local function init()
    db = mysql.connect(server_config.mysql_conf)
    if not db then
        logger.trace("mysql connect error")
        return
    end

    redis = skynet.uniqueservice("redis_service")

    local keys = skynet.call(redis, "lua", "keys", "*")
    for i, key in pairs(keys) do
        local ktype = skynet.call(redis, "lua", "type", key)
        if ktype == "hash" and key == "account" then
            insert_hash_key(key)
        end
    end
end



skynet.start(function()
    

    init()

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)
end)

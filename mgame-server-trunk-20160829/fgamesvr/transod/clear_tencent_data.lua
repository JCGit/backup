--腾讯服上线前数据清楚准备

-- auth:diandian
-- date:2016.4.26

local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local mysql = require "mysql"
local redis = require "redis"

local mysql_db
local redis_db

local mysql_cfg = {
    host            = "127.0.0.1",--"10.66.140.49",
    port            = 3306,
    user            = "root",
    password        = "root",--"MeiweiZZ0321",
    database        = "qq_fgame",
    max_packet_size = 1024*1024,
}

local redis_cfg = {
    is_id_redis = true,
    id          = 1,
    name        = "redis",
    host        = "127.0.0.1",--"10.66.125.63",
    port        = 6379 ,
    db          = 0,
    --auth        = "crs-8vg42ila:MeiweiZZ0321",
}

local keep_keys = {
    unique_code_factory = 1,
    code_type_max_id = 1,
    gift_code = 1,
    gift_type = 1,
}


local function init_mysql_db()
    mysql_db = mysql.connect(mysql_cfg)
    assert(mysql_db, "mysql_db is not connect!")
end

local function init_redis_db()
    redis_db = redis.connect(redis_cfg)
    assert(redis_db, "redis_db is not connect!")
end

local function call_redis(opt, ...)
    return redis_db[opt](redis_db, ...)
end

local function clear_mysql(key)
end

local function clear_keys()
    local allkeys = call_redis("keys", "*")
    for i, key in pairs(allkeys) do
        if not keep_keys[key] then
            call_redis("del", key)
            local sql = "truncate table " .. key
            mysql.query(mysql_db, sql)
            
            print("clear key", key)
        end
    end
end

local function clear_keys_for_platform()
    local allkeys = call_redis("keys", "*")
    for i, key in pairs(allkeys) do
        local platform, k = string.match(key, "(%w+):(%g+)")
        print(platform, k)
        if platform and keep_keys[k] then
            if platform == "weixin" then
                call_redis("rename", key, k)
                call_redis("move", k, 1)
            end
        else
            call_redis("del", key, true)
            clear_mysql(k, platform)
        end
    end
    
    local allkeys = call_redis("keys", "*")
    for i, key in pairs(allkeys) do
        local platform, k = string.match(key, "(%w+):(%g+)")
        if platform and platform == "qq" then
            call_redis("rename", key, k)
        end
    end
end

local function init()
    init_mysql_db()
    init_redis_db()
    clear_keys()
end

local CMD = {}

skynet.start(function()
    logger.trace("clear_tencent_data start...")

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
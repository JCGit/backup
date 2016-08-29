local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local mysql = require "mysql"
local redis = require "redis"
local cjson = require "cjson"

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

local save_keys = {
}
save_keys["gift_type"] = 1


local function init_mysql_db()
    mysql_db = mysql.connect(mysql_cfg)
    assert(mysql_db, "mysql_db is not connect!")
end

local function init_redis_db()
    redis_db = redis.connect(redis_cfg)
    assert(redis_db, "redis_db is not connect!")
end

local function flush_file_data_to_db()
    local str = io.input("./transod/db_data_qq.json"):read("a")
    local dat = cjson.decode(str)
    for key, v in pairs(dat) do
        if type(v) == "string" then
            redis_db:set(key, v)
        elseif type(v) == "table" then
            for hk, hv in pairs(v) do
                if type(hv) == "string" then
                    redis_db:hset(key, hk, hv)
                    local sql = "replace into " .. key .. " values('" .. hk .. "', '" .. hv .. "');";
                    mysql.query(mysql_db, sql)
                end
            end
        end
    end
end

local function init()
    init_redis_db()
    init_mysql_db()
    flush_file_data_to_db()
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
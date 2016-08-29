
local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local redis = require "redis"

local db_qq
local db_wx

local db_cfg_qq = {
    is_id_redis = true,
    id          = 1,
    name        = "redis",
    host        = "127.0.0.1",--"10.66.125.63",
    port        = 6379 ,
    db          = 0,
    --auth        = "crs-8vg42ila:MeiweiZZ0321",
}

local db_cfg_wx = {
    is_id_redis = true,
    id          = 1,
    name        = "redis",
    host        = "127.0.0.1",--"10.66.125.63",
    port        = 6379 ,
    db          = 1,
    --auth        = "crs-8vg42ila:MeiweiZZ0321",
}

local redis_dv = {}

function redis_dv.init()
    db_qq = redis.connect(db_cfg_qq)
    assert(db_qq, "db_qq is not connect!")
    db_wx = redis.connect(db_cfg_wx)
    assert(db_wx, "db_wx is not connect!")
    
    print("db conected ........")
    return db_qq, db_wx
end

return redis_dv
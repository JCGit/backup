
local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local redis_dv = require "redis_dv"

local db_qq
local db_wx

local function start_merge()
    local tmp = db_wx:hgetall("gift_code")
    local len = #tmp / 2
    for i = 1, len do
        local k = tmp[i * 2 - 1]
        local dat = tmp[i * 2]
        db_qq:hset("gift_code", k, dat)
    end
    print("gift code merge complite:", len)
    
end

local function del_db_info()

    print("del gift_code and concat's tables ..")
    db_qq:del("gift_code")
    db_qq:del("player_gift_code")
    db_qq:del("code_type_max_id")
    db_qq:del("gift_type")
    db_qq:del("unique_code_factory")
end

skynet.start(function()
    logger.trace("merge_gift_code_svr start ...")

    db_qq , db_wx = redis_dv.init();
    --start_merge()
    del_db_info()

    local traceback = debug.traceback
end)
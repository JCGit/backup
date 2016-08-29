
local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local redis_dv = require "redis_dv"

local db_qq
local db_wx

local wx_uin_offset


local function start_merge()
    local tmp = db_wx:hgetall("uin_mapping")
    local len = #tmp / 2
    local index = 0
    for i = 1, len do
        local k = tmp[i * 2 - 1]
        local dat = skynet.unpack(tmp[i * 2])
        dat = dat + wx_uin_offset
        
        db_qq:hset("uin_mapping", k, skynet.packstring(dat))
    end
    print("merge_uin_mapping complite:", len)
    
end

skynet.start(function()
    logger.trace("merge_uin_mapping start ...")

    db_qq , db_wx = redis_dv.init()
    wx_uin_offset = tonumber(db_qq:get("account_max_id")) - 10000000
    start_merge()
    local traceback = debug.traceback
end)
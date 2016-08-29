
local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local redis_dv = require "redis_dv"

local db_qq
local db_wx

local wx_uin_offset


local function start_merge()
    local tmp = db_wx:hgetall("friend_info")
    local len = #tmp / 2
    local index = 0
    for i = 1, len do
        local k = tonumber(tmp[i * 2 - 1])
        local dat = skynet.unpack(tmp[i * 2])
        k = k + wx_uin_offset
        dat.uin = k
        
        dat.pk = {}
        dat.requests = {}
        dat.black_list = {}
        
        if next(dat.friends) then
            local friends = {}
            for uin, _ in pairs(dat.friends) do
                uin = uin + wx_uin_offset
                friends[uin] = {}
            end
            dat.friends = friends
        end
        
        db_qq:hset("friend_info", k, skynet.packstring(dat))
    end
    print("merge_friend_svr complite:", len)
    
end

skynet.start(function()
    logger.trace("merge_friend_svr start ...")

    db_qq , db_wx = redis_dv.init()
    wx_uin_offset = tonumber(db_qq:get("account_max_id")) - 10000000
    start_merge()
    local traceback = debug.traceback
end)
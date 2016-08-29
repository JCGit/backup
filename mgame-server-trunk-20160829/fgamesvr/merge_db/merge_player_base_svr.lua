
local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local redis_dv = require "redis_dv"

local db_qq
local db_wx

local wx_uin_offset


local function start_merge()
    local tmp = db_wx:hgetall("account")
    local len = #tmp / 2
    for i = 1, len do
        local k = tonumber(tmp[i * 2 - 1])
        k = k + wx_uin_offset
        local dat = skynet.unpack(tmp[i * 2])
        dat.uin = k
        
        local qq_name_exist = db_qq:hget("player_name", dat.name)
        if qq_name_exist then
            dat.name = tostring(dat.uin)
            dat.change_name_count = 0
        else
            db_qq:hset("player_name", dat.name, skynet.packstring({ value=k, }))
        end
        
        db_qq:hset("account", k, skynet.packstring(dat))
    end
    
    
    local tmp = db_wx:hgetall("activity_info")
    local len = #tmp / 2
    for i = 1, len do
        local uin = tonumber(tmp[i * 2 - 1])
        uin = uin + wx_uin_offset
        local dat = skynet.unpack(tmp[i * 2])
        local ndat = {}
        for k, v in pairs(dat) do
            local u, id, stage = string.match(k, "(%d+)_(%d+)_(%d+)")
            u = uin
            local nk = tostring(u) .. "_" .. id .. "_" .. stage
            ndat[nk] = v
        end
        db_qq:hset("activity_info", uin, skynet.packstring(ndat))
    end
    
    print("merge_player_base_svr complite:", len)
    
end

skynet.start(function()
    logger.trace("merge_player_base_svr start ...")

    db_qq , db_wx = redis_dv.init()
    wx_uin_offset = tonumber(db_qq:get("account_max_id")) - 10000000
    start_merge()
    local traceback = debug.traceback
end)

local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local redis_dv = require "redis_dv"

local db_qq
local db_wx


local trans_map = {
    rank = 1,
    xinge = 1,
    activity = 1,
    pvp_chest = 1,
    skin = 1,
    pvp_pay_guide = 1,
    achievement_info = 1,
    item = 1,
    recharge_info = 1,
    glyphData = 1,
    stuffData = 1,
    institute_info = 1,
    power_energy_activity = 1,
    seven_goal = 1,
    player_titles = 1,
    mission = 1,
    daily_task = 1,
    achievement = 1,
    exchange_info = 1,
    newbie_list = 1,
    player_recharge_gift = 1,
    rank_week_reward_recording = 1,
    rank_week_reward_recorded = 1,
    offline_recharge_order = 1,
    check_points = 1,
    seven_day_sign = 1,
    activity_crazy = 1,
    assistant_info = 1,
    activity = 1,
}

local function start_merge()
    for tb, _ in pairs(trans_map) do
        local tmp = db_wx:hgetall(tb)
        local len = #tmp / 2
        local index = 0
        for i = 1, len do
            local k = tonumber(tmp[i * 2 - 1])
            k = k + wx_uin_offset
            local dat = skynet.unpack(tmp[i * 2])
            if dat.uin then
                dat.uin = k
            end
            db_qq:hset(tb, k, skynet.packstring(dat))
        end
        print("merge db with uin success ", tb)
        collectgarbage("collect")
    end
    
    local wx_max_id = db_wx:get("account_max_id")
    wx_max_id = tonumber(wx_max_id) - 10000000
    local qq_max_id = db_qq:get("account_max_id")
    qq_max_id = tonumber(qq_max_id)
    qq_max_id = qq_max_id + wx_max_id
    db_qq:set("account_max_id", qq_max_id)
    
 

    print("merge db with uin complite..")
    
end

skynet.start(function()
    logger.trace("merge db with uin start ...")
    db_qq , db_wx = redis_dv.init()
    wx_uin_offset = tonumber(db_qq:get("account_max_id")) - 10000000
    
    start_merge()
end)
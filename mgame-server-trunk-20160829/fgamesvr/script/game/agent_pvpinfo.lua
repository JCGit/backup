-- 玩家pvp信息处理
-- auth:diandian
-- date:20160623


local utils = require "utils"
local skynet = require "skynet"
local COMMON = require "common"
local sharedata_utils = require "sharedata_utils"
local agent_pay_guide = require "agent_pay_guide"

local end_type = COMMON.pvp_end_type
local room_flag = COMMON.room_flag


local redis_service
local account_info

--最大连胜荣誉奖励
local cfg_max_streak_win_score = 0
--最大终结连胜奖励
local cfg_max_streak_killwin_score = 0

--最小可获荣誉连胜数
local cfg_min_streakwin_count = 1000000
--最小可获荣誉终结连胜数
local cfg_min_streakkillwin_count = 1000000

local info                   = {
    t_win_count              = 0, --pvp胜利总场次
    t_lose_count             = 0, --pvp失败总场次
    t_draw_count             = 0, --pvp平局总场次
    t_streak_win_count       = 0, --pvp总连胜
    t_daily_streak_win_count = 0, --pvp每日总连胜

    r_win_count              = 0, --pvp随机匹配胜利场次
    r_lose_count             = 0, --pvp随机匹配失败场次
    r_draw_count             = 0, --pvp随机匹配平局总场次
    r_streak_win_count       = 0, --pvp随机匹配当前总连胜
    r_daily_streak_win_count = 0, --pvp随机匹配每日连胜

    n_win_count              = 0, --pvp无道具场胜利场次
    n_lose_count             = 0, --pvp无道具场失败场次
    n_streak_win_count       = 0, --pvp无道具场总连胜
    n_daily_streak_win_count = 0, --pvp无道具场每日连胜
    n_draw_count             = 0, --pvp无道具场平局场次

    m_win_count  = 0, --pvp开房胜利场次
    m_lose_count = 0, --pvp开房失败场次
}

local info_change_flag = false

local agent_pvpinfo = {}

local function beadd(pn)
    info[pn] = info[pn] + 1
end

local function bezero(pn)
    assert(info[pn], pn)
    info[pn] = 0
end

function agent_pvpinfo.pvp_end(room_flag_, end_type_)

    if end_type_ == end_type.WIN then
        beadd("t_win_count")
        beadd("t_streak_win_count")
        beadd("t_daily_streak_win_count")
    else
        bezero("t_streak_win_count")
        bezero("t_daily_streak_win_count")
        if end_type_ == end_type.LOSE then
            beadd("t_lose_count")
        else
            beadd("t_draw_count")
        end
    end

    if room_flag == room_flag.RAND then
        if end_type_ == end_type.WIN then
            beadd("r_win_count")
            beadd("r_streak_win_count")
            beadd("r_daily_streak_win_count")
        else
            --是否有连胜保护盾
            local isProtected = agent_pay_guide.isShieldProtected()
            if not isProtected then
                bezero("r_streak_win_count")
                bezero("r_daily_streak_win_count")
            end
            if end_type_ == end_type.LOSE then
                beadd("r_lose_count")
            else
                beadd("r_draw_count")
            end
        end
    end

    info_change_flag = true
end

local function event_check_save_db()
    if info_change_flag then
        info_change_flag = false
        skynet.call(redis_service, "lua", "hset_obj", account_info.uin, info)
    end
end

local function event_daily_check(event, notice_flag)
    bezero("t_daily_streak_win_count")
    bezero("r_daily_streak_win_count")
    bezero("n_daily_streak_win_count")
    info_change_flag = true
end

local function init_streak_win_score_cfg()
    local allcfg = sharedata_utils.get_stat_all(COMMON.plan.Winner)
    for k, v in pairs(allcfg) do
        if v.streak_win_score > 0 then
            if k < cfg_min_streakwin_count then
                cfg_min_streakwin_count = k
            end
            if v.streak_win_score > cfg_max_streak_win_score then
                cfg_max_streak_win_score = v.streak_win_score
            end
        end

        if v.streak_killwin_score > 0 then
            if k < cfg_min_streakkillwin_count then
                cfg_min_streakkillwin_count = k
            end
            if v.streak_killwin_score > cfg_max_streak_killwin_score then
                cfg_max_streak_killwin_score = v.streak_killwin_score
            end
        end
    end
end

function agent_pvpinfo.init(args)
    redis_service   = skynet.queryservice("redis_service")
    account_info = args.info

    local db_info = skynet.call(redis_service,
        "lua", "hget_obj", "pvpinfo", args.uin)

    if db_info then
        for k, v in pairs(db_info) do
            info[k] = v
        end
    end

    init_streak_win_score_cfg()
    EventCenter.addEventListener(Events.DAY_CHANGE, event_daily_check)
    EventCenter.addEventListener(Events.CHECK_SAVE_DB, event_check_save_db)
end

function agent_pvpinfo.rand_streak_win_score_reward(count)
    if count < cfg_min_streakwin_count then
        return 0
    end
    local cfg = sharedata_utils.get_stat(COMMON.plan.Winner,
        count)
    if cfg then
        return cfg.streak_win_score
    else
        return cfg_max_streak_win_score
    end
end

function agent_pvpinfo.rand_streak_killwin_score_reward(count)
    if count < cfg_min_streakkillwin_count then
        return 0
    end
    local cfg = sharedata_utils.get_stat(COMMON.plan.Winner,
        count)
    if cfg then
        return cfg.streak_killwin_score
    else
        return cfg_max_streak_killwin_score
    end
end

function agent_pvpinfo.get_info()
    return info
end

return agent_pvpinfo

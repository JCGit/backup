--
--成就系统模块
--
--auth:diandian
--date:2016.3.2
--
--数据说明：
--成就系统数据分为三块
--一：成就进度完成情况，针对每个类型的成就存一个data
--      表名：achievement_stats
--      字段：
--          progress : int // 进度
--          rewadeds : hashmap //已领取成就id的map
--
--二：成就总点数和等级情况
--      表名：achievement_data 
--      字段：
--          point : int //成就总点数
--          level : int //成就等级
--
--三：全服玩家成就领取情况
--      表名：achievement_reward_info 
--      字段：
--          id : int //成就id
--          count : init //全服玩家领取总数


local skynet = require "skynet"
local utils = require "utils"
local sharedata_utils = require "sharedata_utils"
local COMMON = require "common"
local proto = require "proto"

local args


local achievement_stats
local achievement_data
local achievement_reward_info
local stats_change_flag = false
local types = COMMON.achievement_type


local function event_pvp_end(room_flag, room_type, pvp_end_type, addscore, addpoint)
    if args.info.pvp_max_point > 0 then
        update_achievement_progress(types.PVP_MAX_POINT, args.info.pvp_max_point)
    end

    if pvp_end_type == COMMON.pvp_end_type.WIN then
        update_achievement_progress(types.PVP_WIN_COUNT, args.info.pvp_win_count)
    end

    if room_flag == COMMON.room_flag.RAND then
        update_achievement_progress(types.PVP_RANDOM_COUNT, args.info.rand_room_count)
    elseif room_flag == COMMON.room_flag.FIXED then
        update_achievement_progress(types.PVP_ROOM_COUNT, args.info.fixed_room_count)
    elseif room_flag == COMMON.room_flag.NOPROP then
        update_achievement_progress(types.PVP_NON_PROPS_COUNT, args.info.pvp_nopro_count)
    end
end

local function event_pve_end(new_flag, stars, cp_info)
    if cp_info.ChapterId == 1 then
    
    elseif cp_info.ChapterId == 2 then

    elseif cp_info.ChapterId == 3 then
    end
end

local function event_daily_task_complete()
    add_achievement_progress(types.DAILY_TASK_COUNT, 1)
end

local function init_data()
    achievement_stats = args.rediscall("hget_obj", "achievement_stats", args.info.uin) or {}
    achievement_data = args.rediscall("hget_obj", "achievement_data", args.info.uin) or {point = 0, level = 0,}
    achievement_reward_info = args.rediscall("hget_obj", "achievement_reward_info", args.info.uin) or {}
end

--添加成就类型进度
local function add_achievement_progress(type, value)
    if not achievement_stats[type] then
        achievement_stats[type] = {
            progress = 0,
        }
    end
    local data = achievement_stats[type]
    data.progress = data.progress + value
    stats_change_flag = true

    local resp_data = {
        type =  type,
        CompleteCount = data.progress,
    }
    args.send_response(COMMON.MESSAGE.CS_CMD_UPDATE_ACHIEVEMENT, resp_data)
end

local function update_achievement_progress(type, value)
    if not achievement_stats[type] then
        achievement_stats[type] = {
            progress = 0,
            rewadeds = {},
        }
    end
    local data = achievement_stats[type]
    data.progress = value
    stats_change_flag = true

    local resp_data = {
        type =  type,
        CompleteCount = data.progress,
    }
    args.send_response(COMMON.MESSAGE.CS_CMD_UPDATE_ACHIEVEMENT, resp_data)
end


--获取所有类型的成就数据
local function handle_get_achievement(cmd, content, len)
    local infos = {}
    for i, v in pairs(achievement_stats) do
        local info = {}
        info.Type = i
        info.Rewarded_list = v.rewadeds
        info.CompleteCount = v.progress
        table.insert(infos, info)
    end
    local resp_data = {
         Data = infos,
         AchievementPointCount = achievement_data.point,
         AchievementLevel = achievement_data.level,
    }
    args.send_response(COMMON.MESSAGE.CS_CMD_GET_ACHIEVEMENT, resp_data)
end

--获取单个成就奖励
local function handle_receive_achievement_point(cmd, content, len)
    local id = proto.parse_proto_req(cmd, content, len)

    local resp_data = {
        ret = 0,
        AchievementId = id,
    }
    local config = sharedata_utils.get_stat(COMMON.plan.AchievementAward, id)
    if config then
        local type = config.type
        local stat = achievement_stats[type]
        if stat and stat.progress >= config.Condition then
            if not stat.rewadeds[id] then
                stat.rewadeds[id] = 1
                stats_change_flag = true
                achievement_data.point = achievement_data.point + config.Award
                args.rediscall("hset_obj", "achievement_data", args.info.uin, achievement_data)
                local record_info = args.rediscall("hget_obj", "achievement_record_info", id)
                if not record_info then
                    record_info = {
                        id = id,
                        count = 0,
                    }
                end
                record_info.count = record_info.count + 1
                args.rediscall("hset_obj", "achievement_record_info", id, record_info)
            else
                resp_data.ret = 3
            end
        else
            resp_data.ret = 2
        end
    else
        resp_data.ret = 1
    end

    args.send_response(cmd, resp_data)
end

--领取成就等级奖励
local function handle_receive_achievement_reward(cmd, content, len)
    local level = proto.parse_proto_req(cmd, content, len)

end

local agent_achievement = {}
agent_achievement.types = types

function agent_achievement.save_db_check()
    if stats_change_flag then
        stats_change_flag = false
        args.rediscall("hset_obj", "achievement", args.info.uin, achievement)
    end
end

function agent_achievement.init(args_)
    args = args_


    args.cmd_map[COMMON.MESSAGE.CS_CMD_GET_ACHIEVEMENT] = handle_get_achievement
    args.cmd_map[COMMON.MESSAGE.CS_CMD_RECEIVE_ACHIEVEMENT] = handle_receive_achievement_point
    args.cmd_map[COMMON.MESSAGE.CS_CMD_RECEIVE_ACHIEVEMENT_REWARD] = handle_receive_achievement_reward

    init_data()

    --EventCenter.addEventListener(Events.PVP_END, event_pvp_end)
end


return agent_achievement

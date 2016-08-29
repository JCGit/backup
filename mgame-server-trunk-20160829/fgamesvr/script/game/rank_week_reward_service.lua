
--排行榜周奖励计算服务
--
--auth : diandian
--date : 2016.2.17
--

local skynet = require "skynet"
local COMMON = require "common"
local utils = require "utils"
local sharedata_utils = require "sharedata_utils"
local logger = require "logger"
local cluster = require "cluster"

local redis_service
local rank_service_v2
local activitys_service

local reward_config

local function get_recording_end()
    local lua_script = [[
        local once_times = ARGV[1]
        local redis_times = redis.call("get", "rank_week_reward_recorded_times")
        if not redis_times or redis_times ~= once_times then
            redis_times = redis_times and redis_times + 1 or 1
            redis.call("set", "rank_week_reward_recorded_times", redis_times)
            return 1
        else
            return 0
        end
    ]]
    return skynet.call(redis_service, "lua", "eval", lua_script, 1, "rank_week_reward_recorded_times", once_times)
end

local function count_rank_for_recorded()
    local all_info = skynet.call(redis_service, "lua", "hgetall_obj", "rank_week_reward_recorded")
    if all_info then
        for i, v in ipairs(all_info) do
            if v.pvp_count >= 20 then
                local rank, group = skynet.call(rank_service_v2, "lua", "get_rank", v.uin)
                v.pvp_group = group
                v.pvp_rank = rank
                skynet.call(redis_service, "lua", "hset_obj", "rank_week_reward_recorded", v.uin, v)
            end
        end
    end
end

local CMD = {}

function CMD.on_pvp_end(uin)
    local info = skynet.call(redis_service, "lua", "hget_obj", "rank_week_reward_recording", uin)
    if not info then
        info = {
            uin = uin,
            pvp_count = 0,
        }
    end
    info.pvp_count = info.pvp_count + 1
    skynet.call(redis_service, "lua", "hset_obj", "rank_week_reward_recording", uin, info)
end

function CMD.get_reward_by_rank(group, rank)
    local ranks_cfg = reward_config[group]
    if ranks_cfg then
        for i, v in pairs(ranks_cfg) do
            if rank >= v.min_rank and rank <= v.max_rank then
                return v.reward
            end
        end
    end
    return nil
end

function CMD.notify_act_open(act_info_)
    local fixed_info = {
        clock_time = {79200,},
        space_time = {},
    }
    skynet.call(activitys_service, "lua", "register_fixed", COMMON.activity_id.ACT_RANK_WEEK_AWARD, fixed_info)
end

function CMD.notify_act_update(act_info_, update_type)
    if update_type == COMMON.act_notify_update_type.CLOCK_TIME then
        local date = os.date("*t")
        logger.trace("date.wday == ", date.wday)
        if date.wday == 1 then --周日开始结算
            if get_recording_end() == 1 then
                logger.trace("rename db rank_week_reward_recording")
                local exists = skynet.call(redis_service, "lua", "exists", "rank_week_reward_recording")
                logger.trace("exists", exists)
                if exists then
                    skynet.call(redis_service, "lua", "rename", "rank_week_reward_recording", "rank_week_reward_recorded")
                    --统计达成条件的玩家的排名
                    count_rank_for_recorded()
                end
            end
            once_times = once_times + 1
        end
    end
end

function CMD.notify_act_end()
    skynet.call(redis_service, "lua", "del", "rank_week_reward_recorded")
    skynet.call(redis_service, "lua", "del", "rank_week_reward_recording")
    skynet.call(redis_service, "lua", "del", "rank_week_reward_recorded_times")
end

local traceback = debug.traceback
skynet.start(function()

    redis_service = skynet.queryservice("redis_service")
    rank_service_v2 = skynet.queryservice("rank_service_v2")
    activitys_service = skynet.queryservice("activitys_service")

    once_times = skynet.call(redis_service, "lua", "get", "rank_week_reward_recorded_times") or 0
    once_times = tonumber(once_times) + 1

    local plan_data = sharedata_utils.get_stat_all(COMMON.plan.RankWeekReward)
    reward_config = {}
    for k, v in ipairs(plan_data) do
        if not reward_config[v.group] then
            reward_config[v.group] = {}
        end
         table.insert(reward_config[v.group], v)
    end

    --skynet.call(activitys_service, "lua", "clear_all_cache_time")
    --skynet.call(activitys_service, "lua", "add_act_open", COMMON.activity_id.ACT_RANK_WEEK_AWARD, os.time(), math.maxinteger)

    skynet.dispatch("lua", function(_, _, command, ...)
        logger.trace("command  : " , command)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)
end)

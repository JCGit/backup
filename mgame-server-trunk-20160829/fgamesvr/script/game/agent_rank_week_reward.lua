--排行榜周奖励处理
--
--auth : diandian
--date : 2016.2.17
--
local skynet = require "skynet"
local utils = require "utils"
local COMMON = require "common"
local proto = require "proto"
local player = require "player"
local logger = require "logger"

local rank_week_reward_service
local rank_service_v2

local account_info
local send_package, send_error_code, send_response
local redis_service

local function event_pvp_end(room_flag, room_type, pvp_end, pvp_end_type, addscore, addpoint)
    skynet.call(rank_week_reward_service, "lua", "on_pvp_end", account_info.uin)
end

local function handle_info(cmd, content, len)
    local resp_data = {
        Status = false,
    }

    local record_info = skynet.call(redis_service, "lua", "hget_obj", "rank_week_reward_recorded", account_info.uin)
    if record_info and record_info.pvp_count >= 5 then
        resp_data.Status = true
    else
        record_info = skynet.call(redis_service, "lua", "hget_obj", "rank_week_reward_recording", account_info.uin)
        if not record_info then
            record_info = {
                    pvp_count = 0,
            }
        end
        --if record_info.pvp_count < 5 then
            local rank, group = skynet.call(rank_service_v2, 
                "lua", "get_rank", account_info.uin)
            record_info.pvp_rank = rank
            record_info.pvp_group = group
        --end
    end

    resp_data.PvpCount = record_info.pvp_count
    resp_data.GroupId = record_info.pvp_group
    resp_data.Rank = record_info.pvp_rank

    --resp_data.Status = true
    --resp_data.PvpCount = 30
    --resp_data.GroupId = 2
    --resp_data.Rank = 10
    send_response(cmd, resp_data)
end

local function handle_reward(cmd, content, len)
    local resp_data = {
         Success = false,
    }
    local record_info = skynet.call(redis_service, "lua", "hget_obj", "rank_week_reward_recorded", account_info.uin)
    if not record_info then
        resp_data.Error = 1
    else
        skynet.call(redis_service, "lua", "hdel_obj", "rank_week_reward_recorded", account_info.uin)

        local reward_str = skynet.call(rank_week_reward_service, "lua", "get_reward_by_rank", record_info.pvp_group, record_info.pvp_rank)
        if reward_str then
            resp_data.Success = true
            logger.trace("pvp rank week reward_str", reward_str)
            player.add_gift_by_str(reward_str, account_info, COMMON.change_src.rank_week_reward)
        else
            resp_data.Success = false
            resp_data.Error = 2
        end
    end

    send_response(cmd, resp_data)
    handle_info(COMMON.MESSAGE.CS_CMD_RANK_WEEK_REWARD_INFO)
end

local agent_rank_week_reward = {
}

function agent_rank_week_reward.on_open()
end

function agent_rank_week_reward.on_end()
end

function agent_rank_week_reward.init(args)
    account_info = args.info
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code

    args.cmd_map[COMMON.MESSAGE.CS_CMD_RANK_WEEK_REWARD_INFO] = handle_info
    args.cmd_map[COMMON.MESSAGE.CS_CMD_RANK_WEEK_REWARD_REWARD] = handle_reward

    rank_week_reward_service = skynet.queryservice("rank_week_reward_service")
    rank_service_v2 = skynet.queryservice("rank_service_v2")
    redis_service = args.redis
    EventCenter.addEventListener(Events.PVP_END, event_pvp_end)
end

return agent_rank_week_reward

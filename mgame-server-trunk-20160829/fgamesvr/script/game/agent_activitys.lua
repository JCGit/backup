
--agent 的活动管理，
--处理接受活动开启或者关闭的消息并分发到相应活动
--
--各种的活动agent接受下列三个消息
--on_open 活动开启时调用
--on_end  活动结束时调用
--on_update 需要实时更新的活动在更新是调用
--
--以上三个消息是必须的，即便不做处理也要写上函数名
--
--auth : diandian
--date : 2016.01.12



local skynet                    = require "skynet"
local utils                     = require "utils"
local agent_greb_red_packet     = require "agent_greb_red_packet"
local activity_new_year            = require "activity_new_year"
local activity_valentine        = require "activity_valentine"
local agent_rank_week_reward    = require "agent_rank_week_reward"
local COMMON                    = require "common"

local activitys = {}
local is_init_complete = false
local activitys_service

local function register_act()
    local act_id = COMMON.activity_id
    activitys[act_id.ACT_NEWYEAR] = activity_new_year
    activitys[act_id.ACT_MONEY]     = agent_greb_red_packet
    activitys[act_id.ACT_LOVER]     = activity_valentine
    activitys[act_id.ACT_RANK_WEEK_AWARD]     = agent_rank_week_reward
end

local agent_activitys = {
}

function agent_activitys.init(args)
    register_act()

    agent_greb_red_packet.init(args)
    activity_new_year.init(args)
    activity_valentine.init(args)
    agent_rank_week_reward.init(args)

    activitys_service = skynet.queryservice("activitys_service")
    local activitys_info = skynet.call(activitys_service, "lua", "get_all_act_info")
    
    for k, v in pairs(activitys_info) do
        if v.is_opened then
            if activitys[k] ~= nil and not activitys[k].is_opened then
                activitys[k].is_opened = true
                activitys[k].on_open(v)
            end
        end
    end

    is_init_complete = true

    args.cmd.on_act_open = agent_activitys.on_act_open
    args.cmd.on_act_update = agent_activitys.on_act_update
    args.cmd.on_act_end = agent_activitys.on_act_end
end


function agent_activitys.on_act_open(act_infos)
    if is_init_complete == false then
        return
    end

    for i, v in ipairs(act_infos) do
        local act = activitys[v.ActivityID]
        if act and not act.is_opened then
            act.is_opened = true
            act.on_open(v)
        end
    end
end

function agent_activitys.on_act_update(act_infos)
    if is_init_complete == false then
        return
    end

    for i, v in ipairs(act_infos) do
        local act = activitys[v.act_info.ActivityID]
        if act and not act.is_opened then
            act.on_update(v)
        end
    end
end

function agent_activitys.on_act_end(act_infos)
    if is_init_complete == false then
        return
    end

    for i, v in ipairs(act_infos) do
        local act = activitys[v.ActivityID]
        if act and act.is_opened then
            act.on_end(v)
        end
    end
end

return agent_activitys

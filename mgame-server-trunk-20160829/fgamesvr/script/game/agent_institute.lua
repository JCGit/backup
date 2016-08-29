--
--美食研究院
--
--auth:diandian
--date:2016.3.7
--

local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local sharedata_utils = require "sharedata_utils"
local player = require "player"
local COMMON = require "common"
local proto = require "proto"
local agent_item = require "agent_item"
local logger = require "logger"
local agent_opt_act = require "agent_operate_activity"

local data_change_flag = false
local args
local datas
local busilog_service

local types = {
    SCORE_ONCE = 1,
    DIAMOND_ONCE = 2,
    DIAMOND_COUNT = 3,
}

local function daily_check(notice_flag)
    for k, v in pairs(datas) do
        v.free_count = 0
    end
    data_change_flag = true
end


local function init_data()
    datas = args.rediscall("hget_obj", "institute_info", args.info.uin)
    if not datas then
        datas = {}
        local config_all = sharedata_utils.get_stat_all(COMMON.plan.Institute)
        for k, v in pairs(config_all) do
            datas[k] = {
                free_count = 0, --当天免费抽取已使用次数
                last_free_time = 0, --上传免费抽取时间
                do_total_count = 0, --所有类型总抽取次数
            }
            if k == 3 then
                datas[k].last_free_time = os.time()
            end
        end
        data_change_flag = true
    end
end

local function make_once_reward(group_id, is_get_assistant)
    local reward_group = sharedata_utils.get_stat(
        COMMON.plan.Reward, group_id)
    if not reward_group then
        logger.trace("agent_institute reward group is not exist",
            args.info.uin, group_id)
        return {}
    end
    reward_group = utils.copy(reward_group, true)

    --检查是否已经获得精灵
    --获得将军的话就删掉将军的随机
    if is_get_assistant then
        for i, g in pairs(reward_group.rand_rewards) do
            local icfg = sharedata_utils.get_stat(
                COMMON.plan.Items, g.item_id)
            if icfg.b_type == COMMON.item_btype.ASSISTANT then
                reward_group.rand_rewards[i] = nil
            end
        end
    end

    local rand_rewards = reward_group.rand_rewards

    for id, reward in pairs(rand_rewards) do
        if not agent_item.check_can_add_item(reward.item_id) then
            logger.trace("check_can_add_item is false", reward.item_id)
            rand_rewards[id] = nil
        end
    end

    local fixed_rewards = reward_group.fixed_rewards
    for id, reward in pairs(fixed_rewards) do
        if not agent_item.check_can_add_item(reward.item_id) then
            fixed_rewards[id] = nil
        end
    end
    return utils.make_reward_v2(reward_group, 1)
end

--计算当前奖励
local function reward_to_player(id, info, isCost)
    local config = sharedata_utils.get_stat(COMMON.plan.Institute, id)
    local notice_ids = {}

    local src = COMMON.change_src.institute_1_1
    if id == 2 then
        if isCost == 0 then
            src = COMMON.change_src.institute_2_1
        else
            src = COMMON.change_src.institute_2
            agent_opt_act.inc_sum_call(1) 
        end
    elseif id == 3 then
        if isCost == 0 then
            src = COMMON.change_src.institute_3_1
        else
            src = COMMON.change_src.institute_3
            agent_opt_act.inc_sum_call(10) 
        end
    elseif id == 1 then
        if isCost == 0 then
            src = COMMON.change_src.institute_1_1
        else
            src = COMMON.change_src.institute_1
        end
    end

    --计算必得保底次数
    local must_reward_index
    for i, count in pairs(config.must_reward_count) do
        if info.do_total_count == tonumber(count) then
            must_reward_index = i
            break
        end
    end

    local must_reward
    if must_reward_index then
        must_reward = config.must_reward[must_reward_index]
    end
    --是否获得将军
    local is_get_assistant

    local fix_items, ran_items
    local ran_items_list = {}
    local fix_items_list = {}
    local extra_add_items = {}
    
    function reward_item_to_player(fix_items, ran_items)
        --固定奖励
        utils.combine_reward(fix_items_list, fix_items)
        for id, item in pairs(fix_items) do
            local ret, _, _, extras = agent_item.add_item(
                id, item.count, src, false)
            if ret then
                utils.combine_items(extra_add_items, extras)
                notice_ids[id] = 1
            end
        end

        --随机奖励
        table.insert(ran_items_list, ran_items)
        for id, item in pairs(ran_items) do
            local ret, item, cfg, extras = agent_item.add_item(
                id, item.count, src, false)
            if ret then
                if cfg.b_type == COMMON.item_btype.ASSISTANT then
                    is_get_assistant = true
                end
                utils.combine_items(extra_add_items, extras)
                notice_ids[id] = 1
            end
        end
    end
    
    local random_reward_count = config.reward_count
    --如果有保底次数奖励就执行一次保底奖励抽取
    if must_reward then
        random_reward_count = random_reward_count - 1
        fix_items, ran_items = make_once_reward(must_reward)
        reward_item_to_player(fix_items, ran_items)
    end

    for i = 1, random_reward_count do
        fix_items, ran_items = make_once_reward(
            config.random_reward, is_get_assistant)
        reward_item_to_player(fix_items, ran_items)
    end
    agent_item.notice_ids(notice_ids)
    skynet.send(busilog_service, "lua", "log_misc", 
        COMMON.busilog_misc_type.misc_institute, args.info.uin, id, isCost)
    return fix_items_list, ran_items_list, src, extra_add_items
end

local function reward_cost(id)
    local config = sharedata_utils.get_stat(COMMON.plan.Institute, id)

    if id == types.SCORE_ONCE then --荣誉
        if args.info.pvp_score >= config.cost then
            player.change_score(args.info, -config.cost, true,
                COMMON.change_src.institute)
            return true
        else
            return false
        end
    else
        if args.info.diamond >= config.cost then
            player.change_diamond(args.info,
                COMMON.change_src.institute, -config.cost, true)
            return true
        else
            return false
        end
    end

    return false
end

local function reward_to_player_for_cost(id, info)
    local ret = reward_cost(id)
    if ret then
       return reward_to_player(id, info, 1)
    else
        return nil
    end
end


local function handle_institute_info(cmd, content, len)
    local resp_data = {info = {},}
    for k, v in pairs(datas) do
        local info = {}
        info.mType = k

        local config = sharedata_utils.get_stat(COMMON.plan.Institute, k)
        info.SurpCount = config.free_count - v.free_count
        info.Cost = config.cost
        if v.free_count >= config.free_count then
            info.Countdown = 0
        else
            info.Countdown = v.last_free_time + config.free_cool_time
        end
        table.insert(resp_data.info, info)
    end
    args.send_response(cmd, resp_data)
end


local function handle_institute_buy(cmd, content, len)
    local resp_data = {
        Countdown = 0,
        SurpCount = 0,
    }

    local fix_items, ran_items, src, extra_add_items
    local msg = proto.parse_proto_req(cmd, content, len)
    local id = msg.mType
    local info = datas[id]
    if not info then
        logger.trace("institute buy type is error", id)
        return
    end
    local config = sharedata_utils.get_stat(COMMON.plan.Institute, id)
    if info.free_count < config.free_count then
        local now = os.time()
        if now - info.last_free_time >= config.free_cool_time then --免费抽取
            fix_items, ran_items, src, extra_add_items = 
                reward_to_player(id, info, 0)
            info.free_count = info.free_count + 1
            info.last_free_time = now
        else
            fix_items, ran_items, src, extra_add_items = 
                reward_to_player_for_cost(id, info)
        end
    else
        fix_items, ran_items, src, extra_add_items =
            reward_to_player_for_cost(id, info)
    end

    resp_data.Countdown = info.last_free_time + config.free_cool_time
    resp_data.SurpCount = config.free_count - info.free_count
    if resp_data.SurpCount == 0 then
        resp_data.Countdown = 0 
    end
    resp_data.ItemList = {}
    
    function add_resp_itemlist(id, count)
        local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
        local item_resp = {
                Id = id,
                Type = item_cfg.property_type,
                Count = count,
        }
        if item_cfg.is_virtual then
            item_resp.Id = item_cfg.virtual_real
        end
        table.insert(resp_data.ItemList, item_resp)
    end
    
    if fix_items and ran_items then
        info.do_total_count = info.do_total_count + 1
        data_change_flag = true
        for id, item in pairs(fix_items) do
            add_resp_itemlist(id, item.count)
        end

        for _, items in pairs(ran_items) do
            for id, item in pairs(items) do
                add_resp_itemlist(id, item.count)
            end
        end
        
        for id, item in pairs(extra_add_items) do
            add_resp_itemlist(id, item.count)
        end
    else
        logger.trace("institute buy reward item is nil:..", args.info.uin)
        return
    end
    
    args.send_response(cmd, resp_data)
end


local agent_institute = {}

function agent_institute.check_save_db()
    if data_change_flag then
        data_change_flag = false
        args.rediscall("hset_obj", "institute_info", args.info.uin, datas)
    end
end

function agent_institute.init(args_)
    args = args_

    args.cmd_map[COMMON.MESSAGE.CS_CMD_INSTITUTE_INFO] = handle_institute_info
    args.cmd_map[COMMON.MESSAGE.CS_CMD_INSTITUTE_BUY] = handle_institute_buy

    busilog_service = args.busilog_service

    if not datas then
        init_data()
    end

    EventCenter.addEventListener(Events.DAY_CHANGE, daily_check)
end

return agent_institute

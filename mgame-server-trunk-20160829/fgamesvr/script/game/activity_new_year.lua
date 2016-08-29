--
--元旦活动相关
-- user: diandian
-- Date : 2015.12.15
--
local skynet = require "skynet"
local COMMON = require "common"
local proto = require "proto"
local sharedata_utils = require "sharedata_utils"
local utils = require "utils"
local logger = require "logger"
local player = require "player"

local redis_service

local account_info
local new_year_info
local send_package, send_response
local act_info = nil

--活动id  写死
local cur_activity_id = 12

local TYPE_MAPPING = {
    PVP_RAND_TIMES                  = 1, --随机匹配pvp参与次数
    PVP_RAND_WIN_TIMES              = 2, --随机匹配pvp获胜次数
    PVP_RAND_STREAK_WIN_TIMES       = 3, --随机匹配pvp连胜次数
    PVP_ROOM_PVP_TIMES              = 4, --开房pvp次数
    PVP_ROOM_PVP_WIN_TIMES          = 5, --开房pvp胜利次数
    PVP_ROOM_PVP_STREAK_WIN_TIMES   = 6, --开房pvp连胜次数
    PVP_ADD_POINT                   = 7, --pvp累计获得积分
    PVP_NON_ITEM_TIMES              = 8, --无道具模式PVP参与次数
    PVP_NON_ITEM_WIN_TIMES          = 9, --无道具模式PVP获胜次数
}

--检查元旦活动是否开启
local function check_opened()
    if act_info then
        return act_info.is_opened
    end
    return false
end

local function create_task_list()
    local all_tasks = sharedata_utils.get_stat_all(COMMON.plan.NewYear)
    local task_id_list = {}
    for i, v in ipairs(all_tasks) do
        table.insert(task_id_list, v.id)
    end
    utils.upset_array(task_id_list)
    new_year_info.task_id_list = task_id_list
end

local function create_new_year_info()
    new_year_info = {
        cur_task_id = nil,
        task_id_list = nil,
        cur_task_complete_count = 0,
        is_complete_all_task = false,
    }
end

local function get_new_year_info()
    if new_year_info == nil then
        new_year_info = skynet.call(redis_service, "lua", "hget_obj", "new_year_info", account_info.uin)
    end
    return new_year_info
end

local function save_new_year_info()
    if new_year_info == nil then
        skynet.call(redis_service, "lua", "hdel_obj", "new_year_info", account_info.uin)
    else
        skynet.call(redis_service, "lua", "hset_obj", "new_year_info", account_info.uin, new_year_info)
    end
end

local function get_have_next_task()
    if new_year_info.task_id_list == nil or #new_year_info.task_id_list == 0 then
        return false
    end
    return true
end

local function get_next_task_id()
    if get_have_next_task() then
        return table.remove(new_year_info.task_id_list)
    end
    return 0
end

--根据当前元旦活动状态可以初始化活动
--并返回proto的请求元旦活动信息的返回数据
local function get_cur_task_info_resp_data()
    local resp_data = {
        startTime = 0,
        endTime = 0,
        conditionId = 0,
        completeCount = 0,
        curCount = 0,
        nextPacketTag = false,
        awardId = 0,
    }
    local cur_task = nil
    local opened = check_opened()
    print("new year is opened:", opened)
    if check_opened() then
        resp_data.startTime = act_info.start_time
        resp_data.endTime = act_info.end_time
        new_year_info = get_new_year_info()
        if new_year_info == nil then
            create_new_year_info()
        end
        if new_year_info.is_complete_all_task ~= true then
            local cur_task_id = new_year_info.cur_task_id
            if cur_task_id ~= nil then
                cur_task = sharedata_utils.get_stat(COMMON.plan.NewYear, cur_task_id)
            else
                if new_year_info.task_id_list == nil then
                    create_task_list()
                end
                cur_task_id = get_next_task_id()
                if cur_task_id ~= 0 then
                    cur_task = sharedata_utils.get_stat(COMMON.plan.NewYear, cur_task_id)
                    new_year_info.cur_task_id = cur_task_id
                end
                save_new_year_info()
            end
        end
    end
    if cur_task ~= nil then
        resp_data.conditionId = cur_task.name
        resp_data.curCount = new_year_info.cur_task_complete_count
        resp_data.completeCount = cur_task.value
        resp_data.nextPacketTag = true
        resp_data.awardId = cur_task.reward
    end
    return resp_data
end

--请求获取当前元旦活动信息
local function handle_new_year_get_info(cmd, content, len)
    check_opened()
    local resp_data = get_cur_task_info_resp_data()
    return send_response(cmd, resp_data)
end

--请求元旦活动领奖
local function handle_new_year_reward(cmd, content, len)
    local resp_data = { ret = 1 }
    if check_opened() and new_year_info ~=nil then
        local cur_task_id = new_year_info.cur_task_id
        if cur_task_id ~= nil then
            local cur_task = sharedata_utils.get_stat(COMMON.plan.NewYear, cur_task_id)
            if cur_task ~= nil and new_year_info.cur_task_complete_count >= cur_task.value then
                resp_data.ret = 0
                player.add_gift(account_info, cur_task.reward, COMMON.change_src.new_year)
            end
        end
    end
    if resp_data.ret == 0 then
        new_year_info.cur_task_id = get_next_task_id()
        new_year_info.cur_task_complete_count = 0
        if new_year_info.cur_task_id == nil then
            new_year_info.is_complete_all_task = true
        end
        resp_data.nextTask = get_cur_task_info_resp_data()
    end
    return send_response(cmd, resp_data)
end

--请求判断元旦活动是否可领奖
local function handle_get_cmp(cmd, content)
    if check_opened() and new_year_info ~= nil then
        local cur_task_id = new_year_info.new_year_task_id
        if cur_task_id ~= nil then
            local cur_task = sharedata_utils.get_stat(COMMON.plan.NewYear, cur_task_id)
            if cur_task ~= nil then
                if new_year_info.cur_task_complete_count >= cur_task.value then
                    local data = {}
                    send_response(cmd, data)
                end
            end
        end
    end
end

local function change_value(task_type, value)
    local cur_task_id = new_year_info.cur_task_id
    if cur_task_id == nil or cur_task_id == 0 then
        return
    end
    local cur_task = sharedata_utils.get_stat(COMMON.plan.NewYear, cur_task_id)
    if cur_task == nil or task_type ~= cur_task.type then
        return
    end
    if new_year_info.cur_task_complete_count >= cur_task.value then
        return
    end
    new_year_info.cur_task_complete_count = new_year_info.cur_task_complete_count + value
    if new_year_info.cur_task_complete_count > cur_task.value then
        new_year_info.cur_task_complete_count = cur_task.value
    end
    
    save_new_year_info()
    if new_year_info.cur_task_complete_count >= cur_task.value then
        local data = {}
        send_response(COMMON.MESSAGE.CS_CMD_NEWYEAR_CMP, data)
    end
end

local function set_value(task_type, value)
    local cur_task_id = new_year_info.cur_task_id
    if cur_task_id == nil or cur_task_id == 0 then
        return
    end
    local cur_task = sharedata_utils.get_stat(COMMON.plan.NewYear, cur_task_id)
    if cur_task == nil or task_type ~= cur_task.type then
        return
    end
    if new_year_info.cur_task_complete_count >= cur_task.value then
        return
    end
    new_year_info.cur_task_complete_count = value
    if new_year_info.cur_task_complete_count > cur_task.value then
        new_year_info.cur_task_complete_count = cur_task.value
    end
    save_new_year_info()
    if new_year_info.cur_task_complete_count >= cur_task.value then
        local data = {}
        send_response(COMMON.MESSAGE.CS_CMD_NEWYEAR_CMP, data)
    end
end

local agent_new_year = {}

function agent_new_year.get_open_activity(open_activity)
    print("new year is open", check_opened())
    if check_opened() then
        local item  = {
            Type = COMMON.activity_icon.ACT_NEWYEAR
        }
        table.insert(open_activity, item)
    end
end

function agent_new_year.daily_check(notice_flag)
    if check_opened() == false then
        get_new_year_info()
        if new_year_info ~= nil then
            new_year_info = nil
            save_new_year_info()
        end
    else
        if new_year_info == nil then
            get_cur_task_info_resp_data()
        end
    end
end

function agent_new_year.process_pvp_end(room_flag, room_type, pvp_end_type, addscore, addpoint)
    if check_opened() == false then
        return
    end
    if new_year_info == nil then
        get_cur_task_info_resp_data()
        if new_year_info == nil then
            return
        end
    end

    logger.trace("new year pvp end", room_flag, room_type, pvp_end_type, addscore, addpoint)
    if room_flag == COMMON.room_flag.RAND then
        change_value(TYPE_MAPPING.PVP_RAND_TIMES, 1)
        if pvp_end_type == COMMON.pvp_end_type.WIN then
            change_value(TYPE_MAPPING.PVP_RAND_WIN_TIMES, 1)
            change_value(TYPE_MAPPING.PVP_RAND_STREAK_WIN_TIMES, 1)
        else
            set_value(TYPE_MAPPING.PVP_RAND_STREAK_WIN_TIMES, 0)
        end
    elseif room_flag == COMMON.room_flag.FIXED then
        change_value(TYPE_MAPPING.PVP_ROOM_PVP_TIMES, 1)
        if pvp_end_type == COMMON.pvp_end_type.WIN then
            change_value(TYPE_MAPPING.PVP_ROOM_PVP_WIN_TIMES, 1)
            change_value(TYPE_MAPPING.PVP_ROOM_PVP_STREAK_WIN_TIMES, 1)
        else
            set_value(TYPE_MAPPING.PVP_ROOM_PVP_STREAK_WIN_TIMES, 0)
        end
    elseif room_flag == COMMON.room_flag.NOPROP then
        change_value(TYPE_MAPPING.PVP_NON_ITEM_TIMES, 1)
        if pvp_end_type == COMMON.pvp_end_type.WIN then
            change_value(TYPE_MAPPING.PVP_NON_ITEM_WIN_TIMES, 1)
        end
    end

    if pvp_end_type == COMMON.pvp_end_type.WIN then
        change_value(TYPE_MAPPING.PVP_ADD_POINT, addpoint)
    end
end

function agent_new_year.init(args)
    account_info  = args.info
    send_package  = args.send_package
    send_response   = args.send_response
    args.cmd_map[COMMON.MESSAGE.CS_CMD_NEWYEAR_GET_INFO] = handle_new_year_get_info
    args.cmd_map[COMMON.MESSAGE.CS_CMD_NEWYEAR_REWARD] = handle_new_year_reward
    args.cmd_map[COMMON.MESSAGE.CS_CMD_NEWYEAR_CMP] = handle_get_cmp

    redis_service = skynet.queryservice("redis_service")
end

function agent_new_year.on_open(act_info_)
    act_info = act_info_
    get_cur_task_info_resp_data()
end

function agent_new_year.on_end(act_info_)
    act_info = nil
    if new_year_info ~= nil then
        new_year_info = nil
        save_new_year_info()
    end
end

function agent_new_year.on_update(act_info_)
    --act_info = act_info_
end

return agent_new_year

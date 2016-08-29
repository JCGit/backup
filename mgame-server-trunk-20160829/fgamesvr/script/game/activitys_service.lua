
--活动时间服务
--
--auth : diandian
--date : 2016.01.12


local skynet = require "skynet"
local utils = require "utils"
local sharedata_utils = require "sharedata_utils"
local COMMON = require "common"
local logger = require "logger"

local redis_service
local notify_services = {}
local notify_update_list = {}
local gamed

local act_time_cache = {}

local act_infos = {}

local last_check_clock_time = 0

local function load_act_info_by_redis(id)
end

local function save_act_info_to_redis(id)
    if act_infos[id] then
        skynet.call(redis_service, "lua", "hset_obj", "activity_infos", id, act_infos[id])
    else
        skynet.call(redis_service, "lua", "hdel_obj", "activity_infos", id)
    end
end

local function save_act_time_cache_to_redis(id)
    if act_time_cache[id] then
        skynet.call(redis_service, "lua", "hset_obj", "activity_time_cache", id,  act_time_cache[id])
    else
        skynet.call(redis_service, "lua", "hdel_obj", "activity_time_cache", id)
    end
end

local function add_act_time_to_cache(id, start_time, end_time)

    act_time_cache[id] = act_time_cache[id] or {}
    local time = {}
    time.start_time = start_time
    time.end_time = end_time
    table.insert(act_time_cache[id], time)

    save_act_time_cache_to_redis(id)
end

local function remove_act_time_from_cache(id, index)
    if act_time_cache[id] and act_time_cache[id][index] then
        local time = table.remove(act_time_cache[id], index)
        save_act_time_cache_to_redis(id)
        return time
    end
    return nil
end

local function check_act_updates(updates, act, err)
    local fixed_info = act.fixed_info
    if not fixed_info then
        return
    end

    local change_flag = false
    local now = os.time()

    --时间间隔推送
    if fixed_info.space_time then
        local space_time = now - act.start_time
        for i = 1, #fixed_info.space_time do
            if space_time >= fixed_info.space_time[i] 
                and (not fixed_info.space_step
                or fixed_info.space_step < i) then

                if space_time - fixed_info.space_time[i] <= 2 then
                    fixed_info.space_step = i
                    local notice = {
                        act_info = act,
                        type = COMMON.act_notify_update_type.SPACE_TIME,
                    }
                    table.insert(updates, notice)
                    change_flag = true
                    break
                end
            end
        end
    end

    --到点推送
    if fixed_info.clock_time then
        local date_now = os.date("*t")
        local sec_count = date_now.hour * 60 * 60 + date_now.min * 60 + date_now.sec
        local interal_days = utils.getTimeInteral(last_check_clock_time, now)
        if interal_days ~= 0 then
            fixed_info.clock_step = 0
            last_check_clock_time = now
            skynet.call(redis_service, "lua", "set_obj", "activity_last_check_clock_time", last_check_clock_time)
            change_flag = true
        end
        local clock_time_count = #fixed_info.clock_time
        for i = 1, clock_time_count do
            local clock_sec = fixed_info.clock_time[i]
            if (not fixed_info.clock_step or fixed_info.clock_step < i)
                and sec_count >= clock_sec
                and sec_count - clock_sec <= 2 then
                fixed_info.clock_step = i
                local notice = {
                    act_info = act,
                    type = COMMON.act_notify_update_type.CLOCK_TIME,
                }
                table.insert(updates, notice)
                change_flag = true
                break
            end
        end
    end

    if change_flag then
        save_act_info_to_redis(act.ActivityID)
    end
end

local function check_cur_space_step(act_info)
    if not act_info.fixed_info or not act_info.fixed_info.space_time then
        return
    end
    
    local fixed_info = act_info.fixed_info
    local now = os.time()
    local time_count = #fixed_info.space_time
    for i = 1, time_count do
        local space_time = fixed_info.space_time[time_count - i + 1]
        print("now: ", now, "space_time:", space_time + act_info.start_time)
        if now >= space_time + act_info.start_time then
            fixed_info.space_step = time_count - i + 1
            return
        end
    end
end

local function check_cur_clock_step(act_info)
    if not act_info.fixed_info or not act_info.fixed_info.clock_time then
        return
    end

    local date_now = os.date("*t")
    local sec_count = date_now.hour * 60 * 60 + date_now.min * 60 + date_now.sec
    local fixed_info = act_info.fixed_info
    local time_count = #fixed_info.clock_time
    for i = 1, time_count do
        local clock_sec = fixed_info.clock_time[time_count - i + 1]
        if (not fixed_info.clock_step or fixed_info.clock_step < i)
            and sec_count >= clock_sec
            and sec_count - clock_sec <= 2 then

            fixed_info.clock_step = time_count - i + 1
            return
        end
    end
end

local function on_acts_open(opens)
    local hav_notify = false
    for i, v in ipairs(opens) do
        check_cur_space_step(v)
        check_cur_clock_step(v)
        save_act_info_to_redis(v.ActivityID)
        hav_notify = true
        local nt_svr = notify_services[v.ActivityID]
        if nt_svr then
            logger.trace("notify activity open :", v.ActivityID)
            skynet.call(nt_svr, "lua", "notify_act_open", v)
        end
    end

    --通知所有玩家agent 活动开启
    if hav_notify then
        skynet.call(gamed, "lua", "notify_act_open", opens)
    end
end

local function on_acts_update(updates)
    local notify_agent_list = {}
    for i, v in ipairs(updates) do
        local nt_svr = notify_services[v.act_info.ActivityID]
        if nt_svr then
            logger.trace("notify activity update", v.act_info.ActivityID, v.type)
            skynet.call(nt_svr, "lua", "notify_act_update", v.act_info, v.type)
        end
        if notify_update_list[v.act_info.ActivityID] then
            table.insert(v)
        end
        save_act_info_to_redis(v.act_info.ActivityID)
    end

    if next(notify_agent_list) then
        --暂时先不通知agent活动更新
        --skynet.call(gamed, "lua", "notify_act_update", notify_agent_list)
    end
end

local function on_acts_end(ends)
    local hav_notify = false
    for i, v in ipairs(ends) do
        skynet.call(redis_service, "lua", "hdel_obj", "activity_infos", v.ActivityID)
        hav_notify = true
        local nt_svr = notify_services[v.ActivityID]
        if nt_svr then
            logger.trace("notify_act_end:", v.ActivityID)
            skynet.call(nt_svr, "lua", "notify_act_end", v)
        end
    end

    if hav_notify then
        skynet.call(gamed, "lua", "notify_act_end", ends)
    end
end

local function check_cache_time(act_info)
        --活动结束清楚活动信息
    local cache_time = remove_act_time_from_cache(act_info.ActivityID, 1)
    if cache_time then
        act_info.start_time = cache_time.start_time
        act_info.end_time = cache_time.end_time
        act_info.ActivityID = act_info.ActivityID
        act_info.fixed_info = nil
    end
end

local function check_acts_time()
    local now = os.time()
    local opens = {}
    local updates = {}
    local ends = {}
    for k, v in pairs(act_infos) do
        if now >= v.start_time and now < v.end_time then
            if v.is_notify_open then
                check_act_updates(updates, v)
            else
                table.insert(opens, v)
                v.is_notify_open = true
                v.is_notify_end = false
                v.is_opened = true
            end
        elseif now >= v.end_time then
            if not v.is_notify_end then
                table.insert(ends, v)
                v.is_notify_end = true
                v.is_notify_open = false
                v.is_opened = false
            else
                check_cache_time(v)
            end
        end
    end

    on_acts_open(opens)
    on_acts_update(updates)
    on_acts_end(ends)

    skynet.timeout(200, check_acts_time)
end

local function register_act_services()
    notify_services[COMMON.activity_id.ACT_MONEY] = skynet.uniqueservice("greb_red_packet_service")
    notify_services[COMMON.activity_id.ACT_NEWYEAR] = skynet.uniqueservice("activity_newyear_service")
    notify_services[COMMON.activity_id.ACT_RANK_WEEK_AWARD] = skynet.uniqueservice("rank_week_reward_service")
end

local function register_agent_update_act()
    
end


local CMD = {}

function CMD.register_fixed(id, fixed_info)
    if act_infos[id] and not act_infos[id].fixed_info then
        act_infos[id].fixed_info = fixed_info
        check_cur_space_step(act_infos[id])
        check_cur_clock_step(act_infos[id])
        save_act_info_to_redis(id)
    end
end

--添加活动开启批次的
function CMD.add_act_open(id, start_time, end_time)
    print("add_act_open:", id, start_time, end_time)
    local act_info = act_infos[id]
    if act_info then
        add_act_time_to_cache(id, start_time, end_time)
        return "success"
    end
    return "act id is not exist"
end

function CMD.close_act(id)
    local act_info = act_infos[id]
    if act_info == nil then
        return "act id is not exist"
    end

    if not act_info.is_opened then
        return "act id is not opened"
    end

    act_info.end_time = os.time() - 1
    return "success"
end

function CMD.clear_all_cache_time(id)
    if act_time_cache[id] then
        act_time_cache[id] = nil
        save_act_time_cache_to_redis(id)
    end
end

function CMD.init()

    last_check_clock_time = skynet.call(redis_service, "lua", "get_obj", "activity_last_check_clock_time") or 0
    local all_activity_config = sharedata_utils.get_stat_all(COMMON.plan.Activity_Time)
    for k , v in pairs(all_activity_config) do

        local redis_act = skynet.call(redis_service, "lua", "hget_obj", "activity_infos", v.ActivityID)

        if redis_act then
            act_infos[v.ActivityID] = redis_act
        else
            local act = {}
            act.start_time = v.begindate
            act.end_time = v.enddate
            act.ActivityID = v.ActivityID

            act_infos[v.ActivityID] = act
        end
        --设置默认值
        act_infos[k].is_notify_open = nil
        act_infos[k].is_notify_end = nil
    end

    act_time_cache = skynet.call(redis_service, "lua", "hgetall_all_value", "activity_time_cache", true)

    register_act_services()
    register_agent_update_act()
    check_acts_time()
end

function CMD.get_all_act_info()
    return act_infos
end


local traceback = debug.traceback
skynet.start(function()

    redis_service = skynet.queryservice("redis_service")
    gamed = skynet.queryservice("gamed")

    skynet.dispatch("lua", function(_, _, command, ...)
        --logger.trace("command  : " .. command)
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

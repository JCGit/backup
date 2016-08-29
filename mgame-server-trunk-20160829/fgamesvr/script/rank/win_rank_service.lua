--
-- Author: yibin
-- Date: 2016/03/15
-- Desc: 巅峰排行榜pvp胜利场次排行榜
--
local skynet  = require "skynet"
local utils   = require "utils"
local logger  = require "logger"
local setting = require "setting"
local COMMON  = require "common"
local player  = require "player"
local cluster = require "cluster"
local queue    = require "skynet.queue"
local sharedata_utils = require "sharedata_utils"

local CMD = {}
local redis_conn = nil
-- critical section
local cs = queue()

--[[
rank_data 存储

redis hash
win_rank
    key1 value1
    key2 value2
    key3 value3

lua table:
{
    {uin = 1000001, value = 10, name = 'abc', time = os.time()},    
    {uin = 1000020, value = 20, name = 'efg', time = os.time()},    
    {uin = 1000030, value = 5, name = 'jkm', time = os.time()}, 
}
-- value 代表pvp当天胜利的场数
]]

local rank_data = {}
local min_value = 0
local hash_table = 'win_rank'
local activity_cfg
-- 是否在活动期间内重启
local restart = false
local interval
local open = false
local timer_tick = 0
local award_hour = 23
local award_min = 59
local traceback = debug.traceback
local log_service

-- 排序及得到最小分 从大到小
local function sort_rank(data)
    if next(data) then
        table.sort(data, function(itema, itemb)
            return (itema.value > itemb.value) or
                (itema.value == itemb.value and itema.time < itemb.time)
            end)
        min_value = data[#data].value
    end
end

local function clear_data()
    logger.debugf("win_rank_service->clear_data at[%s]", utils.date_to_str(os.time()))
    skynet.call(redis_conn, "lua", "del", hash_table, true)
    rank_data = {}
    min_value = 0 
end

local function flush()
    local after = 0
    if open then
        local now = os.time()
        local date = os.date('*t', now)
        -- 改成到半小时或整点再按半小时刷新
        if restart then
            after = interval - date.min % interval 
            restart = false
        else   
            after = interval - date.min % interval 
            if next(rank_data) then
                skynet.call(redis_conn, "lua", "hreset_obj", hash_table, utils.pack_hash_uin_key(rank_data))
            else
                logger.debugf("win_rank_service->flush() data is nil at[%s]", utils.date_to_str(now))
            end
            -- 定时器是按半小点整点刷新的，必然是小时的1/2或是整点 到零里06:00清除昨天的数据
            if date.hour == 6 and date.min == 0 then
                logger.debugf("win_rank_service->flush() clear yesterday data at[%s]", utils.date_to_str(os.time()))
                clear_data()
            end           
        end
        -- 去掉秒数，和game_proxy一致
        after = after * 60 - date.sec
        logger.debugf("win_rank_service->flush() now[%d] interval[%d] again at[%s]", now, after, utils.date_to_str(now + after))
    else
        logger.debugf("win_rank_service->flush() activity is not open at[%s]", utils.date_to_str(now))
        return
    end
    skynet.timeout(after * 100, flush)
end


local function send_mail_2center( uin, title, content, plugin )
    local now = os.time()
    local data = {
        title = title,
        content = content,
        plugin = plugin,
        sendtime = now,
        outdate =  now + 30 * 24 * 60 * 60 * 100  -- 一个月之后
    }
    local ok, ret = xpcall(cluster.call, traceback, "centernode", ".global_mailservice", "add_mail", uin,
        COMMON.mail_type.SINGLE, data, COMMON.change_src.opreate_mail)
    if not ok then
        logger.debugf("win_rank_service->send_mail_2center() failed uin[%d] title[%s] content[%s] plugin[%s]", uin, title, content, plugin)
    end
end

local function begin_award()
    -- 发奖按缓存里最新的发
    for k,v in pairs(rank_data) do
        local award_cfg = activity_cfg.top10[k]
        if award_cfg then
            local plugin
            plugin = string.format("{%s}", award_cfg)
            local ctnt = string.format(activity_cfg.mailContent, k)
            send_mail_2center(v.uin, activity_cfg.mailTitle, ctnt, plugin)
            skynet.send(log_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_top_win, k, v.uin, v.name, v.value)
            logger.debugf("win_rank_service->begin_award() send mail success for uin[%d] plugin[%s]", v.uin, plugin)
        else
            logger.debugf("win_rank_service->begin_award award cfg not found for [%d]", k)
        end
    end
end


local function check_award()
    if open then
        local tmp = timer_tick
        local now = os.time()
        if timer_tick == 0 then -- 到当天整点结算
            local date = os.date('*t', now)
            local award_time
            if (award_hour > date.hour) or 
                (award_hour == date.hour and award_min >= date.min) then
                award_time = os.time({year = date.year, month = date.month,
                        day = date.day, hour = award_hour, min = award_min})
            else
                award_time = os.time({year = date.year, month = date.month,
                        day = date.day + 1, hour = award_hour, min = award_min})
            end
            timer_tick = os.difftime(award_time, now)
        else -- 隔24小时发奖
            timer_tick = 24 * 60 * 60
        end

        logger.debugf("win_rank_service->check_award() now[%d] interval[%d] next_ticker[%s] ", now, timer_tick, utils.date_to_str(now+timer_tick))

        if tmp ~= 0 then
            begin_award()
            --clear_data()
        end
    else
        logger.debug("win_rank_service->check_award() activity is not open")
        return
    end    
    skynet.timeout(timer_tick * 100, check_award)
end



local function load_data()
    if redis_conn then
        rank_data = skynet.call(redis_conn, "lua", "hgetall_obj", hash_table)
        -- 处理值相同的老数据没有时间字段,时间字段用来处理上榜先后顺序
        local now = os.time()
        for k,v in pairs(rank_data) do
            if not v.time then
                v.time = now - 12 * 60 * 60 + k
            end
        end
        sort_rank(rank_data)
    else
        logger.error("win_rank_service->load_data() redis conn is nil")
    end
end

local function load_award_time()
    local cfg = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.WIN_RANK)
    if cfg then
        local hour, sec = string.match(cfg.value, "(%d+):(%d+)")
        logger.debugf("win_rank_service->load_award_time() hour[%s] sec[%s]", hour, sec)
        award_hour = tonumber(hour or award_hour)
        award_min = tonumber(sec or award_min)
    end
end

local function in_rank(uin)
    for k,v in pairs(rank_data or {}) do
        if v.uin == uin then
            return true, k
        end
    end
    return false, 0
end

local function update(uin, name, value)
    local now = os.time()
    local len = #rank_data
    if len >= setting.top_num and min_value >= value then
        logger.debugf("win_rank_service-->update() value[%d] less than the min_value[%d]", value, min_value)
        return
    end

    if len >= setting.top_num then
        if min_value < value then
            local find, k
            find, k = in_rank(uin)
            if find then
                rank_data[k].value = value
                rank_data[k].name = name
                rank_data[k].time = now
            else
                local last_one = rank_data[len]
                last_one.uin = uin
                last_one.value = value
                last_one.name = name
                last_one.time = now
            end
            sort_rank(rank_data)
        end
    else
        local find, k
        find, k = in_rank(uin)
        if find then
            rank_data[k].value = value
            rank_data[k].name = name
            rank_data[k].time = now
        else
            table.insert(rank_data, { uin = uin, name = name, value = value, time = now})
        end
        sort_rank(rank_data)
    end
end

local function queue_get_list()
    local data
    data = skynet.call(redis_conn, "lua", "hgetall_obj", hash_table)
    sort_rank(data)
    return data
end

function CMD.update_list(uin, name, value)
    cs(update, uin, name, value)
end

function CMD.get_list()
    return cs(queue_get_list)
end


function CMD.open( status, config )
    if status and not open then
        open = true
        timer_tick = 0 -- 发奖的定时器，有可能重复开启活动
        activity_cfg = config.topConfig
        interval = activity_cfg.inter or 30
        local hour, min = string.match(activity_cfg.wardTime, "(%d+):(%d+)")
        award_hour = tonumber(hour or 23)
        award_min = tonumber(min or 59)
        check_award()
        flush()
        logger.debugf("win_rank_service->open() open at[%s]", utils.date_to_str(os.time())) 
    elseif not status and open then
        open = false
        clear_data() 
        logger.debugf("win_rank_service->open() closed at[%s]", utils.date_to_str(os.time()))
    else
        logger.debugf("win_rank_service->open() what status[%s] cur status[%s]", tostring(status), tostring(open))
    end

end

-- 缓冲buffer更新到redis
--
function CMD.exit()
    logger.debug("win_rank_service exit")
    if open then
        if next(rank_data) then
            skynet.call(redis_conn, "lua", "hreset_obj", hash_table, utils.pack_hash_uin_key(rank_data))
        else
            logger.debugf("win_rank_service->exit() data is nil")
        end
    end
end


-- 在活动期间内有重启进程
local function check_restart()
    local win_open = false
    local datas = skynet.call(redis_conn, "lua", "hgetall_all_value", "operate_activities")
    local now = os.time()
    for _, data in pairs(datas) do
        if data.type == COMMON.OP_ACT_TYPE.TOP_WIN and
         now >= data.startTime and now < data.endTime then
            win_open = true
            activity_cfg = data.topConfig
            break
        end
    end

    if win_open then
        restart = true
        open = true
        timer_tick = 0 -- 发奖的定时器
        interval = activity_cfg.inter or 30
        local hour, min = string.match(activity_cfg.wardTime, "(%d+):(%d+)")
        award_hour = tonumber(hour or 23)
        award_min = tonumber(min or 59)
        check_award()
        flush()
        logger.debugf("win_rank_service->check_restart() restart in open")
    end
end

local function test_data()
    table.insert(rank_data, {uin = 1000001, value = 10, name = 'abc', time = os.time()})
    table.insert(rank_data, {uin = 1000020, value = 20, name = 'efg', time = os.time()})
    table.insert(rank_data, {uin = 1000030, value = 5, name = 'jkm', time = os.time()})
    table.insert(rank_data, {uin = 1000031, value = 3, name = 'abcdee', time = os.time()})
    table.insert(rank_data, {uin = 1000033, value = 2, name = 'jkafajfljm', time = os.time()})
    table.insert(rank_data, {uin = 1000034, value = 1, name = 'jkmlejw', time = os.time()})
end

skynet.start(function ( ... )

    redis_conn = skynet.queryservice("redis_service")
    log_service = skynet.queryservice("business_log_service")
    load_data()
    check_restart()

    -- test_data()

    skynet.dispatch("lua", function(_, src, command, ...)
        local func = CMD[command]
        if not func then
            logger.warningf("win_rank_service unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(func, traceback, ...)
        if not ok then
            logger.warningf("win_rank_service handle message(%s) failed : %s src:%d", command, ret, src)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)

end)

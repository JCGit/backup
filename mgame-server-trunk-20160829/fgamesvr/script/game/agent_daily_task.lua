--  agent_daily_task.lua
--  每日任务

local agent_daily_task = {}

local skynet    = require "skynet"

local sharedata_utils    = require "sharedata_utils"
local agent_achievement  = require "agent_achievement"
local agent_pve          = require 'agent_pve'

local logger    = require "logger"
local proto     = require "proto"
local utils     = require "utils"
local COMMON    = require "common"
local MESSAGE   = COMMON.MESSAGE
local ERRORCODE = COMMON.ERRORCODE

local send_response, send_error_code
local account_info, uin, REDIS, player,busilog_service

local DAILY_TASK = 'daily_task'

-- open的时候检查玩家是否有今天的每日任务数据，
local function handle_daily_task_status(cmdid, content, len)
    cmdid = cmdid or MESSAGE.CS_CMD_DAILY_TASK_STATUS
    local data = skynet.call(REDIS, 'lua', 'hget_obj', DAILY_TASK, uin)
    local ShowDailyTask = true
    if data and data.is_get_award and utils.is_today(data.timestamp) then
        ShowDailyTask = false
    end
    return send_response(cmdid, { ShowDailyTask = ShowDailyTask })
end


--[[
序号,任务内容,是否可配次数,是否需要校验玩家数据,是否有1级指引（主界面）,
                                是否有2级指引（系统）,优先级,是否核心玩法内容

1  , 进行匹配对战X次（匹配PVP）     , 1 , 0 , 1 , 1 , 1 , 1
2  , 进行X次无道具对战（无道具PVP） , 1 , 0 , 1 , 1 , 3 , 1
3  , 开启房间对战1次（开房PVP）     , 0 , 0 , 1 , 1 , 2 , 1
4  , 任意对战（PVP）胜利X次         , 1 , 0 , 1 , 0 , 4 , 1
5  , PVE模式下消灭XX个小兵          , 1 , 0 , 0 , 0 , 0 , 1
6  , PVE模式下占领XX个建筑          , 1 , 0 , 0 , 0 , 0 , 1
7  , PVP模式下消灭XX个小兵          , 1 , 0 , 0 , 0 , 0 , 1
8  , PVP模式下占领XX个建筑          , 1 , 0 , 0 , 0 , 0 , 1
9  , 使用X次道具                    , 1 , 0 , 0 , 0 , 0 , 1

-- 暂未开启
10 , 美味大冒险推进1关              , 0 , 1 , 1 , 0 , 0 ,
11 , 评星增加1颗                    , 0 , 1 , 1 , 1 , 0 ,
12 , 升级1次雕文                    , 0 , 1 , 1 , 0 , 0 ,
--]]

-- 单个任务的奖励读取策划配置
-- note: 任务的类型必须 >= 4, 不然切换任务的功能会出错
local TaskTypeList = {1,2,3,4,5,6,7,8,9}
local SERVER_CHECK_TASK = {1, 2, 3, 4} -- 服务器检查完成的任务
local CLIENT_CHECK_TASK = {5, 6, 7, 8, 9} -- 客户端上报完成情况的任务
local DailyTaskCount = 3
local DailyTaskConfig, DailyTaskAward

local function get_task_list()
    if agent_pve.get_max_checkpoint_id() >= 1180 then
        return TaskTypeList
    else
        return {1,2,4,5,6,7,8,9} -- 未达到18级的任务列表
    end
end

local function check_task_open(task_type)
    if task_type == 2 then
        local segments = sharedata_utils.get_stat_all(COMMON.plan.PvpMatch)
        return utils.IsInSegments(segments, os.time())
    else
        return true
    end
end

-- 除去今天的奖励已经领取过的情况之外，客户端都会请求今天的每日任务
local function handle_request_daily_task(cmdid, content, len)

    local function roll_daily_tasks()
        local r = {}
        local task_types = utils.get_random_sublist(get_task_list(), DailyTaskCount)
            for _, task_type in ipairs(task_types) do
                local tasks = DailyTaskConfig[task_type]
                assert(type(tasks) == 'table' and #tasks >= 1, '错误的配置')
                local task_config = tasks[math.random(#tasks)]
                table.insert(r, {
                    TaskType      = task_type,
                    GiftID        = task_config.giftID,
                    Progress      = 0,
                    TotalProgress = task_config.totalProgress,
                IsOpen        = check_task_open(task_type),
            })
                skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_daily_task, uin, task_type, 0)
        end
        return r
    end

    local now_ts = math.floor(skynet.time())

    local data = skynet.call(REDIS, 'lua', 'hget_obj', DAILY_TASK, uin)
    if data == nil then
        -- 今天的每日任务还没有做，而且之前也没有每日任务数据

        data = {
            timestamp    = now_ts,
            is_get_award = false,
            tasks        = roll_daily_tasks(),
            can_change   = true,
            task_progress  = 0,
        }
        -- 生成今天的每日任务，保存，返回给客户端
        skynet.call(REDIS, 'lua', 'hset_obj', DAILY_TASK, uin, data)

    else
        -- 有今天的数据直接返回，之前的数据则判断领奖状态
        if (not utils.is_today(data.timestamp)) and data.is_get_award then
            -- 领过奖了，生成今天的新数据
            data.timestamp   = now_ts
            data.is_get_award = false
            data.tasks       = roll_daily_tasks()
            data.can_change  = true
            data.task_progress = 0

            skynet.call(REDIS, 'lua', 'hset_obj', DAILY_TASK, uin, data)
        end
    end

    for _, v in ipairs(data.tasks) do
        if v.TaskType == 2 then
            v.IsOpen = check_task_open(2)
            break
        end
    end

    local r = {
        TaskInfoList = data.tasks,
        TaskProgress = data.task_progress,
        CanChange    = data.can_change,
    }

    return send_response(cmdid, r)
end

local function handle_change_daily_task(cmdid, content, len)
    local data = skynet.call(REDIS, 'lua', 'hget_obj', DAILY_TASK, uin)
    if data == nil or (not data.can_change) then
        return send_response(cmdid, { Result = false })
    end

    local tt = proto.parse_proto_req(cmdid, content, len).TaskType

    local other_task_type = {}
    local idx
    for i, v in ipairs(data.tasks) do
        if v.TaskType == tt then
            idx = i
        else
            table.insert(other_task_type, v.TaskType)
        end
    end
    if not idx or data.tasks[idx].TaskType ~= tt then
        return send_response(cmdid, { Result = false })
    end
    assert(#other_task_type == DailyTaskCount - 1)
    local new_tt
    while true do
        new_tt = utils.get_random_sublist(get_task_list(), 1)[1]
        if new_tt ~= other_task_type[1] and new_tt ~= other_task_type[2]
            and new_tt ~= tt then
            break
        end
    end

    local tsk = data.tasks[idx]
    assert(type(tsk) == 'table')
    local tsk_tp_cfgs = DailyTaskConfig[new_tt]
    local tsk_cfg = tsk_tp_cfgs[math.random(#tsk_tp_cfgs)]

    tsk.TaskType      = new_tt
    tsk.GiftID        = tsk_cfg.giftID
    tsk.Progress      = 0
    tsk.TotalProgress = tsk_cfg.totalProgress
    tsk.IsOpen        = check_task_open(new_tt)

    data.can_change = false
    skynet.call(REDIS, 'lua', 'hset_obj', DAILY_TASK, uin, data)
    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_daily_task, uin, new_tt, tt)
    local r = { Result = true, NewTask = tsk, }
    return send_response(cmdid, r)
end

local function handle_open_daily_task_award(cmdid, content, len)

    local resp = { Ok = false }

    local data = skynet.call(REDIS, 'lua', 'hget_obj', DAILY_TASK, uin)

    if data == nil or data.is_get_award or data.task_progress == 0 then
        return send_response(cmdid, resp)
    end

    if not utils.elem({1, 2, 3}, data.task_progress) then
        return send_response(cmdid, resp)
    end
    local award_gift_id

    -- 根据配置roll奖励
    local cfg = DailyTaskAward[data.task_progress]
    assert(cfg)
    local ri = math.random(cfg.maxw)
    for _, v in ipairs(cfg) do
        if ri <= v[2] then
            award_gift_id = v[1]
            break
        end
    end
    assert(award_gift_id)

    data.is_get_award = true -- 更新领奖状态
    skynet.call(REDIS, 'lua', 'hset_obj', DAILY_TASK, uin, data)

    -- TODO 来源fix
    player.add_gift(account_info, award_gift_id, COMMON.change_src.mission)
    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_daily_task_reward, uin, data.task_progress)

    resp.Ok          = true
    resp.GiftID      = award_gift_id
    resp.HaveNewTask = not utils.is_today(data.timestamp)
    return send_response(cmdid, resp)
end

-- 今天的今日任务处理完成之后，设置此标志
-- 初始化的时候，根据情况取消本标志
local skip_pvp_event_today = false

local function handle_update_daily_task_progress(cmdid, content, len)
    cmdid = cmdid or MESSAGE.CS_CMD_UPDATE_DAILY_TASK_PROGRESS

    if type(content) == 'table' then
        assert(#content >= 1) -- 至少有一个任务

        local data = skynet.call(REDIS, 'lua', 'hget_obj', DAILY_TASK, uin)

        if data == nil then return end

        if utils.is_today(data.timestamp)
            and (data.is_get_award or data.task_progress == 3) then
            -- 任务完成或已经领取过今天的奖励了，停止事件检查
            skip_pvp_event_today = true
            return
        end
        local res = {}
        for _, done_task in ipairs(content) do

            assert(utils.elem(SERVER_CHECK_TASK, done_task))

            for _, t in ipairs(data.tasks) do
                if t.TaskType == done_task and t.Progress < t.TotalProgress then
                    t.Progress = t.Progress + 1

                    if t.Progress == t.TotalProgress then
                        data.task_progress = data.task_progress + 1
                        account_info.daily_task_count =
                            account_info.daily_task_count + 1
                        agent_achievement.update_achievement()

                        -- TODO 来源fix
                        player.add_gift(account_info, t.GiftID, COMMON.change_src.mission)
                        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_daily_task_done, uin, t.TaskType)
                    end

                    table.insert(res, {done_task, t.Progress})
                    break -- 一天的任务中，同一类型的不会有2个，所以处理完就break
                end
            end
        end
        if #res >= 1 then
            -- 保存变化了的结果
            skynet.call(REDIS, 'lua', 'hset_obj', DAILY_TASK, uin, data)

            -- 推送变化情况给客户端
            local resp = {
                Ok       = true,
                TaskType = {},
                Progress = {},
            }
            for _, v in ipairs(res) do
                table.insert(resp.TaskType, v[1])
                table.insert(resp.Progress, v[2])
            end
            return send_response(cmdid, resp)
        end
    else
        -- 客户端上报任务进度的处理
        assert(type(content) == 'string')
        local resp = { Ok = false, TaskType = {}, Progress = {}, }

        local req = proto.parse_proto_req(cmdid, content, len)
        local tts, pgs = req.TaskType, req.Progress
        if #tts == 0 or #tts ~= #pgs then
            logger.trace('上报任务进度数据错误')
            return send_response(cmdid, resp)
        end

        local res = {}
        for i, task in ipairs(tts) do
            local progress = pgs[i]
            if not utils.elem(CLIENT_CHECK_TASK, task) then
                logger.trace('上报任务类型错误')
                return send_response(cmdid, resp)
            end
            table.insert(res, {task, progress})
        end
        local data = skynet.call(REDIS, 'lua', 'hget_obj', DAILY_TASK, uin)
        if data == nil then
            return send_response(cmdid, resp)
        end
        if utils.is_today(data.timestamp)
            and (data.is_get_award or data.task_progress == 3) then
            logger.trace('今天的任务已经领过奖励或者任务已全部做完')
            return send_response(cmdid, resp)
        end
        local cr = {}
        for _, v in ipairs(res) do
            for _, task in ipairs(data.tasks) do
                if task.Progress < task.TotalProgress
                    and v[1] == task.TaskType and v[2] > 0 then
                    task.Progress = task.Progress + v[2]
                    if task.Progress >= task.TotalProgress then

                        task.Progress = task.TotalProgress

                        data.task_progress = data.task_progress + 1
                        account_info.daily_task_count =
                            account_info.daily_task_count + 1
                        agent_achievement.update_achievement()

                        -- TODO 来源fix
                        player.add_gift(account_info, task.GiftID, COMMON.change_src.mission)
                        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_daily_task_done, uin, task.TaskType)
                    end
                    table.insert(cr, {v[1], task.Progress})
                    break
                end
            end
        end
        if #cr >= 1 then
            skynet.call(REDIS, 'lua', 'hset_obj', DAILY_TASK, uin, data)

            resp.Ok = true
            for _, v in ipairs(cr) do
                table.insert(resp.TaskType, v[1])
                table.insert(resp.Progress, v[2])
            end
            return send_response(cmdid, resp)
        else
            return send_response(cmdid, resp)
        end
    end
end

--[[
    room_flag   房间类型: RAND 随机匹配, FIXED 固定开房, NOPROP 无道具场
    room_type   房间人数类型类型: 2人房 3人房 4人房
    result      pvp战斗结果: 胜利 失败 平局
    addscore    增加的荣誉值
    addpoint    增加的积分
--]]
local function on_pvp_end(eventName, room_flag, room_type, result, addscore, addpoint)
    if skip_pvp_event_today then return end

    local done_tasks = {}

    local task_type
    if room_flag == COMMON.room_flag.RAND then
        task_type = 1 -- 成功进行一次pvp对战
    elseif room_flag == COMMON.room_flag.NOPROP then
        task_type = 2 -- 成功进行一次无道具场对战
    elseif room_flag == COMMON.room_flag.FIXED then
        task_type = 3 -- 开放对战
    end

    if task_type then
        table.insert(done_tasks, task_type)
    end

    if result == COMMON.pvp_end_type.WIN then
        table.insert(done_tasks, 4)
    end

    if # done_tasks >= 1 then
        handle_update_daily_task_progress(nil, done_tasks)
    end
end


local function add_cmd_handle_mapping(cmd_map)
    local M = MESSAGE
    cmd_map[M.CS_CMD_DAILY_TASK_STATUS    ] = handle_daily_task_status
    cmd_map[M.CS_CMD_REQUEST_DAILY_TASK   ] = handle_request_daily_task
    cmd_map[M.CS_CMD_CHANGE_DAILY_TASK    ] = handle_change_daily_task
    cmd_map[M.CS_CMD_OPEN_DAILY_TASK_AWARD] = handle_open_daily_task_award
    cmd_map[M.CS_CMD_UPDATE_DAILY_TASK_PROGRESS] = handle_update_daily_task_progress
end

-- 检查无道具场开放时，本玩家有没有每日任务状态信息要更新
local function on_pvp_noprop_info(eventName, is_start)

    local data = skynet.call(REDIS, 'lua', 'hget_obj', DAILY_TASK, uin)
    if data == nil or data.task_progress >= 3 then
        return
    end
    for i, task in ipairs(data.tasks) do
        if task.TaskType == 2 -- 2 无道具场对战
            and task.Progress < task.TotalProgress then
            return send_response(MESSAGE.CS_CMD_UPDATE_TASK_STATUS, {
                 TaskType = 2, IsOpen = is_start,
            })
        end
    end
end

local function init_module_events()
    EventCenter.addEventListener(Events.AGENT_OPEN, function ()
        return handle_daily_task_status()
    end)
    EventCenter.addEventListener(Events.PVP_END, on_pvp_end)
    EventCenter.addEventListener(Events.DAY_CHANGE, function ()
        skip_pvp_event_today = false
        local data = skynet.call(REDIS, 'lua', 'hget_obj', DAILY_TASK, uin)
        if not (data and data.is_get_award and utils.is_today(data.timestamp)) then
            local cmdid = MESSAGE.CS_CMD_NEW_DAILY_TASK
            return send_response(cmdid, { Ok = true })
        end
    end)

    EventCenter.addEventListener(Events.PVP_NOPROP_INFO, on_pvp_noprop_info)
end

function agent_daily_task.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code

    account_info    = args.info
    uin             = args.uin
    REDIS           = args.redis
    player          = args.player
    busilog_service = args.busilog_service

    DailyTaskConfig = sharedata_utils.get_stat_all(COMMON.plan.DAILY_TASK_CONFIG)
    DailyTaskAward  = sharedata_utils.get_stat_all(COMMON.plan.DAILY_TASK_AWARD)

    add_cmd_handle_mapping(args.cmd_map)

    local DAILY_TASK_OPEN_CPID = 1120

    -- 关卡第1章第12关 关卡ID 1120 要求关卡ID有序
    if agent_pve.get_max_checkpoint_id() >= DAILY_TASK_OPEN_CPID then
        init_module_events()
    else
        local lc_handle
        local function ck()
            local mcp = agent_pve.get_max_checkpoint_id()
            if mcp >= DAILY_TASK_OPEN_CPID then
                EventCenter.removeEventListener(lc_handle, Events.PVE_END)
                init_module_events()
                return send_response(MESSAGE.CS_CMD_NEW_DAILY_TASK, { Ok = true })
            end
        end
        lc_handle = EventCenter.addEventListener(Events.PVE_END, ck)
    end
end

return agent_daily_task

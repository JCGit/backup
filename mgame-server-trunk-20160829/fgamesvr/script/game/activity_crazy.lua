
local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local COMMON = require "common"
local sharedata_utils = require "sharedata_utils"
local proto = require "proto"
local player = require "player"
local busilog_service

local account_info
local agent
local redis_service

local activity_crazy = {}

local send_package, send_response, send_error_code

local function add_cmd(CMD)
end

local activity_config

local function get_crazy_config()
    if not activity_config then
        activity_config = sharedata_utils.get_stat_all(COMMON.plan.ActivityCrazy)
        for k, v in pairs(activity_config) do
            v.start_time = utils.get_time(v.start_time)
            v.end_time = utils.get_time(v.end_time)
        end
    end
    return activity_config
end

local function activity_crazy_open()
    local b_crazy = false
    local cur_day = 0
    local time_str = ""
    local term = 0
    local activity_data  = sharedata_utils.get_stat_all(COMMON.plan.ActivityCrazy)
    if not activity_data then
        return b_crazy,cur_day,time_str,term
    else
        for _,data in pairs(activity_data) do
            local start_time = utils.get_time(data.start_time)
            local end_time = utils.get_time(data.end_time)
            local now_time = utils.getSameDayBeginTime()
            if start_time <= now_time and now_time < end_time then
                local day = utils.getTimeInteral(start_time,now_time)
                cur_day = day + 1
                b_crazy = true
                time_str = time_str .. os.date("%Y.%m.%d",start_time) .. "---" .. os.date("%Y.%m.%d",end_time)
                term = data.id -- 记录是狂欢周的第几期
                break
            end
        end
    end
    return b_crazy,cur_day,time_str,term
end

--清除旧的狂欢周数据
local function discard_old()
    local term = 0
    _, _, _,term = activity_crazy_open()
    if term ~= 0 then -- 当前狂欢周活动存在，且存在之前狂欢活动的数据
        local crazy_data = skynet.call(redis_service, "lua", "hget_obj", "activity_crazy", account_info.uin,true)
        if crazy_data and (crazy_data.Term ~= term) then
            skynet.call(redis_service, "lua", "hdel_obj", "activity_crazy", account_info.uin)
        end
    else -- 不存在同样执行清除
        skynet.call(redis_service, "lua", "hdel_obj", "activity_crazy", account_info.uin)
    end
end

local function get_gift_cfg(day)
    local activity_gift  = sharedata_utils.get_stat_all(COMMON.plan.ActivityCrazyGift)
    if not activity_gift then
        return nil
    else
        if activity_gift[day] and activity_gift[day]["id"] == day then
            return activity_gift[day]
        else
            return nil
        end
    end
end

local function daily_check()
    local b_crazy = false
    b_crazy = activity_crazy_open()
    if b_crazy then
        local crazy_data = skynet.call(redis_service, "lua", "hget_obj", "activity_crazy", account_info.uin,true)
        if crazy_data ~= nil then
            crazy_data.PvpWin = 0
            crazy_data.PvpNum = 0
            crazy_data.Usable = 0
            logger.logf("do clear crazy eslap data for uin : %d",account_info.uin)
            skynet.call(redis_service, "lua", "hset_obj", "activity_crazy", account_info.uin,crazy_data,true)
        end
    else
        -- 在线跨天无狂欢周活动清除数据
        skynet.call(redis_service, "lua", "hdel_obj", "activity_crazy", account_info.uin)
    end
end

--pvp结束后调用
function activity_crazy.do_statics(addscore,end_type)
    local data = {
        Score = 0,
        Points = 0,
        PvpWin = 0,
        PvpNum = 0,
        Usable = 0, -- 0.不可领取  1.可领取  2.已领取
        Term = 0,
    }


    local b_crazy = false
    local cur_day = 0
    local gift_cfg = {}
    local term = 0
    b_crazy,cur_day, _,term = activity_crazy_open()
    if not b_crazy then
        return 0
    end

    --记录是狂欢周期数
    data.Term = term

    local crazy_data = skynet.call(redis_service, "lua", "hget_obj", "activity_crazy", account_info.uin,true)
    if crazy_data == nil then
        crazy_data = data
    end

    gift_cfg = get_gift_cfg(cur_day)
    if not gift_cfg then
        logger.trace("not found activity gift")
        return 0
    end


    --累积胜利场次
    if end_type == COMMON.pvp_end_type.WIN then
        crazy_data.PvpWin = crazy_data.PvpWin + 1
    end

    --新增pvp场次
    crazy_data.PvpNum = crazy_data.PvpNum + 1

    local UsableData = {
    }

    if crazy_data.Usable == 2 then
        logger.trace("crazy activity gift already usage ")
    else
        if crazy_data.PvpWin >= gift_cfg["pvpWin"] and crazy_data.Usable == 0 then
            crazy_data.Usable = 1
            UsableData.Usable = 1
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_crazy, account_info.uin, cur_day, COMMON.misc_action.act_task_com)
        elseif crazy_data.PvpNum >= gift_cfg["pvpNum"] and crazy_data.Usable == 0 then
            crazy_data.Usable = 1
            UsableData.Usable = 1
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_crazy, account_info.uin, cur_day, COMMON.misc_action.act_task_com)
        else
            logger.trace("boy come on,be crazy!!! ")
        end
    end

    UsableData.ConditionState = crazy_data.PvpWin
    UsableData.SubConditionState = crazy_data.PvpNum

    local extra_score = 0
    if crazy_data.Points ~= 0 and addscore ~= 0 then
        extra_score = math.floor(crazy_data.Points * 0.1 * addscore)
        crazy_data.Score = crazy_data.Score + extra_score
        --logger.tracef("crazy_data.Score is : %s  account_info.pvp_scroe is : %s\n", tostring(crazy_data.Score),tostring(account_info.pvp_score))
    end

    UsableData.ExtraScore = extra_score
    send_response(COMMON.MESSAGE.CS_CMD_ACTIVITY_CRAZY_USABLE, UsableData)

    skynet.call(redis_service, "lua", "hset_obj", "activity_crazy", account_info.uin,crazy_data,true)

    return extra_score
end

local function handle_activity_details(cmd, content, len)
    local resp_data = {}
    local b_crazy = false
    local cur_day = 0
    local time_str = ""
    b_crazy,cur_day,time_str = activity_crazy_open()
    if not b_crazy then
        logger.trace("not found activity for crazy")
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_DATA_ERROR)
    end

    --logger.logf("activity crazy day is[%d]",cur_day)
    --logger.logf("activity crazy time[%s]",time_str)

    local gift_cfg = {}
    if cur_day ~= 0 then
        resp_data.Day = cur_day
        gift_cfg = get_gift_cfg(cur_day)
        if gift_cfg == nil then
            logger.logf("not found gift configure for day[%d]",cur_day)
            return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_DATA_ERROR)
        else
            local crazy_data = skynet.call(redis_service, "lua", "hget_obj", "activity_crazy", account_info.uin,true)
            if crazy_data ~= nil then
                resp_data.Usable = crazy_data.Usable
                resp_data.Score = crazy_data.Score
                resp_data.Points = crazy_data.Points
                resp_data.RewardId = gift_cfg["rewardId"]
                resp_data.ConditionState = crazy_data.PvpWin
                resp_data.SubConditionState = crazy_data.PvpNum
                resp_data.ScoreAdd = crazy_data.Points * 10
            else
                --logger.logf("not found data in redis")
                resp_data.Usable = 0
                resp_data.Score = 0
                resp_data.Points = 0
                resp_data.RewardId = gift_cfg["rewardId"]
                resp_data.ConditionState = 0
                resp_data.SubConditionState = 0
            end
            resp_data.CrazyTime = time_str
            send_response(cmd, resp_data)
        end
    end
end

local function handle_activity_usage(cmd, content, len)
    local resp_data = {
    }
    local b_crazy = false
    local gift_id = 0
    local cur_day = 0


    b_crazy,cur_day = activity_crazy_open()
    if not b_crazy then
        logger.trace("not found activity data for crazy")
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_DATA_ERROR)
    end

    local gift_cfg = {}
    if cur_day ~= 0 then
        gift_cfg = get_gift_cfg(cur_day)
        if gift_cfg == nil then
            logger.logf("not found gift configure for day[%d]",cur_day)
            return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_DATA_ERROR)
        else
            local crazy_data = skynet.call(redis_service, "lua", "hget_obj", "activity_crazy", account_info.uin,true)
            if crazy_data ~= nil then
                if crazy_data.Usable == 2 then
                    return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_RECEIVE_ERROR)
                elseif (crazy_data.PvpWin >= gift_cfg["pvpWin"] or crazy_data.PvpNum >= gift_cfg["pvpNum"]) and crazy_data.Usable == 1 then
                    crazy_data.Usable = 2
                    gift_id = gift_cfg["rewardId"]
                    crazy_data.Points = crazy_data.Points + 1
                    resp_data.Points = crazy_data.Points
                    resp_data.ScoreAdd = crazy_data.Points * 10
                    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_crazy, account_info.uin, cur_day, COMMON.misc_action.act_task_submit)
                else
                    logger.logf("usage condition not match redis[PvpWin = %d,PvpNum = %d],gift cfg[pvpWin = %d, pvpNum = %d],uin[%d]",crazy_data.PvpWin,crazy_data.PvpNum,gift_cfg["pvpWin"],gift_cfg["pvpNum"],account_info.uin)
                end
                --更新redis
                skynet.call(redis_service, "lua", "hset_obj", "activity_crazy", account_info.uin,crazy_data)
            else
                logger.logf("not fount activity_crazy hash in redis for uin[%d]",account_info.uin)
            end
        end
    end

    if gift_id ~= 0 then
        player.add_gift(account_info, gift_id, COMMON.change_src.crazy)
        resp_data.GiftId = gift_id
        send_response(cmd, resp_data)
    end
end

function activity_crazy.get_score_add_up(score)
    local crazy_data = skynet.call(redis_service, "lua", "hget_obj", "activity_crazy", account_info.uin,true)
    if crazy_data then
        local rate = crazy_data.Points * 10
        return math.floor(score * rate / 100)
    else
        return 0
    end
end

function activity_crazy.get_open_activity(open_activity)
    --logger.trace("activity_crazy get_open_activity .....")
    local activity_data  = sharedata_utils.get_stat_all(COMMON.plan.ActivityCrazy)
    -- if not activity_data then return open_activity end
    local now_time = utils.getSameDayBeginTime()
    for _,v in pairs(activity_data) do
        local start_time = utils.get_time(v.start_time)
        local end_time = utils.get_time(v.end_time)
        -- logger.tracef("get end time is %s",end_time)
        if start_time <= now_time and now_time < end_time then
            local type = {
                Type = COMMON.activity_icon.ACT_CRAZY
            }
            table.insert(open_activity,type)
            break
        end
    end

    return open_activity
end

local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_GET_ACTIVITY_CRAZY_DETAIL] = handle_activity_details
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_ACTIVITY_CRAZY_USAGE] = handle_activity_usage
end

local function register_events()
    EventCenter.addEventListener(Events.DAY_CHANGE, daily_check)
end

function activity_crazy.init(args)
    redis_service = args.redis
    account_info = args.info
    agent = args.agent
    send_package = args.send_package
    busilog_service = args.busilog_service
    send_response   = args.send_response
    send_error_code = args.send_error_code

    -- 模块必须实现的接口
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)

    -- activity_config = get_crazy_config()
    discard_old()
    register_events()
end

return activity_crazy

---体力精力大放送活动

local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local COMMON = require "common"
local sharedata_utils = require "sharedata_utils"
local redis_service
local activity_power_energy = {}
local player = require "player"
local proto = require "proto"
local account_info
local agent

local send_package, send_response

local push_id = 0 -- 当前以推送的活动id 登录时 为0 符合要求就推送,推送完后改为当前推送的活动id
local thirty_minutes = 30 * 60
local check_time = 60 * 100 -- 60秒检查一次
-- 为了方便以后的重加载，这里不做活动数据的备份，所有每次都要从sharedata取数据
local function push_activity_data()
    skynet.timeout(check_time, push_activity_data)
    local activity_data = sharedata_utils.get_stat_all(COMMON.plan.Power_Energy_Activity)
    if activity_data == nil then return end
    local status = 0
    local isOpen = 0
    local cmdid = COMMON.MESSAGE.CS_CMD_GET_POWER_ENERGY_ACTIVITY_DATA
    for k,v in pairs(activity_data) do
        local now_time = math.floor(os.time())
        local ymd = utils.get_yyyyMMdd_time()
        -- 活动开始时间
        local start_time = utils.get_time(ymd..v.start_time)
        -- 活动结束时间
        local end_time = utils.get_time(ymd..v.end_time)
        -- 推送活动开启消息
        if now_time + thirty_minutes >= start_time and now_time < end_time then
            if push_id == v.id then return end
            local power_energy_data = skynet.call(redis_service, "lua", "hget_obj", "power_energy_activity", account_info.uin)
            local startTime = 0--倒计时
            local endTime = end_time - now_time
            if now_time - start_time < 0 then
                startTime = math.abs(now_time - start_time)
            end
            if power_energy_data == nil  or utils.is_same_day(power_energy_data.time) and v.id ~= power_energy_data.id then
                status = 1
            end
            if startTime > 0 then -- 不在活动期间显示 为已领取状态
                status = 0
            end
            -- 同一个活动在线时只推送一次
            push_id = v.id
            isOpen= 1
            local resp = {
                Id = v.id,
                IsOpen = isOpen, --活动是否开启 0 默认未开启 1 已开启 2 活动没开启但需显示图标
                Status = status, -- 玩家活动领取状态     0 活动已领取，1 活动未领取
                StartTime = startTime,
                EndTime = endTime,
            }
            send_response(cmdid, resp)
        end

        -- 推送活动关闭消息  push_id 不等于0说明有活动开启了。
        if push_id ~= 0  then
            -- 当前时间大于结束时间了 活动已经结束了
            if now_time > end_time and push_id == v.id then
                -- 同一个活动在线时只推送一次
                push_id = 0
                isOpen = 2
                local id = 1
                local resp = {
                    Id = id,
                    IsOpen = isOpen, --活动是否开启 0 默认未开启 1 已开启 2 活动没开启但需显示图标
                    Status = 0, -- 玩家活动领取状态     0 活动已领取，1 活动未领取
                    StartTime = 0,
                    EndTime = 0,
                }
                send_response(cmdid, resp)
            end
        end
    end
end

function activity_power_energy.get_open_activity(open_activity)
    local activity_data  = sharedata_utils.get_stat_all(COMMON.plan.Power_Energy_Activity)
    if not activity_data then return open_activity end
    local now_time = math.floor(os.time())
    local ymd = utils.get_yyyyMMdd_time()
    for _,v in pairs(activity_data) do
        local start_time = utils.get_time(ymd..v.start_time)
        local end_time = utils.get_time(ymd..v.end_time)
        if start_time - thirty_minutes <= now_time and now_time < end_time then
            local type = {
                Type = COMMON.activity_icon.ACT_POWER_ENERGY
            }
            table.insert(open_activity,type)
        end
    end
    return open_activity
end

-- 领取类型 1 体力 2 精力
local function receive_reward(id, reward_type, account_info)
    local ret = COMMON.ERRORCODE.OK
    local activity_data = sharedata_utils.get_stat(COMMON.plan.Power_Energy_Activity,id)
    if not activity_data then
        ret = COMMON.ERRORCODE.ACTIVITY_NOT_RECEIVE_ROCODE_ERROR
        return ret
    end
    local ymd = utils.get_yyyyMMdd_time()
    local now_time = math.floor(os.time())
    local start_time = utils.get_time(ymd..activity_data.start_time)
    local end_time = utils.get_time(ymd..activity_data.end_time)
    if now_time < start_time then
        ret = COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR
    end

    if now_time > end_time then
        ret = COMMON.ERRORCODE.ACTIVITY_END_ERROR
    end
    if ret ~= COMMON.ERRORCODE.OK then
        return ret
    end
    local power_energy_data = skynet.call(redis_service, "lua", "hget_obj", "power_energy_activity", account_info.uin)
    if not power_energy_data  then
        power_energy_data = {}
    end

    if utils.is_same_day(power_energy_data.time) and id == power_energy_data.id then
        ret = COMMON.ERRORCODE.ACTIVITY_RECEIVE_ERROR
        return ret
    end

    if reward_type == 1 then
        player.add_power(account_info, 50)
    end
    power_energy_data.id = id
    power_energy_data.time = math.floor(os.time())
    power_energy_data.type = reward_type
    logger.tracef("%d receive id(%d) power_energy(%d) success ", account_info.uin, id, reward_type)
    skynet.call(redis_service, "lua", "hset_obj", "power_energy_activity", account_info.uin, power_energy_data)
    return ret
end

local thirty_minutes = 30 * 60
function activity_power_energy.handle_get_power_energy_activity(cmd)
    local activity_data = sharedata_utils.get_stat_all(COMMON.plan.Power_Energy_Activity)
    if activity_data == nil then return end
    local now_time = math.floor(os.time())
    local ymd = utils.get_yyyyMMdd_time()

    local power_energy_data = skynet.call(redis_service, "lua", "hget_obj", "power_energy_activity", account_info.uin, true)
    local id = 0
    local status = 1 -- 默认未领取
    local startTime = 0 -- 倒计时
    local endTime = 0
    local isOpen = 0
    for _,v in pairs(activity_data) do
        local start_time = utils.get_time(ymd..v.start_time)
        local end_time = utils.get_time(ymd..v.end_time)
        if now_time + thirty_minutes >= start_time and now_time < end_time then
            id = v.id
            endTime = end_time - now_time
            if now_time - start_time < 0 then
                startTime =math.abs(now_time - start_time)
            end
            isOpen = 1
            if power_energy_data and utils.is_same_day(power_energy_data.time) and v.id == power_energy_data.id then
                status = 0
            end
            if startTime > 0 then 
                status = 0
            end
        end
        if isOpen == 0 then
            -- 写死的一个逻辑
            if now_time < start_time and v.id == 1 then  -- 当前时间小于活动开始时间，
                id = 1
            end

            if now_time > end_time and v.id == 2 then
                id = 1
            end

            if id == 0 then
                id = 1
            end
        end
    end  
    if isOpen == 0 then 
        isOpen = 2
        status = 0
        startTime = 0
        endTime = 0
    end
    
    local resp = {
        Id = id,
        IsOpen = isOpen,
        Status = status, -- 玩家活动领取状态
        StartTime = startTime,
        EndTime = endTime,
    }
    send_response(cmd, resp)
end


function activity_power_energy.handle_receive_power_energy_reward(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local id = decode.Id
    local type = decode.Type
    local ret = receive_reward(id, type, account_info)
    local resp = {
        Ret = ret,
        Type = type,
    }
    send_response(cmd, resp)
end


local function add_cmd(CMD)

end

local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_RECEIVE_POWER_ENERGY_REWARD] = activity_power_energy.handle_receive_power_energy_reward
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_GET_POWER_ENERGY_ACTIVITY_DATA] = activity_power_energy.handle_get_power_energy_activity
end

function activity_power_energy.init(args)
    redis_service = args.redis
    account_info = args.info
    agent = args.agent
    send_package = args.send_package
    send_response = args.send_response
    -- 模块必须实现的接口
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)

    push_activity_data()
end

return activity_power_energy


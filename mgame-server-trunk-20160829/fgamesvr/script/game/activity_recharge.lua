-- 充值活动
local skynet = require "skynet"
local utils = require "utils"
local COMMON = require "common"
local proto = require "proto"
local sharedata_utils = require "sharedata_utils"
local ERRORCODE = COMMON.ERRORCODE

local redis_service
local send_response, send_error_code
local account_info, player
local busilog_service

local activity_recharge = {}

local first_type = 1 -- 首充

local recharge_cfg

local function get_recharge_config()
    if not recharge_cfg then
        recharge_cfg = sharedata_utils.get_stat_all(COMMON.plan.Recharge_Activity)
        for _, cfg in pairs(recharge_cfg) do
            cfg.start_time = utils.get_time(cfg.start_time)
            cfg.end_time = utils.get_time(cfg.end_time)
        end
    end

    return recharge_cfg
end

-- 获取当前开启的活动
function activity_recharge.get_open_activity(account_info, open_activity)
    local recharge_cfg = get_recharge_config()
    for _, cfg in pairs(recharge_cfg) do
        local start_time = cfg.start_time
        local end_time = cfg.end_time
        local now_time = math.floor(os.time())
        if now_time >= start_time and now_time < end_time then
            if cfg.recharge_type == first_type then
                local activity_info = skynet.call(redis_service, "lua", "hget_obj", "activity_info", account_info.uin, true) --玩家领取活动记录数据 key为uin
                if activity_info == nil then
                    table.insert(open_activity, {Type = COMMON.activity_icon.ACT_FIRST_CHARGE })
                else
                    local key = account_info.uin .. "_" .. cfg.id .. "_" .. cfg.stage
                    if activity_info[key] and activity_info[key].status == 1 then
                        table.insert(open_activity, {Type = COMMON.activity_icon.ACT_FIRST_CHARGE })
                    end
                end
            end
        end
    end
    return open_activity
end


-- 是否有首充奖励可以领取
local function first_recharge()
    local recharge_cfg = get_recharge_config()
    local activity_info = skynet.call(redis_service, "lua", "hget_obj", "activity_info", account_info.uin, true) --玩家领取活动记录数据 key为uin
    if not activity_info then
        activity_info = {}
    end
    
    -- 活动时间验证
    for _, cfg in pairs(recharge_cfg) do
        local start_time = cfg.start_time
        local end_time = cfg.end_time
        local now_time = math.floor(os.time())
        if now_time > start_time and now_time < end_time then
            if cfg.recharge_type == first_type then
                local key = account_info.uin .. "_" .. cfg.id .. "_" ..  cfg.stage
                if not activity_info[key] then
                    local activity = {
                        id = cfg.id,
                        stage = cfg.stage,
                        reward = cfg.reward,
                        end_time = math.floor(os.time()), --最后有效期就是天时间
                        status = 1, -- 未领取
                    }
                    activity_info[key] = activity
                    skynet.call(redis_service, "lua", "hset_obj", "activity_info", account_info.uin, activity_info)
                    account_info.first_recharge_time = os.time()
                    player.change_flag(account_info, true)
                end
            end
        end
    end
end

-- 充值回调，主要用来首充奖励
function activity_recharge.on_recharge(account_info)
    if redis_service == nil then
        redis_service = skynet.queryservice("redis_service")
    end
    first_recharge()
end

-- 获取充值活动信息(暂时只有首充)
local function get_activity_data(account_info, activity_data)
    local activity_info = skynet.call(redis_service, "lua", "hget_obj", "activity_info", account_info.uin, true) --玩家领取活动记录数据 key为uin
    local recharge_cfg = get_recharge_config()
    if recharge_cfg == nil then
        return activity_data
    end
    if activity_info == nil then
        activity_info = {}
    end

    local recharge_count = 0
    local day_count = 0
    local now_time = math.floor(os.time())
    for _,cfg in pairs(recharge_cfg) do
        repeat
            local start_time = cfg.start_time
            local end_time = cfg.end_time
            if now_time > start_time and now_time < end_time then
                local key = account_info.uin .. "_" .. cfg.id .. "_" .. cfg.stage
                local activity_type
                if cfg.recharge_type == first_type then
                    activity_type = COMMON.activity_icon.ACT_FIRST_CHARGE
                end

                local status = 2
                if activity_info[key] then
                    status = activity_info[key].status
                end
                local activity = {
                    Id = cfg.id,
                    Stage = cfg.stage,
                    Type = activity_type,
                    Status = status,
                    Gift = cfg.reward,
                    StartTime = utils.datetime2string(cfg.start_time),
                    EndTime = utils.datetime2string(cfg.end_time),
                    Recharge_num = cfg.recharge_num,
                }
               table.insert(activity_data,activity)
            end
        until true
    end
    return activity_data, day_count ,recharge_count
end

-- 获取活动数据
function activity_recharge.handle_get_activity_data(cmd, content, len)
    local activity_data = {}
    local activity_data, day_count ,recharge_count = get_activity_data(account_info, activity_data)
    local resp = {
        DayCount = day_count,
        RechargeCount = recharge_count,
        ActivityInfo = activity_data
    }
    send_response(cmd, resp)
end

-- 领取充值相关活动奖励
local function handle_receive_activity_reward(cmd, content, len)
    local ret = ERRORCODE.OK
    local decode = proto.parse_proto_req(cmd, content, len)
    local id = decode.Id
    local stage = decode.Stage
    local type = decode.Type
    local activity_info = skynet.call(redis_service, "lua", "hget_obj", "activity_info", account_info.uin) --玩家领取活动记录数据 key为uin
    if activity_info then
        for _,activity in pairs(activity_info) do
            if activity.id == id and activity.stage == stage then
                if activity.status == 1 then
                    -- 领取奖励
                    local reward = activity.reward
                    player.add_gift(account_info, reward, COMMON.change_src.charge)
                    activity.status = 0
                    skynet.call(redis_service, "lua", "hset_obj", "activity_info", account_info.uin, activity_info)
                    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_recharge_reward, account_info.uin, type, activity.id)
                    break
                else
                    ret = ERRORCODE.ACTIVITY_RECEIVE_ERROR
                end
            end
        end
    else
        ret = ERRORCODE.ACTIVITY_NOT_RECEIVE_ROCODE_ERROR
    end

    local resp = {
        Ret = ret,
        Type = type,
        Id = id,
    }

    send_response(cmd, resp)
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(handle_mapping)
    handle_mapping[COMMON.MESSAGE.CS_CMD_GET_ACTIVITY_DATA]       = activity_recharge.handle_get_activity_data
    handle_mapping[COMMON.MESSAGE.CS_CMD_RECEIVE_ACTIVITY_REWARD] = handle_receive_activity_reward
end

local function add_cmd(cmd)
end

function activity_recharge.init(args)
    redis_service = args.redis
    send_response = args.send_response
    account_info  = args.info
    player        = args.player

    busilog_service = args.busilog_service
    recharge_cfg = get_recharge_config()
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)
end

return activity_recharge

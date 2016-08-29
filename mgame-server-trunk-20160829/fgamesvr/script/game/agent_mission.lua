local skynet = require "skynet"
local proto = require "proto"
local logger = require "logger"
local sharedata_utils = require "sharedata_utils"
local utils = require "utils"
local COMMON = require "common"
local player = require "player"
local agent_vip = require "agent_vip"

local redis_service
local account_info
local missions
local redis_missions
local agent_mission = {}
local mission_config
local CMD

local send_package, send_response, send_error_code

--获得任务详细配置
local function template(mission_id)
    return mission_config[mission_id]
end

local function accept_mission(mission_id)
    local mission = {}
    mission.upvalue = 0
    mission.completed = false
    mission.id = mission_id
    missions[mission_id] = mission
end

local function insert_mission(mission_id, usable)
    local mission = {}
    if usable then
        mission.upvalue = 1 -- 当前完成任务的进度
    else
        mission.upvalue = 0
    end
    mission.completed = false

    mission.id = mission_id
    missions[mission_id] = mission
end


local function delete_mission(mission_id)
    missions[mission_id] = nil
end

local function iscomplete(mission_id)
    local mission = missions[mission_id]
    if not mission then
        return false
    end
    return mission.upvalue >= template(mission_id).value
end

local function update_mission(mission,value)
    --连胜比较特殊，如果此次value小于1 说明没有连胜 需要重置为0
    if value <= 0 and template(mission.id).type == COMMON.mission_type.STREAK_WIN then
        mission.upvalue = 0
    else
        mission.upvalue = mission.upvalue + value
    end
    if mission.upvalue > template(mission.id).value then
        mission.upvalue = template(mission.id).value
    end
    local data = {
        MissionId = mission.id,
        Value = mission.upvalue
    }
    send_response(COMMON.MESSAGE.CS_CMD_MISSION_UPDATE, data)
end

local function save()
    skynet.call(redis_service, "lua", "hset_obj", "mission", account_info.uin, redis_missions)
end

local function update_missions(update_para)
    for k,para in pairs(update_para) do
        local mission_type,require,value = table.unpack(para)
        for id,mission in pairs(missions) do
            if template(id).type == mission_type and not iscomplete(id) then
                --如果该类型没有特殊需求或者有但是达到了条件要求
                if not template(id).require or template(id).require == 0 or template(id).require == require then
                    update_mission(mission, value)
                end
            end
        end
    end
    save()
end

local function get_missionlist_data()
    local data = {}
    local data_missions = {}
    for id,mission in pairs(missions) do
        if not mission.completed then
            mission.completed = false
        end

        table.insert(data_missions, {
            MissionId = id,
            Value = mission.upvalue,
            Completed = mission.completed,
            Request = template(mission.id).value,
            GiftId = template(mission.id).gift,
        })
    end
    data.Missions = data_missions
    data.MissionDay = redis_missions.last_refresh_time
    return data
end


-- vip日常刷新 同级别的会员已经领取后不能再领取
local function refresh_mission(mission_id, usage)
    
    if not missions then
      return
    end

    -- 同级别的会员已经领取不能再领取
    if missions[mission_id] and missions[mission_id].completed then
        return
    end

    -- 直接清除先前已经有的会员
    if missions[COMMON.mission_type.VIP1] then
        missions[COMMON.mission_type.VIP1] = nil
    end

    if missions[COMMON.mission_type.VIP2] then
        missions[COMMON.mission_type.VIP2] = nil
    end

    if missions[COMMON.mission_type.VIP3] then
        missions[COMMON.mission_type.VIP3] = nil
    end

    -- usage false 代表重置 true 代表会员礼包可以领取
    if usage then
        insert_mission(mission_id, usage)
    else
        insert_mission(COMMON.mission_type.VIP1, usage)
    end

    local data = get_missionlist_data()
    send_response(COMMON.MESSAGE.CS_CMD_MISSION_LIST, data)
    save()
end


--把vip礼包任务加到每日任务列表里
local function init_vip_mission()
    local vip_data = agent_vip.get_vip_data()
    if vip_data then
        local id = 0
        if vip_data.vip_level == 1 then
            id = COMMON.mission_type.VIP1
        elseif vip_data.vip_level == 2 then
            id = COMMON.mission_type.VIP2
        else
            id = COMMON.mission_type.VIP3
        end
        insert_mission(tonumber(id), true)
    else
        insert_mission(tonumber(COMMON.mission_type.VIP1), false)
    end
end

local function init_mission()
    redis_missions = {}
    redis_missions.missions = {}
    missions = redis_missions.missions
    for id,mission in pairs(mission_config) do
        if id == COMMON.mission_type.VIP1 or id == COMMON.mission_type.VIP2 or id == COMMON.mission_type.VIP3 then
        else
            accept_mission(id)
        end
    end
    init_vip_mission()
    redis_missions.last_refresh_time = math.modf(os.time())
    save()
end

-- 添加agent回包
local function add_cmd(CMD)
    CMD.update_missions  = update_missions
    CMD.refresh_mission = refresh_mission
end

--完成任务。领取任务奖励
local function handle_mission_complete(cmd, content, len)
    local mission_id = proto.parse_proto_req(cmd, content, len).MissionId
    logger.debug("recv mission reward",mission_id)
    local mission = missions[mission_id]
    if not mission then
         logger.debug("mission doesnt exist",mission_id)
         return send_error_code(cmd, COMMON.ERRORCODE.MISSION_NOT_EXIST)
    end

    if mission.completed then
          logger.debug("mission has already completed",mission_id)
          return send_error_code(cmd, COMMON.ERRORCODE.MISSION_ALREADY_COMPLETED)
    end

    if mission.upvalue < template(mission_id).value then
          logger.debug("mission doesnt finished",mission_id)
          return send_error_code(cmd, COMMON.ERRORCODE.MISSION_NOT_SATISFY_CONDITION)
    end

    local vip_data = agent_vip.get_vip_data()
    if (mission_id == COMMON.mission_type.VIP1 or mission_id == COMMON.mission_type.VIP2 or mission_id == COMMON.mission_type.VIP3) and
           not vip_data then
        return send_error_code(cmd, COMMON.ERRORCODE.VIP_NOT_EXIST)
    end

    local gift_id = template(mission_id).gift
    if gift_id then
        player.add_gift(account_info, gift_id, COMMON.change_src.mission)
        mission.completed = true
        save()
    else
        logger.debugf("not found gift_id for mission : %s", tostring(mission_id))
        return
    end


    local data = {
        Mission = {
            MissionId = mission_id,
            Completed = mission.completed,
            GiftId = template(mission_id).gift,
        }
    }
    send_response(cmd, data)
end

local function handle_mission_list(cmd, content, len)
    local data = get_missionlist_data()
    send_response(cmd, data)
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_MISSION_COMPLETE] = handle_mission_complete
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_MISSION_LIST] = handle_mission_list
end

function agent_mission.init(args)
    account_info = args.info
    CMD = args.cmd

    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code

    -- 模块必须实现的接口
    add_cmd(CMD)
    add_cmd_handle_mapping(args.cmd_map)
    mission_config = sharedata_utils.get_stat_all(COMMON.plan.Mission)
    redis_service = args.redis
    redis_missions = skynet.call(redis_service, "lua", "hget_obj", "mission", account_info.uin)
    if not redis_missions then
        init_mission()
    end
end

function agent_mission.daily_check(notice_flag)
    local tnow = math.modf(os.time())
    local days = utils.getTimeInteral(redis_missions.last_refresh_time, tnow,0)
    if days ~= 0 then
        init_mission()
        if account_info.month_card_time > tnow then
            local update_data = {}
            table.insert(update_data, {COMMON.mission_type.MONTH_CARD, 0, 1})  --完成一次关卡
            CMD.update_missions(update_data)
        end
        --如果需要通知则通知客户端
        if notice_flag then
            local data = get_missionlist_data()
            send_response(COMMON.MESSAGE.CS_CMD_MISSION_LIST, data)
        end
    else
        --检查任务是否存在
        for id,mission in pairs(redis_missions.missions) do
            if not template(id) then
                redis_missions.missions[id] = nil
            end
        end
        missions = redis_missions.missions
    end
end

return agent_mission

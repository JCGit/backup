    
--抢红包活动
--
--auth : diandian
--date : 2016-01-08


local skynet = require "skynet"
local player = require "player"
local logger = require "logger"
local COMMON = require "common"
local proto = require "proto"
local utils = require "utils"
local json = require "cjson"

local redis_service

local account_info
local send_package, send_error_code, send_response

local need_check_time_heart

local act_info
local greb_red_packet_service

--判断当前时间活动有没有开启
local function check_opened()
    if act_info then
        return act_info.is_opened
    end
    return false
end

local function get_red_packet_factory()
    local packet_info_redis_str = skynet.call(redis_service, "lua", "hget", "red_packet_info", "factory")
    if packet_info_redis_str then
        return json.decode(packet_info_redis_str)
    end
    return nil
end

local function create_handle_get_info_resp()
    local resp_data = {
        status = 0,
        value = 0,
        time = 0,
    }

    act_info = skynet.call(greb_red_packet_service, "lua", "get_cur_act_info")
    if check_opened() == false then
        return resp_data
    end

    local fixed_info = act_info.fixed_info
    if not fixed_info then
        return resp_data
    end

    --如果没有step表示活动刚开启,准备生成第一批红包
    if not fixed_info.space_step then
        if fixed_info.space_time  and fixed_info.space_time[1] then
            resp_data.status = 2
            resp_data.time = act_info.start_time + fixed_info.space_time[1]
        end
        return resp_data
    end

    local player_status = skynet.call(redis_service, "lua", "hget_obj", "player_red_packet_info", account_info.uin)
    if player_status and player_status[fixed_info.space_step] then
        --玩家当前红包已抢, 显示下一个红包时间
        if fixed_info.space_step < #fixed_info.space_time then
            resp_data.time = act_info.start_time + fixed_info.space_time[fixed_info.space_step + 1]
        else
            resp_data.time = 1
        end
        resp_data.value = player_status[fixed_info.space_step]
        return resp_data
    else
        local packet_factory = get_red_packet_factory()
        if packet_factory and packet_factory.count > 0 then
            resp_data.value = packet_factory.count
            resp_data.status = 1
        else
            if fixed_info.space_step < #fixed_info.space_time then
                resp_data.time = act_info.start_time + fixed_info.space_time[fixed_info.space_step + 1]
            else
                resp_data.time = 1
            end
        end
        return resp_data
    end
end

local function handle_get_info(msg, content, len)
    local resp_data = create_handle_get_info_resp()
    send_response(msg, resp_data)
end


local function create_handle_greb_packet_resp()
    local resp_data = {
        ret = 0,
        value = 2,
    }

    act_info = skynet.call(greb_red_packet_service, "lua", "get_cur_act_info")
    if not act_info or not act_info.fixed_info or not act_info.fixed_info.space_step then
        return resp_data
    end

    local fixed_info = act_info.fixed_info
    local player_status = skynet.call(redis_service, "lua", "hget_obj", "player_red_packet_info", account_info.uin)
    if player_status and player_status[fixed_info.space_step] then
        resp_data.value = 1
        return resp_data
    end

    local packet_factory = get_red_packet_factory()
    if not packet_factory or packet_factory.count <= 0 then
        resp_data.value = 1
        return resp_data
    end

    local status = skynet.call(greb_red_packet_service, "lua", "greb_packet")
    if status ~= 0 then
        local diamond_num = status
        player.change_diamond(account_info, COMMON.change_src.red_pkg, diamond_num)
        player_status = player_status or {}
        player_status[fixed_info.space_step] = diamond_num
        skynet.call(redis_service, "lua", "hset_obj", "player_red_packet_info", account_info.uin, player_status)
        resp_data.ret = 1
        resp_data.value = diamond_num
    else
        resp_data.ret = 0
        resp_data.value = 2
    end

    return resp_data
end

local function handle_greb_packet(msg, content, len)
    local resp_data = create_handle_greb_packet_resp()
    send_response(msg, resp_data)
    handle_get_info()
end

local function handle_opened(msg, content, len)
    local opened_resp = {
        startTime   = 0,
        endTime     = 0,
    }
    act_info = skynet.call(greb_red_packet_service, "lua", "get_cur_act_info")
    if act_info then
        local opened_resp = {
            startTime   = act_info.start_time,
            endTime     = act_info.end_time,
        }
    end
    send_response(msg,  opened_resp)
end

local agent_greb_red_packet = {}

function agent_greb_red_packet.init(args)
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code
    account_info = args.info
    redis_service = args.redis

    args.cmd_map[COMMON.MESSAGE.CS_CMD_RED_PACKETS_GREB] = handle_greb_packet
    args.cmd_map[COMMON.MESSAGE.CS_CMD_RED_PACKETS_OPENED] = handle_opened
    args.cmd_map[COMMON.MESSAGE.CS_CMD_RED_PACKETS_GET_INFO] = handle_get_info

    greb_red_packet_service = skynet.queryservice("greb_red_packet_service")
end

function agent_greb_red_packet.on_open(act_info_)
    act_info = act_info_
    handle_get_info()
end

function agent_greb_red_packet.on_end(act_info_)
    act_info = nil
    handle_get_info()
end

function agent_greb_red_packet.on_update(act_info_)
    act_info = act_info_
end

return agent_greb_red_packet

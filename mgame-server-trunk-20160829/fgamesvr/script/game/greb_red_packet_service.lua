
--抢红包活动服务
--
--auth : diandian
--date : 2016.01.12
--

local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local sharedata_utils = require "sharedata_utils"
local COMMON = require "common"
local cluster = require "cluster"
local json = require "cjson"

local redis_service
local activitys_service
local act_info

local red_packet_info
local ratios_info

local function start_greb_packet()
    local lua_script = [[
        local info = cjson.decode(redis.call("hget", "red_packet_info", "factory"))

        info.count = tonumber(info.count)
        if info and info.count > 0 then
            info.count = info.count - 1
            redis.call("hset", "red_packet_info", "factory", cjson.encode(info))
            return 1
        else
            return 0
        end
    ]]

    return skynet.call(redis_service, "lua", "eval", lua_script, 1, "red_packet_info")
end

local function use_a_limit_count_ratio(id)
    local lua_script = [[
        local id = KEYS[1]
        local info = cjson.decode(redis.call("hget", "red_packet_info", "ratios"))
        if info[id] then
            info[id] = tonumber(info[id])
            if info[id] > 0 then
                info[id] = info[id] - 1
                redis.call("hset", "red_packet_info", "ratios", cjson.encode(info))
                return 1
            else
                return 0
            end
        else
            return 0
        end
    ]]

    return skynet.call(redis_service, "lua", "eval", lua_script, 1, id)
end

local function create_ratio_info(packet_id)
    if not ratios_info then
        local pack_cf = sharedata_utils.get_stat(COMMON.plan.ActivityRedPackets, packet_id)
        if not pack_cf then
            return 0
        end
        local ratio_s = {}
        local num = 0
        for i, v in pairs(pack_cf.ratio) do
            local ratio_cf = sharedata_utils.get_stat(COMMON.plan.RedPacketRatio, v)
            if ratio_cf then
                local obj = {
                    start_num = num,
                    end_num = num + ratio_cf.ratio,
                    cfg = ratio_cf,
                }
                num = num + ratio_cf.ratio
                table.insert(ratio_s, obj)
            end
        end

        ratios_info = {
             ratios = ratio_s,
             ratio_total = num,
        }
    end
end

local function random_a_ratio_cfg(pack_id)
    create_ratio_info(pack_id)
    if not ratios_info then
        return nil
    end
    local random_num = math.random(0, ratios_info.ratio_total)
    print("random : ratio total:", random_num, ratios_info.ratio_total)
    local cur_ratio_obj
    local cur_index
    for i, v in pairs(ratios_info.ratios) do
        if random_num >= v.start_num and random_num <= v.end_num then
            cur_ratio_obj = v
            cur_index = i
            break
        end
    end

    if cur_ratio_obj then
        if cur_ratio_obj.cfg.limit_count > 0 then
            local ret = use_a_limit_count_ratio(cur_ratio_obj.cfg.id)
            if ret == 1 then
                return cur_ratio_obj.cfg
            else
                return random_a_ratio_cfg()
            end
        else
            return cur_ratio_obj.cfg
        end
    end
    return nil
end


local function clear_red_packet_info()
    skynet.call(redis_service, "lua", "del", "red_packet_info")
    skynet.call(redis_service, "lua", "del", "player_red_packet_info")
end

--创建红包，需要center服创建
local function create_red_packet(id, count)
    local pack_cf = sharedata_utils.get_stat(COMMON.plan.ActivityRedPackets, id)
    if not pack_cf then
        return
    end

    local ratios_count = {}
    for i, v in pairs(pack_cf.ratio) do
        local ratio_cf = sharedata_utils.get_stat(COMMON.plan.RedPacketRatio, v)
        if ratio_cf and ratio_cf.limit_count > 0 then
            ratios_count[tostring(ratio_cf.id)] = ratio_cf.limit_count
        end
    end
    cluster.call("centernode", ".red_packet_mgr_service", "create_red_packet", id, count, ratios_count)
end


local function get_cur_space_step(fixed_info)
    if fixed_info.space_time then
        for i = 1, #fixed_info do
            local config = all_packet_config[i]
            local pack_time = act_info.start_time + fixed_info.space_time[i]
            if now <= pack_time then
                return i
            end
        end
    end
    return 0
end

local function get_red_packet_factory()
    local packet_info_redis_str = skynet.call(redis_service, "lua", "hget", "red_packet_info", "factory")
    if packet_info_redis_str then
        return json.decode(packet_info_redis_str)
    end
    return nil
end

local CMD = {}

function CMD.get_cur_act_info()
    return act_info
end

--抢红包
function CMD.greb_packet()

    if not act_info or not act_info.is_opened
        or not act_info.fixed_info
        or not act_info.fixed_info.space_step then
        return 0
    end
    local ret = start_greb_packet()
    if ret == 0 then return 0 end

    local ratio_cfg = random_a_ratio_cfg(act_info.fixed_info.space_step)
    if ratio_cfg then
        local random_num = math.random(ratio_cfg.min_num, ratio_cfg.max_num)
        return random_num
    end
    return 0
end

function CMD.notify_act_open(act_info_)
    act_info = act_info_

    if act_info.fixed_info and act_info.fixed_info.space_step then
        local packet_info_redis = get_red_packet_factory()
        if packet_info_redis and packet_info_redis.id == act_info.fixed_info.space_step then
        else
            clear_red_packet_info()
            local pack_config = sharedata_utils.get_stat(COMMON.plan.ActivityRedPackets,
                                                            act_info.fixed_info.space_step)
            create_red_packet(act_info.fixed_info.space_step, pack_config.count)
        end
    end

    if not act_info.fixed_info then
        --向时间管理服务注册upadte更新步骤
        local all_packet_config = sharedata_utils.get_stat_all(COMMON.plan.ActivityRedPackets)
        local now = os.time()
        local fixed_info = {
            clock_time = {},
            space_time = {},
        }
        for k, v in pairs(all_packet_config) do
            table.insert(fixed_info.space_time, v.time_space)
        end
        if not act_info.fixed_info then
            act_info.fixed_info = fixed_info
        end
        act_info.fixed_info = fixed_info
        skynet.call(activitys_service, "lua", "register_fixed", COMMON.activity_id.ACT_RANK_WEEK_AWARD, fixed_info)
    end
end


function CMD.notify_act_update(act_info_, update_type)
    act_info = act_info_
    clear_red_packet_info()

    local pack_config = sharedata_utils.get_stat(COMMON.plan.ActivityRedPackets, act_info.fixed_info.space_step)
    create_red_packet(act_info.fixed_info.space_step, pack_config.count)
end

function CMD.notify_act_end(act_info_)
    act_info = nil
    clear_red_packet_info()
end

local traceback = debug.traceback
skynet.start(function()
    redis_service = skynet.queryservice("redis_service")
    activitys_service = skynet.queryservice("activitys_service")

    skynet.dispatch("lua", function(_, _, command, ...)
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
end
)


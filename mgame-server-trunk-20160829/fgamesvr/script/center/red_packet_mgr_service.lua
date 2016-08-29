

--红包总管理服务
--
--auth : diandian
--date : 2016.01.13
--

local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local queue = require "skynet.queue"
local json = require "cjson"
local COMMON = require "common"

local cs = queue()
local redis_service
local factory_info

local is_need_reduction_check = false
local is_start_reduction= false
local reduction_frame_time  = 0

local function reduce_packet_count()
    local lua_script = [[
        local ratio = KEYS[1]

        local json_str = redis.call("hget", "red_packet_info", "factory")
        if json_str then
            local info = cjson.decode(json_str)
            info.count = tonumber(info.count)
            if info and info.count > 0 then
                info.count = info.count - math.ceil(info.count * (ratio/100))
                if info.count < 10 then
                    info.count = 0
                end
                redis.call("hset", "red_packet_info", "factory", cjson.encode(info))
            end
            return info.count
        end
        return 0
    ]]

    --衰减比列 8%-15%
    local random_num = math.random(8, 15)
    return skynet.call(redis_service, "lua", "eval", lua_script, 1, random_num)
end

local function check_need_reduction()
    reduction_frame_time = reduction_frame_time + 2

    if is_start_reduction then
        --每30s开始衰减一次
        if reduction_frame_time >= 30 then
            reduction_frame_time = 0
            local count = reduce_packet_count()
            if count == 0 then
                is_need_reduction_check = false
            end
        end

    else
        --10分钟后开始衰减
        if reduction_frame_time >= 600 then
            is_start_reduction = true
            reduction_frame_time = 0
        end
    end

    if is_need_reduction_check then
        skynet.timeout(200, check_need_reduction)
    end
end

local function create_red_packet(id, count, ratios_count)
    local jstr = skynet.call(redis_service, "lua", "hget", "red_packet_info", "factory")

    if jstr ~= nil then
        factory_info = json.decode(jstr)
    else
        factory_info = nil
    end

    if factory_info and factory_info.id == id then
        return
    end

    factory_info = {
        id = id,
        count = count,
    }
    skynet.call(redis_service, "lua", "hset", "red_packet_info", "factory", json.encode(factory_info))
    skynet.call(redis_service, "lua", "hset", "red_packet_info", "ratios", json.encode(ratios_count))

    is_need_reduction_check = true
    reduction_frame_time = 0
    is_start_reduction = false
    check_need_reduction()
end


local CMD = {
}

function CMD.create_red_packet(id, count, ratios_count)
    cs(create_red_packet, id, count, ratios_count)
end


local traceback = debug.traceback
skynet.start(function()
    redis_service = skynet.queryservice("redis_service")

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


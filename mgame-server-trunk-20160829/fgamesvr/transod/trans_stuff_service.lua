--厨艺材料老数据转换
--
--
--auth:diandian
--date:2016.4.6


local skynet = require "skynet"
local logger = require "logger"
local utils = require "utils"


local redis_service 

local trans_ids = {}

trans_ids[100001] = 100100
trans_ids[100002] = 100100
trans_ids[100003] = 100100
trans_ids[100004] = 100100
trans_ids[100005] = 100100
trans_ids[100008] = 100100
trans_ids[100009] = 100100
trans_ids[100014] = 100100
trans_ids[100015] = 100100
trans_ids[100016] = 100100
trans_ids[100017] = 100100
trans_ids[100018] = 100100

local function combine(t, id, v)
    if trans_ids[id] then
        id = trans_ids[id]
    end

    if not t[id] then
        t[id] = {
            count = 0,
        }
    end
    t[id].count = t[id].count + v.count
end

local function init()
    local all_dat = skynet.call(redis_service, "lua", "hgetall_all_value", "stuffData", true)
    for uin, stuffData in pairs(all_dat) do
        local tans_data = {}
        for id, value in pairs(stuffData) do
            combine(tans_data, id, value)
        end
        skynet.call(redis_service, "lua", "hset_obj", "stuffData", uin, tans_data)
    end

    skynet.call(redis_service, "lua", "del", "glyphData")

    logger.trace("trans stuffData success")
end

local CMD = {}

skynet.start(function()
    logger.trace("trans_stuff_service start...")

    redis_service = skynet.queryservice("redis_service")

    init()

    local traceback = debug.traceback

    skynet.dispatch("lua", function(_, src, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s src:%d", command, ret, src)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)
end)



--
-- desc : 累计召唤活动
--
local skynet = require "skynet"
local COMMON = require "common"
local logger = require "logger"
local utils  = require "utils"
local sharedata_utils = require "sharedata_utils"

local redis_service


local activity_config

-- focus:

--[[
-- 添加活动的接口:
http://127.0.0.1:7602/gm?gmtype=addactivity&data={"id":260175,"name":"累计召唤","desc":"累计召唤活动","type":1012,"startTime":1465872468,"endTime":1467043019,"icon":"","state":2,"sum_call":{"items":[{"id":1,"plugin":"10001:2, 10002:3","call":2},{"id":2,"plugin":"20002:4, 20003:5","call":3},{"id":3,"plugin":"10003:6, 10004:7","call":4},{"id":4,"plugin":"10005:8, 10006:9","call":5},{"id":5,"plugin":"10003:6, 10004:7","call":20},{"id":6,"plugin":"10003:6, 10004:7","call":21},{"id":7,"plugin":"10003:6, 10004:7","call":23},{"id":8,"plugin":"10003:6, 10004:7","call":32},{"id":9,"plugin":"10003:6, 10004:7","call":40},{"id":10,"plugin":"10003:6, 10004:7","call":40}]}}

-- 删除活动的接口:
http://10.10.2.39:7602/gm?gmtype=delactivity&activityId=260174

id需要唯一
]]

--[[
    id              活动ID
    name            活动名称
    desc            活动规则描述
    type            活动类型
    startTime       开始时间【精确到秒】
    endTime         结束时间
    state           活动状态 【1 添加未启用 2 添加已启用 3 可见 4 运行中 5 过期】
    updateTime      活动最近一次操作时间operate_activities
    icon            活动图标
    
    sum_call     -- 累计召唤
        {
            "items": [
            {
                "id" :  1,   -- 条件id
                "plugin" :  "10001:10, 20001:20",   -- 达到条件可领取的道具
                "call" :  3 -- 累计召唤到达领取条件次数
            },
            {
                "id" :  2,   -- 条件id
                "plugin" :  "10001:10, 20001:20",   -- 达到条件可领取的道具
                "call" :  3 -- 累计召唤到达领取条件次数
            },
            {
                "id" :  3,   -- 条件id
                "plugin" :  "10001:10, 20001:20",   -- 达到条件可领取的道具
                "call" :  3 -- 累计召唤到达领取条件次数
            },
            ],
        }
]]


local CMD = {}
--[[ 必须实现的接口
    CMD.load(data)
    CMD.update
    CMD.remove
    CMD.__checkConfig【内部调用】
    CMD.activity_state_changed(data)
    -- ...
--]]

function CMD.__checkConfig(data)
    if not data.sum_call or type(data.sum_call) ~= "table" then
        logger.errorf("operate_activity_sum_call.lua-> __checkConfig(...) not found sum_call field or sum_call field is not table")
        return false
    end

    local tyStr = type('')
    local tyInt = type(10)
    local tyTable = type({})

    local chk1 = { items = tyTable}
    if not utils.checkKeys(chk1, data.sum_call) then
        return false
    end
    

    local chk3 = {id = tyInt, plugin = tyStr, call = tyInt}
    for k,v in pairs(data.sum_call.items) do
        if not utils.checkKeys(chk3, v) then
            return false
        end
    end

    return true
end

function CMD.load(data)
    -- 校验数据合法性
    if not CMD.__checkConfig(data) then
        return false
    end
    activity_config = data

    return true
end

-- 活动调度
-- 如果需要做隔天的判断或者时间的处理 在update中处理
function CMD.update()
end

-- 活动移除处理
-- 移除全局活动数据 移除个人活动数据【框架处理】
-- 如果有排行榜发奖之类的处理 在这里处理
function CMD.remove()
    -- 关闭服务
    skynet.exit()
end

-- 活动状态改变处理
function CMD.activity_state_changed(state)
    activity_config.state = state
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
end)

--
-- desc : 荣誉商店荣誉折扣
--
local skynet = require "skynet"
local COMMON = require "common"
local logger = require "logger"
local utils  = require "utils"
local sharedata_utils = require "sharedata_utils"
local operate_activity_service = ...

local redis_service
local gamed

local activity_config

-- 活动数据
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
    
    shop_dis        -- 兑换商城活动对应的折扣值 id荣誉兑换中商品id dis是折扣，取值范围(0,1)
        id      dis
        id      dis
        ...


http://10.10.2.46:7602/gm?gmtype=addactivity&data={"icon":"","type":1004,"id":261542,"desc":"111","state":2,"name":"兑换活动","shop_dis":{"1":"0.8","2":"0.7","3":"0.5 "},"updateTime":1470048126,"startTime":1469462400,"endTime":1469807999}

 --]]

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
    local extraAttr = {
        "globalTotalCount", "globalTotalCount", "takeExp"
    }
    local exc_cfg = sharedata_utils.get_stat_all(COMMON.plan.ExchangeData)
    if not exc_cfg then
        logger.errorf("operate_activity_honor_exc_dis-> __checkConfig(...) not found COMMON.plan.ExchangeData")
        return false
    end

    if not data.shop_dis or type(data.shop_dis) ~= "table" then
        logger.errorf("operate_activity_honor_exc_dis-> __checkConfig(...) not found shop_dis field or shop_dis field is not table")
        return false
    end

    for k,dis in pairs(data.shop_dis) do
        if not exc_cfg[tonumber(k)] then
            logger.errorf("operate_activity_honor_exc_dis-> __checkConfig(...) not found shopid[%s]", k)
            return false
        end

        local nDis = tonumber(dis)
        if not nDis or not ( nDis > 0 and nDis < 1) then
            logger.errorf("operate_activity_honor_exc_dis-> __checkConfig(...) error discount value[%s]", dis)
            return false 
        end
    end

    -- 如果有更深的数据继续往下判断
    return true
end

function CMD.load(data)
    -- 校验数据合法性
    if not CMD.__checkConfig(data) then
        return false
    end
    activity_config = data
    print("operate_activity_honor_exc_dis.lua<CMD.load> activity_config.id: " .. activity_config.id)

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
    gamed = skynet.queryservice("gamed")

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

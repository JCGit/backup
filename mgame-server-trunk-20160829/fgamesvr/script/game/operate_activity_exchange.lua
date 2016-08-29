--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2016/4/27
-- Time: 16:33
-- Desc: 兑换活动服务
--

local skynet = require "skynet"
local COMMON = require "common"
local logger = require "logger"
local utils  = require "utils"
local operate_activity_service = ...

local redis_service
local gamed

local activity_config
local selfData
local globalData

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

    dropData			{}		掉落数据
        dropEndTime		int		掉落截止时间[时间戳]
  	    dropItem		int		掉落物品id
  	    winDropCount	int		胜利掉落数量
  	    loseDropCount	int		失败掉落数量
  	exchangeData		{}		兑换数据
        id<key>			int		兑换id
        needItem		int		兑换所需物品id
        needCount		int		兑换所需物品数量
        addItem			int		兑换后的物品ID
        addCount		int		兑换后的物品数量
        refreshTime		int		刷新时间[如果为0,表示只能刷新一次;不为0,则到时间刷新一次]
        exchangeCount	int		在刷新时间的可兑换次数


http://10.10.2.46:7602/gm?gmtype=addactivity&data={"icon":"","type":1003,"id":261541,"desc":"111","state":4,"name":"兑换活动","exchangeData":[{"exchangeCount":3,"refreshTime":600,"needItem":10001,"needCount":5,"addItem":20001,"addCount":1,"id":1}],"updateTime":1470048126,"startTime":1469462400,"endTime":1469807999,"dropData":{"dropEndTime":1473285600,"winDropCount":5,"loseDropCount":1,"dropItem":10001}}
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
    local extraData = {
        "dropData", "exchangeData",
    }
    for _, attr in ipairs(extraData) do
        if not data[attr] then
            return false
        end
    end

    local dropData = {
        "dropEndTime", "dropItem", "winDropCount", "loseDropCount",
    }
    for _, attr in ipairs(dropData) do
        if not data.dropData[attr] then
            return false
        end
    end

    local exchangeData = {
        "id", "needItem", "needCount", "addItem", "addCount", "refreshTime", "exchangeCount",
    }
    for _, v in ipairs(data.exchangeData) do
        for _, attr in ipairs(exchangeData) do
            if not v[attr] then
                return false
            end
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
    print("operate_activity_demo.lua<CMD.load> activity_config.id: " .. activity_config.id)

    return true
end

-- 活动调度
-- 如果需要做隔天的判断或者时间的处理 在update中处理
function CMD.update()

end

-- 活动移除处理
-- 移除全局活动数据 移除个人活动数据【框架处理】
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
    logger.trace("operate_activity_demo start...")

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



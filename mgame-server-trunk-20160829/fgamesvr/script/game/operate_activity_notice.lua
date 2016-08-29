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

    http://115.159.150.149:7602/gm?gmtype=addactivity&data={"id":901461,"name":"公告","actName":"test","actType":1,"listName":1695,"desc":"官方交流群：478803945\\c[ffffff]\\h[http://jq.qq.com/?_wv=1027&k=2KiYYU8$$(点击加群)","type":1005,"startTime":1469030400,"endTime":1469106000,"icon":"","state":2,"showType":0,"tcontrolType":1,"pageId":""}
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


function CMD.load(data)
    -- 校验数据合法性
    activity_config = data

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
    logger.trace("operate_activity_notice start...")

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



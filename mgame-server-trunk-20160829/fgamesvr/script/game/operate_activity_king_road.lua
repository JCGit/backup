--
-- desc : 王者之路
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
http://127.0.0.1:7602/gm?gmtype=addactivity&data={"id":260176,"name":"王者之心","desc":"王者之心","type":1015,"startTime":1467648000,"endTime":1468166400,"icon":"","state":2,"king_road":{"win":[140,180,220,260,300,340,380,420,460,500,560,620,680,740,800,880,960,1040,1120,1200,1300,1400,1500,1600,1700,1800,1900,2000,2100,2200],"title_num1":20,"title_num2":30,"title_num3":50, "currency":2}}

-- 删除活动的接口:
http://10.10.2.39:7602/gm?gmtype=delactivity&activityId=260176

id需要唯一
]]


--[[
活动配置

    id              活动ID
    name            活动名称
    desc            活动规则描述
    type            活动类型
    startTime       开始时间【精确到秒】
    endTime         结束时间
    state           活动状态 【1 添加未启用 2 添加已启用 3 可见 4 运行中 5 过期】
    updateTime      活动最近一次操作时间operate_activities
    icon            活动图标
    
    -- 王者之心
    "king_road" : 
        {
            "win": [
                140,
                180,
                220,
                260,
                300,
                340,
                380,
                420,
                460,
                500,
                560,
                620,
                680,
                740,
                800,
                880,
                960,
                1040,
                1120,
                1200,
                1300,
                1400,
                1500,
                1600,
                1700,
                1800,
                1900,
                2000,
                2100,
                2200
            ],
            "title_num1" : 20,
            "title_num2" : 30,
            "title_num3" : 50,
            "currency" : 2
    }

    -- win 代表数组，下标+1代表连胜的胜利场次，下标对应的值代表奖励的荣誉值
    -- title_num*获得常胜将军勋章的连胜条件

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
    if not data.king_road or type(data.king_road) ~= "table" then
        logger.errorf("operate_activity_king_road.lua-> __checkConfig(...) not found king_road field or king_road field is not table")
        return false
    end

    local tyStr = type('')
    local tyInt = type(10)
    local tyTable = type({})

    local chk1 = { win = tyTable, title_num1 = tyInt, title_num2 = tyInt, title_num3 = tyInt, currency = tyInt}
    if not utils.checkKeys(chk1, data.king_road) then
        return false
    end

    if not utils.checkRange(data.king_road.currency, 0, 3) then
        return false
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

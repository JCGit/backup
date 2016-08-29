--
-- desc : 巅峰排行榜星星榜
--
local skynet = require "skynet"
local COMMON = require "common"
local logger = require "logger"
local utils  = require "utils"
local sharedata_utils = require "sharedata_utils"

local redis_service


local activity_config
local opend = false

-- focus:

--[[
-- 添加活动的接口:
http://127.0.0.1:7602/gm?gmtype=addactivity&data={"id":260174,"name":"胜利排行榜","desc":"巅峰排名活动胜利排行榜","type":1008,"startTime":1468166400,"endTime":1468771199,"icon":"","state":2,"topConfig":{"top10":["10001:10,20001:20","10001:10,20001:20","10001:10,20001:20","10001:10,20001:20","10001:10,20001:20","10001:10,20001:20","10001:10,20001:20","10001:10,20001:20","10001:10,20001:20","10001:10,20001:20"],"wardTime":"23:59","mailTitle":"美味大冒险奖励","mailContent":"【巅峰排名来袭!恭喜您在《美味大冒险》中收集的星星数量达到排名第%d名，特发奖励一份，请注意查收，榜单上的额外奖励请通过QQ联系我们的客服雪儿2656247100】","inter":30}}

-- 删除活动的接口:
http://10.10.2.39:7602/gm?gmtype=delactivity&activityId=260174

id需要唯一
]]


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
    
    topConfig:
    {
        top10:        -- 排名前10的奖励，奖励是通过邮件来发送到个人帐户的
        [
        "10001:10,20001:20",
        "10001:10,20001:20",
        "10001:10,20001:20",
        "10001:10,20001:20",
        "10001:10,20001:20",
        "10001:10,20001:20",
        "10001:10,20001:20",
        "10001:10,20001:20",
        "10001:10,20001:20",
        "10001:10,20001:20"
        ],
        "wardTime": "23:59", -- 结算时间(星星榜是活动结束时结算)
        "mailTitle": "美味大冒险奖励",
        "mailContent": "【巅峰排名来袭!恭喜您在《美味大冒险》中收集的星星数量达到排名第%d名，特发奖励一份，请注意查收，榜单上的额外奖励请通过QQ联系我们的客服雪儿2656247100】",
        "inter": 30
    }

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

    if not data.topConfig or type(data.topConfig) ~= "table" then
        logger.errorf("operate_activity_top_win.lua-> __checkConfig(...) not found topConfig field or topConfig field is not table")
        return false
    end

    local tyStr = type('')
    local tyTable = type({})
    local tyInt = type(10)
    local chk1 = {top10 = tyTable, mailTitle = tyStr, mailContent = tyStr, wardTime = tyStr, inter = tyInt}
    if not utils.checkKeys(chk1, data.topConfig) then
        return false
    end
    
    if not string.match(data.topConfig.wardTime, '(%d+):(%d+)') then
        logger.debugf("operate_activity_top_win.lua-> __checkConfig(...) 时间格式错误 %s", topConfig.wardTime)
        return false
    end

    for k,v in pairs(data.topConfig.top10) do
        if type(v) ~= "string" then
            return false
        end
    end
    
    if not utils.checkRange(data.topConfig.inter, 0, 60) then
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
-- 活动期间内重启进程通知活动已开启
function CMD.update()
    if not opend then
        skynet.call(".proxy", "lua", "win_open", activity_config)
        opend = true
    end
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
    if state == COMMON.OP_ACT_STATE.RUN then
        skynet.call(".proxy", "lua", "win_open", activity_config)
        opend = true
    end

    if state == COMMON.OP_ACT_STATE.EXPIRED then
        skynet.call(".proxy", "lua", "win_close")
        opend = false 
    end
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

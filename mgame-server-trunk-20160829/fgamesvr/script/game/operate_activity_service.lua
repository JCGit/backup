--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2016/3/5
-- Time: 18:26
-- Desc: 运营活动服务
--

local skynet = require "skynet"
local logger = require "logger"
local utils = require "utils"
local COMMON = require "common"

local redis_service
local gamed

local OP_ACT_STATE = COMMON.OP_ACT_STATE
local OP_ACT_TYPE = COMMON.OP_ACT_TYPE
local OP_ACT_MAP = COMMON.OP_ACT_MAP

-- key : activityId     value : serverId
local operate_activity_services = {}
-- key : activityId     value:  config
local operate_activities = {}

--[[
存储结构：
    [1] operate_activities  id  => activity_config
           id
           name
           desc
           type
           startTime
           endTime
           state
           icon
           -- 额外数据【每个活动不一样】
           --
    [2] activityGlobalData  id  => globalData
            -- 基础数据
            activityId
            updateTime
            -- 额外数据【每个活动不一样】
            --
    [3] 活动名_活动ID       uin => selfData
            uin
            activityId
            udpateTime
            -- 额外数据【每个活动不一样】
            --
--]]

local CMD = {}

local function checkActivityBaseConfig(data)
    local baseAttr = {
        "id", "name", "desc", "type", "startTime", "endTime", "state", "icon"
    }
    for _, attr in ipairs(baseAttr) do
        if not data[attr] then
            return false
        end
    end
    return true
end

local function getActivityServiceById(id)
    return operate_activity_services[id] or nil
end

local function getActivityConfigById(id)
    return operate_activities[id] or nil
end

-- 【玩家活动状态表】表名格式： 活动名_活动ID -- 支持同时有多个同类型的活动
local function get_table_name(id)
    local activity_config = getActivityConfigById(id)
    assert(activity_config)
    local type = activity_config.type
    assert(type)
    local activity_name = OP_ACT_MAP[type][1]
    assert(activity_name)
    local table_name = activity_name .. "_" .. id
    return table_name
end

-- 注册活动到活动管理服务
local function __registerActivity(data)
    assert(data)
    local id = data.id
    assert(id)
    local type = data.type
    assert(type)

    if not checkActivityBaseConfig(data) then
        logger.trace("activity data is not correct, please check base data! activityId : " .. id)
        return false
    end

    if operate_activity_services[id] or operate_activities[id] then
        logger.trace("activity is exist! activityId : " .. id)
        return false
    end

    local serverId = skynet.newservice(OP_ACT_MAP[type][1], skynet.self())
    local r = skynet.call(serverId, "lua", "load", data)
    if not r then
        pcall(skynet.call, serverId, "lua", "remove")
        logger.trace("activity data is not correct, acctivitId : " .. id)
        return false
    end
    operate_activity_services[id] = serverId
    operate_activities[id] = data
    return true
end

-- 从活动管理服务中注销掉活动 并通知活动服务remove活动
local function __unregisterActivity(id)
    local serverId = operate_activity_services[id]
    if not serverId then
        logger.trace("activity is not exist! activityId :" .. id)
        return false
    end

    pcall(skynet.call, serverId, "lua", "remove")

    -- 移除所有个人活动数据,全局活动数据,活动配置
    CMD.removeAllSelfData(id)
    CMD.removeGlobalData(id)
    skynet.call(redis_service, "lua", "hdel_obj", "operate_activities", id)

    operate_activity_services[id] = nil
    operate_activities[id] = nil

    return true
end

-- 装载运营活动数据
local function __loadAllConfig()
    local datas = skynet.call(redis_service, "lua", "hgetall_all_value", "operate_activities")
    for _, data in pairs(datas) do
        __registerActivity(data)
    end
end

local function change_activity_state(activityId, state)
    local activity_config = getActivityConfigById(activityId)
    local old_state = activity_config.state
    local serverId = getActivityServiceById(activityId)
    activity_config.state = state
    activity_config.updateTime = os.time()
    skynet.call(serverId, "lua", "activity_state_changed", state)
    skynet.call(redis_service, "lua", "hset_obj", "operate_activities", activityId, activity_config)

    if (old_state < OP_ACT_STATE.VISIBLE and state == OP_ACT_STATE.VISIBLE or
            old_state < OP_ACT_STATE.VISIBLE and state == OP_ACT_STATE.RUN) then
        -- 通知所有客户端有新活动增加 下发活动数据
        skynet.call(gamed, "lua", "add_activity", activity_config)
    else
        -- 通知客户端活动状态改变 下发活动ID和状态
        if state >= OP_ACT_STATE.VISIBLE and old_state >= OP_ACT_STATE.VISIBLE  then
            skynet.call(gamed, "lua", "activity_state_changed", activityId, state)
        end
    end

    -- 过期 内存维护
    if state == OP_ACT_STATE.EXPIRED then
        __unregisterActivity(activityId)
    end
end

-- 活动调度
--[[ 状态调度规则：
        启用或者可见状态 到达开启时间 自动进入RUN状态
        到达结束时间 自动进入结束状态
        运行状态调用的活动调用update
--]]
local update_interval = 100 -- 1秒调度一次

local function update_activities()
    local now = os.time()
    for id, data in pairs(operate_activities) do
        if data.state == OP_ACT_STATE.ENABLE or data.state == OP_ACT_STATE.VISIBLE then
            if now >= data.startTime and now < data.endTime then
                change_activity_state(id, OP_ACT_STATE.RUN)
            end
        elseif data.state == OP_ACT_STATE.RUN then
            if now >= data.endTime then
                change_activity_state(id, OP_ACT_STATE.EXPIRED)
            end
        end

        -- 此处调用用到了活动开启内重启重新打开,若修改请注意!!
        if data.state == OP_ACT_STATE.RUN then
            skynet.call(operate_activity_services[id], "lua", "update")
        end
    end
    skynet.timeout(update_interval, update_activities)
end

function CMD.init()
    -- 装载活动数据
    __loadAllConfig()

    gamed = skynet.queryservice("gamed")

    -- 活动调度
    update_activities()
end

function CMD.add_activity(data)
    if __registerActivity(data) then
        skynet.call(redis_service, "lua", "hset_obj", "operate_activities", data.id, data)
        return true
    else
        return  "add activity failed! activityId : " .. data.id
    end
end

-- 移除活动数据
function CMD.del_activity(activityId)
    if getActivityConfigById(activityId) then
        change_activity_state(activityId, OP_ACT_STATE.EXPIRED)
        return true
    else
        return  "del activity failed, activityId : " .. activityId
    end
end

function CMD.set_activity_state(activityId, state)
    local activity_config = getActivityConfigById(activityId)
    local serverId = getActivityServiceById(activityId)
    if not activity_config or not serverId then
        return "set_activity_state error, activityId not exitst" .. activityId
    end
    
    if state <= activity_config.state then
        return "set_activity_state error, state must over current !"
    end

    if state >= OP_ACT_STATE.RUN then
        return "set_activity_state error, state must below run state !"
    end

    change_activity_state(activityId, state)
    return true
end

-- 获取个人活动状态
function CMD.getSelfData(activityId, uin)
    local table_name = get_table_name(activityId)
    return skynet.call(redis_service, "lua", "hget_obj", table_name, uin, true)
end

-- 修改个人活动状态
function CMD.changeSelfData(activityId, uin, data)
    -- 通知个人客户端
    local agent = skynet.call(gamed, "lua", "get_agent", uin)
    if agent then
        skynet.call(agent, "lua", "changeSelfData", activityId, data)
    end
end

--删除个人活动状态
function CMD.removeSelfData(activityId, uin)
    local table_name = get_table_name(activityId)
    skynet.call(redis_service, "lua", "hdel_obj", table_name, uin, true)
end

function CMD.removeAllSelfData(activityId)
    local table_name = get_table_name(activityId)
    skynet.call(redis_service, "lua", "del", table_name, true)
end

-- 获取全局活动状态
function CMD.getGlobalData(activityId)
    return skynet.call(redis_service, "lua", "hget_obj", "activityGlobalData", activityId)
end

-- 修改全局活动状态
function CMD.changeGlobalData(activityId, data)
    --通知所有客户端
    skynet.call(gamed, "lua", "changeGlobalData", activityId, data)
    skynet.call(redis_service, "lua", "hset_obj", "activityGlobalData", activityId, data)
end

-- 删除全局活动状态
function CMD.removeGlobalData(activityId)
    skynet.call(redis_service, "lua", "hdel_obj", "activityGlobalData", activityId)
end

-- 活动开启时的活动产生的数据初始化
-- 在下面添加了初始化记得在agent_operate_activity.lua里面的initSelfData函数加下
function CMD.initSelfData(id, ty, uin)
    local selfData = {
        -- baseData
        activityId  = id,
        uin         = uin,
        updateTime  = os.time(),
        -- extraBase根据不同活动类型增加
    }

    if ty == OP_ACT_TYPE.DEMO then
        selfData.takeCount = 0
    elseif ty == OP_ACT_TYPE.EXCHANGE then
        selfData.exchangeData = {}
    elseif ty == OP_ACT_TYPE.MAIL then
        selfData.retrived = false
    elseif ty == OP_ACT_TYPE.CRAZY then
        -- Score : 狂欢周累积的荣誉
        -- Points : 狂欢周累积的狂欢点
        -- PvpWin : 狂欢周当天得利的场次 
        -- PvpNum : 狂欢周当天pvp场次
        -- Usabel : 狂欢周奖励 0.不可领取  1.可领取  2.已领取
        selfData.crazyData = { Score = 0, Points = 0, PvpWin = 0, PvpNum = 0, Usable = 0, Day = 1}
    elseif ty == OP_ACT_TYPE.TOP_STAR then
        selfData.top_star = 0 -- 活动期间内得到的pve关卡星星数
    elseif ty == OP_ACT_TYPE.TOP_WIN then
        selfData.top_win = 0 -- 活动期间内当天pvp胜利场次
    elseif ty == OP_ACT_TYPE.TOP_KING then
        selfData.top_king = 0 -- 活动期间内当天连胜场次，不赢则终止
        selfData.top_max_king = 0 -- 活动期间内当天最大连胜场次
    elseif ty == OP_ACT_TYPE.LIMIT_EXCHANGE then
        selfData.limit_exchange = {}
    elseif ty == OP_ACT_TYPE.SUM_CALL then
        selfData.sum_call = {call = 0, items = {}} -- call 累计召唤的次数， items 已经领取过的item
    elseif ty == OP_ACT_TYPE.BRAVE then
        selfData.cache = {days = {}, value = 0, curDay = 1} -- value 当天pvp连胜场次 days 保存当天的奖励是否已经领取 curDay 当前活动的第几天
    elseif ty == OP_ACT_TYPE.BATTLE_CRAZY then
        selfData.cache = {days = {}, value = 0, curDay = 1} -- value 当天pvp场次 days 保存当天的奖励是否已经领取 curDay 当前活动的第几天
    elseif ty == OP_ACT_TYPE.KING_ROAD then
        selfData.num_award = 0       -- 王者之路已经领取对应胜利次数的奖励
        selfData.win_count = 0       -- 王者之路连胜场次
    elseif ty == OP_ACT_TYPE.SUM_RECHARGE then
        selfData.sum_recharge = {diamond = 0, items = {}} -- diamond 累计充值的钻石数， items 已经领取过的item
    else
        -- 其他活动数据初始化
    end

    return selfData
end
function CMD.get_op_activities(uin)
    local lists = {}
    for id, data in pairs(operate_activities) do
        if data.state >= OP_ACT_STATE.VISIBLE and data.state < OP_ACT_STATE.EXPIRED then
            lists[id] = {
                serverId        = operate_activity_services[id],
                config          = data,
                selfData       = CMD.getSelfData(id, uin) or CMD.initSelfData(id, data.type, uin),
                globalData     = CMD.getGlobalData(id)  or nil,
            }
        end
    end
    return lists
end

skynet.start(function()
    logger.trace("operate_activity_service start...")

    redis_service = skynet.queryservice("redis_service")

    local traceback = debug.traceback

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

--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2016/3/10
-- Time: 11:36
-- Desc: 运营活动消息处理模块
--

local skynet          = require "skynet"
local sharedata_utils = require "sharedata_utils"
local utils           = require "utils"
local setting         = require "setting"
local logger          = require "logger"
local proto           = require "proto"
local COMMON          = require "common"
local player          = require "player"
local json            = require "cjson"
local agent_item      = require "agent_item"
local agent_player    = require "agent_player"

local OP_ACT_TYPE = COMMON.OP_ACT_TYPE
local OP_ACT_STATE = COMMON.OP_ACT_STATE
local OP_ACT_MAP = COMMON.OP_ACT_MAP

local redis_service, agent, account_info, send_package, uin, mail_service
local operate_activity_service
local send_error_code, send_response
local this  = {}
local player

local busilog_service
local agent_pve
local multicast_service 

-- id => { config, selfData, globalData, serverId}
local activityDatas

--[[
 - - 注意：增加活动时 如果活动有个人数据的时候 修改函数 this.initSelfData
                    增加类型和活动对应的个人活动数据
                    如果有全局活动数据的时候 修改函数 this.initGlobalData
                    增加类型和活动对应的全局活动数据
--]]

local get_send_flag = function(type)
    return OP_ACT_MAP[type][2] == 1
end

-- in : 活动类型
-- out: 是否开启, 活动配置
function this.check_open(type)
    for _, data in pairs(activityDatas) do
        if data.config and
            data.config.type and
            data.config.type == type and
            data.config.state == OP_ACT_STATE.RUN
        then
            return true, data.config
        end
    end
    return false, nil
end

function this.check_openBySubType(type, subType)
    for _, data in pairs(activityDatas) do
        if data.config and
                data.config.type and
                data.config.type == type and
                data.config.subType and
                data.config.subType == subType and
                data.config.state == OP_ACT_STATE.RUN
        then
            return true, data.config
        end
    end
    return false, nil
end

function this.getActivityConfigById(activityId)
    if not activityDatas[activityId] then return nil end
    return activityDatas[activityId].config or nil
end

function this.setActivityConfig(config)
    local id = config.id
    if not activityDatas[id] then
        activityDatas[id] = {}
    end
    activityDatas[id].config = config
end

function this.removeActivity(activityId)
    activityDatas[activityId] = nil
end

function this.setActivityState(id, state)
    local config = this.getActivityConfigById(id)
    if not config then return end
    config.state = state

    if state == OP_ACT_STATE.EXPIRED then
        this.removeActivity(id)
    end
end

function this.getSelfDataById(activityId)
    if not activityDatas[activityId] then return nil end
    return activityDatas[activityId].selfData or nil
end

function this.setSelfDataById(activityId, data)
    if not activityDatas[activityId] then return end
    activityDatas[activityId].selfData = data
end

function this.getGlobalDataById(activityId)
    if not activityDatas[activityId] then return nil end
    return activityDatas[activityId].globalData or nil
end

function this.setGlobalDataById(activityId, data)
    if not activityDatas[activityId] then return end
    activityDatas[activityId].globalData = data
end

function this.getActivityServiceById(activityId)
    if not activityDatas[activityId] then return nil end
    return activityDatas[activityId].serverId or nil
end

function this.getActivityTypeById(activityId)
    local config = this.getActivityConfigById(activityId)
    if not config then return nil end
    return config.type or nil
end


function this.initGlobalData(activityId)
    local globalData = {
        -- baseData
        activityId  = activityId,
        updateTime  = os.time(),
        -- extraData 根据不同活动类型增加
    }

    local type = this.getActivityTypeById(activityId)
    if type == OP_ACT_TYPE.DEMO then
        globalData.takeCount = 0
    else
        -- 其他活动数据初始化
    end

    this.setGlobalDataById(activityId, globalData)
    return globalData
end

-- 【玩家活动状态表】表名格式： 活动名_活动ID
-- 支持同时有多个同类型的活动
local function get_table_name(id)
    local activity_config = this.getActivityConfigById(id)
    assert(activity_config)
    local type = activity_config.type
    assert(type)
    local activity_name = OP_ACT_MAP[type][1]
    assert(activity_name)
    local table_name = activity_name .. "_" .. id
    return table_name
end

function this.changeSelfData(activityId, data, notice)
    -- 更新时间
    if data.updateTime then
        data.updateTime = os.time()
    end
    this.setSelfDataById(activityId, data)
    local table_name = get_table_name(activityId)
    skynet.call(redis_service, "lua", "hset_obj", table_name, uin, data, true)
    
    local type = this.getActivityTypeById(activityId)
    --通知客户端
    if notice and  get_send_flag(type) then
        local cmd = COMMON.MESSAGE.CS_CMD_ACT_SELF_DATA_CHANGED
        local resp = {
            SelfData = json.encode(data),
        }
        send_response(cmd, resp)
    end
end

function this.changeGlobalData(activityId, data)
    this.setGlobalDataById(activityId, data)

    --通知给客户端
    local type = this.getActivityTypeById(activityId)
    if get_send_flag(type) then
        local cmd = COMMON.MESSAGE.CS_CMD_ACT_GLOBAL_DATA_CHANGED
        local resp = {
            GlobalData = json.encode(data),
        }
        send_response(cmd, resp)
    end
end

function this.initSelfData(activityId)
    local selfData = {
        -- baseData
        activityId  = activityId,
        uin         = uin,
        updateTime  = os.time(),
        -- extraBase根据不同活动类型增加
    }

    local ty = this.getActivityTypeById(activityId)
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

    this.setSelfDataById(activityId, selfData)
    return selfData
end

function this.add_activity(config)
    -- 内存维护
    this.setActivityConfig(config)
    this.initSelfData(config.id)

    -- 协议返回
    if get_send_flag(config.type) then
        local cmd = COMMON.MESSAGE.CS_CMD_OP_ACT_ADD
        local resp = {
            ActivityConfig = json.encode(config)
        }
        send_response(cmd, resp)
    end
end

function this.activity_state_changed(id, state)
    -- 内存维护
    local type = this.getActivityTypeById(id) -- 删除之前先保存活动类型
    this.setActivityState(id, state)
    -- 协议返回
    if get_send_flag(type) then
        local cmd = COMMON.MESSAGE.CS_CMD_OP_ACT_STATE_CHANGED
        local resp = {
            ActivityId  = id,
            State       = state,
        }
        send_response(cmd, resp)
    end
end

-- api for module begin --

-- 狂欢周荣誉加成
function this.get_score_add_up(score)
    local open,config,selfData

    open, config = this.check_open(OP_ACT_TYPE.CRAZY)
    if not open then
        return 0
    end
    selfData = this.getSelfDataById(config.id)
    local rate = selfData.crazyData.Points * config.rewards.percent
    return math.floor(score * rate / 100)
end

--pvp结束后 统计狂欢周一些东西
function this.crazy_stuff(addscore, end_type)

    local open,config,selfData

    open, config = this.check_open(OP_ACT_TYPE.CRAZY)
    if not open then
        return 0
    end

    local selfData = this.getSelfDataById(config.id)

    local crazyData = selfData.crazyData

    local dayIndex = utils.getTimeInteral(config.startTime, utils.getSameDayBeginTime()) + 1
    local dayCfg = config.rewards.days[dayIndex]
    assert(dayCfg, '获取狂欢周第' .. dayIndex .. '天配置数据出错')

    if not crazyData.Day or crazyData.Day ~= dayIndex then
        crazyData.Day = dayIndex
    end

    --累积胜利场次
    if end_type == COMMON.pvp_end_type.WIN then
        crazyData.PvpWin = crazyData.PvpWin + 1
    end

    --新增pvp场次
    crazyData.PvpNum = crazyData.PvpNum + 1

    if crazyData.Usable == 2 then
        logger.trace("crazy gift already usage ")
    else
        if crazyData.PvpWin >= dayCfg.pvpWin and crazyData.Usable == 0 then
            crazyData.Usable = 1
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_crazy, uin, dayIndex, COMMON.misc_action.act_task_com)
        elseif crazyData.PvpNum >= dayCfg.pvpNum and crazyData.Usable == 0 then
            crazyData.Usable = 1
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_crazy, uin, dayIndex, COMMON.misc_action.act_task_com)
        else
            -- 条件还没达到 do nothing
        end
    end

    local extra_score = 0
    if crazyData.Points ~= 0 and addscore ~= 0 then
        extra_score = math.floor(crazyData.Points * config.rewards.percent * addscore / 100)
        crazyData.Score = crazyData.Score + extra_score
    end
    this.changeSelfData(config.id, selfData, true)
    return extra_score
end

-- 累计召唤次数递增
function this.inc_sum_call(value)
    local open,config
    open, config = this.check_open(OP_ACT_TYPE.SUM_CALL) 
    if open then
        local self = this.getSelfDataById(config.id)
        if self.sum_call then
            self.sum_call.call = self.sum_call.call + value
            this.changeSelfData(config.id, self, true)
        end
    end
end


function this.check_win_title()
    local open,config
    open, config = this.check_open(OP_ACT_TYPE.KING_ROAD)
    if open then
        local win_count = account_info.streak_win_count
        local t_level, t_id
        if win_count >= config.king_road.title_num1 then 
            t_level = 1 t_id = COMMON.title_type.WIN_GENERAL
        end
        if win_count >= config.king_road.title_num2 then  
            t_level = 2 t_id = COMMON.title_type.WIN_GENERAL_2
        end
        if win_count >= config.king_road.title_num3 then
            t_level = 3 t_id = COMMON.title_type.WIN_GENERAL_3
        end
        
        logger.trace("check_win_title", win_count, t_level)
        if t_level then 
            local ret = agent_player.add_title(t_id)
            if not ret then return end
            local notices = sharedata_utils.get_stat(COMMON.plan.Notice, 5)
            local notice_cfg = notices and notices[1] or nil
            if notice_cfg then
                local data = {
                        NoticeType = tonumber(notice_cfg.type),
                        Serious    = tonumber(notice_cfg.urgent_flag),
                        Content    = string.format(notice_cfg.desc,
                                    account_info.name, t_level),
                        Title      = notice_cfg.title,
                        Count      = tonumber(notice_cfg.times),
                        TimeGap    = tonumber(notice_cfg.time_gap),
                }
                skynet.call(multicast_service, "lua", 
                    "publish_announce", COMMON.MESSAGE.CS_CMD_NOTICE, data)
            end
        end 
    end
end

function this.on_recharge(add_diamond)
    local open,config
    open, config = this.check_open(OP_ACT_TYPE.SUM_RECHARGE)
    if open then
        local self = this.getSelfDataById(config.id)
        self.sum_recharge.diamond = self.sum_recharge.diamond + add_diamond
        this.changeSelfData(config.id, self, true)
    end
end

-- api for module end--

-- 添加agent回包
local function add_cmd(CMD)
    CMD.add_activity            = this.add_activity
    CMD.activity_state_changed  = this.activity_state_changed
    CMD.changeSelfData          = this.changeSelfData
    CMD.changeGlobalData        = this.changeGlobalData
end


local function agent_open_check()
    --邮件判断
    for activityId, data in pairs(activityDatas) do
        local config = data.config
        if config.type == OP_ACT_TYPE.MAIL then

            if not data.selfData.is_trived then
                local mail_data = {
                    title = config.title,
                    content = config.content,
                    plugin = config.plugin,
                    sendtime = math.floor(skynet.time()),
                    outdate = config.endTime
                }
                --Todo: 目前邮件没做成功失败, 针对运营活动的全局邮件视作单点邮件处理
                skynet.call(mail_service, "lua", "add_mail", uin,  COMMON.mail_type.SINGLE, mail_data, COMMON.change_src.opreate_mail)

                data.selfData.is_trived = true
                this.changeSelfData(activityId, data.selfData, true)
            end
        end
    end
end


-- daily check begin----

local function daily_check()

    local open,config,self
    -- 狂欢周隔天处理
    open, config = this.check_open(OP_ACT_TYPE.CRAZY)
    if open then
        self = this.getSelfDataById(config.id)
        self.crazyData.PvpWin = 0
        self.crazyData.PvpNum = 0
        self.crazyData.Usable = 0
        self.crazyData.Day = utils.getTimeInteral(config.startTime, utils.getSameDayBeginTime()) + 1
        this.changeSelfData(config.id, self, true)
    end

    -- 巅峰排行榜胜利排名隔天处理
    local win_open, win_cfg = this.check_open(OP_ACT_TYPE.TOP_WIN)
    local id
    if win_open then
        id = win_cfg.id
        self = this.getSelfDataById(id)
        if self.top_win ~=  0 then
            self.top_win = 0
            this.changeSelfData(id, self, true)
        end
    end

    -- 巅峰排行榜连胜排名隔天处理
    local king_open, king_cfg = this.check_open(OP_ACT_TYPE.TOP_KING)
    if king_open then
        id = king_cfg.id
        self = this.getSelfDataById(id)
        if self.top_king ~= 0 or self.top_max_king ~= 0 then
            self.top_king = 0
            self.top_max_king = 0
            this.changeSelfData(id, self, true)
        end
    end

    -- 限时兑换隔天处理
    open, config = this.check_open(OP_ACT_TYPE.LIMIT_EXCHANGE) 
    if open then
        id = config.id 
        self = this.getSelfDataById(id)
        if self and self.limit_exchange then
            -- 跨天重置兑换的次数
            for k,v in pairs(self.limit_exchange) do
                v[2] = 0
            end
            this.changeSelfData(id, self, true)
        end
    end
    
    open, config = this.check_open(OP_ACT_TYPE.BRAVE)
    if open then
        id = config.id
        self = this.getSelfDataById(id)
        self.cache.value = 0
        self.cache.curDay = utils.getTimeInteral(config.startTime, utils.getSameDayBeginTime()) + 1
        this.changeSelfData(id, self, true)
    end

    open, config = this.check_open(OP_ACT_TYPE.BATTLE_CRAZY)
    if open then
        id = config.id
        self = this.getSelfDataById(id)
        self.cache.value = 0
        self.cache.curDay = utils.getTimeInteral(config.startTime, utils.getSameDayBeginTime()) + 1
        this.changeSelfData(id, self, true)
    end

    open, config = this.check_open(OP_ACT_TYPE.KING_ROAD)
    if open then
        id = config.id
        self = this.getSelfDataById(id)
        self.num_award = 0
        self.win_count = 0
        this.changeSelfData(id, self, true)
    end
end


-- daily check end----


-- event begin --

--[[
star 关卡得到的星星数
cur_star 当前关卡获得的星星数，重复玩只记录这一次得到的星星数
]]
local function event_pve_end(eventName, new_flag, star, cur_star)
    local star_open,star_cfg = this.check_open(OP_ACT_TYPE.TOP_STAR)
    if star_open and cur_star ~= 0 then
        local id = star_cfg.id
        local star_self = this.getSelfDataById(id)
        star_self.top_star = star_self.top_star + cur_star
        this.changeSelfData(id, star_self, true)
        pcall(skynet.call, ".proxy", "lua", "update", OP_ACT_TYPE.TOP_STAR, account_info.uin, account_info.name, star_self.top_star)
    end
end

local function event_pvp_end(eventName, room_flag, room_type, end_type, addscore, addpoint, isProtected)
    if room_flag ~= COMMON.room_flag.FIXED then
        local date = os.date('*t', os.time())
        -- 0点到六点不统计巅峰排行数据
        if not (date.hour >= 0 and date.hour < 6) then
            local win_open, win_cfg = this.check_open(OP_ACT_TYPE.TOP_WIN)
            local win_self
            local id 

            if win_open and end_type == COMMON.pvp_end_type.WIN then
                id = win_cfg.id
                win_self = this.getSelfDataById(id)
                win_self.top_win = win_self.top_win + 1
                this.changeSelfData(id, win_self, true)
                pcall(skynet.call, ".proxy", "lua", "update", OP_ACT_TYPE.TOP_WIN, account_info.uin, account_info.name, win_self.top_win)
            end

            local king_open, king_cfg = this.check_open(OP_ACT_TYPE.TOP_KING)
            local king_self
            -- 当天最大连胜场次
            if king_open then 
                id = king_cfg.id
                king_self = this.getSelfDataById(id)
                if end_type == COMMON.pvp_end_type.WIN  then
                    king_self.top_king = king_self.top_king + 1
                    if king_self.top_king > king_self.top_max_king then
                        king_self.top_max_king = king_self.top_king
                        pcall(skynet.call, ".proxy", "lua", "update", OP_ACT_TYPE.TOP_KING, account_info.uin, account_info.name, king_self.top_king)
                    end
                    this.changeSelfData(id, king_self, true)
                else
                    if not isProtected then
                        king_self.top_king = 0
                        this.changeSelfData(id, king_self, true)
                    end
                end
            end 
        end
    end

    local open, config, id, self
    if room_flag == COMMON.room_flag.RAND then
         open, config = this.check_open(OP_ACT_TYPE.KING_ROAD)   
         if open then
            id = config.id
            self = this.getSelfDataById(id)
            if end_type == COMMON.pvp_end_type.WIN then
                self.win_count = self.win_count + 1
                this.changeSelfData(id, self, true)
            elseif not isProtected then
                self.win_count = 0
                this.changeSelfData(id, self, true)
            end
         end
    end

    -- 勇者之心累积加1
    open,config = this.check_open(OP_ACT_TYPE.BRAVE)
    if open then
        id = config.id
        local dayIndex = utils.getTimeInteral(config.startTime, utils.getSameDayBeginTime()) + 1
        self = this.getSelfDataById(config.id)
        self.cache.value = self.cache.value + 1
        if self.cache.curDay ~= dayIndex then
            self.cache.curDay = dayIndex
        end
        this.changeSelfData(config.id, self, true)
    end

    -- 战斗狂人需要胜利才累积
    if end_type == COMMON.pvp_end_type.WIN then
        open,config = this.check_open(OP_ACT_TYPE.BATTLE_CRAZY)
        if open then
            id = config.id
            local dayIndex = utils.getTimeInteral(config.startTime, utils.getSameDayBeginTime()) + 1
            self = this.getSelfDataById(config.id)
            self.cache.value = self.cache.value + 1
            if self.cache.curDay ~= dayIndex then
               self.cache.curDay = dayIndex
            end
            this.changeSelfData(config.id, self, true)
        end
    end
end

-- event end --


-- handle begin--

local function handle_demo_take(cmd, content, len)
    local activityId = proto.parse_proto_req(cmd, content, len).ActivityId
    if not activityDatas[activityId] then return end
    local selfData = this.getSelfDataById(activityId)
    local globalData = this.getGlobalDataById(activityId)
    local config = this.getActivityConfigById(activityId)

    -- 判断
    if config.state ~= COMMON.OP_ACT_STATE.RUN then
        return send_error_code(cmd, COMMON.ERRORCODE.OP_ACT_STATE_NOT_RUN)
    end

    if globalData and globalData.takeCount >= config.globalTotalCount then
        -- 发送错误码【全局次数不够】
        return send_error_code(cmd, 0)
    end

    if selfData and selfData.takeCount >= config.singleTotalCount then
        -- 发送错误码【个人次数不够】
        return send_error_code(cmd, 0)
    end

    -- 个人数据[个人活动数据通过agent来改变]
    selfData = selfData
    selfData.takeCount = selfData.takeCount + 1
    selfData.updateTime = os.time()
    player.add_exp(account_info, config.takeExp, 0)
    this.changeSelfData(activityId, selfData, true)

    -- 全局数据[全局数据改变通过框架去修改]
    globalData = globalData or this.initGlobalData(activityId)
    globalData.takeCount = globalData.takeCount + 1
    globalData.updateTime = os.time()
    skynet.call(operate_activity_service, "lua", "changeGlobalData", activityId, globalData)

    return send_response(cmd, {})
end

-- 获得运营活动状态为可见以上的列表
local function handle_visable_activity_list(cmd, _, _)
    local resp = {
        ActivityConfig = {}
    }
    for _, data in pairs(activityDatas) do
        if get_send_flag(data.config.type) then
            table.insert(resp.ActivityConfig, json.encode(data.config))
        end
    end
    return send_response(cmd, resp)
end

-- 获取运营活动可见以上的私有数据
local function handle_activity_selfdata_list(cmd, _, _)
    local resp = {
        SelfData = {}
    }
    for _, data in pairs(activityDatas) do
        if get_send_flag(data.config.type) and data.selfData then
            table.insert(resp.SelfData, json.encode(data.selfData))
        end
    end
    return send_response(cmd, resp)
end

-- 获取运营活动可见以上的全局数据
local function handle_activity_globaldata_list(cmd, _, _)
    local resp = {
        GlobalData = {}
    }
    for _, data in pairs(activityDatas) do
        if get_send_flag(data.config.type) then
            table.insert(resp.GlobalData, json.encode(data.globalData))
        end
    end
    return send_response(cmd, resp)
end

local function handle_exchange(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local activityId = decode.ActivityId
    if not activityDatas[activityId] then return end
    local exchangeId = decode.ExchangeId

    local selfData = this.getSelfDataById(activityId)
    local config = this.getActivityConfigById(activityId)

    -- 判断是否在可参与状态
    if config.state ~= COMMON.OP_ACT_STATE.RUN then
        return send_error_code(cmd, COMMON.ERRORCODE.OP_ACT_STATE_NOT_RUN)
    end

    -- 判断次数是否足够[如果配置为0 则无限制]
    local exchangeData
    for _, v in ipairs(config.exchangeData) do
        if v.id == exchangeId then
            exchangeData = v
        end
    end
    if not exchangeData then return end
    local maxCount = exchangeData.exchangeCount
    if maxCount > 0 then
       if selfData and selfData.exchangeData[exchangeId] and selfData.exchangeData[exchangeId].count >= maxCount then
           return send_error_code(cmd, COMMON.ERRORCODE.OP_ACT_EXCHANGE_NO_COUNT)
       end
    end

    -- 判断所需物品是否足够
    local costs = {
        [exchangeData.needItem] = exchangeData.needCount
    }
    if not agent_item.check_can_cost(costs) then
        return send_error_code(cmd, COMMON.ERRORCODE.ITEM_NOT_ENOUGH)
    end

    -- 消耗物品发送兑换物品
    local src = COMMON.change_src.op_act_exchange
    agent_item.reduce_item(exchangeData.needItem, exchangeData.needCount, src, true)
    agent_item.add_item(exchangeData.addItem, exchangeData.addCount, src, true)

    -- 修改selfData
    selfData = selfData
    if not selfData.exchangeData[exchangeId] then
        selfData.exchangeData[exchangeId] = {
            id              = exchangeId,
            count           = 1,
            refreshTime     = os.time(),
        }
    else
        selfData.exchangeData[exchangeId].count = selfData.exchangeData[exchangeId].count + 1
    end
    this.changeSelfData(activityId, selfData, true)
    local send_data = {
        ActivityId  = activityId,
        ExchangeId  = exchangeId,
    }
    return send_response(cmd, send_data)
end


-- 狂欢周领取奖励
local function handle_crazy_cost(cmd, msg, sz)
    local config
    local dayIndex = 0
    local dayCfg
    local selfData
    local giftId = 0
    local rsp = {}
    local crazyData

    open, config = this.check_open(OP_ACT_TYPE.CRAZY)
    if not open then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_DATA_ERROR)
    end

    assert(config, '获取狂欢周活动配置出错' .. OP_ACT_TYPE.CRAZY)

    dayIndex = utils.getTimeInteral(config.startTime, utils.getSameDayBeginTime()) + 1
    
    dayCfg = config.rewards.days[dayIndex]
    assert(dayCfg, '获取狂欢周第' .. dayIndex .. '天配置数据出错')

    selfData = this.getSelfDataById(config.id)
    crazyData = selfData.crazyData 
    
    assert(crazyData, '获取狂欢周活动数据失败')

    if crazyData.Usable == 2 then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_RECEIVE_ERROR)
    elseif (crazyData.PvpWin >= dayCfg.pvpWin or crazyData.PvpNum >= dayCfg.pvpNum) and crazyData.Usable == 1 then
        crazyData.Usable = 2
        giftId = dayCfg.giftId
        crazyData.Points = crazyData.Points + 1
        rsp.GiftId = giftId
        this.changeSelfData(config.id, selfData, true)
        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_crazy, uin, dayIndex, COMMON.misc_action.act_task_submit)
    else
        return send_error_code(cmd, COMMON.ERRORCODE.TASK_MIS_MATCH)
    end

    player.add_gift(account_info, giftId, COMMON.change_src.crazy)
    send_response(cmd, rsp)
end

-- 巅峰排行榜获取排行数据
local function handle_get_top_rank(cmd, msg, sz)
    local pkg = proto.parse_proto_req(cmd, msg, sz)

    local id = pkg.ActivityType
    if not this.check_open(id) then
        logger.debugf("handle_get_top_rank() activity status is not open ActivityType[%d]", id)
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end

    local rank_list = skynet.call(".proxy", "lua", "get_rank_by_category", id)
    local rsp = {}
    local list = {}
    for k,v in pairs(rank_list or {}) do
        table.insert(list, { Rank = k, Uin = v.uin, Name = v.name, Value = v.value, Up = v.up})
    end

    rsp.Data = list
    rsp.ActivityType = id

    send_response(cmd, rsp)
end

-- 获取下一次刷新的时间(s)
local function handle_get_top_time(cmd, msg, sz)
    local pkg = proto.parse_proto_req(cmd, msg, sz)
    local id = pkg.ActivityType
    local open, _ = this.check_open(id)
    if open then
        local rsp = {
            ActivityType = id
        }
        rsp.Secs = skynet.call(".proxy", "lua", "get_remain", id)
        send_response(cmd, rsp)
    else
        logger.debugf("handle_get_top_time without() this ActivityType[%d]", id)
        send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end
end


-- 限时兑换活动兑换道具
--[[
selfData:
    limit_exchange = {[itemid] = {itemid, 0}, [itemid] = {itemid, 2}}
]]
local function handle_limit_exchange_cost(cmd, msg, sz)
    local open,config
    open, config = this.check_open(OP_ACT_TYPE.LIMIT_EXCHANGE) 
    if open then
        local up    -- 最大可兑换次数
        local cost -- 需要消耗的货币
        local gift_str -- 当前兑换对应的道具
        local itemId = proto.parse_proto_req(cmd, msg, sz).ItemId
        for k,v in pairs(config.limit_exchange.items) do
            if itemId == v.id then
                up = v.limit
                cost = v.cost
                gift_str = v.plugin
                break
            end
        end
        assert(up, '没找到对应的兑换限次配置' .. itemId)
        assert(cost, '没找到对应的兑换消费货币值' .. itemId)
        assert(gift_str, '没找到对应的道具附件' .. itemId)

        local self = this.getSelfDataById(config.id) 
        local itemCfg = self.limit_exchange[itemId]
        if not itemCfg then
            self.limit_exchange[itemId] = {itemId, 0} -- 1 代表兑换的id 2 代表当天已经兑换的次数
            itemCfg = self.limit_exchange[itemId]
        end

        if itemCfg[2] >= up then
            return send_error_code(cmd, COMMON.ERRORCODE.LIMIT_EXCEED)
        end

        local currency = config.limit_exchange.currency
        if currency == COMMON.currency_attr.diamond then
            if cost > account_info.diamond then
                return send_error_code(cmd, COMMON.ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR)
            end
            player.change_diamond(account_info, COMMON.change_src.limit_exchange, -cost, true)
        elseif currency == COMMON.currency_attr.score then
            if cost > account_info.pvp_score then
                return send_error_code(cmd, COMMON.ERRORCODE.SCORE_NOT_ENOUGH)
            end
            player.change_score(account_info, -cost, COMMON.change_src.limit_exchange, false)
        else
            logger.debugf("what is you currency?  [%d]", currency)
            return
        end
        player.add_gift_by_str(gift_str, account_info,COMMON.change_src.limit_exchange)   
        itemCfg[2] = itemCfg[2] + 1
        this.changeSelfData(config.id, self, true)
        send_response(cmd, {ItemId = itemId})
    else
        logger.debug("limit exchange activity is not open")
        send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end
end


-- 累计召唤道具领取
--[[
selfData:
    sum_call = {call = 0, items = {[itemid] = {itemid, true}, [itemid] = {itemid, true}}

]]
local function handle_sum_call_get(cmd, msg, sz)
    local open,config
    open, config = this.check_open(OP_ACT_TYPE.SUM_CALL) 
    if open then
        local call -- 当前领取条件的召唤计数
        local gift_str -- 当前兑换对应的道具
        local itemId = proto.parse_proto_req(cmd, msg, sz).ItemId
        for k,v in pairs(config.sum_call.items) do
            if itemId == v.id then
                call = v.call
                gift_str = v.plugin
                break
            end
        end
        assert(call, '没找到对应的达到条件的召唤次数' .. itemId)
        assert(gift_str, '没找到对应的道具附件' .. itemId)

        local self = this.getSelfDataById(config.id) 
        -- 领取条件是否到达
        if self.sum_call.call < call then
            return send_error_code(cmd, COMMON.ERRORCODE.TASK_MIS_MATCH)
        end
        
        -- 是否已经领取
        if self.sum_call.items[itemId] then
            return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_RECEIVE_ERROR)
        end

        self.sum_call.items[itemId] = {itemId, true} -- index 1 领取的id   index 2 是否领取
        this.changeSelfData(config.id, self, true)

        player.add_gift_by_str(gift_str, account_info,COMMON.change_src.sum_call)   
        send_response(cmd, {ItemId = itemId})
    else
        logger.debug("sum call activity is not open")
        send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end
end

--[[
selfData:
    cache = { days = {day,day,day}, curDay = 1, value = 0}
    day 代表已经领取的天数
]]
local function handle_battle_gift(cmd, msg, sz)
    local data = proto.parse_proto_req(cmd, msg, sz)
    local act_type = data.ActivityType
    local open,config
    open, config = this.check_open(act_type) 
    if open then
        local id = config.id
        local self = this.getSelfDataById(id)
        local day = utils.getTimeInteral(config.startTime, utils.getSameDayBeginTime()) + 1
        local dayCfg
        local title -- 勋章id
        if act_type == OP_ACT_TYPE.BRAVE then
            dayCfg = config.brave.days   
            title = COMMON.title_type.BRAVE_HEART
        elseif act_type == OP_ACT_TYPE.BATTLE_CRAZY then
            dayCfg = config.battle_crazy.days
            title = COMMON.title_type.FIGHT_MAD
        else
            assert(nil, "what act_tyep" .. act_type)
        end
        
        -- 是否已经领取
        for k,v in pairs(self.cache.days) do
            if day == v then
                return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_RECEIVE_ERROR) 
            end 
        end
        
        -- 读取当天可领取奖励的条件配置
        local cond = dayCfg[day]
        if self.cache.value >= cond.value then
            player.add_gift(account_info, cond.giftId, COMMON.change_src.king)
            table.insert(self.cache.days, day)
            this.changeSelfData(id, self, true) 
        else
            return send_error_code(cmd, COMMON.ERRORCODE.TASK_MIS_MATCH)
        end
    
        -- 判断是否发送勋章
        local function inTable(t, srcV)
            for i,v in pairs(t) do
                if srcV == v then
                    return true
                end
            end
            return false
        end
        for i, _ in pairs(dayCfg) do
            if not inTable(self.cache.days, i) then
                title = false
                break
            end
        end

        if title then
            player.add_title(account_info, title)
        end
        send_response(cmd, {ActivityType = act_type, Day = day}) 

        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_kind_road, account_info.uin, act_type, day, COMMON.misc_action.act_task_submit)
    else
        logger.debugf("handle_battle_gift() type[%d] not open", act_type)
        send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end
end


--[[
    selfData
       num_award 当天已经领取过了连胜场次对应的荣誉的值
       win_count  活动期间内连胜的场次
--]]
local function handle_battle_score(cmd, msg, sz)
    
    local open,config
    open, config = this.check_open(OP_ACT_TYPE.KING_ROAD) 
    if open then
        local id = config.id
        local self = this.getSelfDataById(id)
        if self.num_award ~= 0 then
            return send_error_code(cmd, ERRORCODE.HAS_GET_WIN_REWARD)
        end
    
        local streak_win = self.win_count
        
        -- 防止为0领到了最大的奖励
        if streak_win == 0 then
            return send_error_code(cmd, COMMON.ERRORCODE.TASK_MIS_MATCH)
        end

        local value = config.king_road.win[streak_win]
        if not value then
            -- 没有找到对应奖励的荣誉默认发最大的配置
            logger.debugf("没有找到当前胜利场次对应奖励的荣誉: %d", streak_win)
            streak_win = #config.king_road.win
            value = config.king_road.win[streak_win]
        end

        self.num_award = streak_win
        local currency = config.king_road.currency
        if currency == COMMON.currency_attr.diamond then
            player.change_diamond(account_info, COMMON.change_src.king, value, true)
        elseif currency == COMMON.currency_attr.score then
            player.change_score(account_info, value, true, COMMON.change_src.king)
        else
            -- no ever come
            logger.debugf("handle_battle_score() what is you currency?  [%d]", currency)
            return
        end
        this.changeSelfData(id, self, true)
        send_response(cmd, { Score = value})
        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_streak_win_reward, account_info.uin, streak_win)
    else
        logger.debug("handle_battle_score() not open")
        send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end
end

-- 累计充值奖励领取
--[[
selfData:
    sum_recharge = {diamond = 0, items = {1,2,3}}
    diamond 活动期间内充值总钻石数量
    items 活动期间内已经领取的id值

]]
local function handle_sum_recharge(cmd, msg, sz)
    local open,config
    open, config = this.check_open(OP_ACT_TYPE.SUM_RECHARGE) 
    if open then
        local diamond -- 当前领取条件的召唤计数
        local plugin -- 当前兑换对应的道具
        local itemId = proto.parse_proto_req(cmd, msg, sz).ItemId
        for k,v in pairs(config.sum_recharge.items) do
            if itemId == v.id then
                diamond = v.value
                plugin = v.plugin
                break
            end
        end
        assert(diamond, '没找到对应的达到条件的充值钻石数' .. itemId)
        assert(plugin, '没找到对应的道具附件' .. itemId)

        local self = this.getSelfDataById(config.id) 
        -- 领取条件是否到达
        if self.sum_recharge.diamond < diamond then
            return send_error_code(cmd, COMMON.ERRORCODE.TASK_MIS_MATCH)
        end
        
        -- 是否已经领取

        local function inTable(t, srcV)
            for i,v in pairs(t) do
                if srcV == v then
                    return true
                end
            end
            return false
        end

        if inTable(self.sum_recharge.items, itemId) then
            return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_RECEIVE_ERROR)
        end
        player.add_gift_by_str(plugin, account_info,COMMON.change_src.sum_call)   
        
        table.insert(self.sum_recharge.items, itemId)
        this.changeSelfData(config.id, self, true)

        send_response(cmd, {ItemId = itemId})
    else
        logger.debug("handle_sum_recharge() not open")
        send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end
end

-- handle end --

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(handle_mapping)
    local msg = COMMON.MESSAGE
    handle_mapping[msg.CS_CMD_OP_ACT_CONFIG_LIST]        = handle_visable_activity_list
    handle_mapping[msg.CS_CMD_OP_ACT_SELF_DATA_LIST]     = handle_activity_selfdata_list
    handle_mapping[msg.CS_CMD_OP_ACT_GLOBAL_DATA_LIST]   = handle_activity_globaldata_list

    handle_mapping[msg.CS_CMD_OP_ACT_DEMO_TAKE]          = handle_demo_take
    handle_mapping[msg.CS_CMD_OP_ACT_EXCHANGE]           = handle_exchange

    -- 狂欢周活动协议
    handle_mapping[msg.CS_CMD_ACTIVITY_CRAZY_USAGE]      = handle_crazy_cost -- 领取奖励

    -- 巅峰排行榜
    handle_mapping[msg.CS_CMD_TOP_RANK]         = handle_get_top_rank
    handle_mapping[msg.CS_CMD_TOP_RANK_TIME]    = handle_get_top_time

    -- 付费活动(限时兑换,累积召唤)
    handle_mapping[msg.CS_CMD_LIMIT_EXCHANGE]         = handle_limit_exchange_cost
    handle_mapping[msg.CS_CMD_SUM_CALL]               = handle_sum_call_get

    -- 勇者之心，战斗狂人领取奖励
    handle_mapping[msg.CS_CMD_BATTLE_GIFT]            = handle_battle_gift
    -- 领取王者之路的奖励(荣誉)
    handle_mapping[msg.CS_CMD_BATTLE_SCORE]           = handle_battle_score

    handle_mapping[msg.CS_CMD_SUM_RECHARGE]           = handle_sum_recharge
end

local function register_events()
    EventCenter.addEventListener(Events.AGENT_OPEN, agent_open_check)
    
    EventCenter.addEventListener(Events.DAY_CHANGE, daily_check) -- 跨天处理

    EventCenter.addEventListener(Events.PVE_END, event_pve_end)
    EventCenter.addEventListener(Events.PVP_END, event_pvp_end)
end

local update_interval = 100
function this.update() -- selfData根据时间变化的在update中处理
    local now = os.time()
    for activityId, data in pairs(activityDatas) do
        local config = data.config
        if config.type == OP_ACT_TYPE.EXCHANGE then
            if data.selfData then
                local is_exchange = false
                for exchangeId, v in pairs(data.selfData.exchangeData) do
                    if config.exchangeData[exchangeId].refreshTime ~= 0 and
                       config.exchangeData[exchangeId].exchangeCount ~= 0
                    then
                        local startTime = config.startTime
                        local refreshTime = config.exchangeData[exchangeId].refreshTime
                        if(math.floor((v.refreshTime - startTime)/refreshTime) ~= math.floor((now - startTime)/refreshTime)) then
                            v.count = 0
                            v.refreshTime = os.time()
                            is_exchange = true
                        end
                    end
                end
                if is_exchange then
                    this.changeSelfData(activityId, data.selfData, true)
                end
            end
        else
            -- 其他需要处理的活动
        end
    end

    skynet.timeout(update_interval, this.update)
end


function this.init(args, data, _operate_activity_service)
    redis_service = args.redis
    agent = args.agent
    account_info = args.info
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code
    player = args.player
    uin = args.uin
    busilog_service = args.busilog_service

    agent_pve = require("agent_pve")
    multicast_service = skynet.queryservice("multicast_service")

    operate_activity_service = _operate_activity_service
    activityDatas = data
    mail_service = args.mail_service

    -- 数据更新
    this.update()

    -- 模块必须实现的接口
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)

    register_events()

end

return this

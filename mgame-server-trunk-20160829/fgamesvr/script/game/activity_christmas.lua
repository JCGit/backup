--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/12/10
-- Time: 14:54
-- Desc: 圣诞活动
--

local skynet          = require "skynet"
local sharedata_utils = require "sharedata_utils"
local utils           = require "utils"
local setting         = require "setting"
local logger          = require "logger"
local proto           = require "proto"
local COMMON          = require "common"
local player          = require "player"
local agent_vip       = require "agent_vip"

local redis_service, agent, account_info, send_package
local send_response, send_error_code
local multicast_service
local busilog_service
local activity_christmas = {}

--装扮类型
local DecorateType = {
    Normal  = 1,
    Mid     = 2,
    Super   = 3,
}

local activity_config
local function check_open_activity()
    local now = os.time()
    local start_time = activity_config.begindate
    local end_time = activity_config.enddate

    if now >= start_time and now <= end_time then
        return true
    else
        return false
    end
end

local function check_broadcst(notice_data)
    local broadCastItemdId = activity_config.broadCastItemdId
    for k,v in pairs(notice_data) do
        local ptype, value, itemid = v[1],v[2],v[3]
        if ptype == COMMON.property_type.ITEM then
            if itemid == broadCastItemdId then
                return true
            end
        end
    end
    return false
end

local function handle_decorate_tree(cmd, content, len)
    if not check_open_activity() then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end

    local decode =  proto.parse_proto_req(cmd, content, len)
    local type = decode.Type
    local count = decode.Count
    count = count or 1
    if type < DecorateType.Normal or type > DecorateType.Super then
        logger.trace("type error " .. type)
        return
    end

    local decorateData = sharedata_utils.getDecorateDataByType(type)
    local itemId = decorateData.decorateItemId

    -- 扣物品
    player.use_item(COMMON.change_src.chris, itemId, count, account_info)
    
    local item_count = agent_item.get_item_count(tonumber(itemId))
    if item_count < count then
        return send_error_code(cmd, COMMON.ERRORCODE.ITEM_NOT_EXIST)
    end
    agent_item.reduce_item(itemId, count, COMMON.change_src.chris, true)
    
    -- 发奖励
    local add_notice = player.takeBonusById(account_info, decorateData.BonusId, count, COMMON.change_src.chris)
    -- 广播
    if check_broadcst(add_notice) then
        local msg = decorateData.msg
        msg = string.gsub(msg, "%%d", account_info.name)
        local send_data = {
            NoticeType  = COMMON.broadcast.MARQUEE,
            Serious     = 1,
            Content     = msg,
            Title       = "",
            Count       = 1,
            TimeGap     = 1,
        }
        skynet.call(multicast_service, "lua", "publish_announce", COMMON.MESSAGE.CS_CMD_NOTICE, send_data)
    end

    -- 改个人积分
    local score = decorateData.score * count
    local activity_data = skynet.call(redis_service, "lua", "hget_obj", "christmas_data", account_info.uin)
    if not activity_data then
        activity_data = {
            uin         = account_info.uin,
            score       = 0,
            takeRecord  = {},
        }
    end
    activity_data.score = activity_data.score + score
    skynet.call(redis_service, "lua", "hset_obj", "christmas_data", account_info.uin, activity_data)
    skynet.send(busilog_service, "lua", "log_coin", account_info.uin , COMMON.currency_attr.chris, score, activity_data.score, COMMON.change_src.chris)

    -- 改全局积分
    local global_score = skynet.call(redis_service, "lua", "get_obj", "christmas_score") or 0
    skynet.call(redis_service, "lua", "set_obj", "christmas_score", global_score + score)

    local update_data = {}
    for k,v in pairs(add_notice) do
        local ptype, value, itemid = v[1],v[2],v[3]
        local data = {
            UpType = ptype,
            Value = value,
            Value2 = itemid
        }
        table.insert(update_data, data)
    end
    local data = {
        Score       = score,
        UpdateData  = update_data,
    }
    return send_response(cmd, data)
end

local function handle_take_bonus(cmd, content, len)
    if not check_open_activity() then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end

    local level = proto.parse_proto_req(cmd, content, len).Level

    -- 判断阶段是否能领取
    local globalScore = skynet.call(redis_service, "lua", "get_obj", "christmas_score")
    local curLevel = sharedata_utils.getChristmasLevelByScore(globalScore)
    if level > curLevel then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_RECEIVE_ROCODE_ERROR)
    end

    -- 判断是否已经领取过
    local activity_data = skynet.call(redis_service, "lua", "hget_obj", "christmas_data", account_info.uin)
    if activity_data and activity_data.takeRecord then
        for k, v in pairs(activity_data.takeRecord) do
            if v.level == level and v.isTake then
                return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_RECEIVE_ERROR)
            end
        end
    end

    -- 修改领取记录
    if not activity_data  then
        activity_data = {
            uin         = account_info.uin,
            score       = 0,
            takeRecord  = {},
        }
    end
    activity_data.takeRecord[level] = {
        level   = level,
        isTake  = true,
    }
    skynet.call(redis_service, "lua", "hset_obj", "christmas_data", account_info.uin, activity_data)

    -- 发送奖励
    local bonusId = sharedata_utils.getChristmasBonusByLevel(level)
    local add_data = player.takeBonusById(account_info, bonusId, 1, COMMON.change_src.chris)

    local update_data = {}
    for k,v in pairs(add_data) do
        local ptype, value, itemid = v[1],v[2],v[3]
        local data = {
            UpType = ptype,
            Value = value,
            Value2 = itemid
        }
        table.insert(update_data, data)
    end

    local data = {
        UpdateData  = update_data,
    }
    return send_response(cmd, data)
end

local function handle_get_data(cmd, content, len)
    if not check_open_activity() then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end

    local global_score = skynet.call(redis_service, "lua", "get_obj", "christmas_score")
    local activity_data = skynet.call(redis_service, "lua", "hget_obj", "christmas_data", account_info.uin)
    local data = {
        StartTime   = activity_config.begindate,
        EndTime     = activity_config.enddate,
        DropEndTime = activity_config.dropEndTime,
        GlobalScore = global_score or 0,
        SelfScore   = activity_data and activity_data.score or 0,
        TakeRecord  = {},
    }
    if activity_data then
        local takeRecord = activity_data.takeRecord
        if takeRecord then
            for _, v in pairs(takeRecord) do
                local item = {
                    Level   = v.level,
                    IsTake  = v.isTake,
                }
                table.insert(data.TakeRecord, item)
            end
        end
    end
    return send_response(cmd, data)
end

local function handle_get_rank(cmd, content, len)
    if not check_open_activity() then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end

    local rank_data = {}
    local lists = {}
    for i, v in pairs(rank_data) do
        local item = {}
        item.Order = i
        item.Id = v.uin
        item.Score = v.score

        local info = skynet.call(redis_service, "lua", "hget_obj", "account", v.uin)
        item.Name = info.name or ""
        local vip_data = agent_vip.get_vip_data_by_uin(v.uin)
        if vip_data then
                item.VipLevel = vip_data.vip_level
        end
        table.insert(lists, item)
    end

    local send_data = {
        RankItems = lists,
    }
    return send_response(cmd, send_data)
end

function activity_christmas.get_open_activity(open_activity)
    if check_open_activity() then
        local item  = {
            Type = COMMON.activity_icon.ACT_CHRISTMAS
        }
        table.insert(open_activity, item)
    end
    return open_activity
end

-- 添加agent回包
local function add_cmd(CMD)
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_CHRISTMAS_DECORATE_TREE]   = handle_decorate_tree
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_CHRISTMAS_TAKE_BONUS]      = handle_take_bonus
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_CHRISTMAS_GET_DATA]        = handle_get_data
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_CHRISTMAS_GET_RANK]        = handle_get_rank
end

function activity_christmas.init(args)
    redis_service = args.redis
    multicast_service = skynet.queryservice("multicast_service")
    agent = args.agent
    account_info = args.info
    send_package = args.send_package
    send_response = args.send_response
    busilog_service = args.busilog_service
    activity_config = player.get_christmas_config()

    -- 模块必须实现的接口
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)
end

return activity_christmas


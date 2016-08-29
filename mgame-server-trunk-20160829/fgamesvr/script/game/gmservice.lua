--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/9/2
-- Time: 18:10
--
local skynet = require "skynet"
local json = require "cjson"

local logger = require "logger"
local utils = require "utils"
local COMMON = require "common"
local proto = require "proto"
local json = require "cjson"
json.encode_sparse_array(true)

local gamed
local redis_service
local multicast_service
local mail_service
local rank_service_v2
local operate_activity_service

local CMD = {}

local GM_TYPE = COMMON.GM_TYPE

--FIXME:ALL gmservice解析出来的类型都为string，要注意类型转换

-- gm充vip
local function handle_gm_vip(body)
    local uin, level, day = string.match(body, 'gmtype=vip&uin=([%d]+)&level=([%d]+)&day=([%d]+)')
    local vip_param = {
        VipLevel  = level,
        Price = 0,
        Payment = 4,
        TimeType = day,
    }
    
    local uin_agent = skynet.call(gamed, "lua", "get_agent", uin)
    if uin_agent then
        skynet.call(uin_agent, "lua", "gmcmd", GM_TYPE.ADD_VIP, vip_param)
    else
        local data = skynet.call(redis_service, "lua", "hget_obj", "offline:account_business", uin,true)
        data = data or {}
        table.insert(data, vip_param)
        skynet.call(redis_service, "lua", "hset_obj", "offline:account_business", uin, data, true)
    end
    return true
end

local notice_index = 0
local notice_queue = {}
local function clients_notice(times, interval_time, ntype, nserious, scon, stitle, ncount, ngap)
    local timeOutTimes = tonumber(times)
    local send_times = 1
    notice_index = notice_index + 1
    local index = tostring(notice_index)
    local data = {
        NoticeType  = tonumber(ntype),
        Serious     = tonumber(nserious),
        Content     = scon,
        Title       = stitle,
        Count       = tonumber(ncount),
        TimeGap     = tonumber(ngap),
    }

    notice_queue[index] = function ()
        skynet.call(multicast_service, "lua", "publish_announce", COMMON.MESSAGE.CS_CMD_NOTICE, data)
        send_times  = send_times + 1
        if(send_times < timeOutTimes) then
            skynet.timeout(interval_time, notice_queue[index])
        else
            notice_queue[index] = nil
        end
    end
    notice_queue[index]()
end

local function handle_send_notice(body)
    local times, interval_time, ntype, nserious, scon, stitle, ncount, ngap = string.match(body,
        "gmtype=sendnotice&times=([%d]+)&interval_time=([%d]+)&ntype=([%d]+)&nserious=([%d]+)&scon=(.*)&stitle=(.*)&ncount=([%d]+)&ngap=([%d]+)")
    times         = tonumber(times)
    interval_time = tonumber(interval_time)
    ntype         = tonumber(ntype)
    nserious      = tonumber(nserious)
    ncount        = tonumber(ncount)
    ngap          = tonumber(ngap)

    clients_notice(times, interval_time, ntype, nserious, scon, stitle, ncount, ngap)
    return true
end

local function calc_max_point(point_data)
    local max_point = 0
    if not point_data then return 0 end
    for i, v in ipairs(point_data) do
        local point = math.floor(i)
        if  point > max_point then
            max_point = point
        end
    end
    return max_point
end

--支持离线查询
local function handle_query_info(body)
    local uin = string.match(body, "gmtype=queryinfo&uin=([%d]+)")
    local agent = skynet.call(gamed, "lua", "get_agent", uin)
    local info
    if agent then
        info = skynet.call(agent, "lua", "get_account_info")
    else
        info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
    end
    if not info then
        return "uin : " .. uin .. " is not exist"
    end

    local _pvp_rank, _pvp_group = skynet.call(rank_service_v2, "lua", "get_rank", uin)
    local _items, _clothes, _assits, _pve_info, _player_titles, _recharge_info, _glyphs_info
    if agent then
        _items          = skynet.call(agent, "lua", "get_items")
        _clothes        = skynet.call(agent, "lua", "get_skins")
        _assits         = skynet.call(agent, "lua", "get_assits")
        _player_titles  = skynet.call(agent, "lua", "get_titles")
        _pve_info       = skynet.call(agent, "lua", "get_pve_info")
        _glyphs_info    = skynet.call(agent, "lua", "get_glyphs_info")
    else
        _items          = skynet.call(redis_service, "lua", "hget_obj", "item", uin) or {}
        _clothes        = skynet.call(redis_service, "lua", "hget_obj", "skin", uin, true) or {}
        _assits         = skynet.call(redis_service, "lua", "hget_obj", "assistant_info", uin) or {}
        _player_titles  = skynet.call(redis_service, "lua", "hget_obj", "player_titles", uin) or {}
        _pve_info       = skynet.call(redis_service, "lua", "hget_obj", "check_points", uin) or {}
        _glyphs_info    = skynet.call(redis_service, "lua", "hget_obj", "glyphData", uin) or {}
    end

    local data = {
        uin                 = info.uin,         -- uin
        platform            = info.platform,    -- 平台
        name                = info.name,        -- 名称
        openid              = info.openid,      -- openid
        score               = info.pvp_score,   -- 荣誉
        diamond             = info.diamond,     -- 钻石
        power               = info.power,       -- 体力
        online              = agent and true or false, -- 是否在线
        continue_win_count  = info.continue_win_count,  -- 连胜场次
        create_time         = info.create_time, -- 创建时间
        last_login_time     = info.login_time,  -- 最近登录时间
        room_ticket         = info.room_ticket, -- 开房券
        can_speak_time      = info.can_speak_time, -- 禁言结束时间
        can_login_time      = info.can_login_time, -- 封号结束时间

        -- pvp[pvp积分,pvp排名,pvp总场次,pvp胜率]
        pvp_point           = info.pvp_point,   -- 积分
        pvp_count           = utils.getPvpTotalCount(info), --总场次
        pvp_group           = _pvp_group,   -- 组别
        pvp_rank            = _pvp_rank,    -- 排名
        win_rate            = utils.getPvpWinRate(info), -- 胜率

        -- 充值信息
        recharge_amount     = info.recharge_amount, -- 充值总额

        -- 厨艺【总厨艺等级 厨艺等级】
        glyphs_info         = _glyphs_info,         -- 符文信息
        -- 成就
        achievement_level   = info.achievement_level,   -- 成就等级
        achievement_point   = info.achievement_point,   -- 成就点数

        -- pve【最高关卡 总星星数】
        pve_info            = _pve_info,        -- 关卡信息

        -- 勋章系统[勋章列表 当前佩戴勋章]
        PlayerTitle         = info.PlayerTitle, -- 当前装备称号
        PlayerTitles        = _player_titles,           -- 称号列表

        -- 将军系统[拥有将军, 出战将军]
        assits              = _assits,
        -- 时装【皮肤拥有情况、有效期，当前使用皮肤】
        skins               = _clothes,
        -- 背包【背包道具情况 开房券需要前台整合 room_ticket】
        items               = _items,
    }

    return json.encode(data)
end

local function handle_forbid_speak(body)
    local uin, speak_time = string.match(body, "gmtype=forbidspeak&uin=([%d]+)&speaktime=([%d]+)")
    -- 设置玩家属性 如果在线就在线设置 如果不在线 修改redis数据
    speak_time = math.floor(tonumber(speak_time))
    local uin_agent = skynet.call(gamed, "lua", "get_agent", uin)
    if uin_agent then
        skynet.call(uin_agent, "lua", "gmcmd", GM_TYPE.FORBID_SPEAK, speak_time)
    else
        local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
        if not account_info then return false end
        account_info.can_speak_time = speak_time
        skynet.call(redis_service, "lua", "hset_obj", "account", uin, account_info)
    end
    return true
end

local function handle_forbid_login(body)
    local uin, logintime = string.match(body, "gmtype=forbidlogin&uin=([%d]+)&logintime=([%d]+)")
    -- 设置玩家属性 如果在线就在线设置 如果不在线 修改redis数据
    logintime = math.floor(tonumber(logintime))
    local uin_agent = skynet.call(gamed, "lua", "get_agent", uin)
    if uin_agent then
        skynet.call(uin_agent, "lua", "gmcmd", GM_TYPE.FORBID_LOGIN, logintime)
    else
        local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
        if not account_info then return false end
        account_info.can_login_time = logintime
        skynet.call(redis_service, "lua", "hset_obj", "account", uin, account_info)
    end

    return true
end

local function handle_change_level(body)
    local uin, level, exp = string.match(body, "gmtype=changelevel&uin=([%d]+)&level=([%d]+)&exp=([%d]+)")
    -- 设置玩家属性 如果在线就在线设置 如果不在线 修改redis数据
    level = math.floor(tonumber(level))
    exp = math.floor(tonumber(exp))
    local uin_agent = skynet.call(gamed, "lua", "get_agent", uin)
    if uin_agent then
        skynet.call(uin_agent, "lua", "gmcmd", GM_TYPE.CHANGE_LEVEL, level, exp)
    else
        local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
        if not account_info then return false end
        account_info.level = level
        account_info.exp = exp
        skynet.call(redis_service, "lua", "hset_obj", "account", uin, account_info)
    end
end

-- 添加活动
local function handle_add_activity(body)
    local json_str = string.match(body, "gmtype=addactivity&data=(.*)")
    local ok, data = pcall(json.decode, json_str) -- data数据可能不是合法json
    if not ok then
        logger.trace("add activity error  data is not correct!" .. json_str)
        return false
    end

    if data.startTime >= data.endTime then
        logger.trace("add activity error startTime : " .. data.startTime .. " over endTime : " .. data.endTime)
        return false
    end

    if os.time() >= data.endTime then
        logger.trace("add activity already closed cur : " .. os.time() .. " large than endTime : " .. data.endTime)
        return false
    end


    if not COMMON.OP_ACT_MAP[data.type] then
        logger.trace("add activity error type is not exist! " .. data.type)
        return false
    end

    return skynet.call(operate_activity_service, "lua", "add_activity", data)
end

-- 设置活动状态
local function handle_set_activity_state(body)
    local activityId, state =  string.match(body, "gmtype=setactivitystate&activityId=([%d]+)&state=([%d]+)")
    activityId = tonumber(activityId)
    state = tonumber(state)
    if state < COMMON.OP_ACT_STATE.HIDE or state > COMMON.OP_ACT_STATE.VISIBLE then
        logger.trace("set activity state error state :" .. state)
        return false
    end
    return skynet.call(operate_activity_service, "lua", "set_activity_state", activityId, state)
end

--删除活动
local function handle_del_activity(body)
    local activityId =  string.match(body, "gmtype=delactivity&activityId=([%d]+)")
    activityId  = tonumber(activityId)
    return skynet.call(operate_activity_service, "lua", "del_activity", activityId)
end

local function handle_add_property(body)
    local uin, type, value =  string.match(body, "gmtype=addproperty&uin=([%d]+)&type=([%d]+)&value=([%d]+)")
    print(body)
    uin = tonumber(uin)
    type = tonumber(type)
    value = tonumber(value)
    local uin_agent = skynet.call(gamed, "lua", "get_agent", uin)
    logger.trace("add pvp point for gm : ", uin)
    if uin_agent then
       return skynet.call(uin_agent, "lua", "add_property", type,  value, true, COMMON.change_src.gm)
    else
        return "player is not online"
    end
end

local function handle_change_player_name(body)
    local uin, new_name = string.match(body, "gmtype=change_player_name&uin=(%d+)&player_name=([^%c]*)$")
    logger.tracef("uin: %s, new_name: %s, %s", uin, new_name, type(new_name))
    local uin_agent = skynet.call(gamed, "lua", "get_agent", uin)
    if uin_agent then
        return skynet.call(uin_agent, "lua", "gmcmd", GM_TYPE.CHANGE_PLAYER_NAME, new_name)
    else
        local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
        if not account_info then
            return false
        end

        -- Modify from agent_player.change_player_name function
        local name_data = skynet.call(redis_service, "lua", "hget_obj", "player_name", new_name)
        local other_uin = name_data and name_data.value or 0
        local uin = account_info.uin
        if other_uin ~= 0 and other_uin ~= uin then
            return false
        end

        local old_name = account_info.name
        if new_name == old_name then
            return true
        end

        account_info.name = new_name
        if new_name ~= '' then
            local player_name = { value = uin }
            skynet.call(redis_service, "lua", "hset_obj", "player_name", new_name, player_name)
        end
        skynet.call(redis_service, "lua", "hdel_obj", "player_name", old_name)
        skynet.call(redis_service, "lua", "hset_obj", "account", uin, account_info)
        return true
    end
end

local function handle_add_item(body)
    local uin, id, count = string.match(body, "gmtype=additem&uin=([%d]+)&id=([%d]+)&count=([%d]+)")
    uin = tonumber(uin)
    id = tonumber(id)
    count = tonumber(count)
    local uin_agent = skynet.call(gamed, "lua", "get_agent", uin)
    logger.trace("add item for gm : ", uin)
    if uin_agent then
       return  skynet.call(uin_agent, "lua", "add_item", id, count, COMMON.change_src.gm, true)
    end
    return false
end

function CMD.search_player_info(uin)
    local uin_agent = skynet.call(gamed, "lua", "get_agent", uin)
    if uin_agent then
       return skynet.call(uin_agent, "lua", "get_account_info")
    end
    return skynet.call(redis_service, "lua", "hget_obj", "account", uin)
end

local process_gm_map = {
    --gm_cmd                    gm_type                     function
    ["sendnotice"]          = {GM_TYPE.SEND_NOTICE,         handle_send_notice},
    ["queryinfo"]           = {GM_TYPE.QUERY_INFO,          handle_query_info},
    ["forbidspeak"]         = {GM_TYPE.FORBID_SPEAK,        handle_forbid_speak},
    ["forbidlogin"]         = {GM_TYPE.FORBID_LOGIN,        handle_forbid_login},
    ["changelevel"]         = {GM_TYPE.CHANGE_LEVEL,        handle_change_level},
    ["addactivity"]         = {GM_TYPE.ADD_ACTIVITY,        handle_add_activity},
    ["setactivitystate"]    = {GM_TYPE.SET_ACTIVITY_STATE,  handle_set_activity_state},
    ["delactivity"]         = {GM_TYPE.DEL_ACTIVITY,        handle_del_activity},
    ["addproperty"]         = {GM_TYPE.ADD_PROPERTY,        handle_add_property},
    ["change_player_name"]  = {GM_TYPE.CHANGE_PLAYER_NAME,  handle_change_player_name},
    ["additem"]             = {GM_TYPE.ADD_ITEM,            handle_add_item},
    ["vip"]                 = {GM_TYPE.ADD_VIP,             handle_gm_vip},
}

function CMD.parse_cmd(body)
    logger.trace("gmservice.lua<CMD.parse_cmd> path: ", body)
    if not body then return false end
    local gm_cmd = string.match(body, "gmtype=([%w]+)&")
    if not gm_cmd then return false end
    return process_gm_map[gm_cmd][2](body)
end

skynet.start(function()
    gamed             = skynet.queryservice("gamed")
    redis_service     = skynet.queryservice("redis_service")
    multicast_service = skynet.queryservice("multicast_service")
    mail_service      = skynet.queryservice("mail_service")
    rank_service_v2      = skynet.queryservice("rank_service_v2")
    operate_activity_service = skynet.queryservice("operate_activity_service")

    proto.load_proto()

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


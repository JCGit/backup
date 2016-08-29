local skynet  = require "skynet"
local cluster = require "cluster"
local logger = require "logger" --"pvp_logger"
local COMMON = require "common"
local MESSAGE = COMMON.MESSAGE
local ErrCode = COMMON.ERRORCODE

local utils           = require "utils"
local sharedata_utils = require "sharedata_utils"
local setting         = require "setting"
local proto           = require "proto"
local player          = require "player"

local busilog_service
local gamed
local redis_service
local rank_service_v2

local traceback = debug.traceback

local player_set = {}       -- [agent] 数组
local camp_set = {}         -- { agent = campno }
local other_server_agent={} -- {agent = {gamenode = 'node', agent = 'agent'}}
local room_start_time       -- 房间开始时间
local room_playerinfo = {}  -- 房间玩家信息【key: source value: account_info】
local room_host             -- 主机
local average_ping = {}     -- ping消息的平均值 key: uin value : [ts]
local room_flag             -- 房间类型： 随机 开放 无道具场
local room_type             -- 房间人数
local room_opener           -- 开房的人的uin
local map_id                -- 地图ID
local real_count = 0        -- 真实玩家数量

local room_msg = {}   -- uin => msgNo 如果小于最大值 则过滤掉
local leave_mode = {} -- uin => mode 如果不是离开则为nil

local draw_uin = -1000 -- 平局
local function get_pvp_end_type(v, win_uin)
    local type = 0
    local uin = room_playerinfo[v].uin
    if win_uin == draw_uin then
        type = COMMON.pvp_end_type.DRAW
    else
        if uin == win_uin then
            type = COMMON.pvp_end_type.WIN
        else
            type = COMMON.pvp_end_type.LOSE
        end
    end
    return type
end

local function room_cluster_call(agent, call_type, ...)
    if agent > 100000000 then
        local t = other_server_agent[agent]
        return cluster.call(t.gamenode, ".cluster_agent_server", "agent_forward",
            t.agent, ...)
    else
        return skynet.call(agent,call_type, ...)
    end
end

-- is_not_pass: 广播目标包括自己时为true
local function broadcast(agent, cmdid, data, is_not_pass)
    for _, v in ipairs(player_set) do
        if v > 0 then
            if is_not_pass or v ~= agent then
                pcall(room_cluster_call, v, "lua", "forward", cmdid, data, skynet.self())
            end
        end
    end
end

local function get_camp_no(camps)
    local camp_no = 0
    local arr = {}
    table.sort(player_set, function(a, b) return a > b end)
    local names = {}
    for _, v in ipairs(player_set) do
        if #camps > 0 then
            camp_no = table.remove(camps, 1)
        end

        if v <= 0 then
            -- AI数据是不完整的 不能用这个来作为玩家数据
            local account_info = {}
            local aiData = sharedata_utils.getAiDataByPlayerInfos(room_playerinfo)
            account_info.uin = v
            account_info.pvp_point = aiData.score
            account_info.pvp_score = math.floor(1, 100)
            account_info.icon_id = aiData.icon
            account_info.win_rate = aiData.rate
            account_info.winCount = aiData.winCount
            account_info.skinData = aiData.skinData
            account_info.assitData = aiData.assitData
            account_info.glyphsData = aiData.glyphsData
            account_info.real_point = aiData.realScore -- 计算电脑分数的时候 要用realScore
            account_info.name = aiData.name
            account_info.playerAI = aiData.playerAI or 0 
            room_playerinfo[v] = account_info
        end
        camp_set[v] = camp_no
    end
end

local function room_exit()
    for _, v in ipairs(player_set) do
        if v > 0 then
            pcall(room_cluster_call, v, "lua", "kick_room", skynet.self())
        end
    end
    logger.trace('room exit')
    skynet.exit()
end

local CMD = {}

local room_exist_time = setting.game_setting.room_exist_time
local function room_check()
    local agent = 0
    for i, v in ipairs(player_set) do
        if v > 0 then
            agent = v
            break
        end
    end
    local agent_uin = room_playerinfo[agent].uin
    -- 平局
    CMD.end_fight(agent, draw_uin, skynet.time(), (room_msg[agent_uin] or 1) + 1)
    room_exit()
end

local function getSkinData(agent)
    local skinId
    if agent > 0 then
        local r = room_cluster_call(agent, "lua", "agent_skin")
        skinId = r
    else
        local info = room_playerinfo[agent]
        skinId = tonumber(info.skinData)
    end
    return  skinId
end

local function getAssistantData(agent)
    local assitId
    local assitLevel
    local data_list = {}
    if agent > 0 then
        local data = room_cluster_call(agent, "lua", "agent_assistant")
        if data and data.slot then
             for k, v in ipairs(data.slot) do 
                local stat = data.as[k]

                local bas = data.as[data.in_battle]
                local exp = stat.ExpTime
                if exp > os.time() then
                    assitId     = stat.AssistantId
                    assitLevel  = stat.Level

                    local conf = sharedata_utils.get_stat(COMMON.plan.AS_ASSISTANT, assitId)
                    local skillId = sharedata_utils.getAsstSkillByLevel(assitId, assitLevel)
                    local assitStat =  {
                        AssistantId   = assitId,
                        Level         = assitLevel,
                        Quality       = conf.Quality,
                        ActiveSkills  = {skillId},
                        PassiveSkills = data.PassiveSkills,
                    }
                end
             end
             table.insert(data_list, assitStat)
        end    
    else
        --现在没对机器人进行处理
        -- local info = room_playerinfo[agent]
        -- local temp = utils.split(info.assitData, ";")

        -- if info.assitData ~= "" then
        --     local t = utils.split(info.assitData, ";")
        --     assitId = t[1]
        --     assitLevel = t[2]
        -- else
        --     assitId = nil
        -- end
    end

    if not assitId then return {} end

    return data_list
end

local function getPvpGlyphData(agent)
    local data = {}
    local info = room_playerinfo[agent]
    local uin = info.uin
    if uin > 0 then
        local gds = skynet.call(redis_service, 'lua', 'hget_obj', 'glyphData', uin)
        if gds == nil then
            return {}
        end

        local configs = sharedata_utils.get_stat_all(COMMON.plan.Glyphs)
        for gid, v in pairs(gds) do
            if v.status == 'Active' then
                local config = configs[gid * 100 + v.level]
                if config then
                    local e = config.effectType
                    -- 厨艺生效类型： 都生效或者pvp生效
                    if e == 0 or e == 2 then
                        table.insert(data, gid * 100 + v.level)
                    end
                end
            end
        end
    else
        local str = info.glyphsData
        if str ~= "" then
            data = utils.split(str, ";")
        else
            return {}
        end
    end

    local r = {}
    for _, gid in ipairs(data) do
        local config = sharedata_utils.get_stat(COMMON.plan.Glyphs, gid)
        table.insert(r, {
            glyphID    = math.floor(gid/100),
            mainData   = config.mainAttr2,
            sideData   = config.sideAttr2,
            soul       = config.soul,
            buildLevel = tostring(config.buildLevel),
            glyphLevel = config.level,
        })
    end
    return r
end

-- 第一个参数为pvpserver的id
-- 第二个参数为room_flag
-- 第三个参数为房主agent的id 【只在固定开房时有用】
-- 第四个参数为房间参与人数
-- 第五个参数为进入房间玩家的信息
-- 第六个参数是代表指定的地图id
function CMD.open(t1, t2, t3, t4, agent_infos,map)
    room_start_time = skynet.time()
    room_flag = t2

    if room_flag == COMMON.room_flag.FIXED then
        room_opener = t3
    end
    room_type = t4

    local ai_camp_no = 0
    --记录agent_id 标记跨服agent_id
    for k, v in pairs(agent_infos) do
        if v.agent == 0 then
            table.insert(player_set, ai_camp_no)
            ai_camp_no = ai_camp_no - 1
        else
            real_count = real_count + 1
            table.insert(player_set, v.agent)
            if v.agent > 100000000 then
               other_server_agent[v.agent] = {gamenode = v.gamenode, agent = v.realagent}
            end

            local info = v.info or room_cluster_call(v.agent, "lua", "get_account_info")
            room_playerinfo[v.agent] = info
            room_msg[info.uin] = 0
        end
    end
    
    --获取地图，以及类型
    map_id = map or sharedata_utils.get_rand_map(room_type, room_playerinfo, room_flag)
    local camps = sharedata_utils.get_map_camp(room_type, room_flag, map_id)
    --获取阵营数据和ai数据
    get_camp_no(camps)

    --所有玩家数据
    local player_infos = {}
    for k, info in pairs(room_playerinfo) do
        local camp = camp_set[k]
        local data = {
            Uin     = info.uin,
            Camp    = camp,
            Name    = info.name,
            Icon    = info.icon_id,
            Point   = info.pvp_point,
            PropData  = k > 0 and room_cluster_call(k, "lua", "getPvpProp") or {},
            GlyphsInfo = getPvpGlyphData(k),
            AsInfo = getAssistantData(k),
            SkinId = getSkinData(k),
            PlayerAI = info.playerAI or 0,
        }
        table.insert(player_infos, data)
    end

    local battleId = os.time() .. "_" .. skynet.self() -- 战斗id: 时间戳_room服务ID
    for k, v in ipairs(player_set) do
        if v > 0 then
            room_cluster_call(v, "lua", "match_ret", skynet.self(), room_type,
                player_infos, room_flag, room_opener, battleId, map_id)
        end
    end

    skynet.timeout(room_exist_time, room_check)
end

function CMD.personal_open(...)
   CMD.open(...)
   for k,v in ipairs(player_set) do
      if v > 0 then
         room_cluster_call(v, "lua", "personal_room_msg_begin_fight",skynet.self(), room_type, room_opener)
      end
   end
end

-- pvp地图数据检测
local last_pvp_map_time
local pvp_map_time_max = setting.game_setting.pvp_map_time_max
local function map_check(source)
    if pvp_map_time_max <= 0 then return end
    local t = last_pvp_map_time + pvp_map_time_max - skynet.now()
    if t <= 0 then
        local len = 0
        for k, v in pairs(average_ping) do
            len = len + 1
        end

        if len <= 1 then
            local is_change = false
            for k, v in pairs(average_ping) do -- 一个人不换主机
                room_host = k
                is_change = true
                break
            end

            if not is_change then
                return
            end
        else
            for k, v in pairs(average_ping) do
                if k ~= room_host then
                    room_host = k
                    break
                end
            end
        end

        local data = {
            Uin = room_host,
            Ts = skynet.time()
        }
        broadcast(source, MESSAGE.CS_CMD_PVP_HOST, data, true)
        last_pvp_map_time = skynet.now()
    end

    skynet.timeout(t, function() map_check(source) end)
end

local ready_queue =  {} -- 准备队列
local ready_max_time = 10 * 100 -- 最长等待时间
local is_first_start = false -- 第一个玩家准备好标示
local is_room_open = false -- 房间开始标示

local function send_ready_resp(source)
    if is_room_open then return end

    is_room_open = true
    local data = {
        Ts = skynet.time()
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_START, data, true)
end

function CMD.start_fight(source, type, msgNo)
    local uin = room_playerinfo[source].uin
    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo

    table.insert(ready_queue, source)
    if not is_room_open then
        if not is_first_start then
            is_first_start = true
            skynet.timeout(ready_max_time, function() send_ready_resp(source) end)
        end

        if #ready_queue >= real_count then
            last_pvp_map_time = skynet.now()
            map_check(source)
            send_ready_resp(source)
        end
    end
end

function CMD.fight(source, src, dest, num, ts, formationId, msgNo)
    local uin = room_playerinfo[source].uin
    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo

    --广播给其他agent
    local data = {
        Uin = room_playerinfo[source].uin,
        Src = src,
        Dest = dest,
        Num = num,
        Ts = ts,
        FormationId = formationId
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_FIGHT, data, true)
end

function CMD.upgrade(source, buildid, ts, level, msgNo)
    local uin = room_playerinfo[source].uin
    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo

    local data = {
        Uin = room_playerinfo[source].uin,
        BuildId = buildid,
        Ts = ts,
        Level = level,
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_UPGRADE, data, true)
end

function CMD.map(source, ts, data, msgNo)
    -- 判断source是不是主机id
    local uin = room_playerinfo[source].uin
    if not uin then return end
    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo

    if uin ~= room_host then
        logger.tracef("not room_host, can't send map %d", uin)
        return
    else
        last_pvp_map_time = skynet.now()
    end

    local data = {
        Ts = ts,
        Data = data,
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_MAP, data)
end

-- ping消息 发三次之后计算平均值 并返回给客户端
local is_ping_ret = false
function CMD.ping(source, ts, msgNo)
    local uin = room_playerinfo[source].uin
    if not uin then return end
    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo

    if not average_ping[uin] then
        average_ping[uin] = {}
    end
    if #average_ping[uin] >= 3 then
        table.remove(average_ping[uin], 1)
    end
    table.insert(average_ping[uin], ts)

    if not is_ping_ret and #average_ping[uin] >= 3 then
        room_host = uin
        local data = {
            Uin = room_host,
            Ts = skynet.time(),
        }
        is_ping_ret = true
        broadcast(source, MESSAGE.CS_CMD_PVP_HOST, data, true)
    end
end

local function process_ai_pvp_end(v, room_flag, room_type, room_opener, pvp_end_type, room_playerinfo, win_uin)
    -- 从排行榜拉取数据 要求不重复
    local data = room_playerinfo[v]
    local addscore = 0 -- 读表
    local addpoint = 0 -- 公式

    addscore = sharedata_utils.calc_score(pvp_end_type, room_flag, data.pvp_point)
    addpoint = sharedata_utils.calc_point(pvp_end_type, room_flag, room_playerinfo, data, win_uin)

    local item  = {
        Uin         = data.uin,
        Name        = data.name,
        PrevScore   = data.pvp_score,
        AddScore    = addscore,
        WinRate     = data.win_rate,
        WinCount    = data.winCount,
        PrevPoint   = data.pvp_point,
        AddPoint    = addpoint,
        NewRank     = 100 + math.random(1, 500)
    }
    return item
end

-- 目前只做了一个假数据下发 如果要处理线下数据 这个地方还需要考虑很多相关的状态
local function process_offline_pvp_end(v, room_flag, room_type, room_opener, pvp_end_type, room_playerinfo, win_uin)
    local info = room_playerinfo[v]
    local addscore = 0 -- 读表
    local addpoint = 0 -- 公式
    local tmp_win_count = info.continue_win_count
    if pvp_end_type == COMMON.pvp_end_type.WIN then
        info.pvp_win_count = info.pvp_win_count + 1
        info.continue_win_count = info.continue_win_count + 1
    elseif pvp_end_type == COMMON.pvp_end_type.DRAW then
        info.pvp_draw_count = info.pvp_draw_count + 1
        info.continue_win_count = 0
    elseif pvp_end_type == COMMON.pvp_end_type.LOSE then
        info.pvp_lose_count = info.pvp_lose_count + 1
        info.continue_win_count = 0
    end
    addscore = sharedata_utils.calc_score(pvp_end_type, room_flag, info.pvp_point)
    addpoint = sharedata_utils.calc_point(pvp_end_type, room_flag, room_playerinfo, info, win_uin)
    info.pvp_score = info.pvp_score + addscore
    info.pvp_point = info.pvp_point + addpoint

    local item  = {
        Uin         = info.uin,
        Name        = info.name,
        PrevScore   = info.pvp_score - addscore,
        AddScore    = addscore,
        WinRate     = utils.getPvpWinRate(info),
        WinCount    = tmp_win_count,
        PrevPoint   = info.pvp_point - addpoint,
        AddPoint    = addpoint,
        NewRank     = skynet.call(rank_service_v2, "lua", "get_rank", info.uin)
    }
    return item
end

local function  is_alive_agent(agent, uin)
    if agent > 100000000 then
        local t = other_server_agent[agent]
        return cluster.call(t.gamenode,  ".gamed", "get_alive_agent", uin)
    else
        return skynet.call(gamed, "lua", "get_alive_agent", uin)
    end
end

local is_send_end = false
function CMD.end_fight(source, win_uin, ts, msgNo)
    if is_send_end then return end
    is_send_end = true

    local uin = room_playerinfo[source].uin
    if type(room_msg[uin]) ~= 'number' then
        return
    end
    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo
    local pvp_result = {}
    -- 1 房间类型【1,2,3】    room_flag
    -- 2 房间人数【2,4】      room_type
    -- 3 房主uin            room_opener
    -- 4 胜利者uin           win_uin
    -- 5 房间玩家信息         room_playerinfo
    for i, v in ipairs(player_set) do
        local result
        local pvp_end_type = get_pvp_end_type(v, win_uin)
        if v > 0 then
            local ok
            local battleTime =  os.time() > room_start_time and os.time() - room_start_time or 0
            local is_alive_agent = is_alive_agent(v, room_playerinfo[v].uin)
            if is_alive_agent then
                ok, result = pcall(room_cluster_call, v, "lua", "process_pvp_end",
                    room_flag, room_type, room_opener, pvp_end_type, room_playerinfo,
                    win_uin, leave_mode[room_playerinfo[v].uin], math.floor(battleTime))
                if not ok then
                    logger.trace("pvp_end error v : " .. v)
                    break
                end
            else
                result = process_offline_pvp_end(v, room_flag, room_type, room_opener, pvp_end_type, room_playerinfo, win_uin)
            end
        else
            result = process_ai_pvp_end(v, room_flag, room_type, room_opener, pvp_end_type, room_playerinfo, win_uin)
        end
        table.insert(pvp_result, result)
    end

    local data = {
        Win = win_uin,
        Ts = ts,
        Result = pvp_result,
    }
    
    broadcast(source, MESSAGE.CS_CMD_PVP_END, data, true)

    -- 结算后发宝箱奖励消息
    if room_flag == COMMON.room_flag.NOPROP then
        for i, v in ipairs(player_set) do
            local info = {
                eventSource = 'pvp_end',
                is_win = get_pvp_end_type(v, win_uin),
            }
            pcall(room_cluster_call, v, 'lua', 'send_pvp_award', info)
        end
    end
    room_exit()
end

function CMD.leave(source, msgNo, leaveMode)
    if not room_playerinfo[source] then return end
    local uin = room_playerinfo[source].uin
    if msgNo and msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo
    if leaveMode then
        leave_mode[uin] = leaveMode
    end
    local is_change_host = false
    -- 主机离开，改变主机
    if uin == room_host then
        for k, v in pairs(average_ping) do
            if k ~= room_host then
                room_host = k
                is_change_host = true
                break
            end
        end
    end

    average_ping[uin] = nil     -- 离开的玩家不能再次变为主机

    local count = 0 --房间剩余人数
    for k, v in pairs(room_playerinfo) do
        if k > 0 then
            if not leave_mode[v.uin] then
                count = count + 1
            end
        end
    end

    if count > 0 then
        if is_change_host then
            local data = { Uin = room_host, Ts = skynet.time() }
            broadcast(source, MESSAGE.CS_CMD_PVP_HOST, data, true)
        end

        if room_flag == COMMON.room_flag.NOPROP then
            local info = {
                eventSource = 'pvp_leave',
                leaveMode = leaveMode,
            }
            pcall(room_cluster_call, source, "lua", "send_pvp_award", info)
        end

        broadcast(source, MESSAGE.CS_CMD_PVP_LEAVE, { Uin = uin }, true)
    else
        -- 无道具场房间没有电脑，不考虑此分支
        pcall(room_cluster_call, source, "lua", "kick_room", skynet.self())
        -- 电脑赢 uin : 0
        CMD.end_fight(source, 0, skynet.time(), (room_msg[uin] or 1)  + 1)
    end
end

function CMD.use_item(source, data, msgNo)
    local uin = room_playerinfo[source].uin
    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo

    local send_data = {
        Uin = data.uin,
        ItemId = data.itemId,
        Ts = data.ts,
        X = data.x,
        Y = data.y,
        BuildId = data.buildid,
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_USE_ITEM, send_data, true)
end

function CMD.pvpchat(source, data)
    local account_info = room_playerinfo[source]
    local send_data = {
        Channel = data.Channel,
        Uin     = account_info.uin,
        Id      = data.Id,
        Name    = data.Name,
        Content = data.Content,
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_CHAT, send_data, true)
end

function CMD.appear_creep(source, creepId, ts, dataId, msgNo)
    local uin = room_playerinfo[source].uin
    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo

    local send_data = {
        CreepId = creepId,
        Ts      = ts,
        DataId  = dataId,
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_APPEAR_CREEP, send_data, true)
end

-- 使用精灵的主动技能
function CMD.use_as_skill(source, camp_id, asst_id, skill_json)
    local msg = {
        Ok            = true,
        CampID        = camp_id,
        AsstID        = asst_id,
        SkillJson     = skill_json,
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_USE_AS_SKILL, msg, true)
end

function CMD.observer(source, msgNo, ob_uin)
    local uin = room_playerinfo[source].uin
    if uin ~= room_host then
        logger.tracef("not room_host, can't send observer %d", uin)
        return
    end

    if msgNo <= room_msg[uin] then return end
    room_msg[uin] = msgNo

    local send_data = {
        Uin = ob_uin,
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_OBSERVER, send_data, true)
end

function CMD.recover_item(source, uin, itemId)
    for k, v in pairs(room_playerinfo) do
        if k > 0 and v.uin == uin then
            pcall(room_cluster_call, k, "lua", "recover_item_ret", uin, itemId)
        end
    end
end

function CMD.trap(source, refreshGroupId, groupId, trapId, state)
    local send_data = {
        RefreshGroupId      = refreshGroupId,
        GroupId             = groupId,
        TrapId              = trapId,
        State               = state
    }
    broadcast(source, MESSAGE.CS_CMD_PVP_TRAP, send_data, true)
end

function CMD.host(source)
    return room_host
end

--针对跨服发往room的消息转发处理，如果希望source被处理,那么source需要在发送时加上所在服的agent_add_value
function CMD.cmd_forward(_, source, cmd_funcname,...)
    local f = CMD[cmd_funcname]
    if f then
        local ok, ret = xpcall(f, traceback, source, ...)
        return ret
    end
end

skynet.start(function()
    redis_service = skynet.queryservice("redis_service")
    rank_service_v2 = skynet.queryservice("rank_service_v2")
    busilog_service = skynet.queryservice("business_log_service")
    gamed           = skynet.queryservice("gamed")
    skynet.dispatch("lua", function(_, source, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, source, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        return skynet.retpack(ret)
    end)
end)


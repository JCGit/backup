local skynet = require "skynet"
local logger = require "logger"
local setting = require "setting"
local COMMON = require "common"
local sharedata_utils = require "sharedata_utils"
local utils = require "utils"

-- 随机匹配和无道具场匹配差值
local rand_diff_point = 600
local fair_diff_point = 600
local map_id = 0

local room_pool = {}
local config = {}
config.pool = 5

-- key : agent; value : account_info
local pvp_match_info = {}

-- 匹配池【随机匹配 和 无道具场】
local pvp_match_pool = {
    [COMMON.room_flag.RAND] = {},
    [COMMON.room_flag.NOPROP] = {},
    [COMMON.room_flag.HAPPY] = {},
}

local CMD = {}

local function get_room()
    local room
    if #room_pool == 0 then
        room = skynet.newservice("room", skynet.self())
        logger.trace("room pool empty, new room service")
    else
        room = table.remove(room_pool, 1)
    end
    return room
end

--[[
 匹配池中是Team的数组 pools = [team]
 Team               队伍
    flag            房间类型
    mode            类型
    players         队伍中的人[agent => account_info]
    createTime      创建时间
 ]]

local function getTeamPlayerCount(team)
    local count = 0
    for _, _ in pairs(team.players) do
        count = count + 1
    end
    return count
end

--[[
-- 新建队伍
-- 当匹配池中没有队伍时
-- 当新进入匹配池中的人在匹配池中同类型的队伍中并没有满足条件的
-- _mode : 可选参数 如果不传就按权重随机 如果传就按参数设置
 ]]

local function createTeam(room_flag, agent, _mode)
    local player_info = pvp_match_info[agent]
    local rand_mode = _mode or sharedata_utils.get_rand_mode(room_flag, player_info)
    local team = {
        flag        = room_flag,
        mode        = rand_mode,
        players     = {},
        createTime  = os.time(),
    }
    team.players[agent] = player_info

    table.insert(pvp_match_pool[room_flag], team)
end

-- 判断两个玩家能匹配到一起
local function check_play_together(room, agent1, agent2)
    -- 1 分差小于`*_diff_point`
    -- 2 玩家pvp场次小于5场
    local player_info1 = pvp_match_info[agent1]
    local player_info2 = pvp_match_info[agent2]

    local dif_point = math.abs(player_info1.pvp_point - player_info2.pvp_point)
    if room == COMMON.room_flag.NOPROP and dif_point > fair_diff_point then
      return false
    elseif room == COMMON.room_flag.RAND and dif_point > rand_diff_point then
      return false
    end

    local is_over_base = false
    local is_less_base = false
    local player_infos = {player_info1, player_info2}
    for _, info in pairs(player_infos) do
        if info.pvp_point <= 0 then
            is_less_base = true
        else
            is_over_base = true
        end
    end

    if is_less_base and is_over_base then
        return false
    else
        return true
    end
end

-- 判断是否能加入队伍
local function checkCanJoinTeam(team, agent)
    -- 判断agent这个玩家的mode能否为team.mode
    local playerInfo = pvp_match_info[agent]
    if not sharedata_utils.check_rand_mode(team.flag, team.mode, playerInfo) then
        return false
    end

    for k, _ in pairs(team.players) do
        if not check_play_together(team.falg, agent, k) then
            return false
        end
    end
    return true
end

local function removeTeam(team)
    for agent, _ in pairs(team.players) do
        pvp_match_info[agent] = nil
    end

    for i, v in ipairs(pvp_match_pool[team.flag]) do
        if v == team then
            table.remove(pvp_match_pool[team.flag], i)
        end
    end
end

-- 进入战斗房间
local function enterRoom(team)
    -- 移除队伍
    removeTeam(team)

    -- 创建pvp房间
    local room = get_room()
    local agent_infos = {}
    local count = 0
    for k, v in pairs(team.players) do
        table.insert(agent_infos, {agent = k, info = v})
        count = count + 1
    end

    if count < team.mode then --人数不足 补AI
        for i = count + 1, team.mode do
            table.insert(agent_infos, {agent = 0, info = {}})
        end
    end
    skynet.call(room, "lua", "open", team.flag, 0, team.mode, agent_infos, (map_id ~= 0 and map_id or false))
end

-- 加入队伍
local function joinTeam(team, agent)
    local player_info = pvp_match_info[agent]
    -- 加入队伍
    team.players[agent] = player_info

    if team.mode == getTeamPlayerCount(team) then
        enterRoom(team)
    end
end

local function changeTeamMode(team, mode)
    local can_change = true
    for _, v in pairs(team.players) do
        if not sharedata_utils.check_rand_mode(team.flag, team.mode, v) then
            can_change = false
        end
    end
    if can_change then
        team.mode = mode
    end
end

local max_wait_time = 18 --最长等待时间
local function process_match_pool(flag)
    for _, team in ipairs(pvp_match_pool[flag]) do
        local teamCount = getTeamPlayerCount(team)
        -- 匹配时间到了
        if os.time() - team.createTime >= max_wait_time then
            -- 随机匹配
            -- 如果 看队伍中是否有>2500分的人 如果有 且大于等于2人 则直接改变类型进入房间
            -- 否则 不改变房间类型 直接补全AI 开始比赛
            if team.flag == COMMON.room_flag.RAND then
                local max_point = 0
                for _, v in pairs(team.players) do
                    max_point = v.pvp_point > max_point and v.pvp_point or max_point
                end

                if max_point >= 2500 then
                    if teamCount >= 2 then
                        changeTeamMode(team, teamCount)
                    end
                end
                enterRoom(team)
            end

            -- 无道具场  欢乐道具场
            -- 如果队伍人数大于等于2人 则直接改变类型进入房间
            -- 否则 解散队伍 重新进入匹配池
            if team.flag == COMMON.room_flag.NOPROP 
                or team.flag == COMMON.room_flag.HAPPY then
                if teamCount >= 2 then
                    changeTeamMode(team, teamCount)
                    enterRoom(team)
                else -- 只有一个人
                    local agent
                    local info
                    for k, v in pairs(team.players) do
                        agent = k
                        info = v
                        removeTeam(team)
                        CMD.match(agent, flag, info)
                    end
                end
            end
        end
    end
end

local MATCH_TIME = setting.game_setting.MATCH_TIME --匹配时间
local function start_match()
    for flag, _ in pairs(pvp_match_pool) do
        process_match_pool(flag)
    end
    skynet.timeout(MATCH_TIME, start_match)
end

-- 获取队伍平均积分
local function getTeamAvgPoint(team)
    local totalPoint = 0
    local count = 0
    for _, v in pairs(team.players) do
        count   = count + 1
        totalPoint =  totalPoint + v.pvp_score
    end

    if count == 0 then return 0 end
    return math.floor(totalPoint/count)
end

-- 获取分差绝对值
local function getDifPoint(team, info)
    local avgPoint = getTeamAvgPoint(team)
    return math.abs(avgPoint - info.pvp_point)
end

-- 匹配
-- 优先看是否有等待超过15s的的队伍 如果有且条件达到 则进入队伍【队伍创建时间长的优先】
-- 如果没有 则选择分数接近的队伍 如果条件达到 则进入队伍 【队伍平均数分数接近优先】
-- 如果没有 则按权重创建队伍
function CMD.match(agent, flag, info)
    pvp_match_info[agent] = info
    if #pvp_match_pool[flag] > 0 then
        -- 遍历所有队伍 按照时间先后顺序 超过15s的按这个规则
        table.sort(pvp_match_pool[flag], function(a, b)
                return a.createTime < b.createTime -- 时间越小 距离现在就越长
        end)
        for _, team in ipairs(pvp_match_pool[flag]) do
            if os.time() - team.createTime >= 15 then
                if checkCanJoinTeam(team, agent) then
                    joinTeam(team, agent)
                    return
                end
            end
        end

        -- 遍历所有队伍 按照平均积分先后顺序
        table.sort(pvp_match_pool[flag], function(a, b)
            return getDifPoint(a, info) < getDifPoint(b, info)
        end)

        for _, team in ipairs(pvp_match_pool[flag]) do
            if checkCanJoinTeam(team, agent) then
                joinTeam(team, agent)
                return
            end
        end
    end

    -- 没有可加入的队伍就创建队伍
    createTeam(flag, agent)
end

-- 从队伍中移除人  如果队伍人数为0 则移除队伍
local function removePlayerFromTeam(team, agent)
    pvp_match_info[agent] = nil
    for k, _ in pairs(team.players) do
        if k == agent then
            team.players[agent] = nil
        end
    end

    if getTeamPlayerCount(team) == 0 then
        removeTeam(team)
    end
end

-- 停止匹配[从队伍中移除]
function CMD.stop_match(_, agent, _)
    for flag, _ in pairs(pvp_match_pool) do
        for _, team in ipairs(pvp_match_pool[flag]) do
            removePlayerFromTeam(team, agent)
        end
    end
end

function CMD.open()
    logger.trace("pvpserver opened")

    local n = config.pool or 0
    for i = 1, n do
        table.insert(room_pool, skynet.newservice("room", skynet.self()))
    end

    skynet.timeout(MATCH_TIME, start_match) -- 时间驱动匹配
end

function CMD.set_rand_diff_point(value)
    rand_diff_point = value
    logger.trace("set rand_diff_point:", rand_diff_point)
end

function CMD.set_fair_diff_point(value)
    fair_diff_point = value
    logger.trace("set fair_diff_point:", fair_diff_point)
end

function CMD.set_rand_map(map)
    logger.trace("set set_rand_map: ", map)
    map_id = map
end

local traceback = debug.traceback
skynet.start(function()
    skynet.dispatch("lua", function(_, _, command, ...)
        --logger.trace("command  : " .. command)
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



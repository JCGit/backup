local skynet = require "skynet"
local logger = require "logger"
local setting = require "setting"
local COMMON = require "common"
local utils = require "utils"
local cluster = require "cluster"



local gamed
local traceback = debug.traceback

local pvp_room_server = {
    rooms = {},
    match_room_pool = {},
    match_room_pool_size = setting.pvp_room_pool_size or 10,
    ID = 1,
    status_normal = 1,  -- 正常
    status_count_down = 2, -- 倒计时
    started_room = {},  -- 已开始的房间
    map_id = false, -- 默认所有的房间地图id
}

function pvp_room_server:get_room()
    return table.remove(self.match_room_pool)
        or skynet.newservice("room", skynet.self())
end

function pvp_room_server.timer_fun()
    local now = math.floor(skynet.time())
    local fight_room = {}
    for room_id,room in pairs(pvp_room_server.rooms) do
        if room.status == pvp_room_server.status_normal then
            if room.time ~= 0 and now - room.time >= 45 then
                room.status = pvp_room_server.status_count_down
                pvp_room_server:count_down(room_id,15)
            elseif #room.players == room.max_player_num and now - room.time >= 15 then
                -- 开始倒计时
                room.status = pvp_room_server.status_count_down
                room.time = now - 50
                pvp_room_server:count_down(room_id,10)
            end
        elseif room.status == pvp_room_server.status_count_down then
            if now - room.time >= 60 then
                fight_room[room_id] = room
            end
        end
    end

    for room_id,room in pairs(fight_room) do
        pvp_room_server:begin_fight(room_id,room.host)
    end
    skynet.timeout(100, pvp_room_server.timer_fun)
end

function pvp_room_server:open()
    logger.trace("pvp_room_server opened")

    for i = 1, self.match_room_pool_size do
        table.insert(self.match_room_pool, skynet.newservice("room", skynet.self()))
    end
    -- 每20秒进行一次检查
    skynet.timeout(100, pvp_room_server.timer_fun)
end

-- 开房
function pvp_room_server:create(uin, agent)
    self.ID = self.ID + 1
    local id = self.ID
    local new_room = {
        host = uin,
        max_player_num = 4,
        global_broadcast_time = 0,
        players = {
            {uin = uin, agent = agent , index=1}
        },
        time = 0,
        requests = {}, -- 邀请玩家uin列表
        status = self.status_normal
    }
    self.rooms[id] = new_room
    return id
end

function pvp_room_server:count_down(room_id,seconds)
    local room = self.rooms[room_id]
    if not room then
        return
    end

    for i,player in ipairs(room.players) do
        if player.gamenode then
            xpcall(cluster.call, traceback, player.gamenode,
                ".cluster_agent_server", "agent_forward", player.realagent,
                "handle_personal_room_count_down", room_id, seconds)
        else
            skynet.call(player.agent, "lua",
                "handle_personal_room_count_down", room_id, seconds)
        end
    end
end

-- 加入房间
function pvp_room_server:enter(room_id, uin, agent, other_server_node)
    local ret = {}

    -- 房间已开战
    if self.started_room[room_id] then
        ret.error_code = COMMON.ERRORCODE.PERSONAL_ROOM_ROOM_HAS_START
        return ret
    end

    local room = self.rooms[room_id]
    if not room then
        ret.error_code = COMMON.ERRORCODE.PERSONAL_ROOM_NOT_EXIST
        return ret
    end

    -- 检查房间是否已倒计时

    -- 检查房间是否已满
    if #room.players >= room.max_player_num then
        ret.error_code = COMMON.ERRORCODE.PERSONAL_ROOM_ROOM_IS_FULL
        return ret
    end

    if room.time == 0 then
        room.time = math.floor(skynet.time())
    end

    local new_player = {uin = uin, agent = agent, index=#room.players + 1}
    if other_server_node then --跨服
        new_player.gamenode = other_server_node
        new_player.realagent = agent

        new_player.agent =  utils.get_agent_add_value(other_server_node) + agent
    end
    table.insert(room.players,new_player)

    local players = {}
    -- 通知agent
    for i,player in ipairs(room.players) do
        if uin ~= player.uin then
            if player.gamenode then
                xpcall(cluster.call, traceback, player.gamenode,
                    ".cluster_agent_server", "agent_forward", player.realagent,
                    "personal_room_msg_enter", room_id, new_player)
            else
                skynet.call(player.agent, "lua",
                    "personal_room_msg_enter", room_id, new_player)
            end
        end
        table.insert(players,player)
    end

    local left_seconds = setting.constant.player_personal_room_time_limit - (math.floor(skynet.now()) - room.time)

    ret = {room_id=room_id,players=players,left_seconds = left_seconds, mode = room.max_player_num}
    self:invite_personal_room_resp(room_id,uin)

    return ret
end

-- 退出房间
function pvp_room_server:leave(room_id, uin, flag)
    local room = self.rooms[room_id]
    if not room then
        return
    end

    -- 房主退出，销毁房间
    if room.host == uin then
        self.rooms[room_id] = nil
        for i,player in ipairs(room.players) do
            if flag and (player.uin == uin) then return end -- 特殊处理 不给自己广播
            if player.gamenode then
                xpcall(cluster.call, traceback, player.gamenode,
                    ".cluster_agent_server", "agent_forward", player.realagent,
                    "personal_room_destory", room_id)
            else
                skynet.call(player.agent, "lua", "personal_room_destory", room_id)
            end
        end
        return
    end

    for i,player in ipairs(room.players) do
        if player.uin == uin then
            table.remove(room.players,i)
            break
        end
    end

    if #room.players == 1 then
        room.time = 0
    end

    -- 通知agent删除退出的玩家
    for i,player in ipairs(room.players) do
        player.index = i
        if flag and (player.uin == uin) then return end -- 特殊处理 不给自己广播
        if player.gamenode then
            xpcall(cluster.call, traceback, player.gamenode,
                ".cluster_agent_server", "agent_forward", player.realagent,
                "personal_room_msg_leave", room_id, uin)
        else
            pcall(skynet.call, player.agent, "lua",
                "personal_room_msg_leave", room_id, uin)
        end
    end

    -- 通知agent房间删除-目前仅能通知本服数据
    for i, v in ipairs(room.requests) do
        local uin_agent = skynet.call(gamed, "lua", "get_agent", v)
        if uin_agent then
            pcall(skynet.call, uin_agent, "lua", "personal_room_msg_leave", room_id, uin, true)
        end
    end

    room.status = self.status_normal
    room.time = 0
end

-- 踢出房间
function pvp_room_server:kick(room_id, my_uin, other_uin)
    local room = self.rooms[room_id]
    if not room then
        return
    end

    if room.host ~= my_uin or my_uin == other_uin then
        return
    end

    -- 删除踢出的玩家
    local del_index
    for i,player in ipairs(room.players) do
        if player.uin == other_uin then
            del_index = i
        end

        if player.gamenode then
            xpcall(cluster.call, traceback, player.gamenode,
                ".cluster_agent_server", "agent_forward", player.realagent,
                "personal_room_msg_leave", room_id, other_uin)
        else
            pcall(skynet.call, player.agent, "lua",
                "personal_room_msg_leave", room_id, other_uin)
        end
    end

    if del_index then
        table.remove(room.players,del_index)
    end

    -- 重新计算index
    for i,player in ipairs(room.players) do
        player.index = i
    end

    room.status = self.status_normal
    room.time = 0
end

function pvp_room_server:invite_personal_room(room_id, uin)
    local room = self.rooms[room_id]
    if not room then return end
    for i, v in ipairs(room.requests) do
        if v == uin then return end
    end
    table.insert(room.requests, uin)
end

function pvp_room_server:invite_personal_room_resp(room_id, uin)
    local room = self.rooms[room_id]
    if not room then return end
    for i, v in ipairs(room.requests) do
        if v == uin then
            table.remove(room.requests, i)
        end
    end
end

function pvp_room_server:mode(roomid, my_uin, mode)
    local room = self.rooms[roomid]
    if not room then
        return false
    end

    if room.host ~= my_uin then
        return false
    end

    if #room.players > mode then
        return false
    end
    room.max_player_num = mode

    for i,player in ipairs(room.players) do
        if my_uin ~= player.uin then
            if player.gamenode then
                xpcall(cluster.call, traceback, player.gamenode,
                    ".cluster_agent_server", "agent_forward", player.realagent,
                    "personal_room_msg_mode", roomid, mode)
            else
                skynet.call(player.agent, "lua", "personal_room_msg_mode", roomid, mode)
            end
        end
    end

    return true
end

function pvp_room_server:chat(room_id, my_uin, id, words)
    local room = self.rooms[room_id]
    if not room then
        return
    end

    for i,player in ipairs(room.players) do
        if my_uin ~= player.uin then
            if player.gamenode then
                xpcall(cluster.call, traceback, player.gamenode,
                    ".cluster_agent_server", "agent_forward", player.realagent,
                    "personal_room_msg_chat", room_id, my_uin, id, words)
            else
                skynet.call(player.agent, "lua", "personal_room_msg_chat", room_id, my_uin, id, words)
            end
        end
    end
end

-- 开始战斗
function pvp_room_server:begin_fight(room_id, my_uin)
    local room = self.rooms[room_id]
    if not room then
        return
    end

    if room.host ~= my_uin then
        return
    end

    if #room.players < 2 then
        return
    end

    local fight_room = self:get_room()
    if not fight_room then
        return
    end

    self.rooms[room_id] = nil
    self.started_room[room_id] = true

    if #room.players < room.max_player_num then
        room.max_player_num = #room.players
    end

    skynet.call(fight_room, "lua", "personal_open", COMMON.room_flag.FIXED, room.host, room.max_player_num, room.players, self.map_id)

    -- 进入房间后 通知邀请列表中的人房间销毁了-目前这里仅能处理本服的agent
    for i, v in ipairs(room.requests) do
        local uin_agent = skynet.call(gamed, "lua", "get_agent", v)
        if uin_agent then
            pcall(skynet.call, uin_agent, "lua", "personal_room_msg_leave", room_id, my_uin, true)
        end
    end
end

function pvp_room_server:set_map_id(value)
  logger.debugf("pvp_room_server:set_map_id  %d", value)
  self.map_id = tonumber(value)
end


skynet.start(function()

    gamed = skynet.queryservice("gamed")

    skynet.dispatch("lua", function (_, _, command, ...)
        local f = pvp_room_server[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, debug.traceback, pvp_room_server, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        return skynet.retpack(ret)
    end)
end)

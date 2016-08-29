-- User: HuangLijun
-- Date: 2015/9/21
-- Time: 16:15
-- Desc: agent的pvp模块
--

local skynet = require "skynet"
local cluster = require "cluster"

local sharedata_utils        = require "sharedata_utils"
local utils                  = require "utils"
local setting                = require "setting"
local logger                 = require "logger"
local proto                  = require "proto"
local player                 = require "player"
local setting                = require "setting"
local server_config          = require "server_config"
local agent_activity         = require "agent_activity"
local agent_vip              = require "agent_vip"
local agent_achievement      = require "agent_achievement"
local activity_valentine     = require "activity_valentine"
local agent_pvp_chest        = require "agent_pvp_chest"
local agent_operate_activity = require "agent_operate_activity"
local COMMON                 = require "common"
local MESSAGE                = COMMON.MESSAGE
local ERRORCODE              = COMMON.ERRORCODE
local agent_item             = require "agent_item"
local agent_pay_guide        = require "agent_pay_guide"
local agent_player           = require "agent_player"
local agent_pvpinfo          = require "agent_pvpinfo"
local agent_pve              = require "agent_pve"

local agent_pvp = {}

local local_nodename
local agent_add_value
local pvpserver
local pvp_room_server
local rank_service_v2
local gamed
local chat_service
local redis_service
local busilog_service

local self_ranklist_cache = {}

local traceback = debug.traceback

local agent
local account_info
local uin
local mapId
local battleId

-- init 时设置

local send_package, send_response, send_error_code
local can_send_pvp_award

local notice_data = {}
local notice_shareId         -- 战斗结束分享Id
local notice_pvp_chestId     -- 竞技场宝箱id
local notice_pvp_chestType   -- 竞技场宝箱类型
local notice_pay_guide_item  -- 付费引导id
local notice_pay_guide_count -- 付费引导数量
local battle_fail_itemid = 0     -- pvp战斗中途失败付费引导的道具id

-- 内部变量
-- 房间
agent_pvp.roomid = nil              -- 房间ID，可能是个人房间，或者是自动匹配的
agent_pvp.room_is_personal = false  -- 房间是否是个人房间，不是的话就是自动匹配的
agent_pvp.has_handle_match = false  -- 自动匹配中
agent_pvp.personal_room_id = 0      -- 个人房间
agent_pvp.is_personal_host = false  -- 是否是个人房间的房主
agent_pvp.room_server_node = nil    --房间所属服务节点 在本服为nil跨服为房间所在服的gamenode名

local room_started_ = false -- 战斗是否开始， 否则将不进行结算

local function skynet_room_call(room_id, func, ...)
    if agent_pvp.room_server_node then
        cluster.call(agent_pvp.room_server_node, ".cluster_agent_server",
            "agent_forward", room_id, "cmd_forward", agent + agent_add_value, func, ...)
    else
        skynet.call(room_id, 'lua', func, ...)
    end
end

------------------------ 以下是个人房间服务客户端接口 ------------------------

-- 处理玩家离开房间的内存数据
local function process_leave_pvp_room()
    agent_pvp.roomid = nil
    agent_pvp.room_server_node = nil
end

-- 请求创建个人房间
function agent_pvp.handle_create_personal_room(cmdid, content, len)
    --开房前判断房间存不存在 如果存在 先踢掉 然后继续
    if agent_pvp.roomid then
        pcall(skynet_room_call, agent_pvp.roomid, "leave")
        process_leave_pvp_room()
        agent_pvp.personal_room_id = 0
    end

    --玩家已经在房间 将玩家从原来的房间踢出来 继续操作
    if agent_pvp.personal_room_id ~= 0 then
        if agent_pvp.room_server_node then
            xpcall(cluster.call, traceback, agent_pvp.room_server_node, "pvp_room_server", "leave", agent_pvp.personal_room_id,account_info.uin, true)
        else
            skynet.call(pvp_room_server, "lua", "leave", agent_pvp.personal_room_id,account_info.uin, true)
        end
        agent_pvp.personal_room_id = 0
    end

    local ErrCode
    repeat
      if agent_pvp.roomid or agent_pvp.has_handle_match or agent_pvp.personal_room_id ~= 0 then
          ErrCode = ERRORCODE.PERSONAL_ROOM_ALREADY_IN_ROOM
          break
      end
      -- 比赛中、自动匹配中、个人房间中不能创建房间
      if account_info.diamond < setting.constant.player_personal_room_create_price and account_info.room_ticket <= 0 then
          ErrCode = ERRORCODE.PERSONAL_ROOM_NOT_ENOUGH_DIAMOND
          break
      end
    until true

    if ErrCode then
        return send_error_code(cmdid, ErrCode)
    end

    local room_id = skynet.call(pvp_room_server, "lua", "create", account_info.uin, agent)
    agent_pvp.personal_room_id = room_id
    return send_response(cmdid, {RoomID=room_id})
end

function agent_pvp.get_player_info(account_info,index)
    local win_rate = utils.getPvpWinRate(account_info)
    local rank_info = cluster.call("ranknode1", ".pvp_rank_service",
                                    "get_player_rank_data", account_info.uin)
    local time = account_info.login_time or 0
    local info = {
        Uin     = account_info.uin,
        Name    = account_info.name or "",
        Icon    = account_info.icon_id or 0,
        -- Group   = rank_info.groupid,
        -- Rank    = rank_info.order,
        Group   = 1,
        Rank    = 0,
        Index   = index,
        WinRate = win_rate,
        Score   = account_info.pvp_point,
        Title   = account_info.PlayerTitle,
        Level   = account_info.achievement_level,
    }
    
    if rank_info then
        info.Group = rank_info.group
        info.Rank = rank_info.rank
    end
    local vip_data = agent_vip.get_vip_data_by_uin(account_info.uin)
    if vip_data then
        info.VipLevel = vip_data.vip_level
    end
    return info
end

-- 请求进入个人房间
function agent_pvp.handle_enter_personal_room(cmd, content, len)
    -- 进入房间前判断房间存不存在 如果存在 先踢掉 然后继续
    if agent_pvp.roomid then
        pcall(skynet_room_call, agent_pvp.roomid, "leave")
        process_leave_pvp_room()
        agent_pvp.personal_room_id = 0
    end

    -- 玩家已经在房间 将玩家从原来的房间踢出来 继续操作
    if agent_pvp.personal_room_id ~= 0 then
        if agent_pvp.room_server_node then
            xpcall(cluster.call, traceback, agent_pvp.room_server_node, "pvp_room_server","leave", agent_pvp.personal_room_id,account_info.uin, true)
        else
            skynet.call(pvp_room_server, "lua", "leave", agent_pvp.personal_room_id,account_info.uin, true)
        end
        agent_pvp.personal_room_id = 0
    end

    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.RoomID == nil then
        return
    end

    -- 比赛中、自动匹配中、个人房间中不能进入房间
    if agent_pvp.roomid or agent_pvp.has_handle_match or agent_pvp.personal_room_id ~= 0 then
        return send_error_code(cmd, ERRORCODE.PERSONAL_ROOM_ALREADY_IN_ROOM)
    end

    --不能在这里扣 要到进入房间的时候扣
    local room_server_node
    local ret
    if po.ServerNode ~= "" and po.ServerNode ~= local_nodename then --跨服
        local result
        result,ret = xpcall(cluster.call, traceback,
            po.ServerNode, "pvp_room_server", "enter",
            po.RoomID, account_info.uin, agent, local_nodename)
        room_server_node = po.ServerNode
    else
        ret = skynet.call(pvp_room_server, "lua", "enter", po.RoomID, account_info.uin, agent)
    end

    if ret.error_code then
        return send_error_code(cmd,  ret.error_code)
    end

    agent_pvp.personal_room_id = ret.room_id
    agent_pvp.room_server_node = room_server_node
    local resp = {RoomID=ret.room_id,Players={},LeftSeconds=ret.left_seconds, RoomType = ret.mode}
    if not resp then
        return
    end
    for _,player in pairs(ret.players) do
        local account = skynet.call(redis_service,"lua","hget_obj","account",player.uin)
        table.insert(resp.Players,agent_pvp.get_player_info(account,player.index))
    end
    send_response(cmd, resp)

    return send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_MODE, {Mode=ret.mode})
end

-- 请求离开个人房间
function agent_pvp.handle_leave_personal_room(cmdid, content, len)
    -- 比赛中、自动匹配中、个人房间中不能进入房间
    if agent_pvp.personal_room_id == 0 then
        return send_response(cmdid, {Uin=account_info.uin})
    end
    if agent_pvp.room_server_node then
       xpcall(cluster.call, traceback, agent_pvp.room_server_node, "pvp_room_server","leave", agent_pvp.personal_room_id,account_info.uin)
    else
       skynet.call(pvp_room_server, "lua", "leave", agent_pvp.personal_room_id,account_info.uin)
    end
    agent_pvp.personal_room_id = 0
    return send_response(cmdid, {Uin=account_info.uin})
end

--请求踢人离开个人房间
function agent_pvp.handle_kick_personal_room(cmd, content, len)
   local po = proto.parse_proto_req(cmd, content, len)
   if type(po) ~= "table" or po.Uin == nil then
       return
   end

   if agent_pvp.personal_room_id == 0 then
       return send_error_code(cmd, ERRORCODE.PERSONAL_ROOM_NOT_EXIST)
   end
   if agent_pvp.room_server_node then
      xpcall(cluster.call, traceback, agent_pvp.room_server_node, "pvp_room_server", "kick", agent_pvp.personal_room_id,account_info.uin,po.Uin)
   else
      skynet.call(pvp_room_server, "lua", "kick", agent_pvp.personal_room_id,account_info.uin,po.Uin)
   end
    return send_response(cmd, {Uin=po.Uin})
end

-- 个人房间邀请好友
function agent_pvp.handle_invite_personal_room(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.Uin == nil then
        return
    end

    if agent_pvp.personal_room_id == 0 then
        return
    end

    send_response(cmd, {Uin=po.Uin})

    local other_friend_info = skynet.call(redis_service,"lua","hget_obj","friend_info",po.Uin)
    if not other_friend_info then
        return
    end

    skynet.call(pvp_room_server, "lua", "invite_personal_room", agent_pvp.personal_room_id, po.Uin)

    local my_info = agent_pvp.get_player_info(account_info)
    my_info.NickName = my_info.Name
    my_info.Name = nil
    my_info.Index = nil
    my_info.Time = nil
    my_info.LastOnlineTime = 0
    local msg = {
        Info     = my_info,
        RoomID   = agent_pvp.personal_room_id,
        Time     = math.floor(skynet.time()),
        gamenode = local_nodename
    }
    local other_msg = {Info=msg,Uin=account_info.uin}
    return skynet.send(".online_mgr", "lua", "handle_invite_friend_personal_room", po.Uin,other_msg)
end

-- 个人房间全服邀请
function agent_pvp.handle_broadcast_personal_room(cmdid, content, len)
    if agent_pvp.personal_room_id == 0 then
        return
    end

    local vip_data = agent_vip.get_vip_data()
    local msg = {
        Uin        = account_info.uin,
        Name       = account_info.name or "",
        RoomID     = agent_pvp.personal_room_id,
        Time       = math.floor(skynet.time()),
        Icon       = account_info.icon_id,
        ServerNode = local_nodename,
        Title      = account_info.PlayerTitle,
        VipLevel   = vip_data and vip_data.vip_level
    }
    skynet.send(".online_mgr","lua","handle_personal_room_broadcast",msg)
    local gamed_list = cluster.call("gslbnode", ".gslbserver", "get_all_gamed")
    if gamed_list then
        for k,gamed_name in pairs(gamed_list) do
            local node_name = utils.query_nodename(gamed_name)
            if node_name and node_name ~= "" and node_name ~= local_nodename then
                cluster.call(node_name, ".cluster_agent_server",
                    "agent_forward_by_servicename", "gamed_online_mgr",
                    "send", "handle_personal_room_broadcast", msg)
            end
        end
    end
end

-- 个人房间聊天
function agent_pvp.handle_chat_personal_room(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.ID == nil then
        return
    end

    if agent_pvp.personal_room_id == 0 then
        return
    end

    --弹幕
    if po.ID == 4 then
        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_chat_barrage, account_info.uin)
    end

    if agent_pvp.room_server_node then
        xpcall(cluster.call, traceback, agent_pvp.room_server_node,
            "pvp_room_server", "chat", agent_pvp.personal_room_id,
            account_info.uin, po.ID, po.Words)
    else
        skynet.call(pvp_room_server, "lua", "chat",
            agent_pvp.personal_room_id, account_info.uin, po.ID, po.Words)
    end
    local resp = {Words=po.Words,Uin=account_info.uin,ID=po.ID}
    return send_response(cmd, resp)
end

-- 改变个人房间模式
function agent_pvp.handle_mode_personal_room(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.Mode == nil then
        return
    end
    if agent_pvp.personal_room_id == 0 then
        return send_error_code(cmd, ERRORCODE.PERSONAL_ROOM_NOT_EXIST)
    end

    skynet.call(pvp_room_server, "lua", "mode", agent_pvp.personal_room_id, account_info.uin, po.Mode)
    return send_response(cmd, { Mode = po.Mode })
end

-- 房主开战
function agent_pvp.handle_begin_fight_personal_room(cmd, content, len)
    if agent_pvp.personal_room_id == 0 then
        return
    end

    skynet.call(pvp_room_server, "lua", "begin_fight", agent_pvp.personal_room_id, account_info.uin)
    local resp = {RoomID=agent_pvp.personal_room_id}
    return send_response(cmd, resp)
end

function agent_pvp.handle_happy_items(cmd, content, len)
    local cfg_items = sharedata_utils.get_stat_all(COMMON.plan.PvpHappyItems)
    local total_rate = 0
    for k, v in pairs(cfg_items) do
        total_rate = total_rate + v.rate
    end

    local items = {}
    for i = 1, 3 do
        local rand_num = math.random(1, total_rate)
        for _, v in pairs(cfg_items) do
            if rand_num <= v.rate then
                table.insert(items, {Id = v.itemid, Count = v.count,})
                break
            else
                rand_num = rand_num - v.rate
            end
        end
    end

    local resp_data = {
        Items = items,
        NextTime = os.time() + 15,
    }
    send_response(cmd, resp_data)

end

function agent_pvp.handle_happy_use_item(cmd, content, len)
    send_response(cmd, {Ret = true,})
end

------------------------ 以下是个人房间服务返回的接口 ------------------------
-- 房间玩家列表
function agent_pvp.personal_room_destory()
    agent_pvp.personal_room_id = 0
    local resp = {Uin=account_info.uin}
    send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_LEAVE, resp)
end

-- 玩家进入房间
function agent_pvp.personal_room_msg_enter(room_id,new_player)
    local resp = {RoomID=room_id,Players={}}
    local account = skynet.call(redis_service,"lua","hget_obj","account",new_player.uin)
    local new_player_info = agent_pvp.get_player_info(account,new_player.index)
    table.insert(resp.Players,new_player_info)
    send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_ENTER, resp)
end

-- 玩家离开房间
function agent_pvp.personal_room_msg_leave(room_id,uin, flag)

    if uin == account_info.uin then
        agent_pvp.personal_room_id = 0
        agent_pvp.room_server_node = nil
    end
    local resp = { Uin = uin }

    if flag then
        resp.RoomID = room_id
    end
    return send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_LEAVE, resp)
end

-- 房间人数调整
function agent_pvp.personal_room_msg_mode(room_id,mode)
    local resp = {Mode=mode}
    return send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_MODE, resp)
end

-- 房间内聊天
function agent_pvp.personal_room_msg_chat(room_id,uin,id,words)
    local resp = {Uin=uin,Words=words,ID=id}
    send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_CHAT, resp)
end

-- 个人房间开战
function agent_pvp.personal_room_msg_begin_fight(room_id,type, host_uin)
    agent_pvp.personal_room_id = 0
    if host_uin == account_info.uin then
        --如果是房主 开战的时候扣减钻石
        if account_info.room_ticket > 0 then
            player.add_room_ticket(account_info, -1)
        else
            player.change_diamond(account_info, COMMON.change_src.pvp_room, -setting.constant.player_personal_room_create_price)
        end
    end
end

-- 收到全服约战申请
function agent_pvp.handle_personal_room_broadcast(msg)
   
    --if account_info.level == nil or account_info.level < 10 then
    -- if not agent_pve.played_id(99050) then
        -- return
    -- end
    send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_BROADCAST, msg)
end

-- 自己接受约战
function agent_pvp.handle_accept_personal_room_invite(the_room_id,the_friend_uin,room_server_node)
    if agent_pvp.roomid or agent_pvp.has_handle_match or agent_pvp.personal_room_id ~= 0 then
        return send_error_code(MESSAGE.CS_CMD_PERSONAL_ROOM_ENTER, ERRORCODE.PERSONAL_ROOM_ROOM_HAS_START)
    end

    -- 比赛中、自动匹配中、个人房间中不能进入房间
    skynet.call(agent, "lua", "save_account_info")
    local ret
    if room_server_node then --跨服
        local result
        result,ret = xpcall(cluster.call, traceback, room_server_node, "pvp_room_server", "enter", the_room_id, account_info.uin, agent, local_nodename)
    else
        ret = skynet.call(pvp_room_server, "lua", "enter", the_room_id, account_info.uin, agent)
    end
    if ret.error_code then
        return send_error_code(MESSAGE.CS_CMD_PERSONAL_ROOM_ENTER,  ret.error_code)
    end

    local resp = {RoomID=ret.room_id,Players={}}
    for _,player in pairs(ret.players) do
        local account = skynet.call(redis_service,"lua","hget_obj","account",player.uin)
        table.insert(resp.Players,agent_pvp.get_player_info(account,player.index))
    end
    agent_pvp.personal_room_id = ret.room_id
    agent_pvp.room_server_node = room_server_node
    send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_ENTER, resp)
    send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_MODE, {Mode=ret.mode})
end

function agent_pvp.handle_refuse_personal_room_invite(the_room_id,the_friend_uin,room_server_node)
    if agent_pvp.roomid or agent_pvp.has_handle_match or agent_pvp.personal_room_id ~= 0 then
        return send_error_code(MESSAGE.CS_CMD_PERSONAL_ROOM_ENTER, ERRORCODE.PERSONAL_ROOM_ROOM_HAS_START)
    end
    if room_server_node then
        xpcall(cluster.call, traceback, room_server_node, "pvp_room_server",
            "invite_personal_room_resp", the_room_id, account_info.uin)
    else
        skynet.call(pvp_room_server, "lua", "invite_personal_room_resp", the_room_id, account_info.uin)
    end
end

function agent_pvp.handle_personal_room_count_down(the_room_id,seconds)
    local msg = {RoomID=the_room_id,Seconds = seconds}
    return send_response(MESSAGE.CS_CMD_PERSONAL_ROOM_COUNT_DOWN,msg)
end

------------------------ 以上是个人房间服务返回的接口 ------------------------

-- 无道具场开启消耗200点荣誉
local noprop_need_score = 200

-- 开始自动匹配
agent_pvp.handle_match_msg = function(cmdid, content, len)
    local decode = proto.parse_proto_req(cmdid, content, len)
    local type = decode.Type
    local mode = decode.Mode

    room_started_ = false

    -- 匹配前判断房间存不存在 如果存在 先踢掉 然后继续
    if agent_pvp.roomid then
        pcall(skynet_room_call, agent_pvp.roomid, "leave")
        process_leave_pvp_room()
        agent_pvp.personal_room_id = 0
    end

    agent_pvp.room_server_node = nil -- 随机匹配是不可能跨服的

    if mode == COMMON.room_flag.NOPROP then
        if account_info.pvp_score < noprop_need_score then
            return send_error_code(cmdid, ERRORCODE.SCORE_NOT_ENOUGH)
        end

        local segments = sharedata_utils.get_stat_all(COMMON.plan.PvpMatch)
        local now = os.time()
        if not utils.IsInSegments(segments, now) then
            return send_error_code(cmdid, ERRORCODE. NOT_PVP_MATCH_TIME)
        end

        --去掉次数限制
        -- if account_info.noprop_count <= 0 then
            -- return send_error_code(cmdid, ERRORCODE.NOT_NOPROP_COUNT)
        -- end
    end

    if agent_pvp.has_handle_match then return end
    agent_pvp.has_handle_match = true

    skynet.call(pvpserver, "lua", "match", agent, mode, account_info)
end

-- 停止自动匹配
agent_pvp.handle_pvp_stop_match_msg = function(cmd, content, len)
    room_started_ = false
    if agent_pvp.has_handle_match == true then
        agent_pvp.has_handle_match = false
    else
        logger.tracef("not match, can't stop %d", account_info.uin)
        return send_error_code(cmd, ERRORCODE.NOT_MATCH_STATE)
    end

    local decode = proto.parse_proto_req(cmd, content, len)
    local type = decode.Type
    local mode = decode.Mode
    skynet.call(pvpserver, "lua", "stop_match", type, agent, mode)
    return send_response(cmd, {})
end

-- 开战
agent_pvp.handle_start_fight_msg = function(cmdid, content, len)
    local decode = proto.parse_proto_req(MESSAGE.CS_CMD_PVP_START, content, len)
    local type = decode.Type
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    if agent_pvp.roomid == roomId then
        room_started_ = true
        return skynet_room_call(agent_pvp.roomid, "start_fight", type, msgNo)
    end
end

-- 转发战斗消息给房间
agent_pvp.handle_fight_msg = function(cmdid, content, len)
    local decode = proto.parse_proto_req(MESSAGE.CS_CMD_PVP_FIGHT, content, len)
    local src = decode.Src
    local dest = decode.Dest
    local num = decode.Num
    local ts = decode.Ts
    local formationId = decode.FormationId
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    if agent_pvp.roomid == roomId then
        return skynet_room_call(agent_pvp.roomid, "fight", src, dest, num, ts, formationId, msgNo)
    end
end

-- 升级建筑消息处理
agent_pvp.handle_upgrade_msg = function(cmdid, content, len)
    local decode = proto.parse_proto_req(cmdid, content, len)
    local buildid = decode.BuildId
    local ts = decode.Ts
    local level = decode.Level
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    if agent_pvp.roomid == roomId then
        return skynet_room_call(agent_pvp.roomid, "upgrade", buildid, ts, level, msgNo)
    end
end

-- 地图信息转发
agent_pvp.handle_map_msg = function(cmdid, content, len)
    local decode = proto.parse_proto_req(cmdid, content, len)
    local ts = decode.Ts
    local data = decode.Data
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo

    if agent_pvp.roomid == roomId then
        return skynet_room_call(agent_pvp.roomid, "map", ts, data, msgNo)
    end
end

-- 结束战斗
agent_pvp.handle_pvp_end_msg = function(cmdid, content, len)
    local decode = proto.parse_proto_req(cmdid, content, len)
    local win = decode.Win
    local ts = decode.Ts
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    if agent_pvp.roomid == nil then
        return send_error_code(cmdid, ERRORCODE.PVP_ROOM_NOT_EXIST)
    else
        if agent_pvp.roomid ~= roomId then return end
        pcall(skynet_room_call, agent_pvp.roomid, "end_fight", win, ts, msgNo)
    end
end

agent_pvp.handle_pvp_ping_msg = function(cmdid, content, len)
    local decode = proto.parse_proto_req(cmdid, content, len)
    local ts = decode.Ts
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    if agent_pvp.roomid == roomId then
        send_response(cmdid, {Ts = skynet.time()})
        return skynet_room_call(agent_pvp.roomid, "ping", ts, msgNo)
    end
end

agent_pvp.handle_pvp_leave_msg = function(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    local leaveMode = decode.Mode
    if agent_pvp.roomid ~= nil then
        if agent_pvp.roomid ~= roomId then return end
        pcall(skynet_room_call, agent_pvp.roomid, "leave", msgNo, leaveMode)
    end
end

agent_pvp.handle_pvp_useitem_msg = function(cmd, content, len)
    if agent_pvp.room_flag == COMMON.room_flag.NOPROP then
        return send_error_code(cmd, ERRORCODE.PVP_NOPROP_ROOM)
    end

    local decode = proto.parse_proto_req(cmd, content, len)
    local data = {}
    data.itemId = decode.ItemId
    data.x = decode.X and decode.X or nil
    data.y = decode.Y and decode.Y or nil
    data.buildid = decode.BuildId and decode.BuildId or nil
    data.ts = decode.Ts
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    if agent_pvp.roomid == roomId then
        --欢乐道具场的物品是临时生成的， 不能消耗玩家自身物品
        data.uin = account_info.uin
        if agent_pvp.room_flag == COMMON.room_flag.HAPPY then
            skynet_room_call(agent_pvp.roomid, "use_item", data, msgNo)
        else
            --使用道具 不涉及逻辑-这里还需要根据返回结果判断是否应该转发
            local ret = player.use_item(COMMON.change_src.pvp,
                data.itemId, 1, account_info)
            if ret then
                skynet_room_call(agent_pvp.roomid, "use_item", data, msgNo)
            end
        end
    end
end

agent_pvp.handle_pvp_ranklist_msg = function(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local groupid = decode.GroundID
    local top_count = decode.TopNSize
    local data = skynet.call(rank_service_v2, "lua", 
        "get_group_rank_top", groupid, top_count)

    local lists = {}
    for i, v in pairs(data) do
        local item = {}
        item.Order = i
        item.Id = v.uin
        item.Name = v.name
        item.Point = v.point
        item.WinRate = v.win_rate
        item.Group = groupid
        item.StreakWin = v.streak_win_count

        local vip_data = agent_vip.get_vip_data_by_uin(v.uin)
        if vip_data then
            item.VipLevel = vip_data.vip_level
        end
        table.insert(lists, item)
    end
    return send_response(cmd, { GroundID = groupid, PlayerList = lists })
end

agent_pvp.handle_pvp_rank_info_msg = function(cmd, content, len)
    local uin = proto.parse_proto_req(cmd, content, len).Uin
    local rank, group = 0, 0
    if account_info.pvp_point > 0 then
        rank, group = cluster.call("ranknode1", ".pvp_rank_service",
                            "search_player_rank", uin)
    end
    
    local send_data = {
        RankInfo = {
            Order   = rank,
            Id      = uin,
            Name    = account_info.name,
            Point   = account_info.pvp_point,
            WinRate = utils.getPvpWinRate(account_info),
            Group   = sharedata_utils.get_group_by_point(account_info.pvp_point),
            Score   = account_info.pvp_score,
            Area    = account_info.area,
        },
        TotalRound  = utils.getPvpTotalCount(account_info),
    }
    return send_response(cmd, send_data)
end

agent_pvp.handle_pvp_chat_msg = function(cmd, content, len)
    if account_info.can_speak_time > os.time() then
        return send_error_code(cmd, ERRORCODE.FORBID_SPEAK)
    end

    local data = proto.parse_proto_req(cmd, content, len)
    local channel = data.Channel

    --弹幕
    if data.Id == 4 then
        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_chat_barrage, account_info.uin)
    end

    if channel == COMMON.channel.ALL_PLAYERS then -- 全服聊天
        local errId
        if data.isBuy ~= nil and data.isBuy ~= 0 then
            --钻石消耗
            if account_info.diamond < setting.constant.player_chat_price then
                errId = COMMON.ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR
            else
                player.change_diamond(account_info, COMMON.change_src.pvp_chat, -setting.constant.player_chat_price)
            end
        else
            if account_info.free_chat_count < 1 then
                errId = COMMON.ERRORCODE.FREE_CHAT_NOT_ENOUGH_ERROR
            else
                if  account_info.free_chat_count == setting.game_setting.max_chat_count then
                    account_info.last_chat_time = math.floor(skynet.time())
                end
                account_info.free_chat_count = account_info.free_chat_count - 1
                player.change_chat_data(account_info)
             --   account_info.last_chat_time = math.floor(os.time())
            end
        end

        if errId then
            return send_error_code(cmd, errId)
        end

        local vip_data = agent_vip.get_vip_data()
        local vip_level = 0
        if vip_data ~= nil then
            vip_level = vip_data.vip_level
        end
        skynet.call(chat_service, "lua", "chat2all", data, account_info, vip_level)
    elseif channel == COMMON.channel.PVP_ROOM then -- pvp房间内聊天
        data.Name = account_info.name
        if agent_pvp.roomid then
            return skynet_room_call(agent_pvp.roomid, "pvpchat", data)
        else
            logger.tracef("room is not exist uin : %d", account_info.uin)
        end
    else
        logger.tracef("chat error channel %d", channel)
    end
end

agent_pvp.handle_pvp_appear_creep = function(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local creepId = decode.CreepId
    local ts = decode.Ts
    local dataId = decode.DataId
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    if agent_pvp.roomid == roomId then
        skynet_room_call(agent_pvp.roomid, "appear_creep", creepId, ts, dataId, msgNo)
    end
end

agent_pvp.handle_pvp_observer = function(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local roomId = decode.RoomId
    local msgNo = decode.MsgNo
    local ob_uin = decode.Uin
    if agent_pvp.roomid == roomId then
        skynet_room_call(agent_pvp.roomid, "observer", msgNo, ob_uin)
    end
end

agent_pvp.handle_pvp_self_ranklist = function(cmd, content, len)
    local ranklist, groupid
    if not self_ranklist_cache.ranklist 
        or (os.time() - self_ranklist_cache.cache_time) > 10 then
        self_ranklist_cache.cache_time = os.time()
        ranklist, groupid = cluster.call("ranknode1", ".pvp_rank_service",
                                                "get_player_self_ranklist", uin)
        self_ranklist_cache.ranklist = ranklist
        self_ranklist_cache.groupid = groupid
    else
        ranklist = self_ranklist_cache.ranklist
        groupid = self_ranklist_cache.groupid
    end
    
    local lists = {}
    for i, v in pairs(ranklist) do
        local item = {}
        item.Order = i
        item.Id = v.uin
        item.Name = v.name
        item.Point = v.point
        item.WinRate = v.win_rate
        item.Group = groupid
        item.StreakWin = v.streak_win_count
        local vip_data = agent_vip.get_vip_data_by_uin(v.uin)
        if vip_data then
            item.VipLevel = vip_data.vip_level
        end
        table.insert(lists, item)
    end

    local data = { PlayerList = lists }
    return send_response(cmd, data)
end

local function match_ret(room, type, player_infos, flag, opener, _battleId, _mapId)
    agent_pvp.roomid = room
    agent_pvp.room_type = type
    agent_pvp.room_flag = flag
    battleId = _battleId
    mapId = _mapId

    local data = {
        Type = type,
        Flag = flag,
        RoomId = room,
        Opener = opener,
        MapId = mapId,
        PlayInfo = player_infos,
    }
    agent_pvp.has_handle_match = false
    send_response(MESSAGE.CS_CMD_PVP_MATCH, data)

    if flag == COMMON.room_flag.NOPROP then
        player.change_score(account_info, -noprop_need_score, true, COMMON.change_src.pvp_noprop)
        account_info.noprop_count = account_info.noprop_count - 1
        can_send_pvp_award = true
    end

    skynet.send(busilog_service, "lua", "log_battle", account_info.uin, "pvp", "begin", flag, type, opener, 0, battleId, mapId)
end

--房间统一回包接口
local function forward(cmdid, data, room)
    -- 避免前面一个房间还没销毁 后面又有一个房间 然后同时接收到2个房间的消息
    if agent_pvp.roomid and agent_pvp.roomid ~= room then return end

    if cmdid == MESSAGE.CS_CMD_PVP_LEAVE then
        if data.Uin == account_info.uin then
            local isProtected = agent_pay_guide.isShieldProtected()
            if not isProtected then
                logger.trace("forward win_count be zero。。", account_info.uin)
                account_info.streak_win_count = 0
                account_info.continue_win_count = 0
            end
            process_leave_pvp_room()
            player.change_flag(account_info)
        end
    end

    -- 如果不在房间 不发送房间消息 (这个判断一定要在leave的下面)
    if not agent_pvp.roomid then return end

    if cmdid == MESSAGE.CS_CMD_PVP_END then
        local update_data = {}
        for k,v in pairs(notice_data) do
            local ptype, value, itemid = v[1],v[2],v[3]
            local data = {
                UpType = ptype,
                Value = value,
                Value2 = itemid
            }
            table.insert(update_data, data)
        end
        data.UpdateData = update_data
        notice_data = {}

        if notice_shareId ~= nil then
            data.ShareId = notice_shareId
            notice_shareId = nil
        end

        if notice_pvp_chestId ~= nil then
            data.PvpChestId = notice_pvp_chestId
            notice_pvp_chestId = nil
        end

        if notice_pvp_chestType ~= nil then
            data.PvpChestType = notice_pvp_chestType
            notice_pvp_chestType = nil
        end

        if notice_pay_guide_item and notice_pay_guide_item ~= 0 then
            data.PayItemId = notice_pay_guide_item
            data.PayItemCount = notice_pay_guide_count
            notice_pay_guide_item = nil
            notice_pay_guide_count = nil
        end

        process_leave_pvp_room()
    end
    return send_response(cmdid, data)
end

-- pvp房间结束 清理数据
local kick_room =  function(roomid)
    if (not agent_pvp.roomid) or (agent_pvp.roomid == roomid) then
        process_leave_pvp_room()
    end
end

function agent_pvp.notice_pvp_end(room_flag, room_type, win_flag)
    local update_data = {}
    --胜利
    if win_flag == COMMON.pvp_end_type.WIN then
        --胜利PVP
        table.insert(update_data, {COMMON.mission_type.WIN_PVP, 0, 1})

        if room_flag == COMMON.room_flag.RAND then
            table.insert(update_data, {COMMON.mission_type.STREAK_WIN, 0, 1})  --连胜
        end
    elseif win_flag == COMMON.pvp_end_type.LOSE then
        --失败
        if room_flag == COMMON.room_flag.RAND then
            table.insert(update_data, {COMMON.mission_type.STREAK_WIN, 0, -1})  --连胜
        end
    elseif win_flag == COMMON.pvp_end_type.DRAW then
        --平局
        if room_flag == COMMON.room_flag.RAND then
            table.insert(update_data, {COMMON.mission_type.STREAK_WIN, 0, -1})  --连胜
        end
    end

    if room_flag == COMMON.room_flag.FIXED then
        --邀请 无论胜负
        table.insert(update_data, {COMMON.mission_type.MULTI_PVP, room_type, 1})
    elseif room_flag == COMMON.room_flag.RAND then
        --随机无论胜负
        table.insert(update_data, {COMMON.mission_type.RAND_PVP, 0, 1})
    end

    agent_achievement.update_achievement()
end

local function process_pvp_lose_notice(tmp_win_count, win_name)
    local notice_info = sharedata_utils.get_stat(COMMON.plan.Notice, COMMON.notice.STOP_STREAK)
    local killwin_score = agent_pvpinfo.
        rand_streak_killwin_score_reward(tmp_win_count)
    if notice_info[1].value <= tmp_win_count then --失败者的连胜达到
        local data = {
            NoticeType = notice_info[1].type,
            Serious = notice_info[1].urgent_flag,
            Content = string.format(notice_info[1].desc, win_name,
                account_info.name, tmp_win_count, killwin_score),
            Title = notice_info[1].title,
            Count = notice_info[1].times,
            TimeGap = notice_info[1].time_gap,
        }
        pcall(skynet_room_call, agent, "publish_announce", MESSAGE.CS_CMD_NOTICE, data)
    end
end

--连胜走马灯通知
local function streak_win_notice(value)
    if value >= 3 then --目前只处理连胜3场以上的
        local notice_info = sharedata_utils.get_stat(COMMON.plan.Notice, COMMON.notice.STREAK_WIN)
        if notice_info then
            --超过了最大值直接处理
            local winscore = agent_pvpinfo.rand_streak_win_score_reward(value)
            if value >= notice_info.max_win.value then
                local notice_config = notice_info.max_win
                local data = {
                    NoticeType = 1,
                    Serious = notice_config.urgent_flag,
                    Content = string.format(notice_config.desc,
                    account_info.name, value, winscore),
                    Title = notice_config.title,
                    Count = notice_config.times,
                    TimeGap = notice_config.time_gap,
                }
                pcall(skynet.call, agent, "lua", "publish_announce",
                    MESSAGE.CS_CMD_NOTICE, data)
                return
            end
            for k, v in pairs(notice_info) do
                if v.value == value then
                    local data = {
                        NoticeType = 1,
                        Serious = v.urgent_flag,
                        Content = string.format(v.desc, account_info.name, winscore),
                        Title = v.title,
                        Count = v.times,
                        TimeGap = v.time_gap,
                    }
                    pcall(skynet.call, agent, "lua", "publish_announce", MESSAGE.CS_CMD_NOTICE, data)
                    break
                end
            end
        end
    end
end

--离开模式
local LEAVE_MODE = {
    ADVANCE = 1, -- 提前离开
    NORMAL  = 2, -- 正常离开
}

local function get_christmas_config()
    local activity_config = sharedata_utils.getActivityConfigById(COMMON.activity_id.ACT_CHRISTMAS)
    return activity_config
end

local process_pvp_end = function(room_flag, room_type,
    room_opener, pvp_end_type,
    room_playerinfo, win_uin, leaveMode, battleTime)

    if not room_started_ then
        local dat        = {
            Uin          = account_info.uin,
            Name         = account_info.name,
            PrevScore    = account_info.pvp_score,
            AddScore     = 0,
            WinRate      = utils.getPvpWinRate(account_info),
            Title        = account_info.PlayerTitle,
            WinCount     = account_info.continue_win_count,
            PrevPoint    = account_info.pvp_point,
            AddPoint     = 0,
        }

        if account_info.pvp_point > 0 then
            item.NewRank = skynet.call(rank_service_v2, "lua",
                "get_rank", account_info.uin)
        end
        return dat
    end
    room_started_ = false

    local addscore = 0 -- 读表
    local addpoint = 0 -- 公式
    local tmp_win_count = account_info.continue_win_count

    if pvp_end_type == COMMON.pvp_end_type.WIN then -- 胜利
        account_info.pvp_win_count = account_info.pvp_win_count + 1
    elseif pvp_end_type == COMMON.pvp_end_type.LOSE then -- 失败
        account_info.pvp_lose_count = account_info.pvp_lose_count + 1
    elseif pvp_end_type == COMMON.pvp_end_type.DRAW then -- 平局
        account_info.pvp_draw_count = account_info.pvp_draw_count + 1
    end

    local is_room_opener = (account_info.uin == room_opener) and true or false
    addscore = sharedata_utils.calc_score(pvp_end_type,
        room_flag, account_info.pvp_point, is_room_opener)
    addpoint = sharedata_utils.calc_point(pvp_end_type,
        room_flag, room_playerinfo, account_info, win_uin)

    local addExp = sharedata_utils.get_pvp_exp(account_info.pvp_point, pvp_end_type, room_type)
    -- 提前退出奖励为0 如果以后有其他奖励 则需要在这处理
    if leaveMode and leaveMode == LEAVE_MODE.ADVANCE then
        if account_info.uin ~= room_opener then -- 房主奖励还是要发的
            addscore = 0
        end
        addExp = 0
    else
        local activity_config = player.get_christmas_config()
        local now = os.time()
        if now >= activity_config.begindate 
            and now <= activity_config.dropEndTime then

            local bonusId = sharedata_utils.getBonusByPvpType(pvp_end_type)
            notice_data = player.takeBonusById(account_info,
                bonusId, 1, COMMON.change_src.chris)
        end

        if activity_valentine.check_open() then
            local bonusId = sharedata_utils.getBonusByPvpType(
                pvp_end_type, COMMON.activity_id.ACT_LOVER)

            local tmp_data = player.takeBonusById(
                account_info, bonusId, 1, COMMON.change_src.lover)

            for _,v in pairs(tmp_data or {}) do
                table.insert(notice_data, v)
            end
        end

        local is_exchange_open, config = agent_operate_activity.check_open(
            COMMON.OP_ACT_TYPE.EXCHANGE)
        if is_exchange_open then
            if now >= config.startTime 
                and now <= config.dropData.dropEndTime then
                local count = (pvp_end_type == COMMON.pvp_end_type.WIN)
                    and config.dropData.winDropCount
                    or config.dropData.loseDropCount
                local ret, item, item_cfg = agent_item.add_item(
                    config.dropData.dropItem, count,
                    COMMON.change_src.op_act_exchange, true)
                if ret then
                    table.insert(notice_data, {item_cfg.property_type,
                        count, config.dropData.dropItem})
                end
            end
        end
    end

    local isProtected = agent_pay_guide.isShieldProtected()
    if room_flag == COMMON.room_flag.RAND then
        account_info.rand_room_count = account_info.rand_room_count + 1
        -- 随机匹配才会影响王者之心连胜 其他模式不影响
        if pvp_end_type == COMMON.pvp_end_type.WIN then
            account_info.streak_win_count = account_info.streak_win_count + 1
            agent_operate_activity.check_win_title()
            streak_win_notice(account_info.streak_win_count)
        else
            if not isProtected then
                account_info.streak_win_count = 0
            end
        end
    elseif room_flag == COMMON.room_flag.FIXED then
        account_info.fixed_room_count = account_info.fixed_room_count + 1
    elseif room_flag == COMMON.room_flag.NOPROP then
        account_info.rand_room_count = account_info.rand_room_count + 1
        -- 无道具场次变更
        player.increment_attr(account_info, 'pvp_nopro_count', 1)
        if pvp_end_type == COMMON.pvp_end_type.WIN then
            addscore = addscore + noprop_need_score * room_type
        end
    end

    local streak_win_score = 0
    local streak_killwin_score = 0
    -- 连胜及广播
    local shiledStatus = agent_pay_guide.getShieldStatus()
    if room_flag == COMMON.room_flag.RAND then
        -- 随机匹配才会影响连胜 其他模式不影响
        if pvp_end_type == COMMON.pvp_end_type.WIN then
            account_info.continue_win_count = 
                account_info.continue_win_count + 1
            if account_info.continue_win_count > 999 then -- 连胜最大场次999
                account_info.continue_win_count = 999
            end
            
            streak_win_score = agent_pvpinfo.rand_streak_win_score_reward(
                account_info.continue_win_count)
            for _,v in pairs(room_playerinfo) do
                if v.continue_win_count and v.uin ~= account_info.uin then
                    streak_killwin_score = streak_killwin_score +
                        agent_pvpinfo.rand_streak_killwin_score_reward(
                            v.continue_win_count)
                end
            end
        else
            --设置连胜次数
            local notice_flag = true
            if agent_pay_guide.isShieldProtected() then
                agent_pay_guide.setShiled()
                shiledStatus = 2
                notice_flag = false
            else
                account_info.continue_win_count = 0
            end

            --没保护盾下连胜终结通告
            if notice_flag then
                local win_name = "GOD"
                for k, v in pairs(room_playerinfo) do
                    if v.uin == win_uin then
                        win_name = v.name
                        break
                    end
                end
                process_pvp_lose_notice(tmp_win_count, win_name)
            end
        end
    end

    local addup_extra = 0
    local addup_ratio = agent_player.get_title_property_addup(
        COMMON.property_type.SCORE) or 0
    if addup_ratio ~= 0 then
        addup_extra = math.floor(addscore * addup_ratio / 100)
    end
        
    local extra_score = agent_operate_activity.crazy_stuff(
        addscore, pvp_end_type)

    
    addscore = addscore + extra_score + addup_extra
    addscore = addscore + streak_win_score + streak_killwin_score
    player.change_score(account_info, addscore, true,
        COMMON.change_src.pvp, true)
    player.change_point(account_info, addpoint, true,
        COMMON.change_src.pvp)
    player.change_max_streak(account_info) -- 改变最大连胜记录
    player.add_exp(account_info, addExp, COMMON.change_src.from_pvp)

    EventCenter.dispatchEvent(
        Events.PVP_END,
        room_flag,    -- 房间类型: RAND 随机匹配, FIXED 固定开房, NOPROP 无道具场
        room_type,    -- 房间人数类型类型: 2人房 3人房 4人房
        pvp_end_type, -- pvp战斗结果: 胜利 失败 平局
        addscore,     -- 增加的荣誉值
        addpoint,     -- 增加的积分
        isProtected)  -- 是否有保护盾

    -- 写入redis【战斗场次超过5场才进入排行榜】
    local isInRank = false
    local totalCount = utils.getPvpTotalCount(account_info)
    if totalCount >= 5 then
        if account_info.pvp_point == 0 then
            addpoint = 1800
            player.change_point(account_info, addpoint, true, COMMON.change_src.pvp)
        end

        local data = {
            uin = account_info.uin,
            name = account_info.name,
            win_rate = utils.getPvpWinRate(account_info),
            point = account_info.pvp_point,
            icon = account_info.icon_id,
            streak_win_count = account_info.streak_win_count,
            -- 要显示在界面上的值最好存在redis里面 这样就可以不用查询离线玩家的数据了
            -- 玩家是否在线字段可以通过查看在线玩家表得到结果
        }
        local rank_rate = math.floor(data.win_rate * 10000)
        skynet.call(redis_service, "lua", "hset_obj", "rank", account_info.uin, data)
        skynet.call(rank_service_v2, "lua", "on_player_rank_change", data)
        isInRank = true

        -- 5场以上开启pvp分享,
        if  room_flag == COMMON.room_flag.RAND or room_flag == COMMON.room_flag.NOPROP then
            if pvp_end_type == COMMON.pvp_end_type.WIN then
                account_info.cur_continue_win = account_info.cur_continue_win + 1
                account_info.cur_continue_lost = 0
            else
                account_info.cur_continue_lost = account_info.cur_continue_lost + 1
                account_info.cur_continue_win = 0
            end
        end
    end

    -- 失败付费引导(如果战斗中提前失败已经购买了之前的付费引导不再弹出引导)
    if  (room_flag == COMMON.room_flag.RAND or room_flag == COMMON.room_flag.NOPROP) 
        and pvp_end_type ~= COMMON.pvp_end_type.WIN  and not agent_pay_guide.pvp_buy(battle_fail_itemid) then
         notice_pay_guide_item, notice_pay_guide_count = agent_pay_guide.on_pvp_lost()
    end
    battle_fail_itemid = 0

    --通知PVP结束 胜利
    agent_pvp.notice_pvp_end(room_flag, room_type, pvp_end_type)
    player.change_flag(account_info)

    local item       = {
        Uin          = account_info.uin,
        Name         = account_info.name,
        PrevScore    = account_info.pvp_score - addscore,
        AddScore     = addscore,
        WinRate      = utils.getPvpWinRate(account_info),
        Title        = account_info.PlayerTitle,
        WinCount     = tmp_win_count,
        PrevPoint    = account_info.pvp_point - addpoint,
        AddPoint     = addpoint,
        ExtraScore   = extra_score,
        ShieldStatus = shiledStatus,
        StreakWin    = streak_win_score,
        EndWin       = streak_killwin_score,
        AddExp       = addExp,
    }
    --随机匹配不加荣誉
    if  room_flag == COMMON.room_flag.RAND then
        item.AddScore = 0
    end
    if isInRank then
        item.NewRank = skynet.call(rank_service_v2, "lua",
                            "get_rank", account_info.uin)
    else
        item.NewRank = 0
    end

    local vip_data = agent_vip.get_vip_data_by_uin(account_info.uin)
    if vip_data then
        item.VipLevel = vip_data.vip_level
    end

    local ok,res = sharedata_utils.isNeedShareByPvp(
            account_info.cur_continue_win,account_info.cur_continue_lost)
    if ok then
        if not account_info["share_data"][res] then
            account_info["share_data"][res] = 1
            notice_shareId = res
        end
    end

    if room_flag == COMMON.room_flag.RAND then
        local chest_id, chest_type = agent_pvp_chest.on_rand_end(pvp_end_type, isProtected)
        if chest_id then
            notice_pvp_chestId = chest_id
            notice_pvp_chestType = chest_type
        end
    end

    skynet.send(busilog_service, "lua", "log_battle", account_info.uin,
        "pvp", "end", room_flag, room_type, room_opener, pvp_end_type,
        win_uin, leaveMode, battleTime, addscore, addpoint, extra_score,
        battleId, mapId, account_info.continue_win_count)
    return item
end

agent_pvp.handle_pvp_recover_item = function(cmd, content, len)
    local decode =  proto.parse_proto_req(cmd, content, len)
    local uin = decode.Uin
    local itemId = decode.ItemId
    if agent_pvp.roomid then
        skynet_room_call(agent_pvp.roomid, "recover_item", uin, itemId)
    end
end

local function recover_item_ret(uin, itemId)
    if account_info.uin ~= uin then return end
    local src = COMMON.change_src.pvp_recover
    local ok, item, item_cfg = agent_item.add_item(itemId, 1, src, true)
    local data = {
        Uin     = uin,
        ItemId  = itemId,
        Num     = item.count,
    }
    return send_response(MESSAGE.CS_CMD_PVP_RECOVER_ITEM, data)
end

function agent_pvp.handle_pvp_use_as_skill(cmd, content, len)
    local req = proto.parse_proto_req(cmd, content, len)
    local camp_id, asst_id, skill_json = req.CampID, req.AsstID, req.SkillJson

    local er = {
        Ok            = false,
        CampID        = camp_id,
        AsstID        = asst_id,
        SkillJson     = skill_json,
    }

    if agent_pvp.roomid then
        skynet_room_call(agent_pvp.roomid, 'use_as_skill', camp_id, asst_id, skill_json)
    end
end

local function send_pvp_award(info)
    if can_send_pvp_award then
        can_send_pvp_award = false

        local cmdid = MESSAGE.CS_CMD_PVP_GET_AWARD
        -- AwardType 0:无； 1:正常结束奖励 2:提前结束奖励
        local resp = { AwardType = 0, UpdateData = {} }

        local point = account_info.pvp_point
        local pvp_grade = sharedata_utils.get_group_by_point(point)

        if not utils.elem({1,2,3,4}, pvp_grade) then
            -- pvp没有段位的，直接返回
            return send_response(cmdid, resp)
        end
        local pvp_result -- nil 2分钟内离开，无奖励；true:胜利;false:失败

        if info.eventSource == 'pvp_leave' and info.leaveMode == 2 then
            pvp_result = false -- 两分钟后或被打败后离开
            resp.AwardType = 2
        elseif info.eventSource == 'pvp_end' then
            resp.AwardType = 1
            pvp_result = info.is_win == COMMON.pvp_end_type.WIN
        end

        
        if pvp_result ~= nil then

            local idx = COMMON.plan.PVP_NOPROP_AWARD
            local cfg = sharedata_utils.get_stat(idx, pvp_grade)
            local bid = pvp_result and cfg.win or cfg.lose
            local r = {}
            local src = COMMON.change_src.pvp_noprop
            local isOpen, config = agent_operate_activity.check_openBySubType(COMMON.OP_ACT_TYPE.MULTIPLEREWARD, 4)
            local rate = config and config.rewardRatio or 1
            local curRate = isOpen and tonumber(rate) or 1
            local content = player.takeBonusById(account_info, bid, 1, src, curRate)
            for _, v in pairs(content or {}) do
                table.insert(r, { UpType = v[1], Value = v[2], Value2 = v[3] })
            end
            resp.UpdateData = r
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_pvp_noprop_award, account_info.uin, bid, pvp_grade)
        end

        return send_response(cmdid, resp)
    end
end

function agent_pvp.handle_pvp_trap(cmd, content, len)
    local decode =  proto.parse_proto_req(cmd, content, len)
    local refreshGroupId    = decode.RefreshGroupId
    local groupId           = decode.GroupId
    local trapId            = decode.TrapId
    local state             = decode.State
    if agent_pvp.roomid then
        skynet_room_call(agent_pvp.roomid, "trap", refreshGroupId, groupId, trapId, state)
    end
end

function agent_pvp.handle_pvp_host(cmd, content, len)
    if agent_pvp.roomid then
        local room_host = skynet_room_call(agent_pvp.roomid, "host")
        return send_response(cmd, {Uin = room_host, Ts = skynet.time()})
    end
end

local function handle_pvp_battle_fail(cmd, msg, sz)
    local id, num = agent_pay_guide.on_pvp_lost()
    if id ~= 0 then
        battle_fail_itemid = id
    end
    send_response(cmd, {ItemId = id, Count = num})
end

function agent_pvp.deal_exit()
    if agent_pvp.roomid then
        skynet_room_call(agent_pvp.roomid, "leave")
        process_leave_pvp_room()
    end
end

-- 添加agent回包
local function add_cmd(CMD)
    CMD.match_ret                           = match_ret
    CMD.kick_room                           = kick_room
    CMD.forward                             = forward
    CMD.process_pvp_end                     = process_pvp_end
    CMD.recover_item_ret                    = recover_item_ret
    CMD.send_pvp_award                      = send_pvp_award

    CMD.personal_room_msg_enter             = agent_pvp.personal_room_msg_enter
    CMD.personal_room_msg_leave             = agent_pvp.personal_room_msg_leave
    CMD.personal_room_msg_chat              = agent_pvp.personal_room_msg_chat
    CMD.personal_room_msg_mode              = agent_pvp.personal_room_msg_mode

    CMD.personal_room_msg_begin_fight       = agent_pvp.personal_room_msg_begin_fight
    CMD.handle_personal_room_broadcast      = agent_pvp.handle_personal_room_broadcast
    CMD.handle_accept_personal_room_invite  = agent_pvp.handle_accept_personal_room_invite
    CMD.personal_room_destory               = agent_pvp.personal_room_destory
    CMD.handle_personal_room_count_down     = agent_pvp.handle_personal_room_count_down
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_MATCH]                = agent_pvp.handle_match_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_START]                = agent_pvp.handle_start_fight_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_FIGHT]                = agent_pvp.handle_fight_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_UPGRADE]              = agent_pvp.handle_upgrade_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_MAP]                  = agent_pvp.handle_map_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_PING]                 = agent_pvp.handle_pvp_ping_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_END]                  = agent_pvp.handle_pvp_end_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_LEAVE]                = agent_pvp.handle_pvp_leave_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_USE_ITEM]             = agent_pvp.handle_pvp_useitem_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_RANKING_LIST]         = agent_pvp.handle_pvp_ranklist_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_RANKING_INFO]         = agent_pvp.handle_pvp_rank_info_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_STOP_MATCH]           = agent_pvp.handle_pvp_stop_match_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_CHAT]                 = agent_pvp.handle_pvp_chat_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_APPEAR_CREEP]         = agent_pvp.handle_pvp_appear_creep
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_OBSERVER]             = agent_pvp.handle_pvp_observer
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_SELF_RANKLIST]        = agent_pvp.handle_pvp_self_ranklist
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_RECOVER_ITEM]         = agent_pvp.handle_pvp_recover_item
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_USE_AS_SKILL]         = agent_pvp.handle_pvp_use_as_skill
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_TRAP]                 = agent_pvp.handle_pvp_trap
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_HOST]                 = agent_pvp.handle_pvp_host

    -- 个人房间消息
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_CREATE]     = agent_pvp.handle_create_personal_room
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_ENTER]      = agent_pvp.handle_enter_personal_room
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_LEAVE]      = agent_pvp.handle_leave_personal_room
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_KICK]       = agent_pvp.handle_kick_personal_room
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_INVITE]     = agent_pvp.handle_invite_personal_room
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_BROADCAST]  = agent_pvp.handle_broadcast_personal_room
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_CHAT]       = agent_pvp.handle_chat_personal_room
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_MODE]       = agent_pvp.handle_mode_personal_room
    cmd_handle_mapping[MESSAGE.CS_CMD_PERSONAL_ROOM_BEGIN_FIGHT]= agent_pvp.handle_begin_fight_personal_room

    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_HAPPY_ITEMS]          = agent_pvp.handle_happy_items
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_HAPPY_USE_ITEM]       = agent_pvp.handle_happy_use_item
    cmd_handle_mapping[MESSAGE.CS_CMD_PVP_BATTLE_FAILE]         = handle_pvp_battle_fail
end

function agent_pvp.init(args)
    pvpserver       = skynet.queryservice("pvpserver")
    pvp_room_server = skynet.queryservice("pvp_room_server")
    rank_service_v2 = skynet.queryservice("rank_service_v2")
    redis_service   = skynet.queryservice("redis_service")
    gamed               = skynet.queryservice("gamed")
    chat_service    = skynet.queryservice("chat_service")
    busilog_service = skynet.queryservice("business_log_service")

    local_nodename  = utils.query_nodename(server_config.gamed_conf.servername)
    agent_add_value = utils.get_agent_add_value(local_nodename)    --Todo:这个地方要处理, agent_add_value要移除clustername.lua文件中

    agent        = args.agent
    account_info = args.info
    uin          = args.uin
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code


    -- 模块必须实现的接口
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)

    -- 登录 有房间的要离开房间 清掉房间
    agent_pvpinfo.init(args)
end

function agent_pvp.reinit()
    agent_pvp.has_handle_match = false -- 让玩家重连能匹配
    if agent_pvp.roomid then
        skynet_room_call(agent_pvp.roomid, "leave")
        process_leave_pvp_room()
    end
end

return agent_pvp


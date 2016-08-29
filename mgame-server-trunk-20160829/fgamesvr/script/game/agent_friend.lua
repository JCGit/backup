--
-- User: yuanfengyun
-- Date: 2015/9/28
-- Time: 16:15
-- Desc: agent的friend模块
--

local skynet = require "skynet"

local logger        = require "logger"
local utils         = require "utils"
local proto         = require "proto"
local COMMON        = require "common"
local setting       = require "setting"
local server_config = require "server_config"
local agent_pve     = require "agent_pve"
local player        = require "player"

local local_nodename

local account_info, redis_service, rank_service_v2, 
        send_package, send_response, send_error_code

--[[
    friend_info
    {
        friends = {[uin]={},[uin]={}}
        requests = {[uin]={uin,name,time},[uin]={uin,name,time}}
        black_list ={ { } }
    }
]]

local agent_friend = {}

function agent_friend.get_friend_info(account_info)
    local win_rate = utils.getPvpWinRate(account_info)
    local rank, group = skynet.call(rank_service_v2, "lua", 
        "get_rank", account_info.uin)
    return {
        Uin             = account_info.uin,
        NickName        = account_info.name or "",
        Icon            = account_info.icon_id or 0,
        Group           = group or 0,
        Rank            = rank or 0,
        WinRate         = win_rate,
        LastOnlineTime  = account_info.login_time or 0,
        LastOfflineTime = account_info.login_out_time,
        Score           = account_info.pvp_point,
        Title           = account_info.PlayerTitle,
        Level           = account_info.achievement_level,
        ExpLevel        = account_info.level,
        Exp             = account_info.exp,
        OpenId          = account_info.openid,
        VipLevel        = account_info.vip_level,
    }
end

-- 查找
function agent_friend.query(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.Uin == nil then
        return
    end

    local uin = tonumber(po.Uin)
    local resp = {Players={}}
    local account = nil
    if uin ~= 0 then
       account = skynet.call(redis_service,"lua","hget_obj","account", uin)
       if type(account)=="table" then
          resp.Players = {agent_friend.get_friend_info(account)}
       end
    end
    if not account then
       local name_data = skynet.call(redis_service, "lua", "hget_obj", "player_name", po.NickName)
       if name_data then
          account = skynet.call(redis_service,"lua","hget_obj","account", name_data.value)
          if account then
             resp.Players = {agent_friend.get_friend_info(account)}
          end
       end
    end

    send_response(cmd, resp)
end

-- 用户申请加好友
function agent_friend.add_friend_request(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.Uin == nil then
        return
    end
    local other_uin = tonumber(po.Uin)
    local other_account = skynet.call(redis_service,"lua","hget_obj","account", other_uin)
    -- 不存在该用户
    if type(other_account) ~= "table" then
        return
    end

    local data  = {
        Info = agent_friend.get_friend_info(account_info),
        Time = math.floor(skynet.time())
    }
    local isonline = skynet.call(".online_mgr","lua","handle_add_friend_request",other_uin, data)

    --如果不在线则 替他处理
    if not isonline then
       local my_uin = account_info.uin
       local friend_info = skynet.call(redis_service,"lua","hget_obj","friend_info", other_uin)
       if type(friend_info) ~= "table" then
           friend_info = {friends={},requests={},black_list={},pk={}}
       end

       friend_info.requests[my_uin] = {uin = my_uin, name = account_info.name, time = math.floor(skynet.time())}
       skynet.call(redis_service,"lua","hset_obj","friend_info", other_uin, friend_info)
    end

    local resp = { Uin = other_uin }
    send_response(cmd, resp)
end

function agent_friend.add_friend_refuse(Uins)
    local my_uin = account_info.uin
    local my_friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", my_uin)

    if type(my_friend_info) ~= "table" then
        return
    end

    for i,v in ipairs(Uins) do
        local other_uin = tonumber(v)
        my_friend_info.requests[other_uin] = nil
    end
    skynet.call(redis_service,"lua", "hset_obj", "friend_info", my_uin, my_friend_info)
end

-- 同意或拒绝加好友
function agent_friend.add_friend_confirm(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or type(po.Uins) ~= "table" then
        return
    end

    if not po.Agree then
        agent_friend.add_friend_refuse(po.Uins)
        return
    end

    local my_uin = account_info.uin
    local my_friend_info = skynet.call(redis_service,"lua","hget_obj","friend_info", my_uin)
    if type(my_friend_info) ~= "table" then
        my_friend_info = {friends={},requests={},black_list={},pk={}}
    end

    local resp = {Agree=po.Agree, NewFriend={}}
    local my_uin = account_info.uin
    for i,v in ipairs(po.Uins) do
        local other_uin = tonumber(v)
        repeat
            if my_friend_info.requests[other_uin] == nil then
                break
            end
            local isonline = skynet.call(".online_mgr","lua","handle_add_friend_confirm",other_uin,agent_friend.get_friend_info(account_info))
            if not isonline then
                local other_friend_info = skynet.call(redis_service,"lua","hget_obj","friend_info", other_uin)
                if type(other_friend_info) ~= "table" then
                    other_friend_info = {friends={}, requests={}, black_list={}, pk={}}
                end
                if not other_friend_info.friends[my_uin] then
                    other_friend_info.friends[my_uin] = {}
                    skynet.call(redis_service,"lua","hset_obj","friend_info", other_uin, other_friend_info)
                end
            end
            -- 互加好友
            my_friend_info.friends[other_uin] = {}
            my_friend_info.requests[other_uin]=nil
            local other_account_info = skynet.call(redis_service,"lua","hget_obj","account", other_uin)
            table.insert(resp.NewFriend, agent_friend.get_friend_info(other_account_info))
        until true
    end
    skynet.call(redis_service, "lua", "hset_obj", "friend_info", my_uin, my_friend_info)

    send_response(cmd, resp)
end

-- 获取我的好友列表
function agent_friend.get_my_friend_list(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.Uin == nil then
        return
    end

    local my_uin = account_info.uin
    local friend_info_ret = {Friends = {}, Requests = {}, BlackList={}, Pk={}}
    repeat
        local friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", my_uin)
    if type(friend_info) ~= "table" then
            break
        end

    if po.Friends then
            for uin, info in pairs(friend_info.friends) do
            local account_info = skynet.call(redis_service,"lua","hget_obj","account", uin)
                table.insert(friend_info_ret.Friends, agent_friend.get_friend_info(account_info))
            end
    end

        if po.Requests then
            for uin, info in pairs(friend_info.requests) do
            local account_info = skynet.call(redis_service,"lua","hget_obj","account",uin)
                table.insert(friend_info_ret.Requests,{Info = agent_friend.get_friend_info(account_info),Time=info.time,})
            end
    end

    if po.BlackList then
        for uin, info in pairs(friend_info.requests) do
                local account_info = skynet.call(redis_service,"lua","hget_obj","account",uin)
                table.insert(friend_info_ret.BlackList,{Info = agent_friend.get_friend_info(account_info)})
            end
        end

        if po.Pk then
            friend_info_ret.Pk = friend_info.pk
            for k,v in pairs(friend_info_ret.Pk) do
               --这里不可以将friend_info重新存入redis
              if v.gamenode then
                 v.gamenode = nil
              end
            end
        end
    until true

    send_response(cmd, friend_info_ret)
end

-- 删除好友
function agent_friend.remove_friend(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)

    if type(po) ~= "table" or type(po.Uin) ~= "number" then
        return
    end

    local my_uin = account_info.uin
    local other_uin = po.Uin
    local friend_info = skynet.call(redis_service,"lua","hget_obj","friend_info",my_uin)
    if type(friend_info) ~= "table" then
        return
    end
    if friend_info.friends[other_uin] == nil then
        return
    end
    friend_info.friends[other_uin] = nil
    skynet.call(redis_service,"lua","hset_obj","friend_info",my_uin,friend_info)

    local my_account = skynet.call(redis_service,"lua","hget_obj","account",my_uin)
    if type(my_account) ~= "table" then
        return
    end

    local isonline = skynet.call(".online_mgr","lua","handle_remove_friend",other_uin,my_uin,my_account.name or "")
    if not isonline then
        local other_friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", other_uin)
        if type(other_friend_info) == "table" then
            other_friend_info.friends[my_uin] = nil
            skynet.call(redis_service, "lua", "hset_obj", "friend_info", other_uin, other_friend_info)
        end
    end

    local other_account = skynet.call(redis_service, "lua", "hget_obj", "account", other_uin)
    if type(other_account) ~= "table" then
        return
    end

    send_response(cmd, { Uin = other_uin, NickName = other_account.name })
end

-- 加入黑名单
function agent_friend.add_black_list(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.Uin == nil then
        return
    end
    local other_uin = po.Uin
    local my_uin = account_info.uin
    repeat
        local friend_info = skynet.call(redis_service,"lua","hget_obj","friend_info",my_uin)
        if type(friend_info) ~= "table" then
            break
        end
        -- 当前好友不能加黑名单
        if friend_info.friends[other_uin] then
            break
        end
        friend_info.black_list[other_uin]={}
        skynet.call(redis_service, "hset_obj", "friend_info", my_uin,friend_info)
    until true
    send_response(cmd, {Uin = other_uin})
end

-- 去除黑名单
function agent_friend.remove_black_list(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.Uin == nil then
        return
    end
    local other_uin = po.Uin
    local my_uin = account_info.uin
    repeat
        local friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", my_uin)
        if type(friend_info) ~= "table" then
            break
        end
        -- 当前好友不能加黑名单
        if friend_info.friends[other_uin] then
            break
        end

        if friend_info.black_list[other_uin] == nil then
            break
        end
        friend_info.black_list[other_uin] = nil
        skynet.call(redis_service, "lua", "hset_obj", "friend_info", my_uin,friend_info)
    until true
    local other_account = skynet.call(redis_service, "lua", "hget_obj", "account", other_uin)
    if type(other_account) ~= "table" then
        return
    end
    send_response(cmd, {Uin = other_uin, NickName = other_account.name})
end

--接受或拒绝好友房间邀请
function agent_friend.apply_or_refuse_personal_room_invite(cmd, content, len)
    local po = proto.parse_proto_req(cmd, content, len)
    if type(po) ~= "table" or po.RoomID == nil then
        return
    end
    local is_apply = po.IsApply
    local room_id = po.RoomID
    local uin = po.Uin

    -- 删除好友约战信息
    local my_friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", account_info.uin)
    if not my_friend_info then
        return
    end

    local pk_flag = false
    local room_server_node
    for i,v in ipairs(my_friend_info.pk) do
        if v.RoomID == room_id and v.Info.Uin == uin then
            pk_flag = true
            if v.gamenode ~= local_nodename then
               room_server_node = v.gamenode
            end
            table.remove(my_friend_info.pk,i)
            break
        end
    end
    if not pk_flag then
       logger.tracef("has not pk info", uin)
       return
    end

    skynet.call(redis_service, "lua", "hset_obj", "friend_info", account_info.uin,my_friend_info)

    send_response(cmd, { IsApply = is_apply, RoomID = room_id })

    -- 请求加入房间
    if is_apply then
        skynet.call(skynet.self(),"lua","handle_accept_personal_room_invite",room_id,uin,room_server_node)
    else
        skynet.call(skynet.self(),"lua","handle_refuse_personal_room_invite",room_id,uin,room_server_node)
    end
end

-- 收到加好友请求
function agent_friend.handle_add_friend_request(other_info)
    local friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", account_info.uin)
    if type(friend_info) ~= "table" then
        friend_info = {friends={},requests={},black_list={},pk={}}
    end

    friend_info.requests[other_info.Info.Uin] = {uin = other_info.Info.Uin, name = other_info.Info.NickName, time = math.floor(skynet.time())}
    skynet.call(redis_service,"lua","hset_obj","friend_info", account_info.uin, friend_info)

    local data = {
        Uin=other_info.Info.Uin,
        OtherUin=other_info.Info.Uin,
        Info=other_info,
    }
    send_response(COMMON.MESSAGE.CS_CMD_FRIEND_REQUEST_ADD, data)
end

-- 收到同意加好友消息
function agent_friend.handle_add_friend_confirm(other_info)
    local my_uin = account_info.uin
    local friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", my_uin)
    if type(friend_info) ~= "table" then
        friend_info = {friends={}, requests={}, black_list={}, pk={}}
    end
    if not friend_info.friends[other_info.Uin] then
       friend_info.friends[other_info.Uin] = {}
       skynet.call(redis_service, "lua", "hset_obj", "friend_info", my_uin, friend_info)
    end

    send_response(COMMON.MESSAGE.CS_CMD_FRIEND_CONFIRM_ADD, {Agree = true, NewFriend = {other_info}})
end

function agent_friend.handle_remove_friend(uin,name)
    local friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", account_info.uin)
    if type(friend_info) == "table" then
        if friend_info.friends[uin] then
            friend_info.friends[uin] = nil
            skynet.call(redis_service, "lua", "hset_obj", "friend_info", account_info.uin, friend_info)
        end
    end

    send_response(COMMON.MESSAGE.CS_CMD_FRIEND_REQUEST_REMOVE, {Uin = uin, NickName = name})
end

function agent_friend.handle_invite_friend_personal_room(msg)
--    if agent_pve.get_max_checkpoint_id() <= 1120 then
--       return
--    end
    local friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", account_info.uin)
    for i,v in ipairs(friend_info.pk) do
        if v.Info.Uin == msg.Uin then
            table.remove(friend_info.pk, i)
            break
        end
    end
    table.insert(friend_info.pk, msg.Info)
    skynet.call(redis_service, "lua", "hset_obj", "friend_info", account_info.uin, friend_info)
    msg.Info.gamenode = nil
    send_response(COMMON.MESSAGE.CS_CMD_PERSONAL_ROOM_INVITE, msg)
end

--相互添加对方为自己好友
function agent_friend.mutual_add_friend(my_uin, other_uin)
    local my_friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", my_uin)
    if type(my_friend_info) ~= "table" then
        my_friend_info = {friends={}, requests={}, black_list={}, pk={}}
    end
    if not my_friend_info.friends[other_uin] then
        my_friend_info.friends[other_uin] = {}
        skynet.call(redis_service, "lua", "hset_obj", "friend_info", my_uin, my_friend_info)
    end

    local other_friend_info = skynet.call(redis_service, "lua", "hget_obj", "friend_info", other_uin)
    if type(other_friend_info) ~= "table" then
        other_friend_info = {friends={}, requests={}, black_list={}, pk={}}
    end
    if not other_friend_info.friends[my_uin] then
        other_friend_info.friends[my_uin] = {}
        skynet.call(redis_service, "lua", "hset_obj", "friend_info", other_uin, other_friend_info)
    end
end

function agent_friend.add_cmd(CMD)
    CMD.handle_add_friend_request          = agent_friend.handle_add_friend_request
    CMD.handle_add_friend_confirm          = agent_friend.handle_add_friend_confirm
    CMD.handle_invite_friend_personal_room = agent_friend.handle_invite_friend_personal_room
    CMD.handle_remove_friend               = agent_friend.handle_remove_friend
end

function agent_friend.syc_list(cmd, content, len)
    local data_resp = proto.parse_proto_req(cmd, content, len)
    if type(data_resp) ~= "table" or data_resp.OpenId == nil then
        return
    end

    local openids = data_resp.OpenId
    local resp = {}
    resp.Friends = {}
    local platform = COMMON.platform[account_info.platform]
    for k, v in pairs(openids) do
        local open_id = string.format("%s_%s", platform, v)
        logger.tracef("syc friend openid : %s", open_id)
        local other_uin = skynet.call(redis_service, "lua", "hget_obj", "uin_mapping", open_id)
        if other_uin ~= nil then
            local my_uin = account_info.uin
            agent_friend.mutual_add_friend(my_uin, other_uin)
            local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", other_uin)
            if account_info ~= nil then
                table.insert(resp.Friends, agent_friend.get_friend_info(account_info))
            end
        end
    end
    send_response(cmd, resp)
end

-- 添加agent消息处理函数
function agent_friend.add_cmd_handle_mapping(cmd_handle_mapping)
    local M = COMMON.MESSAGE
    cmd_handle_mapping[M.CS_CMD_FRIEND_QUERY]                         = agent_friend.query
    cmd_handle_mapping[M.CS_CMD_FRIEND_REQUEST_ADD]                   = agent_friend.add_friend_request
    cmd_handle_mapping[M.CS_CMD_FRIEND_CONFIRM_ADD]                   = agent_friend.add_friend_confirm
    cmd_handle_mapping[M.CS_CMD_FRIEND_GET_INFO]                      = agent_friend.get_my_friend_list
    cmd_handle_mapping[M.CS_CMD_FRIEND_REQUEST_REMOVE]                = agent_friend.remove_friend
    cmd_handle_mapping[M.CS_CMD_FRIEND_ADD_BLACK_LIST]                = agent_friend.add_black_list
    cmd_handle_mapping[M.CS_CMD_FRIEND_REMOVE_BLACK_LIST]             = agent_friend.remove_black_list
    cmd_handle_mapping[M.CS_CMD_PERSONAL_ROOM_APPLY_OR_REFUSE_INVITE] = agent_friend.apply_or_refuse_personal_room_invite
    cmd_handle_mapping[M.CS_CMD_FRIEND_SYC_LIST]                      = agent_friend.syc_list
end

function agent_friend.init(args)
    rank_service_v2 = skynet.queryservice("rank_service_v2")
    redis_service = args.redis
    account_info = args.info
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code

    -- 模块必须实现的接口
    agent_friend.add_cmd(args.cmd)
    agent_friend.add_cmd_handle_mapping(args.cmd_map)

    local_nodename = utils.query_nodename(server_config.gamed_conf.servername)
end

return agent_friend

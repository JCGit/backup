-- 本服在线管理

local skynet = require "skynet"
local logger = require "logger"

local server_config = require "server_config"
local cluster       = require "cluster"
local utils         = require "utils"

local local_gamenode = utils.query_nodename(server_config.gamed_conf.servername)
local on_line_agent = {}
local redis_service

local CMD = {}

-- 我请求加别人为好友
function CMD.handle_add_friend_request(other_uin, my_info)
    local agent = on_line_agent[other_uin]
    if not agent then
        local node_info = skynet.call(redis_service, "lua", "hget_obj", "player_node", other_uin)
        if node_info == nil or not node_info.isonline then
            return false
        end
        local gamenode = utils.query_nodename(node_info.servername)
        if gamenode == nil or gamenode == local_gamenode then
           return false
        end
        cluster.call(gamenode, ".cluster_agent_server", "agent_forward_by_uin", other_uin, "handle_add_friend_request", my_info)
        return true
    end
    skynet.call(agent,"lua","handle_add_friend_request",my_info)
    return true
end


-- 我同意了别人的好友请求
function CMD.handle_add_friend_confirm(other_uin, my_info)
    local agent = on_line_agent[other_uin]

    if not agent then
        local node_info = skynet.call(redis_service, "lua", "hget_obj", "player_node", other_uin)
        if node_info == nil or not node_info.isonline then
            return false
        end
        local gamenode = utils.query_nodename(node_info.servername)
        if gamenode == nil or gamenode == local_gamenode then
            return false
        end
        cluster.call(gamenode, ".cluster_agent_server", "agent_forward_by_uin", other_uin, "handle_add_friend_confirm", my_info)
        return true
    end

    skynet.call(agent,"lua","handle_add_friend_confirm",my_info)
    return true
end

function CMD.handle_remove_friend(other_uin, my_uin, my_name)
    local agent = on_line_agent[other_uin]
    if not agent then
        local node_info = skynet.call(redis_service, "lua", "hget_obj", "player_node", other_uin)
        if node_info == nil or not node_info.isonline then
            return false
        end
        local gamenode = utils.query_nodename(node_info.servername)
        if gamenode == nil or gamenode == local_gamenode then
            return false
        end
        cluster.call(gamenode, ".cluster_agent_server", "agent_forward_by_uin", other_uin, "handle_remove_friend",my_uin,my_name)
        return true
    end
    skynet.call(agent,"lua","handle_remove_friend",my_uin,my_name)
    return true
end

--我邀好友进入个人房间
function CMD.handle_invite_friend_personal_room(other_uin,msg)
    local agent = on_line_agent[other_uin]
    if not agent then
        local node_info = skynet.call(redis_service, "lua", "hget_obj", "player_node", other_uin)
        if node_info == nil or not node_info.isonline then
            return
        end
        local gamenode = utils.query_nodename(node_info.servername)
        if gamenode == nil or gamenode == local_gamenode then
            return
        end
        cluster.call(gamenode, ".cluster_agent_server", "agent_forward_by_uin", other_uin, "handle_invite_friend_personal_room", msg)
        return
    end
    skynet.call(agent,"lua","handle_invite_friend_personal_room",msg)
end

--我发起全服
function CMD.handle_personal_room_broadcast(msg)
    for k,v in pairs(on_line_agent) do
        pcall(skynet.call, v,"lua","handle_personal_room_broadcast",msg)
    end
end

function CMD.online(uin, agent)
    on_line_agent[uin] = agent
end

function CMD.offline(uni, agent)
    on_line_agent[uni] = nil
end

function CMD.get_uin_agent(uin)
   return on_line_agent[uin]
end

local traceback = debug.traceback
local function dispatch(_, _, command, ...)
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
    if ret ~= nil then
        skynet.retpack(ret)
    end
end

skynet.start(function ()
    redis_service = skynet.queryservice("redis_service")
    skynet.dispatch("lua", dispatch)
end)

-- game接口命令文件
-- 这个文件主要是做 gameserver.start  handle的各种消息的匹配处理
local skynet       = require "skynet"
local socketdriver = require "socketdriver"
local cluster      = require "cluster"
local netpack      = require "netpack"

local gate_server = require "gate_server"
local logger      = require "logger"
local proto       = require "proto"

local COMMON      = require "common"
local MESSAGE     = COMMON.MESSAGE
local ERRORCODE   = COMMON.ERRORCODE

local setting     = require "setting"
local db_setting  = require "db_setting"
local utils       = require "utils"

local sharedata_utils = require "sharedata_utils"

local redis_service
local busilog_service
local gameserver = {}
local user_fd_map = {}
local auth_user_list = {}
local fd_agent_map = {}
local fd_user_map = {}
local online_uin = {}
local pending_msg = {}
local online_num = 0 --在线用户数
local alive_agent_num = 0 --存活agent数
local traceback = debug.traceback
local the_gamed
local heart_tick_flag = setting.heart_tick_flag
local self_id = skynet.self()

-- game gate server 维护的fd-key字典
local fd_key_dict = {}

local online_agent_limit = setting.limit.online_agent_limit
local queue_user_limit = setting.limit.queue_user_limit
local hotfix = {}

local uin_trans_map = {}
uin_trans_map["0"] = 5
uin_trans_map["1"] = 4
uin_trans_map["2"] = 6
uin_trans_map["3"] = 7
uin_trans_map["4"] = 1
uin_trans_map["5"] = 0
uin_trans_map["6"] = 2
uin_trans_map["7"] = 3
uin_trans_map["8"] = 9
uin_trans_map["9"] = 8

---------------- 热更新相关 ---------------
hotfix.gameserver_force_exit = false

local lcrypt = require 'lcrypt'
local lcrypt_key = lcrypt.key
local string_pack = string.pack
local string_rep = string.rep

----------------
local function send_package(user_fd, cmdid, content)
    if not user_fd then
        return
    end

    local bfkey = fd_key_dict[user_fd]
    if bfkey then
        local rl = #content
        local m8 = rl % 8
        if m8 ~= 0 then
            content = content .. string_rep(0, 8 - m8)
        end
        local header = string_pack(">I2I2I2", #content + 4, cmdid, rl)
        local key = lcrypt_key('blowfish', 'cbc', bfkey, '12345678')
        socketdriver.send(user_fd, header .. key:encrypt(content))
    else
        local len = #content + 2
        socketdriver.send(user_fd, string_pack(">I2>I2", len, cmdid) .. content)
    end
end

local p1 = proto.pack_proto_resp
local p2 = proto.pack_error_resp
local error_code_tb = { ErrCode = 0 }
local function send_response(fd, cmdid, resp)
    return send_package(fd, cmdid, p1(cmdid, resp))
end
local function send_error_code(fd, cmdid, err)
    error_code_tb.ErrCode = err
    return send_package(fd, cmdid, p2(cmdid, error_code_tb))
end

--向同步服发送玩家上下线数据
local node_name
local host
local function report_online(uin)
    local now = math.floor(skynet.time())
    skynet.call(redis_service, "lua", "hset_obj", "player_node", uin, {isonline = true, servername = node_name, ts= now})
end

local function report_offline(uin)
    local now = math.floor(skynet.time())
    skynet.call(redis_service, "lua", "hset_obj", "player_node", uin, {isonline = false, servername = node_name, ts = now})
end

--向负载均衡服发送心跳
local heartbeat_time = 20 * 100
local function send_heartbeat()
    if heart_tick_flag == 1 then
       xpcall(cluster.call, traceback, "gslbnode", ".gslbserver", "heart", "game", node_name, host)
       xpcall(cluster.call, traceback, "centernode", ".syncserver", "heart", "game", node_name, host)
    end
    skynet.timeout(heartbeat_time, send_heartbeat)
end

function gameserver.notice_newagent()
    alive_agent_num = alive_agent_num + 1
end

function gameserver.forward(fd, agent)
    fd_agent_map[fd] = agent
    gate_server.forward(fd, agent)
end

--skynet的主动断开连接触发
function gameserver.kick(fd, kick_uin)
    if fd then
        --由于客户端有时候不回主动关闭的fin报文，导致不能只在handler.disconnect做连接的处理工作，
        -- 这个主动关闭的接口也要做连接的管理工作
        local agent = fd_agent_map[fd]
        local uin = fd_user_map[fd]
        if agent then
            fd_agent_map[fd] = nil
        end

        if uin == kick_uin then
            fd_user_map[fd] = nil
            if online_uin[uin] then
                online_uin[uin]  = nil
                online_num = online_num - 1
            end
            user_fd_map[uin] = nil
            report_offline(uin)
        end

        logger.trace("fd active disconnect ", fd)
        gate_server.closeclient(fd)
    else
        --特殊临界条件下fd为nil， 当还是要计算统计数据
        if online_uin[kick_uin] then
            online_uin[kick_uin] = nil
            online_num = online_num - 1
        end
        user_fd_map[kick_uin] = nil
        report_offline(kick_uin)        
    end
end

function gameserver.kickoff()
    alive_agent_num = alive_agent_num - 1  --这个计算有问题
end

function gameserver.exit()
    logger.error("Gameserver has setting ban new conn")
    gate_server.ban_newconn()
end

local next_time = 0
local last_time = os.time()
-- 发送在线数据日志
-- 按小时点每过5分钟发送 一次数据
local function send_online_log()
    skynet.timeout(15*100, send_online_log)

    local tnow = os.time()
    local hour = os.date("%H", tnow)
    local minute = os.date("%M", tnow)
    local cha = 5 -  minute % 5     -- 当前时间距离下一个整5分的差

    -- 隔天
    if utils.getSameDayBeginTime(tnow) ~= utils.getSameDayBeginTime(last_time) then
        the_gamed.process_daily_check()
    end
    last_time = tnow
    if  tnow > next_time then
        next_time = utils.get_next_time(cha)
        local serial = hour * 12 + minute / 5
        --统计各平台
        local platform_data = {}
        for k, platform in pairs(online_uin) do
            if platform then
                if platform_data[platform] then
                    platform_data[platform] = platform_data[platform] + 1
                else
                    platform_data[platform] = 1
                end
            end
        end

        for k, v in pairs(platform_data) do
            skynet.send(busilog_service, "lua", "log_online", serial, k, v)
        end
    end
end

--定时器启动这个操作
local user_wait_hash = {}
local user_wait_list = utils.List.new()
local function deal_wait_queue()

    if alive_agent_num > online_agent_limit then
        skynet.timeout(setting.game_setting.QUEUE_REFRESH_TIME, deal_wait_queue)
        return
    end

    if utils.List.getSize(user_wait_list) <= 0 then
        skynet.timeout(setting.game_setting.QUEUE_REFRESH_TIME, deal_wait_queue)
        return
    end

    local num = online_agent_limit - alive_agent_num
    if num < 0 then
        skynet.timeout(setting.game_setting.QUEUE_REFRESH_TIME, deal_wait_queue)
        return
    end

    local count = 0
    while true do
        if count > num then break end
        local active_uin = utils.List.popFront(user_wait_list)
        if active_uin == nil then break end
        local user_fd = user_fd_map[active_uin]
        if user_fd then
            --发送玩家可登进协议
            local data = { Uin = active_uin }
            local content = proto.pack_proto_s2c_req(MESSAGE.CS_CMD_CANENTER, data)
            logger.trace("Send CanEnter  ", user_fd, " : ", active_uin)
            count = count + 1
            send_package(user_fd, MESSAGE.CS_CMD_CANENTER, content)
        end

        if user_wait_hash[active_uin] then
            user_wait_hash[active_uin] = nil
            if user_fd_map[active_uin] then user_fd_map[active_uin] = nil end
            if fd_user_map[user_fd] == active_uin then  fd_user_map[user_fd] = nil end
        end
    end

    --队列中其他玩家，发送当前的等待人数
    local iter = utils.List.listIter(user_wait_list)
    local remain = 1
    while true do
        local alive_uin = iter()
        if alive_uin == nil then break end

        local data = {
            Uin = alive_uin,
            Remain_Num = remain
        }
        local user_fd = user_fd_map[alive_uin]
        if user_fd then
            remain = remain + 1
            local content = proto.pack_proto_s2c_req(MESSAGE.CS_CMD_QUEUE, data)
            send_package(user_fd, MESSAGE.CS_CMD_QUEUE, content)
        end
    end

    skynet.timeout(setting.game_setting.QUEUE_REFRESH_TIME, deal_wait_queue)
end

local function process_enter_server(fd, cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local uin = decode.Uin
    local openid = decode.OpenId
    local retry = decode.Retry
    local IMEI = decode.IMEI
    local Version = decode.Version

   

    logger.trace("enterserver uin: ", uin, IMEI, Version)

    local bfkey = skynet.call(redis_service, 'lua', 'hget_obj', 'bfkey', uin, true)
    if bfkey then
        fd_key_dict[fd] = bfkey
    end

    --如果当前出于停服状态且玩家不存于停服白名单中
    if hotfix.stop_status then
        -- ForceExit = 2 表示当前出于停服状态
        local data = { Overload = 1, ForceExit = 2}
        return send_response(fd, cmd, data)
    end

    local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
    Version = Version or 0
    if (account_info.version and account_info.version ~= Version) or (not account_info.version) then
        local alive_agent = the_gamed.get_alive_agent(account_info.uin)
        if alive_agent then
            skynet.call(alive_agent, "lua", "set_account_info", "version", Version)
        else
            account_info.version = Version
            skynet.call(redis_service, "lua", "hset_obj", "account", uin, account_info)
        end
    end

    -- 判断能否登录
    if account_info and account_info.can_login_time and account_info.can_login_time > os.time() then
        return send_error_code(fd, cmd, ERRORCODE.FORBID_LOGIN)
    end

    -- uin, fd映射
    user_fd_map[uin] = fd
    fd_user_map[fd] = uin

    --断线重连直接进入游戏服
    if retry == 1 then
        report_online(uin)
        local agent = the_gamed.login_handler(fd, uin, bfkey, true, IMEI)

        if not online_uin[uin] then
            online_num = online_num + 1
            online_uin[uin] = account_info.platform
        end


        for _,t in ipairs(pending_msg[fd] or {}) do
            if type(t) == "table" then
                logger.logf ("forward pending to agent %d", agent)
                skynet.redirect(agent, self_id, "client", 0, t.msg, t.sz)
            end
        end
        pending_msg[fd] = nil

        local data = { Overload = 0, GameServerName = node_name, Ts = math.floor(skynet.time()) }
        return send_response(fd, cmd, data)
    end

    if alive_agent_num >= online_agent_limit then
        local queue_num = utils.List.getSize(user_wait_list)
        local force_exit

        logger.trace("queue_num , queue_user_limit ", queue_num , " : ", queue_user_limit)
        if queue_num >= queue_user_limit then
            force_exit = 1
        elseif not user_wait_hash[uin] then
            utils.List.pushBack(user_wait_list, uin)
            user_wait_hash[uin] = true
            queue_num = queue_num + 1
        end

        local data = { Overload = 1, ForceExit = force_exit, Wait_Num = queue_num }
        return send_response(fd, cmd, data)
    else
        if auth_user_list[uin] then
            report_online(uin) --报告上下线
            local agent = the_gamed.login_handler(fd, uin, bfkey, IMEI)
            if not online_uin[uin] then
                online_num = online_num + 1
                online_uin[uin] = account_info.platform
            end

            for _,t in ipairs(pending_msg[fd] or {}) do
                logger.logf("forward pending to agent %d", agent)
                skynet.redirect(agent, self_id, "client", 0, t.msg, t.sz)
            end
            pending_msg[fd] = nil

            local data = { Overload = 0, GameServerName = node_name, Ts = math.floor(skynet.time()) }
            return send_response(fd, cmd, data)
        else
            --查找此用户是否是在auth_user_list中，不在则报异常
            return send_error_code(fd, cmd, ERRORCODE.ACCOUNT_NO_LOGIN)
        end
    end
end

local function trans_uin_to_name(uin)
    uin = uin + 987654
    local uin_str = tostring(uin)
    local len = uin_str:len()
    local name = ""
    for i = 1, len do
        local char = tostring(string.sub(uin_str, i, i))
        name = name .. uin_trans_map[char]
    end
    return name
end

--定时清理缓存队列

--主启动函数
function gameserver.start(gamed)
    local handler = {}
    the_gamed = gamed
    handler.hotfix = hotfix

    handler.set_heart_tick_flag = function (flag) heart_tick_flag = flag end

    proto.load_proto()

    function handler.open(source, conf)
        --给centerserver发送注册gameserver消息
        node_name = conf.servername
        host = conf.host .. ":" .. tostring(conf.port)

        if heart_tick_flag == 1 then
           pcall(cluster.call, "gslbnode", ".gslbserver", "reg_gamed", conf.servername, host)
        end
        redis_service = skynet.queryservice("redis_service")
        busilog_service = skynet.queryservice("business_log_service")

        --发送心跳给负载均衡服
        skynet.timeout(heartbeat_time, send_heartbeat)

        -- 发送在线数据
        send_online_log()
        deal_wait_queue() --排队处理定时器
        return gamed.open(conf)
    end

    function handler.connect(fd, addr)
        logger.logf("connect from %s (fd = %d)", addr, fd)
        gate_server.openclient(fd)
    end

    --只要收到fin包，就会触发这个函数
    function handler.disconnect(fd)
        local agent = fd_agent_map[fd]
        local uin = fd_user_map[fd]
        if agent then
            --服务器主动关闭时,agent可能已经销毁
            pcall(skynet.call, agent, "lua", "disconnect", uin, fd)
            fd_agent_map[fd] = nil
        end

        if uin then
            fd_user_map[fd] = nil
            if online_uin[uin] then
                online_uin[uin]  = nil
                online_num = online_num - 1
            end
            user_fd_map[uin] = nil
            report_offline(uin)
        end

        logger.logf("fd (%d) disconnected", fd)
    end

    --消息转发处
    function handler.message(fd, msg, sz)

        -- 只有第一个消息,拿到uin之前,key是空的,要求客户端是明文消息
        local cmd, content, len = proto.parse_prototype(msg, sz, fd_key_dict[fd])

        logger.tracef("online num:%d, agent:%d ", online_num, alive_agent_num)

        if cmd == COMMON.MESSAGE.CS_CMD_ENTER_SERVER then
            if hotfix.gameserver_force_exit then
                --用于紧急情况下，禁止用户登陆本服的处理
                local data = { Overload = 1, ForceExit = 1, Wait_Num = 2000 }
                return send_response(fd, cmd, data)
            end
            return process_enter_server(fd, cmd, content, len)
        elseif cmd == COMMON.MESSAGE.CS_CMD_DIRECT_ENTER then
            --从排队队列中出来，发送直接登陆协议
            return send_response(fd, cmd, { Ret = 1 })
        elseif COMMON.protomap[cmd] then
            if not pending_msg[fd] then
                pending_msg[fd] = {}
            end
            local queue = pending_msg[fd]
            if #queue < 5 then
                table.insert(queue, {msg = msg, sz = sz})
                queue.update_ts = math.floor(skynet.time())
            end
        else
            logger.trace("gameserver.lua:handler.message> invalid request cmdid:" .. cmd)
        end
    end

    local CMD = {}
    function CMD.login(openid, channelid, secret_key, bfkey, logind)
        local ret = 0
        logger.trace("login openid: ", openid, " logind: ", logind)
        if not COMMON.platform[channelid] then
            return COMMON.LOGIN_PLATFORM_NOT_EXIST
        end

        -- 改变表里面的Key的值
        local function change_player_attr(des,src)
            for k,v in pairs(src) do
                des[k] = v
            end
        end

        local channel_name = COMMON.platform[channelid]
        local combine_id = channel_name .. "_" .. openid
        local uin = skynet.call(redis_service, "lua", "hget_obj", "uin_mapping", combine_id)
        if uin == nil then
            -- 创建新用户
            uin = skynet.call(redis_service, "lua", "id_handler", "account_max_id")
            skynet.call(redis_service, "lua", "hset_obj", "uin_mapping", combine_id, uin)
            local xinge_info = {
                uin = uin,
                secret_key = secret_key,
                login_time = math.floor(os.time()),
            }
            --信鸽数据
            skynet.call(redis_service, "lua", "hset_obj", "xinge", uin, xinge_info)

            local account_info = {}
            for k, v in pairs(db_setting.account_attr) do
                account_info[k] = v
            end
            change_player_attr(account_info, {
                uin = uin, name = uin, openid = openid,
                create_time = math.floor(skynet.time()),
                platform = channelid
            })
            account_info.name = trans_uin_to_name(uin)
            skynet.call(redis_service, "lua", "hset_obj", "account", uin, account_info)
            skynet.send(busilog_service, "lua", "log_login", account_info , COMMON.player_operation.player_create)
        end

        if uin then
            --信鸽数据
            local xinge_info = skynet.call(redis_service, "lua", "hget_obj", "xinge", uin)
            if xinge_info then
                xinge_info.login_time = math.floor(os.time())
                skynet.call(redis_service, "lua", "hset_obj", "xinge", uin, xinge_info)
            else
                local xinge_info = {
                    uin = uin,
                    secret_key = secret_key,
                    login_time = math.floor(os.time()),
                }
                --信鸽数据
                skynet.call(redis_service, "lua", "hset_obj", "xinge", uin, xinge_info)
            end
            local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
            change_player_attr(account_info, {
                login_time = os.time(),
                login_out_time = 0, openid = openid, platform = channelid
            })
            skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info)
            auth_user_list[uin] = true  --Todo:需要对这个地方进行重新置位

            -- 登录服务器传来的bfkey保存在redis中 (需要持久化)
            skynet.call(redis_service, 'lua', 'hset_obj', 'bfkey', uin, bfkey, true)

            ret = 0
        else
            ret = ERRORCODE.ACCOUNT_LOGIN_EXPECTION
        end
        return ret, uin
    end

    function CMD.destory_agent(uin)
        local agent = gamed.get_alive_agent(uin)
        if agent then
            gamed.destory_agent(agent, uin)
        end
        return true
    end

    -- clustername集群配置更新
    function CMD.cluster_reload()
        logger.trace("gameserver clustername reload !!!")
        cluster.reload()
    end

    function handler.command(cmd, _, ...)
        local f = CMD[cmd]
        if f then
            return f(...)
        else
            return gamed.command_handler(cmd, ...)
        end
    end

    return gate_server.start(handler)
end

return gameserver


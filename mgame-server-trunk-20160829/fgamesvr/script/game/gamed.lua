local skynet     = require "skynet"
local gameserver = require "gameserver"
local logger     = require "logger"
local utils      = require "utils"
local cluster    = require "cluster"

local COMMON          = require "common"
local sharedata_utils = require "sharedata_utils"
local setting        = require "setting"

local gamed = {}

local agent_pool = {}
local alive_agents = {}
local online_account = {}

local MULTICAST

local last_pvp_noprop_status
local stop_send_flag = setting.stop_notice_flag

local function check_pvp_noprop_status()
    local segments = sharedata_utils.get_stat_all(COMMON.plan.PvpMatch)
    return utils.IsInSegments(segments, os.time())
end

-- 每隔1分钟检测一次pvp无道具场开启状态，发现不同就发广播
local function check_pvp_noprop_time()
    skynet.timeout(100 * 60, check_pvp_noprop_time)
    local cs = check_pvp_noprop_status()
    if last_pvp_noprop_status ~= cs then
        skynet.call(MULTICAST, 'lua', 'server_event', 'PVP_NOPROP_INFO', cs)
    end

    last_pvp_noprop_status = cs
end

function gamed.open(config)
    logger.log("gamed opened")
    local self = skynet.self()
    local n = config.pool or 0
    for i = 1, n do
        table.insert(agent_pool, skynet.newservice("agent", self))
    end

    MULTICAST = skynet.queryservice('multicast_service')

    last_pvp_noprop_status = check_pvp_noprop_status()
    local rs = 59 - os.date('*t', os.time()).sec + 1 -- 1 sec fix
    skynet.timeout(rs * 100, check_pvp_noprop_time)
end

--------------- CMD定义-------------
local CMD = {}

--从在线用户中剔除
function CMD.kick(agent, fd, uin, client_close)
    online_account[uin] = nil
    if not client_close then
        gameserver.kick(fd, uin)
    end
end

function CMD.kickoff(agent, uin)
    alive_agents[uin] = nil
    gameserver.kickoff(uin)
end

--通过uin判断玩家是否在线 返回agent或nil
function CMD.get_agent(uin)
    return online_account[tonumber(uin)] or nil
end

--支持存活agent查找，支付应该调用这个接口
function CMD.get_alive_agent(uin)
    return alive_agents[tonumber(uin)] or nil
end

function CMD.exit()
    gameserver.exit()
    for k, v in pairs(online_account) do
        -- NOTICE: logout may call skynet.exit, so you should use pcall.
        pcall(skynet.call, v, "lua", "exit_save_db")
    end

    for k, v in pairs(alive_agents) do
        -- NOTICE: logout may call skynet.exit, so you should use pcall.
        pcall(skynet.send, v, "lua", "agent_stop_exit", stop_send_flag)
    end
end

--通知所有agent活动开始
function CMD.notify_act_open(act)
    for k, v in pairs(alive_agents) do
        pcall(skynet.call, v, "lua", "on_act_open", act)
    end
end

--通知所有agent活动结束
function CMD.notify_act_end(act)
    for k, v in pairs(alive_agents) do
        pcall(skynet.call, v, "lua", "on_act_end", act)
    end
end

--通知所有agent活动更新
function CMD.notify_act_update(act)
    for k, v in pairs(alive_agents) do
        pcall(skynet.call, v, "lua", "on_act_update", act)
    end
end

function CMD.add_activity(config)
    for _, v in pairs(alive_agents) do
        pcall(skynet.call, v, "lua", "add_activity", config)
    end
end

function CMD.activity_state_changed(activityId, state)
    for _, v in pairs(alive_agents) do
        pcall(skynet.call, v, "lua", "activity_state_changed", activityId, state)
    end
end

function CMD.changeGlobalData(activityId, data)
    for _, v in pairs(alive_agents) do
        pcall(skynet.call, v, "lua", "changeGlobalData", activityId, state)
    end
end

function gamed.command_handler(cmd, ...)
    local f = assert(CMD[cmd])
    return f(...)
end

function gamed.destory_agent(agent, account)
    online_account[account] = nil
    alive_agents[account] = nil
    pcall(skynet.call, agent, "lua", "exit_agent", account)
end

function gamed.login_handler(fd, account, bfkey, retry, IMEI)
    local agent = alive_agents[account]
    if agent then
        logger.warningf("multiple login detected for account %d", account)
        local kick_flag = online_account[account] and true or false
        skynet.call(agent, "lua", "kick", account, kick_flag, IMEI)
    else
        if #agent_pool == 0 then
            agent = skynet.newservice("agent", skynet.self())
            logger.logf("agent_pool is empty, new agent(%d) created", agent)
        else
            agent = table.remove(agent_pool, 1)
        end

        retry = false --非重连agent，要重置为false

        alive_agents[account] = agent

        --在这里记agent个数
        gameserver.notice_newagent()
    end

    online_account[account] = agent
    skynet.call(agent, "lua", "open", fd, account, bfkey, retry, IMEI)
    gameserver.forward(fd, agent)

    --向本服的在线管理器注册
    skynet.send(".online_mgr","lua", "online", account, agent)

    return agent
end

-- 处理凌晨在线的玩家数据  --也处理缓存的agent，要不然会漏掉的
function gamed.process_daily_check()
    for k, v in pairs(alive_agents) do
        pcall(skynet.call, v, "lua", "process_daily_check")
    end
end

function gamed.get_alive_agent(uin)
    return alive_agents[tonumber(uin)]
end
gameserver.start(gamed)

--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/9/29
-- Time: 15:20
-- Desc: 聊天服务
--

local skynet = require "skynet"
local logger = require "logger"
local utils = require "utils"
local COMMON = require "common"

local multicast_service

local CMD = {}

local last_broadcast_time
--最多存储10条 每秒钟触发一次定时器 看队列中有没有 如果有则播一条
local broadcast_check_time = 100
local global_msg = {}
local function check_broadcast()
    if skynet.now() - last_broadcast_time >= 100 then
        if #global_msg > 0 then
            local send_data = table.remove(global_msg, 1)
            skynet.call(multicast_service, "lua", "publish_announce", COMMON.MESSAGE.CS_CMD_PVP_CHAT, send_data)
        end
        last_broadcast_time = skynet.now()
    end

    skynet.timeout(broadcast_check_time, check_broadcast)
end

function CMD.chat2all(data, account_info, vip_level)
    local send_data = {
        Channel = data.Channel,
        Uin     = account_info.uin,
        Name    = account_info.name,
        Title   = account_info.PlayerTitle,
        Content = data.Content,
        Icon    = account_info.icon_id,
        Id      = data.Id,
    }

    if vip_level and vip_level ~= 0 then
        send_data.VipLevel = vip_level
    end

    if #global_msg > 10 then
        table.remove(global_msg, 1)
    end
    table.insert(global_msg, send_data)
    --    skynet.call(multicast_service, "lua", "publish_announce", COMMON.MESSAGE.CS_CMD_PVP_CHAT, send_data)
end

local traceback = debug.traceback
skynet.start(function()
    multicast_service = skynet.queryservice("multicast_service")

    last_broadcast_time = skynet.now()
    check_broadcast()

    skynet.dispatch("lua", function(_, _, command, ...)
--        logger.trace("chat_service command : " .. command)
--        logger.trace("params : ", ...)
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





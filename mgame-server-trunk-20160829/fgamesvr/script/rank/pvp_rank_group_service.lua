local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local pvp_rank_mgr = require "pvp_rank_mgr"

local mgr_handle = pvp_rank_mgr.create_handle()


local CMD = {}

function CMD.on_player_rank_change(info)
    pvp_rank_mgr.on_player_rank_change(mgr_handle, info)
end

function CMD.search_player_rank(uin)
    return pvp_rank_mgr.search_player_rank(mgr_handle, uin)
end

function CMD.remove_player(uin)
    pvp_rank_mgr.remove_player(mgr_handle, uin)
end

function CMD.flush_all()
    pvp_rank_mgr.flush_all(mgr_handle)
end

function CMD.search_rank_player(rank)
    return pvp_rank_mgr.search_rank_player(mgr_handle, rank)
end

function CMD.get_player_count()
    return pvp_rank_mgr.get_info_count(mgr_handle)
end

skynet.start(function()
    local traceback = debug.traceback

    skynet.dispatch("lua", function(session, src, command, ...)
        local f = CMD[command]
        if f then
            if session ~= 0 then
                skynet.retpack(f(...))
            else
                f(...)
            end
        else
            logger.trace("unknow command : ", command, src)
            if session ~= 0 then
                skynet.ret()
            end
        end
    end)
    logger.trace("group ser started")
end)

--
--逻辑服排行榜数据管理模块
--auth : diandian
--date : 2016.3.1
--

local skynet = require "skynet"
local logger = require "logger"
local utils = require "utils"
local COMMON = require "common"
local cluster = require "cluster"

--前50名排名玩家数据缓存
local rank_top_cache = {}

--单个玩家排名数据缓存
local player_rank_cache = {}

local CMD = {}

function CMD.get_rank(uin)
    local cache_dat = player_rank_cache[uin]
    if not cache_dat or (os.time() - cache_dat.cache_time) > 10 then
        local rank, group = cluster.call("ranknode1", ".pvp_rank_service",
            "search_player_rank", uin)
        cache_dat = {}
        cache_dat.rank = rank
        cache_dat.group = group
        cache_dat.cache_time = os.time()
        player_rank_cache[uin] = cache_dat
    end
    return cache_dat.rank, cache_dat.group
end


function CMD.get_group_rank_top(group, top)
    if not rank_top_cache[group]
        or (os.time() - rank_top_cache[group].cache_time) > 6 then
        rank_top_cache[group] = {}
        rank_top_cache[group].cache_time = os.time()
        rank_top_cache[group].data = cluster.call("ranknode1", 
            ".pvp_rank_service", "get_group_rank_top", group, top)
    end
    return rank_top_cache[group].data
end

function CMD.on_player_rank_change(data)
    cluster.call("ranknode1", ".pvp_rank_service", "on_player_rank_change", data)
    if player_rank_cache[data.uin] then
        player_rank_cache[data.uin].cache_time = 0
    end
end

local traceback = debug.traceback
skynet.start(function()

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
end)


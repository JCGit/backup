local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local cluster = require "cluster"

local name

local CMD = {}

local function space_search()
    local ft = skynet.now()
    local bt
    
    for i = 1, 10000 do
        local uin = math.random(10000001, 15000010)
                    
        if (i % 2) == 0 then
            cluster.call("ranknode1", ".pvp_rank_service",
                        "get_player_rank_data", uin)
        else
            cluster.call("ranknode1", ".pvp_rank_service",
                        "search_player_rank", uin)
        end
                    
        if (i % 8) == 0 then
            cluster.call("ranknode1", ".pvp_rank_service",
                        "get_player_self_ranklist", uin)
        end
    end
    
    bt = skynet.now()
    
    print(name, "space_search:", bt - ft)
    skynet.timeout(100, space_search)
end

function CMD.start(name_)
    name = name_
    
    skynet.timeout(100, space_search)
end

skynet.start(function()
    skynet.dispatch("lua", function(session, src, command, ...)
        local f = CMD[command]
        skynet.retpack(f(...))
    end)
end)
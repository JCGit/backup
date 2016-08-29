local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local cluster = require "cluster"

local name

local function space_insert()
    local ft = skynet.now()
    local bt
    
    local map = {}
    for i=1, 10000 do
        local uin = math.random(10000001, 15000010)
        if not map[uin] then
            map[uin] = {
                point = math.random(1400, 4500),
                win_rate = math.random(0, 10000),
                uin = uin,--999999999 - max_id,
                name = tostring(uin),
                icon = 0,
            }
        else
            map[uin].point = math.random(1400, 4500)
            map[uin].win_rate = math.random(0, 10000)
        end
        
        cluster.call("ranknode1", ".pvp_rank_service",
                    "on_player_rank_change", map[uin])
    end
    
    bt = skynet.now()
    print(name, "space_insert:", bt - ft)
    skynet.timeout(100, space_insert)
end

local CMD = {}

function CMD.start(name_)
    name = name_
    
    skynet.timeout(100, space_insert)
end

skynet.start(function()
    skynet.dispatch("lua", function(session, src, command, ...)
        local f = CMD[command]
        skynet.retpack(f(...))
    end)
end)
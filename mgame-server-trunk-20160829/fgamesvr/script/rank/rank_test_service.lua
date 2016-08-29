local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local pvp_rank_service

local function sort_fun(a, b)
    if a.point == b.point then
        if a.win_rate == b.win_rate then
            return a.uin < b.uin
        else
            return a.win_rate > b.win_rate 
        end
    else
        return a.point > b.point
    end
end

local max_id
local index = 0
local total_count = 1


local function get_group(point)
    if point <= 2000 then
        return 1
    elseif point > 2000 and point <= 2400 then
        return 2
    elseif point > 2400 and point <= 2800 then
        return 3
    else
        return 4
    end
end

local function check_rank_true(map)

    local list = {}
    for uin, info in pairs(map) do
        table.insert(list, info)
    end
    table.sort(list, sort_fun)
    print("check count", #list)
    
    local count = 0
    for i, v in ipairs(list) do
        local rank = skynet.call(pvp_rank_service, "lua", "search_player_rank", v.uin)
        if i ~= rank then
            --print(i, rank, v.point, v.win_rate, v.uin)
            count = count + 1
        end
    end
    print("no match count ", count)
end

    local group_list = {}
    group_list[1] = {}
    group_list[2] = {}
    group_list[3] = {}
    group_list[4] = {}
    
    map = {}
    list = {}
local function start_insert()
    local time
    local end_time


    time = skynet.now()
    for i=1, 50000 do
        local uin = math.random(10000001, 21000010)
        if not map[uin] then
            map[uin] = {
                point = math.random(1400, 3500),
                win_rate = math.random(30, 1000),
                uin = uin,--999999999 - max_id,
            }
            table.insert(list, uin)
        else
            local od_group = get_group(map[uin].point)
            group_list[od_group][uin] = nil
            map[uin].point = math.random(1400, 3500)
            map[uin].win_rate = math.random(30, 1000)
        end
        local info = map[uin]
        info.group = get_group(info.point)
        group_list[info.group][uin] = info;
        skynet.send(pvp_rank_service, "lua", "on_player_rank_change", info)
        skynet.sleep(0)

        skynet.call(pvp_rank_service, "lua", "search_player_rank", uin)
    end
    end_time = skynet.now()
    local spend_time = end_time - time
    print("insert space time: ", spend_time)

    time = skynet.now()
    for i=1, 10000 do
        local uin = math.random(10000001, 21000010)
        local dat = map[uin]
        if dat then
            skynet.call(pvp_rank_service, "lua", "search_player_rank", uin)
        end
    end
    end_time = skynet.now()
    print("search space time", end_time - time)
        
    index = index + 1
    if index >= 10 then
        print("insert finished....", #list)
        
        skynet.send(pvp_rank_service, "lua", "flush_all")
        --skynet.sleep(600)
        print("start check rank true")
        for group, map in pairs(group_list) do
            check_rank_true(map)
        end
        print("check true complete")
    else
        if spend_time < 100 then
            skynet.timeout(100-spend_time, start_insert)
        else
            skynet.timeout(100, start_insert)
        end
    end
end

local function init()
    start_insert()
    --skynet.timeout(10, start_insert)
end

local CMD = {}


skynet.start(function()
    local traceback = debug.traceback
    redis_service = skynet.queryservice("redis_service")
    pvp_rank_service = skynet.queryservice("pvp_rank_service")

    --init()
    skynet.dispatch("lua", function(_, src, command, ...)
        local f = cmd[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s src:%d", command, ret, src)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)

end)

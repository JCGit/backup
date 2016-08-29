--排行榜服务
--auth : diandian
--date : 2016.2.23


local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local pvp_rank_mgr = require "pvp_rank_mgr"
local sharedata_utils = require "sharedata_utils"
local COMMON = require "common"

local redis_service

local group_services = {}
local player_datas = {}

local group_point = {}


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

local function get_group(point)
    local gpid
    for gpid, gp in ipairs(group_point) do
        if point >= gp.min_point and point <= gp.max_point then
            return gpid
        end
    end
    return gpid
end

local function init_group_point()
    local all_cfg = sharedata_utils.get_stat_all(COMMON.plan.GradeData)
    for id, cfg in pairs(all_cfg) do
        local gp = group_point[tonumber(cfg.group)]
        if not gp then
            gp = {
                min_point = tonumber(cfg.minPoint),
            }
        end
        group_point[tonumber(cfg.group)] = gp
        gp.max_point = tonumber(cfg.maxPoint)
    end
end

--做一次单个数据handle数据测试
local function test_rank(CMD)
    local handle = pvp_rank_mgr.create_handle()
    print("handle:>>>", handle)
    
    local time
    local end_time

    local list = {}
    local map = {}
    time = skynet.now()
    for i=1, 1000000 do
        local uin = math.random(10000001, 21000010)
        if not map[uin] then
            map[uin] = {
                point = math.random(1400, 4500),
                win_rate = math.random(0, 10000),
                uin = uin,--999999999 - max_id,
            }
            table.insert(list, map[uin])
        else
            map[uin].point = math.random(1400, 4500)
            map[uin].win_rate = math.random(0, 10000)
        end
        local dat = map[uin]
        pvp_rank_mgr.on_player_rank_change(handle, dat)
    end
    end_time = skynet.now()
    
    print(end_time - time)
    
    table.sort(list, sort_fun)
    time = skynet.now()
    pvp_rank_mgr.flush_all(handle)
    end_time = skynet.now()
    print("flush_all", end_time - time)
    
    time = skynet.now()
    for i, v in ipairs(list) do
        local rank = pvp_rank_mgr.search_player_rank(handle, v.uin)
        if i ~= rank then
            print(i, rank, v.point)
        end
    end
    end_time = skynet.now()
    print("search", end_time - time, #list)
end

--做一次数据库正式数据测试
local function test_search_rank_player()
    local list = {}
    for uin, v in pairs(player_datas) do
        table.insert(list, v)
    end
    
    table.sort(list, sort_fun)
    print("test_search_rank_player", #list)
    for i, v in ipairs(list) do
        local uin = skynet.call(group_services[v.group], "lua", "search_rank_player", i)
        if v.uin ~= uin then
            print(v.uin, uin)
        end
    end
end


local CMD = {}


function CMD.on_player_rank_change(info)
    if not info.group then
        info.group = get_group(info.point)
    end
    
    local old_info = player_datas[info.uin]
    if old_info then
        if old_info.group ~= info.group then
            skynet.send(group_services[old_info.group],
                "lua", "remove_player", old_info.uin)
        end
    end
    player_datas[info.uin] = info
    skynet.send(group_services[info.group], 
        "lua", "on_player_rank_change", info)
end

function CMD.search_player_rank(uin)
    if not player_datas[uin] then
        return 0
    end
    return skynet.call(group_services[player_datas[uin].group], "lua",
        "search_player_rank", uin), player_datas[uin].group
end

function CMD.get_player_rank_data(uin)
    if not player_datas[uin] then
        return nil
    end
    
    local data = player_datas[uin]
    local rank = skynet.call(group_services[data.group], "lua",
        "search_player_rank", uin)
    data.rank = rank
    return data
end

local function flush_all()
    for i, group_svr in pairs(group_services) do
        skynet.send(group_svr, "lua", "flush_all")
    end
    collectgarbage("collect")
end

function CMD.flush_all()
    flush_all()
end

function CMD.get_group_rank_top(group, top)
    local count = skynet.call(group_services[group],
        "lua", "get_player_count")
    local num = top <= count and top or count
    local rank_top = {}
    for i = 1, num do
        local uin = skynet.call(group_services[group],
            "lua", "search_rank_player", i)
        if player_datas[uin] then
            table.insert(rank_top, player_datas[uin])
        end
    end
    return rank_top
end

function CMD.get_player_self_ranklist(uin)
    if not player_datas[uin] then
        return {}, 0
    end
        
    local ranklist = {}
    local data = player_datas[uin]
    local rank = skynet.call(group_services[data.group], "lua",
        "search_player_rank", uin)
    ranklist[rank] = data
    
    local front = (rank <= 10) and 1 or (rank - 10)
    for i = front, (rank - 1) do
        local fuin = skynet.call(group_services[data.group], "lua",
            "search_rank_player", i)
        ranklist[i] = player_datas[fuin]
    end
    
    local count = skynet.call(group_services[data.group], "lua",
        "get_player_count")
    local back = ((count - rank) < 10) and count or (rank + 10)
    for i = rank + 1, back do
         local fuin = skynet.call(group_services[data.group], "lua",
            "search_rank_player", i)
        ranklist[i] = player_datas[fuin]
    end
    return ranklist, data.group
end

function CMD.remove_player(uin)
    if not player_datas[uin] then
        return
    end
    
    local info = player_datas[uin]
    skynet.send(group_services[info.group],
                "lua", "remove_player", info.uin)
end

function CMD.check_login_out_long_time_player(space_time)
    local all_info = skynet.call(redis_service, "lua", 
                            "hgetall_all_value", "in_pvp_rank_info")
                            
    if not all_info then
        return 0
    end
    
    local clear_num = 0
                            
    local now = os.time()
    for uin, info in pairs(all_info) do
        if space_time >= (now - info.login_out_time) then
            skynet.call(skynet.self(), "lua", "remove_player", uin)
            skynet.call(redis_service, "lua", "hdel_obj", "in_pvp_rank_info", uin)
            clear_num = clear_num + 1
        end
    end
    return clear_num
end

local function space_flush()
    flush_all()
    skynet.timeout(1000, space_flush)
end

--插入假数据 让排行榜开启
local function init_virtual_data()
    local len = skynet.call(redis_service, "lua", "hlen", "rank")

    -- 排行榜人数不超过50个就插入假数据
    if len < 50 then
        logger.trace("rank len is < 50 , create virtual rank player")
        local plan_data = sharedata_utils.get_stat_all(COMMON.plan.Rank)
        for k, v in pairs(plan_data) do
            local rand_name = sharedata_utils.get_rand_name()
            local data = {
                uin = v.id,
                name = rand_name,
                win_rate = tonumber(v.winRate),
                point = 1800 + tonumber(v.score),
                icon = 1,
                streak_win_count = math.random(0, 3),
            }
            skynet.call(redis_service, "lua", "hset_obj", "rank", v.id, data)
        end
    end
end

local function init_data(CMD)
    init_virtual_data()

    local temp = skynet.call(redis_service, "lua", "hgetall", "rank")
    local len = #temp / 2
    
    for i = 1, len do
        local uin = tonumber(temp[i * 2 - 1])
        local dat = skynet.unpack(temp[i * 2])
        if not dat.group then
            dat.group = get_group(dat.point)
            skynet.call(redis_service, "lua", "hset_obj", "rank", dat.uin, dat)
        end
        CMD.on_player_rank_change(dat)
        player_datas[uin] = dat
    end
    
    collectgarbage("collect")
end

skynet.start(function()
    local traceback = debug.traceback
    redis_service = skynet.queryservice("redis_service")

    group_services[1] = skynet.newservice("pvp_rank_group_service")
    group_services[2] = skynet.newservice("pvp_rank_group_service")
    group_services[3] = skynet.newservice("pvp_rank_group_service")
    group_services[4] = skynet.newservice("pvp_rank_group_service")
    
    init_group_point();
    --test_rank(CMD)
    
    init_data(CMD)
    space_flush()
    
    --test_search_rank_player()
    
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
    logger.trace("pvp_rank_service started")
end)

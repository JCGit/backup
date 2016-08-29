--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2015/8/22
-- Time: 17:28
-- Desc: pvp排行榜服务
-- 定期从redis拉取数据,玩家取的排行榜不是实时的
--

local skynet = require "skynet"
local logger = require "logger"
local utils = require "utils"
local COMMON = require "common"
local setting = require "setting"
local sharedata_utils = require "sharedata_utils"
local cluster = require "cluster"
local player = require "player"
local server_config = require "server_config"

local redis_service
local mail_service
local gamed
local busilog_service

local redis_cache = {}
local rank_data = {}

-- 排名结果
local rank_result = {} -- group => [ rank _data ]
local rank_cache = {} -- key: uin value : {group = a, rank = b)

local christmas_rank = {}

local GROUP = {
    Bronze  = 1, -- 学徒
    Silver  = 2, -- 主厨
    Gold    = 3, -- 御厨
    Diamond = 4, -- 厨神
}

-- 排行规则
local function compare(a, b)
    if tonumber(a.point) == tonumber(b.point) then
        return tonumber(a.win_rate) > tonumber(b.win_rate)
    else
        return tonumber(a.point) > tonumber(b.point)
    end
end

--折半查找 arr是按照积分排好序的数组 info是数组中单个信息 func是排序规则
local function bin_search(arr, info, func)
    if #arr <= 0 then return 1 end

    info.point = info.point or info.pvp_point
    info.win_rate = info.win_rate or utils.getPvpWinRate(info)

    local low = 1
    local high = #arr
    if func(arr[high], info) then
        return high
    end

    if func(info, arr[low]) then
        return low
    end

    while low < high do
        local mid = low + math.floor((high- low)/2)
        if func(arr[mid], info) then
            low = mid
        else
            high = mid
        end

        if high - low == 1 then
            if arr[low].uin == info.uin then
                return low
            else
                return high
            end
        end
    end
end

-- 从redis取数据并计算排行榜
local function init_ranklist()
    redis_cache = {}

    rank_data = {
        [GROUP.Bronze] = {},
        [GROUP.Silver] = {},
        [GROUP.Gold] = {},
        [GROUP.Diamond] = {},
    }
    -- 积分排行榜(从大到小)
    redis_cache = skynet.call(redis_service, "lua", "hgetall_obj", "rank")
    table.sort(redis_cache, compare)

    local gradeData = sharedata_utils.get_stat_all(COMMON.plan.GradeData)
    for i = GROUP.Bronze, GROUP.Diamond do
        local groupId = i
        local minGroupId = groupId * 3 - 2
        local maxGroupId = groupId * 3
        local minPoint = tonumber(gradeData[minGroupId].minPoint)
        local maxPoint = tonumber(gradeData[maxGroupId].maxPoint)

        local remove_tab = {}
        for i, v in ipairs(redis_cache) do
            if v.point >= minPoint and v.point <= maxPoint then -- 判断条件[minPoint, maxPoint]
                table.insert(rank_data[groupId], v)
                table.insert(remove_tab, i)
            end
        end

        for i = #remove_tab, 1, -1 do
            table.remove(redis_cache, remove_tab[i])
        end
    end

    rank_result = rank_data
    rank_cache = {}
    for key, data in pairs(rank_result) do
        for i, v in ipairs(data) do
            rank_cache[v.uin] = {
                group   = key,
                index   = i,
            }
        end
    end

    return
end

-- 前50名写入oss
local function logger_rank()
    local max = setting.game_setting.oss_rank
    local oss_logger = {}
    local count = 0
    local new = table.insert

    -- 从高到低组别总共拿前50排名
    for k,v in ipairs(rank_result[GROUP.Diamond]) do
        if count < max then
            new(oss_logger,v)
            count = count + 1
        else
            break
        end
    end

    for k,v in ipairs(rank_result[GROUP.Gold]) do
        if count < max then
            new(oss_logger,v)
            count = count + 1
        else
            break
        end
    end

    for k,v in ipairs(rank_result[GROUP.Silver]) do
        if count < max then
            new(oss_logger,v)
            count = count + 1
        else
            break
        end
    end

    for k,v in ipairs(rank_result[GROUP.Bronze]) do
        if count < max then
            new(oss_logger,v)
            count = count + 1
        else
            break
        end
    end

    -- 读出数据写到oss里
    for k,v in pairs(oss_logger) do
        if v.uin >= server_config.id_conf.account_max_id then --排除机器人
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_fiftyRank, v.uin, k, v.point, v.win_rate, v.name)
        end
    end

    oss_logger = nil -- help gc
end

local function init(flag)
    init_ranklist()
    rank_result = rank_data
end

local CMD = {}

--插入假数据 让排行榜开启
local function insert_data()
    local len = skynet.call(redis_service, "lua", "hlen", "rank")

    -- 排行榜人数不超过50个就插入假数据
    if len < 50 then
        local plan_data = sharedata_utils.get_stat_all(COMMON.plan.Rank)
        for k, v in pairs(plan_data) do
            local rand_name = sharedata_utils.get_rand_name()
            local data = {
                uin = v.id,
                name = rand_name,
                win_rate = tonumber(v.winRate),
                point = 1800 + tonumber(v.score),
                icon = 1,
            }
            skynet.call(redis_service, "lua", "hset_obj", "rank", v.id, data)
        end
    end
end

local function send_christmas_rankbonus()
    local rank_bonus = sharedata_utils.getChristmasRankBonus()
    for i, v in ipairs(christmas_rank) do
        if christmas_rank[i] then
            local data = rank_bonus[i]
            if not data then return end
            -- 称号处理
            local title = data.title
            if title then
                local agent = skynet.call(gamed, "lua", "get_alive_agent", christmas_rank[i].uin)
                if agent then
                    skynet.call(agent, "lua", "add_title", COMMON.title_type.CHRISTMAS)
                else
                    local player_titles = skynet.call(redis_service, "lua", "hget_obj", "player_titles", christmas_rank[i].uin) or {}
                    if not player_titles[COMMON.title_type.CHRISTMAS] then
                        local data = {}
                        data.time = os.time()
                        player_titles[title] = data
                    end
                    skynet.call(redis_service, "lua", "hset_obj", "player_titles", christmas_rank[i].uin, player_titles)
                end
            end

            local now = os.time()
            local maiContent = data.mailContent
            maiContent = string.gsub(maiContent, "%%d", i)
            local data = {
                title = data.mailTitle,
                content = maiContent,
                plugin = sharedata_utils.Bonus2Plugin(data.BonusId),
                sendtime = now,
                outdate =  now + 30 * 24 * 60 * 60 * 100  -- 一个月之后
            }
            skynet.call(mail_service, "lua", "add_mail", christmas_rank[i].uin, COMMON.mail_type.SINGLE, data, COMMON.change_src.opreate_mail)
        end
    end
end

local function update_christmas_rank()
    local activity_config = player.get_christmas_config()
    if not activity_config then return end
    local now = os.time()
    --if now < activity_config.begindate and now > activity_config.enddate + 3600 then return end
    local data_lists = skynet.call(redis_service, "lua", "hgetall_obj", "christmas_data")
    if not data_lists then return end
    if #data_lists == 0 then return end
    table.sort(data_lists, function(a, b)
        return tonumber(a.score) > tonumber(b.score)
    end)
    christmas_rank = data_lists

    if now > activity_config.enddate then
        -- 发奖,清理活动数据
        send_christmas_rankbonus()
        skynet.call(redis_service, "lua", "del", "christmas_data", true)
        skynet.call(redis_service, "lua", "del", "christmas_score", true)
        christmas_rank = {}
    end
end

function CMD.get_christmas_rank(uin)
    local rank_lists = christmas_rank
    local selfRank = 0
    local lists = {}
    local count = 0
    for i, v in ipairs(rank_lists) do
        local data = rank_lists[i]
        if count < 50 or data.uin == uin then
            count = count + 1
            lists[i] = {
                uin     = data.uin,
                score   = data.score,
            }
        end
    end
    return lists
end

local last_check_time
local pvp_season_time = setting.game_setting.pvp_season_time
local rank_update_time = setting.game_setting.rank_update_time
local update_count = 0
local last_logger_rank = os.time()
local function update_rank()
    local interval_time = 0
    if update_count == 0 then
        local date = os.date("*t", os.time())
        interval_time = ((60 - date.sec) + (60 - date.min - 1) * 60) * 100
    else
        interval_time = rank_update_time
    end

    local now = skynet.now()
    local t = last_check_time + interval_time - skynet.now()
    if t <= 0 then
        init(false)
        if not utils.is_same_day(last_logger_rank) then --第二天颠倒零晨记录前50排名(写入oss)
            logger_rank()
        end
        update_christmas_rank()
        last_check_time = skynet.now()
        last_logger_rank = os.time()
    end

    skynet.timeout(interval_time, update_rank)
    update_count = update_count + 1
end

function CMD.get_rank(groupid, top_count)
    local rank_data = rank_result
    local lists = {}
    if rank_data[groupid] then
        for i = 1, top_count do
            if rank_data[groupid][i] then
                lists[i] = {
                    uin         = rank_data[groupid][i].uin,
                    name        = rank_data[groupid][i].name,
                    point       = rank_data[groupid][i].point,
                    win_rate    = rank_data[groupid][i].win_rate,
                }
            end
        end
    end

    local data = {
        groupid = groupid,
        lists = lists,
    }
    return data
end

local pvp_rank_range = 10 -- 玩家前后10名

function CMD.get_self_ranklist(account_info)
    local rank_data = rank_result
    local self_groupid = sharedata_utils.get_group_by_point(account_info.pvp_point)
    local self_order = 0
    local rank_lists = {}

    for k, v in pairs(rank_data) do
        for key, value in ipairs(v) do
            if value.uin == account_info.uin then
                self_order = key
                break
            end
        end
    end

    if self_groupid ~= 0 and self_order ~= 0 then
        local newRank = CMD.getrankbyplayerinfo(account_info)
        local minRank = (newRank - pvp_rank_range >= 1) and  newRank - pvp_rank_range or 1
        local maxRank = newRank + pvp_rank_range - 1
        for i = minRank, maxRank do
            local rank_item = rank_data[self_groupid][i]
            if rank_item then
                rank_lists[i] = rank_item
            end
        end
    end

    return rank_lists, self_groupid
end

--通过uin查找玩家排名【用玩家当前数据查排名】
function CMD.getrankbyid(uin)
    local data
    local r = rank_cache[uin] or nil
    if r then
        local group = r.group
        local index = r.index
        local info = rank_result[group][index]
        data = {
            groupid     = group,
            order       = index,
            name        = info.name,
            point       = info.point,
            win_rate    = info.win_rate,
        }
    else
        data = {
            groupid     = 0,
            order       = 0,
            name        = "",
            point       = 0,
            win_rate    = 0,
        }
    end
    return data
end

-- 通过玩家信息寻找在排行榜中当前的名次【用玩家当前数据查排名】
function CMD.getrankbyplayerinfo(account_info)
    local point = account_info.pvp_point or account_info.point
    local group = sharedata_utils.get_group_by_point(point) or 0
    local rank_data = rank_result[group]
    if not rank_data then return 0 end -- 玩家积分为0分 返回0 表示未上榜
    local newOrder = bin_search(rank_data, account_info, compare)
    return newOrder
end

-- 随机的玩家是从学徒组选出来的
function CMD.get_rand_data(arr)
    local rank_data = rank_result
    local rand_index
    while 1 do
        rand_index =  math.random(1, #rank_data[GROUP.Bronze])
        local flag = false
        for i, v in ipairs(arr) do
            if v == rank_data[GROUP.Bronze][rand_index].uin then
                flag = true
                break
            end
        end

        if flag == false then
            -- AI的积分范围在1750-1850之间
            if rank_data[GROUP.Bronze][rand_index].point >= 1750 and rank_data[GROUP.Bronze][rand_index].point <= 1850 then
                local data = {
                    uin = rank_data[GROUP.Bronze][rand_index].uin,
                    name = rank_data[GROUP.Bronze][rand_index].name,
                    point = rank_data[GROUP.Bronze][rand_index].point,
                    win_rate = rank_data[GROUP.Bronze][rand_index].win_rate,
                    icon = rank_data[GROUP.Bronze][rand_index].icon,
                }
                return data
            end
        end
    end
end

local traceback = debug.traceback
skynet.start(function()
    gamed = skynet.queryservice("gamed")
    mail_service = skynet.queryservice("mail_service")
    redis_service = skynet.queryservice("redis_service")
    busilog_service = skynet.queryservice("business_log_service")

    -- 导入初始排行榜数据到redis
    --insert_data()

    last_check_time = skynet.now()
    init(true)
    update_rank()
    update_christmas_rank()

    skynet.dispatch("lua", function(_, _, command, ...)
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
        return skynet.retpack(ret)
    end)
end)

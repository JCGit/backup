--
-- Author: 
-- Date: 2016/03/15
-- Desc: 用于缓存全局的一些东西
--
local skynet = require "skynet"
local utils  = require "utils"
local COMMON   = require "common"
local logger   = require "logger"
local cluster  = require "cluster"
local setting  = require "setting"
local queue    = require "skynet.queue"
local sharedata_utils   = require "sharedata_utils"



-- 上一次排行，用于排名上升|下降计算
local pre_star_rank = {} -- 星星排名
local pre_win_rank = {} -- pvp当天胜利场次排名
local pre_king_rank = {} -- pvp连胜场次排名

-- 缓存刷新周期内的排名
local cur_star_rank = {}
local cur_win_rank = {}
local cur_king_rank = {}
--[[
数据格式

pre
{
    --[uin] = rank, 玩家uin 及上次的排名
    [1000001] = 10,
    [1000020] = 20,
    [1000030] = 5
}


cur
{
    {uin = 1000001, value = 10, name = 'abc', up = true},    
    {uin = 1000020, value = 20, name = 'efg', up = false},    
    {uin = 1000030, value = 5, name = 'jkm', up = false},    
}

]]

-- 巅峰排名活动各排行榜的下次刷新时间点
local next_star_ticker = 0 
local next_win_ticker = 0 
local next_king_ticker = 0

local star_update_date = 0
local win_update_date = 0
local king_update_date = 0

local pve_score_rank = {} -- pve每关卡排名的缓存数据(前一天的，也就是redis里面的)
local last_score_date = 0
local score_rank_min = {} -- pve关卡积分最小值
--[[
{
    [1010] = 1000,
    [1020] = 2000,
}
]]

-- 活动的状态 true false
local star_opened = false
local win_opened = false
local king_opened = false


-- 当前排名里面的最小值
local star_min = 0
local win_min = 0
local king_min = 0

local star_table = 'star_rank'
local win_table = 'win_rank'
local king_table = 'king_rank'
local pve_table = 'pve_rank'

local redis_conn 
local tracedebug = debug.traceback

-- 巅峰拓行榜延迟秒数取数据
local delay_sec = 5

local CMD = {}

-- critical section
local cs = queue()

local function convert_star_data(list)
    if next(list) then
        pre_star_rank = {}
        
        -- cur to pre
        for k,v in pairs(cur_star_rank or {}) do
            pre_star_rank[v.uin] = k
        end

        cur_star_rank = {}
        for k,v in pairs(list or {}) do
            local index = pre_star_rank[v.uin]
            if index then
                local up = 0
                up = (k > index and 2) or (k < index and 1) or 0
                table.insert(cur_star_rank, {uin = v.uin, value = v.value, name = v.name, up = up})
            else
                table.insert(cur_star_rank, {uin = v.uin, value = v.value, name = v.name, up = 1})
            end
        end

        if #cur_star_rank >= setting.top_num then
            star_min = cur_star_rank[#cur_star_rank].value
        end
    else -- 没有数据说明就是清空了
        cur_star_rank = {}
        star_min = 0 -- 0 点到 6点会清掉数据所以最小值也需要清空
    end
end

local function convert_win_data(list)
    if next(list) then
        pre_win_rank = {}
        
        -- cur to pre
        for k,v in pairs(cur_win_rank or {}) do
            pre_win_rank[v.uin] = k
        end

        cur_win_rank = {}
        for k,v in pairs(list or {}) do
            local index = pre_win_rank[v.uin]
            if index then
                local up = 0
                up = (k > index and 2) or (k < index and 1) or 0
                table.insert(cur_win_rank, {uin = v.uin, value = v.value, name = v.name, up = up})
            else
                table.insert(cur_win_rank, {uin = v.uin, value = v.value, name = v.name, up = 1})
            end
        end

        if #cur_win_rank >= setting.top_num then
            win_min = cur_win_rank[#cur_win_rank].value
        end
    else -- 没有数据说明就是清空了
        cur_win_rank = {}
        win_min = 0 -- 0 点到 6点会清掉数据所以最小值也需要清空
    end
end

local function convert_king_data(list)
    if next(list) then
        pre_king_rank = {}
        -- cur to pre
        for k,v in pairs(cur_king_rank or {}) do
            pre_king_rank[v.uin] = k
        end

        cur_king_rank = {}
        for k,v in pairs(list or {}) do
            local index = pre_king_rank[v.uin]
            if index then
                local up = 0
                up = (k > index and 2) or (k < index and 1) or 0
                table.insert(cur_king_rank, {uin = v.uin, value = v.value, name = v.name, up = up})
            else
                table.insert(cur_king_rank, {uin = v.uin, value = v.value, name = v.name, up = 1})
            end
        end

        if #cur_king_rank >= setting.top_num then
            king_min = cur_king_rank[#cur_king_rank].value
        end
    else -- 没有数据说明就是清空了
        cur_king_rank = {}
        king_min = 0 -- 0 点到 6点会清掉数据所以最小值也需要清空
    end
end

-- 巅峰排行榜各榜的刷新间隔
local star_inter, win_inter, king_inter
--local interval = setting.top_rank_update + delay_sec
local function should_refresh(pre, now)
    return os.difftime(pre, now) <= 0
end

local function sort_top_data(data)
    table.sort(data, function(itema, itemb)
        return (itema.value > itemb.value) or
            (itema.value == itemb.value and itema.time < itemb.time)
        end)
end

-- 获取下一次刷新的时间间隔
local function time2_hour(date, now, category)
    local after = 0
    local next_time = 0
    local activity_id = COMMON.OP_ACT_TYPE
    
    if category == activity_id.TOP_STAR then
        after = star_inter - date.min % star_inter
    elseif category == activity_id.TOP_WIN then 
        after = win_inter - date.min % win_inter
    elseif category == activity_id.TOP_KING then 
        after = king_inter- date.min % king_inter
    else
        after = 30 - date.min % 30
    end
    --next_time = now + after * 60
    after = after * 60 + delay_sec - date.sec
    next_time = now + after
    return after, next_time
end

local function get_star_list()
    local now = os.time()
    local date = os.date('*t', now)
    if should_refresh(next_star_ticker, now) then
        --next_star_ticker = now + interval
        _, next_star_ticker = time2_hour(date, now, COMMON.OP_ACT_TYPE.TOP_STAR) -- 不采取上面的方法，重新计算一次
        local data = skynet.call(redis_conn, "lua", "hgetall_obj", star_table)
        sort_top_data(data)
        convert_star_data(data)
    end
end

local function get_win_list()
    local now = os.time()
    local date = os.date('*t', now)
    if should_refresh(next_win_ticker, now) then
        --next_win_ticker = now + interval
        _, next_win_ticker = time2_hour(date, now, COMMON.OP_ACT_TYPE.TOP_WIN)
        local data
        data = skynet.call(redis_conn, "lua", "hgetall_obj", win_table)
        sort_top_data(data)
        convert_win_data(data)
    end
end

local function get_king_list()
    local now = os.time()
    local date = os.date('*t', now)
    if should_refresh(next_king_ticker, now) then
        --next_king_ticker = now + interval
        _, next_king_ticker = time2_hour(date, now, COMMON.OP_ACT_TYPE.TOP_KING)
        local data
        data = skynet.call(redis_conn, "lua", "hgetall_obj", king_table)
        sort_top_data(data)
        convert_king_data(data)
    end
end

local function get_top_rank(category)
    local actvity_id = COMMON.OP_ACT_TYPE
    local cur_rank = nil
    assert(category, '获取巅峰排行榜的类型为空')
    if category == actvity_id.TOP_STAR then
        get_star_list()
        cur_rank = cur_star_rank
    elseif category == actvity_id.TOP_WIN then
        get_win_list()
        cur_rank = cur_win_rank
    elseif category == actvity_id.TOP_KING then
        get_king_list()
        cur_rank = cur_king_rank
    else
        assert(nil, '获取巅峰排行榜的类型不对')
    end

    if next(cur_rank) then
        return cur_rank
    end
    return {}
end

local function get_top_secs(category)
    local now = os.time()
    local after = 0
    local next_time = 0
    local date = os.date('*t', now)
    local actvity_id = COMMON.OP_ACT_TYPE
    assert(category, '获取巅峰排行榜的类型为空')
    if category == actvity_id.TOP_STAR then
        if next_star_ticker == 0 then
            after, next_time = time2_hour(date, now, category)
            next_star_ticker = next_time
            return after
        else
            local inter = os.difftime(next_star_ticker, now)
            return ( inter < 0 ) and 0 or inter
        end
    elseif category == actvity_id.TOP_WIN then
        if next_win_ticker == 0 then
            after, next_time = time2_hour(date, now, category)
            next_win_ticker = next_time
            return after
        else
            local inter = os.difftime(next_win_ticker, now)
            return ( inter < 0 ) and 0 or inter
        end
    elseif category == actvity_id.TOP_KING then
        if next_king_ticker == 0 then
            after, next_time = time2_hour(date, now, category)
            next_king_ticker = next_time
            return after
        else
            local inter = os.difftime(next_king_ticker, now)
            return ( inter < 0 ) and 0 or inter
        end
    else
        assert(nil, '获取巅峰排行榜的类型不对')
    end 
    return 0
end

-- 获取距离下次刷新的秒数
function CMD.get_remain(category)
    return cs(get_top_secs, category)
end


-- 获取巅峰各排行榜的排名
-- 前台通过后台下发的时间后，倒计时到了后再发请求到后台
function CMD.get_rank_by_category(category)
    return cs(get_top_rank, category)
end


--[[
转化数据格式

from 
{
    [1002] = {
        {uin = 1000001, name = 'lajf', avator = '', score = 300, time = os.time()},
        {uin = 1000021, name = 'lajf', avator = '', score = 20, time = os.time() },
        {uin = 1000301, name = 'lajf', avator = '', score = 10, time = os.time() },
        update = os.time(),
        min_score = 0,
    },
    [1003] = {
        {uin = 1003201, name = 'lajf', avator = '', score = 300, time = os.time()},
        {uin = 1040201, name = 'lajf', avator = '', score = 20, time = os.time() },
        {uin = 1050001, name = 'lajf', avator = '', score = 10, time = os.time() },
        update = os.time(),
        min_score = 0,
    },
}


to
{
    [1002] = {
        {uin = 1000001, name = 'lajf', avator = '', score = 300, time = os.time()},
        {uin = 1000021, name = 'lajf', avator = '', score = 20, time = os.time() },
        {uin = 1000301, name = 'lajf', avator = '', score = 10, time = os.time() },
    },
    [1003] = {
        {uin = 1003201, name = 'lajf', avator = '', score = 300, time = os.time()},
        {uin = 1040201, name = 'lajf', avator = '', score = 20, time = os.time() },
        {uin = 1050001, name = 'lajf', avator = '', score = 10, time = os.time() },
    },
}

]]

local pve_top_num = setting.pve_rank_top
local function convert_score_data(pve_data)
    local new = table.insert
    if pve_data and (type(pve_data) == "table") then
        pve_score_rank = {}
        for k,v in pairs(pve_data) do
            pve_score_rank[k] = {}
            local cp_id = pve_score_rank[k]
            for index = 1, pve_top_num do
                if v[index] then
                    new(cp_id, v[index])
                end
            end

            -- 上榜人数满了后，记录最小值
            local len = #cp_id
            if (len > 0) and (len == pve_top_num) then
                score_rank_min[k] = cp_id[len].score
            end
        end
    else
        logger.debugf("game proxy convert_score_data param is null or not table")
    end    
end


-- 定时去pve排行服务去拉数据
local score_interval = 5 -- 分钟
local score_after = 0 -- 分钟
local function syn_score_list()
    local now = os.time()
    if score_after == 0 then
        local date = os.date('*t', now)
        score_after = score_interval - date.min % score_interval
    else
        score_after = score_interval
    end
    local ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".score_rank_service", "get_rank_list")        
    if ok then
        convert_score_data(ret)
    else
        logger.debugf("game proxy syn_score_list cluster.call failed")
    end
    skynet.timeout(score_after * 6000, syn_score_list)
end

local function get_score_list()
    local tmp = last_score_date
    local now = os.time()
    last_score_date = now
    -- 是否需要去拿score_rank_service缓存数据
    if not next(pve_score_rank) then
        local ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".score_rank_service", "get_rank_list")        
        if ok then
            convert_score_data(ret)
        else
            logger.debugf("game proxy get_score_list cluster.call failed")
        end
    elseif not utils.is_same_day(tmp, now) then
        -- 玩家登录后是隔天了直接从缓存拿，为避免.score_rank_service不工作，再从redis里拿
        local ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".score_rank_service", "get_rank_list")        
        if ok then
            convert_score_data(ret)
        else
            logger.debugf("game proxy cluster.call get_score_list failed when day change, so load from redis")
            ret = skynet.call(redis_conn, "lua", "hgetall_all_value", pve_table, true)
            convert_score_data(ret)
        end
    else
        logger.debugf("game proxy get_score_list do nothing")
    end

end

local function sort_cp(data)
    table.sort(data, function (itema, itemb)
        return (itema.score > itemb.score) or
                    (itema.score == itemb.score and itema.time < itemb.time)
    end)
end

-- 更新关卡id的排行到本地
local function update_cp_rank(id, data)
    if type(data) ~= "table" then
        logger.warningf("update_cp_rank data param is not table for id[%d]  type[%s]", id, type(data))
    end
    pve_score_rank[id] = {} 
    local tmp = pve_score_rank[id]
    for index = 1, pve_top_num do
        if data and data[index] then
            table.insert(tmp, data[index])
        end
    end  

    local len = #tmp
    if len > 0 then
        sort_cp(tmp)
        -- 上榜人数满了后，记录最小值
        if (len == pve_top_num) then
            score_rank_min[id] = tmp[len].score
        end
    end
end

local function notify_score_service(id, uin, name, avator, score)
    -- 即更新全服
    local ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".score_rank_service", "update_pve_rank", id, uin, name, avator, score)
    if not ok then
        logger.debugf("game proxy notify_score_service cluster.call failed for id[%d] uin[%d] name[%s] avator[%s] score[%d]", id, uin, name, avator, score)
    else
        update_cp_rank(id, ret)
    end
end

local function update_score_rank(id, uin, name, avator, score)

    local tmp = pve_score_rank[id]
    local len = 0
    local min = score_rank_min[id]

    if tmp then 
        len = #tmp 
    end

    if len == 0 then
        notify_score_service(id, uin, name, avator, score)
    elseif 0 < len and len < pve_top_num then
        notify_score_service(id, uin, name, avator, score)
    elseif min < score then 
        notify_score_service(id, uin, name, avator, score)        
    else
        -- do nothing
        logger.debugf("game proxy update_score_rank() min[%s] uin[%d] score[%d] id[%d] no need update", tostring(min), uin, score, id)
    end

end

function CMD.pve_update(id, uin, name, avator, score)
    cs(update_score_rank, id, uin, name, avator, score)
end

-- 获取pve积分排名
-- 都是五分钟同步内存数据，所以直接返回缓存数据
function CMD.get_score_rank()
    return pve_score_rank
end

function CMD.update(category, uin, name, value)
    local actvity_id = COMMON.OP_ACT_TYPE
    local ok = true, ret 
    local date = os.date('*t', os.time())
    if category ~= actvity_id.TOP_STAR and date.hour >= 0 and date.hour < 6 then
        logger.debugf("the date no need to save at[%s]", utils.date_to_str(os.time()))
        return
    end
    if category == actvity_id.TOP_STAR and star_min < value then
        ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".star_rank_service", "update_list", uin, name, value)
    elseif category == actvity_id.TOP_WIN and win_min < value then
        ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".win_rank_service", "update_list", uin, name, value)
    elseif category == actvity_id.TOP_KING and king_min < value then
        ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".king_rank_service", "update_list", uin, name, value)
    else
        logger.debugf("game proxy CMD.update category[%d] with value[%d] star_min[%d] win_min[%d] king_min[%d]", category, value, star_min, win_min, king_min)
        return
    end

    if not ok then
        logger.debugf("game proxy update failed for category[%d] for uin[%d]", category, uin)
    end
end

local function star_notify_status( open, config)
    local ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".star_rank_service", "open", open, config)
    if not ok then
        logger.debugf("game proxy star_notify_status() failed for status[%s]", tostring(open)) 
    end
end

local function win_notify_status( open, config )
    local ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".win_rank_service", "open", open, config)
    if not ok then
        logger.debugf("game proxy win_notify_status() failed for status[%s]", tostring(open)) 
    end
end

local function king_notify_status( open, config )
    local ok, ret = xpcall(cluster.call, tracedebug, "commonrank", ".king_rank_service", "open", open, config)
    if not ok then
        logger.debugf("game proxy king_notify_status() failed for status[%s]", tostring(open)) 
    end
end

-- 巅峰星星排行榜活动开启
function CMD.star_open(config)
    assert(config, 'star_open 参数为空')
    if not star_opened then
        star_opened = true
        local now = os.time()
        local date = os.date('*t', now)
        star_inter = config.topConfig.inter or 30
        -- 活动整点开启,不是整点，有可能是服务器重启
        if date.hour == 0 and date.min == 0 then
            _, next_star_ticker= time2_hour(date, now, COMMON.OP_ACT_TYPE.TOP_STAR)
        else
            -- 服务器重启去拿数据
            get_star_list()
        end
        star_notify_status(true, config)
        logger.debugf("game proxy star_open() is open at[%s] update again[%s] ", utils.date_to_str(now), utils.date_to_str(next_star_ticker))
    else
        logger.debug("start_open() already open")
    end
end

-- 巅峰星星排行榜活动关闭
function CMD.star_close()
    if star_opened then
        star_opened = false
        next_star_ticker = 0
        star_notify_status(false, nil)
        logger.debugf("game proxy star_close() closed at[%s]", utils.date_to_str(os.time()))
    else
        logger.debug("start_close() without open")
    end
end


-- 巅峰pvp胜利排行榜活动开启
function CMD.win_open( config )
    assert(config, 'win_open 参数为空')
    if not win_opened then
        win_opened = true
        local now = os.time()
        local date = os.date('*t', now)
        win_inter = config.topConfig.inter or 30
        -- 活动整点开启,不是整点，有可能是服务器重启
        if date.hour == 0 and date.min == 0 then
            _, next_win_ticker = time2_hour(date, now, COMMON.OP_ACT_TYPE.TOP_WIN)
        else
            -- 服务器重启去拿数据
            get_win_list()
        end
        win_notify_status(true, config)
        logger.debugf("game proxy win_open() is open at[%s] update again[%s] ", utils.date_to_str(now), utils.date_to_str(next_win_ticker))
    else
        logger.debug("win_open() already open")
    end
end

-- 巅峰pvp胜利排行榜活动关闭
function CMD.win_close(  )
    if win_opened then
        win_opened = false
        next_win_ticker = 0
        win_notify_status(false, nil)
        logger.debugf("game proxy win_close() closed at[%s]", utils.date_to_str(os.time()))
    else
        logger.debug("win_close() without open")
    end
end

-- 巅峰pvp连胜排行榜活动开启
function CMD.king_open(config)
    assert(config, 'king_open 参数为空')
    if not king_opened then
        king_opened = true
        local now = os.time()
        local date = os.date('*t', now)
        king_inter = config.topConfig.inter or 30
        -- 活动整点开启,不是整点，有可能是服务器重启
        if date.hour == 0 and date.min == 0 then
            _, next_king_ticker = time2_hour(date, now, COMMON.OP_ACT_TYPE.TOP_KING)
        else
            -- 服务器重启去拿数据
            get_king_list()
        end
        king_notify_status(true, config)
        logger.debugf("game proxy king_open() is open at[%s] update again[%s] ", utils.date_to_str(now), utils.date_to_str(next_king_ticker))
    else
        logger.debug("king_open() already open")
    end
end

-- 巅峰pvp连胜排行榜活动开启
function CMD.king_close()
    if king_opened then
        -- 重置
        king_opened = false
        next_king_ticker = 0
        king_notify_status(false)
        logger.debugf("game proxy king_close() closed at[%s]", utils.date_to_str(os.time()))
    else
        logger.debug("king_close() without open")
    end
end


skynet.start(function ()
    
    redis_conn = skynet.queryservice("redis_service")
    syn_score_list()

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("game proxy unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, tracedebug, ...)
        if not ok then
            logger.warningf("game proxy handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)
end)

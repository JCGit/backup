--
-- Author: yibin
-- Date: 2016/03/15
-- Desc: pve积分排行榜
--
local skynet  = require "skynet"
local utils   = require "utils"
local logger  = require "logger"
local setting = require "setting"
local queue    = require "skynet.queue"

local CMD = {}
local redis_conn = nil

-- critical section
local cs = queue()

-- 每个关卡排行榜的buf
--[[
cache存储格式:
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
每个关卡里面的数据不超过15条
--]]
local rank_hash = 'pve_rank'
local rank_cache = {}

local function init_test()
    -- 第一
    for id = 1000, 1200 do
        rank_cache[id] = {min_score = 0}
        local num = 1
        for uin = 1000001, 1000015 do
            table.insert(rank_cache[id],{uin = uin, name = uin, avator = 'http://wx.qlogo.cn/mmopen/7B9rKkMYF5ohVh6AEpQJjcF93HEiaDqhk5ISLVKZiaao52SMnfNicP1sO9X1BJjdc3Sp8sfUicInAXANjfriaThXnVydYCN6cz0Pf/132', score = num, time = os.time()})
            rank_cache[id].update = os.time() - 24 * 60 * 60 - 10
            if rank_cache[id].min_score < num then
                rank_cache[id].min_score = num
            end
            num = num + 1
        end
    end

    -- -- 第二章节
    -- for id = 2010, 2040, 10 do
    --     rank_cache[id] = {}
    --     local num = 1
    --     for uin = 1000101, 1000113 do
    --         table.insert(rank_cache[id],{uin = uin, name = uin, avator = 'img', score = num})
    --         rank_cache[id].update = os.time()
    --         if rank_cache[id].min_score < num then
    --             rank_cache[id].min_score = num
    --         end
    --         num = num + 1
    --     end
    -- end 

    -- -- 第三章节
    -- for id = 3010, 3030, 10 do
    --     rank_cache[id] = {}
    --     local num = 1
    --     for uin = 1000201, 1000213 do
    --         table.insert(rank_cache[id],{uin = uin, name = uin, avator = 'img', score = num})
    --         rank_cache[id].update = os.time()
    --         if rank_cache[id].min_score < num then
    --             rank_cache[id].min_score = num
    --         end
    --         num = num + 1
    --     end
    -- end

end

local function is_garbage(uin)
    if uin then
        local player = skynet.call(redis_conn, "lua", "hget_obj", "account", uin)
        if player then
            local days = utils.getTimeInteral(player.last_day_login_time, os.time())
            if days >= setting.pve_rank_day then
                logger.debugf("score_rank_service->is_garbage() uin[%d] long time to login since[%d]", uin, player.last_day_login_time)
                return true
            else
                return false
            end
        else
            return true
        end
    end
    return true
end

local function sort_cp(data)
    -- 根据分数来排序
    table.sort(data, function (itema, itemb)
        if type(itema) == "table" and type(itemb) == "table" then
            return (itema.score > itemb.score) or
                    (itema.score == itemb.score and itema.time < itemb.time)
        else
            return false
        end
    end)
end

local function copy(item)
    local new = {}
    for k,v in pairs(item) do
        if type(v) == "table" then
            table.insert(new,v)
        else
            new[k] = v
        end
    end
    return new
end


local function update_cache(cp_cache, uin, name, avator, score)
    -- 更新缓存
    local now = os.time()
    local len = #cp_cache
    if len >= setting.pve_rank_len then
        if cp_cache.min_score < score then
            local cached = false
            -- 查找是否有自己
            for k,v in pairs(cp_cache) do
                if type(v) == "table" and v.uin == uin then
                    v.uin = uin
                    v.score = score
                    v.name = name
                    v.avator = avator
                    v.time = now
                    cached = true
                    break
                end
            end

            if not cached then
                local last_one = cp_cache[len]
                last_one.uin = uin
                last_one.score = score
                last_one.name = name
                last_one.avator = avator
                last_one.time = now
            end
            --有可能排名更靠前，重新计算最小分
            sort_cp(cp_cache)
            cp_cache.min_score = cp_cache[#cp_cache].score

        end
    else
        if cp_cache.min_score == 0 or cp_cache.min_score > score then
            cp_cache.min_score = score
            table.insert(cp_cache, {uin = uin, name = name, avator = avator, score = score, time = now})
        else
            -- 默认是新的玩家
            local cached = false
            -- 查找是否有自己
            for k,v in pairs(cp_cache) do
                if type(v) == "table" and v.uin == uin then
                    v.uin = uin
                    v.score = score
                    v.name = name
                    v.avator = avator
                    v.time = now
                    cached = true
                    break
                end
            end
            if not cached then
                table.insert(cp_cache, {uin = uin, name = name, avator = avator, score = score, time = now})
            elseif len == 1 then
                cp_cache.min_score = score
            else

            end

            if #cp_cache ~= 1 then
                --有可能排名更靠前，重新计算最小分
                sort_cp(cp_cache)
                cp_cache.min_score = cp_cache[#cp_cache].score
            end
        end
    end
    cp_cache.update = os.time()
end

-- return true : 数据有隔天  false : 当天
local function remove_garbage(cp_cache)
    local b_garbage = false
    if not utils.is_same_day(cp_cache.update, os.time()) then
        local index = {}
        -- 跨天时去掉超过10天没登录的
        for k,v in pairs(cp_cache) do
            if type(v) == "table" and is_garbage(v.uin) then
                cp_cache[k] = nil
                b_garbage = b_garbage or true
            end
        end
        return true, b_garbage
    end
    return false, b_garbage
end

-- 如果有玩家超过n久没登录需要计算下最小的分数
local function change_min_score(cache)
    for k,v in pairs(cache) do
        if type(v) == "table" and v.score < cache.min_score then
            cache.min_score = v.score
        end
    end
end

local function get_rank_by_id(id)
    assert(id, '获取关卡id的全服排名的时候id为空')
    local change_day = false
    local b_garbage = false
    local cp_cache = rank_cache[id]
    if not cp_cache then
        rank_cache[id] = skynet.call(redis_conn, "lua", "hget_obj", rank_hash, id, true) or {update = os.time(), min_score = 0}
        cp_cache = rank_cache[id]
        change_day, b_garbage = remove_garbage(cp_cache)
    else
        change_day, b_garbage = remove_garbage(cp_cache)
    end


    -- 有删除的数据就需要把排名重新拷贝一份，否则table[1] = nil时，#table取的长度会算上等于nil的
    if b_garbage then
        cp_cache = copy(cp_cache)
        if #cp_cache == 0 then
            cp_cache.min_score = 0
        else
            cp_cache.min_score = cp_cache[#cp_cache].score
        end
    end

    -- 如果有跨天就更新redis
    if change_day then
        cp_cache.update = os.time()
        skynet.call(redis_conn, "lua", "hset_obj", rank_hash, id, cp_cache)
    end
    return cp_cache
end

local function load_data()
    local now = os.time()
    rank_cache = skynet.call(redis_conn, "lua", "hgetall_all_value", rank_hash, true)
    for k, v in pairs(rank_cache) do
        for k2,v2 in pairs(v) do
            if type(v2) == "table" then
                if not v2.time then
                    v2.time = now - 12 * 60 * 60 + k2
                end
            end
        end
    end    
end

local function update_id_list(cp_id, uin,name,avator,score)
    local change_day = false
    local b_garbage = false
    local cp_cache = rank_cache[cp_id]
    if not cp_cache then
        rank_cache[cp_id] = skynet.call(redis_conn, "lua", "hget_obj", rank_hash, cp_id, true) or { update = os.time(), min_score = 0}
        cp_cache = rank_cache[cp_id]
        change_day, b_garbage = remove_garbage(cp_cache)
    else
        change_day, b_garbage = remove_garbage(cp_cache)
    end
    
    sort_cp(cp_cache)

    if b_garbage then
        cp_cache = copy(cp_cache)
        if #cp_cache == 0 then
            cp_cache.min_score = 0
        else
            cp_cache.min_score = cp_cache[#cp_cache].score
        end
    end

    update_cache(cp_cache, uin, name, avator, score)
    -- 如果有跨天就更新redis
    if change_day or #cp_cache >= setting.pve_rank_len then
        cp_cache.update = os.time()
        skynet.call(redis_conn, "lua", "hset_obj", rank_hash, cp_id, cp_cache)
    end
    return cp_cache
end

-- 刷新排行榜数据
function CMD.update_pve_rank(cp_id, uin, name, avator, score)
    --是否有此关卡的缓存
    --    没有,
    --       从redis里面加载一下  
    --           没有，
    --               redis里同样没有，说明还没人玩过此关卡
    --               刷新缓存 
    --           有，
    --               redis里面有，放入缓存
    --               如果跨天了就做跨天的逻辑处理
    --               更新缓存
    --    有，
    --        如果跨天了就做跨天的逻辑处理
    --        刷新缓存
     
    -- 用cs保护起来 
    return cs(update_id_list, cp_id, uin, name, avator, score)
end


-- 获取每一关卡的排行榜里的第一名
-- param 类型 : table
-- 参数含义   : 所有关卡的id
-- return :
--
--
--
function CMD.get_pve_rank_list(id_list)
    assert(id_list, "获取关卡排行榜列表的参数为空")
    local rank_data = {}
    for k,v in pairs(id_list) do
        rank_data[v] = {}
        local rank = get_rank_by_id(v)
        for index = 1, setting.pve_rank_top do
            if rank[index] then
                rank_data[v][index] = rank[index]
            end
        end
    end
    return rank_data
end

local function rank_list_cs()
    if not next(rank_cache) then
        rank_cache = skynet.call(redis_conn, "lua", "hgetall_all_value", rank_hash, true)
        return rank_cache
    else
        return rank_cache
    end
    return {}
end

-- 通过game_proxy请求列表
function CMD.get_rank_list()
    return cs(rank_list_cs)
end


-- 缓冲buffer更新到redis
--
function CMD.exit()
    logger.debug("score_rank_service exit")
    for k,v in pairs(rank_cache) do
        skynet.call(redis_conn, "lua", "hset_obj", rank_hash, k, v)
    end
end

-- 方便查询问题flush某一关卡的排行榜数据到redis
function CMD.flush(id)
    assert(id, 'id值为空')
    if rank_cache[id] then
       skynet.call(redis_conn, "lua", "hset_obj", rank_hash, id, rank_cache[id]) 
    end
end


-- 获取某一关卡的排行榜
function CMD.get_pve_rank_by_id(cp_id)
    local cp_cache = rank_cache[cp_id]
    if cp_cache then
        local rank = {}
        for index = 1, setting.pve_rank_top do
            if cp_cache[index] then
                table.insert(rank, cp_cache[index])
            end
        end
        return rank
    else
        return {}
    end
end

skynet.start(function ( ... )
    -- body
    local traceback = debug.traceback
    redis_conn = skynet.queryservice("redis_service")
    load_data() -- 重启加载redis里数据
    -- init_test()

    skynet.dispatch("lua", function(_, src, command, ...)
        local func = CMD[command]
        if not func then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(func, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s src:%d", command, ret, src)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)

end)
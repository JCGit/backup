local skynet = require "skynet"
local logger = require "logger"
local utils  = require "utils"
local COMMON = require "common"


local redis_conn 

-- 数组下标，务必要递增！！
local array_index_new = {
    begin_at  = 1,       -- 开始时间
    stars     = 2,       -- 战斗获得的星星数
    bits      = 3,       -- 为true fals的bit位集合
    score     = 4,       -- 此关卡的得分
    category  = 5,       -- 关卡所属章节
}

--[[
    id          = cp_id, -- 关卡id
    stars       = 0,     -- 战斗获得的星星数
    reward      = false, -- 是否通关奖励
    full_reward = false, -- 是否满星奖励
    start_time  = os.time(),
    win         = 0,    -- 胜利次数
    fail        = 0,    -- 失败次数
    num         = 0,    -- 进入关卡的次数
    reset       = 0,    -- 重置关卡的次数
    exit        = 0,    -- 退出关卡的次数
    new_flag    = true, -- 是否新关卡,创建的时候默认是新的，pve结算后就为false
    score       = 0,    -- 此关卡的得分
    battle_time = 0,    -- 战斗时长
]]

local array_index_old = {
    id      = 1,
    stars   = 2,
    reward  = 3,
    full_reward = 4,
    start_time  = 5,
    win     = 6,
    fail    = 7,
    num     = 8,
    reset   = 9,
    exit    = 10,
    new_flag = 11,
    score    = 12,
    battle_time = 13,
}

local pass_bit = 0x01 -- 通关奖励
local full_bit = 0x02 -- 完美通关奖励
local new_bit  = 0x04 -- 是否第一次玩 
local win_bit  = 0x08 -- 关卡首次胜利

local function init_chapter()
    return {
        max_cp = 0,   -- 最大关卡号
        lock = true,    -- 是否解锁
        stars = 0,       -- 章节所得到的星星数
        chest = {},      -- 所获得的宝箱
    }
end

local function init_cp_meta(category)
    return {
        0,  -- 开始时间
        0,          -- 战斗获得的星星数
        0,          -- 为true bit位集合，4代表第3位为1即是新的关卡
        0,          -- 此关卡的得分
        category,   -- 所属章节
    }
end

local function init_cp()
    return {
        cp_list = {},
        cp_category = {}
    }
end


local function trans_data(new, old)
    local cp_list = new.cp_list
    for chapter, data in pairs(old) do
        if type(chapter) == "number" then
            local cp_category = new.cp_category[chapter]
            if not cp_category then
                new.cp_category[chapter] = init_chapter()
                cp_category = new.cp_category[chapter]
                cp_category.max_cp = data.max_cp
                cp_category.lock = data.lock
                cp_category.stars = data.stars
                cp_category.chest = data.chest
            end
            for cp_id, meta in pairs(data) do
                if type(cp_id) == "number" then
                    local cp_data = cp_list[cp_id]
                    if not cp_data then
                        cp_list[cp_id] = init_cp_meta(chapter)
                        cp_data = cp_list[cp_id]
                    end
                    cp_data[array_index_new.begin_at] =  meta.start_time    
                    cp_data[array_index_new.category]  = chapter
                    if meta.new_flag then
                        cp_data[array_index_new.bits] = utils.set_bit(cp_data[array_index_new.bits], new_bit)
                    end

                    if meta.full_reward then
                        cp_data[array_index_new.bits] = utils.set_bit(cp_data[array_index_new.bits], full_bit)
                    end

                    if meta.reward then
                        cp_data[array_index_new.bits] = utils.set_bit(cp_data[array_index_new.bits], pass_bit)
                    end
                    
                    if meta.win > 0 then
                        cp_data[array_index_new.bits] = utils.set_bit(cp_data[array_index_new.bits], win_bit)
                    end

                    if meta.stars then
                        cp_data[array_index_new.stars] = meta.stars
                    end

                    if meta.score then
                        cp_data[array_index_new.score] = meta.score
                    end

                    if chapter == 99 and meta.showed then
                        cp_data.showed = true
                    end
                end
            end
        else
            new[chapter] = data
        end
    end
end


skynet.start(function()
    redis_conn = skynet.queryservice("redis_service")

    local being_uin = 1000001
    logger.debugf("tran data begin at : %d", os.time())
    local max_uin = skynet.call(redis_conn, "lua" ,"get", "account_max_id")
	print("max_uin is ", max_uin)
    max_uin = tonumber(max_uin)
    -- local max_uin = 1000001
    local count = 0
    for uin = being_uin, max_uin do
        local old_data = skynet.call(redis_conn, "lua", "hget_obj", "check_points", uin)
        if old_data then
            local new = init_cp()
            trans_data(new, old_data)
            skynet.call(redis_conn, "lua", "hset_obj", "check_points", uin, new)
            count = count + 1
        end
    end

    logger.debugf("tran data end at : %d with count : %d", os.time(), count)

end)

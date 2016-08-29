
local skynet = require "skynet"
local logger = require "logger"
local utils  = require "utils"
local COMMON = require "common"


local redis_conn 


-- hash to array
--[[
local array_index = {
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
]]
local function copy_table(prek, new, old)
    for k,v in pairs(old) do
        if type(v) == "table" then
            new[k] = {}
            local new_value = new[k]
            copy_table(k, new_value, v)
        else
            if prek and type(prek) == "number" and (prek > 100) then
                if k == 1 then
                    new.id = v
                elseif k == 2 then
                    new.stars = v
                elseif k == 3 then
                    new.reward = v
                elseif k == 4 then
                    new.full_reward = v
                elseif k == 5 then
                    new.start_time = v
                elseif k == 6 then
                    new.win = v
                elseif k == 7 then
                    new.fail = v
                elseif k == 8 then
                    new.num = v
                elseif k == 9 then
                    new.reset = v
                elseif k == 10 then
                    new.exit = v
                elseif k == 11 then
                    new.new_flag = v
                elseif k == 12 then
                    new.score = v
                elseif k == 13 then
                    new.battle_time = v
                else
                    new[k] = v
                end
            else
                new[k] = v
            end
        end

    end
end

local function b_change(data)
    if data[99] and data[99][99010] and not data[99][99010].exit
        or #data[1][1010] > 0 then
        return true
    else
        return false
    end
end

skynet.start(function()
    redis_conn = skynet.queryservice("redis_service")

    logger.debugf("tran data begin at : %d", os.time())
    local begin_uin = 1019000
    local max_uin = skynet.call(redis_conn, "lua" ,"get", "account_max_id")
    local count = 0
    for uin = begin_uin, max_uin do
        local old_data = skynet.call(redis_conn, "lua", "hget_obj", "check_points", uin, true)
        if old_data and b_change(old_data)then
            local new = {}
            copy_table(nil, new, old_data)
            skynet.call(redis_conn, "lua", "hset_obj", "check_points", uin, new)
            count = count + 1
        end
    end

    logger.debugf("tran data end at : %d with count : %d", os.time(), count)

end)

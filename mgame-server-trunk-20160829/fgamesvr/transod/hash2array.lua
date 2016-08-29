
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
                if k == "id" then
                    new[1] = v
                elseif k == "stars" then
                    new[2] = v
                elseif k == "reward" then
                    new[3] = v
                elseif k == "full_reward" then
                    new[4] = v
                elseif k == "start_time" then
                    new[5] = v
                elseif k == "win" then
                    new[6] = v
                elseif k == "fail" then
                    new[7] = v
                elseif k == "num" then
                    new[8] = v
                elseif k == "reset" then
                    new[9] = v
                elseif k == "exit" then
                    new[10] = v
                elseif k == "new_flag" then
                    new[11] = v
                elseif k == "score" then
                    new[12] = v
                elseif k == "battle_time" then
                    new[13] = v
                else
                    new[k] = v
                end
            else
                new[k] = v
            end
        end

    end
end

skynet.start(function()
    redis_conn = skynet.queryservice("redis_service")


    local being_uin = 10000001
    logger.debugf("tran data begin at : %d", os.time())
    local max_uin = skynet.call(redis_conn, "lua" ,"get", "account_max_id")
    max_uin = tonumber(max_uin)
    -- local max_uin = 1000001
    local count = 0
    for uin = being_uin, max_uin do
        local old_data = skynet.call(redis_conn, "lua", "hget_obj", "check_points", uin, true)
        if old_data then
            local new = {}
            copy_table(nil, new, old_data)
            skynet.call(redis_conn, "lua", "hset_obj", "check_points", uin, new)
            count = count + 1
        end
    end

    logger.debugf("tran data end at : %d with count : %d", os.time(), count)

end)

--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2016/3/28
-- Time: 19:47
-- Desc: 查询月充值总额记录
-- 注意：  1 hgetall_obj 这个函数反序列话的时候 需要用json.decode
--         2 如果需要查其他平台 要替换_lenovo
--         3 这段代码目前只支持查月充值记录 如有其他需求 可根据情况自己修改代码
--

local skynet = require "skynet"
require "skynet.manager" -- import skynet.name

local server_config = require "server_config"
local utils = require "utils"

skynet.start(function()
    print("Server Start")

    skynet.uniqueservice("mysql_service")
    local redis_service = skynet.uniqueservice("redis_service")
    local data = skynet.call(redis_service, "lua", "hgetall_obj", "recharge_order")
    -- hgetall_obj 反序列化的时候需要用json.decode
    local tmp = {}
    for i, v in pairs(data) do
        if type(v) == "table" then
            if v.Status == "success" then
                if string.match(v.OrderId, "_lenovo") then -- 如果是其他平台 需要替换"_lenovo"
                local date = os.date("*t", v.Time)
                local year = date.year
                local month = date.month
                local money = v.Money or v.money or 0
                if not tmp[year] = then
                    tmp[year] = {}
                end

                if not tmp[year][month] then
                    tmp[year][month] = 0
                end
                tmp[year][month] = tmp[year][month] + money
                end
            end
        end
    end
    utils.var_dump(tmp)
    skynet.exit()
end)


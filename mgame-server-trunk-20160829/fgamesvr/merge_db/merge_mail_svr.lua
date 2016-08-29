
local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local redis_dv = require "redis_dv"

local db_qq
local db_wx

local max_global_mail_id
local wx_uin_offset

local function start_merge()
    local tmp = db_wx:hgetall("usermail")
    local len = #tmp / 2
    local index = 0
    for i = 1, len do
        local k = tonumber(tmp[i * 2 - 1])
        k = k + wx_uin_offset
        local data = skynet.unpack(tmp[i * 2])

        if data then
            data.mail_num = 0
            data.max_global_id = max_global_mail_id
            data.content = {}
        end

        db_qq:hset("usermail", k, skynet.packstring(data))
    end
    print("merge db with usermail success ")
    collectgarbage("collect")

end

skynet.start(function()
    logger.trace("merge db with usermail start ...")
    db_qq , db_wx = redis_dv.init()
    wx_uin_offset = tonumber(db_qq:get("account_max_id")) - 10000000

    max_global_mail_id = tonumber(db_qq:get("max_global_mail_id"))

    start_merge()
end)
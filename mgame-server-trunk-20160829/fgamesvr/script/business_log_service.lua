local skynet = require "skynet"
local cluster = require "cluster"
require "skynet.manager"

local logger = require "logger"
local COMMON = require "common"
local utils = require "utils"
local busilog_setting = require "busilog_setting"
local traceback = debug.traceback

skynet.register_protocol {
    name = "business_logger",
    id = 0,
    pack = function (...)
        local t = {...}
        for i,v in ipairs(t) do
            t[i] = tostring(v)
        end
        return table.concat(t," ")
    end,
    unpack = skynet.unpack,
}

local function send_to_logserver(logtype, msg)
    cluster.call("lognode", ".log_receiver_service", "revc", logtype, msg)
end

local function get_online_time(account)
    if os.time() >= account.login_time then
        return account.online_time + os.time() - account.login_time
    end
    return  account.online_time
end

local CMD = {}
CMD.init_log = {}
CMD.counter = 0

local function set_counter()
    CMD.counter = CMD.counter + 1
end


function CMD.init(obj)
    if type(obj) == "table" then
        for key,value in pairs(obj) do
            skynet.launch("busilogger",value)
            CMD.init_log[key] = 1
        end
    end
end

function CMD.log(logtype, msg)
    if not CMD.init_log[logtype] then
        logger.warningf("cannot find bussiness by type=", logtype)
        return
    end

    local ok, ret = xpcall(send_to_logserver, traceback, logtype, msg)
    if not ok then
        skynet.send(busilog_setting.log_init_name[logtype], "business_logger", msg)
    end
end


-- 货币日志
-- 输出格式   [[ 1.用户ID| 2.时间| 3.计数器| 4.货币类型| 5.改变值| 6.剩余值| 7.原因 ]]
function CMD.log_coin(uin, type, change_value, cur_value, reason)
    set_counter()
    CMD.log(busilog_setting.log_index.LOG_COIN, string.format("%s|%s|%s|%s|%s|%s|%s", uin, os.time(), CMD.counter, type, change_value, cur_value, reason))
end


-- 物品日志
-- 输出格式   [[ 1.用户ID| 2.时间| 3.计数器| 4.物品ID| 5.数量| 6.原因 ]]
function CMD.log_item(uin, item_id, count, reason)
    set_counter()
    CMD.log(busilog_setting.log_index.LOG_ITEM, string.format("%s|%s|%s|%s|%s|%s", uin, os.time(), CMD.counter, item_id, count, reason))
end


-- 登录日志
-- 输出格式   [[ 1.用户ID| 2.时间| 3.计数器| 4.类型| 5.登录时间| 6.登出时间 ]]
function CMD.log_login(account, type)
    set_counter()
    local tmp = 0
    local tmp1 = account.newbie_flag
    if type == COMMON.player_operation.player_login_out then
        tmp =  account.login_out_time -  account.login_time
    elseif type ==  COMMON.player_operation.player_create then
        tmp1 =  account.openid
    end
    CMD.log(busilog_setting.log_index.LOG_LOGIN, string.format("%s|%s|%s|%s|%s|%s|%s|%s|%s|%s", account.uin, os.time(), CMD.counter, account.platform, type, tmp, account.diamond, account.level, tmp1, account.name))
end


-- 战斗日志
-- 输出格式   [[ 1.用户ID| 2.时间| 3.计数器| 4.类型| 5子类型| ... ]]
-- 参数说明   [[ type 分为 pvp,pve , subtype 分为 begin,end
function CMD.log_battle(uin, type, subtype, ...)
    if (type ~= "pvp" and type ~= "pve") or (subtype ~= "begin" and subtype ~= "end") then
        logger.warningf("log_battle params error")
        return
    end

    local t = {...}
    for i=1,#t do
        if not t[i] then
            t[i] = 0
        end
    end
    local str = table.concat(t,"|")
    set_counter()
    CMD.log(busilog_setting.log_index.LOG_BATTLE, string.format("%s|%s|%s|%s|%s|%s", uin, os.time(), CMD.counter, type, subtype, str))
   -- skynet.send(busilog_service, "lua", "log_battle", uin, "pvp", "begin" ... )
end


-- 在线日志
-- 输出格式   [[ 1.时间| 2.计数器| 3.平台| 4.序号| 5.在线数 ]]
function CMD.log_online(serial, platform_type, online_count)
    set_counter()
    math.randomseed(skynet.now())
    local number = math.random(1, 10000000)
    CMD.log(busilog_setting.log_index.LOG_ONLINE, string.format("%s|%s|%s|%s|%s|%s", os.time(), CMD.counter, platform_type, serial, online_count, number))
end


-- 充值日志
-- 输出格式   [[ 1.用户ID| 2.时间| 3.计数器| 4.订单id| 5.shopId| 6.金额| 7.钻石数 ]]
function CMD.log_recharge(account, order_id, shop_id, money, add_diamond, pay_amount, pay_currency)
    pay_amount = pay_amount or 0
    pay_currency = pay_currency or 0
    local ontlineTime = get_online_time(account)
    set_counter()
    CMD.log(busilog_setting.log_index.LOG_RECHARGE, string.format("%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s", 
        account.uin, os.time() ,CMD.counter, account.platform, money, order_id, shop_id, add_diamond, ontlineTime, account.level, account.name, pay_amount, pay_currency))
end

-- exp日志
-- 输出格式   [[ 1.用户ID| 2.时间| 3.计数器| 4.是否升级| 5.等级| 6.经验| 7.增加经验| 8.在线时间| 9.原因 ]]
function CMD.log_exp(account, upFlag, addExp, src)
    set_counter()
    local count = get_online_time(account)
    CMD.log(busilog_setting.log_index.LOG_EXP, string.format("%s|%s|%s|%s|%s|%s|%s|%s|%s", account.uin, os.time() ,CMD.counter, upFlag, account.level, account.exp, addExp, count, src))
end


--混合日志
--输出格式  [[ 1.类型| 2.时间| 3.计数器| 4.类型对应数据 ]]
function CMD.log_misc(type, ...)
    if not type then
        logger.warningf("type must not nil")
        return
    end

    local t = {...}
    for i=1,#t do
        if not t[i] then
            t[i] = 0
        end
    end
    local str = table.concat(t,"|")
    set_counter()
    CMD.log(busilog_setting.log_index.LOG_MISC, string.format("%s|%s|%s|%s", type, os.time() ,CMD.counter, str))
end


skynet.start(function()

    local res,value = xpcall(cluster.query, traceback, "lognode", "log_receiver_service")
    if not res then
        logger.warningf("connect to log_receiver_server failed:%s", value)
    end

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
        end
        return
    end)
end)
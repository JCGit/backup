-- mysql_service.lua
-- mysql服务

local skynet = require "skynet"
local mysql = require "mysql"
local logger = require "logger"
local utils = require "utils"
local server_config = require "server_config"
local setting = require "setting"

local db
local CMD = {}
local traceback = debug.traceback

-- lua obj 缓存队列
local queue = utils.List.new()

local function exec_one_sql(queue)
    local obj = utils.List.popFront(queue)
    if obj then
      local table_name = obj.table_name
      local key = obj.key
      local value = skynet.packstring(obj.value)
      local sql = "replace into " .. table_name .. " values('" .. key .. "', '" .. value .. "');";
      mysql.query(db, sql)
    end
end

local pop_max_count = 500
local function exec_queue()
    if queue.first > queue.last then return end -- 队列中没有元素
    if utils.List.getSize(queue) >= pop_max_count then
        for i = 1, pop_max_count do
            exec_one_sql(queue)
        end
    else
        while queue.first <= queue.last do
            exec_one_sql(queue)
        end
    end
end

-- 定时将队列中的数据写入到mysql
local last_exec_time = 0
local EXEC_INTERVAL_TIME = setting.game_setting.EXEC_INTERVAL_TIME
local function check_exec_queue()
    skynet.timeout(EXEC_INTERVAL_TIME, check_exec_queue)
    local t = last_exec_time + EXEC_INTERVAL_TIME - skynet.now()
    if t <= 0 then
        exec_queue()
        last_exec_time = skynet.now()
    end
end

-- 数据库保活
local last_check_time = 0
local mysql_keepalive_time = setting.game_setting.mysql_keepalive_time
local function keep_alive()
    skynet.timeout(mysql_keepalive_time, keep_alive)
    local t  = last_check_time + mysql_keepalive_time - skynet.now()
    if t <= 0 then
        mysql.query(db, "select 1")
        last_check_time = skynet.now()
    end
end

-- 存对象到缓存队列
function CMD.insert_queue(obj)
    utils.List.pushBack(queue, obj)
end

-- 用于进程退出
function CMD.flush_queue()
    exec_queue()
end

function CMD.truncate_tab(tab)
    local sql = "truncate table " .. tab
    logger.tracef("CMD.query %s", sql)
    mysql.query(db, sql)
end

function CMD.query(table_name, key)
    local sql = string.format("select * from %s where id = '%s'", table_name, key)
    local res = mysql.query(db, sql)
    if #res == 0 then
        return nil
    end

    local obj = nil
    local ret
    for k, v in pairs(res[1]) do
        if k ~= "id" then
            ret, obj = pcall(skynet.unpack, v)
            assert(ret, string.format("%s => tn:%s  key:%s  v:%s", obj, table_name, key, v))
            break;
        end
    end
    return obj
end

function CMD.remove_obj(table_name, key)
    local sql = "delete from " .. table_name .. " where id = '" .. key .. "'"
    logger.tracef("CMD.delete %s", sql)
    mysql.query(db, sql)
end

skynet.start(function()
    logger.trace("Mysql start...")
    db = mysql.connect(server_config.mysql_conf)
    if not db then
        logger.trace("failed to connect mysql service")
        return
    end
    db:query("set names utf8")
    logger.trace("success to connect to mysql service")

    last_exec_time = skynet.now()
    check_exec_queue()
    last_check_time = skynet.now()
    keep_alive()

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
        skynet.retpack(ret)
    end)
end)

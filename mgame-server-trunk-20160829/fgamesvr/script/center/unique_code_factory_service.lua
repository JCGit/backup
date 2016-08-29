
--唯一码生成工厂服务
--可以启动多个服务来对应不同的唯一码标识和唯一码样式
--
--auth : diandian
--date : 2016-01-07

local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local queue = require "skynet.queue"

local cs = queue()

local redis_service

local char_mapping_tmp = {
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "w",
    "x",
    "y",
    "z",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
}

local name
local redis_name
local factory_info

local max_num_table
local add_sum_table

local function get_max_num_table()
    if max_num_table == nil then
        max_num_table = {}
        for i = 1, factory_info.code_count do
            table.insert(max_num_table, #factory_info.char_mapping - 1)
        end
    end
    return max_num_table
end

local function get_add_sum_table()
    if add_sum_table == nil then
        add_sum_table = {}
        for i = 2, factory_info.code_count do
            table.insert(add_sum_table, math.floor(#factory_info.char_mapping / 1.5))
        end
    end
    return add_sum_table
end

local function get_char_str(num_table)
    local char_t = {}
    for i = 1, #num_table do
        table.insert(char_t,factory_info.char_mapping[num_table[i] + 1])
    end
    return table.concat(char_t)
end

local function sava_info()
    if not factory_info then
        skynet.call(redis_service, "lua", "hdel_obj", "unique_code_factory", redis_name)
    else
        skynet.call(redis_service, "lua", "hset_obj", "unique_code_factory", redis_name, factory_info)
    end
end

local function create_code(num_t, err)
    if factory_info == nil then 
        table.insert(err, "code factory service is not init!")
        return
    end

    local add_sum_table = get_add_sum_table()
    factory_info.cur_num_table = utils.num_table_sum(factory_info.cur_num_table, add_sum_table, #factory_info.char_mapping)

    if utils.num_table_compare(factory_info.cur_num_table, get_max_num_table()) == 1 then
        factory_info.cur_create_page_table = utils.num_table_sum(factory_info.cur_create_page_table, {1,}, #factory_info.char_mapping)
        if utils.num_table_compare(factory_info.cur_create_page_table, add_sum_table) ~= -1 then
            sava_info()
            table.insert(err, "code factory is full")
            return
        end

        factory_info.cur_num_table = utils.num_table_sum(factory_info.init_num_table, factory_info.cur_create_page_table, #factory_info.char_mapping)
    end
    sava_info()
    for i,v in pairs(factory_info.cur_num_table) do
        table.insert(num_t, v)
    end
end

local CMD = {}

function CMD.create_code()
    local num_t = {}
    local err = {}

    --按照规则生成，所以函数时序不能乱，用queue队列保护起来
    cs(create_code, num_t, err)

    return get_char_str(num_t), table.concat(err)
end

function CMD.init(name_, code_len_, char_mapping_)
    name = name_
    redis_name = name .. "_code_info"

    factory_info = skynet.call(redis_service, "lua", "hget_obj", "unique_code_factory", redis_name)
    if factory_info == nil then
        factory_info = {}
        factory_info.code_count = code_len_

        --第一次初始化的时后随机打乱字符模板，这样会让码显得更随机
        --并且也不容找出码的生成规律来猜出后面的码
        local char_mapping = not char_mapping_ and char_mapping_tmp or char_mapping_
        factory_info.char_mapping = utils.copyTable(char_mapping)
        utils.upset_array(factory_info.char_mapping)

        factory_info.cur_num_table = {}
        factory_info.init_num_table = {}
        factory_info.cur_create_page_table = {1,}
        for i = 1, factory_info.code_count do
            local num = i < #factory_info.char_mapping and i -1 or #factory_info.char_mapping - 1 
            table.insert(factory_info.cur_num_table, num)
            table.insert(factory_info.init_num_table, num)
        end
    end

    char_mapping_tmp = nil
    char_mapping_ = nil
end

local traceback = debug.traceback
skynet.start(function()
    
    redis_service = skynet.uniqueservice("redis_service")
    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s) id : %d", command, skynet.self())
            return skynet.ret()
        end

        local ok, ret, err = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s  id : %d", command, ret, skynet.self())
            return skynet.ret()
        end
        skynet.retpack(ret, err)
    end)
end)

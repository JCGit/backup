-- utils.lua
-- 帮助函数库

local utils = {}

local skynet = require 'skynet'

local assert       = assert
local getmetatable = getmetatable
local load         = load
local pairs        = pairs
local setmetatable = setmetatable
local tostring     = tostring
local type         = type

local string_format = string.format
local string_sub    = string.sub
local string_gsub   = string.gsub
local string_find   = string.find
local string_match  = string.match

local table_insert  = table.insert
local table_concat  = table.concat

local math_floor  = math.floor
local math_random = math.random

local os_date = os.date
local os_time = os.time

-- 从array中返回随机n个不重复的元素
function utils.get_random_sublist(array, n)
    assert(n >= 1 and n <= #array)
    local r = {}
    local t = {}
    local len = # array
    for i = 1, n do
        while true do
            local ri = math_random(len)
            if not t[ri] then
                t[ri] = true
                table_insert(r, array[ri])
                break
            end
        end
    end
    return r
end

-- elem from Haskell in Lua
function utils.elem(list, e)
    for _, v in ipairs(list) do
        if v == e then
            return true
        end
    end
    return false
end

--序列化
function utils.serialize_table(obj, lvl)
    local lua = {}
    local t = type(obj)
    if t == "number" then
        table_insert(lua, obj)
    elseif t == "boolean" then
        table_insert(lua, tostring(obj))
    elseif t == "string" then
        table_insert(lua, string_format("%q", obj))
    elseif t == "table" then
        lvl = lvl or 0
        local lvls = ('  '):rep(lvl)
        local lvls2 = ('  '):rep(lvl + 1)
        table_insert(lua, "{\n")
        for k, v in pairs(obj) do
            table_insert(lua, lvls2)
            table_insert(lua, "[")
            table_insert(lua, utils.serialize_table(k,lvl+1))
            table_insert(lua, "]=")
            table_insert(lua, utils.serialize_table(v,lvl+1))
            table_insert(lua, ",\n")
        end
        local metatable = getmetatable(obj)
        if metatable ~= nil and type(metatable.__index) == "table" then
            for k, v in pairs(metatable.__index) do
                table_insert(lua, "[")
                table_insert(lua, utils.serialize_table(k, lvl + 1))
                table_insert(lua, "]=")
                table_insert(lua, utils.serialize_table(v, lvl + 1))
                table_insert(lua, ",\n")
            end
        end
        table_insert(lua, lvls)
        table_insert(lua, "}")
    elseif t == "nil" then
        return nil
    else
        print("can not serialize a " .. t .. " type.")
    end
    return table_concat(lua, "")
end

--反序列化
function utils.unserialize_table(lua)
    local t = type(lua)
    if t == "nil" or lua == "" then
        return nil
    elseif t == "number" or t == "string" or t == "boolean" then
        lua = tostring(lua)
    else
        print("can not unserialize a " .. t .. " type.")
    end
    lua = "return " .. lua
    local func = load(lua)
    if func == nil then
        return nil
    end
    return func()
end

function utils.var_dump(data, max_level, prefix)
    if type(prefix) ~= "string" then
        prefix = ""
    end
    if type(data) ~= "table" then
        print(prefix .. tostring(data))
    else
        print(data)
        if max_level ~= 0 then
            local prefix_next = prefix .. "    "
            print(prefix .. "{")
            for k, v in pairs(data) do
                io.stdout:write(prefix_next .. k .. " = ")
                if type(v) ~= "table" or (type(max_level) == "number" and max_level <= 1) then
                    print(v, ",")
                else
                    if max_level == nil then
                        utils.var_dump(v, nil, prefix_next)
                    else
                        utils.var_dump(v, max_level - 1, prefix_next)
                    end
                end
            end
            print(prefix .. "}")
        end
    end
end

function utils.split(str, split)
    local list = {}
    local pos = 1
    if string_find("", split, 1) then -- this would result in endless loops
        error("split matches empty string!")
    end
    while true do
        local first, last = string_find(str, split, pos)
        if first then
            table_insert(list, string_sub(str, pos, first - 1))
            pos = last + 1
        else
            table_insert(list, string_sub(str, pos))
            break
        end
    end
    return list
end

function utils.join(tb, join_string)
    return table_concat(tb, join_string)
end

-- 写文件
function utils.write_file(file_name, string)
    local f = assert(io.open(file_name, 'w'))
    f:write(string)
    f:close()
end

-- 浅拷贝
function utils.clone(t, nometa)
    local result = {}
    if not nometa then
        setmetatable(result, getmetatable(t))
    end
    for k, v in pairs(t) do
        result[k] = v
    end
    return result
end

-- 深拷贝
function utils.copy(t, nometa)
    local result = {}

    if not nometa then
        setmetatable(result, getmetatable(t))
    end

    for k, v in pairs(t) do
        if type(v) == "table" then
            result[k] = utils.copy(v, nometa)
        else
            result[k] = v
        end
    end
    return result
end

--获取日期hour级之间的大小
function utils.compareTimeHour(sDate, eDate)
    if type(sDate) == "number" then
        local hour = sDate
        sDate = {}
        sDate.hour = hour
        sDate.min = 0
    end

    if type(eDate) == "number" then
        local hour = eDate
        eDate = {}
        eDate.hour = hour
        eDate.min = 0
    end

    if (sDate.hour < eDate.hour) then
        return -1
    elseif (sDate.hour > eDate.hour) then
        return 1
    else
        if (sDate.min < eDate.min) then
            return -1
        elseif (sDate.min > eDate.min) then
            return 1
        else
            return 0
        end
    end
end

--获取日期之间的间隔(按照定义的刷新点计算)
--同一天为0
function utils.getTimeInteral(sTime, eTime, re_point)
    local seconds = eTime - sTime
    local days = math.ceil(seconds / 86400)
    local sDate = os.date("*t", sTime)
    local eDate = os.date("*t", eTime)

    if (sDate.day == eDate.day) and 
        (sDate.month == eDate.month) and 
        (sDate.year == eDate.year)  then  --跨天通过day判断就行了
        days = 0
    end
    return days
end

-- 获取当天凌晨的时间
function utils.getSameDayBeginTime(time)
    if not time then time = os.time() end
    local date = os_date("*t", time)
    date.hour = 0
    date.min = 0
    date.sec = 0
    return math_floor(os_time(date))
end

--是否是同一天
function utils.is_same_day(timestamp1, timestamp2)
    local old = os_date("*t", timestamp1)
    local now = os_date("*t", timestamp2 or os.time())
    return now.year == old.year and now.month == old.month and now.day == old.day
end

-- 是否在minutes分钟之内, 默认为2
function utils.is_in_minutes(timestamp1, timestamp2, minutes)
    local old = os_date("*t", timestamp1)
    local now = os_date("*t", timestamp2 or os.time())
    return now.year == old.year
        and now.month == old.month
        and now.day == old.day
        and now.hour == old.hour
        and math.abs(now.min - old.min) <= (minutes or 2)
end

utils.is_today = utils.is_same_day

-- XXX 正式版记得改回来
--utils.is_today = utils.is_in_minutes

function utils.IsInsegment(seg, now)
    local start_hour, start_min = string_match(seg.startTime, "^([%d]+):([%d]+)$")
    local end_hour, end_min = string_match(seg.endTime, "^([%d]+):([%d]+)$")
    local start_time = utils.getSameDayBeginTime() + start_hour * 3600 + start_min * 60
    local end_time = utils.getSameDayBeginTime() + end_hour * 3600 + end_min * 60

    return now >= start_time and now <= end_time
end

function utils.IsInSegments(segments, now)
    for i, v in ipairs(segments) do
        if utils.IsInsegment(v, now) then
            return true
        end
    end
    return false
end


-- 获取N分钟后的时间戳
function utils.get_next_time(cha)
    local date = os.date("*t", os.time())
    date.sec = 0
    return math_floor(os.time(date)) + cha * 60
end

--获取yyyyMMddHHmmss格式的时间
function utils.get_time(time) -- string2datetime
    time       = string_gsub(time,"#","") -- 策划表此类时间会在前面加个#以保持时间正常 不能去掉
    local date = os_date("*t", os_time())
    date.year  = string_sub(time,1,4)
    date.month = string_sub(time,5,6)
    date.day   = string_sub(time,7,8)
    date.hour  = string_sub(time,9,10)
    date.min   = string_sub(time,11,12)
    date.sec   = string_sub(time,13,14)
    return math_floor(os_time(date))
end

function utils.datetime2string(datetime)
    local date = os.date("*t", datetime)
    local arr = {}
    table.insert(arr, "#")
    local sYear = tostring(date.year)
    table.insert(arr, sYear)
    local sMonth = (date.month < 10) and ("0" .. date.month) or tostring(date.month)
    table.insert(arr, sMonth)
    local sDay = (date.day < 10) and ("0" .. date.day) or tostring(date.day)
    table.insert(arr, sDay)
    local sHour = (date.hour < 10) and ("0" .. date.hour) or tostring(date.hour)
    table.insert(arr, sHour)
    local sMin = (date.min < 10) and ("0" .. date.min) or tostring(date.min)
    table.insert(arr, sMin)
    local sSec = (date.sec < 10) and ("0" .. date.sec) or tostring(date.sec)
    table.insert(arr, sSec)
    return table_concat(arr, "")
end

--获取yyyyMMdd格式的时间
function utils.get_yyyyMMdd_time()
    return os.date('%Y%m%d', os.time())
end

function utils.date_to_str(seconds)
    return os.date("%Y-%m-%d %H:%M:%S", seconds)
end

function utils.table_remove(the_table, value, one_flag)
  if(type(the_table) ~= "table") then
    return nil
  end
  local l_value = nil
  local key_mody = 0
  local key_table = {}
  for k,v in pairs(the_table) do
    if v == value then
      table.insert(key_table, k)
      l_value = v
      if one_flag then
        break
      end
    end
  end
  local cur_key
  for k,v in pairs(key_table) do
    cur_key = v
    if(type(cur_key) == "number") then
      if #the_table + key_mody >= cur_key then
        cur_key = cur_key - key_mody
        table.remove(the_table, cur_key)
        key_mody = key_mody + 1
      else
        the_table[cur_key] = nil
      end
    else
      the_table[cur_key] = nil
    end
  end
  return l_value
end

function utils.time_format(format_str, time)
  local t1,t2 = math.modf(time)
  t2 = math.modf(t2 * 1000)
  return os.date(format_str,t1) .. "-" .. t2 .. "ms"
end

-- 获取当天时间在某个小时某分钟某秒的时间秒数
function utils.set_huor_min_sec(hour, min, sec)
    local date = os.date("*t", os.time())
    date.hour = hour
    date.min = min
    date.sec = sec
    return math_floor(os.time(date))
end


local function parseargs (s)
    local arg = {}
    string.gsub(s, "(%w+)=([\"'])(.-)%2", function (w, _, a)
        arg[w] = a
    end)
    return arg
end

function utils.collect(s)
    local stack = {}
    local top = {}
    table.insert(stack, top)
    local ni,c,label,xarg, empty
    local i, j = 1, 1
    while true do
        ni,j,c,label,xarg, empty = string.find(s, "<(%/?)([%w:]+)(.-)(%/?)>", i)
        if not ni then break end
        local text = string.sub(s, i, ni-1)
        if not string.find(text, "^%s*$") then
            table.insert(top, text)
        end
        if empty == "/" then  -- empty element tag
            table.insert(top, {label=label, xarg=parseargs(xarg), empty=1})
        elseif c == "" then   -- start tag
            top = {label=label, xarg=parseargs(xarg)}
            table.insert(stack, top)   -- new level
        else  -- end tag
            local toclose = table.remove(stack)  -- remove top
            top = stack[#stack]
            if #stack < 1 then
                error("nothing to close with "..label)
            end
            if toclose.label ~= label then
                error("trying to close "..toclose.label.." with "..label)
            end
            table.insert(top, toclose)
        end
        i = j+1
    end
    local text = string.sub(s, i)
    if not string.find(text, "^%s*$") then
        table.insert(stack[#stack], text)
    end
    if #stack > 1 then
        error("unclosed "..stack[#stack].label)
    end
    return stack[1]
end

-- 集群节点对应列表
function utils.query_nodename(server_name)
    local node_name
    if string.match(server_name, "logind") then
        local index = string.match(server_name, "%d")
        node_name = "loginnode" .. index
    elseif string.match(server_name, "gamed") then
        local index = string.match(server_name, "%d")
        node_name = "gamenode" .. index
    end
    return node_name
end

-- 跨服对战agent地址区分, 原clustername.lua中的 clustername.agent_add_value["gamenode1"] = 100000000
function utils.get_agent_add_value(nodename)
    local value
    if string.match(nodename, "gamenode") then
        local index = string.match(nodename, "%d")
        value = index * 100000000
    end
    return value
end

-- lua双端队列
utils.List = {}

function utils.List.new()
    return {first = 0, last = -1}
end

function utils.List.pushFront(list, value)
    local first = list.first - 1
    list.first = first
    list[first] = value
end

function utils.List.pushBack(list, value)
    local last = list.last + 1
    list.last = last
    list[last] = value
end

function utils.List.popFront(list)
    local first = list.first
    if first > list.last then
        print("List is empty")
        return nil
    end
    local value = list[first]
    list[first] = nil
    list.first = first + 1
    return value
end

function utils.List.popBack(list)
    local last = list.last
    if list.first > last then
        print("List is empty")
        return nil
    end
    local value = list[last]
    list[last] = nil
    list.last = last - 1
    return value
end

function utils.List.getSize(list)
  return list.last - list.first + 1
end

function utils.List.listIter(list)
    local index = list.first - 1
    local n = list.last

    return function ()
        index = index + 1
        if index <= n then  return list[index] end
    end
end

-- 判断是否为闰年
function utils.isLeapYear(year)
    assert(type(year) == 'number')
    return (year % 4== 0 and year % 100 ~= 0)
        or (year % 100 ==0 and year % 400 == 0)
end

-- 计算两天日期之间的天数差别
-- 可以正确处理闰年的情况
function utils.calcDaysBetween(time2, time1)
    local test_use_minute = false -- 测试的时候，以分钟为单位，打开此标志
    if test_use_minute then
        return math_floor(os.difftime(time2, time1) / 60)
    else
        local d2, d1 = os.date('*t', time2), os.date('*t', time1)
        local t2 = os.time { year = d2.year, month = d2.month, day = d2.day, }
        local t1 = os.time { year = d1.year, month = d1.month, day = d1.day, }
        return os.difftime(t2, t1) / (60 * 60 * 24)
    end
end

function utils.urlencode(s)
    return (string.gsub(s, "([^A-Za-z0-9_])", function(c)
        return string.format("%%%02X", string.byte(c))
    end))
end

function utils.urldecode(s)
    s = string.gsub(s,"+"," ")  
    s,n = string.gsub(s,"%%(%x%x)",function(c)  
        return string.char(tonumber(c,16))  
    end)  
    return s
end

function utils.upset_array(array)
    local len = #array
    for i, v in ipairs(array) do
        local w = math.random(1, len)
        array[w], array[i] = array[i], array[w]
    end
end

function utils.decodeURI(s)
    s = string.gsub(s, '%%(%x%x)', function(h) return string.char(tonumber(h, 16)) end)
    return s
end



function utils.copyTable(data)
    local tab = {}
    for k, v in pairs(data or {}) do
        if type(v) ~= "table" then
            tab[k] = v
        else
            tab[k] = copyTab(v)
        end
    end
    return tab
end

--返回两个元素为数字类型的数组型table按照指定进制相加后的新table
--radix 进制数 默认：10
function utils.num_table_sum(t1, t2, radix)
    radix = radix == nil and 10 or radix
    
    local count_1 = #t1
    local count_2 = #t2

    local max_count = count_1 >= count_2 and count_1 or count_2

    local t_sum = {}
    local cur_index_1 = 0
    local cur_index_2 = 0
    local cur_num_1 = 0

    local cur_num_2 = 0
    local cur_step = 0
    local cur_sum_num = 0
    for i = 1, max_count do
        cur_index_1 = count_1 - i + 1
        cur_index_2 = count_2 - i + 1
    
        cur_num_1 = cur_index_1 <= 0 and 0 or (not t1[cur_index_1] and 0 or tonumber(t1[cur_index_1]))
        cur_num_2 = cur_index_2 <= 0 and 0 or (not t2[cur_index_2] and 0 or tonumber(t2[cur_index_2]))

        cur_sum_num = cur_num_1 + cur_num_2 + cur_step
        cur_step = cur_sum_num >= radix and 1 or 0

        if cur_step == 1 then
            table.insert(t_sum, 1, cur_sum_num - radix)
        else
            table.insert(t_sum, 1, cur_sum_num)
        end
    end

    if cur_step == 1 then
        table.insert(t_sum, 1, 1)
    end

    return t_sum
end

--比较两个元素为数字类型的数组大小
--返回 -1，0，1
function utils.num_table_compare(t1, t2)

    local count_1 = #t1
    local count_2 = #t2

    local max_count = count_1 >= count_2 and count_1 or count_2

    local cur_index_1 = 0
    local cur_index_2 = 0
    for i = 1, max_count do
        cur_index_1 = count_1 - max_count + i
        cur_index_2 = count_2 - max_count + i


        local cur_num_1 = cur_index_1 <= 0 and 0 or (not t1[cur_index_1] and 0 or tonumber(t1[cur_index_1]))
        local cur_num_2 = cur_index_2 <= 0 and 0 or (not t2[cur_index_2] and 0 or tonumber(t2[cur_index_2]))

        if cur_num_1 ~= cur_num_2 then
            return cur_num_1 > cur_num_2 and 1 or -1
        end
    end
    return 0
end

--二分查找
function utils.binary_search(start_pos, end_pos, hash_value, range_map)
    if not range_map and not next(range_map) then  return -1 end
    if start_pos > end_pos then return -1 end

    local gamed_range_map = {}
    local mid = math.floor((start_pos + end_pos)/2)
    if range_map[mid] == hash_value then
        return mid
    elseif range_map[mid] > hash_value then
        end_pos = mid - 1
        return utils.binary_search(start_pos, end_pos, hash_value, range_map)
    else
        start_pos = mid + 1
        return utils.binary_search(start_pos, end_pos, hash_value, range_map)
    end
end

--二分区间查找算法  --Notice: 这个目前只能用于openid哈希匹配选服，使用这个函数切记确认使用场景
function utils.binary_region_search(start_pos, end_pos, hash_value, range_map)
    if not range_map and not next(range_map) then  return -1 end
    if start_pos > end_pos then return start_pos end  --start_pos指向的就是hash_value落地的区间, 超出最多值时，返回的是 end_pos + 1

    local gamed_range_map = {}
    local mid = math.floor((start_pos + end_pos)/2)
    if range_map[mid] == hash_value then
        return mid
    elseif range_map[mid] > hash_value then
        end_pos = mid - 1
        return utils.binary_region_search(start_pos, end_pos, hash_value, range_map)
    else
        start_pos = mid + 1
        return utils.binary_region_search(start_pos, end_pos, hash_value, range_map)
    end
end

--生成奖励
function utils.make_reward(rewards, count)
    local items = {}

    local function add_item(id, count)
        if not items[id] then
            items[id] = {
                itemId = id,
                count = 0,
            }
        end
        items[id].count = items[id].count + count
    end

    local total_rate = 0
    for _, info in pairs(rewards.rand_rewards) do
        total_rate = total_rate + info.random_rate 
    end

    for i = 1, count do
        for _, cfg in pairs(rewards.fixed_rewards) do
            --判断数量是否随机
            if cfg.max_count == 0 and cfg.max_count == cfg.count then
                add_item(cfg.item_id, cfg.count)
            else
                local fix_rcount = math.random(cfg.count, cfg.max_count)
                add_item(cfg.item_id, fix_rcount)
            end
        end

        if total_rate > 0 then
            local rand_num = math.random(1, total_rate)
            for _, cfg in pairs(rewards.rand_rewards) do
                if rand_num<= cfg.random_rate then
                    if cfg.count > 0 then
                        if cfg.max_count == 0 or cfg.max_count == cfg.count then
                            add_item(cfg.item_id, cfg.count)
                        else
                            local ran_rcount = math.random(cfg.count, cfg.max_count)
                            add_item(cfg.item_id, ran_rcount)
                        end
                    end
                    break
                else
                    rand_num = rand_num - cfg.random_rate
                end
            end
        end
    end
    return items
end

--合并奖励r2到r1
function utils.combine_reward(r1, r2)
    local function add_item(id, count)
        if not r1[id] then
            r1[id] = {
                itemId = id,
                count = 0,
            }
        end
        r1[id].count = r1[id].count + count
    end
    for id, item in pairs(r2) do
        add_item(id, item.count)
    end
end

--合并物品数据
function utils.combine_items(r1, r2, clear_empty)
    local function add_item(id, count)
        if not r1[id] then
            r1[id] = {
                itemId = id,
                count = 0,
            }
        end
        r1[id].count = r1[id].count + count
        
        if clear_empty then
            if r1[id].count < 1 then
                r1[id] = nil
            end
        end
    end
    for id, item in pairs(r2) do
        add_item(id, item.count)
    end
end

function utils.itemlist_to_map(itemlist)
    local t = {}
    for _, itemmap in ipairs(itemlist) do
        for id, item in pairs(itemmap) do
            if not t[item.itemId] then
                t[item.itemId] = {
                    itemId = id,
                    count = 0,
                }
            end
            t[item.itemId].count = t[item.itemId].count + item.count
        end
    end
    
    return t
end

--生成奖励
--固定奖励 和 随机奖励 分开返回
function utils.make_reward_v2(rewards, count)
    local fix_items = {}
    local ran_items = {}

    local function add_fix_items(id, count)
        if not fix_items[id] then
            fix_items[id] = {
                itemId = id,
                count = 0,
            }
        end
        fix_items[id].count = fix_items[id].count + count
    end

    local function add_ran_items(id, count)
        if not ran_items[id] then
            ran_items[id] = {
                itemId = id,
                count = 0,
            }
        end
        ran_items[id].count = ran_items[id].count + count
    end

    local total_rate = 0
    for _, info in pairs(rewards.rand_rewards) do
        total_rate = total_rate + info.random_rate 
    end

    for i = 1, count do
        --固定奖励
        for _, cfg in pairs(rewards.fixed_rewards) do
            --判断数量是否随机
            if cfg.max_count == 0 and cfg.max_count == cfg.count then
                add_fix_items(cfg.item_id, cfg.count)
            else
                local fix_rcount = math.random(cfg.count, cfg.max_count)
                add_fix_items(cfg.item_id, fix_rcount)
            end
        end

        --随机奖励
        if total_rate > 0 then
            local rand_num = math.random(1, total_rate)
            for _, cfg in pairs(rewards.rand_rewards) do
                if rand_num<= cfg.random_rate then
                    if cfg.count > 0 then
                        if cfg.max_count == 0 or cfg.max_count == cfg.count then
                            add_ran_items(cfg.item_id, cfg.count)
                        else
                            local ran_rcount = math.random(cfg.count, cfg.max_count)
                            add_ran_items(cfg.item_id, ran_rcount)
                        end
                    end
                    break
                else
                    rand_num = rand_num - cfg.random_rate
                end
            end
        end
    end
    return fix_items, ran_items
end
function utils.pack_hash_uin_key(hash)
    if type(hash) ~= "table" then
        return {}
    end

    local r = {}
    for k,v in pairs(hash) do
        table.insert(r, v.uin)
        table.insert(r, skynet.packstring(v))
    end
    return table.unpack(r)
end

-- src 源数字
-- hex 是二进制
-- 位操作需要5.3以上版本才支持
function utils.set_bit( src, hex )
    if type(src) ~= "number" then
        assert(nil, 'utils.set_bit is not number type')
    end    
    
    src = src | hex
    return src    
end

function utils.zero_bit( src, hex)
    if type(src) ~= "number" then
        assert(nil, 'utils.zero_bit is not number type')
    end

    src = src & (~hex)
    return src
end

function utils.bit_set( src, hex)
    if type(src) ~= "number" then
        assert(nil, 'utils.bit_set is not number type')
    end

    return (src & hex) == hex
end

function utils.trim(s)
    return (string.gsub(s, "^%s*(.-)%s*$", "%1"))
end

function utils.getPvpTotalCount(info)
    return info.pvp_win_count + info.pvp_lose_count + info.pvp_draw_count
end

function utils.getPvpWinRate(info)
    local totoal_count = utils.getPvpTotalCount(info)
    return (totoal_count == 0) and 0 or (info.pvp_win_count/totoal_count*100)
end


-- 检查key及类型是否一致
--[[
{
    "key" = type(10),
    "key2" = type('')
}
--]]

function utils.checkKeys(keys, src)
    for key,ty in pairs(keys) do
        -- for key
        if not src[key] then
            skynet.error(string.format("checkKeys(...) not found key[%s]", key))
            return false
        end

        -- for value type
        local from = type(src[key])
        if from ~= ty then
            skynet.error(string.format("checkKeys(...) value want[%s] but[%s] for key[%s]", ty, from, key))
            return false
        end
    end
    return true
end

function utils.checkRange(value, min, max)
    if value <= min and value >= max then
        skynet.error(string.format("值超过了范围 min[%d] max[%d] valule[%d]", min, max, value))
        return false
    end
    return true
end

return utils

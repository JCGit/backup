local skynet    = require "skynet"
local socket    = require 'socket'
local sharedata = require "sharedata"

local logger        = require "logger"
local utils         = require "utils"
local COMMON        = require "common"
-- local player        = require "player" 

local function initnotice(table_b)
    local table_a = {}
    local max_win_value = 0
    for _, v in pairs(table_b) do
        if v.user == 2 then
            if not table_a[v.msg_type] then
                table_a[v.msg_type] = {}
            end
            if (v.msg_type == COMMON.notice.STREAK_WIN) then
                if (v.value > max_win_value) then
                    table_a[v.msg_type].max_win = v
                    max_win_value = v.value
                end
            end
            table.insert(table_a[v.msg_type], v)
        end
    end
    return table_a
end

local function initMissionTable(old_table)
   local new_table = {}
   for k,v in pairs(old_table) do
      v.mission_id = tonumber(v.num)
      v.num = nil
      v.type = tonumber(v.type)
      v.hard_level = tonumber(v.hard_level)
      v.require = tonumber(v.require)
      v.value = tonumber(v.value)
      v.gift = tonumber(v.gift)
      new_table[v.mission_id] = v
   end
   return new_table
end

local function restructPvpMap(stat)
    local tmp = {}
    for k, v in pairs(stat) do
        local model = v.model
        local type = v.type
        local mapId = v.mapid
        if not tmp[model] then
            tmp[model] = {}
        end

        if not tmp[model][type] then
            tmp[model][type] = {}
        end

        tmp[model][type][mapId] = v
    end

    return tmp
end

local function initPVPActivityTime(stat)
  local new_table = {}
  local tnow = os.time()
  for k,v in pairs(stat) do
     local data = {}
     data.ActivityID = v.ActivityID
     data.DetailID = v.DetailID
     data.begindate = v.begindate
     data.enddate = v.enddate
     data.type = v.type

     if not data.begindate or #data.begindate == 0 then
        data.begindate = 0
     else
        data.begindate = utils.get_time(data.begindate)
     end

     if not data.enddate or #data.enddate == 0 then
        data.enddate = 0
     else
        data.enddate = utils.get_time(data.enddate)
     end

     if data.ActivityID == 11 then -- 圣诞活动
         data.loseBonusId = v.loseBonusId
         data.winBonusId  = v.winBonusId
     end

     new_table[data.ActivityID]  = data
  end

  return new_table
end

local function initActivityTime(stat)
    local new_table = {}
    local tnow = os.time()
    for k,v in pairs(stat) do
        local data = {}
        data.ActivityID = v.ActivityID
        data.begindate = v.begindate
        data.enddate = v.enddate
        data.loseBonusId = v.loseBonusId
        data.winBonusId  = v.winBonusId
        data.dropEndTime = v.dropEndTime

         if not data.begindate or #data.begindate == 0 then
            data.begindate = 0
         else
            data.begindate = utils.get_time(data.begindate)
         end

         if not data.enddate or #data.enddate == 0 then
            data.enddate = 0
         else
            data.enddate = utils.get_time(data.enddate)
         end

        if not data.dropEndTime or #data.dropEndTime == 0 then
            data.dropEndTime = 0
        else
            data.dropEndTime = utils.get_time(data.dropEndTime)
        end

         new_table[data.ActivityID]  = data
    end
    return new_table
end

local function initExchange(stat)
    -- 转化时间格式
    for k,v in pairs(stat) do
        if not v.beginAt or #v.beginAt == 0 then
            v.beginAt = 0
        else
            v.beginAt = utils.get_time(v.beginAt)
        end

        if not v.endAt or #v.endAt == 0 then
            v.endAt = 0
        else
            v.endAt = utils.get_time(v.endAt)
        end
    end
    return stat
end

local function initActivityDetail(stat)
   local new_table = {}
   for k,v in pairs(stat) do
     if not new_table[v.DetailID] then
       new_table[v.DetailID] = {}
     end
     table.insert(new_table[v.DetailID], v)
   end
   return new_table
end

local function initShopData(stat)
   local new_table = {}
   for k,v in pairs(stat) do
     if not v.start_time or #v.start_time == 0 then
        v.start_time = 0
     else
        v.start_time = utils.get_time(v.start_time)
     end

     if not v.end_time or #v.end_time == 0 then
        v.end_time = 0
     else
        v.end_time = utils.get_time(v.end_time)
     end
     new_table[k] = v
   end
   return new_table
end

local function restructBonus(stat)
    local tmp = {}
    for k, v in pairs(stat) do
        local BonusId = v.BonusId
        if not tmp[BonusId] then
            tmp[BonusId] = {}
        end

        local type = v.Type
        local item
        if type == COMMON.BONUS_TYPE.ITEM or type == COMMON.BONUS_TYPE.STUFF then -- 物品
            item = {
                type    = v.Type,
                itemId  = v.itemId,
                count   = v.count or 1,
                isrand  = v.isrand,
                rate    = v.rate,
            }
        else  -- 属性
            item = {
                type    = v.Type,
                value   = v.value,
                isrand  = v.isrand,
                rate    = v.rate,
            }
        end
        table.insert(tmp[BonusId], item)
    end
    return tmp
end

local function initGlyphsData(stat)
    local pattern = '(%d+)%*(%d+)'
    local gmatch = string.gmatch
    for k, v in pairs(stat) do
        assert(#v.stuff > 0, 'error in glyphs config')
        assert(type(v.needLevel) == 'number' and v.needLevel >= 1)

        local t = {}
        for stuffID, count in gmatch(v.stuff, pattern) do
            t[tonumber(stuffID)] = tonumber(count)
        end
        v.stuffInfo = t
    end
    return stat
end

local function initPropAttr(stat)
    for k, v in pairs(stat) do
        if v.startTime and v.startTime ~= "" then
            v.startTime = utils.get_time(v.startTime)
        else
            v.startTime = nil
        end

        if v.endTime and v.endTime ~= "" then
            v.endTime = utils.get_time(v.endTime)
        else
            v.endTime = nil
        end
    end
    return stat
end

--等级 最大等级独立出来
local function initLevelData(stat)
    local new_table = {}
    local max_level = 0
    for k, v in pairs(stat) do
        new_table[tonumber(k)] = v
        max_level = max_level + 1
    end
    new_table["max_level"] = max_level
    return new_table
end

local function initSysMail(stat)
    for k, v in pairs(stat) do
        v.plugin = string.gsub(v.plugin, ";", ",")
        v.outdate = tonumber(v.outdate) or 0
        if v.outdate == 0 then
            v.outdate = 3153600
        end
    end
    return stat
end

local function initDailyTaskConfig(stat)
    local r = {}
    for k, v in pairs(stat) do
        local dct = r[v.type]
        if not dct then
            r[v.type] = {}
            dct = r[v.type]
        end
        assert(v.type >= 1 and v.type <= 9)
        assert(v.para >= 1)
        table.insert(dct, {
            taskType      = v.type,
            totalProgress = v.para,
            giftID        = v.reward,
        })
    end
    return r
end

-- 每日任务宝箱奖励配置
-- {
--      [1] = {
--          maxw = sum(weights)
--          {1551, 1000}, {}, ...}, -- 奖励ID，权重
--      [2] = {...},
--      [3] = {...},
-- }
local function initDailyTaskAward(stat)
    local r = {}
    for k, v in pairs(stat) do
        local award_list = {}
        local sum = 0
        for i = 1, 13 do
            local gid = v['giftid' .. i]
            local wgt = v['weight' .. i]
            if type(gid) == 'number' and type(wgt) == 'number' and wgt > 0 then
                sum = sum + wgt
                table.insert(award_list, {gid, sum})
            end
        end
        assert(utils.elem({1, 2, 3}, k))
        award_list.maxw = sum
        r[k] = award_list
    end
    return r
end

-- PVP无道具场奖励
-- {
--      [1] = {win = 315, lose = 318}, -- key是段位 取值：{1,2,3,4}
-- }
local function initPvpAward(stat)
    local r = {}
    for _, v in pairs(stat) do
        -- 段位有效取值范围
        assert(utils.elem({1,2,3,4}, v.grade))
        --assert(is_valid_gift_id(v.winReward, v.LoseReward))
        r[v.grade] = { win = v.winReward, lose = v.LoseReward }
    end
    return r
end

-- 精灵主动技能
local function initAsSkill(stat)
    local r = {}
    for k, v in pairs(stat) do
        if type(k) == 'number' then
            assert(r[k] == nil, '技能ID重复')
            r[k] = {
                 Name         = v.Name,     -- 技能名 调试用
                 CoolDownTime = v.CoolTime, -- CD时间
            }
        end
    end
    return r
end

-- 精灵
local function initAsAssistant(stat)
    local r = {}
    for k, v in pairs(stat) do
        if type(k) == 'number'
            and type(v.Quality) == 'number' and v.Quality >= 1
            and type(v.ActiveSkills) == 'number' then
            r[k] = {
                 TypeID        = k,
                 Level         = 1,
                 Quality       = v.Quality,
                 ActiveSkills  = {v.ActiveSkills},
                 PassiveSkills = {},
                 ExpTime       = 0, -- XXX 策划设置有效期的方式
                 PieceId       = v.PieceId,
                 Show           = v.Show,
            }
        end
    end
    return r
end

local function initInsitituteInfo(stat)
    local dat = {}
    for k, v in pairs(stat) do
        local info = {}
        info.cost = tonumber(v.cost)
        info.id = tonumber(v.id)
        info.reward_count = tonumber(v.reward_count)
        info.be_reward = v.be_reward
        info.random_reward = v.random_reward 
        info.must_reward = v.must_reward and utils.split(v.must_reward, "|") or {}
        info.must_reward_count = v.must_reward and utils.split(v.must_reward_count, "|") or {}
        info.free_count = tonumber(v.free_count)
        info.free_cool_time = tonumber(v.free_cool_time)
        dat[k] = info
    end
    return dat
end

local function initItemsInfo(stat)
    local dat = {}
    for k, v in pairs(stat) do
        local info = {}
        info.id = tonumber(v.id)
        info.name = v.name
        info.type = tonumber(v.type)
        info.b_type = math.floor(info.type / 10000)
        info.s_type = math.floor(info.type % 10000)
        info.only_one = tonumber(v.only_one) == 1 and true or false
        info.notice = tonumber(v.notice) == 1 and true or false
        info.type_one = tonumber(v.type_one) and tonumber(v.type_one) or 0
        info.start_time = tonumber(v.start_time)
        info.end_time = tonumber(v.end_time)
        info.be_used = tonumber(v.be_used) == 1 and true or false
        info.use_extra = v.use_extra
        info.is_client_bag = tonumber(v.is_client_bag) == 1 and true or false
        info.property_type = tonumber(v.property_type)
        info.is_virtual = tonumber(v.is_virtual) == 1 and true or false
        info.virtual_real = tonumber(v.virtual_real)
        info.virtual_type = tonumber(v.virtual_type)
        dat[k] = info
    end
    return dat
end

local function initRewardInfo(stat)
    local dat = {}
    for k, v in pairs(stat) do
        local info = {}
        info.id = tonumber(v.id)
        info.group = tonumber(v.group)
        info.item_id = tonumber(v.item_id)
        info.count = tonumber(v.count) and tonumber(v.count) or 0
        info.max_count = tonumber(v.max_count) and tonumber(v.max_count) or 0
        info.be_random = tonumber(v.be_random) == 1 and true or false
        info.random_rate = tonumber(v.random_rate)
        if not dat[info.group] then
            dat[info.group] = {}
            dat[info.group].rand_rewards = {}
            dat[info.group].fixed_rewards = {}
        end

        local group_data = dat[info.group]

        if info.be_random then
            group_data.rand_rewards[info.id] = info
        else
            if not info.id then
                utils.var_dump(info)
            end
            group_data.fixed_rewards[info.id] = info
        end
    end

    return dat
end

local function initConst( stat )
    local r = {}
    for k,v in pairs(stat) do
        local item = {}
        item.value = v.value
        r[k] = item
    end
    return r
end

local function initTopRank(stat)
    local r = {}
    for k,v in pairs(stat) do
        local item = {}
        item.mailTitle = v.mailTitle
        item.mailContent = v.mailContent
        item.giftId = v.giftId
        item.realItem1 = v.realItem1
        item.realItem2 = v.realItem2
        item.realItem3 = v.realItem3
        r[v.rankType..'_'..v.rank] = item
    end
    return r
end

local function initSkin(stat)
    local r = {}
    for k,v in pairs(stat) do
        local item = {}
        item.cost = v.cost
        item.forever = v.forever
        item.itemid = v.itemid
        item.free = v.free
        r[v.id] = item
    end
    return r
end

local function initTopRankTime( stat )
    local r = {}
    for k,v in pairs(stat) do
        local category = tonumber(v.category)
        if not r[category] then
            r[category] = {}
        end
        local tc = r[category]
        if not v.beginAt or #v.beginAt == 0 then
            v.beginAt = 0
        else
            v.beginAt = utils.get_time(v.beginAt)
        end

        if not v.endAt or #v.endAt == 0 then
            v.endAt = 0
        else
            v.endAt = utils.get_time(v.endAt)
        end

        table.insert(tc, {beginAt = v.beginAt, endAt = v.endAt})
    end
    return r
end

local function split(string, split)
    local list = {}
    local pos = 1
    -- this would result in endless loops
    if string.find("", split, 1) then
        error("split matches empty string!")
    end
    while true do
        local first, last = string.find(string, split, pos)
        if first then
            table.insert(list, string.sub(string, pos, first - 1))
            pos = last + 1
        else
            table.insert(list, string.sub(string, pos))
            break
        end
    end
    return list
end

local function getRowContent(file)
    local content
    local check = false
    local count = 0
    while true do
        local t = file:read()
        if not t then
            if count == 0 then
                check = true
            end
            break
        end

        -- 正确处理在Linux平台上， \r\n 换行符的问题
        if t:sub(#t) == '\r' then t = t:sub(1, -2) end

        if not content then
            content = t
        else
            content = content..t
        end

        local i = 1
        while true do
            local index = string.find(t, "\"", i)
            if not index then break end
            i = index + 1
            count = count + 1
        end

        if count % 2 == 0 then
            check = true
            break
        end
    end

    assert(check)

    return content and (string.gsub(content, " ", ""))
end

local function loadCsvFile(filePath)
    -- 读取文件
    logger.debugf("cur file is %s", tostring(filePath))

    local alls = {}
    local file = assert(io.open(filePath, "r"))
    while true do
        local line = getRowContent(file)
        if not line then
            file:close()
            break
        end
        table.insert(alls, line)
    end
    -- 从第3行开始保存（第一行是标题，第二行是后面的行才是内容）
    -- 用二维数组保存：arr[ID][属性标题字符串]
    local titles = split(alls[1], ",")
    local arrs = {}
    for i = 3, #alls, 1 do
        -- 一行中，每一列的内容,第一位为ID
        local content = split(alls[i], ",")
        local ID = tonumber(content[1])
        if ID then
            arrs[ID] = {}
            -- 以标题作为索引，保存每一列的内容，取值的时候这样取：arrs[1].Title
            for j = 1, #titles, 1 do
                local value = content[j]
                if not value then
                    value = 0
                end

                if string.match(value, "^[%d]+$") then
                    arrs[ID][titles[j]] = tonumber(value)
                else
                    arrs[ID][titles[j]] = value
                end
            end
        end
    end
    return arrs
end

local function todo()
    local xml = require "luaXml"
    local lfs = require "lfs"
    -- 地图数据
    local maps = {}
    local map_path = lfs.currentdir() .. "\\..\\map"
    -- 解析单个地图文件
    function parser_map(filename)
        local xfile = xml.load(filename)
        local map = {}
        local objectgroup = xfile:find("objectgroup")
        local len = #objectgroup
        while len > 0 do
            for i = 1, 1, 1 do
                local object = objectgroup[len]

                if object[1][1].name ~= "BuildId" then
                    len = len - 1
                    break
                end

                local obj = {
                    id      = tonumber(object[1][1].value),
                    type    = object.name,
                    x       = tonumber(object.x),
                    y       = tonumber(object.y),
                    width   = tonumber(object.width),
                    height  = tonumber(object.height),
                }

                map[obj.id] = obj

                len = len - 1
            end
        end

        local type, index = string.match(filename, "\\pvp_([%d]+)_([%d]+).tmx$")
        if not type then
            print("error filename " .. filename)
            return
        end
        if not index then
            print("error filename " .. filename)
            return
        end

        type = tonumber(type)
        index = tonumber(index)
        if not maps[type] then
            maps[type] = {}
        end
        maps[type][index] = map
    end

    function parser_plist(filename)
        local xfile = xml.load(filename)
        local sex, index = string.match(filename, "\\([%w]+)_([%w]+)_name.plist$")

        if not sex then
            print("error filename " .. filename)
            return
        end
        if not index then
            print("error filename " .. filename)
            return
        end

        local dict = xfile:find("dict")
        local len = #dict
        while len > 0 do
            local order = dict[len-1][1]
            local name  = dict[len][1]
            table.insert(names[sex][index], name)
            len = len - 2
        end
    end

    function load_map_data(path)
        for file_name in lfs.dir(path) do
            if file_name ~= "." and file_name ~= ".." then
                local full_file_name = path .. '\\' .. file_name
                print(full_file_name .. " parse start!!!")
                if string.match(full_file_name, "\\pvp_([%d]+)_([%d]+).tmx$") then
                    parser_map(full_file_name)
                elseif string.match(full_file_name, ".+.plist$") then
                    parser_plist(full_file_name)
                end
                print(full_file_name .. " parse end!!!")
            end
        end
        return maps
    end
end

local function initPvpAiData(stat)
    local temp = {} -- score => [aidata]
    for k, v in pairs(stat) do
        local score = tonumber(v.score)
        if not temp[score] then
            temp[score] = {}
        end

        table.insert(temp[score], v)
    end

    local r = {} -- [{score=>[aidata]}]
    for k, v in pairs(temp) do
        local item = {
            score   = k,
            data    = v,
        }
        table.insert(r, item)
    end
    table.sort(r, function(a, b) return a.score < b.score end)
    return r
end

local function initProduceName(stat)
    local r = {}
    for k,v in pairs(stat) do
        local item = {}
        item.id = v.id
        item.name = v.name
        item.abandon_notice = tonumber(v.abandon_notice) == 1 and true or false
        r[v.id] = item
    end
    return r
end

local plan = COMMON.plan
local init_data_map = {
    -- type                         function
    [plan.Notice]               =   initnotice,
    [plan.Names]                =   function(stat) return stat end,
    [plan.Mission]              =   initMissionTable,
    [plan.PvpMap]               =   restructPvpMap,
    [plan.SHOP_DATA]            =   initShopData,
    [plan.Bonus]                =   restructBonus,
    [plan.Glyphs]               =   initGlyphsData,
    [plan.LEVEL]                =   initLevelData,
    [plan.PropAttr]             =   initPropAttr,
    [plan.Activity_Time]        =   initActivityTime,
    [plan.Sys_Mail]             =   initSysMail,
    [plan.ExchangeData]         =   initExchange,
    [plan.DAILY_TASK_CONFIG]    =   initDailyTaskConfig,
    [plan.DAILY_TASK_AWARD]     =   initDailyTaskAward,
    [plan.PVP_NOPROP_AWARD]     =   initPvpAward,
    [plan.Institute]            =   initInsitituteInfo,
    [plan.Items]                =   initItemsInfo,
    [plan.Reward]               =   initRewardInfo,
    [plan.AS_ACTIVE_SKILLS]     =   initAsSkill,
    [plan.AS_ASSISTANT]         =   initAsAssistant,
    [plan.Const]                =   initConst,
    [plan.SKIN]                 =   initSkin,
    [plan.PVPAIDATA]            =   initPvpAiData,
    [plan.ProduceName]          =   initProduceName,
}

local function load_plan_data()
    local plan_set = {}
    for key, file in pairs(COMMON.plan_map) do
        local stat
        if file == 'map.lua' or file == 'names.lua' then
            -- 不是通过csv转换过来的数据
            local f = assert(io.open("data/" .. file), 'r')
            stat = load(f:read("*a"))()
            f:close()
        else
            stat = loadCsvFile(string.format("data/%s", file))
            local saveLuaFile = false
            if saveLuaFile then
                local t = utils.serialize_table(stat)
                io.open('data/' .. file, 'w'):write('return ' .. t)
            end
        end

        if init_data_map[key] then  -- 需要重组数据
            plan_set[key] = init_data_map[key](stat)
        else -- 默认处理方式
            plan_set[key] = {}
            for k, v in pairs(stat) do
                plan_set[key][tonumber(k)] = v
            end
        end
    end
    return plan_set
end

local CMD = {}

function CMD.reload()
    logger.trace('sharedata reload config data')
    sharedata.update('plan_set', load_plan_data())
    return 'update plan set okay'
end

local sharedata_plan_set
function CMD.show(idx)
    if not sharedata_plan_set then
        sharedata_plan_set = sharedata.query('plan_set')
    end
    local data = sharedata_plan_set[idx]
    local r = {}
    for i, v in ipairs(data or {}) do
        local vt = {}
        for k, v in pairs(v) do
            table.insert(vt, tostring(k) .. ' : ' .. tostring(v))
        end
        table.insert(r, table.concat(vt, '\n'))
    end
    return table.concat(r, '\n------\n')
end


skynet.start(function()
    sharedata.new("plan_set", load_plan_data())

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if f then
            local ok, ret = pcall(f, ...)
            if ok then
                return skynet.retpack(ret)
            else
                return skynet.ret()
            end
        else
            return skynet.ret()
        end
    end)
end)


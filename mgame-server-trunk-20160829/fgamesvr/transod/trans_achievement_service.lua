
--成就老数据转换
--
--
--auth:diandian
--date:2016.4.6


local skynet = require "skynet"
local logger = require "logger"
local utils = require "utils"
local db_setting = require "db_setting"

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

local function update_achievement(account_info, cfg, achievements)
    local a = cfg
    -- 2#5#条件值
    -- 拆分条件字符串
    local cond_param
    local param1
    local param2
    local param3

    cond_param = utils.split(a.Condition, "#")
    param1= tonumber(cond_param[1])
    param2= tonumber(cond_param[2])
    param3= tonumber(cond_param[3])

    local check_point_dat = skynet.call(redis_service, "lua", "hget_obj", "check_points", account_info.uin, true)
    if not achievements[a.Id] or achievements[a.Id].Status ~= 0 then
        if a.Category == 1 then -- 关卡类别
            if param1 == 2 then -- pve道具使用次数
                if account_info.pve_use_items >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1, -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end
            elseif param1 == 1 and param2 == 1 then
                if check_point_dat then
                    local check_point_id = tonumber(param3)
                    local chapter_id = math.floor(check_point_id / 1000)
                    local chapter_dat = check_point_dat[chapter_id]
                    if chapter_dat then
                        local pd = chapter_dat[check_point_id]
                        if pd and pd.stars >= 3 then
                            if not achievements[a.Id] then
                                achievements[a.Id] = {
                                    Id = a.Id,
                                    Status = 1, -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                    Award = a.Award
                                }
                            end
                        end
                    end
                end
            end
        elseif a.Category == 2 then
            if param1 == 1 then
                if check_point_dat then
                    local count = tonumber(param2)
                    if check_point_dat.chapter_stars[1] >= count then
                        if not achievements[a.Id] then
                            achievements[a.Id] = {
                                Id = a.Id,
                                Status = 1, -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                                Award = a.Award
                            }
                        end
                    end
                end
            end
        elseif a.Category == 3 then -- 对站类别          
            if param1 == 2 and param2 == 5 then -- 随机pk
                if account_info.rand_room_count >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1, -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end  
            elseif param1 == 2 and param2 == 6 then -- 开房pk
                if account_info.fixed_room_count >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id, 
                            Status = 1, -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end
            elseif param1 == 2 and param2 == 7 then -- 累计无道具场次数
                if account_info.pvp_nopro_count >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end
            elseif param1 == 2 and param2 == 8 then -- 段位积分(最大积分)
                if account_info.pvp_max_point >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end
            elseif param1 == 2 and param2 == 9 then -- 连胜成就(最大连胜记录)
                if account_info.max_streak_win >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end  
            elseif param1 == 2 and param2 == 10 then -- PVP道具累计使用次数
                if account_info.pvp_use_items >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end
            elseif param1 == 2 and param2 == 11 then -- 累计对战胜利场次
                if account_info.pvp_win_count >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end
            elseif param1 == 2 and param2 == 12 then --累计荣誉
                if account_info.pvp_addup_score and account_info.pvp_addup_score >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end
            elseif param1 ==2 and param2 == 13 then --累计每日任务完成次数
                if account_info.daily_task_count and account_info.daily_task_count >= param3 then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,   -- 1 : 代表成就可以领取 0 : 已经领取 2 : 未完成
                            Award = a.Award
                        }
                    end
                end
            else
                -- 不需要处理
            end
        end
    else 
        -- 其它类别暂时不需要处理
    end

    if update then
        skynet.call(redis_service, "lua", "hset_obj", "achievement", account_info.uin, achievements)
        local data = { -- 发送状态可领取的成就id值 前端把完成进度直接置成条件值
           ListIds = list
        }
    end
end


local function init()
    skynet.call(redis_service, "lua", "del", "achievement")
    skynet.call(redis_service, "lua", "del", "achievement_info")
    local all_cfg = loadCsvFile("data/AchievementAward.csv")

    local max_uin = skynet.call(redis_service, "lua" ,"get", "account_max_id")
    local being_uin = 1000001
    max_uin = tonumber(max_uin)
    local cur_plan = 0
    local step = 0
    for uin = being_uin, max_uin do
        cur_plan = cur_plan + 1
        step = step + 1
        if step >= 2000 then
            logger.trace("trans_achievement_service plan ", cur_plan)
            step = 0
        end

        local info = skynet.call(redis_service, "lua", "hget_obj", "account", uin, true)
        info.achievement_level = db_setting.achievement_level
        info.achievement_point = db_setting.achievement_point
        skynet.call(redis_service, "lua", "hset_obj", "account", uin, info)
        local achievements = {}
        for id, cfg in pairs(all_cfg) do
            update_achievement(info, cfg, achievements)
        end
        skynet.call(redis_service, "lua", "hset_obj", "achievement", uin, achievements)
    end

    logger.trace("trans achievement success")
end



local CMD = {}
skynet.start(function()
    logger.trace("trans_achievement_service start...")

    redis_service = skynet.queryservice("redis_service")

    init()

    local traceback = debug.traceback

    skynet.dispatch("lua", function(_, src, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s src:%d", command, ret, src)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)
end)

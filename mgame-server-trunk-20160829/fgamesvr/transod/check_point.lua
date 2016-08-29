
local skynet = require "skynet"
local logger = require "logger"
local utils  = require "utils"
local COMMON = require "common"


local gift_cfg = nil
local redis_conn 

local newbie_list = {
    33,
    34,
    80,
    90,
    100,
    110,
    1,
    13,
    14,
    15,
    20,
    130,
    140,
    150,
    160,
}

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

-- 得到对应章节的关卡所获得的星星
local function get_chapter_star(data)
    local stars = 0
    for k,v in pairs(data or {}) do
        -- 关卡在table表里面是[id] = {}
        if type(k) == "number" then
            stars = stars + v.stars
        end
    end
    return stars
end


-- 初始当前关卡Id的数据
local function init_cp(cp_id)
    return {
        id          = cp_id, -- 关卡id
        stars       = 0,     -- 战斗获得的星星数
        reward      = false, -- 是否通关奖励
        full_reward = false, -- 是否满星奖励
        start_time  = os.time(),
        win         = 0,    -- 胜利次数
        fail        = 0,    -- 失败次数
        num         = 0,    -- 进入关卡的次数
        reset       = 0,    -- 重置关卡的次数
        exit        = 0,    -- 退出关卡的次数
        new_flag    = false, -- 是否新关卡,创建的时候默认是新的，pve结算后就为false
        score       = 0,    -- 此关卡的得分
    }
end

-- 初始化所有章节关卡的数据
local function init_cache()
    local cp = {}
    -- 动态读取所有的pve章节数
    -- 99 章节是新手引导章节
    for k,v in pairs({[1] = 1, [2] = 2, [3] = 3}) do
        if k == 1 then
            cp[k] = {max_cp = 0, lock = false, chest = {}, stars = 0} -- max_cp : 最大关卡id  chest : 获得的宝箱
        else
            cp[k] = {max_cp = 0, lock = true, chest = {}, stars = 0} -- max_cp : 最大关卡id  chest : 获得的宝箱
        end
    end
    cp[99] = {max_cp = 99010, lock = false, chest = {}, stars = 0}
    cp[99][99010] = init_cp(99010)
    cp[99][99010].reward = true
    cp.chapter_stars = {}     --大关卡行星
    cp.share = {}           -- pve关卡是否可分享的Id
    cp.update = os.time()   -- 缓存更新时间，用来处理每天只能用钻石跳过的次数
    cp.skip_num = 0         -- 已经跳过的次数
    return cp
end

local function which_chapter(id)
    local one_ids = {
        [1010]=1010,
        [1020]=1020,
        [1030]=1030,
        [1040]=1040,
        [1050]=1050,
        [1060]=1060,
        [1070]=1070,
        [1080]=1080,
        [1090]=1090,
        [1100]=1100,
        [1110]=1110,
        [1120]=1120,
        [1130]=1130,
        [1140]=1140,
        [1150]=1150,
        [2010]=1160,
        [2020]=1170,
        [2030]=1180,
        [2040]=1190,
        [2050]=1200,
        [2060]=1210,
        [2070]=1220,
        [2080]=1230,
        [2090]=1240,
        [2100]=1250,
        [2110]=1260,
        [2120]=1270,
        [2130]=1280,
        [2140]=1290,
        [2150]=1300,
    }

    local one = false
    local new_id = 0
    for k,v in pairs(one_ids) do
        if k == id then
            one = true
            new_id = v
            break
        end
    end

    if one and new_id ~= 0 then
        return 1, new_id
    end

    local two_ids = {
        [2160]=2010,
        [2170]=2020,
        [2180]=2030,
        [2190]=2040,
        [2200]=2050,
        [2210]=2060,
        [2220]=2070,
        [2230]=2080,
        [2240]=2090,
        [2250]=2100,
        [2260]=2110,
        [2270]=2120,
        [2280]=2130,
        [2290]=2140,
        [2300]=2150,
        [2310]=2160,
        [2320]=2170,
        [2330]=2180,
        [2340]=2190,
        [2350]=2200,
        [2360]=2210,
        [2370]=2220,
        [2380]=2230,
        [2390]=2240,
        [2400]=2250,
        [2410]=2260,
        [2420]=2270,
        [2430]=2280,
        [2440]=2290,
        [2450]=2300,
        [2460]=2310,
        [2470]=2320,
        [2480]=2330,
        [2490]=2340,
        [2500]=2350,
    }

    local two = false
    for k,v in pairs(two_ids) do
        if k == id then
            two = true
            new_id = v
            break
        end
    end
    if two and new_id ~= 0 then
        return 2, new_id
    end

    return 0,0
end

local function gift2plugin( gift_data )
    local container = {}
    if gift_data.diamond > 0 then -- 钻石
        table.insert(container, COMMON.plugin_type.DIAMOND .. ':' .. gift_data.diamond)
    end

    if gift_data.manual > 0 then -- 体力
        table.insert(container, COMMON.plugin_type.POWER .. ':' .. gift_data.manual)
    end
    if gift_data.energy > 0 then -- 精力
        table.insert(container, COMMON.plugin_type.ENERGY .. ':' .. gift_data.energy)
    end

    if gift_data.honor > 0 then --荣誉
        table.insert(container, COMMON.plugin_type.SCORE .. ':' .. gift_data.honor)
    end

    for i = 1, 5 do
        local item_str = gift_data['item' .. i]
        if type(item_str) == 'string' and item_str ~= '' then
            local item_id, item_count = item_str:match('(%d+)%+(%d+)')
            if item_id and item_count then
                table.insert(container, item_id .. ':' .. item_count)
            end
        end
    end
    return '{ ' .. table.concat(container, ',') .. ' }'
end

-- 发邮件
local function send_mail(uin, plugin, content)
    local now = os.time()
    local data = {
        title = '美味冒险奖励',
        content = content,
        plugin = plugin,
        sendtime = now,
        outdate =  now + 30 * 24 * 60 * 60 * 100  -- 一个月之后
    }
    local mail_id = skynet.call(redis_conn, "lua", "id_handler", "max_single_mail_id")
    skynet.call(redis_conn, "lua", "hset_obj", "single_mail", mail_id, data)

    local user_mail = skynet.call(redis_conn, "lua", "hget_obj", "usermail", uin, true)
    if not user_mail then
        user_mail = {total_no = 0, mail_num = 0, max_global_id = 0, content = {}}
    end
    user_mail.mail_num = user_mail.mail_num + 1
    user_mail.total_no = user_mail.total_no + 1
    user_mail.content[user_mail.total_no] = {type = COMMON.mail_type.SINGLE, id = mail_id}
    skynet.call(redis_conn, "lua", "hset_obj", "usermail", uin, user_mail)
end

local function trans_data(new, old, cur_csv, uin)
    for k,v in pairs(old) do
        if cur_csv[k] then
            local chapter,id = which_chapter(k)
            if chapter ~= 0 then
                if chapter == 2 and new[chapter].lock then
                    new[chapter].lock = false
                end
                new[chapter][id] = init_cp(id)
                local cp = new[chapter][id]
                cp.stars = v.stars
                cp.reward = true --v.reward
                cp.full_reward = v.full_pass
                -- 发通关奖励发邮件
                if cp.reward and (cur_csv[id].baseBonus ~= 0) then
                    send_mail(uin, gift2plugin(gift_cfg[tonumber(cur_csv[id].baseBonus)]), string.format('恭喜您在%s里获得通关奖励^_^!', cur_csv[id].passName))
                end

                -- 发完美通关奖励邮件
                if cp.full_reward and (cur_csv[id].perfectBonus ~= 0) then
                    send_mail(uin, gift2plugin(gift_cfg[tonumber(cur_csv[id].perfectBonus)]), string.format('恭喜您在%s里获得完美通关奖励^_^!', cur_csv[id].passName))
                end

                cp.start_time = v.ts
                if id > new[chapter].max_cp then
                    new[chapter].max_cp = id
                end
            else
                logger.debugf("chapter error for cp id[%d]", k)
            end
        end
    end
    
    new[1].stars = get_chapter_star(new[1])
    new[2].stars = get_chapter_star(new[2])
    new[3].stars = get_chapter_star(new[3]) 
    new.chapter_stars[1] =  new[1].stars + new[2].stars + new[3].stars
end


skynet.start(function()
    skynet.uniqueservice("mysql_service")
    redis_conn = skynet.uniqueservice("redis_service")

    -- 加载新的CheckPoint.csv
    -- 得到最大的用户id
    -- 遍历玩家
    -- 初始化新关卡的结构
    -- 计算星星&解锁&关卡   解锁条件(1. 31就解锁二章节   2. 未达到31大于1020)

    -- 新手训练营99010设为完成
    -- 新手指引33 34 设置完成 hash newbie_list
    -- 修改名字的次数大于0

    local cp_file = 'data/CheckPoint.csv'
    local being_uin = 1000001
    local new_csv = loadCsvFile(cp_file)
    if not new_csv then
        logger.debugf("load file failed")
        skynet.exit()
    end

    gift_cfg = loadCsvFile('data/Gift.csv')
    if not gift_cfg then
        logger.debugf("load file failed")
        skynet.exit()
    end

    logger.debugf("tran data begin at : %d", os.time())
    local max_uin = skynet.call(redis_conn, "lua" ,"get", "account_max_id")
    max_uin = tonumber(max_uin)
    -- local max_uin = 1000001
    local count = 0
    for uin = being_uin, max_uin do
        local old_data = skynet.call(redis_conn, "lua", "hget_obj", "check_points", uin, true)
        if old_data then
            local new_data = init_cache()
            trans_data(new_data, old_data.CheckPoint_info or {}, new_csv, uin)
            skynet.call(redis_conn, "lua", "hset_obj", "check_points", uin, new_data)

            local new_list = {}
            for k,v in pairs(newbie_list) do
                new_list[tonumber(v)] = true
            end
            skynet.call(redis_conn, "lua", "hset_obj", "newbie_list", uin, new_list)

            local player = skynet.call(redis_conn, "lua", "hget_obj", "account", uin, true)            
            if player and player.change_name_count == 0 then
                player.change_name_count = 1
                skynet.call(redis_conn, "lua", "hset_obj", "account", uin, player)
            end
            count = count + 1
        end
    end

    logger.debugf("tran data end at : %d with count : %d", os.time(), count)

end)

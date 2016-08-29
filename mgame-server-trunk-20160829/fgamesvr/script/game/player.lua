local player = {}

local skynet          = require "skynet"
local utils           = require "utils"
local logger          = require "logger"
local COMMON          = require "common"
local sharedata_utils = require "sharedata_utils"
local setting         = require "setting"
local proto           = require "proto"
local json            = require "cjson"
local agent_item      = require "agent_item"
local agent_achievement = require("agent_achievement")
local agent_player

local send_package
local send_response, send_error_code
local redis_call

local redis_service
local busilog_service



-- 道具注意事项
-- 所有的道具操作都需要在这里进行操作
-- 道具存储为hash table
-- key必须为string,因为需要将items对象json化,key必须是string的,数据在转换成json时会有问题

----------------------------       道具信息     -------------------------------
-- 联网情况下，物品的同步
local agent_vip
local function check_redis()
    if redis_service == nil then
        redis_service = skynet.queryservice("redis_service")
    end
end

local function notice_player(notice_data, extra, src)
    local update_data = {}
    for k,v in pairs(notice_data) do
        local ptype, value, value2 = v[1],v[2],v[3]
        local data
        if ptype ~= COMMON.property_type.EXP and
            ptype ~= COMMON.property_type.TITLE then
            data = {
                UpType  = ptype,
                Value   = value,
                Value2  = value2,
                Src     = src, -- 来源
            }
        else
            data = { UpType = ptype, Extra =  value }
        end
        table.insert(update_data, data)
    end

    logger.trace('玩家属性变化:\n', logger.dump(update_data))
    send_response(COMMON.MESSAGE.CS_CMD_PROPERTY_UPDATE, { UpdateData = update_data })
end

player.notice_player = notice_player

function player.change_flag(account_info, flag, save)
    if flag == nil then
        flag = true
    end
    account_info.change_flag = flag
    if save then
        account_info.change_flag = false
        skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info)
    end
end

-- 装载玩家物品数据
function player.load_item(uin)
    check_redis()
    local items = skynet.call(redis_service, "lua", "hget_obj", "item", uin, true) or {} -- 玩家所有物品
    local past = false
    local now = os.time()
    for k, v in pairs(items) do
        local item = sharedata_utils.getItemById(k)
        if item then
            if item.endTime and item.endTime <= now then
                past = true
                items[k] = nil
            end
        else
            logger.trace("error not exist item " .. k)
        end
    end

    if past then
        player.save_item(uin, items)
    end
    return items
end

-- 保存玩家数据
function player.save_item(uin, items)
    --check_redis()
    --skynet.call(redis_service, "lua", "hset_obj", "item", uin, items)
end

-- src来源 物品的使用
function player.use_item(src, itemId, count, account_info)
    if count < 1 then
        logger.trace("use item count error!", count)
        return false
    else
        player.record_item_use(account_info, src) -- 使用道具记
        return agent_item.use_item(itemId, count, src, true)
    end
end


----------------------------       关卡信息     -------------------------------

--获取某个关卡点的星星数
function player.getPointStar(uin, s_piontId)
    check_redis()
    local points = skynet.call(redis_service, "lua", "hget_obj", "point", uin, true)
    if not points then
        return nil
    end
    for _, point in pairs(points) do
        if point and point.pointId == s_piontId then
            return point.star
        end
    end
    if points[s_piontId] then
        return points[s_piontId].star
    end
    return nil
end

--获取某个大关卡的星星通关总数
function player.getStageStarCount(uin, stageId)
    check_redis()
    local points = skynet.call(redis_service, "lua", "hget_obj", "point", uin, true)
    local chapter_star = {}
    for _, v in pairs(points) do
        local chapter = math.floor(v.pointId / 1000)
        if not chapter_star[chapter] then
            chapter_star[chapter] = v.star
        else
            chapter_star[chapter] = chapter_star[chapter] + v.star
        end
    end
    return chapter_star[stageId]
end

-- 关卡成就检查
function player.check_point_achievement(account_info, cp_id, data, chapter, chapter_star)
    check_redis()
    local update = false
    local list = {}
    local achievements = skynet.call(redis_service, "lua", "hget_obj", "achievement", account_info.uin, true) -- 角色已标记的成就
    if not achievements then
        achievements = {}
    end
    local achievementData = sharedata_utils.get_stat_all(COMMON.plan.AchievementAward) -- 所有成就

    local new_value = table.insert -- 加快访问速度
    --单关卡成就检查
    for j, a in pairs(achievementData) do
        if not achievements[a.Id] and a.Category == 1 and utils.bit_set(data[3], 0x02) then -- 完美通关
            local cond_param = utils.split(a.Condition, "#")
            local param3 = tonumber(cond_param[3]) -- 获取关卡的编号 如 CheckPoint.csv 1152
            if param3 == cp_id then
                achievements[a.Id] = {
                    Id = a.Id,
                    Status = 1,
                    Award = a.Award
                }
                update = true
                new_value(list, a.Id)
            end
        elseif not achievements[a.Id] and a.Category == 2 then -- 章节通关星星总数成就
            local IdAndStar = utils.split(a.Condition, "#")
            local chapterId = tonumber(IdAndStar[1])
            local countStar = tonumber(IdAndStar[2])
            --条件判断
            if chapterId == chapter and chapter_star >= countStar then
               if not achievements[a.Id] then
                   achievements[a.Id] = {
                       Id = a.Id,
                       Status = 1,
                       Award = a.Award
                   }
                   update = true
                   new_value(list, a.Id)
               end
            end
        else

        end
    end

    if update then
        skynet.call(redis_service, "lua", "hset_obj" ,"achievement", account_info.uin, achievements)
        send_response(COMMON.MESSAGE.CS_CMD_UPDATE_ACHIEVEMENT, { ListIds = list })
    end
end

-- 检查房间相关的成就
function player.check_pvp_room_achievement(account_info)
    check_redis()
    local update = false
    local list = {}
    local achievements = skynet.call(redis_service, "lua", "hget_obj", "achievement", account_info.uin, true) -- 角色已标记的成就
    if not achievements then
        achievements = {}
    end
    local achievementData = sharedata_utils.get_stat_all(COMMON.plan.AchievementAward) -- 所有成就
    for _, a in pairs(achievementData) do
        if a.Category == 3 then
            local type_target = utils.split(a.Condition, "#")
            local chapterId = tonumber(type_target[1])
            local type = tonumber(type_target[2])
            local target = tonumber(type_target[3])
            -- 随机pk
            if chapterId == 2 and type == 5 then
                if account_info.rand_room_count >= target then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,
                            Award = a.Award
                        }
                        update = true
                        table.insert(list, a.Id)
                    end
                end
            end
            -- 开房pk
            if chapterId == 2 and type == 6 then
                if account_info.fixed_room_count >= target then
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,
                            Award = a.Award
                        }
                        update = true
                        table.insert(list, a.Id)
                    end
                end
            end
        end
    end
    if update then
        skynet.call(redis_service, "lua", "hset_obj", "achievement", account_info.uin, achievements)
        send_response(COMMON.MESSAGE.CS_CMD_UPDATE_ACHIEVEMENT, { ListIds = list })
    end
end


-- 检查pvp排行榜相关的成就
function player.check_pvp_rank_achievement(account_info, rank_data)
    check_redis()
    if rank_data == nil then return end
    local silver = rank_data[2] -- 白银
    local gold = rank_data[3] -- 黄金
    local diamond = rank_data[4] -- 钻石
    local king = rank_data[5] -- 皇者

    if silver == nil then silver = 0 end
    if gold == nil then gold = 0 end
    if diamond == nil then diamond = 0 end
    if king == nil then king = 0 end

    local diamond_count = diamond + king
    local gold_count = diamond_count + gold
    local silver_count = gold_count + silver

    local update = false
    local list = {}
    local achievements = skynet.call(redis_service, "lua", "hget_obj", "achievement", account_info.uin) -- 角色已标记的成就
    if not achievements then
        achievements = {}
    end
    local achievementData = sharedata_utils.get_stat_all(COMMON.plan.AchievementAward) -- 所有成就
    for _, a in pairs(achievementData) do
        if a.Category == 3 then
            local type_target = utils.split(a.Condition, "#")
            local chapterId = tonumber(type_target[1])
            local type = tonumber(type_target[2])
            local target = tonumber(type_target[3])
            if chapterId == 1 then
                if type == 4 and king >= target then -- 王者成就
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,
                            Award = a.Award
                        }
                        update = true
                        table.insert(list, a.Id)
                    end
                end
                if type == 3 and diamond_count >= target then -- 黄金组以上段位成就
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,
                            Award = a.Award
                        }
                        update = true
                        table.insert(list, a.Id)
                    end
                end
                if type == 2 and gold_count >= target then --白银组以上段位成就
                    if not achievements[a.Id] then
                        achievements[a.Id] = {
                            Id = a.Id,
                            Status = 1,
                            Award = a.Award
                        }
                        update = true
                        table.insert(list, a.Id)
                    end
                end
                if type == 1 and silver_count >= target then --青铜组以上段位成就
                if not achievements[a.Id] then
                    achievements[a.Id] = {
                        Id = a.Id,
                        Status = 1,
                        Award = a.Award
                    }
                    update = true
                    table.insert(list, a.Id)
                end
                end
            end
        end
    end
    if update then
        skynet.call(redis_service, "lua", "hset_obj", "achievement", account_info.uin, achievements)
        send_response(COMMON.MESSAGE.CS_CMD_UPDATE_ACHIEVEMENT, { ListIds = list })
    end
end

--判断金币是否足够
function player.is_diamond_enough(account_info, change_value)
    local ret = true
    local diamond = account_info.diamond
    if diamond + change_value < 0 then
        ret = false
    end
    return ret
end

-- 所有的diamond变化，都走此函数 方便日志记录
function player.change_diamond(account_info, src, change_value, notice_flag)
    if notice_flag == nil then
       notice_flag = true
    end
    check_redis()
    logger.tracef("uin=%d&changevalue=%d&currrValue=%d", account_info.uin, change_value, account_info.diamond )
    if change_value == 0 then
        return true
    end
    local diamond = account_info.diamond
    if diamond + change_value < 0 then
        return false
    end
    account_info.diamond = diamond + change_value
--    skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info, true)
    player.change_flag(account_info)

    -- 货币日志
    skynet.send(busilog_service, "lua", "log_coin", account_info.uin , COMMON.currency_attr.diamond, change_value, account_info.diamond, src)

    if notice_flag then
       notice_player({{COMMON.property_type.DIAMOND, account_info.diamond}}, 0, src)
    end
    return true
end

local max_power = setting.game_setting.max_power
function player.add_power(account_info, point, notice_flag)
    if notice_flag == nil then
        notice_flag = true
    end
    check_redis()
    if point < 0 or account_info.power < max_power then
        account_info.power = account_info.power + point
        if account_info.power > max_power then
            account_info.power = max_power
        end
        player.change_flag(account_info)
        if notice_flag then
            notice_player({{COMMON.property_type.POWER, account_info.power}})
        end
        return true
    end
    return false
end

--增加荣誉接口
function player.change_score(account_info, score, notice_flag, src, not_update_achievement)
    if notice_flag == nil then
        notice_flag = true
    end
    check_redis()

    if score > 0 then
        account_info.pvp_addup_score = account_info.pvp_addup_score + score
    end

    if not not_update_achievement then
        agent_achievement.update_achievement()
    end
    if account_info.pvp_score + score >= 0 then
        local cur_score = account_info.pvp_score
        account_info.pvp_score = cur_score + score
        player.change_flag(account_info)
        if notice_flag then
            notice_player({{COMMON.property_type.SCORE, account_info.pvp_score}})
        end

        EventCenter.dispatchEvent(Events.HONOR_ADD, score)
        skynet.send(busilog_service, "lua", "log_coin", account_info.uin , COMMON.currency_attr.score, score, account_info.pvp_score, src)
        return true
    end

    return false
end

function player.change_point(account_info, point, notice_flag, src)
    if notice_flag == nil then
        notice_flag = true
    end
    check_redis()
    if account_info.pvp_point + point >= 0 then
        local cur_point = account_info.pvp_point
        account_info.pvp_point = cur_point + point
        -- 变更最大积分
        if account_info.pvp_max_point and account_info.pvp_point > account_info.pvp_max_point then
            account_info.pvp_max_point = account_info.pvp_point
        end
        player.change_flag(account_info)
        if notice_flag then
            notice_player({{COMMON.property_type.POINT, account_info.pvp_point}})
        end
        skynet.send(busilog_service, "lua", "log_coin", account_info.uin , COMMON.currency_attr.point, point, account_info.pvp_point, src)
        return true
    end
    return false
end

-- 最大连胜记录
function player.change_max_streak(account_info)
    check_redis()
    if account_info.max_streak_win and account_info.continue_win_count > account_info.max_streak_win then
        account_info.max_streak_win = account_info.continue_win_count
        player.change_flag(account_info)
        return true
    end
    return false
end

-- 属性值变更
function player.increment_attr( account_info, attr, step )

    -- 没有指定属性
    if not attr then
        return false
    end

    if not step then
        step = 1
    end

    if type(step) ~= "number" then
        return false
    end

    local update = false
    if account_info[attr] then
        account_info[attr] = account_info[attr] + step
        update = true
    end

    if update then
        player.change_flag(account_info)
    end
end

-- 道具使用次数记录
function player.record_item_use(account_info, src)
    if not src then
        return false
    end

    local update = false
    if src == COMMON.change_src.use_item then -- pve使用道具
        if account_info.pve_use_items then
            account_info.pve_use_items = account_info.pve_use_items + 1
        else
            account_info.pve_use_items = 1
        end
        update = true
    elseif src == COMMON.change_src.pvp then -- pvp使用道具
        if account_info.pvp_use_items then
            account_info.pvp_use_items = account_info.pvp_use_items + 1
        else
            account_info.pvp_use_items = 1
        end
        update = true
    end

    if update then
        player.change_flag(account_info)
        agent_achievement.update_achievement()
    end

    return false
end

function player.add_power_solt(account_info, value, notice_flag)
    if notice_flag == nil then
        notice_flag = true
    end
    local power_slot = 100
    local vip_slot = agent_vip.get_vip_privilege_param(1)
    if vip_slot ~= 0 then
        power_slot = vip_slot
    end

    if account_info.power_slot < power_slot then
        account_info.power_slot = account_info.power_slot + value
        if account_info.power_slot > power_slot then
            account_info.power_slot = power_slot
        end
        player.change_flag(account_info)
        if notice_flag then
            notice_player({{COMMON.property_type.POWER_SOLT, account_info.power_slot}})
        end
    end
end

function player.add_month_card(account_info, value)
   local tnow = os.time()
   if account_info.month_card_time < tnow then
      account_info.month_card_time = tnow + value * 86400
   else
      account_info.month_card_time = account_info.month_card_time + value * 86400
   end
   player.change_flag(account_info)
   local days,add = math.modf((account_info.month_card_time - tnow) / 86400)
   if add ~= 0 then
      days = days + 1
   end
   agent_player.check_month_card_reward()
   notice_player({{COMMON.property_type.MONTH_CARD, days}})
end

function player.add_room_ticket(account_info, value, notice_flag)
    if notice_flag == nil then
        notice_flag = true
    end

    account_info.room_ticket = account_info.room_ticket + value
    if account_info.room_ticket < 0 then
        account_info.room_ticket = 0
    end
    if notice_flag then
        notice_player({{COMMON.property_type.ROOM_TICKET, account_info.room_ticket}})
    end
end


-- 给玩家添加礼品
-- return: 成功返回true, 失败返回false 貌似return值没什么用 -_-!
function player.add_gift(account_info, gift_id, src, use_price)
    check_redis()
    local gift_data = sharedata_utils.get_stat(COMMON.plan.Gift, gift_id)
    if not src then
        src = COMMON.change_src.use_item
    end
    logger.trace('add gift 添加礼物接口: ', account_info.uin, gift_id)
    if gift_data then
        local notice_data = {}
        if gift_data.diamond > 0 then -- 钻石
            player.change_diamond(account_info, src, gift_data.diamond, false)
            table.insert(notice_data, {COMMON.property_type.DIAMOND, account_info.diamond})
        end

        if gift_data.manual > 0 then -- 体力
            player.add_power(account_info, gift_data.manual, false)
            table.insert(notice_data, {COMMON.property_type.POWER, account_info.power})
        end

        if gift_data.roomcard > 0 then --开房券
            player.add_room_ticket(account_info, gift_data.roomcard, false)
            table.insert(notice_data, {COMMON.property_type.ROOM_TICKET, account_info.room_ticket})
        end

        if gift_data.honor > 0 then --荣誉
            player.change_score(account_info, gift_data.honor, false, src)
            table.insert(notice_data, {COMMON.property_type.SCORE, account_info.pvp_score})
        end

        if gift_data.badge > 0 then -- 勋章id
            player.add_title(account_info, gift_data.badge)
        end

        for i = 1, 5 do
            local item_str = gift_data['item' .. i]
            if type(item_str) == 'string' and item_str ~= '' then
                local item_id, item_count = item_str:match('(%d+)%+(%d+)')
                item_id = tonumber(item_id)
                item_count = tonumber(item_count)
                if item_id and item_count then
                    local ret, item, item_cfg = agent_item.add_item(
                        item_id, item_count, src, false)
                    if ret then
                        table.insert(notice_data, 
                            {item_cfg.property_type, item.count, item_id})
                    end
                end
            end
        end

        if gift_data.items and gift_data.items ~= "" then
            for k, v in string.gmatch(gift_data.items, "([.%w]+)+(%w+)") do
                k = tonumber(k)
                v = tonumber(v)
                assert(k and v, k, v)
                local ret, item, cfg = agent_item.add_item( 
                    k, v, src, false)
                table.insert(notice_data, {cfg.property_type, item.count, item.itemId})
            end
        end
        notice_player(notice_data)
        skynet.send(busilog_service, "lua", "log_misc",
            COMMON.busilog_misc_type.misc_add_gift,
                account_info.uin, gift_id, src)
        return true
    else
        logger.trace('add gift: error gift id')
    end
    return false
end

function player.gift2plugin( gift_data )
    local container = {}
    if gift_data.diamond > 0 then -- 钻石
        table.insert(container,
            COMMON.plugin_type.DIAMOND .. ':' .. gift_data.diamond)
    end

    if gift_data.manual > 0 then -- 体力
        table.insert(container,
            COMMON.plugin_type.POWER .. ':' .. gift_data.manual)
    end

    if gift_data.honor > 0 then --荣誉
        table.insert(container,
            COMMON.plugin_type.SCORE .. ':' .. gift_data.honor)
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
    return table.concat(container, ',')
end

function player.add_gift_by_str(gift_str, account_info, src)
    local notice_data = {}
    for item_id, count in string.gmatch(gift_str, "([.%w]+):(%w+)") do
        count = tonumber(count)
        item_id = tonumber(item_id)
        -- item_id也有可能为字符串
        if item_id then
            if item_id == COMMON.plugin_type.DIAMOND then
                player.change_diamond(account_info, src, count, false)
                table.insert(notice_data, 
                    {COMMON.property_type.DIAMOND, account_info.diamond})
            elseif item_id == COMMON.plugin_type.POWER then
                player.add_power(account_info, count, false)
                table.insert(notice_data, 
                    {COMMON.property_type.POWER, account_info.power})
            elseif item_id == COMMON.plugin_type.SCORE then
                player.change_score(account_info, count, false, src)
                table.insert(notice_data, 
                    {COMMON.property_type.SCORE, account_info.pvp_score})
            else
                local ret, item, item_cfg = agent_item.add_item(
                    item_id, count, src, false)
                if ret then
                    table.insert(notice_data, 
                        {item_cfg.property_type, item.count, item_id})
                end

            end
        end
    end
    if next(notice_data) then
        notice_player(notice_data)
    end
end

-- 给指定UIN的玩家增加厨艺材料
function player.add_glyph_stuff(uin, stuff_id, count, src)
    check_redis()

    stuff_id, count = assert(tonumber(stuff_id)), assert(tonumber(count))

    local stuffData = skynet.call(redis_service, 'lua', 'hget_obj', 'stuffData', uin) or {}

    local t = stuffData[stuff_id]
    if t then
        t.count = t.count + count
    else
        stuffData[stuff_id] = { count = count }
    end
    skynet.call(redis_service, 'lua', 'hset_obj', 'stuffData', uin, stuffData)
    EventCenter.dispatchEvent(Events.GLYPH_STUFF_CHANGE)
    return stuffData[stuff_id].count
end

function player.change_chat_data(account_info)
    local tnow = math.floor(skynet.time())
    local data = {
        count = account_info.free_chat_count,
        time   = ( tnow < account_info.last_chat_time + setting.game_setting.chat_count_check_time) and account_info.last_chat_time + setting.game_setting.chat_count_check_time - tnow  or  0
    }

    if data.count ==  setting.game_setting.max_chat_count then
        data.time = 0
    end
    notice_player({{COMMON.property_type.CHAT, data.count, data.time}})
end

--添加一个title
function player.add_title(account_info, title)
    return agent_player.add_title(title)
end

function player.add_property(account_info, add_data)
    local notice_data = {}
    for k,v in pairs(add_data) do
        local add_type, add_value, src, id = v[1],v[2],v[3],v[4]

        if add_type == COMMON.property_type.POWER then
            player.add_power(account_info, add_value, false)
            table.insert(notice_data, {add_type, account_info.power})
        end

        if add_type == COMMON.property_type.DIAMOND then
            player.change_diamond(account_info, src, add_value, false)
            table.insert(notice_data, {add_type, account_info.diamond})
        end

        if add_type == COMMON.property_type.ROOM_TICKET then
            player.add_room_ticket(account_info, add_value, false)
            table.insert(notice_data, {add_type, account_info.room_ticket})
        end

    end
    if #notice_data > 0 then
        notice_player(notice_data)
    end
end

function player.set_agent_vip(business)
    agent_vip = business
end

function player.takeBonusById(account_info, bonusId, count, src, rate)
    local bonusData = sharedata_utils.getBonusById(bonusId)
    return player.takeBonus(account_info, bonusData, count, src, rate)
end

-- count可选 默认为1
function player.takeBonus(account_info, bonusData, count, src, rate)
    if not rate or rate < 1 then
        rate = 1
    end

    count = count or 1
    local notice_data = {}
    local add_data = {}
    if not bonusData then return add_data end
    local fixedItems = {}
    for i = 1, count do
        local randItems = {}
        for _, v in ipairs(bonusData) do
            if tonumber(v.isrand) == 1 then
                table.insert(randItems, v)
            else
                table.insert(fixedItems, v)
            end
        end

        -- 处理随机 从randItems中随机一个出来 插入到 fixedItems中去
        if #randItems > 0 then
            local randItem
            local totalRate = 0
            for i, v in ipairs(randItems) do
                totalRate = totalRate + v.rate
            end
            local randRate = math.random(1, totalRate)

            for i, v in ipairs(randItems) do
                if randRate <= v.rate then
                    randItem = v
                    break
                else
                    randRate = randRate - v.rate
                end
            end
            table.insert(fixedItems, randItem)
        end
    end

    --设置倍数
    if rate > 1 then
        for _, item in ipairs(fixedItems) do
            if item.count and item.count ~= 0 then
                item.count = item.count * rate
            end

            if item.value and item.value ~= 0 then
                item.value = item.value * rate
            end
        end
    end

    for i, item in ipairs(fixedItems) do
        local type = item.type
        if type == COMMON.BONUS_TYPE.POWER then
            player.add_power(account_info, item.value, false)
            table.insert(notice_data, {COMMON.property_type.POWER, account_info.power})
            table.insert(add_data, {COMMON.property_type.POWER, item.value})
        end

        if type == COMMON.BONUS_TYPE.DIAMOND then
            player.change_diamond(account_info, src, item.value, false)
            table.insert(notice_data, {COMMON.property_type.DIAMOND, account_info.diamond})
            table.insert(add_data, {COMMON.property_type.DIAMOND, item.value})
        end

        if type == COMMON.BONUS_TYPE.SCORE then
            player.change_score(account_info, item.value, false, src)
            table.insert(notice_data, {COMMON.property_type.SCORE, account_info.pvp_score})
            table.insert(add_data, {COMMON.property_type.SCORE, item.value})
        end

        if type == COMMON.BONUS_TYPE.ITEM then
            local ret, it, itcfg = agent_item.add_item(item.itemId, item.count, src, false)
            if ret then
                table.insert(notice_data, {itcfg.property_type, it.count, it.itemId})
                table.insert(add_data, {itcfg.property_type, item.count, it.itemId})
            end
        end

        if type == COMMON.BONUS_TYPE.STUFF then
            local ret, it = agent_item.add_item(item.itemId, item.count, src, false)
            if ret then
                table.insert(notice_data, {COMMON.property_type.STUFF, it.count, item.itemId})
                table.insert(add_data, {COMMON.property_type.STUFF, item.count, item.itemId})
            end
        end

        if type == COMMON.BONUS_TYPE.ROOMTICKET then
            player.add_room_ticket(account_info, item.value, false)
            table.insert(notice_data, {COMMON.property_type.ROOM_TICKET, account_info.room_ticket})
            table.insert(add_data, {COMMON.property_type.ROOM_TICKET, item.value})
        end

        if type == COMMON.BONUS_TYPE.TITLE then
            player.add_title(account_info, item.value)
            table.insert(add_data, {COMMON.property_type.TITLE, item.value})
        end
    end

    if #notice_data > 0 then
        notice_player(notice_data)
    end

    -- 合并
    local combine_data = {}
    for _, v in ipairs(add_data) do
        local isExist = false
        for _, v1 in ipairs(combine_data) do
            if v1[1] == v[1] then
                if v1[1] == COMMON.property_type.ITEM or v1[1] == COMMON.property_type.STUFF then
                    if v1[3] == v[3] then
                        isExist = true
                        v1[2] = v1[2] + v[2]
                    end
                else
                    isExist = true
                    v1[2] = v1[2] + v[2]
                end
            end
        end
        if not isExist then
            table.insert(combine_data, v)
        end
    end
    return combine_data
end

local function get_next_level_exp(level, max_level)
    local exp_value = 0
    if level + 1 <  max_level then
        local exp_stat = sharedata_utils.get_stat(COMMON.plan.LEVEL, level + 1)
        if exp_stat then
            logger.logf("the d% level config not exist ", level)
            return
        end
        exp_value = exp_stat.exValue
    end

    return exp_value
end


local function calc_exp_level(exp, level, max_level)
    local stat = sharedata_utils.get_stat(COMMON.plan.LEVEL, level)
    local level_exp = stat.exValue

    if exp >= level_exp  and level <= max_level then
        level = level + 1
        exp = exp - level_exp
        return calc_exp_level(exp, level, max_level)
    end

    return level, exp
end

function player.add_exp(account_info, exp, src)
    if exp <= 0 then
        return
    end

    --读取配置文件，关卡等级经验
    local stat = sharedata_utils.get_stat_all(COMMON.plan.LEVEL)
    local max_level = stat["max_level"]

    local level = account_info.level or 1
    account_info.exp = account_info.exp + exp
    local current_exp = 0
    level, current_exp = calc_exp_level(account_info.exp, level, max_level)

    local level_up = 0 -- 等级是否升级
    -- 检测是否升级
    if level > account_info.level then
        EventCenter.dispatchEvent(Events.PLAYER_LEVEL_CHANGE, level)
        level_up = 1
    end

    account_info.level = level
    account_info.exp = current_exp

    -- oss记录
    skynet.send(busilog_service, "lua", "log_exp", account_info, level_up, exp, src)

    local level_data = {
        level = level,
        exp   = current_exp
    }
    local data = json.encode(level_data)
    notice_player({{COMMON.property_type.EXP, data}})

    player.change_flag(account_info)
end

function player.get_christmas_config()
    check_redis()
    local activity_config = sharedata_utils.getActivityConfigById(COMMON.activity_id.ACT_CHRISTMAS)
    return activity_config
end

function player.change_vip( account_info, vip_level )
    if account_info.vip_level ~= vip_level then
        local old = account_info.vip_level
        account_info.vip_level = vip_level
        player.change_flag(account_info, true)
        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_vip, account_info.uin, account_info.vip_level, old)
    end
end

function player.change_online_time(account_info)
    if account_info.login_out_time > account_info.login_time then
        account_info.online_time = account_info.online_time + (account_info.login_out_time - account_info.login_time)
        if account_info.online_time == (account_info.login_out_time - account_info.login_time) then
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_first_play_time, account_info.uin, account_info.online_time)
        end
        if account_info.login_time >= account_info.last_day_login_time then
            account_info.day_onilne_time = account_info.day_onilne_time + (account_info.login_out_time - account_info.login_time)
        else
            account_info.day_onilne_time = account_info.day_onilne_time + (account_info.login_out_time - account_info.last_day_login_time)
        end
    end
end

function player.init(args)
    send_package    = args.send_package
    send_response   = args.send_response
    send_error_code = args.send_error_code

    redis_service   = args.redis
    busilog_service = args.busilog_service

    redis_call      = args.rediscall
    
    agent_player = require "agent_player"
end

return player

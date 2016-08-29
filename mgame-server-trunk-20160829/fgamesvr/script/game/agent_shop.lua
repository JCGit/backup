--  agent_shop.lua
--  荣誉商店

local agent_shop = {}

local skynet    = require "skynet"

local logger    = require "logger"
local proto     = require "proto"
local utils     = require "utils"
local COMMON    = require "common"
local MESSAGE   = COMMON.MESSAGE
local ERRORCODE = COMMON.ERRORCODE
local setting   = require "setting"

local sharedata_utils    = require "sharedata_utils"
local agent_vip          = require "agent_vip"
local agent_pve          = require 'agent_pve'
local agent_item         = require "agent_item"
local agent_operate_activity = require "agent_operate_activity"
local agent_assistant    = require "agent_assistant"
local json = require "cjson"
local callback_service

local send_response, send_error_code
local account_info, uin, redis_service, player

local function update_exchange_limit(giftid, end_str, value)
    local lua_script = [[
local hash_table = KEYS[1]
local hash_key = ARGV[1]
local c = redis.call('hget', hash_table, hash_key)
if c then
    local ic = tonumber(c)
    if ic > 0 then
        local hash_value = ic - 1
        redis.call('hset', hash_table, hash_key, hash_value)
        return hash_value
    else
        return 0
    end
else
    local hash_value = tonumber(ARGV[2])
    redis.call('hset', hash_table, hash_key, hash_value)
    return hash_value
end
]]

    local key = string.format('%d:%s', giftid, end_str)
    local remain = skynet.call(redis_service, 'lua',
           'eval', lua_script, 1, 'exchange_limit', key, value)
    return remain
end

local function handle_update_exchange_list(cmdid, content, len)

    local cfg_data = sharedata_utils.get_stat_all(COMMON.plan.ExchangeData)
    local exi = skynet.call(redis_service, 'lua', 'hget_obj', 'exchange_info', uin)
    exi = exi or { data = {} }
    local now = os.time()

    -- 返回数据给前端
    local result = { ExchangeItem = {}, HonorCount = account_info.pvp_score }
    local items = result.ExchangeItem
    for k, v in pairs(cfg_data or {}) do
        -- 每一种兑换的商品是有起止时间的
        repeat
            if v.beginAt <= now and now < v.endAt then
                local item = { ItemId = k }
                local ei = exi.data[k]
                if ei then
--[[
example :
            开始时间           结束时间         销售时长(小时)        再次上架时间(天)
        　   beginAt             endAt            saleTime                upValue
            #20160205000000    #203800000000       24                      2

|       商品上架       |    商品下次上架时间   |     商品再次上架的结束时间 |    商品下架　 |
2016.2.5        2016.2.6                2016.2.8                     2016.2.9       2016.2.11
                    cur_end              next_begin                      cur_end      next_begin
]]

                    if not ei.cur_end or v.beginAt > ei.cur_end then -- 当前商品销售的结束时间
                        ei.cur_end = v.beginAt + v.saleTime * 60 * 60
                        ei.limited = v.limited
                    end

                    if not ei.next_begin or v.beginAt > ei.next_begin then -- 商品下次上架的时间
                        ei.next_begin = ei.cur_end + v.upValue * 24 * 60 * 60
                    end

                    -- 如果beginAt+saleTime*60*60的时间(cur_end)小于当前时间(now)
                    -- 并且next_begin时间小于当前时间(now),说明上一个销售周期已经结束
                    if now > ei.cur_end and now > ei.next_begin then
                        ei.cur_end = utils.getSameDayBeginTime() + v.saleTime * 60 *60
                        ei.next_begin = ei.cur_end + v.upValue * 24 * 60 * 60
                        ei.limited = v.limited
                    end

                    if now > ei.cur_end and now < ei.next_begin then -- 商品不在上架时间内
                        break
                    end

                    local all_limit = '' -- 此商品在上架期间内所有人的可兑换的限数的key
                    -- 商品处于当前上架时间内
                    if v.beginAt < now and now <= ei.cur_end then
                        item.BuyLeftCount = ei.limited -- 商品对应的兑换限数
                        all_limit = os.date('%Y%m%d', ei.cur_end)
                    -- 商品在再一次上架的时间内则更新
                    elseif now >= ei.next_begin and ei.next_begin < v.endAt and now > ei.cur_end then 
                        ei.cur_end = ei.next_begin + v.saleTime * 60 * 60 -- 更新商品当前上架商品销售的结束时间
                        ei.next_begin = ei.cur_end + v.upValue * 24 * 60 * 60 -- 更新商品下次上架时间
                        ei.limited = v.limited   -- 重新上架更新个人限制(每个人对此商品的兑换限数)
                        item.BuyLeftCount = v.limited -- 重新计算此商品对应的兑换限数
                        all_limit = os.date('%Y%m%d', ei.cur_end)
                    else
                        logger.debug(v.giftId, v.beginAt, v.endAt, now, ei.cur_end, ei.next_begin)
                        break
                    end
                    -- 剩余销售时间(s)
                    item.RefreshTime = ei.cur_end - now
                else
                    exi.data[k] = {}
                    local cache = exi.data[k]
                    cache.cur_end = v.beginAt + v.saleTime * 60 * 60
                    cache.next_begin = cache.cur_end + v.upValue * 24 * 60 * 60
                    cache.limited = v.limited
                    -- 如果beginAt+saleTime*60*60的时间(cur_end)小于当前时间(now)
                    -- 并且next_begin时间小于当前时间(now),说明上一个销售周期已经结束
                    if now > cache.cur_end and now > cache.next_begin then
                        cache.cur_end = utils.getSameDayBeginTime() + v.saleTime * 60 *60
                        cache.next_begin = cache.cur_end + v.upValue * 24 * 60 * 60
                    end
                    item.BuyLeftCount = v.limited
                    item.RefreshTime = cache.cur_end - now
                end
                table.insert(items, item)
            end
        until true
    end

    if next(exi.data) then
        local exi = skynet.call(redis_service, 'lua', 'hset_obj', 'exchange_info', uin, exi)
    end
    send_response(cmdid, result)
end

-- 荣誉值兑换奖励
-- NOTE: 客户端保证会在兑换奖励前先调用上面的update功能
local function handle_exchange_item(cmdid, content, len)

    local eid = proto.parse_proto_req(cmdid, content, len).ItemId

    local cd = sharedata_utils.get_stat(COMMON.plan.ExchangeData, eid)
    if not cd then
        logger.trace('invalid exchange id ', eid)
        return send_error_code(cmdid, ERRORCODE.PVP_EXCHANGE_ID_INVALID)
    end

    local now = os.time()
    if cd.beginAt > now or cd.endAt < now then
        return send_error_code(cmdid, ERRORCODE.PVP_EXCHANGE_NO_SELL)
    end

    local c_pvp_score = account_info.pvp_score
    local reduce_score = cd.honorPoint
    local open, cfg = agent_operate_activity.check_open(COMMON.OP_ACT_TYPE.SCORE_EXCHANGE)
    local dis = open and cfg.shop_dis[tostring(eid)]
    if open and dis then
        reduce_score = math.ceil(reduce_score * tonumber(dis))
    end
    -- 先判断荣誉值是否足够
    if c_pvp_score >= reduce_score then

        local exi = skynet.call(redis_service, 'lua', 'hget_obj', 'exchange_info', uin)
        exi = exi or { data = {} }
        if not exi.data[eid] then
            exi.data[eid] = {
                limited = cd.limited, -- 本轮剩余兑换次数
                cur_end = cd.beginAt + cd.saleTime * 60 * 60,
                next_begin = cd.beginAt + cd.saleTime * 60 * 60 + cd.upValue * 24 * 60 * 60
            }
        end

        local exchange_data = exi.data[eid]
        -- 有可能商品已经下架
        if now > exchange_data.cur_end and now < exchange_data.next_begin then
            return send_error_code(cmdid, ERRORCODE.PVP_EXCHANGE_OFF_SELL)
        end

        -- 全服限量暂时不做
        --local remain = update_exchange_limit(cd.giftId, os.date('%Y%m%d', exchange_data.cur_end), cd.num)
        --if remain <= 0 then
            --return send_error_code(cmdid, ERRORCODE.PVP_EXCHANGE_SELL_OUT)
        --end

        -- 记录全服剩余可兑换的量(已经减1)
        -- exchange_data.all_limit = remain

        -- 判断本轮剩余购买次数
        local c_limit = exchange_data.limited
        if c_limit > 0 then
            -- 减少本轮剩余购买次数
            exchange_data.limited = c_limit - 1
            skynet.call(redis_service, 'lua', 'hset_obj', 'exchange_info', uin, exi)

            -- 减少荣誉值
            player.change_score(account_info, -reduce_score, true, COMMON.change_src.exchange)

            if cd.giftId ~= 0 then
                player.add_gift(account_info, cd.giftId, COMMON.change_src.exchange, reduce_score)
            end

            local data = {
                ExchangeItem = {
                    ItemId       = eid,
                    BuyLeftCount = c_limit - 1,
                    -- LimitCount   = remain, -- 全服限量暂时不做
                    RefreshTime  = exchange_data.cur_end - now,
                },
                HonorCount   = account_info.pvp_score, -- 兑换后的荣誉点
                Ret = ERRORCODE.OK,
            }
            return send_response(cmdid, data)
        else
            -- 剩余次数不够
            return send_error_code(cmdid, ERRORCODE.PVP_EXCHANGE_NO_MORE)
        end
    else
        -- 荣誉值不够,兑换失败
        return send_error_code(cmdid, ERRORCODE.PVP_EXCHANGE_NO_SCORE)
    end
end

-- 购买商城礼包
local function handle_buy_gift(cmd, content, len)
    local t = proto.parse_proto_req(cmd, content, len)
    if not (t and t.ShopId) then
        logger.trace("error request")
        return
    end
    local shop_id = t.ShopId
    local shop_data = sharedata_utils.get_stat(COMMON.plan.SHOP_DATA, shop_id)

    if not shop_data then
        return logger.trace("error shopid, no config data")
    end

    -- VIP购买不在此处处理
    if shop_data.type ~= 2 then
        return logger.trace("error shopid!!shop type isn't 2", shop_id)
    end

    local tnow = os.time()
    -- 只能在有效期内购买
    if tnow < shop_data.start_time or tnow > shop_data.end_time then
        return send_error_code(cmd, ERRORCODE.DISCOUNT_PERIOD_OVERDUE)
    end

    -- 检查钻石数量
    local need_diamond = shop_data.price
    if account_info.diamond < need_diamond then
        return send_error_code(cmd, ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR)
    end

    local left_count = shop_data.leftCount
    -- 为0时不限量，>0时限量
    if left_count > 0 then
        -- 尝试减少全局剩余数量
        local lua_script = [[
local key = KEYS[1]
local field = ARGV[1]
local c = redis.call('hget', key, field)
if c then
    local ic = tonumber(c)
    if ic > 0 then
        redis.call('hset', key, field, ic - 1)
        return 'ok'
    else
        return 'no_more'
    end
else
    redis.call('hset', key, field, tonumber(ARGV[2]) - 1)
    return 'ok'
end
]]
        local d1 = shop_data.start_time
        local d2 = shop_data.end_time
        local field = string.format('%d:%d:%d', shop_id, d1, d2)
        --local r = skynet.call(redis_service, 'lua', 'eval', lua_script, 1, 'shop_info', field, left_count)
        --if r ~= 'ok' then return send_error_code(cmd, ERRORCODE.SHOP_GIFT_COUNT_ERROR) end
    end

    player.change_diamond(account_info, COMMON.change_src.shop, -need_diamond)
    player.add_gift(account_info, shop_data.giftId, COMMON.change_src.shop)
    return send_response(cmd, { ShopId = shop_id })
end


local function get_item_price(str, count)
    for num, price in string.gmatch(str, "(%d+)#(%d+)") do
        if tonumber(num) == count then
            return tonumber(price)
        end
    end
    return nil
end

-- 购买道具 体力 精力 上限问题
local buy_value = setting.game_setting.buy_value
local function handle_buy_item_msg(cmd, content, len)
    local ret = ERRORCODE.OK
    local shop_data = proto.parse_proto_req(cmd, content, len)
    local itemId = shop_data.ItemId
    local price = shop_data.Price
    local count = shop_data.Count
    local from = shop_data.From
    local src = 0
    local count_str = ''
    local limit = nil

    --联想网络版临时购买bug修复
    local buy_data = sharedata_utils.get_stat_all(COMMON.plan.ItemBuy)
    for k,v in pairs(buy_data) do
        if itemId == v.name then
            count_str = v.costMoney
            limit = v.limit
            break
      end
    end

    -- 验证买的东西是不是存在配置文件里面 精力、体力不需要做此判断
    if itemId ~= 1002 then
        price = get_item_price(count_str, count)
        if not price then
            return send_error_code(cmd, ERRORCODE.ITEM_COUNT_ERROR)
        end
    end

    local discount = agent_vip.get_vip_privilege_param(6)
    if discount ~= 0 then
        price = math.floor(price*tonumber(discount)/10)
    end

    if from == 1 then
        src = COMMON.change_src.buy_item_pve
    elseif from == 2 then
        src = COMMON.change_src.buy_item_pvp
    elseif from == 0 then
        src = COMMON.change_src.buy_item
    else
        logger.debugf("handle_buy_item_msg() error from[%d]", from)
        return
    end

    if limit and limit ~= 0 then 
        local balance = agent_assistant.get_item_balance(itemId, limit)
        if balance <= 0 then
            return send_error_code(cmd, ERRORCODE.ITEM_LIMIT)
        else
            agent_assistant.reduce_item_balance(itemId, balance - 1)
        end
    end

    if count > 0 then
        if account_info.diamond >= price then
            if itemId == 1002 then --1002体力
                if player.add_power(account_info, count * buy_value) then
                   account_info.power_buy_count = account_info.power_buy_count + count
                   src = COMMON.change_src.buy_power
                else
                   ret = ERRORCODE.POWER_IS_MAX
                end
            else
                agent_item.add_item(itemId, count, src, true)
            end
        else
            ret = ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR
        end
    else
        ret = ERRORCODE.ITEM_COUNT_ERROR
    end

    if ret == ERRORCODE.OK then
        player.change_diamond(account_info, src, -price)
        return send_response(MESSAGE.CS_CMD_BUY_ITEM, {
            Ret    = ret,
            ItemId = itemId,
            Count  = count,
            Price  = price,
        })
    else
        return send_error_code(cmd, ret)
    end
end

-- 获取商品列表
local function handle_shop_list(cmd, content, len)
    local shop_data = sharedata_utils.get_stat_all(COMMON.plan.SHOP_DATA)

    -- 购买钻石，首次购买有赠送
    local recharge_gift = skynet.call(redis_service, "lua",
        "hget_obj", "player_recharge_gift", account_info.uin) or {}

    local tnow = os.time()

    -- 获取限量类礼包的全服剩余数量
    -- shop_info数据：
    -- 哈希表
    -- field是 shopid:开始日期:结束日期
    -- value是 剩余数量
    local function getServerRemainCount()
        local t = skynet.call(redis_service, 'lua', 'hgetall', 'shop_info')
        local data = {}
        for i = 1, #t, 2 do
            local shopid, d1, d2 = string.match(t[i], '(%d+):(%d+):(%d+)')
            shopid, d1, d2 = tonumber(shopid), tonumber(d1), tonumber(d2)
            if tnow >= d1 and tnow <= d2 then
                local sc = shop_data[shopid]
                if sc and sc.start_time == d1 and sc.end_time == d2 then
                    data[shopid] = tonumber(t[i + 1])
                end
            end
        end
        return data
    end

    local diamondList = {} -- 钻石列表
    local giftList    = {} -- 礼包列表

    local redis_data = getServerRemainCount()

    for k,v in pairs(shop_data) do
        if v.type == 1 then
            table.insert(diamondList, {
                ShopId = v.id,
                First = not recharge_gift[tostring(v.id)]
            })
        elseif v.type == 2 and tnow >= v.start_time and tnow <= v.end_time then
            -- 剩余数量，先读服务器保存的值，如果没有，读配置
            local left_count = redis_data[v.id] or v.leftCount
            if left_count > 0 then
                local price
                if v.price ~= '' then
                    price = tonumber(v.price)
                else
                    price = nil
                end
                table.insert(giftList, {
                    ShopId    = v.id,         -- 商城ID 配置文件的编号
                    GiftId    = v.giftId,     -- 礼品ID
                    StartTime = v.start_time, -- 开始时间
                    EndTime   = v.end_time,   -- 结束时间
                    LeftCount = left_count,   -- 全服剩余数量
                    GiftName  = v.name,       -- 礼包名
                    Discount  = v.discount,   -- 折扣
                    Price     = price,        -- 价格
                    GiftIcon  = v.giftIcon,   -- 礼包图标
                })
            end
        end
    end
    return send_response(cmd, {
        diamondList = diamondList, -- 钻石列表
        giftList    = giftList,    -- 礼包列表
    })
end

--[[
giftid      兑换的礼包id
end_str     当前销售结束时间
value       此商品全服可兑换的限数

redis : exchange_limit giftid:end_str value

return  当前全服可况换的剩余数量
]]
local function get_exchange_limit(giftid, end_str, value)
    local lua_script = [[
        local hash_table = KEYS[1]
        local hash_key = ARGV[1]
        local c = redis.call('hget', hash_table, hash_key)
        if c then
            return tonumber(c)
        else
            local hash_value = tonumber(ARGV[2])
            redis.call('hset', hash_table, hash_key, hash_value)
            return hash_value
        end
    ]]

    local key = string.format('%d:%s', giftid, end_str)
    local remain = skynet.call(redis_service, 'lua', 'eval', lua_script, 1, 'exchange_limit', key, value)
    return remain
end

-- 获取每日限额道具的列表(因为只有碎片有限额，暂时数据保存到将军模块,agent_assistant)
local function handle_limit_item_list(cmd, msg, sz)
    local cfg = sharedata_utils.get_stat_all(COMMON.plan.ItemBuy)
    assert(cfg, '获取COMMON.plan.ItemBuy失败')

    local rsp = { Items = {}}
    for k,v in pairs(cfg) do
        if v.limit ~= 0 then
            local item = {}
            item.ItemId = v.name
            item.Balance = agent_assistant.get_item_balance(v.name, v.limit) or v.limit
            item.Max = v.limit
            table.insert(rsp.Items, item)
        end
    end
    send_response(cmd, rsp)
end

local function handle_pay_success(cmd, msg, sz)
    local req = proto.parse_proto_req(cmd, content, len)
    local orderid = req.Orderid
    local receipt = req.Receipt
    local url = "https://buy.itunes.apple.com"
    local ad = "/verifyReceipt"
    local t = {}
    t["receipt-data"] = receipt
    local content = json.encode(t)
    local status, body = httpc.request("POST", url, ad, {}, {}, content)
    local ret = json.decode(body)
    utils.var_dump(ret)

    --[[
    local info = ret["latest_receipt_info"]
    local proid = info.product_id
    skynet.call(callback_service, "lua", "callback",
        account_info.uin, 1, info.transaction_id)
        ]]
end

local function add_cmd_handle_mapping(cmd_map)
    local M = MESSAGE

    -- 荣誉值兑换奖励
    cmd_map[M.CS_CMD_UPDATE_EXCHANGE_LIST] = handle_update_exchange_list
    cmd_map[M.CS_CMD_EXCHANGE_ITEM]        = handle_exchange_item
    cmd_map[M.CS_CMD_SHOP_BUY]             = handle_buy_gift
    cmd_map[M.CS_CMD_SHOP_LIST]            = handle_shop_list
    cmd_map[M.CS_CMD_BUY_ITEM]             = handle_buy_item_msg
    cmd_map[M.CS_LIMIT_ITEM_LIST]          = handle_limit_item_list
    cmd_map[M.CS_CMD_PAY_SUCCESS]          = handle_pay_success
end

local function daily_check(notice_flag)
    if notice_flag then
        handle_update_exchange_list(MESSAGE.CS_CMD_UPDATE_EXCHANGE_LIST)
    end
    
    agent_assistant.daily_check(notice_flag)
    handle_limit_item_list(MESSAGE.CS_LIMIT_ITEM_LIST)
end

local function register_events()
    EventCenter.addEventListener(Events.DAY_CHANGE, daily_check)
end

function agent_shop.daily_check(day_skip)

end

function agent_shop.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code

    account_info    = args.info
    uin             = args.uin
    redis_service   = args.redis
    player          = args.player

    add_cmd_handle_mapping(args.cmd_map)
    register_events()

    callback_service = skynet.queryservice("callback_service")
end

return agent_shop

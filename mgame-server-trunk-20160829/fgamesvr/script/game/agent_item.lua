--

--公共物品模块
--auth:diandian
--date:2016.3.7
--
--设计思路：
--  将玩家数据中所有的拥有属性抽象成一个个物品
--  用id和type做区分, 物品使用,增加,删除将以事件的形式通知相应模块。
--  同时相应和物品模块合作的模块也应该维护物品数据的正确性
--
--  ** 详细说明可以参看doc/下的`w-物品模块设计综述.md`


--redis data example
--{
--  1000={ItemId = 1000, count=1,},
--  2000={ItemId = 2000, count=2,},
--}

local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local sharedata_utils = require "sharedata_utils"
local COMMON = require "common"

local multicast_service

local items_types
local items_ids
local data_change_flag = false
local args
local busilog_service

local agent_item = {}

local function notice_player(notice_data, extra)
    local update_data = {}
    print(#notice_data)
    for k,v in pairs(notice_data) do
       local ptype, value, value2 = v[1],v[2],v[3]
       local data
       if ptype ~= COMMON.property_type.EXP and
           ptype ~= COMMON.property_type.TITLE then
           data = {
              UpType = ptype,
              Value = value,
              Value2 = value2,
           }
       else
           data = {
               UpType = ptype,
               Extra =  value
           }
       end
       table.insert(update_data, data)
    end
    logger.trace('player 属性变化 通知玩家')
    logger.trace(logger.dump(update_data))

    local cmdid = COMMON.MESSAGE.CS_CMD_PROPERTY_UPDATE
    args.send_response(cmdid, { UpdateData = update_data })
end

--检查物品是否过期
local function check_item_deadline()
    local now = os.time()
    for type, con in pairs(items_types) do
        for id, item in pairs(con) do
            local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
            if item_cfg then
                if item_cfg.endTime and item_cfg.endTime <= now then
                    con[id] = nil
                    data_change_flag = true
                end
            else
                con[id] = nil
                data_change_flag = true
            end
        end
    end
end

local function init_data()
    local data  = args.rediscall("hget_obj", "item", args.info.uin)
    items_types = {}
    items_ids = {}
    if data then
        for id, item in pairs(data) do
            id = tonumber(id)
            item.count = tonumber(item.count)
            item.itemId = tonumber(item.itemId)
            local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
            if item_cfg then
                if not items_types[item_cfg.type] then
                    items_types[item_cfg.type] = {}
                end
                items_types[item_cfg.type][id] = item
                items_ids[id] = item
            end
        end
    end
    check_item_deadline()
end

local function use_reward_item(item_cfg, count, src)
    local reward_id = tonumber(item_cfg.use_extra)
    if not reward_id then
        return {}
    end
    local rcfg = sharedata_utils.get_stat(COMMON.plan.Reward, reward_id)
    if not rcfg then
        logger.trace("use item reward cfg is not exist: ", reward_id)
        return {}
    end

    local list = utils.make_reward(rcfg, count)
    for id, item in pairs(list) do
        agent_item.add_item(id, item.count, src)
    end
    return list
end

local function getPropertyTypeItemCount(stype)
    local item_stype = COMMON.item_stype
    if stype == item_stype.DIAMOND then
        return args.info.diamond
    elseif stype == item_stype.POWER then
        return args.info.power 
    elseif stype == item_stype.ENERGY then
        return args.info.energy
    elseif stype == item_stype.SCORE then
        return args.info.pvp_score
    end
end

function agent_item.check_save_db()
    if data_change_flag then
        args.rediscall("hset_obj", "item", args.info.uin, items_ids)
        data_change_flag = false
    end
end

local function getPvpProp()
    local items = agent_item.get_items_by_btype(COMMON.item_btype.PVP)
    local data = {}
    for id, item in pairs(items) do
        table.insert(data, {ItemId = id, Count = item.count})
    end
    return data
end

local function add_cmd(CMD)
    CMD.get_items                           = function() return items_ids end
    CMD.getPvpProp                          = getPvpProp
end

function agent_item.init(args_)
    args = args_
    if not items_ids then
        init_data()
    end
    multicast_service = skynet.queryservice("multicast_service")
    busilog_service = args_.busilog_service

    add_cmd(args.cmd)
end

--
--@return ret, item, item_cfg, extra_add_items
function agent_item.add_item(id, count, 
    src, notice_flag, extra, is_virtual_real, extra_add_items)

    id = assert(tonumber(id), id)
    count = assert(tonumber(count), count)
    local ret, item, item_cfg, notice_data 
        = agent_item.item_on_add(id, count, src, notice_flag, is_virtual_real)
    if not ret then
        return false
    end

    --如果是属性类型， 不参与记录
    local extra_add_items_t = extra_add_items or {}
    if extra_add_items then
        local cb = {}
        cb[id] = { itemId = id, count = count,}
        utils.combine_items(extra_add_items_t, cb)
    end
    
    if item_cfg.b_type == COMMON.item_btype.PROPERTY then
        EventCenter.dispatchEvent(Events.ITEM_ON_ADD, 
            count, item, item_cfg, extra, src, notice_flag, extra_add_items_t)
        item.count = getPropertyTypeItemCount(item_cfg.s_type)
        return true, item, item_cfg, extra_add_items_t
    end

    if notice_flag and not item_cfg.is_virtual then
        table.insert(notice_data, {item_cfg.property_type, item.count, id})
        notice_player(notice_data)
    end
    
    if item_cfg.is_virtual then
        if item_cfg.virtual_real then
            agent_item.add_item(item_cfg.virtual_real, 
                count, src, notice_flag, item_cfg.use_extra, true, extra_add_items_t)
        end
    else
        EventCenter.dispatchEvent(Events.ITEM_ON_ADD,
                count, item, item_cfg, extra, src, notice_flag, extra_add_items_t)
    end
    return true, item, item_cfg, extra_add_items_t
end

function agent_item.reduce_item(id, count, src, notice_flag, extra, extra_reduce_items)
    id = tonumber(id)
    count = tonumber(count)

    local ret, item, item_cfg = agent_item.item_on_reduce(id, count, src, notice_flag)
    if not ret then
        return false
    end
    
    local extra_reduce_items_t = extra_reduce_items or {}
    local cb = {}
    cb[id] = { itemId = id, count = count,}
    utils.combine_items(extra_reduce_items_t, cb)
    
     --如果是属性类型， 不参与记录
    if item_cfg.b_type == COMMON.item_btype.PROPERTY then
        EventCenter.dispatchEvent(Events.ITEM_ON_REDUCE, 
                count, item, item_cfg, extra, src, notice_flag, extra_reduce_items_t)
        return true, item, item_cfg, extra_reduce_items_t
    end
    
    EventCenter.dispatchEvent(Events.ITEM_ON_REDUCE, 
            ount, item, item_cfg, item_cfg.use_extra, src, notice_flag, extra_reduce_items_t)

    if notice_flag then
        notice_player({{item_cfg.property_type, item.count, id}})
    end
	return ret, item, item_cfg, extra_reduce_items_t
end

function agent_item.use_item(id, count, src, notice_flag)
    id = tonumber(id)
    count = tonumber(count)
    if count < 1 then
       logger.trace("use item count !", count)
       return false
    end

    local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
    if not item_cfg then
        logger.trace("use_item item_cfg id is not exist", args.info.uin, id)
        return false
    end

    local type_con = items_types[item_cfg.type]
    if not type_con then
        logger.trace("use_item item id is not exist", 
            args.info.uin, id, item_cfg.type)
        return false
    end

    local item = type_con[id]
    if not item then
        logger.trace("use_item item id is not exist", args.info.uin, id)
        return false
    end

    if item.count < count then
        logger.trace("use_item item count is not enough", args.info.uin, id, count)
        return false
    end
	
    agent_item.reduce_item(id, count, src, notice_flag)

    local use_reward
    if item_cfg.be_used then
        use_reward = {}
        --如果是开启奖励类型则直接打开
        if item_cfg.b_type == COMMON.item_btype.REWARD then
           use_reward = use_reward_item(item_cfg, count, src)
        else
            EventCenter.dispatchEvent(Events.ITEM_ON_USE,
                count, item, item_cfg, item_cfg.use_extra, src, notice_flag)
        end
    end

    if notice_flag and use_reward then
        agent_item.notice_ids(use_reward)
    end

    return true, use_reward
end

function agent_item.get_items_by_type(type)
    return items_types[type]
end

function agent_item.get_items_by_btype(btype)
    local data = {}
    for type, con in pairs(items_types) do
        local bt = math.floor(type / 10000)
        if bt == btype then
            for id, item in pairs(con) do
                data[id] = item
            end
        end
    end
    return data
end

function agent_item.get_all_client_bag_item()
    local dat = {}
    for id, item in pairs(items_ids) do
        local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
        if item_cfg and item_cfg.is_client_bag then
            dat[id] = item
        end
    end
    return dat
end

function agent_item.get_item(id)
    return items_ids[id]
end

function agent_item.del_item_by_id(id, src, notice_flag, extra)
	if items_ids[id] then
		return agent_item.reduce_item(id, items_ids[id].count, src, notice_flag, extra)
	end
	return false
end

function agent_item.get_item_count(id)
	if not items_ids[id] then
		return 0
	end
	return items_ids[id].count
end

function agent_item.del_item_by_type(type, src, notice_flag, extra)
	if items_types[type] then
		local con = items_types[type]
		for id, item in pairs(con) do
			agent_item.reduce_item(id, item.count, src, notice_flag, extra)
		end
		return true
	else
		return false
	end
end

function agent_item.on_item_del(id, src, notice_flag)
	local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
	if item_cfg then
		if items_ids[id] then
			return agent_item.on_item_reduce(id, items_ids[id].count, src, notice_flag)
		end
	else
		logger.trace("warning: on_item_del faild, item_cfg is nil", id)
	end
	return false
end

local function check_real_item_can_add(id)
    local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
    if not item_cfg then
        return false
    end

    if item_cfg.only_one then
        local item = items_ids[id]
        if not item then
            return true
        end
        return false
    else
        return true
    end
end

function agent_item.check_can_add_item(id)
    return check_real_item_can_add(id)
    --[[
    local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
    if not item_cfg then
        logger.trace("check_can_add_item item id is not exist", id)
        return false
    end

    local item = agent_item.get_item(id)
    if not item then
        if item_cfg.is_virtual then
            return check_real_item_can_add(item_cfg.virtual_real)
        else
            return true
        end
    else
        if not item_cfg.is_virtual then
            return not item_cfg.only_one
        else
            return check_real_item_can_add(item_cfg.virtual_real)
        end
    end

    return true
    ]]
end

--查询是否可以消耗物品
--{[100001] = 10,}
function agent_item.check_can_cost(costs)
    local can_cost = true
    for id, count in pairs(costs) do
        local nc = agent_item.get_item_count(id)
        if nc < count then
            can_cost = false
        end
    end
    return can_cost
end

function agent_item.notice_ids(ids)
    local notice_data = {}
    for id, _ in pairs(ids) do
        local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
        if item_cfg and not item_cfg.is_virtual then
            if item_cfg.b_type ~= COMMON.item_btype.PROPERTY then
                local item = items_ids[id]
                if item then
                    table.insert(notice_data, {item_cfg.property_type, item.count, id})
                else
                    table.insert(notice_data, {item_cfg.property_type, 0, id})
                end
            else
                local count = 0
                if item_cfg.s_type == 1 then
                    count = args.info.diamond
                elseif item_cfg.s_type == 2 then
                    count = args.info.power 
                elseif item_cfg.s_type == 4 then
                    count = args.info.pvp_score
                end
                table.insert(notice_data, {item_cfg.property_type, count, id})
            end
        end
    end
    notice_player(notice_data)
end

--当物品增加是调用，
--@is_virtual_real 代表是否是虚拟物品代理增加的真实物品 是函数内部实现用， 不要传值
function agent_item.item_on_add(id, count, src, notice_flag, is_virtual_real)
    id = tonumber(id)
    count = tonumber(count)
    if count < 1 then
        logger.trace("on add item count is error", args.info.uin, count)
        return false
    end

    local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
    if not item_cfg then
        logger.trace("on add item id config is not exist:", args.info.uin, id)
        return false
    end

    if item_cfg.b_type == COMMON.item_btype.PROPERTY then
        return true, {itemId = item_cfg.id, count = count, }, item_cfg
    end

    local type_con = items_types[item_cfg.type]
    if not type_con then
        type_con = {}
        items_types[item_cfg.type] = type_con
    end

    if item_cfg.only_one then
        if not is_virtual_real then
        if type_con[id] then
            logger.trace("on item is only one count:", args.info.uin, id)
            return false
        end
    end
    end

    --判断物品是否有同类型相抵触的等级
    local is_have_high_level = false
    local notices = {}
    if item_cfg.type_one ~= 0 then
        for k, v in pairs(type_con) do
            local v_cfg = sharedata_utils.get_stat(COMMON.plan.Items, k)
            if v_cfg.type_one <= item_cfg.type_one then
                type_con[k] = nil
                items_ids[k] = nil
                if notice_flag then
                    table.insert(notices, {COMMON.property_type.ITEM, k, 0})
                end
                data_change_flag = true
            else
                is_have_high_level = true
            end
        end
    end

    if not is_have_high_level then
        if not type_con[id] then
            type_con[id] = {
                itemId = id,
                count = 0,
            }
        end
    end

    if not type_con[id] then
        logger.trace("have high level item ", args.info.uin, id)
        if notice_flag then
            notice_player(notices)
        end
        return false
    end
    local item = type_con[id]
    items_ids[id] = item
    item.count = item.count + count
    data_change_flag = true

    if item_cfg.only_one then
        if item.count > 1 then
            item.count = 1
            --如果唯一物品数量已有，说明是替换use_extra属性
            EventCenter.dispatchEvent(Events.ITEM_REPLASE_EXTRA,
                item, item_cfg, src, notice_flag)
        end
    end

    --日志记录
    skynet.send(args.busilog_service, "lua", "log_item", args.info.uin, id, count, src)
    if not src then
        logger.trace("add item but do not have src", args.info.uin, id)
        --logger.trace(debug.traceback())
    end
    
    --获得会全服公告的物品
    if item_cfg.notice and not is_virtual_real and src then
        local pncfg = sharedata_utils.get_stat(COMMON.plan.ProduceName, src)
        if pncfg and not pncfg.abandon_notice then
            local pn = pncfg.name
            local notices = sharedata_utils.get_stat(COMMON.plan.Notice, 4)
            local notice_cfg = notices and notices[1] or nil
            if notice_cfg then
                local data = {
                     NoticeType = tonumber(notice_cfg.type),
                     Serious    = tonumber(notice_cfg.urgent_flag),
                     Content    = string.format(notice_cfg.desc,
                                    args.info.name, pn, item_cfg.name),
                     Title      = notice_cfg.title,
                     Count      = tonumber(notice_cfg.times),
                     TimeGap    = tonumber(notice_cfg.time_gap),
                }
                skynet.call(multicast_service, "lua", 
                    "publish_announce", COMMON.MESSAGE.CS_CMD_NOTICE, data)
            end
        end
    end

    logger.trace("add item success: ", args.info.uin, id, count)
    return true, item, item_cfg, notices

end

--档物品减少时调用
function agent_item.item_on_reduce(id, count, src, notice_flag)
    id = tonumber(id)
    count = tonumber(count)
    if count < 1 then
        logger.trace("on reduce item count is error", id, args.info.uin, count)
        return false
    end

    local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
    if not item_cfg then
        logger.trace("on reduce item id is not exist:", args.info.uin, id)
        return false
    end

    if item_cfg.b_type == COMMON.item_btype.PROPERTY then
        return true, {itemId = item_cfg.id, count = count, }, item_cfg
    end

    local type_con = items_types[item_cfg.type]
    if type_con then
        local item = type_con[id]
        if item then
            item.count = item.count - count
            if item.count <= 0 then
                item.count = 0
                type_con[id] = nil
                items_ids[id] = nil
                if not next(type_con) then
                    items_types[item_cfg.type] = nil
                end
            end
            data_change_flag = true
            skynet.send(args.busilog_service, "lua", "log_item", args.info.uin , id, -count, src)

            if item_cfg.is_virtual then
            else
                if item_cfg.virtual_type then
                    local vtype_con = items_types[item_cfg.virtual_type]
                    if vtype_con then
                        for id, it in pairs(vtype_con) do
                            agent_item.reduce_item(id, it.count, src, notice_flag)
                        end
                    end
                end
            end

            return true, item, item_cfg
        else
            logger.trace("on reduce item count is not exist:", args.info.uin, id)
            return false
        end
    else
        logger.trace("on reduce item count is not exist:", args.info.uin, id)
        return false
    end
end

return agent_item

local skynet  = require "skynet"
local utils   = require "utils"
local logger  = require "logger"
local COMMON  = require "common"
local proto   = require "proto"
local agent_item = require "agent_item"
local agent_pve  = require "agent_pve"
local sharedata_utils = require "sharedata_utils"
local agent_operate_activity = require "agent_operate_activity"
local agent_player  = require "agent_player"
local agent_vip     = require "agent_vip"

local redis_service
local busilog_service

local account_info
local player
local send_package, send_response, send_error_code


local agent_pvp_chest = {}
local datas

local luck_chest_status = {
    UNLOCKED = 0,
    LOCKED = 1
}

local const_normal_chest_type = 1
local const_gold_chest_type = 2
local const_luck_chest_type = 3
local goldChestId

local config
local const_luckCdTIme
local const_luckMaxCount
local const_goldChestRate
local const_slotFourCost
local const_slotFiveCost
local const_maxCDTime
local const_minCDTime
local const_timeCostRate

--[[竞技场宝箱数据格式
{
    [1000001] = {
        slot =
        {
            [1] = {
             open  = 1, -- 槽位是否开启 (0:false,1:true)
             id    = 0，-- 槽位宝箱id
            },
            [2] = {
             open  = 1, -- 槽位是否开启 (0:false,1:true)
             id    = 0，-- 槽位宝箱id
            },
            [3] = {
             open  = 1, -- 槽位是否开启 (0:false,1:true)
             id    = 0，-- 槽位宝箱id
            },
            [4] = {
             open  = 1, -- 槽位是否开启 (0:false,1:true)
             id    = 0，-- 槽位宝箱id
            },
            [5] = {
             open  = 1, -- 槽位是否开启 (0:false,1:true)
             id    = 0，-- 槽位宝箱id
            }
        },
        luck =
        {
            status = 0,
            time = 0,
            win = 0
        },
        value = 0, -- 累计冷却时间
        time  = 0, -- 冷却时间记录时间点,
        flag  = 0，-- 冷却时间下判断能不能打开宝箱标志
        count = 0, -- 黄金宝箱获取概率累计计数
        record = { -- 当天各宝箱的领取记录[只针对普通宝箱]
            id         =    count
        }
    },
}
 --]]

--可以优化成缓存 定时写
local function change_datas()
    if datas then
        skynet.call(redis_service, "lua", "hset_obj", "pvp_chest", account_info.uin, datas)
    end
end

function agent_pvp_chest.daily_login_check(flag)
    if datas then
        datas.record = {}
    end
    change_datas()
end

local function getChestIdByPoint(type, point)
    for _,v in pairs (config) do
        if v.type == type then
            local list =  utils.split(v.condition, "|")
            if #list == 2 and point >= tonumber(list[1]) and point <= tonumber(list[2]) then
                return tonumber(v.id),tonumber(v.type)
            end
        end
    end
    return 0,0
end

function agent_pvp_chest.on_rand_end(win, isProtected)
    local id, type

    if os.time() - datas.luck.time >= const_luckCdTIme and datas.luck.win < const_luckMaxCount then
        if win == 1 then
            datas.luck.win = datas.luck.win + 1
        else
            if not isProtected then
                datas.luck.win = 0
            end
        end
    end

    if win == 1 then
        if datas.count >= 3 then
            if datas.count >= 8 then
                type = const_gold_chest_type
                id = goldChestId
            else
                local number = math.random(1, 10000)
                if number <= const_goldChestRate + datas.count * 500 then
                    type = const_gold_chest_type
                    id = goldChestId
                end
            end
        end

        --type = const_gold_chest_type
        --id = goldChestId

        for i=1,5 do
            if datas.slot[i].open == 1 and datas.slot[i].id == 0 then
                if not id then
                    id, type = getChestIdByPoint(const_normal_chest_type, account_info.pvp_point)
                end
                datas.slot[i].id = id
              --  notice_normal = true
                break
            end
        end

        if type == const_gold_chest_type then
            datas.count = 0
        else
            datas.count = datas.count + 1
        end

    end
    change_datas()
    return id,type
end

local function init_conf()
    config =  sharedata_utils.get_stat_all(COMMON.plan.pvp_chest)
    if config then
        for _,v in pairs (config) do
            if v.type == const_gold_chest_type then
                goldChestId = v.id
            end
        end
    end

    local confStr
    confStr = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVP_LUCK_CHEST_CD)
    const_luckCdTIme = confStr and confStr.value or 18000

    confStr = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVP_LUCK_CHEST_COUNT)
    const_luckMaxCount = confStr and confStr.value or 3

    confStr = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVP_NORMAL_CHEST_GOLD_RATE)
    const_goldChestRate = confStr and confStr.value or 500
    local vipRate = agent_vip.get_vip_privilege_param(7)
    if vipRate and vipRate ~= 0 then
        const_goldChestRate = vipRate
    end

    confStr = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVP_NORMAL_CHEST_SOLT_COST1)
    const_slotFourCost = confStr and confStr.value or 50

    confStr = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVP_NORMAL_CHEST_SOLT_COST2)
    const_slotFiveCost = confStr and confStr.value or 300

    confStr = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVP_NORMAL_CHEST_MAX_TIME)
    const_maxCDTime = confStr and confStr.value or 14400
    local vipCD = agent_vip.get_vip_privilege_param(3)
    if vipCD and vipCD ~= 0 then
        const_maxCDTime = vipCD
    end

    confStr = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVP_NORMAL_CHEST_MIN_TIME)
    const_minCDTime = confStr and confStr.value or 7200
    local minCD = agent_vip.get_vip_privilege_param(4)
    if minCD and minCD ~= 0 then
        const_minCDTime = minCD
    end

    confStr = sharedata_utils.get_stat(COMMON.plan.Const, COMMON.const.PVP_NORMAL_CHEST_COST_RATE)
    const_timeCostRate = confStr and confStr.value or 120

end

local function init_data()
    datas = skynet.call(redis_service, "lua", "hget_obj", "pvp_chest",  account_info.uin)
    if not datas then
        datas = {}
        local slot = {}
        for i = 1, 5 do
            slot[i] = {}
            slot[i]["open"] = (i <= 3) and 1 or 0   --默认开启三个槽
            slot[i]["id"] = 0
        end

        local luck = {}
        luck.status = luck_chest_status.UNLOCKED
        luck.time = 0
        luck.win = 0

        datas.slot = slot
        datas.luck = luck
        datas.value = 0
        datas.time = 0
        datas.flag = 0
        datas.count = 0
        datas.record = {}
        change_datas()
    end
end

-- 读const配置
local function getDaimondBySlotIndex(i)
    if i == 4 then
        return const_slotFourCost
    elseif i == 5 then
        return const_slotFiveCost
    else
        return 0
    end
end

local function getLuckChestInfo()
    local tmp = {}
    tmp.progress = datas.luck.win
    if datas.luck.status == luck_chest_status.UNLOCKED then
        tmp.countdown = 0
    else
        tmp.countdown = (os.time() <= datas.luck.time + const_luckCdTIme) and datas.luck.time + const_luckCdTIme or 0
    end
    return tmp
end

local function getNormalChestInfo()
    local tmp ={}
    tmp.chestList = {}

    for i=1,5 do
        local slot = {}
        slot.id = datas.slot[i].id
        if datas.slot[i].open == 1 then
            slot.diamond = 0
        else
            slot.diamond = getDaimondBySlotIndex(i)
        end

        if slot.id == 0 then
            slot.name = ""
            slot.cd = 0
        else
            slot.name = config[slot.id]["name"]
            local hadTakeCount = datas.record[slot.id] or 0
            local addPercent = hadTakeCount * config[slot.id].addPercent / 100
            if addPercent > 1 then addPercent = 1 end
            slot.cd = math.floor(config[slot.id].cd * (1 + addPercent)) -- 宝箱开启时间
        end

        table.insert(tmp.chestList, slot)
    end

    -- 读配置
    -- datas.time = (datas.time == 0) and os.time() or datas.time
    tmp.CdTime = (datas.value > (os.time() - datas.time)) and (datas.value + datas.time) or 0
    tmp.lock = (tmp.CdTime <= const_minCDTime) and 0 or datas.flag
    tmp.maxTime = const_maxCDTime
    tmp.unLockTime  = (tmp.lock == 1) and tmp.CdTime - const_minCDTime or 0
    return tmp
end

local function isCanOpenNormalChest()
    if datas.flag == 0 then
        return true
    end

    -- datas.time = (datas.time == 0) and os.time() or datas.time
    if datas.value - (os.time() - datas.time) <= const_minCDTime then
        return true
    end
    return false
end

local function handle_luck_chest_info(cmd, content, len)
    if not datas then
        return
    end

    local resp_data = {}
    resp_data.luckInfo = getLuckChestInfo()
    return send_response(cmd, resp_data)
end

local function reward_to_player(id)
    --local isOpen, conf = agent_operate_activity.check_open(COMMON.OP_ACT_TYPE.MULTIPLEREWARD)

    local subType
    if id < goldChestId then
        subType = 3
    elseif id == goldChestId then
        subType = 2
    else
        subType = 1
    end

    local isOpen, conf = agent_operate_activity.check_openBySubType(COMMON.OP_ACT_TYPE.MULTIPLEREWARD, subType)
    local rate = conf and conf.rewardRatio or 1
    local curRate = isOpen and tonumber(rate) or 1
    print ("curRate="..curRate)
    local rewardConfig = config[id]
    local score_value = 0
    local diamond_value = 0


    if rewardConfig.score and rewardConfig.score ~= "-1" then
        local list =  utils.split(rewardConfig.score, "|")
        if #list == 2  then
            math.randomseed(skynet.now())
            local add = math.random(tonumber(list[1]), tonumber(list[2])) * curRate
            local title_rate = agent_player.get_title_property_addup(
                COMMON.property_type.SCORE) or 0
            local title_add = 0
            if title_rate ~= 0 then
                title_add = math.floor(add * title_rate / 100)
            end
            local crazy_add = agent_operate_activity.get_score_add_up(add)
            local vip_add = 0
            if rewardConfig.type == 1 or rewardConfig.type == 2 then
                vip_add = agent_vip.get_vip_added(add)
            end
            score_value = add + title_add + crazy_add + vip_add
            player.change_score(account_info, score_value, false, COMMON.change_src.pvp_chest)
        end
    end

    if rewardConfig.diamond and rewardConfig.diamond ~= "-1" then
        local list =  utils.split(rewardConfig.diamond, "|")
        if #list == 2  then
            math.randomseed(skynet.now())
            local add = math.random(tonumber(list[1]), tonumber(list[2])) * curRate
            diamond_value = add
            player.change_diamond(account_info, COMMON.change_src.pvp_chest, add, false)
        end
    end

    local keyTable = {"item1", "item2", "item3", "item4", "item5" }

    local total_list = {}
    for _,v in pairs (keyTable) do
        if rewardConfig[v] and rewardConfig[v] ~= "-1" then
            local reward_group = sharedata_utils.get_stat(COMMON.plan.Reward, tonumber(rewardConfig[v]))
            local list = utils.make_reward(reward_group, 1)
            utils.combine_reward(total_list, list)
        end
    end

    for id, item in pairs(total_list) do
        item.count = item.count * curRate
        agent_item.add_item(id, item.count, COMMON.change_src.pvp_chest, false)
    end

    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_pvp_chest, account_info.uin, id)

    if score_value > 0 then
        total_list[COMMON.plugin_type.SCORE] = {
            itemId = COMMON.plugin_type.SCORE,
            count = score_value,
        }
    end

    if diamond_value > 0 then
        total_list[COMMON.plugin_type.DIAMOND] = {
            itemId = COMMON.plugin_type.DIAMOND,
            count = diamond_value,
        }
    end

    agent_item.notice_ids(total_list)
    return total_list
end

local function handle_luck_chest_open(cmd, content, len)
    if not datas then
        return
    end

    local info = getLuckChestInfo()
    if info.countdown > 0 or info.progress < const_luckMaxCount then
        return
    end

    local luckChestId = getChestIdByPoint(const_luck_chest_type, account_info.pvp_point)
    local list = reward_to_player(luckChestId)
    datas.luck.status = luck_chest_status.LOCKED
    datas.luck.time = os.time()
    datas.luck.win = 0
    change_datas()

    local resp_data = {}
    resp_data.ItemList = {}
    resp_data.luckInfo = getLuckChestInfo()

    for id, item in pairs(list) do
        local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
        local item_resp = {
            Id = id,
            Type = item_cfg.property_type,
            Count = item.count,
        }
        if item_cfg.is_virtual then
            item_resp.Id = item_cfg.virtual_real
        end
        table.insert(resp_data.ItemList, item_resp)
    end

    return send_response(cmd, resp_data)
end

local function handle_normal_chest_info(cmd, content, len)
    if not datas then
        return
    end

    local resp_data = {}
    resp_data.info = getNormalChestInfo()

    return send_response(cmd, resp_data)
end


local function handle_normal_chest_open(cmd, content, len)
     local decode = proto.parse_proto_req(cmd, content, len)
     if not datas then
         return
     end

     local flag = isCanOpenNormalChest()
     local costDiamond = 0

     if decode.idx ~= 6 then
         if not datas.slot[decode.idx] or datas.slot[decode.idx].id == 0 then
             return
         end

         if not flag then
             return
         end
     else
         if not flag then
             costDiamond =  math.ceil(config[goldChestId].cd / const_timeCostRate)
             if account_info.diamond < costDiamond then
                 print("costDiamond:", costDiamond)
                 return
             end
         end
     end

     local chestId = decode.idx == 6 and goldChestId or datas.slot[decode.idx].id
     local list = reward_to_player(chestId)
     if costDiamond == 0 then
         local hadTakeCount = datas.record[chestId] or 0
         local addPercent = hadTakeCount * config[chestId].addPercent / 100
         if addPercent > 1 then addPercent = 1 end
         local addValue = math.floor(config[chestId].cd * (1 + addPercent))
         local oldValue = (os.time() < datas.time + datas.value) and (datas.time + datas.value - os.time()) or 0
         datas.time = os.time()
         datas.value = oldValue + addValue < 0 and 0 or oldValue + addValue
         datas.flag = datas.value >= const_maxCDTime and 1 or 0
         if decode.idx ~= 6 then
             datas.slot[decode.idx].id = 0
         end
        datas.record[chestId] = datas.record[chestId] and datas.record[chestId] + 1 or 1
     else
         player.change_diamond(account_info, COMMON.change_src.open_pvp_gold_chest, -costDiamond, true)
     end

     change_datas()

     local resp_data = {}
     resp_data.ItemList = {}
     resp_data.info = {}
     resp_data.info = getNormalChestInfo()

     for id, item in pairs(list) do
         local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
         local item_resp = {
             Id = id,
             Type = item_cfg.property_type,
             Count = item.count,
         }
         if item_cfg.is_virtual then
             item_resp.Id = item_cfg.virtual_real
         end
         table.insert(resp_data.ItemList, item_resp)
     end

     return send_response(cmd, resp_data)

end

local function handle_normal_chest_slot_buy(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    if not datas then
        return
    end

    if not datas.slot[decode.idx] or datas.slot[decode.idx].open == 1 then
        return
    end

    local cost = getDaimondBySlotIndex(decode.idx)
    if account_info.diamond < cost then
        return
    end

    player.change_diamond(account_info, COMMON.change_src.buy_pvp_chest_slot, -cost, true)
    datas.slot[decode.idx].open = 1
    change_datas()

    local resp_data = {}
    resp_data.info = {}
    resp_data.info = getNormalChestInfo()

    return send_response(cmd, resp_data)
end

local function handle_normal_chest_clear_time(cmd, content, len)
    if not datas or datas.time == 0 then
        return
    end

    local clearValue =  (os.time() >= datas.time) and (datas.value - (os.time() - datas.time)) or datas.value
    if clearValue <= 0 then
        return
    end

    local cost =  math.ceil(clearValue/const_timeCostRate)
    if account_info.diamond < cost then
        return
    end

    player.change_diamond(account_info, COMMON.change_src.clear_pvp_chest_time, -cost, true)
    datas.time = 0
    datas.value = 0
    datas.flag = 0
    change_datas()

    local resp_data = {}
    resp_data.info = {}
    resp_data.info = getNormalChestInfo()

    return send_response(cmd, resp_data)
end

-- vip 变更通知，重置参数
local function vip_update()
    local vipRate = agent_vip.get_vip_privilege_param(7)
    if vipRate and vipRate ~= 0 then
        const_goldChestRate = vipRate
    end

    local vipCD = agent_vip.get_vip_privilege_param(3)
    if vipCD and vipCD ~= 0 then
        const_maxCDTime = vipCD
    end

    local minCD = agent_vip.get_vip_privilege_param(4)
    if minCD and minCD ~= 0 then
        const_minCDTime = minCD
    end
end

local function register_events()
    EventCenter.addEventListener(Events.PLAYER_VIP_CHANGE, vip_update)
end

local function add_cmd_handle_mapping(handle_mapping)
    handle_mapping[COMMON.MESSAGE.CS_CMD_PVP_LUCK_CHEST_INFO]   = handle_luck_chest_info
    handle_mapping[COMMON.MESSAGE.CS_CMD_PVP_LUCK_CHEST_OPEN]   = handle_luck_chest_open
    handle_mapping[COMMON.MESSAGE.CS_CMD_PVP_NORMAL_CHEST_INFO] = handle_normal_chest_info
    handle_mapping[COMMON.MESSAGE.CS_CMD_PVP_NORMAL_CHEST_OPEN] = handle_normal_chest_open
    handle_mapping[COMMON.MESSAGE.CS_CMD_PVP_BUY_CHEST_SLOT]    = handle_normal_chest_slot_buy
    handle_mapping[COMMON.MESSAGE.CS_CMD_PVP_CLEAR_CHEST_TIME]  = handle_normal_chest_clear_time
end

function agent_pvp_chest.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code

    account_info  = args.info

    redis_service = args.redis
    busilog_service = args.busilog_service
    player = args.player
    add_cmd_handle_mapping(args.cmd_map)

    register_events()

    if not datas then
        init_data()
    end

    if not config then
        init_conf()
    end
end

return agent_pvp_chest

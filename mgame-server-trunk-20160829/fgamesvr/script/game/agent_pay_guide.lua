local skynet = require "skynet"
local utils = require "utils"
local logger = require "logger"
local sharedata_utils = require "sharedata_utils"
local COMMON = require "common"
local proto = require "proto"
local agent_item = require "agent_item"

local redis_service
local busilog_service

local account_info
local player
local send_package, send_response, send_error_code

local agent_pay_guide = {}
local datas

local conf_shield_price
local conf_pay_guide
local conf_pay_guile_type = {
    ALL = 0,
    PVP = 1,
    PVE = 2,
}

local flush = false

--[[
{
    [1000001] = {
        record = {
            1         =    {id,count,flag}
        }

        id = {
            id = 1,
        }
        flag = 0
    },
}
 --]]


function agent_pay_guide.check_save_db()
    if flush and datas then
        skynet.call(redis_service, "lua", "hset_obj", "pvp_pay_guide", account_info.uin, datas)
        flush = false
    end
end

function agent_pay_guide.daily_login_check(flag)
    if datas then
        datas.record = {}
        datas.id = {}
    end
    flush = true
end


local function init_data()
    datas = skynet.call(redis_service, "lua", "hget_obj", "pvp_pay_guide",  account_info.uin)
    if not datas then
        datas = {}
        datas.record = {}
        datas.id = {}
        datas.flag = 0
        flush = true
    end
end

local function getRandItem(type)
    local id = 0
    local count = 0

    if not datas or #datas.record >= 10 then
        return id,count
    end

    if not datas.id then
        datas.id = {}
    end

    if not conf_pay_guide then
        return id,count
    end

    local tab = {}
    local totalRate = 0
    for _,v in pairs (conf_pay_guide) do
        if v.type == conf_pay_guile_type.ALL or v.type == type then
            if not datas.id[v.id] then
                table.insert(tab, v)
                totalRate = totalRate + tonumber(v.rate)
            end
        end
    end

    if totalRate <= 0 then
        return id,count
    end

    local randNumber = math.random(1, totalRate)
    local curTotal = 0
    local flag = false
    for _,v in pairs (tab) do
        curTotal = curTotal + tonumber(v.rate)
        if randNumber <= curTotal then
            id = tonumber(v.id)
            count = math.random(v.min, v.max)
            datas.record[#datas.record + 1] = {id, count, 1 }
            datas.id[id] = 1
            break
        end
    end

    flush = true
    return id,count
end

function agent_pay_guide.on_pve_lost()
    return getRandItem(conf_pay_guile_type.PVE)
end

function agent_pay_guide.on_pvp_lost()
    return getRandItem(conf_pay_guile_type.PVP)
end

-- pvp战斗失败付费引导是否已经购买
function agent_pay_guide.pvp_buy(id)
    if id and id > 0 then
        for _,v in pairs (datas.record) do
            if v[1] == id and v[3] == 1 then
                return true
            end
        end
    end
    return false
end

local function init_conf()
        conf_pay_guide = sharedata_utils.get_stat_all(COMMON.plan.PAY_GUIDE)
        conf_shield_price = sharedata_utils.get_stat_all(COMMON.plan.SHIELD_PRICE)
end

function agent_pay_guide.setShiled()
    datas.flag = 0
    flush = true
end

function agent_pay_guide.isShieldProtected()
    if datas and datas.flag == 1 then
        return true
    end
    return false
end

function agent_pay_guide.getShieldStatus()
    if datas and datas.flag == 1 then
        return 1
    end
    return 3
end

local function handle_pay_guide_buy(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    if not datas or #datas.record <= 0 then
        return send_error_code(cmd, COMMON.ERRORCODE.ITEM_NOT_EXIST)
    end

    if not conf_pay_guide[decode.itemId] then
        return send_error_code(cmd, COMMON.ERRORCODE.ITEM_NOT_EXIST)
    end

    local resp_data = {}

    local costDiamond = 0
    for _,v in pairs (datas.record) do
        if v[1] == decode.itemId and v[3] == 1 and v[2] > 0 then
            costDiamond = math.floor(conf_pay_guide[decode.itemId].price * v[2] * conf_pay_guide[decode.itemId].discount / 100)
            if account_info.diamond < costDiamond then
                return send_error_code(cmd, COMMON.ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR)
            end
            v[3] = 0
            flush = true
            agent_item.add_item(decode.itemId, v[2], COMMON.change_src.pay_guide_buy, true)
            player.change_diamond(account_info, COMMON.change_src.pay_guide_buy, -costDiamond, true)
            local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, decode.itemId)
            resp_data.Count =  v[2]
            resp_data.Type = item_cfg.property_type
            resp_data.ItemId = decode.itemId
        end
    end

    return send_response(cmd, resp_data)
end

local function handle_pay_guide_buy_shield(cmd, content, len)
    if not datas or datas.flag == 1 then
        return
    end

    if not conf_shield_price or not conf_shield_price[account_info.continue_win_count] then
        return
    end

    local costDiamond = conf_shield_price[account_info.continue_win_count].price
    if account_info.diamond < costDiamond then
        return
    end

    player.change_diamond(account_info, COMMON.change_src.pay_guide_buy_shield, -costDiamond, true)
    datas.flag = 1
    flush = true

    local resp_data = {}
    resp_data.ret = 1
    return send_response(cmd, resp_data)
end

local function add_cmd_handle_mapping(handle_mapping)
        handle_mapping[COMMON.MESSAGE.CS_CMD_PAY_GUIDE_BUY]   = handle_pay_guide_buy
        handle_mapping[COMMON.MESSAGE.CS_CMD_PAY_GUIDE_BUY_SHIELD]   = handle_pay_guide_buy_shield
end

function agent_pay_guide.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code

    account_info  = args.info

    redis_service = args.redis
    busilog_service = args.busilog_service
    player = args.player
    add_cmd_handle_mapping(args.cmd_map)

    if not datas then
        init_data()
    end

    if not conf_pay_guide then
        init_conf()
    end
end

return agent_pay_guide


--  新手礼包
local skynet                = require "skynet"
local utils                 = require "utils"
local sharedata_utils       = require "sharedata_utils"
local COMMON                = require "common"
local MESSAGE               = COMMON.MESSAGE
local ERRORCODE             = COMMON.ERRORCODE
local proto                 = require "proto"
local player                = require "player"

local agent_newbie = {}

local account_info
local agent
local redis_service
local send_response
local busilog_service

function agent_newbie.get_open_activity(open_activity)
    local gift_data  = sharedata_utils.get_stat(COMMON.plan.Newbie_Gift, 1)
    if not gift_data then return open_activity end
    local newbie_gift = skynet.call(redis_service, "lua", "hget_obj", "newbie_gift", account_info.uin, true)
    if newbie_gift == nil or newbie_gift["1"] == nil then
        local type = {
            Type = COMMON.activity_icon.ACT_NEWBIE_GIGT
        }
        table.insert(open_activity,type)
    end
   return open_activity
end

local function handle_buy_newbie_gift(cmd, content, len)
    local ret = ERRORCODE.OK
    local gift_data  = sharedata_utils.get_stat(COMMON.plan.Newbie_Gift, 1)
    local newbie_gift = skynet.call(redis_service, "lua", "hget_obj", "newbie_gift", account_info.uin, true)
    if newbie_gift == nil then newbie_gift = {} end
    if gift_data then
        if newbie_gift[tostring(gift_data.id)] == nil then
            if player.change_diamond(account_info,COMMON.change_src.newer, -gift_data.price) then
                local data = {
                    id = gift_data.id,
                    time = math.floor(os.time())
                }
                player.add_gift(account_info, gift_data.giftId,COMMON.change_src.newer)
                newbie_gift[tostring(gift_data.id)] = data
                skynet.call(redis_service, "lua", "hset_obj", "newbie_gift", account_info.uin, newbie_gift)
                skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_newbie_gift, account_info.uin, gift_data.id)
            else
                ret = ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR
            end
        else
            ret = ERRORCODE.ACTIVITY_RECEIVE_ERROR
        end
    else
        ret = ERRORCODE.ACTIVITY_DATA_ERROR
    end

    local resp = {
        Ret = ret
    }
    send_response(cmd, resp)
end

local function handle_set_newbie(cmd, content, len)
    local nb_info = proto.parse_proto_req(cmd, content, len)

    local newbie_list = skynet.call(redis_service, "lua", "hget_obj", "newbie_list", account_info.uin) or {}
    local change_flag = false

    if not account_info.NewBie_Complete then
        --如果已经完成新手引导 则重置新手标识list
        if nb_info.isComplete then
            account_info.NewBie_Complete = nb_info.isComplete
            newbie_list = {}
        else
            for _, v in pairs(nb_info.NewBieId) do
                newbie_list[v] = true
                change_flag = true
            end
        end
    end

    if change_flag then
        skynet.call(redis_service, "lua", "hset_obj", "newbie_list", account_info.uin, newbie_list)
    end

    send_response(cmd, {})
end


local function handle_newbie_list(cmd, content, len)
    local data = {}
    local list = {}
    local newbie_list = skynet.call(redis_service, "lua", "hget_obj", "newbie_list", account_info.uin) or {}

    if not account_info.NewBie_Complete then
        for k,v in pairs(newbie_list) do
            table.insert(list, k)
        end
    end

    local data = {
        isComplete = account_info.NewBie_Complete,
        NewBieIds = list
    }
    send_response(cmd, data)
end

-- 修改钻石 新手引导用
local function handle_shop_guide(cmd, content, len)
    local old_diamond = account_info.diamond
    local ret = ERRORCODE.OK

    -- 读id为 1 的道具的配置信息
    local itembuy = sharedata_utils.get_stat(COMMON.plan.ItemBuy, 1)
    local diamond = itembuy.moneynum

    if account_info.shop_guide == 0 then
        player.change_diamond(account_info, COMMON.change_src.newer_shop, diamond)
        account_info.shop_guide = 1
    end
    -- 如果加过了钻石，则只返回数字，实际上不加钻石

    local resp = {
        Ret = ret,
        Diamond = account_info.diamond
    }
    send_response(cmd, resp)
    logger.tracef("uin=%d&old_diamond=%d&now_diamond=%d", account_info.uin, old_diamond, account_info.diamond)
end

local function handle_setnewbie_oss(cmd, content, len)
    local req_data = proto.parse_proto_req(cmd, content, len)
    account_info.newbie_flag = req_data.NewBieId
    player.change_flag(account_info, true)
    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_newbie, account_info.uin, account_info.newbie_flag)
    local data = {
        NewBieId = req_data.NewBieId
    }
    send_response(cmd, data)
end

local function add_cmd(CMD)

end


local function add_cmd_handle_mapping(handle_mapping)
    handle_mapping[MESSAGE.CS_CMD_BUY_NEWBIE_GIFT] = handle_buy_newbie_gift
    handle_mapping[MESSAGE.CS_CMD_SET_NEWBIE]      = handle_set_newbie
    handle_mapping[MESSAGE.CS_CMD_GET_NEWBIE_LIST] = handle_newbie_list
    handle_mapping[MESSAGE.CS_CMD_SHOP_GUIDE]      = handle_shop_guide
    handle_mapping[MESSAGE.CS_CMD_SET_NEWBIE_OSS]  = handle_setnewbie_oss
end

function agent_newbie.init(args)
    redis_service   = args.redis
    account_info    = args.info
    agent           = args.agent
    send_response   = args.send_response
    busilog_service = args.busilog_service

    -- 模块必须实现的接口
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)
end

return agent_newbie


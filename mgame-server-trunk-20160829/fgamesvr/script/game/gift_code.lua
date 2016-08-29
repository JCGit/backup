local skynet = require "skynet"
local logger = require "logger"
local utils = require "utils"
local proto = require "proto"
local COMMON = require "common"
local player = require "player"
local sharedata_utils = require "sharedata_utils"
local account_info
local agent
local redis_service
local mail_service

local gift_code = {}

local send_package, send_response

-- TODO cdk生成放在了global_web里，如果redis不是本机或者和此redis不是一个 需要修改 注意一下
function gift_code.handle_use_gift_code(cmdid, msg, sz)
    local code = proto.parse_proto_req(cmdid, msg, sz).GiftCode
    local ret = COMMON.ERRORCODE.OK
    logger.tracef("=========    useKey     ======================")
    -- TODO 有可能分redis，注意下
    local exist_code = skynet.call(redis_service, "lua", "hget_obj", "gift_code", code)
    local giftId = 0
    if exist_code then
        if exist_code.playerData == "" then
            local exist_type = skynet.call(redis_service, "lua", "hget_obj", "gift_type", exist_code.giftName)
            if exist_type then
                local reward= exist_type.reward
                local typeId = exist_type.type
                local code_id = exist_code.id
                local reward_type = tonumber(exist_type.reward_type)
                logger.trace("reward:  ，typeId:  , code_id: ",reward, typeId , code_id)
                local player_gift_code = skynet.call(redis_service, "lua", "hget_obj", "player_gift_code", account_info.uin)
                if player_gift_code == nil then player_gift_code = {} end
                if typeId == 2 or player_gift_code[tostring(code_id)] == nil then
                    if typeId ~= 3 then -- 3为通用激活码，因此不修改激活码的数据
                        exist_code.playerData = account_info.uin
                        exist_code.useTime = math.floor(os.time())
                        skynet.call(redis_service, "lua", "hset_obj", "gift_code", code, exist_code)
                    end

                    if reward_type == nil or reward_type == 0 then
                        giftId = tonumber(reward)
                        if giftId ~= nil then
                            player.add_gift(account_info, giftId, COMMON.change_src.gift)
                        else
                            logger.trace("gift code is nil")
                        end
                    else
                        local mail = utils.clone(sharedata_utils.get_stat(COMMON.plan.Sys_Mail, COMMON.sys_mail_id.GIFT_CODE), true)
                        local now  = math.floor(skynet.time())
                        mail.sendtime = now
                        mail.outdate = now + mail.outdate
                        mail.plugin = reward
                        skynet.call(mail_service, "lua", "add_mail", account_info.uin, COMMON.mail_type.SINGLE, mail, COMMON.change_src.gift_code_mail)
                    end

                    local use_gift_code = {
                        type = tostring(code_id)
                    }
                   player_gift_code[tostring(code_id)] = use_gift_code
                   skynet.call(redis_service, "lua", "hset_obj", "player_gift_code", account_info.uin, player_gift_code)
                else
                    logger.tracef("this code type is used code: %s , typeId: %s , code_id : %s ", code , typeId, code_id)
                    ret = COMMON.ERRORCODE.GIFT_TYPE_IS_USED
                end
            else
                logger.trace("code type is nil")
                ret = COMMON.ERRORCODE.GIFT_TYPE_NOE_EXIST
            end
        else
            logger.trace("code is used")
            ret = COMMON.ERRORCODE.GIFT_CODE_IS_USED
        end
    else
        logger.trace("code is nil")
        ret =  COMMON.ERRORCODE.GIFT_CODE_NOE_EXIST
    end
    local resp = {
        Ret = ret,
        Reward = giftId
    }
    logger.tracef("handle_use_gift_code，uin,  ret: %d ， Reward： %s ", account_info.uin, ret, giftId)
    send_response(cmdid, resp)
end

local function add_cmd(CMD)
end

local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_USE_GIFT_CODE] =  gift_code.handle_use_gift_code
end

function gift_code.init(args)
    redis_service = args.redis
    mail_service = skynet.queryservice("mail_service")
    account_info = args.info
    agent = args.agent

    send_package = args.send_package
    send_response = args.send_response

    -- 实现接口
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)
end

return gift_code

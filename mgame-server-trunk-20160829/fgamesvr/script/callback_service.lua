-- 支付 中心服回调节点服务器
-- 不在查询redis,也不再修改redis订单信息
-- 走到此处的订单，都将当作正确的订单处理
local skynet = require "skynet"
local logger = require "callback_logger"
local proto  = require "proto"
local COMMON = require "common"

local sharedata_utils = require "sharedata_utils"

local redis_service, gamed

local CMD = {}

function CMD.callback(uin, shopId, orderId, pay_amount, pay_currency)
    logger.tracef("uin: %d , shopId: %d OrderId: %s", uin, shopId, orderId)
    local shop_data = sharedata_utils.get_stat(COMMON.plan.SHOP_DATA, shopId)
    if shop_data == nil then
        logger.tracef("shopdata: %d is nil" , shopId)
        return "fail"
    end
    if shop_data.diamond <= 0 then
        logger.tracef("recharge data error, shop_data %d is invalid。 ", shopId)
        return "fail"
    end
    local agent = skynet.call(gamed, "lua", "get_alive_agent", uin)
    if agent then -- 有可能玩家下线了，保存数据再下次登录的时候另行通知前端
        local ret = skynet.call(agent, "lua", "recharge_change_diamond", shop_data.manual, shop_data.diamond, orderId, shopId, pay_amount, pay_currency)
        if not ret then
            logger.tracef("recharge callback fiald ,data: %d, %d, %s ", uin, shopId, orderId)
            return "fail"
        end
        local resp = { Ret = 0, Id = shop_data.id, OrderId = orderId, }
        local cmd = COMMON.MESSAGE.CS_CMD_RECHARGE_CALLBACK
        skynet.call(agent, "lua" , "send_pack", cmd, proto.pack_proto_resp(cmd, resp))
    else
        logger.tracef("offline =>uin: %d , shopId: %d OrderId: %s diamond: %d", uin, shopId, orderId, shop_data.diamond)
        local orders = skynet.call(redis_service, "lua", "hget_obj", "offline_recharge_order", uin)
        if not orders then
            orders = {}
        end
        local order = {
            diamond = shop_data.manual,
            amount = shop_data.diamond,
            order_id = orderId,
            shop_id = shopId,
            pay_amount = pay_amount,
            pay_currency = pay_currency,
        }
        table.insert(orders, order)
        skynet.call(redis_service, "lua", "hset_obj", "offline_recharge_order", uin, orders)
    end
    logger.trace("recharge success")
    return "success"
end

local traceback = debug.traceback
skynet.start(function()
    proto.load_proto()

    gamed         = skynet.queryservice("gamed")
    redis_service = skynet.queryservice("redis_service")

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end

        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        skynet.retpack(ret)
    end)
end)

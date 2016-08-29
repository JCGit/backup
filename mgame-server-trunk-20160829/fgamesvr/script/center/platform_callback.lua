-- 平台充值回调
local skynet = require "skynet"
local logger = require "callback_logger"
--local logger = require "logger"
local crypt = require "crypt"
local redis_service
local utils = require "utils"
local cluster = require "cluster"
local platform_setting = require "platform_setting"
local CMD = {}
local syncserver
local COMMON = require "common"

local function get_sign(params)
    local key_test = {}
    for i in pairs(params) do
        table.insert(key_test, i) --提取test1中的键值插入到key_test表中
    end

    local sort_param
    table.sort(key_test)
    for _, v in pairs(key_test) do
        if v ~= "signature" then
            if not sort_param then
                sort_param = v .. "=" .. params[v] .. "&"
            else
                sort_param = sort_param .. v .. "=" .. params[v] .. "&"
            end
        end
    end
    return string.sub(sort_param, 0, string.len(sort_param) - 1)
end


local function get_params(data)
    local params = {}
    local param = utils.split(data, "&")
    for _, v in pairs(param) do
        local k_v = utils.split(v, "=")
        params[k_v[1]] = k_v[2]
    end
    return params
end


local function xiaomi_callback(data)
    logger.tracef("========= xiaomi call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    logger.tracef("=========xiaomi gameserver1: %s for uin : %d", gameserver, uin)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("=========xiaomi gameserver2: %s ", gameserver)
    end
    if gameserver then
        logger.tracef("xiaomi callback addr: %s  uin: %d , shopId: %d", gameserver, uin, shopId)
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        return "success"
    end
    return "callback fail"
    --[[
    logger.tracef("========= xiaomi call back request: %s", data)
    local params = get_params(data)
    --    appId 必须  游戏ID
    --    cpOrderId 必须  开发商订单ID
    --    cpUserInfo  可选  开发商透传信息
    --    uid 必须  用户ID
    --    orderId 必须  游戏平台订单ID
    --    orderStatus 必须  订单状态，TRADE_SUCCESS 代表成功
    --    payFee  必须  支付金额,单位为分,即0.01 米币。
    --    productCode 必须  商品代码
    --    productName 必须  商品名称
    --    productCount  必须  商品数量
    --    payTime 必须  支付时间,格式 yyyy-MM-dd HH:mm:ss
    --    orderConsumeType  可选  订单类型：10：普通订单11：直充直消订单
    --    partnerGiftConsume  可选  使用游戏券金额 （如果订单使用游戏券则有,long型），如果有则参与签名
    --    signature 必须  签名,签名方法见后面说明
    local sort_params = get_sign(params)
--    local appId = params["appId"]
    local cpOrderId = params["cpOrderId"]
    local cpUserInfo = params["cpUserInfo"]
--    local uid = params["uid"]
    local payTime = params["payTime"]
    local orderStatus = params["orderStatus"]
    local signature = params["signature"]
    if orderStatus ~= "TRADE_SUCCESS" then
        logger.tracef("========= orderStatus error: %s ", orderStatus)
        return "fail"
    end
    local my_signature = crypt.hexencode(crypt.hmac_sha1(platform_setting.xiaomi.AppSecret, sort_params))
    if (my_signature ~= signature) then
        logger.tracef("========= signature error   my: %s", my_signature)
        logger.tracef("========= signature error plat: %s", signature)
        return "{\"errcode\":1525}"
    end


    local callback_info = utils.split(cpUserInfo, "#")
    local uin = tonumber(callback_info[1])
    local shopId = tonumber(callback_info[2])
    if uin == nil or shopId == nil then
        logger.tracef("========= data error: %s ", cpUserInfo)
        return "fail"
    end
    if redis_service == nil then
        redis_service = skynet.queryservice("redis_service")
    end
    local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
    if account_info then 
        local orderId = cpOrderId .. "_" .. COMMON.platform[account_info.platform] -- 订单id_平台字符串 以保证唯一
        local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("========= gameserver1: %s ", gameserver)
        if gameserver == nil then
            gameserver = skynet.call(syncserver, "lua", "get_location", uin)
            logger.tracef("========= gameserver2: %s ", gameserver)
        end
        if gameserver then
            logger.tracef("callback addr: %s  uin: %d , shopId: %d", gameserver, uin, shopId)
            cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
            logger.tracef("========= callback success ")
            local recharge_data = {
                Uin = uin,
                ShopoId = shopId, -- 商品id
                OrderId = orderId,
                PayTime = payTime, -- 支付时间
                Time = math.floor(os.time()), -- 记录时间
                Status = "success" -- 处理状态
            }
            skynet.call(redis_service, "lua", "hset_obj", "recharge_order", orderId, recharge_data)
            return "{\"errcode\":200}"
        else
            logger.tracef("========= callback fail")
            -- 网游版的是否可以在此处理钻石增加问题   
        end
        
        local data = skynet.call(redis_service, "lua", "hget_obj", "recharge_order", orderId, true)
        if data == nil or data.Status ~= "success" then
            local recharge_data = {
                Uin = uin,
                ShopoId = shopId,
                OrderId = orderId,
                PayTime = payTime,
                Time = math.floor(os.time()),
                Status = "faild"
            }
            skynet.call(redis_service, "lua", "hset_obj", "recharge_order", orderId, recharge_data)
        end
    end 
    
    return "fail"
    --]]

end

--联网版 联想回调 不再去检查订单的成功状态，直接当作成功处理
local function lenovo_callback(data)
    logger.tracef("========= lenovo call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        logger.tracef("========= data nil error: %s ", data)
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    logger.tracef("========= gameserver1: %s ", gameserver)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("========= gameserver2: %s ", gameserver)
    end
    if gameserver then
        logger.tracef("callback addr: %s  uin: %d , shopId: %d", gameserver, uin, shopId)
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        logger.trace("========= callback success ")
        return "callback success"
    end
    logger.trace("========= callback fail")
    return "callback fail"
end


local function memoriki_callback(data)
    logger.tracef("========= memoriki call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local openId = params["openId"]
    local shopId = params["shopId"]
    local pay_amount = params["pay_amount"]
    local pay_currency = params["pay_currency"]
    if openId == nil or shopId == nil or orderId == nil then
        logger.tracef("========= data nil error: %s ", data)
        return "params error"
    end
    local redis_svr = skynet.queryservice("redis_service")
    local uin = skynet.call(redis_svr, "lua", "hget_obj", "uin_mapping", openId)
    if uin == nil then
        logger.tracef("=======openid %s don't found uin", openId)
        return "non openId"
    end

    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    logger.tracef("========= gameserver1: %s ", gameserver)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("========= gameserver2: %s ", gameserver)
    end
    if gameserver then
        logger.tracef("callback addr: %s  uin: %d , shopId: %d", gameserver, uin, shopId)
        cluster.call(gameserver, ".callback_service", "callback", 
            uin, shopId, orderId, pay_amount, pay_currency)
        logger.trace("========= callback success ")
        return "success"
    end
    logger.trace("========= callback fail")
    return "callback fail"
end

local function huawei_callback(data)
    logger.tracef("========= huiwei call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        logger.tracef("========= data nil error: %s ", data)
        return "params error"
    end

    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    logger.tracef("========= gameserver1: %s ", gameserver)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("========= gameserver2: %s ", gameserver)
    end
    if gameserver then
        logger.tracef("callback addr: %s  uin: %d , shopId: %d", gameserver, uin, shopId)
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        logger.tracef("========= callback success ")
        return "success"
    end
    logger.trace("========= callback fail")
    return "callback fail"
end
--  咪咕游戏回调记录 只是记录 不做钻石修改
local function migu_callback(data)
    local xml_success = "<root><hRet>0</hRet><message>Successful</message></root>"
    local xml_fail = "<root><hRet>1</hRet><message>failure</message></root>"
    logger.tracef("========= migu call back request: %s", data)
    if data == nil or data == "" then
        logger.trace("data error")
        return xml_fail
    end
    local body = data
    local xml_table = {}
    local version = string.find(body,"version")
    local encoding = string.find(body,"encoding")
    local xml = string.find(body,"xml")
    if xml and version  and encoding then
        local last_str = string.find(body,"?>")
        body = string.sub(body,last_str+2,#body)
    end
    local s,e = pcall(function()
        xml_table = utils.collect(body)
        xml_table = xml_table[1]
    end)
    local userId
    local contentId
    local consumeCode
    local cpId
    local hRet
    local status
    local versionId
    local cpparam
    local packageID
    for _,v in pairs(xml_table)do
        if v.label == "userId" then
            userId = v[1]
        end
        if v.label == "contentId" then
            contentId = v[1]
        end
        if v.label == "consumeCode" then
            consumeCode = v[1]
        end
        if v.label == "cpId" then
            cpId = v[1]
        end
        if v.label == "hRet" then
            hRet = v[1]
        end
        if v.label == "status" then
            status = v[1]
        end
        if v.label == "versionId" then
            versionId = v[1]
        end
        if v.label == "cpparam" then
            cpparam = v[1]
        end
        if v.label == "packageID" then
            packageID = v[1]
        end
    end
    if cpparam == nil then
        logger.tracef("========= cpparam error: %s ", cpparam)
        return xml_fail
    end
    local callback_info = utils.split(cpparam, "#")
    local uin = tonumber(callback_info[1])
    local shopId = tonumber(callback_info[2])
    local orderId = callback_info[3]
    if uin == nil or shopId == nil then
        logger.tracef("========= cpparam error: %s ", cpparam)
        return xml_fail
    end
    if redis_service == nil then
        redis_service = skynet.queryservice("redis_service")
    end
    local recharge_data = {
        userId = userId,
        contentId = contentId,
        consumeCode = consumeCode,
        cpId = cpId,
        hRet = hRet,
        status = status,
        versionId = versionId,
        cpparam = cpparam,
        packageID = packageID,
    }
    skynet.call(redis_service, "lua", "hset_obj", "recharge_order", orderId, recharge_data)
    logger.tracef("migu callback success")
    return xml_success
end

--jinli支付回调接口
local function jinli_callback(data)
    logger.tracef("========= jinli call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    logger.tracef("=========jinli gameserver1: %s for uin : %d", gameserver, uin)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    end
    if gameserver then
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        return "success"
    end
    return "callback fail"
end

--oppo支付回调接口
local function oppo_callback(data)
    logger.tracef("========= oppo call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    end
    if gameserver then
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        return "success"
    end
    return "callback fail"
end

local function wdj_callback(data)
    logger.tracef("========= wdj call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    end
    if gameserver then
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        return "success"
    end
    return "callback fail"
end


local function kupai_callback(data)
    logger.tracef("========= kupai call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    end
    if gameserver then
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        return "success"
    end
    return "callback fail"
end

--qihoo支付回调接口
local function qihoo_callback(data)
    logger.tracef("========= qihoo call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        logger.tracef("=========qihoo data nil error: %s ", data)
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    logger.tracef("=========qihoo gameserver1: %s for uin : %d", gameserver, uin)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("=========qihoo gameserver2: %s ", gameserver)
    end
    if gameserver then
        logger.tracef("callback addr: %s  uin: %d , shopId: %d", gameserver, uin, shopId)
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        return "success"
    end
    logger.tracef("=========qihoo callback fail")
    return "callback fail"
end

--M-Market支付充值回调
--[[
验证必须域，没有则无效的请求

MsgType         消息类型
OrderID         订单号
ChannelID       渠道ID
PayCode         应用计费点编码
AppID           应用ID
MD5Sign         MD5(OrderID#ChannelID#PayCode#AppKey) 验证签名
ActionTime
ExData          前台传给M-Market平台的东西

-- ]]
local function mmarket_callback(data)
    local result_str = '<?xml version="1.0" encoding="UTF-8"?>' ..
                        '<SyncAppOrderResp xmlns="http://www.monternet.com/dsmp/schemas/">' ..
                        '<Version>1.0.0</Version>' ..
                        '#BODY' ..
                        '</SyncAppOrderResp>'
    local ret_body = '<hRet>'
    local xml_table = {}

    --todo 两秒没返回的时候，会收到重复的回调 所以在给用户弃值后得检测订单是否存在

    logger.logf("========= mmarket_callback recv data : %s", data)
    if data == nil and data == "" then
        ret_body = ret_body .. 1 .. '</hRet>'
        return string.gsub(result_str,'#BODY',ret_body)
    end
    --解析body(xml)
    local version = string.find(data,"version")
    local encoding = string.find(data,"encoding")
    local xml = string.find(data,"xml")

    --截取<?xml  ?>后边部分
    if xml and version  and encoding then
        local last_str = string.find(data,"?>")
        data = string.sub(data,last_str+2,#data)
    end

    --解析xml内容
    xpcall(function()
            xml_table = utils.collect(data)
            xml_table = xml_table[1]
          end, debug.traceback)

    --获取xml值
    local msg_type,order_id,channel_id,pay_code, md5_sign,extra_data,action_time;
    for _,tag in pairs(xml_table) do

        if tag.label == "MsgType" then
            msg_type = tag[1]
        end

        if tag.label == "OrderID" then
            order_id = tag[1]
        end

        if tag.label == "ChannelID" then
            channel_id = tag[1]
        end

        if tag.label == "PayCode" then
            pay_code = tag[1]
        end

        if tag.label == "MD5Sign" then
            md5_sign = tag[1]
        end

        if tag.label == "ExData" then
            extra_data = tag[1]
        end

        if tag.label == "action_time" then
            action_time = tag[1]
        end

--        if tag.label == "" then
--        end
    end

    if md5_sign ~= order_id .. '#' .. channel_id .. '#' .. pay_code .. '#' .. 'appkey' then
        ret_body = ret_body .. 4005 .. '</hRet>'
        return string.gsub(result_str,'#BODY',ret_body)
    end

    local uin,shop_id
    local users_data = utils.split(extra_data,'#')
    if #users_data ~= 2 then
        ret_body = ret_body .. 1 .. '</hRet>'
        return string.gsub(result_str,'#BODY',ret_body)
    end

    uin = to_number(users_data[1])
    shop_id = to_number(users_data[2])

    if redis_service == nil then
        redis_service = skynet.queryservice("redis_service")
    end

    logger.tracef("========= mmarket_callback uin : %d ", uin)
    logger.tracef("========= mmarket_callback shop_id : %d ", shop_id)

    local account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
    if account_info then
        local in_order = order_id .. "_" .. COMMON.platform[account_info.platform] -- 订单id_平台字符串 以保证唯一
        local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("========= mmarket_callback gameserver1 : %s ", gameserver)
        if gameserver == nil then
            gameserver = skynet.call(syncserver, "lua", "get_location", uin)
            logger.tracef("========= mmarket_callback  gameserver2 : %s ", gameserver)
        end
        if gameserver then
            logger.tracef("callback addr: %s  uin: %d , shopId: %d", gameserver, uin, shop_id)
            cluster.call(gameserver, ".callback_service", "callback", uin, shop_id, in_order)
            logger.trace("========= mmarket_callback success ")
            local recharge_data = {
                Uin = uin,
                ShopoId = shop_id, -- 商品id
                OrderId = in_order,
                PayTime = action_time, -- 支付时间
                Time = math.floor(os.time()), -- 记录时间
                Status = "success" -- 处理状态
            }
            skynet.call(redis_service, "lua", "hset_obj", "recharge_order", in_order, recharge_data)
        else
            logger.trace("========= mmarket_callback not found gameserver")
            -- 网游版的是否可以在此处理钻石增加问题
            ret_body = ret_body .. 2 .. '</hRet>'
            return string.gsub(result_str,'#BODY',ret_body)
        end

        local data = skynet.call(redis_service, "lua", "hget_obj", "recharge_order", in_order, true)
        if data == nil or data.Status ~= "success" then
            local recharge_data = {
                Uin = uin,
                ShopoId = shop_id,
                OrderId = in_order,
                PayTime = action_time,
                Time = math.floor(os.time()),
                Status = "faild"
            }
            skynet.call(redis_service, "lua", "hset_obj", "recharge_order", in_order, recharge_data)
            ret_body = ret_body .. 2 .. '</hRet>'
            return string.gsub(result_str,'#BODY',ret_body)
        end
    end

    ret_body = ret_body .. 0 .. '</hRet>'
    return string.gsub(result_str,'#BODY',ret_body)
end

-- 应用宝
local function inner_callback(data)
    logger.tracef("========= inner call back request: %s", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        logger.tracef("=========inner data nil error: %s ", data)
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    logger.tracef("=========inner gameserver1: %s for uin : %d", gameserver, uin)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("=========inner gameserver2: %s ", gameserver)
    end
    if gameserver then
        logger.tracef("callback addr: %s  uin: %d , shopId: %d", gameserver, uin, shopId)
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        return "success"
    end
    logger.trace("=========inner callback fail")
    return "callback fail"
end

-- 用于测试的支付回调
local function midas_callback(data)
    logger.tracef("========= midas call back request: %s", data)
    print("midas, ", data)
    local params = get_params(data)
    local orderId = params["orderId"]
    local uin = params["uin"]
    local shopId = params["shopId"]
    if uin == nil or shopId == nil or orderId == nil then
        logger.tracef("=========midas data nil error: %s ", data)
        return "params error"
    end
    local gameserver = skynet.call(syncserver, "lua", "get_location", uin)
    logger.tracef("=========midas gameserver1: %s for uin : %d", gameserver, uin)
    if gameserver == nil then
        gameserver = skynet.call(syncserver, "lua", "get_location", uin)
        logger.tracef("=========midas gameserver2: %s ", gameserver)
    end
    if gameserver then
        cluster.call(gameserver, ".callback_service", "callback", uin, shopId, orderId)
        return "success"
    end
    logger.trace("=========midas callback fail")
    return "callback fail"
end



local on_platform = {
    ["xiaomi_callback"] = xiaomi_callback,
    ["migu_callback"] = migu_callback,
    ["lenovo_callback"] = lenovo_callback,
    ["mmarket_callback"] = mmarket_callback,
    ["qihoo_callback"] = qihoo_callback,
    ["jinli_callback"] = jinli_callback,
    ["wdj_callback"] = wdj_callback,
    ["oppo_callback"] = oppo_callback,
    ["kupai_callback"] = kupai_callback,
    ["memoriki_callback"] = memoriki_callback,
    ["huawei_callback"] = huawei_callback,
    ["midas_callback"] = midas_callback,
    ["inner_callback"] = inner_callback,
}


--处理请求
function CMD.on_action(path, body)
    logger.tracef("path=>  %s ", path)
    logger.tracef("body=>  %s ", body)
    local platf_action = utils.split(path, "/")
    local platf = platf_action[2] -- 请求平台
    local action = platf_action[3] -- 请求操作  login or callback
    if platf and action then
        local index = platf .. "_" .. action
        return on_platform[index](body)
    end
end


local traceback = debug.traceback
skynet.start(function()
    redis_service = skynet.queryservice("redis_service")
    syncserver = skynet.queryservice("syncserver")
    skynet.dispatch("lua", function(_, _, command, ...)
        --logger.trace("command  : " .. command)
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

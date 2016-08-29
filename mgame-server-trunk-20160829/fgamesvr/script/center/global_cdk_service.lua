local skynet = require "skynet"

local server_config = require "server_config"
local logger = require "logger"
local CMD = {}
local COMMON = require "common"
local json = require "cjson"
local utils = require "utils"
local redis_service
local keys
local unique_code_factory_service_4
local unique_code_factory_service_6
local unique_code_factory_service_8
local unique_code_factory_service_12


local char_mapping_tmp = {
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "w",
    "x",
    "y",
    "z",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
}

local function check_type(type, reward, gift_name, platform, reward_type)
    local exist_type = skynet.call(redis_service, "lua", "hget_obj", "gift_type", gift_name)
    local name
    local id = 0
    if exist_type == nil then
        id = skynet.call(redis_service, "lua", "id_handler", "code_type_max_id")
        name = gift_name
        local gift_type = {
            id = id,
            type = type, -- 礼包使用规则 1,玩家该类型只能领取一次,2,不限领取次数，但每个码只能用一次 3,通用激活码，所有人都能用一次
            giftName = gift_name, -- 礼包名称 不能重复
            reward = reward, -- 奖励
            reward_type = reward_type,
            platform = platform -- 所属平台
        }
        skynet.call(redis_service, "lua", "hset_obj", "gift_type", gift_name, gift_type)
    else
        name = exist_type.giftName
        id = exist_type.id
    end
    return name, id
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

local function error_return()
    local resp = {
        status = 11 -- 参数错误
    }
    return COMMON.ERRORCODE.PARAM_ERROR, json.encode(resp)
end

-- 生成cdk function名不要动
function CMD.genKey(param)
    logger.trace("==================   gen_cdk    ====================")
    local params = get_params(param)
    local count =tonumber( params["count"])
    local len = tonumber(params["length"])
    local type = tonumber(params["type"])
    local reward = params["reward"]
    local gift_name = params["name"]
    local platform = tonumber(params["pf"])
    local reward_type = params["reward_type"]

    logger.trace("cdk:", type, "...", reward, "..", platform, "len:", len)
    if count == nil or len == nil or type == nil or reward == nil or gift_name == nil or platform == nil then
        return error_return()
    end

    local name, id = check_type(type, reward, gift_name, platform, reward_type)
    if name == nil then return error_return() end

    local json_data = {}

    local unique_code_f = nil
    if len == 8 then
       unique_code_f = unique_code_factory_service_4
    elseif len == 10 then
        unique_code_f = unique_code_factory_service_6
    elseif len == 12 then
        unique_code_f = unique_code_factory_service_8
    elseif len == 16 then
        unique_code_f = unique_code_factory_service_12
    else
        return error_return()
    end


    for i = 1, count do
        local str, err = skynet.call(unique_code_f, "lua", "create_code")
        if str == "" then
            table.insert(json_data, 1, "error : " .. err .. " create num is " .. i - 1)
            break
        else
            --后面加4位随机串， 以保证难以看出规律性
            local rand_str = ""
            for i = 1, 4 do
                rand_str = rand_str .. char_mapping_tmp[math.random(1, #char_mapping_tmp)];
            end
            str = str .. rand_str
            local code = {
                id = id,
                key = str,
                playerData = "",
                useTime = 0,
                giftName = name,
            }
            skynet.call(redis_service, "lua", "hset_obj", "gift_code", str, code)
            table.insert(json_data, str)
        end
    end
    local sub = table.concat(json_data, ",")
    local resp = {
        status = COMMON.ERRORCODE.OK,
        cdks = sub
    }
    return COMMON.ERRORCODE.OK, json.encode(resp)
end

-- 使用cdk function名不要动
function CMD.useKey(account_info, key)
--    logger.trace("=========    useKey     ======================")
--    local exist_code = skynet.call(redis_service, "lua", "hget_obj", "gift_code", key)
--    if exist_code == nil then
--        return COMMON.ERRORCODE.GIFT_CODE_NOE_EXIST
--    end
--    if exist_code.playerData ~= "" then
--        return COMMON.ERRORCODE.GIFT_CODE_IS_USED
--    end
--    local exist_type = skynet.call(redis_service, "lua", "hget_obj", "gift_type", exist_code.giftName)
--    if exist_type == nil then return COMMON.ERRORCODE.GIFT_TYPE_NOE_EXIST end
--    exist_code.playerData = account_info.uin
--    exist_code.useTime = math.floor(os.time())
--    skynet.call(redis_service, "lua", "hset_obj", "gift_code", key, exist_code)
--    return COMMON.ERRORCODE.OK, exist_type.reward, exist_code.id
end

-- 查询cdk function名不要动
function CMD.selectKey(param)
    logger.trace("=========    selectKey     ======================")
    local params = get_params(param)
    local keys = params["key"]
    if keys == nil then return error_return() end
    local select_keys = utils.split(keys, ",")
    local json_data = {}
    for _, key in pairs(select_keys) do
        local exist_code = skynet.call(redis_service, "lua", "hget_obj", "gift_code", key)
        if exist_code then
            table.insert(json_data, json.encode(exist_code))
        end
    end
    local sub = table.concat(json_data, ",")
    local resp = {
        status = COMMON.ERRORCODE.OK,
        ckey = sub
    }
    return COMMON.ERRORCODE.OK, json.encode(resp)
end

-- 查询cdk function名不要动
function CMD.update(param)
    logger.trace("=========    update     ======================")
    local sub = "策划暂定激活码没有期限,因为不需要设置过期"
    local resp = {
        status = COMMON.ERRORCODE.OK,
        ckey = sub
    }
    return COMMON.ERRORCODE.OK, json.encode(resp)
end

-- 查询cdk function名不要动
function CMD.selectKeyByName(param)
    logger.trace("=========    selectKeyByName     ======================")
    local sub = "考虑到性能,暂不提供该功能,只提供selectKey功能"
    local resp = {
        status = COMMON.ERRORCODE.OK,
        ckey = sub
    }
    return COMMON.ERRORCODE.OK, json.encode(resp)
end


local traceback = debug.traceback
-- 最多返回三个参数
skynet.start(function()
    redis_service = skynet.queryservice("redis_service")

    unique_code_factory_service_4 = skynet.newservice("unique_code_factory_service")
    skynet.call(unique_code_factory_service_4, "lua", "init", "cdk_4", 4, char_mapping_tmp)

    unique_code_factory_service_6 = skynet.newservice("unique_code_factory_service")
    skynet.call(unique_code_factory_service_6, "lua", "init", "cdk_6", 6, char_mapping_tmp)

    unique_code_factory_service_8 = skynet.newservice("unique_code_factory_service")
    skynet.call(unique_code_factory_service_8, "lua", "init", "cdk_8", 8, char_mapping_tmp)

    unique_code_factory_service_12 = skynet.newservice("unique_code_factory_service")
    skynet.call(unique_code_factory_service_12, "lua", "init", "cdk_12", 12, char_mapping_tmp)

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end
        local ok, ret, param1, param2 = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        skynet.retpack(ret, param1, param2)
    end)
end)




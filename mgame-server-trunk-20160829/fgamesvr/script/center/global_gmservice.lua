--
-- Created by IntelliJ IDEA.
-- User: Denny
-- Date: 2015/9/29
-- Time: 11:24
-- To change this template use File | Settings | File Templates.
--
local skynet = require "skynet"
local cluster = require "cluster"

local logger = require "logger"
local utils = require "utils"
local COMMON = require "common"
local proto = require "proto"
local md5 = require "md5"
local json = require "cjson"
json.encode_sparse_array(true)

local syncserver
local global_mailserice
local redis_service

local CMD = {}

local function handle_send_mail(body)
    local pattern = "gmtype=sendmail&uin=([%d]+)&type=([%d]+)&title=(.*)&content=(.*)&plugin=(.*)"

    local uin, type, title, content, plugin = string.match(body, pattern)
    type = tonumber(type)
    title = tostring(title)
    content = tostring(content)
    plugin = tostring(plugin)

    local now = math.floor(skynet.time())
    local data = {
        title = title,
        content = content,
        plugin = plugin,
        sendtime = now,
        outdate =  now + 30 * 24 * 60 * 60 * 100  -- 一个月之后
    }

    local ret = true
    ret = skynet.call(global_mailserice, "lua", "add_mail", uin,
        type, data, COMMON.change_src.gm_mail)
    return ret
end

-- pvp随机匹配分差值
local function handle_pvp_rand_diff_point(body)
    local pattern = "value=([%d]+)"
    local value = tonumber(string.match(body, pattern))
    logger.trace("gm :", "set_pvprand_pointdv....", value)
    if not value then
        return "param error"
    end

    if value <= 0 or value > 10000 then
        return "value is error: " .. value
    end
    
    local all_gamed = skynet.call(syncserver, "lua", "get_all_gamecluster")
    for i, v in ipairs(all_gamed) do
        cluster.call(v, ".pvpserver", "set_rand_diff_point", value)
    end
    return true
end

-- pvp无道具场匹配分差值
local function handle_pvp_fair_diff_point(body)
    local pattern = "value=([%d]+)"
    local value = tonumber(string.match(body, pattern))
    if not value then
        return "param error"
    end

    if value <= 0 or value > 10000 then
        return "value is error: " .. value
    end
    
    local all_gamed = skynet.call(syncserver, "lua", "get_all_gamecluster")
    for i, v in ipairs(all_gamed) do
        cluster.call(v, ".pvpserver", "set_fair_diff_point", value)
    end
    return true
end

local function handle_pvp_map_id(body)
    local pattern = "value=([%d]+)"
    local value = tonumber(string.match(body, pattern))
    if not value then
        return "param error"
    end

    local all_gamed = skynet.call(syncserver, "lua", "get_all_gamecluster")
    for i, v in ipairs(all_gamed) do
        cluster.call(v, ".pvp_room_server", "set_map_id", value)
    end
    return true
end

local function handle_pvp_rand_map_id(body)
    local pattern = "value=([%d]+)"
    local value = tonumber(string.match(body, pattern))
    if not value then
        return "param error"
    end

    local all_gamed = skynet.call(syncserver, "lua", "get_all_gamecluster")
    for i, v in ipairs(all_gamed) do
        cluster.call(v, ".pvpserver", "set_rand_map", value)
    end
    return true
end

local function handle_activity_opt(body)
    print(body)
    local pattern = "gmtype=activity&id=([%d]+)&opt=(.*)&start_time=([%d]+)&end_time=([%d]+)&extra=(.*)"
    local id, opt, start_time, end_time, extra = string.match(body, pattern)
    if not id or not opt or not start_time or not end_time then
        return "param is error"
    end
    id = tonumber(id)
    start_time = tonumber(start_time)
    end_time = tonumber(end_time)
    print("handle_activity_opt:", id, opt, start_time, end_time)

    local all_gamed = skynet.call(syncserver, "lua", "get_all_gamecluster")
    local ret
    for i, v in ipairs(all_gamed) do
        if opt == "add" then
            ret = cluster.call(v, ".activitys_service", "add_act_open", id, start_time, end_time)
        elseif opt == "close" then
            ret = cluster.call(v, ".activitys_service", "close_act", id)
        end
        print(ret)
        if not ret or ret ~= "success" then
            return ret
        end
    end
    return ret
end

local function call_redis(opt, param1, param2, param3)
    return skynet.call(redis_service, "lua", opt, param1, param2, param3)
end

local function handle_search_redis(body)
    local pattern = "gmtype=redis_opt&opt=(.*)&param1=(.*)&param2=(.*)&param3=(.*)"
    local opt, param1, param2, param3 = string.match(body, pattern)

    logger.trace("handle_search_redis", param1, param2, param3)
    param1 = utils.unserialize_table(param1) or param1
    param2 = utils.unserialize_table(param2) or param2
    param3 = utils.unserialize_table(param3) or param3

    logger.trace("handle_search_redis", param1, param2, param3)
    return utils.serialize_table(call_redis(opt, param1, param2, param3))

end

local function handle_del_openid(body)
    local pattern = "gmtype=del_openid&uin=([%d]+)"
    local uin = string.match(body, pattern)
    uin = tonumber(uin)
    local account_info = call_redis("hget_obj", "account", uin)
    if not account_info then
        return "uin is not exist"
    end
    local open_id = COMMON.platform[account_info.platform] .. "_" .. account_info.openid
    call_redis("hdel_obj", "uin_mapping", open_id)
    return "openid " .. open_id .. " was deleted"
end

local function handle_search_player_info(body)
    local pattern = "gmtype=searchinfo&openid=(.*)&platform=([%d]+)&sign=(.*)"
    local openid, platform, sign = string.match(body, pattern)
    if openid == nil or platform == nil or sign == nil then
        return "{ret:1}"
    end

    local sign_str = openid..platform.."61gamestks"
    local md_sign = md5.sumhexa(sign_str)
    if md_sign ~= sign then
        return "{ret:2}"
    end

    platform = tonumber(platform)
    local platform_str = COMMON.platform[platform];
    if platform_str == nil then
        return "{ret:3}"
    end

    local uin = skynet.call(redis_service, "lua", "hget_obj", "uin_mapping", platform_str.."_"..openid)
    if uin == nil then
        return "{ret:4}"
    end

    local account_info
    local clusternode = skynet.call(syncserver, "lua", "get_user_location", uin)
    if clusternode then
        account_info = cluster.call(clusternode, ".gmservice", "search_player_info", uin)
    else
        account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
    end
    if account_info then
        local ret = {
            ret = 0,
            info = account_info,
        }
        return json.encode(ret)
    else
        return "{ret:5}"
    end
end

local function handle_balance( body )
    local data = string.match(body, "gmtype=balance&data=(.*)")
    logger.debugf("handle_balance data[%s]",data)
    local ok, ret = xpcall(cluster.call, debug.traceback, "gslbnode", ".gslbserver", "update_balance", data)        
    if not ok then
        logger.debugf("handle_balance cluster call failed")
        return "false"
    end
    return "true"
end

local function handle_gray( body )
    local data = string.match(body, "gmtype=gray&data=(.*)")
    logger.debugf("handle_gray data[%s]",data)
    local ok, ret = xpcall(cluster.call, debug.traceback, "gslbnode", ".gslbserver", "update_gray", data)        
    if not ok then
        logger.debugf("handle_gray cluster call failed")
        return "false"
    end
    return "true"
end

local function handle_whitelist( body )
    local data = string.match(body, "gmtype=whitelist&data=(.*)")
    logger.debugf("handle_whitelist data[%s]",data)
    local ok, ret = xpcall(cluster.call, debug.traceback, "gslbnode", ".gslbserver", "update_stop", data)        
    if not ok then
        logger.debugf("handle_whitelist cluster call failed")
        return "false"
    end
    return "true"
end

local function handle_delcdk(body)
    local jstr = string.match(body, "gmtype=delcdk&data=(.*)")
    if not jstr then
        return false
    end
    
    local data = json.decode(jstr)
    if not data then
        return false
    end
    
    for i, cdk in ipairs(data) do
        skynet.call(redis_service, "lua", "hdel_obj", "gift_code", cdk)
    end
    return true
end

function CMD.sys_ctrl(body)
    local r = {}

    local cluster_list = skynet.call(syncserver, "lua", "get_all_gamecluster")
    for _, v in ipairs(cluster_list) do
        local t = cluster.call(v, ".plan_service", "reload")
        table.insert(r, t)
    end

    return table.concat(r, '\r\n')
end

function CMD.parse_cmd(body)
    if body then
        logger.trace("parse_cmd data : ", body)
        if string.match(body, "gmtype=sendmail") then
            return handle_send_mail(body)
        elseif string.match(body, "gmtype=activity") then
            return handle_activity_opt(body)
        elseif string.match(body, "gmtype=pvp_rand_diff_point") then
            return handle_pvp_rand_diff_point(body)
        elseif string.match(body, "gmtype=pvp_fair_diff_point") then
            return handle_pvp_fair_diff_point(body)
        elseif string.match(body, "gmtype=pvp_map_id") then
            return handle_pvp_map_id(body)
        elseif string.match(body, "gmtype=pvp_rand_map_id") then
            return handle_pvp_rand_map_id(body)
        elseif string.match(body, "gmtype=redis_opt") then
            return handle_search_redis(body)
        elseif string.match(body, "gmtype=del_openid") then
            return handle_del_openid(body)
        elseif string.match(body, "gmtype=searchinfo") then
            return handle_search_player_info(body)
        elseif string.match(body, "gmtype=balance") then
            return handle_balance(body)
        elseif string.match(body,"gmtype=gray") then
            return handle_gray(body)
        elseif string.match(body, "gmtype=whitelist") then
            return handle_whitelist(body)
        elseif string.match(body, "gmtype=delcdk") then
            return handle_delcdk(body)
        else
            local uin = string.match(body, "uin=([%d]+)")
            if uin then uin = tonumber(uin) end

            print("uin:", uin)
            if not uin or uin == 0 then
                local cluster_list = skynet.call(syncserver, "lua", "get_all_gamecluster")
                for k, v in ipairs(cluster_list) do
                    return cluster.call(v, ".gmservice", "parse_cmd", body)
                end
            else
                local clusternode = skynet.call(syncserver, "lua", "get_user_location", uin)
                if clusternode then
                    return cluster.call(clusternode, ".gmservice", "parse_cmd", body)
                else
                    clusternode = skynet.call(syncserver, "lua", "get_random_gamecluster")
                    if clusternode then
                        return cluster.call(clusternode, ".gmservice", "parse_cmd", body)
                    else
                        logger.error("have no gamed now")
                        return "have no gamed now"
                    end
                end
            end
        end
        return true
    else
        return false
    end
end

skynet.start(function()
    syncserver = skynet.queryservice("syncserver")
    global_mailserice = skynet.queryservice("global_mailservice")
    redis_service = skynet.queryservice("redis_service")
    proto.load_proto()

    local traceback = debug.traceback
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


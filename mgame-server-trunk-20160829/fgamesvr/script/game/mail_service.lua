--
-- Created by IntelliJ IDEA.
-- User: Denny
-- Date: 2015/9/10
-- Time: 16:23
-- To change this template use File | Settings | File Templates.
--
local skynet = require "skynet"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name

local logger = require "logger"
local utils = require "utils"
local COMMON = require "common"
local server_config = require "server_config"

local redis_service
local query_service
local multicast_service
local gamed
local local_gamenode = utils.query_nodename(server_config.gamed_conf.servername)

local global_mail_data = {}
local global_mail_len = 0
local max_global_mail_id = 0

local single_mail_data = {}
local single_mail_len = 0
local max_single_mail_id = 0

local function init()
    --加载全局邮件信息
    global_mail_data = skynet.call(redis_service, "lua", "hgetall_all_value", "global_mail", true)
    global_mail_len  =  skynet.call(redis_service, "lua", "hlen", "global_mail")
    max_global_mail_id = skynet.call(redis_service, "lua", "get", "max_global_mail_id")  --id_handler
    max_global_mail_id = tonumber(max_global_mail_id) or 0
    if not global_mail_data then
        global_mail_data = {}
    end

    for k,v in pairs(global_mail_data) do
        if v.outdate < math.floor(skynet.time()) then
            skynet.call(redis_service, "lua", "hdel_obj", "global_mail", k)
            global_mail_data[k] = nil
        end
    end

    --加载单点邮件信息
    single_mail_data = skynet.call(redis_service, "lua", "hgetall_all_value", "single_mail", true)
    single_mail_len = skynet.call(redis_service, "lua", "hlen", "single_mail")
    max_single_mail_id = skynet.call(redis_service, "lua", "get", "max_single_mail_id")
    max_single_mail_id = tonumber(max_single_mail_id) or 0
    if not single_mail_data then
        single_mail_data = {}
    end
end

local CMD = {}
-- 目前这个接口只做本服内的玩家的单点邮件处理
-- mail_business_type  mail ,  gm_mail, operate_mail, operate_mail 这几个mail来源类型
function CMD.add_mail(uin, mail_type, data, mail_business_type)
    logger.tracef("add_mail mail_type:%d, uin:%d data:%s", mail_type,  uin, data)
    --安全处理一下，没有sendtime就设置为当前
    if data.sendtime == nil then
        data.sendtime = math.floor(skynet.time())
    end

    if data.outdate == nil then
        data.outdate = data.sendtime + 10*24*60*60   --默认过期时间10天
    end

    if mail_type == COMMON.mail_type.GLOBAL then
        cluster.call("centernode", ".global_mailservice", "add_mail", uin, mail_type, data)
        return
    end

    data.business_type = mail_business_type

    --针对单点邮件的优化，如果位置记录在本服，就进行本服操作， 不在本服，就转到global_mailservice进行处理
    local mail_id
    if mail_type == COMMON.mail_type.SINGLE then
        local node_info = skynet.call(redis_service, "lua", "hget_obj", "player_node", uin)
        if node_info then
            local gamenode = utils.query_nodename(node_info.servername)
            if gamenode == local_gamenode then

                mail_id = skynet.call(redis_service, "lua", "id_handler", "max_single_mail_id")
                skynet.call(redis_service, "lua", "hset_obj", "single_mail", mail_id, data)
                single_mail_data[mail_id] = data

                gamed = gamed or skynet.queryservice("gamed")
                local uin_agent = skynet.call(gamed, "lua", "get_alive_agent", uin)
                local user_mail
                if uin_agent then
                    skynet.call(uin_agent, "lua", "add_mail", mail_type, mail_id)
                else
                    user_mail = skynet.call(redis_service, "lua", "hget_obj", "usermail", uin)

                    if not user_mail then
                        user_mail = {total_no = 0, mail_num = 0, max_global_id = 0, content = {}}
                    end

                    user_mail.mail_num = user_mail.mail_num + 1
                    user_mail.total_no = user_mail.total_no + 1

                    local mail_data
                    if mail_type == COMMON.mail_type.SINGLE then
                        mail_data = {type = COMMON.mail_type.SINGLE, id = mail_id}
                    else
                        mail_data = {type = COMMON.mail_type.GLOBAL, id = mail_id}
                    end

                    user_mail.content[user_mail.total_no] = mail_data
                    skynet.call(redis_service, "lua", "hset_obj", "usermail", uin, user_mail)
                end

                return
            end
        else
            cluster.call("centernode", ".global_mailservice", "add_mail", uin, mail_type, data)
        end
    end
end

-- global_mailservice 中调用 add_usermail, 添加邮件
function CMD.add_usermail(mail_type, uin, mail_id, mail_business_type)
    logger.tracef("add_usermail mail_type:%d, uin:%d, mailid:%d ", mail_type, uin, mail_id)
    --1.把邮件插入到相应的redis表中
    if mail_type == COMMON.mail_type.SINGLE then
        local data = skynet.call(redis_service, "lua", "hget_obj", "single_mail", mail_id)
        if data then
            data.business_type = mail_business_type
            single_mail_data[mail_id] = data
        else
            logger.error("single mail id not exsit")
            return
        end
    elseif mail_type == COMMON.mail_type.GLOBAL then
        local data = skynet.call(redis_service, "lua", "hget_obj", "global_mail", mail_id)
        if data then
            data.business_type = mail_business_type
            global_mail_data[mail_id] = data
            local max_global_id = skynet.call(redis_service, "lua", "get", "max_global_mail_id")  --id_handler
            max_global_id = tonumber(max_global_id) or 0
            max_global_mail_id = max_global_mail_id + 1 > max_global_id and max_global_mail_id + 1 or max_global_id
        else
            logger.error("global mail id not exsit")
            return
        end
    end

    --2.更新mail信息,现在只需处理单点邮件，全服邮件不需要做额外处理
    if mail_type == COMMON.mail_type.SINGLE then
        gamed = gamed or skynet.queryservice("gamed")  --初始化放在用到的地方
        local uin_agent = skynet.call(gamed, "lua", "get_alive_agent", uin)
        local user_mail
        if uin_agent then
            skynet.call(uin_agent, "lua", "add_mail", mail_type, mail_id)
        else
            user_mail = skynet.call(redis_service, "lua", "hget_obj", "usermail", uin)

            if not user_mail then
                user_mail = {total_no = 0, mail_num = 0, max_global_id = 0, content = {}}
            end

            user_mail.mail_num = user_mail.mail_num + 1
            user_mail.total_no = user_mail.total_no + 1

            local mail_data
            if mail_type == COMMON.mail_type.SINGLE then
                mail_data = {type = COMMON.mail_type.SINGLE, id = mail_id}
            else
                mail_data = {type = COMMON.mail_type.GLOBAL, id = mail_id}
            end

            user_mail.content[user_mail.total_no] = mail_data
            skynet.call(redis_service, "lua", "hset_obj", "usermail", uin, user_mail)
        end
    end
end

function CMD.get_maildata(mail_type, mail_id)
    local mail
    if mail_type == COMMON.mail_type.GLOBAL then
        if global_mail_data then
            mail = global_mail_data[mail_id]
        end
    else
        if single_mail_data then
            mail = global_mail_data[mail_id]
        end
    end

    if mail_type == COMMON.mail_type.GLOBAL then
        if not global_mail_data then
            return nil
        end
        mail = global_mail_data[mail_id]
    else
        if not single_mail_data then
            return nil
        end
        mail = single_mail_data[mail_id]
    end

    return mail
end

--这个只做全局邮件加载
function CMD.get_global_mail(global_no, create_time)
    local receive_num = 0
    local mail_list = {}

    if global_no >= max_global_mail_id then
        return receive_num
    end

    for cursor = max_global_mail_id, global_no+1, -1 do
        if not create_time then
            logger.error("account create_time is nil ")
            break
        end
        local data = global_mail_data[cursor]
        if data and data.sendtime then
            if create_time < data.sendtime then
                table.insert(mail_list, cursor)
                receive_num = receive_num + 1
                if receive_num > 60 then  --最多全局邮件为60
                    break
                end
            end
        end
    end

    mail_list.max_id = max_global_mail_id

    return receive_num, mail_list
end

--只针对单点邮件，才有删除
function CMD.del_mail(type, mail_id)
    assert(type == COMMON.mail_type.SINGLE)
    if type == COMMON.mail_type.SINGLE then
        skynet.call(redis_service, "lua", "hdel_obj", "single_mail", mail_id)
    end
end

local traceback = debug.traceback
skynet.start(function()
    redis_service = skynet.queryservice("redis_service")
    multicast_service = skynet.queryservice("multicast_service")

    init()

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        logger.trace("command : " .. command)
        if not f then
            logger.warningf("unhandled message(%s) id : %d", command, skynet.self())
            return skynet.ret()
        end

        local ok, ret, extra = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s  id : %d", command, ret, skynet.self())
            return skynet.ret()
        end

        if not extra then
            skynet.retpack(ret)
        else
            skynet.retpack(ret, extra)
        end
    end)
end)

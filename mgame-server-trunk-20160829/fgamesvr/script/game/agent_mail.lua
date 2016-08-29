--
-- Created by IntelliJ IDEA.
-- User: Denny
-- Date: 2016/2/25
-- Time: 11:40
-- To change this template use File | Settings | File Templates.
--

local agent_mail = {}

local skynet          = require "skynet"

local sharedata_utils = require "sharedata_utils"
local utils           = require "utils"
local setting         = require "setting"
local logger          = require "logger"
local proto           = require "proto"
local COMMON          = require "common"

local send_response, send_error_code
local account_info, uin, player
local redis_service, agent, account_info
local mail_service

--相关数据结构
--[[
-- global_mail
{
   outdate=1470378046,   --过期时间
   business_type=60,     --邮件业务类型 gm邮件，运营邮件，激活码邮件等
   sendtime=1464458806,  --发送时间
   plugin="{20001:1}",   --插件
   content="test",       --内容
   title="test"          --标题
}

-- single_mail
{
   outdate=1470378046,   --过期时间
   business_type=60,     --邮件业务类型 gm邮件，运营邮件，激活码邮件等
   sendtime=1464458806,  --发送时间
   plugin="{20001:1}",   --插件
   content="test",       --内容
   title="test"          --标题
}

-- usermail
{
  mail_num = 1,    邮件数
  total_no = 1,    邮件编号递增
  max_global_id = 1,  最大的全局邮件号
  content = {
    [1] = {id = 1, type = 0}   邮件所属类型， 以及所属类型的编号
  }
}
--]]

local CMD = {}

local mail_service
local mail_content         -- 邮件列表
local change_flag = false
local init_flag = false

function agent_mail.check_save_db()
    if change_flag then
        skynet.call(redis_service, "lua", "hset_obj", "usermail", uin, mail_content)
        change_flag = false
    end
end

--获取邮件
local function handle_get_mail(cmd, content, len)
    local getnum = proto.parse_proto_req(cmd, content, len).GetNum

    local data = {}
    local mail_list = {}
    local mailid_set = {}
    local mail_num = 0
    local have_new_mail = false

    logger.log("get_mail  uin: ".. uin  .. " get_mail: " .. getnum .. " total_no: " .. mail_content.total_no)

    --if getnum < mail_content.total_no then
        for k, mail in pairs(mail_content.content) do
            table.insert(mailid_set, k)
            if getnum < k then
                local mailinfo
                mailinfo = skynet.call(mail_service, "lua", "get_maildata", mail.type, mail.id)
                if mailinfo then
                    local mailstat = {}
                    mailstat.Id = k
                    mailstat.Content = mailinfo.content
                    mailstat.PlugIn = mailinfo.plugin
                    mailstat.Title = mailinfo.title
                    mailstat.SendTime = mailinfo.sendtime
                    mailstat.OutDate = mailinfo.outdate

                    table.insert(mail_list, mailstat)
                    mail_num = mail_num + 1
                    have_new_mail = true
                end
            end
        end
    --end

    if have_new_mail then
        data = {
            NewMail = true,
            MailNO = mail_content.total_no,
            MailInfo = mail_list,
            MailIdSet = mailid_set
        }
    else
        data = {
            NewMail = false,
            MailNO = mail_content.total_no,
            MailIdSet = mailid_set
        }
    end

    send_response(cmd, data)
end

--删除邮件
local function handle_del_mail(cmd, content, len)
    local mail_no_set = proto.parse_proto_req(cmd, content, len).Id

    local ret_list = {}
    if mail_no_set then
        for _, mail_no in ipairs(mail_no_set) do
            local mail = mail_content.content[mail_no]
            local ret = 0
            if not mail then
                ret = COMMON.ERRORCODE.MAIL_ID_NOT_EXIST
            else
                --获取邮件内容，并给玩家加上相应的
                local mail_cont = skynet.call(mail_service, "lua", "get_maildata", mail.type, mail.id)
                if mail_cont then
                    local business_type = mail_cont.business_type or COMMON.change_src.mail
                    player.add_gift_by_str(mail_cont.plugin, account_info, business_type)
                    logger.tracef("retrive_mail id=%d, mail_no=%d, title=%s, plugin=%s ", mail.id, mail_no, mail_cont.title, mail_cont.plugin)
                end

                if mail_content.content[mail_no].type == COMMON.mail_type.SINGLE then
                    skynet.call(mail_service, "lua", "del_mail", mail.type, mail.id)

                end
                mail_content.mail_num  = mail_content.mail_num - 1
                mail_content.content[mail_no] = nil
                change_flag = true
            end
            table.insert(ret_list, ret)
        end
    end

    local data = {Ret = ret_list}
    send_response(cmd, data)
end

-- 需要考虑到下线邮件处理, 定义统一的下线接口，并在里面添加这个函数
local function mail_update()
    --if online_flag then  --Todo: 这段代码后面再考虑怎么分解
        if mail_content then
            skynet.call(redis_service, "lua", "hset_obj", "usermail", uin, mail_content)
        end
        skynet.timeout(5 * 60 * 100, mail_update)
    --end
end

--邮件初始化
local function init()
    --uin:mail结构: {total_no=4, mail_num=4, max_global_id=3, content={[1]={type = 0, id = 1}}
    mail_content = skynet.call(redis_service, "lua", "hget_obj", "usermail", uin)

    if not mail_content then
        mail_content = {total_no = 0, mail_num = 0, max_global_id = 0, content = {} }
        change_flag = true
    end

    local global_no = mail_content.max_global_id

    --获取全局更新邮件
    local mail_nums, mail_list = skynet.call(mail_service, "lua", "get_global_mail", global_no, account_info.create_time)
    if mail_list and mail_nums > 0 then

        for i=#mail_list, 1, -1 do
            mail_content.total_no = mail_content.total_no + 1
            mail_content.mail_num = mail_content.mail_num + 1
            local stat = {type = 1, id = mail_list[i] }
            mail_content.content[mail_content.total_no] = stat
        end

        mail_content.max_global_id = mail_list.max_id
    end

    --超过100封邮件就进行删除
    if mail_content.mail_num > 100 then
        local key_table = {}
        for key,_ in pairs(mail_content.content) do
            table.insert(key_table, key)
        end
        table.sort(key_table)
        local delete_num = mail_content.mail_num - 100
        for _, key in ipairs(key_table) do
            if delete_num < 0 then
                break
            end
            mail_content.content[key] = nil
        end
        change_flag = true
    end
end

--邮件在重连时需要重新加载 , 第一个参数为事件类型, 第二个参数为重连标识
local function reinit(_, reconnect_flag)
    if not reconnect_flag then return end

    --获取全局更新邮件
    local global_no = mail_content.max_global_id
    local mail_nums, mail_list = skynet.call(mail_service, "lua", "get_global_mail", global_no, account_info.create_time)
    if mail_list and mail_nums > 0 then

        for i=#mail_list, 1, -1 do
            mail_content.total_no = mail_content.total_no + 1
            mail_content.mail_num = mail_content.mail_num + 1
            local stat = {type = 1, id = mail_list[i] }
            mail_content.content[mail_content.total_no] = stat
        end

        mail_content.max_global_id = mail_list.max_id
        change_flag = true
    end
end

--邮件处理  type:系统邮件和单点邮件的类型区分
local function add_mail(mail_type, mail_id)
    logger.tracef("add_mail mail_type:%d mail_id:%d ", mail_type, mail_id)
    if mail_type == 1 then
        mail_content.max_global_id = mail_content.max_global_id + 1
    end
    mail_content.total_no = mail_content.total_no + 1
    mail_content.mail_num = mail_content.mail_num + 1
    local mail = {type = mail_type, id = mail_id}
    mail_content.content[mail_content.total_no] = mail

    change_flag = true

    --给客户端发送有邮件增加的消息
    send_response(COMMON.MESSAGE.CS_CMD_NOTICE_MAIL, {})
end

-- 添加agent命令
local function add_cmd(CMD)
    CMD.add_mail        = add_mail
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_GET_MAIL]  = handle_get_mail
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_DEL_MAIL]  = handle_del_mail
end

function agent_mail.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code

    account_info  = args.info
    uin           = args.uin
    player        = args.player
    redis_service = args.redis
    CMD           = args.cmd
    mail_service  = args.mail_service

    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)

    init()

    EventCenter.addEventListener(Events.AGENT_EXIT, agent_mail.check_save_db)
    EventCenter.addEventListener(Events.AGENT_OPEN, reinit)
end

return agent_mail


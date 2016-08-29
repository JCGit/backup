--这个文件主要是做agent相关处理的
local skynet       = require "skynet"
local socketdriver = require "socketdriver"
local datacenter   = require "datacenter"
local mc           = require "multicast"
local json         = require "cjson"
json.encode_sparse_array(true)

-- 设置成global
EventCenter = require 'event_center'
Events      = require 'game_events'

local logger                = require "logger"
local proto                 = require "proto"
local utils                 = require "utils"
local sharedata_utils       = require "sharedata_utils"
local setting               = require "setting"
local db_setting            = require "db_setting"
local COMMON                = require "common"
local MESSAGE               = COMMON.MESSAGE
local ERRORCODE             = COMMON.ERRORCODE
local player                = require "player"
local activity_recharge     = require "activity_recharge"
local activity_power_energy = require "activity_power_energy"
local agent_newbie          = require "agent_newbie"
local gift_code             = require "gift_code"
local agent_pvp             = require "agent_pvp"
local agent_friend          = require "agent_friend"
local agent_pve             = require "agent_pve"
local agent_activity        = require "agent_activity"
local agent_glyphs          = require "agent_glyphs"
local agent_vip             = require "agent_vip"
local activity_christmas    = require "activity_christmas"
local activity_new_year     = require "activity_new_year"
local agent_activitys       = require "agent_activitys"
local agent_daily_task      = require "agent_daily_task"
local activity_valentine    = require "activity_valentine"
local agent_mail            = require "agent_mail"
local agent_shop            = require "agent_shop"
local activity_sign         = require "activity_sign"
local agent_player          = require "agent_player"
local agent_achievement_v2  = require "agent_achievement_v2"
local agent_operate_activity = require "agent_operate_activity"
local agent_achievement     = require "agent_achievement"
local activity_seven_goal   = require "activity_sevenday_goal"
local agent_assistant       = require "agent_assistant"
local agent_institute       = require "agent_institute"
local agent_item            = require "agent_item"
local agent_pvp_chest       = require "agent_pvp_chest"
local agent_skin            = require "agent_skin"
local agent_pay_guide       = require "agent_pay_guide"



local redis_service
local mail_service
local busilog_service
local operate_activity_service

local user_fd
local uin               -- user id
local account_info      -- 玩家属性
local multicast_handler -- 公告句柄

local init_flag = false -- agent初始化标识
local destroy_prepare_ts = 0
local destroy_ts         = setting.game_setting.AGENT_REMAIN_TIME
local online_flag        = false

local gamed = ...

local string_rep  = string.rep
local string_pack = string.pack
local lcrypt      = require 'lcrypt'
local lcrypt_key  = lcrypt.key

-- agent open 的时候设置, blowfish key userdata
local bfkey
local is_dev = setting.debug.is_dev
local dev_verbose_mode = setting.debug.dev_verbose_mode

local dev_log_network_pack = false
local msg_file
local msg_id, send_msg_id = 0, 0 -- 消息序号
local crypt = require 'crypt'

local ANSI_COLOR_RED    = '\x1b[31m'
local ANSI_COLOR_GREEN  = '\x1b[32m'
local ANSI_COLOR_YELLOW = '\x1b[33m'
local ANSI_COLOR_BLUE   = '\x1b[34m'
local ANSI_COLOR_RESET  = '\x1b[0m'

local output_msgs = {
    MESSAGE.CS_CMD_PROPERTY_UPDATE,
    MESSAGE.CS_CMD_PVP_SELF_RANKLIST,
}

-- 不需要输出的消息【主要是pvp中比较频繁的消息】
local notput_msgs = {
    MESSAGE.CS_CMD_PVP_FIGHT,
    MESSAGE.CS_CMD_PVP_UPGRADE,
    MESSAGE.CS_CMD_PVP_MAP,
    MESSAGE.CS_CMD_NOTICE,
}

---------------- 公共接口 -------------------
local function send_package(cmdid, pack)
    if user_fd then
        -- debug
        if is_dev and cmdid ~= MESSAGE.CS_CMD_HEART_BEAT then
            if not utils.elem(notput_msgs, cmdid) then
                logger.tracef('\x1b[32msend %s %d to client\x1b[0m',
                    COMMON.MESSAGE_HELPER[cmdid], #pack)
            end

            if dev_verbose_mode and utils.elem(output_msgs, cmdid) then
                logger.trace(ANSI_COLOR_GREEN, '\n',
                    logger.dump(proto.parse_proto_resp(cmdid, pack, #pack)),
                    ANSI_COLOR_RESET)
            end
        end

        if dev_log_network_pack then

            if not msg_file then
                local fmt = 'msg_%d_%s.txt'
                msg_file = assert(io.open(fmt:format(uin, os.date('%H_%M_%S')), 'wb'))
            end
            local msg = {
                id        = send_msg_id,
                cmdid     = cmdid,
                msg       = crypt.base64encode(pack),
                msg_size  = #pack,
                msg_name  = COMMON.MESSAGE_HELPER[cmdid],
                timestamp = skynet.time(),
                is_recv   = false,
            }
            send_msg_id = send_msg_id + 1
            msg_file:write(json.encode(msg), '\n')
            msg_file:flush()
        end

        local rl = #pack
        if rl > 65531 then --包长度大于64K的判断
            logger.error("packet now over 64K")
        end

        local m8 = rl % 8
        if m8 ~= 0 then pack = pack .. string_rep(0, 8 - m8) end
        local header = string_pack(">I2I2I2", #pack + 4, cmdid, rl)
        bfkey.iv = '12345678'
        socketdriver.send(user_fd, {header, bfkey:encrypt(pack)})
    else
        -- debug
        if is_dev and cmdid ~= MESSAGE.CS_CMD_HEART_BEAT then
            local cmd = COMMON.MESSAGE_HELPER[cmdid]
            logger.tracef('client disconnect:, uin:%d discard msg %s', uin, cmd)
        end
    end
end

local send_response, send_error_code
do
    local p1 = proto.pack_proto_resp
    local p2 = proto.pack_error_resp
    function send_response(cmdid, resp)
        return send_package(cmdid, p1(cmdid, resp))
    end

    local error_code_tb = { ErrCode = 0 }
    function send_error_code(cmdid, errid)
        error_code_tb.ErrCode = errid
        return send_package(cmdid, p2(cmdid, error_code_tb))
    end
end

local function deal_exit()
    -- 房间处理
    agent_pvp.deal_exit()

    if account_info.change_flag == true then
        skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info)
        player.change_flag(account_info, false)
    end

    EventCenter.dispatchEvent(Events.AGENT_EXIT)

    skynet.exit()
end

local function destroy_agent()
    logger.tracef("agent %d exit", uin)
    skynet.call(gamed, "lua", "kickoff", skynet.self(), uin)
    deal_exit()
end


--公告订阅
local function announce_subscribe()
    -- 公告广播频道
    local announce_channel = datacenter.get "MCCHANNEL"
    if not announce_channel then
        error("have no multicast_service channel")
    end

    multicast_handler = mc.new {
        channel = announce_channel,
        dispatch = function (_, _, cmdid, data)
            if user_fd then
                local content
                if cmdid == MESSAGE.CS_CMD_PVP_CHAT then
                    content = proto.pack_proto_resp(cmdid, data)
                else
                    content = proto.pack_proto_s2c_req(cmdid, data)
                end
                send_package(cmdid, content)
            end
        end,
    }

    multicast_handler:subscribe()
end

-- 订阅游戏服务器广播事件
local function subscribe_server_event()
    -- 多播对象的生命周期一定要注意，超出生命周期后就会被回收，从而会取消订阅
    g_server_event_subscriber = mc.new {
        channel = datacenter.get 'SERVER_EVENT_CHANNEL',
        dispatch = function (channel, source, event, ...)
            if event == 'PVP_NOPROP_INFO' then
                EventCenter.dispatchEvent(Events.PVP_NOPROP_INFO, ...)
            end
        end,
    }
    g_server_event_subscriber:subscribe()
end

-- client_close 是否是客户端主动关闭
local function deal_offline(type)
    if not online_flag then return end

    logger.trace("deal_offline type: ", type, uin)
    local client_close = type == COMMON.offline_type.CLIENT_CLOSE and true or false
    skynet.call(gamed, "lua", "kick", skynet.self(), user_fd, uin, client_close)

    account_info.login_out_time = os.time()
    player.change_online_time(account_info)
    player.change_flag(account_info)   --写redis
    
    --记录下进入排行榜的玩家的登出时间，
    --以便统计长时间不登录的玩家从排行榜删除
    if account_info.pvp_point > 0 then
        local in_rank_info = {
                    login_out_time = os.time(),
                }
        skynet.call(redis_service, "lua", "hset_obj", "in_pvp_rank_info",
            account_info.uin, in_rank_info)
    end
    
    --信鸽处理
    local xinge_info = skynet.call(redis_service, "lua", "hget_obj", "xinge", account_info.uin)
    xinge_info.login_out_time = os.time()
    skynet.call(redis_service, "lua", "hset_obj", "xinge", account_info.uin, xinge_info)

    skynet.send(busilog_service, "lua", "log_login", account_info , COMMON.player_operation.player_login_out)
    skynet.send(".online_mgr","lua","offline", uin, skynet.self())

    online_flag = false
    user_fd = nil
end

-- 心跳包检测
local last_heartbeat_time
local HEARTBEAT_TIME_MAX = setting.game_setting.HEARTBEAT_TIME_MAX
local CACHE_TIME_MAX = setting.game_setting.AGENT_REMAIN_TIME
local heartbeat_check_time = 15 * 100  --30秒检测一次心跳
local function heartbeat_check()
    skynet.timeout(heartbeat_check_time, heartbeat_check)
    local t = skynet.now() - last_heartbeat_time
    if t > HEARTBEAT_TIME_MAX  and t < 2 * HEARTBEAT_TIME_MAX then
        deal_offline(COMMON.offline_type.HEARTBEAT_FAIL)
    end

    if t > HEARTBEAT_TIME_MAX  + CACHE_TIME_MAX then
        logger.tracef("cache check fail %d", uin)
        destroy_agent()
    end
end

local chat_count_check_time = setting.game_setting.chat_count_check_time
local function free_chat_check()
    local flag = false
    local tnow = math.floor(skynet.time())
    if account_info.last_chat_time == 0 then
        account_info.free_chat_count = setting.game_setting.max_chat_count
        account_info.last_chat_time = tnow
        flag = true
    end

    if  account_info.free_chat_count <  setting.game_setting.max_chat_count and account_info.last_chat_time + setting.game_setting.chat_count_check_time <= tnow then
        local add = math.floor((tnow - account_info.last_chat_time) /  setting.game_setting.chat_count_check_time)
        account_info.free_chat_count = account_info.free_chat_count + add
        account_info.last_chat_time = tnow
        if account_info.free_chat_count > setting.game_setting.max_chat_count then
            account_info.free_chat_count =  setting.game_setting.max_chat_count
        end
        flag = true
    end

    if flag then
        player.change_chat_data(account_info)
        player.change_flag(account_info)
    end
    skynet.timeout(3*100, free_chat_check)
end

-- 体力自然增长
local power_check_time = setting.game_setting.power_check_time
local function power_check()
    local tnow = math.floor(skynet.time())
    if account_info.last_power_check_time < tnow then
        local power_slot = account_info.power_slot
        -- 应急处理 让玩家能正常恢复精力
        -- 客户端可能会让power_slot为0 需要客户端查看
        if power_slot < 50 then
            power_slot = 50
        end
        local n = math.floor((tnow - account_info.last_power_check_time) / power_check_time)
        if n > 0 then
           account_info.last_power_check_time = tnow
           player.change_flag(account_info)
        end
        if account_info.power < power_slot then
            n = account_info.power + n > power_slot and power_slot - account_info.power or n
            player.add_power(account_info, n)
        end
    else
        account_info.last_power_check_time = tnow
    end
    skynet.timeout(power_check_time * 100, power_check)
end

-- 重置玩家属性值
local function clear_player_attr(attrs)
    if type(attrs) ~= "table" then
        return
    end

    for k,v in pairs(attrs) do
        account_info[v] = db_setting.account_attr[v]
    end
end

-- 跨天刷新开启的活动列表
local function refresh_activitys()
    local open_activity = {}
    -- 注意顺序问题(和前端确认)
    activity_sign.get_open_activity(open_activity)
    activity_recharge.get_open_activity(account_info, open_activity)
    activity_power_energy.get_open_activity(open_activity) --在主界面显示，暂不返送
    activity_christmas.get_open_activity(open_activity)
    activity_new_year.get_open_activity(open_activity)
    activity_valentine.get_open_activity(open_activity)
    send_response(MESSAGE.CS_CMD_GET_OPEN_ACTIVITY, { OpenActivityInfo = open_activity })
end

-- 跨天登录days~=0
-- 在线跨天notice_flag = true
local function daily_login_check(notice_flag)
    local days = utils.getTimeInteral(account_info.last_day_login_time, os.time())

    --玩家跨天登录处理,notice_flag为true说明是系统跨天调用此函数
    if (days ~= 0) then
        EventCenter.dispatchEvent(Events.DAY_CHANGE, notice_flag)
        account_info.last_day_login_time = os.time()
        clear_player_attr({
            "streak_win_count", "power_buy_count", "watch_movie_times", "cur_continue_win",
            "cur_continue_lost", "share_data", "day_onilne_time"
            })
        -- [推荐做法]： 在自己的模块处理隔天数据
        agent_player.daily_login_check(notice_flag)
        agent_pvp_chest.daily_login_check(notice_flag)
        agent_pay_guide.daily_login_check(notice_flag)
        player.change_flag(account_info)

        --如果在线跨天主动通知活动变更
        if notice_flag then
           -- 刷新开启的活动列表
           refresh_activitys()

           -- 获取充值信息
           activity_recharge.handle_get_activity_data(MESSAGE.CS_CMD_GET_ACTIVITY_DATA, '', 0)
        end
    end


    activity_new_year.daily_check(notice_flag)
    activity_valentine.daily_check(notice_flag)
    activity_sign.daily_check(notice_flag)
    agent_shop.daily_check(notice_flag)
end

local function handle_heart_beat_msg(cmdid, content, len)
    return send_response(cmdid, { Ret = ERRORCODE.OK, Ts = math.floor(skynet.time()) })
end

local function handle_logout_msg(cmd, content, len)
    logger.trace("client active log out")
    deal_offline(COMMON.offline_type.LOGOUT)
end


-- 消息处理映射表【各子模块处理各自的协议】
local cmd_handle_mapping = {
    [MESSAGE.CS_CMD_LOGOUT]             = handle_logout_msg,
    [MESSAGE.CS_CMD_HEART_BEAT]         = handle_heart_beat_msg,
    -- 其他协议
}

local CMD = {}

--[[
商品 充值表 配置说明
id          编号 php充值回调接口中关联的shopID
type        模块类型 1: 钻石 2: 礼包 3: VIP
diamond     充值金额
diamondStr  充值显示
manual      钻石
img         钻石图片
icon        推荐标识 1: 月卡; 2: 限购; 3: 推荐;

gift        赠送
giftCout    次数 赠送次数, 月卡的赠送次数为30,其他的是首次购买赠送

text        描述
sdkType     支付 sdk 类型:  移动联通#电信#网游
text        备注
name        礼包名称
start_time  开始时间 (yyyy-MM-dd HH:mm:ss)
end_time    结束时间
leftCount   剩余数量 全服限量类礼包的限制数量， 为0表示不限量；与有效日期关联
            有效日期改变后，服务器会重设限制数量
giftId      礼包ID
discount    折扣
price       价格
giftIcon    礼包icon
--]]

local callback_logger = require "callback_logger"
-- 充值回调
-- pay_amount : 平台充值时的金额
-- pay_currency : 平台充值时的货币类型
function CMD.recharge_change_diamond(recharge_diamond, recharge_amount, order_id, shopId, pay_amount, pay_currency)
    local shop_data = sharedata_utils.get_stat(COMMON.plan.SHOP_DATA, shopId)
    local recharge_gift = skynet.call(redis_service, "lua", "hget_obj", "player_recharge_gift", uin) or {}
    local is_first = true

    -- 解决累积充值排除额外送的钻石
    local add_diamond = recharge_diamond

    -- 1 和 13 是月卡 不做首次充值赠送处理
    if shopId ~= 1 and shopId ~= 13 then
        if recharge_gift[shopId] then
            if recharge_gift[shopId].count >= shop_data.giftCout then
                is_first = false
            else
                recharge_gift[shopId].count = recharge_gift[shopId].count + 1
            end
        else
            recharge_gift[shopId] = { count = 1 }
        end
        if is_first then
            skynet.call(redis_service, "lua", "hset_obj", "player_recharge_gift", uin, recharge_gift)
        end
    end

    -- NOTE: 配置中只有 0， 1， 5， 没有2,3,4
    if shop_data.energy == 1 then
        player.add_month_card(account_info, 30)
    elseif shop_data.energy == 2 then
        -- 额外增加10点体力上限，体力上限最高100点
        player.add_power_solt(account_info, 10)
    elseif shop_data.energy == 5 and is_first then
        -- 钻石
        recharge_diamond = recharge_diamond + shop_data.gift
    end

    if shop_data.type == 2 then --购买礼品
       player.add_gift(account_info, shop_data.giftId, COMMON.change_src.recharge)
    end

    local now_diamond = account_info.diamond
    local src = COMMON.change_src.recharge
    player.change_diamond(account_info, src, recharge_diamond)
    account_info.recharge_amount = account_info.recharge_amount + recharge_amount

    callback_logger.tracef("uin=%d&before=%d&add=%d&after=%d,shopid=%d", 
        uin ,now_diamond,recharge_diamond,account_info.diamond,shopId)
        
    logger.tracef("uin=%d&before=%d&add=%d&after=%d,shopid=%d", 
        uin ,now_diamond,recharge_diamond,account_info.diamond,shopId)

    -- 处理首次充值数据
    activity_recharge.on_recharge(account_info)
    activity_recharge.handle_get_activity_data(MESSAGE.CS_CMD_GET_ACTIVITY_DATA)
    -- 处理累积充值
    agent_operate_activity.on_recharge(add_diamond)

    agent_vip.buy_vip_with_cach(tonumber(shopId))

    -- 充值日志
    skynet.send(busilog_service, "lua", "log_recharge", account_info, 
        order_id, shopId, recharge_amount, recharge_diamond, pay_amount, pay_currency)
    return true
end

local function check_offline_recharge_order()
    local orders = skynet.call(redis_service, "lua", "hget_obj", "offline_recharge_order", account_info.uin)
    if orders then
        for i, order in ipairs(orders) do
            CMD.recharge_change_diamond(order.diamond, order.amount, 
                order.order_id, order.shop_id, order.pay_amount, order.pay_currency)
        end
        skynet.call(redis_service, "lua", "hdel_obj", "offline_recharge_order", account_info.uin)
    end
end

--30s将有修改的account_info写一次数据库
local save_db_time = 30 * 100
local function save_db_check()
    --定时一定要写在前面， 以防出现其他问题导致定时器失效
    skynet.timeout(save_db_time, save_db_check)
    if account_info.change_flag and online_flag then
        skynet.call(redis_service, "lua", "hset_obj", "account", uin, account_info)
        player.change_flag(account_info, false)
    end

    -- 其他内存数据存储处理
    --agent_achievement_v2.save_db_check()
    agent_vip.check_save_db()
    agent_institute.check_save_db()
    agent_item.check_save_db()
    agent_pve.check_save_db()
    agent_mail.check_save_db()
    agent_assistant.check_save_db()
    agent_skin.check_save_db()
    agent_pay_guide.check_save_db()

end

function CMD.open(fd, id, bfkey_, retry, IMEI_)
    logger.tracef("\x1b[31m新玩家进入 open uin: %d fd : %d %s\x1b[0m", id, fd, retry and '重连' or '')

    local reconnect = retry and true or false

    --在线状态
    online_flag     = true
    user_fd         = fd
    uin             = id
    bfkey           = lcrypt_key('blowfish', 'cbc', bfkey_, '12345678')
    IMEI            = IMEI_

    -- 装载玩家数据
    if not account_info then
        reconnect = false
        account_info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
        if not account_info then
            logger.warningf("agent.lua CMD.open get account_info failed : %s", uin)
            return skynet.exit()
        end
    else
        reconnect = true
    end

    -- 校验所有属性 如果没有就初始化
    local is_change = false
    local account_attr = db_setting.account_attr
    for k, v in pairs(account_attr) do
        if account_info[k] == nil then
            account_info[k] = v
            player.change_flag(account_info)
        end
    end

    -- 新用户最后登录时间默认为进入时间
    account_info.login_time = os.time()
    if account_info.last_day_login_time == 0 then
        account_info.last_day_login_time = os.time()
    end
    player.change_flag(account_info)

    -- 只有首次cmd.open才需要【初始化模块，注册事件，启动定时器】 重连不需要
    local me = skynet.self()
    
    if not init_flag then
        init_flag = true
        local args = {
            cmd             = CMD,
            cmd_map         = cmd_handle_mapping,
            send_package    = send_package,
            send_response   = send_response,
            send_error_code = send_error_code,
            agent           = me,
            info            = account_info,
            uin             = uin,
            player          = player,
            reconnect       = reconnect,
            redis           = redis_service,
            rediscall       = function(...) return skynet.call(redis_service, "lua", ...) end,
            mail_service    = mail_service,
            busilog_service = busilog_service,
        }

        -- 模块初始化
        player.init(args)
        agent_mail.init(args)
        agent_vip.init( args)
        agent_player.init(args)
        agent_item.init(args)
        agent_pvp.init(args)
        agent_friend.init(args)
        --agent_achievement_v2.init(args)
        agent_achievement.init(args)
        agent_newbie.init(args)
        gift_code.init(args)
        agent_glyphs.init(args)
        agent_pve.init(args)
        agent_daily_task.init(args) -- 依赖pve模块,开启条件中包括检查关卡进度
        agent_activitys.init(args)  -- 新版服务器时间管理
        agent_shop.init(args)
        agent_activity.init(args)   -- 老版服务器时间管理兼勇者之心，王者之路，常胜将军
        agent_assistant.init(args)
        agent_institute.init(args)
        agent_skin.init(args)

        --活动初始化
        activity_power_energy.init(args)
        activity_christmas.init(args)
        activity_valentine.init(args)
        activity_sign.init(args)
        activity_recharge.init(args)
        activity_seven_goal.init(args)
        agent_pvp_chest.init(args)
        agent_pay_guide.init(args)

        -- 运营活动模块初始化
        local lists = skynet.call(operate_activity_service, "lua", "get_op_activities", uin)
        agent_operate_activity.init(args, lists, operate_activity_service)

        -- 启动定时器
        last_heartbeat_time = skynet.now()
        heartbeat_check()
        power_check()
        free_chat_check()
        save_db_check()
        skynet.timeout(200, agent_player.check_month_card_reward)

        agent_pvp.reinit()
    end

    --登陆日志
    skynet.send(busilog_service, "lua", "log_login", account_info , COMMON.player_operation.player_login)

    daily_login_check()
    player.change_vip(account_info, agent_vip.get_vip_level())
    player.set_agent_vip(agent_vip)
    check_offline_recharge_order()

    -- 注册事件
    EventCenter.dispatchEvent(Events.AGENT_OPEN, reconnect)
    -- 通知上线
    skynet.send(".online_mgr", "lua", "online", id, me)
end

-- 发送GM命令
function CMD.gmcmd(gmtype, value, value2)
    if gmtype == COMMON.GM_TYPE.ADD_POWER then
        player.add_power(account_info, value)
    elseif gmtype == COMMON.GM_TYPE.ADD_DIAMOND then
        player.change_diamond(account_info, COMMON.change_src.gm, value)
    elseif gmtype == COMMON.GM_TYPE.FORBID_LOGIN then
        account_info.can_login_time = value
        player.change_flag(account_info, false, true)
        skynet.call(gamed, "lua", "kick", skynet.self(), user_fd, uin)
    elseif gmtype == COMMON.GM_TYPE.FORBID_SPEAK then
        account_info.can_speak_time = value
        player.change_flag(account_info)
    elseif gmtype == COMMON.GM_TYPE.CHANGE_LEVEL then
        account_info.level = value
        account_info.exp = value2
        player.change_flag(account_info)
    elseif gmtype == COMMON.GM_TYPE.ADD_SCORE then
        account_info.score = value
    elseif gmtype == COMMON.GM_TYPE.CHANGE_PLAYER_NAME then
        local ret = agent_player.change_player_name(value, true, account_info)
        player.change_flag(account_info)
        return ret == ERRORCODE.OK
    elseif gmtype == COMMON.GM_TYPE.ADD_VIP then
        agent_vip.gm_vip(value)
        return ERRORCODE.OK
    end
end

-- 获取玩家信息
function CMD.get_account_info()
    return account_info
end

-- 设置玩家属性【只修改内存】
function CMD.set_account_info(key, value)
    account_info[key] = value
    player.change_flag(account_info)
end

--存储玩家信息
function CMD.save_account_info()
    skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info)
    player.change_flag(account_info, false)
end

--踢掉重复登录的老用户
function CMD.kick(flag, IMEI_)
    if user_fd then
        if flag and IMEI ~= IMEI_ then
            local cmdid = MESSAGE.CS_CMD_KICK_PLAYER
            send_package(cmdid, proto.pack_proto_s2c_req(cmdid, {}))
        end
        skynet.call(gamed, "lua", "kick", skynet.self(), user_fd, uin)
        user_fd = nil
    end
end

--网络连接断开
function CMD.disconnect(id, fd)
    if uin ~= id then
        return logger.errorf("disconnet uin isn't the same uin :%d id : %d", uin, id)
    end
    --和当前user_fd不一致的关闭不处理
    if user_fd ~= fd then return end

    deal_offline(COMMON.offline_type.CLIENT_CLOSE)
end

-- 往客户端发送信息
-- 同一个服务内，不要使用skynet.call/send这种方式来调用，开销比直接调用大
function CMD.send_pack(cmdid, content)
    send_package(cmdid, content)
end

--停服agent销毁
function CMD.agent_stop_exit()
    local cmdid = MESSAGE.CS_CMD_STOP_KICK_PLAYLER
    --send_package(cmdid, proto.pack_proto_s2c_req(cmdid, {}))
    deal_exit()
end

-- 针对已经在一个gameserver有agent,重定位到另一个gameserver中，销毁agent缓存
function CMD.exit_agent()
    if user_fd then
        local cmdid = MESSAGE.CS_CMD_KICK_PLAYER
        send_package(cmdid, proto.pack_proto_s2c_req(cmdid, {}))
    end
    deal_exit()
end

-- 用于进程退出保存数据
function CMD.exit_save_db()
    if account_info.change_flag == true then
        skynet.call(redis_service, "lua", "hset_obj", "account", account_info.uin, account_info)
        player.change_flag(account_info, false)
    end
end

-- 处理凌晨在线玩家的数据改变
function CMD.process_daily_check()
    daily_login_check(true)
end

--发布公告
function CMD.publish_announce(kind, data)
    multicast_handler:publish(kind, data)
end

-- 注册客户端消息处理协议
local p_parse_prototype = proto.parse_prototype
skynet.register_protocol {
    name = "client",
    id = skynet.PTYPE_CLIENT,
    unpack = function(msg, sz)
        return p_parse_prototype(msg, sz, bfkey)
    end,
    dispatch = function(_, _, cmd, content, len)

        if dev_log_network_pack then

            if not msg_file then
                local fmt = 'msg_%d_%s.txt'
                msg_file = assert(io.open(fmt:format(uin, os.date('%H_%M_%S')), 'wb'))
            end
            local msg = {
                id        = msg_id,
                cmdid     = cmd,
                msg       = crypt.base64encode(content),
                msg_size  = len,
                msg_name  = COMMON.MESSAGE_HELPER[cmd],
                timestamp = skynet.time(),
                is_recv   = true,
            }
            msg_id = msg_id + 1
            msg_file:write(json.encode(msg), '\n')
            msg_file:flush()
        end

        last_heartbeat_time = skynet.now()
        if is_dev and cmd ~= MESSAGE.CS_CMD_HEART_BEAT then
            if not utils.elem(notput_msgs, cmd) then
                logger.trace('\x1b[33mrecv ', COMMON.MESSAGE_HELPER[cmd],
                    cmd, uin, ANSI_COLOR_RESET)
            end
        end
        local handle = cmd_handle_mapping[cmd]
        if handle then
            return handle(cmd, content, len)
        else
            return logger.tracef('unknown cmdid %d from client socket!', cmd)
        end
    end,
}

skynet.start(function()
    redis_service = skynet.queryservice("redis_service")
    mail_service = skynet.queryservice("mail_service")
    busilog_service = skynet.queryservice("business_log_service")
    operate_activity_service = skynet.queryservice("operate_activity_service")

    proto.load_proto()
    announce_subscribe()

    subscribe_server_event()

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
        return skynet.retpack(ret)
    end)
end)

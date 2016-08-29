local skynet = require "skynet"
local utils  = require "utils"
local logger = require "logger"
local COMMON = require "common"
local sharedata_utils = require "sharedata_utils"
local proto   = require "proto"
local player  = require "player"
local setting = require "setting"

local account_info
local agent
local redis_service

local agent_vip = {}

local vip_data
local send_package, send_error_code, send_response

local hash_table = "account_business"
local off_table = "offline:account_business"

--[[

存储格式:

{
    vip_level = 1,
    vip_create_time = os.time(),
    vip_end_time = os.time() + 7 * 86400,
    mail_time = os.time(),
    vip_freeze = vip_level|freeze_time|remain_time#vip_level|freeze_time|remain_time,
    expired = true,
}

说明:
    vip_level  会员级别  1  贵族1  2 贵族2  3 贵族3
    vip_create_time  会员购买时间
    vip_end_time  会员失效时间
    mail_time  会员每日礼包发送的时间(没有此功能就不会有此字段，如繁体版不需要会员每日礼包)
    vip_freeze 冻结的会员，会员可以累计购买(拥有会员时，再购买比当前级别大的时会冻结当前会员)
    expired 会员是否失效，只有会员失效了才会有此字段
]]


local flush = false

--按会员级别更新每日礼包可领取
local function update_daily(level, usage)
    local update_data = {}
    if level == 1 then
        skynet.call(agent, "lua", "refresh_mission", COMMON.mission_type.VIP1, usage)
    elseif level == 2 then
        skynet.call(agent, "lua", "refresh_mission", COMMON.mission_type.VIP2, usage)
    elseif level == 3 then
        skynet.call(agent, "lua", "refresh_mission", COMMON.mission_type.VIP3, usage)
    else
        logger.debugf("the invalidate vip level[%d]", level)
    end
end


--更新精力，体力上限
local function update_slot(level)
    local vip_cfg = sharedata_utils.get_stat(COMMON.plan.Vip, level)
    if vip_cfg then
        account_info.power_slot = vip_cfg["powerSlot"]
        player.change_flag(account_info)
    else
        logger.errorf("not found privilege configuration for level[%d]", level)
    end
end

local function reset_slot()
    account_info.power_slot = setting.constant.player_default_power_solt
    player.change_flag(account_info)
end

local function notify_update(vip_levl, remain_time)
    player.change_vip(account_info, vip_levl)
    local resp_data = {
        VipLevel = vip_levl,
        RemainTime = remain_time
    }
    send_response(COMMON.MESSAGE.CS_CMD_BUY_VIP,resp_data)
end

local function send_vip_mail(title, content, plugin)
    local data = {
        title = title,
        content = content,
        plugin = plugin,
        sendtime = now,
    }
    skynet.call(".mail_service", "lua", "add_mail", account_info.uin, COMMON.mail_type.SINGLE, data, COMMON.change_src.mail)
end

-- 购买当天就发送邮件，下次登录或隔天再发送
local function do_send_mail(endTime)
    local days = utils.getTimeInteral(utils.getSameDayBeginTime(vip_data.mail_time or os.time()), utils.getSameDayBeginTime(endTime))
    if days ~= 0 then
        local vip_cfg = sharedata_utils.get_stat(COMMON.plan.Vip, vip_data.vip_level)
        if vip_cfg and vip_cfg.giftId ~= 0 then
            local gift_cfg = sharedata_utils.get_stat(COMMON.plan.Gift, vip_cfg.giftId) 
            local title = vip_cfg.mailTitle
            local content = vip_cfg.mailContent
            local plugin =  gift_cfg and string.format("{%s}",player.gift2plugin(gift_cfg)) or ''
            for v = days,1,-1 do
                send_vip_mail(title, content, plugin)
            end
            vip_data.mail_time = os.time()
            flush = true
        else
            logger.debugf("no vip cfg for level[%d] giftId[%d]", vip_data.vip_level, vip_cfg.giftId)
        end
    end
end

-- 过期后，解冻被冻结的会员
local function update_freeze()
    local tnow = os.time()
    local vip_freeze = utils.split(vip_data.vip_freeze,'#')
    --"vip_level|freeze_time|remain_time#vip_level|freeze_time|remain_time"
    --优先找出大的level
    if #vip_freeze > 1 then
        --有多个拿出优先级高的
        local max_level = 0
        local find_index = 0
        for i = 1, #vip_freeze do
            local param = utils.split(vip_freeze[i],'|')
            if tonumber(param[1]) > max_level then
                max_level = tonumber(param[1])
                find_index = i
            end
        end

        if find_index ~= 0 then
            local param = utils.split(vip_freeze[find_index],'|')
            vip_data.vip_level = tonumber(param[1])
            vip_data.vip_create_time = tnow
            vip_data.vip_end_time = tnow + tonumber(param[3])

            --remove find_index
            table.remove(vip_freeze,find_index)
            --concat with '#'
            vip_data.vip_freeze = table.concat(vip_freeze,'#')
        end
    else
        --只有一个
        local param = utils.split(vip_freeze[1],'|')
        vip_data.vip_level = tonumber(param[1])
        vip_data.vip_create_time = tnow
        vip_data.vip_end_time = tnow + tonumber(param[3])
        vip_data.vip_freeze = nil
    end

    --update_daily(vip_data.vip_level, true)
    update_slot(vip_data.vip_level)
    flush = true

    -- 解冻后通知前台
    local timStr = ''
    timStr = timStr .. vip_data.vip_level .. '|' .. vip_data.vip_end_time - os.time()
    if vip_data.vip_freeze then
        timStr = timStr .. '#' .. agent_vip.get_low_level_time(vip_data.vip_freeze)
    end
    notify_update(vip_data.vip_level, timStr)
end

local function load_vip_data()
    vip_data = skynet.call(redis_service, "lua", "hget_obj", hash_table, account_info.uin,true)
end

local function update_vip_data()
    --是否过期？是否有冻结的？
    if vip_data then
        local now = os.time()
        if not vip_data.expired and now > vip_data.vip_end_time then
            -- 发送会员邮件 
            do_send_mail(utils.getSameDayBeginTime(vip_data.vip_end_time))
            
            if vip_data.vip_freeze then
                update_freeze()
            else
                -- 过期不删除数据，物理删除
                -- skynet.call(redis_service, "lua", "hdel_obj", hash_table, account_info.uin)
                if not vip_data.expired then -- 首次过期则设置过期标记
                    vip_data.expired = true
                    flush = true
                    -- vip日常任务重置
                    --update_daily(vip_data.vip_level, false)
                    -- 过期通知
                    notify_update(0, '')

                    -- 精力，体力重置
                    reset_slot()
                end
            end
        end
    end
end


function agent_vip.get_vip_data_by_uin(uin)
    -- 存在去检查是否过期，过期了更新冻结的，无冻结的就说明不是会员
    local data = skynet.call(redis_service, "lua", "hget_obj", hash_table, uin,true)
    -- 过期返回空
    if data and data.expired then
        return nil
    end
    return data
end

function agent_vip.get_vip_data()
    -- 存在去检查是否过期，过期了更新冻结的，无冻结的就说明不是会员
    if vip_data then
        if not vip_data.expired and os.time() > vip_data.vip_end_time then
            update_vip_data()
        end
    end
    
    if vip_data and not vip_data.expired then
        return vip_data
    else
        return nil
    end
    return nil
end

function agent_vip.get_vip3()
    -- 存在去检查是否过期，过期了更新冻结的，无冻结的就说明不是会员
    if vip_data then
        if not vip_data.expired and os.time() > vip_data.vip_end_time then
            update_vip_data()
        end
    end
    
    if vip_data and not vip_data.expired and vip_data.vip_level == 3 then
        return (vip_data.vip_end_time - os.time())
    else
        return 0
    end
    return 0
end

function agent_vip.get_vip_level()
    if vip_data then
        if not vip_data.expired and os.time() > vip_data.vip_end_time then
            update_vip_data()
        end
    end

    if vip_data and not vip_data.expired then
        return vip_data.vip_level
    else
        return 0
    end
    return 0
end


--获取冻结会员(比当前会员级别低)的剩余时间
function agent_vip.get_low_level_time(freeze_str)
    if not freeze_str then
        return ''
    end

    --"level|freeze_time|remain#level|freeze_time|remain"
    local low_level = utils.split(freeze_str,'#')
    local total_remain = ''
    for i = 1,#low_level do
        local param = utils.split(low_level[i],'|')
        if #total_remain > 0 then
            total_remain  = total_remain .. '#'
        end
        total_remain = total_remain .. param[1] .. '|'  .. param[3]
    end

    return total_remain
end


-- 外部接口得到vip特权参数
function agent_vip.get_vip_privilege_param(type)
    update_vip_data()
    local cfg = sharedata_utils.get_stat_all(COMMON.plan.Vip)
    if vip_data and not vip_data.expired and cfg then
        local param = cfg[vip_data.vip_level]
        if not param then
            logger.debugf("not found vip param for level[%d]", vip_data.vip_level)
            return 0
        end
        if type == 1 then
            return param["powerSlot"]
        elseif type == 2 then
            return param["powerPoint"]
        elseif type == 3 then
            return param["chestCD"]
        elseif type == 4 then
            return param["chestCD2"]
        elseif type == 5 then
            return param["addHonor"]
        elseif type == 6 then
            return param["shopDiscount"]
        elseif type == 7 then
            return param["dropRate"]
        else
            logger.debugf("not found cfg for type[%d]",type)
            return 0
        end
    else
        return 0
    end
end

function agent_vip.get_vip_added(score)
    local per = agent_vip.get_vip_privilege_param(5)
    if per  ~= 0 then
        return math.floor(score * per / 100)
    end
    return per
end

--[[
--用现金购买vip 充值回调调用
101 30天黄金会员
102 7天钻石
103 30天钻石
 ]]
function agent_vip.buy_vip_with_cach(shop_id)

    local vip_level = 0
    local time_type = 0
    if shop_id == 101 then
        vip_level = 2
        time_type = 30
    elseif shop_id == 102 then
        vip_level = 3
        time_type = 7
    elseif shop_id == 103 then
        vip_level = 3
        time_type = 30
    else
        return
    end

    local data = {
        VipLevel  = vip_level,
        Price = 0,
        Payment = 3,
        TimeType = time_type,
    }

    local content = proto.pack_proto_c2s_req(COMMON.MESSAGE.CS_CMD_BUY_VIP, data)
    handle_buy_vip(COMMON.MESSAGE.CS_CMD_BUY_VIP, content, #content)
end

local function get_need_cost(str, day, currency)
    -- 如果没找到对应的时间#货币类型，可能被改，按最大的来扣款 多扣点哈哈
    local hack_day,hack_cost
    for iday,icurrency,icost in string.gmatch(str, '(%d+)#(%d+)#(%d+)') do
        hack_day = tonumber(iday)
        hack_cost = tonumber(icost)
        if hack_day == day and tonumber(icurrency) == currency then
            return day, hack_cost
        end
    end
    return hack_day, hack_cost
end


local function handle_buy_vip(cmd, content, len)
    local buy_data = proto.parse_proto_req(cmd, content, len)

    local vip_cfg = sharedata_utils.get_stat(COMMON.plan.Vip, buy_data.VipLevel)
    if not vip_cfg then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_DATA_ERROR)
    end

    local day, need_price

    -- gm加的vip特殊处理
    if buy_data.Payment == 4 then
        day = buy_data.TimeType
    else
        day, need_price = get_need_cost(vip_cfg.choice, buy_data.TimeType, buy_data.Payment)
    end

    if buy_data.Payment == 1 then --用荣誉购买
        if need_price > account_info.pvp_score then
            return send_error_code(cmd,  COMMON.ERRORCODE.SCORE_NOT_ENOUGH)
        end
        player.change_score(account_info, -need_price, true, COMMON.change_src.vip)
    elseif buy_data.Payment == 2 then --用钻石购买
        if need_price > account_info.diamond then
            return send_error_code(cmd,  COMMON.ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR)
        end
        player.change_diamond(account_info, COMMON.change_src.vip, -need_price)
    else

    end

    local add_sec = 0
    add_sec = tonumber(day) * 86400

    local tnow = os.time()
    if vip_data == nil or vip_data.expired then -- 之前没有会员或者已经过期
        vip_data = {}
        vip_data.vip_level = buy_data.VipLevel
        vip_data.vip_create_time = tnow
        vip_data.vip_end_time = tnow + add_sec
        vip_data.mail_time = (vip_cfg.giftId ~= 0 and (tnow - 86400) or nil)
        if vip_data.mail_time then
            do_send_mail(tnow) 
        end
    else -- 已经买过会员
        if buy_data.VipLevel > vip_data.vip_level  then
            -- 当前买的会员大于之前买过的(只显示当前级别的会员，之前的冻结起来)
            -- 把之前的会员冻结
            -- 3 > 2 > 1
            -- level|freeze_time|remain_time 会员级别 | 冻结时间 | 剩余时间(sec)
            local remain = vip_data.vip_end_time - tnow
            local freeze = vip_data.vip_level .. '|' .. tnow .. '|' .. remain
            if vip_data.vip_freeze then
                vip_data.vip_freeze = vip_data.vip_freeze .. '#' .. freeze
            else
                vip_data.vip_freeze = freeze
            end
            --保存当前会员
            vip_data.vip_level = buy_data.VipLevel
            vip_data.vip_create_time = tnow
            vip_data.vip_end_time = tnow + add_sec
            if vip_data.mail_time then
                vip_data.mail_time = vip_data.mail_time - 86400
                do_send_mail(tnow)
            end
        elseif  buy_data.VipLevel == vip_data.vip_level then -- 同级别的的时间递增
            vip_data.vip_end_time = vip_data.vip_end_time + add_sec
        else  -- 购买的级别比当前的级别低就递增被冻结的时间
            if vip_data.vip_freeze then
                --找之前的,然后再递增
                local freezes = utils.split(vip_data.vip_freeze,'#')
                local find_index = 0
                local changed = ''
                for i = 1,#freezes do
                    local param = utils.split(freezes[i],'|')
                    if buy_data.VipLevel == tonumber(param[1]) then
                        param[3] = tonumber(param[3]) + add_sec
                        find_index = i
                        changed = changed .. param[1] .. '|' .. param[2] .. '|' .. param[3]
                    end
                end

                local result = ""
                for j = 1,#freezes do
                    if #result > 0 then
                        result = result .. '#'
                    end
                    if j == find_index then
                        --
                        result = result .. changed
                    else
                        result = result .. freezes[j]
                    end
                end
                vip_data.vip_freeze = result
                -- 之前没找到，说明之前还没冻结过
                if find_index == 0 then
                    vip_data.vip_freeze = string.format("%s#%d|%d|%d", vip_data.vip_freeze, buy_data.VipLevel, tnow, add_sec)
                end
            else
                vip_data.vip_freeze = buy_data.VipLevel .. '|' .. os.time() .. '|' .. add_sec
            end
        end
    end

    --每日任务更新
    --update_daily(vip_data.vip_level, true)

    --精力，体力上限变更
    update_slot(vip_data.vip_level)

    --保存
    flush = true
    local timStr = vip_data.vip_level .. '|' .. vip_data.vip_end_time - os.time()
    if vip_data.vip_freeze then
        timStr = timStr .. '#' .. agent_vip.get_low_level_time(vip_data.vip_freeze)
    end

    EventCenter.dispatchEvent(Events.PLAYER_VIP_CHANGE)

    player.change_vip(account_info, vip_data.vip_level)
    local resp_data = {
        VipLevel = vip_data.vip_level,
        RemainTime = timStr,
        Type = 1,
    }
    send_response(cmd, resp_data)
end

local function handle_time_req(cmd, content, len)
    if vip_data and not vip_data.expired then
        local timStr = ''
        if os.time() <= vip_data.vip_end_time then
            timStr = timStr .. vip_data.vip_level .. '|' .. vip_data.vip_end_time - os.time()
        end
        if vip_data.vip_freeze then
            timStr = timStr .. '#' .. agent_vip.get_low_level_time(vip_data.vip_freeze)
        end
        notify_update(vip_data.vip_level, timStr)
    else
        notify_update(0, '')
    end
end

local function daily_check()
    -- 会员每日礼包
    update_vip_data()
    if vip_data and not vip_data.expired then
        -- 这里是发送每日的，update_vip_data()里面的是有vip且过期了补发邮件
        do_send_mail(utils.getSameDayBeginTime())
    end
end

local function register_events()
    EventCenter.addEventListener(Events.DAY_CHANGE, daily_check)
end

local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_BUY_VIP]  = handle_buy_vip
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_VIP_TIME] = handle_time_req
end


function agent_vip.gm_vip( data )
    local content = proto.pack_proto_c2s_req(COMMON.MESSAGE.CS_CMD_BUY_VIP, data)
    handle_buy_vip(COMMON.MESSAGE.CS_CMD_BUY_VIP, content, #content) 
end

-- 加载gm离线vip
local function load_offline()
    local data = skynet.call(redis_service, "lua", "hget_obj", off_table, account_info.uin,true)
    if data then
        for k,v in pairs(data) do
            agent_vip.gm_vip( v )
        end
        skynet.call(redis_service, "lua", "hdel_obj", off_table, account_info.uin,true)
    end
end

function agent_vip.check_save_db()
    if flush and vip_data then
        --保存
        flush = false
        skynet.call(redis_service, "lua", "hset_obj", hash_table, account_info.uin, vip_data, true)
    end
end

function agent_vip.init(args)
    redis_service = args.redis
    account_info = args.info
    agent = args.agent
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code

    add_cmd_handle_mapping(args.cmd_map)

    load_offline()

    register_events()

    --获取会员信息
    load_vip_data()
    update_vip_data()
end


return agent_vip

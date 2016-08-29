--
-- Created by IntelliJ IDEA.
-- User: HuangLijun
-- Date: 2016/2/27
-- Time: 10:54
-- Desc: 玩家基础属性操作处理模块
--

local agent_player = {}

local skynet          = require "skynet"

local sharedata_utils = require "sharedata_utils"
local utils           = require "utils"
local setting         = require "setting"
local logger          = require "logger"
local proto           = require "proto"
local COMMON          = require "common"
local MESSAGE         = COMMON.MESSAGE
local ERRORCODE       = COMMON.ERRORCODE
local agent_pve       = require "agent_pve"
local agent_glyphs    = require "agent_glyphs"
local agent_vip       = require "agent_vip"
local agent_item      = require "agent_item"
local json            = require "cjson"
local agent_skin      = require "agent_skin"

local send_package
local send_response, send_error_code
local account_info, uin, player
local redis_service, agent
local rank_service_v2
local mail_service
local CMD
local busilog_service
local cmd_handle_mapping

local player_titles

local function init_payer_titles()
    player_titles = skynet.call(redis_service, "lua", 
        "hget_obj", "player_titles", account_info.uin) or {}
     --[[ 
    for id, dat in pairs(player_titles) do
        local tcfg = sharedata_utils.get_stat(COMMON.plan.Badge, id)
        if tcfg.extra ~= "0" then
            for ptype, addup in string.gmatch(tcfg.extra, "([.%w]+)#(%w+)") do
                if not title_value_addup[ptype] then
                    title_value_addup[ptype] = 0
                end
                title_value_addup[ptype] = title_value_addup[ptype] + addup
            end
        end
    end
    ]]  
end

local function handle_set_title(cmd, content, len)
    local req_data = proto.parse_proto_req(cmd, content, len)

    if not player_titles[req_data.Title] then
        logger.error("player hasnt this title", req_data.Title)
        return
    end

    account_info.PlayerTitle = req_data.Title
    player.change_flag(account_info)
    local data = {
        Title = req_data.Title
    }
    send_response(cmd, data)
end

--  使用道具 不做任何的验证
local function handle_use_item_msg(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local itemId = decode.ItemId
    local count = decode.Count
    -- 暂时不对物品数量是否正确的判定
    local ret = ERRORCODE.OK
    player.use_item(COMMON.change_src.use_item, itemId, count, account_info)
    local data = {
        Ret = ret,
        ItemId = itemId
    }
    send_response(cmd, data)
    logger.tracef("%d use item suceess ..........", uin)
end

local function handle_change_player_name(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local new_name = decode.Name
    local new_area = decode.Area
    logger.tracef("uin=%d&name=%s&newarea=%d", account_info.uin, new_name, new_area)
    local ret = agent_player.change_player_name(new_name, false, account_info)
    if ret == ERRORCODE.OK then
        account_info.area = new_area
        player.change_flag(account_info)
    end
    return send_response(cmd, { Ret = ret })
end

function agent_player.change_player_name(new_name, is_gm, account_info)

    local regex = {
        "[%.]+","[%']+","[%%]+","[\"]+","[%,]+","[%=]+","[%;]+",
        "[%$]+","[%&]+","[%*]+","[\\]+","[\\+]+"
    }

    local function check(name)
        for _, reg in pairs(regex) do
            if string.find(name, reg) then
                return true
            end
        end
        return false
    end

    local not_ok = check(new_name) or (not is_gm and new_name == "")
    if not_ok then
        return ERRORCODE.NAME_ERROR
    end

    local name_data = skynet.call(redis_service, "lua", "hget_obj", "player_name", new_name)
    local other_uin = name_data and name_data.value or 0

    local uin = account_info.uin
    if other_uin ~= 0 and other_uin ~= uin then
        return ERRORCODE.NAME_EXIST_ERROR
    end

    local old_name = account_info.name
    if not is_gm then
        local price = setting.constant.player_change_name_price
        if account_info.change_name_count < 2 then
            price = 0
        end
        if not player.change_diamond(account_info, COMMON.change_src.rename, -price) then --检测钻石够不够
            return ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR
        end

        account_info.change_name_count = account_info.change_name_count + 1
    end

    if new_name == old_name then
        return ERRORCODE.OK
    end

    account_info.name = new_name

    if new_name ~= '' then
        local player_name = { value = uin }
        skynet.call(redis_service, "lua", "hset_obj", "player_name", new_name, player_name)
    end

    skynet.call(redis_service, "lua", "hdel_obj", "player_name", old_name)

    return ERRORCODE.OK
end

local function handle_player_icon_id(cmd, content, len)
    local ret = ERRORCODE.OK
    local icon_id =  proto.parse_proto_req(cmd, content, len).IconId
    if account_info.icon_id ~= icon_id then
        account_info.icon_id = icon_id
        player.change_flag(account_info)
    end
    local resp = { Ret = ret , IconId = icon_id, }
    return send_response(cmd, resp)
end

local function handle_set_signature(cmd, content, len)
    local signature = proto.parse_proto_req(cmd, content, len).Signature

    account_info.signature = signature
    player.change_flag(account_info)

    local data = { Signature = account_info.signature }
    return send_response(cmd, data)
end

local function handle_set_extrainfo_visible(cmd, content, len)
    local flag = proto.parse_proto_req(cmd, content, len).Visible
    account_info.extrainfo_visible = flag
    player.change_flag(account_info)

    local data = { Visible = account_info.extrainfo_visible }
    return send_response(cmd, data)
end

local function handle_get_extrainfo(cmd, content, len)
    local uin  = proto.parse_proto_req(cmd, content, len).Uin
    local info = {}
    if account_info.uin == uin then
        info = account_info
    else
        info = skynet.call(redis_service, "lua", "hget_obj", "account", uin)
    end

    local data = {
        Uin             = uin,
        TotalPvpCount   = utils.getPvpTotalCount(info),
        TotalPveCount   = info.pve_count,
        TotalPvpUseItem = info.pvp_use_items,
        TotalPveUseItem = info.pve_use_items,
        MaxPvpPoint     = info.pvp_max_point,
        MaxPvpWinCount  = info.max_streak_win,
    }
    send_response(cmd, data)
end

local function handle_query_playerinfo(cmd, content, len)
    local queryId = proto.parse_proto_req(cmd, content, len).Uin
    local player_info
    if queryId == uin then
        player_info = account_info
    else
        player_info = skynet.call(redis_service, "lua", "hget_obj", "account", queryId)
    end
    local rank, group = skynet.call(rank_service_v2, "lua", "get_rank", queryId)
    local data = {
        Uin     = player_info.uin,
        Name    = player_info.name,
        Icon    = player_info.icon_id or 0,
        Title   = player_info.PlayerTitle,
        Level   = player_info.achievement_level,
        Score   = player_info.pvp_point,
        WinRate = utils.getPvpWinRate(player_info),
        Group   = group,
        Rank    = rank,
        ExpLevel = player_info.level or 0,
        Exp     = player_info.exp or 0,
        GlyphsLevel     = agent_glyphs.getGlyphsTotalLevel(player_info.uin),
        MaxCheckPoint   = agent_pve.get_max_checkpoint_id(player_info.uin),
        Signature       = player_info.signature,
        Visible         = player_info.extrainfo_visible,
        Area             = player_info.area  or 0,
        SkinId          = agent_skin.get_skin_id(player_info.uin)
    }

    local vip_data = agent_vip.get_vip_data_by_uin(player_info.uin)
    if vip_data then
        data.VipLevel = vip_data.vip_level
    end
    send_response(cmd, data)
end

--同步获取玩家信息
--如果有需要同步客户端数据已经做一些处理的话，可以解析客户端的content 然后处理
--返回给客户端的data信息可以新增
local function handle_sys_playerinfo(cmd, content, len)
    local item_data = {}
    local items = agent_item.get_all_client_bag_item()
    if items then
        for k,v in pairs(items) do
            table.insert(item_data, {ItemId = v.itemId, Count = v.count})
        end
    end

    local days = 0
    local tnow = os.time()
    if account_info.month_card_time > tnow then
        local add = 0
        days,add = math.modf((account_info.month_card_time - tnow) / 86400)
        if add ~= 0 then
            days = days + 1
        end
    end

    local title_items = {}
    for k, v in pairs(player_titles) do
        local data = {}
        data.Type = k
        data.Time = v.time or math.floor(skynet.time())
        table.insert(title_items, data)
    end

    local data = {
        PlayerTitle = account_info.PlayerTitle,
        Energy = account_info.energy,
        Name = account_info.name,
        ChangeNameCount = account_info.change_name_count,
        EnergyBuyCount = account_info.energy_buy_count,
        Diamond = account_info.diamond,
        IconId = account_info.icon_id,
        EnergySlot = account_info.energy_slot,
        Power = account_info.power,
        PowerSlot = account_info.power_slot,
        Items = item_data,
        MonthCardDays = days,
        ShopGuide = account_info.shop_guide,
        RoomTicket = account_info.room_ticket,
        ExpLevel = account_info.level or 0,
        Exp     = account_info.exp or 0,
        TitleItems = title_items,
        Area = account_info.area or 0,
        ChatFreeCount = account_info.free_chat_count,
        TimeLeft = (math.floor(tnow) > account_info.last_chat_time) and math.floor(tnow) - account_info.last_chat_time or 0,
        HonorCount = account_info.pvp_score,
        Score = account_info.pvp_point,
        NopropCount = account_info.noprop_count,
        NopropBuyCount = account_info.noprop_buy_count,
        Score   = account_info.pvp_point,
        AsstMaterial = agent_item.get_item_count(COMMON.ITEMID.ITEM1),
    }

    local vip_data = agent_vip.get_vip_data()
    if vip_data ~= nil then
        local timeStr = ''
        timeStr = timeStr .. vip_data.vip_level .. '|' .. vip_data.vip_end_time - os.time()
        if vip_data.vip_freeze then
            timeStr = timeStr .. '#' .. agent_vip.get_low_level_time(vip_data.vip_freeze)
        end

        data.VipLevel = vip_data.vip_level
        data.RemainTime = timeStr
    end

    send_response(cmd, data)
    return agent_glyphs.sendInitData()
end


local function handle_open_chest(cmd, content, len)
    local decode = proto.parse_proto_req(cmd, content, len)
    local itemId = decode.ItemId
    local count = decode.Count or 1

    local ret, reward = agent_item.use_item(itemId, count, COMMON.change_src.chest, true)
    local update_data = {}
    if ret then
        if reward then
            for id, item in pairs(reward) do
                local item_cfg = sharedata_utils.get_stat(COMMON.plan.Items, id)
                local data = {
                    UpType = item_cfg.property_type,
                    Value = item.count,
                    Value2 = id,
                }
                --如果物品时虚拟物品 应该就要转换成对应的真实物品
                if item_cfg.is_virtual then
                    data.Value2 = item_cfg.virtual_real
                end
                table.insert(update_data, data)
            end
        end
    end
    local data = { UpdateData = update_data }
    return send_response(cmd, data)
end

local function handle_wechat_share_reward(cmd, content, len)

    local decode = proto.parse_proto_req(cmd, content, len)
    local shareId = decode.ShareId
    local configData = sharedata_utils.get_stat_all(COMMON.plan.WechatShare)

    local shareConf =  configData[shareId]
    if not shareConf then
        logger.warningf("handle_wechat_share_reward 11conf error shareId %d", shareId)
        return
    end

    local list =  utils.split(shareConf["type"], "#")
    if #list ~= 2 then
        logger.warningf("handle_wechat_share_reward conf error shareId %d", shareId)
        return
    end

    local shareType = tonumber(list[1])
    if shareType == 1 or shareType == 2 or shareType == 3 then
        if account_info["share_data"][shareId] == 1 and shareConf["reward"] then
            player.add_gift(account_info, tonumber(shareConf["reward"]), COMMON.change_src.wechat_share)
            account_info["share_data"][shareId] = 0
        else
            return
        end
    elseif shareType == 4 then
        local res = agent_pve.isCanRewardByShare(shareId)
        if res then
            player.add_gift(account_info, tonumber(shareConf["reward"]), COMMON.change_src.wechat_share)
            agent_pve.setShareRewardFlag(shareId)
        else
            return
        end
    end

    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_share, uin, shareId)

    local data = {
        Ret = 1,
        ShareId = shareId,
    }

    send_response(cmd, data)
end

local function add_item(id, count, src, notice_flag, extra)
    agent_item.add_item(id, count, src, notice_flag, extra)
    return true
end

local function add_pvp_point(count, notice_flag, src)
    local totalCount = utils.getPvpTotalCount(account_info)
    if totalCount >= 5 then
        logger.trace("add pvp point", count, src)
        player.change_point(account_info, count, notice_flag, src)
        local data = {
            uin = account_info.uin,
            name = account_info.name,
            win_rate = utils.getPvpWinRate(account_info),
            point = account_info.pvp_point,
            icon = account_info.icon_id,
        }
        skynet.call(redis_service, "lua", "hset_obj", "rank", account_info.uin, data)
        return "success"
    end
    return "player is not in ranklist"
end

local function add_pvp_win_count(count, notice_flag, src)
    local totalCount = utils.getPvpTotalCount(account_info)
    if totalCount >= 5 then
        logger.trace("add pvp win count", count, src)
        account_info.pvp_win_count = account_info.pvp_win_count + count
        player.change_flag(account_info, true)
        local data = {
            uin = account_info.uin,
            name = account_info.name,
            win_rate = utils.getPvpWinRate(account_info),
            point = account_info.pvp_point,
            icon = account_info.icon_id,
            -- 要显示在界面上的值最好存在redis里面 这样就可以不用查询离线玩家的数据了
            -- 玩家是否在线字段可以通过查看在线玩家表得到结果
        }
        skynet.call(redis_service, "lua", "hset_obj", "rank", account_info.uin, data)
        return "success"
    end
    return "player is not in ranklist"
end

local function add_property(type, count, notice_flag, src)
    if type == 1 then
       return  add_pvp_point(count, notice_flag, src)
    elseif type == 2 then
        return add_pvp_win_count(count, notice_flag, src)
    else
        return "add type is error"
    end
end

local function event_item_on_add(event_name, count, item, item_cfg, extra, src, notice_flag)
    if item_cfg.b_type == COMMON.item_btype.PROPERTY then
        if item_cfg.s_type == 1 then
            player.change_diamond(account_info, src, count, notice_flag)
        elseif item_cfg.s_type == 2 then
            player.add_power(account_info, count, notice_flag)
        elseif item_cfg.s_type == 4 then
            player.change_score(account_info, count, notice_flag, src)
        end
    end
end

local function handle_buy_noprop_count(cmd, content, len)
    local needDiamond = sharedata_utils.get_stat(COMMON.plan.powerBuy, account_info.noprop_buy_count + 1).nopropMoney
    needDiamond = (needDiamond == "") and nil or tonumber(needDiamond)
    if not needDiamond or needDiamond <= 0 then -- 确认一下 是否有上限次数
        return send_error_code(cmd, COMMON.ERRORCODE.NOT_BUY_NOPROP_COUNT)
    end

    if account_info.diamond < needDiamond then
        return send_error_code(cmd, COMMON.ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR)
    end

    player.change_diamond(account_info, COMMON.change_src.buy_noprop_count, -needDiamond)
    account_info.noprop_count = account_info.noprop_count + 1
    account_info.noprop_buy_count = account_info.noprop_buy_count + 1

    local data = {
        NopropCount         = account_info.noprop_count,
        NopropBuyCount      = account_info.noprop_buy_count
    }
    return send_response(cmd, data)
end

local function handle_msg_transaction(cmd, content, len)
    local req_data = proto.parse_proto_req(cmd, content, len)
    for i, v in ipairs(req_data.MsgPack) do
        if cmd_handle_mapping[v.MsgId] then
            local dat = json.decode(v.MsgContent)
            local dc = proto.pack_proto_c2s_req(v.MsgId, dat)
            cmd_handle_mapping[v.MsgId](v.MsgId, dc, string.len(dc))
        else
            logger.error("msg_transaction msg id is not in mapping", v.MsgId)
        end
    end
    send_response(cmd, {})
end


function agent_player.get_title_property_addup(ptype)
    local up = 0
    local level = 0
    for id, dat in pairs(player_titles) do
        id = tonumber(id)
        if id then
            local tcfg = sharedata_utils.get_stat(COMMON.plan.Badge, id)
            if tcfg and tcfg.extra ~= "0" then
                local cfg_level = tonumber(tcfg.level)
                if cfg_level > level then
                    local pt, cp = string.match(tcfg.extra, "([.%d]+)#([.%d]+)")
                    pt = tonumber(pt)
                    cp = tonumber(cp)
                    if pt == ptype then
                        level = cfg_level
                        up = cp
                    end
                end
            end
        end
    end
    return up
end

function agent_player.add_title(title)
    title = tonumber(title)
    assert(title)
    if not player_titles[title] then
         local data = {}
        data.time = math.floor(skynet.time())
        player_titles[title] = data
        
        print("add_title : ", COMMON.plan.Badge, title)
        local tcfg = sharedata_utils.get_stat(COMMON.plan.Badge, title)

        local title_data = {
            type = title,
            time   = data.time
        }
        local stat = json.encode(title_data)
        player.notice_player({{COMMON.property_type.TITLE, stat}})
        skynet.call(redis_service, "lua", "hset_obj", "player_titles",
            account_info.uin, player_titles)
        skynet.send(busilog_service, "lua", "log_misc", 
            COMMON.busilog_misc_type.misc_title, 
            account_info.uin, title, COMMON.misc_action.title_get)
            
        if account_info.PlayerTitle == 0 then
            account_info.PlayerTitle = title
        else
            local title_type = COMMON.title_type
            if (title == title_type.WIN_GENERAL_2
                or title == title_type.WIN_GENERAL_3) 
                and (account_info.PlayerTitle == title_type.WIN_GENERAL 
                or account_info.PlayerTitle == title_type.WIN_GENERAL_2)
                then
                account_info.PlayerTitle = title
            end
        end
        player.change_flag(account_info)
        send_response(COMMON.MESSAGE.CS_CMD_SET_TITLE, { Title = account_info.PlayerTitle })
        skynet.send(busilog_service, "lua", "log_misc", 
            COMMON.busilog_misc_type.misc_title, 
            account_info.uin, title, COMMON.misc_action.title_set)
            
        return true
    end
    return false
end

local function handle_bind_openid(cmd, msg, sz)
    local req = proto.parse_proto_req(cmd, msg, sz)
    local openid = req.Openid

    assert(openid ~= "", account_info.uin)
    account_info.openid = openid

    local platform = COMMON.platform[account_info.platform]
    local db_openid = platform .. "_" .. openid
    skynet.call(redis_service, "lua", "hset_obj", "uin_mapping",
        db_openid, account_info.uin)

    send_response(cmd, {})
end

local function check_month_card_reward()
    local now = os.time()
    if account_info.month_card_time > now then
        local ret = utils.is_same_day(account_info.month_card_reward, now)
        if not ret then
            account_info.month_card_reward = now
            player.change_flag(account_info)
            local mail = utils.clone(sharedata_utils.get_stat(
                COMMON.plan.Sys_Mail, COMMON.sys_mail_id.MONTH_CARD), true)
            mail.sendtime = now
            mail.outdate = now + mail.outdate
            skynet.call(mail_service, "lua", "add_mail", account_info.uin,
                COMMON.mail_type.SINGLE, mail, COMMON.change_src.month_card_mail)
        end
    end
end

agent_player.check_month_card_reward = check_month_card_reward

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(cmd_handle_mapping_)

    cmd_handle_mapping = cmd_handle_mapping_
    cmd_handle_mapping[MESSAGE.CS_CMD_SET_TITLE]            = handle_set_title
    cmd_handle_mapping[MESSAGE.CS_CMD_USE_ITEM]             = handle_use_item_msg
    cmd_handle_mapping[MESSAGE.CS_CMD_CHANGE_NAME]          = handle_change_player_name
    cmd_handle_mapping[MESSAGE.CS_CMD_CHANGE_ICON_ID]       = handle_player_icon_id
    cmd_handle_mapping[MESSAGE.CS_CMD_SET_SIGNATURE]        = handle_set_signature
    cmd_handle_mapping[MESSAGE.CS_CMD_SET_EXTRAINFO_VISIBLE] = handle_set_extrainfo_visible
    cmd_handle_mapping[MESSAGE.CS_CMD_GET_EXTRAINFO]        = handle_get_extrainfo
    cmd_handle_mapping[MESSAGE.CS_CMD_QUERY_PLAYERINFO]     = handle_query_playerinfo
    cmd_handle_mapping[MESSAGE.CS_CMD_SYC_PLAYERINFO]       = handle_sys_playerinfo
    cmd_handle_mapping[MESSAGE.CS_CMD_OPEN_CHEST]           = handle_open_chest
    cmd_handle_mapping[MESSAGE.CS_CMD_BUY_NOPROP_COUNT]     = handle_buy_noprop_count
    cmd_handle_mapping[MESSAGE.CS_CMD_SHARE_REWARD_GET]     = handle_wechat_share_reward
    cmd_handle_mapping[MESSAGE.CS_CMD_MSG_TRANSACTION]      = handle_msg_transaction
    cmd_handle_mapping[MESSAGE.CS_CMD_BIND_OPENID]          = handle_bind_openid
end

local function add_cmd(CMD)
    CMD.add_title       = agent_player.add_title
    CMD.add_property    = add_property
    CMD.add_item        = add_item
    CMD.get_titles      = function() return player_titles end
end

function agent_player.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code
    send_package    = args.send_package

    account_info    = args.info
    uin              = args.uin
    player           = args.player
    redis_service   = args.redis
    CMD              = args.cmd
    busilog_service = args.busilog_service

    init_payer_titles()
    add_cmd(CMD)
    add_cmd_handle_mapping(args.cmd_map)
    
    EventCenter.addEventListener(Events.ITEM_ON_ADD, event_item_on_add)
    rank_service_v2 = skynet.queryservice("rank_service_v2")
    mail_service = skynet.queryservice("mail_service")

end

function agent_player.daily_login_check(notice_flag)
    account_info.noprop_count       = 10
    account_info.noprop_buy_count   = 0
    --检测当天玩家月卡是否领取
    skynet.timeout(200, check_month_card_reward)
end

return agent_player

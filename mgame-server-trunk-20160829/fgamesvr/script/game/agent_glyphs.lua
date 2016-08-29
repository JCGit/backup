--  agent_glyphs.lua
--  雕文系统
local agent_glyphs = {}

local skynet          = require "skynet"
local logger          = require "logger"

local proto           = require "proto"
local sharedata_utils = require "sharedata_utils"
local utils           = require "utils"
local COMMON          = require "common"
local agent_vip       = require "agent_vip"
local agent_item      = require "agent_item"

local agent, account_info, uin, player, redis
local send_response, send_error_code
local busilog_service

local checkGlyphsStatus
--[[

NotOpen   = 1; // 未开放
CanActive = 2; // 可激活
Active    = 3; // 已激活
LevelCan  = 4; // 等级未达到可激活等级
VipCan    = 5; // VIP未达到可激活等级
VipNot    = 6; // VIP已过期，VIP锁定

id, id

ID为5位数，10101
万位：符文大类，1.攻击，2.防御，3.速度，4.兵力，5.士气
千百位：小类，如01小兵攻击
十个位：等级

name, 名称,
desc, 描述,
icon, 图标,
type, 性质类型: 1.攻击 2.防御 3.速度 4.兵力 5.士气
class, 阶级类型: 1.初阶 2.中阶 3.高阶 4.贵族
openLevel, 开放等级； 现在已废弃
activateType, 激活类型:
0：当前版本未开启 1：角色等级 2：段位 3：钻石 4：vip等级 5：未定

effectType, 生效类型:   0：both都有效 1：pve 2：pvp

soul, 火候,
level, 等级: 符文当前等级
mainAttr, 主属性,
sideAttr, 副属性,
buildLevel, 建筑等级影响值: 填写规格 10+10+10+10+10 共5个数值，对应建筑5个等级

stuff, 升级材料: 1-98: 材料编号， 99通关证明卡 100钻石
vipLevel vip等级限制 需要的VIP等级: 0.无限制 1.白银 2.黄金 3.钻石
needLevel: 需求等级 (后来添加的)

-]]

-- 保存玩家的雕文数据
local function saveGlyphData(glyphData, query_uin)
    query_uin = query_uin or uin
    skynet.call(redis, 'lua', 'hset_obj', 'glyphData', query_uin, glyphData)
end

-- 获取玩家的雕文数据
local function loadGlyphData(query_uin)
    query_uin = query_uin or uin
    local glyphData = skynet.call(redis, 'lua', 'hget_obj', 'glyphData', query_uin)
    if glyphData == nil then
        glyphData = {}
        saveGlyphData(glyphData, query_uin)
    end
    return glyphData
end

-- 保存玩家的材料数据
local function saveStuffData(stuffData)
    skynet.call(redis, 'lua', 'hset_obj', 'stuffData', uin, stuffData)
end

-- 加载玩家的材料数据
local function loadStuffData()
    local stuffData = skynet.call(redis, 'lua', 'hget_obj', 'stuffData', uin)
    if stuffData == nil then
        stuffData = {}
        logger.trace('get stuff data nil, set init value')
        saveStuffData(stuffData)
    end
    return stuffData
end


-- 遍历配置表，检查全部的符文状态
-- 根据玩家当前的等级，检查全部的符文 可激活状态
--
-- vip激活类型的符文，等级1的，无需材料即可激活
-- 每次检查的时候，只要是vip，就自动把等级1的设置为激活状态
local function updatePlayerGlyphInfo(playerGlyphData)
    local player_level = account_info.level or 1

    local configs = sharedata_utils.get_stat_all(COMMON.plan.Glyphs)
    for glyph_db_key, config in pairs(configs) do
        -- e.g. 获取10201 中102
        local glyph_id = math.floor(glyph_db_key / 100)

        local glyphData = playerGlyphData[glyph_id]
        if glyphData == nil then
            glyphData = { level = 1 } -- 等级改成1，就算是未激活状态
            playerGlyphData[glyph_id] = glyphData
        end

        if glyphData.level == config.level then
            -- 检查是否已激活
            if glyphData.status == 'Active' then
                -- 激活的情况下，检查是否为vip类型，是的话就检查是否已过期
                if config.activateType == 4 then
                    local vip_data = agent_vip.get_vip_data_by_uin(uin)
                    if not (vip_data and vip_data.vip_level >= config.vipLevel) then
                        glyphData.status = 'VipNot' -- vip过期，锁定
                    end
                end
            else
                -- 未激活，检查具体的状态
                if config.activateType == 0 then
                    glyphData.status = 'NotOpen' -- 未开放
                elseif config.activateType == 1 then
                    if player_level >= config.needLevel then
                        glyphData.status = 'CanActive' -- 可以激活
                    else
                        glyphData.status = 'LevelCan' -- 等级未达到，不可激活
                    end
                elseif config.activateType == 4 then

                    local vip_data = agent_vip.get_vip_data_by_uin(uin)
                    if vip_data and vip_data.vip_level >= config.vipLevel then
                        glyphData.status = 'Active'
                    else
                        glyphData.status = 'VipCan'
                    end
                else
                    logger.trace('雕文系统配置数据错误！不支持的有效激活类型！')
                    glyphData.status = 'NotOpen' -- 未开放
                end
            end
        end
    end
    saveGlyphData(playerGlyphData)
    return playerGlyphData
end

-- 请求雕文数据
local function handle_request_info(cmdid)

    cmdid = cmdid or COMMON.MESSAGE.CS_CMD_GLYPHS_REQUEST_INFO

    local playerGlyphData = loadGlyphData()
    updatePlayerGlyphInfo(playerGlyphData)

    local table_insert = table.insert
    local data = {}
    for k, v in pairs(playerGlyphData) do
        table_insert(data, { glyphID = k, level = v.level, status = v.status })
    end

    return send_response(cmdid, {glyphData = data})
end

-- 发送材料数据
-- @param [playerStuffData] 可选，玩家的材料数据，为nil时会从redis查找
local function sendStuffInfo(playerStuffData)
    local table_insert = table.insert
    local data = {}
    for k, v in pairs(playerStuffData or loadStuffData()) do
        table_insert(data, { stuffID = k, count = v.count })
    end
    local cmdid = COMMON.MESSAGE.CS_CMD_GLYPHS_REQUEST_BASIC_INFO
    return send_response(cmdid, {stuffInfo = data})
end


-- 请求材料数据
local function handle_request_basic_info(cmdid, content, len)
    return sendStuffInfo()
end

local function checkStuffNeed(playerStuffData, config)
    for needStuffID, needCount in pairs(config.stuffInfo) do
        local stuff = playerStuffData[needStuffID]
        if not (stuff and stuff.count >= needCount) then
            return false
        end
    end
    return true
end

-- 激活雕文
local function handle_active(cmdid, content, len)
    local glyphID = proto.parse_proto_req(cmdid, content, len).glyphID

    local function sendResult(status)
        return send_response(cmdid, { glyphID = glyphID, status = status })
    end

    -- 根据请求的ID获取1级雕文的配置ID
    local db_id = tonumber(glyphID .. '01') -- 配置中的ID编号规则

    local config = sharedata_utils.get_stat(COMMON.plan.Glyphs, db_id)
    if not config then
        return sendResult('FAILED_INVALID_ID')
    end

    -- 获取全部的雕文数据
    local playerGlyphData = loadGlyphData()
    local glyph = playerGlyphData[glyphID]

    if glyph == nil then
        logger.trace('glyph is nil, this should never happen! client error')
        return sendResult 'FAILED_ERROR'
    end

    -- 检查激活条件, 目前支持两种：
    if glyph.status == 'NotOpen' then
        return sendResult 'FAILED_CANNOT_ACTIVE'
    elseif glyph.status == 'CanActive' then
        -- 1. VIP等级达到要求后，消耗指定材料激活
        -- vip_level 取值范围: 1-3
        -- 2. 消耗指定材料然后激活

        if config.activateType == 4 then
            local vip_data = agent_vip.get_vip_data_by_uin(uin)
            if not (vip_data and vip_data.vip_level >= config.vipLevel) then
                return sendResult('FAILED_VIP')
            end
        elseif config.activateType == 1 then
        else
            return sendResult('FAILED_ERROR')
        end

        local playerStuffData = loadStuffData()
        -- 检查全部所需材料是否足够
        if checkStuffNeed(playerStuffData, config) then
            -- 扣除材料
            for needStuffID, needCount in pairs(config.stuffInfo) do
                local stuff = playerStuffData[needStuffID]
                assert(stuff and stuff.count >= needCount, 'inconsequence data')
                stuff.count = stuff.count - needCount
                agent_item.item_on_reduce(needStuffID, needCount, COMMON.change_src.glyph, false)
                --skynet.send(busilog_service, "lua", "log_item", account_info.uin, needStuffID, -needCount, COMMON.change_src.glyph)
            end
            -- 保存更新后的材料数据
            saveStuffData(playerStuffData)
            -- 激活雕文
            glyph.status = 'Active'
            glyph.level = 1
            saveGlyphData(playerGlyphData)
            sendResult('OK')

            local a, b = checkGlyphsStatus()
            local cmdid = COMMON.MESSAGE.CS_CMD_GLYPHS_UPDATE
            send_response(cmdid, {HaveUpdate = a, TypeList = b})

            -- 写入oss
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_cooker, account_info.uin, db_id)
        else
            return sendResult('FAILED_NO_STUFF')
        end

    elseif glyph.status == 'LevelCan' then
        return sendResult('FAILED_ERROR') -- 等级未达到，正常情况下客户端也不会发此请求
    elseif glyph.status == 'VipCan' then
        return sendResult('FAILED_ERROR') -- vip未达到，正常情况下客户端也不会发此请求
    elseif glyph.status == 'Active' then
        assert(glyph.level >= 1)
        return sendResult('FAILED_ACTIVE')
    else
        logger.trace('error glyph status')
        return sendResult('FAILED_ERROR')
    end
end

-- 升级雕文
local function handle_upgrade(cmdid, content, len)
    local glyphID = proto.parse_proto_req(cmdid, content, len).glyphID
    logger.trace('handle_upgrade: ', glyphID)

    local function sendResult(ok, level)
        local result = { glyphID = glyphID, level = level or 1, is_ok = ok }
        return send_response(cmdid, result)
    end

    -- 获取全部的雕文数据
    local playerGlyphData = loadGlyphData()
    local glyph = playerGlyphData[glyphID]
    if glyph == nil then
        logger.trace('do not have this glyph')
        return sendResult(false)
    end
    if glyph.status ~= 'Active' then
        logger.trace('glyph status is not active')
        return sendResult(false)
    end

    -- 获取当前符文下一级配置
    local configs = sharedata_utils.get_stat_all(COMMON.plan.Glyphs)
    local db_key = tonumber(glyphID .. string.format('%02d', glyph.level + 1))
    local config = configs[db_key]
    if config == nil then
        logger.trace('non exist glyph id: ', db_key)
        return sendResult(false)
    end

    local player_level = account_info.level or 1

    -- 检查是否达到了升级等级要求
    if (config.needLevel or 1) > player_level then
        logger.trace('厨艺升级：未达到等级要求')
        return sendResult(false)
    end

    local playerStuffData = loadStuffData()
    -- 检查全部所需材料是否足够
    if checkStuffNeed(playerStuffData, config) then
        -- 扣除材料
        for needStuffID, needCount in pairs(config.stuffInfo) do
            local stuff = playerStuffData[needStuffID]
            assert(stuff and stuff.count >= needCount, 'inconsequence data')
            stuff.count = stuff.count - needCount
            agent_item.item_on_reduce(needStuffID, needCount, COMMON.change_src.glyph, false)
            --skynet.send(busilog_service, "lua", "log_item", account_info.uin, needStuffID, -needCount, COMMON.change_src.glyph)
        end
        -- 保存更新后的材料数据
        saveStuffData(playerStuffData)
        -- 升级雕文
        glyph.level = glyph.level + 1

        EventCenter.dispatchEvent(Events.GLYPH_UPGRADE, glyph.level)

        saveGlyphData(playerGlyphData)
        sendResult(true, glyph.level)

        local a, b = checkGlyphsStatus()
        local cmdid = COMMON.MESSAGE.CS_CMD_GLYPHS_UPDATE
        send_response(cmdid, {HaveUpdate = a, TypeList = b})

        -- 写入oss
        skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_cooker, account_info.uin, db_key)
    else
        return sendResult(false)
    end
end

-- 检查符文状态(可升级 可激活)有无变化 (玩家的等级 材料 VIP变化之后)
function checkGlyphsStatus()

    local a, b = false, {}

    local configs = sharedata_utils.get_stat_all(COMMON.plan.Glyphs)

    local player_level = account_info.level or 1
    local vip_data = agent_vip.get_vip_data_by_uin(uin)

    local glyphData = loadGlyphData()
    local stuffData = loadStuffData()

    for glyph_id, glyph in pairs(glyphData) do

        assert(glyph.level <= 99, 'error glyph level')

        local db_key = math.floor(glyph_id * 100 + glyph.level)
        local config = assert(configs[db_key])

        if glyph.status == 'NotOpen' then
            -- 配置文件变化，开放新符文了（不太可能）
            if config.activateType == 1 or config.activateType == 4 then
                logger.trace('开放新符文', glyph_id)
                a = true
                table.insert(b, config.type)
            end
        elseif glyph.status == 'LevelCan' then
            -- 等级达到了开放等级
            if player_level >= config.needLevel then
                logger.trace('等级达到了开放等级', glyph_id)
                a = true
                table.insert(b, config.type)
            end
        elseif glyph.status == 'VipCan' then
            if vip_data and vip_data.vip_level >= config.vipLevel then
                logger.trace('vip can', glyph_id)
                a = true
                table.insert(b, config.type)
            end
        elseif glyph.status == 'VipNot' then
            if vip_data and vip_data.vip_level >= config.vipLevel then
                logger.trace('vip 解锁', glyph_id)
                a = true
                table.insert(b, config.type)
            end
        elseif glyph.status == 'Active' then
            -- 检查材料是否足够 (材料)
            local next_db_key = math.floor(glyph_id * 100 + glyph.level + 1)
            local next_config = configs[next_db_key]
            if next_config
                and (next_config.needLevel or 1) <= player_level
                and checkStuffNeed(stuffData, next_config) then

                logger.trace('达到升级厨艺的等级并且 材料够升级了', glyph_id)
                a = true
                table.insert(b, config.type)
            end
        elseif glyph.status == 'CanActive' then
            -- 检查材料是否足够 (材料)
            if checkStuffNeed(stuffData, config) then
                logger.trace('材料可以激活了', glyph_id)
                a = true
                table.insert(b, config.type)
            end
        end
    end

    return a, b
end

-- 与符文相关的新手引导
local function handle_glyph_guide(cmdid, content, len)
    local ret = COMMON.ERRORCODE.OK
    -- 加一次雕文材料
    if account_info.stuff_guide == 0 then
        account_info.stuff_guide = 1

        local gift_config = sharedata_utils.get_stat(COMMON.plan.Gift, 999)

        -- 增加容错处理
        if gift_config == nil then
            gift_config = { item1 = '100001+10' }
        end

        local stuff_id, count = gift_config.item1:match('(%d+)%+(%d+)')
        player.add_glyph_stuff(uin, stuff_id, count, COMMON.change_src.newer_glyph)
    end
    return send_response(cmdid, { Ret = ret, GiftID = 999 })
end

local function handle_glyph_notify(cmdid)
    local a, b = checkGlyphsStatus()
    return send_response(cmdid, { HaveUpdate = a, TypeList = b, })
end

local function add_cmd_handle_mapping(mapping)
    local msg = COMMON.MESSAGE
    mapping[msg.CS_CMD_GLYPHS_REQUEST_INFO      ] = handle_request_info
    mapping[msg.CS_CMD_GLYPHS_REQUEST_BASIC_INFO] = handle_request_basic_info
    mapping[msg.CS_CMD_GLYPHS_ACTIVE            ] = handle_active
    mapping[msg.CS_CMD_GLYPHS_UPGRADE           ] = handle_upgrade
    mapping[msg.CS_CMD_GLYPH_GUIDE_ADDSTUFF     ] = handle_glyph_guide
    mapping[msg.CS_CMD_GLYPHS_UPDATE            ] = handle_glyph_notify
end

local function event_item_on_add(event_name, count, item, item_cfg, extra, src, notice_flag)
    if item_cfg.b_type == COMMON.item_btype.GLYPH then
        player.add_glyph_stuff(account_info.uin, item_cfg.id, count, src)
    end
end

local function add_cmd(cmd)
    cmd.get_glyphs_info     = function()loadGlyphData() end  -- todo: 待优化 将符文信息缓存起来
end

function agent_glyphs.init(args)
    agent        = args.agent
    account_info = args.info
    uin          = args.uin
    player       = args.player
    redis        = args.redis

    send_response = args.send_response
    send_error_code = args.send_error_code
    busilog_service = args.busilog_service

    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)

    local function check(eventName)
        local a, b = checkGlyphsStatus()
        if a then
            local cmdid = COMMON.MESSAGE.CS_CMD_GLYPHS_UPDATE
            send_response(cmdid, {HaveUpdate = true, TypeList = b})
        end

        -- 符文材料改变后发送材料数据给客户端
        if eventName == Events.GLYPH_STUFF_CHANGE then
            sendStuffInfo()
        end
    end
    -- 完善一下确保只添加一次的处理
    -- 符文可以升级或激活时给客户端红点提醒
    --  等级变化
    --  vip变化
    --  材料变化
    EventCenter.addEventListener(Events.GLYPH_STUFF_CHANGE, check)
    EventCenter.addEventListener(Events.PLAYER_VIP_CHANGE, check)
    EventCenter.addEventListener(Events.PLAYER_LEVEL_CHANGE, check)
    EventCenter.addEventListener(Events.ITEM_ON_ADD, event_item_on_add)
end

-- 提供给agent调用的，必须在init之后调用
function agent_glyphs.sendInitData()
    handle_request_info()
    return sendStuffInfo()
end

-- 获取符文的总等级
function agent_glyphs.getGlyphsTotalLevel(query_uin)
    local gd = loadGlyphData(query_uin)
    local r = 0
    for k, v in pairs(gd) do
        if v.status == 'Active' then
            r = r + v.level
        end
    end
    return r
end

return agent_glyphs

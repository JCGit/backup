--  agent_assistant.lua
--  精灵系统

local agent_assistant = {}

local skynet    = require "skynet"

local sharedata_utils = require "sharedata_utils"
local agent_pve       = require 'agent_pve'

local logger    = require "logger"
local proto     = require "proto"
local utils     = require "utils"
local COMMON    = require "common"
local agent_item = require 'agent_item'
local MESSAGE   = COMMON.MESSAGE
local ERRORCODE = COMMON.ERRORCODE

local send_response, send_error_code
local account_info, uin, player
local redis_call
local busilog_service

--[[ 精灵表配置格式
{
    [101001] = {
        TypeID        = 101001,
        Quality       = 1,
        ActiveSkills  = {200004},

        PassiveSkills = {},
        ExpTime       = 0, -- 单位: 天
        Level         = 1, -- 支持直接产出任意等级的精灵
    },
}
--]]
local ASSISTANT_CONFIG

-- {
-- as        = { AssistantInfo },
-- in_battle = as_type_id,
-- }
local ASSISTANT_INFO = 'assistant_info'

local as_cache
local flush = false

local function loadAssistantData(query_uin)
    query_uin = query_uin or uin
    as_cache = redis_call('hget_obj', ASSISTANT_INFO, query_uin)
    if as_cache == nil then
        as_cache = {
            as        = {},   -- 精灵列表 key是精灵类型编号
            --in_battle = false, 此字段现在废弃 -- 上阵精灵类型编号，false为当前没有上阵的精灵
            slot = {}, --槽位信息
        }
    end

    -- 道具购买碎片限额初始化
    if not as_cache.piece_limit then
        as_cache.piece_limit = {}
    end
end

local function checkExp()
    local data = as_cache
    if data ~= nil then
        local now = os.time()
        for k, v in pairs(data.as) do
            local exp = v.ExpTime
            if exp ~= 0 and exp < now then
                v.Level = 0
                v.IsInBattle = false
                if data.in_battle == k then
                    data.in_battle = false
                end
                flush = true
                agent_item.item_on_reduce(v.AssistantId, 1, COMMON.change_src.assistant, false)
            end
        end
    end
end

local function add_exp_time(assistant, exptime)
    if exptime < 0 then
        return
    end
    if assistant.ExpTime ~= 0 then
        if exptime == 0 then
            assistant.ExpTime = 0
        elseif exptime > 0 then
            local d = 24 * 60 * 60 * exptime
            local now_ts = math.floor(skynet.time())

            if assistant.ExpTime <= now_ts then
                assistant.ExpTime = now_ts + d --已过期，重新激活
            else
                assistant.ExpTime = assistant.ExpTime + d -- 增加有效期
            end
        end
        -- 通知客户端更新已有精灵
        send_response(MESSAGE.CS_CMD_AS_SYNC_INFO, { Assistant = {assistant} })
    end
end

-- 增加精灵
-- 暂定所有数据从表中读取（后续根据策划需求做修改调整）
local function add_assistant(uin, assistant_id, exp)
    local config = ASSISTANT_CONFIG[assistant_id]
    if not config then
        logger.debugf("not found assistant[%d]", assistant_id)
        return false
    end

    local as = as_cache.as

    local assistant_type = config.TypeID

    local assistant = as[assistant_type]
    if assistant then
        if assistant.Level > 0 then
            add_exp_time(assistant, exp)
        else
            local d = 24 * 60 * 60 * exp
            local now_ts = math.floor(skynet.time())
            assistant.ExpTime = exp == 0 and 0 or now_ts + d
            assistant.Level = 1
            assitant.SlotIndex = 0
            -- 通知客户端新增精灵
            send_response(MESSAGE.CS_CMD_AS_NEW_ADD, {
                NewAddAssistant = {assistant}
            })
        end
        flush = true
        return true
    end
    return false
end


-- 同步精灵数据
local function handle_as_sync_info(cmdid, content, len)
    -- if as_cache and as_cache.in_battle then
    --     assert(as_cache.as[as_cache.in_battle], '将军已经装备，但不存在此将军')
    --     as_cache.as[as_cache.in_battle].IsInBattle = true
    -- end

    local al = {}
    for k, v in pairs(as_cache.as) do
        local piece_num 
        piece_num = agent_item.get_item_count(ASSISTANT_CONFIG[v.AssistantId].PieceId)
        if piece_num ~= v.PieceNum then
            v.PieceNum = piece_num
            flush = true
        end
        --引导将军不显示
        if ASSISTANT_CONFIG[v.AssistantId].Show ~= 0 then
            table.insert(al, v)
        end
    end
    return send_response(cmdid, { Assistant = al })
end

-- 装备/卸下精灵; 进阶 升级
local function handle_as_operate(cmdid, content, len)
    local req = proto.parse_proto_req(cmdid, content, len)
    local aid, slotIndex = req.AssistantId, req.SlotIndex

    local res = {
        AssistantId = aid,
        Res         = false,
        SlotIndex = slotIndex,
    }

    local assistant = as_cache.as[aid]
    if not assistant then
        return send_response(cmdid, res)
    end

    local now = os.time()
    if (assistant.ExpTime ~= 0) and (now > assistant.ExpTime) then
        return send_error_code(cmdid, ERRORCODE.AS_TIME_EXPIRED)
    end

    -- reset before and set cur if is 上阵
    -- if as_cache.in_battle and (op == 1) then
    --     as_cache.as[as_cache.in_battle].IsInBattle = false
    -- end

    if not as_cache.slot then
        as_cache.slot = {}
    end

    if slotIndex > 0 then
        if as_cache.slot[slotIndex] then
            as_cache.as[as_cache.slot[slotIndex]].IsInBattle = false
        end     
        as_cache.as[aid].IsInBattle = true
        as_cache.as[aid].SlotIndex = slotIndex
        as_cache.slot[slotIndex] = aid
    else
        as_cache.as[aid].IsInBattle = false 
        as_cache.as[aid].SlotIndex = slotIndex
    end

    -- as_cache.in_battle = (op == 1) and aid or false
    -- assistant.IsInBattle = (op == 1) and true or false
    flush = true
    res.Res = true
    return send_response(cmdid, res)
end

-- 将军升级
local function handle_as_upgrad(cmd, msg, sz)
    local req = proto.parse_proto_req(cmd, msg, sz)

    local assistant = as_cache.as[req.AssistantId]
    if not assistant then
        return send_error_code(cmd, ERRORCODE.AS_NOT_EXIST)
    end
    
    -- 当前等级
    local level = req.Level
    if level ~= assistant.Level then
        return send_error_code(cmd, ERRORCODE.AS_LEVEL_MIS)
    end

    -- 没获得将军的时候默认是0，以前老数据默认是1
    local level_str = string.format("%d%02d", req.AssistantId, (level == 0 and 1 or level))
    local to_str   = string.format("%d%02d", req.AssistantId, level+1)
    local skill 
    
    local cfg = sharedata_utils.get_stat(COMMON.plan.AS_LEVEL, tonumber(level_str))
    if not cfg then
        return send_error_code(cmd, ERRORCODE.AS_LEVEL_NO)
    end
    skill = cfg.SkillId

    local peice_id = ASSISTANT_CONFIG[req.AssistantId].PieceId

    cfg = sharedata_utils.get_stat(COMMON.plan.AS_LEVEL, tonumber(to_str))
    if cfg then
        level = level + 1
        skill = cfg.SkillId
    else
        return send_error_code(cmd, ERRORCODE.AS_LEVEL_MAX)
    end

    -- 需要扣掉的消耗是升级到的级别的
    local need_piece = cfg.Piece
    local need_honor = cfg.Honor
    local need_item = cfg.Material

    if need_piece > agent_item.get_item_count(peice_id) then
        return send_error_code(cmd, ERRORCODE.AS_PIECE_LACK)
    end

    if need_honor > account_info.pvp_score then
        return send_error_code(cmd, ERRORCODE.SCORE_NOT_ENOUGH)
    end

    if need_item > 0 and need_item > agent_item.get_item_count(COMMON.ITEMID.ITEM1) then
        return send_error_code(cmd, ERRORCODE.AS_ITEM_LACK)
    end

    local item
    if need_piece > 0 then
        _, item,_ =  agent_item.reduce_item(peice_id, need_piece, COMMON.change_src.assistant, true)
    end
    _ = need_honor > 0 and player.change_score(account_info, -need_honor, true, COMMON.change_src.assistant)
    _ = need_item > 0 and agent_item.reduce_item(COMMON.ITEMID.ITEM1, need_item, COMMON.change_src.assistant, true)

    local skills = {skill}
    assistant.ActiveSkills = skills
    assistant.Level = level
    flush = true
    assistant.PieceNum = item and item.count or 0
    skynet.send(busilog_service, "lua", "log_misc",
        COMMON.busilog_misc_type.misc_assistant_add,
        account_info.uin, req.AssistantId, assistant.Level)

    local rsp = {
        AssistantId = req.AssistantId,
        PieceNum     = assistant.PieceNum,
        Level       = level,
        ActiveSkills = skills,
        PassiveSkills = assistant.PassiveSkills,
    }
    return send_response(cmd, rsp)
end

local function add_cmd_handle_mapping(cmd_map)
    local M = MESSAGE
    cmd_map[M.CS_CMD_AS_SYNC_INFO] = handle_as_sync_info
    cmd_map[M.CS_CMD_AS_OPERATE  ] = handle_as_operate
    cmd_map[M.CS_CMD_AS_UPGRAD]    = handle_as_upgrad
end


local function event_item_on_add(eventName, count, item, cfg, extra, src, notice_flag, extra_add_items)
    extra = tonumber(extra)
    extra = extra and extra or 0
    if type(cfg) == 'table' and cfg.b_type == COMMON.item_btype.ASSISTANT then

        aid, exp = tonumber(cfg.id), extra
        exp = count * exp

        if type(ASSISTANT_CONFIG[aid]) == 'table' and exp >= 0 then

            if item.count == 0 then
                --reward[aid] = {item_id = aid, count=1}
                -- NOTE: 如果确实有删除精灵的需求，再实现
            elseif add_assistant(uin, aid, exp) then
                logger.trace("add ASSISTANT is success:", aid, exp)
            end

        end
    elseif type(cfg) == "table" and cfg.b_type == COMMON.item_btype.PIECE then
        local as = as_cache.as[cfg.virtual_real]
        if as then
            -- 将军级别为0的时候若碎片足够就升级为1
            if as.Level == 0 or as.ExpTime ~= 0 then
                local level_cfg = sharedata_utils.get_stat(COMMON.plan.AS_LEVEL, tonumber(cfg.virtual_real .. '01'))
                if not level_cfg then
                    logger.errorf("the first level cfg not found for assistant[%d]", cfg.virtual_real)
                    return
                end 
                
                if item and item.count >= level_cfg.Piece then
                    as.Level = 1
                    as.ExpTime = 0
                    flush = true
                    
                    local item_cb = {}
                    item_cb[as.AssistantId] = {itemId = as.AssistantId, count = 1,}
                    utils.combine_items(extra_add_items, item_cb)
                    agent_item.item_on_add(as.AssistantId, 1, src, false)
                    
                    local ret, item, item_cfg, extra_reduce_items = 
                        agent_item.reduce_item(cfg.id, level_cfg.Piece,
                            COMMON.change_src.assistant, true, extra_add_items)
                    as.PieceNum = item and item.count or 0
                    -- 不要被名字误解，此协议获得将军是将军解锁
                    --
                    local function sd()
                        skynet.sleep(300)
                        send_response(MESSAGE.CS_CMD_AS_NEW_ADD, {
                            NewAddAssistant = {as}
                        })
                    end
                    skynet.fork(sd)
                        
                    skynet.send(busilog_service, "lua", "log_misc",
                        COMMON.busilog_misc_type.misc_assistant_add,
                        account_info.uin, cfg.virtual_real, as.Level)
                end
            end
        else
            logger.warningf("agent_assistant->event_item_on_add without this assistant[%d]", cfg.virtual_real)
        end
    end
end

local function get_as_cache()
    return as_cache
end


local function assistant_check()
    for k,v in pairs(ASSISTANT_CONFIG) do
        if not as_cache.as[k] then
            as_cache.as[k] = {
                AssistantId   = v.TypeID,
                Level         = 0,
                Quality       = v.Quality,
                IsInBattle    = false, -- 默认未出战
                ActiveSkills  = v.ActiveSkills,
                PassiveSkills = v.PassiveSkills,
                ExpTime       = 0,
                PieceNum      = 0,
                SlotIndex  = 0,
            }
            flush = true
        end

        if not as_cache.as[k].PieceNum then
            as_cache.as[k].PieceNum = 0
            flush = true
        end
    end
end


local function register_events()
    EventCenter.addEventListener(Events.ITEM_ON_ADD, event_item_on_add)
end

-- 跨天处理
function agent_assistant.daily_check(...)
    if as_cache.piece_limit then
        as_cache.piece_limit = {}
        flush = true
    end
end

-- 碎片每天购买次数限制
function agent_assistant.reduce_item_balance(item, value)
    as_cache.piece_limit[item] = value
    flush = true
end

function agent_assistant.get_item_balance(item, value)
    if as_cache.piece_limit[item] then
        return as_cache.piece_limit[item]
    else
        as_cache.piece_limit[item] = value
        flush = true
        return nil
    end
end

function agent_assistant.check_save_db()
    if flush and as_cache then
        redis_call('hset_obj', ASSISTANT_INFO, uin, as_cache)
        flush = false
    end
end

local function add_cmd(cmd)
    cmd.agent_assistant = get_as_cache
    cmd.get_assits      = function() return  end
end

function agent_assistant.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code

    account_info    = args.info
    uin             = args.uin
    redis_call      = args.rediscall
    player          = args.player
    busilog_service = args.busilog_service

    ASSISTANT_CONFIG = sharedata_utils.get_stat_all(COMMON.plan.AS_ASSISTANT)

    loadAssistantData(uin)
    -- 将军检查，由于新增碎片需要将所有的将军类型返回，不论是否得到将此将军
    assistant_check()

    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)

    register_events()
    checkExp()
end

return agent_assistant

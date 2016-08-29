--  agent_skin.lua
--  时装系统

local agent_skin = {}

local skynet    = require "skynet"
local sharedata_utils = require "sharedata_utils"

local logger    = require "logger"
local proto     = require "proto"
local utils     = require "utils"
local COMMON    = require "common"
local agent_vip = require "agent_vip"
local MESSAGE   = COMMON.MESSAGE
local ERRORCODE = COMMON.ERRORCODE

local send_response, send_error_code
local account_info, uin, player
local redis_call
local busilog_service

local TABLE = 'skin'
local clothe_cfg
local clothe_cache
local flush = false
local default_skin = 0 -- 默认装备的时间
--[[
为方便扩展，玩家外饰都用一个数组结构 现暂时只有时装
{
    clothes = {
        [id] = {
            up = 0,        -- 0 未装备  1 装备
            expire = 0,
            forever = false,
        }
    },
    up = id  -- 默认false 代表没装备  装备了则为时装id
}

up = false 代表没有装备，装备后则为true
expire = 0 代表不能便用此时装
forever = true 代表永久
]]


local function init_clothe()
    return {up = 0, expire = 0, forever = false}
end


local function load_cfg()
    clothe_cfg = sharedata_utils.get_stat_all(COMMON.plan.SKIN)
    assert(clothe_cfg,'时装系统加载配置失败')
    for k, v in pairs(clothe_cfg) do
        if v.forever == 1 then -- 这个forever是代表默认送的，存储的forever是指买的永久的时装
            default_skin = k
            break
        end
    end
end

-- 加载缓存并处理新增的时装
local function load_cache()
    local new = false
    clothe_cache = redis_call("hget_obj", TABLE, uin, true)
    if not clothe_cache then
        clothe_cache = {
            clothes = {},
            up = false,
        }
        flush = true
        new = true
    end

    -- 新增新加的时装
    local clothes = clothe_cache.clothes
    for k,v in pairs(clothe_cfg) do
        if clothes and not clothes[k] then
            clothes[k] = init_clothe()
            -- vip时装
            if v.forever == 2 then
              clothes[k].expire = os.time() + agent_vip.get_vip3()
            end
            flush = true
        end

        -- 默认永久的时装
        if new and clothes[k] and 
            (v.forever == 1)  then
            clothe_cache.up = k
            clothes[k].up = 1
            clothes[k].forever = true
            flush = true
        end
    end
end

-- 同步时装信息
local function handle_sync(cmd, msg, sz)
    local rsp = {
        Data = {},
        DefaultSkin = default_skin
    }    
    local now = os.time()
    local up_expire = false
    for k,v in pairs(clothe_cache.clothes) do
        local expire
        if v.expire > 0 and now < v.expire then
            expire = v.expire 
        else
            expire = 0
        end

        -- 当前装备的时装过期了，默认为包子时装
        if clothe_cache.up and expire == 0 and k ~= default_skin and 
            clothe_cache.up == k and not v.forever then
            up_expire = true
            v.up = 0
        end
        
        table.insert(rsp.Data, { SkinId = k, Expire = expire, Up = v.up, Forever = v.forever})
    end

    if up_expire then
        clothe_cache.up = default_skin
        clothe_cache.clothes[default_skin].up = 1
        for k, v in pairs(rsp.Data) do
            if v.SkinId == default_skin then
                v.Up = 1
                break
            end
        end
    end
    send_response(cmd, rsp)
end


-- 装备、卸下时装
local function handle_up_down(cmd, msg, sz)
    local req = proto.parse_proto_req(cmd, msg, sz)

    local skin = clothe_cache.clothes[req.SkinId]
    if not skin then
        return send_error_code(cmd, ERRORCODE.SKIN_NO)
    end

    -- 时装失效
    if ((skin.expire == 0) or (os.time() > skin.expire)) and
        not skin.forever then
        return send_error_code(cmd, ERRORCODE.SKIN_DISABLE)
    end

    -- 若之前装备过就重置
    if clothe_cache.up and (req.Opt == 1) then
        local pre = clothe_cache.clothes[clothe_cache.up]
        assert(pre, '之前装备的时装丢失')
        pre.up = 0
    end

    -- 把引导的值重置,有引导则为引导获得的时装id
    if clothe_cache.guide then
        clothe_cache.guide = false
    end

    -- 装备当前时装
    skin.up = (req.Opt == 1) and 1 or 0
    clothe_cache.up = (req.Opt == 1) and req.SkinId or false
    flush = true
    
    local rsp = {
        SkinId = req.SkinId
    }
    -- 若是卸下，则装备默认的时装
    if req.Opt ~= 1 then
        rsp.SkinId = default_skin
        clothe_cache.up = default_skin
        clothe_cache.clothes[default_skin].up = 1
    end

    send_response(cmd, rsp)
end

-- 续命 ^_^ 延长时装有效期
local function handle_buy(cmd, msg, sz)
    local req = proto.parse_proto_req(cmd, msg, sz) 

    local skin = clothe_cache.clothes[req.SkinId]
    if not skin then
        return send_error_code(cmd, ERRORCODE.SKIN_NO)
    end

    local day = req.Day
    local cost = req.Cost
    if not string.match(clothe_cfg[req.SkinId].cost, day .. '#' .. cost) then
        return send_error_code(cmd, ERRORCODE.SKIN_BUY)
    end

    if cost > account_info.diamond then
        return send_error_code(cmd, ERRORCODE.DIAMAON_NOT_ENOUGH_ERROR)
    end
    player.change_diamond(account_info, COMMON.change_src.skin, -cost, true)

    local now = os.time()
    if day == 0 then
        skin.forever = true
    else
        local add_sec = day * 24 * 60 * 60
        if (skin.expire > 0) and (now < skin.expire) then
            skin.expire = skin.expire + add_sec
        else
            skin.expire = now + add_sec
        end
    end

    flush = true
    send_response(cmd, {Data = {SkinId = req.SkinId, Limit = skin.limit, Expire = skin.expire, 
        Up = skin.up, Forever = skin.forever}})

    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_skin_ori, uin, req.SkinId, day)
end

--  更新一个时装
local function update_skin(id, limit, day)
    local skin = clothe_cache.clothes[id]
    local now = os.time()
    local add_sec = day * 24 * 60 * 60
    if skin then
        if (skin.expire > 0) and (now < skin.expire) then
            skin.expire = skin.expire + add_sec
        else
            skin.expire = now + add_sec
        end
    else
        clothe_cache.clothes[id] = init_clothe()
        skin = clothe_cache.clothes[id]
        skin.expire = now + add_sec
    end
    flush = true
    local rsp = {Data = {SkinId = id, Expire = skin.expire, 
        Up = skin.up, Forever = skin.forever}}
    send_response(MESSAGE.CS_CMD_SKIN_NEW, rsp)
    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_skin_ori, uin, id, day)
end

-- 引导随机分配一个时装
local function handl_guide(cmd, msg, sz)
    local itemid = 0
    local skinId = 0
    -- 是否之前引导没有走完
    if clothe_cache.guide then
        assert(clothe_cfg[clothe_cache.guide], '没有这样的时装')
        skinId = clothe_cache.guide
    else
        local skinsId = {}
        for k,v in pairs(clothe_cfg) do
            if v.free ~= 0 then
                table.insert(skinsId, k)
            end
        end
        local len = #skinsId
        if len > 0 then
            local id = skinsId[math.random(1,len)]
            local cfg = clothe_cfg[id]
            itemid = cfg.itemid
            update_skin(id, cfg.limit, assert(cfg.free, '配置的体验时间为空'))
            clothe_cache.guide = id
            skinId = id
        else
            assert(nil, '时装引导没有得到限时时装')
        end
    end

    local rsp = {
        ItemId  = skinId, -- 时装id
        ItemProperty = COMMON.property_type.FASHION,
    }

    send_response(cmd, rsp)
end


local function add_cmd_handle_mapping(handle_mapping)
    local msg = MESSAGE
    handle_mapping[msg.CS_CMD_SKIN_SYNC]       = handle_sync
    handle_mapping[msg.CS_CMD_SKIN_UP_DOWN]    = handle_up_down
    handle_mapping[msg.CS_CMD_SKIN_BUY]        = handle_buy
    handle_mapping[msg.CS_CMD_SKIN_GUIDE]      = handl_guide
end

-- 得到装备好的时装
local function get_equip_skin()
    if clothe_cache.up and clothe_cache.clothes[clothe_cache.up] and 
        (clothe_cache.clothes[clothe_cache.up].forever or 
        os.time() < clothe_cache.clothes[clothe_cache.up].expire) then
        return clothe_cache.up
    else
        return default_skin  -- 装备的时装过期默认是包子时装
    end
end


-- 邮件添加限定时装的id 当属于时装类型的时候count代表天数,此物品不保存
local function on_item_add(eventName, count, item, cfg, extra, src, notice_flag)
    if cfg and  type(cfg) == "table" and cfg.s_type == COMMON.item_stype.SKIN then
        -- 是限定时装才
        assert(cfg.use_extra, '物品对应的时装为nil')
        local skin_cfg = clothe_cfg[cfg.use_extra]
        if skin_cfg then
            local id = cfg.use_extra
            local now = os.time()
            local add_sec = count * 24 * 60 * 60
            local skin = clothe_cache.clothes[id]
            if skin then
                if (skin.expire > 0) and (now < skin.expire) then
                    skin.expire = skin.expire + add_sec
                else
                    skin.expire = now + add_sec
                end
            else
                clothe_cache.clothes[id] = init_clothe(skin_cfg.limit)
                skin = clothe_cache.clothes[id]
                skin.expire = now + add_sec
            end
            flush = true
            send_response(MESSAGE.CS_CMD_SKIN_NEW, {Data = {SkinId = id, Limit = skin_cfg.limit, Expire = skin.expire, 
                Up = skin.up, Forever = skin.forever}})

            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_skin_ori, uin, id, count)
        end
    end
end

local function vip_change()
  local expire = agent_vip.get_vip3()
  if expire ~= 0 then
    local id
    for k,v in pairs(clothe_cfg) do
      if v.forever == 2 then
        id = k
        break
      end
    end
    if not id then return end
    local skin = clothe_cache.clothes[id]
    local now = os.time()
    if skin then
        if (skin.expire > 0) and (now < skin.expire) then
            skin.expire = skin.expire + expire
        else
            skin.expire = now + expire
        end
    else
        clothe_cache.clothes[id] = init_clothe()
        skin = clothe_cache.clothes[id]
        skin.expire = now + expire
    end
    flush = true
    local rsp = {Data = {SkinId = id, Expire = skin.expire, 
        Up = skin.up, Forever = skin.forever}}
    send_response(MESSAGE.CS_CMD_SKIN_NEW, rsp)
    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_skin_ori, uin, id, expire//86400)
  end
end

local function register_events()
    EventCenter.addEventListener(Events.ITEM_ON_ADD, on_item_add)
    EventCenter.addEventListener(Events.PLAYER_VIP_CHANGE, vip_change)
end

function agent_skin.get_skin_id(id)
    local tmp  = nil
    if uin == id then
        tmp = clothe_cache
    else
        tmp = redis_call("hget_obj", TABLE, id, true)
    end

    if tmp and tmp.up and tmp.clothes[tmp.up] and 
        (tmp.clothes[tmp.up].forever or 
        os.time() < tmp.clothes[tmp.up].expire) then
        return tmp.up
    else
        return default_skin -- 装备的时装过期默认是包子时装
    end
end

function agent_skin.check_save_db(  )
    if flush and clothe_cache then
        redis_call("hset_obj", TABLE, uin, clothe_cache, true)
        flush = false
    end
end

local function add_cmd(cmd)
    cmd.agent_skin  = get_equip_skin
    cmd.get_skins   = function() return clothe_cache end
end

function agent_skin.init(args)
    send_response   = args.send_response
    send_error_code = args.send_error_code

    account_info    = args.info
    uin             = args.uin
    redis_call      = args.rediscall
    player          = args.player
    busilog_service = args.busilog_service

    load_cfg()
    load_cache()
    add_cmd(args.cmd)
    add_cmd_handle_mapping(args.cmd_map)
    
    register_events()
end

return agent_skin

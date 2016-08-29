-- activity_valentine.lua
-- 情人节活动
local skynet          = require "skynet"
local logger          = require "logger"
local proto           = require "proto"
local sharedata_utils = require "sharedata_utils"
local utils           = require "utils"
local COMMON          = require "common"
local agent_item 	  = require "agent_item"

local agent, account_info, player, redis, send_package, send_response, send_error_code
local act_info = {}
local lover = {}


local flower = {
    Red     = 40001,  --红玫瑰
    Pink    = 40002,  --粉玫瑰
    Blue    = 40003,  --蓝玫瑰
}


-- 活动结束清除之前的玫瑰花~~
local function clear_flower()
	local src = COMMON.change_src.lover
	agent_item.del_item_by_id(flower.Red, src, false)
	agent_item.del_item_by_id(flower.Pink, src, false)
	agent_item.del_item_by_id(flower.Blue, src, false)
    -- 加载玫瑰花~~
	--[[
    local items = player.load_item(account_info.uin)
    if not items then
        logger.debugf("not found items for uin[%s] in activity_valentine.clear_flower", tostring(account_info.uin))
        return
    end

    -- red rose
    if items[flower.Red] then
        items[flower.Red] = nil
    end

    -- pink rose
    if items[flower.Pink] then
        items[flower.Pink] = nil
    end
    
    -- blue rose
    if items[flower.Blue] then
        items[flower.Blue] = nil
    end

    player.save_item(account_info.uin, items)
	]]
end

-- 获取基本信息
local function handle_get_data(cmd, content, len)

    if not act_info then
        logger.errorf("act_info is nil")
        return
    end

    -- 判断活动是否开启
    if not lover.check_open() then
        return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_NOT_START_ERROR)
    end
    
    -- 加载玫瑰花~~
    local resp = {
        StartTime = act_info.start_time,
        EndTime = act_info.end_time,
        RedRose = agent_item.get_item_count(flower.Red),
        PinkRose = agent_item.get_item_count(flower.Pink),
        BlueRose = agent_item.get_item_count(flower.Blue),
    }

    if not resp.RedRose then
        resp.RedRose = 0
    end

    if not resp.PinkRose then
        resp.PinkRose = 0
    end

    if not resp.BlueRose then
        resp.BlueRose = 0
    end

    return send_response(cmd, resp)
end

-- 礼盒兑换
local function handle_do_exchange(cmd, content, len)
    -- 拿到礼盒id
    local item_id = proto.parse_proto_req(cmd, content, len).ItemId
    -- 获取PropAttr.csv里面的配置
    local prop_attr = sharedata_utils.getItemById(item_id)
    if not prop_attr then
        logger.debugf("not found the item [%d] in PropAttr.csv", item_id)
        return send_error_code(cmd, COMMON.ERRORCODE.ITEM_NOT_EXIST)
    end

    -- 加载玫瑰花~~
    local not_exist = false
    local need_flower = 5
    local flowe_id = tonumber(prop_attr.params)
    
    if flowe_id == flower.Red then
        need_flower = 5
    elseif flowe_id == flower.Pink then
        need_flower = 10
    elseif flowe_id == flower.Blue then
        need_flower = 20
    else
        logger.errorf("not found flowers for uin[%s] flowe_id[%s]", tostring(account_info.uin), flowe_id)
        return 
    end
	
    if agent_item.get_item_count(flowe_id) < need_flower then
        return send_error_code(cmd, COMMON.ERRORCODE.ITEM_NOT_ENOUGH)
    end
    agent_item.reduce_item(flowe_id, need_flower, COMMON.change_src.lover, true)
    -- 增加物品
    agent_item.add_item(item_id, 1, COMMON.change_src.lover, true)
    return send_response(cmd, {})
end

local function add_cmd_handle_mapping( mapping )
    local msg = COMMON.MESSAGE
    mapping[msg.CS_CMD_VALENTINE_GET_DATA] = handle_get_data
    mapping[msg.CS_CMD_VALENTINE_EXCHANGE] = handle_do_exchange
end

local function get_config() 
    local activity_config = sharedata_utils.getActivityConfigById(COMMON.activity_id.ACT_LOVER)
    local now = os.time()
    if activity_config then
        act_info.is_opened = now >= activity_config.begindate and now < activity_config.enddate
        act_info.start_time = activity_config.begindate
        act_info.end_time = activity_config.enddate
    else
        act_info.is_opened = false
        act_info.start_time = 0
        act_info.end_time = 0
    end
end


function lover.init(args)
    agent        = args.agent
    account_info = args.info
    player       = args.player
    redis        = args.redis
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code

    add_cmd_handle_mapping(args.cmd_map)

    get_config()
end


function lover.get_open_activity(open_activity)
    if lover.check_open() then
        local item  = {
            Type = COMMON.activity_icon.ACT_LOVER
        }
        table.insert(open_activity, item)
    end
end

function lover.daily_check( notice_flag )
    if notice_flag then
        if not lover.check_open() then
            clear_flower()
        end
    end
end

-- 判断是否开启
function lover.check_open()
    local now = os.time()
    return now >= act_info.start_time and now < act_info.end_time
end

function lover.on_open(_act_info)
    act_info = _act_info
    -- get_cur_task_info_resp_data()
    -- print("new year is on_open")
end

function lover.on_end(_act_info)
    act_info.is_opened = false
    clear_flower()
end

function lover.on_update(_act_info)
    --act_info = act_info_
end

return lover

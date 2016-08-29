-- User: zhenggp
-- Date: 2015/11/11
-- Time: 20.52
-- Desc: agent的activity模块
--

local skynet = require "skynet"
local utils  = require "utils"
local logger = require "logger"
local proto  = require "proto"
local COMMON = require "common"
local MESSAGE   = COMMON.MESSAGE
local ERRORCODE = COMMON.ERRORCODE
local player    = require "player"
local sharedata_utils   = require "sharedata_utils"
local activity_sign         = require "activity_sign"
local activity_recharge     = require "activity_recharge"
local activity_power_energy = require "activity_power_energy"
local activity_christmas    = require "activity_christmas"
local activity_new_year     = require "activity_new_year"
local activity_valentine    = require "activity_valentine"


local send_package, send_response, send_error_code
local account_info
local agent_activity = {}


--- 发送已经开启的活动数据
-- 所有的活动都实现这个function get_open_activity(account_info, open_activity)
local function handle_get_open_activity_data(cmd, content, len)
    local open_activity = {}
    -- 注意顺序问题(和前端确认)
    activity_sign.get_open_activity(open_activity)
    activity_recharge.get_open_activity(account_info, open_activity)
    activity_power_energy.get_open_activity(open_activity) --在主界面显示，暂不返送
    activity_christmas.get_open_activity(open_activity)
    activity_new_year.get_open_activity(open_activity)
    activity_valentine.get_open_activity(open_activity)

    local resp = {
        OpenActivityInfo = open_activity
    }
    send_response(cmd, resp)
end


local function handle_streak_win_info(cmd, content, len)
    local flag = 0
    
    local data = {
        Ret = flag,
        StreakWinValue = account_info.streak_win_count,
        GetWinValue = 0,
    }
    return send_response(cmd, data)
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(cmd_handle_mapping)
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_GET_OPEN_ACTIVITY]  = handle_get_open_activity_data
    cmd_handle_mapping[COMMON.MESSAGE.CS_CMD_STREAK_WIN]         = handle_streak_win_info
end


function agent_activity.init(args)
    account_info = args.info
    send_package = args.send_package
    send_response = args.send_response
    send_error_code = args.send_error_code

    -- 模块必须实现的接口
    add_cmd_handle_mapping(args.cmd_map)
end


return agent_activity

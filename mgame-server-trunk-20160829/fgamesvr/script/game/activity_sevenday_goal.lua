--
-- Created by IntelliJ IDEA.
-- User: Denny
-- Date: 2016/3/7
-- Time: 11:27
-- To change this template use File | Settings | File Templates.
--

local activity_sevenday_goal = {}

local skynet          = require "skynet"

local sharedata_utils = require "sharedata_utils"
local utils           = require "utils"
local setting         = require "setting"
local logger          = require "logger"
local proto           = require "proto"
local COMMON          = require "common"
local MESSAGE         = COMMON.MESSAGE


local send_response, send_error_code
local account_info, uin, player
local redis_service, agent, account_info
local busilog_service

local CMD

local seven_goal
local init_flag = false
--存储结构
--[[
seven_goal:
	now_day  =  1,    --表示是第几天的活动
	retrived = false,
	over     = false,
	now_type = 1,
	retrive_time = 0, --领取时间

	task_list = {}
	goal_list = {}
	done_list = {}   --这个每个类型任务的完成数量
	finish_list = {}
--]]


local change_flag = false
local save_check_time = 30 * 100  -- 邮件保存检测时间
local function save_data()
	if change_flag then
		skynet.call(redis_service, "lua", "hset_obj", "seven_goal", uin, seven_goal)
		change_flag = false
	end

	skynet.timeout(save_check_time, save_data)
end

local function init_stat()
	seven_goal = {
		now_day  =  1,
		retrived = false,
		over     = false,
		now_type = 1,
	}

	local task_list = {}
	local goal_list = {}
	local done_list = {}
	local finish_list = {}

	for i=1, 7, 1 do
		local data = sharedata_utils.get_stat(COMMON.plan.SEVEN_DAY_GOAL, i)
		table.insert(task_list, data.type)
		table.insert(goal_list, data.para)
		table.insert(finish_list, false)
	end

	seven_goal.task_list = task_list
	seven_goal.goal_list = goal_list
	for k,v in ipairs(task_list) do
		done_list[v] = 0
	end
	seven_goal.done_list = done_list
	seven_goal.finish_list = finish_list

	change_flag = true
end

--初始化七天目标信息
local function init()
	if seven_goal then return end
	seven_goal = skynet.call(redis_service, "lua", "hget_obj", "seven_goal", uin)
	if not seven_goal then
		init_stat()
	end
end

local goal_type_name = {
	PVE_NUM = 1,
	PVP_NUM = 2,
	PVE_WIN_NUM = 3,
	PVE_STAR_NUM = 4,
	GLYPH_LEVEL = 5,
	HONOR = 6,
}

--检查今日任务有无完成， 完成就发送通知
local function check_mission(event_type)
	local now_day = seven_goal.now_day
	if now_day > 7 then return end
	if event_type == seven_goal.now_type then
		if seven_goal.finish_list[now_day] then return end
		if seven_goal.done_list[seven_goal.task_list[now_day]] >= seven_goal.goal_list[now_day] then
			seven_goal.finish_list[now_day] = true
			change_flag = true

			local num = seven_goal.goal_list[now_day]
			local next_num = 0
			if now_day < 7 then
				next_num = seven_goal.done_list[seven_goal.task_list[now_day]]
				local expect_next_num = seven_goal.goal_list[now_day]
				if next_num > expect_next_num then
					next_num = expect_next_num
				end
			end

			local data = {
				NowDay = seven_goal.now_day,
				IsRetrived = seven_goal.retrived,
				Num = num,
				NextNum = next_num
			}

			send_response(COMMON.MESSAGE.CS_CMD_GET_SEVENDAY_GOAL_LIST, data)
            skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_sevenday_goal, account_info.uin, now_day, COMMON.misc_action.act_task_com)
		end
	end
end

local function on_pvp_end(eventName, room_flag, room_type, result, addscore, addpoint)
	seven_goal.done_list[goal_type_name.PVP_NUM] = seven_goal.done_list[goal_type_name.PVP_NUM] + 1
	change_flag = true

	--判断是否完成了，如果完成了就销毁这类事件的监听

	if result == COMMON.pvp_end_type.WIN then
		seven_goal.done_list[goal_type_name.PVE_WIN_NUM] = seven_goal.done_list[goal_type_name.PVE_WIN_NUM] + 1
		change_flag = true
	end

	check_mission(goal_type_name.PVP_NUM)
	check_mission(goal_type_name.PVE_WIN_NUM)
end

local function on_pve_end(eventName, new_flag, stars)
	if new_flag then
		seven_goal.done_list[goal_type_name.PVE_NUM] = seven_goal.done_list[goal_type_name.PVE_NUM] + 1
		change_flag = true
	end
	if stars then
		seven_goal.done_list[goal_type_name.PVE_STAR_NUM] = seven_goal.done_list[goal_type_name.PVE_STAR_NUM] + stars
		change_flag = true
	end

	check_mission(goal_type_name.PVE_NUM)
	check_mission(goal_type_name.PVE_STAR_NUM)
end

local function on_glyph_upgrade(eventName, level)
	if level ~= seven_goal.goal_list[6] then return end
	seven_goal.done_list[goal_type_name.GLYPH_LEVEL] = level
	seven_goal.finish_list[6] = true  --第6天为符文升级， 出于效率这样写的

	check_mission(goal_type_name.GLYPH_LEVEL)
end

local function on_honor_add(eventName, honor)
	if honor > 0 then
		seven_goal.done_list[goal_type_name.HONOR] = seven_goal.done_list[goal_type_name.HONOR] + honor
		change_flag = true
	end
	check_mission(goal_type_name.HONOR)
end

--对应策划表中的type
local events_type_array = {
	[1] = {event = Events.PVE_END,      func = on_pve_end},
	[2] = {event = Events.PVP_END,      func = on_pvp_end},
	[3] = {event = Events.PVP_END,      func = on_pvp_end},
	[4] = {event = Events.PVE_END,      func = on_pve_end},
	[5] = {event = Events.GLYPH_UPGRADE,  func = on_glyph_upgrade},
	[6] = {event = Events.HONOR_ADD,    func = on_honor_add},
}

-- notice_flag:跨天提醒标识
-- flag:初始化判断标识
local function daily_check(notice_flag, flag)
	if seven_goal.over then return end
	if not seven_goal.retrived then return end

	if seven_goal.now_day <= 7 then
		if flag then
			if utils.is_same_day(seven_goal.retrive_time or 0, math.floor(skynet.time())) then
				return
			end
		end

		seven_goal.now_day = seven_goal.now_day + 1
		if seven_goal.now_day <= 7 then
			seven_goal.retrived = false
			seven_goal.now_type = seven_goal.task_list[seven_goal.now_day]
			if seven_goal.done_list[seven_goal.now_type] > seven_goal.goal_list[seven_goal.now_day] then
				seven_goal.finish_list[seven_goal.now_day] = true
			end
		end
	else
		seven_goal.over = true
	end

	check_mission(seven_goal.now_type)

	change_flag = true
end

local function on_daily_check(eventName, notice_flag, flag)
	--dailycheck不关心eventName
	daily_check(notice_flag, flag)
end

local function init_event_modules()
	EventCenter.addEventListener(Events.DAY_CHANGE, daily_check)

	local events_hash = {}
	for k,v in ipairs(seven_goal.task_list) do
		if not seven_goal.finish_list[k] then
			local event = events_type_array[v].event
			local func = events_type_array[v].func
			if not events_hash[event] then
				EventCenter.addEventListener(event, func)
				events_hash[event] = true
			end
		end
	end
end

local function handle_get_list(cmd, content, len)
	local now_day = seven_goal.now_day
	local data
	if now_day <= 7 then
		local num = seven_goal.done_list[seven_goal.task_list[now_day]]
		local expect_num = seven_goal.goal_list[now_day]
		if num > expect_num then
			num = expect_num
		end

		local next_num = 0
		if now_day < 7 then
			next_num = seven_goal.done_list[seven_goal.task_list[now_day+1]]
			local expect_next_num = seven_goal.goal_list[now_day+1]
			if next_num > expect_next_num then
				next_num = expect_next_num
			end
		end

		data = {
			NowDay = now_day,
			IsRetrived = seven_goal.retrived,
			Num = num,
			NextNum = next_num
		}
	else
		data = {
			NowDay = now_day,
			IsRetrived = true,
			Num = 0,
			NextNum = 0,
		}

	end

	send_response(cmd, data)
end

local function handle_retrive(cmd, content, len)
	if seven_goal.retrive then
		return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_RECEIVE_ERROR)
	end

	if not seven_goal.finish_list[seven_goal.now_day] then
		return send_error_code(cmd, COMMON.ERRORCODE.ACTIVITY_GOAL_NOT_REACH)
	end

	--领取礼包
	local conf = sharedata_utils.get_stat(COMMON.plan.SEVEN_DAY_GOAL, seven_goal.now_day)
	player.add_gift(account_info, conf.reward, COMMON.change_src.sevenday_goal)
	seven_goal.retrived = true
	change_flag = true
	seven_goal.retrive_time = math.floor(skynet.time())

	send_response(cmd, {})
    skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_sevenday_goal, account_info.uin, seven_goal.now_day, COMMON.misc_action.act_task_submit)
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(cmd_handle_mapping)
	cmd_handle_mapping[MESSAGE.CS_CMD_GET_SEVENDAY_GOAL_LIST]   = handle_get_list
	cmd_handle_mapping[MESSAGE.CS_CMD_RETRIVE_SEVENDAY_GOAL]    = handle_retrive
end

function activity_sevenday_goal.init(args)
	send_response   = args.send_response
	send_error_code = args.send_error_code

	account_info  = args.info
	uin           = args.uin
	player        = args.player
	redis_service = args.redis
	CMD           = args.cmd
	uin           = account_info.uin
	busilog_service = args.busilog_service

	add_cmd_handle_mapping(args.cmd_map)

	if not init_flag then
		init()
		daily_check(false, true) --只需要在第一次打开的时候，对比下时间，看是否有需要更新天数
		--Todo：可在这个完成后，就不在加载这个模块，这样是最合理的
		if not seven_goal.over then  --所有目标都完成后就不用注册事件了
			init_event_modules()
		end

		init_flag = false
	end

	skynet.timeout(save_check_time, save_data)
end

return activity_sevenday_goal
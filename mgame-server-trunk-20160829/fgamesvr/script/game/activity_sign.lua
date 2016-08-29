--
-- Created by IntelliJ IDEA.
-- User: Administrator
-- Date: 2016/2/26
-- Time: 10:45
-- To change this template use File | Settings | File Templates.
--

local activity_sign = {}

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

-- 玩家签到数据
-- {
-- now_day:签到进行到了第几天 1-7
-- last_day:上一次签到是一年中的第几天,
-- get_list:[是否已签到],
-- first_get_list:[第一次是否已签到], 是否已经领取过一次礼物
-- nor_list:[对已领取过的礼品显示是否常规礼包]
-- }
local p_sign
local uin

-- 请求签到列表
-- uin : 玩家唯一标识
local function execute_list_req(uin)
	local resp = {
		nowDay = p_sign.now_day,
		getList = {}
	}

	local get_list       = p_sign.get_list
	local first_get_list = p_sign.first_get_list
	local nor_list       = p_sign.nor_list

	for i = 1, #get_list do

		-- 是否已经领取过一次礼物
		if first_get_list[i] then

			if nor_list[i] then
				table.insert(resp.getList, { isGet = get_list[i], isNewGift = false })
			else
				table.insert(resp.getList, { isGet = get_list[i], isNewGift = true })
			end
		else
			-- 没有领取过礼物,显示新礼物和未领取
			table.insert(resp.getList, { isGet = false, isNewGift = true })
		end
	end

	return resp
end

-- 日期处理 重置签到周期, 或增加当前进行到第N天,更新上次签到日期
function activity_sign.daily_check(notice_flag)
	local c_yday = os.date("*t", os.time()).yday -- 当前是一年中的第几天
	local days = c_yday - p_sign.last_day
	if days ~= 0 then
		local old_day_index = p_sign.now_day
		--如果前一天的奖励有领取
		if p_sign.get_list[old_day_index] then
			p_sign.now_day = p_sign.now_day + 1
		end

		if p_sign.now_day > 7 then
			p_sign.now_day = 1
			p_sign.get_list = { false, false, false, false, false, false, false }

			local first_get_list = p_sign.first_get_list
			for i = 1, #first_get_list do
				if first_get_list[i] then
					p_sign.nor_list[i] = true
				else
					break
				end
			end
		end
		p_sign.last_day = c_yday
		skynet.call(redis_service, "lua", "hset_obj", "seven_day_sign", uin, p_sign)
		if notice_flag then
			send_response(COMMON.MESSAGE.CS_CMD_SEVEN_DAY_SIGN_LIST, execute_list_req(uin))
		end
	end
end

--初始化七天签到信息
local function init()

	redis_service = skynet.queryservice("redis_service")
	if p_sign == nil then
		p_sign = skynet.call(redis_service, "lua", "hget_obj", "seven_day_sign", uin, true)
		if p_sign == nil then
			p_sign = {
				now_day = 1,
				last_day = os.date("*t", os.time()).yday,
				nor_list =
				{ false, false, false, false, false, false, false },
				first_get_list =
				{ false, false, false, false, false, false, false },
				get_list = { false, false, false, false, false, false, false }
			}

			skynet.call(redis_service, "lua", "hset_obj", "seven_day_sign", uin, p_sign)
		end
	end
end

-- 请求签到
local function execute_sign_req(account_info)
	-- 查看玩家在内存中是否有数据
	if p_sign == nil then
		return
	end

	local get_list       = p_sign.get_list
	local first_get_list = p_sign.first_get_list
	local nor_list       = p_sign.nor_list
	local now_day_index  = p_sign.now_day

	local resp = {}

	if get_list[now_day_index] then
		resp.giftId = 0 -- 本次已经领取过了礼物,没有礼物了~
	else
		local actLoginAwardData = sharedata_utils.get_stat_all(COMMON.plan.ActLoginAward)
		local now_day_data = actLoginAwardData[now_day_index]

		if first_get_list[now_day_index] then
			-- 给玩家添加常规奖励
			resp.giftId = now_day_data.GiftNor
		else
			first_get_list[now_day_index] = true

			-- 给玩家添加新手奖励
			resp.giftId = now_day_data.GiftSpe
		end

		get_list[now_day_index] = true

		skynet.send(busilog_service, "lua", "log_misc", COMMON.busilog_misc_type.misc_sevenday_sign, account_info.uin, now_day_index)
		logger.debug("玩家7日签到成功 uin:", uin, " , gift_id = ", resp.giftId)

		-- 向redis中设置数据
		skynet.call(redis_service, "lua", "hset_obj",
			"seven_day_sign", uin, p_sign)
	end

	return resp.giftId, resp
end

function activity_sign.get_open_activity(open_activity)
	table.insert(open_activity, { Type = COMMON.activity_icon.ACT_SIGN })
	return open_activity
end


-- 请求获取7日签到列表
local function handle_sign_list(cmdid)
	local pack_content = execute_list_req(account_info.uin)
	if pack_content then
		send_response(cmdid, pack_content)
	end
end

-- 请求签到
local function handle_sign(cmdid)
	local gift_id, pack_content = execute_sign_req(account_info)
	if gift_id ~= nil then
		if gift_id ~= 0 then
			player.add_gift(account_info, gift_id, COMMON.change_src.sign)
		end

		send_response(cmdid, pack_content)
	end
end

-- 添加agent消息处理函数
local function add_cmd_handle_mapping(cmd_handle_mapping)
	cmd_handle_mapping[MESSAGE.CS_CMD_SEVEN_DAY_SIGN_LIST]  = handle_sign_list
	cmd_handle_mapping[MESSAGE.CS_CMD_SEVEN_DAY_SIGN]  = handle_sign
end

function activity_sign.init(args)
	send_response   = args.send_response
	send_error_code = args.send_error_code

	account_info  = args.info
	uin           = args.uin
	player        = args.player
	redis_service = args.redis
	CMD           = args.cmd
    busilog_service = args.busilog_service

	add_cmd_handle_mapping(args.cmd_map)

	init()

	uin = account_info.uin
end

return activity_sign

--信鸽服务
--
--由于信鸽服务用到http请求所以函数执行时间无法控制，
--调用信鸽服务器的方法只能用 skynet.send 
--切忌不可用skynet.call 因为函数不会返回任何东西
--
--author diandian
--
--
--
local skynet = require "skynet"
local logger = require "logger"
local traceback = debug.traceback
local sharedata_utils = require "sharedata_utils"
local COMMON = require "common"
local utils = require "utils"
local redis_service
local one_day = 24 * 60 * 60
local md5 = require "md5"
local httpc = require "http.httpc"
local platform_setting = require "platform_setting"
local json = require "cjson"


local xinge_host = "openapi.xg.qq.com"
local xinge_uri = "/v2/push/"
local xinge_methed = "GET"

-- 检查发送
local next_push_time = {}
local function ckeck_push()
    local now_time = math.floor(os.time())
    for push_time, push_data in pairs(next_push_time) do
        if now_time > tonumber(push_time) then
            next_push_time[os.date(push_time + one_day)] = push_data
            if push_data then
                -- TODO 推送人选择...
                local xinge_infos = skynet.call(redis_service, "lua", "hgetall_obj", "xinge")
                if xinge_infos then
                    for _, xinge_info in pairs(xinge_infos) do
                        if xinge_info.secret_key and xinge_info.secret_key ~= "" then
                            --if not utils.is_same_day(xinge_info.login_time) then
                            os.execute("python ./python/push_token_android.py " .. xinge_info.secret_key .. " " .. push_data.title .. " " .. push_data.content)
                            --end
                        end
                    end
                end
            end
            next_push_time[push_time] = nil
        end
    end
    skynet.timeout(2000, ckeck_push)
end

-- 服务器启动时初始化发送时间
local function init_push_time()
    local push_data = sharedata_utils.get_stat_all(COMMON.plan.Push)
    if not push_data then return end
    local now_time = math.floor(os.time())
    for _, push in pairs(push_data) do
        local hour_m_s = utils.split(push.time, ":") -- 配置的时分秒
        local date = os.date("*t", os.time())
        date.hour = hour_m_s[1]
        date.min = hour_m_s[2]
        date.sec = hour_m_s[3]
        local push_time = math.floor(os.time(date))
        if now_time - push_time > 0 then
            next_push_time[os.date(push_time + one_day)] = push
        else
            next_push_time[push_time] = push
        end
    end
    ckeck_push()
end

local function http2xinge(param, push_type)
	param.access_id = platform_setting.xinge.access_id
	param.timestamp = os.time()

	local keys = {}
	for k, v in pairs(param) do
		table.insert(keys, k)
	end
	table.sort(keys)

	local param_str = "";
	xinge_uri = xinge_uri .. push_type
	local sign_str = xinge_methed .. xinge_host .. xinge_uri
	for k, v in ipairs(keys) do
		sign_str = sign_str .. v .. "=" .. param[v]
	end
	sign_str = sign_str .. platform_setting.xinge.secret_key
	param.sign = md5.sumhexa(sign_str)

	local content = ""
	for k, v in pairs(param) do
		k = utils.urlencode(k)
		v = utils.urlencode(v)
		if content == "" then
			content = k .. "=" .. v
		else
			content = content .. "&".. k .. "=" .. v
		end
	end

	local recvheader = {}
	local status, body = httpc.request(xinge_methed, xinge_host, xinge_uri, recvheader, nil, content)

	print(body)
	if status ~= 200 then
	end
end

local CMD = {}
function CMD.pushMsgToPlayer(uin, msg)
	local param = {}
	param.account = uin
	param.message_type = 1

	msg.vibrate = 1
	param.message = json.encode(msg)
	http2xinge(param, "single_account")
end

function CMD.pushAll(msg)
	local param = {}
	param.message_type = 1
	param.message = json.encode(msg)
	http2xinge(param, "all_device")
end

skynet.start(function()
    logger.trace("xinge service start...")
    redis_service = skynet.queryservice("redis_service")

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
		
		if ret ~= nil then
			skynet.retpack(ret)
		end
    end)
end)

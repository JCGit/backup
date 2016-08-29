--
-- Created by IntelliJ IDEA.
-- User: Administrator
-- Date: 2016/5/16
-- Time: 11:55
-- To change this template use File | Settings | File Templates.
--

local skynet = require "skynet"
local COMMON = require "common"
local logger = require "logger"
local utils  = require "utils"
local operate_activity_service = ...

local redis_service
local gamed

local activity_config
local selfData
local globalData

-- �����
--[[
    id              �ID
    name            �����
    desc            ���������
    type            �����
    startTime       ��ʼʱ�䡾��ȷ���롿
    endTime         ����ʱ��
    state           �״̬ ��1 ���δ���� 2 ��������� 3 �ɼ� 4 ������ 5 ���ڡ�
    updateTime      ����һ�β���ʱ��operate_activities
    icon            �ͼ��
    ]]

local CMD = {}
--[[ ����ʵ�ֵĽӿ�
    CMD.load(data)
    CMD.update
    CMD.remove
    CMD.__checkConfig���ڲ����á�
    CMD.activity_state_changed(data)
    -- ...
--]]


function CMD.load(data)
	-- У�����ݺϷ���
	-- У�����ݺϷ���
	if not CMD.__checkConfig(data) then
		return false
	end


	activity_config = data

	return true
end

--����У��
function CMD.__checkConfig(data)
	local extraData = {
		"title", "content", "plugin",
	}
	for _, attr in ipairs(extraData) do
		if not data[attr] then
			return false
		end
	end

	return true
end


-- �����
-- �����Ҫ��������жϻ���ʱ��Ĵ��� ��update�д���
function CMD.update()

end

-- ��Ƴ�����
-- �Ƴ�ȫ�ֻ���� �Ƴ����˻���ݡ���ܴ���
function CMD.remove()
	-- �رշ���
	skynet.exit()
end

-- �״̬�ı䴦��
function CMD.activity_state_changed(state)
	activity_config.state = state
end

local traceback = debug.traceback
skynet.start(function()
	logger.trace("operate_activity_notice start...")

	redis_service = skynet.queryservice("redis_service")
	gamed = skynet.queryservice("gamed")

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
		skynet.retpack(ret)
	end)
end)



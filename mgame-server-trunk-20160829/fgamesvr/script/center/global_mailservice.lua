--
-- Created by IntelliJ IDEA.
-- User: Administrator
-- Date: 2015/10/27
-- Time: 22:19
-- To change this template use File | Settings | File Templates.
--

local skynet = require "skynet"
local cluster = require "cluster"
require "skynet.manager" -- import skynet.name

local logger = require "logger"
local utils = require "utils"
local COMMON = require "common"

local redis_service
local query_service
local syncserver

local CMD = {}
function CMD.add_mail(uin, mail_type, data, business_type)
	if not mail_type then
		return false
	end

	--1.���ʼ����뵽��Ӧ��redis����
	local mail_id
	if mail_type == COMMON.mail_type.SINGLE then
		mail_id = skynet.call(redis_service, "lua", "id_handler", "max_single_mail_id")
		skynet.call(redis_service, "lua", "hset_obj", "single_mail", mail_id, data)
	elseif mail_type == COMMON.mail_type.GLOBAL then
		mail_id = skynet.call(redis_service, "lua", "id_handler", "max_global_mail_id")
		skynet.call(redis_service, "lua", "hset_obj", "global_mail", mail_id, data)
	else
		return false
	end

	--1.�����ж��Ƿ�Ϊȫ���ʼ��� �����ȫ����ֱ�Ӹ�ÿһ�������ͼ��ʼ�����
	--2.����ǵ����ֻ��ָ���ڵ㷢���������Ҳ����ߣ�Ҳ�Ǹ����ԭ���ڷ���������

	if mail_type == COMMON.mail_type.SINGLE then
		--��ȡ������Ǹ��ӵ㣬û����ֱ��дredis
		local data = skynet.call(redis_service, "lua", "hget_obj", "player_node", uin)

		local gamed
		if data then
			gamed = data.servername
		end

		if gamed == nil then
			return nil
		end


		local clusternode = utils.query_nodename(gamed)
		logger.trace("query_nodename : ", clusternode)

		if clusternode then
			cluster.call(clusternode, ".mail_service", "add_usermail", mail_type, uin,  mail_id, business_type)
		else
			clusternode = skynet.call(syncserver, "lua", "get_random_gamecluster")
			if clusternode then
				cluster.call(clusternode, ".mail_service", "add_usermail", mail_type, uin, mail_id, business_type)
			else
				logger.error("have no gamed now")
			end
		end
	elseif mail_type == COMMON.mail_type.GLOBAL then
		local cluster_list = skynet.call(syncserver, "lua", "get_all_gamecluster")
		for k, v in ipairs(cluster_list) do
			cluster.call(v, ".mail_service", "add_usermail", mail_type, uin, mail_id, business_type)
		end
	end

	return true
end

local traceback = debug.traceback
skynet.start(function()
	redis_service = skynet.queryservice("redis_service")
	syncserver = skynet.queryservice("syncserver")

	skynet.dispatch("lua", function(_, _, command, ...)
		logger.trace("command : " .. command)
		local f = CMD[command]
		if not f then
			logger.warningf("unhandled message(%s) id : %d", command, skynet.self())
			return skynet.ret()
		end

		local ok, ret = xpcall(f, traceback, ...)
		if not ok then
			logger.warningf("handle message(%s) failed : %s  id : %d", command, ret, skynet.self())
			return skynet.ret()
		end

		skynet.retpack(ret)
	end)
end)


local skynet = require "skynet"

local logger = require "logger"
local redis_service
local unique_code_factory_service

local CMD = {}

local char_mapping = {
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"w",
	"x",
	"y",
	"z",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"0",
}


function CMD.get_invite_code()
	return skynet.call(unique_code_factory_service, "lua", "create_code")
end

local traceback = debug.traceback
skynet.start(function()
	
	logger.tracef("invite_code_service is starting..")
	redis_service = skynet.queryservice("redis_service")
	unique_code_factory_service = skynet.newservice("unique_code_factory_service")
	skynet.call(unique_code_factory_service, "lua", "init", "invite", 5, char_mapping)

	--测试代码
--	local fac_s = skynet.newservice("unique_code_factory_service")
--	skynet.call(fac_s, "lua", "init", "test_1", 5, char_mapping)
--
--	print("start test code factory ....")
--	for i = 1, 200 do
--		local code, err = skynet.call(fac_s, "lua", "create_code")
--
--		if code == "" then
--			print("code create error : ", err)
--			return;
--		end
--
--		local code_is = skynet.call(redis_service, "lua", "hget_obj", "test_1_code_mapping", code, true)
--		if code_is ~= nil then
--			print("test create a exist code : ", code)
--			return;
--		end
--		skynet.call(redis_service, "lua", "hset_obj", "test_1_code_mapping", code, 1, true)
--		skynet.call(redis_service, "lua", "set_obj", "test_num", i)
--	end
--

	skynet.dispatch("lua", function(_, _, command, ...)
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

local skynet       = require "skynet"
local socket       = require "socket"
local httpd        = require "http.httpd"
local sockethelper = require "http.sockethelper"
local urllib       = require "http.url"

local utils         = require "utils"
local server_config = require "server_config"
local logger        = require "logger"

local table = table
local string = string

local mode = ...

if mode == "agent" then

    local function response(id, ...)
        local ok, err = httpd.write_response(sockethelper.writefunc(id), ...)
        if not ok then
            skynet.error(string.format("fd = %d, %s", id, err))
        end
    end

    skynet.start(function()

        local gmservice = skynet.queryservice("global_gmservice")
        local global_cdk_service = skynet.queryservice("global_cdk_service")

        skynet.dispatch("lua", function(_, _, id)

            socket.start(id)
            local code, url, method, header, body = httpd.read_request(sockethelper.readfunc(id), 8192)
            if code then
                if code ~= 200 then
                    response(id, code)
                else
                    local tmp = {}

                    local path, query = urllib.parse(url)
                    if body == "" then   --Notice这里是针对Get请求，body为空的情况
                        body = query
                    end
                    logger.tracef("start dispatch path %s body %s", path, body )
                    --通过路径来做判断[ip:port/gm/addgold] [ip:port/xiaomi/login]
                    if string.match(path, "gm") then
                        local result = skynet.call(gmservice, "lua", "parse_cmd", body)
                        table.insert(tmp, tostring(result))
                    elseif string.match(path, "management") then
                        local result = skynet.call(gmservice, "lua", "sys_ctrl", body)
                        table.insert(tmp, tostring(result))
                    elseif string.match(path, "cdk") then
                        local platf_action = utils.split(path, "/")
                        local ret, resp = skynet.call(global_cdk_service, "lua", platf_action[3], body)
                        table.insert(tmp, tostring(resp))
                    end

                    response(id, code, table.concat(tmp,"\n"))
                end
            else
                -- 如果抛出的异常是 sockethelper.socket_error 表示是客户端网络断开了
                if url == sockethelper.socket_error then
                    skynet.error("socket closed")
                else
                    skynet.error(url)
                end
            end
            socket.close(id)
        end)
    end)
else
    skynet.start(function()

        local conf = server_config.global_web_conf

        local agent = {}
        for i = 1, conf.agent_number or 10 do
            agent[i] = skynet.newservice("global_webservice", "agent")
        end

        logger.tracef("listen addr: %s:%d ",  conf.ip, conf.port)

        local id = socket.listen(conf.ip, conf.port)
        local balance = 1
        socket.start(id , function(id, addr)
            skynet.send(agent[balance], "lua", id)
            balance = balance + 1
            if balance > #agent then
                balance = 1
            end
        end)
    end)
end

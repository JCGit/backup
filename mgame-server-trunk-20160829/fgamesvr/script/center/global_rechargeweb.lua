local skynet       = require "skynet"
local socket       = require "socket"
local httpd        = require "http.httpd"
local sockethelper = require "http.sockethelper"
local urllib       = require "http.url"

local platform_callback

local table  = table
local string = string

local mode = ...

local server_config = require "server_config"
local logger = require "logger"

if mode == "agent" then

    local function response(id, ...)
        logger.tracef("response  id %d  " , id)
        local ok, err = httpd.write_response(sockethelper.writefunc(id), ...)
        if not ok then
            skynet.error(string.format("fd = %d, %s", id, err))
        end
    end

    skynet.start(function()

        platform_callback = skynet.queryservice("platform_callback")

        skynet.dispatch("lua", function(_, _, id)
            logger.tracef("start dispatch  id  %d " , id)
            socket.start(id) -- 开始接收一个 socket
            -- limit request body size to 8192 (you can pass nil to unlimit)
            -- 一般的业务不需要处理大量上行数据，为了防止攻击，做了一个 8K 限制。这个限制可以去掉。
            local code, url, method, header, body = httpd.read_request(sockethelper.readfunc(id), 8192)
            if code then
                if code ~= 200 then
                    response(id, code)
                else
                    local tmp = {}
                    local path, query = urllib.parse(url)
                    if query then
                        --ip:port/xiaomi/login
                        if body == "" then   --Notice这里是针对Get请求，body为空的情况
                            body = query
                        end
                        logger.tracef("request: %s , body:%s" , path, body)
                        if string.match(path,"callback") then
                            local state = skynet.call(platform_callback, "lua", "on_action", path, body)
                            table.insert(tmp, tostring(state))
                        end
                    end
                    logger.tracef(table.concat(tmp, "\n"))
                    response(id, code, table.concat(tmp, "\n"))
                end
            else
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

        local conf = server_config.recharge_conf

        local agent = {}
        for i = 1, conf.agent_number or 20 do
            agent[i] = skynet.newservice(SERVICE_NAME, "agent")
        end

        logger.tracef("listen addr: %s:%d", conf.ip, conf.port)

        local id = socket.listen(conf.ip, conf.port)
        local balance = 1
        socket.start(id, function(id, addr)
            skynet.send(agent[balance], "lua", id)
            balance = balance + 1
            if balance > #agent then
                balance = 1
            end
        end)
    end)
end

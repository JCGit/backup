local skynet = require "skynet"
local netpack = require "netpack"
local socketdriver = require "socketdriver"

local gateserver = {}

local socket -- listen socket
local queue -- message queue
local maxclient -- max client
local client_number = 0
local CMD = setmetatable({}, { __gc = function() netpack.clear(queue) end })
local nodelay = false

local ban_newconn = false
local connection = {}

skynet.register_protocol {
    name = "client",
    id = skynet.PTYPE_CLIENT,
}

function gateserver.openclient(fd)
    if connection[fd] then
        socketdriver.start(fd)
    end
end

function gateserver.closeclient(fd)
    local c = connection[fd]

    if c then
        local agent = c.agent
        if agent then
            c.agent = nil
        end
        client_number = client_number - 1
        connection[fd] = nil
        socketdriver.close(fd)
    end
end

function gateserver.ban_newconn()
    ban_newconn = true
end

-- 这里增加的forward,是为了分发agent做的
function gateserver.forward(fd, agent)
    local c = connection[fd]
    if c then
        c.agent = agent
    end
end

function gateserver.start(handler)
    assert(handler.message)
    assert(handler.connect)

    function CMD.open(source, conf)
        assert(not socket)
        local address = conf.address or "0.0.0.0"
        local port = assert(conf.port)
        maxclient = conf.maxclient or 1024
        nodelay = conf.nodelay
        skynet.error(string.format("Listen on %s:%d", address, port))
        socket = socketdriver.listen(address, port)
        socketdriver.start(socket)
        if handler.open then
            return handler.open(source, conf)
        end
    end

    function CMD.close()
        assert(socket)
        socketdriver.close(socket)
    end

    local MSG = {}

    local function dispatch_msg(fd, msg, sz)
        if connection[fd] then
            local c = connection[fd]
            local agent = c.agent
            if agent then
                skynet.redirect(agent, 0, "client", 0, msg, sz)
            else
                handler.message(fd, msg, sz)
            end
        else
            skynet.error(string.format("Drop message from fd (%d) : %s", fd, netpack.tostring(msg, sz)))
        end
    end

    MSG.data = dispatch_msg

    local function dispatch_queue()
        local fd, msg, sz = netpack.pop(queue)
        if fd then
            -- may dispatch even the handler.message blocked
            -- If the handler.message never block, the queue should be empty, so only fork once and then exit.
            skynet.fork(dispatch_queue)
            dispatch_msg(fd, msg, sz)

            for fd, msg, sz in netpack.pop, queue do
                dispatch_msg(fd, msg, sz)
            end
        end
    end

    MSG.more = dispatch_queue

    function MSG.open(fd, msg)
        if client_number >= maxclient  or ban_newconn then
            socketdriver.close(fd)
            return
        end

        if nodelay then
            socketdriver.nodelay(fd)
        end

        connection[fd] = { fd = fd, addr = addr, }

        client_number = client_number + 1
        handler.connect(fd, msg)
    end

    local function close_fd(fd)
        local c = connection[fd]
        if c ~= nil then
            if c.agent then
                c.agent = nil
            end
            connection[fd] = nil
            client_number = client_number - 1
        end
    end

    function MSG.close(fd)
        if fd ~= socket then
            if handler.disconnect then
                handler.disconnect(fd)
            end
            close_fd(fd)
        else
            socket = nil
        end
    end

    function MSG.error(fd, msg)
        if fd == socket then
            socketdriver.close(fd)
            skynet.error(msg)
        else
            if handler.error then
                handler.error(fd, msg)
            end
            close_fd(fd)
        end
    end

    function MSG.warning(fd, size)
        if handler.warning then
            handler.warning(fd, size)
        end
    end

    skynet.register_protocol {
        name = "socket",
        id = skynet.PTYPE_SOCKET, -- PTYPE_SOCKET = 6
        unpack = function(msg, sz)
            return netpack.filter(queue, msg, sz)
        end,
        dispatch = function(_, _, q, type, ...)
            queue = q
            if type then
                MSG[type](...)
            end
        end
    }

    skynet.start(function()
        skynet.dispatch("lua", function(_, address, cmd, ...)
            local f = CMD[cmd]
            if f then
                skynet.ret(skynet.pack(f(address, ...)))
            else
                skynet.ret(skynet.pack(handler.command(cmd, address, ...)))
            end
        end)
    end)
end

return gateserver

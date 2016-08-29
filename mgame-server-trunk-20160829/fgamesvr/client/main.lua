local skynet = require 'skynet'

skynet.start(function ()
    local settings = require 'settings'

    --skynet.newservice('debug_console', settings.console_port)

    --skynet.send(skynet.newservice('redisman'), 'lua', 'go')

    -- [[
    for i = 1, settings.client_n do
        local client  = skynet.newservice('client')
        local config = {}
        config.addr  = settings.login_addr
        config.open_id = settings.open_id .. i
        config.channel_id = settings.channel_id
        skynet.send(client, 'lua', 'start', config)
    end
    --]]
    skynet.exit()
end)

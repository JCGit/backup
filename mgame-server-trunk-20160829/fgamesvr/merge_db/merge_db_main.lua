
local skynet = require "skynet"
local cluster = require "cluster"
local utils = require "utils"
require "skynet.manager" -- import skynet.name


skynet.start(function()
    print("Transod Server Start")
    skynet.newservice("debug_console", 8333)
    
    --Ë³Ðò²»¿É´òÂÒ
    
    local svr
    svr = skynet.uniqueservice("merge_gift_code_svr")
    skynet.kill(svr)
    
    svr = skynet.uniqueservice("merge_uin_mapping_svr")
    skynet.kill(svr)
    
    svr = skynet.uniqueservice("merge_friend_svr")
    skynet.kill(svr)

    svr = skynet.uniqueservice("merge_mail_svr")
    skynet.kill(svr)

    svr = skynet.uniqueservice("merge_player_base_svr")
    skynet.kill(svr)

    svr = skynet.uniqueservice("merge_db_with_uin_svr")
    skynet.kill(svr)
    
    print("merge success")
end)
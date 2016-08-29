
local skynet = require "skynet"
local cluster = require "cluster"
local utils = require "utils"
require "skynet.manager" -- import skynet.name

local server_config = require "server_config"

skynet.start(function()
    print("Transod Server Start")
    skynet.newservice("debug_console", 8333)

    local mysql_service = skynet.uniqueservice("mysql_service")
    skynet.uniqueservice("redis_service")

    --skynet.uniqueservice("trans_pack_service")
    --skynet.uniqueservice("check_point")
    --skynet.uniqueservice("trans_stuff_service")
    --skynet.uniqueservice("trans_achievement_service")


    --skynet.call(mysql_service, "lua", "flush_queue")

    --assert(skynet.uniqueservice("hash2array"))
    --skynet.uniqueservice("insert_mysql_service")

    --skynet.uniqueservice("clear_tencent_data")
    --skynet.uniqueservice("save_db_data_to_file")
    --skynet.uniqueservice("flush_file_data_to_db")
    -- skynet.uniqueservice("flush_db_to_mysql")
    --assert(skynet.uniqueservice("array2hash"))
    assert(skynet.uniqueservice("cp_struct"))
    
    print("Transod success")
end)



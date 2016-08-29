local skynet = require "skynet"
local netpack = require "netpack"
local socketdriver = require "socketdriver"
local cluster = require "cluster"

local logger = require "logger"
local proto = require "proto"
local utils = require "utils"
local COMMON = require "common"
local server_config = require "server_config"
local lcrypt = require "lcrypt"
local hcrypt = require "hcrypt"

local login_gate_server = ...

local platform
local redis_service
local login_server
local gameserver_list
local gameserver_map
local logind_name = server_config.logind_conf.servername
local gray_basic    --灰度发布的配置
local gray_region_stat = {}  --灰度匹配区域
local stop_basic  --停服白名单相关配置
local stop_game_op_flag = false  --停服维护标识
local release_basic

local traceback = debug.traceback
local lcrypt_key = lcrypt.key

local function send_package(cmdid, pack, user_fd, bfkey)

    if not user_fd then return end

    local rl = #pack
    local m8 = rl % 8
    if m8 ~= 0 then
        pack = pack .. string.rep(0, 8 - m8)
    end

    local key = lcrypt_key('blowfish', 'cbc', bfkey, '12345678')
    local data = string.pack(">I2I2I2", #pack + 4, cmdid, rl)
    socketdriver.send(user_fd, data .. key:encrypt(pack))
end

local function auth_inner_handle(openid, token, params, platform)
    return true
end

local function auth_dianxin_handle(openid, token, params, platform)
    return true
end

local function auth_lenevo_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/lenovo/login", openid, token, params)
end


local function auth_baidu_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/baidu/login", openid, token, params)
end

local function auth_jidi_handle(openid, token, params, platform)
    return true
end

local function auth_xiaomi_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/xiaomi/login", openid, token, params)
end

local function auth_oppo_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/oppo/login", openid, token, params)
end

local function auth_qihoo_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/qihoo/login", openid, token, params)
end

local function auth_kupai_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/kupai/login", openid, token, params)
end

local function auth_huawei_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/huawei/login", openid, token, params)
end

-- 腾讯应用宝
local function auth_qq_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/qq/login", openid, token, params)
end

local function auth_wx_handle(openid, token, params, platform)
    return skynet.call(platform, "lua", "on_action", "/wx/login", openid, token, params)
end

local auth_channel_handler = {
    [0] = auth_inner_handle,    -- 内网登录 网络版
    [1] = auth_inner_handle,    -- 陌陌
    [2] = auth_qihoo_handle,    -- 奇虎360
    [3] = auth_inner_handle,    -- 苹果
    [4] = auth_xiaomi_handle,   -- 小米
    [5] = auth_lenevo_handle,   -- 联想
--    [501] = auth_lenevo_handle,   -- 乐商店
--    [502] = auth_lenevo_handle,   -- 游戏中心
--    [6] = auth_inner_handle,  -- 移动基地
    [601] = auth_inner_handle,  -- 移动基地
    [602] = auth_inner_handle,  -- 移动基地
    [7]   = auth_baidu_handle,  -- 百度
    [701] =auth_inner_handle,   -- 百度91
    [702] = auth_inner_handle,  -- 百度手机助手
    [703] = auth_inner_handle,  -- 百度多酷
    [704] = auth_inner_handle,  -- 百度贴吧
    [8] = auth_inner_handle,    -- 腾讯
    [801] = auth_inner_handle,  -- 腾讯应用宝
    [802] = auth_inner_handle,  -- 腾讯qq浏览器
    [803] = auth_inner_handle,  -- 腾讯游戏中心
    [804] = auth_inner_handle,  -- 腾讯应用市场
    [805] = auth_qq_handle,     -- 手机qq
    [806] = auth_wx_handle,     -- 微信
    [9] = auth_inner_handle,    -- uc
    [10] = auth_oppo_handle,   -- OPPO
    [11] = auth_inner_handle,   -- VIVO
    [12] = auth_huawei_handle,   -- 华为
    [13] = auth_kupai_handle,   -- 酷派
    [14] = auth_inner_handle,   -- 金立
    [15] = auth_inner_handle,   -- 安智
    [16] = auth_inner_handle,   -- 豌豆荚
    [17] = auth_inner_handle,   -- 拇指玩
    [18] = auth_inner_handle,   -- 爱奇艺PPS
    [19] = auth_inner_handle,   -- PPTV
    [20] = auth_inner_handle,   -- 搜狗
    [2001] = auth_inner_handle, -- 搜狗手机助手
    [2002] = auth_inner_handle, -- 搜狗输入法
    [2003] = auth_inner_handle, -- 搜狗浏览器
    [2004] = auth_inner_handle, -- 搜狗搜索
    [2005] = auth_inner_handle, -- 搜狗应用市场
    [2006] = auth_inner_handle, -- 搜狗游戏大厅
    [21] = auth_inner_handle,   -- 新浪
    [22] = auth_inner_handle,   -- 优酷土豆
    [23] = auth_inner_handle,   -- 电信爱游戏
    [26] = auth_inner_handle,   -- 美图
    [27] = auth_inner_handle,   -- 酷狗
    [28] = auth_inner_handle,   -- wifi万能钥匙
    [29] = auth_inner_handle,   -- 魅族
    [30] = auth_inner_handle,   -- 沃商店
    [31] = auth_inner_handle,   -- 掌盟
    [32] = auth_inner_handle,   -- 凯斯冒泡
    [3201] = auth_inner_handle, -- 凯斯冒泡市场
    [3202] = auth_inner_handle, -- 凯斯冒泡游戏
    [33] = auth_inner_handle,   -- 当乐
    [34] = auth_inner_handle,   -- 4399
    [35] = auth_inner_handle,   -- 应用汇
    [36] = auth_inner_handle,   -- 今日头条
    [37] = auth_inner_handle,   -- 迅雷
    [38] = auth_inner_handle,  -- 卓易市场
    [39] = auth_inner_handle,  -- 欧朋
    [40] = auth_inner_handle,  -- XY助手
    [41] = auth_inner_handle,  -- 乐视
    [42] = auth_inner_handle,  -- 上海益玩
    [4201] = auth_inner_handle,  --上海益玩统接推广渠道
    [4202] = auth_inner_handle,  -- 子渠道益玩app

    [100] = auth_inner_handle,  -- 自主运营
    [101] = auth_inner_handle,  -- M+
	[102] = auth_inner_handle,  -- fb_ios
    [103] = auth_inner_handle,  -- fb_android
    [999] = auth_inner_handle,  -- 内网登录 单机版
}

local function auth_handle(openid, token, channelId, params, platform)
    logger.tracef("login request, openid: %s, channelId: %d , platform: %s  ",
        openid, channelId, COMMON.platform[channelId] )
    local handle = auth_channel_handler[channelId]
    return handle(openid, token, params, platform)
end

local balance_basic
local gamed_range_array = {}
local gamed_index_array = {}
local function init_balance_map()
    balance_basic = skynet.call(login_server, "lua",  "get_schema_data")
    if not balance_basic then return end

    local gamed_array = {}
    for k,v in pairs(balance_basic.dispatch) do
        local num = tonumber(k)
        table.insert(gamed_array, num)
    end
    table.sort(gamed_array)

    local index = 1
    for k, v in ipairs(gamed_array) do
        gamed_range_array[index] = v
        gamed_index_array[index] = balance_basic.dispatch[tostring(v)]
        index = index + 1
    end
end

local function update_balance_map(balance_data)
    if not balance_data then return end

    balance_basic = balance_data

    local gamed_array = {}
    for k,v in pairs(balance_basic.dispatch) do
        local num = tonumber(k)
        table.insert(gamed_array, num)
    end
    table.sort(gamed_array)

    local gamed_range_bak_array = {}
    local gamed_index_bak_array = {}

    local index = 1
    for k, v in ipairs(gamed_array) do
        gamed_range_bak_array[index] = v
        gamed_index_bak_array[index] = balance_basic.dispatch[tostring(v)]
        index = index + 1
    end
    gamed_range_array = gamed_range_bak_array
    gamed_index_array = gamed_index_bak_array

end

local function init_gray_schema()
    gray_basic = skynet.call(login_server, "lua",  "get_gray_schema")
    if not gray_basic then return end

    if gray_basic then
        for k,v in ipairs(gray_basic.gray_region) do
            for begin_pos, end_pos in string.gmatch(v, "(%w+)=(%w+)") do
                gray_region_stat[tonumber(begin_pos)] = tonumber(end_pos)
            end
        end
    end
end

local function init_stop_schema()
    stop_basic = skynet.call(login_server, "lua",  "get_stop_schema")
    if not stop_basic then return end
    stop_game_op_flag = stop_basic.stop_flag
end

local function init_release_schema()
    release_basic = skynet.call(login_server, "lua", "get_release_schema")
end

local function init_gamed()
    gameserver_map = skynet.call(login_server, "lua", "get_alive_gamed_map")
    if not gameserver_map then print("gamed_map nil") return end

    for k,v in pairs(gameserver_map) do
        if not gameserver_list then gameserver_list = {} end
        table.insert(gameserver_list, k)
    end
end

local function get_random_alive_gamesever()
    if not gameserver_list or not next(gameserver_list)then
        return nil
    end

    local rand = math.random(1, #gameserver_list)
    return gameserver_list[rand]
end

local function match_openid_hash(openid, gray_match_ret)
    if not next(gamed_range_array) then
        return nil
    end

    local gameserver
    local gray_release
    local match_flag = false
    if not gray_match_ret then
        local hash_key =  hcrypt.string_hash(openid)
        local hash_value = hash_key % balance_basic.base
        logger.tracef("hash_key:%d , hash_value:%d ", hash_key, hash_value)
        --二分查找hash_value在那个节点
        local index = utils.binary_region_search(1, #gamed_range_array, hash_value, gamed_range_array)
        if index == -1 then
            --对于没有找到节点的玩家，就是设置默认值为第一节点
            gameserver = gamed_index_array[1]
        else
            gameserver = gamed_index_array[index]
            --如果选择的gameserver不是存活的，就在当前存活的gameserver中随机选取一个gameserver
        end


        if gameserver_map and gameserver_map[gameserver] then
            match_flag = true
        end
        if not match_flag then
            gameserver = get_random_alive_gamesever()
            if gameserver then match_flag = true end
        end
    else
        logger.trace("gray update")
        --gameserver = "gamed1"  --Todo:先写死一个灰度发布服的名字，以后要通过配置来区分
        gameserver = gray_basic.gameserver
        gray_release = gray_basic.gray_version --每次灰度都应该更新
        match_flag = true   --灰度时，默认为true
    end
    if not match_flag then gameserver = nil end
    return gameserver, gray_release
end

--灰度发布名单匹配
local function match_gray_list(openid)
    local ret = false

    if gray_basic.gray_list[openid] then
        ret = true
    else
        local hash_key =  hcrypt.string_hash(openid)
        local hash_value = hash_key % gray_basic.base
        local max_begin = 0
        for k,v in pairs(gray_region_stat) do
            if hash_value > k and max_begin < k then
                max_begin = k
            end
        end

        if max_begin > 0 then
            ret = true
        end
    end

    return ret
end

--停服白名单匹配
local function match_stop_game_list(openid)
    local ret = false
    if not stop_basic then return ret end

    if stop_basic.stop_list[openid] then
        ret = true
    end

    return ret
end

local function do_login(fd, msg, sz, bfkey, platform)
    local ret = false
    --  联想1 金山2 基地3 以后每接一个递增一个
    local decode = proto.parse_proto_req(COMMON.MESSAGE.CS_CMD_LOGIN, msg, sz)
    local openid = decode.OpenId       -- 第三方uid
    local channelId = decode.ChannelId -- 平台类型
    local token = decode.Token         -- token验证
    local params = decode.Params       -- 透传参数
    local secret_key = decode.SecretKey
    local gamed_name = decode.GameServerName
    local version = decode.Version

    if openid == "" then
        openid = skynet.call(redis_service, "lua", "id_handler", "openid_max_id")
        assert(openid, openid)
        openid = tostring(openid)
    end

    --  第三方平台认证
    local uin, ret, content
    logger.trace(openid, token, channelId, params, gamed_name)
    local auth_result, Uid, auth_token = auth_handle(openid, token, channelId, params, platform)
    if auth_result == true then

        --到gameserver取openid对应的uin, 同时生成secret用于本次的通信
        if Uid then openid = Uid logger.trace("openid:", openid, "|", Uid) end --这个是针对联想，openid是验证之后获取的

        local gamed_node
        local gameserver
        local release_test_flag = false
        if release_basic and release_basic.release_flag and version and version == release_basic.release_version then
            release_test_flag = true
        end

        local gray_release
        local stop_game_flag
        if not release_test_flag then
            --在停服状态下，如果玩家在白名单中，就进行正常流程处理，如果不在，就跳转到服务器状态页面

            if stop_game_op_flag then
                --这一块可根据openid,也可根据uin进行匹配
                local stopgame_ret = match_stop_game_list(openid)
                stop_game_flag = stopgame_ret and 0 or 1
            end

            --灰度发布相关逻辑
            local gray_match_ret = false
            if gray_basic and gray_basic.gray_release_flag then
                gray_match_ret = match_gray_list(openid)
                gray_release = 0  --灰度测试中，正常用户要收到这个字段，来到达不更新的目的
            end

            local need_gray_release
            gameserver, need_gray_release = match_openid_hash(openid, gray_match_ret)
            if need_gray_release then
                gray_release = need_gray_release
            end

            if gameserver then
                --Notice:暂时不做gameserver的存活验证
                gamed_node = utils.query_nodename(gameserver)
            end

            logger.tracef("normal gamed_name:%s, gamed_node:%s ", gameserver, gamed_node)
        else
            gameserver = "test_gamed"
            gamed_node = release_basic.gamed_node
            logger.tracef("release_test gamed_name:%s, gamed_node:%s ", gameserver, gamed_node)
        end

        if gamed_node then
            ret, uin = cluster.call(gamed_node, ".gamed", "login", openid, channelId, secret_key, bfkey, logind_name)

            --缓存一致性处理
            local node_data = skynet.call(redis_service, "lua", "hget_obj", "player_node", uin)
            if node_data and  node_data.servername ~= gameserver then
                local player_gamed_node = utils.query_nodename(node_data.servername)
                --玩家的记录节点和当前节点不一致时，触发踢号机制（这里没有区分agent存活情况，直接发送踢号请求）
                xpcall(cluster.call, traceback, player_gamed_node, ".gamed", "destory_agent", uin)
            end

            local gamed_addr
            if not release_test_flag then
                if gameserver_map[gameserver] then
                    gamed_addr = gameserver_map[gameserver].addr
                end
            else
                gamed_addr = release_basic.gamed_addr
            end

            if gamed_addr then
                logger.trace("(loginserver|do_login): ret:", ret, " uin:", uin, "channelId: ", channelId, "gamed_addr: ", gamed_addr)
                if ret == 0 then
                    local data = { Uin = uin, GameServerAddr = gamed_addr, OpenId = openid, AuthToken = auth_token,
                        GrayRelease = gray_release, StopGameFlag = stop_game_flag }
                    content = proto.pack_proto_resp(COMMON.MESSAGE.CS_CMD_LOGIN, data)
                else
                    logger.trace('ret is not 0')
                    local data = { ErrCode = ret }
                    content = proto.pack_error_resp(COMMON.MESSAGE.CS_CMD_LOGIN, data)
                end
            else
                --Todo:目前二个地方处理这个，应当放到一处
                --登陆异常，gameserver获取uin失败导致的
                logger.trace('first gamed node error')
                local data = { ErrCode = COMMON.ERRORCODE.LOGIN_GAMEID_NOT_EXIST }
                content = proto.pack_error_resp(COMMON.MESSAGE.CS_CMD_LOGIN, data)
            end
        else
            --登陆异常，gameserver获取uin失败导致的
            logger.trace('gamed node error')
            local data = { ErrCode = COMMON.ERRORCODE.LOGIN_GAMEID_NOT_EXIST }
            content = proto.pack_error_resp(COMMON.MESSAGE.CS_CMD_LOGIN, data)
        end
    else
        --返回错误码
        logger.trace('auth failed')
        ret = COMMON.ERRORCODE.ACCOUNT_AUTH_FAIL
        local data = {
            ErrCode = ret
        }
        content = proto.pack_error_resp(COMMON.MESSAGE.CS_CMD_LOGIN, data)
    end

    send_package(COMMON.MESSAGE.CS_CMD_LOGIN, content, fd, bfkey)
    return ret, openid, uin
end

---------------------------加密-----------------------
local rsa      = lcrypt.rsa
local rsa_keys = require 'rsa_keys' -- server rsa keys

local rsa_key = rsa.create()
local r = rsa.import(rsa_key, lcrypt.base64_decode(rsa_keys.private_key))
if not r then
    logger.trace('load rsa key failed')
end

local function process_request_pkey(fd, cmd)
    local res = { pkey_id = 1, pkey = rsa_keys.public_key }
    local pack = proto.pack_proto_resp(cmd, res)
    socketdriver.send(fd, string.pack(">I2>I2", #pack + 2, cmd) .. pack)
end

local function process_send_client_key(fd, cmd, content, len)
    local data = proto.parse_proto_req(cmd, content, len)
    local e_client_key = data.client_key

    local client_key = rsa.decrypt(rsa_key, e_client_key)

    if type(client_key) == 'string' and #client_key == 24 then
        -- 成功解密且长度正确
        local s_key = lcrypt.random(8)
        local key = client_key .. s_key

        local bfkey = lcrypt.key('blowfish', 'cbc', key, '12345678')
        local payload = bfkey:encrypt('alice go')

        local res = { ok = true, s_key = s_key, payload = payload }

        skynet.call(login_gate_server, 'lua', "update_fd_key", fd, key)

        local pack = proto.pack_proto_resp(cmd, res)
        local msg_header = string.pack(">I2I2", #pack + 2, cmd)
        socketdriver.send(fd, msg_header .. pack)

    else
        logger.trace('client key error! ', type(client_key))
        local res = { ok = false, s_key = '', payload = '' }
        local pack = proto.pack_proto_resp(cmd, res)
        local msg_header = string.pack(">I2I2", #pack + 2, cmd)
        socketdriver.send(fd, msg_header .. pack)
    end
end

----------------- CMD命令-------------------------
local CMD = {}
function CMD.handle_login_check(fd, msg, sz, bfkey, platform)
    if bfkey then
        local cmd, content, len = proto.parse_prototype(msg, sz, bfkey)
        if cmd == COMMON.MESSAGE.CS_CMD_LOGIN then

            local ret, openid, uin = do_login(fd, content, len, bfkey, platform)

            if ret == 0 then
                logger.trace("login success :" .. openid .. " : " .. uin)
            else
                logger.trace("login fail openid:" .. openid)
            end
        end
    else
        local cmd, content, len = proto.parse_prototype(msg, sz)
        if cmd == COMMON.MESSAGE.CS_CMD_REQUEST_PKEY then
            process_request_pkey(fd, cmd)
        elseif cmd == COMMON.MESSAGE.CS_CMD_SEND_CLIENT_KEY then
            process_send_client_key(fd, cmd, content, len)
        end
    end
end

function CMD.update_balance_config(balance_data)
    update_balance_map(balance_data)
end

function CMD.update_gamed_list(gamed_list)
    gameserver_list = gamed_list
end

function CMD.update_gamed_map(gamed_map)
    gameserver_map = gamed_map
    local gamed_list = {}
    for k,v in pairs(gamed_map) do
        table.insert(gamed_list, k)
    end
    gameserver_list = gamed_list
end

--同步灰度配置，并生成灰度区域数据
function CMD.sync_gray_config(gray_data)
    gray_basic = gray_data
    if gray_basic then
        for k,v in ipairs(gray_basic.gray_region) do
            for begin_pos, end_pos in string.gmatch(v, "(%w+)=(%w+)") do
                gray_region_stat[tonumber(begin_pos)] = tonumber(end_pos)
            end
        end
    end
    logger.trace("sync_gray_config success")
end

function CMD.sync_stop_config(stop_data)
    stop_basic = stop_data
    if stop_basic then
        stop_game_op_flag = stop_basic.stop_flag
    end
end

function CMD.update_release_schema(release_schema)
    release_basic = release_schema
end

local function init_openidcreater()
    local openid = skynet.call(redis_service, "lua", "get", "openid_max_id")
    if not openid then
        skynet.call(redis_service, "lua", "set", "openid_max_id",
            server_config.id_conf.account_max_id)
    end
end

--主启动函数
skynet.start(function()
    proto.load_proto()
    
    --platform = skynet.queryservice("platform")
    redis_service = skynet.queryservice("redis_service")
    login_server = skynet.queryservice("login_server")

    init_openidcreater()
    init_balance_map()
    init_gray_schema()
    init_stop_schema()
    init_gamed()
    init_release_schema()


    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s) id : %d", command, skynet.self())
            return
        end
        local ok, ret = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end

        return skynet.retpack(ret)
    end)
end)

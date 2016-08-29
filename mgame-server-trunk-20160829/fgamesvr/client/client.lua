local skynet = require 'skynet'
local socket = require 'socket'

local protobuf  = require 'protobuf'
local utils = require "utils"

local pb_encode = protobuf.encode
local pb_decode = protobuf.decode
do
    local file = io.open("proto-bin/ClientSvrMsg.pb", "rb")
    protobuf.register(file:read "*a")
    file:close()
end

local stats_service

local lcrypt     = require 'lcrypt'
local lcrypt_key = lcrypt.key

local COMMON = require 'common'
local MESSAGE = COMMON.MESSAGE

local settings = require 'settings'


local client = {
    pk_file = 'client/server_pk_der.txt', -- server pkey缓存路径
    key     = '',                         -- 通信用的blowfish key
}

local protomap = COMMON.protomap


local ANSI_COLOR_RED    = '\x1b[31m'
local ANSI_COLOR_GREEN  = '\x1b[32m'
local ANSI_COLOR_YELLOW = '\x1b[33m'
local ANSI_COLOR_BLUE   = '\x1b[34m'
local ANSI_COLOR_RESET  = '\x1b[0m'



local function send_package(sid, msgid, data, key)
    print(ANSI_COLOR_YELLOW .. '\n--------')
    print('send msg', msgid, ANSI_COLOR_RESET)
    local stat = { [protomap[msgid].Req] = data }
    local buf = pb_encode('fgame.C2ServerMsg', stat)

    if key then
        local rl = #buf
        local m8 = rl % 8
        if m8 ~= 0 then buf = buf .. string.rep(0, 8 - m8) end

        local k = lcrypt_key('blowfish', 'cbc', key, '12345678')
        local hd = string.pack('>I2I2I2I2', #buf + 10, msgid, 0, rl)
        socket.write(sid, hd .. k:encrypt(buf) .. '1234')
    else
        local hd = string.pack('>I2I2I2I2', #buf + 10, msgid, 0, 0)
        socket.write(sid, hd .. buf .. '1234')
    end
end


local function unpack_msg(msgid, buf, key)
    print(ANSI_COLOR_GREEN .. 'recv msg', msgid, ANSI_COLOR_RESET)
    if key then
        local rl = string.unpack('>I2', buf)
        local k = lcrypt_key('blowfish', 'cbc', key, '12345678')
        buf = k:decrypt(buf:sub(3, -1)):sub(1, rl)
    end
    local msg = pb_decode('fgame.S2ClientMsg', buf)
    if msg.Error_Msg and msg.Error_Msg.ErrCode ~= 0 then
        return false, msg.Error_Msg.ErrCode
    else
        return msg[protomap[msgid].Resp]
    end
end

function client:on_gslb_msg(msgid, buf)
    print(ANSI_COLOR_GREEN .. msgid .. ANSI_COLOR_RESET)
    local msg, err_code = unpack_msg(msgid, buf)
    if msg then
        if msgid == MESSAGE.CS_CMD_GETSERVER then
            client.game_server_addr = msg.GameServer
            client.game_server_name = msg.GameServerName
            client:connect_login(msg.LoginServer)
        end
    else
        print('RETURN error, code: ', err_code)
    end
end

local function gslb_reader(sid, client)
    local hd = socket.read(sid, 4)
    if hd then
        local len, msgid = string.unpack('>I2I2', hd)
        local buf = socket.read(sid, len - 2)
        if buf then
            client:on_gslb_msg(msgid, buf)
        end
    end
    socket.close(sid)
end

local function login_reader(sid, client)
    --socket.lock(sid)
    while true do
        local hd = socket.read(sid, 4)
        if not hd then break end
        local len, msgid = string.unpack('>I2I2', hd)
        local buf = socket.read(sid, len - 2)
        if not buf then break end
        if client:on_login_msg(msgid, buf) then
            break
        end
    end
    print('login socket read done')
    --socket.unlock(sid)
end

function client:on_login_msg(msgid, buf)
    print(ANSI_COLOR_GREEN .. msgid .. ANSI_COLOR_RESET)
    if msgid == MESSAGE.CS_CMD_REQUEST_PKEY then
        local msg, err_code = unpack_msg(msgid, buf)
        if msg then
            local cmd_fmt = 'echo "%s" | openssl rsa %s -out %s'
            local args    = '-pubin -inform PEM -outform DER'
            os.execute(cmd_fmt:format(msg.pkey, args, client.pk_file))
            client:handshake_key()
        else
            print('RETURN error, code: ', err_code)
        end
    elseif msgid == MESSAGE.CS_CMD_SEND_CLIENT_KEY then
        local msg, err_code = unpack_msg(msgid, buf)
        if msg then
            if msg.ok then
                client.key = client.key .. msg.s_key
                local k = lcrypt_key('blowfish', 'cbc', client.key, '12345678')
                if k:decrypt(msg.payload) == 'alice go' then
                    local login_req = {
                        OpenId         = client.open_id,
                        ChannelId      = client.channel_id, -- 测试用
                        Version  = client.version,
                    }
                    utils.var_dump(login_req)
                    send_package(client.sid, MESSAGE.CS_CMD_LOGIN, login_req, client.key)
                end
            else
                print(ANSI_COLOR_RED .. 'handshake failed!' .. ANSI_COLOR_RESET)
            end
        else
            print('RETURN error, code: ', err_code)
        end
    elseif msgid == MESSAGE.CS_CMD_LOGIN then
        local msg, err_code = unpack_msg(msgid, buf, client.key)
        if msg then
            client.game_server_addr = msg.GameServerAddr
            client.uin = msg.Uin
            socket.close(client.sid)
            utils.var_dump(msg)
            --skynet.fork(client.connect_game, client)
            client:connect_game()
            return true
        else
            print('RETURN error, code: ', err_code)
        end
    end
end

function client:handshake_key(pkf)
    if not pkf then
        pkf = io.open(client.pk_file, 'rb')
    end

    local rsa = lcrypt.rsa
    local rsa_key = rsa.create()
    assert(rsa.import(rsa_key, pkf:read('a')))
    pkf:close()
    client.key = lcrypt.random(24)
    local send_key_req = {
        client_key = rsa.encrypt(rsa_key, client.key)
    }
    rsa.destroy(rsa_key)
    send_package(client.sid, MESSAGE.CS_CMD_SEND_CLIENT_KEY, send_key_req)
end

function client:connect_login(addr)
    local ip, port = addr:gmatch('(.+):(.+)')()
    print(ip, " : ", port)
    local sid = socket.open(ip, tonumber(port))
    skynet.fork(login_reader, sid, client)
    client.sid = sid

    local pkf = io.open(client.pk_file, 'rb')
    if pkf then
        client:handshake_key(pkf)
    else
        local get_pk_req = { dummy = '', not_used = 1 }
        send_package(sid, MESSAGE.CS_CMD_REQUEST_PKEY, get_pk_req)
    end
end


local function game_socket_reader(sid, client)
    socket.lock(sid)
    while true do
        local hd = socket.read(sid, 4)
        if not hd then break end
        local len, msgid = string.unpack('>I2I2', hd)
        local buf = socket.read(sid, len - 2)
        if not buf then break end
        client:on_game_msg(msgid, buf)
    end
    print('game socket read done')
    socket.unlock(sid)
end

function client:connect_game()
    local ip, port = self.game_server_addr:gmatch('(.+):(.+)')()
    local sid = socket.open(ip, tonumber(port))
    skynet.fork(game_socket_reader, sid, client)
    client.sid = sid

    local enter_req = {
        Uin    = client.uin,
        OpenId = client.open_id,
        Retry  = 0,
    }
    -- 发给游戏服务器的消息，只有第一个不是加密的
    send_package(sid, MESSAGE.CS_CMD_ENTER_SERVER, enter_req)

    client:send_test_msg()
end

function client:send_msg(msgid, msg)
    send_package(self.sid, msgid, msg, self.key)
end

-- 测试消息在这里添加
function client:send_test_msg()
    --self:send_msg(MESSAGE.CS_CMD_HEART_BEAT, {})
    print(ANSI_COLOR_GREEN .. "after 600 secs then send client msg" .. ANSI_COLOR_RESET)
    skynet.sleep(6 * 100)
    local list = {
        MESSAGE.CS_CMD_BATTLE_SCORE, {},
        --MESSAGE.CS_CMD_HEART_BEAT, {},
        --MESSAGE.CS_CMD_GET_MAIL, {GetNum = 0},
    }
    self:send_msg_list(list)
end

-- 一次发送一组消息：
-- @list 要依次批量发送的消息列表，格式：
-- 消息id， 消息表，……
-- 必须注意，表的长度必须是偶数
-- TODO: 如有需要，可以修改结构，支持设置一个可选的回调函数
function client:send_msg_list(list)
    assert(#list % 2 == 0)

    self.b_mode     = true
    self.b_msg_list = list
    self.b_index    = 1

    local i = self.b_index
    client:send_msg(list[i], list[i + 1])
end

-- 收到消息在这里处理
function client:on_game_msg(msgid, buf)
    print(ANSI_COLOR_GREEN .. msgid .. ANSI_COLOR_RESET)
    local msg, err_code = unpack_msg(msgid, buf, client.key)
    if msg then
        -- 批量发消息模式，不做其他处理
        if self.b_mode and msgid == self.b_msg_list[self.b_index] then
            self.b_index = self.b_index + 2
            local l = self.b_msg_list
            local i = self.b_index
            if i < # self.b_msg_list then
                client:send_msg(l[i], l[i + 1])
            else
                self.b_mode = false
            end
        else
            -- 常规消息处理
        end

        utils.var_dump(msg)
    else
        print(ANSI_COLOR_RED .. 'error code: ', err_code .. ANSI_COLOR_RESET)
    end
end

local CMD = {}

-- 主启动函数
function CMD.start(config)
    client.open_id = config.open_id
    client.channel_id = config.channel_id
    client:connect_login(config.addr)
end

skynet.start(function ()
    local traceback = debug.traceback

    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if f then
            local ok, ret = xpcall(f, traceback, ...)
            --if ok then return skynet.retpack(ret) end
        end
        --return skynet.ret()
    end)
end)


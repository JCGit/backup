local skynet   = require "skynet"
local protobuf = require "protobuf"
local netpack  = require "netpack"
local print_r  = require 'print_r'

local logger   = require "logger"

local utils    = require "utils"
local COMMON   = require "common"


local protomap = COMMON.protomap
local proto = {}

function proto.load_proto()
    local file = io.open("./proto-bin/ClientSvrMsg.pb", "rb")
    protobuf.register(file:read "*a")
    file:close()
end

local c2server_msg = "fgame.C2ServerMsg"
local s2client_msg = "fgame.S2ClientMsg"

function proto.parse_proto_req(cmdid, msg, sz)
    if #msg == 0 then
        error("msg can't 0 size")
    end

    local decode = protobuf.decode(c2server_msg, msg, sz)

    local name = protomap[cmdid].Req
    assert(name ~= '', 'error cmdid req name')

    if type(decode) == "boolean" then
        error("protobuf decode failed msg:" .. name)
    end

    assert(decode[name] ~= nil)

    local is_dev = false
    if is_dev then
        logger.trace('\x1b[33m\n' .. logger.dump(decode[name]) .. '\x1b[0m')
    end
    return decode[name]
end

function proto.parse_proto_resp(cmdid, msg, sz)
    if #msg == 0 then
        return error("msg can't 0 size")
    end
    local decode = protobuf.decode(s2client_msg, msg, sz)

    local name = protomap[cmdid].Resp
    if name == '' then
        name = protomap[cmdid].Req
        assert(name ~= '')
    end
    assert(decode[name] ~= nil)
    return decode[name]
end

function proto.pack_proto_resp(cmdid, data)
    return protobuf.encode(s2client_msg, {[protomap[cmdid].Resp] = data})
end

--针对服务器主动发报处理函数
function proto.pack_proto_s2c_req(cmdid, data)
    return protobuf.encode(s2client_msg, {[protomap[cmdid].Req] = data})
end

function proto.pack_proto_c2s_req(cmdid, data)
    return protobuf.encode(c2server_msg, {[protomap[cmdid].Req] = data})
end

function proto.pack_error_resp(cmdid, data)
    return protobuf.encode(s2client_msg, {Error_Msg = data})
end

--Use:打印proto结构属性
local function show_message_attr(message)
    for _, v in ipairs(message) do
        print(v.name)
        for _, v in ipairs(v.field) do
            print("\t" .. v.name .. " [" .. v.number .. "] " .. v.label)
        end
    end
end

local lcrypt = require 'lcrypt'
local lcrypt_key = lcrypt.key

function proto.parse_prototype(msg, sz, bfkey)
    local len = sz - 10 -- I2 I2 I2 content I4 (校验码)
    assert(len > 0)

    -- 这里可以做优化，减少一次Lua与C的字符串传递开销
    local netdata = skynet.tostring(msg, sz)
    local cmdid, _v, rl, content = string.unpack(">I2I2I2c" .. len, netdata)

    if bfkey then
        assert(#content % 8 == 0)

        local c
        if type(bfkey) == 'string' then
            local k = lcrypt_key('blowfish', 'cbc', bfkey, '12345678')
            c = k:decrypt(content)
        else --elseif type(bfkey) == 'userdata' then
            bfkey.iv = '12345678'
            c = bfkey:decrypt(content)
        end

        assert(rl <= #c)
        return cmdid, string.sub(c, 1, rl), rl
    else
        return cmdid, content, len
    end
end

return proto

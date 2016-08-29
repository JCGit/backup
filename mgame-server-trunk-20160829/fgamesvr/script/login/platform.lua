local CMD = {}
local utils = require "utils"
local skynet = require "skynet"
local httpc = require "http.httpc"
local http_service
local crypt = require "crypt"
local platform_setting = require "platform_setting"
local json = require "cjson"
local logger = require "logger"
local md5 = require "md5"

local function get_sign(params)
    local key_test = {}
    for i in pairs(params) do
        table.insert(key_test, i) --提取test1中的键值插入到key_test表中
    end

    local sort_param
    table.sort(key_test)
    for _,v in ipairs(key_test) do
        if v ~= "signature" then
            if not sort_param then
                sort_param = v .. "=" .. params[v] .. "&"
            else
                sort_param = sort_param .. v .. "=" .. params[v] .. "&"
            end
        end
    end
    return string.sub(sort_param,0,string.len(sort_param) - 1)
end

local function get_sort_param(params)
    local key_test = {}
    for i in pairs(params) do
        table.insert(key_test, i) --提取test1中的键值插入到key_test表中
    end

    local sort_param
    table.sort(key_test)
    for _,v in ipairs(key_test) do
        if v ~= "signature" then
            if not sort_param then
                sort_param = v .. "=" .. params[v] .. "&"
            else
                sort_param = sort_param .. v .. "=" .. params[v] .. "&"
            end
        end
    end
    return string.sub(sort_param,0,string.len(sort_param) - 1)
end

local function sort_param(params)
    local key_test = {}
    for i in pairs(params) do
        table.insert(key_test, i) --提取test1中的键值插入到key_test表中
    end

    local sort_param = {}
    table.sort(key_test)
    for _,v in ipairs(key_test) do
       sort_param.v = params.v
    end
    return sort_param
end
--小米登录
local function xiaomi_login(uid, session)
    if not session then
        logger.trace("xiaomi login param session is null")
        return false
    end
    local param = {
        ["appId"] = platform_setting.xiaomi.AppID,
        ["session"] = session,
        ["uid"] = uid,
    }
    local sort_prama = get_sort_param(param)
    local signature = crypt.hexencode(crypt.hmac_sha1(platform_setting.xiaomi.AppSecret, sort_prama))
    local content = sort_prama .. "&signature=" .. signature
    local recvheader = {}
    local status, body = httpc.request("GET", platform_setting.xiaomi.IP, platform_setting.xiaomi.LoginUrl, recvheader, {}, content)

   logger.tracef("status: " .. status .. "--- body: " .. body)
    local data = json.decode(body)
    local errorCode = tonumber(data.errcode)
    if errorCode == 200 then
        return true
    end
    return false
end

--lenovo登录
local function lenovo_login(uid, lpsust)
    if not lpsust or "" == lpsust then
        logger.tracef("%s lenovo login param lpsust is null ", uid)
        return false
    end
    local param = {
        ["realm"] = platform_setting.lenovo.AppID,
        ["lpsust"] = lpsust,
    }
    local content = get_sign(param)
    local recvheader = {}
    local status, body = httpc.request("GET", platform_setting.lenovo.IP, platform_setting.lenovo.LoginUrl, recvheader, {}, content)
    if body then
        logger.tracef("body: %s", body)
    end
    if status == 200 then
        local pos = string.find(body, "<AccountID>")
        local end_pos = string.find(body, "</AccountID>")
        local Uid
        if pos and end_pos then
            Uid = string.sub(body, pos+11, end_pos-1)
        end
        if Uid then
            return true, Uid
        else
            return false
        end
    end
    return false
end

local function generate_sign(key, data)
    return crypt.base64encode(crypt.hmac_sha1(key, data))
end


local function oppo_login(openid, access_token, param)
    -- 顺序一定要保持如下，否则签名不通过
    local base_str = 'oauthConsumerKey=' .. utils.urlencode(platform_setting.oppo.AppKey) .. '&oauthToken=' .. utils.urlencode(access_token) .. '&oauthSignatureMethod=' .. utils.urlencode('HMAC-SHA1') .. '&oauthTimestamp=' .. utils.urlencode(os.time()) .. '&oauthNonce=' .. utils.urlencode(os.time() + math.random(0,9)) .. '&oauthVersion=' .. utils.urlencode('1.0') .. '&'
    local sign = generate_sign(platform_setting.oppo.AppSecret .. '&', base_str)

    local header = {
        ['param'] = base_str .. '&',
        ['oauthSignature'] = utils.urlencode(sign),
    }
    local recvheader = {}
    local content = 'fileId=' .. utils.urlencode(openid) .. '&token=' .. utils.urlencode(access_token)

    local status, body = httpc.request("GET", platform_setting.oppo.IP, platform_setting.oppo.LoginUrl, recvheader, header, content)

    if status == 200 and tonumber(json.decode(body).resultCode) == 200 then
        return true
    end
    return false
end



-- 360登录
local function qihoo_login(openid, token)
    local content = 'access_token=' .. token
    local recvheader = {}

    -- https的需要nginx做正向代理
    local status, body = httpc.request("GET", platform_setting.nginx, platform_setting.qihoo.LoginUrl, recvheader, nil, content)

    logger.trace("qihoo_login status is %s body is %s", tostring(status), tostring(body))

    -- 有可能是502错误
    if status ~= 200 then
        return false
    end

    local res = json.decode(body)
    if not res.error_code then
        return true, res.id
    end
    return false
end


local function kupai_login(openid, token)
    local content = 'grant_type=authorization_code&client_id=' .. platform_setting.kupai.AppId .. '&redirect_uri=' .. platform_setting.kupai.AppKey .. '&client_secret=' .. platform_setting.kupai.AppKey .. '&code=' .. token
    local recvheader = {}

    -- https的需要nginx做正向代理
    local status, body = httpc.request("GET", platform_setting.nginx, platform_setting.kupai.LoginUrl, recvheader, nil, content)

    logger.trace("kupai_login status is %s body is %s", tostring(status), tostring(body))

    -- 有可能是502错误
    if status ~= 200 then
        return false
    end

    local res = json.decode(body)
    if not res.error then
        return true, res.openid, res.access_token
    end

    return false
end

--baidu登录
local function baidu_login(uid, access_token)
    if not access_token or "" == access_token then
        logger.tracef("%s baidu login param access_token is null ", uid)
        return false
    end

    --生成Sign
    --$Sign = md5($this->AppId.$accessToken.$this->Secretkey);
    local sign = md5.sumhexa(platform_setting.baidu.AppID .. access_token .. platform_setting.baidu.AppSecret)
    local content = "AppId=" .. platform_setting.baidu.AppID .. "&AccessToken=" .. access_token .. "&Sign=" .. sign
    local form = {}

    form["Sign"] = sign
    form["AccessToken"] = access_token
    form["AppID"] = platform_setting.baidu.AppID

    local recvheader = {}
    local status, body = httpc.post(platform_setting.baidu.ip, platform_setting.baidu.loginurl, form, recvheader)

    logger.trace("status: " .. status .. "  --- body: " .. access_token)

    local data = json.decode(body)
    if not data then
        logger.trace("http response data nil")
        return false
    end

    logger.trace("status: " .. status .. " ResultCode: " .. data.ResultCode)

    if data.ResultCode == 1 then
        return true
    end
    return false
end

local function huawei_login(openid, token)
    token = utils.urlencode(token)
    local content = "nsp_svc=OpenUP.User.getInfo" .. "&nsp_ts=" .. os.time() .. "&access_token=" .. token
    print(content)
    local recvheader = {}
    -- https的需要nginx做正向代理
    local status, body = httpc.request("GET", platform_setting.nginx, platform_setting.huawei.Url, recvheader, nil, content)

    if status ~= 200 then
        return false;
    end
    local res = json.decode(body)
    if not res.error then
        return res.userID == openid, res.userID
    end
    return false
end


--[[
登录分扣扣和微信
扣扣:http://wiki.dev.4g.qq.com/v2/ZH_CN/router/index.html#!qq.md#2.1
微信:http://wiki.dev.4g.qq.com/v2/ZH_CN/router/index.html#!wx.md#3.1auth

扣扣:/auth/verify_login
微信:/auth/check_token
]]
local function qq_login(openid, token, platform)
    local recvheader = {}
    local now = os.time()

    local appid = platform_setting.midas.QQAppID
    local sign = md5.sumhexa(platform_setting.midas.QQAppKey .. now)
    local url = platform_setting.midas.QQLoginUrl

    local body = "{\n".."\"appid\": " .. appid .. ",\n" .. "\"openid\": \"" .. openid .. "\",\n" .. "\"openkey\": \"" .. token .. "\"\n" .. "}" 
    local uri = url .. '?timestamp=' .. now .. '&appid=' .. appid .. '&sig=' .. sign .. '&openid='.. openid .. '&encode=1'

    local header = {
        ["content-type"] = "application/x-www-form-urlencoded",
    }
    local status, body  = httpc.request("POST", platform_setting.midas.IP, uri, recvheader, header, body)
    logger.trace("qq_login status is %s body is %s", tostring(status), tostring(body))

    if status ~= 200 then
        return false
    end
    local res = json.decode(body)
    if tonumber(res.ret) == 0 then
        return true
    end
    return false
end


local function wx_login(openid, token, platform)
    local recvheader = {}
    local now = os.time()
    local appid = platform_setting.midas.WXAppID
    local sign = md5.sumhexa(platform_setting.midas.WXAppKey .. now)
    local url = platform_setting.midas.WXLoginUrl
    local body = "{\n" .. "\"accessToken\": \"" .. token .. "\",\n" .. "\"openid\": \"" .. openid .. "\"\n" .. "}"

    local uri = url .. '?timestamp=' .. now .. '&appid=' .. appid .. '&sig=' .. sign .. '&openid='.. openid ..'&encode=1'

    local header = {
        ["content-type"] = " application/x-www-form-urlencoded",
    }
    local status, body  = httpc.request("POST", platform_setting.midas.IP, uri, recvheader, header, body)
    logger.trace("wx_login status is %s body is %s", tostring(status), tostring(body))

    if status ~= 200 then
        return false
    end
    local res = json.decode(body)
    if tonumber(res.ret) == 0 then
        return true
    end
    return false
end

-- 上面定义function  函数名字规则 平台名_请求action
-- 注意on_platform的键
local on_platform = {
    ["lenovo_login"] = lenovo_login,
    ["xiaomi_login"] = xiaomi_login,
    ["baidu_login"] = baidu_login,
    ["oppo_login"] = oppo_login,
    ["qihoo_login"] = qihoo_login,
    ["kupai_login"] = kupai_login,
    ["huawei_login"] = huawei_login,
    ["qq_login"] = qq_login,
    ["wx_login"] = wx_login,
}

--处理请求
function CMD.on_action(path, ...)
    logger.trace("path=> " .. path)
    local platf_action = utils.split(path, "/")
    local platf = platf_action[2] -- 请求平台
    local action = platf_action[3] -- 请求操作  login or callback
    if platf and action then
        local index = platf .. "_" .. action
        return on_platform[index](...)
    end
end

local traceback = debug.traceback
skynet.start(function()
    logger.trace("platform start...")
    skynet.dispatch("lua", function(_, _, command, ...)
        local f = CMD[command]
        if not f then
            logger.warningf("unhandled message(%s)", command)
            return skynet.ret()
        end
        local ok, ret, ret2, ret3 = xpcall(f, traceback, ...)
        if not ok then
            logger.warningf("handle message(%s) failed : %s", command, ret)
            return skynet.ret()
        end
        skynet.retpack(ret, ret2, ret3)
    end)
end)

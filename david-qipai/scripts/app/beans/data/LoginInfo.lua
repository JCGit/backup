local LoginInfo = class("LoginInfo")
local TestDataModule = require("app.test.TestDataModule")
local DailyBonusView = require("app.beans.dailybonus.view.DailyBonusView")
local RegisterRewardView = require("app.beans.register.view.RegisterRewardView")
function LoginInfo:ctor()
end
function LoginInfo:requestLogin(params, successCallback, failCallback)
  local requestData = {}
  if params.loginType == nil then
    failCallback()
    return
  elseif params.loginType == "facebook" then
    requestData.accessToken = params.accessToken
  end
  requestData.deviceKey = g.native:getDeviceKey()
  if DEBUG_MEMORY_STATUS then
    requestData.deviceKey = g.loginTempDevice or g.native:getDeviceKey()
  end
  requestData.type = params.loginType
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("users", "login", requestData), function(data)
    local jsonData = json.decode(data)
    if jsonData ~= nil and jsonData.ret and jsonData.ret == 0 or jsonData ~= nil and jsonData.ret == 1 then
      jsonData.info.uid = tonumber(jsonData.info.uid)
      g.vars.user = jsonData.info
      self:_loginSuccessJobs()
      successCallback(jsonData)
    else
      failCallback(jsonData)
    end
  end, function()
    failCallback()
  end)
end
function LoginInfo:_loginSuccessJobs()
  g.activityCenter:requestActiviteList()
  g.tasksInfo:refreshData()
  g.settingsInfo:copyOnlineSetting()
  g.storage:setString(g.vars.storageKeys.loginType, g.vars.user.loginType)
  g.dataLogger:loginWithUid(tostring(g.vars.user.uid))
  local level = g.levelInfo:getLevelByExperience(tonumber(g.vars.user.exp))
  g.dataLogger:bindUserLevel(level)
  g.dataLogger:logAllLevel(level, g.levelInfo:getMaxLevel())
  if g.vars.user.loginRewards then
    local loginReward = tonumber(g.vars.user.loginRewards.loginReward or 0)
    local signReward = tonumber(g.vars.user.loginRewards.signReward or 0)
    local vipReward = tonumber(g.vars.user.loginRewards.vipReward or 0)
    local facebookReward = tonumber(g.vars.user.loginRewards.facebookReward or 0)
    g.dataLogger:logChipsProduce(loginReward + signReward, "登录奖励")
    g.dataLogger:logChipsProduce(facebookReward, "Facebook奖励")
  end
  if g.vars.user.newbee == 1 then
    g.dataLogger:logChipsProduce(tonumber(g.vars.user.chips), "注册初始奖励")
  end
  if device.platform == "android" then
    g.push:register(g.vars.user.uid, function(isSuccess, data)
      print("xg registered, uid=", g.vars.user.uid)
    end)
  elseif device.platform == "ios" then
    g.push:register(g.vars.user.uid, function(isSuccess, data)
      if isSuccess then
        print("xg registered, uid=", g.vars.user.uid)
        g.native:bindXingeAcount(tostring(g.vars.user.uid or ""))
      end
    end)
  end
end
function LoginInfo:showRewardView()
  if not g.vars.user.hasShowReward then
    g.vars.user.hasShowReward = true
    if self:_hasRegisterReward() then
      RegisterRewardView.new():showPanel()
    elseif self:_hasLoginReward() then
      DailyBonusView.new():showPanel()
    end
  end
end
function LoginInfo:_hasRegisterReward()
  if g.vars.user.registerRewards ~= nil and tonumber(g.vars.user.registerRewards.days) > 0 then
    return true
  else
    return false
  end
end
function LoginInfo:_hasLoginReward()
  if g.vars.user.loginRewards ~= nil and tonumber(g.vars.user.loginRewards.ret) == 0 then
    return true
  else
    return false
  end
end
function LoginInfo:getRegisterRewards(successCallback, failCallback)
  local requestData = {}
  requestData.uid = g.vars.user.uid
  g.httpRequest:postMethod(g.requestParamsFactory:createRequest("register_rewards", "get", requestData), function(data)
    local jsonData = json.decode(data)
    logger:debug(g.util.inspect(data))
    if jsonData and jsonData.ret and jsonData.ret == 0 then
      local chipsReward = 0
      for i, item in ipairs(jsonData.info) do
        if item.type == "chips" then
          chipsReward = chipsReward + tonumber(item.quantity or 0)
        end
      end
      successCallback(chipsReward)
    else
      failCallback(tonumber(jsonData.ret))
    end
  end, function()
    failCallback()
  end)
end
return LoginInfo

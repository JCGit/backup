require("app.thirdparty.luabit.bit")
cc.net = require("framework.cc.net.init")
cc.utils = require("framework.cc.utils.init")
scheduler = require(cc.PACKAGE_NAME .. ".scheduler")
g = g or {}
g.config = require("app.config.init")
g.util = require("app.util.init")
g.ui = require("app.ui.init")
g.httpRequest = require("app.core.http.HttpRequest").new()
g.requestParamsFactory = require("app.core.http.RequestParamsFactory").new()
g.downloader = require("app.core.downloader.Downloader").new()
g.fileManager = require("app.core.manager.FileSystemManager").new()
g.imageManager = require("app.core.manager.ImageManager").new()
g.memoryManager = require("app.core.manager.MemoryManager").new()
g.eventCenter = require("app.core.event.EventCenter").new()
g.lang = require("app.core.manager.LangManager")
g.font = require("app.styles.font")
g.actionsManager = require("app.core.manager.ActionsManager").new()
g.animationsManager = require("app.core.manager.AnimationsManager").new()
local LightStorage = require("app.core.storage.LightStorage")
g.storage = LightStorage.new()
if device.platform == "android" then
  g.native = import(".core.system.NativeAndroid").new()
  g.facebook = import(".core.social.FacebookAndroid").new()
  g.push = import(".core.social.XinGePushAndroid").new()
  g.push:register("", function(isSuccess, data)
    print("xg registered")
  end)
elseif device.platform == "ios" then
  g.native = import(".core.system.NativeiPhone").new()
  g.facebook = import(".core.social.FacebookiOS").new()
  g.push = import(".core.social.ApplePush").new()
  g.push:register("", function(isSuccess, data)
    if isSuccess then
      print("xg registered success")
    else
      print("xg registered fail!")
    end
  end)
else
  g.native = import(".core.system.NativeSimulator")
  g.facebook = import(".core.social.FacebookSimulator").new()
end
local SystemControlInfo = require("app.beans.data.SystemControlInfo")
g.systemControlInfo = SystemControlInfo.new()
local LoginInfo = require("app.beans.data.LoginInfo")
g.loginInfo = LoginInfo.new()
local LevelInfo = require("app.beans.data.LevelInfo")
g.levelInfo = LevelInfo.new()
local GiftInfo = require("app.beans.data.GiftInfo")
g.giftInfo = GiftInfo.new()
local FriendsInfo = require("app.beans.data.FriendsInfo")
g.friendsInfo = FriendsInfo.new()
local MessageInfo = require("app.beans.data.MessageInfo")
g.messageInfo = MessageInfo.new()
local ChartsInfo = require("app.beans.data.ChartsInfo")
g.chartsInfo = ChartsInfo.new()
local TablesInfo = require("app.beans.data.TablesInfo")
g.tablesInfo = TablesInfo.new()
local TasksInfo = require("app.beans.data.TasksInfo")
g.tasksInfo = TasksInfo.new()
local JewelBoxInfo = require("app.beans.data.JewelBoxInfo")
g.jewelBoxInfo = JewelBoxInfo.new()
local RouletteInfo = require("app.beans.data.RouletteInfo")
g.rouletteInfo = RouletteInfo.new()
local SettingsInfo = require("app.beans.data.SettingsInfo")
g.settingsInfo = SettingsInfo.new()
local InviteInfo = require("app.beans.data.InviteInfo")
g.inviteInfo = InviteInfo.new()
g.playTogetherInfo = nil
local ActivityCenter = require("app.beans.data.ActivityCenter")
g.activityCenter = ActivityCenter.new()
local PaymentInfo = require("app.beans.data.PaymentInfo")
g.paymentInfo = PaymentInfo.new()
local FeedbackInfo = require("app.beans.data.FeedbackInfo")
g.feedbackInfo = FeedbackInfo.new()
g.vars = require("app.beans.data.GlobalVars").new()
g.obj = g.obj or {}
g.audio = require("app.core.manager.AudioManager").new()
g.dataLogger = require("app.beans.analysis.DataAnalysis").new()
g.scaleWidth = display.width / 960
g.scaleHeight = display.height / 640
g.vars.bgScale = 1
if display.width > 1140 and display.height == 640 then
  g.vars.bgScale = display.width / 1140
elseif display.width == 960 and display.height > 640 then
  g.vars.bgScale = display.height / 640
end
return g

local InAppBillingPlugin = import(".plugin.InAppBillingPlugin")
local InAppPurchasePlugin = import(".plugin.InAppPurchasePlugin")
local BluePayPlugin = import(".plugin.BluePayPlugin")
local PayChannel = require("app.beans.payment.model.PayChannel")
local PayPluginManager = class("PayPluginManager")
function PayPluginManager:ctor()
  self.enabledPlugins = {}
  self.plugins = {}
  if device.platform == "android" then
    self.enabledPlugins[PayChannel.IN_APP_BILLING] = InAppBillingPlugin
    self.enabledPlugins[PayChannel.EASY_2_PAY] = Easy2PayPlugin
    self.enabledPlugins[PayChannel.UNITRY_GAMERTOPUP] = UnitryPlugin
    self.enabledPlugins[PayChannel.MOL_TRUE_MONEY] = MolPlugin
    self.enabledPlugins[PayChannel.MOL_Z_CARD] = MolPlugin
    self.enabledPlugins[PayChannel.MOL_POINTS_CARD] = MolPlugin
    self.enabledPlugins[PayChannel.MOL_12_CALL] = MolPlugin
    self.enabledPlugins[PayChannel.BLUE_PAY] = BluePayPlugin
    self.enabledPlugins[PayChannel.BLUE_PAY_TRUE_MONEY] = BluePayPlugin
    self.enabledPlugins[PayChannel.BLUE_PAY_12_CALL] = BluePayPlugin
  elseif device.platform == "ios" then
    self.enabledPlugins[PayChannel.IN_APP_BILLING] = InAppBillingPlugin
  elseif device.platform == "windows" or device.platform == "mac" then
    self.enabledPlugins[PayChannel.IN_APP_BILLING] = InAppBillingPlugin
    self.enabledPlugins[PayChannel.IN_APP_PURCHASE] = InAppPurchasePlugin
    self.enabledPlugins[PayChannel.EASY_2_PAY] = Easy2PayPlugin
    self.enabledPlugins[PayChannel.UNITRY_GAMERTOPUP] = UnitryPlugin
    self.enabledPlugins[PayChannel.MOL_TRUE_MONEY] = MolPlugin
    self.enabledPlugins[PayChannel.MOL_Z_CARD] = MolPlugin
    self.enabledPlugins[PayChannel.MOL_POINTS_CARD] = MolPlugin
    self.enabledPlugins[PayChannel.MOL_12_CALL] = MolPlugin
    self.enabledPlugins[PayChannel.BLUE_PAY] = BluePayPlugin
    self.enabledPlugins[PayChannel.BLUE_PAY_TRUE_MONEY] = BluePayPlugin
    self.enabledPlugins[PayChannel.BLUE_PAY_12_CALL] = BluePayPlugin
  end
end
function PayPluginManager:isPluginEnabled(channel)
  return not not self.enabledPlugins[channel]
end
function PayPluginManager:initialize(payConfig)
  for i, config in ipairs(payConfig) do
    local PayPluginClass = self.enabledPlugins[config.channel]
    local payPluginInstance = self.plugins[config.channel]
    if PayPluginClass then
      if not payPluginInstance then
        payPluginInstance = PayPluginClass.new()
        self.plugins[config.channel] = payPluginInstance
      end
      payPluginInstance:initialize(config)
    end
  end
end
function PayPluginManager:getPlugin(channel)
  return self.plugins[channel]
end
function PayPluginManager:autoDispose()
  for channel, plugin in pairs(self.plugins) do
    plugin:autoDispose()
  end
end
return PayPluginManager

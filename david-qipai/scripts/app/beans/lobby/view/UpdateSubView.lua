local LoginTable = import(".LoginTable")
local LoginClickView = import(".LoginClickView")
local LobbyConfig = import(".LobbyConfig")
local LogoView = import(".LogoView")
local UpdateSubView = class("UpdateSubView", function()
  return display.newNode()
end)
UpdateSubView.PRO_WIDTH = 400
UpdateSubView.PRO_HEIGHT = 12
function UpdateSubView:ctor()
  self:setNodeEventEnabled(true)
  self._tableNode = display.newNode():addTo(self)
  local table = LoginTable.new():addTo(self._tableNode, 0)
  local tableWidth = table:getTableWidth()
  if tableWidth < display.width * 0.75 then
    self._visibleTableWidth = tableWidth
  else
    self._visibleTableWidth = display.width * 0.75
  end
  table:pos(display.width * 0.5 + tableWidth * 0.5 - self._visibleTableWidth, 0)
  self._logoBatchNode = LogoView.new():pos(LobbyConfig.LOGO_POS_X, 0):addTo(self._tableNode)
  self:createProgressBar()
  self:createCopyRight()
end
function UpdateSubView:createProgressBar()
  self._progressNode = display.newNode():addTo(self):hide()
  UpdateSubView.PRO_WIDTH = UpdateSubView.PRO_WIDTH
  self.progressBG = display.newScale9Sprite("#login_update_progressbar_bg.png")
  self.progressBG:setContentSize(CCSize(UpdateSubView.PRO_WIDTH, UpdateSubView.PRO_HEIGHT))
  self.progressBG:addTo(self._progressNode)
  self.progressBG:pos(LobbyConfig.PROGRESS_POS_X, LobbyConfig.PROGRESS_POS_Y)
  self.imgClip = CCClippingNode:create():addTo(self._progressNode)
  self.imgClip:pos(LobbyConfig.PROGRESS_POS_X, LobbyConfig.PROGRESS_POS_Y)
  self.stencil = display.newScale9Sprite("#login_update_progressbar.png", -UpdateSubView.PRO_WIDTH, 0, cc.size(UpdateSubView.PRO_WIDTH, UpdateSubView.PRO_HEIGHT))
  self.imgClip:setAlphaThreshold(0)
  self.imgClip:setStencil(self.stencil)
  self.progress = display.newScale9Sprite("#login_update_progressbar.png")
  self.progress:addTo(self.imgClip)
  self.progress:setContentSize(CCSize(UpdateSubView.PRO_WIDTH, UpdateSubView.PRO_HEIGHT))
  self.progressLabel = ui.newTTFLabel({
    text = g.lang:getContent("update.updating"),
    size = 21,
    color = ccc3(255, 255, 255),
    align = ui.TEXT_ALIGN_LEFT
  })
  self.progressLabel:align(display.CENTER, LobbyConfig.PROGRESS_POS_X, LobbyConfig.PROGRESS_POS_Y + 30)
  self.progressLabel:addTo(self._progressNode)
end
function UpdateSubView:createCopyRight()
  self._copyrightLabel = ui.newTTFLabel({
    text = g.lang:getContent("lobby.copyright"),
    color = ccc3(0, 109, 50),
    size = 18,
    align = ui.TEXT_ALIGN_CENTER
  }):align(display.RIGHT_BOTTOM, display.cx - 4, -(display.cy - 4)):addTo(self)
end
function UpdateSubView:onCleanup()
  if self.progressHandler then
    scheduler.unscheduleGlobal(self.progressHandler)
    self.progressHandler = nil
  end
end
function UpdateSubView:setTipsLabel(msg, dotCount)
  for i = 1, dotCount do
    msg = msg .. "."
  end
  self.progressLabel:setVisible(true)
  self.progressLabel:setString(msg)
end
function UpdateSubView:refreshProgressAnimation()
  self.stepX = self.stepX or -UpdateSubView.PRO_WIDTH
  self.stepX = self.stepX + 2
  local keyX = -UpdateSubView.PRO_WIDTH * 0.4
  local realX = -UpdateSubView.PRO_WIDTH * (1 - self.progressValue)
  if keyX < realX or realX > self.stepX then
    self.stencil:setPositionX(realX)
    if self.progressValue == 1 then
      scheduler.unscheduleGlobal(self.progressHandler)
      self.progressHandler = nil
    end
  elseif keyX > self.stepX then
    self.stencil:setPositionX(self.stepX)
  end
end
function UpdateSubView:startUpdateService(enterAppDirectoryBlock, restartAppBlock)
  local updateManager = require("app.core.updater.UpdateManager").new()
  print("UpdateSubView:startUpdateService(enterAppDirectoryBlock, restartAppBlock)")
  self.enterAppDirectoryBlock = enterAppDirectoryBlock
  self.restartAppBlock = restartAppBlock
  updateManager.updateDidStartDownFlistCallback = handler(self, self.updateDidStartDownFlistCallback)
  updateManager.updateFinishedCallback = handler(self, self.updateFinishedCallback)
  updateManager.updateProgressCallback = handler(self, self.updateProgressCallback)
  updateManager:startUp()
end
function UpdateSubView:updateDidStartDownFlistCallback()
  self._progressNode:show()
  self.progressValue = 0
  self:setTipsLabel(g.lang:getContent("update.updating"), 3)
  if self.progressHandler == nil then
    self.progressHandler = scheduler.scheduleUpdateGlobal(handler(self, self.refreshProgressAnimation))
  end
end
function UpdateSubView:updateProgressCallback(updatedSize, totalUpdateSize)
  self.progressValue = updatedSize / totalUpdateSize
  print("start----update progress-----")
  print(updatedSize, totalUpdateSize)
  print(self.progressValue)
  if self.progressValue > 1 then
    self.progressValue = 1
  end
end
function UpdateSubView:updateFinishedCallback(enterAppDirectly)
  print("------update finished---------")
  if enterAppDirectly then
    print("本轮无更新的zip文件,直接进入app")
    self.enterAppDirectoryBlock()
  else
    print("本轮有更新的zip文件,需要重load-app")
    self.restartAppBlock()
  end
end
return UpdateSubView

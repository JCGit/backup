local FAQItem = import(".FAQItem")
local LevelItem = class("LevelItem", FAQItem)
function LevelItem:ctor()
  LevelItem.super.ctor(self)
end
function LevelItem:foldContent()
  if self._isFold then
    self._isFold = false
  else
    self._isFold = true
  end
  self:unscheduleUpdate()
  self:scheduleUpdate()
  if self._index == 1 and not self._isExpItemInitialized then
    self._bottomBg:show()
    self._isExpItemInitialized = true
    self:createExpItem(self._data)
  elseif self._index == 2 and not self._isLevelItemInitialized then
    self._bottomBg:show()
    self._isLevelItemInitialized = true
    self:createLevelItem(self._data)
  end
end
function LevelItem:onDataSet(isModified, data)
  if isModified then
    self._data = data
    self._titleLabel:setString(self._data.subject)
  end
end
function LevelItem:createExpItem(data)
  self.answerLabel:show()
  local answerLabelMarginBottom = 17
  local linePadding = 26
  self.answerLabel:setString(data.content)
  local answerLabelSize = self.answerLabel:getContentSize()
  local answerLabelMarginLeft = self.titleMarginLeft
  local h = linePadding + answerLabelSize.height
  self._bottomBg:size(self.contentWidth - 8, h)
  self._bottomBg:setPositionY(h * 0.5 - FAQItem.HEIGHT * 0.5 + 16)
  self.answerLabel:setPosition(ccp(-FAQItem.WIDTH / 2 + answerLabelMarginLeft, h * 0.5 - FAQItem.HEIGHT * 0.5 + answerLabelMarginBottom))
end
function LevelItem:createLevelItem(data)
  local levelList = data.content
  local marginTop = 15
  local h = marginTop
  local size = FAQItem.ANSWER_SIZE
  local color = FAQItem.ANSWER_COLOR
  local labelPadding = 30
  local labelHeight = 0
  local items = {}
  local dividerWidth = 692
  local dividerHeight = 2
  local padding = 5
  local bottomContainer = display.newNode():addTo(self._bottomPanel)
  for i = 1, #levelList + 1 do
    local levelData
    if i == 1 then
      levelData = data.header
    else
      levelData = levelList[i - 1]
    end
    local levelText
    if i == 1 then
      levelText = levelData.level
    else
      levelText = string.gsub(g.lang:getContent("common.level"), "{level}", i - 1)
    end
    local levelLabel = ui.newTTFLabel({
      text = levelText,
      size = size,
      color = color,
      align = ui.TEXT_ALIGN_LEFT
    }):addTo(bottomContainer)
    levelLabel:setAnchorPoint(ccp(0, 0.5))
    local titleLabel = ui.newTTFLabel({
      text = levelData.title,
      size = size,
      color = color,
      align = ui.TEXT_ALIGN_LEFT
    }):addTo(bottomContainer)
    titleLabel:setAnchorPoint(ccp(0, 0.5))
    local expLabel = ui.newTTFLabel({
      text = levelData.experience,
      size = size,
      color = color,
      align = ui.TEXT_ALIGN_LEFT
    }):addTo(bottomContainer)
    expLabel:setAnchorPoint(ccp(0, 0.5))
    local rewardText
    if i == 1 then
      rewardText = levelData.reward
    elseif 0 < levelData.chips and 0 < levelData.magic then
      rewardText = string.gsub(g.lang:getContent("help.levelRewardChipsAndMagic"), "{chips}", g.util.functions.formatBigInt(levelData.chips))
      rewardText = string.gsub(rewardText, "{magic}", levelData.magic)
    elseif 0 < levelData.chips then
      rewardText = string.gsub(g.lang:getContent("help.levelRewardChips"), "{chips}", g.util.functions.formatBigInt(levelData.chips))
    else
      rewardText = ""
    end
    local levelRewardLabel = ui.newTTFLabel({
      text = rewardText,
      size = size,
      color = color,
      align = ui.TEXT_ALIGN_RIGHT
    }):addTo(bottomContainer)
    levelRewardLabel:setAnchorPoint(ccp(1, 0.5))
    local divider = display.newScale9Sprite("#common_divider.png"):addTo(self._bottomPanel):size(dividerWidth, dividerHeight)
    local levelLabelSize = levelLabel:getContentSize()
    labelHeight = levelLabelSize.height
    h = h + labelHeight + dividerHeight + padding * 2
    items[i] = {
      levelLabel,
      titleLabel,
      expLabel,
      levelRewardLabel,
      divider
    }
  end
  self._bottomBg:size(self.contentWidth - 8, h)
  local bottomBgMarginTop = 16
  self._bottomBg:setPositionY(h * 0.5 - FAQItem.HEIGHT * 0.5 + bottomBgMarginTop)
  local y = h - FAQItem.HEIGHT * 0.5 - marginTop + (bottomBgMarginTop + dividerHeight)
  local pokerTitleMarginLeft = 65
  local sumExpMarginLeft = 350
  for _, item in ipairs(items) do
    item[1]:pos(-FAQItem.WIDTH / 2 + labelPadding, y - labelHeight / 2 - padding)
    item[2]:pos(-FAQItem.WIDTH / 2 + labelPadding + pokerTitleMarginLeft, y - labelHeight / 2 - padding)
    item[3]:pos(-FAQItem.WIDTH / 2 + labelPadding + sumExpMarginLeft, y - labelHeight / 2 - padding)
    item[4]:pos(FAQItem.WIDTH / 2 - labelPadding, y - labelHeight / 2 - padding)
    item[5]:pos(0, y - labelHeight - dividerHeight / 2 - padding * 2)
    y = y - labelHeight - dividerHeight - padding * 2
  end
end
return LevelItem

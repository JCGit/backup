----------------------------------------------------------------------------------
--[[
FILE:			BattleResultPopPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	战斗结算页面弹出框
AUTHOR:			zhenhui
CREATED:		2014-09-10
--]]
----------------------------------------------------------------------------------

local thisPageName = "BattleResultPopPage"

local BossWipeResultConfg = {}
local option = {
ccbiFile = "SystemMessageBox01.ccbi",
handlerMap = {
onConfirmation 		= "onClose",
onClose 		= "onClose"
},
opcode = opcodes
};


local ResManager = require("ResManagerForLua");
local NewbieGuideManager = require("NewbieGuideManager")
local GameConfig = require("GameConfig");
local BattleResultPopPageBase = {}

local NodeHelper = require("NodeHelper");

local wordList = {}
local m_chanllengeName = ""
local m_battleResultMsg = {}
-----------------------------------------------
--BattleResultPopPageBase页面中的事件处理
----------------------------------------------
function BattleResultPopPageBase:onEnter(container)
	container:runAnimation("fadeOut")
	NodeHelper:initScrollView(container, "mContent", 1);	
	self:refreshPage(container);
	self:rebuildAllItem(container);
end

function BattleResultPopPageBase:onExecute(container)
end

function BattleResultPopPageBase:onExit(container)
	NodeHelper:deleteScrollView(container);
end
----------------------------------------------------------------

function BattleResultPopPageBase:refreshPage(container)
	--[[	local noticeStr = common:getLanguageString("@MailNotice", TodoStr);
	NodeHelper:setStringForLabel(container, {mMailPromptTex = noticeStr});--]]
end
----------------scrollview-------------------------
function BattleResultPopPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:prepareBossResult(container);
	self:buildItem(container);
end

function BattleResultPopPageBase:addAlignCenter(source)
	return FightLogConfig["P_AlignCenterPre"].content..source..FightLogConfig["P_AlignCenterAfter"].content
end

function BattleResultPopPageBase:prepareBossResult(container)
	local message = m_battleResultMsg	
	wordList = {}	
	if message.award == nil or message == nil  then
		CCLuaLog("Error in award , is nil");
		return
	end	
	local hintData = BattleResultPopPageBase:addAlignCenter(FightLogConfig["BattleResultTitle"].content)
	table.insert(wordList,hintData)	
	if message.fightResult == 0 then
		local loseData = BattleResultPopPageBase:addAlignCenter(FightLogConfig["Lose"].content)
		table.insert(wordList,loseData)		
		return
	end	
--[[	local mapCfg = ConfigManager:getMapCfg()	
	local mapStr = mapCfg[m_mapId]["name"]
	local mTitle = container:getVarLabelBMFont("mTitle")
	if mTitle~=nil then
		local newTitle = common:getLanguageString("@MapBossWipeResultTitle", mapStr);
		mTitle:setString(newTitle);
	end--]]
	local fightReport = ""
	if m_chanllengeName == nil then
		m_chanllengeName = ""
	end
	fightReport = BattleResultPopPageBase:addAlignCenter(FightLogConfig["FightBossResultReport"].content)
	local newFightReport = common:fill(fightReport,m_chanllengeName);		
	table.insert(wordList,newFightReport)	
	
	if message.award:HasField("exp") and message.award:HasField("coin")  then
		local fightReportExp = BattleResultPopPageBase:addAlignCenter(FightLogConfig["FightBossResultExpCoin"].content)
		local newFightReportExp = common:fill(fightReportExp,tostring(message.award.exp),tostring(message.award.coin));		
		table.insert(wordList,newFightReportExp)	
	end
	
	
	--如果有掉落
	if message.award:HasField("drop") then
		local drop = message.award.drop				
		--详细装备掉落情况
		for i=1,#drop.detailEquip  do
			local oneEquip = drop.detailEquip[i]
			local equipStr = nil
			local rewardStr = ""
			local resInfo = ResManager:getResInfoByTypeAndId(40000, oneEquip.itemId, oneEquip.count);
			local equipCfg = ConfigManager.getEquipCfg();
			local equipLevel = equipCfg[oneEquip.itemId]["level"]
			rewardStr = rewardStr .."LV"..equipLevel.." ".. resInfo.name .."*"..oneEquip.count.." ";
			if resInfo.quality == 1 then
				equipStr = FightLogConfig["WhiteDetailEquip"].content
			elseif resInfo.quality == 2 then
				equipStr = FightLogConfig["GreenDetailEquip"].content
			elseif resInfo.quality == 3 then
				equipStr = FightLogConfig["BlueDetailEquip"].content
			elseif resInfo.quality == 4 then
				equipStr = FightLogConfig["PurpleDetailEquip"].content
			elseif resInfo.quality == 5 then
				equipStr = FightLogConfig["OrangeDetailEquip"].content
			end
			equipStr = BattleResultPopPageBase:addAlignCenter(equipStr)
			local newEquipStr = common:fill(equipStr,rewardStr)
			table.insert(wordList,newEquipStr)
			--BattleFightHelper:AddCotentIntoSVByString(container,newEquipStr)
			if oneEquip.sellCoin>0 then
				local sellStr = BattleResultPopPageBase:addAlignCenter(FightLogConfig["SellEquipGot"].content)
				local sellStrDetail = common:fill(sellStr,tostring(oneEquip.sellCoin))
				table.insert(wordList,sellStrDetail)
				--BattleFightHelper:AddCotentIntoSVByString(container,sellStr)
			end
			
		end
		
		--物品掉落情况
		for i=1,#drop.item  do
			local oneEquip = drop.item[i]
			local equipStr = nil
			local rewardStr = ""
			local resInfo = ResManager:getResInfoByTypeAndId(oneEquip.itemType, oneEquip.itemId, oneEquip.itemCount);
			rewardStr = rewardStr .. resInfo.name .."*"..oneEquip.itemCount.." ";
			if resInfo.quality == 1 then
				equipStr = FightLogConfig["WhiteDetailEquip"].content
			elseif resInfo.quality == 2 then
				equipStr = FightLogConfig["GreenDetailEquip"].content
			elseif resInfo.quality == 3 then
				equipStr = FightLogConfig["BlueDetailEquip"].content
			elseif resInfo.quality == 4 then
				equipStr = FightLogConfig["PurpleDetailEquip"].content
			elseif resInfo.quality == 5 then
				equipStr = FightLogConfig["OrangeDetailEquip"].content
			end
			equipStr = BattleResultPopPageBase:addAlignCenter(equipStr)
			local newEquipStr = common:fill(equipStr,rewardStr)
			table.insert(wordList,newEquipStr)						
		end
				
		--宝箱掉落情况
		for i=1,#drop.treasure  do
			local oneBox = drop.treasure[i]
			if oneBox.state == Battle_pb.TREASURE_DISCARD  then
				--如果没有钥匙，逃走了
				local boxStr = nil
				if oneBox.itemId == 40001 then
					boxStr = FightLogConfig["FindTongBoxFail"].content
				elseif oneBox.itemId == 40002 then
					boxStr = FightLogConfig["FindYinBoxFail"].content
				elseif oneBox.itemId == 40003 then
					boxStr = FightLogConfig["FindJinBoxFail"].content		
				else 
					CCLuaLog("Error in box's itemid")		
				end	
				boxStr = BattleResultPopPageBase:addAlignCenter(boxStr)	
				local newBoxStr = common:fill(boxStr,tostring(oneBox.count))
				table.insert(wordList,newBoxStr)								
			else
				--如果有钥匙
				local boxStr = nil
				if oneBox.itemId == 40001 then
					boxStr = FightLogConfig["FindTongBoxWin"].content
				elseif oneBox.itemId == 40002 then
					boxStr = FightLogConfig["FindYinBoxWin"].content
				elseif oneBox.itemId == 40003 then
					boxStr = FightLogConfig["FindJinBoxWin"].content		
				else 
					CCLuaLog("Error in box's itemid")		
				end
				local itemStr = ""
				for i=1,#oneBox.item do
					local oneBoxItem = oneBox.item[i]					
					local resInfo = ResManager:getResInfoByTypeAndId(oneBoxItem.itemType, oneBoxItem.itemId, oneBoxItem.itemCount);
					 itemStr = itemStr..resInfo.name.."*"..oneBoxItem.itemCount.."" 
				end
				boxStr = BattleResultPopPageBase:addAlignCenter(boxStr)	
				local newBoxStr = common:fill(boxStr,itemStr)					
				table.insert(wordList,newBoxStr)
			end
			
			
		end	
		--[[if #drop.detailEquip ==0 and #drop.treasure ==0 then			
			local noDropStr = BattleResultPopPageBase:addAlignCenter(FightLogConfig["NoEquipInfo"].content)
			table.insert(wordList,noDropStr)
		end	--]]
	else
		--[[local noDropStr = BattleResultPopPageBase:addAlignCenter(FightLogConfig["NoEquipInfo"].content)
		table.insert(wordList,noDropStr)--]]
	end
end

function BattleResultPopPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end


function BattleResultPopPageBase:buildItem(container)
	--准备文字	
	
	container.mScrollViewRootNode:removeAllChildren();
	local currentPos = GameConfig.BattleResultGap
	local size = #wordList	
	for i = size, 1,-1 do
		local oneContent  =   wordList[i]
		if oneContent~=nil then
			local content = CCHTMLLabel:createWithString(oneContent,CCSize(600,200),"Helvetica");
			content:setPosition(ccp(20,currentPos));
			content:setTag(i);
			currentPos = currentPos + content:getContentSize().height + GameConfig.BattleResultGap;
			container.mScrollViewRootNode:addChild(content)
		end
	end
	currentPos = currentPos
	local bg = container:getVarScale9Sprite("mMsgBg")
	bg:setContentSize(CCSize(600,currentPos + 10));	
	container.mScrollView:setViewSize(CCSize(640,currentPos))
	container.mScrollView:setContentSize(CCSize(640,currentPos));	
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))
end

function BattleResultPopPageBase:onAnimationDone(container)
	local animationName=tostring(container:getCurAnimationDoneName())
	if animationName=="fadeOut" then	
		BattleResultPopPageBase:onClose(container)
	end
end

----------------click event------------------------
function BattleResultPopPageBase:onClose(container)
	PageManager.popPage(thisPageName);
	m_battleResultMsg = {}
	m_chanllengeName = nil
end

function BattleResultPopPage_setBossName(nextChanllengeName)
	m_chanllengeName = nextChanllengeName
end

function BattleResultPopPage_setBossResult(BattleDataSequence,nextChanllengeName)
	m_battleResultMsg = BattleDataSequence	
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local BattleResultPopPage = CommonPage.newSub(BattleResultPopPageBase, thisPageName, option);
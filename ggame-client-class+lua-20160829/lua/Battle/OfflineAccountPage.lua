----------------------------------------------------------------------------------
--[[
FILE:			OfflineAccountPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	能量之核: 探索、拼合、列表页面
AUTHOR:			hgs
CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------

local thisPageName = "OfflineAccountPage"

local OfflineAccountConfg = {}
local option = {
ccbiFile = "OfflineBattleAccountPopUp.ccbi",
handlerMap = {
onConfirmation 		= "onClose"
},
opcode = opcodes
};

local GameConfig = require("GameConfig");
local OfflineAccountPageBase = {}

local NodeHelper = require("NodeHelper");

local wordList = {}

-----------------------------------------------
--OfflineAccountPageBase页面中的事件处理
----------------------------------------------
function OfflineAccountPageBase:onEnter(container)
	
	NodeHelper:initScrollView(container, "mContent", 1);
	--OfflineAccountConfg = ConfigManager.getOfflineAccountCfg()
	self:refreshPage(container);
	self:rebuildAllItem(container);
end

function OfflineAccountPageBase:onExecute(container)
end

function OfflineAccountPageBase:onExit(container)
	NodeHelper:deleteScrollView(container);
end
----------------------------------------------------------------

function OfflineAccountPageBase:refreshPage(container)
	--[[	local noticeStr = common:getLanguageString("@MailNotice", TodoStr);
	NodeHelper:setStringForLabel(container, {mMailPromptTex = noticeStr});--]]
end
----------------scrollview-------------------------
function OfflineAccountPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:prepareWorld(container);
	self:buildItem(container);
end

function OfflineAccountPageBase:prepareWorld(container)
	local message = {}
	wordList = {}
	
	local BattleDataHelper = require("BattleDataHelper")
	local mTitle = container:getVarLabelBMFont("mTitle")
	if isOfflineFightReport then
		message = BattleDataHelper.BattleOfflineStatics.result;
		local titleStr=Language:getInstance():getString("@OfflineBattleReportTitle");
		mTitle:setString(titleStr);
	else
		
		message = BattleDataHelper.FastBattleStatics.result;
		local titleStr=Language:getInstance():getString("@FastBattleReportTitle");
		mTitle:setString(titleStr);
	end
	
	CCLuaLog(tostring(message))
	if message.useTime == nil or message.mapId ==nil or message == nil then
		CCLuaLog("Error in useTime , is nil");
		return
	end
	local useTime = common:secondToDateXXYY(message.useTime)
	local mapCfg = ConfigManager:getMapCfg()
	CCLuaLog(tostring(message.mapId))
	local mapStr = mapCfg[message.mapId]["name"]
	local fightReport = ""
	local fightTimeStr = tostring(message.winTimes + message.loseTimes )
	if message:HasField("fromLevel") and  message:HasField("upToLevel") then
		fightReport = FightLogConfig["FightReportForLevelUp"].content
		CCLuaLog(useTime..mapStr..fightTimeStr..tostring(message.winTimes)..tostring(message.loseTimes)..tostring(message.award.exp)..tostring(message.award.coin))
		local newFightReport = common:fill(fightReport,useTime,mapStr,fightTimeStr,tostring(message.winTimes),
		tostring(message.loseTimes),tostring(message.award.exp),tostring(message.fromLevel),tostring(message.upToLevel),tostring(message.award.coin));
		
		table.insert(wordList,newFightReport)
	else
		fightReport = FightLogConfig["FightReport"].content
		CCLuaLog(useTime..mapStr..fightTimeStr..tostring(message.winTimes)..tostring(message.loseTimes)..tostring(message.award.exp)..tostring(message.award.coin))
		local newFightReport = common:fill(fightReport,useTime,mapStr,fightTimeStr,tostring(message.winTimes),
		tostring(message.loseTimes),tostring(message.award.exp),tostring(message.award.coin));
		
		table.insert(wordList,newFightReport)
	end
	
	
	
	--如果有掉落
	if message.award:HasField("drop") then
		local drop = message.award.drop
		--装备掉落情况
		for i=1,#drop.equip  do
			local oneEquip = drop.equip[i]
			local equipStr = nil
			if oneEquip.quality == 1 then
				equipStr = FightLogConfig["WhiteEquip"].content
			elseif oneEquip.quality == 2 then
				equipStr = FightLogConfig["GreenEquip"].content
			elseif oneEquip.quality == 3 then
				equipStr = FightLogConfig["BlueEquip"].content
			elseif oneEquip.quality == 4 then
				equipStr = FightLogConfig["PurpleEquip"].content
			elseif oneEquip.quality == 5 then
				equipStr = FightLogConfig["OrangeEquip"].content
			end
			local newEquipStr = common:fill(equipStr,tostring(oneEquip.count))
			table.insert(wordList,newEquipStr)
			if oneEquip.sellCoin> 0 then
				local sellStr = common:fill(FightLogConfig["SellEquipGot"].content,tostring(oneEquip.sellCoin))
				table.insert(wordList,sellStr)
			end
						
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
					local ResManagerForLua = require("ResManagerForLua")
					local resInfo = ResManagerForLua:getResInfoByTypeAndId(oneBoxItem.itemType, oneBoxItem.itemId, oneBoxItem.itemCount);
					 itemStr = itemStr..resInfo.name.."*"..oneBoxItem.itemCount.."" 
				end
				local newBoxStr = common:fill(boxStr,itemStr)					
				table.insert(wordList,newBoxStr)
			end
			
			
		end	
		if #drop.equip ==0 and #drop.treasure ==0 then			
			local noDropStr = FightLogConfig["NoEquipInfo"].content
			table.insert(wordList,noDropStr)
		end	
	else
		local noDropStr = FightLogConfig["NoEquipInfo"].content
		table.insert(wordList,noDropStr)
	end
end

function OfflineAccountPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

--[[container.mScrollView = container:getVarScrollView(svName);
container:autoAdjustResizeScrollview(container.mScrollView);
container.mScrollViewRootNode = container.mScrollView:getContainer();--]]

function OfflineAccountPageBase:buildItem(container)
	--准备文字	
	
	container.mScrollViewRootNode:removeAllChildren();
	local currentPos = 0
	local size = #wordList
	for i = size, 1,-1 do
		local oneContent  =   wordList[i]
		if oneContent~=nil then
			local content = CCHTMLLabel:createWithString(oneContent,CCSize(530,200),"Helvetica");
			content:setPosition(ccp(0,currentPos));
			content:setTag(i);
			currentPos = currentPos + content:getContentSize().height + GameConfig.OfflineSlotWidth;
			container.mScrollViewRootNode:addChild(content)
		end
	end
	
	container.mScrollView:setContentSize(CCSize(640,currentPos));
	--container.mScrollView:setContentOffset(ccp(0,0));
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))
end

----------------click event------------------------
function OfflineAccountPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local OfflineAccountPage = CommonPage.newSub(OfflineAccountPageBase, thisPageName, option);
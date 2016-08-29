----------------------------------------------------------------------------------
--[[
FILE:			BossWipeResultPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	能量之核: 探索、拼合、列表页面
AUTHOR:			hgs
CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------

local thisPageName = "BossWipeResultPage"

local BossWipeResultConfg = {}
local option = {
ccbiFile = "OfflineBattleAccountPopUp.ccbi",
handlerMap = {
onConfirmation 		= "onClose",
onClose 		= "onClose"
},
opcode = opcodes
};

local ResManager = require("ResManagerForLua");

local GameConfig = require("GameConfig");
local BossWipeResultPageBase = {}

local NodeHelper = require("NodeHelper");

local wordList = {}

-----------------------------------------------
--BossWipeResultPageBase页面中的事件处理
----------------------------------------------
function BossWipeResultPageBase:onEnter(container)
	
	NodeHelper:initScrollView(container, "mContent", 1);
	--BossWipeResultConfg = ConfigManager.getBossWipeResultCfg()
	self:refreshPage(container);
	self:rebuildAllItem(container);
end

function BossWipeResultPageBase:onExecute(container)
end

function BossWipeResultPageBase:onExit(container)
	NodeHelper:deleteScrollView(container);
end
----------------------------------------------------------------

function BossWipeResultPageBase:refreshPage(container)
	--[[	local noticeStr = common:getLanguageString("@MailNotice", TodoStr);
	NodeHelper:setStringForLabel(container, {mMailPromptTex = noticeStr});--]]
end
----------------scrollview-------------------------
function BossWipeResultPageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:prepareWorld(container);
	self:buildItem(container);
end

function BossWipeResultPageBase:prepareWorld(container)
	local message = BossWipeMsg
	wordList = {}	
	if message.award == nil or message == nil then
		CCLuaLog("Error in award , is nil");
		return
	end		
	local mapCfg = ConfigManager:getMapCfg()	
	local mapStr = mapCfg[message.mapId]["name"]
	local mTitle = container:getVarLabelBMFont("mTitle")
	if mTitle~=nil then
		local newTitle = common:getLanguageString("@MapBossWipeResultTitle", mapStr);
		mTitle:setString(newTitle);
	end
	local fightReport = ""
	--local fightTimeStr = tostring(message.winTimes + message.loseTimes )
	if message:HasField("fromLevel") and  message:HasField("upToLevel") then
		fightReport = FightLogConfig["BossWipeWinReportLevelUp"].content		
		local newFightReport = common:fill(fightReport,tostring(message.award.exp),tostring(message.fromLevel),tostring(message.upToLevel),tostring(message.award.coin));
		
		table.insert(wordList,newFightReport)
	else
		fightReport = FightLogConfig["BossWipeWinReport"].content	
		local newFightReport = common:fill(fightReport,tostring(message.award.exp),tostring(message.award.coin));
		
		table.insert(wordList,newFightReport)
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
			local newEquipStr = common:fill(equipStr,rewardStr)
			table.insert(wordList,newEquipStr)
			--BattleFightHelper:AddCotentIntoSVByString(container,newEquipStr)
			if oneEquip.sellCoin>0 then
				local sellStr = common:fill(FightLogConfig["SellEquipGot"].content,tostring(oneEquip.sellCoin))
				table.insert(wordList,sellStr)
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
				local newBoxStr = common:fill(boxStr,itemStr)					
				table.insert(wordList,newBoxStr)
			end
			
			
		end	
		if #drop.detailEquip ==0 and #drop.treasure ==0 then			
			local noDropStr = FightLogConfig["NoEquipInfo"].content
			table.insert(wordList,noDropStr)
		end	
	else
		local noDropStr = FightLogConfig["NoEquipInfo"].content
		table.insert(wordList,noDropStr)
	end
end

function BossWipeResultPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

--[[container.mScrollView = container:getVarScrollView(svName);
container:autoAdjustResizeScrollview(container.mScrollView);
container.mScrollViewRootNode = container.mScrollView:getContainer();--]]

function BossWipeResultPageBase:buildItem(container)
	--准备文字	
	
	container.mScrollViewRootNode:removeAllChildren();
	local currentPos = 0
	local size = #wordList
	for i = size, 1,-1 do
		local oneContent  =   wordList[i]
		if oneContent~=nil then
			local content = CCHTMLLabel:createWithString(oneContent,CCSize(460,200),"Helvetica");
			content:setPosition(ccp(0,currentPos));
			content:setTag(i);
			currentPos = currentPos + content:getContentSize().height + GameConfig.bossWipeSlotWidth ;
			container.mScrollViewRootNode:addChild(content)
		end
	end
	
	container.mScrollView:setContentSize(CCSize(640,currentPos));
	--container.mScrollView:setContentOffset(ccp(0,0));
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - container.mScrollView:getContentSize().height * container.mScrollView:getScaleY()))
end

----------------click event------------------------
function BossWipeResultPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local BossWipeResultPage = CommonPage.newSub(BossWipeResultPageBase, thisPageName, option);
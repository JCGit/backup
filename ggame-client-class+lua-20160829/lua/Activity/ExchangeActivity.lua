----------------------------------------------------------------------------------
--[[
	FILE:			ExchangeActivity.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			hgs
	CREATED:		2014-08-23
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local UserItemManager = require("UserItemManager");
local NodeHelper = require("NodeHelper");
local thisPageName = "ExchangeActivity"

local COUNT_EXCHANGE_TYPE = 4;
local COUNT_CONSUMEITEM_PER_LINE = 5;

local opcodes = {
	WORDS_EXCHANGE_INFO_C 	= HP_pb.WORDS_EXCHANGE_INFO_C,
	WORDS_EXCHANGE_INFO_S	= HP_pb.WORDS_EXCHANGE_INFO_S,
	WORDS_EXCHANGE_C		= HP_pb.WORDS_EXCHANGE_C,
	WORDS_EXCHANGE_S		= HP_pb.WORDS_EXCHANGE_S
};
local option = {
	ccbiFile = "Act_Mid-autumnPage.ccbi",
	handlerMap = {
		onReturnButton	= "onBack",
		onHelp			= "onHelp"
	},
	opcode = opcodes
};

for i = 1, COUNT_EXCHANGE_TYPE do
	option.handlerMap[string.format("onGift%02d", i)] = "changeExchangeType"
end

--活动基本信息，结构基本与协议pb Message相同
local thisActivityInfo = {
	id				= 7,
	remainTime 		= 0,
	consumeCfg		= {},
	rewardCfg		= {},
	rewardIds 		= {},
	leftExchangeTimes	= {},
	canExchange		= {},
	exchangeId		= 1,
	optionIds		= {},
	selectedId		= nil
};
thisActivityInfo.timerName = "Activity_" .. thisActivityInfo.id;
thisActivityInfo.itemCfg = ActivityConfig[thisActivityInfo.id]["items"] or {};

----------------- local data -----------------
local ExchangeActivityBase = {};


local ExchangeItem = {
	ccbiFile = "Act_Mid-autumnContent.ccbi",
};

function ExchangeItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ExchangeItem.onRefreshItemView(container);
	elseif eventName == "onRewardBtn" then
		ExchangeItem.onReceiveReward(container);
	end		
end

function ExchangeItem.onRefreshItemView(container)
	local index = container:getItemDate().mID;
	local itemCfgId = thisActivityInfo.rewardIds[index];
	local rewardItem = thisActivityInfo.itemCfg[itemCfgId];
	if rewardItem == nil then return; end
	
	local cfg = thisActivityInfo.rewardCfg[itemCfgId];
	if cfg == nil then
		cfg = ConfigManager.getRewardById(rewardItem.r);
		thisActivityInfo.rewardCfg[itemCfgId] = cfg;
	end
	local day = rewardItem.d;
	NodeHelper:fillRewardItem(container, cfg);	
	NodeHelper:setStringForLabel(container, {mRechargeRebateName = common:getLanguageString("@ExchangeActivityItem", day)});
	local canReceive = day <= thisActivityInfo.hasRecharged;
	NodeHelper:setMenuItemEnabled(container, "mRewardBtn", canReceive);
end

function ExchangeItem.onReceiveReward(container)
	local index = container:getItemDate().mID;
	local itemCfgId = thisActivityInfo.rewardIds[index];
	local rewardItem = thisActivityInfo.itemCfg[itemCfgId];
	if rewardItem == nil then return; end
	

end

local function setSelectOptions()
	local id = thisActivityInfo.exchangeId;
	local itemCfg = thisActivityInfo.itemCfg[id];
	local consumeCfg = ConfigManager.getConsumeById(itemCfg.c);
	if consumeCfg == nil or consumeCfg["type"] ~= 2 then return; end
	
	thisActivityInfo.optionIds = {};
	for _, cfg in ipairs(consumeCfg["items"]) do
		local ownCount = UserItemManager:getCountByItemId(cfg.itemId);
		if ownCount >= cfg.count then
			thisActivityInfo.optionIds[cfg.itemId] = {
				type 	= cfg.type,
				need 	= cfg.count,
				own 	= ownCount
			};
		end
	end
end

local function GoodsItemOnFunction(eventName, container)
	if eventName == "onHand" then
		local exchangeId = thisActivityInfo.exchangeId;
		if exchangeId ~= 4 then
			local index = container:getTag();
			local itemCfg = thisActivityInfo.itemCfg[exchangeId];
			local consumeCfg = ConfigManager.getConsumeById(itemCfg.c);
			local cfg = consumeCfg["items"][index];
			if cfg then
				PageManager.showResInfo(cfg.type, cfg.itemId, cfg.count);
			end
			return;
		end
		
		setSelectOptions();	
		local options = common:table_keys(thisActivityInfo.optionIds);
		if #options <= 0 then
			MessageBoxPage:Msg_Box_Lan("@NoItemToSelect");
			return;
		end
		RegisterLuaPage("ItemSelectPage");
		ItemSelectPage_setOptions(
			options,
			thisActivityInfo.selectedId,
			function (selectedId)
				thisActivityInfo.selectedId = selectedId;
				PageManager.refreshPage(thisPageName);
			end 
		);
		PageManager.pushPage("ItemSelectPage");	
	end
end

local function ExchangeItemOnFunction(eventName, container)
	if eventName == "onExchangeBtn" then
		local msg = Activity_pb.HPWordsExchange();
		msg.type = tonumber(thisActivityInfo.exchangeId);
		if msg.type == 4 then
			if thisActivityInfo.selectedId == nil then
				return;
			end
			msg.itemId = thisActivityInfo.selectedId;
		end
		common:sendPacket(opcodes.WORDS_EXCHANGE_C, msg, false);
	elseif eventName == "onRewardFrame" then
		local id = thisActivityInfo.exchangeId;
		local itemCfg = thisActivityInfo.itemCfg[id];
		if itemCfg == nil then return; end
		
		local rewardCfg = ConfigManager.getRewardById(itemCfg.r);
		PageManager.showResInfo(rewardCfg[1].type, rewardCfg[1].itemId, rewardCfg[1].count);
	end
end
-----------------------------------------------
--ExchangeActivityBase页面中的事件处理
----------------------------------------------
function ExchangeActivityBase:onEnter(container)
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	NodeHelper:initRawScrollView(container, "mContent");
	if container.mScrollView ~= nil then
		container:autoAdjustResizeScrollview(container.mScrollView);
	end	
	self:clearPage(container);
	self:registerPacket(container);
	self:getActivityInfo(container);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
	
	local svItem = ScriptContentBase:create("Act_Mid-autumnContent.ccbi");
	svItem:registerFunctionHandler(ExchangeItemOnFunction);
	svItem:setPosition(CCPointMake(0, 0));
	container.mScrollView:addChild(svItem);
	NodeHelper:setStringForLabel(svItem, {mRewardName = ""});
	svItem:release();
	container.svItem = svItem;
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - svItem:getContentSize().height * container.mScrollView:getScaleY()))
end

function ExchangeActivityBase:onExecute(container)
	self:onTimer(container);
end

function ExchangeActivityBase:onExit(container)
	TimeCalculator:getInstance():removeTimeCalcultor(thisActivityInfo.timerName);
	NodeHelper:deleteScrollView(container);
	self:removePacket(container);
	container:removeMessage(MSG_MAINFRAME_REFRESH);
	thisActivityInfo.exchangeId = 1;
	thisActivityInfo.selectedId = nil;
end
----------------------------------------------------------------
function ExchangeActivityBase:onTimer(container)
	local timerName = thisActivityInfo.timerName;
	if not TimeCalculator:getInstance():hasKey(timerName) then return; end

	local remainTime = TimeCalculator:getInstance():getTimeLeft(timerName);
	if remainTime + 1 > thisActivityInfo.remainTime then
		return;
	end

	thisActivityInfo.remainTime = math.max(remainTime, 0);
	local timeStr = common:second2DateString(thisActivityInfo.remainTime, false);
	NodeHelper:setStringForLabel(container, {mCD = timeStr});
end

function ExchangeActivityBase:clearPage(container)
	NodeHelper:setStringForLabel(container, {
		mCD					= "",
		mRechargeDaysNum	= ""
	});
end

function ExchangeActivityBase:getActivityInfo(container)
	common:sendEmptyPacket(opcodes.WORDS_EXCHANGE_INFO_C);
end

function ExchangeActivityBase:refreshPage(container)
	if thisActivityInfo.remainTime > 0 and not TimeCalculator:getInstance():hasKey(timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(thisActivityInfo.timerName, thisActivityInfo.remainTime);
	end
	self:showRedNotice(container);
	self:showExchangeItem(container);
end
---------------------------------------------------
function ExchangeActivityBase:judgeCanExchange(container, id)
	local canExchange = false;
	local itemCfg = thisActivityInfo.itemCfg[id];
	local leftTimes = thisActivityInfo.leftExchangeTimes[id] or 0;
	if itemCfg.t < 0 or leftTimes > 0 then
		canExchange = ResManagerForLua:canConsume(itemCfg.c);
	end
	NodeHelper:setNodesVisible(container, {[string.format("mGiftPoint%02d", id)] = canExchange});
	thisActivityInfo.canExchange[id] = canExchange;
end

function ExchangeActivityBase:showRedNotice(container)
	for i = 1, COUNT_EXCHANGE_TYPE do
		self:judgeCanExchange(container, i);
	end
end

function ExchangeActivityBase:showExchangeItem(container)
	local id = thisActivityInfo.exchangeId;
	local itemCfg = thisActivityInfo.itemCfg[id];
	if itemCfg == nil then return; end

	if id == 4 then
		setSelectOptions();
	end
	
	--menu selected
	local menuName = string.format("mGift%02d", thisActivityInfo.exchangeId);
	NodeHelper:setMenuItemSelected(container, {[menuName] = true});
	
	--consume 
	local sv = container.svItem:getVarScrollView("mCollectionContentItem");
	if thisActivityInfo.initSize == nil then
		thisActivityInfo.initSize = sv:getViewSize();
	end
	sv:setBounceable(false);
	sv:getContainer():removeAllChildren();
	
	local consumeCfg = ConfigManager.getConsumeById(itemCfg.c);
	local isSingle = consumeCfg["type"] == 2;
	local consumeNum = consumeCfg["type"] == 2 and 1 or #consumeCfg["items"];
	local line = math.ceil(consumeNum / COUNT_CONSUMEITEM_PER_LINE);
	local height = 0;
	for i = line, 1, -1 do
		local listNode = ScriptContentBase:create("Act_Mid-autumnContentItem.ccbi");
		local baseIndex = (i - 1) * COUNT_CONSUMEITEM_PER_LINE;
		for j = 1, COUNT_CONSUMEITEM_PER_LINE do
			local posNode = listNode:getVarNode(string.format("mPositionNode%02d", j));
			posNode:removeAllChildren();
			local index = baseIndex + j;
			if isSingle and index <= 3 then
				index = j ~= 3 and 0 or 1;
			end
			if index > 0 and index <= consumeNum then
				local itemNode = CCBManager:getInstance():createAndLoad2("GoodsItem.ccbi");
				itemNode:setTag(index);
				itemNode:registerFunctionHandler(GoodsItemOnFunction);
				local resInfo = {};
				local countStr = "";
				local colorKey = "Own";
				if isSingle then
					if thisActivityInfo.selectedId then
						local info = thisActivityInfo.optionIds[thisActivityInfo.selectedId];
						if info then
							resInfo = ResManagerForLua:getResInfoByTypeAndId(info.type, thisActivityInfo.selectedId, info.need);
							countStr = info.own .. "/" .. info.need;
							colorKey = info.need > info.own and "LackRed" or "Own";
						end
					end
				else
					local cfg = consumeCfg["items"][index];
					resInfo = ResManagerForLua:getResInfoByTypeAndId(cfg.type, cfg.itemId, cfg.count);
					local ownCount = UserItemManager:getCountByItemId(cfg.itemId) or 0;
					countStr = ownCount .. "/" .. resInfo.count;
					colorKey = resInfo.count > ownCount and "LackRed" or "Own";
				end
				local lb2Str = {
					mName 	= countStr,
					mNumber	= ""
				};
				NodeHelper:setStringForLabel(itemNode, lb2Str);
				NodeHelper:setSpriteImage(itemNode, {mPic = resInfo.icon or GameConfig.Image.ClickToSelect});
				NodeHelper:setQualityFrames(itemNode, {mHand = resInfo.quality or GameConfig.Default.Quality});
				NodeHelper:setColor3BForLabel(itemNode, {mName = common:getColorFromConfig(colorKey)});
				
				itemNode:setPosition(CCPointMake(0, 0));
				posNode:addChild(itemNode);
			end
		end
		listNode:setPosition(CCPointMake(0, height));
		height = height + listNode:getContentSize().height;
		sv:addChild(listNode);
		listNode:release();
	end
	
	local size = thisActivityInfo.initSize;
	local offset = height - size.height;
	
	local node = container.svItem:getVarNode("mScale9Sprite02");
	if thisActivityInfo.initSize2 == nil then
		thisActivityInfo.initSize2 = node:getContentSize();
	end
	local size2 = thisActivityInfo.initSize2;
	
	local label = container.svItem:getVarNode("mTitle");
	if label then
		if thisActivityInfo.initPosY == nil then
			local posX = nil;
			posX, thisActivityInfo.initPosY = label:getPosition();
		end
	
		local posY = thisActivityInfo.initPosY + offset;
		label:setPositionY(posY);
	end

	if offset > 0 then
		node:setContentSize(CCSizeMake(size2.width, size2.height + offset));
	else
		node:setContentSize(size2);
	end
	
	sv:setContentSize(CCSizeMake(size.width, height));
	sv:setViewSize(CCSizeMake(size.width, height));
	sv:setContentOffset(CCPointMake(0, 0));
	
	--times
	local totalTimes = itemCfg.t or 0;
	local timesStr = "";
	--NodeHelper:setNodesVisible(container.svItem, {mExchangeNumNode = totalTimes >= 0});
	if totalTimes >= 0 then
		local leftTimes = math.max(totalTimes - (thisActivityInfo.leftExchangeTimes[id] or 0), 0);
		timesStr = string.format("%d/%d", leftTimes, totalTimes);
	else
		timesStr = common:getLanguageString("@NoLimit");
	end
	NodeHelper:setStringForLabel(container.svItem, {mExchangeNum = timesStr});
	
	--canExchange
	local canExchange = thisActivityInfo.canExchange[id];
	if canExchange and id == 4 then
		canExchange = thisActivityInfo.selectedId ~= nil;
	end
	NodeHelper:setMenuItemEnabled(container.svItem, "mExchangeBtn", canExchange);
	
	--reward 
	local rewardCfg = ConfigManager.getRewardById(itemCfg.r);
	local resInfo = ResManagerForLua:getResInfoByTypeAndId(rewardCfg[1].type, rewardCfg[1].itemId, rewardCfg[1].count);
	local lb2Str = {
		mRewardName = resInfo.name .. "x" .. resInfo.count
	};
	NodeHelper:setStringForLabel(container.svItem, lb2Str);
	NodeHelper:setSpriteImage(container.svItem, {mRewardPic = resInfo.icon or GameConfig.Image.DefaultGift});
	NodeHelper:setQualityFrames(container.svItem, {mRewardFrame = resInfo.quality or GameConfig.Default.Quality});
	
	--height
	if thisActivityInfo.initSVSize == nil then
		thisActivityInfo.initSVSize = container.svItem:getContentSize();
	end
	local svSize = CCSizeMake(thisActivityInfo.initSVSize.width, thisActivityInfo.initSVSize.height + offset);
	container.mScrollView:setContentSize(svSize);
	container.mScrollView:setContentOffset(ccp(0, container.mScrollView:getViewSize().height - svSize.height * container.mScrollView:getScaleY()))
end	
----------------scrollview-------------------------
function ExchangeActivityBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function ExchangeActivityBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function ExchangeActivityBase:buildItem(container)
	NodeHelper:buildScrollView(container, #thisActivityInfo.rewardIds, ExchangeItem.ccbiFile, ExchangeItem.onFunction);
end
	
----------------click event------------------------	
function ExchangeActivityBase:onClose(container)
	PageManager.popPage(thisPageName);
end	

function ExchangeActivityBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_ACT_AUTUMN);
end

function ExchangeActivityBase:onBack()
	PageManager.changePage("ActivityPage");
end

function ExchangeActivityBase:changeExchangeType(container, eventName)
	local id = tonumber(eventName:sub(-2));
	if thisActivityInfo.itemCfg[id] then
		local menuName = string.format("mGift%02d", thisActivityInfo.exchangeId);
		if thisActivityInfo.exchangeId ~= id then
			NodeHelper:setMenuItemSelected(container, {[menuName] = false});
			thisActivityInfo.exchangeId = id;
			self:showExchangeItem(container);
		else
			NodeHelper:setMenuItemSelected(container, {[menuName] = true});
		end
	end
end

function ExchangeActivityBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();

	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:showExchangeItem(container);
		end
	end
end

--回包处理
function ExchangeActivityBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.WORDS_EXCHANGE_INFO_S then
		local msg = Activity_pb.HPWordsExchangeInfo();
		msg:ParseFromString(msgBuff);
		
		for i, times in ipairs(msg.leftExchangeTimes) do
			thisActivityInfo.leftExchangeTimes[i] = times;
		end
		thisActivityInfo.remainTime = msg.activityLeftTime;
		
		if thisActivityInfo.selectedId and 
			UserItemManager:getCountByItemId(thisActivityInfo.selectedId) <= 0
		then
			thisActivityInfo.selectedId = nil;
		end

		self:refreshPage(container);
		return;
	end
end

function ExchangeActivityBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function ExchangeActivityBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ExchangeActivity = CommonPage.newSub(ExchangeActivityBase, thisPageName, option);

function ExchangeActivity_reset()
	--Todo
end
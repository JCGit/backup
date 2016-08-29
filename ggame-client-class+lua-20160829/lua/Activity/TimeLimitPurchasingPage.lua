----------------------------------------------------------------------------------
--[[
	FILE:			TimeLimitPurchasingPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	¾º¼¼³¡
	AUTHOR:			dmq
	CREATED:		2014-11-04
--]]
----------------------------------------------------------------------------------
local thisPageName = "TimeLimitPurchasingPage"
local NodeHelper = require("NodeHelper");
local opcodes = {
	TIME_LIMIT_PURCHASE_INFO_C = HP_pb.TIME_LIMIT_PURCHASE_INFO_C,
	TIME_LIMIT_PURCHASE_INFO_S = HP_pb.TIME_LIMIT_PURCHASE_INFO_S,
	TIME_LIMIT_BUY_C		   = HP_pb.TIME_LIMIT_BUY_C	
}
local option = {
	ccbiFile = "Act_TimeLimitPurchasePage", 
	handlerMap = {
		onHelp = "onHelp",
		onReturnButton = "onBack"
    },
	opcode = opcodes
}

local TimeLimitPurchasingBase = {}

local PageInfo = {
	timerName = "Activity_TimeLimit_26",
	timeLeft = 0,
	timeLimitItems = {}
}	

local ItemManager = require "ItemManager"

local TimeLimitPurchaseCfg = {}
------------Page item start----------------------
local TimeLimitPurchasingItem = {}

function TimeLimitPurchasingItem.onFunction( eventName , container )
	if eventName == "luaRefreshItemView" then
		TimeLimitPurchasingItem.refreshItemView( container )
	elseif eventName == "onBuyBtn" then
		TimeLimitPurchasingItem.buyItem( container )
	elseif eventName == "onFeet" then
		TimeLimitPurchasingItem.showGoodsInfo( container )
	end
end

function TimeLimitPurchasingItem.refreshItemView( container )
	local index = container:getItemDate().mID
	local itemId = PageInfo.timeLimitItems[index].id
	local itemInfo = TimeLimitPurchaseCfg[itemId]
	
	local rewardCfg = {ConfigManager.getRewardById(itemInfo.goodsId)[1]}
	
	local params = {
	        mainNode = "mItemName",
	        countNode = "mNum",
            nameNode = "mNameLabel",
            frameNode = "mFeet01",
            picNode = "mPic",
            startIndex = 1
	}
	
	TimeLimitPurchasingItem.fillRewardItem(container, rewardCfg,params)
	
	--container:getVarSprite("mPic"):setTexture( goodsItem.icon )
	--NodeHelper:setMenuItemQuality( container , "mFeet01" ,goodsItem.quality  )
	
	local buyStr = ""
	if itemInfo.buyType == "1" or itemInfo.buyType == "2" then
		buyStr = common:getLanguageString("@BuyStr" .. itemInfo.buyType , PageInfo.timeLimitItems[index].buyTimes , itemInfo.maxBuyTimes )
	end
	container:getVarLabelBMFont("mActCanBuyLabel"):setString( buyStr )
	container:getVarLabelBMFont("mRemainingQuantity"):setString( PageInfo.timeLimitItems[index].leftBuyTimes )
	container:getVarLabelBMFont("mOriginalPrice"):setString( common:getLanguageString( "@OriginalPrice" , itemInfo.originalPrice) )
	container:getVarLabelBMFont("mSpecialOffer"):setString( common:getLanguageString( "@SalePrice" , itemInfo.salePrice ))
	container:getVarLabelBMFont("mCanBuyVip"):setString( common:getLanguageString( "@PurchasingVipLimit" , itemInfo.vipLimit ) )
	
	
	container:getVarNode("mAllSurplus"):setVisible( PageInfo.timeLimitItems[index].leftBuyTimes ~= -1 )
	
end

function TimeLimitPurchasingItem.showGoodsInfo( container )
	local index = container:getItemDate().mID
	local itemId = PageInfo.timeLimitItems[index].id
	local itemInfo = TimeLimitPurchaseCfg[itemId]
	
	local thisItemId = ConfigManager.getRewardById(itemInfo.goodsId)[1].itemId
	local thisItemType = ItemManager:getTypeById(thisItemId)
	
	if thisItemType == Const_pb.GIFT then
		PageManager.showGiftPackage(thisItemId,nil)
	elseif thisItemType == Const_pb.EQUIP then
		local stepLevel = EquipManager:getEquipStepById(thisItemId)
	    GameUtil:showTip(container:getVarNode('mFeet01'), {
		type 		= ConfigManager.getRewardById(itemInfo.goodsId)[1].type, 
		itemId 		= thisItemId,
		buyTip		= true,
		starEquip	= stepLevel == GameConfig.ShowStepStar
		})
	else
		GameUtil:showTip(container:getVarNode('mFeet01'), {
		type 		= ConfigManager.getRewardById(itemInfo.goodsId)[1].type, 
		itemId 		= thisItemId,
		buyTip		= false,
		starEquip	= false
		})
	end
	
end

function TimeLimitPurchasingItem.buyItem( container )
	local index = container:getItemDate().mID
	local msg = Activity_pb.HPTimeLimitBuy()
	msg.cfgId = PageInfo.timeLimitItems[index].id
	common:sendPacket(opcodes.TIME_LIMIT_BUY_C , msg ,false)
end

function TimeLimitPurchasingItem.fillRewardItem(container, rewardCfg,params)
    local nodesVisible = {};
	local lb2Str = {};
	local sprite2Img = {};
	local menu2Quality = {};
	local btnSprite = {};
	
	local mainNode = params.mainNode 
	local countNode = params.countNode 
    local nameNode = params.nameNode 
    local frameNode = params.frameNode 
    local picNode   = params.picNode  
	
	
	local cfg = rewardCfg[1];
	nodesVisible[mainNode] = cfg ~= nil;
	if cfg ~= nil then
	    local resInfo = ResManagerForLua:getResInfoByTypeAndId(cfg.type, cfg.itemId, cfg.count);
	    if resInfo ~= nil then
		    sprite2Img[picNode] 		= resInfo.icon;
		    lb2Str[countNode]				= "x" .. cfg.count;
		    lb2Str[nameNode]			= resInfo.name;
		    menu2Quality[frameNode]		= resInfo.quality;
	    else
	        CCLuaLog("Error::***reward item not found!!");
	    end
	end
	
	NodeHelper:setNodesVisible(container, nodesVisible);
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img);
	NodeHelper:setQualityFrames(container, menu2Quality);
end
---------Page item end----------------------

function TimeLimitPurchasingBase:onEnter( container )
	container:registerPacket(opcodes.TIME_LIMIT_PURCHASE_INFO_S)
	NodeHelper:initScrollView( container,"mContent",5)
	TimeLimitPurchaseCfg = ConfigManager.getTimeLimitPurcgaseItem()
	self:getActivityInfo( container )
end

function TimeLimitPurchasingBase:getActivityInfo( container )
	common:sendEmptyPacket(opcodes.TIME_LIMIT_PURCHASE_INFO_C , true)
end

function TimeLimitPurchasingBase:onExecute( container )
	self:onTimer( container )
end

function TimeLimitPurchasingBase:onTimer( container )
	local timeStr = '00:00:00'
	if TimeCalculator:getInstance():hasKey(PageInfo.timerName) then
		PageInfo.timeLeft = TimeCalculator:getInstance():getTimeLeft(PageInfo.timerName)
		if PageInfo.timeLeft > 0 then
			 timeStr = common:second2DateString(PageInfo.timeLeft , false)
		end
	end
	NodeHelper:setStringForLabel(container, { mActivityDaysNum = timeStr})
end

function TimeLimitPurchasingBase:onExit( container )
	TimeCalculator:getInstance():removeTimeCalcultor(PageInfo.timerName)
end

function TimeLimitPurchasingBase:onReceivePacket( container )
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.TIME_LIMIT_PURCHASE_INFO_S then
		local msg = Activity_pb.TimeLimitPurchase()
		msg:ParseFromString(msgBuff)
		self:handleAcitivityInfo( container , msg )
	end
end

function TimeLimitPurchasingBase:handleAcitivityInfo( container , msg )
	PageInfo.timeLeft = msg.leftTime
	if PageInfo.timeLeft > 0 and not TimeCalculator:getInstance():hasKey(PageInfo.timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(PageInfo.timerName, PageInfo.timeLeft);
	end
	
	PageInfo.timeLimitItems = msg.timeLimitItems
	self:rebuildAllItem( container ) 
end	

function TimeLimitPurchasingBase:rebuildAllItem( container )
	self:clearAllItem(container)
	self:buildItem(container)
end

function TimeLimitPurchasingBase:clearAllItem( container )
	NodeHelper:clearScrollView(container)
end

function TimeLimitPurchasingBase:buildItem( container )
	local maxSize = table.maxn(PageInfo.timeLimitItems)	
	NodeHelper:buildScrollView(container, maxSize, "Act_TimeLimitPurchaseContent.ccbi", TimeLimitPurchasingItem.onFunction)
end

function TimeLimitPurchasingBase:onBack( container )
	PageManager.changePage("ActivityPage")
end

function TimeLimitPurchasingBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_TIMELIMITPURCHASE)
end
------------------------------------------------------------------------------------
local CommonPage = require "CommonPage"
TimeLimitPurchasingPage = CommonPage.newSub(TimeLimitPurchasingBase , thisPageName ,option)
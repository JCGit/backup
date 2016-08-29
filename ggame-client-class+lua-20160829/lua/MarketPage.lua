----------------------------------------------------------------------------------
--[[
	FILE:			MarketPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	商城页面
	AUTHOR:			zhao zhen
	CREATED:		2014-08-04
--]]
----------------------------------------------------------------------------------
local thisPageName = "MarketPage"
local HP = require("HP_pb");
require("Shop_pb");
local NodeHelper = require("NodeHelper");
local ResManagerForLua = require("ResManagerForLua");
local Const = require('Const_pb');
local Reward_pb = require "Reward_pb"
local Consume_pb = require("Consume_pb")
local UserInfo = require("UserInfo");

------------local variable for system global api--------------------------------------
local tostring = tostring;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
local mRebuildLock = true
local mRefreshCout = 0

local MarketPageBase = {};

local MarketItem = 
{
	ccbiFile = "FairContentItem.ccbi",
};

local MarketItemSub = 
{
	ccbiFile = "FairContent.ccbi",
};

local opcodes = 
{
	OPCODE_SHOP_DROPS_C = HP.SHOP_C,
	OPCODE_SHOP_DROPS_S = HP.SHOP_S,
	OPCODE_SHOP_COINS_C = HP.SHOP_COIN_C,
	OPCODE_SHOP_COINS_S = HP.SHOP_COIN_S,
	OPCODE_PLAYER_AWARD_S = HP_pb.PLAYER_AWARD_S,
	OPCODE_PLAYER_CONSUME_S = HP_pb.PLAYER_CONSUME_S
}

--商店类型
local iConType = 
{
	type_Coins = 1,
	type_Gold = 2,
	type_Coins_Pic = "UI/MainScene/Common/u_CmnCoin.png",
	type_Gold_Pic = "UI/MainScene/Common/u_CmnGold.png"
}

local option = {
	ccbiFile = "FairPage.ccbi",
	handlerMap = {
		onGoblinmerchant		= "openDropsMarket",
		onBuyGold			= "openCoinsMarket",
		onBuyAll				= "onBuyAll",
		onBrush				= "onBuyCoinsOrRefreshDrops",
		onHelp			= "onHelp"
	},
	opcode = opcodes
};

--商店购买类型
local ShopInfoType = 
{
	TYPE_INIT_INFO = 1,
	TYPE_BUY_SINGLE = 2,
	TYPE_BUY_ALL = 3,
	TYPE_REFRESH = 4
}

local MainContainer = nil;

--商店被选择的类型
local ShopSelectedState = 
{
	STATE_DROPS = 1,
	STATE_COINS = 2
}
--商店商品List信息
marketAdventureInfo = 
{
	dropsItems = {},
	coinReward = 0,
	coinCost = 0,
	coinCount = 0
}

local dropsItemsTeam = 
{

}

--商店当前选择类型
SHOP_CURRENT_STATE = ShopSelectedState.STATE_DROPS;

--折扣信息
local PercentItem = 
{
	eight_percent = 80,
	five_percent = 50,
	one_percent = 10
}

local ITEM_COUNT_PER_LINE = 5;

local VipConfig = ConfigManager.getVipCfg();
local RefreshCostCfg = ConfigManager.getRefreshMarketCostCfg();

-----------------注销，clear 商店list信息-----------------------------
function RESETINFO_MARKET()
	SHOP_CURRENT_STATE = ShopSelectedState.STATE_DROPS;
	marketAdventureInfo = {}
	marketAdventureInfo.dropsItems = {};
	marketAdventureInfo.coinReward = 0;
	marketAdventureInfo.coinCost = 0;
	marketAdventureInfo.coinCount = 0;
end

local function sendMsgForDropsInfo( container, dType, itemId)
	local msg = Shop_pb.OPShopInfo();
	msg.type = dType;
	msg.itemId = tostring(itemId);

	local pb_data = msg:SerializeToString();
	container:sendPakcet(opcodes.OPCODE_SHOP_DROPS_C, pb_data, #pb_data, true);
end

function MarketItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		MarketItem.onRefreshItemView(container);
	end
end

function MarketItem.onSubFunction( eventName, container )
	if eventName == "onbuy" then
		MarketItem.buySingleDrop(container);
	elseif eventName == "onHand" then
		MarketItem.showTip(container);
	end
end

function MarketItem.showTip(container)
	local index = container:getTag();
	local item = marketAdventureInfo.dropsItems[index];
	if item == nil then return; end
	
	local stepLevel = EquipManager:getEquipStepById(item.itemId)

	GameUtil:showTip(container:getVarNode('mHand'), {
		type 		= item.itemType, 
		itemId 		= item.itemId,
		buyTip		= true,
		starEquip	= stepLevel == GameConfig.ShowStepStar
	});
end

function MarketItem.onTipClick() 
    CCLuaLog("this is a on tip Click")
end

--刷新商店content,一个里面有三个物品
function MarketItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;

	local currenTeamInfo = dropsItemsTeam[contentId];
	local infoSize = table.maxn(currenTeamInfo);

	for i = 1, infoSize, 1 do
		local itemInfo = currenTeamInfo[i];

		local resInfo = ResManagerForLua:getResInfoByTypeAndId(itemInfo.itemType, itemInfo.itemId, itemInfo.itemCount);

		local tag = MarketItem.getMainInfoIndexById(itemInfo.id);
		local subContainer = ScriptContentBase:create(MarketItemSub.ccbiFile,tag);
		subContainer:registerFunctionHandler(MarketItem.onSubFunction);

		--label
		local dropStr = 
		{
			mNumber = "X"..itemInfo.itemCount,
			mCommodityName = tostring(resInfo.name),
			mCommodityNum = tostring(itemInfo.buyPrice)
		};

		NodeHelper:setStringForLabel(subContainer, dropStr);

		--image
		NodeHelper:setSpriteImage(subContainer,{mPic = resInfo.icon});

		NodeHelper:setMenuItemQuality(subContainer, "mHand", resInfo.quality);
		NodeHelper:setQualityBMFontLabels(subContainer, {mCommodityName = resInfo.quality});

		local percentPic = MarketItem.getPercentTextureStr(itemInfo.buyDiscont);
		if percentPic == nil then
			NodeHelper:setNodesVisible(subContainer,{mLabel = false});
		else
			NodeHelper:setNodesVisible(subContainer,{mLabel = true});
			NodeHelper:setSpriteImage(subContainer,{mLabel = percentPic});
		end

		local mainType = ResManagerForLua:getResMainType(resInfo.itemType);

		if mainType == Const.TOOL then
			NodeHelper:setNodesVisible(subContainer,{mLv = false, mNumber = true});
		elseif mainType == Const.EQUIP then
			local equipLevel = EquipManager:getLevelById(itemInfo.itemId);
			NodeHelper:setStringForLabel(subContainer, {mLv = "LV "..equipLevel});
			NodeHelper:setNodesVisible(subContainer,{mLv = true, mNumber = false});
		else
			NodeHelper:setNodesVisible(subContainer,{mLv = false, mNumber = false});
		end

		local iconPic = "";
		if itemInfo.buyType == iConType.type_Coins then
			iconPic = iConType.type_Coins_Pic;
		elseif itemInfo.buyType == iConType.type_Gold then
			iconPic = iConType.type_Gold_Pic;
		end
		NodeHelper:setSpriteImage(subContainer,{mConsumptionType = iconPic});

		if itemInfo.isAdd then
			NodeHelper:setNodesVisible(subContainer,{mPanel01 = true});
		else
			NodeHelper:setNodesVisible(subContainer,{mPanel01 = false});
		end

		--add node
		local mainNodeName = "mPosition"..tostring(i);
		local mainNode = container:getVarNode(mainNodeName);
		mainNode:removeAllChildren();
		mainNode:addChild(subContainer);
		subContainer:release();
	end

end

--根据物品信息，得到content id即index（list中）
function MarketItem.getMainInfoIndexById( id )
	local maxSize = table.maxn(marketAdventureInfo.dropsItems);
	for i = 1,maxSize,1 do
		local item = marketAdventureInfo.dropsItems[i];

		if item.id == id then
			return i;
		end
	end

	return nil;
end

--根据content id即index（list中）,得到物品信息
function MarketItem.getIdByMainInfoIndex( index )
	local item = marketAdventureInfo.dropsItems[index];

	if item ~= nil then
		return item.id;
	end

	return nil;
end

--得到折扣图片
function MarketItem.getPercentTextureStr( percentCount )
	if percentCount == PercentItem.eight_percent then
		return "UI/MainScene/UI/u_Label01.png";
	elseif percentCount == PercentItem.five_percent then
		return "UI/MainScene/UI/u_Label02.png";
	elseif percentCount == PercentItem.one_percent then
		return "UI/MainScene/UI/u_Label03.png";
	else
		return nil;
	end
end


----------------------------------------------------------------------------------
	
-----------------------------------------------

----------------------------------------------
function MarketPageBase:onLoad( container )
	container:loadCcbiFile(option.ccbiFile);
	NodeHelper:initScrollView(container, "mContent", 3);
	MainContainer = container;
end

function MarketPageBase:onEnter(container)

    container.scrollview=container:getVarScrollView("mContent");
	if container.scrollview~=nil then
		container:autoAdjustResizeScrollview(container.scrollview);
	end

	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end

	self:registerPacket(container)
	container:registerMessage(MSG_MAINFRAME_CHANGEPAGE)
	container:registerMessage(MSG_MAINFRAME_REFRESH);

	if SHOP_CURRENT_STATE == ShopSelectedState.STATE_COINS then
		self:openCoinsMarket(container);
	else
		self:openDropsMarket(container);
	end


	self:refreshBasicInfo( container );

	self:rebuildCoinsInfo(container);


	--self:refreshPage(container);
	--self:rebuildAllItem(container);
end

--刷新页面基本信息，lv,vip等
function MarketPageBase:refreshBasicInfo( container )
	UserInfo.syncPlayerInfo();
	local pageInfo = 
	{
		mMailPromptTex = mailNoticeStr,
		mLV = "LV " .. UserInfo.roleInfo.level,
		mVip = "VIP " .. UserInfo.playerInfo.vipLevel,
		mCoin = UserInfo.playerInfo.coin,
		mGold = UserInfo.playerInfo.gold
	}

	NodeHelper:setStringForLabel(container, pageInfo);
end

function MarketPageBase:onExecute(container)
end

function MarketPageBase:onExit(container)
	self:removePacket(container)
	container:removeMessage(MSG_MAINFRAME_CHANGEPAGE)	
	container:removeMessage(MSG_MAINFRAME_REFRESH);
	NodeHelper:deleteScrollView(container);
	mRebuildLock = true;
end
----------------------------------------------------------------

function MarketPageBase:refreshPage(container)
	if SHOP_CURRENT_STATE == ShopSelectedState.STATE_COINS then
		self:openCoinsMarket(container);
	else
		self:openDropsMarket(container);
	end

	self:refreshBasicInfo( container );
end

----------------刷新金币商店页面-------------------------
function MarketPageBase:rebuildCoinsInfo( container )

	UserInfo.syncPlayerInfo();

	local pageInfo = 
	{
		mMailPromptTex = mailNoticeStr,

	}

	NodeHelper:setStringForLabel(container, pageInfo);

	local vipLevel = UserInfo.playerInfo.vipLevel;
	local vipCount = VipConfig[vipLevel].buyCoinTime;
	local vipKey = common:getLanguageString("@ShopVip", vipLevel, vipCount);
	local coinMarketStr = 
	{
		mLV = "LV " .. UserInfo.roleInfo.level,	
		mCoin = UserInfo.playerInfo.coin,
		mGold = UserInfo.playerInfo.gold,
		mCoinNum = common:getLanguageString("@ShopCoins", tostring(marketAdventureInfo.coinReward)),
		mGoldNum = common:getLanguageString("@ShopCost", tostring(marketAdventureInfo.coinCost)),
		mPurchaseTimesNum = common:getLanguageString("@ShopCount", tostring(marketAdventureInfo.coinCount)),
		mVipTex = common:getLanguageString(vipKey);
	}

	local sprite2Img = {
		mVip		= UserInfo.getVipImage()
	};
	NodeHelper:setSpriteImage(container, sprite2Img);
	
	NodeHelper:setStringForLabel(container, coinMarketStr);

	-- ToDo
	if tonumber(marketAdventureInfo.coinCount) <= 0 then
		NodeHelper:setMenuItemEnabled(container, "mBrush", false);
	else
		NodeHelper:setMenuItemEnabled(container, "mBrush", true);
	end
end

--刷新物品商店页面
function MarketPageBase:rebuildAllItem(container)
     --预防同一时间刷新多次
    if mRebuildLock then
        mRebuildLock = false
        self:clearAllItem(container);
        self:buildItem(container);
        
        --延迟1s
        container:runAction(
			CCSequence:createWithTwoActions(
				CCDelayTime:create(0.3),
				CCCallFunc:create(function()
					mRebuildLock = true;
					--判断是否有未被刷新的情况存在，无论未被刷新多少次都只重新刷新一次
					if mRefreshCout > 0 then
					    mRefreshCout = 0
					    self:rebuildAllItem(container)
					end
				end)
			)
		);
	else
	--记录下未被刷新的次数
	    mRefreshCout = mRefreshCout + 1;
	end
end

function MarketPageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function MarketPageBase:buildItem(container)
	self:cutDropsInfoForTeam(container);
	local contentsSize = table.maxn(dropsItemsTeam);
	NodeHelper:buildScrollView(container, contentsSize, MarketItem.ccbiFile, MarketItem.onFunction);
end

local function sortFunc( m1, m2 )
	if not m1 then return true end
	if not m2 then return false end

	if m1.isAdd and not m2.isAdd then
		return true;
	elseif (not m2.isAdd and m1.isAdd) then
		return false;
	else 
		return m1.id > m2.id;
	end

end

--物品按3个3个一组进行分割
function MarketPageBase:cutDropsInfoForTeam( container )

	local maxSize = table.maxn(marketAdventureInfo.dropsItems);
	--服务器来排序，客户端省去排序操作,--by zhenhui 2014/8/15
	--table.sort(marketAdventureInfo.dropsItems, sortFunc);
	MAIL_ISSORTED = true;

	local teamId = 1;
	dropsItemsTeam = {};
	dropsItemsTeam[teamId] = {};
	local count = 1;
	local currentTeam = {};

	for i = 1, maxSize, 1 do
		if count < 4 then
			currentTeam[count] = marketAdventureInfo.dropsItems[i];
			count = count + 1;
		else
			dropsItemsTeam[teamId] = {};
			self:copyItems( teamId, currentTeam )
			currentTeam = {};
			count = 1;
			teamId = teamId + 1;

			currentTeam[count] = marketAdventureInfo.dropsItems[i];
			count = count + 1;
		end

		if i + 1 > maxSize then
			if table.maxn(currentTeam) > 0 then
				self:copyItems( teamId, currentTeam );
			end
		end
	end
end

function MarketPageBase:copyItems( index, currentTeam )
	local maxSize = table.maxn(currentTeam);
	dropsItemsTeam[index] = {};
	for i = 1, maxSize, 1 do
		local item = currentTeam[i];
		dropsItemsTeam[index][i] = {};
		dropsItemsTeam[index][i].id = item.id;
		dropsItemsTeam[index][i].itemId = item.itemId;
		dropsItemsTeam[index][i].itemType = item.itemType;
		dropsItemsTeam[index][i].itemCount = item.itemCount;
		dropsItemsTeam[index][i].buyType = item.buyType;
		dropsItemsTeam[index][i].buyPrice = item.buyPrice;
		dropsItemsTeam[index][i].buyDiscont = item.buyDiscont;
		dropsItemsTeam[index][i].level = item.level;
		dropsItemsTeam[index][i].isAdd = item.isAdd;
	end
end

-------------------------------------------------------------------------
function MarketPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function MarketPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode);
		end
	end
end	

function MarketPageBase:sendMsgForCoinsInfo( container, dType)
	local msg = Shop_pb.OPBuyCoin();
	msg.type = dType;

	local pb_data = msg:SerializeToString();
	container:sendPakcet(opcodes.OPCODE_SHOP_COINS_C, pb_data, #pb_data, true);
end

function MarketPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.OPCODE_SHOP_DROPS_S then
		local msg = Shop_pb.OPShopInfoRet()
		msg:ParseFromString(msgBuff)
		self:onReceiveDropsInfo(container, msg)
		return
	end

	if opcode == opcodes.OPCODE_SHOP_COINS_S then
		local msg = Shop_pb.OPBuyCoinRet()
		msg:ParseFromString(msgBuff)
		self:onReceiveCoinsInfo(container, msg)
		return
	end
	
	if opcode == opcodes.OPCODE_PLAYER_AWARD_S then
	    local msg = Reward_pb.HPPlayerReward();
	    msg:ParseFromString(msgBuff)
	    if msg:HasField("rewards") then
            if msg.rewards:HasField("gold")
                    or msg.rewards:HasField("coin")
                    or msg.rewards:HasField("level")
                    or msg.rewards:HasField("exp")
                    or msg.rewards:HasField("vipLevel")
                    then				
                    self:refreshBasicInfo( container )
            end
	    end
	    return
	end
	
	if opcode == opcodes.OPCODE_PLAYER_CONSUME_S then 
	    local msg = Consume_pb.HPConsumeInfo();
		msg:ParseFromString(msgBuff);
	    if msg:HasField("attrInfo") then
			if msg.attrInfo:HasField("gold")
				or msg.attrInfo:HasField("coin")
				or msg.attrInfo:HasField("level")
				or msg.attrInfo:HasField("exp")
				or msg.attrInfo:HasField("vipLevel")
				then
				self:refreshBasicInfo( container )
			end
	    end
	    return 
	end
end

function MarketPageBase:onReceiveDropsInfo( container, msg )

	if msg.type == ShopInfoType.TYPE_BUY_SINGLE then
		if msg.item ~= nil then
			--local resInfo = ResManagerForLua:getResInfoByTypeAndId(msg.item.itemType, msg.item.itemId, msg.item.itemCount);
			--local str = resInfo.name .."X" ..resInfo.count;
			--local messageStr = common:getLanguageString("@BuySuccess", str);
			--MessageBoxPage:Msg_Box(messageStr);
		end
	elseif msg.type == ShopInfoType.TYPE_BUY_ALL then
		--MessageBoxPage:Msg_Box("@BuyAllSuccess");
	elseif msg.type == ShopInfoType.TYPE_INIT_INFO then

	end

	marketAdventureInfo.dropsItems = msg.shopItems;
	marketAdventureInfo.refreshCount = msg.refreshCount;

	self:refreshBasicInfo( container )
	self:rebuildAllItem(container);
end

function MarketPageBase:refreshItemsData(newItems)
	-- body
end

function MarketPageBase:onReceiveCoinsInfo( container, msg )
	if msg.type == ShopInfoType.TYPE_BUY_SINGLE then
		local messageStr = common:getLanguageString("@BuyCoinSuccess", msg.coinNums);
		--MessageBoxPage:Msg_Box(messageStr);
	elseif msg.type == ShopInfoType.TYPE_BUY_ALL then
		--MessageBoxPage:Msg_Box("@BuyAllSuccess");
	end


	marketAdventureInfo.coinReward = msg.coin;
	marketAdventureInfo.coinCost = msg.coinPrice;
	marketAdventureInfo.coinCount = msg.canBuyNums;

	self:refreshBasicInfo( container )
	self:rebuildCoinsInfo(container);
end

-----------------------切换到物品商店页面-------------------------------------
function MarketPageBase:openDropsMarket( container )
	if marketAdventureInfo.dropsItems ~= nil then
		self:rebuildAllItem(container);
	end
	SHOP_CURRENT_STATE = ShopSelectedState.STATE_DROPS;
	self:switchPageType( container, true );
	sendMsgForDropsInfo( container, ShopInfoType.TYPE_INIT_INFO, 0);
end

-----------------------切换到金币商店页面-------------------------------------
function MarketPageBase:openCoinsMarket( container )
	SHOP_CURRENT_STATE = ShopSelectedState.STATE_COINS;
	self:switchPageType( container, false );
	self:sendMsgForCoinsInfo( container, ShopInfoType.TYPE_INIT_INFO);
end

--node切换控制显示哪个商店，金币商店分成两个node是用于屏幕适配
function MarketPageBase:switchPageType( container, typeFlag )

	--visible
	local visibleMap = 
	{
		mGoblinmerchantNode = typeFlag,
		mBuyGoldNum = not typeFlag,
		mBuyGoldNum2 = not typeFlag
	}

	NodeHelper:setNodesVisible(container, visibleMap);

	--selected
	local selectedMap =
	{
		mGoblinmerchant = typeFlag,
		mBuyGold = not typeFlag
	}

	NodeHelper:setMenuItemSelected( container, selectedMap );

	--name
	local btnStr = "";
	if typeFlag then
		btnStr = common:getLanguageString("@Brush");
	else
		btnStr = common:getLanguageString("@Buy");
	end

	NodeHelper:setStringForLabel(container, {mBrush = btnStr});
end

--购买所有物品
function MarketPageBase:onBuyAll( container )

	if SHOP_CURRENT_STATE == ShopSelectedState.STATE_DROPS then
		sendMsgForDropsInfo( container, ShopInfoType.TYPE_BUY_ALL, 0);
	elseif SHOP_CURRENT_STATE == ShopSelectedState.STATE_COINS then
		if marketAdventureInfo.coinCount <= 0 then
			MessageBoxPage:Msg_Box("@ShopCountLimit");
		else
			self:sendMsgForCoinsInfo( container, ShopInfoType.TYPE_BUY_ALL);
		end
	end
end

--购买单个金币，或者刷新物品商店
function MarketPageBase:onBuyCoinsOrRefreshDrops( container )
	if SHOP_CURRENT_STATE == ShopSelectedState.STATE_DROPS then
		self:popRefreshBox(container);
	elseif SHOP_CURRENT_STATE == ShopSelectedState.STATE_COINS then

		if marketAdventureInfo.coinCount <= 0 then
			MessageBoxPage:Msg_Box("@ShopCountLimit");
		elseif UserInfo.isGoldEnough(marketAdventureInfo.coinCost) then
			self:sendMsgForCoinsInfo(container, ShopInfoType.TYPE_BUY_SINGLE);
		end
	end
end

--弹出刷新页面
function MarketPageBase:popRefreshBox( container )
	local max = table.maxn(RefreshCostCfg);
	local cost = 0;
	if marketAdventureInfo.refreshCount <= max then
		cost = RefreshCostCfg[marketAdventureInfo.refreshCount].cost;
	else
		cost = RefreshCostCfg[max].cost;
	end

	local refreshFunc = function ( isOK )
		if isOK and UserInfo.isGoldEnough(cost) then
			sendMsgForDropsInfo(container, ShopInfoType.TYPE_REFRESH, 0);
		end
	end

	local title = common:getLanguageString("@ShopRefreshTitle");
	local msg = common:getLanguageString("@ShopRefreshContent",cost);

	PageManager.showConfirm(title, msg, refreshFunc);
end

--帮助页面
function MarketPageBase:onHelp( container )
	if SHOP_CURRENT_STATE == ShopSelectedState.STATE_DROPS then
		PageManager.showHelp(GameConfig.HelpKey.HELP_MARKET_ITEM)
	else
		PageManager.showHelp(GameConfig.HelpKey.HELP_MARKET_GOLD)
	end
	
	
end

--购买单个道具
function MarketItem.buySingleDrop(container)
	local index = container:getTag();
	local item = marketAdventureInfo.dropsItems[index];
	if item == nil then return; end
	
	--local id = MarketItem.getIdByMainInfoIndex(index);
	if ( item.buyType == iConType.type_Coins and not UserInfo.isCoinEnough(item.buyPrice) )
		or ( item.buyType == iConType.type_Gold and not UserInfo.isGoldEnough(item.buyPrice) )
	then
		return;
	end

	sendMsgForDropsInfo(MainContainer, ShopInfoType.TYPE_BUY_SINGLE, item.id);
end

function MarketPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		local extraParam = MsgMainFrameRefreshPage:getTrueType(message).extraParam;
		if pageName == thisPageName then
			--self:refreshBasicInfo(container);
			if extraParam == "TopMsgUpdate" then
			    MarketPageBase:refreshBasicInfo( container )
			    return
			end
			self:refreshPage(container);
		end
	elseif typeId == MSG_MAINFRAME_CHANGEPAGE then
		local pageName = MsgMainFrameChangePage:getTrueType(message).pageName
		if pageName == thisPageName then
			self:refreshPage(container);
		end
	end
end

-------------------------------------------------------------------------


local CommonPage = require("CommonPage");
local MarketPage = CommonPage.newSub(MarketPageBase, thisPageName, option);

function MarketPage_showBuyCoin()
	SHOP_CURRENT_STATE = ShopSelectedState.STATE_COINS;
	if MainFrame:getInstance():getCurShowPageName() == thisPageName then
		PageManager.refreshPage(thisPageName);
	else
		PageManager.changePage(thisPageName);
	end
end

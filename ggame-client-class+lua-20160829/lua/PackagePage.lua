----------------------------------------------------------------------------------
--[[
	FILE:			PackagePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
------------local variable for system api--------------------------------------
local ceil = math.ceil;
--------------------------------------------------------------------------------
local filterPage = "EquipFilterPage";
local batchSellPage = "BatchSellEquipPage";
local PackagePageEx = require("PackagePageEx")
local NodeHelper = require("NodeHelper");
local UserInfo = require("UserInfo");

registerScriptPage(filterPage);
registerScriptPage(batchSellPage);

local HP_pb = require("HP_pb");
local Const_pb = require("Const_pb");

local thisPageName = "PackagePage"

local opcodes = {
	STATE_INFO_SYNC_S = HP_pb.STATE_INFO_SYNC_S
};

local option = {
	ccbiFile = "BackpackPage.ccbi",
	handlerMap = {
		onEquipment				= "showEquipment",
		onGoods					= "showGoods",
		onBatchSell				= "batchSell",
		onEquipmentScreening	= "filterEquip",
		onExpansionBackpack		= "expandPackage"
	},
	opcode = opcodes
};

local PackagePageBase = {}

local EquipOprHelper = require("EquipOprHelper");
local UserItemManager = require("UserItemManager");
local PBHelper = require("PBHelper");
local ItemManager = require("ItemManager");

local ITEM_COUNT_PER_LINE = 5;

local PageType = {
	EQUIPMENT 	= 1,
	GOODS 		= 2
};

local PageInfo = {
	pageType = PageType.EQUIPMENT,
	filter = {
		mainClass = "All",
		subClass = 0
	},
	equipCount = 0,
	itemCount = 0,
	equipIds = {},
	itemIds = {}
};


local function sortEquip(id_1, id_2)
	local isAsc = true;
	
	if id_2 == nil then
		return isAsc;
	end		
	if id_1 == nil then
		return not isAsc;
	end
	
	local userEquip_1 = UserEquipManager:getUserEquipById(id_1);
	local userEquip_2 = UserEquipManager:getUserEquipById(id_2);
	
	local isGodly_1 = UserEquipManager:isEquipGodly(userEquip_1);
	local isGodly_2 = UserEquipManager:isEquipGodly(userEquip_2);
	
	if isGodly_1 ~= isGodly_2 then
		if isGodly_1 then return isAsc; end
		return not isAsc;
	end
	
	local quality_1 = EquipManager:getQualityById(userEquip_1.equipId);
	local quality_2 = EquipManager:getQualityById(userEquip_2.equipId);
	
	if quality_1 ~= quality_2 then
		if quality_1 > quality_2 then
			return isAsc;
		else
			return not isAsc;
		end
	end
	
	local part_1 = EquipManager:getPartById(userEquip_1.equipId);
	local part_2 = EquipManager:getPartById(userEquip_2.equipId);
	
	if part_1 ~= part_2 then
		if GameConfig.PartOrder[part_1] > GameConfig.PartOrder[part_2] then
			return isAsc;
		end
		return not isAsc;
	end
	
	if userEquip_1.score ~= userEquip_2.score then
		if userEquip_1.score > userEquip_2.score then
			return isAsc;
		else
			return not isAsc;
		end
	end
	
	if id_1 > id_2 then
		return isAsc;
	end
	
	return not isAsc;
end
--------------------------------------------------------------
local PackageItem = {
	ccbiFile = "BackpackItem.ccbi"
};	

function PackageItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		PackageItem.onRefreshItemView(container);
	end
end


local function EquipmentItemOnFunction(eventName, container)
	if eventName == "onHand" then
		local index = container:getTag();
		local userEquipId = PageInfo.equipIds[index];
		PageManager.showEquipInfo(userEquipId);
	end
end

local function GoodsItemOnFunction(eventName, container)
	if eventName == "onHand" then
		local index = container:getTag();
		local itemId = PageInfo.itemIds[index];
		local userItem = UserItemManager:getUserItemByItemId(itemId);
		PageManager.showItemInfo(userItem.id);
	end
end

function PackageItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;
	local baseIndex = (contentId - 1) * ITEM_COUNT_PER_LINE;

	for i = 1, ITEM_COUNT_PER_LINE do
		local nodeContainer = container:getVarNode(string.format("mPosition%d", i));
		NodeHelper:setNodeVisible(nodeContainer, false);
		nodeContainer:removeAllChildren();
		
		local itemNode = nil;
		local index = baseIndex + i;
		if PageInfo.pageType == PageType.EQUIPMENT then
			if PageInfo.equipIds[index] then
				itemNode = PackageItem.newEquipmentItem(index);
			end
		else
			if index <= PageInfo.itemCount then
				itemNode = PackageItem.newGoodsItem(index);
			end
		end
		
		if itemNode then
			nodeContainer:addChild(itemNode);
			NodeHelper:setNodeVisible(nodeContainer, true);
		end
	end
end

function PackageItem.newEquipmentItem(index)
	--local itemNode = ScriptContentBase:create("EquipmentItem.ccbi", index);
	local itemNode = CCBManager:getInstance():createAndLoad2("EquipmentItem.ccbi");
	itemNode:setTag(index);
	itemNode:registerFunctionHandler(EquipmentItemOnFunction);
	local userEquip = UserEquipManager:getUserEquipById(PageInfo.equipIds[index]);
	local equipId = userEquip.equipId;

	local lb2Str = {
		mLv		 	= "Lv" .. EquipManager:getLevelById(equipId),
		mLvNum		= "+" .. userEquip.strength
	};
	local nodesVisible = {};
	local sprite2Img = {mPic = EquipManager:getIconById(equipId)};
	
	local gemVisible = false;
	quality = EquipManager:getQualityById(equipId);
	local aniVisible = UserEquipManager:isGodly(userEquip.id);			
	local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
	if table.maxn(gemInfo) > 0 then
		gemVisible = true;
		for i = 1, 4 do
			local gemId = gemInfo[i];
			nodesVisible["mGemBG" .. i] = gemId ~= nil;
			local gemSprite = "mGem0" .. i;
			nodesVisible[gemSprite] = false;
			if gemId ~= nil and gemId > 0 then
			local icon = ItemManager:getGemSmallIcon(gemId);
			if icon then
				nodesVisible[gemSprite] = true;
				sprite2Img[gemSprite] = icon;
				end
			end
		end
	end
	nodesVisible["mAni"]	= aniVisible;
	nodesVisible["mGemNode"]	= gemVisible;
	
	NodeHelper:setStringForLabel(itemNode, lb2Str);
	
	NodeHelper:setSpriteImage(itemNode, sprite2Img, {mPic = 1.0});
	
	NodeHelper:setQualityFrames(itemNode, {mHand = EquipManager:getQualityById(equipId)});
	
	NodeHelper:setNodesVisible(itemNode, nodesVisible);
	
	NodeHelper:addEquipAni(itemNode, "mAni", aniVisible, nil, userEquip);
	
	--itemNode:release();
	return itemNode;
end

function PackageItem.newGoodsItem(nodeIndex)
	local index = nodeIndex;
	--local itemNode = ScriptContentBase:create("GoodsItem.ccbi", index);
	local itemNode = CCBManager:getInstance():createAndLoad2("GoodsItem.ccbi");
	itemNode:setTag(index);
	itemNode:registerFunctionHandler(GoodsItemOnFunction);
	
	local itemId = PageInfo.itemIds[index];
	local userItem = UserItemManager:getUserItemByItemId(itemId);
	local resInfo = ResManagerForLua:getResInfoByMainTypeAndId(Const_pb.TOOL, userItem.itemId, userItem.count);
	local lb2Str = {
		mName 	= resInfo.name,
		mNumber	= "x" .. userItem.count
	};
	NodeHelper:setStringForLabel(itemNode, lb2Str);
	NodeHelper:setSpriteImage(itemNode, {mPic = resInfo.icon});
	NodeHelper:setQualityFrames(itemNode, {mHand = resInfo.quality});
	NodeHelper:setColor3BForLabel(itemNode, {mName = common:getColorFromConfig("Own")});

	--itemNode:release();
	return itemNode;
end

----------------------------------------------------------------------------------
	
-----------------------------------------------
--PackagePageBase页面中的事件处理
----------------------------------------------
function PackagePageBase:onEnter(container)
	container:registerMessage(MSG_SEVERINFO_UPDATE);
	container:registerMessage(MSG_MAINFRAME_POPPAGE);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
	
	NodeHelper:initScrollView(container, "mContent", 4);
	container.scrollview=container:getVarScrollView("mContent");
	if container.scrollview~=nil then
		container:autoAdjustResizeScrollview(container.scrollview);
	end
	
	local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	self:refreshPage(container);
	self:rebuildAllItem(container);
	
	
end

function PackagePageBase:onExit(container)
	container:removeMessage(MSG_SEVERINFO_UPDATE);
	container:removeMessage(MSG_MAINFRAME_POPPAGE);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
	NodeHelper:deleteScrollView(container);
end
----------------------------------------------------------------

function PackagePageBase:refreshPage(container)
	local equipOprVisible = false;
	if PageInfo.pageType == PageType.EQUIPMENT then
		--UserEquipManager:classify();
		PageInfo.equipIds = UserEquipManager:getEquipIdsByClass(PageInfo.filter.mainClass, PageInfo.filter.subClass);
		PageInfo.equipCount = #PageInfo.equipIds;
		table.sort(PageInfo.equipIds, sortEquip);
		self:showCapacityInfo(container);
		self:showTypeInfo(container);
		equipOprVisible = true;
	else
		PageInfo.itemIds = UserItemManager:getUserItemIds();
		table.sort(PageInfo.itemIds);
		PageInfo.itemCount = #PageInfo.itemIds;
		NodeHelper:setStringForLabel(container, {mEquipmentTitle = common:getLanguageString("@AllGoods")});
	end
	NodeHelper:setNodesVisible(container, {
		mEquipmentBtnNode 	= equipOprVisible,
		mEquipmentBtnNode02 	= equipOprVisible
	});		
	self:setTabSelected(container);
end

function PackagePageBase:setTabSelected(container)
	local isEquipSelected = PageInfo.pageType == PageType.EQUIPMENT;
	NodeHelper:setMenuItemSelected(container, {
		mTabEquipment	= isEquipSelected,
		mTabGoods		= not isEquipSelected
	})
end;

function PackagePageBase:showCapacityInfo(container)
	UserInfo.syncStateInfo();
	local bagSize = UserInfo.stateInfo.currentEquipBagSize;
	local capacityStr = PageInfo.equipCount .. "/" .. bagSize;
	NodeHelper:setStringForLabel(container, {mBackpackCapacity = capacityStr});
end

function PackagePageBase:showTypeInfo(container)
	local typeStr = "";
	local mainClass = PageInfo.filter.mainClass;
	local subClass = PageInfo.filter.subClass;
	if mainClass == "All" then
		typeStr = common:getLanguageString("@AllEquip");
	elseif mainClass == "Part" then
		typeStr = common:getLanguageString("@EquipPart_" .. subClass);
	elseif mainClass == "Quality" then
		typeStr = common:getLanguageString("@QualityName_" .. subClass)
			.. common:getLanguageString("@Equipment");
	elseif mainClass == "Godly" then
		typeStr = common:getLanguageString("@GodlyEquip");
	end
	NodeHelper:setStringForLabel(container, {mEquipmentTitle = typeStr});
end

----------------scrollview-------------------------
function PackagePageBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function PackagePageBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function PackagePageBase:buildItem(container)
	local size = 0;
	if PageInfo.pageType == PageType.EQUIPMENT then
		size = PageInfo.equipCount;
	else
		size = PageInfo.itemCount;
	end
	size = ceil(size / ITEM_COUNT_PER_LINE);
	NodeHelper:buildScrollView(container, size, PackageItem.ccbiFile, PackageItem.onFunction);
end
	
----------------click event------------------------
function PackagePageBase:showEquipment(container)
	self:changePageType(container, PageType.EQUIPMENT);
end

function PackagePageBase:showGoods(container)
	self:changePageType(container, PageType.GOODS);
end

function PackagePageBase:changePageType(container, targetType)
	if targetType ~= PageInfo.pageType then
		PageInfo.pageType = targetType;
		self:refreshPage(container);
		self:rebuildAllItem(container);
	else
		self:setTabSelected(container);
	end
end

function PackagePageBase:batchSell(container)
	PageManager.pushPage(batchSellPage);
end

function PackagePageBase:filterEquip(container)
	PageManager.pushPage(filterPage);
end	

function PackagePageBase:expandPackage(container)
	local title = common:getLanguageString("@BuyPackageTitle");
	local count = GameConfig.BuyPackage.Count;
	local cost = GameConfig.BuyPackage.Cost;
	local timesCanBuy = UserInfo.stateInfo.leftEquipBagExtendTimes;
	local msg = common:getLanguageString("@BuyPackageMsg", count, cost, timesCanBuy);
	
	if timesCanBuy <= 0 then
		MessageBoxPage:Msg_Box_Lan("@PackageCannotExpand");
		return;
	end
	
	PageManager.showConfirm(title, msg, function(isSure)
		if isSure and UserInfo.isGoldEnough(cost) then
			EquipOprHelper:expandPackage();
		end
	end);
end

--回包处理
--[[
function PackagePageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.STATE_INFO_SYNC_S then
		UserEquipManager:checkPackage();
		self:showCapacityInfo(container);
		return;
	end
end
--]]

--继承此类的活动如果同时开，消息监听不能同时存在,通过tag来区分
function PackagePageBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();
	if typeId == MSG_SEVERINFO_UPDATE then
		local opcode = MsgSeverInfoUpdate:getTrueType(message).opcode;
		local opcodes = {
			HP_pb.EQUIP_INFO_SYNC_S,
			HP_pb.ITEM_INFO_SYNC_S,
			HP_pb.EQUIP_SELL_S
		}; 

		if common:table_hasValue(opcodes, opcode) then
			self:refreshPage(container);
			self:rebuildAllItem(container);
		elseif opcode == HP_pb.STATE_INFO_SYNC_S then
			self:showCapacityInfo(container);
		end
	elseif typeId == MSG_MAINFRAME_POPPAGE then
		local pageName = MsgMainFramePopPage:getTrueType(message).pageName;
		if pageName == filterPage then
			self:refreshPage(container);
			self:rebuildAllItem(container);
		end
	elseif typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:refreshPage(container);
			self:rebuildAllItem(container);
		end
	end
end

--[[
function PackagePageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function PackagePageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
--]]
------------------------------------------页面开关

local isUseScrollTab = VaribleManager:getInstance():getSetting("IsUseNewPackagePage") or 0
isUseScrollTab = tonumber(isUseScrollTab)

--------------------------------------------------
local CommonPage = require("CommonPage");
if isUseScrollTab == 0 then
    PackagePage = CommonPage.newSub(PackagePageBase, thisPageName, option);
    function PackagePage_setFilter(mainClass, subClass)
        PageInfo.filter = {
            mainClass = mainClass or "All",
            subClass = subClass or 0
        };
    end

    function PackagePage_reset()
        PageInfo = {
            pageType = PageType.EQUIPMENT,
            filter = {
                mainClass = "All",
                subClass = 0
            },
            equipCount = 0,
            itemCount = 0,
            equipIds = {}
        };
    end

    function PackagePage_showItems()
        PageInfo.pageType = PageType.GOODS;
        PageManager.changePage(thisPageName);
    end

    function MainFrame_onBackpackPageBtn()
        PageManager.changePage("PackagePage");
        return 0;
    end
else  
    PackagePage = CommonPage.newSub(PackagePageEx, thisPageName, option);
end



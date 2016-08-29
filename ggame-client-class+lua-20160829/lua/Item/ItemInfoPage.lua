----------------------------------------------------------------------------------
--[[
	FILE:			ItemInfoPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb 		= require("Const_pb");
--------------------------------------------------------------------------------
local gemUpgradePage = "GemUpgradePage";
registerScriptPage(gemUpgradePage);

local thisPageName 	= "ItemInfoPage";
local thisUserItemId	= 0;
local thisItemId = 0;
local thisItemType = Const_pb.NORMAL;
local PageType = {
	GemUpgrade 				= 1,
	SoulStoneUpgrade  		= 2,
}

local opcodes = {
	ITEM_USE_S	= HP_pb.ITEM_USE_S
};

local option = {
	ccbiFile = "BackpackGoodsInfoPopUp.ccbi",
	handlerMap = {
		onClose			= "onClose",
		onButtonMiddle 	= "onBtnMiddle",
		onButtonLeft	= "onBtnLeft",
		onButtonRight	= "onBtnRight"
	}
};

local ItemInfoPageBase = {};

local NodeHelper = require("NodeHelper");
local ItemManager = require("ItemManager");
local UserItemManager = require("UserItemManager");
local ItemOprHelper = require("ItemOprHelper");

--不同道具类型对应按钮的文本（个数对应按钮个数）
local type2BtnName = {
	[Const_pb.NORMAL] 					= {"@Cancel"},
	[Const_pb.FRAGMENT] 				= {"@MergeFragment"},
	[Const_pb.GEM] 						= {"@UpgradeGems"},
	[Const_pb.GIFT] 					= {"@Open"},
	[Const_pb.LUCK_TREASURE] 			= {"@OpenOne", "@OpenTen"},
	[Const_pb.GEM_PACKAGE] 				= {"@OpenOne", "@OpenTen"},
	[Const_pb.WORDS_EXCHANGE_NORMAL] 	= {"@Recycle", "@Exchange"},
	[Const_pb.WORDS_EXCHANGE_SPECIAL] 	= {"@Recycle", "@Exchange"},
	[Const_pb.BOSS_CHALLENGE_TIMES] 	= {"@Use"},
	[Const_pb.ELITE_MAP_BOOK] 	= {"@Use"},
	[Const_pb.ALLOANCE_VITALITY_PILL] 	= {"@Use"},
	[Const_pb.SOUL_STONE]               = {"@Compound"},
	[Const_pb.EXPEDITION_ARMORY]        = {"@GoExpedition"}
};

local lackInfo = {item = nil};
-----------------------------------------------
--ItemInfoPageBase页面中的事件处理
----------------------------------------------
function ItemInfoPageBase:onEnter(container)
	self:refreshPage(container);
	container:registerPacket(HP_pb.ITEM_USE_S);
end

function ItemInfoPageBase:onExit(container)
	container:removePacket(HP_pb.ITEM_USE_S);
end
----------------------------------------------------------------

function ItemInfoPageBase:refreshPage(container)
	self:showItemInfo(container);
	self:showButtons(container);
end

function ItemInfoPageBase:showItemInfo(container)
	local userItem = UserItemManager:getUserItemById(thisUserItemId);
	local itemInfo = ResManagerForLua:getResInfoByMainTypeAndId(Const_pb.TOOL, userItem.itemId, 1);
	local lb2Str = {
		mName 			= itemInfo.name,
		mNumber			= "",
		mGoodsInfoTex 	= common:stringAutoReturn(itemInfo.describe or "", GameConfig.LineWidth.ItemDescribe)
	};
	local sprite2Img = {
		mPic = itemInfo.icon
	};
	local itemImg2Qulity = {
		mHand = itemInfo.quality
	};
	local scaleMap = {mPic = 1.0};	
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
	
	lackInfo.item = userItem.count < GameConfig.Count.PieceToMerge;
end


function ItemInfoPageBase:showButtons(container)
	local userItem = UserItemManager:getUserItemById(thisUserItemId);
	thisItemId = userItem.itemId;
	thisItemType = ItemManager:getTypeById(thisItemId);
	
	local btnName = type2BtnName[thisItemType] or {"@Cancel"};
	local btnCount = #btnName;
	
	local btnVisible = {
		mButtonMiddleNode	= btnCount == 1,
		mButtonDoubleNode	= btnCount == 2
	};
	local btnTex = {};
	if btnCount == 1 then
		btnTex["mButtonMiddle"]	= common:getLanguageString(btnName[1]);
	elseif btnCount == 2 then
		btnTex["mButtonLeft"]	= common:getLanguageString(btnName[1]);
		btnTex["mButtonRight"]	= common:getLanguageString(btnName[2]);
	end
	
	NodeHelper:setStringForLabel(container, btnTex);
	NodeHelper:setNodesVisible(container, btnVisible);
end
----------------click event------------------------
function ItemInfoPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function ItemInfoPageBase:onBtnMiddle(container)
	if thisItemType == Const_pb.GEM then
		GemUpgradePage_setItemId(thisItemId);
		PageManager.pushPage(gemUpgradePage);
	elseif thisItemType == Const_pb.FRAGMENT then
		if lackInfo.item then
			MessageBoxPage:Msg_Box_Lan("@GodlyPieceNotEnough");
		else
			ItemOprHelper:useItem(thisItemId);
		end
	elseif thisItemType == Const_pb.GIFT then
		PageManager.showGiftPackage(thisItemId, function()
			ItemOprHelper:useItem(thisItemId);
		end);
	elseif thisItemType == Const_pb.BOSS_CHALLENGE_TIMES or thisItemType == Const_pb.ELITE_MAP_BOOK or thisItemType == Const_pb.ALLOANCE_VITALITY_PILL then
		ItemOprHelper:useItem(thisItemId);
	elseif thisItemType == Const_pb.EXPEDITION_ARMORY then
	    self:jumpPage();
    elseif thisItemType == Const_pb.SOUL_STONE then
	    -- ItemOprHelper:useItem(thisItemId)
	    ItemManager:setNowSelectItem(thisItemId)
		PageManager.pushPage("SoulStoneUpgradePage");
	end
	self:onClose();
end

function ItemInfoPageBase:onBtnLeft(container)
	if thisItemType == Const_pb.GEM_PACKAGE 
		or thisItemType == Const_pb.LUCK_TREASURE
	then
		self:useItem();
		return;
	elseif thisItemType == Const_pb.WORDS_EXCHANGE_NORMAL then
		self:sellItem();
	elseif thisItemType == Const_pb.WORDS_EXCHANGE_SPECIAL then
		local title = common:getLanguageString("@RecycleTitle");
		local cfg = ConfigManager.getRewardById(GameConfig.WordRecycleRewardId);
		local msg = common:getLanguageString("@RecycleMsg", ResManagerForLua:getResStr(cfg));
		PageManager.showConfirm(title, msg, function(isSure)
			if isSure then
				ItemOprHelper:recycleItem(thisItemId);
			end
		end);
	end
	self:onClose();
end

function ItemInfoPageBase:onBtnRight(container)
	if thisItemType == Const_pb.GEM_PACKAGE 
		or thisItemType == Const_pb.LUCK_TREASURE
	then
		ItemOprHelper:useTenItem(thisItemId);
		return;
	elseif thisItemType == Const_pb.WORDS_EXCHANGE_NORMAL then
		self:jumpPage();
	elseif thisItemType == Const_pb.WORDS_EXCHANGE_SPECIAL then
		RegisterLuaPage("WordExchangeRewardPage");
		WordExchangeRewardPage_setItemId(thisItemId);
		PageManager.pushPage("WordExchangeRewardPage");
	end
	self:onClose();
end

function ItemInfoPageBase:useItem()
	ItemOprHelper:useItem(thisItemId);
end

function ItemInfoPageBase:jumpPage()
	local itemCfg = ItemManager:getItemCfgById(thisItemId);
	if itemCfg.jumpPage ~= "0" then
		PageManager.showActivity(tonumber(itemCfg.jumpPage));
	end
end

function ItemInfoPageBase:sellItem()
	local title = common:getLanguageString("@SellItemTitle");
	local cfg = {{
		type = Const_pb.PLAYER_ATTR * 10000,
		itemId = Const_pb.COIN,
		count = ItemManager:getPriceById(thisItemId)
	}};
	local msg = common:getLanguageString("@SellItemMsg", ResManagerForLua:getResStr(cfg));
	PageManager.showConfirm(title, msg, function(isSure)
		if isSure then
			ItemOprHelper:recycleItem(thisItemId);
		end
	end);
end

--回包处理
function ItemInfoPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.ITEM_USE_S then
		if UserItemManager:getCountByItemId(thisItemId) <= 0 then
			self:onClose();
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ItemInfoPage = CommonPage.newSub(ItemInfoPageBase, thisPageName, option);

function ItemInfoPage_setItemId(itemId)
	thisUserItemId = itemId;
end
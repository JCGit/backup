----------------------------------------------------------------------------------
--[[
	FILE:			EquipSwallowPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local HP_pb = require("HP_pb");
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local table = table;
local math = math;
--------------------------------------------------------------------------------

local thisPageName = "EquipSwallowPage";
local UserInfo = require("UserInfo");
local opcodes = {
	EQUIP_SWALLOW_S = HP_pb.EQUIP_SWALLOW_S,
	ERROR_CODE_S	= HP_pb.ERROR_CODE
};

local COUNT_EQUIPMENT_SOURCE_MAX = 6;

local option = {
	ccbiFile = "EquipmentGobbleUpPopUp.ccbi",
	handlerMap = {
		onHelp					= "showHelp",
		onClose					= "onClose",
		onAutomaticScreening	= "onAutoSelect",
		onGobbleUp				= "onSwallow"
	},
	opcode = opcodes
};
local Order = {"A", "B", "C", "D", "E", "F"};
for i = 1, COUNT_EQUIPMENT_SOURCE_MAX do
	option.handlerMap["on" .. Order[i] .. "Hand"] = "goSelectEquip";
end

local EquipSwallowPageBase = {};

local NodeHelper = require("NodeHelper");
local EquipOprHelper = require("EquipOprHelper");
local PBHelper = require("PBHelper");
local ItemManager = require("ItemManager");

local thisEquipId = 0;
local currentSlotId = 1;
local selectedIds = {};
local originalExpScaleX = nil;
local originalExpScaleX2 = nil;
local btnLock = false;
local lackInfo = {item = false, coin = false};
-----------------------------------------------
--EquipSwallowPageBase页面中的事件处理
----------------------------------------------
function EquipSwallowPageBase:onEnter(container)
	btnLock = false;
	self:registerPacket(container)
	container:registerMessage(MSG_SEVERINFO_UPDATE)
	self:refreshPage(container);
end

function EquipSwallowPageBase:onExecute(container)
end

function EquipSwallowPageBase:onExit(container)
	selectedIds = {};
	self:removePacket(container)
	container:removeMessage(MSG_SEVERINFO_UPDATE)	
end
----------------------------------------------------------------

function EquipSwallowPageBase:refreshPage(container)
	self:showEquipInfo(container);
	self:showGodlyInfo(container);
	self:showCondition(container);
	self:showSourceEquips(container);
end

function EquipSwallowPageBase:showEquipInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	if userEquip == nil or userEquip.id == nil then
		return;
	end
	
	local equipId = userEquip.equipId;
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local lb2Str = {
		mLv 				= "Lv" .. level,
		mLvNum				= "+" .. userEquip.strength,
		mEquipmentName		= common:getLanguageString("@LevelName", level, name)
	};
	local sprite2Img = {
		mPic = EquipManager:getIconById(equipId)
	};
	local itemImg2Qulity = {
		mHand = EquipManager:getQualityById(equipId)
	};
	local scaleMap = {mPic = 1.0};	
	
	local nodesVisible = {};
	local gemVisible = false;
	local aniVisible = UserEquipManager:isEquipGodly(userEquip);			
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
	NodeHelper:setNodesVisible(container, nodesVisible);
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
	
	NodeHelper:addEquipAni(container, "mAni", aniVisible, thisEquipId);
end

function EquipSwallowPageBase:showGodlyInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	local expNeed =  EquipManager:getExpNeedForLevelUp(userEquip.starLevel);
	expNeed = math.max(1, expNeed);
	
	local starLevel2, starExp2 = 1, 0;
	if userEquip:HasField("godlyAttrId2") and userEquip.godlyAttrId2 > 0 then
		starLevel2, starExp2 = userEquip.starLevel2, userEquip.starExp2;
	end
	local expNeed2 =  EquipManager:getExpNeedForLevelUp(starLevel2, true);
	expNeed2 = math.max(1, expNeed2);
		
	local lb2Str = {
		mGodEquipmentLevel 		= "Lv " .. userEquip.starLevel,
		mGodEquipmentExp		= userEquip.starExp .. "/" .. expNeed,
		mGodEquipmentLevel01 	= "Lv " .. starLevel2,
		mGodEquipmentExp01		= starExp2 .. "/" .. expNeed2
	};
	
	local mExpSprite = container:getVarScale9Sprite("mExp");
	if originalExpScaleX == nil then
		originalExpScaleX = mExpSprite:getScaleX();
	end
	mExpSprite:setScaleX(originalExpScaleX * (userEquip.starExp / expNeed));

	local mExpSprite2 = container:getVarScale9Sprite("mExp01");
	if originalExpScaleX2 == nil then
		originalExpScaleX2 = mExpSprite2:getScaleX();
	end
	mExpSprite2:setScaleX(originalExpScaleX2 * (starExp2 / expNeed2));
	
	NodeHelper:setStringForLabel(container, lb2Str);
end

function EquipSwallowPageBase:showCondition(container)	
	local coinCostStr, itemCostStr = '--', '--';
	lackInfo.coin = false;
	lackInfo.item = false;
	
	local validIds = self:getValidIds();
	if #validIds > 0 then
		local coinCost, itemCost = UserEquipManager:getSwallowCost(validIds, thisEquipId);
		UserInfo.syncPlayerInfo();
		coinCostStr = common:getLanguageString("@CurrentOwnInfo", coinCost, UserInfo.playerInfo.coin);
		lackInfo.coin = coinCost > UserInfo.playerInfo.coin;
	--[[
	--注灵之石不消耗了
		if itemCost and itemCost > 0 then
			local UserItemManager = require("UserItemManager");
			local ownNum = UserItemManager:getCountByItemId(GameConfig.ItemId.GodlyEquipStone);
			itemCostStr = ownNum .. "/" .. itemCost;
			lackInfo.item = itemCost > ownNum;
		end
	--]]
	end
	
	local lb2Str = {
		mGold 		= coinCostStr,
		mMaterial 	= itemCostStr
	};
	NodeHelper:setStringForLabel(container, lb2Str);
	
	local colorMap = {
		mGold 		= common:getColorFromConfig(lackInfo.coin and "Lack" or "Own"),
		mMaterial	= common:getColorFromConfig(lackInfo.item and "Lack" or "Own")
	};
	NodeHelper:setColor3BForLabel(container, colorMap);
end

function EquipSwallowPageBase:showSourceEquips(container)
	local lb2Str = {};
	local sprite2Img = {};
	local itemImg2Qulity = {};
	local scaleMap = {};
	local nodesVisible = {};
	
	for i = 1, COUNT_EQUIPMENT_SOURCE_MAX do
		local levelStr = "";
		local enhanceLvStr = "";
		local icon = GameConfig.Image.ClickToSelect;
		local quality = GameConfig.Default.Quality;
		local aniVisible = false;
		local gemVisible = false;
		
		local name= "m" .. Order[i];
		local userEquipId = selectedIds[i];
		if userEquipId then
			local userEquip = UserEquipManager:getUserEquipById(userEquipId);
			local equipId = userEquip.equipId;
			levelStr = "Lv" .. EquipManager:getLevelById(equipId);
			enhanceLvStr = "+" .. userEquip.strength;
			icon = EquipManager:getIconById(equipId);
			scaleMap[name .. "Pic"]			= 1.0;
			quality = EquipManager:getQualityById(equipId);
			aniVisible = UserEquipManager:isEquipGodly(userEquip);			
			local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
			if table.maxn(gemInfo) > 0 then
				gemVisible = true;
				for i = 1, 4 do
					local gemId = gemInfo[i];
					nodesVisible[name .. "GemBG" .. i] = gemId ~= nil;
					local gemSprite = name .. "Gem0" .. i;
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
		end
		
		lb2Str[name .. "Lv"] 			= levelStr;
		lb2Str[name .. "LvNum"]			= enhanceLvStr;
		sprite2Img[name .. "Pic"] 		= icon;
		itemImg2Qulity[name .. "Hand"] 	= quality;
		scaleMap[name .. "Pic"] 		= 1.0;
		nodesVisible[name .. "Ani"]		= aniVisible;
		nodesVisible[name .. "GemNode"]	= gemVisible;
		
		NodeHelper:addEquipAni(container, name .. "Ani", aniVisible, userEquipId);
	end
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setNodesVisible(container, nodesVisible);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
end
	
----------------click event------------------------
function EquipSwallowPageBase:showHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_SWALLOW);
end

function EquipSwallowPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end	

function EquipSwallowPageBase:onSwallow(container)
	if btnLock then return; end
	
	if not (UserEquipManager:canSwallow(thisEquipId)) then
		MessageBoxPage:Msg_Box_Lan("@EquipmentStarLevelHighest");
		return;
	end	
	
	local validIds = self:getValidIds();
	if #validIds > 0 then
		for _, id in ipairs(validIds) do
			if UserEquipManager:hasGem(id) then
				MessageBoxPage:Msg_Box_Lan("@SelectedEquipHasGem");
				return;
			end
		end
		
		if lackInfo.item then
			MessageBoxPage:Msg_Box_Lan("@GodlyEquipStoneNotEnough");
			return;
		elseif lackInfo.coin then
			PageManager.notifyLackCoin();
			return;
		end
		
		EquipOprHelper:swallowEquip(thisEquipId, validIds);
		btnLock = true;
	else
		MessageBoxPage:Msg_Box_Lan("@PlzSelectGodlyEquip");
	end
end

function EquipSwallowPageBase:onAutoSelect(container)
	if not (UserEquipManager:canSwallow(thisEquipId)) then
		MessageBoxPage:Msg_Box_Lan("@EquipmentStarLevelHighest");
		return;
	end
	local ids = UserEquipManager:getEquipIdsForSwallow(thisEquipId, true) or {};
	if #ids <= 0 then
		MessageBoxPage:Msg_Box_Lan("@NoGodlyEquipToSelect");
		return;
	end
	selectedIds = common:table_tail(ids, COUNT_EQUIPMENT_SOURCE_MAX);
	self:showSourceEquips(container);
	self:showCondition(container);
end

function EquipSwallowPageBase:goSelectEquip(container, eventName)
	local pos = common:table_arrayIndex(Order, eventName:sub(3):sub(1, -5));
	if selectedIds[pos] then
		selectedIds[pos] = nil;
		self:refreshPage(container);
		return;
	end
	
	if not (UserEquipManager:canSwallow(thisEquipId)) then
		MessageBoxPage:Msg_Box_Lan("@EquipmentStarLevelHighest");
		return;
	end
	
	local validIds = self:getValidIds();
	EquipSelectPage_multiSelect(validIds, COUNT_EQUIPMENT_SOURCE_MAX, function(ids)
		if not common:table_isSame(validIds, ids) then
			selectedIds = ids;
		end
		self:showSourceEquips(container);
		self:showCondition(container);
	end, thisEquipId, EquipFilterType.Swallow);
	PageManager.pushPage("EquipSelectPage");
end

function EquipSwallowPageBase:getValidIds()
	return common:table_arrayFilter(selectedIds, function(id)
		return id ~= nil;
	end);
end

--回包处理
function EquipSwallowPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	--local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.EQUIP_SWALLOW_S then
		selectedIds = {};
		self:refreshPage(container);
		btnLock = false;
	elseif opcode == opcodes.ERROR_CODE_S then
		btnLock = false;
	end
end

function EquipSwallowPageBase:onPacketError(container)
	btnLock = false;
end

function EquipSwallowPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function EquipSwallowPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipSwallowPage = CommonPage.newSub(EquipSwallowPageBase, thisPageName, option);

function EquipSwallowPage_setEquipId(equipId)
	thisEquipId = equipId;
end	


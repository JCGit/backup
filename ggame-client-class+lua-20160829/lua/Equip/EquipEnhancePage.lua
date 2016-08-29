----------------------------------------------------------------------------------
--[[
	FILE:			EquipEnhancePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb 		= require("Const_pb");
local EquipOpr_pb 	= require("EquipOpr_pb");
local HP_pb			= require("HP_pb");
local UserInfo = require("UserInfo");
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local table = table;
--------------------------------------------------------------------------------

local thisPageName 	= "EquipEnhancePage";
local thisEquipId	= 0;

local opcodes = {
	EQUIP_ENHANCE_S	= HP_pb.EQUIP_ENHANCE_S
};

local option = {
	ccbiFile = "EquipmentEnhancePopUp.ccbi",
	handlerMap = {
		onEnhance			= "onEnhance",
		onHelp				= "onHelp",
		onClose				= "onClose"
	},
	opcode = opcodes
};

local EquipEnhancePageBase = {};

local NodeHelper = require("NodeHelper");
local PBHelper = require("PBHelper");
local EquipOprHelper = require("EquipOprHelper");
local UserItemManager = require("UserItemManager");
local ItemManager = require("ItemManager");

local lackInfo = {item = false, coin = false};
-----------------------------------------------
--EquipEnhancePageBase页面中的事件处理
----------------------------------------------
function EquipEnhancePageBase:onEnter(container)
	self:registerPacket(container);
	self:refreshPage(container);
end

function EquipEnhancePageBase:onExit(container)
	self:removePacket(container);
end
----------------------------------------------------------------

function EquipEnhancePageBase:refreshPage(container)
	self:showEquipInfo(container);
	self:showEnhanceInfo(container);
end

function EquipEnhancePageBase:showEquipInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	if userEquip == nil or userEquip.id == nil then
		return;
	end
	
	local equipId = userEquip.equipId;
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local quality = EquipManager:getQualityById(equipId);
	local lb2Str = {
		mLv 			= "Lv" .. level .. "\n" .. EquipManager:getPartNameById(equipId),
		mLvNum			= "+" .. userEquip.strength,
		mEquipmentName	= "", --common:getLanguageString("@LevelName", level, name),
		mEquipmentTex	= "" --UserEquipManager:getEquipInfoString(thisEquipId)
	};
	local sprite2Img = {
		mPic = EquipManager:getIconById(equipId)
	};
	local itemImg2Qulity = {
		mHand = quality
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
	
	local tag = GameConfig.Tag.HtmlLable;
	local nameStr = common:getLanguageString("@LevelName", level, name);
	nameStr = common:fillHtmlStr("Quality_" .. quality, nameStr);
	local nameNode = container:getVarNode("mEquipmentName");
	
	local _label = NodeHelper:addHtmlLable(nameNode, nameStr, tag);
	local stepLevel = EquipManager:getEquipStepById(equipId)	
	local starSprite = container:getVarSprite("mStar")
	local posX = _label:getContentSize().width + _label:getPositionX()
	local posY = _label:getPositionY() - ( _label:getContentSize().height - starSprite:getContentSize().height )/2
	EquipManager:setStarPosition(starSprite, stepLevel == GameConfig.ShowStepStar, posX, posY)

	local str = UserEquipManager:getEquipInfoString(thisEquipId);
	local lbNode = container:getVarNode("mEquipmentTex");
	NodeHelper:addHtmlLable(lbNode, str, tag + 1);
	
	NodeHelper:addEquipAni(container, "mAni", aniVisible, thisEquipId);
end

function EquipEnhancePageBase:showEnhanceInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);

	local levelLimit = GameConfig.LevelLimit.EnhanceLevelMax or 15;
	local isMax = userEquip.strength >= levelLimit;
	NodeHelper:setNodesVisible(container, {
		mEnhanceCommon	= not isMax,
		mEnhanceHighest	= isMax
	});
	if isMax then return; end
	
	local quality = EquipManager:getQualityById(userEquip.equipId);
	local currVal = EquipManager:getAttrAddVAl(quality, userEquip.strength) or 0;
	local nextVal = EquipManager:getAttrAddVAl(quality, userEquip.strength + 1) or currVal;
	local needItem = UserEquipManager:getItemNeedForEnhance(thisEquipId);
	local ownItem = UserItemManager:getCountByItemId(GameConfig.ItemId.EnhanceElite);
	local needCoin = UserEquipManager:getEnhanceCoinCost(userEquip.equipId, needItem);
	UserInfo.syncPlayerInfo();
	local addStr = string.format("%.1f", (nextVal - currVal) / 100);
	local lb2Str = {
		mEnhanceEffect	= common:getLanguageString("@EquipEnhanceEffect", addStr),
		mEssence		= common:getLanguageString("@CurrentOwnInfo", needItem, ownItem),
		mGold			= common:getLanguageString("@CurrentOwnInfo", needCoin, UserInfo.playerInfo.coin)
	};
	lackInfo.item = needItem > ownItem;
	lackInfo.coin = needCoin > UserInfo.playerInfo.coin;
	
	NodeHelper:setStringForLabel(container, lb2Str);
	local colorMap = {
		mEssence 	= common:getColorFromConfig(lackInfo.item and "Lack" or "Own"),
		mGold 		= common:getColorFromConfig(lackInfo.coin and "Lack" or "Own")
	};
	NodeHelper:setColor3BForLabel(container, colorMap);
end
	
----------------click event------------------------
function EquipEnhancePageBase:onEnhance(container)
	if lackInfo.item then
		MessageBoxPage:Msg_Box_Lan("@EnhanceEliteNotEnough");
	elseif lackInfo.coin then
		PageManager.notifyLackCoin();
	else
		EquipOprHelper:enhanceEquip(thisEquipId);
	end
end

function EquipEnhancePageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_ENHANCE);
end	

function EquipEnhancePageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--回包处理
function EquipEnhancePageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.EQUIP_ENHANCE_S then
		self:refreshPage(container);
		return
	end
end

function EquipEnhancePageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function EquipEnhancePageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipEnhancePage = CommonPage.newSub(EquipEnhancePageBase, thisPageName, option);

function EquipEnhancePage_setEquipId(equipId)
	thisEquipId = equipId;
end

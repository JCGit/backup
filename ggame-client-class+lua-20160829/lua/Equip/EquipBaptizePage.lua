----------------------------------------------------------------------------------
--[[
	FILE:			EquipBaptizePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	装备洗炼
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

local thisPageName 	= "EquipBaptizePage";
local thisEquipId	= 0;

local opcodes = {
	EQUIP_BAPTIZE_S	= HP_pb.EQUIP_BAPTIZE_S
};

local option = {
	ccbiFile = "EquipmentRefinementPopUp.ccbi",
	handlerMap = {
		onRefinement 	= "onBaptize",
		onHelp 			= "onHelp",
		onClose			= "onClose"
	},
	opcode = opcodes
};

local EquipBaptizePageBase = {};

local NodeHelper = require("NodeHelper");
local PBHelper = require("PBHelper");
local EquipOprHelper = require("EquipOprHelper");
local ItemManager = require("ItemManager");

local lackInfo = {coin = false};
-----------------------------------------------
--EquipBaptizePageBase页面中的事件处理
----------------------------------------------
function EquipBaptizePageBase:onEnter(container)
	self:registerPacket(container);
	self:showSecondAttrInfo(container);
	self:refreshPage(container);
end

function EquipBaptizePageBase:onExit(container)
	self:removePacket(container);
end
----------------------------------------------------------------

function EquipBaptizePageBase:refreshPage(container)
	self:showEquipInfo(container);
	self:showBaptizeInfo(container);
end

function EquipBaptizePageBase:showEquipInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	if userEquip == nil or userEquip.id == nil then
		return;
	end
	
	local equipId = userEquip.equipId;
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local quality = EquipManager:getQualityById(equipId);
	local lb2Str = {
		mLv 				= "Lv" .. level,
		mLvNum				= "+" .. userEquip.strength,
		mEquipmentName		= "",
		mEquipmentTex		= ""
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

function EquipBaptizePageBase:showBaptizeInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	local cost = EquipManager:getBaptizeCost(userEquip.equipId);
	local lb2Str = {
		mGold = common:getLanguageString("@CurrentOwnInfo", cost, UserInfo.playerInfo.coin)
	};
	
	NodeHelper:setStringForLabel(container, lb2Str);
	
	lackInfo.coin = cost > UserInfo.playerInfo.coin;
	local colorMap = {
		mGold 		= common:getColorFromConfig(lackInfo.coin and "Lack" or "Own")
	};
	NodeHelper:setColor3BForLabel(container, colorMap);
end

function EquipBaptizePageBase:showSecondAttrInfo(container)
	local lb2Str = {
		mExplain = common:getLanguageString("@SecondAttrInclude")
	};
	
	NodeHelper:setStringForLabel(container, lb2Str);
end
	
----------------click event------------------------
function EquipBaptizePageBase:onBaptize(container)
	if lackInfo.coin then
		PageManager.notifyLackCoin();
	else
		EquipOprHelper:baptizeEquip(thisEquipId);
	end
end

function EquipBaptizePageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_BAPTIZE);
end	

function EquipBaptizePageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--回包处理
function EquipBaptizePageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.EQUIP_BAPTIZE_S then
		UserInfo.syncPlayerInfo();
		self:refreshPage(container);
		return
	end
end

function EquipBaptizePageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function EquipBaptizePageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipBaptizePage = CommonPage.newSub(EquipBaptizePageBase, thisPageName, option);

function EquipBaptizePage_setEquipId(equipId)
	thisEquipId = equipId;
end

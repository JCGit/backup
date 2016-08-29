----------------------------------------------------------------------------------
--[[
	FILE:			EquipBuildPage.lua
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

local thisPageName 	= "EquipBuildPage";
local thisEquipId	= 0;

local opcodes = {
	EQUIP_SMELT_CREATE_S	= HP_pb.EQUIP_SMELT_CREATE_S,
	EQUIP_SMELT_INFO_S		= HP_pb.EQUIP_SMELT_INFO_S
};

local option = {
	ccbiFile = "RefiningBuildingEquipmentPopUp.ccbi",
	handlerMap = {
		onRefresh			= "onRefresh",
		onCreate			= "onBuild",
		onHelp				= "onHelp",
		onClose				= "onClose"
	},
	opcode = opcodes
};

local EquipBuildPageBase = {};

local NodeHelper = require("NodeHelper");
local EquipOprHelper = require("EquipOprHelper");
local PBHelper = require("PBHelper");
local ItemManager = require("ItemManager");

local lackInfo = {semltValue = false};
-----------------------------------------------
--EquipBuildPageBase页面中的事件处理
----------------------------------------------
function EquipBuildPageBase:onEnter(container)
	self:registerPacket(container);
	self:refreshPage(container);
end

function EquipBuildPageBase:onExit(container)
	self:removePacket(container);
end
----------------------------------------------------------------

function EquipBuildPageBase:refreshPage(container)
	self:showTargetInfo(container);
	self:showBuildInfo(container);
end

function EquipBuildPageBase:showTargetInfo(container)
	if UserInfo.smeltInfo:HasField("equipInfo") then
		self:showEquipInfo(container);
	elseif UserInfo.smeltInfo:HasField("itemInfo") then
		self:showItemInfo(container);
	else
		PageManager.popPage(thisPageName);
	end
end

function EquipBuildPageBase:showEquipInfo(container)
	local equipInfo = UserInfo.smeltInfo.equipInfo;
	local equipId = equipInfo.equipId;
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local quality = EquipManager:getQualityById(equipId);
	local lb2Str = {
		mLv 				= "Lv" .. level,
		mEquipmentName		= "",--common:getLanguageString("@LevelName", level, name),
		mEquipmentInfoTex	= "" --UserEquipManager:getDesciptionWithEquipInfo(equipInfo)
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
	local aniVisible = UserEquipManager:isEquipGodly(equipInfo);			
	local gemInfo = PBHelper:getGemInfo(equipInfo.gemInfos);
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
	NodeHelper:addHtmlLable(nameNode, nameStr, tag);
	
	local str = UserEquipManager:getDesciptionWithEquipInfo(equipInfo, true);
	local lbNode = container:getVarNode("mEquipmentInfoTex");
	NodeHelper:addHtmlLable(lbNode, str, tag + 1);
	
	NodeHelper:addEquipAni(container, "mAni", aniVisible, nil, equipInfo);
end

function EquipBuildPageBase:showItemInfo(container)
	local tag = GameConfig.Tag.HtmlLable;
	local nameNode = container:getVarNode("mEquipmentName");
	nameNode:getParent():removeChildByTag(tag, true);
	local lbNode = container:getVarNode("mEquipmentInfoTex");
	lbNode:getParent():removeChildByTag(tag + 1, true);
	 
	local itemInfo = UserInfo.smeltInfo.itemInfo;
	local resInfo = ResManagerForLua:getResInfoByMainTypeAndId(Const_pb.TOOL, itemInfo.itemId, itemInfo.count) or {};
	
	local lb2Str = {
		mLv 				= "",
		mEquipmentName		= resInfo.name or "",
		mEquipmentInfoTex	= common:stringAutoReturn(resInfo.describe or "", GameConfig.LineWidth.BuildEquip)
	};
	local sprite2Img = {
		mPic = resInfo.icon
	};
	local itemImg2Qulity = {
		mHand = resInfo.quality
	};
	local scaleMap = {mPic = 1.0};	
	
	local nodesVisible = {};
	local gemVisible = false;
	local aniVisible = false;			
	nodesVisible["mAni"]	= aniVisible;
	nodesVisible["mGemNode"]	= gemVisible;
	NodeHelper:setNodesVisible(container, nodesVisible);
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
end

function EquipBuildPageBase:showBuildInfo(container)
	local needSmeltValue = 0;
	if UserInfo.smeltInfo:HasField("equipInfo") then
		local equipInfo = UserInfo.smeltInfo.equipInfo;
		if UserEquipManager:isEquipGodly(equipInfo) then
			needSmeltValue = GameConfig.Cost.BuildGodlyEquip;
		else
			needSmeltValue = EquipManager:getSmeltNeedById(equipInfo.equipId);
		end
	elseif UserInfo.smeltInfo:HasField("itemInfo") then
		local itemInfo = UserInfo.smeltInfo.itemInfo;
		needSmeltValue = ItemManager:getSmeltNeedById(itemInfo.itemId);
	end
	local lb2Str = {
		mCreateRefiningNum 	= common:getLanguageString("@CurrentOwnInfo", needSmeltValue, UserInfo.playerInfo.smeltValue),
		mFreeRefreshNum		= UserInfo.smeltInfo.freeRefreshTimes
	};
	
	NodeHelper:setStringForLabel(container, lb2Str);
	
	lackInfo.smeltValue = needSmeltValue > UserInfo.playerInfo.smeltValue;
	local colorMap = {
		mCreateRefiningNum 	= common:getColorFromConfig(lackInfo.smeltValue and "Lack" or "Own")
	};
	NodeHelper:setColor3BForLabel(container, colorMap);
end

----------------click event------------------------
function EquipBuildPageBase:onBuild(container)
	if lackInfo.smeltValue then
		MessageBoxPage:Msg_Box_Lan("@SmeltValueNotEnough");
	else
		EquipOprHelper:buildEquip(thisEquipId);
	end
end

local function doRefresh()
	EquipOprHelper:refreshEquipBulding();
end

function EquipBuildPageBase:onRefresh(container)
	if UserInfo.smeltInfo.freeRefreshTimes > 0 then
		doRefresh();
	else
		local title = common:getLanguageString("@ConfirmToRefresh_Title");
		local msg = common:getLanguageString("@ConfirmToRefresh_Msg", GameConfig.Cost.RefreshBuildingEquip);
		PageManager.showConfirm(title, msg, function(isSure)
			if isSure and UserInfo.isGoldEnough(GameConfig.Cost.RefreshBuildingEquip) then
				doRefresh();
			end
		end);
	end
end

function EquipBuildPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_FORGE);
end	

function EquipBuildPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--回包处理
function EquipBuildPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if common:table_hasValue(opcodes, opcode) then
		if opcode == opcodes.EQUIP_SMELT_INFO_S then
			EquipOprHelper:syncSMeltInfo(msgBuff);
			UserInfo.syncPlayerInfo();
		end
		self:refreshPage(container);
	end
end

function EquipBuildPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function EquipBuildPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipBuildPage = CommonPage.newSub(EquipBuildPageBase, thisPageName, option);

function EquipBuildPage_setEquipId(equipId)
	thisEquipId = equipId;
end

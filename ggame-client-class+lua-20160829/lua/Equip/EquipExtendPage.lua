----------------------------------------------------------------------------------
--[[
	FILE:			EquipExtendPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	能量之核: 探索、拼合、列表页面
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

local thisPageName 	= "EquipExtendPage";
local thisEquipId	= 0;
local targetEquipId = 0;

local opcodes = {
	EQUIP_EXTEND_S	= HP_pb.EQUIP_EXTEND_S
};

local option = {
	ccbiFile = "EquipmentInheritPopUp.ccbi",
	handlerMap = {
		onInherit			= "onExtend",
		onAnotherHand		= "onSelect",
		onHelp				= "onHelp",
		onClose				= "onClose"
	},
	opcode = opcodes
};

local EquipExtendPageBase = {};

local NodeHelper = require("NodeHelper");
local EquipOprHelper = require("EquipOprHelper");
local PBHelper = require("PBHelper");
local ItemManager = require("ItemManager");

local hasExtended = false;
local lackInfo = {coin = false};
-----------------------------------------------
--EquipExtendPageBase页面中的事件处理
----------------------------------------------
function EquipExtendPageBase:onEnter(container)
	hasExtended = false;
	self:registerPacket(container);
	self:refreshPage(container);
end

function EquipExtendPageBase:onExit(container)
	hasExtended = false;
	targetEquipId = 0;
	self:removePacket(container);
end
----------------------------------------------------------------

function EquipExtendPageBase:refreshPage(container)
	self:showEquipInfo(container);
	self:showTargetEquipInfo(container);
	self:showExtendInfo(container);
	self:showConsumeInfo(container);
end

function EquipExtendPageBase:showEquipInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	if userEquip == nil or userEquip.id == nil then
		return;
	end
	
	local equipId = userEquip.equipId;
	local level = EquipManager:getLevelById(equipId);
	local name	= EquipManager:getNameById(equipId);
	local lb2Str = {
		mLv 		= "Lv" .. level,
		mLvNum		= "+" .. userEquip.strength
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

function EquipExtendPageBase:showTargetEquipInfo(container)
	local lb2Str = {
		mAnotherLv  		= "",
		mAnotherLvNum		= "",
	};
	local sprite2Img = {
		mAnotherPic = GameConfig.Image.ClickToSelect
	};
	local itemImg2Qulity = {
		mAnotherHand = GameConfig.Default.Quality
	};
	local scaleMap = {mAnotherPic = 1.0};
	local aniVisible = false;
	
	if targetEquipId and targetEquipId > 0 then
		local userEquip = UserEquipManager:getUserEquipById(targetEquipId);
		if userEquip == nil or userEquip.id == nil then
			return;
		end
	
		local equipId = userEquip.equipId;
		local level = EquipManager:getLevelById(equipId);
		local name	= EquipManager:getNameById(equipId);
		lb2Str = {
			mAnotherLv 				= "Lv" .. level,
			mAnotherLvNum			= "+" .. userEquip.strength
		};
		sprite2Img = {
			mAnotherPic = EquipManager:getIconById(equipId)
		};
		itemImg2Qulity = {
			mAnotherHand = EquipManager:getQualityById(equipId)
		};
		
		local nodesVisible = {};
		local gemVisible = false;
		aniVisible = UserEquipManager:isEquipGodly(userEquip);			
		local gemInfo = PBHelper:getGemInfo(userEquip.gemInfos);
		if table.maxn(gemInfo) > 0 then
			gemVisible = true;
			for i = 1, 4 do
				local gemId = gemInfo[i];
				nodesVisible["mAnotherGemBG" .. i] = gemId ~= nil;
				local gemSprite = "mAnotherGem0" .. i;
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
		nodesVisible["mAnotherAni"]	= aniVisible;
		nodesVisible["mAnotherGemNode"]	= gemVisible;
		NodeHelper:setNodesVisible(container, nodesVisible);
	end
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
	
	NodeHelper:addEquipAni(container, "mAnotherAni", aniVisible, targetEquipId);
end

function EquipExtendPageBase:showExtendInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	local texNode = container:getVarNode("mInheritAttributeTex");
	texNode:setVisible(false);
	local str = common:fill('<p>#v1#</p>', UserEquipManager:getMainGodlyAttr(userEquip));
	NodeHelper:addHtmlLable(texNode, str, GameConfig.Tag.HtmlLable);
end
	
function EquipExtendPageBase:showConsumeInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	local coinCost = "--";
	lackInfo.coin = false;
	UserInfo.syncPlayerInfo();
	if not hasExtended and targetEquipId and targetEquipId > 0 then
		coinCost =  UserEquipManager:getExtendCoinCost(thisEquipId, targetEquipId);
		lackInfo.coin = coinCost > UserInfo.playerInfo.coin;
	end

	local lb2Str = {
		mGold = common:getLanguageString("@CurrentOwnInfo", coinCost, UserInfo.playerInfo.coin)
	};
	
	NodeHelper:setStringForLabel(container, lb2Str);
	
	local colorMap = {
		mGold = common:getColorFromConfig(lackInfo.coin and "Lack" or "Own")
	};
	NodeHelper:setColor3BForLabel(container, colorMap);
end
----------------click event------------------------
function EquipExtendPageBase:onSelect(container)
	if hasExtended then return; end
	local selectedIds = targetEquipId and targetEquipId > 0 and {targetEquipId} or {};
	EquipSelectPage_multiSelect(selectedIds, 1, function(ids)
		targetEquipId = ids[1];
		self:showTargetEquipInfo(container);
		self:showConsumeInfo(container);
	end, thisEquipId, EquipFilterType.Extend);
	PageManager.pushPage("EquipSelectPage");
end

function EquipExtendPageBase:onExtend(container)
	if targetEquipId and targetEquipId > 0 then
		if hasExtended then return; end
		if lackInfo.coin then
			PageManager.notifyLackCoin();
			return;
		end
		EquipOprHelper:extendEquip(thisEquipId, targetEquipId);
		hasExtended = true;
	else
		MessageBoxPage:Msg_Box("@PlzSelectExtendTarget");
	end
end

function EquipExtendPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_EXTEND);
end	

function EquipExtendPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--回包处理
function EquipExtendPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.EQUIP_EXTEND_S then
		self:refreshPage(container);
		common:popString(common:getLanguageString('@ExtendSuccess'), 'COLOR_GREEN');
		return
	end
end

function EquipExtendPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function EquipExtendPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipExtendPage = CommonPage.newSub(EquipExtendPageBase, thisPageName, option);


function EquipExtendPage_setEquipId(equipId)
	thisEquipId = equipId;
end

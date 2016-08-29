----------------------------------------------------------------------------------
--[[
	FILE:			EquipEmbedPage.lua
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
local gemSelectPage = "GemSelectPage";
registerScriptPage(gemSelectPage);

local thisPageName 	= "EquipEmbedPage";
local thisEquipId	= 0;
local COUNT_SLOT = 4;

local opcodes = {
	EQUIP_STONE_UNDRESS_S	= HP_pb.EQUIP_STONE_UNDRESS_S,
	EQUIP_PUNCH_S			= HP_pb.EQUIP_PUNCH_S,
	EQUIP_STONE_DRESS_S		= HP_pb.EQUIP_STONE_DRESS_S
};

local option = {
	ccbiFile = "EquipmentCameoIncrustationPopUp.ccbi",
	handlerMap = {
		onFastUnload		= "onFastUnload",
		onHelp				= "onHelp",
		onClose				= "onClose"
	},
	opcode = opcodes
};
for i = 1, COUNT_SLOT do
	option.handlerMap["onGemHand" .. i] = "onGemOpr"	
end

local EquipEmbedPageBase = {};

local NodeHelper = require("NodeHelper");
local PBHelper = require("PBHelper");
local EquipOprHelper = require("EquipOprHelper");
local ItemManager = require("ItemManager");

local GemStatus = {
	NoSlot = -1,
	NoGem	= 0
	};
local gemPos2Id = {};
local minUnlockSlot = COUNT_SLOT + 1;
local gemCount = 0;
local gemQuality = {};
-----------------------------------------------
--EquipEmbedPageBase页面中的事件处理
----------------------------------------------
function EquipEmbedPageBase:onEnter(container)
	self:registerPacket(container);
	self:showEmbedTip(container);
	self:refreshPage(container);
end

function EquipEmbedPageBase:onExit(container)
	self:removePacket(container);
end
----------------------------------------------------------------

function EquipEmbedPageBase:refreshPage(container)
	self:showEquipInfo(container);
	self:showGemInfo(container);
end

function EquipEmbedPageBase:showEmbedTip(container)
	local lb2Str = {
		mExplain = common:getLanguageString("@EmbedGemMsg", GameConfig.OpenLevel.GemPunch)
	};
	NodeHelper:setStringForLabel(container, lb2Str);
end
	
function EquipEmbedPageBase:showEquipInfo(container)
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
		mEquipmentIName		= common:getLanguageString("@LevelName", level, name)
	};
	local sprite2Img = {
		mPic = EquipManager:getIconById(equipId)
	};
	local itemImg2Qulity = {
		mHand = EquipManager:getQualityById(equipId)
	};
	local scaleMap = {mPic = 1.0};
	
	gemQuality = {};
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
				local quality = ItemManager:getQualityById(gemId);
				table.insert(gemQuality, tonumber(quality));
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

function EquipEmbedPageBase:showGemInfo(container)
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	
	local lb2Str = {};
	local sprite2Img = {};
	local itemImg2Quality = {};
	
	minUnlockSlot = COUNT_SLOT + 1;
	gemCount = 0;
	for _, gemInfo in ipairs(userEquip.gemInfos) do
		local pos = gemInfo.pos;
		local gemId = gemInfo.gemItemId;
		gemPos2Id[pos] = gemId;
		
		local nameLb = "mGemName" .. pos;
		local numLb = "mGemNum" .. pos;
		local pic = "mGemPic" .. pos;
		local frame = "mGemHand" .. pos;

		lb2Str[nameLb] = "";
		lb2Str[numLb] = "";
		itemImg2Quality[frame] = GameConfig.Default.Quality;
		
		local hasSlot = gemId ~= GemStatus.NoSlot;
		if hasSlot then
			local hasGem = gemId ~= GemStatus.NoGem;
			if hasGem then
				lb2Str[nameLb] = ItemManager:getNameById(gemId);
				lb2Str[numLb] = ItemManager:getGemAttrString(gemId);
				sprite2Img[pic] = ItemManager:getIconById(gemId);
				gemCount = gemCount + 1;
			else
				sprite2Img[pic] = GameConfig.Image.ClickToSelect;
			end
		else
			lb2Str[nameLb] = common:getLanguageString("@ClickToPunch");
			sprite2Img[pic] = GameConfig.Image.PunchSlot;
			minUnlockSlot = math.min(minUnlockSlot, pos);
		end
	end
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setSpriteImage(container, sprite2Img);
	NodeHelper:setQualityFrames(container, itemImg2Quality);
end
	
----------------click event------------------------
function EquipEmbedPageBase:onFastUnload(container)
	if gemCount > 0 then
		EquipOprHelper:unloadAllGem(thisEquipId);
	else
		MessageBoxPage:Msg_Box_Lan("@NoneEmbeded");
	end
end

function EquipEmbedPageBase:onGemOpr(container, eventName)
	local pos = tonumber(eventName:sub(-1));
	local gemId = gemPos2Id[pos];

	local hasSlot = gemId ~= GemStatus.NoSlot;
	if hasSlot then
		local hasGem = gemId ~= GemStatus.NoGem;
		if not hasGem then
			GemSelectPage_setEquipIdAndPos(thisEquipId, pos, gemQuality);
			PageManager.pushPage(gemSelectPage);
		else
			EquipOprHelper:unloadGem(thisEquipId, pos);
		end 	
	else
		local openLevel = GameConfig.OpenLevel.GemPunch;
		if UserInfo.roleInfo.level < openLevel then
			MessageBoxPage:Msg_Box(common:getLanguageString("@EmbedGemMsg", openLevel));
		else
			self:doPunch(pos);
		end
	end
end	

function EquipEmbedPageBase:doPunch(pos)
	local title = common:getLanguageString("@PunchSlot_Title");
	if pos > minUnlockSlot then
		local msg = common:getLanguageString("@PlzPunchPreSlot", minUnlockSlot);
		PageManager.showNotice(title, msg);
		return;
	end
	
	local sureToPunch = function(isSure)
		if isSure then
			EquipOprHelper:punchEquip(thisEquipId, pos);
		end
	end;
	
	local userEquip = UserEquipManager:getUserEquipById(thisEquipId);
	local consume = EquipManager:getPunchConsume(userEquip.equipId, pos);
	if consume == nil then
		sureToPunch(true);
		return;
	end
	
	local resInfo = ResManagerForLua:getResInfoByTypeAndId(consume.type, consume.id, consume.count);
	
	local msg = common:getLanguageString("@PunchSlot_Msg", resInfo.name, consume.count);
	PageManager.showConfirm(title, msg, function(isSure)
		if isSure and ResManagerForLua:checkConsume(consume.type, consume.id, consume.count) then
			sureToPunch(true);
		end
	end);
end	

function EquipEmbedPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_EMBED);
end		

function EquipEmbedPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--回包处理
function EquipEmbedPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if common:table_hasValue(opcodes, opcode) then
		self:refreshPage(container);
		return
	end
end

function EquipEmbedPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function EquipEmbedPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipEmbedPage = CommonPage.newSub(EquipEmbedPageBase, thisPageName, option);

function EquipEmbedPage_setEquipId(equipId)
	thisEquipId = equipId;
end

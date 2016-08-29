----------------------------------------------------------------------------------
--[[
	FILE:			MeltPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb 		= require("Const_pb");
local EquipOpr_pb 	= require("EquipOpr_pb");
local HP_pb			= require("HP_pb");
local NewbieGuideManager = require("NewbieGuideManager")
local UserInfo = require("UserInfo");
------------local variable for system api--------------------------------------
local pairs = pairs;
--------------------------------------------------------------------------------
local buildPage = "EquipBuildPage";
--registerScriptPage(buildPage);

local thisPageName = "MeltPage";

local opcodes = {
	EQUIP_SMELT_INFO_S	= HP_pb.EQUIP_SMELT_INFO_S,
	EQUIP_SMELT_S		= HP_pb.EQUIP_SMELT_S,
	ERROR_CODE_S		= HP_pb.ERROR_CODE
};

local COUNT_EQUIPMENT_SOURCE_MAX = 6;

local option = {
	ccbiFile = "RefiningPopUp.ccbi",
	handlerMap = {
		onHelp				= "showHelp",
		onClose				= "onClose",
		onBuildingEquipment	= "onBuild",
		onSpecialBuilding	= "onSpecialBuild",
		onAutoFilter		= "onAutoFilter",
		onRefining			= "onMelt"
	},
	opcode = opcodes
};
local Order = {"A", "B", "C", "D", "E", "F"};
for i = 1, COUNT_EQUIPMENT_SOURCE_MAX do
	option.handlerMap["on" .. Order[i] .. "Hand"] = "goSelectEquip";
end

local MeltPageBase = {};

local NodeHelper = require("NodeHelper");
local PBHelper = require("PBHelper");
local EquipOprHelper = require("EquipOprHelper");
local ItemManager = require("ItemManager");

local selectedIds = {};
local btnLock = false;
local isFirstEnter = true;
-----------------------------------------------
--MeltPageBase页面中的事件处理
----------------------------------------------
function MeltPageBase:onEnter(container)
	NewbieGuideManager.guide(GameConfig.NewbieGuide.FirstSmelt)
	btnLock = false;
	self:registerPacket(container);
	container:registerMessage(MSG_MAINFRAME_REFRESH);
	if isFirstEnter then
		EquipOprHelper:getSmeltEquipInfo();
	end
	self:refreshPage(container);
end

function MeltPageBase:onExit(container)
	selectedIds = {};
	btnLock = false;
	self:removePacket(container);
	container:removeMessage(MSG_MAINFRAME_REFRESH);
end
----------------------------------------------------------------

function MeltPageBase:refreshPage(container)
	self:showMeltInfo(container);
	self:showSourceEquips(container);
end

function MeltPageBase:showMeltInfo(container)
	local lb2Str = {	
		mRefiningValue 	= UserInfo.playerInfo.smeltValue or 0
	};
	NodeHelper:setStringForLabel(container, lb2Str);
end

function MeltPageBase:showSourceEquips(container)
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
			aniVisible = false;			
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
	end
	
	NodeHelper:setStringForLabel(container, lb2Str);
	NodeHelper:setNodesVisible(container, nodesVisible);
	NodeHelper:setSpriteImage(container, sprite2Img, scaleMap);
	NodeHelper:setQualityFrames(container, itemImg2Qulity);
end
	
----------------click event------------------------
function MeltPageBase:showHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_SMELT);
end

function MeltPageBase:onClose(container, eventName)
	PageManager.popPage(thisPageName);
end	

function MeltPageBase:onBuild(container)
	if not common:table_isEmpty(UserInfo.smeltInfo) then
		RegisterLuaPage(buildPage);
		PageManager.pushPage(buildPage);
	end
end

function MeltPageBase:onSpecialBuild(container)
	RegisterLuaPage("GodlyEquipBuildPage");
	PageManager.pushPage("GodlyEquipBuildPage");
end

function MeltPageBase:onMelt(container)
	local ids = self:getValidIds();
	if #ids > 0 then
		if btnLock then return; end
		
		for _, id in ipairs(selectedIds) do
			if UserEquipManager:hasGem(id) then
				MessageBoxPage:Msg_Box_Lan("@SelectedEquipHasGem");
				return;
			end
		end
		
		local hasHighLevelEquip = false;
		local userLevel = UserInfo.roleInfo.level or 1;
		local gap = GameConfig.LevelLimit.EquipDress or 10;
		local highLv = userLevel + gap;
		for _, id in ipairs(ids) do
			local userEquip = UserEquipManager:getUserEquipById(id);
			if EquipManager:getLevelById(userEquip.equipId) > highLv then
				hasHighLevelEquip = true;
				break;
			end
		end
		local title = common:getLanguageString("@SmeltConfirm_Title");
		local lvMsg = common:getLanguageString("@SmeltConfirm_LvMsg");
		local callback = function(isSure)
			if isSure then
				if hasHighLevelEquip then
					PageManager.showConfirm(title, lvMsg, function(isOK)
						if isOK then
							EquipOprHelper:smeltEquip(ids);
							btnLock = true;
							PageManager.popPage('DecisionPage');
						end
					end, false);
				else
					EquipOprHelper:smeltEquip(ids);
					btnLock = true;
				end
			end
		end;
		
		local bestEquipIds = UserEquipManager:getEquipIdsByClass("Quality", Const_pb.ORANGE);
		if bestEquipIds and #bestEquipIds > 0 then
			for _, id in ipairs(ids) do
				if common:table_hasValue(bestEquipIds, id) then
					local msg = common:getLanguageString("@SmeltConfirm_Msg");
					PageManager.showConfirm(title, msg, callback);
					return;
				end
			end
		end
		
		callback(true);
	else
		MessageBoxPage:Msg_Box_Lan("@PlzSelectEquipToSmelt");
	end
end

function MeltPageBase:onAutoFilter(container)
	local ids = UserEquipManager:getEquipIdsForSmelt(true);
	if ids == nil or #ids <= 0 then
		MessageBoxPage:Msg_Box_Lan("@NoEquipToSmelt");
	else
		selectedIds = common:table_sub(ids, 1, COUNT_EQUIPMENT_SOURCE_MAX);
		self:showSourceEquips(container);
	end
end

function MeltPageBase:goSelectEquip(container, eventName)
	local pos = common:table_arrayIndex(Order, eventName:sub(3):sub(1, -5));
	if selectedIds[pos] then
		selectedIds[pos] = nil;
		self:refreshPage(container);
		return;
	end
	
	local validIds = self:getValidIds();
	EquipSelectPage_multiSelect(validIds, COUNT_EQUIPMENT_SOURCE_MAX, function(ids)
		if not common:table_isSame(validIds, ids) then
			selectedIds = ids;
		end
		self:showSourceEquips(container);
	end, nil, EquipFilterType.Melt);
	PageManager.pushPage("EquipSelectPage");
end

function MeltPageBase:getValidIds()
	return common:table_arrayFilter(selectedIds, function(id)
		return id ~= nil;
	end);
end

--回包处理
function MeltPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.EQUIP_SMELT_INFO_S then
		EquipOprHelper:syncSMeltInfo(msgBuff);
		UserInfo.syncPlayerInfo();
		isFirstEnter = false;
		self:showMeltInfo(container);
	elseif opcode == opcodes.EQUIP_SMELT_S then
		selectedIds = {};
		UserInfo.syncPlayerInfo();
		self:refreshPage(container);
		EquipOprHelper:equipSmelted(msgBuff);
		btnLock = false;
	elseif opcode == opcodes.ERROR_CODE_S then
		btnLock = false;
	end
end

function MeltPageBase:onPacketError(container)
	btnLock = false;
end

function MeltPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function MeltPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

function MeltPageBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:refreshPage(container);
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local MeltPage = CommonPage.newSub(MeltPageBase, thisPageName, option);

function MeltPage_reset()
	isFirstEnter = true;
end
----------------------------------------------------------------------------------
--[[
	FILE:			EquipFilterPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	能量之核: 探索、拼合、列表页面
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb = require("Const_pb");

------------local variable for system api--------------------------------------
local pairs = pairs;
--------------------------------------------------------------------------------

local thisPageName = "EquipFilterPage";

local EquipPartNames = {
	["Helmet"] 		= Const_pb.HELMET,
	["Neck"]		= Const_pb.NECKLACE,
	["Finger"]		= Const_pb.RING,
	["Wrist"]		= Const_pb.GLOVE,	
	["Waist"]		= Const_pb.BELT,	
	["Feet"]		= Const_pb.SHOES,
	["Chest"]		= Const_pb.CUIRASS,
	["Legs"]		= Const_pb.LEGGUARD,
	["MainHand"]	= Const_pb.WEAPON1,
	["OffHand"]		= Const_pb.WEAPON2
};

local ColorNames = {
	["White"] 		= Const_pb.WHITE,
	["Green"]		= Const_pb.GREEN,
	["Blue"]		= Const_pb.BLUE,
	["Purple"]		= Const_pb.PURPLE,	
	["Orange"]		= Const_pb.ORANGE
};

local option = {
	ccbiFile = "BackpackEquipmentFilteringPopUp.ccbi",
	handlerMap = {
		onClose			= "onClose",
		onGodEquipment	= "filterGodEquip",
		onAllEquipment	= "filterAllEquip"
	}
};

for equipName, _ in pairs(EquipPartNames) do
	option.handlerMap["on" .. equipName] = "filterEquipPart";
end
for colorName, _ in pairs(ColorNames) do
	option.handlerMap["on" .. colorName .. "Equipment"] = "filterEquipColor";
end

local EquipFilterPageBase = {};

local NodeHelper = require("NodeHelper");
	
-----------------------------------------------
--EquipFilterPageBase页面中的事件处理
----------------------------------------------
function EquipFilterPageBase:onEnter(container)
	self:refreshPage(container);
end

----------------------------------------------------------------

function EquipFilterPageBase:refreshPage(container)
	self:showCountInfo(container);
end

function EquipFilterPageBase:showCountInfo(container)
	--UserEquipManager:classify();
	
	local lb2Str = {};
	
	for equipName, part in pairs(EquipPartNames) do
		local name = common:getLanguageString("@EquipPart_" .. part);
		local count = UserEquipManager:countEquipWithPart(part);
		lb2Str["m" .. equipName] = common:getLanguageString("@EquipCount", name, count);
	end
	
	for colorName, quality in pairs(ColorNames) do
		local name = common:getLanguageString("@QualityName_" .. quality);
		local count = UserEquipManager:countEquipWithQuality(quality);
		lb2Str["m" .. colorName .. "Equipment"] = common:getLanguageString("@EquipColorCount", name, count);
	end
	
	local name = common:getLanguageString("@GodlyEquip");
	local count = UserEquipManager:countEquipGodly();
	lb2Str["mGodEquipment"] = common:getLanguageString("@EquipCount", name, count);
	
	local name = common:getLanguageString("@AllEquip");
	local count = UserEquipManager:countEquipAll();
	lb2Str["mAllEquipment"] = common:getLanguageString("@EquipCount", name, count);
	
	NodeHelper:setStringForLabel(container, lb2Str);
end
	
----------------click event------------------------
function EquipFilterPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function EquipFilterPageBase:filterEquipPart(container, eventName)
	local part = EquipPartNames[string.sub(eventName, 3)];
	PackagePage_setFilter("Part", part);
	self:onClose();
end

function EquipFilterPageBase:filterEquipColor(container, eventName)
	local quality = ColorNames[eventName:sub(3):sub(1, -10)];
	PackagePage_setFilter("Quality", quality);
	self:onClose();	
end	

function EquipFilterPageBase:filterGodEquip(container)
	PackagePage_setFilter("Godly");
	self:onClose();
end	

function EquipFilterPageBase:filterAllEquip(container)
	PackagePage_setFilter("All");
	self:onClose();
end	

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
EquipFilterPage = CommonPage.newSub(EquipFilterPageBase, thisPageName, option);
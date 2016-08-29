----------------------------------------------------------------------------------
--[[
	FILE:			BatchSellEquipPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------
local Const_pb = require("Const_pb");

------------local variable for system api--------------------------------------
local pairs = pairs;
--------------------------------------------------------------------------------

local thisPageName = "BatchSellEquipPage";

local ColorNames = {
	["White"] 		= Const_pb.WHITE,
	["Green"]		= Const_pb.GREEN,
	["Blue"]		= Const_pb.BLUE,
	["Purple"]		= Const_pb.PURPLE,	
	["Orange"]		= Const_pb.ORANGE
};

local option = {
	ccbiFile = "BackpackBatchSellOutPopUp.ccbi",
	handlerMap = {
		onClose			= "onClose"
	}
};

for colorName, _ in pairs(ColorNames) do
	option.handlerMap["on" .. colorName .. "SellOut"] = "sellEquip";
end

local BatchSellEquipPageBase = {};

local NodeHelper = require("NodeHelper");
local EquipOprHelper = require("EquipOprHelper");
	
-----------------------------------------------
--BatchSellEquipPageBase页面中的事件处理
----------------------------------------------
function BatchSellEquipPageBase:onEnter(container)
	self:refreshPage(container);
end
----------------------------------------------------------------

function BatchSellEquipPageBase:refreshPage(container)
	self:showCountInfo(container);
end

function BatchSellEquipPageBase:showCountInfo(container)
	--UserEquipManager:classify();
	
	local lb2Str = {};
	
	for colorName, quality in pairs(ColorNames) do
		local name = common:getLanguageString("@QualityName_" .. quality);
		local count = UserEquipManager:countEquipForBatchSell(quality);
		lb2Str["m" .. colorName .. "Equipment"] = common:getLanguageString("@EquipColorCount", name, count);
	end
	
	NodeHelper:setStringForLabel(container, lb2Str);
end
	
----------------click event------------------------
function BatchSellEquipPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function BatchSellEquipPageBase:sellEquip(container, eventName)
	local qualityName = eventName:sub(3):sub(1, -8);
	local quality = ColorNames[qualityName];
	
	EquipOprHelper:sellEquipsWithQuality(quality);
	self:onClose();
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
BatchSellEquipPage = CommonPage.newSub(BatchSellEquipPageBase, thisPageName, option);
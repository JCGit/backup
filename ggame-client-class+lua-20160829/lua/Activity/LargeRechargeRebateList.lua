----------------------------------------------------------------------------------
--[[
	FILE:			LargeRechargeRebateList.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-09-23
--]]
----------------------------------------------------------------------------------
------------local variable for system api--------------------------------------
local ceil = math.ceil;
--------------------------------------------------------------------------------
local HP_pb = require("HP_pb");
local Activity_pb = require("Activity_pb");

local thisPageName = "LargeRechargeRebateList";
local NodeHelper = require("NodeHelper");
local opcodes = {
	RECHARGE_DOUBLE_S = HP_pb.RECHARGE_DOUBLE_S
};
local option = {
	ccbiFile = "Act_TimedDoubleItem.ccbi",
	handlerMap = {
		onClose			= "onClose"
	},
	opcode = opcodes
};

local LargeRechargeRebateListBase = {}

local thisItemList = {};
local thisRebateType = 2;

--------------------------------------------------------------
local RechargeItem = {
	ccbiFile = "Act_TimedDoubleContent.ccbi"
};

function RechargeItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		RechargeItem.onRefreshItemView(container);
	elseif eventName == "onRecharge" then
		RechargeItem.doRecharge(container);
	end
end	

function RechargeItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;
	local item = thisItemList[contentId];
	local cfg = g_RechargeItemList[item.goodsId];
	local rechargeStr = common:getLanguageString("@LargeRechargeRebateItem", cfg.productPrice);
	NodeHelper:setStringForLabel(container, {mRechargeNum = rechargeStr});
end

function RechargeItem.doRecharge(container)
	local contentId = container:getItemDate().mID;
	local item = thisItemList[contentId];
	if item.type == 3 then
		local title = common:getLanguageString("@LargeRechargeAgain_Title");
		local msg = common:getLanguageString("@LargeRechargeAgain_Msg");
		PageManager.showConfirm(title, msg, function(isSure)
			if isSure then
				GameUtil:doRecharge(item.goodsId);
			end
		end);
	else
		GameUtil:doRecharge(item.goodsId);
	end
end
----------------------------------------------------------------------------------
	
-----------------------------------------------
--LargeRechargeRebateListBase页面中的事件处理
----------------------------------------------
function LargeRechargeRebateListBase:onEnter(container)
	local titleKey = "@LargeRechargeRebateTitle_" .. thisRebateType;
	NodeHelper:setStringForLabel(container, {mRechargeExplainNode = common:getLanguageString(titleKey)});
	
	NodeHelper:initScrollView(container, "mContent", 3);
	
	self:registerPacket(container);
	self:refreshPage(container);
end

function LargeRechargeRebateListBase:onExit(container)
	NodeHelper:deleteScrollView(container);
	self:removePacket(container);
	self:clearCache();
end

----------------------------------------------------------------
function LargeRechargeRebateListBase:clearCache()
	thisItemList = {};
end

function LargeRechargeRebateListBase:refreshPage(container)
	self:rebuildAllItem(container);
end
----------------scrollview-------------------------
function LargeRechargeRebateListBase:rebuildAllItem(container)
	self:clearAllItem(container);
	if #thisItemList > 0 then
		self:buildItem(container);
	end
end

function LargeRechargeRebateListBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function LargeRechargeRebateListBase:buildItem(container)
	local size = #thisItemList;
	NodeHelper:buildScrollView(container, size, RechargeItem.ccbiFile, RechargeItem.onFunction);
end
	
----------------click event------------------------
function LargeRechargeRebateListBase:onClose(container)
	PageManager.popPage(thisPageName);
end

--回包处理
function LargeRechargeRebateListBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.RECHARGE_DOUBLE_S then
		local msg = Activity_pb.HPDoubleRechargeRet();
		msg:ParseFromString(msgBuff);
		
		if msg.leftTimes <= 0 then
			self:onClose();
		end
		LargeRechargeRebateList_setList(msg.items, msg.activityType);

		self:refreshPage(container);
		return;		
	end
end

function LargeRechargeRebateListBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function LargeRechargeRebateListBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
LargeRechargeRebateList = CommonPage.newSub(LargeRechargeRebateListBase, thisPageName, option)

function LargeRechargeRebateList_setList(itemList, rebateType)
	thisItemList = itemList;
	thisRebateType = rebateType;
end	
----------------------------------------------------------------------------------
--[[
	FILE:			LargeRechargeRebate.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			hgs
	CREATED:		2014-09-23
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local UserItemManager = require("UserItemManager");
local NodeHelper = require("NodeHelper");
local thisPageName = "LargeRechargeRebate"

local opcodes = {
	RECHARGE_DOUBLE_C		= HP_pb.RECHARGE_DOUBLE_C,
	RECHARGE_DOUBLE_S		= HP_pb.RECHARGE_DOUBLE_S,
	FETCH_SHOP_LIST_C		= HP_pb.FETCH_SHOP_LIST_C,
	FETCH_SHOP_LIST_S		= HP_pb.FETCH_SHOP_LIST_S,
	FETCH_SHOP_LIST_S		= HP_pb.FETCH_SHOP_LIST_S,
	DATA_NOTIFY_S			= HP_pb.DATA_NOTIFY_S
};
local option = {
	ccbiFile = "Act_TimedDouble.ccbi",
	handlerMap = {
		onRecharge	= "onRecharge",
		onClose		= "onClose"
	},
	opcode = opcodes
};

--活动基本信息，结构基本与协议pb Message相同
local thisActivityInfo = {
	id				= 9,
	remainTime 		= 0,
	items			= {},
	activityType 	= 1
};
thisActivityInfo.timerName = "Activity_" .. thisActivityInfo.id;

----------------- local data -----------------
local LargeRechargeRebateBase = {};
-----------------------------------------------
--LargeRechargeRebateBase页面中的事件处理
----------------------------------------------
function LargeRechargeRebateBase:onEnter(container)
	NodeHelper:setStringForLabel(container, {mActivitiesExplain = common:getLanguageString("@LargeRechargeRebateExplain")});
	self:clearPage(container);
	self:registerPacket(container);
	self:getActivityInfo(container);
	--container:registerMessage(MSG_MAINFRAME_REFRESH);
end

function LargeRechargeRebateBase:onExecute(container)
	self:onTimer(container);
end

function LargeRechargeRebateBase:onExit(container)
	TimeCalculator:getInstance():removeTimeCalcultor(thisActivityInfo.timerName);
	self:removePacket(container);
	--container:removeMessage(MSG_MAINFRAME_REFRESH);
end
----------------------------------------------------------------
function LargeRechargeRebateBase:onTimer(container)
	local timerName = thisActivityInfo.timerName;
	if not TimeCalculator:getInstance():hasKey(timerName) then return; end

	local remainTime = TimeCalculator:getInstance():getTimeLeft(timerName);
	if remainTime + 1 > thisActivityInfo.remainTime then
		return;
	end

	thisActivityInfo.remainTime = math.max(remainTime, 0);
	local timeStr = common:second2DateString(thisActivityInfo.remainTime, false);
	NodeHelper:setStringForLabel(container, {mSurplusTime = timeStr});
end

function LargeRechargeRebateBase:clearPage(container)
	NodeHelper:setStringForLabel(container, {
		mSurplusTime		= "",
		mRechargeDaysNum	= "",
		mRechargeExplain	= ""
	});
end

function LargeRechargeRebateBase:getActivityInfo(container)
	local msg = Activity_pb.HPDoubleRecharge();
	common:sendPacket(opcodes.RECHARGE_DOUBLE_C, msg);
end

function LargeRechargeRebateBase:refreshPage(container)
	if thisActivityInfo.remainTime > 0 and not TimeCalculator:getInstance():hasKey(timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(thisActivityInfo.timerName, thisActivityInfo.remainTime);
	end
	
	if common:table_isEmpty(g_RechargeItemList) then
		GameUtil:getRechargeList();
	else
		self:showExplain(container);
	end
end
---------------------------------------------------
function LargeRechargeRebateBase:showExplain(container)
	if #thisActivityInfo.items == 1 then
		local item = thisActivityInfo.items[1];
		local cfg = g_RechargeItemList[item.goodsId];
		local goldStr = cfg.gold * item.ratio;
		local tip = common:getLanguageString("@LargeRechargeRebateTip", cfg.productPrice, goldStr);
		NodeHelper:setStringForLabel(container, {mRechargeExplain = tip});
	end
end

----------------click event------------------------	
function LargeRechargeRebateBase:onClose(container)
	PageManager.popPage(thisPageName);
end	

function LargeRechargeRebateBase:onRecharge(container)
	if common:table_isEmpty(g_RechargeItemList) then return; end
	
	if #thisActivityInfo.items == 0 then
		return;
	elseif #thisActivityInfo.items == 1 then
		local item = thisActivityInfo.items[1];
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
	else
		local listPage = "LargeRechargeRebateList";
		RegisterLuaPage(listPage);
		LargeRechargeRebateList_setList(thisActivityInfo.items, thisActivityInfo.activityType);
		PageManager.pushPage(listPage);
	end
end

function LargeRechargeRebateBase:onReceiveMessage(container)
	local message = container:getMessage();
	local typeId = message:getTypeId();

	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:showExchangeItem(container);
		end
	end
end

--回包处理
function LargeRechargeRebateBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.RECHARGE_DOUBLE_S then
		local msg = Activity_pb.HPDoubleRechargeRet();
		msg:ParseFromString(msgBuff);
		
		thisActivityInfo.remainTime = msg.leftTimes;
		thisActivityInfo.activityType = msg.activityType;
		thisActivityInfo.items = msg.items;

		self:refreshPage(container);
		return;		
	end
	
	if opcode == opcodes.FETCH_SHOP_LIST_S then
		self:refreshPage(container);
		return;
	end
	
	if opcode == opcodes.DATA_NOTIFY_S then
		local msg = Player_pb.HPDataNotify();
		msg:ParseFromString(msgBuff)
		if msg.type == Const_pb.NOTIFY_RECHARGE then
			self:getActivityInfo(container);
		end
	end
end

function LargeRechargeRebateBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function LargeRechargeRebateBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
LargeRechargeRebate = CommonPage.newSub(LargeRechargeRebateBase, thisPageName, option);
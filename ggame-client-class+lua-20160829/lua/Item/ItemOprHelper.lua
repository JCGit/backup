local ItemOprHelper = {};
--------------------------------------------------------------------------------
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
local ItemOpr_pb 	= require("ItemOpr_pb");
local HP_pb		= require("HP_pb");
local ItemManager = require("ItemManager")
local UserInfo = require("UserInfo");
--------------------------------------------------------------------------------
--使用
function ItemOprHelper:useItem(itemId, count)
    local levelLimit = tonumber(ItemManager:getAttrById(itemId, "levelLimit"))
    if UserInfo.level < levelLimit then
        MessageBoxPage:Msg_Box_Lan(common:getLanguageString("@ItemLevelLimitContext", levelLimit))
        return
    end

	local msg = ItemOpr_pb.HPItemUse();
	msg.itemId = itemId;
	msg.itemCount = count or 1;
	common:sendPacket(HP_pb.ITEM_USE_C, msg);
end	

--打开10个
function ItemOprHelper:useTenItem(itemId)
	self:useItem(itemId, 10);
end

--出售
function ItemOprHelper:sellItem(itemId, count)
	local msg = ItemOpr_pb.HPItemSell();
	msg.itemId = itemId;
	msg.count = count or 1;
	
	common:sendPacket(HP_pb.ITEM_SELL_C, msg, false);
end

--回收
function ItemOprHelper:recycleItem(itemId)
	local msg = ItemOpr_pb.HPGongceWordCycle();
	msg.itemId = itemId;
	common:sendPacket(HP_pb.WORDS_EXHCNAGE_CYCLE_C, msg, false);
end
--------------------------------------------------------------------------------
return ItemOprHelper;
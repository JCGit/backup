local UserItemManager = {};
--------------------------------------------------------------------------------

------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
local table = table;
local math = math;
--------------------------------------------------------------------------------
local Item_pb = require("Item_pb");

local ItemManager = require("ItemManager");
--------------------------------------------------------------------------------

function UserItemManager:getUserItemByIndex(index)
	local ItemStr = ServerDateManager:getInstance():getItemInfoByIndexForLua(index);
	local userItem = Item_pb.ItemInfo();
	userItem:ParseFromString(ItemStr);
	return userItem;
end

function UserItemManager:getUserItemById(userItemId)
	local ItemStr = ServerDateManager:getInstance():getItemInfoByIdForLua(userItemId);
	local userItem = Item_pb.ItemInfo();
	userItem:ParseFromString(ItemStr);
	return userItem;
end

function UserItemManager:getUserItemByItemId(itemId)
	local size = ServerDateManager:getInstance():getItemInfoTotalSize();
	for i = 0, size - 1 do
		local userItem = self:getUserItemByIndex(i);
		if userItem and userItem.itemId == itemId then
			return userItem;
		end
	end
	return nil;
end

function UserItemManager:getUserItemIds()
	local size = ServerDateManager:getInstance():getItemInfoTotalSize();
	local ids = {};
	for i = 0, size - 1 do
		local userItem = self:getUserItemByIndex(i);
		if userItem then
			table.insert(ids, userItem.itemId);
		end
	end
	return ids;
end

function UserItemManager:getItemIdsByType(toolType)
	local ids = {};
	local maxIndex = ServerDateManager:getInstance():getItemInfoTotalSize() - 1;
	for i = 0, maxIndex do
		local userItem = self:getUserItemByIndex(i);
		local itemType = ItemManager:getTypeById(userItem.itemId);
		if itemType == toolType then
			table.insert(ids, userItem.itemId);
		end
	end
	return ids;
end

function UserItemManager:getCountByItemId(itemId)
	local userItem = self:getUserItemByItemId(itemId)
	return userItem and math.max(userItem.count, 0) or 0
end

--------------------------------------------------------------------------------
return UserItemManager;
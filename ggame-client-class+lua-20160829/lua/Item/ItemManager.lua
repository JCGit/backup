local ItemManager = {};
--------------------------------------------------------------------------------
local ItemCfg = ConfigManager.getItemCfg();
local nowSelectItemId = nil
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
local Item_pb = require("Item_pb");
local Const_pb = require("Const_pb");
--------------------------------------------------------------------------------
--从item.txt获取道具配置
function ItemManager:getItemCfgById(itemId, attrName)
	return ItemCfg[itemId] or {};
end

function ItemManager:getAttrById(itemId, attrName)
	local config = ItemCfg[itemId];
	if config then
		return config[attrName];
	end
	return "";
end

function ItemManager:getTypeById(itemId)
	return self:getAttrById(itemId, "type");
end

function ItemManager:getQualityById(itemId)
	return self:getAttrById(itemId, "quality");
end

function ItemManager:getIconById(itemId)
	return self:getAttrById(itemId, "icon");
end

function ItemManager:getNameById(itemId)
	return self:getAttrById(itemId, "name");
end

function ItemManager:getLevelUpCost(itemId)
	return self:getAttrById(itemId, "levelUpCost");
end
		
function ItemManager:getLevelUpTarget(itemId)
	return self:getAttrById(itemId, "levelUpItem");
end

function ItemManager:getLevelUpCostMax(itemId)
	return self:getAttrById(itemId, "levelUpCostMax");
end

--获取宝石的小icon,根据属性区分
function ItemManager:getGemSmallIcon(itemId)
	local attr2Id = {
		strength	= Const_pb.STRENGHT,
		agility		= Const_pb.AGILITY,
		intellect	= Const_pb.INTELLECT,
		stamina		= Const_pb.STAMINA
	};
	for attr, attrId in pairs(attr2Id) do
		local attrVal = self:getAttrById(itemId, attr) or 0;
		if tonumber(attrVal) > 0 then
			return GameConfig.Image.GemIcon[attrId];
		end
	end
	return nil;
end

--获取宝石加成属性
function ItemManager:getGemAttrString(itemId)
	local retStr = "";
	local attr2Id = {
		strength	= Const_pb.STRENGHT,
		agility		= Const_pb.AGILITY,
		intellect	= Const_pb.INTELLECT,
		stamina		= Const_pb.STAMINA
	};
	for attr, attrId in pairs(attr2Id) do
		local attrVal = self:getAttrById(itemId, attr) or 0;
		if tonumber(attrVal) > 0 then
			retStr = common:getLanguageString("@AttrName_" .. attrId) .. " +" .. attrVal;
			return retStr;
		end
	end
	return retStr;
end

--获取打所需熔炼值(装备打造可以打造部分道具)
function ItemManager:getSmeltNeedById(itemId)
	return tonumber(self:getAttrById(itemId, "smeltCost")) or 0;
end

--获取礼包等包含物品配置
function ItemManager:getContainCfg(itemId)
	local cfgStr = self:getAttrById(itemId, "containItem");
	if cfgStr == "" or cfgStr == "0" then return nil; end
	
	local cfg = {};
	for _, item in ipairs(common:split(cfgStr, ",")) do
		local _type, _id, _count = unpack(common:split(item, "_"));
		table.insert(cfg, {
			type 	= tonumber(_type),
			itemId	= tonumber(_id),
			count 	= tonumber(_count)
		});
	end
	
	return cfg;
end

-- 当前选择的物品是什么
function ItemManager:setNowSelectItem(itemId)
	assert(itemId, "no Item Selected")
	nowSelectItemId = itemId
end

function ItemManager:getNowSelectItem()
	return nowSelectItemId
end

function ItemManager:getPriceById(itemId)
	return tonumber(self:getAttrById(itemId, "price")) or 0;
end
--------------------------------------------------------------------------------
return ItemManager;
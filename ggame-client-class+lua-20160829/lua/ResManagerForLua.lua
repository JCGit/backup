require "Const_pb"
local UserInfo = require("UserInfo");
local ResManagerForLua = {
};
--------------------------------------------------------------------------------
local roleCfg = ConfigManager.getRoleCfg();
local equipCfg = ConfigManager.getEquipCfg();
local itemCfg = ConfigManager.getItemCfg();
local resPropCfg = ConfigManager.getResPropertyCfg();
local userProCfg = ConfigManager.getUserPropertyCfg();
local musciCfg = ConfigManager.getBGMusicCfg(); -- 获取背景音乐

local UserItemManager = require("UserItemManager");
--------------------------------------------------------------------------------

-- 添加背景音乐的方法
function ResManagerForLua:playBGMusic(musicName)
	for k,v in pairs(musciCfg) do
		if tostring(v.englishName) == tostring(musicName) then
			SoundManager:getInstance():playMusic(tostring(v.musicPath))
		end
	end
end

function ResManagerForLua:getResInfoByTypeAndId(resType,resId,resCount)
	local resMainType = self:getResMainType(resType)
	if resMainType == nil then
		CCLuaLog("@ResManagerForLua:getResInfoByTypeAndId -- type is error.")
		return nil
	end
	
	return self:getResInfoByMainTypeAndId(resMainType, resId, resCount, resType);
end

function ResManagerForLua:getResInfoByMainTypeAndId(resMainType, resId, resCount, resType)
	local ResInfoLua = {	
		name,
		describe,
		quality,
		icon,
		count,
		itemId,
		itemType,
		mainType,
		typeName
	}
	ResInfoLua.itemId = resId
	ResInfoLua.itemType = resType
	ResInfoLua.count = resCount
	ResInfoLua.mainType = resMainType
	
	local newType = resMainType * 10000	
	ResInfoLua.typeName = resPropCfg[newType]["name"]
		
	if resMainType == Const_pb.ROLE then
		ResInfoLua.name = roleCfg[resId]["name"]	
		--describe 暂时为name，需要策划配置 by zhenhui 2014/8/5	
		ResInfoLua.describe = roleCfg[resId]["name"]
		ResInfoLua.icon = roleCfg[resId]["icon"]
		--暂时定为4
		ResInfoLua.quality = 4
	elseif resMainType == Const_pb.TOOL then
		if(itemCfg[resId] == nil) then
			CCLuaLog('itemCfg[resId] nil '..resId)
		end
		ResInfoLua.name = itemCfg[resId]["name"]	
		ResInfoLua.describe = itemCfg[resId]["description"]
		ResInfoLua.icon = itemCfg[resId]["icon"]		
		ResInfoLua.quality = itemCfg[resId]["quality"]
	elseif resMainType == Const_pb.EQUIP then 
		ResInfoLua.name = equipCfg[resId]["name"]	
		--describe 暂时为name，需要策划配置 by zhenhui 2014/8/5	
		ResInfoLua.describe = equipCfg[resId]["name"]
		ResInfoLua.icon = equipCfg[resId]["icon"]		
		ResInfoLua.quality = equipCfg[resId]["quality"]
	elseif resMainType == Const_pb.PLAYER_ATTR then
	    if userProCfg[resId] == nil then return nil end
	    ResInfoLua.name = userProCfg[resId]['name']
		ResInfoLua.describe = userProCfg[resId]['discribe']
		ResInfoLua.icon = userProCfg[resId]['icon']
		ResInfoLua.quality = userProCfg[resId]['quality']
	end
	
	
	
	return ResInfoLua
end

function ResManagerForLua:getResMainType(type)
	local resPropInfo = resPropCfg[type]
	local MainType = nil;
	if resPropInfo~= nil and resPropCfg[type]["type"]~=nil then
	    MainType = resPropCfg[type]["type"]
		return MainType
	else
		if type > 10000 then
			MainType = math.floor(type/ 10000)
		end
		return MainType;
	end
	
	
end

function ResManagerForLua:canConsume(consumeId)
	local consumeCfg = ConfigManager.getConsumeById(consumeId);
	local isOptional = consumeCfg["type"] == 2;
	for _, cfg in ipairs(consumeCfg["items"]) do
		local ownCount = 0;
		local mainType = self:getResMainType(cfg.type);
		if mainType == Const_pb.TOOL then
			ownCount = UserItemManager:getCountByItemId(cfg.itemId);
		end
		local isEnough = ownCount >= cfg.count;
		if isEnough and isOptional then return true; end
		if not isEnough and not isOptional then return false; end 
	end
	return not isOptional;
end

function ResManagerForLua:getResStr(cfg)
	local strTb = {};
	for _, subCfg in ipairs(cfg) do
		local resInfo = self:getResInfoByTypeAndId(subCfg.type, subCfg.itemId, subCfg.count);
		table.insert(strTb, resInfo.name .. "x" .. resInfo.count);
	end
	return table.concat(strTb, ",");
end

function ResManagerForLua:checkConsume(typeId, itemId, count)
	local mainType = self:getResMainType(typeId);
	if mainType == Const_pb.PLAYER_ATTR then
		if itemId == Const_pb.COIN then
			return UserInfo.isCoinEnough(count);
		end
	elseif mainType == Const_pb.TOOL then
		if count > UserItemManager:getCountByItemId(itemId) then
			MessageBoxPage:Msg_Box_Lan("@LackItem");
			return false;
		end
		return true;
	end
	return true;
end
--------------------------------------------------------------------------------
return ResManagerForLua;
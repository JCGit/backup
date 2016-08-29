local UserMercenaryManager = {};
--------------------------------------------------------------------------------

------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
local Player_pb = require("Player_pb");

local RoleManager = require("RoleManager");
local PBHelper = require("PBHelper");
--------------------------------------------------------------------------------
function UserMercenaryManager:getUserMercenaryByIndex(index)
	local maxSize = ServerDateManager:getInstance():getRoleInfoTotalSize();
	if index < 0 or index > (maxSize - 1) then
		CCLuaLog("ERROR in synAndGetRoleInfoByIndex")
		return nil;
	end
	local mercenaryStr = ServerDateManager:getInstance():getRoleInfoByIndexForLua(index);
	local userMercenary = Player_pb.RoleInfo();
	userMercenary:ParseFromString(mercenaryStr);
	return userMercenary;
end

function UserMercenaryManager:getFightMercenaryIndex()
	local maxSize = ServerDateManager:getInstance():getRoleInfoTotalSize()
	local fightMercenary = 1
	for i = 1 ,maxSize,1 do
		local mercenaryStr = ServerDateManager:getInstance():getRoleInfoByIndexForLua(i - 1)
		local data = Player_pb.RoleInfo()
		data:ParseFromString(mercenaryStr);
		if data.status == Const_pb.FIGHTING then
			fightMercenary = i
		end
	end
	
	return fightMercenary
end

function UserMercenaryManager:getUserMercenaryById(roleId)
	local mercenaryStr = ServerDateManager:getInstance():getRoleInfoByIdForLua(roleId);
	local userMercenary = Player_pb.RoleInfo();
	userMercenary:ParseFromString(mercenaryStr);
	return userMercenary;
end

function UserMercenaryManager:isEquipDressed(userEquipId)
	local size = ServerDateManager:getInstance():getRoleInfoTotalSize();
	for i = 0, size - 1 do
		local userMercenary = self:getUserMercenaryByIndex(i);
		if userMercenary then
			for _, roleEquip in ipairs(userMercenary.equips) do
				if roleEquip and roleEquip.equipId == userEquipId then
					return true;
				end
			end
		end
	end
	return false;
end

function UserMercenaryManager:getEquipDressedBy(userEquipId)
	local size = ServerDateManager:getInstance():getRoleInfoTotalSize();
	for i = 0, size - 1 do
		local userMercenary = self:getUserMercenaryByIndex(i);
		if userMercenary then
			for _, roleEquip in ipairs(userMercenary.equips) do
				if roleEquip and roleEquip.equipId == userEquipId then
					return RoleManager:getOccupationById(userMercenary.itemId);
				end
			end
		end
	end
	return "";
end

function UserMercenaryManager:getUpStepTableByRoleId( roleId )
	local cfg = ConfigManager.getMercenaryUpStepTable()
	local tab = {}
	for k,v in ipairs(cfg) do
		if v.roleId == roleId then
			tab[v.stepLevel] = v
		end
	end
	
	return tab
end

function UserMercenaryManager:getEquipByPart(roleId, part)
	local userMercenary = self:getUserMercenaryById(roleId);
	return PBHelper:getRoleEquipByPart(userMercenary.equips, part);
end	

function UserMercenaryManager:getProfessioinIdByPart(roleId, part)
	local userMercenary = self:getUserMercenaryById(roleId);
	return userMercenary.prof;
end	
--------------------------------------------------------------------------------
return UserMercenaryManager;
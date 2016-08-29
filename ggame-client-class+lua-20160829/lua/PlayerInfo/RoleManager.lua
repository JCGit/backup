local RoleManager = {
};
-------------------------------------------------------------------------------

local roleCfg = ConfigManager.getRoleCfg();

------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------

function RoleManager:getAttrById(roleId, attrName)
	local config = roleCfg[roleId];
	if config then
		return config[attrName];
	end
	return "";
end

function RoleManager:getOccupationById(roleId)
	return self:getAttrById(roleId, "name");
end

function RoleManager:getIconById(roleId)
	return self:getAttrById(roleId, "icon");
end

function RoleManager:getPosterById(roleId)
	return self:getAttrById(roleId, "poster");
end

function RoleManager:getOccupationIconById(roleId)
	local professionId = self:getAttrById(roleId, "profession") or 1;
	return GameConfig.ProfessionIcon[professionId] or "";
end
--------------------------------------------------------------------------------
return RoleManager;
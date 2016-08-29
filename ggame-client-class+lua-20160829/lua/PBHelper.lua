local PBHelper = {};
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
function PBHelper:getAttrById(attrInfo, attrId)
	local defVal = 0;
	if attrInfo == nil then return defVal; end
	
	for _, attr in ipairs(attrInfo) do
		if attr.attrId == attrId then
			return attr.attrValue;
		end
	end
	return defVal;
end

function PBHelper:getRoleEquipByPart(equips, part)
	local defVal = nil;
	if equips == nil then return defVal; end
	
	for _, roleEquip in ipairs(equips) do
		if roleEquip.part == part then
			return roleEquip;
		end
	end
	return defVal;
end

function PBHelper:getGemInfo(gemInfos)
	local pos2GemId = {};
	for _, gemInfo in ipairs(gemInfos) do
		local gemId = gemInfo.gemItemId;
		if gemId and gemId >= 0 then
			pos2GemId[gemInfo.pos] = gemId;
		end
	end
	return pos2GemId;
end	
--------------------------------------------------------------------------------
return PBHelper;
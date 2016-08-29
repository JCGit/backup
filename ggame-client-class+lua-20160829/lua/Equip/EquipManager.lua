local EquipManager = {};


--------------------------------------------------------------------------------
local EquipCfg = ConfigManager.getEquipCfg();

------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
local Equip_pb = require("Equip_pb");
local Const_pb = require("Const_pb");
local NodeHelper = require("NodeHelper")
local UserInfo = require("UserInfo");
--------------------------------------------------------------------------------

function EquipManager:getEquipCfgById(equipId)
	return EquipCfg[equipId] or {};
end	

--从equip.txt获取装备属性
function EquipManager:getAttrById(equipId, attrName)
	local config = EquipCfg[equipId];
	if config then
		return config[attrName];
	end
	return "";
end

--从godlyAttr.txt获取神器属性（成长属性、强化加成）
function EquipManager:getGodlyAttrById(equipId, attrName)
	local GodlyAttrCfg = ConfigManager.getGodlyAttrCfg();
	local config = GodlyAttrCfg[equipId];
	if config then
		return config[attrName];
	end
	return "";
end

function EquipManager:getLevelById(equipId)
	local level = self:getAttrById(equipId, "level");
	return tonumber(level);
end

function EquipManager:getQualityById(equipId)
	return self:getAttrById(equipId, "quality");
end

function EquipManager:getIconById(equipId)
	return self:getAttrById(equipId, "icon");
end

function EquipManager:getNameById(equipId)
	return self:getAttrById(equipId, "name");
end

function EquipManager:getPartById(equipId)
	return self:getAttrById(equipId, "part");
end

function EquipManager:getPartNameById(equipId)
	return self:getAttrById(equipId, "partName");
end

function EquipManager:getSmeltNeedById(equipId)
	return self:getAttrById(equipId, "forgeSmeltNeed");
end

function EquipManager:getSmeltGainById(equipId)
	return self:getAttrById(equipId, "smeltGain");
end

function EquipManager:getEquipStepById(equipId)
	local stepLevel = self:getAttrById(equipId, "stepLevel")
	return tonumber(stepLevel)
end

function EquipManager:getProfessionById(equipId)
	local profs = self:getAttrById(equipId, "profession") or {};
	local prof = profs[1] or 0;
	return tonumber(prof) % 100;
end

function EquipManager:getBaptizeCost(equipId)
	local cost =  self:getAttrById(equipId, "washCoinCost");
	return tonumber(cost);
end

function EquipManager:getOccupationIconById(equipId)
	local professionId = self:getProfessionById(equipId);	
	return GameConfig.ProfessionIcon[professionId] or "";
end	

--装备能否洗炼
function EquipManager:canBeBaptized(equipId)
	return self:getBaptizeCost(equipId) > 0;
	--self:getLevelById(equipId) >= GameConfig.OpenLevel.Baptize;
end

--装备能否打孔或镶嵌宝石
function EquipManager:canBeEmbed(equipId)
	return self:getLevelById(equipId) >= GameConfig.OpenLevel.EmbedGem;
end

--根据星级获取神器属性
function EquipManager:getStarAttrByLevel(id, level, fmt)
	local baseVal = self:getGodlyAttrById(id, "baseAttr");
	local growVal = self:getGodlyAttrById(id, "growAttr");
	local val = tonumber(baseVal) + (tonumber(level) - 1) * tonumber(growVal);
	return self:getGodlyAttrString(id, val, fmt);
end

--装备显示星星位置
function EquipManager:setStarPosition(starSprite, bVisible, posX, posY)
    if starSprite ~= nil then
    	local xDistance = VaribleManager:getInstance():getSetting("EquipStarXDistance")
	    xDistance = tonumber(xDistance) or 40
        NodeHelper:setNodeVisible(starSprite, bVisible)
	    starSprite:setPosition(posX+xDistance, posY)
	end
end

--获取神器全身强化加成属性
function EquipManager:getActiveValByLevel(id, level, fmt)
	local enhanceAttrs = self:getGodlyAttrById(id, "enhanceAttrs");
	local val = tonumber(enhanceAttrs[level] or 0);
	return self:getGodlyAttrString(id, val, fmt);
end	

--神器属性加成是否是数值(３种穿透等级是加成数据，其它是加成百分比）
function EquipManager:isGodlyAttrPureNum(id)
	return (math.floor(id / 100) == GameConfig.GodlyAttrPureNum or id == Const_pb.BUFF_AVOID_ARMOR);
end

function EquipManager:getGodlyAttrString(id, val, fmt)
	if val ~= 0 then
		if not self:isGodlyAttrPureNum(id)	then
			local fmt = fmt or "%.1f%%%%%%%%";	--multi % for gsub()
			val = string.format(fmt, val / 100);
		end
	end
	return val;
end

--从equipStrength.txt根据强化等级获取装备主属性数值
function EquipManager:getAttrAddVAl(_id, _lv)
	local enhanceCfg = ConfigManager.getEquipEnhanceAttrCfg();
	local cfg = enhanceCfg[_id] or {};
	local val = cfg["mainAttr"][_lv];

	return tonumber(val or 0);
end

--从equipStrengthRatio.txt获取装备强化时各种相关数值所占权重
--用于装备强化所需强化精华的计算(见UserEquip:getItemNeedForEnhance)
function EquipManager:getWeightByIdAndType(_id, _type)
	local enhanceCfg = ConfigManager.getEquipEnhanceWeightCfg();
	local key = _id .. "_" .. _type;
	local cfg = enhanceCfg[key] or {};
	return cfg["weight"] or 0;
end

--从godlyLevelExp.txt获取神器升星所需经验
--@param isAttr2: 是否是升级第二条属性
function EquipManager:getExpNeedForLevelUp(level, isAttr2)
	local expCfg = ConfigManager.getGoldlyLevelExpCfg();
	local cfg = expCfg[level] or {};
	if isAttr2 then
		return cfg["exp2"] or 1;
	end
	return cfg["exp"] or 1;
end

--根据装备部位获取权重，用于吞噬、传承、融合等金币计算
function EquipManager:getWeightByPart(part)
	return GameConfig.EquipPartWeight[part] or 0;
end	

--获取打孔消耗
function EquipManager:getPunchConsume(equipId, pos)
	local consumes = self:getAttrById(equipId, "punchConsume") or {};
	local pos = tonumber(pos);
	return consumes[pos] or nil;
end

--装备等级是否满足装备条件
function EquipManager:isDressableWithLevel(equipId)
	local level = self:getLevelById(equipId);
	local gap = GameConfig.LevelLimit.EquipDress or 10;
	if level > (UserInfo.roleInfo.level or 1) + gap then
		return false;
	end
	return true;
end

--装备职业是否满足装备条件
function EquipManager:isDressableWithProfession(equipId, professionId)
	local profs = self:getAttrById(equipId, "profession") or {};
	for _, prof in ipairs(profs) do
		local prof = tonumber(prof);
		if prof == 0 or prof == professionId then
			return true;
		end
	end
	return false;
end

--装备是否能够装备
function EquipManager:isDressable(equipId, professionId)
	return self:isDressableWithLevel(equipId) and self:isDressableWithProfession(equipId, professionId);
end

--获取装备初始主属性
function EquipManager:getInitAttr(equipId)
	local cfgStr = self:getAttrById(equipId, "equipAttr") or '';
	return self:getAttrLangStr(cfgStr);
end

--获取属性属于主、副、神器属性中的哪一类
function EquipManager:getAttrGrade(attrId)
	local attrId = tonumber(attrId);
	if attrId > 1000 then
		return Const_pb.GODLY_ATTR;
	elseif attrId > 100 then
		return Const_pb.SECONDARY_ATTR;
	else
		return Const_pb.PRIMARY_ATTR;
	end
end

--@param glue: defalut = "\n"
function EquipManager:getAttrLangStr(attrStr, glue)
	local glue = glue or " ";
	
	local attrTb = {};
	
	for _, subStr in ipairs(common:split(attrStr, ",")) do
		local attrId, attrVal, attrValMax = unpack(common:split(subStr, '_'));
		attrVal = tonumber(attrVal);
		if attrVal > 0 then
			local name = common:getLanguageString("@AttrName_" .. attrId);
			local str = "";
			if self:getAttrGrade(attrId) == Const_pb.GODLY_ATTR and not self:isGodlyAttrPureNum(attrId) then
				str = string.format("%s +%.1f%%", name, attrVal / 100);
			else
				str = string.format("%s +%d", name, attrVal);
			end
			table.insert(attrTb, str);
		end
	end
	
	return table.concat(attrTb, glue);
end

--根据神器属性个数获取神器初始神器属性文本
function EquipManager:getGodlyAttr(id, count, glue)
	local count = count or 1;
	if count < 1 or count > 2 then return ""; end
	
	local glue = glue or "\n";
	
	local strTb = {};
	
	local part = self:getPartById(id);
	if count > 1 then
		local attrId = GameConfig.Part2GodlyAttr_1[part];
		local attrVal = self:getActiveValByLevel(attrId, 1, "%.1f%%");
		local name = common:getLanguageString("@AttrName_" .. attrId);
		table.insert(strTb, string.format("%s +%s", name, attrVal));
	end
	local attrId = GameConfig.Part2GodlyAttr_2[part];
	local attrVal = self:getActiveValByLevel(attrId, 1, "%.1f%%");
	local name = common:getLanguageString("@AttrName_" .. attrId);
	table.insert(strTb, string.format("%s +%s", name, attrVal));
	
	return table.concat(strTb, glue);
end

--根据神器属性个数获取神器特效ccbi，用于声望打造中特效显示（1个时一定是第二条）
function EquipManager:getGodlyAni(count)
	local aniKey = count > 1 and "Double" or "Second";
	return GameConfig.GodlyEquipAni[aniKey];
end

--从battleParameter.txt中获取加成属性的修正参数，用于“更多属性”页中属性计算
function EquipManager:getBattleAttrEffect(attrId, attrVal)
	local BattleParamCfg = ConfigManager.getBattleParamCfg();
	local cfg = BattleParamCfg[UserInfo.roleInfo.level];
	if attrVal == 0 then return "0.0"; end
	local param = cfg["Attr_" .. attrId];
	return string.format("%.1f", 100 * attrVal / (attrVal + tonumber(param)));
end
--------------------------------------------------------------------------------
return EquipManager;
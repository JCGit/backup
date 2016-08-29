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

--��equip.txt��ȡװ������
function EquipManager:getAttrById(equipId, attrName)
	local config = EquipCfg[equipId];
	if config then
		return config[attrName];
	end
	return "";
end

--��godlyAttr.txt��ȡ�������ԣ��ɳ����ԡ�ǿ���ӳɣ�
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

--װ���ܷ�ϴ��
function EquipManager:canBeBaptized(equipId)
	return self:getBaptizeCost(equipId) > 0;
	--self:getLevelById(equipId) >= GameConfig.OpenLevel.Baptize;
end

--װ���ܷ��׻���Ƕ��ʯ
function EquipManager:canBeEmbed(equipId)
	return self:getLevelById(equipId) >= GameConfig.OpenLevel.EmbedGem;
end

--�����Ǽ���ȡ��������
function EquipManager:getStarAttrByLevel(id, level, fmt)
	local baseVal = self:getGodlyAttrById(id, "baseAttr");
	local growVal = self:getGodlyAttrById(id, "growAttr");
	local val = tonumber(baseVal) + (tonumber(level) - 1) * tonumber(growVal);
	return self:getGodlyAttrString(id, val, fmt);
end

--װ����ʾ����λ��
function EquipManager:setStarPosition(starSprite, bVisible, posX, posY)
    if starSprite ~= nil then
    	local xDistance = VaribleManager:getInstance():getSetting("EquipStarXDistance")
	    xDistance = tonumber(xDistance) or 40
        NodeHelper:setNodeVisible(starSprite, bVisible)
	    starSprite:setPosition(posX+xDistance, posY)
	end
end

--��ȡ����ȫ��ǿ���ӳ�����
function EquipManager:getActiveValByLevel(id, level, fmt)
	local enhanceAttrs = self:getGodlyAttrById(id, "enhanceAttrs");
	local val = tonumber(enhanceAttrs[level] or 0);
	return self:getGodlyAttrString(id, val, fmt);
end	

--�������Լӳ��Ƿ�����ֵ(���ִ�͸�ȼ��Ǽӳ����ݣ������Ǽӳɰٷֱȣ�
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

--��equipStrength.txt����ǿ���ȼ���ȡװ����������ֵ
function EquipManager:getAttrAddVAl(_id, _lv)
	local enhanceCfg = ConfigManager.getEquipEnhanceAttrCfg();
	local cfg = enhanceCfg[_id] or {};
	local val = cfg["mainAttr"][_lv];

	return tonumber(val or 0);
end

--��equipStrengthRatio.txt��ȡװ��ǿ��ʱ���������ֵ��ռȨ��
--����װ��ǿ������ǿ�������ļ���(��UserEquip:getItemNeedForEnhance)
function EquipManager:getWeightByIdAndType(_id, _type)
	local enhanceCfg = ConfigManager.getEquipEnhanceWeightCfg();
	local key = _id .. "_" .. _type;
	local cfg = enhanceCfg[key] or {};
	return cfg["weight"] or 0;
end

--��godlyLevelExp.txt��ȡ�����������辭��
--@param isAttr2: �Ƿ��������ڶ�������
function EquipManager:getExpNeedForLevelUp(level, isAttr2)
	local expCfg = ConfigManager.getGoldlyLevelExpCfg();
	local cfg = expCfg[level] or {};
	if isAttr2 then
		return cfg["exp2"] or 1;
	end
	return cfg["exp"] or 1;
end

--����װ����λ��ȡȨ�أ��������ɡ����С��ںϵȽ�Ҽ���
function EquipManager:getWeightByPart(part)
	return GameConfig.EquipPartWeight[part] or 0;
end	

--��ȡ�������
function EquipManager:getPunchConsume(equipId, pos)
	local consumes = self:getAttrById(equipId, "punchConsume") or {};
	local pos = tonumber(pos);
	return consumes[pos] or nil;
end

--װ���ȼ��Ƿ�����װ������
function EquipManager:isDressableWithLevel(equipId)
	local level = self:getLevelById(equipId);
	local gap = GameConfig.LevelLimit.EquipDress or 10;
	if level > (UserInfo.roleInfo.level or 1) + gap then
		return false;
	end
	return true;
end

--װ��ְҵ�Ƿ�����װ������
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

--װ���Ƿ��ܹ�װ��
function EquipManager:isDressable(equipId, professionId)
	return self:isDressableWithLevel(equipId) and self:isDressableWithProfession(equipId, professionId);
end

--��ȡװ����ʼ������
function EquipManager:getInitAttr(equipId)
	local cfgStr = self:getAttrById(equipId, "equipAttr") or '';
	return self:getAttrLangStr(cfgStr);
end

--��ȡ�������������������������е���һ��
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

--�����������Ը�����ȡ������ʼ���������ı�
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

--�����������Ը�����ȡ������Чccbi������������������Ч��ʾ��1��ʱһ���ǵڶ�����
function EquipManager:getGodlyAni(count)
	local aniKey = count > 1 and "Double" or "Second";
	return GameConfig.GodlyEquipAni[aniKey];
end

--��battleParameter.txt�л�ȡ�ӳ����Ե��������������ڡ��������ԡ�ҳ�����Լ���
function EquipManager:getBattleAttrEffect(attrId, attrVal)
	local BattleParamCfg = ConfigManager.getBattleParamCfg();
	local cfg = BattleParamCfg[UserInfo.roleInfo.level];
	if attrVal == 0 then return "0.0"; end
	local param = cfg["Attr_" .. attrId];
	return string.format("%.1f", 100 * attrVal / (attrVal + tonumber(param)));
end
--------------------------------------------------------------------------------
return EquipManager;
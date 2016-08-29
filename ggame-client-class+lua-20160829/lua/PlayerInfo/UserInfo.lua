local UserInfo = {
	playerInfo 	= {},
	roleInfo	= {},
	stateInfo	= {},
	smeltInfo	= {},
	serverId	= GamePrecedure:getInstance():getServerID(),
	level		= 0,
	skillUnlock = {
		level	= 0,	--技能槽开放等级
		hasNew	= false
	},
	guideInfo	= {
		Gift 		= false,
		Equip 		= false,
		Help 		= false
	};
};
--------------------------------------------------------------------------------
local Player_pb = require("Player_pb");
local Const_pb 	= require("Const_pb");

local PBHelper = require("PBHelper");


------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
function UserInfo.sync()
	UserInfo.syncPlayerInfo();
	UserInfo.syncRoleInfo();
	serverId	= GamePrecedure:getInstance():getServerID()
	--UserInfo.syncStateInfo();
end

function UserInfo.syncPlayerInfo()
	local msgStr = ServerDateManager:getInstance():getUserBasicInfoForLua();
	local msg = Player_pb.PlayerInfo();
	msg:ParseFromString(msgStr);
	UserInfo.playerInfo = msg;	
end

function UserInfo.syncRoleInfo()
	local msgStr = ServerDateManager:getInstance():getUserRoleInfoForLua();
	if msgStr ~= "" then
		local msg = Player_pb.RoleInfo();
		msg:ParseFromString(msgStr);
		UserInfo.roleInfo = msg;
	end
	local SkillManager = require("SkillManager")
	SkillManager:checkNewSkill();
	UserInfo.checkLevelUp()
	local level = UserInfo.roleInfo.level or 0;
	UserInfo.level = level;
end

function UserInfo.syncStateInfo()
	--Player State 目前直接从lua里读取，不从c++中获得了 by zhenhui 2014/8/9
	--[[local msgStr = ServerDateManager:getInstance():getPlayerStateForLua();
	local msg = Player_pb.HPPlayerStateSync();
	msg:ParseFromString(msgStr);
	UserInfo.stateInfo = msg;--]]
end	

function UserInfo.checkLevelUp()
	local level = UserInfo.roleInfo.level or 0;
	if UserInfo.level < level and UserInfo.level~=0 then
		--play levelup timeline 		
		GameUtil:showLevelUpAni()
		local SkillManager = require("SkillManager")
		SkillManager:unlockSkillListener()
	end
end	

function UserInfo.reset()
	UserInfo.roleInfo.level = 0;
	UserInfo.level = 0;
	UserInfo.skillUnlock = {
		level = 0,
		hasNew = false
	};
	PageManager.showRedNotice("Skill", false);
	UserInfo.guideInfo = {
		Gift  = false,
		Equip = false,
		Help  = false
	};
end
--------------------------------------------------------------------------------

function UserInfo.getVipString()
	return "VIP " .. (UserInfo.playerInfo.vipLevel or 0);
end

function UserInfo.getVipImage()
	return string.format(GameConfig.Image.Vip, UserInfo.playerInfo.vipLevel or 0);
end

function UserInfo.getRoleAttrById(attrId)
	return PBHelper:getAttrById(UserInfo.roleInfo.attribute.attribute, attrId);
end

function UserInfo.getDamageString()
	return (UserInfo.getRoleAttrById(Const_pb.MINDMG) .. "-"
		 .. UserInfo.getRoleAttrById(Const_pb.MAXDMG));
end

function UserInfo.getEquipByPart(part)
	return PBHelper:getRoleEquipByPart(UserInfo.roleInfo.equips, part);
end	

function UserInfo.hasEquipInRole()
	UserInfo.syncRoleInfo();
	local roleEquipSize = #UserInfo.roleInfo.equips
	if roleEquipSize>0 then
		UserInfo.guideInfo.Equip = false;
		return true
	else
		if UserInfo.guideInfo.Equip then
			UserInfo.guideInfo.Help = true;
		end
		return false
	end		
end	

function UserInfo.getProfessionName()
	return common:getLanguageString("@ProfessionName_" .. UserInfo.roleInfo.prof);
end

--判断金币够不够，不够提示购买
function UserInfo.isCoinEnough(need)
	local need = tonumber(need or 0);
	UserInfo.syncPlayerInfo()
	if UserInfo.playerInfo.coin < need then
		PageManager.notifyLackCoin();
		return false;
	end
	return true;
end

--判断钻石够不够，不够提示购买
function UserInfo.isGoldEnough(need)
	local need = tonumber(need or 0);
	UserInfo.syncPlayerInfo()
	if UserInfo.playerInfo.gold < need then
		PageManager.notifyLackGold();
		return false;
	end
	return true;
end

function UserInfo.hasPassNewBeeFightBoss()
	local passedMapId = UserInfo.stateInfo.passMapId
	if passedMapId == 1 and NewBeeFightBossDone == true then
		return true
	else
		return false
	end
end

function UserInfo.isNewbeeInFightBoss()
	--passMapId ==0 means has not fight boss ever since the first boss is been beaten as designed by zhenhui 
	local passedMapId = UserInfo.stateInfo.passMapId
	local hasEquip = UserInfo.hasEquipInRole()
	if passedMapId ==0 and hasEquip == true  then
		return true
	end
	return false;
end

function UserInfo.isNewbeeInFastFight()
	local passedMapId = UserInfo.stateInfo.passMapId
	if passedMapId == 1 and UserInfo.stateInfo.fastFightBuyTimes == 0 then
		return true
	end
	return false;
end

function UserInfo.hasPassGiftEquip()
	return not (UserInfo.guideInfo.Gift or UserInfo.guideInfo.Equip);
end

function UserInfo.hasFastFight()
	local passedMapId = UserInfo.stateInfo.passMapId
	if passedMapId > 1 or (passedMapId == 1 and UserInfo.stateInfo.fastFightBuyTimes > 0) then
		return true;
	end
	return false;
end	

--获取全身装备强化等级
function UserInfo.getAllEnhancedLevel(isViewingOther)
	local roleInfo = isViewingOther and ViewPlayerInfo:getRoleInfo() or UserInfo.roleInfo;
	
	if #roleInfo.equips < GameConfig.Count.PartTotal then return 0; end
	
	local allLevel = 1000;
	for _, roleEquip in ipairs(roleInfo.equips) do
		local level  = roleEquip.strength;
		if level <= 0 then
			return 0;
		end
		if allLevel > level then
			allLevel = level;
		end
	end
	
	return allLevel;
end
--------------------------------------------------------------------------------
return UserInfo;
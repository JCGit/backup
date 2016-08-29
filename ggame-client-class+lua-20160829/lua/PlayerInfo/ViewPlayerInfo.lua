local ViewPlayerInfo = {
	snapshot = {};
	isFriend = false;
	isShield = false;
	isSendAllow = false;
};
--------------------------------------------------------------------------------
local Const_pb = require("Const_pb");
local HP_pb = require("HP_pb");
local Snapshot_pb = require("Snapshot_pb");

local PBHelper = require("PBHelper");

------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
function ViewPlayerInfo:getInfo(playerId)
	local msg = Snapshot_pb.HPSeeOtherPlayerInfo();
	msg.playerId = playerId;
	common:sendPacket(HP_pb.SEE_OTHER_PLAYER_INFO_C, msg,false);
end

function ViewPlayerInfo:setInfo(msgBuff)
	local msg = Snapshot_pb.HPSeeOtherPlayerInfoRet();
	msg:ParseFromString(msgBuff);
	self.snapshot = msg.playerInfo;
	self.isFriend = msg.isFriend
	self.isShield = msg.isShield
	self.isSendAllow = msg.isSendAllow
end

function ViewPlayerInfo:clearInfo()
	self.snapshot = {};
end	
--------------------------------------------------------------------------------
function ViewPlayerInfo:getRoleAttrById(attrId)
	return PBHelper:getAttrById(self:getRoleInfo().attribute.attribute, attrId);
end


function ViewPlayerInfo:getDamageString()
	return (self:getRoleAttrById(Const_pb.MINDMG) .. "-"
		 .. self:getRoleAttrById(Const_pb.MAXDMG));
end

function ViewPlayerInfo:getRoleInfo()
	return self.snapshot.mainRoleInfo;
end

function ViewPlayerInfo:getPlayerInfo()
	return self.snapshot.playerInfo;
end

function ViewPlayerInfo:getTittleInfo()
	return self.snapshot.titleInfo;
end

function ViewPlayerInfo:setIsFriend(flag)
	self.isFriend = flag
end

function ViewPlayerInfo:setIsShield(flag)
	self.isShield = flag
end

function ViewPlayerInfo:setIsSendAllow(flag)
	self.isSendAllow = flag
end


function ViewPlayerInfo:isFriendLabelStr()
	if self.isFriend then
		return common:getLanguageString("@DeleteFriend");
	else
		return common:getLanguageString("@AddFriend");
	end
end

function ViewPlayerInfo:isShieldLabelStr()
	if self.isShield then
		return common:getLanguageString("@UnshieldMsg");
	else
		return common:getLanguageString("@ShieldMsg");
	end
end

function ViewPlayerInfo:isSendAllowLabelStr()
	if self.isSendAllow then
		return common:getLanguageString("@PrivateChat");
	else
		return common:getLanguageString("@DisablePrivateChat");
	end
end

function ViewPlayerInfo:getProfessionName()
	return common:getLanguageString("@ProfessionName_" .. self:getRoleInfo().prof);
end

function ViewPlayerInfo:getRoleEquipByPart(part)
	return PBHelper:getRoleEquipByPart(self:getRoleInfo().equips, part);
end	

function ViewPlayerInfo:getEquipById(equipId)
	for _, equipInfo in pairs(self.snapshot.equipInfo) do
		if equipInfo.id == equipId then
			return equipInfo;
		end
	end
	return nil;
end
--------------------------------------------------------------------------------
return ViewPlayerInfo;
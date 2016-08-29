local EquipOprHelper = {};
--------------------------------------------------------------------------------
------------local variable for system api--------------------------------------
local tostring = tostring;
local tonumber = tonumber;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------
local EquipOpr_pb 	= require("EquipOpr_pb");
local Equip_pb = require("Equip_pb");
local Attribute_pb = require("Attribute_pb");
local HP_pb		= require("HP_pb");
local UserInfo = require("UserInfo");
--------------------------------------------------------------------------------
--ǿ��
function EquipOprHelper:enhanceEquip(equipId)
	local msg = EquipOpr_pb.HPEquipEnhance();
	msg.equipId = equipId;
	common:sendPacket(HP_pb.EQUIP_ENHANCE_C, msg);
end	

--ϴ��
function EquipOprHelper:baptizeEquip(equipId)
	local msg = EquipOpr_pb.HPEquipBaptize();
	msg.equipId = equipId;
	common:sendPacket(HP_pb.EQUIP_BAPTIZE_C, msg);
end	

--ж��/װ��/����
function EquipOprHelper:dressEquip(equipId, roleId, _type)
	local msg = EquipOpr_pb.HPEquipDress();
	msg.equipId = equipId;
	msg.roleId = roleId;
	msg.type = _type;
	common:sendPacket(HP_pb.EQUIP_DRESS_C, msg);
end	

--����
function EquipOprHelper:swallowEquip(equipId, swalledId)
	local msg = EquipOpr_pb.HPEquipSwallow();
	msg.equipId = equipId;
	for _, id in ipairs(swalledId) do
		msg.swallowedEquipId:append(id);
	end
	common:sendPacket(HP_pb.EQUIP_SWALLOW_C, msg);
end	

--��Ƕ
function EquipOprHelper:embedEquip(equipId, pos, stoneId)
	local msg = EquipOpr_pb.HPEquipStoneDress();
	msg.equipId = equipId;
	msg.punchPos = pos;
	msg.stoneId = stoneId;
	common:sendPacket(HP_pb.EQUIP_STONE_DRESS_C, msg);
end	

--����
function EquipOprHelper:extendEquip(equipId, extendedId)
	local msg = EquipOpr_pb.HPEquipExtend();
	msg.equipId = equipId;
	msg.extendedEquipId = extendedId;
	common:sendPacket(HP_pb.EQUIP_EXTEND_C, msg);
end

--���
function EquipOprHelper:punchEquip(equipId, pos)
	local msg = EquipOpr_pb.HPEquipPunch();
	msg.equipId = equipId;
	msg.punchPos = pos;
	common:sendPacket(HP_pb.EQUIP_PUNCH_C, msg);	
end	

--ж�±�ʯ
function EquipOprHelper:unloadGem(equipId, pos)
	local msg = EquipOpr_pb.HPEquipStoneUndress();
	msg.equipId = equipId;
	msg.pos = pos;
	common:sendPacket(HP_pb.EQUIP_STONE_UNDRESS_C, msg);
end

--һ��ж�±�ʯ
function EquipOprHelper:unloadAllGem(equipId)
	self:unloadGem(equipId, -1);
	--[[
	local msg = EquipOpr_pb.HPEquipStoneUndress();
	msg.equipId = equipId;
	common:sendPacket(HP_pb.EQUIP_STONE_UNDRESS_C, msg);
	--]]
end

--����
function EquipOprHelper:smeltEquip(equipIds)
	local msg = EquipOpr_pb.HPEquipSmelt();
	for _, id in ipairs(equipIds) do
		msg.equipId:append(id);
	end
	common:sendPacket(HP_pb.EQUIP_SMELT_C, msg);
end

--�����󣬱�ʯ�Զ��ر���
function EquipOprHelper:equipSmelted(msgBuff)
	local msg = EquipOpr_pb.HPEquipSmeltRet();
	msg:ParseFromString(msgBuff);
	if msg.gemUndress == 1 then
		MainFrame:getInstance():getMsgNodeForLua():runAction(
			CCSequence:createWithTwoActions(
				CCDelayTime:create(1.5),
				CCCallFunc:create(function()
					MessageBoxPage:Msg_Box_Lan("@GemAutoUnloaded");
				end)
			)
		);
		--common:popString(common:getLanguageString("@GemAutoUnloaded"), "COLOR_RED");
	end
end

--����
function EquipOprHelper:buildEquip(equipId)
	local msg = EquipOpr_pb.HPEquipCreate();
	msg.version = 1;
	common:sendPacket(HP_pb.EQUIP_SMELT_CREATE_C, msg);
end

--����
function EquipOprHelper:sellEquip(equipId)
	local msg = EquipOpr_pb.HPEquipSell();
	msg.equipId = equipId;
	common:sendPacket(HP_pb.EQUIP_SELL_C, msg);
end	

--��������
function EquipOprHelper:sellEquipsWithQuality(quality)
	local msg = EquipOpr_pb.HPEquipSell();
	msg.quality = quality;
	common:sendPacket(HP_pb.EQUIP_SELL_C, msg);
end	

--��ȡ������Ϣ
function EquipOprHelper:getSmeltEquipInfo()
	local msg = EquipOpr_pb.HPEquipSmeltInfo();
	msg.version = 1;
	common:sendPacket(HP_pb.EQUIP_SMELT_INFO_C, msg);
end

--ˢ�´�����Ϣ
function EquipOprHelper:refreshEquipBulding()
	local msg = EquipOpr_pb.HPEquipCreateAvailableRefresh();
	msg.version = 1;
	common:sendPacket(HP_pb.EQUIP_SMELT_REFRESH_C, msg, false);
end	

--��չ����
function EquipOprHelper:expandPackage()
	local msg = EquipOpr_pb.HPEquipBagExtend();
	msg.version = 1;
	common:sendPacket(HP_pb.EQUIP_BAG_EXTEND_C, msg, false);
end	

--�����������
function EquipOprHelper:buildGodlyEquip(cfgId)
	local msg = EquipOpr_pb.HPEquipSpecialCreate();
	msg.cfgId = tonumber(cfgId);
	common:sendPacket(HP_pb.EQUIP_SPECIAL_CREATE_C, msg);
end

--�����ϳ�
function EquipOprHelper:compoundEquip(fromId, toId)
	local msg = EquipOpr_pb.HPEquipCompound();
	msg.fromEquipId = fromId;
	msg.toEquipId = toId;
	common:sendPacket(HP_pb.EQUIP_COMPOUND_C, msg);
end

--ͬ������������Ϣ
function EquipOprHelper:syncSMeltInfo(pbMsg)
	local smeltInfo = EquipOpr_pb.HPEquipSmeltInfoRet();
	smeltInfo:ParseFromString(pbMsg);
	UserInfo.smeltInfo = smeltInfo;
end

function EquipOprHelper:syncEquipInfoFromMsg(pbMsg)
	if not UserEquipManager:hasInited() then return; end
	
	local syncInfo = Equip_pb.HPEquipInfoSync();
	syncInfo:ParseFromString(pbMsg);
	if not UserInfo.roleInfo.level then
		UserInfo.syncRoleInfo();
	end
	self:syncEquipInfos(syncInfo.equipInfos);
end

function EquipOprHelper:syncEquipInfos(equipInfos)
	for _, equipInfo in ipairs(equipInfos) do
		UserEquipManager:syncOneEquipInfo(equipInfo);
	end
end

function EquipOprHelper:deleteEquip(equipId)
	UserEquipManager:deleteEquip(equipId);
end
--------------------------------------------------------------------------------
return EquipOprHelper;
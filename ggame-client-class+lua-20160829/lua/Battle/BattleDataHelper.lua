require "HP_pb"
require "Battle_pb"
require "Chat_pb"
--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			BattleDataHelper.lua
--  DESCRIPTION:	BattleDataHelper page
--
--
--  AUTHOR:		Zhen Hui
--  CREATED:	2014/8/3
--  MODIFY2:
--
--  MODIFY1:
--
--------------------------------------------------------------------------------

local BattleDataHelper = {}

--ս������BattleInfo
BattleDataHelper.BattleDataSequence= {}

--�ж����߻��ǿ���
isOfflineFightReport = true

--����ս�����
BattleDataHelper.BattleOfflineStatics = {}

--����ս��
BattleDataHelper.FastBattleStatics  = {}

--ս������
BattleDataHelper.battleInfoList = {}

AllianceOpen = false;


--����ս���ᴫ�����´�ս����ʱ�䣬����н����ֱ�ӵ������ڣ���û����ȴʱ�䣬�����෴
BattleDataHelper.hasBattleColdTime = false
BattleDataHelper.nextBattleLeftTime = 0



--�Ƿ񹤻��Ա����
function BattleDataHelper.onReceiveAllianceSwitch(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Chat_pb.HPAllianceSwitch();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		--CCLuaLog("@onReceiveAllianceSwitch -- ");
		if msg~=nil then
			AllianceOpen = msg.tag;
			PageManager.refreshPage("BattlePage");
		else
			--CCLuaLog("@onReceiveAllianceSwitch -- error in data");
		end
	end
end
HPAllianceSwitchHandler = PacketScriptHandler:new(HP_pb.ALLIANCE_SWITCH_S, BattleDataHelper.onReceiveAllianceSwitch);




--����ս�����չʾ
function BattleDataHelper.onReceiveBattleOfflineAccountSync(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Battle_pb.HPOfflineAccount();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		--CCLuaLog("@onReceiveBattleOfflineAccountSync -- get the offline account data");
		-- ����ս�����չʾ
		
		if msg:HasField("result") then
			BattleDataHelper.BattleOfflineStatics = msg;
			if BattleDataHelper.BattleOfflineStatics~=nil then
				isOfflineFightReport = true
				----CCLuaLog(tostring(msg));
			end
			BattleDataHelper.hasBattleColdTime = false
			BattleDataHelper.nextBattleLeftTime = 0
		else
			BattleDataHelper.hasBattleColdTime = true
			local currentTime = GamePrecedure:getInstance():getServerTime()
			BattleDataHelper.nextBattleLeftTime  = (msg.nextBattleTime - currentTime) * 1000
			if BattleDataHelper.nextBattleLeftTime< 0  then
				BattleDataHelper.nextBattleLeftTime = 0;
			end
			----CCLuaLog("@onReceiveMapStaticSync -- error in data");
		end
	end
end
HPBattleOfflineAccountHandler = PacketScriptHandler:new(HP_pb.BATTLE_OFFLINE_ACCOUNT_S, BattleDataHelper.onReceiveBattleOfflineAccountSync);

--����ս�����չʾ
function BattleDataHelper.onReceiveFastFightAccount(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Battle_pb.HPFastBattleRet();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		--CCLuaLog("@onReceiveFastFightAccount -- get the offline account data");
		-- ����ս�����չʾ
		if msg~=nil then
			BattleDataHelper.FastBattleStatics = msg;
			if BattleDataHelper.FastBattleStatics ~=nil then
				isOfflineFightReport = fasle
				------CCLuaLog(tostring(msg));
				PageManager.pushPage("OfflineAccountPage");
			end
		else
			--CCLuaLog("@onReceiveMapStaticSync -- error in data");
		end
	end
end
HPBattleFastFightHandler = PacketScriptHandler:new(HP_pb.BATTLE_FAST_FIGHT_S, BattleDataHelper.onReceiveFastFightAccount);

--����ս��Э�飬�����������
function BattleDataHelper.onReceiveBattleInfoSync(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Battle_pb.BattleInfo();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		
		if not msg then return end
		local size = #BattleDataHelper.battleInfoList
		--������µ��Ƿ񾺼�������BOSS������Ǿ���������BOSS�����ǰ���List,ֱ�Ӳ��ź�����
		if msg.battleType ~= Battle_pb.BATTLE_PVE_MONSTER then
			if size>0 then
				for k in pairs (BattleDataHelper.battleInfoList) do
					BattleDataHelper.battleInfoList[k] = nil
				end
			end
		end
		--�������
		table.insert(BattleDataHelper.battleInfoList,msg);
		
		--����յ������ݣ�ֱ�����õȴ�ʱ��Ϊ1�룬1������ս��
		g_waitingFightTime = 1
	end
end	
HPBattleInfoHandler = PacketScriptHandler:new(HP_pb.BATTLE_INFO_SYNC_S, BattleDataHelper.onReceiveBattleInfoSync);


--�³�ս������ͬ��
function BattleDataHelper:onReceiveNextBattleType(msgBuff)
	local msg = Battle_pb.HPBattleNextInfo();
	msg:ParseFromString(msgBuff)
	if not msg then return end
	--if not msg:HasField("name") then return end
	
	if msg.battleType == Battle_pb.BATTLE_PVE_BOSS or msg.battleType == Battle_pb.BATTLE_PVE_ELITE_BOSS then
		if msg:HasField("name") then
			nextChanllengeName = msg.name 
			nextFightIsBoss = true
		else
			nextChanllengeName = common:getLanguageString("@EliteMap")
			nextFightIsBoss = true
		end
		
	elseif msg.battleType == Battle_pb.BATTLE_PVP_ARENA then
		nextChanllengeName = msg.name
		nextFightIsBoss = false
	end
	
	if msg.battleType == Battle_pb.BATTLE_PVE_ELITE_BOSS  then
		PageManager.popPage("EliteMapRewardPopup")
		PageManager.showFightPage();
	end
	
	if msg:HasField("waitTime") then 
		g_waitingFightTime = msg.waitTime
		PageManager.refreshPage("BattlePage","ArenaOrBoss");
	else
		PageManager.refreshPage("BattlePage");
	end
	
end

--��ʼ��ս������,��ȡս��Э����еĵ�һ�����Ƶ�BattleDataHelper.BattleDataSequence
function BattleDataHelper:onInitBattleData(container)
	local ret = true
	local listCount = table.getn(BattleDataHelper.battleInfoList)
	if listCount>0 then
		--��ȡ��һ�������е�Ԫ��,��ֵ��BattleDataHelper.BattleDataSequence����ɾ����һ��
		local head = table.remove(BattleDataHelper.battleInfoList,1)
		if head~=nil then
			BattleDataHelper.BattleDataSequence = head
			ret = true
		else
			ret = false
		end
	else
		ret = false
	end
	return ret
end

function BattleDataHelper:validateAndRegister()
	HPBattleInfoHandler:registerFunctionHandler(BattleDataHelper.onReceiveBattleInfoSync)
	HPBattleFastFightHandler:registerFunctionHandler(BattleDataHelper.onReceiveFastFightAccount)
	HPBattleOfflineAccountHandler:registerFunctionHandler(BattleDataHelper.onReceiveBattleOfflineAccountSync)
	HPAllianceSwitchHandler:registerFunctionHandler(BattleDataHelper.onReceiveAllianceSwitch)
end

function BattleDataHelper:resetData()
	for i=1,#BattleDataHelper.battleInfoList do
		table.remove(BattleDataHelper.battleInfoList);
	end
	BattleDataHelper.hasBattleColdTime = false
	BattleDataHelper.BattleOfflineStatics = nil
	BattleDataHelper.FastBattleStatics = nil
end

return BattleDataHelper;
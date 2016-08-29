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

--战斗序列BattleInfo
BattleDataHelper.BattleDataSequence= {}

--判断离线还是快速
isOfflineFightReport = true

--离线战斗结果
BattleDataHelper.BattleOfflineStatics = {}

--快速战斗
BattleDataHelper.FastBattleStatics  = {}

--战斗队列
BattleDataHelper.battleInfoList = {}

AllianceOpen = false;


--离线战斗会传过来下次战斗的时间，如果有结果，直接弹出窗口，且没有冷却时间，否则相反
BattleDataHelper.hasBattleColdTime = false
BattleDataHelper.nextBattleLeftTime = 0



--是否工会成员开关
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




--离线战斗结果展示
function BattleDataHelper.onReceiveBattleOfflineAccountSync(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Battle_pb.HPOfflineAccount();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		--CCLuaLog("@onReceiveBattleOfflineAccountSync -- get the offline account data");
		-- 离线战斗结果展示
		
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

--快速战斗结果展示
function BattleDataHelper.onReceiveFastFightAccount(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Battle_pb.HPFastBattleRet();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		--CCLuaLog("@onReceiveFastFightAccount -- get the offline account data");
		-- 快速战斗结果展示
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

--接收战斗协议，并插入队列中
function BattleDataHelper.onReceiveBattleInfoSync(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Battle_pb.BattleInfo();
		local msgbuff = handler:getRecPacketBuffer();
		msg:ParseFromString(msgbuff)
		
		if not msg then return end
		local size = #BattleDataHelper.battleInfoList
		--检测最新的是否竞技场或者BOSS，如果是竞技场或者BOSS，清掉前面的List,直接播放后续的
		if msg.battleType ~= Battle_pb.BATTLE_PVE_MONSTER then
			if size>0 then
				for k in pairs (BattleDataHelper.battleInfoList) do
					BattleDataHelper.battleInfoList[k] = nil
				end
			end
		end
		--插入队列
		table.insert(BattleDataHelper.battleInfoList,msg);
		
		--如果收到新数据，直接重置等待时间为1秒，1秒后进入战斗
		g_waitingFightTime = 1
	end
end	
HPBattleInfoHandler = PacketScriptHandler:new(HP_pb.BATTLE_INFO_SYNC_S, BattleDataHelper.onReceiveBattleInfoSync);


--下场战斗类型同步
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

--初始化战斗数据,读取战斗协议队列的第一个，推到BattleDataHelper.BattleDataSequence
function BattleDataHelper:onInitBattleData(container)
	local ret = true
	local listCount = table.getn(BattleDataHelper.battleInfoList)
	if listCount>0 then
		--获取第一个队列中的元素,赋值给BattleDataHelper.BattleDataSequence，并删除第一个
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
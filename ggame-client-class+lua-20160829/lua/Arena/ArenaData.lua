require "HP_pb"
require "Arena_pb.lua"

local ArenaData = {}

ArenaDataPageInfo = nil

function onReceiveArenaDataPageInfo(eventName,handler)
	if eventName == "luaReceivePacket" then
		local msg = Arena_pb.HPArenaDefenderListSyncS()
		local msgbuff = handler:getRecPacketBuffer()
		msg:ParseFromString(msgbuff)				
		if msg ~= nil then
			ArenaDataPageInfo = msg
			table.sort(ArenaDataPageInfo.defender,
				function (e1, e2)
				if not e2 then return true end
				if not e1 then return false end
		
				return e1.rank < e2.rank
			end
			)
			--刷新需要显示的页面信息，避免切换到排名页面有短暂不一致现象
			ArenaRankCacheInfo.self.rank = msg.self.rank
			ArenaRankCacheInfo.self.fightValue = msg.self.fightValue
			ArenaRankCacheInfo.self.rankAwardsStr = msg.self.rankAwardsStr
			local msg = MsgMainFrameRefreshPage:new()
			msg.pageName = "ArenaPage"
			MessageManager:getInstance():sendMessageForScript(msg)
		else
			CCLuaLog("@onReceiveArenaDataPageInfo -- error in data")
		end
	end 
end

ArenaDataPageHandler = PacketScriptHandler:new(HP_pb.ARENA_DEFENDER_LIST_SYNC_S, onReceiveArenaDataPageInfo)

function ArenaData.validateAndRegister()
	
	ArenaDataPageHandler:registerFunctionHandler(onReceiveArenaDataPageInfo) 
end

function ArenaData.setArenaLastTimes( lastTime )
	ArenaDataPageInfo.self.surplusChallengeTimes = lastTime
end

return ArenaData
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");

local BaseDataHelper = require("BaseDataHelper")
local CumulativeLoginConfig = require("CumulativeLoginConfig")

local CumulativeLoginDataHelper = BaseDataHelper:new(CumulativeLoginConfig)
CumulativeLoginDataHelper.CumulativeLoginDays = 1
CumulativeLoginDataHelper.RemainTime = tonumber(-1)
CumulativeLoginDataHelper.mReceiveDays = 1
CumulativeLoginDataHelper.gotAwardCfgId = {}
--获得当前首个未领取
function CumulativeLoginDataHelper:pickFirstUnReward()
    for i=1,#self.gotAwardCfgId do
        if self.gotAwardCfgId[i+1]~=nil then
            if (self.gotAwardCfgId[i+1] - self.gotAwardCfgId[i]) > 1 then
                self.mReceiveDays = i+1;
                break
            end
        end
        if i == #self.gotAwardCfgId then
            self.mReceiveDays = i + 1
        end
    end
    
    if self.mReceiveDays > #self.config.DailyRewardItem then
        self.mReceiveDays = #self.config.DailyRewardItem
    end
end
--收包
function CumulativeLoginDataHelper:onReceivePacket(container,page)
    local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == HP_pb.ACC_LOGIN_INFO_S then
		local msg = Activity_pb.HPAccLoginInfoRet();
		msg:ParseFromString(msgBuff);
		
		self.CumulativeLoginDays = msg.loginDays
		self.RemainTime = msg.leftTime
		self.gotAwardCfgId = msg.gotAwardCfgId
		
		table.sort(self.gotAwardCfgId,function(a,b) return (a<b) end)
		self:pickFirstUnReward()
		
		page:refreshPage(container)
    elseif opcode == HP_pb.ACC_LOGIN_AWARDS_S then
		local msg = Activity_pb.HPAccLoginAwardsRet();
		msg:ParseFromString(msgBuff);
		
		self.RemainTime = msg.leftTime
		self.gotAwardCfgId = msg.gotAwardCfgId
		
		table.sort(self.gotAwardCfgId,function(a,b) return (a<b) end)
        self:pickFirstUnReward()
        
		page:refreshPage(container)
	end
end

return CumulativeLoginDataHelper
local BaseDataHelper = require("BaseDataHelper")
local MainConfig = require("MailConfig")
local Mail_pb = require("Mail_pb");
local HP = require("HP_pb");


local MailDataHelper = BaseDataHelper:new(MainConfig)
MailDataHelper.mailInvalidateList = {}
MailDataHelper.mailAreanAll = {}
MailDataHelper.mails = {}
MailDataHelper.commonMails = {}
MailDataHelper.systemMails = {}
MailDataHelper.lastMail = {}
MailDataHelper.requestId = 0
MailDataHelper.SystemMail = {}

function MailDataHelper:onReceivePacket(container,page)
    local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == HP_pb.MAIL_INFO_S then
	    local msg = Mail_pb.OPMailInfoRet()
		msg:ParseFromString(msgBuff)
		if msg~=nil then
			local maxSize = table.maxn(msg.mails);
			if maxSize > 0 then
			    self.lastMail = msg.mails[maxSize]
			end
			for i=1,maxSize,1 do
				local mail = msg.mails[i];
				local mailId = mail.id;
				if self.mailInvalidateList[mailId] ~=nil then
					CCLuaLog("already in the list");
				else					
					self.mailInvalidateList[mailId] = true
					if mail.type == Mail_pb.ARENA_ALL then
						table.insert(self.mailAreanAll , mail)
					else
						table.insert(self.mails , mail)
					end
				end
			end
			table.sort(MailDataHelper.mails,
			function ( e1, e2 )			
				if not e2 then return true end
				if not e1 then return false end
				if e1.type == Mail_pb.Reward and e2.type ~= Mail_pb.Reward then return true end
				if e1.type ~= Mail_pb.Reward and e2.type == Mail_pb.Reward then return false end
				if e1.type == Mail_pb.Reward and e2.type == Mail_pb.Reward then 
					--mailId = 7 为月卡
					if e1.mailId == 7 and e2.mailId ~= 7 then return true end 
					if e1.mailId ~= 7 and e2.mailId == 7 then return false end
					return e1.id + 0 > e2.id + 0
				end
				return e1.id + 0 > e2.id + 0
			end
			);
			
			-- new mail notice
			if maxSize > 0 then
				local msg = MsgMainFrameGetNewInfo:new()
				msg.type = Const_pb.NEW_MAIL;
				MessageManager:getInstance():sendMessageForScript(msg)
				
				local msg = MsgMainFrameRefreshPage:new()
				msg.pageName = "MailPage";
				MessageManager:getInstance():sendMessageForScript(msg)
				
				local msg1 = MsgMainFrameRefreshPage:new()
				msg1.pageName = "ArenaRecordPage";
				MessageManager:getInstance():sendMessageForScript(msg1)
			end
		end						
	end
	self:RefreshMail()
end

function MailDataHelper:RefreshMail()
    self.commonMails = {}
    self.systemMails = {}
    for i=1,#self.mails do
        local mail = self.mails[i]
        if mail:HasField("mailClassify") then
            if mail.mailClassify == 2 then
                table.insert(self.systemMails , mail)
            else
                table.insert(self.commonMails , mail)
            end
        end
    end
end


function MailDataHelper:ResetMailData()
    self.mailInvalidateList = {}
    self.mailAreanAll = {}
    self.mails = {}
    self.lastMail = {}
    self.requestId = 0
end


return MailDataHelper
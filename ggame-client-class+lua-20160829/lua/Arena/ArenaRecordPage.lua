registerScriptPage("LeaveMessageDetailPage")
local thisPageName = "ArenaRecordPage"
local Mail_pb = require("Mail_pb")
local HP = require("HP_pb")
local NodeHelper = require("NodeHelper")
local MailDataHelper = require("MailDataHelper")


local option = {
	ccbiFile = "ArenaRecordPopUp.ccbi",
	handlerMap = {
		onHelp				= "onHelp",
		onClose				= "onClose"
	}
	--opcode = opcodes
}

local ArenaRecordPageBase = {}
local ArenaRecordItem = {}
local MailContetnCfg = ConfigManager.getMailContentCfg()
local RoleConfig = {}
-----------------------------------------------------------------

function ArenaRecordItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		ArenaRecordItem.onRefreshItemView(container)
	elseif eventName == "onHand" then
		ArenaRecordItem.onHand(container)
	elseif eventName == "onLeaveMessage" then
		ArenaRecordItem.onLeaveMessage(container)
	end
end 

function ArenaRecordItem.onRefreshItemView( container )
	local index = container:getItemDate().mID
	local mail = MailDataHelper:getVariableByKeyAndIndex("mailAreanAll",index)
	local itemInfo = {
		[1] = "", 	-- name
		[2] = 0,	-- id
		[3] = 0,	-- level
		[4] = 0		-- itemId
	}
	local sTitle = ""	
	if mail.mailId ~= 0 then
		local mailObj = MailContetnCfg[mail.mailId];
		if mailObj ~= nil then

			sTitle = mailObj.content

			local pSize = table.maxn(mail.params)
			for i=1,pSize - 4,1 do
				local vStr = "#v"..i.."#";
				local params = common:checkStringLegal( mail.params[i] )
				sTitle = GameMaths:replaceStringWithCharacterAll(sTitle,vStr,mail.params[i]);
			end
			
			for i = 1,4,1 do
				itemInfo[i] = mail.params[pSize - 4 + i]
			end
			
		end
	else
		sTitle = common:stringAutoReturn(mail.title, REWRAD_LINE_COUNT);
	end
	
	local str = FreeTypeConfig[56].content
	str = GameMaths:replaceStringWithCharacterAll(str,"#v1#",sTitle)
	
	labelNode = container:getVarLabelTTF("mArenaRecordTex")
	local tag = GameConfig.Tag.HtmlLable
	local size = CCSizeMake(GameConfig.LineWidth.ArenaRecordContent, 200);
	
	if labelNode ~= nil then
		NodeHelper:addHtmlLable(labelNode, str ,tag, size)
	end
	
	container:getVarLabelBMFont("mLv"):setString( "LV." .. itemInfo[3] )
	
	local headPic = RoleConfig[tonumber(itemInfo[4])]["icon"]
	
	NodeHelper:setSpriteImage(container, {mPic = headPic})
	
end

function ArenaRecordItem.onHand( container )
	local index = container:getItemDate().mID
	local mail = MailDataHelper:getVariableByKeyAndIndex("mailAreanAll",index)
	local itemInfo = {
		[1] = "", 	-- name
		[2] = 0,	-- id
		[3] = 0,	-- level
		[4] = 0		-- itemId
	}
	local pSize = table.maxn(mail.params)
	for i = 1,4,1 do
		itemInfo[i] = mail.params[pSize - 4 + i]
	end
	
	PageManager.viewPlayerInfo(tonumber(itemInfo[2]), true)
	
	
end

function ArenaRecordItem.onLeaveMessage( container )
	local index = container:getItemDate().mID
	local mail = MailDataHelper:getVariableByKeyAndIndex("mailAreanAll",index)
	local itemInfo = {
		[1] = "", 	-- name
		[2] = 0,	-- id
		[3] = 0,	-- level
		[4] = 0		-- itemId
	}
	local pSize = table.maxn(mail.params)
	for i = 1,4,1 do
		itemInfo[i] = mail.params[pSize - 4 + i]
	end
	
	LeaveMsgDetail_setPlayId( tonumber(itemInfo[2]) , itemInfo[1] )
	PageManager.pushPage("LeaveMessageDetailPage")
	
end

function ArenaRecordPageBase:onEnter( container )
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	RoleConfig = ConfigManager.getRoleCfg()	
	NodeHelper:initScrollView( container,"mContent",5)
	self:getPageInfo( container )
end

function ArenaRecordPageBase:getPageInfo( container )
	if MailDataHelper:getVariableByKey("mailAreanAll") ~= nil and #MailDataHelper:getVariableByKey("mailAreanAll") ~= 0 then
		self:rebuildAllItem(container)
	end
	
	local msg = Mail_pb.OPMailInfo()

	if MailDataHelper:getVariableByKey("lastMail") ~= nil and MailDataHelper:getVariableByKey("lastMail").id ~= nil then
		msg.version = MailDataHelper:getVariableByKey("lastMail").id
	else
		msg.version = 0;
	end

	local pb_data = msg:SerializeToString()
	container:sendPakcet(HP.MAIL_INFO_C, pb_data, #pb_data, true)
end

function ArenaRecordPageBase:onExecute( container )
	
end

function ArenaRecordPageBase:onExit( container )
	NodeHelper:deleteScrollView(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)	
end

function ArenaRecordPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_ARENA)
end

function ArenaRecordPageBase:onClose( container )
	PageManager.popPage(thisPageName)
end

function ArenaRecordPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			self:rebuildAllItem(container)
		end
	end
end

function ArenaRecordPageBase:rebuildAllItem( container )
	self:clearAllItem(container)
	self:buildItem(container)
end

function ArenaRecordPageBase:clearAllItem( container )
	NodeHelper:clearScrollView(container)
end

function ArenaRecordPageBase:buildItem( container )
	if (MailDataHelper:getVariableByKey("mailAreanAll") == nil) then
		return;
	end

	local maxSize = table.maxn(MailDataHelper:getVariableByKey("mailAreanAll"));

	NodeHelper:buildScrollViewWithCache(container, maxSize, "ArenaRecordContent.ccbi", ArenaRecordItem.onFunction);
end

-----------------------------------------------------------------
local CommonPage = require("CommonPage")
ArenaRecordPage = CommonPage.newSub(ArenaRecordPageBase, thisPageName, option)
----------------------------------------------------------------------------------
--[[
FILE:			CDKeyPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	换技能
AUTHOR:			zhenhui
CREATED:		2014-08-6
--]]
----------------------------------------------------------------------------------
local thisPageName = "CDKeyPage"

local hp = require('HP_pb')
require("Cdk_pb")

local option = {
	ccbiFile = "CDKeyPopUp.ccbi",
	handlerMap = {
		onCancel				= "onCancel",
		onInput				= "onInput",
		onConfirmation		= "onSave",
		luaInputboxEnter 	= "onInputboxEnter"
	}
}

local CDKeyPageBase = {}
local cdkey = nil
local NodeHelper = require("NodeHelper");
--------------------------------------------------------------

----------------------------------------------------------------------------------

-----------------------------------------------
--CDKeyPageBase页面中的事件处理
----------------------------------------------
function CDKeyPageBase:onEnter(container)
	container:registerPacket(HP_pb.USE_CDK_S)
	self:refreshPage(container);
end

function CDKeyPageBase:onExecute(container)
end

function CDKeyPageBase:onExit(container)
	container:removePacket(HP_pb.USE_CDK_S)
end
----------------------------------------------------------------

function CDKeyPageBase:refreshPage(container)
	
end

----------------click event------------------------

function CDKeyPageBase:onInput(container)
	cdkey = nil
	container:registerLibOS()
	libOS:getInstance():showInputbox(false);	
end


function CDKeyPageBase:onInputboxEnter(container)
	 
	local content = container:getInputboxContent();
	local nameOK = true
    if GameMaths:isStringHasUTF8mb4(content) then
		nameOK = false
    end		
    if content == "" then
		nameOK = false
    end
	if not nameOK then
        MessageBoxPage:Msg_Box("@NameHaveForbbidenChar")
        content = nil
        return
    end		
	cdkey = content;
	local contentLabel = container:getVarLabelBMFont("mCDKey");
	if contentLabel ~= nil then		
		--[[local length = GameMaths:calculateStringCharacters(content);
		if  length > 30 then 
			--提示名字字数
			MessageBoxPage:Msg_Box_Lan("@NameExceedLimit");
			return;
		end	--]]
		contentLabel:setString(tostring(cdkey));
	end
end


function CDKeyPageBase:onCancel(container)
	PageManager.popPage(thisPageName);
end

function CDKeyPageBase:onSave(container)
	
	if cdkey == nil then
		MessageBoxPage:Msg_Box_Lan("@PleaseEnterCDKFirst");
		return 
	end
	
	local msg = Cdk_pb.HPUseCdk()
	msg.cdkey = cdkey	
	local pb = msg:SerializeToString()
	PacketManager:getInstance():sendPakcet(HP_pb.USE_CDK_C,pb,#pb,false);		
	--PageManager.popPage(thisPageName)
end

--回包处理

function CDKeyPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()
	
	if opcode == HP_pb.USE_CDK_S then
		local msg = Cdk_pb.HPUseCdkRet()
		msg:ParseFromString(msgBuff)
		if msg.status == 1 then
			MessageBoxPage:Msg_Box_Lan("@CDKeyNumNotExist");
		elseif msg.status == 2 then
			MessageBoxPage:Msg_Box_Lan("@CDKeyNumNoOpenUse");
		elseif msg.status == 3 then
			MessageBoxPage:Msg_Box_Lan("@CDKeyNumExpire");
		elseif msg.status == 4 then
			MessageBoxPage:Msg_Box_Lan("@CDKeyNumAlreadyUse");
		elseif msg.status == 5 then
			MessageBoxPage:Msg_Box_Lan("@CDKeyNumPlatformNotRight");
		elseif msg.status == 6 then
			MessageBoxPage:Msg_Box_Lan("@CDKeyNumSameKeyUsed");
		end			
		return
	end		
end


--继承此类的活动如果同时开，消息监听不能同时存在,通过tag来区分
--[[
function CDKeyPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			local posx = container.mScrollView:getContainer():getPositionX();
			local posy = container.mScrollView:getContainer():getPositionY();
			CDKeyPageBase:rebuildAllItem(container)
			container.mScrollView:getContainer():setPosition(ccp(posx,posy));
		end
	end
end
--]]

--[[
function CDKeyPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function CDKeyPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
--]]
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
CDKeyPage = CommonPage.newSub(CDKeyPageBase, thisPageName, option);
--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  DESCRIPTION:	断线返回登录页面
--				
--				
--  AUTHOR:		liu longfei
--  CREATED:	2014-06-13
-------------------------------------------------------------------------------

local opcodes = {

}

local ConnectFailedPage = {};
local opcodes = {
}

function luaCreat_ConnectFailedPage(container)
	CCLuaLog("OnCreat_ConnectFailedPage")
	container:registerFunctionHandler(ConnectFailedPage.onFunction)
end

function ConnectFailedPage.onFunction(eventName,container)
	if eventName == "luaInit" then
		ConnectFailedPage.onInit(container);
	elseif eventName == "luaEnter" then
		ConnectFailedPage.onEnter(container);
	elseif eventName == "luaExit" then
		ConnectFailedPage.onExit(container);
	elseif eventName == "luaExecute" then
		ConnectFailedPage.onExecute(container);
	elseif eventName == "luaLoad" then
		ConnectFailedPage.onLoad(container);
	elseif eventName == "luaUnLoad" then
		ConnectFailedPage.onUnLoad(container);
	elseif eventName == "luaGameMessage" then
		ConnectFailedPage.onGameMessage(container);
	elseif eventName == "luaReceivePacket" then
		ConnectFailedPage.onReceivePacket(container);
	elseif eventName == "luaInputboxEnter" then
		ConnectFailedPage.onInputboxEnter(container);
	elseif eventName == "onDetermine" then
		--确定
		PageManager.showPage("MainPage",true);		
		GamePrecedure:getInstance():enterLoading();
		GamePrecedure:getInstance():reEnterLoading();
	elseif eventName == "onClose" then 
		--取消
					
	end
end		

function ConnectFailedPage.onInit(container)
	
end	

function ConnectFailedPage.onEnter(container)	
	local title = container:getVarLabelTTF("mTitle");
	if title ~= nil then
		title:setString(tostring(Language:getInstance():getString("@LeaguePrompt")));
	end
	
end

function ConnectFailedPage.onExit(container)
	
end

function ConnectFailedPage.onExecute(container)
	
end

function ConnectFailedPage.onLoad(container)
	container:loadCcbiFile("ConnectionFailedPopUp.ccbi");
end

function ConnectFailedPage.onUnLoad(container)
	
end

function ConnectFailedPage.onGameMessage(container)
	
end

function ConnectFailedPage.onReceivePacket(container)
	--if container:getRecPacketOpcode() == opcodes.OPCODE_DISSOLUTION_ALLIANCE_S then
				
	--end
end




















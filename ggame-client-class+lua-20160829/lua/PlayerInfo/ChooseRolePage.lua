--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			ChooseRolePage.lua
--  DESCRIPTION:	ChooseRolePage page 
--
--
--  AUTHOR:		Zhen Hui
--  CREATED:	2014/7/30
--  MODIFY2:
--
--  MODIFY1:
--
--------------------------------------------------------------------------------
local HP = require("HP_pb");
local Player_pb = require("Player_pb");

local NodeHelper = require("NodeHelper");
------------local variable for system api--------------------------------------
local tostring = tostring;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------

local ChooseRolePageInfo = {};

local opcodes=
{
	-- 角色创建
	ROLE_CREATE_C = HP.ROLE_CREATE_C
};

local name2RoleId = {
	ManWorrior 		= 1,
	ManHunter		= 2,
	ManMage			= 3,
	WomanWorrior	= 4,
	WomanHunter		= 5,
	WomanMage		= 6
};

local g_UserName = nil;
local g_RoleId = name2RoleId[name2RoleId.ManWorrior];
local DefaultRoleName = "ManWorrior";
local selectedRoleName = nil;
local roleCfg = {};


function luaCreat_ChooseRolePage(container)
	container:registerFunctionHandler(ChooseRolePageInfo.onFunction);
end

function ChooseRolePageInfo.onFunction(eventName,container)
	
	if eventName == "luaInit" then
		return;
	elseif eventName == "luaLoad" then
		ChooseRolePageInfo.onLoad(container);
	elseif eventName == "luaEnter" then
		ChooseRolePageInfo.onEnter(container);
	elseif eventName == "luaExecute" then
		return;
	elseif eventName == "luaExit" then
		ChooseRolePageInfo.onExit(container);
	elseif eventName == "luaUnLoad" then
		ChooseRolePageInfo.onUnLoad(container);
	elseif eventName == "luaReceivePacket" then
		return;
	elseif eventName == "luaGameMessage" then
		ChooseRolePageInfo.onGameMessage(container);
	elseif eventName == "luaInputboxEnter" then
		ChooseRolePageInfo.onInputboxEnter(container);
	elseif eventName == "onCreateRole" then
		ChooseRolePageInfo.onCreateRole(container);
	elseif eventName == "onUserNameBtn" then
		container:registerLibOS()
		g_UserName = nil
		libOS:getInstance():showInputbox(false);
	else
		local roleName = string.sub(eventName, 3);
		if roleName and name2RoleId[roleName] then
			ChooseRolePageInfo.chooseRole(container, roleName);
		end
	end
end


function ChooseRolePageInfo.onInputboxEnter(container)
	 
	local content = container:getInputboxContent();
	local nameOK = true
    if GameMaths:isStringHasUTF8mb4(content) then
		nameOK = false
    end
	if not RestrictedWord:getInstance():isStringOK(content) then
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
	
	g_UserName = content;
	local contentLabel = container:getVarLabelTTF("mName");
	if contentLabel ~= nil then		
		local length = GameMaths:calculateStringCharacters(content);
		if  length > GameConfig.WordSizeLimit.RoleNameLimit then 
			--提示名字字数
			MessageBoxPage:Msg_Box_Lan("@NameExceedLimit");
			return;
		end	
		contentLabel:setString(tostring(g_UserName));
	end
end

function ChooseRolePageInfo.onCreateRole(container)
	if g_UserName~=nil then
		local message = Player_pb.HPRoleCreate();
	
		if message ~= nil then	
			message.roleItemId = g_RoleId;	
			message.roleName = g_UserName;				
			local pb_data = message:SerializeToString();
			PacketManager:getInstance():sendPakcet(opcodes.ROLE_CREATE_C,pb_data,#pb_data,true);			
		end
	else
		MessageBoxPage:Msg_Box_Lan("@PleaseEnterNameFirst");
	end
	
end	

function ChooseRolePageInfo.onLoad(container)
	--进入页面时，重置当前状态	
	container:loadCcbiFile("ChoiceRolePage.ccbi",false);	
	roleCfg = ConfigManager.getRoleCfg();
end

function ChooseRolePageInfo.onEnter(container)
	ChooseRolePageInfo.chooseRole(container, DefaultRoleName);
end

function ChooseRolePageInfo.onExit(container)
	selectedRoleName = nil;
end

function ChooseRolePageInfo.onUnLoad(container)
end

function ChooseRolePageInfo.chooseRole(container, roleName)
	if roleName == selectedRoleName then return end
	
	g_RoleId = name2RoleId[roleName];
	
	local nodeVisible = {
		["mChoice" .. roleName]			= true
	};
	if selectedRoleName then
		nodeVisible["mChoice" .. selectedRoleName] = false;
	end
	NodeHelper:setNodesVisible(container, nodeVisible);
	
	selectedRoleName = roleName;
	
	ChooseRolePageInfo.showDescription(container, g_RoleId);
end

function ChooseRolePageInfo.showDescription(container, roleId)
	--Todo
	local professionId = roleCfg[roleId]["profession"];
	local lb2Str = {
		mPromptTex1 = common:getLanguageString("@RoleDescription_" .. professionId),
		mPromptTex2 = common:getLanguageString("@RoleKeywords_" .. professionId)
	};
	NodeHelper:setStringForLabel(container, lb2Str);
end
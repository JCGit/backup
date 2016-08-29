--encoding UTF-8 without BOM
--------------------------------------------------------------------------------
--  FILE:			main.lua
--  DESCRIPTION:	lua脚本程序代码入口，在C++端执行pEngine->executeString("require \"main\"");调用
--
--
--  AUTHOR:
--  CREATED:	2013-10-01
--  MODIFY2:	zhengxin at 2013-12-09
--				整理可读性

--  MODIFY1:	author at 2013-11-01
--				something
--------------------------------------------------------------------------------
--avoid memory leak
collectgarbage("setpause", 200)
collectgarbage("setstepmul", 100)

--引入其他module，注意顺序有依赖
local dirs = {
	'include',
	'Activity', 'Arena', 'Battle', 'Chat', 'Equip',
	'Friend', 'Guide', 'Guild', 'Item', 'Mail',
	'Mercenary', 'PlayerInfo', 'PVP', 'Recharge', 'Skill','protobuf'
};	
local pathTB = {};
for _, dir in ipairs(dirs) do
	table.insert(pathTB, string.format('.\\lua\\%s\\?.lua', dir));
end
package.path = package.path .. '.\\lua\\?.lua;' .. table.concat(pathTB, ';');

--初始化
require "Login_pb"



common = require("common");
require("GameUtil");
require("PackageLogicForLua");
require("IncPbCommon");
require("ListenMessage");
require("IncScriptPage");

--------------------------------------------------------------------------------
--本module导出的全局变量
--Variables
--json = require('json')
--------------------------------------------------------------------------------
--本module内部的局部变量、函数

--Variables
local isFirstUpdate = true

--当前的playerId serverId,用于判断是否需要reset，如果是同样的id,不用reset数据，如果不同需要reset
--g_curPlayerId = 0
--g_curServerId = 0

--updateHandler can be removed after big version update
local updateHandler = {}
--Functions
local function refreshImageset()
	
end
--
local function requestPackages()
	
end
--
local function onFirstUpdate()
	refreshImageset()
	requestPackages()
	
	isFirstUpdate = false
end
--------------------------------------------------------------------------------
--本module导出的全局变量
--Functions
function RegisterUpdateHandler(name, func)
	updateHandler[name] = func
end
--
function RemoveUpdateHandler(name)
	if name == nil then return end
	updateHandler[name] = nil
end
--
function GamePrecedure_preEnterMainMenu()
    GameUtil:createFont()

	CCLuaLog("GamePrecedure_preEnterMainMenu")
	--已经登录成功后，刷新最新的playerId和serverid
--	g_curPlayerId = ServerDateManager:getInstance().mLoginInfo.m_iPlayerID
--	g_curServerId = GamePrecedure:getInstance():getServerID()
end
--
function GamePrecedure_enterBackGround()
	CCLuaLog("GamePrecedure_enterBackGround")
end
--
function GamePrecedure_enterForeground()
	CCLuaLog("GamePrecedure_enterForeground")
end
--
function GamePrecedure_update(gameprecedure)
	if isFirstUpdate then
		onFirstUpdate();
	end
	--CCLuaLog("GamePrecedure_update")
	for name, func in pairs(updateHandler) do
		func()
	end
end
--------------------------------------------------------------------------------
--本module的执行代码主体

CCLuaLog("main.lua excute");

--------------------------------------------------------------------------------
 
--重置lua页面及数据，为重登陆做准备
--different player id login, reset all data
function resetAllLuaDataAndScene()
	local needReset = true	
	if needReset then	
		UserEquipManager:reset()		
		PackagePage_reset()		
		BattlePage_Reset()		
		ArenaPage_Reset()
		ChatList_Reset() -- 切换账号登陆，删除聊天记录
		if LeaveMessagePage_reset then
			LeaveMessagePage_reset()
		end
		if LeaveMessageDetailPage_reset then
			LeaveMessageDetailPage_reset()
		end	
		GuildDataManager = require("GuildDataManager")		
		GuildDataManager:resetTwoPage()

		ResetGiftPage()		
		RESETINFO_MARKET()
		RESETINFO_MAILS()
		RESETINFO_TEAMBATTLE()
		RESETINFO_NOTICE_STATE()	
		local UserInfo = require("UserInfo");	
		UserInfo.reset();
		MeltPage_reset();
		SkillManager_Reset()		
		isFirstUpdate = true;
		if ProfessionRankingPage_reset then
			ProfessionRankingPage_reset()
		end
		ChatManager_reset()
		require("BattleFightHelper")		
	end	
end

--same player id login, reset some data 
function reloginScene()
	UserEquipManager:reset();
	ChatList_Reset()
	ChatManager_reset()
	--ChatManager_little_reset()
	require("BattleFightHelper")
	BattleFightHelper_reloginPlayerUnchange()
end
--
function theOnLogOutConfirm(flag)
	if flag then
		CCLuaLog("ReEnter")
		--UserEquipManager:setUninited();
		--zhengxin 2014-08-10
		GamePrecedure:getInstance():reEnterLoading();
		--
	end
end
--安卓机，返回键调用此模式对话框

function askLogoutFromMainFrameToLoadingFrame()
	local title = Language:getInstance():getString("@LogOffTitle")
	local message = Language:getInstance():getString("@LogOffMSG")
	--
	PageManager.showConfirm(title,message,theOnLogOutConfirm)
end
--

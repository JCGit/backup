----------------------------------------------------------------------------------
--[[
	FILE:			PlayerInfoPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	
	AUTHOR:			hgs
	CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------

local thisPageName = "PlayerInfoPage";
local player_pb = require("Player_pb");
local titleManager = require("TitleManager")
registerScriptPage("SignaturePage");
local UserInfo = require("UserInfo");
local opcodes = {};

local option = {
	ccbiFile = "PersonalConfidencePopUp.ccbi",
	handlerMap = {
		onClose				= "onClose",
		onCancellation		= "onLogOff",
		onChangeSignature	= "onChangeSignature",
		onVIPDetailed		= "onShowVipDetail",
		onHelp				= "onHelp",
		onOpinion			= "onFeedback",
		onChangeTitle 		= "onChangeTitle",
		onBattleSetUp       = "onBattleSetUp"
	},
	opcode = opcodes
};

local PlayerInfoPageBase = {};

local NodeHelper = require("NodeHelper");	
-----------------------------------------------
--PlayerInfoPageBase脪鲁脙忙脰脨碌脛脢脗录镁麓娄脌铆
----------------------------------------------
function PlayerInfoPageBase:onEnter(container)
	--self:registerPacket(container)
	container:registerMessage(MSG_MAINFRAME_REFRESH)
	--container:registerMessage(MSG_SEVERINFO_UPDATE)
	local platformName = libPlatformManager:getPlatform():getClientChannel()
	if  platformName~= nil and (platformName == "PP" or platformName == "KY") then -- PP脝陆脤篓脨猫脪陋碌梅脫脙脝盲脫脙禄搂脰脨脨脛 
		NodeHelper:setStringForLabel(container, {mCancellation = common:getLanguageString("@UserCenter")}) 
	else
		NodeHelper:setStringForLabel(container, {mCancellation = common:getLanguageString("@LogOffTitle")})
	end
	self:refreshPage(container);
end

function PlayerInfoPageBase:onExecute(container)
end

function PlayerInfoPageBase:onExit(container)
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	--self:removePacket(container)
	--container:removeMessage(MSG_SEVERINFO_UPDATE)	
end
----------------------------------------------------------------

function PlayerInfoPageBase:refreshPage(container)
	self:showPlayerInfo(container);
end

function PlayerInfoPageBase:showPlayerInfo(container)
	UserInfo.sync();
	
	local roleExpCfg = ConfigManager.getRoleLevelExpCfg();
	local nextLevel = math.min(UserInfo.roleInfo.level, GameConfig.LevelLimit.PlayerLevel);
	local nextLevelExp = roleExpCfg[nextLevel]["exp"];
	local vipTexture = self:getVIPTexture(UserInfo.playerInfo.vipLevel);
	local RoleManager = require("RoleManager");
	local lb2Str = {
		mName				 	= UserInfo.roleInfo.name,
		mID 					= UserInfo.playerInfo.playerId,
		mOccupation				= RoleManager:getOccupationById(UserInfo.roleInfo.itemId),
		mLevel					= common:getLanguageString("@LevelInfo", UserInfo.roleInfo.level),
		mExp 					= UserInfo.roleInfo.exp .. "/" .. nextLevelExp,
		mVipLevelNum			= UserInfo.playerInfo.vipLevel,
		mMySignature		 	= UserInfo.playerInfo.signature,
		--mVIP					= UserInfo.getVipString()
	};
	NodeHelper:setStringForLabel(container, lb2Str);

	NodeHelper:setStringForTTFLabel(container, {mName = UserInfo.roleInfo.name});
	-- 称号
	local titleNode = container:getVarLabelTTF("mName")
	local fontSize = VaribleManager:getInstance():getSetting("myInfoTittleSize")
	local ttfLab = titleManager:setBMFontLabelTittle(titleNode,titleManager.myNowTitleId,fontSize)
	if ttfLab ~= nil then
		ttfLab:setAnchorPoint(ccp(0,0.5))
	end
	NodeHelper:setNodeVisible(container:getVarSprite("mChangeTitlePoint"),NoticePointState.TITLE_CHANGE_MSG)

	NodeHelper:setSpriteImage(container, {mVIP = vipTexture});

	local icon = RoleManager:getIconById(UserInfo.roleInfo.itemId);
	NodeHelper:setSpriteImage(container, {mPic = icon}, {mPic = 1.0});
end

function PlayerInfoPageBase:getVIPTexture( vipLevel )
	return "UI/MainScene/Font/u_vip"..vipLevel..".png";
end

----------------click event------------------------
function PlayerInfoPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end


local function onLogOffConfirm(flag)
	if flag then	
		--脰脴碌脟脗陆 reenter拢卢脳垄脪芒脭脷main.lua脌茂脫脨赂枚theOnLogOutConfirm潞炉脢媒拢卢脫毛卤戮潞炉脢媒脫娄赂脙卤拢鲁脰脪禄脰脗
		CCLuaLog("ReEnter")
		--UserEquipManager:setUninited();
		--zhengxin 2014-08-10
		GamePrecedure:getInstance():reEnterLoading();
		--
	end
end

function PlayerInfoPageBase:onLogOff(container)
	
	local platformName = libPlatformManager:getPlatform():getClientChannel()
	if  platformName~= nil and (platformName == "PP" or platformName == "KY") then -- PP脝陆脤篓脨猫脪陋碌梅脫脙脝盲脫脙禄搂脰脨脨脛 
		libPlatformManager:getPlatform():switchUsers()
	else
		local title = Language:getInstance():getString("@LogOffTitle")
		local message = Language:getInstance():getString("@LogOffMSG")	
	
		PageManager.showConfirm(title,message, onLogOffConfirm)
	end
end

function PlayerInfoPageBase:onChangeSignature(container)
	PageManager.pushPage("SignaturePage");
end	

function PlayerInfoPageBase:onShowVipDetail(container)
	--MessageBoxPage:Msg_Box('@CommingSoon')
	RechargePage_showPage(false);
end

function PlayerInfoPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.PERSONAL_INFO)
end

function PlayerInfoPageBase:onFeedback( container )
      PageManager.showHelp("FreedBack", common:getLanguageString("@FreedBack"))
end

function PlayerInfoPageBase:onChangeTitle( container )
	PageManager.pushPage("PlayerTitlePage")
	--取消红点
	NoticePointState.TITLE_CHANGE_MSG = false
	NoticePointState.isChange = true
	NodeHelper:setNodeVisible(container:getVarSprite("mChangeTitlePoint"),NoticePointState.TITLE_CHANGE_MSG)
end

function PlayerInfoPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
			self:refreshPage(container);
		end
	end
end

function PlayerInfoPageBase.onBattleSetUp( container )
    PageManager.pushPage("BattleSettingPage")
end

function PlayerInfoPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function PlayerInfoPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local PlayerInfoPage = CommonPage.newSub(PlayerInfoPageBase, thisPageName, option);

function MainFrame_onPlayerInfoPageBtn()
	PageManager.pushPage("PlayerInfoPage");
	return 1;
end

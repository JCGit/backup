----------------------------------------------------------------------------------
--[[
FILE:			BattleSettingPage.lua
ENCODING:		UTF-8, no-bomb
DESCRIPTION:	能量之核: 探索、拼合、列表页面
AUTHOR:			hgs
CREATED:		2014-01-21
--]]
----------------------------------------------------------------------------------

local Player_pb = require "Player_pb"
local HP_pb = require "HP_pb"
local bit = require "bit"
local UserInfo = require("UserInfo");

local thisPageName = "BattleSettingPage"

local BattleSettingPageConfg = {}
local option = {
	ccbiFile = "BattleSetUpPopUp.ccbi",
	handlerMap = {
		onCancelingSaving 		= "onCancelingSaving",
		onShowArea				= "onShowArea",
		onMusicChoice			= "onMusicChoice",
		onSoundChoice 			= "onSoundChoice",
		onCloseChatChoice 		= "onCloseChatChoice",
		onWhiteChoice 			= "onWhiteChoice",
		onGreenChoice 			= "onGreenChoice",
		onBlueChoice 			= "onBlueChoice",
		onPurpleChoice 			= "onPurpleChoice",
		onNonOccupationChoice 	= "onNonOccupationChoice",
		onSaveSettings 			= "onSaveSettings"
	},
	opcode = opcodes
};


local BattleSettingPageBase = {}

local NodeHelper = require("NodeHelper");

local flagList = {
}

local autoSellStatus = 0
local chatClose = 0
local musicOn = 1
local soundOn = 0
local showArea = 0;


-----------------------------------------------
--BattleSettingPageBase页面中的事件处理
----------------------------------------------
function BattleSettingPageBase:onEnter(container)
	
	self:refreshPage(container);
end

function BattleSettingPageBase:onExecute(container)
end

function BattleSettingPageBase:onExit(container)
end
----------------------------------------------------------------

function BattleSettingPageBase:refreshPage(container)
	--[[	local noticeStr = common:getLanguageString("@MailNotice", TodoStr);
	NodeHelper:setStringForLabel(container, {mMailPromptTex = noticeStr});--]]
	--UserInfo.syncStateInfo();
	autoSellStatus = UserInfo.stateInfo.autoSellEquip
	chatClose = UserInfo.stateInfo.chatClose
	musicOn = UserInfo.stateInfo.musicOn
	soundOn = UserInfo.stateInfo.soundOn
	showArea = UserInfo.stateInfo.showArea;
	flagList[1] = false;
	flagList[2] = false;
	flagList[3] = false;
	flagList[4] = false;
	flagList[5] = false;
	for i=1,5 do
		flagList[i] = self:checkAutoSellStatus(i)
	end
	
	self:onRefreshPage(container)
end


--
function BattleSettingPageBase:onRefreshPage(container)
	for i=1,5 do
		local markStr = "mMarkSprite"..i
		local markSprite = container:getVarSprite(markStr)
		local flag = flagList[i]	
--[[		local visibleFlag= false
		if flag >0 then
			visibleFlag = true
		end--]]
		markSprite:setVisible(flag)
	end
	
	local chatSprite = container:getVarSprite("mMarkSprite6")
	if chatClose>0 then
		chatSprite:setVisible(true)
	else
		chatSprite:setVisible(false)
	end

	local musicSprite = container:getVarSprite("mMusicSprite7")
	if musicSprite ~= nil then
		musicSprite:setVisible(tonumber(musicOn)==0)
	end
	local soundSpite = container:getVarSprite("mSoundSprite8")
	if soundSpite ~= nil then
		soundSpite:setVisible(tonumber(soundOn)==0)
	end
	NodeHelper:setNodesVisible(container, {mAreaSprite9 = (tonumber(showArea) == 0)});
end

function BattleSettingPageBase:boolToint(flag)
	if flag then
		return 1
	else
		return 0
	end
end

function BattleSettingPageBase:assemblyAutoSellStatus()
	local finalFlag = 0
	for i=1,5 do
		local curFlag = bit:blshift(self:boolToint(flagList[i]),i)
		finalFlag = finalFlag + curFlag;
	end
	CCLuaLog("assembly result is "..finalFlag)
	return finalFlag
end


function BattleSettingPageBase:checkAutoSellStatus(index)
	local midFlag = bit:blshift(1,index)
	local flag = bit:band(autoSellStatus,midFlag)
	if flag>0 then
		return true
	else
		return false
	end
end


----------------click event------------------------
function BattleSettingPageBase:onCancelingSaving(container)
	PageManager.popPage(thisPageName);
end

function BattleSettingPageBase:onCloseChatChoice(container)
	if chatClose == 1  then
		chatClose =0
	else
		chatClose =1
	end
	self:onRefreshPage(container)
end

function BattleSettingPageBase:onShowArea(container)
	if showArea == 1 then
		showArea = 0;
	else
		showArea = 1;
	end
	self:onRefreshPage(container);
end

function BattleSettingPageBase:onMusicChoice(container)
	if musicOn == 1  then
		musicOn =0
	else
		musicOn =1
	end
	self:onRefreshPage(container)
end


function BattleSettingPageBase:onSoundChoice(container)
	if soundOn == 1  then
		soundOn =0
	else
		soundOn =1
	end
	self:onRefreshPage(container)
end

function BattleSettingPageBase:onWhiteChoice(container)
	if flagList[1] == true  then
		flagList[1] =false
	else
		flagList[1] =true
	end
	self:onRefreshPage(container)
end

function BattleSettingPageBase:onGreenChoice(container)
	if flagList[2] == true  then
		flagList[2] =false
	else
		flagList[2] =true
	end
	self:onRefreshPage(container)
end

function BattleSettingPageBase:onBlueChoice(container)
	if flagList[3] == true  then
		flagList[3] =false
	else
		flagList[3] =true
	end
	self:onRefreshPage(container)
end

function BattleSettingPageBase:onPurpleChoice(container)
	if flagList[4] == true  then
		flagList[4] =false
	else
		flagList[4] =true
	end
	self:onRefreshPage(container)
end

function BattleSettingPageBase:onNonOccupationChoice(container)
	if flagList[5] == true  then
		flagList[5] =false
	else
		flagList[5] =true
	end
	self:onRefreshPage(container)
end

function BattleSettingPageBase:onSaveSettings(container)
	local status = self:assemblyAutoSellStatus()
	local message = Player_pb.HPSysSetting();
	
	if message~=nil then	
		message.autoSellEquip = status;
		message.chatClose	  = chatClose;
		message.musicOn 	  = musicOn;
		message.soundOn 	  = soundOn;
		message.showArea	  = showArea;
		UserInfo.stateInfo.autoSellEquip = status;
		UserInfo.stateInfo.chatClose = chatClose;
		UserInfo.stateInfo.musicOn = musicOn;
		UserInfo.stateInfo.soundOn = soundOn;
		UserInfo.stateInfo.showArea	= showArea;
		SoundManager:getInstance():setMusicOn(tonumber(UserInfo.stateInfo.musicOn)==1)
		SoundManager:getInstance():setEffectOn(tonumber(UserInfo.stateInfo.soundOn)==1)
		local pb_data = message:SerializeToString();
		PacketManager:getInstance():sendPakcet(HP_pb.SYS_SETTING_C,pb_data,#pb_data,false);		
	end
	
	PageManager.popPage(thisPageName)
	
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
BattleSettingPage = CommonPage.newSub(BattleSettingPageBase, thisPageName, option);
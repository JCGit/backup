----------------------------------------------------------------------------------
--[[
	FILE:			MailBattlePage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	邮件轮列表页面
	AUTHOR:			zz
	CREATED:		2014-08-04
--]]
----------------------------------------------------------------------------------
--require "ExploreEnergyCore_pb"
registerScriptPage("MailBattleSubPage");

local thisPageName = "MailBattlePage"
local HP = require("HP_pb");
local Mail_pb = require("Mail_pb");
local NodeHelper = require("NodeHelper");
local ResManagerForLua = require("ResManagerForLua");
local Const = require('Const_pb');

local MailDataHelper = require("MailDataHelper")

------------local variable for system global api--------------------------------------
local tostring = tostring;
local string = string;
local pairs = pairs;
--------------------------------------------------------------------------------

local MailBattleBase = {};

--requestId = 0;

local MailBattleItem = 
{
	ccbiFile = "RegimentWarResultMailContent.ccbi",
};

local opcodes = 
{
	MAIL_BATTLE_C = HP.MAIL_BATTLE_C,
	MAIL_BATTLE_S = HP.MAIL_BATTLE_S
}


local option = {
	ccbiFile = "RegimentWarResultMailPopUp.ccbi",
	handlerMap = {
		onConfirmation		= "onClose"
	},
	opcode = opcodes
};

--轮数信息总列表
local BattleList = {rounds = {}};

--------------------------------------------------------------


function MailBattleItem.onFunction(eventName, container)
	if eventName == "luaRefreshItemView" then
		MailBattleItem.onRefreshItemView(container);
	elseif eventName == "mDekaron" then
		MailBattleItem.showVSPage(container)
	end
end


--刷新content信息
function MailBattleItem.onRefreshItemView(container)
	local contentId = container:getItemDate().mID;

	local roundInfo = BattleList.rounds[contentId];

	if roundInfo ~= nil then

		local winStr = "";

		if roundInfo.isWin then
			winStr = common:getLanguageString("@Win");
		else
			winStr = common:getLanguageString("@Failed");
		end

		local roundCount = common:getLanguageString("@BattleRound",roundInfo.roundId);
		local leftName = "[ "..roundInfo.leftTeamName.. " ]";
		local rightName = "[ "..roundInfo.rightTeamName.. " ]";
		if string.len(rightName) <=0 then 
			rightName = common:getLanguageString("@BattleEmpty");
			NodeHelper:setNodesVisible(container, {mButtonNode = false});
		else
			NodeHelper:setNodesVisible(container, {mButtonNode = true});
		end
		
        leftName = GameMaths:stringAutoReturnForLua( leftName , GameConfig.Default.MailBattleTeamNameCount , 0 )    
		
        local teamStr = 
		{
			mRound = roundCount,
			mWinOrLose = winStr,
			mAlly = leftName,
			mEnemy = rightName
		}

		NodeHelper:setStringForLabel(container, teamStr);

		local colorMap = {};

		if roundInfo.isWin then
			colorMap = {
				mAlly = GameConfig.ColorMap.COLOR_GREEN,
				mEnemy = GameConfig.ColorMap.COLOR_RED,
				mWinOrLose = GameConfig.ColorMap.COLOR_GREEN
			}
		else
			colorMap = {
				mAlly = GameConfig.ColorMap.COLOR_RED,
				mEnemy = GameConfig.ColorMap.COLOR_GREEN,
				mWinOrLose = GameConfig.ColorMap.COLOR_RED
			}
		end

		NodeHelper:setColorForLabel(container, colorMap);
end





end

--调到指定轮具体对战信息页面：MailBattleSubPage
function MailBattleItem.showVSPage( container )
	local contentId = container:getItemDate().mID;

	local roundInfo = BattleList.rounds[contentId];

	local teamList = 
	{
		roundId = roundInfo.roundId,
		isWin = roundInfo.isWin,
		leftTeamName = roundInfo.leftTeamName,
		rightTeamName = roundInfo.rightTeamName
	}

	MailBattleSubPage_setDataList( roundInfo.vsInfo, teamList);

	PageManager.pushPage("MailBattleSubPage");
end


----------------------------------------------

function MailBattleBase:onEnter(container)
	NodeHelper:initScrollView(container, "mContent", 3);
	self:registerPacket(container)
	--container:registerMessage(MSG_SEVERINFO_UPDATE)
	self:sendMsgForBattleList(container);
	--self:refreshPage(container);
	self:rebuildAllItem(container);
end

function MailBattleBase:onClose(container)
	PageManager.popPage(thisPageName);
end

function MailBattleBase:onExecute(container)
end

function MailBattleBase:onExit(container)
	self:removePacket(container)
	--container:removeMessage(MSG_SEVERINFO_UPDATE)	
	NodeHelper:deleteScrollView(container);
end
---------------------------------------------------------------

----------------scrollview-------------------------
function MailBattleBase:rebuildAllItem(container)
	self:clearAllItem(container);
	self:buildItem(container);
end

function MailBattleBase:clearAllItem(container)
	NodeHelper:clearScrollView(container);
end

function MailBattleBase:buildItem(container)

	local contentsSize = table.maxn(BattleList.rounds);
	NodeHelper:buildScrollView(container, contentsSize, MailBattleItem.ccbiFile, MailBattleItem.onFunction);
end
-------------------------------------------------------------------------
function MailBattleBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function MailBattleBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode);
		end
	end
end

--请求论述列表信息
function MailBattleBase:sendMsgForBattleList( container)
	local msg = Mail_pb.OPMailViewTeam();
	msg.teamId = MailDataHelper:getVariableByKey("requestId")

	local pb_data = msg:SerializeToString();
	container:sendPakcet(opcodes.MAIL_BATTLE_C, pb_data, #pb_data, true);
end

function MailBattleBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == opcodes.MAIL_BATTLE_S then
		local msg = Mail_pb.OPMailViewTeamRet()
		msg:ParseFromString(msgBuff)
		self:onReceiveBattleList(container, msg)
		return
	end
end

--缓存轮数列表
function MailBattleBase:onReceiveBattleList( container, msg )
	BattleList = {};

	BattleList.rounds = {};

	local roundsSize = table.maxn(msg.rounds);

	for i = 1,roundsSize,1 do
		BattleList.rounds[i] = {};
		BattleList.rounds[i].roundId = msg.rounds[i].roundId;
		BattleList.rounds[i].isWin = msg.rounds[i].isWin;
		BattleList.rounds[i].leftTeamName = msg.rounds[i].leftTeamName;
		BattleList.rounds[i].rightTeamName = msg.rounds[i].rightTeamName;

		local vsInfoSize = table.maxn(msg.rounds[i].vsInfo);
		local vsInfo = msg.rounds[i].vsInfo;

		BattleList.rounds[i].vsInfo = {};
		for j=1,vsInfoSize,1 do
			BattleList.rounds[i].vsInfo[j] = {};
			BattleList.rounds[i].vsInfo[j].leftPlayer = {};
			BattleList.rounds[i].vsInfo[j].leftPlayer.playId = vsInfo[j].leftPlayer.playId;
			BattleList.rounds[i].vsInfo[j].leftPlayer.name = vsInfo[j].leftPlayer.name;
			BattleList.rounds[i].vsInfo[j].leftPlayer.killCount = vsInfo[j].leftPlayer.killCount;
			BattleList.rounds[i].vsInfo[j].leftPlayer.record = vsInfo[j].leftPlayer.record;

			BattleList.rounds[i].vsInfo[j].rightPlayer = {};
			BattleList.rounds[i].vsInfo[j].rightPlayer.playId = vsInfo[j].rightPlayer.playId;
			BattleList.rounds[i].vsInfo[j].rightPlayer.name = vsInfo[j].rightPlayer.name;
			BattleList.rounds[i].vsInfo[j].rightPlayer.killCount = vsInfo[j].rightPlayer.killCount;
			BattleList.rounds[i].vsInfo[j].rightPlayer.record = vsInfo[j].rightPlayer.record;
		end

	end

	table.sort(BattleList.rounds, 
	function ( e1, e2 )
		if not e2 then return ture end
		if not e1 then return false end
		return e1.roundId + 0 < e2.roundId + 0
	end
		)

	self:rebuildAllItem(container);
end


-------------------------------------------------------------------------


local CommonPage = require("CommonPage");
local MailBattlePage = CommonPage.newSub(MailBattleBase, thisPageName, option);


--[[
function MailBattleBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_SEVERINFO_UPDATE then
		local opcode = MsgSeverInfoUpdate:getTrueType(message).opcode
		if opcode == OPCODE_GET_USER_BASIC_INFORET_S then
			self:refreshTitle(container)
		elseif opcode == OPCODE_REMOVE_USER_ENERGYCORE_RET_S or opcode == OPCODE_USER_ENERGYCORE_INFO_RET_S then
			pageInfo.needRefresh = true
		end
	elseif typeId == MSG_MAINFRAME_POPPAGE then
		local pageName = MsgMainFramePopPage:getTrueType(message).pageName
		if pageName == "EnergyCoreUpgradePage" and pageInfo.needRefresh then
			self:rebuildAllItem(container)
		end
	elseif typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName
		if pageName == thisPageName then
			local pageType = common:getBlackBoardVariable("ToMarketPageType", true)
			self:switchPageType(container, tonumber(pageType))
		end
	end
end
--]]

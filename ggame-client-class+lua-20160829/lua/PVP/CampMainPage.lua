----------------------------------------------------------------------------------
--[[
	FILE:			CampMainPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	阵营站主页面
	AUTHOR:			zhenhui
	CREATED:		2014-09-18
--]]
----------------------------------------------------------------------------------


local thisPageName = "CampMainPage";
local HP = require("HP_pb");
local CampWar_pb = require("CampWar_pb");
local UserInfo = require("UserInfo");

local opcodes = {
	CAMPWAR_STATE_SYNC_S = HP.CAMPWAR_STATE_SYNC_S,
	ROLE_CARRY_SKILL_S = HP.ROLE_CARRY_SKILL_S,
	ENTER_BATTLEFIELD_S = HP.ENTER_BATTLEFIELD_S
};	

local option = {
	ccbiFile = "CampWarPage.ccbi",
	handlerMap = {
		onEnterFightBtn = "onEnterFightBtn",
		onRanking = "onRanking",
		onHelp = "onHelp",
		onSkillHand = "onSkillHand",
		onAutomatic = "onAutomatic"		
	},
	opcode = opcodes
};

local CampMainPageBase = {};

local NodeHelper = require("NodeHelper");

local CampWarManager = require("CampWarManager")

--离开启剩余多少时间
local closeLeftTimeKey = 'CampLeftCDForClose'
--离关闭剩余多少时间
local openLeftTimeKey = 'CampLeftCDForOpen'

local TimerFunc = {};

-----------------------------------------------

function CampMainPageBase:onEnter(container)
	NodeHelper:setNodesVisible(container,{mAutomaticNode = false});
	self:registerPacket(container);	
	container:registerMessage(MSG_MAINFRAME_REFRESH)				
	--self:refreshPage(container);
	CampMainPageBase:requestServerInfo(container)
end	

function CampMainPageBase:requestServerInfo(container)
	common:sendEmptyPacket(HP.GET_CAMPWAR_STATE_C);
end

function CampMainPageBase:onExecute(container)
	TimerFunc.updateCD(container)	
end

function CampMainPageBase:onHelp(container)
	PageManager.showHelp(GameConfig.HelpKey.HELP_CAMP)
end

--准备状态倒计时
function TimerFunc.updateCD( container )
	--时间刷新
	local cdString = '00:00:00'
	if CampWarManager.state == CampWar_pb.CAMP_WAR_CLOSED   or CampWarManager.state == CampWar_pb.CAMP_WAR_PREPARE then
		if TimeCalculator:getInstance():hasKey(closeLeftTimeKey) then
			local timeleft = TimeCalculator:getInstance():getTimeLeft(closeLeftTimeKey)
			if timeleft >= 0 then
				 cdString = GameMaths:formatSecondsToTime(timeleft)
			else
				CCLuaLog("TimerFunc.updateCD -- timeleft <0 ");
			end			
		end
	elseif CampWarManager.state == CampWar_pb.CAMP_WAR_FIGHT  then 
		if TimeCalculator:getInstance():hasKey(openLeftTimeKey) then
			local timeleft = TimeCalculator:getInstance():getTimeLeft(openLeftTimeKey)
			if timeleft >= 0 then
				 cdString = GameMaths:formatSecondsToTime(timeleft)
			else
				CCLuaLog("TimerFunc.updateCD -- timeleft <0 ");
			end			
		end
	end
	NodeHelper:setStringForLabel(container, { mCD = cdString})
--[[	
	if CampWarManager.state == CampWar_pb.CAMP_WAR_PREPARE then
		NodeHelper:setColorForLabel( container, {mCD = GameConfig.ColorMap.COLOR_RED} )
	elseif CampWarManager.state == CampWar_pb.CAMP_WAR_FIGHT  then 
		NodeHelper:setColorForLabel( container, {mCD = GameConfig.ColorMap.COLOR_BLUE} )
	else
		NodeHelper:setColorForLabel( container, {mCD = GameConfig.ColorMap.COLOR_RED} )
	end--]]
	
	
end

function CampMainPageBase:removeTimer(container)
	TimeCalculator:getInstance():removeTimeCalcultor(closeLeftTimeKey);
	TimeCalculator:getInstance():removeTimeCalcultor(openLeftTimeKey);
end

function CampMainPageBase:onExit(container)
	self:removePacket(container)	
	container:removeMessage(MSG_MAINFRAME_REFRESH)
	CampMainPageBase:removeTimer(container)
end
----------------------------------------------------------------

function CampMainPageBase:refreshSkillPage(container)
	local SkillManager = require("SkillManager")
	local skillList = {}
	skillList  = SkillManager:getArenaSkillList()
	local arenaSkillSize = #skillList
	local sprite2Img = {};
	for i=1,4 do
		local skStr = "mSkillPic0"..tostring(i)
		if i<=arenaSkillSize then		
			local skillId = skillList[i]
			if skillId == 0 then 
				sprite2Img[skStr] = GameConfig.Image.ClickToSelect
			else 
				local skillItemInfo = SkillManager:getSkillInfoBySkillId( skillId )
				sprite2Img[skStr] = skillItemInfo["icon"]
			end
			
		else
			sprite2Img[skStr] = GameConfig.Image.SkillDisable;
		end			
	end
	NodeHelper:setSpriteImage(container, sprite2Img);
end

function CampMainPageBase:onSkillHand(container)
	ReplaceSkillPage_setBagId(2)
	PageManager.pushPage("ReplaceSkillPage")
end

function CampMainPageBase:refreshPage(container)
	
	CampMainPageBase:refreshSkillPage(container)
	local enterGameBtn = container:getVarMenuItemImage("mEnterFightBtn")
	 
	if enterGameBtn==nil then
		CCLuaLog("error in enter game btn") 		
		return 
	end
	--创建Timer
	
	if CampWarManager.state == CampWar_pb.CAMP_WAR_CLOSED or CampWarManager.state == CampWar_pb.CAMP_WAR_PREPARE then
		local labelStr = common:getLanguageString("@BeforeCampFightStart")
		NodeHelper:setStringForLabel(container,{mBeforeOrStartLabel  = labelStr})
		if CampWarManager.leftTime > 0 and not TimeCalculator:getInstance():hasKey(closeLeftTimeKey) then
			TimeCalculator:getInstance():removeTimeCalcultor(openLeftTimeKey);
			TimeCalculator:getInstance():createTimeCalcultor(closeLeftTimeKey, CampWarManager.leftTime);						
		end
	elseif CampWarManager.state == CampWar_pb.CAMP_WAR_FIGHT  then 
		local labelStr = common:getLanguageString("@BeforeCampFightEnd")
		NodeHelper:setStringForLabel(container,{mBeforeOrStartLabel  = labelStr})
		if CampWarManager.leftTime > 0 and not TimeCalculator:getInstance():hasKey(openLeftTimeKey) then
			TimeCalculator:getInstance():removeTimeCalcultor(closeLeftTimeKey);
			TimeCalculator:getInstance():createTimeCalcultor(openLeftTimeKey, CampWarManager.leftTime);			
		end	
	end
	
	local vipLevel = UserInfo.playerInfo.vipLevel;
	if vipLevel<GameConfig.CampAutoRegisterVip then
		NodeHelper:setNodesVisible(container,{mAutomaticNode = false})
	else
		NodeHelper:setNodesVisible(container,{mAutomaticNode = true})
	end

	--根据是否开启Auto，显示或隐藏节点
	if CampWarManager.isAuto == 1 then
		if CampWarManager.state == CampWar_pb.CAMP_WAR_FIGHT then
			NodeHelper:setNodesVisible(container,
			{
			mChoice02 = true,
			mSkillNode = false,
			mAutoCloseNode = true,
			mAutoOpenNode = false
			})
		else
			NodeHelper:setNodesVisible(container,
			{
			mChoice02 = true,
			mSkillNode = false,
			mAutoCloseNode = false,
			mAutoOpenNode = true
			})
		end
		if enterGameBtn~=nil then
			enterGameBtn:setEnabled(false)
		end			
		NodeHelper:setNodesVisible(container,{	mPersonalChoice = true})
	else
		NodeHelper:setNodesVisible(container,
		{
		mChoice02 = false,
		mSkillNode = true,
		mAutoCloseNode = false,
		mAutoOpenNode = false
		})
		
		if CampWarManager.state == CampWar_pb.CAMP_WAR_CLOSED  then
			if enterGameBtn~=nil then
				enterGameBtn:setEnabled(false)
			end				
		else
			if enterGameBtn~=nil then
				enterGameBtn:setEnabled(true)
			end				
		end			
		NodeHelper:setNodesVisible(container,{	mPersonalChoice = false})
	end				
end


function CampMainPageBase:onEnterFightBtn(container)
	--发送加入战斗协议，同时跳转到主战斗页面		
	common:sendEmptyPacket(HP.ENTER_BATTLEFIELD_C,false)		
end

function CampMainPageBase:onAutomatic(container)
	if CampWarManager.state ~= CampWar_pb.CAMP_WAR_FIGHT then
		--勾选自动选择，判断当前是否自动状态，	
		common:sendEmptyPacket(HP.UPDATE_AUTO_CAMPWAR_STATE_C,false)	
	end
	
end

function CampMainPageBase:onRanking(container)
	local CampWarRankSize = #CampWarManager.m_lastCampWarInfo.rankInfo;
	if CampWarRankSize>0 and  CampWarManager.m_lastCampWarInfo~=nil and CampWarManager.m_lastCampWarInfo.winCampId ~=nil then
		PageManager.pushPage("CampWarRankPage")
	else
		MessageBoxPage:Msg_Box_Lan("@CampHasNoLastRank")
	end
	
end

function CampMainPageBase:onHelp( container )
	PageManager.showHelp(GameConfig.HelpKey.HELP_CAMP)
end	

function CampMainPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode()
	local msgBuff = container:getRecPacketBuffer()

	if opcode == HP.ROLE_CARRY_SKILL_S then
		--先删除原有timer，重新创建
		self:removeTimer(container)
		return self:refreshPage(container)		
	end		
	if opcode == HP.ENTER_BATTLEFIELD_S then
		PageManager.changePage("CampFightPage")
	end
end

function CampMainPageBase:onReceiveMessage(container)
	local message = container:getMessage()
	local typeId = message:getTypeId()
	if typeId == MSG_MAINFRAME_REFRESH then
		local pageName = MsgMainFrameRefreshPage:getTrueType(message).pageName;
		if pageName == thisPageName then
		    local extraParam = MsgMainFrameRefreshPage:getTrueType(message).extraParam;
		    if extraParam=="SyncState" then
		        self:removeTimer(container)
		        return self:refreshPage(container)	
		    end
		
			self:refreshSkillPage(container)
		end
	end
end

function CampMainPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end	

function CampMainPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end

-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
local CampMainPage = CommonPage.newSub(CampMainPageBase, thisPageName, option);
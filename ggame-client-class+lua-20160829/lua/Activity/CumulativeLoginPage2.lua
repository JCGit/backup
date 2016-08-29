----------------------------------------------------------------------------------
--[[
	FILE:			CumulativeLoginPage2.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	累计登陆活动界面
	AUTHOR:			zhoutong
	CREATED:		2014-11-05
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local NodeHelper = require("NodeHelper")

local CumulativeLoginDataHelper = require("CumulativeLoginDataHelper2")
local ActivityBasePage = require("ActivityBasePage")

local thisPageName = "CumulativeLoginPage2"
local opcodes = {
	ACC_LOGIN_INFO_C = HP_pb.ACC_LOGIN2_INFO_C;
	ACC_LOGIN_INFO_S = HP_pb.ACC_LOGIN2_INFO_S;
	ACC_LOGIN_AWARDS_C = HP_pb.ACC_LOGIN2_AWARDS_C;
	ACC_LOGIN_AWARDS_S = HP_pb.ACC_LOGIN2_AWARDS_S;
};
local option = {
	ccbiFile = "Act_CumulativeLandPage.ccbi",
	handlerMap = {
		onReturnButton	= "onBack",
		onHelp			= "onHelp",
		onRanking       = "onRanking",
		onContributionMaterials = "onContributionMaterials",
		onLeftActivityBtn          = "onLeft",
		onRightActivityBtn         = "onRight"
	},
	opcode = opcodes,
	DataHelper = CumulativeLoginDataHelper
};

----------------- local data -----------------
local CumulativeLoginPage = ActivityBasePage:new(option,thisPageName,nil,opcodes)
CumulativeLoginPage.timerName = "Activity_CumulativeLogin"
CumulativeLoginPage.timerLabel = "mActivityDaysNum"
-----------------------------------------------
--------------------------Content--------------
local CumulativeLoginContent = {
	ccbiFile = "Act_CumulativeLandContent.ccbi",
	rewardParams = {
	        mainNode = "mRewardNode",
	        countNode = "mNum",
            nameNode = "mName",
            frameNode = "mFrame",
            picNode = "mPic",
            startIndex = 1
	}
}

function CumulativeLoginContent.onFunction(eventName, container)
    if eventName == "luaRefreshItemView" then
		CumulativeLoginContent.onRefreshItemView(container);
	elseif eventName == "onRewardBtn" then
		CumulativeLoginContent.onReceiveReward(container);
    elseif eventName:sub(1, 7) == "onFrame" then
		CumulativeLoginContent.showItemInfo(container, eventName);
	end	
end

function CumulativeLoginContent.onRefreshItemView(container)
	local index = container:getItemDate().mID;
	local rewardInfo = CumulativeLoginPage.DataHelper:getConfigDataByKeyAndIndex("DailyRewardItem",index)

	if rewardInfo~=nil then
	    local nodeVisible = {}
	    local labelString = {}
        labelString.mContinuousLandingReward = common:getLanguageString("@CumulativeLandingReward",tostring(index))
        
        if CumulativeLoginPage.DataHelper:getVariableByKey("gotAwardCfgId") ~= nil then
            if common:table_hasValue(CumulativeLoginPage.DataHelper:getVariableByKey("gotAwardCfgId"), index) then        
                NodeHelper:setMenuItemEnabled(container,"mRewardBtn",false)
            else
                NodeHelper:setMenuItemEnabled(container,"mRewardBtn",true)
            end
        end
        
        local CumulativeLoginDays = CumulativeLoginPage.DataHelper:getVariableByKey("CumulativeLoginDays")
        if index > CumulativeLoginDays then
            NodeHelper:setMenuItemEnabled(container,"mRewardBtn",false)
        end
	    
        local cfg = ConfigManager.getRewardById(rewardInfo.r);
        NodeHelper:fillRewardItemWithParams(container, cfg, 4,CumulativeLoginContent.rewardParams)
        
        NodeHelper:setNodesVisible(container, nodeVisible);
        NodeHelper:setStringForLabel(container, labelString);
	end
end

function CumulativeLoginContent.showItemInfo(container,eventName)
    local index = container:getItemDate().mID;
	local rewardInfo = CumulativeLoginPage.DataHelper:getConfigDataByKeyAndIndex("DailyRewardItem",index)
	
	if rewardInfo~=nil then
	    local cfg = ConfigManager.getRewardById(rewardInfo.r); 
	    local rewardIndex = tonumber(eventName:sub(8));
	    if cfg[rewardIndex] ~= nil then
	        GameUtil:showTip(container:getVarNode('mFrame' .. rewardIndex), common:table_merge(cfg[rewardIndex],{buyTip=true,hideBuyNum=true}));
	    end
	end
end


function CumulativeLoginContent.onReceiveReward(container)
    local index = container:getItemDate().mID;
    if CumulativeLoginPage.DataHelper:getVariableByKey("gotAwardCfgId") ~= nil then
        if common:table_hasValue(CumulativeLoginPage.DataHelper:getVariableByKey("gotAwardCfgId"), index)  then
            MessageBoxPage:Msg_Box_Lan("@VipWelfareAlreadyReceive");
            return 
        end
    end
	local msg = Activity_pb.HPAccLoginAwards();
	msg.rewwardDay = index;
	common:sendPacket(opcodes.ACC_LOGIN_AWARDS_C, msg);
end
-----------------------end Content------------

---------------------------------------------------------------
function CumulativeLoginPage:getPageInfo(container)
    local msg = Activity_pb.HPAccLoginInfo();
	common:sendPacket(opcodes.ACC_LOGIN_INFO_C, msg);
	self:refreshPage(container)
end

function CumulativeLoginPage:refreshPage(container)
    CallSuperFunc(container,self,"refreshPage")
    --self.super.refreshPage(self,container)
	
	local bmLabels = {
	    mLoginDaysNum = string.format(" %02d",self.DataHelper:getVariableByKey("CumulativeLoginDays"))
    }
	local nodeVisible = {}
	local textureSet = {}
    local selectedMap = {}

    NodeHelper:setMenuItemSelected( container, selectedMap )
	NodeHelper:setNodesVisible(container, nodeVisible);
    NodeHelper:setStringForLabel(container, bmLabels);
	NodeHelper:setSpriteImage(container, textureSet);
	
	self:rebuildAllItem(container)
	self:setCurDayReward( container )
end

function CumulativeLoginPage:buildItem(container)
    NodeHelper:buildScrollView(container, #self.DataHelper:getConfigDataByKey("DailyRewardItem"), CumulativeLoginContent.ccbiFile, CumulativeLoginContent.onFunction)
end

function CumulativeLoginPage:setCurDayReward( container )
	local totalOffset = container.mScrollView:getContentOffset()
	local currentDay = self.DataHelper:getVariableByKey("mReceiveDays")
	if currentDay == 0 or currentDay == 1 then
	    return
	end
	
	local curY = totalOffset.y + container.mScrollView:getContentSize().height * ( currentDay - 1) / (#self.DataHelper:getConfigDataByKey("DailyRewardItem"))
	
	if curY > 0  then
		curY = 0
	end
	
	local curOffset = CCPointMake( totalOffset.x , curY ) 
	NodeHelper:setScrollViewStartOffset(container,curOffset)
end

----------------click event------------------------	

-------------------------------------------------------------------------

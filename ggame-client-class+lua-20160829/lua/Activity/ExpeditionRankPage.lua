----------------------------------------------------------------------------------
--[[
	FILE:			ExpeditionRankPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			zhoutong
	CREATED:		2014-10-15
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local UserInfo = require("UserInfo");
local NodeHelper = require("NodeHelper");
local thisPageName = "ExpeditionRankPage"

local opcodes = {
    --页面信息协议
	EXPEDITION_ARMORY_RANKING_C	    = HP_pb.EXPEDITION_ARMORY_RANKING_C,
	EXPEDITION_ARMORY_RANKING_S	    = HP_pb.EXPEDITION_ARMORY_RANKING_S,
};
local option = {
	ccbiFile = "Act_ExpeditionMaterialsCollectionRankPopUp.ccbi",
	handlerMap = {
		onClose	= "onBack",
		onHelp			= "onHelp",
		onNowRanking    = "onNowRanking",
		onRankingReward = "onRankingReward"
	},
	opcode = opcodes
};

--活动基本信息
local ExpeditionDataHelper = require("ExpeditionDataHelper")

local TabEnum = {
    RankingTab = 1,
    RewardTab = 2
}
local nowTab = TabEnum.RankingTab
----------------- local data -----------------
local ExpeditionRankPageBase = {}


local ExpeditionRankItem = {
	ccbiFile = "Act_ExpeditionMaterialsCollectionRankC.ccbi",
}

function ExpeditionRankItem.onFunction(eventName, container)
    if eventName == "luaRefreshItemView" then
		ExpeditionRankItem.onRefreshItemView(container);
	elseif eventName == "onSee" then
		ExpeditionRankItem.onSee(container)
	end	
end

function ExpeditionRankItem.onRefreshItemView(container)
    local bmLabel = {}
    local nodeVisible = {}
	local index = container:getItemDate().mID;
    
    local rankInfo = ExpeditionDataHelper.getRankInfoByIndex(index)
    if rankInfo.playerId == UserInfo.playerInfo.playerId then
        if rankInfo~=nil then
            bmLabel.Rank = tostring(rankInfo.rank)
            bmLabel.mName = tostring(rankInfo.name)
            bmLabel.mContribution = tostring(rankInfo.exp)
            nodeVisible.mScale9Sprite2 = false
            nodeVisible.mScale9Sprite3 = true
        end
    else
        if rankInfo~=nil then
            bmLabel.Rank = tostring(rankInfo.rank)
            bmLabel.mName = tostring(rankInfo.name)
            bmLabel.mContribution = tostring(rankInfo.exp)
            nodeVisible.mScale9Sprite2 = true
            nodeVisible.mScale9Sprite3 = false
        end
    end
	NodeHelper:setStringForLabel(container, bmLabel);
	NodeHelper:setNodesVisible(container, nodeVisible);
end

function ExpeditionRankItem.onSee(container)
    local index = container:getItemDate().mID;
    
    local rankInfo = ExpeditionDataHelper.getRankInfoByIndex(index)
    if rankInfo~=nil then
        PageManager.viewPlayerInfo(rankInfo.playerId, true);
    end

   
end

----------------------------------------------------------

local ExpeditionRewardItem = {
	ccbiFile = "Act_ExpeditionMaterialsCollectionRankRewardC.ccbi",
	rewardParams = {
	        mainNode = "mRewardNode",
	        countNode = "mNum",
            nameNode = "mName",
            frameNode = "mFeet0",
            picNode = "mRewardPic",
            startIndex = 1
	}
}

function ExpeditionRewardItem.onFunction(eventName, container)
    if eventName == "luaRefreshItemView" then
		ExpeditionRewardItem.onRefreshItemView(container);
	elseif eventName == "onReceive" then
		ExpeditionRewardItem.onReceiveReward(container);
    elseif eventName:sub(1, 6) == "onFeet" then
		ExpeditionRewardItem.showItemInfo(container, eventName);
	end	
end

function ExpeditionRewardItem.onRefreshItemView(container)
	local index = container:getItemDate().mID;
	local rewardInfo = ExpeditionDataHelper.getRankRewardInfobyIndex(index)
	local selfRank = ExpeditionDataHelper.getSelfRank()
	local rewardState = ExpeditionDataHelper.getRankRewardState()
	
	if rewardInfo~=nil then
	    local nodeVisible = {}
	    local labelString = {}
	    if rewardInfo.rankT == rewardInfo.rankB then
	        nodeVisible.mReceiveRanking = false
	        nodeVisible.mReceiveRanking01 = false
	        nodeVisible.mReceiveRanking02 = false
	        nodeVisible.mReceiveRanking03 = false
	        nodeVisible["mReceiveRanking0"..index] = true
	    else
	        labelString.mReceiveRanking = tostring(rewardInfo.rankT).."-"..tostring(rewardInfo.rankB)
	        nodeVisible.mReceiveRanking = true
	        nodeVisible.mReceiveRanking01 = false
	        nodeVisible.mReceiveRanking02 = false
	        nodeVisible.mReceiveRanking03 = false
	    end
	    if selfRank~=nil and selfRank<=rewardInfo.rankB and selfRank>=rewardInfo.rankT and rewardState==1 then
	        nodeVisible.mShieldMessageNode = true
	    else
	        nodeVisible.mShieldMessageNode = false
	    end
	    
        local cfg = ConfigManager.getRewardById(rewardInfo.r);
        NodeHelper:fillRewardItemWithParams(container, cfg, 3,ExpeditionRewardItem.rewardParams)
        
        NodeHelper:setNodesVisible(container, nodeVisible);
        NodeHelper:setStringForLabel(container, labelString);
	end
end

function ExpeditionRewardItem.showItemInfo(container,eventName)
    local index = container:getItemDate().mID;
	local rewardInfo = ExpeditionDataHelper.getRankRewardInfobyIndex(index)
	
	if rewardInfo~=nil then
	    local cfg = ConfigManager.getRewardById(rewardInfo.r); 
	    local rewardIndex = tonumber(eventName:sub(8));
	    if cfg[rewardIndex] ~= nil then
	        GameUtil:showTip(container:getVarNode('mFeet0' .. rewardIndex), common:table_merge(cfg[rewardIndex],{buyTip=true,hideBuyNum=true}));
	    end
	end
end


function ExpeditionRewardItem.onReceiveReward(container)
	--local msg = Activity_pb.HPExpeditionArmoryRankReward();
	--common:sendPacket(opcodes.EXPEDITION_ARMORY_RANK_REWARD_C, msg);
end
--------------------------------------------------
-----------------------------------------------
--ExpeditionRankPageBase页面中的事件处理
----------------------------------------------
function ExpeditionRankPageBase:onEnter(container)
	NodeHelper:initScrollView(container, "mContent", 6);
	self:registerPacket(container);
	self:getActivityInfo(container)
end

function ExpeditionRankPageBase:getActivityInfo(container)
    local msg = Activity_pb.HPExpeditionArmoryRanking();
	common:sendPacket(opcodes.EXPEDITION_ARMORY_RANKING_C, msg);
end


function ExpeditionRankPageBase:onExecute(container)

end

function ExpeditionRankPageBase:onExit(container)
    NodeHelper:deleteScrollView(container);
	self:removePacket(container);
end

function ExpeditionRankPageBase:refreshPage(container)
    local labelString = {}
    local selfTotal = ExpeditionDataHelper.getSelfContribution()
    if  selfTotal~=nil then
        labelString.mPersonalContributionNum = tostring(selfTotal)
    end
    NodeHelper:setStringForLabel(container, labelString);


    self:rebuildAllItem(container)
end
----------------scrollview-------------------------
function ExpeditionRankPageBase:rebuildAllItem(container)
    self:clearAllItem(container);
	self:buildItem(container);
end

function ExpeditionRankPageBase:clearAllItem(container)
    NodeHelper:clearScrollView(container);
end

function ExpeditionRankPageBase:buildItem(container)
    local nodeVisible = {}
    if nowTab == TabEnum.RankingTab then
        nodeVisible.mRankNameNode = true
        nodeVisible.mRankingRewardNode = false
        
        local rankInfo = ExpeditionDataHelper.getRankInfo()
        if rankInfo~=nil then
            local size = (#rankInfo)
            NodeHelper:buildScrollView(container,size, ExpeditionRankItem.ccbiFile, ExpeditionRankItem.onFunction);
        end
    else
        nodeVisible.mRankNameNode = false
        nodeVisible.mRankingRewardNode = true
        
        local rankRewardInfo = ExpeditionDataHelper.getRankRewardInfo()
        if rankRewardInfo~=nil then
            local size = #rankRewardInfo
            NodeHelper:buildScrollView(container, size, ExpeditionRewardItem.ccbiFile, ExpeditionRewardItem.onFunction);
        end
    end
    
    NodeHelper:setNodesVisible(container, nodeVisible);
end
	
----------------click event------------------------	
function ExpeditionRankPageBase:onNowRanking(container)
    nowTab = TabEnum.RankingTab
    self:refreshPage(container)
end	

function ExpeditionRankPageBase:onRankingReward(container)
    nowTab = TabEnum.RewardTab
    self:refreshPage(container)
end	

function ExpeditionRankPageBase:onBack()
    PageManager.popPage(thisPageName);    
end

--回包处理
function ExpeditionRankPageBase:onReceivePacket(container)
    local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
	if opcode == opcodes.EXPEDITION_ARMORY_RANKING_S then
	    ExpeditionDataHelper.onReceiveRankInfo(msgBuff)
		self:refreshPage(container);
	end
end

function ExpeditionRankPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function ExpeditionRankPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ExpeditionRankPage = CommonPage.newSub(ExpeditionRankPageBase, thisPageName, option);

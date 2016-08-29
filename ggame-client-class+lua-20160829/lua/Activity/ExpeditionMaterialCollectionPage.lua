----------------------------------------------------------------------------------
--[[
	FILE:			ExpeditionMaterialCollectionPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			zhoutong
	CREATED:		2014-10-15
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local NodeHelper = require("NodeHelper");
local thisPageName = "ExpeditionMaterialCollectionPage"
local opcodes = {
    --获取远征活动信息
	EXPEDITION_ARMORY_INFO_C 		= HP_pb.EXPEDITION_ARMORY_INFO_C,
	EXPEDITION_ARMORY_INFO_S		= HP_pb.EXPEDITION_ARMORY_INFO_S
	
};
local option = {
	ccbiFile = "Act_ExpeditionMaterialsCollectionPage.ccbi",
	handlerMap = {
		onReturnButton	= "onBack",
		onHelp			= "onHelp",
		onRanking       = "onRanking",
		onContributionMaterials = "onContributionMaterials",
		onLeftActivityBtn          = "onLeft",
		onRightActivityBtn         = "onRight"
	},
	opcode = opcodes
};

--活动基本信息，结构基本与协议pb Message相同
local ExpeditionDataHelper = require("ExpeditionDataHelper")
----------------- local data -----------------
local ExpeditionMaterialCollectionPageBase = {}
local CurrentStageId = 1
local RewardParams = {
    mainNode = "mGemNode",
    countNode = "mNum",
    nameNode = "mName",
    frameNode = "mFeet0",
    picNode = "mGemPic",
    startIndex = 1
}
-----------------------------------------------
--ExpeditionMaterialCollectionPageBase页面中的事件处理
----------------------------------------------
function ExpeditionMaterialCollectionPageBase.onFunction(eventName, container)
    if eventName:sub(1, 6) == "onFeet" then
		ExpeditionMaterialCollectionPageBase.showItemInfo(container, eventName);
	end	
end

--------------------------滑动事件-------------------
local touchEffectiveDis = 100;
local m_BegainX = 0
function ExpeditionMaterialCollectionPageBase.onTouchBegin(container,eventName,pTouch)
    local point = pTouch:getLocation();
    point = container:convertToNodeSpace(point)
    m_BegainX = point.x;
end

function ExpeditionMaterialCollectionPageBase.onTouchEnd(container,eventName,pTouch)
    local point = pTouch:getLocation();
    point = container:convertToNodeSpace(point)
    local moveDisX = point.x - m_BegainX
    
    --向左滑动
    if moveDisX > 0 and moveDisX > touchEffectiveDis then
        local oldId = CurrentStageId
        CurrentStageId = CurrentStageId - 1
        CurrentStageId = math.max(CurrentStageId,1)
        if oldId~=CurrentStageId then
            local nodeVisible = {}
            nodeVisible["mPoint0"..oldId] = false
            NodeHelper:setNodesVisible(container, nodeVisible);
            ExpeditionMaterialCollectionPageBase:refreshPage(container)
        end
    elseif moveDisX < 0 and moveDisX < (-touchEffectiveDis) then
        local oldId = CurrentStageId
        CurrentStageId = CurrentStageId + 1
        CurrentStageId = CurrentStageId>ExpeditionDataHelper.getMaxStageId() and ExpeditionDataHelper.getMaxStageId() or CurrentStageId
        if oldId~=CurrentStageId then
            local nodeVisible = {}
            nodeVisible["mPoint0"..oldId] = false
            NodeHelper:setNodesVisible(container, nodeVisible);
            ExpeditionMaterialCollectionPageBase:refreshPage(container)
        end
    end
end



function ExpeditionMaterialCollectionPageBase:onEnter(container)
    local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
    NodeHelper:initScrollView(container, "mContent", 4);
	if container.mScrollView~=nil then
		container:autoAdjustResizeScrollview(container.mScrollView);
	end
    NodeHelper:createTouchLayerByScrollView(container,ExpeditionMaterialCollectionPageBase.onTouchBegin,nil,ExpeditionMaterialCollectionPageBase.onTouchEnd)
	self:clearPage(container);
	self:registerPacket(container);
	self:getActivityInfo(container);
end

function ExpeditionMaterialCollectionPageBase:onExecute(container)
    self:onTimer(container);
end

function ExpeditionMaterialCollectionPageBase:onExit(container)
    TimeCalculator:getInstance():removeTimeCalcultor(ExpeditionDataHelper.getPageTimerName());
	NodeHelper:deleteScrollView(container);
	self:removePacket(container);
end
----------------------------------------------------------------
function ExpeditionMaterialCollectionPageBase:onTimer(container)
    local timerName = ExpeditionDataHelper.getPageTimerName()
	if not TimeCalculator:getInstance():hasKey(timerName) then
	    if ExpeditionDataHelper.getActivityRemainTime() <= 0 then
	        local endStr = common:getLanguageString("@ActivityEnd");
	        NodeHelper:setStringForLabel(container, {mCD = endStr});
        end
        return; 
    end

	local remainTime = TimeCalculator:getInstance():getTimeLeft(timerName);
	if remainTime + 1 > ExpeditionDataHelper.getActivityRemainTime() then
		return;
	end

	ExpeditionDataHelper.setActivityRemainTime(remainTime)
	local timeStr = common:second2DateString(ExpeditionDataHelper.getActivityRemainTime(), false);
	
	if ExpeditionDataHelper.getActivityRemainTime() <= 0 then
	    timeStr = common:getLanguageString("@ActivityEnd");
    end
	NodeHelper:setStringForLabel(container, {mCD = timeStr});
	
end

function ExpeditionMaterialCollectionPageBase:clearPage(container)
    NodeHelper:setStringForLabel(container, {
		mCD	= ""
	});
end

function ExpeditionMaterialCollectionPageBase:getActivityInfo(container)
    local msg = Activity_pb.HPExpeditionArmoryInfo();
    msg.version = 1
	common:sendPacket(opcodes.EXPEDITION_ARMORY_INFO_C, msg);
end

function ExpeditionMaterialCollectionPageBase:refreshPage(container)
    local timerName = ExpeditionDataHelper.getPageTimerName()
    local remainTime = ExpeditionDataHelper.getActivityRemainTime()
    if remainTime > 0 and not TimeCalculator:getInstance():hasKey(timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(timerName, remainTime);
	end
	
	local bmLabels = {}
	local nodeVisible = {}
	local textureSet = {}
	
	if CurrentStageId < 1 then
	    CurrentStageId = 1
	elseif CurrentStageId > ExpeditionDataHelper.getMaxStageId() then
	    CurrentStageId = ExpeditionDataHelper.getMaxStageId()
	end
	
	--������ť����
	nodeVisible.mLeftActivityBtn = CurrentStageId~=1
	nodeVisible.mRightActivityBtn = CurrentStageId~=ExpeditionDataHelper.getMaxStageId()
	for i=1,7 do
	    nodeVisible["mPointNode"..i] = i<=ExpeditionDataHelper.getMaxStageId()
	end
	--
	
	local mStageInfo = ExpeditionDataHelper.getStageInfoByStageId(CurrentStageId)
	if mStageInfo~=nil then
	    --整体经验进度
	    if mStageInfo.needExp==0 then
	        bmLabels.mSeepNum = tostring(mStageInfo.curExp).."/"..common:getLanguageString("@ExpeditionFinalStage")
	    else
            bmLabels.mSeepNum = tostring(mStageInfo.curExp).."/"..tostring(mStageInfo.needExp)
        end
        --当前阶段个人贡献
        bmLabels.mNowContributionNum = tostring(mStageInfo.personalStageExp)
        --当前阶段是否可领取奖励
	end

	--获得当前阶段配置信息
	local stageRewardInfo = ExpeditionDataHelper.getStageRewardInfoByStageId(CurrentStageId)
	
	--配置阶段奖励信息
	if stageRewardInfo~=nil then
	    local cfg = ConfigManager.getRewardById(stageRewardInfo.r);
        NodeHelper:fillRewardItemWithParams(container, cfg, 3,RewardParams)
	end
	--完成
	
	--富文本显示获得当前阶段所需贡献
	local mContributionRewardPrompt = container:getVarLabelBMFont("mContributionRewardPrompt")
	local mStageRewardLimit = stageRewardInfo.q
    if mContributionRewardPrompt~=nil and mStageRewardLimit~=nil then
        local str = FreeTypeConfig[101].content
        str = common:fill(str,tostring(mStageRewardLimit))
        NodeHelper:setCCHTMLLabelAutoFixPosition( mContributionRewardPrompt, CCSize(400,32),str )
        mContributionRewardPrompt:setVisible(false)
    end
	--完成
	
    --配置当前页签	
	nodeVisible["mPoint0"..CurrentStageId] = true
	
	--配置当前阶段图片信息
	if stageRewardInfo.p~=nil then
	    textureSet.mExpeditionPic = stageRewardInfo.p
	end
	local tabPic = ExpeditionDataHelper.getTabTexture()
	if tabPic~=nil then
	    textureSet.mNotOpen = common:fill(tabPic.notOpen,tostring(CurrentStageId))
	    textureSet.mStagecompletion = common:fill(tabPic.complete,tostring(CurrentStageId))
	    textureSet.mStage = common:fill(tabPic.open,tostring(CurrentStageId))
	    if CurrentStageId==ExpeditionDataHelper.getMaxStageId() then
	        textureSet.mNotOpen = common:fill(tabPic.notOpen,tostring(7))
            textureSet.mStagecompletion = common:fill(tabPic.complete,tostring(7))
            textureSet.mStage = common:fill(tabPic.open,tostring(7))
	    end
	end
	--配置是否开启状态
	local curId = ExpeditionDataHelper.getCurrentStageId()
	if curId then
	    nodeVisible.mNotOpen = curId<CurrentStageId
	    nodeVisible.mStagecompletion = curId>CurrentStageId
	    nodeVisible.mStage = curId==CurrentStageId
	    nodeVisible.mShieldMessageNode = curId==CurrentStageId
	    if nodeVisible.mShieldMessageNode == true then
	        nodeVisible.mShieldMessageNode = remainTime > 0
	    end
	     
	end
	--完成
	
	nodeVisible.mViewNode = true
	
	NodeHelper:setNodesVisible(container, nodeVisible);
    NodeHelper:setStringForLabel(container, bmLabels);
	NodeHelper:setSpriteImage(container, textureSet);
end
	
----------------click event------------------------	
function ExpeditionMaterialCollectionPageBase:onClose(container)
    PageManager.popPage(thisPageName);
end	

function ExpeditionMaterialCollectionPageBase:onBack()
    PageManager.changePage("ActivityPage");
end

function ExpeditionMaterialCollectionPageBase:onHelp()
    PageManager.showHelp(GameConfig.HelpKey.HELP_EXPEDITION);
end

function ExpeditionMaterialCollectionPageBase:onRanking()
    PageManager.pushPage("ExpeditionRankPage");
end

function ExpeditionMaterialCollectionPageBase:onContributionMaterials()
    PageManager.pushPage("ExpeditionContributePage");
end

function ExpeditionMaterialCollectionPageBase.showItemInfo(container,eventName)
    local stageRewardInfo = ExpeditionDataHelper.getStageRewardInfoByStageId(CurrentStageId)
	if stageRewardInfo~=nil then
	    local cfg = ConfigManager.getRewardById(stageRewardInfo.r); 
	    local rewardIndex = tonumber(eventName:sub(8));
	    if cfg[rewardIndex] ~= nil then
	        GameUtil:showTip(container:getVarNode('mFeet0' .. rewardIndex), common:table_merge(cfg[rewardIndex],{buyTip=true,hideBuyNum=true}));
	    end
	end
end

function ExpeditionMaterialCollectionPageBase:onLeft(container)
    local oldId = CurrentStageId
    CurrentStageId = CurrentStageId - 1
    CurrentStageId = math.max(CurrentStageId,1)
    if oldId~=CurrentStageId then
        local nodeVisible = {}
        nodeVisible["mPoint0"..oldId] = false
        NodeHelper:setNodesVisible(container, nodeVisible);
        ExpeditionMaterialCollectionPageBase:refreshPage(container)
    end
end

function ExpeditionMaterialCollectionPageBase:onRight(container)
    local oldId = CurrentStageId
    CurrentStageId = CurrentStageId + 1
    CurrentStageId = CurrentStageId>ExpeditionDataHelper.getMaxStageId() and ExpeditionDataHelper.getMaxStageId() or CurrentStageId
    if oldId~=CurrentStageId then
        local nodeVisible = {}
        nodeVisible["mPoint0"..oldId] = false
        NodeHelper:setNodesVisible(container, nodeVisible);
        ExpeditionMaterialCollectionPageBase:refreshPage(container)
    end
end

--回包处理
function ExpeditionMaterialCollectionPageBase:onReceivePacket(container)
	local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.EXPEDITION_ARMORY_INFO_S then
		ExpeditionDataHelper.onReceiveExpeditionInfo(msgBuff)
		--ȥ��֮ǰ�׶εĻƵ�
		local nodeVisible = {}
		nodeVisible["mPoint0"..CurrentStageId] = false
        NodeHelper:setNodesVisible(container, nodeVisible);
        --
		CurrentStageId = ExpeditionDataHelper.getCurrentStageId()
		self:refreshPage(container);
		return;
	end
end

function ExpeditionMaterialCollectionPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function ExpeditionMaterialCollectionPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
ExpeditionMaterialCollectionPage = CommonPage.newSub(ExpeditionMaterialCollectionPageBase, thisPageName, option,ExpeditionMaterialCollectionPageBase.onFunction);

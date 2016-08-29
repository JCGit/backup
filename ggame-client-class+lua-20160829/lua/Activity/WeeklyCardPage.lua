----------------------------------------------------------------------------------
--[[
	FILE:			WeeklyCardPage.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			zhoutong
	CREATED:		2014-10-15
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local NodeHelper = require("NodeHelper");
local thisPageName = "WeeklyCardPage"

local opcodes = {
	WEEK_CARD_REWARD_C		= HP_pb.WEEK_CARD_REWARD_C,
	WEEK_CARD_REWARD_S		= HP_pb.WEEK_CARD_REWARD_S,
	WEEK_CARD_INFO_C	= HP_pb.WEEK_CARD_INFO_C,
	WEEK_CARD_INFO_S	= HP_pb.WEEK_CARD_INFO_S
};
local option = {
	ccbiFile = "Act_WeekCardPage.ccbi",
	handlerMap = {
		onReturnButton	= "onBack",
		onRecharge		= "onRechargeFirstWeekCard",
		onRecharge2  = "onRechargeSecondWeekCard",
		onHelp			= "onHelp",
		onReceive       = "onReceive",
		onUpdateCard = "onUpdateCardType"
	},
	opcode = opcodes
};

--活动基本信息，结构基本与协议pb Message相同
local thisActivityInfo = {
	id				= 24,
	remainTime 		= 100000,
	activeWeekCardId 	= 0,
	isReward        = 1,
	--isNeedYestReward = 0,
	leftDays        = 7,
	productIds        = {},
	levelUpProductId = -1,
	
	rewardCfg		= {},
	rewardIds 		= {},
	rewardTables    = {
	    [1] = {
	        mainNode = "mRechargeNode",
	        countNode = "mNum0",
            nameNode = "mName",
            frameNode = "mFeet0",
            picNode = "mTextPic0",
            startIndex = 1
        },
	    [2] = {
	        mainNode = "mRechargeNode",
            countNode = "mNum0",
            nameNode = "mName",
            frameNode = "mFeet0",
            picNode = "mTextPic0",
            startIndex = 4
        },
	    [3] = {
	        mainNode = "mReceiveNode",
            countNode = "mReceiveNum0",
            nameNode = "mName",
            frameNode = "mReceiveFeet0",
            picNode = "mReceivePic0",
            startIndex = 1
        }
    }
};
thisActivityInfo.timerName = "Activity_" .. thisActivityInfo.id;
thisActivityInfo.itemCfg = ActivityConfig[thisActivityInfo.id]["items"] or {};

----------------- local data -----------------
local WeeklyCardPageBase = {}
-----------------------------------------------
--WeeklyCardPageBase页面中的事件处理
----------------------------------------------
function WeeklyCardPageBase:onEnter(container)
    local mScale9Sprite = container:getVarScale9Sprite("mScale9Sprite")
	if mScale9Sprite ~= nil then
		container:autoAdjustResizeScale9Sprite( mScale9Sprite )
	end
	
	self:registerPacket(container);
	self:getActivityInfo(container);
end

function WeeklyCardPageBase:onExecute(container)
	self:onTimer(container);
end

function WeeklyCardPageBase:onExit(container)
	TimeCalculator:getInstance():removeTimeCalcultor(thisActivityInfo.timerName);
	self:removePacket(container);
end

function WeeklyCardPageBase.onFunction(eventName, container)
	if eventName:sub(1, 6) == "onFeet" then
		WeeklyCardPageBase.showItemInfo(container,tonumber(string.sub(eventName, -1)))
	end	
end

----------------------------------------------------------------
function WeeklyCardPageBase:onTimer(container)
	local timerName = thisActivityInfo.timerName;
	if not TimeCalculator:getInstance():hasKey(timerName) then return; end

	local remainTime = TimeCalculator:getInstance():getTimeLeft(timerName);
	if remainTime + 1 > thisActivityInfo.remainTime then
		return;
	end

	thisActivityInfo.remainTime = math.max(remainTime, 0);
	local timeStr = common:second2DateString(thisActivityInfo.remainTime, false);
	NodeHelper:setStringForLabel(container, {mActivityDaysNum = timeStr});
end

function WeeklyCardPageBase:clearPage(container)
	NodeHelper:setStringForLabel(container, {
		mActivityDaysNum	= ""
	});
end

function WeeklyCardPageBase:getActivityInfo(container)
	local msg = Activity_pb.HPWeekCardInfo();
	msg.version = 1
	common:sendPacket(opcodes.WEEK_CARD_INFO_C, msg);
end

function WeeklyCardPageBase:refreshPage(container)
    if thisActivityInfo.remainTime > 0 and not TimeCalculator:getInstance():hasKey(thisActivityInfo.timerName) then
		TimeCalculator:getInstance():createTimeCalcultor(thisActivityInfo.timerName, thisActivityInfo.remainTime);
	end
	
    if thisActivityInfo.activeWeekCardId~=0  then
        NodeHelper:setNodesVisible(container,{
            mWeekCardRecharge = false,
            mWeekCardReceive = true,
            mReceiveNode = thisActivityInfo.isReward==0,
            mSurplusDays = false,
            mReceiveNode      =  thisActivityInfo.levelUpProductId>0,
            mReceive      =  thisActivityInfo.levelUpProductId<=0

        });
        local mIndex = 8 - thisActivityInfo.leftDays
        --[[if thisActivityInfo.isNeedYestReward==0 then
            mIndex = mIndex
        else
            mIndex = (mIndex-1)
        end--]]
        mIndex = mIndex>0 and mIndex or 1
        local type = 0
        type = thisActivityInfo.activeWeekCardId

        
        local weekType = thisActivityInfo.itemCfg[type];
        if weekType~=nil then
            local weekReward = weekType.DailyRewardItem
            local cfg = weekReward[mIndex];
            if cfg ~= nil then
                cfg = ConfigManager.getRewardById(cfg.r);
                NodeHelper:fillRewardItemWithParams(container, cfg, 3,thisActivityInfo.rewardTables[3])
            end
        end
        
        
        
        local surplusDayNode = container:getVarLabelBMFont("mSurplusDays")
        if surplusDayNode~=nil then
            local str = FreeTypeConfig[100].content
            str = common:fill(str,tostring(thisActivityInfo.leftDays))
            NodeHelper:setCCHTMLLabelAutoFixPosition( surplusDayNode, CCSize(180,32), str )
        end
        
        
    else
        NodeHelper:setNodesVisible(container,{
            mWeekCardRecharge = true,
            mWeekCardReceive = false
        });
        

        for i=1,#thisActivityInfo.itemCfg do
            local weekType = thisActivityInfo.itemCfg[i];
            if weekType~=nil then
                local weekReward = weekType.BuyRewardItem
                if weekReward ~= nil then
                    cfg = ConfigManager.getRewardById(weekReward.r);
                    NodeHelper:fillRewardItemWithParams(container, cfg, 3,thisActivityInfo.rewardTables[i])
                end
            end
        end
    end
    
end
----------------click event------------------------	
function WeeklyCardPageBase:onClose(container)
	PageManager.popPage(thisPageName);
end	

function WeeklyCardPageBase:onRechargeFirstWeekCard(container)
    WeeklyCardPageBase:toSKDRechargePage(container,1)
end

function WeeklyCardPageBase:onRechargeSecondWeekCard(container)
    WeeklyCardPageBase:toSKDRechargePage(container,2)
end

function WeeklyCardPageBase:onBack()
	PageManager.changePage("ActivityPage");
end

function WeeklyCardPageBase:onHelp()
    PageManager.showHelp(GameConfig.HelpKey.HELP_WEEKLYCARD);
end

function WeeklyCardPageBase:onReceive(container)
	local msg = Activity_pb.HPWeekCardReward();
	--msg.isYest = thisActivityInfo.isNeedYestReward
	common:sendPacket(opcodes.WEEK_CARD_REWARD_C, msg,false);
end

function WeeklyCardPageBase:onUpdateCardType(container)
    local type = 2
    if thisActivityInfo.productIds~=nil then
        for i=1,#thisActivityInfo.productIds do
            if thisActivityInfo.productIds[i]~=nil and thisActivityInfo.productIds[i] == thisActivityInfo.levelUpProductId then
                type = i
            end
        end
    end

    WeeklyCardPageBase:toSKDRechargePage(container,type)
end

function WeeklyCardPageBase:toSKDRechargePage(container,type)
    local productName = common:getLanguageString("@WeeklyCard")
    local card  = thisActivityInfo.productIds[type]

    if card~=nil then
        local buyInfo = BUYINFO:new()
        buyInfo.productCount = 1
        buyInfo.productName = productName
        buyInfo.productId = card
        if card==1 then
            buyInfo.productPrice = 10
            buyInfo.productOrignalPrice = 100
        elseif card==5 then
            buyInfo.productPrice = 100
            buyInfo.productOrignalPrice = 1000
        else
            buyInfo.productPrice = 0
            buyInfo.productOrignalPrice = 0
        end
        
        buyInfo.description = GamePrecedure:getInstance():getServerID()--ServerDateManager:getInstance().mLoginInfo.m_iPlayerID
        
        libPlatformManager:getPlatform():buyGoods(buyInfo)
    end
end

function WeeklyCardPageBase.showItemInfo(container, index)
    local itemCfgId
    local nodeName
    if index >=4 and index<7 then 
        local type = thisActivityInfo.itemCfg[2];
        itemCfgId = type.BuyRewardItem.r
        nodeName = 'mFeet0' .. index
    elseif index>=7 then
        local type = thisActivityInfo.itemCfg[thisActivityInfo.activeWeekCardId];
        itemCfgId = type.DailyRewardItem[8-thisActivityInfo.leftDays].r
        nodeName = 'mReceiveFeet0' .. (index-6)
    else
        local type = thisActivityInfo.itemCfg[1];
        itemCfgId = type.BuyRewardItem.r
        nodeName = 'mFeet0' .. index
    end
    
    
	local cfg = ConfigManager.getRewardById(itemCfgId);
	local rewardIndex = index % 3
	rewardIndex = rewardIndex~=0 and rewardIndex or 3
	if cfg[rewardIndex] ~= nil then
	    GameUtil:showTip(container:getVarNode(nodeName), common:table_merge(cfg[rewardIndex],{buyTip=true,hideBuyNum=true}));
	end
end

--回包处理
function WeeklyCardPageBase:onReceivePacket(container)
    local opcode = container:getRecPacketOpcode();
	local msgBuff = container:getRecPacketBuffer();
    if opcode == opcodes.WEEK_CARD_INFO_S then
		local msg = Activity_pb.HPWeekCardInfoRet();
		msg:ParseFromString(msgBuff);
		thisActivityInfo.activeWeekCardId = msg.activeWeekCardId;
		thisActivityInfo.remainTime = msg.leftSenconds;
		thisActivityInfo.isReward = msg.isTodayReward;	
		--thisActivityInfo.isNeedYestReward  = msg.isNeedYestReward or 0
		thisActivityInfo.leftDays = msg.leftDays
		thisActivityInfo.productIds = msg.productId or {}
		thisActivityInfo.levelUpProductId = msg.levelUpProductId
		self:refreshPage(container);
		return;
	end
end

function WeeklyCardPageBase:registerPacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:registerPacket(opcode)
		end
	end
end

function WeeklyCardPageBase:removePacket(container)
	for key, opcode in pairs(opcodes) do
		if string.sub(key, -1) == "S" then
			container:removePacket(opcode)
		end
	end
end
-------------------------------------------------------------------------
local CommonPage = require("CommonPage");
WeeklyCardPage = CommonPage.newSub(WeeklyCardPageBase, thisPageName, option,WeeklyCardPageBase.onFunction);

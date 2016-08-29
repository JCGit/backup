----------------------------------------------------------------------------------
--[[
	FILE:			ExpeditionDataHelper.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	活动
	AUTHOR:			zhoutong
	CREATED:		2014-10-15
--]]
----------------------------------------------------------------------------------
local Activity_pb = require("Activity_pb");
local HP_pb = require("HP_pb");
local UserInfo = require("UserInfo");

local ExpeditionDataHelper = {}

local thisActivityInfo = {
	id				= 25,
	remainTime 		= 1000,
	curStageId      = 1,
	stageInfo       = {},
	selfRank        = 1,
	selfTotalExp    = 0,
	rankReward      = 0,
	rankInfo        = {},
	rewardCfg		= {},
	rewardIds 		= {}
};
thisActivityInfo.timerName = "Activity_" .. thisActivityInfo.id;
thisActivityInfo.itemCfg = ActivityConfig[thisActivityInfo.id]["items"] or {};


--回包处理
function ExpeditionDataHelper.onReceiveExpeditionInfo(msgBuff)
		local msg = Activity_pb.HPExpeditionArmoryInfoRet();
		msg:ParseFromString(msgBuff);
		--活动信息
		thisActivityInfo.remainTime = msg.leftTime or 0
		--活动当前阶段
		thisActivityInfo.curStageId = msg.curStage or 1
        --获得阶段信息
        thisActivityInfo.stageInfo = msg.expeditionArmoryStage or {}
        table.sort(thisActivityInfo.stageInfo,function(a,b) return (a.stage < b.stage) end )
end

function ExpeditionDataHelper.onReceiveRankInfo(msgBuff)
		local msg = Activity_pb.HPExpeditionArmoryRankingRet();
		msg:ParseFromString(msgBuff);
		thisActivityInfo.rankInfo = msg.rankItem or {}
        thisActivityInfo.selfRank = msg.selfRank or 0
        thisActivityInfo.selfTotalExp = msg.selfTotalExp or 0
        thisActivityInfo.rankReward	 = 	msg.rankReward or 0
        
        table.sort(thisActivityInfo.rankInfo,function(a,b) return (a.rank < b.rank) end )
end
---------------------------------------------------------------------
---------------------------------------------------------------------

--数据接口
--
--获得活动倒计时时间名称
function ExpeditionDataHelper.getPageTimerName()
    return thisActivityInfo.timerName
end
--设置活动时间
function ExpeditionDataHelper.setActivityRemainTime(timer)
    thisActivityInfo.remainTime = timer
end
--获得活动剩余时间
function ExpeditionDataHelper.getActivityRemainTime()
    return thisActivityInfo.remainTime
end
--获得阶段信息
function ExpeditionDataHelper.getStageInfoByStageId(id)
    return thisActivityInfo.stageInfo[id];
end
--获得当前阶段的阶段奖励
function ExpeditionDataHelper.getStageRewardInfoByStageId(id)
    return thisActivityInfo.itemCfg.stageInfo[id]
end
--获得当前阶段
function ExpeditionDataHelper.getCurrentStageId()
    return thisActivityInfo.curStageId
end
--获得阶段状态图片信息
function ExpeditionDataHelper.getTabTexture()
    return thisActivityInfo.itemCfg.tabPicConfig
end
--获得最大阶段
function ExpeditionDataHelper.getMaxStageId()
    return #thisActivityInfo.itemCfg.stageInfo
end

--排行数据接口
--获得排行信息
function ExpeditionDataHelper.getRankInfo()
    return thisActivityInfo.rankInfo
end
--获得排行奖励配置信息
function ExpeditionDataHelper.getRankRewardInfo()
    return thisActivityInfo.itemCfg.rankReward
end
--获得单项排行信息
function ExpeditionDataHelper.getRankInfoByIndex(index)
    return thisActivityInfo.rankInfo[index]
end
--获得单项排行奖励配置信息
function ExpeditionDataHelper.getRankRewardInfobyIndex(index)
    return thisActivityInfo.itemCfg.rankReward[index]
end
--获得排行数量
function ExpeditionDataHelper.getRankInfoSize()
    return #thisActivityInfo.rankInfo
end
--获得玩家排行榜信息
function ExpeditionDataHelper.getSelfRankInfo()
    local rankInfo = {
        exp = thisActivityInfo.selfTotalExp,
        rank = thisActivityInfo.selfRank,
        name = UserInfo.roleInfo.name
    }
    return rankInfo
end
--获得玩家排名
function ExpeditionDataHelper.getSelfRank()
    return thisActivityInfo.selfRank
end
--获得当前是否可领取排行奖励
function ExpeditionDataHelper.getRankRewardState()
    return thisActivityInfo.rankReward
end
--获得玩家总贡献度
function ExpeditionDataHelper.getSelfContribution()
    return thisActivityInfo.selfTotalExp
end
--贡献页面接口
--获得单项贡献信息
function ExpeditionDataHelper.getDonationItemInfoByIndex(index)
    return thisActivityInfo.itemCfg.donationItem[index]
end
--获得贡献道具种类数量
function ExpeditionDataHelper.getDonationItemSize()
    return #thisActivityInfo.itemCfg.donationItem
end

---------------------------------------------------------------------
---------------------------------------------------------------------

return ExpeditionDataHelper;
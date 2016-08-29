ActivityConfig = 
{--活动页面排列优先级order：福利活动1X，消耗活动2X，充值活动3X，开服活动4X
	[2]	= {	--每日返利（每日累计充值返利）
		openWay	= "change",
		image	= "UI/Activities/u_icoqyDailyRecharge.png",
		page	= "AccumulativeRecharge",
		order	= 31,
		items 	= {	--*********fmt:	[id] = {c = 充值数量, r =	奖励id}*******
			[1] = {c = 200, 	r = 71},
			[2] = {c = 500, 	r = 72},
			[3] = {c = 1000, 	r = 73},
			[4] = {c = 3000, 	r = 74},
			[5] = {c = 5000, 	r = 75}
		}
	},
	[29]	= {	-- 每日累计充值（开服活动）
		openWay	= "change",
		image	= "UI/Activities/u_icoqyDailyRecharge.png",
		page	= "AccumulativeRecharge2",
		order	= 31,
		items 	= {	--*********fmt:	[id] = {c = 充值数量, r =	奖励id}*******
			[1] = {c = 500, 	r = 701},
			[2] = {c = 1000, 	r = 702},
			[3] = {c = 3000, 	r = 703},
			[4] = {c = 5000, 	r = 704},
			[5] = {c = 8000, 	r = 705},
			[6] = {c = 10000, 	r = 706}
		}
	},
	[3]	= {	--连续充值
		openWay	= "change",
		image	= "UI/Activities/u_icoqyRechargeRebate.png",
		page	= "ContinueRecharge",
		order	= 32,
		items 	= {	--*********fmt:	[id] = {d = 天数, r =	奖励id}*******
			[1] = {d = 1, 	r = 61},
			[2] = {d = 2, 	r = 62},
			[3] = {d = 3, 	r = 63},
			[4] = {d = 4, 	r = 64},
			[5] = {d = 5, 	r = 65},
			[6] = {d = 6, 	r = 66},
			[7] = {d = 7, 	r = 67}		
		}
	},
	[21] = {	--单笔充值（每日单笔充值返利）
		openWay	= "change",
		image	= "UI/Activities/u_icoqySinglePenRecharge.png",
		page	= "DailyOnceRecharge",
		order	= 33,
		items 	= {	--*********fmt:	[id] = {c = 充值数量, r =	奖励id}*******
			[1] = {c = 100, 	r = 33},
			[2] = {c = 500, 	r = 34},
			[3] = {c = 1000, 	r = 35}
		}
	},
	[31] = {	--每日单笔充值（开服活动）
		openWay	= "change",
		image	= "UI/Activities/u_icoqySinglePenRecharge.png",
		page	= "DailyOnceRecharge2",
		order	= 33,
		items 	= {	--*********fmt:	[id] = {c = 充值数量, r =	奖励id}*******
			[1] = {c = 100, 	r = 303},
			[2] = {c = 500, 	r = 304},
			[3] = {c = 1000, 	r = 305},
			[4] = {c = 2000, 	r = 306},
			[5] = {c = 5000, 	r = 307},
			[6] = {c = 10000, 	r = 308},
			[7] = {c = 20000, 	r = 309},
			[8] = {c = 30000, 	r = 400}
		}
	},
	[20] = {	--新手活动（新手8天专享翻倍活动）
		openWay	= "change",
		image	= "UI/Activities/u_icoqyNoviceActivities.png",
		page	= "DailyLogin",
		order	= 41,
	},
	[22] = {	--充值返利
		openWay	= "change",
		image	= "UI/Activities/u_icoqyRechargeReturnDiamond.png",
		page	= "RechargeRebatePage",
		order	= 42,
	},
	[28] = { --充值返利2
		openWay	= "change",
		image	= "UI/Activities/u_icoqyRechargeReturnDiamond.png",
		page	= "RechargeRebatePage",
		order	= 24,
	},
	[6]	= {	--累计消耗
		openWay	= "change",
		image	= "UI/Activities/u_icoqyRechargeConsumption.png",
		page	= "AccumulativeConsume",
		order	= 23,
		items 	= {	--*********fmt:	[id] = {n = 数量, r =	奖励id}*******
			[1] = {n = 3000, 	r = 101},
			[2] = {n = 5000, 	r = 102},
			[3] = {n = 8000, 	r = 103},
			[4] = {n = 10000, 	r = 104},
			[5] = {n = 15000, 	r = 105},
			[6] = {n = 20000, 	r = 106},
			[7] = {n = 25000, 	r = 107},
			[8] = {n = 30000, 	r = 108},
			[9] = {n = 35000, 	r = 109},
			[10] = {n = 40000, 	r = 110},
			[11] = {n = 50000, 	r = 111},
			[12] = {n = 60000, 	r = 112},
			[13] = {n = 70000, 	r = 113},
			[14] = {n = 80000, 	r = 114},
			[15] = {n = 100000, r = 115}
		}
	},
	[30]	= {	--累计消耗(开服活动)
		openWay	= "change",
		image	= "UI/Activities/u_icoqyRechargeConsumption.png",
		page	= "AccumulativeConsume2",
		order	= 23,
		items 	= {	--*********fmt:	[id] = {n = 数量, r =	奖励id}*******
			[1] = {n = 3000, 	r = 1001},
			[2] = {n = 5000, 	r = 1002},
			[3] = {n = 8000, 	r = 1003},
			[4] = {n = 10000, 	r = 1004},
			[5] = {n = 15000, 	r = 1005},
			[6] = {n = 20000, 	r = 1006},
			[7] = {n = 25000, 	r = 1007},
			[8] = {n = 30000, 	r = 1008},
			[9] = {n = 35000, 	r = 1009},
			[10] = {n = 40000, 	r = 1010},
			[11] = {n = 50000, 	r = 1011},
			[12] = {n = 60000, 	r = 1012},
			[13] = {n = 70000, 	r = 1013},
			[14] = {n = 80000, 	r = 1014},
			[15] = {n = 100000, r = 1015}
		}
	},
	[7]	= {	--中秋欢乐兑（兑字活动）
		openWay	= "change",
		image	= "UI/Activities/u_icoqyMid-autumn.png",
		page	= "ExchangeActivity",
		order	= 21,
		items 	= {	--*********fmt:	[id] = {c = 消耗id, r =	奖励id, t = 兑换次数}<<t = -1 表示无限制 >> *******
			[1] = {c = 1, 	r = 11,	t = -1},
			[2] = {c = 2, 	r = 12,	t = -1},
			[3] = {c = 3, 	r = 13,	t = -1},
			[4] = {c = 4, 	r = 14,	t = -1}
		}
	},
	[9] = { --限时翻倍（充值翻倍活动）
		openWay = "push",
		image	= "UI/Activities/u_icoqyTimedDouble.png",
		page	= "LargeRechargeRebate",
		order	= 35
	},
	[10] = { --假日秘宝（宝箱活动）
		openWay = "change",
		image	= "UI/Activities/u_icoqyHolidayTreasure.png",
		page	= "HolidayTreasure",
		order	= 22,
		items	= {
			--[id] = 奖励id
			[1] = 31,
			[2] = 32
		}
	},
	[11] = { --新人福利（公测兑字及公共兑换码）
		openWay = "change",
		image	= "UI/Activities/u_icoqyNewWelfare.png",
		page	= "ExchangeBetaWord",
		order	= 42
	},
	
	[24] = { --周卡
		openWay = "change",
		image	= "UI/Activities/u_icoqyWeekCard.png",
		page	= "WeeklyCardPage",
		order	= 36,
		items	= {
			--这是第一类周卡（10元）的奖励id  d是第几天，r是奖励id  下面是第二类
			[1] = {
				BuyRewardItem = {r = 24001},
				DailyRewardItem = {
					 [1] = {d = 1, 	r = 24001},
					 [2] = {d = 2, 	r = 24001},
					 [3] = {d = 3, 	r = 24001},
					 [4] = {d = 4, 	r = 24001},
					 [5] = {d = 5, 	r = 24001},
					 [6] = {d = 6, 	r = 24001},
					 [7] = {d = 7, 	r = 24001}
				}
			},
			--
			[2] = {
				BuyRewardItem = {r = 24002},
				DailyRewardItem = {
					 [1] = {d = 1, 	r = 24002},
					 [2] = {d = 2, 	r = 24002},
					 [3] = {d = 3, 	r = 24002},
					 [4] = {d = 4, 	r = 24002},
					 [5] = {d = 5, 	r = 24002},
					 [6] = {d = 6, 	r = 24002},
					 [7] = {d = 7, 	r = 24002}
				}
			}
		}
	},
	
	
	[25] = { --远征
		openWay = "change",
		image	= "UI/Activities/u_icoqyExpeditionMaterialsCollection.png",
		page	= "ExpeditionMaterialCollectionPage",
		order	= 25,
		
		items	= {
			--[id] = 奖励id  r是奖励id号  q是达到多少贡献才能领取
			stageInfo = {
				[1] = {r=41,q=800,p="UI/Activities/ExpeditionMaterialsCollection/u_ContinuousLandingBG03.png"},
				[2] = {r=42,q=1200,p="UI/Activities/ExpeditionMaterialsCollection/u_ContinuousLandingBG04.png"},
				[3] = {r=43,q=1600,p="UI/Activities/ExpeditionMaterialsCollection/u_ContinuousLandingBG05.png"},
				[4] = {r=44,q=2400,p="UI/Activities/ExpeditionMaterialsCollection/u_ContinuousLandingBG07.png"}
			},
			--这是排行奖励配置，rankT和rankB是多少名到多少名  例如第一名 rankT = rankB = 1  4~10名奖励  rankT = 4 rankB = 10
			rankReward = {
				[1] = {rankT = 1,rankB = 1,r = 51},
				[2] = {rankT = 2,rankB = 2,r = 52},
				[3] = {rankT = 3,rankB = 3,r = 53},
				[4] = {rankT = 4,rankB = 10,r = 54},
				[5] = {rankT = 11,rankB = 20,r = 55},
				[6] = {rankT = 21,rankB = 50,r = 56}
			},
			--这是贡献道具配置，itemType是道具类型，item是道具id，reward是捐献一个获得多少贡献值
			donationItem = {
				[1] = {itemType=30000,item = 10017,reward=5},
				[2] = {itemType=30000,item = 10018,reward=30},
				[3] = {itemType=30000,item = 10019,reward=100}
			},
			
			tabPicConfig = {
				notOpen="UI/Activities/ExpeditionMaterialsCollection/u_ContinuousLandingContent0#v1#_3.png",
					open="UI/Activities/ExpeditionMaterialsCollection/u_ContinuousLandingContent0#v1#_1.png",
					complete="UI/Activities/ExpeditionMaterialsCollection/u_ContinuousLandingContent0#v1#_2.png"

			}
		}
	},
	
	[26] = {
		openWay = "change",
		image = "UI/Activities/u_icoqyTimeLimitPurchase.png",
		page = "TimeLimitPurchasingPage",
		order = 1
	},
	
	[5] = {--微信分享
	    openWay = "click",
	    image	= "UI/Activities/u_icoqyNewWelfare.png",
		page	= "ExpeditionMaterialCollectionPage",
		order	= 38
	},
	
	[23] = { --VIP福利
		openWay = "change",
		image	= "UI/Activities/u_icoqyVipWelfare.png",
		page	= "VipWelfarePage",
		order	= 19
	},
	[19] = { -- 全服快速战斗钻石减半

	},
	
	[27] = { --累计登陆
		openWay = "change",
		image	= "UI/Activities/u_icoqyCumulativeLand.png",
		page	= "CumulativeLoginPage",
		order	= 11
	},
	[32] = { --累计登陆（开服活动）
		openWay = "change",
		image	= "UI/Activities/u_icoqyCumulativeLand.png",
		page	= "CumulativeLoginPage2",
		order	= 11
	},
	[99999] = {
		openWay	= "none",
		image	= "UI/Activities/u_icoqyNotOpend.png",
	}
};

ActivityPicCfg = {
	[2]	= {	--每日返利（每日累计充值返利）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/DailyRecharge/u_DailyRecharge.png",
	},
	[3]	= {	--连续充值
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/RechargeRebate/u_RechargeRebateTitle.png",
	},
	[21] = {	--单笔充值（每日单笔充值返利）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/SinglePenRecharge/u_SinglePenRecharge.png",
	},
	[20] = {	--新手活动（新手8天专享翻倍活动）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/NoviceActivities/u_NoviceActivities.png",
	},
	[22] = {	--充值返利
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/RechargeReturnDiamond/u_RechargeReturnDiamondTitle.png",
	},
	[6]	= {	--累计消耗
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/RechargeConsumption/u_RechargeConsumptionTitle.png",
	},
	[7]	= {	--中秋欢乐兑（兑字活动）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/Mid-autumn/u_Mid-autumnTitle.png",
	},
	[9] = { --限时翻倍（充值翻倍活动）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/TimedDouble/u_TimedDoubleTitle.png",
	},
	[10] = { --假日秘宝（宝箱活动）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/HolidayTreasure/u_HolidayTreasureTitle.png",
	},
	[11] = { --新人福利（公测兑字及公共兑换码）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/NewWelfare/u_NewWelfareTitle.png",
	},
	
	[24] = { --周卡
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/WeekCard/u_WeekCardTitle.png",
	},
	[25] = { --远征
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/ExpeditionMaterialsCollection/u_ContinuousLandingContentTitle01.png",
	},
	[5] = {--微信分享
	    BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/u_ActivitiesTitle.png",
	},
	
	[23] = { --VIP福利
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/VipWelfare/VipWelfareTitle.png",
	},
	[29]	= {	--每日累计充值（开服）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/DailyRecharge/u_DailyRecharge.png",
	},
	[30]	= {	--累计消耗（开服）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/RechargeConsumption/u_RechargeConsumptionTitle.png",
	},
	[31] = {	--每日单笔充值返利（开服）
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/SinglePenRecharge/u_SinglePenRecharge.png",
	},
	[99999] = {
		BGPic		= "UI/Activities/u_ActivitiesBG04.png",
		LabelPic 	= "UI/Activities/u_ActivitiesTitle01.png",
	},
}
return {
	ProfessionIcon = {
		[1] = "UI/Program/u_IcoWarrior.png",
		[2]	= "UI/Program/u_IcoHunter.png",
		[3] = "UI/Program/u_IcoMaster.png"
	},
	QualityImage = {
		[1]	= "UI/MainScene/UI/u_Icobg01.png",	--white
		[2]	= "UI/MainScene/UI/u_Icobg02.png",	--green
		[3]	= "UI/MainScene/UI/u_Icobg03.png",	--blue
		[4]	= "UI/MainScene/UI/u_Icobg04.png",	--purple
		[5]	= "UI/MainScene/UI/u_Icobg05.png"	--orange
	},
	QualityColor = {
		[1]	= "255 255 255",	--white
		[2]	= "0 255 0",		--green
		[3]	= "14 157 213",		--blue
		[4]	= "255 0 255",		--purple
		[5]	= "255 125 0"		--orange
	},
	Default	= {
		Quality 		= 1,
		ProfessionId 	= 1,
		ArenaBuyTimes   = 20,
		LeaveMessageLenCount = 17,
		LeaveMessageLen = 2,
		LeaveMsgDetailLenCount = 15,
		NewMsgKey = "NewMessageKey",
		LeaveMsgCount = 40,
		LeaveMsgConfirmCount = 16,
		ChatCoolTime = 4,
		MailBattleTeamNameCount = 7,
		ShopItemNum = 6
	},
	LevelLimit = {
        MecenaryLvLimit = 15,
		PlayerLevel		= 90,
		EquipDress 		= 10,
		EnhanceLevelMax	= 15,
		GodlyLevelMax	= 15
    },
	FightLimit = {
		CampWarLimit = 2000
	},
	DressEquipType = {
		On 		= 1,
		Off 	= 2,
		Change 	= 3
	},
	BuyPackage = {
		Count	= 10,	--一次购买背包个数
		Cost	= 100	--一次购买背包所需钻石数
	},
	OpenLevel = {
		GemPunch	= 10, 		--钻石打孔
		EmbedGem	= 10,		--镶嵌
		Baptize		= 10
	},
	Image = {
		ClickToSelect 	= "UI/MainScene/Font/u_Font07.png",		--点击选择图片
		PunchSlot		= "Item/42.png",		--钻孔图片
		Empty			= "UI/MainScene/UI/u_Icobg07.png",		--图片
		GemIcon			= {
			"UI/MainScene/Common/u_Gem01.png",
			"UI/MainScene/Common/u_Gem02.png",
			"UI/MainScene/Common/u_Gem03.png",
			"UI/MainScene/Common/u_Gem04.png"
		},
		Vip 			= "UI/MainScene/Font/u_vip%d.png",
		Diamond			= "Item/2.png",
		MonthCard		= "Item/5.png",
		DefaultGift		= "Item/gift1.png",
		FriendBtn_1		= "UI/MainPage/u_MainPageBtn06.png",
		FriendBtn_2		= "UI/MainPage/u_MainPageBtn09.png",
		SkillDisable	= "UI/MainScene/UI/u_Equipmentbg11.png",
		DefaultSoulStone = "UI/MainScene/UI/u_Item01.png",
		EmptySoulStone = "UI/MainScene/UI/u_ico000.png"
	},
	Cost = {
		RefreshBuildingEquip 	= 20,	--打造刷新
		RoleTrain 				= {
			Common 		= 500,
			GoldNormal	= 20,
			GoldMedium	= 60,
			GoldSenior	= 200
		},
		BuildGodlyEquip 		= 5000,
		SpecialBuild 			= 20000,
		CreateRegimentTeam		= 50,		--团战创建队伍	
		CreateAlliance			= 300,		--创建公会
		CompoundEquip			= 2000,		--神器融合消耗声望
		CampInspire				= 20		--阵营战鼓舞
	},
	--游戏帮助
	HelpKey = {
		HELP_DEFAULT				= "HelpDefault",		--选角色默认help
		HELP_MAIN 					= "Help",				--主页help
		HELP_LEAVEMSG     			= "HelpLeaveMsg",		--留言help
		HELP_LEAVEMSG_DETAIL		= "HelpLeaveMsgDetail",	--留言详情help
		HELP_ALLIANCE				= "Helpalliance",		--公会help
		HELP_FORGE					= "HelpForge", 			--打造help
		HELP_FORGEGODLY				= "HelpForgeGodly", 	--打造神器help
		HELP_REGIMENTWAR			= "HelpRegimentwar", 	--多人团战help
		HELP_SKILL  				= "Helpskill", 			--技能help
		HELP_SMELT  				= "HelpSmelt", 			--熔炼help
		HELP_COMPOUND				= "HelpCompound",		--神器融合
		ANNOUNCEMENT  				= "announcement", 		--公告
		HELP_EXTEND					= "Helpinherit",		--传承
		HELP_EMBED					= "Helpinlay",			--镶嵌
		HELP_ENHANCE				= "HelpStrength",		--强化
		HELP_BAPTIZE				= "HelpSuccinct",		--洗炼
		HELP_SWALLOW				= "Helpswallow",		--吞噬
		HELP_GEM_UPGRADE			= "Helpjewel",			--宝石升级
		HELP_MARKET_ITEM			= "HelpMarketItem",		--商城help
		HELP_MARKET_GOLD			= "HelpMarketGold",		--商城help
		HELP_REGIMENTWAR_TEAM		= "HelpRegimentwarTeam",--多人团战成员管理页面
		PERSONAL_INFO				= "HelpPersonInfo",		--多人团战成员管理页面
		SIGNATURE_PAGE_HELP			= "SignatureHelp",		--签名帮助
		HELP_ACT_AUTUMN				= "HelpAutumn",			--中秋兑字活动说明
		HELP_GIFT					= "HelpGift",			--礼包页面帮助
		HELP_ACTIVE					= "HelpActive",			--活动
		HELP_FIGHT_NUM				= "HelpFightNum",		--战力
		HELP_FRIEND					= "HelpFriend",			--好友
		HELP_PVP					= "HelpPVP",			--PVP
		HELP_CAMP					= "HelpCamp",			--阵营战相关
		--HELP_HOLIDYTREASURE			= "HelpHolidayTreasure",	--假日秘宝			
		HELP_TITLE 					= "HelpTitle", 			--称号
		HELP_PvpActivity			= "HelpPvpActivity", 	--团战
		HELP_VIPWELFARE				= "HelpVipWelefare",	--vip福利
		HELP_ELITEMAP				= "HelpEliteMap",		--阵营战相关
		HELP_MERCENARY				= "HelpMercenary",		--佣兵
		HELP_MERCENARYSKILL			= "HelpMercenarySkill",	--佣兵技能
		HELP_MERCENARYUPSTAR		= "HelpMercenaryUpStar",--佣兵升星
		HELP_MERCENARYHALO			= "HelpMercenaryHalo",	--佣兵光环
		HELP_WEEKLYCARD				= "HelpWeeklyCard",	--周卡
		HELP_EXPEDITION				= "HelpExpedition",	--远征
		HELP_DAILYONCERECHARGE 		= "HelpDailyOnceRecharge", --每日单笔充值
		HELP_TIMELIMITPURCHASE		= "HelpTimeLimitPurchas",   --限时限购
		HELP_RECHARGEREBATE 		= "HelpRechargeRebate", -- 充值返利帮助
		
		HELP_ARENA				= "HelpArena",	--比武记录页面
	},
	ItemId = {
		EnhanceElite	= 60001,	--强化精华
		ChallengeTicket = 90001,	--Boss挑战券
		GodlyEquipStone = 90002		--注灵之石
	}
	,
	ColorMap = 
	{
		COLOR_WHITE = "255 255 255",
		COLOR_GREEN = "4 234 101",
		COLOR_RED = "255 0 0",
		COLOR_YELLOW = "239 224 53",
		COLOR_QING = "0 255 255",
		COLOR_BLUE = "27 222 239",
		COLOR_PURPLE = "255 0 255",
		COLOR_ORANGE = "231 101 26",
		COLOR_GRAY = "127 127 127",
		COLOR_FRIEND_OWN = "2 253 32",
		COLOR_FRIEND_OTHER = "254 212 73",
		COLOR_DESCRIPTION_ORANGE = "255 192 0",
		COLOR_TITLE_PURPLE = "247 159 255",
		COLOR_DESCRIPTION_PURPLE = "238 47 255",
		COLOR_TITLE_BLUE = "114 220 255",
		COLOR_DESCRIPTION_BLUE = "0 192 255",
	},
	LineWidth = {
		GodlyAttr			= 6,
		EquipInfo 			= 280,
		EquipInfoFull 		= 400,
		BuildEquip			= 13,
		MoreAttribute 		= 610,		--更多属性
		ItemDescribe		= 18,
		SecondaryAttrNum 	= 8,
		MailContent			= 480,		--邮件内容
		BattleChat			= 18,
		ArenaRecordContent  = 270,
		PlayerTitle 		= 200, 		--人物称号
		PlayerTitleDescribe = 380,		--称号描述
		Tip 				= 275,		--Tip内容宽度
		MercenaryHaloDescribe = 18      --佣兵光环描述
	},
	EquipPartWeight = {
		2, 2, 0, 1, 1,
		3, 4, 4, 2, 3
	},
	PartOrder = common:table_flip({
		2, 10, 9, 8, 7, 3, 4, 1, 6, 5
	}),
	Tag = {
		GemList 	= 20101,
		HtmlLable	= 20102,
		EquipAni	= 30000,
		TipLayer	= 40000,
		TipLevelUp	= 50000,
		FriendBtn	= 1
	},
	NewPointType = 
	{
		TYPE_RegimentWar_NEW_CLOSE = -5,
		TYPE_ALLIANCE_NEW_CLOSE = -4,
		TYPE_GIFT_NEW_CLOSE = -3,
		TYPE_CHAT_MESSAGE_CLOSE = -2,
		TYPE_MAIL_CLOSE = -1

	},
	TeamBattleLimit = 1500,
	--战斗日志的间距，以像素为单位
	FightLogSlotWidth = 10,
	--离线战斗行间距，以像素为单位
	OfflineSlotWidth = 10,
	bossWipeSlotWidth = 10,
	FreeTypeId = {
		Quality_1 			= 43,
		Quality_2 			= 44,
		Quality_3 			= 45,
		Quality_4 			= 46,
		Quality_5 			= 47,
		EquipCondition 		= 48,
		EquipGrade			= 49,
		MainAttr			= 50,
		GodlyAttr			= 51,
		GodlyActiveAttr		= 52,
		GodlyUnactiveAttr	= 53,
		GemInfo				= 54,
		SecondaryAttr_1		= 57,
		SecondaryAttr_2		= 58,
		SecondaryAttr_3		= 59,
		SecondaryAttr_4		= 60,
		SecondaryAttr_5		= 61,
		LvNameProf			= 63,
		EquipInfoWrap		= 64,
		FullGodlyAttr		= 65,
		EquipCondition_1 	= 66,
		EquipGrade_1		= 67,
		MainAttr_1			= 68,
		GodlyAttr_1			= 69,
		SecondaryAttr_1_1	= 70,
		SecondaryAttr_2_1	= 71,
		SecondaryAttr_3_1	= 72,
		SecondaryAttr_4_1	= 73,
		SecondaryAttr_5_1	= 74,
		GodlyNextStarAttr	= 75,
		FullGodlyAttr_1		= 76,
		QMGJ_CDK			= 84,
		--Tips 相关 begin
		TipCommon			= 85,
		TipBuyEquip			= 86,
		TipCondition		= 87,
		TipName_1			= 93,
		TipName_2			= 94,
		TipName_3			= 95,
		TipName_4			= 96,
		TipName_5			= 97,
		TipSmeltEquip		= 98,
		TipStarEquip		= 102,
		TipForGuild         = 601,
		--Tips 相关 end
		--周卡
		WeekCardRemainTime  = 100,
		--
		--远征
		ExpeditionLimit     = 101,
		MecenaryUpStepFormat = 103,
		MecenaryUpStepFormat1 = 104,
		ExpeditionContribute= 501
		--
	},
	Color = {
		Lack	= {246, 54, 69},
		Own		= {255, 255, 255},
		Grey	= {185, 185, 185},
		LackRed	= {255, 0, 0}	
	},
	Count = {
		PieceToMerge 	= 10,
		PartTotal		= 10,
		MinActivity		= 5
	},
	Margin = {
		EquipInfo 			= 10,
		EquipSelect 		= 5,
		EquipSelectBottom 	= 0
	},
	WordSizeLimit = {
        RoleNameLimit = 6
    },
	VipLevelPic = {
		[0] = "UI/MainScene/Font/u_vip0.png",
		[1] = "UI/MainScene/Font/u_vip1.png",
		[2] = "UI/MainScene/Font/u_vip2.png",
		[3] = "UI/MainScene/Font/u_vip3.png",
		[4] = "UI/MainScene/Font/u_vip4.png",
		[5] = "UI/MainScene/Font/u_vip5.png",
		[6] = "UI/MainScene/Font/u_vip6.png",
		[7] = "UI/MainScene/Font/u_vip7.png",
		[8] = "UI/MainScene/Font/u_vip8.png",
		[9] = "UI/MainScene/Font/u_vip9.png",
		[10] = "UI/MainScene/Font/u_vip10.png",
		[11] = "UI/MainScene/Font/u_vip11.png",
		[12] = "UI/MainScene/Font/u_vip12.png",
		[13] = "UI/MainScene/Font/u_vip13.png",
		[14] = "UI/MainScene/Font/u_vip14.png",
		[15] = "UI/MainScene/Font/u_vip15.png"
	},
	ALLIANCE_OPEN_LEVEL = 12,
	ELITEMAP_OPEN_LEVEL = 30,
	--主页面广播持续时间--单位毫秒，1秒为1000
	BroadcastLastTime = 60000,
	RechargeItemFrame = {
		[2] = "UI/MainScene/UI/u_Icobg08.png",
		[3] = "UI/MainScene/UI/u_Icobg09.png",
		[4] = "UI/MainScene/UI/u_Icobg10.png",
		[5] = "UI/MainScene/UI/u_Icobg11.png",
		[6] = "UI/MainScene/UI/u_Icobg12.png"
	},
	BattleChatColor = {
		system 		  = "0 0 0",
		systemContent = "246 51 69",
		nameMan		  = "41 81 239",
		nameWoman	  = "214 44 247",
		content		  = "0 0 0"
	
	},
	WordExchangeConsume = {
		10002, 10003, 10004, 10005, 10006, 10007
	},
	WordExchangeReward = {	----[id] = {d = 天数, r = 奖励id}----
		[1]	= {d = 1, 	r = 16},
		[2]	= {d = 3, 	r = 17},
		[3]	= {d = 5, 	r = 18},
		[4]	= {d = 7, 	r = 19},
		[5]	= {d = 9, 	r = 20},
		[6]	= {d = 11, 	r = 21},
		[7]	= {d = 13, 	r = 22},
		[8]	= {d = 15, 	r = 23},
		[9]	= {d = 17, 	r = 24},
		[10]= {d = 19, 	r = 25},
		[11]= {d = 20, 	r = 26}
	},
	ChatMsgMaxSize = 50,				--世界、工会聊天最多存储msg大小
	MsgBoxMaxSize = 20,	
	CampAutoRegisterVip = 4, 			--阵营战自动挂机VIP限制
	WordRecycleRewardId = 15,			--"公测"字回收奖励id
	Part2GodlyAttr_1 = {
		1001, 1002, 1003, 1004, 1005,
		1006, 1007, 1008, 1009, 1010
	},
	Part2GodlyAttr_2 = {
		2001, 2104, 2006, 2008, 2009,
		2010, 2103, 2007, 2005, 2002
	},
	GodlyAttrPureNum = 21,		--神器属性>2100的为纯数值加成，而非百分比加成
	GodlyEquipAni = {
		First 	= "EquipAni01.ccbi",
		Second 	= "EquipAni02.ccbi",
		Double 	= "EquipAni03.ccbi"
	},
	SpineAvatar = {
		[1] = {"Spine/Man_Warrior", "Man_Warrior"},
		[2] = {"Spine/Man_Hunter", "Man_Hunter"},
		[3] = {"Spine/Man_Master", "Man_Master"},
		[4] = {"Spine/Woman_Warrior", "Woman_Warrior"},
		[5] = {"Spine/Woman_Hunter", "Woman_Hunter"},
		[6] = {"Spine/Woman_Master", "Woman_Master"}
	},
	SpineCCBI = {
		[1] = "HeroWarrior_Man.ccbi",
		[2] = "HeroHunter_Man.ccbi",
		[3] = "HeroMaster_Man.ccbi",
		[4] = "HeroWarrior_Woman.ccbi",
		[5] = "HeroHunter_Woman.ccbi",
		[6] = "HeroMaster_Woman.ccbi",
	},
	CampWar = {
		basicRep = 10,
		basicCoin = 30,
		reportMax = 30,		
		GapSecond = 3000
	},
	ShowSpineAvatar = true,
	CurrentOpenMaxMap = 45, -- 当前最大地图
	NewbieGuide = {
		FirstFight 		= "newbieGuide_firstFight",			--战斗	
		FirstFailBoss 	= "newbieGuide_firstFailBoss",		--打boss失败
		FirstSmelt		= "newbieGuide_firstSmelt",			--熔炼
		FirstSkill		= "newbieGuide_firstSkill",			--技能
		FirstFightBoss	= "newbieGuide_firstFightBoss"		--挑战boss		
	},
	Quality2AttrNum = {--白绿蓝紫橙　对应副属性个数
		0, 1, 2, 3, 4
	},
	MercenaryBackgroundPic = {
		[7] = "UI/Program/MercenaryWarrior_3.png",
		[8] = "UI/Program/MercenaryHunter_3.png",
		[9] = "UI/Program/MercenaryMaster_3.png"
	},
	MercenarySkillState = {
		forbidden = "UI/MainScene/UI/u_Equipmentbg11.png",
		canSelect = "UI/MainScene/Font/u_Font07.png"
	},
	MercenaryMaxStar = 10,
	BattleResultGap = 25, -- 战斗结果页面显示间隔
	ShowStepStar = 1
	
};

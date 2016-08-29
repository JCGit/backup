----------------------------------------------------------------------------------
--[[
	FILE:			GuildData.lua
	ENCODING:		UTF-8, no-bomb
	DESCRIPTION:	公会数据集合
	AUTHOR:			zhaolu
	CREATED:		2014-11-4

	you know: 		Guild == Alliance
--]]
----------------------------------------------------------------------------------
local GuildData = {}

-- 公会操作类型
GuildData.OperType = {
	ChangeLeader = 1,
	ChangeViceLeader = 2,
	JoinAlliance = 3,
	QuitAlliance = 4,
	DemoteViceLeader = 5,
}

GuildData.PositionType = {
	Leader = 2, 		--会长
	ViceLeader = 1,		--副会长
	Normal = 0, 		--普通成员
}

GuildData.allianceInfo = {
	-- 公会基本信息
	commonInfo = nil,
	-- 可加入公会列表
	joinList = nil,
}

-- 我的公会个人信息
GuildData.MyAllianceInfo = {}

-- 创建公会等级要求
GuildData.CreateAllianceOpenLevel = 18

-- 初始化公会商店、公会排名、boss排行、成员列表标志位
GuildData.shopInfoInited = false
GuildData.rankInfoInited = false
GuildData.bossRankInited = false
GuildData.memberInfoInited = false

GuildData.GuildPage = {}

GuildData.BossPage = {
	-- CONST
	FreeTimePerWeek = 2, 		-- boss 免费开启次数
	InspirePercent = 20, 		-- boss 鼓舞增加百分比
	InspireCost = 20, 			-- boss 鼓舞花费钻石
	-- end CONST

	-- BOSS STATE
	BossNotOpen = 1, 			-- boss状态：未开启
	BossCanJoin = 2, 			-- boss状态：已开启，可加入
	BossCanInspire = 3, 		-- boss状态：已加入，可鼓舞

	CDTimeKey = 'BossIntrusionCD',

	-- BOSS OPERATION
	BossOperOpen = 1, 			-- boss操作：开始boss
	BossOperJoin = 2, 			-- boss操作：加入boss战斗
	BossOperInspire = 3, 		-- boss操作：鼓舞

	bossBloodLeft = 0,

	bossJoinFlag = false,
	bossCfg = ConfigManager.getAllianceBossCfg(),
}

-- 公会排名
GuildData.rankingList = {}
-- 成员列表
GuildData.memberList = {}
-- 商店列表
GuildData.shopList = {}
-- 伤害排行
GuildData.harmRankList = {}

-- 魔兽元气
GuildData.vitalityCfg = {
	type = 10000,
	itemId = 2001,
	count = 1,
}

-- 计时请求数据
GuildData.enterPageTime = 0

-- 公会页面的container
GuildData.mainContainer = {}
GuildData.joinListContainer = {}
GuildData.allianceContainer = {}
GuildData.bossContainer = {}
GuildData.bossHitContainer = {}

-- now guild refresh page num
GuildData.nowRefreshPageNum = 1

return GuildData

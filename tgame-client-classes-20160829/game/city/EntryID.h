#ifndef EntryID_h__
#define EntryID_h__

typedef enum EntryID {
	ENTRY_UNKNOWN, // 未知
	ENTRY_ROLE, // 角色
	ENTRY_EQUIP_CULTURE, // 装备养成
	ENTRY_WEAPON_LIB, // 枪械库
	ENTRY_GROWTH, // 成长
	ENTRY_SOCIAL, // 社交
	ENTRY_CHALLENGE, // 挑战
	ENTRY_BUSINESS, // 交易
	ENTRY_RANK, // 排行榜
	ENTRY_SHOP, // 商城
	ENTRY_LOTTERY, // 夺宝奇兵
	ENTRY_WEAPON_BOOK, // 枪魂
	ENTRY_CHAT, // 聊天
	ENTRY_MAIL, // 邮件
	ENTRY_DAILY_TASK, // 日常任务
	ENTRY_SIGN_IN, // 签到
	ENTRY_BUY_COIN, // 兑换所
	ENTRY_DAILY_ACT, // 日常活动
	ENTRY_SETTINGS, // 设置
	ENTRY_REFIT, // 改装
	ENTRY_LIMITED_SHOP, // 限购商城
	ENTRY_ACTIVITIES, // 活动
	ENTRY_SECRET_CHAT, // 私聊
	ENTRY_DAILY_WAR, // 日常副本
	ENTRY_LEVEL_UP, // 提升等级
	ENTRY_WAR, // 普通副本
	ENTRY_BOSS_WAR, // 灭霸入侵
	ENTRY_GET_AWARD, // 领取奖励
	ENTRY_WAR_CHALLENGE, // 挑战副本
	ENTRY_FIRST_CHARGE_PKG, // 首充礼包
	ENTRY_VIP,	// vip
	ENTRY_ONLINE_AWARD, //在线礼包
	ENTRY_MORROW_AWARD, //次日礼包

	// 枪械库
	ENTRY_WEAPON_CULTURE = (ENTRY_WEAPON_LIB << 8) + 1, // 枪械培养
	ENTRY_WEAPON_ARRAY, // 战备

	// 成长
	ENTRY_EVOLUTION = (ENTRY_GROWTH << 8) + 1, // 进化
	ENTRY_ATTR_STRENGTHEN, // 机体强化
	ENTRY_GENE_STRENGTHEN, // 基因强化

	// 社交
	ENTRY_FRIEND = (ENTRY_SOCIAL << 8) + 1, // 好友
	ENTRY_GUILD, // 部落
	// 挑战
	ENTRY_ARENA = (ENTRY_CHALLENGE << 8) + 1, // 竞技场
	ENTRY_NOTHING_LAND, // 虚无之地
	ENTRY_ELITE_WAR, // 精英副本
	// 商会
	ENTRY_BLACK_MARKET = (ENTRY_BUSINESS << 8) + 1, // 黑市
	ENTRY_FRAGMENT, // 碎片

} ENTRY_ID;

#endif // EntryID_h__
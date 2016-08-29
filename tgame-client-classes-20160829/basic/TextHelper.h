#ifndef __TEXT_HELPER_H__
#define __TEXT_HELPER_H__

#include "TextCfgMaster.h"
#include "vlib/base/CVSingleton.h"

enum enumBaseTextConsts
{
    EQUIP_REFINE_PANEL   = 100, ///< 装备强化面板
    WEAPON_PANEL = 200,
    TEXT_GUILD_BASE = 300, // 部落
	ENDLESS_PANEL = 400,		//无尽模式中面板
	ATTR_STRENGTHEN_BASE = 500, // 属性培养
	SWEEP_INFO_BASE = 600,		//扫荡
	ARENA_BASE = 700,			//竞技场
	VIP_BASE = 800, // VIP
	QIANGHUN_BASE = 900,		//枪魂
	COMMON_BASE = 1100, // 通用
	EVOLUTION_BASE = 1200, // 进化
	MODULE_OPEN_BASE = 1300, // 功能开放
	FIGHT_BASE = 1400, // 战斗
	BAG_BASE = 1500, // 背包
	TASK_BASE = 1600, // 任务
	DAILYT_ACT_BASE = 1700,	//日常活动
	LOTTERY_BASE = 1800, // 抽卡
	GOLD_TIP_BASE = 2000,	//消费提示
	ACT_TIP_BASE = 2200,	//活动提示
	WORLD_BOSS_BASE = 2400,	//世界boss战提示
	WEAPON_ARR_BASE = 2500,	//战备提示
	GET_AWARD_BASE = 2600, // 领取奖励
	GEN_TIP_BASE = 2700,	//基因强化
	WAR_MAP_BASE = 2800,//副本
}; //end enumBaseTextConsts


//装备强化面板
enum enumEquipRefineTextConsts
{
    //100
    EQUIP_REFINE_FULL_TIP   = EQUIP_REFINE_PANEL + 1, ///< 强化全部达到%d级\n可激活\n%s+%d
    EQUIP_REFINE_MAX_REFINE_LV   = EQUIP_REFINE_PANEL + 2, ///< 已经达到最高强化等级
    TEXT_MAIL_DEL_TIP   = EQUIP_REFINE_PANEL + 3, ///< 还有未领取的奖励，确认删除？
    TEXT_REMAIN_MANUAL_TIMES  = EQUIP_REFINE_PANEL + 4, ///< 剩余可领取体力次数：%d
    TEXT_ACT_DONE  = EQUIP_REFINE_PANEL + 5, ///< 活动结束
    TEXT_ACT_FAIL  = EQUIP_REFINE_PANEL + 6, ///< 活动失败
    TEXT_ADD_FRIEND_TIP = EQUIP_REFINE_PANEL + 7, ///< 发送加好友申请成功，等待对方确认。
    TEXT_SENDER = EQUIP_REFINE_PANEL + 8,   ///< 发送者
    TEXT_SUBJECT = EQUIP_REFINE_PANEL + 9,  ///< 主题
    TEXT_NO_MAIL = EQUIP_REFINE_PANEL + 10, ///< 暂时没有未处理邮件
    TEXT_REFINE_LV_INVALID = EQUIP_REFINE_PANEL + 11, ///< 只能将强化等级高的装备转移给低的装备
    TEXT_MAIL_MANUAL = EQUIP_REFINE_PANEL + 12, ///< 您的好友【%s】向您赠送了2点体力
    TEXT_REBOOT_TIP = EQUIP_REFINE_PANEL + 13, ///< 更新完成，点击按钮后将关闭游戏，请重新进入
    TEXT_ENTER_TIP = EQUIP_REFINE_PANEL + 14, ///< 点击按钮进入游戏
    TEXT_MAX_MANUAL_RECV = EQUIP_REFINE_PANEL + 15, ///< 每日可领取10次体力
    TEXT_NO_MANUAL_RECV = EQUIP_REFINE_PANEL + 16, ///< 没有可以领取的体力 
    TEXT_REFINE_MAX_LV = EQUIP_REFINE_PANEL + 17, ///< %d级装备最高可强化至+%d
    TEXT_WEAPON_ADV_LV_TIP = EQUIP_REFINE_PANEL + 18, ///< 等级达到%d级可进阶
    TEXT_REFINE_SUCCESS = EQUIP_REFINE_PANEL + 19, ///< 强化成功
    TEXT_REFINE_FAIL    = EQUIP_REFINE_PANEL + 20, ///< 强化失败
    TEXT_SEL_EQUIP_TIP  = EQUIP_REFINE_PANEL + 21, ///< 还没有选择装备
    TEXT_EAT_EQUIP_TIP  = EQUIP_REFINE_PANEL + 22, ///< 消耗材料中包含装备，是否继续升级？
    TEXT_EQUIP_ADV_TIP  = EQUIP_REFINE_PANEL + 23, ///< 升至满级后才可进阶
    TEXT_EQUIP_MIX_FULL_TIP  = EQUIP_REFINE_PANEL + 24, ///< 已达到最高融合等级
    TEXT_EQUIP_MAT_NULL  = EQUIP_REFINE_PANEL + 25, ///< 没有可以使用的材料

}; //end enumEquipRefineTextConsts

//武器面板
enum enumWeaponTextConsts
{
	WEAPON_NO_ENOUGH = WEAPON_PANEL + 1,				//材料不足
	WEAPON_NO_LEVEL  = WEAPON_PANEL + 2,				//等级不足
	WEAPON_GET_EXP  = WEAPON_PANEL + 3,					//获得经验
	WEAPON_OFFER_EXP  = WEAPON_PANEL + 4,				//提供经验
	WEAPON_CHOOSE_WEAPON  = WEAPON_PANEL + 5,			//选择枪械
	WEAPON_FULL_LEVEL  = WEAPON_PANEL + 6,				//等级已满
	WEAPON_CHOOSE_MATERIAL  = WEAPON_PANEL + 7,			//选择材料

	WEAPON_ATTR_TYPE_NONE = WEAPON_PANEL + 8,			//无属性
	WEAPON_ATTR_TYPE_ICE = WEAPON_PANEL + 9,			//冰攻
	WEAPON_ATTR_TYPE_FIRE = WEAPON_PANEL + 10,			//火攻
	WEAPON_ATTR_TYPE_DRUG = WEAPON_PANEL + 11,			//毒攻
	WEAPON_ATTR_TYPE_BIO = WEAPON_PANEL + 12,			//生化攻
	WEAPON_ATTR_TYPE_THUNDER  = WEAPON_PANEL + 13,		//雷攻

	WEAPON_STAR_UP_DESC = WEAPON_PANEL + 14,			//%d星枪械进阶至%d星枪械所需原料

	WEAPON_REFIT_DESC = WEAPON_PANEL + 15,				//%s改装为%s所需零件
	WEAPON_REFIT_TITLE = WEAPON_PANEL + 16,				//改装手册
	WEAPON_REFIT_ENOUGH = WEAPON_PANEL + 17,			//改装已满
    WEAPON_STAR_UP_ENOUGH = WEAPON_PANEL + 18,			//星级已满
    WEAPON_BAG_FULL = WEAPON_PANEL + 19,			    //枪械库已满
	WEAPON_LEVEL_UP_SELECTED_WEAPON = WEAPON_PANEL + 20, // 升级后，作为材料的枪械将会消失，是否继续？
	WEAPON_NO_ENOUGH_GOTOBUY = WEAPON_PANEL + 21, //材料不足，前往黑市和限购商店购买
}; //end enumEquipRefineTextConsts

enum enumSweepTextConsts
{
	SWEEP_ROUND_DESC = SWEEP_INFO_BASE + 1,				//第几轮
	SWEEP_WAR = SWEEP_INFO_BASE + 2,				//第%d关
};

enum enumArenaTextConsts
{
	ARENA_WIN_TXT = ARENA_BASE + 1,						//竞技场胜利描述
	ARENA_LOSE_TXT = ARENA_BASE + 2,					//竞技场失败描述
	ARENA_MATCH_COST = ARENA_BASE + 3,					//消耗:%d联邦币
	ARENA_FREE_MATCH = ARENA_BASE + 4,					//免费匹配对手
	COIN_DESC_TXT = ARENA_BASE + 5,						//联邦币的描述
	GOLD_DESC_TXT = ARENA_BASE + 6,						//星钻的描述
	ARENA_COIN_DESC_TXT = ARENA_BASE + 7,				//竞技场功勋的描述
	ARENA_MATCH_FAIL_TXT = ARENA_BASE + 8,				//匹配失败
	ARENA_COIN_NOT_ENOUGH_TXT = ARENA_BASE + 9,			//功勋不足
    ARENA_FIGHT_NUM_NOT_ENOUGH_TXT = ARENA_BASE + 10,	//挑战次数为零，请购买挑战次数
    ARENA_EXCHANGE_NUM_TIP = ARENA_BASE + 11,	        //当前拥有：%d
    ARENA_RD_WIN        = ARENA_BASE + 12,				//您击败【%s】取得胜利，获得联邦币%d，积分%s。
    ARENA_RD_LOSE       = ARENA_BASE + 13,				//您挑战【%s】失败，获得联邦币%d，积分%s。
    ARENA_RD_LOSE_OTHER = ARENA_BASE + 14,				//您被【%s】击败，损失联邦币%d，积分%s。
    ARENA_RD_LOSE_BACK  = ARENA_BASE + 15,				//您受到【%s】的反击，损失联邦币%d，积分%s。
    ARENA_RD_WIN_BACK   = ARENA_BASE + 16,				//您受到【%s】的反击，获得了胜利。
	ARENA_BUY_NUM_NOT_ENOUGH = ARENA_BASE + 17,			//购买次数已用完
	ARENA_REFRESH_NUM_NOT_ENOUGH = ARENA_BASE + 18,		//刷新次数已用完
	ARENA_FIGHT_CD_TXT = ARENA_BASE + 19,				//CD冷却中
};

// 部落
enum enumGuildTextConsts {
	TEXT_GUILD_CREATE_COST = TEXT_GUILD_BASE + 1, // 消耗%d万星币，可创建部落！
	TEXT_GUILD_NULL_GUILD_NAME, // 部落名称不能为空！
	TEXT_GUILD_INVALID_GUILD_NAME, // 请输入有效的部落名称！
	TEXT_GUILD_IMPEACH_CHAIRMAN, // 确定要弹劾大酋长吗？
	TEXT_GUILD_LEAVE_GUILD, // 确定要离开部落吗？
	TEXT_GUILD_DISMISS_GUILD, // 确定要解散部落吗？
    TEXT_GUILD_HAS_IMPEACHED, // 大酋长正在被弹劾中
    TEXT_GUILD_NOTIFY_JOIN,     // 恭喜，您已经被批准加入部落 8
    TEXT_GUILD_NOTIFY_DISMISS,  // 您所在的部落已被解散 9
    TEXT_GUILD_NOTIFY_KICK,     // 您已被移出部落 10
    TEXT_GUILD_IMPEACH_TIME,     // 距离弹劾成功还有 11
    TEXT_GUILD_IMPEACH_INVALID_RULE,     // 大酋长离线时间超过5天可发起弹劾 12
	TEXT_GUILD_APPLIED_LIMITED, // 已经超出了可申请上限！
};

// 属性培养
enum enumAttrStrengthen {
	TEXT_ATTR_STEP_1 = ATTR_STRENGTHEN_BASE + 1, // 一阶
	TEXT_ATTR_STEP_2, // 二阶
	TEXT_ATTR_STEP_3, // 三阶
	TEXT_ATTR_STEP_4, // 四阶
	TEXT_ATTR_STEP_5, // 五阶
	TEXT_ATTR_STEP_6, // 六阶
	TEXT_ATTR_STEP_7, // 七阶
	TEXT_ATTR_STEP_8, // 八阶
	TEXT_ATTR_STEP_9, // 九阶
	TEXT_ATTR_STEP_10, // 十阶
};

// VIP
enum enumVIPTextConsts {
	TEXT_VIP_PRIVILEGE = VIP_BASE + 1, // VIP%d特权
    TEXT_VIP_GIFT = VIP_BASE + 2, // VIP%d礼包
    TEXT_VIP_NO_BUY_COIN_NUM = VIP_BASE + 3, // 今天的购买次数已经用完
    TEXT_VIP_BUY_COIN_NUM_DESC = VIP_BASE + 4,	//（今日可用%s）
    TEXT_SIGN_IN_DAY = VIP_BASE + 5,	//签到%d天
    TEXT_SIGN_IN_REFRESH_TIME = VIP_BASE + 6,	//每日%d点刷新
	TEXT_VIP_AUTO_FIGHT = VIP_BASE + 7,			//成为VIP可自动战斗
	TEXT_VIP_EXTRA_AWARD = VIP_BASE + 8,		//额外赠送送
};

enum enumEndlessTextConsts {
	ENDLESS_RAND_GEN = ENDLESS_PANEL + 1,				//随机强化
	ENDLESS_ADD_ATK_GEN = ENDLESS_PANEL + 2,			//提高攻击
	ENDLESS_ADD_HP_DEF_GEN = ENDLESS_PANEL + 3,			//提高防御和生命力
	ENDLESS_ADD_SCORE_GEN = ENDLESS_PANEL + 4,			//积分增加
	ENDLESS_ADD_COIN_GEN = ENDLESS_PANEL + 5,			//星币增加
	ENDLESS_RUSH_GEN = ENDLESS_PANEL + 6,			    //乘坐幻影战机冲刺%d层
};

enum enumQianghunTextConsts {
	TEXT_QIANGHUN_LEVEL = QIANGHUN_BASE + 1,			//第%d层
	TEXT_QIANGHUN_FEW = QIANGHUN_BASE + 2,				//稀少
	TEXT_QIANGHUN_BIT = QIANGHUN_BASE + 3,				//少量
	TEXT_QIANGHUN_MORE = QIANGHUN_BASE + 4,				//较多
	TEXT_QIANGHUN_MASS = QIANGHUN_BASE + 5,				//海量
	TEXT_QIANGHUN_GET_LEVEL = QIANGHUN_BASE + 6,		//第%d层获得：
	TEXT_QIANGHUN_NEXT_LEV_TIP = QIANGHUN_BASE + 7,		//您还有未抽的魂卡，确定要继续前往下一层吗？
	TEXT_QIANGHUN_KEY_TIP = QIANGHUN_BASE + 8,			//您已获得【枪魂秘钥】，进入下一层按钮已经开启……
};

// 通用
enum enumCommonTextConsts {
	TEXT_NOTHING = COMMON_BASE + 1, // 没有了！
	TEXT_LEVEL_UNREACHED, // 您的等级不够！
	TEXT_COIN_NOT_ENOUGH, // 您的联邦币不足！
	TEXT_GOLD_NOT_ENOUGH, // 您的星钻不足！
	TEXT_CONFIRM_GOLD_COST, // 是否花费%d星钻？
    TEXT_ITEM_NOT_ENOUGH, // 您的[%s]不足！
    TEXT_NET_CLOSE, // 网络连接断开
    TEXT_CONFIRM_COIN_COST, // 需要花费%d联邦币，是否继续？
    TEXT_NET_RECONNECT, // 重新连接
    TEXT_NET_RECONNECT_DONE, // 重连成功
	TEXT_TODAY, // 今天
	TEXT_A_FEW_DAYS_AGO, // 几天以前
    TEXT_A_MONTH_AGO, // 一个月以前
    TEXT_SDK_LOGIN_FAIL, // 登陆失败
    TEXT_SDK_BUY_SUCCESS, // 支付成功，请等待到账
    TEXT_SDK_BUY_FAIL, // 支付失败
	TEXT_CONGRATULATIONS_TO_GET, // 恭喜您获得 %s
	TEXT_SDK_BIND_SUCCESS_TXT,	//绑定账号成功
	TEXT_SDK_LOGOUT_SUCCESS_TXT,	//登出成功，请重新登陆！
	TEXT_SDK_SHARE_SUCCESS,		//分享成功
	TEXT_SDK_SHARE_FAILED,		//分享失败
	TEXT_SVR_NOT_SERVICES,		//服务器未开服
	TEXT_SVR_STOP_SERVICES,		//停服维护
	TEXT_SVR_RECOMMEND,			//新服
	TEXT_SVR_CROWED,			//拥挤
	TEXT_SVR_FULL,				//爆满
};

// 进化
enum enumEvolutionTextConsts {
	TEXT_EVOLUTION_STONE_NOT_ENOUGH = EVOLUTION_BASE + 1, // 您的进化石不足！
	TEXT_EVOLUTION_LEVLE_LMITED_DESC, // 等级达到%d级才能升级!
};

// 功能开放
enum enumModuleOpenTextConsts {
	TEXT_MODULE_OPEN_EVOLUTION_SKILL = MODULE_OPEN_BASE + 1, // 恭喜您！开启了[进化技能]！
	TEXT_MODULE_OPEN_BAG, // 恭喜您！开启了[背包]！
	TEXT_MODULE_OPEN_EQUIP_LEVELUP, // 恭喜您！开启了[装备强化]！
	TEXT_MODULE_OPEN_LOTTERY, // 恭喜您！开启了[探秘]！
	TEXT_MODULE_OPEN_WEAPON_LEVELUP, // 恭喜您！开启了[枪械强化]！
	TEXT_MODULE_OPEN_WEAPON_STARSTEP, // 恭喜您！开启了[枪械升星]！
	TEXT_MODULE_OPEN_DAILY_TASK, // 恭喜您！开启了[日常任务]！
	TEXT_MODULE_OPEN_RANK, // 恭喜您！开启了[排行榜]！
	TEXT_MODULE_OPEN_FRIENDS, // 恭喜您！开启了[好友]！
	TEXT_MODULE_OPEN_GUILD, // 恭喜您！开启了[部落]！
	TEXT_MODULE_OPEN_EVOLUTION, // 恭喜您！开启了[进化]！
	TEXT_MODULE_OPEN_ATTR_STRENGTHEN, // 恭喜您！开启了[属性攻抗]！
	TEXT_MODULE_OPEN_ARENA, // 恭喜您！开启了[竞技场]！
	TEXT_MODULE_OPEN_INFINITE, // 恭喜您！开启了[无尽模式]！
	TEXT_MODULE_OPEN_MAIL, // 恭喜您！开启了[邮件]！
	TEXT_MODULE_OPEN_REFIT, // 恭喜您！开启了[改装]！
	TEXT_MODULE_NOOPEN, //该功能尚未开放！
};

// 战斗
enum enumFightTextConsts {
    TEXT_NOT_RECV_THE_TASK = FIGHT_BASE + 1,    // 还未接取该任务！
    TEXT_BOSS_WAR_PASS_TIP = FIGHT_BASE + 2,    // 击杀BOSS【%s】
    TEXT_NORMAL_WAR_PASS_TIP = FIGHT_BASE + 3,  // 清除所有怪物
    TEXT_WAR_PASS_TIP_KILL_NUM = FIGHT_BASE + 4,  // 清除【%d】个敌人
    TEXT_WAR_PASS_TIP_KILL_NUM_SPEC = FIGHT_BASE + 5,  // 清除【%d】个【%s】
    TEXT_WAR_PASS_TIP_TIME = FIGHT_BASE + 6,  // 坚持战斗【%s】
	TEXT_WAR_HAVE_NO_FIGHT_TIMES = FIGHT_BASE + 7,	// 关卡次数已用完
	TEXT_NIHILITY_USE_ITEM = FIGHT_BASE + 8,		// 使用载具
	TEXT_NIHILITY_FREE_TXT = FIGHT_BASE + 9,		// 免费%s次
	TEXT_SURE_TO_REFRESH = FIGHT_BASE + 10,		// 确认要刷新吗？
	TEXT_SWEEP_STOP = FIGHT_BASE + 11,		// 背包已满，停止扫荡。
    TEXT_NIHILITY_USE_ITEM_TIP = FIGHT_BASE + 12,	//请选择载具
    TEXT_WAR_NOT_CLEAR_TIP = FIGHT_BASE + 13,	//该关卡尚未通关
	TEXT_NIHILITY_STAGE_DESC = FIGHT_BASE + 14, //当前第%d层
	TEXT_RELIVE_COST_ITEM_TIP = FIGHT_BASE + 15,	//（剩余%d）复活？
	TEXT_FILL_BULLET_COST_ITEM_TIP = FIGHT_BASE + 16,	//（剩余%d）补满弹药？
	TEXT_BULLET_OVER_TIP = FIGHT_BASE + 17,			//弹药用尽，请换枪
	TEXT_AUTO_FILL_BULLET_TIP = FIGHT_BASE + 18,	//弹药补满，弹药箱-1
	TEXT_RELIVE_BTN_GOLD_LABEL_TIP = FIGHT_BASE + 19,	//星钻复活
	TEXT_RELIVE_BTN_LABEL_TIP = FIGHT_BASE + 20,		//复   活
	TEXT_STAR_CNT_UNREACHED = FIGHT_BASE + 21,		// 收集%d星领取宝箱。
	TEXT_FULL_STAR_CONDITION = FIGHT_BASE + 22,  //%d秒内通关
};

// 背包
enum enumBagTextConsts {
	TEXT_SALE_ITEM = BAG_BASE + 1, // 确定要卖出吗？
    TEXT_BAG_FULL_OP_TIP = BAG_BASE + 2, // 背包已满，请进行整理
    TEXT_SHOP_BUY_DONE = BAG_BASE + 3, // 购买成功
	TEXT_BAG_NOT_ENOUGH = BAG_BASE + 4, // 背包剩余空间不足，请进行整理
};

// 任务
enum enumTaskTextConsts {
	TEXT_TASK_LEVEL_UNREACHED = TASK_BASE + 1, // 等级达到%d级可接取此任务？
	TEXT_MONTH_CARD, // 月卡
	TEXT_BUY_MONTH_CARD, // 购买月卡
	TEXT_MONTH_CARD_AWARD_DESC, // 购买月卡每日可以获得%d星钻
	TEXT_NONE_VALID_TASK, // 暂无可接任务
};

//日常活动
enum enumDailyActTextConsts{
	TEXT_DAILY_ACT_PREPARE_STAT = DAILYT_ACT_BASE + 1,	//未开始
	TEXT_DAILY_ACT_START_STAT = DAILYT_ACT_BASE + 2,	//已开始
	TEXT_DAILY_ACT_END_STAT = DAILYT_ACT_BASE + 3,		//已结束
	TEXT_DAILY_ACT_JOINED_STAT = DAILYT_ACT_BASE + 4,	//已参与
	TEXT_DAILY_FORCE_QUIT = DAILYT_ACT_BASE + 5,		//是否退出活动？
	TEXT_DAILY_ACT_ENTER = DAILYT_ACT_BASE + 6,			//进  入
	TEXT_DAILY_MANUAL_CAN_GET = DAILYT_ACT_BASE + 7,	//可领取
	TEXT_DAILY_MANUAL_GET = DAILYT_ACT_BASE + 8,		//领  取
	TEXT_DAILY_MANUAL_GETED = DAILYT_ACT_BASE + 9,		//已领取
};

//抽卡
enum enumLotteryTextConsts{
	TEXT_AFTER_TIMES_CERTAINLY_LOTTERY = LOTTERY_BASE + 1,	// 再寻宝%d次后，下次必得四星装备
	TEXT_CERTAINLY_LOTTERY,	// 下次必得四星装备
	TEXT_NEED_GOLD, // %d星钻
	TEXT_VIP_LOTTERY_INTRO, // VIP%d将获得每日开启%d次至尊宝箱的特权
	TEXT_FIRST_CHARGE_PKG, // 首充礼包
};


enum enumGoldCostConsts{
	TEXT_COMMON_COST_TIP = GOLD_TIP_BASE + 1,		//通用消费提示:是否消耗%d星钻购买
	TEXT_MANUAL_COST_TIP = GOLD_TIP_BASE + 2,		//购买体力，是否消耗%d星钻购买\n%d体力，剩余可购买次数%d次
	TEXT_REFRESH_COST_TIP = GOLD_TIP_BASE + 3,		//刷新商店，是否消耗%d星钻刷新
    TEXT_BUY_ARENA_COST_TIP = GOLD_TIP_BASE + 4,	//购买次数，是否消耗%d星钻购买挑战次数，剩余可购买%d次
    TEXT_RMB_FIRST_GIFT = GOLD_TIP_BASE + 5,		//首次赠送%d星钻
    TEXT_RMB_EXTRA_GIFT = GOLD_TIP_BASE + 6,		//额外赠送%d星钻
    TEXT_MONTH_CARD_TIP = GOLD_TIP_BASE + 7,		//%d天内每天可领取%d星钻
	TEXT_MANUAL_BUY_TIP = GOLD_TIP_BASE + 8,		//体力不足，是否购买？
	TEXT_MANUAL_LOW_TIP = GOLD_TIP_BASE + 9,		//体力不足，
	TEXT_LEVEL_LOW_TIMES_TIP = GOLD_TIP_BASE + 10,	//关卡次数不足
	TEXT_CLEAN_CD_TIP = GOLD_TIP_BASE + 11,			//是否消耗%d星钻清除CD
	TEXT_RESTRICT_BUY_TIP = GOLD_TIP_BASE + 12,		//今日已购买，无法重复购买
	TEXT_RESTRICT_SELL_OUT_TIP = GOLD_TIP_BASE + 13,//今日已售尽，请下次再来
	TEXT_RELIVE_COST_GOLD_TIP = GOLD_TIP_BASE + 14, //是否消耗%d星钻复活？
	TEXT_FILL_BULLET_COST_GOLD_TIP = GOLD_TIP_BASE + 15,	//是否消耗%d星钻购买弹药？
	TEXT_DIFF_GOLD_PROMPT = GOLD_TIP_BASE + 16,		// 此次花费还差%d星钻
	TEXT_CHARGE_GOLD_NUM_TIP = GOLD_TIP_BASE + 17,	//每充值%d钻石即可
	TEXT_BUY_WAR_FIGHT_CNT_TIP = GOLD_TIP_BASE + 18,//是否消耗%d星钻购买10次挑战次数？
	TEXT_BUY_EXP_ITEM = GOLD_TIP_BASE + 19,// %s，仅需%d元，是否继续培养？
	TEXT_RMB_BUY = GOLD_TIP_BASE + 20,// ￥%d 购买
	TEXT_BUY_ITEM_GOLD_TIP = GOLD_TIP_BASE + 21,	//是否消耗%d星钻购买%d个？
	TEXT_SWEEPCARD_LOW_TIP = GOLD_TIP_BASE + 22,//扫荡卡不足
};

enum enumActTipTextConsts{
	TEXT_ACT_CDK_CLAIM_TIP = ACT_TIP_BASE + 1,		//兑换成功
	TEXT_ACT_FIRST_CHARGE_TIP,	//首冲礼包领取成功
	TEXT_ACT_SPRING_DAY_1,	// 初一
	TEXT_ACT_SPRING_DAY_2,	// 初二
	TEXT_ACT_SPRING_DAY_3,	// 初三
	TEXT_ACT_SPRING_DAY_4,	// 初四
	TEXT_ACT_SPRING_DAY_5,	// 初五
	TEXT_ACT_SPRING_DAY_6,	// 初六
	TEXT_ACT_SPRING_DAY_7,	// 初七
	TEXT_ACT_SPRING_DAY_8,	// 初八
	TEXT_ACT_SPRING_DAY_9,	// 初九
	TEXT_ACT_SPRING_DAY_10,	// 初十
	TEXT_ACT_SPRING_DAY_11,	// 十一
	TEXT_ACT_SPRING_DAY_12,	// 十二
	TEXT_ACT_TIME_LAST_FORMAT,	//充值活动时间XXX-XXX
	TEXT_ACT_CHARGE_LEVEL,	//充值XX星钻
	TEXT_ACT_TOTAL_CHARGE,	// 当前充值
	TEXT_ACT_TOTAL_LOGINED,	// 当前累计登陆天数
	TEXT_ACT_TIME_FOREVER,	// 活动时间-  永久
	TEXT_ACT_REWARD_BEST,	// 终极礼包
	TEXT_ACT_LOGIN_DAYS,	// 登陆第几天
	TEXT_ACT_TOTAL_FIGHT,	// 当前战斗力
	TEXT_ACT_FIGHTING_LEVEL,// 战力等级
	TEXT_ACT_CUR_LEVEL,		// 当前等级
	TEXT_ACT_LEVEL,			// 等级
	TEXT_ACT_REWARD_LAST,	// 礼包剩余描述
	TEXT_ACT_GET_FAILED,	// 领取奖励失败，数量不足
	TEXT_ACT_VIP_GIFT,//VIP等级
	TEXT_ACT_VIP_NOW,//当前VIP等级
	TEXT_ACT_DAILY_CONSUME,//今日累计消费
	TEXT_ACT_SUM_CONSUME,//当前消费金额
	TEXT_ACT_NEED_GOLD,//星钻
	TEXT_ONLINE_AWARD, //在线礼包
	TEXT_NEXTDAY_AWARD, //次日登录礼包
};

enum enumWorldBossWarTextConsts{
	TEXT_WORLD_BOSS_ENTRY_TIP = WORLD_BOSS_BASE + 1,			//%s进入战场
	TEXT_WORLD_BOSS_DAMAGE_TIP = WORLD_BOSS_BASE + 2,			//%s对BOSS造成%d伤害，碉堡啦
	TEXT_WORLD_BOSS_SKILL_DAMAGE_TIP = WORLD_BOSS_BASE + 3,		//%s使用%s对BOSS造成%d伤害，狂拽酷炫吊炸天！
	TEXT_WORLD_BOSS_RELIVE_TIP = WORLD_BOSS_BASE + 4,			//是否消耗%d星钻复活？
	TEXT_WORLD_BOSS_TISHEN_TIP = WORLD_BOSS_BASE + 5,			//是否使用替身？BOSS战开始时自动扣除%d星钻！
};

enum enumWeaponArrTextConsts{
	TEXT_WEAPON_ARR_ADD_TIP = WEAPON_ARR_BASE + 1,				//战备加成：秒伤提升%d%，生命提升%d%，防御力提升%d% ！
};

enum enumGetAwardTextConsts {
	TEXT_MONTH_CARD_AWARD = GET_AWARD_BASE + 1, // 月卡每日奖励
	TEXT_VIP_DAILY_AWARD, // VIP每日奖励
};

enum enumGenTextConsts {
	TEXT_GEN_UP_LIMIT = GEN_TIP_BASE + 1,						//(已满)
	TEXT_GEN_DOWN_LIMIT = GEN_TIP_BASE + 2,						//(已达下限)
	TEXT_GEN_COST_GOLD_TIP = GEN_TIP_BASE + 3,					//是否消耗%d星钻基因强化？
};

enum enumWarMapTextConsts {
	TEXT_WAR_ELITE_CHAPTER = WAR_MAP_BASE + 1,      //精英副本章节
	TEXT_WAR_ELITE_NO_OPEN = WAR_MAP_BASE +2,    //该章节尚未开放

};

#define NEW_LINE "\n"
inline const char* TextStr(const int iKey)
{
    return TEXT_CFG_MASTER->GetCstr(iKey); 
}



#endif //__TEXT_HELPER_H__



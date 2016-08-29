#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#define MIN_INNER_CMD  5000
#define MAX_INNER_CMD  10000

#define HERO_INIT_LEVEL 1
#define MAX_EQUIP_POS 4 	//身上装备的数量上?
#define MAX_WEAR_ARMOR_NUM 2 //最多带2个护甲
#define MAX_ROLE_NUM 3 //最多建3个角色
#define MAX_NAME_HASH_KEY_LEN 27		//名字hash key最大长度,格式为：名字.sXXX
#define MAX_HEROSKILL_CNT 5 	//武将技能格的最大数量
#define HERO_DEFAULT_SKILL_NUM 2 //武将的默认技能数量，攻击和待命
#define MAX_NAME_LEN 22		//名字最大长度
#define MAX_ACHIEVE_CNT	10	//成就的最大数量
#define MAX_PLAYERSKILL_CNT 30	//君主技能的最大数量
#define MAX_SUBHERO_CNT 9	        //  副将的最带携带数量
#define MAX_HEROONDUTY_CNT 8//出征武将的最大数量
#define MAX_EQUIP_EXTATTR_CNT 5 //装备附加属性的最大数量
#define MAX_PRO_AI_PER_UNIT 11
#define MAX_PAS_AI_PER_UNIT 11
#define MAX_PAS_AI_ENTRIES 10000
#define MAX_PRO_AI_ENTRIES 10000
#define MAX_CELL_CNT 144 //背包格上限，玩家看到的上限只有(MAX_CELL_CNT-SYS_BAG_CELL_CNT)个
#define SYS_BAG_CELL_CNT 10 //留给系统使用的背包格子数,玩家看到可用格子数是DEFAULT_CELL_CNT，还有SYS_BAG_CELL_CNT个空间给奖励用
#define DEFAULT_CELL_CNT 72 //默认背包的开格数,玩家看到的是（DEFAULT_CELL_CNT - SYS_BAG_CELL_CNT）个
#define MAX_WARPOINT_CNT 50//每张关卡图中的关卡最大数量
#define MAX_WAR_MAP_CNT 500//关卡图的最大数量
#define MAX_WARMAP_DROP_CNT 10
#define MAX_REFINE_LV 150 //最大强化等级
#define MAX_PLAYER_LV 150 //角色等级上限
#define MAX_LOT_ITEM_NUM 10 //寻宝额外奖励物品上限
//体力
#define MAX_MANUAL_VAL 100 //最大体力值
#define MANUAL_REC_TIME (60*5) //恢复时间
#define MANUAL_REC_POINT 1  //恢复点
#define MAX_BUY_MANUAL_CNT 10

#define DEC_EQUIP_REFINE_LV 5 //升级升质是导致强化等级的降低值
#define MAX_HOLE_CNT 5 //孔上限
#define MAX_BUFF_COUNT 12
#define MAX_DEBUFFEFF_COUNT 12
#define MAX_DROP_ELEMENT 3//掉落物品的种类

#define MAX_DAILYTASK_CNT 10

#define SECOND_PER_DAY (24*60*60)
#define MAX_GOLD 2000000000
#define MAX_OPEN_ID_LEN 64
#define MAX_SOURCE_LEN 64   //外部来源长度
#define MAX_OPEN_KEY_LEN 512
#define MAX_PF_LEN 64

//////////////////////////////////////////////////////////////////////////枪械
#define MAX_WEAPON_CNT 100	//最大枪械数量

#define MAX_WARMAP_STAR 3

#define MAX_PACKAGE_AWARD_CNT 10

/////////////好友/////////////
#define MAX_FRIEND_CNT 100 //好友个数上限
#define MAX_FRIEND_ADD_REQ_CNT 10 //添加好友申请上限
#define MAX_SEND_MANUAL_CNT 10 //每天最多送10次体力
#define MAX_USE_RECV_MANUAL_CNT 10 //每天最多领取10次好友送的体力
#define MAX_PROPOSE_FRIEND_NUM 6 //好友推荐面板一次推荐6个

enum enumSystemMaxData
{
    EMIN_VALID_UIN  =   10000,  ///< 最小有效uin
};

enum enumUnitType
{
    EUNIT_TYPE_MIN = 0,
    EUNIT_SUBHERO   = EUNIT_TYPE_MIN + 0,          ///< 武将
    EUNIT_MONSTER       = EUNIT_TYPE_MIN + 1,          ///< 怪物
    EUNIT_EQUIPMENT = EUNIT_TYPE_MIN + 2, ///< 装备
    EUNIT_WEAPON = EUNIT_TYPE_MIN + 3, ///< 枪械
    EUNIT_MAIL = EUNIT_TYPE_MIN + 4,    ///< 邮件
    EUNIT_GUILD = EUNIT_TYPE_MIN + 5,   ///< 家族
    EUNIT_TEAM = EUNIT_TYPE_MIN + 6,
    EUNIT_TYPE_MAX = EUNIT_TYPE_MIN + 7,
};

////////////////////////////////////////////////// 炼魂
#define MAX_SUBHERO_SPIRIT_CELL_NUM 8
#define MAX_SPIRIT_CELL_BAG_NUM 20
#define MAX_SPIRIT_CELL_IN_SHOP 30
#define MAX_SPIRIT_HOLE_NUM 5
#define MAX_ATTR_TYPE_PER_SPIRIT 2

////////////////////////////////////////////////// 神秘商人
#define MAX_PLAYER_SECRET_STORE_CELL_NUM 6

////////////////////////////////////////////////// 押镖
#define MAX_PLAYER_CARGO_ROB_TIME 2

////////////////////////////////////////////////// 组队系统
#define MAX_TEAM_MATE_NUM 4

////////////////////////////////////////////////// 多人副本
#define MAX_MULTIINSTANCE_NUM 20
//////////////////////////////////////////////////  用户锁状态
#define MAX_PLAYER_LOCK_TYPE 10

////////////////////////////////////////////////// 用户通关记录
#define MAX_INSTANCE_RECORD_UIN_NUM 10

#define MAX_PLANT_FIELD_NUM 9
#define MAX_PLAYER_CREATION_FLAG 16

#define MAX_PLAYER_ONLINE (1000+100)

#define MAX_PACKAGE_ITEM 10

#define MAX_REG_PLAYER_CNT 25000

/////////////////////////////////////////////////////// 签到活动
#define MAX_DAILY_CHECK_IN_SLOT 32
#define MAX_DAILY_CHECK_IN_AWARD_SLOT 8

/////////////////////////////////////////////////////// 新手黄金礼包
#define MAX_SIGNIN_PKG_NUM 3

////////////////////////////////////////////////// 活动数据
#define MAX_ACTIVITY_FETCH_NUM_PER_TURN 60
#define MAX_ACTIVITY_INDIANA_JONES_RECORD_NUM  3

///////////////////////////////////////////////////////////  活动svr
#define MAX_ACTPLAYER_INFO_CELL 20
#define MAX_PLATVIP_INFO_ARR_LEN 2


#define MAX_TOKEN_LEN 256

#define MAX_CLAIM_ONLY_ACT_NUM 16

#define MAX_CONTINOUSDAY_LEN 8

#define MAX_DAILY_INVITE_ITEMS 4

///////////////////////////////////////////////////////////  竞技场
#define MAX_ARENA_FREE_NUM 10				//每日竞技场免费次数
#define ARENA_WIN_ADD_SCORE 5				//获胜加的分数
#define ARENA_LOSE_DEC_SCORE 2				//失败扣的分数
#define ARENA_STORE_MAX_ITEMS 6				//竞技场兑换商店最多6个物品
#define ARENA_STORE_REFRESH_GOLD 100		//刷新兑换商店消耗Gold
#define MAX_ARENA_RECORD 20					//战斗记录最多20条
#endif

#ifndef __NOUN_HELPER_H__
#define __NOUN_HELPER_H__

#include "NounCfgMaster.h"


enum enumNounConsts
{
    EQUIP   = 1, ///< 装备
    ITEM    = 2, ///< 道具
    EXP     = 3, ///< 经验
    LEVEL   = 4, ///< 等级
    ATTR    = 5, ///< 属性
    COIN    = 6, ///< 铜钱
    GOLD    = 7, ///< 黄金
    COST    = 8, ///< 消耗
    UPVAL   = 9, ///< 上限
    MAXVAL  = 10, ///< 最大值
    WEAPON  = 11, ///< 武器
    RUNSPEED = 12, ///< 疾行速度
    HP      = 13, ///< 生命
    DEF     = 14, ///< 防御
    MOVESPEED = 15, ///< 移动速度
    MP      = 16, ///< 能量
    ATK     = 17, ///< 攻击
    RING    = 18, ///< 护符
    ARMOR   = 19, ///< 甲具
    CLOTH   = 20, ///< 披风
    SHOE    = 21, ///< 鞋子
    OPEN    = 22, ///< 开启
    OPENING = 23, ///< 开启中
    CLOSE   = 24, ///< 关闭
    CLOSING = 25, ///< 关闭中
    SEMICOLON = 26, ///< ：
    MULTIPLY = 27, ///< ×
    NOT_ENOUGH = 28, ///< 不足
    OK      = 29, ///< 确定
    CANCEL  = 30, ///< 取消
    HEARY_ARMOR_DEF_STR = 31, ///< 重甲
    LIGHT_ARMOR_DEF_STR = 32, ///< 轻甲
	NONE_ARMOR_DEF_STR = 33, ///< 无甲
    NONE = 34, ///< 无
    STR_ICE     = 35, ///< 冰
    STR_FLAME   = 36, ///< 火
    STR_POISON  = 37, ///< 毒
    STR_BIOTECH = 38, ///< 生化
    STR_LIGHTING= 39, ///< 闪电
    TXT_THING_TYPE_GEN = 40, ///< 基因
    TXT_THING_TYPE_MONTHCARD = 41, ///< 月卡
    TXT_THING_TYPE_DEBRIS = 42, ///< 碎片
    TXT_THING_TYPE_MANUAL = 43, ///< 体力
    TXT_TGAME_NAME = 44, ///< 特兰战地 
    TXT_TGAME_DESC_MONTHCARD = 45, ///< 激活后，30天内每日可获得130钻石。
    TXT_NOW_VERSION = 46, ///< 当前版本：

}; //end enumNounConsts


//neil[1000-1999]
enum enumNounConstsNeil {
    MONTH       = 1000, ///< 月 
    DAY         = 1001, ///< 天
    HOUR        = 1002, ///< 小时
    MINUTE      = 1003, ///< 分钟
    SECOND      = 1004, ///< 秒
    LEFT_TIME   = 1005, ///< 剩余时间
    SYSTEM      = 1006, ///< 系统
    NOUN_FREE   = 1007, ///< 免费
    NOUN_FREE2  = 1008, ///< 后免费
    NOUN_ADD    = 1009, ///< 添加
    NOUN_REFUSE = 1010, ///< 拒绝
    NOUN_DEL    = 1011, ///< 删除
    NOUN_CHAT1  = 1012, ///< 聊天
    NOUN_SEND_MANUAL    = 1013, ///< 赠送体力
    NOUN_CHECK_VER      = 1014, ///< 检查更新
    NOUN_DOWNLOAD       = 1015, ///< 下载资源
    NOUN_ENTER_GAME     = 1016, ///< 进入
    NOUN_REBOOT_GAME    = 1017, ///< 重启
    NOUN_DONE           = 1018, ///< 完成
    NOUN_EXTRACE_RES    = 1019, ///< 更新资源
    NOUN_BASE           = 1020, ///< 基础
    NOUN_REFINE         = 1021, ///< 强化
    NOUN_FRIEND_NUM     = 1022, ///< 好友数量
    NOUN_ACTIVATE       = 1023, ///< 已激活
    NOUN_RECV_MANUAL    = 1024, ///< 接收体力
    NOUN_OUTTIME        = 1025, ///< 后过期
};

//beck[2000-2999]
enum enumNounConstsBeck {
	WEAPON_TAB = 2000,					//枪械
	MATERIAL_TAB = 2001,				//资源
	CLICK_TIP = 2002,					//点你妹
	PLAYER_NAME_TITLE = 2003,			//玩家名
	POWER_TITLE = 2004,					//战力
	WARMAP_TITLE = 2005,				//关卡
	ENDLESS_TITLE = 2006,				//积分
	NO_RANK_TIP = 2007,					//暂无排名
	TIME_COUNT_TXT = 2008,				//倒计时
	RANK_AWARD_TXT = 2009,				//排行榜礼包
	PLAYER_POWER_TXT = 2010,			//个人战力
	PLAYER_ENDLESS_TXT = 2011,			//个人积分
	PLAYER_WARMAP_TXT = 2012,			//个人关卡
	LOW_MANUAL_TXT = 2013,				//体力不足
	COIN_NOT_ENOUGH_TXT = 2014,			//联邦币不足
	ENERGY_NOT_ENOUGH_TXT = 2015,		//能量不足
	DESC_STRING_TXT = 2016,				//描述
	WIN_STRING_TXT = 2017,				//胜
	FAIL_STRING_TXT = 2018,				//负
	YEAR_STRING_TXT = 2019,				//年
	DAY_STRING_TXT = 2020,				//日
	HOUR_STRING_TXT = 2021,				//时
	MINUTE_STRING_TXT = 2022,			//分
	ARENA_COIN_TXT = 2023,				//竞技场货币，功勋
	NOUN_STAGE_TXT = 2024,				//阶段
	NOUN_WEAPON_TXT = 2025,				//枪械
	NOUN_COLOR_WHITE = 2026,			//普通
	NOUN_COLOR_GREEN = 2027,			//绿色
	NOUN_COLOR_BLUE = 2028,				//蓝色
	NOUN_COLOR_PURPLE = 2029,			//紫色
	NOUN_COLOR_GOLDEN = 2030,			//金色
	NOUN_GOLD_NOT_ENOUGH = 2031,		//星钻不足，是否充值？
	NOUN_GOLD_CHARGE = 2032,			//充 值
	NOUN_NAME_NOT_EMPTY = 2033,			//名字不能为空 
	NOUN_NAME_NO_SPACE = 2034,			//名字中不能有空格
	NOUN_NAME_TOO_LONG = 2035,			//名字长度超出限制
	NOUN_STOCK_TXT = 2036,				//库存:
};

//taddy[3000-3999]
enum enumNounConstsTaddy {
	NOUN_ALL_GUILD = 3001, // 所有部落
	NOUN_APPLY, // 申请
	NOUN_APPLIED, // 已申请
	NOUN_UNDO, // 撤销
	NOUN_SEARCH, // 查找
	NOUN_CREATE_GUILD, // 创建部落
	NOUN_ENTER_GUILD, // 进入部落
	NOUN_INVITE, // 邀请
	NOUN_AGREE, // 同意
	NOUN_APPLY_LIST, // 申请列表
	NOUN_EXIT_GUILD, // 退出部落
	NOUN_DISMISS_GUILD, // 解散部落
	NOUN_IMPEACH_CHAIRMAN, // 弹劾大酋长
	NOUN_TO_DEPUTY, // 升职
	NOUN_TO_MEMBER, // 降职
	NOUN_BE_FRIEND, // 加为好友
	NOUN_CHAT, // 私聊
	NOUN_IGNORE_LIST, // 黑名单
	NOUN_KICKOUT_FROM_GUILD, // 踢出部落
	NOUN_CHAIRMAN, // 大酋长
	NOUN_DEPUTY, // 长老
	NOUN_MEMBER, // 部众
	NOUN_ONLINE, // 在线
	NOUN_OFFLINE, // 不在线
	NOUN_HP, // 生命
	NOUN_MP, // 能量
	NOUN_ATTACK, // 攻击
	NOUN_DEFENSE, // 防御
	NOUN_MOVE_SPEED, // 移动速度
	NOUN_RUN_SPEED, // 疾行速度
	NOUN_DODGE_DISTANCE, // 闪避距离
	NOUN_DODGE_COOLING, // 闪避冷却
	NOUN_ATTR_DPS, // 秒伤
	NOUN_RECV_TASK, // 接取任务
	NOUN_COMMIT_TASK, // 提交任务
	NOUN_ATK_ICE, // 冰属性攻击
	NOUN_ATK_FIRE, // 火属性攻击
	NOUN_ATK_THUNDER, // 雷属性攻击
	NOUN_ATK_POISON, // 毒属性攻击
	NOUN_ATK_BIOCHEMISTRY, // 生化属性攻击
	NOUN_DEF_ICE, // 冰属性防御
	NOUN_DEF_FIRE, // 火属性防御
	NOUN_DEF_THUNDER, // 雷属性防御
	NOUN_DEF_POISON, // 毒属性防御
	NOUN_DEF_BIOCHEMISTRY, // 生化属性防御
	NOUN_DRESSED_EQUIP, // 当前装备
	NOUN_SELECTED_EQUIP, // 选中装备
	NOUN_TAKE_ON, // 穿上
	NOUN_TAKE_OFF, // 卸下
	NOUN_REPLACE, // 替换
	NOUN_ITEM, // 物件
	NOUN_MATEARIAL, // 材料
	NOUN_GIFT, // 礼包
	NOUN_COIN, // 联邦币
	NOUN_GOLD, // 星钻
	NOUN_CLOSE_PANITRATE, // 近程刺击
	NOUN_CLOSE_BLUNT, // 近程钝击
	NOUN_CLOSE_MAGIC, // 近程属性攻击
	NOUN_CLOSE_ORDINARY, // 近程普通攻击
	NOUN_REMOTE_MAGIC, // 远程属性攻击
	NOUN_REMOTE_PANITRATE, // 远程穿刺攻击
	NOUN_REMOTE_ORDINARY, // 远程普通攻击
	NOUN_GET, // 领取
	NOUN_GOT, // 已领取
	NOUN_USE, // 使用
	NOUN_BUY, // 购买
	NOUN_EXP, // 经验
	NOUN_EQUIP, // 装备
	NOUN_WEAPON, // 武器
	NOUN_ARENA_COIN, // 功勋
	NOUN_WEAPON_LEVELUP_PROP, // 枪械强化道具
	NOUN_WEAPON_STEPSTAR_PROP, // 升星升级道具
	NOUN_ATTR_STRENGTHEN_PROP, // 属性道具
	NOUN_EVOLUTION_PROP, // 进化道具
	NOUN_HOT_PROP, // 热销道具
	NOUN_GOTO, // 前往
	NOUN_COMPLETED, // 已完成
	NOUN_BAG, // 背包
    NOUN_DETAIL_PROP, // 详细属性
    NOUN_ATTR_HIT, // 命中
	NOUN_DEL_FRIEND, //删除好友
	NOUN_CULTURE, //培养
};


inline std::string NounStr(const int iKey)
{
    return NOUN_CFG_MASTER->GetStr(iKey);
}

inline std::string AttrTypeStr(const int iAttrType)
{
    //属性类型定义在ClientSvrMsg里，这里就不引包了吧
    switch(iAttrType) {
    case 1:
        return NOUN_CFG_MASTER->GetStr(RUNSPEED);
    case 2:
        return NOUN_CFG_MASTER->GetStr(HP);
    case 3:
        return NOUN_CFG_MASTER->GetStr(DEF);
    case 4:
        return NOUN_CFG_MASTER->GetStr(MOVESPEED);
    case 5:
        return NOUN_CFG_MASTER->GetStr(MP);
    case 6:
        return NOUN_CFG_MASTER->GetStr(ATK);
    case 7:
        return NOUN_CFG_MASTER->GetStr(NOUN_ATTR_HIT);
    default:
        return "";
    }
    
}


//防御类型 ARMOR_TYPE
inline std::string ArmorTypeStr(const int iArmorType)
{
    switch(iArmorType) {
    case 0:
        return NOUN_CFG_MASTER->GetStr(NONE);
    case 1:
        return NOUN_CFG_MASTER->GetStr(HEARY_ARMOR_DEF_STR);
    case 2:
        return NOUN_CFG_MASTER->GetStr(LIGHT_ARMOR_DEF_STR);
    default:
        return "";
    }

}


//WeaponCfg AtkAttrType, 
inline std::string AtkAttrTypeStr(const int iAtkAttrType)
{
    //定义在Macros.proto的MAGIC_TYPE
    switch(iAtkAttrType) {
    case 1:
        return NOUN_CFG_MASTER->GetStr(STR_ICE); ///< 冰
    case 2:
        return NOUN_CFG_MASTER->GetStr(STR_FLAME); ///< 火
    case 3:
        return NOUN_CFG_MASTER->GetStr(STR_POISON);  ///< 毒
    case 4:
        return NOUN_CFG_MASTER->GetStr(STR_BIOTECH);  ///< 生化
    case 5:
        return NOUN_CFG_MASTER->GetStr(STR_LIGHTING); ///< 闪电
    default:
        return "";
    }

}



#endif //__NOUN_HELPER_H__



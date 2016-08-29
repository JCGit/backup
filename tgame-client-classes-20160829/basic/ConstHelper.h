#ifndef ConstHelper_h__
#define ConstHelper_h__

#include "ConstantCfgMaster.h"
#include "vlib/base/CVSingleton.h"

//配置中的ID
enum enumConstCfgIDs{
    FIGHT_EP_ADD_VALUE = 1,
    WEAPON_BAG_MAX_NUM = 3,			//枪械库背包数量上限100
	FIGHT_ANI_EFF_MAX = 4,			//战斗中受伤、死亡特效数量上限
	FIGHT_ANI_EFF_INTER = 5,		//战斗中受伤、死亡特效间隔时间(毫秒)
	FIGHT_STAGE_MAX_MONSTERS = 6,	//战斗中同屏中怪物最大数量
	FIGHT_BULLET_SKILL_ADD_BUFF_MAX = 7,	//战斗中子弹、技能添加buff数量上限
	FIGHT_STAGE_WALK_TIME = 8,			//战斗中过阶段时行走时间(毫秒)
	ARENA_MONSTER_LV_PARAM_ONE = 9,		//竞技场被除数常量系数
	ARENA_MONSTER_LV_PARAM_TWO = 10,	//竞技场防御方等级系数
	FIGHT_MEDICAL_CFG_DI = 11,			//医药包的配置ID
	FIGHT_RELIVE_INVINCIBLE_BUFF_ID = 12,	//复活无敌buff
	FIGHT_DASH_INVINCIBLE_BUFF_ID = 13,		//闪避无敌buff
	FIGHT_BULLET_INVINCIBLE_BUFF_ID = 14,	//补充弹药箱无敌buff
};

inline unsigned int ConstHelper(const int iKey)
{
	return CONSTANT_CFG_MASTER->GetConstantCfgByID(iKey)->value();
}

#endif // ConstHelper_h__

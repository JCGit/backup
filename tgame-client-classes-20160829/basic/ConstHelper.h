#ifndef ConstHelper_h__
#define ConstHelper_h__

#include "ConstantCfgMaster.h"
#include "vlib/base/CVSingleton.h"

//�����е�ID
enum enumConstCfgIDs{
    FIGHT_EP_ADD_VALUE = 1,
    WEAPON_BAG_MAX_NUM = 3,			//ǹе�ⱳ����������100
	FIGHT_ANI_EFF_MAX = 4,			//ս�������ˡ�������Ч��������
	FIGHT_ANI_EFF_INTER = 5,		//ս�������ˡ�������Ч���ʱ��(����)
	FIGHT_STAGE_MAX_MONSTERS = 6,	//ս����ͬ���й����������
	FIGHT_BULLET_SKILL_ADD_BUFF_MAX = 7,	//ս�����ӵ����������buff��������
	FIGHT_STAGE_WALK_TIME = 8,			//ս���й��׶�ʱ����ʱ��(����)
	ARENA_MONSTER_LV_PARAM_ONE = 9,		//����������������ϵ��
	ARENA_MONSTER_LV_PARAM_TWO = 10,	//�������������ȼ�ϵ��
	FIGHT_MEDICAL_CFG_DI = 11,			//ҽҩ��������ID
	FIGHT_RELIVE_INVINCIBLE_BUFF_ID = 12,	//�����޵�buff
	FIGHT_DASH_INVINCIBLE_BUFF_ID = 13,		//�����޵�buff
	FIGHT_BULLET_INVINCIBLE_BUFF_ID = 14,	//���䵯ҩ���޵�buff
};

inline unsigned int ConstHelper(const int iKey)
{
	return CONSTANT_CFG_MASTER->GetConstantCfgByID(iKey)->value();
}

#endif // ConstHelper_h__

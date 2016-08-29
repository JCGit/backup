#pragma once

#include <map>

enum eStateTag
{
	EST_INVALID = 0,
	EST_Build_CanUpgrade,	// ����������ʱ
	EST_Build_ProductRate,	// ���������ﵽ�������޵İٷֱ�
	EST_Camp_MorePeople,	// �����ܱ����ﵽN
	EST_Build_HoldRate,		// ���������ﵽ�������޵İٷֱ�
	EST_Builds_BeAttN,		// ����N�񼺷������ܵ�����ʱ
	EST_N_NeutralBuild,		// ����N�������������Ӫʱ
	EST_N_MorePeople,		// �������������з�����N, �෽��ս�򳬹�����ΪN
	EST_N_Builds,			// ����ռ��Ľ�����������N
	EST_Battle_Other,		// �����ѷ��������������ʱ
	EST_D_Monster,			// �������ɴ�Ұ��
	EST_D_Builds,			// �������ɸ���ڵĵз���������N
	EST_D_MorePeople,		// �������ɸ���ڵĵз�������������N
	EST_D_LessPeople,		// �������ɸ���ڵĵз���������С��N
	EST_NO_BeAttack,		// ���м�������û���ܵ��κι���
	EST_Build_PeopleNum,	// ��ǰ�����ڵı����ﵽָ��ֵ
	EST_PeopleCountInN,		// N��Χ�ڴ��ڱ�������M�Ľ���
	EST_HaveXXbuffer,		// ӵ��xxbuff
	EST_MoraleGtN,			// ����ʿ������M
	EST_OwnerNotDamage,		// ������û���յ��˺�
	EST_OwnerCampPeople,	// �ܱ�������M
	EST_RunTransformCommand,//ָ������������Ԯָ��
	EST_CountDownSurplus,	//ʣ��ʱ������M��
	EST_BuildSoldierNum,	//ָ����������
	EST_EnemySubDstSoldierNum,	//�з��򱾽����ɱ�����פ�����Ĳ��
	EST_EnemySubFriendSoldierNum, //�з����ѷ������ɱ�����פ�����Ĳ��
	EST_EnemySubOtherSoldierNum, //�з���������ɱ�����פ�����Ĳ��
	EST_EnemyBuildAfterSendNum, //�з��ɱ�֮�������
	EST_EnemyBuildAfterReceiveNum, //�з��ܵ�֮�������
	EST_SelfBuildLevel, //�������ȼ�
	EST_BuildTypeNum,		//�Ѿ�ռ�������
	EST_TimeCompareLeft=31,		//��ǰʱ��Ƚ�ʣ��ʱ��
	EST_SoldierNumCompareALL=32,	//�ܱ��������������޵İٷֱ���M�Ƚ�
	EST_SildierNumOrBuildNum=33,	//�з��ܱ������ܽ���������
	EST_PlayerPropCount,			//��һ�����ս������Я���ĵ�������
};	

enum eActionTag
{
	EAT_INVALID = 0,
	EAT_DO_NOTHING = 1,			// ��������
	EAT_ALL_ATTACK = 2,			// ȫ������
	EAT_BUILD_UPGRADE = 3,		// ����
	EAT_SHIFT1 = 4,				// �ͱ�1
	EAT_SHIFT2 = 5,				// �ͱ�2
	EAT_ATTACK1 = 6,			// ����1
	EAT_ATTACK2 = 7,			// ����2
	EAT_HELP1 = 8,				// ��Ԯ1
	EAT_HELP2 = 9,				//
	EAT_MONSTER_ATTACK1 = 10,			// ��Ұ
	EAT_MONSTER_ATTACK2 = 11,
	EAT_ATTACK_RANGE = 12,			// �ɳ�M��������N�񹥻���������
	EAT_ATTACK_NEAREST = 13,		// �ɳ�M������������ĵз����� 
	EAT_ATTACK_RATE_RANGE = 14,			// �ɳ�M%��������N�񹥻���������
	EAT_ATTACK_RATE_NEAREST = 15,		// �ɳ�M%������������ĵз����� 
	EAT_PROP_SILDIER = 16,				//����IDʹ�ñ�����
	EAT_PROP_SILDIER_TYPE = 17,			//���ݽ����������ٱ�����ʹ�ñ�����
	EAT_PROP_BOOM = 18,					//ը�� Ŀ���Ϊ�з�С������λ��
	EAT_PROP_ICE = 19,					//����	Ŀ���Ϊ�з�С������λ��
	EAT_PROP_OIL =20,					//�ع���	Ŀ���Ϊ�з�С������λ��

};

class BaseState;
class StateFactory;
class BaseAction;
class ActionFactory;

typedef std::map<eStateTag, BaseState*> StateList;
typedef std::map<eStateTag, StateFactory*> StateFactoryList;

typedef std::map<eActionTag, BaseAction*> ActionList;
typedef std::map<eActionTag, ActionFactory*> ActionFactoryList;
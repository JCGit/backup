#ifndef __INSTANCE_AI_H__
#define __INSTANCE_AI_H__
#include "fight/BaseFightScene.h"


class CInstanceAI
{
public:
	static void UpdateInstanceByAI(CBaseFightScene* pstLayer);
	static void UpdateMonsterByAI(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);
	static void UpdateFriendByAI(CBaseFightScene* pstLayer, CQStageEntity* pstFriend);
	static void UpdateHeroProtectMeByAI(CBaseFightScene* pstLayer, CQStageEntity* pstHero);
	static void UpdateOrdinaryFriendByAI(CBaseFightScene* pstLayer, CQStageEntity* pstEntity);
	static void UpdateHostilePlayerByAI(CBaseFightScene* pstLayer, CQStageEntity* pstEntity);

	static void UpdateMonsterAtWill(CBaseFightScene* pstLayer, CQStageEntity* pstMonster, CQStageEntity* pstTarget);

	static void UpdateMonsterAttackMarch(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);

	//initScale�� 0�����̶�����-1���̶�����1���̶�����
	static void UpdateEntityStandStillAttack(CBaseFightScene* pstLayer, CQStageEntity* pstMonster, int initScale = 0);		

	static void UpdateEntityRandMoveAttackWOChase(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);

	static void UpdateEntityLockEnimyPotrol(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);	

	//��һ����Χ���й䣬����cd����������ڷ�Χ�ڲ����򲢹�����ң�cdδ�������й�
	static void UpdateEntityMoveAround(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);		

	//ս����AI��ֻ�������ƶ�����ҽ��빥����Χ��������������ƶ�
	static void UpdateWarShipAI(CBaseFightScene* pstLayer, CQStageEntity* pstMonster, unsigned int aiID);		

	//���������ŵ�λ��
	static void CorrectAtkerPos(CBaseFightScene* pstLayer, CQStageEntity* pstAtk, CQStageEntity* pstDef);

	//������ɫ��λ�ã����ܳ�����������
	static bool CorrectEntityPos(CBaseFightScene* pstLayer, CQStageEntity* pstEntity);

	//�Զ�����boss��ai
	static void UpdateHeroAttackBossByAI(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);

	//����Զ���ǹ
	static void AutoHeroChangeWeapon(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);

	static int HERO_AI_ATK_LENGTH;
	static int HERO_AI_NUETRAL_LENGTH;
};

#endif
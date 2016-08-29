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

	//initScale， 0：不固定方向，-1：固定向左，1：固定向右
	static void UpdateEntityStandStillAttack(CBaseFightScene* pstLayer, CQStageEntity* pstMonster, int initScale = 0);		

	static void UpdateEntityRandMoveAttackWOChase(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);

	static void UpdateEntityLockEnimyPotrol(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);	

	//在一定范围内闲逛，技能cd到了且玩家在范围内才走向并攻击玩家，cd未到继续闲逛
	static void UpdateEntityMoveAround(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);		

	//战舰的AI，只能上下移动，玩家进入攻击范围，随着玩家上下移动
	static void UpdateWarShipAI(CBaseFightScene* pstLayer, CQStageEntity* pstMonster, unsigned int aiID);		

	//矫正攻击放的位置
	static void CorrectAtkerPos(CBaseFightScene* pstLayer, CQStageEntity* pstAtk, CQStageEntity* pstDef);

	//矫正角色的位置，不能超出行走区域
	static bool CorrectEntityPos(CBaseFightScene* pstLayer, CQStageEntity* pstEntity);

	//自动攻击boss的ai
	static void UpdateHeroAttackBossByAI(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);

	//玩家自动换枪
	static void AutoHeroChangeWeapon(CBaseFightScene* pstLayer, CQStageEntity* pstMonster);

	static int HERO_AI_ATK_LENGTH;
	static int HERO_AI_NUETRAL_LENGTH;
};

#endif
#include "InstanceAI.h"
#include "defines.h"
#include "InstanceUtils.h"
#include "BasicTypes.h"

#include "AIParamCfgMaster.h"

int CInstanceAI::HERO_AI_ATK_LENGTH = 5;
int CInstanceAI::HERO_AI_NUETRAL_LENGTH = 3;

void CInstanceAI::UpdateInstanceByAI(CBaseFightScene* pstLayer)
{
	if (!pstLayer){
		return;
	}

	CQStageEntity* pstMonster;
	for (unsigned int iIdx = 0; iIdx < pstLayer->getStageEntities()->count(); iIdx++)
	{
		pstMonster = (CQStageEntity*)pstLayer->getStageEntities()->objectAtIndex(iIdx);
		if (!pstMonster->GetIsControlledByAI() || pstMonster->getFAiTime() < 0.2){
			continue; //������Щ����AI���ƵĶ���
		}
		if (pstMonster->getMyState() == kActionStateDie)
		{//���Ѿ��������������������������
			pstLayer->getSceneForeground()->removeChild(pstMonster, true);
			pstLayer->getStageEntities()->removeObjectAtIndex(iIdx);
			iIdx --;
			//pstLayer->getMonsters().erase(it);
		}else if (pstMonster->getRoleParty() == ROLE_PARTY_HOSTILE && pstMonster->getRoleType() != ROLE_TYPE_PLAYER){
			UpdateMonsterByAI(pstLayer, pstMonster);
		}else if (pstMonster->getRoleParty() == ROLE_PARTY_HOSTILE && pstMonster->getRoleType() == ROLE_TYPE_PLAYER){
			UpdateHostilePlayerByAI(pstLayer, pstMonster);
		}else if (pstMonster->getRoleParty() == ROLE_PARTY_FRIEND){
			UpdateFriendByAI(pstLayer, pstMonster);
		}

		pstMonster->setFAiTime(0);
	}
}

void CInstanceAI::UpdateMonsterByAI(CBaseFightScene* pstLayer, CQStageEntity* pstMonster)
{
	
	if (!pstLayer || !pstMonster){
		CCLog("null input %p, %p", pstLayer, pstMonster);
		return;
	}

	//���Ȼ���������Ļ�ϵ�ʵ��λ��

	CCPoint stPos = ccpAdd(pstMonster->getPosition(), pstLayer->getSceneForeground()->getPosition());
	CCPoint stDist = ccpSub(pstMonster->getPosition(), pstLayer->getMainHero()->getPosition());
	if (pstMonster->getArmature()){

// 		CCLog("(%f, %f),dist %f, %f",
// 			pstMonster->GetFootPos().x,
// 			pstMonster->GetFootPos().y,
// 			pstMonster->GetFootPos().x - pstLayer->getMainHero()->GetFootPos().x, 
// 			pstMonster->GetFootPos().y - pstLayer->getMainHero()->GetFootPos().y);

		
		bool bIsCastSkill = false;
		CQStageEntity* pstTargetEntity = NULL;
		CCObject* pstEntity;
		CCARRAY_FOREACH(pstLayer->getStageEntities(), pstEntity){
			CQStageEntity* pstTmpTarget = dynamic_cast<CQStageEntity*>(pstEntity);
			if (pstTmpTarget->getRoleParty() == pstMonster->getRoleParty()){
				continue; //ֻ����ҷ���λ�ͷ�
			}

			if (pstTmpTarget->IsImmune() || pstTmpTarget->IsDead() || !pstTmpTarget->getArmature()){
				continue;
			}

			float tdx = fabs(pstTmpTarget->GetFootPos().x - pstMonster->GetFootPos().x);
			float tdy = fabs(pstTmpTarget->GetFootPos().y - pstMonster->GetFootPos().y);

			int skillID = CInstanceUtils::FindAtkSkill(pstMonster, tdx, tdy);
			if (skillID > 0 && pstMonster->getAIType() != AI_TYPE_MOVE_LEFT_ONLY){
				//CCLog("distX = %f, distY = %f\n", distX, distY);
				if(pstMonster->getAIType() != AI_TYPE_STAND_ATTACK_LEFT
					&& pstMonster->getAIType() != AI_TYPE_WARSHIP){
					if (stDist.x > 0){
						pstMonster->setScaleX(-1);
						//�ȵ�������ĳ���
					}else{
						pstMonster->setScaleX(1);
					}
				}
				pstMonster->AttackWithSkill(skillID);
				return;
			}
			pstTargetEntity = pstTmpTarget;
		}

		unsigned int aiID = pstMonster->getAIType();
		unsigned int aiType = AI_PARAM_CFG_MASTER->GetAICfgByID(aiID)->aitype();

		switch (aiType)
		{
		case AI_TYPE_FIRE_AT_WILL:
			if (pstTargetEntity){
				UpdateMonsterAtWill(pstLayer, pstMonster, pstTargetEntity);				
			}else{
				UpdateMonsterAtWill(pstLayer, pstMonster, pstLayer->getMainHero());	
			}
			break;
		case AI_TYPE_ATTACK_MARCH:
        case AI_TYPE_LAST_SKILL_ONLY:
		case AI_TYPE_MOVE_LEFT_ONLY:
			UpdateMonsterAttackMarch(pstLayer, pstMonster);
			break;
		case AI_TYPE_STANDSTILL_ATTACK:
			UpdateEntityStandStillAttack(pstLayer, pstMonster);
			break;
		case AI_TYPE_STAND_ATTACK_LEFT:
			UpdateEntityStandStillAttack(pstLayer, pstMonster, -1);
			break;
		case AI_TYPE_MOVE_ATTACK_WO_CHASE:
			UpdateEntityRandMoveAttackWOChase(pstLayer, pstMonster);
			break;
		case AI_TYPE_LOCK_ENIMY_POTROL:
			UpdateEntityLockEnimyPotrol(pstLayer, pstMonster);
			break;
		case AI_TYPE_MOVE_AROUND:
			UpdateEntityMoveAround(pstLayer, pstMonster);
			break;
		case AI_TYPE_WARSHIP:
			UpdateWarShipAI(pstLayer, pstMonster, aiID);
			break;
		default:
			break;
		}

	}else{

		if (pstMonster->getArmature() && pstMonster->getMyState() != kActionStateIdle && pstMonster->getMyState() != kActionStateDie){
			pstMonster->Idle();
		}
	}
}

void CInstanceAI::UpdateMonsterAttackMarch(CBaseFightScene* pstLayer, CQStageEntity* pstMonster){
	if (!pstLayer || !pstMonster){
		return;
	}
	pstMonster->WalkWithDirection(ccp(-1, 0)); //һֱ�����ߣ��Ժ�����������ߵ�������˵��
}

void CInstanceAI::UpdateMonsterAtWill(CBaseFightScene* pstLayer, CQStageEntity* pstMonster, CQStageEntity* pstTarget){
	if (!pstLayer || !pstMonster || !pstTarget){
		return;
	}

	CCPoint stDist = ccpSub(pstMonster->getPosition(), pstTarget->getPosition());
	float distX = fabs(pstMonster->GetFootPos().x - pstTarget->GetFootPos().x);
	float distY = fabs(pstMonster->GetFootPos().y - pstTarget->GetFootPos().y);
	float disW = pstTarget->getCenterToSides()*2/3;

	bool bNeedRepath = false;

	if ((pstTarget->GetFootPos().x - pstMonster->GetFootPos().x) * pstMonster->getScaleX() < 0){
		bNeedRepath = true;
	}


	if ((pstMonster->getMyState() == kActionStateIdle || (pstMonster->getMyState() == kActionStateWalkToDst && bNeedRepath))
		&& (distX > pstMonster->getAttakRange() || distY > pstMonster->getDamageRange())){	
		static int aaa = 0;
		if(aaa++ >= 2){
			CCPoint stMovDst;
			if(stDist.x > 0){
				stMovDst = ccpAdd(pstTarget->getPosition(), ccp(disW, 0));	
			}else{
				stMovDst = ccpAdd(pstTarget->getPosition(), ccp(-disW, 0));
			}

			if(stMovDst.x > pstLayer->getStageRect().getMaxX()){
				stMovDst.x = pstLayer->getStageRect().getMaxX();
			}else if(stMovDst.x < pstLayer->getStageRect().getMinX()){
				stMovDst.x = pstLayer->getStageRect().getMinX();
			}

			pstLayer->moveEntity(pstMonster, stMovDst);
			aaa = 0;
		}
	}

	CorrectAtkerPos(pstLayer, pstMonster, pstTarget);
		
}

void CInstanceAI::UpdateFriendByAI(CBaseFightScene* pstLayer, CQStageEntity* pstEntity){
	if (!pstLayer || !pstEntity){
		CCLog("null input %p, %p", pstLayer, pstEntity);
		return;
	}

	if (pstEntity->getRoleParty() != ROLE_PARTY_FRIEND){
		return;
	}

	if (pstEntity->getRoleType() == ROLE_TYPE_FRIEND){
		UpdateOrdinaryFriendByAI(pstLayer, pstEntity);
	}else if (pstEntity->getRoleType() == ROLE_TYPE_PLAYER 
		&& pstEntity->getAIType() == AI_TYPE_PROTECT_ME){
		UpdateHeroProtectMeByAI(pstLayer, pstEntity);
	}else if (pstEntity->getRoleType() == ROLE_TYPE_PLAYER 
		&& pstEntity->getAIType() == AI_TYPE_ATTACK_BOSS_CLOSEST){
		UpdateHeroAttackBossByAI(pstLayer, pstEntity);
	}
	return;
}

void CInstanceAI::UpdateHeroProtectMeByAI(CBaseFightScene* pstLayer, CQStageEntity* pstHero){
	CMainHero* pstFriendHero = dynamic_cast<CMainHero*>(pstHero);

	CCPoint stDist = ccpSub(pstFriendHero->getPosition(), pstLayer->getMainHero()->getPosition());
	if (fabs(stDist.x) > 500){
		const string stMovement = pstFriendHero->getArmature()->getAnimation()->getCurrentMovementID();
		if ((stDist.x * pstFriendHero->getScaleX() < 0 	|| !pstFriendHero->IsWalking())
			&& pstFriendHero->IsWalkable()){
			pstLayer->moveEntity(pstFriendHero, pstLayer->getMainHero()->getPosition());
			//CCLog("2 friend move from %f, %f to %f, %f", pstFriendHero->getPositionX(), pstFriendHero->getPositionY(), pstLayer->getMainHero()->getPositionX(), pstLayer->getMainHero()->getPositionY());
		}
		
		pstFriendHero->stateChangeByAttack(true);
		return;
	}

	float fCurXdist = 1000;
	float fCurYdist = 1000;
	float fDstX = -1;
	float fDstY = -1;
	float fCurDist = 1000;
	//�ж���Χ�ɲ�����ʩ�ż��ܻ��߽��й�����
	CCObject* pstObj;
	CCARRAY_FOREACH(pstLayer->getStageEntities(), pstObj){
		if (dynamic_cast<CQStageEntity*>(pstObj)->getRoleParty() == pstFriendHero->getRoleParty()){
			continue; //ֻ����ҷ���λ�ͷ�
		}

		if (dynamic_cast<CQStageEntity*>(pstObj)->IsImmune()){
			continue;
		}


		float tdx = dynamic_cast<CQStageEntity*>(pstObj)->GetFootPos().x - pstFriendHero->GetFootPos().x;
		float tdy = dynamic_cast<CQStageEntity*>(pstObj)->GetFootPos().y - pstFriendHero->GetFootPos().y;

		int skillID = CInstanceUtils::FindAtkSkill(pstFriendHero, fabs(tdx), fabs(tdy));
		if (skillID > 0){
			//CCLog("distX = %f, distY = %f\n", distX, distY);
			if (tdx < 0){
				pstFriendHero->setScaleX(-1);
				//�ȵ�������ĳ���
			}else{
				pstFriendHero->setScaleX(1);
			}
			pstFriendHero->stateChangeByAttack(false);
			pstFriendHero->CastSkillWithSkillID(skillID);
			return;
		}

		if (fabs(tdx) <= pstFriendHero->GetBulletShootDistance()){
			if (fCurYdist > fabs(tdy)){
				fCurXdist = fabs(tdy);
				fDstX = dynamic_cast<CQStageEntity*>(pstObj)->getPositionX();
				fDstY = dynamic_cast<CQStageEntity*>(pstObj)->getPositionY();
			}
			break;
		}else if(fCurDist > ccpDistance(ccp(tdx, tdy), CCPointZero)){
			fCurDist = ccpDistance(ccp(tdx, tdy), CCPointZero);
			fDstX = dynamic_cast<CQStageEntity*>(pstObj)->getPositionX();
			fDstY = dynamic_cast<CQStageEntity*>(pstObj)->getPositionY();
// 			if (fDstX > pstFriendHero->getPositionX()){
// 				fDstX -= pstFriendHero->GetBulletShootDistance() / 2;
// 			}else{
// 				fDstX += pstFriendHero->GetBulletShootDistance() / 2;
// 			}
		}


	}
	if (pstFriendHero->IsWalkable() && fDstY != -1){
		
		bool bIsMovingToDest = true;
		if ((fDstY - pstFriendHero->getPositionY()) * pstFriendHero->getVelocity().y < 0){
			bIsMovingToDest = false;
		}else if ((fDstX - pstFriendHero->getPositionX()) * pstFriendHero->getVelocity().x < 0){
			bIsMovingToDest = false;
		}

		if ((fabs(fDstY - pstFriendHero->getPositionY()) > 80 || fabs(fDstX - pstFriendHero->getPositionX()) > pstFriendHero->GetBulletShootDistance())
			&& !bIsMovingToDest){
			pstLayer->moveEntity(pstFriendHero, ccp(fDstX, fDstY));
			//CCLog("1 friend move from %f, %f to %f, %f", pstFriendHero->getPositionX(), pstFriendHero->getPositionY(), fDstX, fDstY);
		}
		
		
		
	}
	

	//û���κμ��ܿ����ͷţ��Ǿ������������
	if(!pstFriendHero->isShooting() 
		&& !pstFriendHero->IsAttacking()
		&& pstFriendHero->IsAttackable()
		&& fDstY > 0){
		pstFriendHero->stateChangeByAttack(true);
	}else if(pstFriendHero->isShooting()){
		pstFriendHero->stateChangeByAttack(false);
	}

}

void CInstanceAI::UpdateOrdinaryFriendByAI(CBaseFightScene* pstLayer, CQStageEntity* pstEntity){
	

	
}

void CInstanceAI::UpdateHostilePlayerByAI(CBaseFightScene* pstLayer, CQStageEntity* pstPlayer){
	CMainHero* pstHostileHero = dynamic_cast<CMainHero*>(pstPlayer);
	bool isCorrect = CorrectEntityPos(pstLayer, pstPlayer);

	CCPoint stDist = ccpSub(pstHostileHero->getPosition(), pstLayer->getMainHero()->getPosition());
	if (fabs(stDist.x) > pstHostileHero->GetBulletShootDistance() / 1.5){
		const string stMovement = pstHostileHero->getArmature()->getAnimation()->getCurrentMovementID();
		if ((stDist.x * pstHostileHero->getScaleX() < 0 	|| !pstHostileHero->IsWalking())
			&& pstHostileHero->IsWalkable()){
				pstLayer->moveEntity(pstHostileHero, pstLayer->getMainHero()->getPosition());
				//CCLog("2 friend move from %f, %f to %f, %f", pstFriendHero->getPositionX(), pstFriendHero->getPositionY(), pstLayer->getMainHero()->getPositionX(), pstLayer->getMainHero()->getPositionY());
		}
		if (pstHostileHero->getFireStateChgTime() > HERO_AI_NUETRAL_LENGTH){
			pstHostileHero->stateChangeByAttack(true);
		}
		
		return;
	}else if (!isCorrect && fabs(stDist.x) < 50 && fabs(stDist.y) < 50){

		CCPoint stMovDst;
		
		if (stDist.x > 0){
			stMovDst = ccpAdd(pstLayer->getMainHero()->getPosition(), ccp(100, 0));	
			
		}else{
			stMovDst = ccpAdd(pstLayer->getMainHero()->getPosition(), ccp(-100, 0));
		}

		if(stMovDst.x > pstLayer->getStageRect().getMaxX() - pstPlayer->getCenterToSides()){
			stMovDst.x = pstLayer->getStageRect().getMaxX() - pstPlayer->getCenterToSides();
		}else if(stMovDst.x < pstLayer->getStageRect().getMinX() + pstPlayer->getCenterToSides()){
			stMovDst.x = pstLayer->getStageRect().getMinX() + pstPlayer->getCenterToSides();
		}

		if(stMovDst.y > pstLayer->getStageRect().getMaxY() - 10){
			stMovDst.y = pstLayer->getStageRect().getMaxY() - 10;
		}else if(stMovDst.y < pstLayer->getStageRect().getMinY() + 10){
			stMovDst.y = pstLayer->getStageRect().getMinY() + 10;
		}

		CCPoint stToVec = ccpSub(stMovDst, pstHostileHero->getPosition());
		
		pstHostileHero->AttackCounterMarchWithDirection(stToVec);
				
	}

	float fCurXdist = 10000;
	float fCurYdist = 10000;
	float fDstX = -1;
	float fDstY = -1;
	float fCurDist = 10000;
	//�ж���Χ�ɲ�����ʩ�ż��ܻ��߽��й�����
	CCObject* pstObj;
	CCARRAY_FOREACH(pstLayer->getStageEntities(), pstObj){
		if (dynamic_cast<CQStageEntity*>(pstObj)->getRoleParty() == pstHostileHero->getRoleParty()){
			continue; //ֻ����ҷ���λ�ͷ�
		}

		if (dynamic_cast<CQStageEntity*>(pstObj)->IsImmune()){
			continue;
		}


		float tdx = dynamic_cast<CQStageEntity*>(pstObj)->GetFootPos().x - pstHostileHero->GetFootPos().x;
		float tdy = dynamic_cast<CQStageEntity*>(pstObj)->GetFootPos().y - pstHostileHero->GetFootPos().y;
		if (tdx < 0){
			pstHostileHero->setScaleX(-1);
			//�ȵ�������ĳ���
		}else{
			pstHostileHero->setScaleX(1);
		}

		//�ü��ܿ��������ͷ�
		int skillID = CInstanceUtils::FindAtkSkill(pstHostileHero, 1, 1);
		if (skillID > 0){
			//CCLog("distX = %f, distY = %f\n", distX, distY);
			if (tdx < 0){
				pstHostileHero->setScaleX(-1);
				//�ȵ�������ĳ���
			}else{
				pstHostileHero->setScaleX(1);
			}
			pstHostileHero->stateChangeByAttack(false);
			pstHostileHero->AttackWithSkill(skillID);
			return;
		}

		if (fabs(tdx) <= pstHostileHero->GetBulletShootDistance()){
			if (fCurYdist > fabs(tdy)){
				fCurXdist = fabs(tdy);
				fDstX = dynamic_cast<CQStageEntity*>(pstObj)->getPositionX();
				fDstY = dynamic_cast<CQStageEntity*>(pstObj)->getPositionY();
			}
			break;
		}else if(fCurDist > ccpDistance(ccp(tdx, tdy), CCPointZero)){
			fCurDist = ccpDistance(ccp(tdx, tdy), CCPointZero);
			fDstX = dynamic_cast<CQStageEntity*>(pstObj)->getPositionX();
			fDstY = dynamic_cast<CQStageEntity*>(pstObj)->getPositionY();
			// 			if (fDstX > pstFriendHero->getPositionX()){
			// 				fDstX -= pstFriendHero->GetBulletShootDistance() / 2;
			// 			}else{
			// 				fDstX += pstFriendHero->GetBulletShootDistance() / 2;
			// 			}
		}
	}

	if (pstHostileHero->IsWalkable() && fDstY != -1){
		bool bIsMovingToDest = true;
		if ((fDstY - pstHostileHero->getPositionY()) * pstHostileHero->getVelocity().y <= 0){
			bIsMovingToDest = false;
		}else if ((fDstX - pstHostileHero->getPositionX()) * pstHostileHero->getVelocity().x <= 0){
			bIsMovingToDest = false;
		}

		if ((fabs(fDstY - pstHostileHero->getPositionY()) > 80 || fabs(fDstX - pstHostileHero->getPositionX()) > pstHostileHero->GetBulletShootDistance())
			&& !bIsMovingToDest){
				pstLayer->moveEntity(pstHostileHero, ccp(fDstX, fDstY));
				//CCLog("1 friend move from %f, %f to %f, %f", pstFriendHero->getPositionX(), pstFriendHero->getPositionY(), fDstX, fDstY);
		}
	}


	//û���κμ��ܿ����ͷţ��Ǿ������������
	if(!pstHostileHero->isShooting() 
		&& !pstHostileHero->IsAttacking()
		&& pstHostileHero->IsAttackable()
		&& fDstY >= 0
		&& !pstHostileHero->IsWalking()
		&& pstHostileHero->getFireStateChgTime() > HERO_AI_NUETRAL_LENGTH){
			pstHostileHero->stateChangeByAttack(true);
	}else if(pstHostileHero->isShooting() && pstHostileHero->getFireStateChgTime() > HERO_AI_ATK_LENGTH){
		pstHostileHero->stateChangeByAttack(false);
	}
}


void CInstanceAI::UpdateEntityStandStillAttack(CBaseFightScene* pstLayer, CQStageEntity* pstMonster, int initScale/* = 0*/){
	if (!pstMonster){
		return;
	}

	if(initScale != 0){
		pstMonster->setScaleX(initScale);
	}

 	int iIdx = pstMonster->GetAReadySkill();
 	if (iIdx != -1){
 		CCObject* pstObj;
 		CCARRAY_FOREACH(pstLayer->getStageEntities(), pstObj){
 			CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
			if (dynamic_cast<CQStageEntity*>(pstObj)->getRoleParty() == pstMonster->getRoleParty()){
				continue; //ֻ����ҷ���λ�ͷ�
			}

			if (dynamic_cast<CQStageEntity*>(pstObj)->IsImmune()){
				continue;
			}

			float tdx = dynamic_cast<CQStageEntity*>(pstObj)->GetFootPos().x - pstMonster->GetFootPos().x;
			float tdy = dynamic_cast<CQStageEntity*>(pstObj)->GetFootPos().y - pstMonster->GetFootPos().y;

			int skillID = CInstanceUtils::FindAtkSkill(pstMonster, fabs(tdx), fabs(tdy));
			if (skillID > 0){
				if(initScale == 0){
					if (tdx < 0){
						pstMonster->setScaleX(-1);
						//�ȵ�������ĳ���
					}else{
						pstMonster->setScaleX(1);
					}
				}
				
				pstMonster->AttackWithSkill(skillID);
				break;
			}
 		}
 	}

}

void CInstanceAI::UpdateEntityRandMoveAttackWOChase(CBaseFightScene* pstLayer, CQStageEntity* pstMonster){
		
	if (!pstMonster){
		return;
	}
// 	int iIdx = pstMonster->GetAReadySkill();
// 	if (iIdx != -1){
// 		CCObject* pstObj;
// 		CCARRAY_FOREACH(pstLayer->getStageEntities(), pstObj){
// 			CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
// 			if (pstEntity->getArmature() && pstEntity->getRoleParty() != pstMonster->getRoleParty()){
// 				if (pstEntity->getPositionX() - pstEntity->getPositionX()> 0){
// 					pstMonster->setScaleX(1);
// 				}else{
// 					pstMonster->setScaleX(-1);
// 				}
// 				break;
// 			}
// 		}
// 
// 		pstMonster->AttackWithSkillAtIdx(iIdx);
// 	}

	CCRect stDest = pstLayer->getPlayerVisibleSize();
	CCRect stageRect = pstLayer->getStageRect();

	//������Դ��˵Ļ����Ѿ����ˣ���������һ���ط���������ʼ������
	if (pstMonster->IsWalkable() && 
		(!pstMonster->IsWalking() || pstMonster->getPositionX() <= stDest.getMinX() || pstMonster->getPositionX() >= stDest.getMaxX())){//�����ܣ�����Ŀǰ��û����,�����Ѿ��ܳ�ȥ��
		
		float randdd = random_range(0, RANDOM_DOMINATOR);
		float rando = (random_range(0, RANDOM_DOMINATOR) *1.0)/ (RANDOM_DOMINATOR *1.0);
		float x = stDest.getMinX() + (random_range(0, RANDOM_DOMINATOR) *1.0)/ (RANDOM_DOMINATOR *1.0) * fabs(stDest.getMaxX() - stDest.getMinX());
		float y = stDest.getMinY() + (random_range(0, RANDOM_DOMINATOR) *1.0)/ (RANDOM_DOMINATOR *1.0) * fabs(stDest.getMaxY() - stDest.getMinY());

		if(pstMonster->getPositionX() > stageRect.getMaxX()){
			x = stageRect.getMaxX() - pstMonster->getCenterToSides()*2;
		}else if (x < stageRect.getMinX()){
			x = stageRect.getMinX() + pstMonster->getCenterToSides()*2;
		}

		pstLayer->moveEntity(pstMonster, ccp(x, y));
	}

	
}



void CInstanceAI::UpdateEntityLockEnimyPotrol(CBaseFightScene* pstLayer, CQStageEntity* pstMonster){
	CCRect walkableRect = pstLayer->getStageRect();

	//���������Ѳ�߷�Χ�ڣ���������
	float dixX = fabs(pstLayer->getMainHero()->getPositionX() - pstMonster->getPositionX());
	float dixY = fabs(pstLayer->getMainHero()->getPositionY() - pstMonster->getPositionY());
	if(dixX <= pstMonster->getPatrolArea() && dixY <= pstMonster->getPatrolArea()){
		if(pstMonster->getAttakRange()*0.8 > dixX){  //�����߽����Լ��Ĺ��������80%�����򹥻������80-90%
			if(pstMonster->IsWalkable() && !pstMonster->IsWalking()){
				float randWDis = pstMonster->getAttakRange()*0.2*((random_range(0, RANDOM_DOMINATOR) *1.0)/ (RANDOM_DOMINATOR *1.0));
				float randHDis = ((random_range(0, RANDOM_DOMINATOR) *1.0)/ (RANDOM_DOMINATOR *1.0))*walkableRect.size.height/2;

				if(pstMonster->getPositionX() > walkableRect.getMaxX()){
					randWDis = walkableRect.getMaxX() - pstMonster->getCenterToSides()*2;
				}else if (randWDis < walkableRect.getMinX()){
					randWDis = walkableRect.getMinX() + pstMonster->getCenterToSides()*2;
				}

				pstLayer->moveEntity(pstMonster, ccp(randWDis, randHDis));
			}else{
				pstMonster->Idle();
			}
			return;
		}

		//idle
		if(pstMonster->IsWalkable() && !pstMonster->IsWalking()){
			pstLayer->moveEntity(pstMonster, pstLayer->getMainHero()->getPosition());
		}else{
			if ((pstLayer->getMainHero()->GetFootPos().x - pstMonster->GetFootPos().x) * pstMonster->getScaleX() < 0){  //�������Ҳ�ͬ��ֱ���������
				pstLayer->moveEntity(pstMonster, pstLayer->getMainHero()->getPosition());
			}
		}

		//������̫��������λ��
		CorrectAtkerPos(pstLayer, pstMonster, pstLayer->getMainHero());
		return;
	}

	//�����ǰ��idle״̬�����Ҹ��ط���������
	if (pstMonster->IsWalkable() && !pstMonster->IsWalking()){
		float x = pstMonster->getPositionX() + ((random_range(0, RANDOM_DOMINATOR) - RANDOM_DOMINATOR / 2) * 1.0) / (RANDOM_DOMINATOR *0.5) * pstMonster->getPatrolArea(); //�������������ɫ��Ѳ�߷�Χ
		float y = pstMonster->getPositionY() + ((random_range(0, RANDOM_DOMINATOR) - RANDOM_DOMINATOR / 2) * 1.0) / (RANDOM_DOMINATOR *0.5) * pstMonster->getPatrolArea();
		if (y > pstLayer->getWalkHight()){
			y = pstLayer->getWalkHight();
		}else if (y < 0){
			y = 10;
		}

		if(pstMonster->getPositionX() > walkableRect.getMaxX()){
			x = walkableRect.getMaxX() - pstMonster->getCenterToSides()*2;
		}else if (x < walkableRect.getMinX()){
			x = walkableRect.getMinX() + pstMonster->getCenterToSides()*2;
		}

		pstLayer->moveEntity(pstMonster, ccp(x, y));
	}
}

void CInstanceAI::UpdateEntityMoveAround(CBaseFightScene* pstLayer, CQStageEntity* pstMonster){
	CCRect walkableRect = pstLayer->getStageRect();

	//���������Ѳ�߷�Χ�ڣ���������
	float dixX = fabs(pstLayer->getMainHero()->GetFootPos().x - pstMonster->GetFootPos().x);
	float dixY = fabs(pstLayer->getMainHero()->GetFootPos().y - pstMonster->GetFootPos().y);
	if(dixX <= pstMonster->getPatrolArea() && dixY <= pstMonster->getPatrolArea()){
		int skillID = CInstanceUtils::FindAtkSkill(pstMonster, fabs(dixX), fabs(dixY));
		if(skillID > 0 && (pstMonster->IsWalkable() && !pstMonster->IsWalking())){  //�ڷ�Χ�ڣ����м���
			pstLayer->moveEntity(pstMonster, pstLayer->getMainHero()->getPosition());
			return;
		}
	}

	 //�ڷ�Χ�⣬�����ڷ�Χ��û�м���
	if (pstMonster->IsWalkable() && !pstMonster->IsWalking()){
		float x = pstMonster->getPositionX() + ((random_range(0, RANDOM_DOMINATOR) - RANDOM_DOMINATOR / 2) * 1.0) / (RANDOM_DOMINATOR *0.5) * pstMonster->getPatrolArea(); //�������������ɫ��Ѳ�߷�Χ
		float y = pstMonster->getPositionY() + ((random_range(0, RANDOM_DOMINATOR) - RANDOM_DOMINATOR / 2) * 1.0) / (RANDOM_DOMINATOR *0.5) * pstMonster->getPatrolArea();
		if (y > pstLayer->getWalkHight()){
			y = pstLayer->getWalkHight();
		}else if (y < 0){
			y = 10;
		}

		if(pstMonster->getPositionX() > walkableRect.getMaxX()){
			x = walkableRect.getMaxX() - pstMonster->getCenterToSides()*2;
		}else if (x < walkableRect.getMinX()){
			x = walkableRect.getMinX() + pstMonster->getCenterToSides()*2;
		}

		pstLayer->moveEntity(pstMonster, ccp(x, y));
	}else{
		pstMonster->Idle();
	}
}

void CInstanceAI::UpdateWarShipAI(CBaseFightScene* pstLayer, CQStageEntity* pstMonster, unsigned int aiID){
	static float fPreWalkTime = 0;		//�ϴ��ƶ�ʱ��
	unsigned int uParam = AI_PARAM_CFG_MASTER->GetAICfgByID(aiID)->aiparams(0);		//����0��ʾ��ֱ�
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize();
	float disH = 1.0f*pstLayer->getWalkHight()*uParam/EGAME_DENOM;		//���߸߶�
	float posY = pstMonster->getPositionY();
	pstMonster->setScaleX(-1);
	if(fabs(pstLayer->getMainHero()->getPositionX() - posY) > stSize.width*3/4){  //�����������3/4����Idle
		pstMonster->Idle();
		return;
	}

	bool bIsCanMove = false;
	float heroPosY = pstLayer->getMainHero()->getPositionY();
	float lastX = pstMonster->getPositionX();
	float lastY = 0;

	if(pstMonster->GetEntityTime() < fPreWalkTime){  //����ʱ��
		fPreWalkTime = 0;
	}

	if(fPreWalkTime == 0 || (pstMonster->GetEntityTime() - fPreWalkTime) > 2.0f ){
		bIsCanMove = true;
	}else{
		bIsCanMove = false;
	}


	if(pstMonster->IsWalkable() && !pstMonster->IsWalking() && bIsCanMove){
		unsigned int uParam1 = AI_PARAM_CFG_MASTER->GetAICfgByID(aiID)->aiparams(1);

		if(uParam1 == 1){   //ս���������
			lastY = heroPosY;
			if(heroPosY > disH){
				lastY = disH;
			}
		}else{		//0��ʾս�������ƶ�
			if(heroPosY > pstMonster->getPositionY()){
				lastY = disH;
			}else{
				lastY = 0;
			}
		}


		if(fabs(pstMonster->getPositionY() - lastY) > pstLayer->getGridH()){
			pstMonster->WalkTo(ccp(lastX, lastY));
		}else{
			pstMonster->Idle();
		}

		fPreWalkTime = pstMonster->GetEntityTime();
	}
}

void CInstanceAI::CorrectAtkerPos(CBaseFightScene* pstLayer, CQStageEntity* pstAtk, CQStageEntity* pstDef){
	CCPoint stDist = ccpSub(pstAtk->getPosition(), pstDef->getPosition());
	if(pstAtk->IsWalkable() && !pstAtk->IsWalking() && fabs(stDist.x) < 50 && fabs(stDist.y) < 50){
		CCPoint stMovDst;

		if (stDist.x > 0){
			stMovDst = ccpAdd(pstLayer->getMainHero()->getPosition(), ccp(100, 0));	
		}else{
			stMovDst = ccpAdd(pstLayer->getMainHero()->getPosition(), ccp(-100, 0));
		}

		if(stMovDst.x > pstLayer->getStageRect().getMaxX() - 10){
			stMovDst.x = pstLayer->getStageRect().getMaxX() - 10;
		}else if(stMovDst.x < pstLayer->getStageRect().getMinX() + 10){
			stMovDst.x = pstLayer->getStageRect().getMinX() + 10;
		}

		if(stMovDst.y > pstLayer->getStageRect().getMaxY() - 10){
			stMovDst.y = pstLayer->getStageRect().getMaxY() - 10;
		}else if(stMovDst.y < pstLayer->getStageRect().getMinY() + 10){
			stMovDst.y = pstLayer->getStageRect().getMinY() + 10;
		}

		pstLayer->moveEntity(pstAtk, stMovDst);
	}
}

bool CInstanceAI::CorrectEntityPos(CBaseFightScene* pstLayer, CQStageEntity* pstEntity){
	float posX = 0.0f;
	float posY = 0.0f;
	bool isCorrect = false;
	
	if(pstEntity->getDesiredPosition().x - pstEntity->getCenterToSides() <= pstLayer->getStageRect().getMinX()) {
		posX = pstLayer->getStageRect().getMinX() + pstEntity->getCenterToSides();
		isCorrect = true;
	} else if(pstEntity->getDesiredPosition().x + pstEntity->getCenterToSides() >= pstLayer->getStageRect().getMaxX()) {
		posX = pstLayer->getStageRect().getMaxX() - pstEntity->getCenterToSides();
		isCorrect = true;
	} else {
		posX = pstEntity->getDesiredPosition().x;
	}

	if(pstEntity->getDesiredPosition().y <= pstLayer->getStageRect().getMinY()) {
		posY = pstLayer->getStageRect().getMinY();
	} else if(pstEntity->getDesiredPosition().y >= pstLayer->getStageRect().getMaxY()) {
		posY = pstLayer->getStageRect().getMaxY();
	} else {
		posY = pstEntity->getDesiredPosition().y;
	}

	pstEntity->setPosition(ccp(posX, posY));

	return isCorrect;
}

void CInstanceAI::UpdateHeroAttackBossByAI(CBaseFightScene* pstLayer, CQStageEntity* pstMonster){
	CMainHero* pstHero = dynamic_cast<CMainHero*>(pstMonster);
	CQStageEntity* pstTarget = NULL;	
	//����һ���в���boss����boss Or ��Ӣ����Ļ���������Ѿ�Ĵ�,����boss�Ĺ����У�ͬʱ�ҵ����Լ������С��
	CCObject* pstEntity;
	CCARRAY_FOREACH(pstLayer->getStageEntities(), pstEntity){
		if ((dynamic_cast<CQStageEntity*>(pstEntity)->getRoleType() == ROLE_TYPE_JUNIOR_BOSS
			|| dynamic_cast<CQStageEntity*>(pstEntity)->getRoleType() == ROLE_TYPE_SENIOR_BOSS
			|| dynamic_cast<CQStageEntity*>(pstEntity)->getRoleType() == ROLE_TYPE_ELITE)
			&& (dynamic_cast<CQStageEntity*>(pstEntity)->getRoleParty() == pstMonster->getRoleParty())){
			pstTarget = dynamic_cast<CQStageEntity*>(pstEntity);
			break;
		}else if (dynamic_cast<CQStageEntity*>(pstEntity)->getRoleParty() != pstMonster->getRoleParty()
			&& (!pstTarget || ccpDistance(pstTarget->getPosition(), dynamic_cast<CQStageEntity*>(pstEntity)->getPosition()))){
			pstTarget = dynamic_cast<CQStageEntity*>(pstEntity);
		}
	}


	//�����Ŀ�꿪��
	if (!pstTarget){
		pstHero->WalkWithDirection(ccp(1.0, 0));
		return;
	}

	double tdx = fabs(pstHero->getPositionX() - pstTarget->getPositionX());
	double tdy = fabs(pstHero->getPositionY() - pstTarget->getPositionY());
	int skillID = CInstanceUtils::FindAtkSkill(pstMonster, tdx, tdy);
	if (skillID > 0){
		pstHero->CastSkillWithSkillID(skillID);
		return;
	}

	CCPoint stDist = ccpSub(pstTarget->getPosition(), pstHero->getPosition());
	if (fabs(stDist.x) > pstHero->GetBulletShootDistance()){
		const string stMovement = pstHero->getArmature()->getAnimation()->getCurrentMovementID();
		if ((stDist.x * pstHero->getScaleX() < 0 	|| !pstHero->IsWalking())
			&& pstHero->IsWalkable()){
				pstLayer->moveEntity(pstHero, pstTarget->getPosition());
				//CCLog("2 friend move from %f, %f to %f, %f", pstFriendHero->getPositionX(), pstFriendHero->getPositionY(), pstLayer->getMainHero()->getPositionX(), pstLayer->getMainHero()->getPositionY());
		}

		AutoHeroChangeWeapon(pstLayer, pstMonster);
		pstHero->stateChangeByAttack(true);
		return;
	}else if(fabs(stDist.x) < pstHero->GetBulletShootDistance() / 2){
		if ((stDist.x * pstHero->getScaleX() < 0 || !pstHero->IsWalking())
			&& pstHero->IsWalkable()){
				CCPoint stDest = pstTarget->getPosition();
				if (stDist.x < 0){
					stDest.x += pstHero->GetBulletShootDistance() / 1.5;
				}else{
					stDest.x -= pstHero->GetBulletShootDistance() / 1.5;
				}

				if(stDest.x > pstLayer->getStageRect().getMaxX() - 10){
					stDest.x = pstTarget->getPositionX() - pstHero->GetBulletShootDistance() / 1.5;
				}else if(stDest.x < pstLayer->getStageRect().getMinX() + 10){
					stDest.x = pstTarget->getPositionX() + pstHero->GetBulletShootDistance() / 1.5;
				}

				pstLayer->moveEntity(pstHero, stDest);
				//CCLog("2 friend move from %f, %f to %f, %f", pstFriendHero->getPositionX(), pstFriendHero->getPositionY(), pstLayer->getMainHero()->getPositionX(), pstLayer->getMainHero()->getPositionY());
		}
		//pstHero->stateChangeByAttack(true);
		return;
	}else{
		if (stDist.x * pstHero->getScaleX() < 0 && !pstHero->IsWalking()){
			pstHero->setScaleX(pstHero->getScaleX() * -1);
		}

		AutoHeroChangeWeapon(pstLayer, pstMonster);
		pstHero->stateChangeByAttack(true);
	}
	
}

void CInstanceAI::AutoHeroChangeWeapon(CBaseFightScene* pstLayer, CQStageEntity* pstMonster){
	CMainHero* pstHero = dynamic_cast<CMainHero*>(pstMonster);

	if(!pstHero->getIsBulletLimit() || !pstHero->isShooting()){  //û���ӵ����� OR �������״̬ 
		return;
	}

	if(pstHero->getBulletTime() > 0 || pstLayer->isAllWeaponOutBullet()){  //��ǰǹе��ҩδ�þ� OR ��ҩȫ������
		return;
	}

	pstLayer->autoChangeNextWeapon();
}



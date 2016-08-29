#include "InstanceUtils.h"
#include "game/role/Enemy.h"
#include "game/control/AudioManager.h"
#include "Macros.pb.h"
#include "BattleSysCfgMaster.h"
#include "BattleDefRatioCfgMaster.h"
#include "BulletCfgMaster.h"
#include "role/CCShake.h"
#include "fight/SceneBuff.h"
#include "vlib/CVRand.h"
#include "GlobalTime.h"
#include "math.h"
#include "ConstHelper.h"
#include "AnimationUtils.h"
#include "fight/LevelObj.h"
#include "WeaponCfg.pb.h"
#include "WeaponCfgMaster.h"

pfEffCalculator_t CInstanceUtils::m_stBuffCalculators[BUFF_TYPES_MAX];
bool CInstanceUtils::m_isInited = false;

bool CInstanceUtils::IsEntityInStraightSkillRange(CSkillEffLayer* pstSkill, CQStageEntity* pstEntity){
	if (!pstEntity || !pstSkill || !pstEntity->getArmature() || !pstSkill->getArmature()){
		return false;
	}

	if (pstSkill->getFlyState() == SKILL_ANIMATION_STATE_PREPARE  && 
		(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FOLLOW || pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_FOLLOW)){
		//跟踪类，还在准备期，不用检测冲突
		return false;
	}else if (pstSkill->IsFollowType() && !pstSkill->getTarget()->isEqual(pstEntity)){
		return false;
	}

	if (pstSkill->getFlyState() == SKILL_ANIMATION_STATE_NULL){
		float zdist = fabs(pstSkill->getZalias() - pstEntity->GetFootPos().y);
		if (zdist > pstSkill->m_stCfg.damagezone()){
			//这两个对象的z不再攻击区域内
			return false;
		}
	}
	
	string stCurAction = pstSkill->getArmature()->getAnimation()->getCurrentMovementID();
	CCMovementData* movData = pstSkill->getArmature()->getAnimation()->getAnimationData()->getMovement(stCurAction.c_str());
    if(!movData) {
        return false;
    }
	CCDictionary* stSkillBoneDic = &(movData->movBoneDataDic);
	if (!stSkillBoneDic){
		return false;
	}

	CCDictElement *element = NULL;
	CCDICT_FOREACH(stSkillBoneDic, element){
		CCBone* pstBone1 = pstSkill->getArmature()->getBone(element->getStrKey());
		if (!pstBone1){
			continue;
		}

		CCArray* skillColiArray = pstBone1->getColliderBodyList();
		if (!skillColiArray){
			continue;
		}

		unsigned int frameIdx = pstSkill->getArmature()->getAnimation()->getCurrentFrameIndex();
		CCMovementBoneData *movBoneData = movData->getMovementBoneData(pstBone1->getName().c_str());
		CCFrameData* stFrameData = NULL;
		CCFrameData* pData;
		for(unsigned int i = 0; i < movBoneData->frameList.count(); i++){
			pData = movBoneData->getFrameData(i);
			if(pData->frameID == pstSkill->getCurrentFrameIdx()){
				stFrameData = pData;
				break;
			}
		}

		CCPoint stBonePos;
		if(stFrameData == NULL){
			stBonePos = ccpAdd(ccp(pstBone1->getBoneData()->x * pstSkill->getScaleX(), pstBone1->getBoneData()->y * pstSkill->getScaleY()), pstSkill->getPosition());
		}else{
			stBonePos = ccp(pstBone1->getBoneData()->x, pstBone1->getBoneData()->y) + ccp(stFrameData->x, stFrameData->y);
			stBonePos = ccpAdd(ccp(stBonePos.x * pstSkill->getScaleX(), stBonePos.y * pstSkill->getScaleY()), pstSkill->getPosition());
		}

		CCBone* pstEntityBone = pstEntity->getArmature()->getBone("hurt");
		CCPoint stEntityAnckorPos = pstEntity->GetCenterPos();
		CCArray* entityColiArray = NULL;
		if (pstEntityBone == NULL){
			CCLOG("[CInstanceUtils::IsEntityInStraightSkillRange]::the defender has no hurt bone!!!!!!!!, the armature is %s", pstEntity->getArmature()->getName().c_str());
			return false;
		}
		entityColiArray = pstEntityBone->getColliderBodyList();
		stEntityAnckorPos = ccpAdd(ccp(pstEntityBone->getBoneData()->x, pstEntityBone->getBoneData()->y), pstEntity->getPosition());

		if (skillColiArray && entityColiArray && skillColiArray->count() && entityColiArray->count()){
			for (unsigned int iIdx = 0; iIdx < skillColiArray->count(); iIdx++){
				ColliderBody * body1 = (ColliderBody *)skillColiArray->objectAtIndex(iIdx);
				for (unsigned int jIdx = 0; jIdx < entityColiArray->count(); jIdx++){
					ColliderBody* body2 = (ColliderBody*) entityColiArray->objectAtIndex(jIdx);
					if (DoesContoursCollide(body1->getContourData(),
						body2->getContourData(), 
						stBonePos, 
						stEntityAnckorPos,
						pstSkill->getScaleX() * pstSkill->getBaseScale(),
						pstSkill->getScaleY() * pstSkill->getBaseScale(),
						pstEntity->getScaleX() * pstEntity->getBaseScale(),
						pstEntity->getScaleY() * pstEntity->getBaseScale())){
							return true;
					}
				}
			}
		}
	}

	return false;
}


bool CInstanceUtils::IsEntityInVerticalSkillRange(CSkillEffLayer* pstSkill, CQStageEntity* pstEntity)
{
	if (!pstEntity || !pstSkill || !pstEntity->getArmature() || !pstSkill->getArmature()){
		return false;
	}
	CCPoint stStartPoint = pstSkill->getPosition();
	//根据范围类型来算
	switch (pstSkill->m_stCfg.damageareatype())
	{
	case SKILL_DAMAGE_CIRCLE:
		return IsEntityInCircle(pstEntity, stStartPoint, pstSkill->m_stCfg.effectx());
		break;
	case SKILL_DAMAGE_RECT:		
		if (pstSkill->getArmature()->getAnchorPoint().x == 0 && pstSkill->getArmature()->getAnchorPoint().y == 0.5){
			stStartPoint.x += pstSkill->getContentSize().height / 2 * pstSkill->getScaleX();
		}
		return IsEntityInRect(pstEntity, stStartPoint, pstSkill->m_stCfg.effectx(), pstSkill->m_stCfg.effecty());
		break;
	case SKILL_DAMAGE_CROSS:
		return IsEntityInCross(pstEntity, stStartPoint, pstSkill->m_stCfg.effectx(), pstSkill->m_stCfg.effecty(), pstSkill->m_stCfg.damagezone());
		break;
	case SKILL_DAMAGE_T:
		return IsEntityInT(pstEntity, stStartPoint, pstSkill->m_stCfg.effectx(), pstSkill->m_stCfg.effecty(), pstSkill->m_stCfg.damagezone(), pstSkill->getOwnerEntity()->getScaleX());
		break;
	default:
		return false;
		break;
	} 


	return false;

}

bool CInstanceUtils::IsEntityInSkillEffRange(CSkillEffLayer* pstSkill, CQStageEntity* pstEntity){
	if (!pstEntity || !pstSkill || !pstEntity->getArmature() || !pstSkill->getArmature()){
		return false;
	}

	if (pstEntity->IsImmune()){
		//当前目标不可攻击
		return false;
	}

	bool bResult = false;

	if ( pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_FLY_COLIDE && pstSkill->IsFlying()){
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_AREA){
		return IsEntityInVerticalSkillRange(pstSkill, pstEntity);
	}else if (pstSkill->IsFollowType() && pstSkill->IsChasing() && dynamic_cast<void*>(pstSkill->getTarget()) == dynamic_cast<void*>(pstEntity)){
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_FLY_COLIDE && pstSkill->IsHiting()){
		return IsEntityInVerticalSkillRange(pstSkill, pstEntity);
	}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_ON_GUN_SPREAD){
		bResult = IsEntityInStraightSkillRange(pstSkill, pstEntity);  //二针要求所有枪口出来的特效都按照碰撞区域去计算是否有伤害,他们要敢变卦，就搞死他们
		return bResult;
	}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_ON_GUN_DIRECT){
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);  //二针要求所有枪口出来的特效都按照碰撞区域去计算是否有伤害,他们要干边关，就搞死他们
	}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_SEPERATE_AREA){
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_DIRECT){
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_TRHOWN_TRAP && pstSkill->getFlyState() == SKILL_ANIMATION_STATE_HITING){
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}else if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FLY_TO && pstSkill->getFlyState() == SKILL_ANIMATION_STATE_STANDBY){
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}else if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_FLY_TO_GROUND || pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_DIRECT_ON_GROUND){
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}if ( pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_FLY_DIRECT_COLIDE && pstSkill->IsFlying()){  //技能释放后直线飞行，中途碰撞后消失，未碰撞就穿屏
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}if ( pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_THROWN_COLIDE && pstSkill->IsFlying()){		//抛物线飞行，中途碰撞后消失，未碰撞落地后爆炸
		return IsEntityInStraightSkillRange(pstSkill, pstEntity);
	}

	return false;
}

void CInstanceUtils::SkillCollides(CSkillEffLayer* pstSkill,
								   CCArray* pstEntities,
								   CBaseFightScene* pstFightScene){
	CCObject* pstTarget;
	CCARRAY_FOREACH(pstEntities, pstTarget){
		if (dynamic_cast<CQStageEntity*>(pstTarget)->IsImmune()){
			continue;
		}
		if ( pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_FLY_COLIDE 
			&& pstSkill->IsFlying() 
			&& dynamic_cast<CQStageEntity*>(pstTarget)->getRoleParty() != pstSkill->getOwnerParty()
			&& IsEntityInStraightSkillRange(pstSkill, dynamic_cast<CQStageEntity*>(pstTarget))){
				//播放撞击特效，后续的伤害在特效打击帧触发
			pstSkill->PlayHitAnimation();
			pstSkill->setFlyState(SKILL_ANIMATION_STATE_HITING);
			return;

		}else if (pstSkill->IsFollowType() 
			&& pstSkill->IsChasing() 
			&& dynamic_cast<CQStageEntity*>(pstTarget)->getRoleParty() != pstSkill->getOwnerParty()
			&& IsEntityInStraightSkillRange(pstSkill, dynamic_cast<CQStageEntity*>(pstTarget))){
			//这里要造成实际伤害了。
			SkillMakeEffect(pstSkill, dynamic_cast<CQStageEntity*>(pstTarget), pstFightScene);
			pstSkill->setFlyState(SKILL_ANIMATION_STATE_DONE);
		}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_TRHOWN_TRAP
			&& pstSkill->getFlyState() == SKILL_ANIMATION_STATE_STANDBY
			&& dynamic_cast<CQStageEntity*>(pstTarget)->getRoleParty() != pstSkill->getOwnerParty()
			&& IsEntityInStraightSkillRange(pstSkill, dynamic_cast<CQStageEntity*>(pstTarget))){
			pstSkill->setFlyState(SKILL_ANIMATION_STATE_HITING);
			pstSkill->Boom();
		}else if( pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_FLY_DIRECT_COLIDE 
			&& pstSkill->IsFlying() 
			&& dynamic_cast<CQStageEntity*>(pstTarget)->getRoleParty() != pstSkill->getOwnerParty()
			&& IsEntityInStraightSkillRange(pstSkill, dynamic_cast<CQStageEntity*>(pstTarget))){
				//这里要造成实际伤害了。
				SkillMakeEffect(pstSkill, dynamic_cast<CQStageEntity*>(pstTarget), pstFightScene);
				pstSkill->setFlyState(SKILL_ANIMATION_STATE_DONE);
		}else if( pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_THROWN_COLIDE 
			&& pstSkill->IsFlying() 
			&& dynamic_cast<CQStageEntity*>(pstTarget)->getRoleParty() != pstSkill->getOwnerParty()
			&& IsEntityInStraightSkillRange(pstSkill, dynamic_cast<CQStageEntity*>(pstTarget))){
				//这里要造成实际伤害了。
				SkillMakeEffect(pstSkill, dynamic_cast<CQStageEntity*>(pstTarget), pstFightScene);
				pstSkill->setFlyState(SKILL_ANIMATION_STATE_DONE);
				pstSkill->Boom();
		}
	}
	
}

void CInstanceUtils::SkillAtkEffect(CSkillEffLayer* pstSkill,
									CCArray* pstEntities,
									bool& bIsCleanSkillEff,
									CBaseFightScene* pstFightScene){
	bIsCleanSkillEff = false;
	CQStageEntity* pstMonster;
	//这里要根据技能的类型来判断应该哪些是要作用到对方身上的
	if (pstSkill->m_stCfg.castdist() >= 2){
		for (unsigned int iIdx = 0; iIdx < pstEntities->count(); iIdx++){
			pstMonster = (CQStageEntity*)pstEntities->objectAtIndex(iIdx);
			if (CInstanceUtils::IsEntityInSkillEffRange(pstSkill, dynamic_cast<CQStageEntity*>(pstMonster))){

				CInstanceUtils::SkillMakeEffect(pstSkill, dynamic_cast<CQStageEntity*>(pstMonster), pstFightScene);
				if (pstSkill->IsFollowType()){
					return;
				}
			}
		}
	}
}

void CInstanceUtils::CloseSkillAtkEffect(CQStageEntity* pstAtker,
									CCArray* pstEnimies,
									CBaseFightScene* pstFightScene){
	CQStageEntity* pstMonster;
	CQSkillCell* pstSkill = pstAtker->GetCurSkillCell();
	if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_AREA){
		for (unsigned int iIdx = 0; iIdx < pstEnimies->count(); iIdx++){
			pstMonster = dynamic_cast<CQStageEntity*>(pstEnimies->objectAtIndex(iIdx));
			if (pstMonster->IsImmune()){
				continue;
			}
			if (DoTwoEntityCollide(pstAtker, pstMonster, pstSkill->m_stCfg.effecty())){
				//CloseSkillAtkEffect(pstAtker, pstMonster);
			}
		}
	}else{
		//这个技能只能是打到离自己最近的，在攻击距离上的敌人，是一个方向性攻击技能
		CQStageEntity* pstTarget = NULL;
		float distance = 2000;
		for (unsigned int iIdx = 0; iIdx < pstEnimies->count(); iIdx++)
		{
			pstMonster = dynamic_cast<CQStageEntity*>(pstEnimies->objectAtIndex(iIdx));
			
			if (pstAtker->getRoleParty() == pstMonster->getRoleParty()
				|| pstMonster->IsImmune()){
				continue;
			}
			

			if (pstAtker->getArmature() == NULL || pstMonster->getArmature() == NULL){
				continue;
			}

			pstMonster = dynamic_cast<CQStageEntity*>(pstEnimies->objectAtIndex(iIdx));
			CCPoint stDist = ccpSub(pstMonster->getPosition(), pstAtker->getPosition());
			if (DoTwoEntityCollide(pstAtker, pstMonster, pstSkill->m_stCfg.damagezone()) && pstAtker->getRoleParty() != pstMonster->getRoleParty()){
				float newDist = fabs(stDist.x);
				if ( newDist < distance){
					pstTarget = pstMonster;
					distance = newDist;
				}
			}
			
		}
		if (pstTarget != NULL){
			SkillMakeEffect(pstSkill, pstAtker, pstTarget, pstFightScene);
		}
	}
}


void CInstanceUtils::ActionTriggeredNoAniSkill(CQStageEntity* pstAtker, CCArray* pstEntities, CBaseFightScene* pstFightScene){
	CQSkillCell* pstSkill = pstAtker->GetCurSkillCell();
	if (!pstSkill){
		return;
	}

	switch (pstSkill->m_stCfg.triggertype())
	{
	case SKILL_TRIGGER_ACTION_ONLY_ON_ME:
		//萌大坑说这种技能只会对施法者自己产生效果，对其他人不会有任何关系 2014-06-11
		SkillMakeEffect(pstSkill, pstAtker, pstAtker, pstFightScene);
		break;
	case SKILL_TRIGGER_CLOSE_ATTACK:
		CloseSkillAtkEffect(pstAtker,
			pstEntities,
			pstFightScene);
		break;
	default:
		break;
	}
}

int CInstanceUtils::FindAtkSkill(CQStageEntity* pstAtker, float distX, float distY){
	if (!pstAtker->IsAttackable()){
			return -1;
	}

	if (pstAtker->getAIType() == AI_TYPE_LAST_SKILL_ONLY){
		int iIdx = pstAtker->getSkillArray()->count() - 1;
		if (iIdx >= 0){
			CQSkillCell* pstSkill = (CQSkillCell*) pstAtker->getSkillArray()->objectAtIndex(iIdx);
			if ((pstSkill->m_uLastCastTime + pstSkill->m_stCfg.cd() < pstAtker->GetEntityTime()
				&& pstSkill->m_stCfg.castdist() >= distX
				&& pstSkill->m_stCfg.damagezone() >= distY
				&& pstSkill->m_stCfg.epcost() <= pstAtker->getEP())){
				return pstSkill->m_uSkillID;				
			}
		}
		return -1;
	}
	
	unsigned int iSkillCnt = pstAtker->getSkillArray()->count();
	for (unsigned int iIdx = 0; iIdx < iSkillCnt; iIdx++){
		//这一段应该是给玩家角色的，优先放大招
		CQSkillCell* pstSkill = (CQSkillCell*) pstAtker->getSkillArray()->objectAtIndex(iIdx);
		if (pstSkill->m_stCfg.skillid() > 20000 
			&& pstSkill->m_stCfg.skillid() < 30000
			&& pstSkill->m_uLastCastTime + pstSkill->m_stCfg.cd() < pstAtker->GetEntityTime()){
				if (pstSkill->m_stCfg.epcost() > pstAtker->getEP()){
					return -1;
				}else if (pstSkill->m_stCfg.epcost() <= pstAtker->getEP() 
					&& pstSkill->m_stCfg.castdist() >= distX
					&& pstSkill->m_stCfg.damagezone() >= distY){
						return pstSkill->m_uSkillID;
				}
		}
	}	

	for (unsigned int iIdx = 0; iIdx < iSkillCnt; iIdx++){
		CQSkillCell* pstSkill = (CQSkillCell*) pstAtker->getSkillArray()->objectAtIndex(iIdx);
		if(pstSkill->m_stCfg.epcost() > pstAtker->getEP()){
			continue;
		}

		if (pstSkill->m_stCfg.triggertype() != SKILL_TRIGGER_ON_ME){
			if (pstSkill->m_uLastCastTime + pstSkill->m_stCfg.cd() < pstAtker->GetEntityTime()){
				switch (pstSkill->m_stCfg.castcondtype())
				{
				case EFFECT_CAST_COND_TYPE_LE_HP:
					if (pstAtker->getHP() * 1.0 / pstAtker->getMaxHP() < pstSkill->m_stCfg.castcondvalue() / (RANDOM_DOMINATOR * 1.0)){
						return pstSkill->m_stCfg.skillid();
					}
					break;
				case EFFECT_CAST_COND_TYPE_LE_DIST:
					if (pstSkill->m_stCfg.castcondvalue() >= distX && pstSkill->m_stCfg.damagezone() >= distY){
						return pstSkill->m_stCfg.skillid();
					}
					
					break;
				case EFFECT_CAST_COND_TYPE_GE_DIST:
					if (pstSkill->m_stCfg.castcondvalue() <= distX && pstSkill->m_stCfg.damagezone() >= distY){
						return pstSkill->m_stCfg.skillid();
					}
					
					break;
				case EFFECT_CAST_COND_TYPE_CD_READY:
					return pstSkill->m_stCfg.skillid();
				default:
					break;
				}
			}
		}else{
			if (pstSkill->m_uLastCastTime + pstSkill->m_stCfg.cd() < pstAtker->GetEntityTime()){				
					return pstSkill->m_uSkillID;				
			}
		}
	}

	return -1;
}

bool CInstanceUtils::DoesContoursCollide(CCContourData* contour1, 
										 CCContourData* contour2,
										 CCPoint pos1,
										 CCPoint pos2,
										 float scale1_x,
										 float scale1_y,
										 float scale2_x,
										 float scale2_y){
	if (!contour1 || !contour2){
		return false;
	}

	CCContourVertex2* LeftUp1 = (CCContourVertex2*)contour1->vertexList.objectAtIndex(0);
	CCContourVertex2* LeftDown1 = (CCContourVertex2*)contour1->vertexList.objectAtIndex(1);
	CCContourVertex2* RightDown1 = (CCContourVertex2*)contour1->vertexList.objectAtIndex(2);
	CCContourVertex2* RightUp1 = (CCContourVertex2*)contour1->vertexList.objectAtIndex(3);

	CCContourVertex2* LeftUp2 = (CCContourVertex2*)contour2->vertexList.objectAtIndex(0);
	CCContourVertex2* LeftDown2 = (CCContourVertex2*)contour2->vertexList.objectAtIndex(1);
	CCContourVertex2* RightDown2 = (CCContourVertex2*)contour2->vertexList.objectAtIndex(2);
	CCContourVertex2* RightUp2 = (CCContourVertex2*)contour2->vertexList.objectAtIndex(3);

	CCPoint aPos = ccp((RightUp1->x + LeftUp1->x) / 2, (LeftUp1->y + LeftDown1->y) / 2);
	CCPoint bPos = ccp((RightUp2->x + LeftUp2->x) / 2, (LeftUp2->y + LeftDown2->y) / 2);

	aPos.x *= scale1_x;
	aPos.y *= scale1_y;

	bPos.x *= scale2_x;
	bPos.y *= scale2_y;

	//CCLog("apos %f, %f, bpos %f, %f", aPos.x, aPos.y, bPos.x, bPos.y);
	aPos = ccpAdd(aPos, pos1);
	bPos = ccpAdd(bPos, pos2);
	CCSize aSize = CCSizeMake(RightUp1->x - LeftUp1->x, RightUp1->y - RightDown1->y);
	CCSize bSize = CCSizeMake(RightUp2->x - LeftUp2->x, RightUp2->y - RightDown2->y);

	aSize.width *= scale1_x;
	aSize.height *= scale1_y;

	bSize.width *= scale2_x;
	bSize.height *= scale2_y;

 	//CCLog("pos1 %f, %f, pos2 %f, %f, size1 %f, %f, size2 %f, %f",
 	//	aPos.x, aPos.y, bPos.x, bPos.y,
 	//	aSize.width, aSize.height, bSize.width, bSize.height);

	if(fabs(aPos.x - bPos.x) <= fabs(aSize.width/2) + fabs(bSize.width/2) && fabs(aPos.y - bPos.y) <= fabs(aSize.height/2) + fabs(bSize.height/2))
	{
		return true;
	}
	return false;
}

bool CInstanceUtils::DoTwoEntityCollide(CQStageEntity* pstAtk, CQStageEntity* pstDef, float fDist){
	if (!pstAtk->getArmature() || !pstDef->getArmature()){
		return false;
	}

	float zdist = fabs(pstAtk->getPositionY()) - (pstDef->getPositionY());
	if (zdist > fDist){
		//这两个对象的z不再攻击区域内
		return false;
	}

	//攻击方
	unsigned int curFrameIdx = pstAtk->getArmature()->getAnimation()->getCurrentFrameIndex();
	string atkActName = pstAtk->getArmature()->getAnimation()->getCurrentMovementID();
	CCBone* pstAtkBone = pstAtk->getArmature()->getBone("atk");
	if(pstAtkBone == NULL){
		CCLOG("[CInstanceUtils::DoTwoEntityCollide]::the atker has no atk bone!!!!!!!!, the armature is %s", pstAtk->getArmature()->getName().c_str());
		return false;
	}

	CCArray* atkColiArray = pstAtkBone->getColliderBodyList();
	CCFrameData* atkFrameData = CAnimationUtils::getFrameDataByFrameIdx(pstAtk->getArmature(), atkActName, "atk", 0);  //第一帧的数据
	CCPoint stAtkerPos; 
	CCPoint stBonePos;
	float stAtkScaleX;
	float stAtkScaleY;
	if(atkFrameData == NULL){
		stAtkScaleX = pstAtk->getArmature()->getScaleX();
		stAtkScaleY = pstAtk->getArmature()->getScaleY();
		stAtkerPos = ccpAdd(ccp(pstAtkBone->getBoneData()->x*stAtkScaleX, pstAtkBone->getBoneData()->y*stAtkScaleY), pstAtk->getPosition());
	}else{
		stAtkScaleX = pstAtk->getArmature()->getScaleX();
		stAtkScaleY = pstAtk->getArmature()->getScaleY();
		stBonePos = ccpAdd(ccp(pstAtkBone->getBoneData()->x, pstAtkBone->getBoneData()->y), ccp(atkFrameData->x, atkFrameData->y));
		stAtkerPos = ccpAdd(ccp(stBonePos.x*stAtkScaleX, stBonePos.y*stAtkScaleY), pstAtk->getPosition());
	}

	CCBone* pstHuttBone = pstDef->getArmature()->getBone("hurt");
	if(pstHuttBone == NULL){
		CCLOG("[CInstanceUtils::DoTwoEntityCollide]::the defender has no hurt bone!!!!!!!!, the armature is %s", pstDef->getArmature()->getName().c_str());
		return false;
	}
	CCArray* hurtColiArray = pstHuttBone->getColliderBodyList();

	if (!atkColiArray || !hurtColiArray){
		if (fabs(pstAtk->getPositionY() - pstDef->getPositionY()) <= pstAtk->getDamageRange()
			&& fabs(pstAtk->getPositionX() - pstDef->getPositionX()) < (pstAtk->getContentSize().width / 2 + pstDef->getContentSize().width / 3)){
				return true;
		}
	}else if (atkColiArray->count() && hurtColiArray->count()){
		for (unsigned int iIdx = 0; iIdx < atkColiArray->count(); iIdx++){
			ColliderBody * body1 = (ColliderBody *)atkColiArray->objectAtIndex(iIdx);
			for (unsigned int jIdx = 0; jIdx < hurtColiArray->count(); jIdx++){

				ColliderBody* body2 = (ColliderBody*) hurtColiArray->objectAtIndex(jIdx);
				if (DoesContoursCollide(body1->getContourData(),
					body2->getContourData(), 
					stAtkerPos,
					pstDef->getPosition(),
					stAtkScaleX * pstAtk->getBaseScale(),
					stAtkScaleY * pstAtk->getBaseScale(),
					pstDef->getScaleX() * pstDef->getBaseScale(),
					pstDef->getScaleY() * pstDef->getBaseScale())){
						return true;
				}
			}
		}
	}

	return false;
}


void CInstanceUtils::GetSomeTargets(const CCPoint& stOrig, unsigned int uiParty, CCArray* stObjects, int iNum, float fDist, CCArray* stTargets){
	for (unsigned int iIdx = 0; iIdx < stObjects->count(); iIdx++){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(stObjects->objectAtIndex(iIdx));
		if (!pstEntity || pstEntity->getRoleParty() != uiParty || !pstEntity->getArmature() || pstEntity->IsDead()){
			continue;
		}
		if (ccpDistance(stOrig, pstEntity->getPosition()) <= fDist){
			stTargets->addObject(pstEntity);
			iNum --;
			if (iNum == 0){
				return;
			}
		}		
	}

	return;
}

void CInstanceUtils::SkillMakeEffect(CQSkillCell* pstSkill, 
									 CQStageEntity* pstAtker, 
									 CQStageEntity* pstEntity,
									 CBaseFightScene* pstFightScene){								 
	CEntityFightInfoBrief stAtkBrief;
	BuildEntityFightInfoBrief(stAtkBrief, pstAtker);

	if(!pstSkill->IsCanDamge(pstEntity->m_uID)){  //伤害次数为0
		return;
	}

	int iCurBuffNum = pstFightScene->getBulletSkillBuffNum();
	for (int i = 0; i < pstSkill->m_stCfg.effects_size(); i++){
		iCurBuffNum = pstFightScene->getBulletSkillBuffNum();
		if (!IsTargetType(pstSkill->m_stCfg.effects(i).targettype(), pstAtker->getRoleParty(), pstEntity->getRoleParty(), bool(pstAtker->m_uID == pstEntity->m_uID))){
			continue;
		}
		switch (pstSkill->m_stCfg.effects(i).effecttype())
		{
		case SKILL_EFFECT_HP_ABS_HURT:			
			SkillHPAbsHurtEffect(&pstSkill->m_stCfg, 
				pstSkill->m_stCfg.effects(i).effectval(),
				pstEntity, 
				pstAtker->getScaleX(),
				pstAtker->getPosition(),
				pstFightScene->getCurAniTotalNum());						
			break;
		case SKILL_EFFECT_ADD_BUFF:	
			if(iCurBuffNum < (int)ConstHelper(FIGHT_BULLET_SKILL_ADD_BUFF_MAX)){  //超出当前buff添加数量
				pstEntity->AddBuff(pstSkill->m_stCfg.effects(i).effectval(), pstSkill->m_uID, true);
				iCurBuffNum++;
				pstFightScene->setBulletSkillBuffNum(iCurBuffNum);
			}else{
				pstEntity->AddBuff(pstSkill->m_stCfg.effects(i).effectval(), pstSkill->m_uID, false);
			}
			break;
		case SKILL_EFFECT_HP_RATIO_HURT:
			SkillNormalAtkEffect(&pstSkill->m_stCfg, 
				stAtkBrief, 
				pstSkill->m_stCfg.effects(i).effectval(),
				pstEntity,
				pstAtker->getScaleX(),
				pstAtker->getPosition(),
				pstFightScene->getCurAniTotalNum());						
			break;
		default:
			break;
		}
	}

	pstSkill->DecDamgeCnt(pstEntity->m_uID);
	pstEntity->ShowBeatenAnimation(pstSkill->m_stCfg.hurtanimation());
}


bool CInstanceUtils::IsTargetType(unsigned int uiEffectTargetType, 
								  unsigned int uiOwnerParty,
								  unsigned int uiTargetType,
								  bool isSelf){
	if (uiEffectTargetType == EFFECT_TARGET_TYPE_FRIEND && uiOwnerParty == uiTargetType){
		return true;
	}else if (uiEffectTargetType == EFFECT_TARGET_TYPE_ENIMY && uiOwnerParty != uiTargetType){
		return true;
	}else if (uiEffectTargetType == EFFECT_TARGET_TYPE_ME && isSelf){
		return true;
	}

	return false;
}

void CInstanceUtils::SkillMakeEffect(CSkillEffLayer* pstSkill,
									 CQStageEntity* pstEntity,
									 CBaseFightScene* pstFightScene){
	bool bNeedForceHurtAction = false;

	if(!pstSkill->IsCanDamge(pstEntity->m_uID)){  //伤害次数为0
		return;
	}

	if (reinterpret_cast<uint64>(pstSkill->getOwnerEntity()) == reinterpret_cast<uint64>(pstFightScene->getMainHero())
		&& (pstSkill->m_stCfg.skillid() > 20000 && pstSkill->m_stCfg.skillid() < 30000)){  //大招
		bNeedForceHurtAction = true;
	}

	bool isPlayHurtArm = false;
	int iCurBuffNum = pstFightScene->getBulletSkillBuffNum();
	for (int i = 0; i < pstSkill->m_stCfg.effects_size(); i++){
		iCurBuffNum = pstFightScene->getBulletSkillBuffNum();
		if(isPlayHurtArm == false){
			isPlayHurtArm = IsTargetType(pstSkill->m_stCfg.effects(i).targettype(), pstSkill->getOwnerParty(), 
				pstEntity->getRoleParty(), bool(pstSkill->getOwnerEntity()->m_uID == pstEntity->m_uID));
		}

		if (!IsTargetType(pstSkill->m_stCfg.effects(i).targettype(), pstSkill->getOwnerParty(),
			pstEntity->getRoleParty(), bool(pstSkill->getOwnerEntity()->m_uID == pstEntity->m_uID))){
			continue;
		}

		switch (pstSkill->m_stCfg.effects(i).effecttype())
		{
		case SKILL_EFFECT_HP_ABS_HURT:
			if (bNeedForceHurtAction == true){
				//pstEntity->ForceIdle();
			}
			SkillHPAbsHurtEffect(pstSkill, pstSkill->m_stCfg.effects(i).effectval(), pstEntity, pstFightScene->getCurAniTotalNum());						
			break;
		case SKILL_EFFECT_ADD_BUFF:
			if(iCurBuffNum < (int)ConstHelper(FIGHT_BULLET_SKILL_ADD_BUFF_MAX)){  //超出当前buff添加数量
				pstEntity->AddBuff(pstSkill->m_stCfg.effects(i).effectval(), pstSkill->m_uID, true);
				iCurBuffNum++;
				pstFightScene->setBulletSkillBuffNum(iCurBuffNum);
			}else{
				pstEntity->AddBuff(pstSkill->m_stCfg.effects(i).effectval(), pstSkill->m_uID, false);
			}
			break;
		case SKILL_EFFECT_HP_RATIO_HURT:
			if (bNeedForceHurtAction == true && !pstEntity->IsForwardDashing()){
				//pstEntity->ForceIdle();
			}
			SkillNormalAtkEffect(pstSkill, pstSkill->m_stCfg.effects(i).effectval(), pstEntity, pstFightScene->getCurAniTotalNum());						
			break;
		case SKILL_EFFECT_ADD_GROUND_BUFF:
			//给地面加buff，需要调用地面接口，我草。。。。这里没有地面接口。。。
			break;
		default:
			break;
		}
	}

	pstSkill->DecDamgeCnt(pstEntity->m_uID);  //技能伤害次数减一

	//技能是否会对地表添加buff
	if(pstSkill->m_stCfg.floorbuffid() != 0){
		pstFightScene->addBuffBySkill(pstSkill, pstEntity->getPosition());
	}

	if(isPlayHurtArm){
		pstEntity->ShowBeatenAnimation(pstSkill->m_stCfg.hurtanimation());
	}

	if (pstSkill->m_stCfg.hitsound().length() && strcmp(pstSkill->m_stCfg.hitsound().c_str(), "null")){
		AudioManager::playEffect(pstSkill->m_stCfg.hitsound().c_str());
	}

	if (pstEntity->IsDead()){  
		//策划说这个不要了，2014-9-17

		//int iAngle = random_range(-90, 90);
		//if (abs(iAngle) < 30){
		//	iAngle = abs(iAngle) + 30;
		//}else if (pstEntity->getPositionY() < 200){
		//	iAngle = 45;
		//}
		//float x = cos(CC_DEGREES_TO_RADIANS(iAngle)) * 1500 * pstSkill->getScaleX();
		//float y = sin(CC_DEGREES_TO_RADIANS(iAngle)) * 1500 * pstSkill->getScaleY();
		//pstEntity->setVelocity(ccp(x, y));
	}
}

void CInstanceUtils::SkillHPAbsHurtEffect(CSkillEffLayer* pstSkill, int value, CQStageEntity* pstTarget, int iCurAniEffNum){	
	SkillHPAbsHurtEffect(&pstSkill->m_stCfg, value, pstTarget, pstSkill->getScaleX(), pstSkill->getPosition(), iCurAniEffNum);
}

void CInstanceUtils::SkillHPAbsHurtEffect(const SkillCfg* pstSkill, 
										  int value,
										  CQStageEntity* pstTarget, 
										  int iBackScale,
										  const CCPoint& stSkillPos,
										  int iCurAniEffNum){
	CCPoint stHitBackPos;
	if (iBackScale > 0){
		stHitBackPos.x = pstSkill->hitbackdist();
	}else{
		stHitBackPos.x = pstSkill->hitbackdist() * -1.0;
	}
	stHitBackPos.y = 0;
	CHurtDamageObj stHurtDamageObj;
	stHurtDamageObj.fDamage = value * 1.0;
	stHurtDamageObj.pstSkillCfg = pstSkill;
	stHurtDamageObj.stHitBackPos = stHitBackPos;
	stHurtDamageObj.stSkillPos = stSkillPos;	
	stHurtDamageObj.iCurAniEffNum = iCurAniEffNum;
	pstTarget->HurtWithDamage(stHurtDamageObj);
}

void CInstanceUtils::SkillNormalAtkEffect(CSkillEffLayer* pstSkill, int iRatio, CQStageEntity* pstTarget, int iCurAniEffNum){
	SkillNormalAtkEffect(&pstSkill->m_stCfg, 
		pstSkill->m_stOwnerBrief, 
		iRatio, 
		pstTarget, 
		pstSkill->getScaleX(),
		pstSkill->getPosition(),
		iCurAniEffNum);
}

void CInstanceUtils::SkillNormalAtkEffect(const SkillCfg* pstSkill, 
										  const CEntityFightInfoBrief& stAtkBrief, 
										  int ratio, 
										  CQStageEntity* pstTarget, 
										  int iBackScale,
										  const CCPoint& stSkillPos,
										  int iCurAniEffNum){
	CCPoint stHitBackPos;
	if (iBackScale > 0){
		stHitBackPos.x = pstSkill->hitbackdist();
	}else{
		stHitBackPos.x = pstSkill->hitbackdist() * -1.0;
	}
	stHitBackPos.y = 0;
	unsigned int uiDamage = GetBasicDamage(stAtkBrief, pstTarget);
	CHurtDamageObj stHurtDamageObj;
	stHurtDamageObj.fDamage = (float)(uiDamage * 1.0 * ratio / RANDOM_DOMINATOR);
	stHurtDamageObj.pstSkillCfg = pstSkill;
	stHurtDamageObj.stHitBackPos = stHitBackPos;
	stHurtDamageObj.stSkillPos = stSkillPos;	
	stHurtDamageObj.iCurAniEffNum = iCurAniEffNum;
	pstTarget->HurtWithDamage(stHurtDamageObj);
}

unsigned int CInstanceUtils::GetGunDamage(const CQStageEntity* pstAtker, const CQStageEntity* pstDefer){
	BattleSysCfg* pstBattleSysCfg = BATTLE_SYS_CFG_MASTER->GetBattleCfgByID(1);
	if (!pstBattleSysCfg){
		return 0;
	}

	unsigned int uAtkMagicType =  pstAtker->GetAtkMagicType();

	unsigned int uiArmorRatioCfgID = BuildArmorRatioCfgID(pstAtker->GetAtkType(), pstDefer->GetArmorType());
	BattleDefRatioCfg* pstArmorRatioCfg = BATTLE_DEF_RATIO_CFG_MASTER->GetBattleDefRatioCfgByID(uiArmorRatioCfgID);
	if (!pstArmorRatioCfg){
		return 0;
	}


	float bRatio = 1.0 + (pstAtker->GetMagicAtkRatio(uAtkMagicType) - pstDefer->GetMagicDefRatio(uAtkMagicType)) / (pstBattleSysCfg->damageadjustratio() * 1.0 * RANDOM_DOMINATOR);
	if (bRatio <= 0.5){
		bRatio = 0.5;
	}else if (bRatio >= 2.0){
		bRatio = 2.0;
	}

	int iLevelDelta = pstAtker->getLevel() - pstDefer->getLevel();
	if (iLevelDelta < -18){
		iLevelDelta = -18;
	}
	float fLevelRatio = 1 + iLevelDelta / 20.0;

	int64 damage = int64(pstAtker->getEffAtk()
		* (pstBattleSysCfg->targetdefadjust() / (1.0 * pstDefer->getEffDef() + pstBattleSysCfg->targetdefadjust())) 
		* pstArmorRatioCfg->damageratio() 
		* bRatio
		/ RANDOM_DOMINATOR
		* fLevelRatio);

	//二针说这里永远不会成为负数，立贴为证，要是那天他丫的把这个最后的*后面部分做出负数了，就照他脸抽丫的！ -- 2014-03-29 16：52

	return  damage;
}

unsigned int CInstanceUtils::GetBasicDamage(const CEntityFightInfoBrief& pstAtker, const CQStageEntity* pstDefer){
	BattleSysCfg* pstBattleSysCfg = BATTLE_SYS_CFG_MASTER->GetBattleCfgByID(1);
	if (!pstBattleSysCfg){
		return 0;
	}

	if (pstAtker.m_uAttackMagicType >= MAX_MAGIC_CNT){
		return 0;
	}

	unsigned int uAtkMagicType =  pstAtker.m_uAttackMagicType;

	unsigned int uiArmorRatioCfgID = BuildArmorRatioCfgID(pstAtker.m_uAttackType, pstDefer->GetArmorType());
	BattleDefRatioCfg* pstArmorRatioCfg = BATTLE_DEF_RATIO_CFG_MASTER->GetBattleDefRatioCfgByID(uiArmorRatioCfgID);
	if (!pstArmorRatioCfg){
		return 0;
	}

	float bRatio = 1.0 + (pstAtker.m_stMagicAtkRatio[uAtkMagicType] - pstDefer->GetMagicDefRatio(uAtkMagicType)) / (pstBattleSysCfg->damageadjustratio() * 1.0 * RANDOM_DOMINATOR);
	if (bRatio <= 0.5){
		bRatio = 0.5;
	}else if (bRatio >= 2.0){
		bRatio = 2.0;
	}

	int iLevelDelta = pstAtker.m_iLevel - pstDefer->getLevel();
	if (iLevelDelta < -18){
		iLevelDelta = -18;
	}
	float fLevelRatio = 1 + iLevelDelta / 20.0;

	int64 damage = int64(pstAtker.m_uEffAtk
		* (pstBattleSysCfg->targetdefadjust() / (1.0 * pstDefer->getEffDef() + pstBattleSysCfg->targetdefadjust())) 
		* pstArmorRatioCfg->damageratio() 
		* bRatio
		/ RANDOM_DOMINATOR
		* fLevelRatio);

	//二针说这里永远不会成为负数，立贴为证，要是那天他丫的把这个最后的*后面部分做出负数了，就照他脸抽丫的！ -- 2014-03-29 16：52

	return  damage;
}

unsigned int CInstanceUtils::BuildArmorRatioCfgID(unsigned int uAtkType, unsigned int uArmorType){
	return uAtkType * 10000 + uArmorType * 100;
}

bool CInstanceUtils::IsEntityInCircle(CQStageEntity* pstEntity, const CCPoint& stPos, unsigned int uRadius){
	
	return ccpDistance(pstEntity->getPosition(), stPos) < uRadius;
}

bool CInstanceUtils::IsEntityInCross(CQStageEntity* pstEntity, const CCPoint& stPos, unsigned int x, unsigned int y, unsigned int uDamageZone){
	CCPoint stDist = ccpSub(pstEntity->getPosition(), stPos);
	return (fabs(stDist.x) <= x / 2.0 && fabs(stDist.y) < uDamageZone / 2.0) || ( fabs(stDist.y) < y / 2.0 && fabs(stDist.x) < uDamageZone / 2.0);
}

bool CInstanceUtils::IsEntityInRect(CQStageEntity* pstEntity, const CCPoint& stPos, unsigned int x, unsigned int y){
	CCPoint stDist = ccpSub(pstEntity->getPosition(), stPos);
	return fabs(stDist.x) <= x / 2.0 && fabs(stDist.y) <= y / 2.0;
}

bool CInstanceUtils::IsEntityInT(CQStageEntity* pstEntity, const CCPoint& stPos, unsigned int x, unsigned int y, unsigned int uDamageZone, float iXscale){
	CCPoint stDist = ccpSub(pstEntity->getPosition(), stPos);
	return (stDist.x * iXscale <= x / 2.0 && stDist.x * iXscale >= 0 && fabs(stDist.y) < uDamageZone / 2.0) || ( fabs(stDist.y) < y / 2.0 && fabs(stDist.x) < uDamageZone / 2.0);;
}

int CInstanceUtils::AddEffect(CQStageEntity* pstEntity, unsigned int uiEffID, unsigned int uiEffVal, unsigned int uiEffType, bool isAddVal){
	if (m_isInited == false){
		m_stBuffCalculators[BUFF_CONT_CUR_HP_BY_RATIO] = AddContHpByCurRatio;
		m_stBuffCalculators[BUFF_DEF_BY_RATIO] = AddDefByRatio;
		m_stBuffCalculators[BUFF_ATK_BY_RATIO] = AddAtkByRatio;
		m_stBuffCalculators[BUFF_WALK_SPEED_BY_RATIO] = AddWalkSpeedByRatio;
		m_stBuffCalculators[BUFF_DASH_SPEED_BY_RATIO] = AddDashSpeedByRatio;
		m_stBuffCalculators[BUFF_HP_MAX_BY_RATIO] = AddMaxHpByRatio;
		m_stBuffCalculators[BUFF_ICE_ATK_BY_RATIO] = AddIceAtkByRatio;
		m_stBuffCalculators[BUFF_FIRE_ATK_BY_RATIO] = AddFireAtkByRatio;
		m_stBuffCalculators[BUFF_ICE_DEF_BY_RATIO] = AddIceAtkByRatio;

		m_stBuffCalculators[BUFF_FIRE_DEF_BY_RATIO] = AddFireDefByRatio;
		m_stBuffCalculators[BUFF_POISON_ATK_BY_RATIO] = AddPoisonAtkByRatio;

		m_stBuffCalculators[BUFF_POISON_DEF_BY_RATIO] = AddPoisonDefByRatio;
		m_stBuffCalculators[BUFF_BIOTIC_ATK_BY_RATIO] = AddBioticAtkByRatio;

		m_stBuffCalculators[BUFF_BIOTIC_DEF_BY_RATIO] = AddBioticDefByRatio;
		m_stBuffCalculators[BUFF_BULLET_DEPTH_BY_RATIO] = AddBulletDepthByRatio;
		m_stBuffCalculators[BUFF_BULLET_LIGHT_BY_RATIO] = AddBulletLightAtkByRatio;
		m_stBuffCalculators[BUFF_CONT_MAX_HP_BY_RATIO] = AddContHPByAbs;
		m_stBuffCalculators[BUFF_CONT_CUR_HP_BY_ABS] = AddContHPByAbs;
		m_stBuffCalculators[BUFF_DIZZY] = AddDizzyBuff;
		m_stBuffCalculators[BUFF_IMMOBILIZE] = AddImmobilizeBuff;
		m_stBuffCalculators[BUFF_BLIND] = AddBlindBuff;
		m_stBuffCalculators[BUFF_INVINCIBLE] = AddInvincibleBuff;
		m_stBuffCalculators[BUFF_ADD_BULLET_DAMAGE] = AddBulletDamageAdjBuff;
		m_stBuffCalculators[BUFF_ADD_HP_BY_FORMULA] = AddMaxHpByFormula;
		m_isInited = true;
	}

	if (uiEffID >= BUFF_TYPES_MAX){
		return -1;
	}

	CBuffEffObj stEffObj;
	stEffObj.pstEntity = pstEntity;
	stEffObj.uiVal = uiEffVal;
	stEffObj.uiEffType = uiEffType;
	stEffObj.isAddVal = isAddVal;
	stEffObj.isAddBuff = true;

	return m_stBuffCalculators[uiEffID](stEffObj);

}

int CInstanceUtils::RemoveEffect(CQStageEntity* pstEntity, unsigned int uiEffID, unsigned int uiEffVal, unsigned int uiEffType, bool isAddVal){
	if (m_isInited == false){
		return -1;
	}

	if (uiEffID >= BUFF_TYPES_MAX){
		return -1;
	}

	CBuffEffObj stEffObj;
	stEffObj.pstEntity = pstEntity;
	stEffObj.uiVal = uiEffVal;
	stEffObj.uiEffType = uiEffType;
	stEffObj.isAddVal = isAddVal;
	stEffObj.isAddBuff = false;

	return m_stBuffCalculators[uiEffID](stEffObj);

}

int CInstanceUtils::AddMaxHpByFormula(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}

	unsigned int levelID = P_LEVEL_OBJ->getEnterLevelID();
	unsigned int targetMagicDef = stEffObj.pstEntity->GetMagicDefRatio(stEffObj.uiEffType);
	float defDamge = 1.0*(levelID*4 - targetMagicDef)/603;
	float levelDamge = (levelID - stEffObj.pstEntity->getLevel());

	if(defDamge < 0.5){
		defDamge = 0.5;
	}else if(defDamge > 1){
		defDamge = 1;
	}

	if(levelDamge < -18){
		levelDamge = -18;
	}else if(levelDamge > 0){
		levelDamge = 0;
	}

	float baseDamge = stEffObj.uiVal*(1 - (1.0*stEffObj.pstEntity->getEffDef())/(stEffObj.pstEntity->getEffDef() + 1006)) * 
		(1 + defDamge) * (1 + 1.0*(levelDamge)/20);

	CCLOG("the base damge is %f", baseDamge);

	if (stEffObj.isAddBuff){
		stEffObj.pstEntity->AddHP((int)baseDamge, stEffObj.isAddVal);
	}else{
		stEffObj.pstEntity->AddHP((int)baseDamge, !stEffObj.isAddVal);
	}

	return 0;
}

int CInstanceUtils::AddAtkByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddAtkRatio(stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecAtkRatio(stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecAtkRatio(uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddAtkRatio(uDecVal);
		}
	}

	return 0;	
}

int CInstanceUtils::AddContHpByCurRatio(CBuffEffObj stEffObj){
	return 0;
}

int CInstanceUtils::AddContHpByMaxRatio(CBuffEffObj stEffObj){
	return 0;
}

int CInstanceUtils::AddMaxHpByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddHpMaxByRatio(stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecHpMaxByRatio(stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecHpMaxByRatio(uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddHpMaxByRatio(uDecVal);
		}
	}

	return 0 ;
}
int CInstanceUtils::AddDefByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}	

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddDefRatio(stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecDefRatio(stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecDefRatio(uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddDefRatio(uDecVal);
		}
	}

	return 0;	
}

int CInstanceUtils::AddIceDefByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}
	

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddMagicDefRatio(ICE_MAGIC, stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecMagicDefRatio(ICE_MAGIC, stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecMagicDefRatio(ICE_MAGIC, uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddMagicDefRatio(ICE_MAGIC, uDecVal);
		}
	}

	return 0;
}
int CInstanceUtils::AddIceAtkByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}
	
	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddMagicAtkRatio(ICE_MAGIC, stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecMagicAtkRatio(ICE_MAGIC, stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecMagicAtkRatio(ICE_MAGIC, uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddMagicAtkRatio(ICE_MAGIC, uDecVal);
		}
	}
	return 0;
}
int CInstanceUtils::AddFireDefByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}
	
	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddMagicDefRatio(FLAME_MAGIC, stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecMagicDefRatio(FLAME_MAGIC, stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecMagicDefRatio(FLAME_MAGIC, uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddMagicDefRatio(FLAME_MAGIC, uDecVal);
		}
	}

	return 0;
}
int CInstanceUtils::AddFireAtkByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}
	
	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddMagicAtkRatio(FLAME_MAGIC, stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecMagicAtkRatio(FLAME_MAGIC, stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecMagicAtkRatio(FLAME_MAGIC, uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddMagicAtkRatio(FLAME_MAGIC, uDecVal);
		}
	}

	return 0;
}
int CInstanceUtils::AddPoisonDefByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}	

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddMagicDefRatio(POISON_MAGIC, stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecMagicDefRatio(POISON_MAGIC, stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecMagicDefRatio(POISON_MAGIC, uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddMagicDefRatio(POISON_MAGIC, uDecVal);
		}
	}

	return 0;
}
int CInstanceUtils::AddPoisonAtkByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddMagicAtkRatio(POISON_MAGIC, stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecMagicAtkRatio(POISON_MAGIC, stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecMagicAtkRatio(POISON_MAGIC, uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddMagicAtkRatio(POISON_MAGIC, uDecVal);
		}
	}

	return 0;
}
int CInstanceUtils::AddBioticDefByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddMagicDefRatio(BIOTECH_MAGIC, stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecMagicDefRatio(BIOTECH_MAGIC, stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecMagicDefRatio(BIOTECH_MAGIC, uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddMagicDefRatio(BIOTECH_MAGIC, uDecVal);
		}
	}

	return 0;
}
int CInstanceUtils::AddBioticAtkByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}	

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddMagicAtkRatio(BIOTECH_MAGIC, stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecMagicAtkRatio(BIOTECH_MAGIC, stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecMagicAtkRatio(BIOTECH_MAGIC, uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddMagicAtkRatio(BIOTECH_MAGIC, uDecVal);
		}
	}

	return 0;
}
int CInstanceUtils::AddDashSpeedByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}	

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddDashSpeedByRatio(stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecDashSpeedByRatio(stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecDashSpeedByRatio(uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddDashSpeedByRatio(uDecVal);
		}
	}

	return 0;
}
int CInstanceUtils::AddWalkSpeedByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}

	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddWalkSpeedByRatio(stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecWalkSpeedByRatio(stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecWalkSpeedByRatio(uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddWalkSpeedByRatio(uDecVal);
		}
	}
	return 0;
}
int CInstanceUtils::AddBulletDepthByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}


	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddBulletDepth(stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecBulletDepth(stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecBulletDepth(uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddBulletDepth(uDecVal);
		}
	}

	return 0;
}

int CInstanceUtils::AddBulletLightAtkByRatio(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return -1;
	}
	if (stEffObj.isAddBuff){
		if (stEffObj.isAddVal){
			stEffObj.pstEntity->AddBulletLight(stEffObj.uiVal);
		}else{
			stEffObj.pstEntity->DecBulletLight(stEffObj.uiVal);
		}
	}else{
		if (stEffObj.isAddVal){
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR + stEffObj.uiVal));
			stEffObj.pstEntity->DecBulletLight(uDecVal);
		}else{
			unsigned int uDecVal = (unsigned int)(stEffObj.uiVal * 1.0 * RANDOM_DOMINATOR / (RANDOM_DOMINATOR - stEffObj.uiVal));
			stEffObj.pstEntity->AddBulletLight(uDecVal);
		}
	}

	return 0;
}

int CInstanceUtils::AddContHPByAbs(CBuffEffObj stEffObj){
	return 0;
}

int CInstanceUtils::AddDizzyBuff(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return 0;
	}

	if (stEffObj.isAddBuff){
		stEffObj.pstEntity->Dizzy();
	}else{
		stEffObj.pstEntity->Idle();
	}

	return 0;

}

int CInstanceUtils::AddBlindBuff(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return 0;
	}

	if (stEffObj.isAddBuff){
		stEffObj.pstEntity->Blind();
	}else{
		stEffObj.pstEntity->Idle();
	}

	return 0;
}


int CInstanceUtils::AddImmobilizeBuff(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return 0;
	}

	if (stEffObj.isAddBuff){
		stEffObj.pstEntity->Immobilize();
	}else{
		stEffObj.pstEntity->Idle();
	}

	return 0;
}


int CInstanceUtils::AddInvincibleBuff(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return 0;
	}

	if (stEffObj.isAddBuff){
		stEffObj.pstEntity->setIsInvincible(true);

	}else{
		stEffObj.pstEntity->setIsInvincible(false);
	}

	return 0;
}

int CInstanceUtils::AddBulletDamageAdjBuff(CBuffEffObj stEffObj){
	if (!stEffObj.pstEntity){
		return 0;
	}

	if (stEffObj.isAddBuff){
		stEffObj.pstEntity->SetBulletDamageAdj(stEffObj.uiVal);

	}else{
		stEffObj.pstEntity->UnSetBulleteDamageAdj();
	}

	return 0;
}


bool CInstanceUtils::IsBulletHitEntity(CBullet* pstBullet, unsigned int uiParty, CQStageEntity* pstTarget){
	if (!pstTarget->getParent() 
		|| pstTarget->getRoleParty() != uiParty 
		|| !pstTarget->getArmature()
		|| pstTarget->IsDead()){
		return false;
	}
	
	float x = pstTarget->getPositionX() - pstBullet->getPositionX();
	float xdist = fabs(x);
	if (pstBullet->getTrail() != TRA_CON_SPREAD
		&& pstTarget->getContentSize().width * fabs(pstTarget->getScaleX()) + pstBullet->getContentSize().width * fabs(pstBullet->getScaleX()) < xdist){
		return false;
	}

	float zdist = fabs(pstBullet->getZalias() - (pstTarget->GetFootPos().y));
	if (zdist > pstBullet->getDamageZone() * pstBullet->getScaleY()){
		//这两个对象的z不再攻击区域内
		return false;
	}

	string boneName1 = pstBullet->getBullet()->getAnimation()->getCurrentMovementID();
	string boneName2 = pstTarget->getArmature()->getAnimation()->getCurrentMovementID();
	if (boneName1.length() == 0 || boneName2.length() == 0){
		return false;
	}

	CCArray* entity1ColiArray = NULL;
	CCPoint stBulletAnckorPos = pstBullet->getPosition();
	CCBone* pstBulletBone = NULL;
	if (pstBullet->getBullet()->getBone(boneName1.c_str())){
		entity1ColiArray = pstBullet->getBullet()->getBone(boneName1.c_str())->getColliderBodyList();
		pstBulletBone = pstBullet->getBullet()->getBone(boneName1.c_str());
	}else if(pstBullet->getBullet()->getBone("body")) {
		entity1ColiArray = pstBullet->getBullet()->getBone("body")->getColliderBodyList();
		pstBulletBone = pstBullet->getBullet()->getBone("body");
	}
	
	if (pstBulletBone){
		stBulletAnckorPos = ccpAdd(ccp(pstBulletBone->getBoneData()->x  * pstBullet->getScaleX(), pstBulletBone->getBoneData()->y  * pstBullet->getScaleY()),
			pstBullet->getPosition());
	}

	CCArray* entity2ColiArray = NULL;
	CCPoint stTargetAnckorPos = pstTarget->GetCenterPos();
	CCBone* pstTargetBone = NULL;
	if(pstTarget->getArmature()->getBone("hurt")) {
		entity2ColiArray = pstTarget->getArmature()->getBone("hurt")->getColliderBodyList();
		pstTargetBone = pstTarget->getArmature()->getBone("hurt");
	}else if (pstTarget->getArmature()->getBone(boneName2.c_str())){
		entity2ColiArray = pstTarget->getArmature()->getBone(boneName2.c_str())->getColliderBodyList();
		pstTargetBone = pstTarget->getArmature()->getBone(boneName2.c_str());
	}

	if (pstTargetBone){
		stTargetAnckorPos = ccpAdd(ccp(pstTargetBone->getBoneData()->x, pstTargetBone->getBoneData()->y), pstTarget->getPosition());
	}
	
	if (entity1ColiArray && entity2ColiArray && entity1ColiArray->count() && entity2ColiArray->count()){
		for (unsigned int iIdx = 0; iIdx < entity1ColiArray->count(); iIdx++){
			ColliderBody * body1 = (ColliderBody *)entity1ColiArray->objectAtIndex(iIdx);
			for (unsigned int jIdx = 0; jIdx < entity2ColiArray->count(); jIdx++){

				ColliderBody* body2 = (ColliderBody*) entity2ColiArray->objectAtIndex(jIdx);
				if (DoesContoursCollide(body1->getContourData(),
						body2->getContourData(), 
						stBulletAnckorPos, 
						stTargetAnckorPos,
						pstBullet->getScaleX(),
						pstBullet->getScaleY(),
						pstTarget->getScaleX() * pstTarget->getBaseScale(),
						pstTarget->getScaleY() * pstTarget->getBaseScale())){
					return true;
				}
			}
		}
	}

	return false;
}

bool CInstanceUtils::IsBulletHitItem(CBullet* pstBullet, CSceneItem* pstItem){
	float x = pstItem->getPositionX() - pstBullet->getPositionX();
	float xdist = fabs(x);
	if (pstBullet->getTrail() != TRA_CON_SPREAD
		&& pstItem->getContentSize().width * fabs(pstItem->getScaleX()) + pstBullet->getContentSize().width * fabs(pstBullet->getScaleX()) < xdist){
			return false;
	}

	float zdist = fabs(pstBullet->getZalias() - (pstItem->getPositionY()));
	if (zdist > pstBullet->getDamageZone() * pstBullet->getScaleY()){
		//这两个对象的z不再攻击区域内
		return false;
	}

	string boneName1 = pstBullet->getBullet()->getAnimation()->getCurrentMovementID();
	string boneName2 = pstItem->getArmature()->getAnimation()->getCurrentMovementID();
	if (boneName1.length() == 0 || boneName2.length() == 0){
		return false;
	}

	CCArray* entity1ColiArray = NULL;
	CCPoint stBulletAnckorPos = pstBullet->getPosition();
	CCBone* pstBulletBone = NULL;
	if (pstBullet->getBullet()->getBone(boneName1.c_str())){
		entity1ColiArray = pstBullet->getBullet()->getBone(boneName1.c_str())->getColliderBodyList();
		pstBulletBone = pstBullet->getBullet()->getBone(boneName1.c_str());
	}else if(pstBullet->getBullet()->getBone("body")) {
		entity1ColiArray = pstBullet->getBullet()->getBone("body")->getColliderBodyList();
		pstBulletBone = pstBullet->getBullet()->getBone("body");
	}

	if (pstBulletBone){
		stBulletAnckorPos = ccpAdd(ccp(pstBulletBone->getBoneData()->x, pstBulletBone->getBoneData()->y), pstBullet->getPosition());
	}

	CCArray* entity2ColiArray = NULL;
	CCPoint stTargetAnckorPos = pstItem->getPosition();
	CCBone* pstTargetBone = NULL;
	if(pstItem->getArmature()->getBone("body")) {
		entity2ColiArray = pstItem->getArmature()->getBone("body")->getColliderBodyList();
		pstTargetBone = pstItem->getArmature()->getBone("body");
	}

	if (pstTargetBone){
		stTargetAnckorPos = ccpAdd(ccp(pstTargetBone->getBoneData()->x, pstTargetBone->getBoneData()->y), pstItem->getPosition());
	}

	if (entity1ColiArray && entity2ColiArray && entity1ColiArray->count() && entity2ColiArray->count()){
		for (unsigned int iIdx = 0; iIdx < entity1ColiArray->count(); iIdx++){
			ColliderBody * body1 = (ColliderBody *)entity1ColiArray->objectAtIndex(iIdx);
			for (unsigned int jIdx = 0; jIdx < entity2ColiArray->count(); jIdx++){

				ColliderBody* body2 = (ColliderBody*) entity2ColiArray->objectAtIndex(jIdx);
				if (DoesContoursCollide(body1->getContourData(),
					body2->getContourData(), 
					pstBullet->getPosition(), 
					stTargetAnckorPos,
					pstBullet->getScaleX(),
					pstBullet->getScaleY(),
					pstItem->getScaleX(),
					pstItem->getScaleY())){
						return true;
				}
			}
		}
	}

	return false;
}

bool CInstanceUtils::IsCanHit(unsigned int hit, unsigned int atkLev, unsigned int defLev){
	if(hit <= 0){  //怪物命中为0，即全中
		return true;
	}

	unsigned int hitVal;
	if(hit > 9500){
		hitVal = 9500;
	}else if(hit < 9000){
		hitVal = 9000;
	}else{
		hitVal = hit;
	}

	int realHit = hitVal*(1 + (atkLev - defLev)*1.0/20.0f);
	if(realHit > 9500){
		realHit = 9500;
	}else if(realHit < 7000){
		realHit = 7000;
	}

	//随机
	int randHit = CRandom::GetRandHaveMax(EGAME_DENOM);
	if(randHit <= realHit){
		return true;
	}else{
		return false;
	}
}

int CInstanceUtils::CalBulletDamage(CBullet* pstBullet, CQStageEntity* pstTarget){
	if(!IsCanHit(pstBullet->getHit(), pstBullet->getOwnerLevel(), pstTarget->getLevel())){  //未命中
		pstTarget->ShowMiss();		//展示miss
		return 0;
	}

	BattleSysCfg* pstBattleSysCfg = BATTLE_SYS_CFG_MASTER->GetBattleCfgByID(1);
	if (!pstBattleSysCfg){
		return -1;
	}

	unsigned int uAtkMagicType =  pstBullet->getAtkMagic();

	unsigned int uiArmorRatioCfgID = BuildArmorRatioCfgID(pstBullet->getAtkType(), pstTarget->GetArmorType());
	BattleDefRatioCfg* pstArmorRatioCfg = BATTLE_DEF_RATIO_CFG_MASTER->GetBattleDefRatioCfgByID(uiArmorRatioCfgID);
	if (!pstArmorRatioCfg){
		return -1;
	}

	float bRatio = 1.0 + (pstBullet->getAtkRatio() - pstTarget->GetMagicDefRatio(uAtkMagicType)) / (pstBattleSysCfg->damageadjustratio() * 1.0 * RANDOM_DOMINATOR);
	if (bRatio <= 0.5){
		bRatio = 0.5;
	}else if (bRatio >= 2.0){
		bRatio = 2.0;
	}

	int iLevelDelta = pstBullet->getOwnerLevel() - pstTarget->getLevel();
	if (iLevelDelta < -18){
		iLevelDelta = -18;
	}
	float fLevelRatio = 1 + iLevelDelta / 20.0;


	int64 damage = int64(pstBullet->getAtkVal()
		* (pstBattleSysCfg->targetdefadjust() / (1.0 * pstTarget->getEffDef() + pstBattleSysCfg->targetdefadjust())) 
		* pstArmorRatioCfg->damageratio() 
		* bRatio * fLevelRatio
		/ RANDOM_DOMINATOR);

	//除以子弹特效中的伤害帧事件数量
	damage = damage / pstBullet->getEvtCnt();

	//二针说这里永远不会成为负数，立贴为证，要是那天他丫的把这个最后的*后面部分做出负数了，就照他脸抽丫的！ -- 2014-03-29 16：52

	return  (int)damage;
}


int CInstanceUtils::CalBulletDamage(CBullet* pstBullet){
	BattleSysCfg* pstBattleSysCfg = BATTLE_SYS_CFG_MASTER->GetBattleCfgByID(1);
	if (!pstBattleSysCfg){
		return -1;
	}

	unsigned int uAtkMagicType =  pstBullet->getAtkMagic();

	unsigned int uiArmorRatioCfgID = BuildArmorRatioCfgID(pstBullet->getAtkType(), LIGHT_ARMOR_DEF);
	BattleDefRatioCfg* pstArmorRatioCfg = BATTLE_DEF_RATIO_CFG_MASTER->GetBattleDefRatioCfgByID(uiArmorRatioCfgID);
	if (!pstArmorRatioCfg){
		return -1;
	}


	float bRatio = 1.0 + (pstBullet->getAtkRatio() - 0) / (pstBattleSysCfg->damageadjustratio() * 1.0 * RANDOM_DOMINATOR);
	if (bRatio <= 0.5){
		bRatio = 0.5;
	}else if (bRatio >= 2.0){
		bRatio = 2.0;
	}



	int64 damage = int64(pstBullet->getAtkVal()		
		* pstArmorRatioCfg->damageratio() 
		* bRatio
		/ RANDOM_DOMINATOR);

	//除以子弹特效中的伤害帧事件数量
	damage = damage / pstBullet->getEvtCnt();

	//二针说这里永远不会成为负数，立贴为证，要是那天他丫的把这个最后的*后面部分做出负数了，就照他脸抽丫的！ -- 2014-03-29 16：52

	return  (int)damage;
}

int CInstanceUtils::CalBulletDamage(CBullet* pstBullet, CQStageEntity* pstTarget, double dt){
	int iDamagePerSec = CalBulletDamage(pstBullet, pstTarget);
	if (pstBullet->IsContinuous() && pstBullet->getUndealedTime() == -1){
		pstBullet->setUndealedTime(0.1f);
	}else if (pstBullet->IsContinuous() && pstBullet->getUndealedTime()< 0.1){
		pstBullet->setUndealedTime(pstBullet->getUndealedTime() + dt);
		return -1;
	}

	if (pstBullet->IsContinuous()){
		if ( pstBullet->getUndealedTime() >= 0.1){
			int iDamage = int(iDamagePerSec * (dt + pstBullet->getUndealedTime()) + 0.5);
			pstBullet->setUndealedTime(0);
			if (iDamage < 0){
				CCLog("damage < 0");
			}			
			return iDamage;
		}else{
			return iDamagePerSec;
		}
	}else{
		return iDamagePerSec;
	}	
}


int CInstanceUtils::CalBulletDamage(CBullet* pstBullet, double dt){
	int iDamagePerSec = CalBulletDamage(pstBullet);
	if (pstBullet->IsContinuous() && pstBullet->getUndealedTime() == -1){
		pstBullet->setUndealedTime(0.2f);
	}else if (pstBullet->IsContinuous() && pstBullet->getUndealedTime()< 0.1){
		pstBullet->setUndealedTime(pstBullet->getUndealedTime() + dt);
		return -1;
	}

	if (pstBullet->IsContinuous()){
		if ( pstBullet->getUndealedTime() >= 0.1){
			int iDamage = int(iDamagePerSec * (dt + pstBullet->getUndealedTime()) + 0.5);
			pstBullet->setUndealedTime(0);
			if (iDamage < 0){
				CCLog("damage < 0");
			}			
			return iDamage;
		}else{
			return iDamagePerSec;
		}
	}else{
		return iDamagePerSec;
	}	
}

void CInstanceUtils::RemoveLineBullets(CQStageEntity* pstEntity, CBaseFightScene* pstLayer){
	if (!pstEntity || !pstEntity->getArmature()){
		return;
	}

	unsigned int uBulletID = pstEntity->GetBulletID();
	const BulletCfg* pstBulletCfg = BULLET_CFG_MASTER->GetBulletCfgByID(uBulletID);

	if (!pstBulletCfg){
		return;
	}

	//处理连续光柱类子弹
	if (pstBulletCfg->bullettype() == TRA_CON_HORI_LINE || pstBulletCfg->bullettype() == TRA_JTL_LINE){
		//首先要检查是否一个属于这个人的子弹存在了
		CCArray *astBullets = pstLayer->getHeroBullets();

		CCObject* obj;
		CCARRAY_FOREACH(astBullets, obj){
			CBullet* pstBullet = dynamic_cast<CBullet*>(obj);
			if (pstBullet->getBulletID() == uBulletID && pstBullet->getOwner()->m_uID == pstEntity->m_uID){
				pstBullet->removeFromParentAndCleanup(true);
				pstLayer->getHeroBullets()->removeObject(pstBullet);
			}
		}		
	}
}

void CInstanceUtils::FireABulletEvent(CQStageEntity* pstEntity, CBaseFightScene* pstLayer){
	//发射一个子弹
	if (!pstEntity || !pstEntity->getArmature()){
		return;
	}

	if(pstEntity->getIsBulletLimit() && pstEntity->getBulletTime() <= 0){   //有弹药限制且弹药为0
		if(!pstLayer->getIsShowBulletOut()){
			pstLayer->startShowBulletOutTip();
		}

		//删掉光柱类子弹
		RemoveLineBullets(pstEntity, pstLayer);
		pstLayer->autoChangeNextWeapon();
		return;
	}

	unsigned int iBulletCnt = pstEntity->getBulletDepth();
	unsigned int uBulletID = pstEntity->GetBulletID();
	const BulletCfg* pstBulletCfg = BULLET_CFG_MASTER->GetBulletCfgByID(uBulletID);

	if (!pstBulletCfg){
		return;
	}

	//处理连续光柱类子弹
	if (pstBulletCfg->bullettype() == TRA_CON_HORI_LINE || pstBulletCfg->bullettype() == TRA_JTL_LINE){
		//首先要检查是否一个属于这个人的子弹存在了
		CCArray *astBullets = pstLayer->getHeroBullets();

		CCObject* obj;
		CCARRAY_FOREACH(astBullets, obj){
			CBullet* pstBullet = dynamic_cast<CBullet*>(obj);
			if (pstBullet->getBulletID() == uBulletID && pstBullet->getOwner()->m_uID == pstEntity->m_uID){
				//他的弹道已经在了，直接返回
				return;
			}
		}		
	}

	if (pstBulletCfg->bullettype() == TRA_CON_HORI_LINE || pstBulletCfg->bullettype() == TRA_CON_SPREAD
		|| pstBulletCfg->bullettype() == TRA_JTL_LINE){
		iBulletCnt = 1;
	}

	CCArray* pstTargets = CCArray::create();
	if (iBulletCnt > 0 && pstBulletCfg->bullettype() == TRA_ISO_FORWARD_TRACE){

	}else if (iBulletCnt > 0 && pstBulletCfg->bullettype() == TRA_ISO_FULL_TRACE){
		unsigned int uiTargetParty = ROLE_PARTY_FRIEND;
		if (pstEntity->getRoleParty() == ROLE_PARTY_FRIEND){
			uiTargetParty = ROLE_PARTY_HOSTILE;
		}
		CInstanceUtils::GetSomeTargets(pstEntity->GetBulletPosition(), uiTargetParty, pstLayer->getStageEntities(), iBulletCnt, pstEntity->GetBulletShootDistance(), pstTargets);
	}

	float fCenterY = pstEntity->GetBulletPosition().y;
	for (unsigned int iIdx = 0; iIdx < iBulletCnt; iIdx++){		
		CBullet* pstBullet = CBullet::create();
		pstBullet->setAtkMagic(pstEntity->GetAtkMagicType());
		pstBullet->setAtkRatio(pstEntity->GetMagicAtkRatio(pstEntity->GetAtkMagicType()));
		pstBullet->setAtkType(pstEntity->GetAtkType());
		pstBullet->setBulletID(uBulletID);
		pstBullet->setHit(pstEntity->getHit());

		if(pstEntity->getToatlAtkSpeed() <= 0){
			CCLOG("[ERROR]::the bullet's owner atk speed is below zero");
			pstBullet->setAtkVal(pstEntity->getEffAtk());
		}else{
			float bulletAtk = pstEntity->getEffAtk()*1270.0f/(1.0*pstEntity->getToatlAtkSpeed());
			pstBullet->setAtkVal(int(bulletAtk));
		}
		pstBullet->setPosition(pstEntity->GetFirePosition());
		pstBullet->setSpeed(pstBulletCfg->flyspeed());
		pstBullet->setTrail(pstBulletCfg->bullettype());
		pstBullet->setZalias(pstEntity->GetZAlias());
		pstBullet->setScaleX(pstEntity->getScaleX());
		pstBullet->setShootDistance(pstEntity->GetBulletShootDistance());
		pstBullet->setOwner(pstEntity);
		pstBullet->setDepth(pstEntity->getBulletDepth());
		pstBullet->setLight(pstEntity->getBulletLight());
		pstBullet->setDamageZone(pstBulletCfg->damagezone());
        pstBullet->setRoleType(pstEntity->getRoleType());
		if(pstEntity->getRoleParty() == ROLE_PARTY_FRIEND){
			if(pstEntity->m_uID == pstLayer->getMainHero()->m_uID){
				pstBullet->setParty(BULLET_PARTY_HERO);
			}else{
				pstBullet->setParty(BULLET_PARTY_FRIEND);
			}
		}else if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){
			pstBullet->setParty(BULLET_PARTY_ENEMY);
		}
		pstBullet->setAttenuation(pstEntity->GetBulletAttenuation());
		pstBullet->setMaxPenatrate(pstEntity->GetBulletPenatrate());
		

		pstBullet->setPosition(pstEntity->GetFirePosition());
		CCPoint stDestination;
		stDestination.x = pstBullet->getPositionX() + pstBullet->getScaleX() * pstEntity->GetBulletShootDistance();
		stDestination.y = pstBullet->getPositionY();
		if (pstBulletCfg->bullettype() != TRA_CON_HORI_LINE){
			pstBullet->setDestination(stDestination);
		}

		if (pstBulletCfg->bullettype() == TRA_ISO_FULL_TRACE || pstBulletCfg->bullettype() == TRA_ISO_FORWARD_TRACE){
			if (iIdx < pstTargets->count()){
				pstBullet->setTarget(pstTargets->objectAtIndex(iIdx));
			}else if (pstTargets->count()){
				pstBullet->setTarget(pstTargets->objectAtIndex(0));
			}
		}else if (pstBulletCfg->bullettype() == TRA_ISO_HORI_LINE){
			pstBullet->setPositionY(fCenterY + (iIdx - (iBulletCnt - 1) / 2) * 20);
			
			//可能需要一次发几颗子弹
			if (pstEntity->getBulletCurCnt() >= pstEntity->getBulletBurstCnt()){
				pstEntity->setBulletCurCnt(0);
			}

			float fRand = random_range(1, pstEntity->getBulletBurstCnt());
			float fAngle = (fRand - 0.5 - pstEntity->getBulletBurstCnt() / 2.0) * 2.5;
			pstBullet->setRotation(fAngle * -1.0);
			
			pstBullet->setVelocity(ccp(cos(CC_DEGREES_TO_RADIANS(fAngle)), sin(CC_DEGREES_TO_RADIANS(fAngle)) * pstEntity->getScaleX()));
			pstEntity->setBulletCurCnt(pstEntity->getBulletCurCnt() + 1);


		}else if (pstBulletCfg->bullettype() == TRA_CON_HORI_LINE){						
			pstBullet->setHitDist(pstEntity->GetBulletShootDistance());
		}
// 		}else if (pstBulletCfg->bullettype() == TRA_JTL_ISOLATE){
// 			if (pstEntity->getBulletCurCnt() >= pstEntity->getBulletBurstCnt()){
// 				pstEntity->setBulletCurCnt(0);
// 			}
// 
// 			float fRand = pstEntity->getBulletCurCnt() - pstEntity->getBulletBurstCnt() / 2.0;			
// 			pstBullet->setVelocity(ccp(1.0, 0));
// 			pstEntity->setBulletCurCnt(pstEntity->getBulletCurCnt() + 1);
// 			float fY = pstBullet->getPositionY() + 10 * fRand;
// 			pstBullet->setPositionY(fY);
// 			pstBullet->setDestination(ccp(pstBullet->getDestination().x, fY));
// 			//CCLOG("bullet position y %f", pstBullet->getPositionY());
// 		}

		if(pstEntity->IsInThrash()){
			pstBullet->setRes(pstBulletCfg->superani());
		}else{
			pstBullet->setRes(pstBulletCfg->oridinaryani());
		}

		pstBullet->setHurtRes(pstBulletCfg->beatanis());
		pstBullet->setUndealedTime(0.2f);
		pstBullet->setHitBackDist(pstBulletCfg->hitbackdist());
		pstBullet->setOwnerLevel(pstEntity->getLevel());
		pstBullet->setEvtCnt(pstBulletCfg->evtcnt());
			

		bool bAddToStage = true;
		if (((pstBulletCfg->bullettype() == TRA_ISO_FULL_TRACE || pstBulletCfg->bullettype() == TRA_ISO_FORWARD_TRACE) && !pstBullet->getTarget())){
			bAddToStage = false;
		}

		if (bAddToStage){
			pstLayer->getSceneForeground()->addChild(pstBullet);
			pstLayer->getHeroBullets()->addObject(pstBullet);

			int iNewZorder =GetZOrderByType(ROLE_Z_ORDER, pstBullet->getZalias() + pstBulletCfg->damagezone());
			pstBullet->setZOrder(iNewZorder);
			pstBullet->Fly();
		}
		pstBullet->setCfg(pstBulletCfg);
		pstBullet->AddEventListener(pstLayer, CBullet::BULLET_HIT_KEY, eventListner_selector(CBaseFightScene::HandleBulletKeyFrameEvent));
		if (pstBulletCfg->bullettype() == TRA_CON_HORI_LINE || pstBulletCfg->bullettype() == TRA_JTL_LINE){
			return; //只需要一个子弹，用不着那么多
		}
	}
	const WeaponCfg* pstWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(pstEntity->getWeaponCfgID());
	if (pstWeaponCfg->firesoundtype() == FIRE_SOUND_ISOLATE_TYPE){
		AudioManager::playEffect(pstWeaponCfg->firesound().c_str(), false);
	}
}

void CInstanceUtils::HandleBulletHitTargetEffect(CBullet* pstBullet, CQStageEntity* pstTarget, CBaseFightScene* pstFightScene, double dt){
	int iDamage = CInstanceUtils::CalBulletDamage(pstBullet, pstTarget, dt);
	if (iDamage >= 0){
		CCPoint stHitBackDist = ccp(pstBullet->getHitBackDist(), 0);
		if (pstBullet->getScaleX() < 0){
			stHitBackDist.x *= -1; 
		}
		CHurtDamageObj stHurtObj;
		stHurtObj.fDamage = iDamage  * pstBullet->GetLeftPenaRatio() + pstTarget->GetBulletDamageAdj();
		stHurtObj.stHurtAnimation = pstBullet->getHurtRes();
		stHurtObj.uBulletParty = pstBullet->getParty();
		stHurtObj.iBulletOwnerLevel = pstBullet->getOwnerLevel();
		stHurtObj.iBulletRoleType = pstBullet->getRoleType();
		stHurtObj.stHitBackPos = stHitBackDist;
		stHurtObj.stBulletPos = pstBullet->getPosition();
		stHurtObj.stPapaAnimation = pstBullet->getCfg()->papapa();
		stHurtObj.iCurAniEffNum = pstFightScene->getCurAniTotalNum();
		pstTarget->HurtWithDamage(stHurtObj);	
		if (pstTarget->IsDead()){
			//策划说这个不要了，2014-9-17

			//int iAngle = random_range(-90, 90);
			//if (abs(iAngle) < 30){
			//	iAngle = abs(iAngle) + 30;
			//}else if (pstTarget->getPositionY() < 200){
			//	iAngle = 45;
			//}
			//float x = cos(CC_DEGREES_TO_RADIANS(iAngle)) * 1500 * pstBullet->getScaleX();
			//float y = sin(CC_DEGREES_TO_RADIANS(iAngle)) * 1500 * pstBullet->getScaleY();
			//pstTarget->setVelocity(ccp(x, y));
		}

		//加处理附加效果

		int iCurBuffNum = pstFightScene->getBulletSkillBuffNum();
		for(int i = 0; i < pstBullet->getCfg()->additionaleffs_size(); i++){
			if (CRandom::GetRand(RANDOM_DOMINATOR) < pstBullet->getCfg()->additionaleffs(i).prob()){
				iCurBuffNum = pstFightScene->getBulletSkillBuffNum();
				switch (pstBullet->getCfg()->additionaleffs(i).efftype())
				{
				case SKILL_EFFECT_ADD_BUFF:
					if(iCurBuffNum < (int)ConstHelper(FIGHT_BULLET_SKILL_ADD_BUFF_MAX)){		//超出当前buff添加数量
						pstTarget->AddBuff(pstBullet->getCfg()->additionaleffs(i).effval(), pstBullet->m_uID);
						iCurBuffNum++;
						pstFightScene->setBulletSkillBuffNum(iCurBuffNum);
					}else{
						pstTarget->AddBuff(pstBullet->getCfg()->additionaleffs(i).effval(), pstBullet->m_uID, false);
					}
					break;
				default:
					break;
				} 
			}
		}
	}
}

void CInstanceUtils::HandleBulletHitItemEffect(CBullet* pstBullet, CSceneItem* pstItem, double dt){
	int iDamage = CalBulletDamage(pstBullet, dt);
	if (iDamage){
		pstItem->hurtWithDemage(iDamage);
	}
}

void CInstanceUtils::ShakeScene(BaseScene* pstScene, float d, float strength, ELayerID layerID){
	CCLayer *layer = pstScene->getLayer(layerID);
	if(layer == NULL){
		return;
	}

	CCShake *shake = CCShake::create(d, strength);
	CCSequence *seq = CCSequence::create(shake, NULL);
	layer->runAction(seq);
}

void CInstanceUtils::BuildEntityFightInfoBrief(CEntityFightInfoBrief& stBrief, CQStageEntity* pstEntity){
	for (int i = 0; i < MAX_MAGIC_CNT; i++){
		stBrief.m_stMagicAtkRatio[i] = pstEntity->GetMagicAtkRatio(i);
		stBrief.m_stMagicDefRatio[i] = pstEntity->GetMagicDefRatio(i);
	}

	stBrief.m_uArmorType = pstEntity->GetArmorType();
	stBrief.m_uAtkAdjustRatio = pstEntity->m_uAtkAdjustRatio;
	stBrief.m_uDefAdjustRatio = pstEntity->m_uDefAdjustRatio;
	stBrief.m_uAttackMagicType = pstEntity->GetAtkMagicType();
	stBrief.m_uAttackType = pstEntity->GetAtkType();
	stBrief.m_uEffAtk = pstEntity->getEffAtk();
	if (pstEntity->getRoleType() == ROLE_TYPE_PLAYER){
		CMainHero* pstPlayer = dynamic_cast<CMainHero*>(pstEntity);
		if (pstPlayer && pstPlayer->GetShootInterval() != 0){
			//stBrief.m_uEffAtk = pstEntity->getEffAtk() / pstPlayer->GetShootInterval();
			stBrief.m_uEffAtk = pstEntity->getEffAtk();
		}
		
	}
	stBrief.m_uEffDef = pstEntity->getEffDef();
	stBrief.m_iLevel = pstEntity->getLevel();

}

void CInstanceUtils::HandleTryKickEvent(CBaseFightScene* pstFightScene, CMainHero* pstHero){

	CCObject* obj;
	CCARRAY_FOREACH(pstFightScene->getStageEntities(), obj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(obj);
		if (!pstEntity){
			continue;
		}
		if (pstEntity->getRoleParty() == pstHero->getRoleParty()){
			//他的弹道已经在了，直接返回
			continue;;
		}
		float distx = (pstEntity->getPositionX() - pstHero->getPositionX()) * pstHero->getArmature()->getScaleX();
		if (fabs(pstEntity->GetFootPos().y - pstHero->GetFootPos().y) < 200
			&& distx < 200
			&& distx > 0){
			pstHero->Kick();
			return;
		}
	}		
}


void CInstanceUtils::HandleKickEvent(CBaseFightScene* pstFightScene, CQStageEntity* pstHero){

	CCObject* obj;
	CCARRAY_FOREACH(pstFightScene->getStageEntities(), obj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(obj);
		if (!pstEntity){
			continue;
		}
		if (pstEntity->getRoleParty() == pstHero->getRoleParty()){
			//他的弹道已经在了，直接返回
			continue;;
		}
		float distx = (pstEntity->getPositionX() - pstHero->getPositionX()) * pstHero->getArmature()->getScaleX();
		if (fabs(pstEntity->GetFootPos().y - pstHero->GetFootPos().y) < 200
			&& distx < 200
			&& distx > 0){
			pstEntity->KnockDown(); //或者另外做一个动作			
		}
	}		
}

bool CInstanceUtils::IsContoursCollide(CBaseSceneItem* pstItem, CQStageEntity* pstEntity){
	if (!pstEntity || !pstItem || !pstEntity->getArmature() || !pstItem->getArmature()){
		return false;
	}

	string boneName = pstItem->getArmature()->getAnimation()->getCurrentMovementID();
	if (!boneName.length()){
		return false;
	}	


	CCBone* pstBone1 = NULL;
	if (pstItem->getArmature()->getBone("body")){
		pstBone1 = pstItem->getArmature()->getBone("body");

	}else if (pstItem->getArmature()->getBone(boneName.c_str())){
		pstBone1 = pstItem->getArmature()->getBone(boneName.c_str());
	}

	if(pstBone1 == NULL){
		return false;
	}

	CCArray* skillColiArray = pstBone1->getColliderBodyList();
	if (!skillColiArray){
		return false;
	}

	CCPoint stBonePos = ccpAdd(ccp(pstBone1->getBoneData()->x * pstItem->getScaleX(), pstBone1->getBoneData()->y * pstItem->getScaleY()), pstItem->getPosition());

	if (skillColiArray->count()){
		for (unsigned int iIdx = 0; iIdx < skillColiArray->count(); iIdx++){
			ColliderBody * body1 = (ColliderBody *)skillColiArray->objectAtIndex(iIdx);
			CCContourData* contour1 = body1->getContourData();
			CCContourVertex2* LeftUp1 = (CCContourVertex2*)contour1->vertexList.objectAtIndex(0);
			CCContourVertex2* LeftDown1 = (CCContourVertex2*)contour1->vertexList.objectAtIndex(1);
			CCContourVertex2* RightDown1 = (CCContourVertex2*)contour1->vertexList.objectAtIndex(2);
			CCContourVertex2* RightUp1 = (CCContourVertex2*)contour1->vertexList.objectAtIndex(3);

			CCPoint stPos = ccp((RightUp1->x + LeftUp1->x) / 2, (LeftUp1->y + LeftDown1->y) / 2);
			CCPoint aPos = ccpAdd(stPos, stBonePos);
			CCPoint bPos = pstEntity->GetFootPos();
			CCSize aSize = CCSizeMake(RightUp1->x - LeftUp1->x, RightUp1->y - RightDown1->y);
			CCRect aRect = CCRectMake(aPos.x - aSize.width/2, aPos.y - aSize.height/2, aSize.width, aSize.height);

			//CCLOG("the collider rect is x === %f, y ==== %f, w ==== %f, h === %f", aRect.getMinX(), aRect.getMinY(), aRect.size.width, aRect.size.height);
			//CCLOG("the QStageEnity is x === %f, y ==== %f", pstEntity->GetFootPos().x, pstEntity->GetFootPos().y);

			if(aRect.containsPoint(bPos)){
				return true;
			}
		}
	}

	return false;
}

void CInstanceUtils::DashSkillAtk(CQStageEntity* pstAtker, CCArray* pstEntities, CBaseFightScene* pstLayer){
	CQStageEntity* pstMonster;
	CQSkillCell* pstSkill = pstAtker->GetCurSkillCell();

	if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_DASH){
		for (unsigned int iIdx = 0; iIdx < pstEntities->count(); iIdx++){
			pstMonster = dynamic_cast<CQStageEntity*>(pstEntities->objectAtIndex(iIdx));
			if (pstAtker->getRoleParty() == pstMonster->getRoleParty()
				|| pstMonster->IsImmune()){
					continue;
			}

			if(fabs(pstAtker->getPositionX() - pstMonster->getPositionX()) > (pstAtker->getCenterToSides() + pstMonster->getCenterToSides())){
				continue;
			}

			if (DoTwoEntityCollide(pstAtker, pstMonster, pstSkill->m_stCfg.damagezone())){
				SkillMakeEffect(pstSkill, pstAtker, pstMonster, pstLayer);
			}
		}
	}
}

void CInstanceUtils::SkillWarningOnGround(CQStageEntity* pstAtker, CQSkillCell* pstSkill, CCPoint stPos, CBaseFightScene* pstLayer){
	CSkillEffLayer* pstWarning = CSkillEffLayer::create();
	pstWarning->Warning(pstSkill->m_uSkillID);
	pstWarning->setZalias(stPos.y + 1);
	pstWarning->setOrigPos(stPos);
	pstWarning->setPosition(stPos);
	pstWarning->AddEventListener(pstLayer, CSkillEffLayer::WARNING_COMPLETE_EVENT, eventListner_selector(CBaseFightScene::handleWarningCompleteEvent));
	pstWarning->setOwnerEntity(pstAtker);
	pstWarning->setOwnerParty(pstAtker->getRoleParty());
	CInstanceUtils::BuildEntityFightInfoBrief(pstWarning->m_stOwnerBrief, pstAtker);
	int iNewZorder = GetZOrderByType(FLOOR_OTHER_Z_ORDER, stPos.y);
	pstLayer->getSceneForeground()->addChild(pstWarning, iNewZorder);

	//播放预警音效
	AudioManager::playEffect("skill_warning.ogg");
}

void CInstanceUtils::SkillRandOnGround(CQStageEntity* pstAtker, CSkillEffLayer* pstSkill, CCPoint stPos, CBaseFightScene* pstLayer){
	CSkillEffLayer* pstSkillEffLayer = CSkillEffLayer::create();
	if (pstSkillEffLayer->LoadRes(pstSkill->m_stCfg.skillid())){
		return;
	}
	pstSkillEffLayer->setZalias(stPos.y + 1);
	pstSkillEffLayer->setOrigPos(stPos);
	pstSkillEffLayer->setPosition(stPos);
	pstSkillEffLayer->setDstPos(stPos);
	pstSkillEffLayer->Fly(false);

	pstSkillEffLayer->AddEventListener(pstLayer, CSkillEffLayer::HIT_KEY_FRAME_EVENT, eventListner_selector(CBaseFightScene::handleSkillEffEvent));
	pstSkillEffLayer->AddEventListener(pstLayer, CSkillEffLayer::SPAWN_KEY_FRAME_EVENT, eventListner_selector(CBaseFightScene::handleSkillEffEvent));
	pstSkillEffLayer->setOwnerEntity(pstAtker);
	pstSkillEffLayer->setOwnerParty(pstAtker->getRoleParty());
	pstSkillEffLayer->m_stOwnerBrief = pstSkill->m_stOwnerBrief;

	int iNewZorder = GetZOrderByType(SKILL_Z_ORDER, stPos.y);
	pstLayer->getSceneForeground()->addChild(pstSkillEffLayer, iNewZorder);
	pstLayer->getSkills()->addObject(pstSkillEffLayer);
}

void CInstanceUtils::SkillRandFlyToGround(CQStageEntity* pstAtker, CSkillEffLayer* pstSkill, CCPoint stPos, CBaseFightScene* pstLayer){
	
}

void CInstanceUtils::SkillNextAction(CQStageEntity* pstAtker, CQSkillCell* pstSkill, CBaseFightScene* pstLayer){
	if(pstSkill->m_stCfg.nextaction() == "null"){
		return;
	}

	pstAtker->AttackWithAction(pstSkill->m_stCfg.nextaction(), pstSkill->m_stCfg.nextactsound());
}

int CInstanceUtils::GetZOrderByType(unsigned int uType, float y){
	int iNewZorder;// = uType + (OPTIMAL_HEIGHT - y)/OPTIMAL_HEIGHT;

	switch (uType)
	{
	case FLOOR_BUFF_Z_ORDER:
		iNewZorder = FLOOR_BUFF_Z_ORDER*((OPTIMAL_HEIGHT - y)/OPTIMAL_HEIGHT);
		break;
	case FLOOR_OTHER_Z_ORDER:
		iNewZorder = FLOOR_BUFF_Z_ORDER + (FLOOR_OTHER_Z_ORDER - FLOOR_BUFF_Z_ORDER)*((OPTIMAL_HEIGHT - y)/OPTIMAL_HEIGHT);
		break;
	case ROLE_Z_ORDER:
		iNewZorder = FLOOR_OTHER_Z_ORDER + (ROLE_Z_ORDER - FLOOR_OTHER_Z_ORDER)*((OPTIMAL_HEIGHT - y)/OPTIMAL_HEIGHT);
		break;
	case SKILL_Z_ORDER:
		iNewZorder = FLOOR_OTHER_Z_ORDER + (SKILL_Z_ORDER - FLOOR_OTHER_Z_ORDER)*((OPTIMAL_HEIGHT - y)/OPTIMAL_HEIGHT);
		break;
	default:
		break;
	}

	return iNewZorder;
}

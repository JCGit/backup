#include "InstanceSkillUtils.h"
#include "InstanceUtils.h"
#include "GlobalTime.h"

void CInstanceSkillUtils::SkillEffCastLogic(CQStageEntity* pAtker, CBaseFightScene* pLayer, CCPoint stTargePos){
	CQSkillCell* pstSkill = pAtker->GetCurSkillCell();

	if (!pstSkill){
		return;
	}

	if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FOLLOW
		|| pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_SPAWN_DIRECT){
			//�ҵ�n����Ҫ���ٵ�Ŀ����
			//����Ҫ�������Ҫ���ȷɵ��Ķ�ȥ
			CCPoint stDstPos;

			unsigned int uiTargetParty = ROLE_PARTY_FRIEND;
			if (pAtker->getRoleParty() == ROLE_PARTY_FRIEND){
				uiTargetParty = ROLE_PARTY_HOSTILE;
			}

			CCArray * pstTargets = CCArray::create();
			CInstanceUtils::GetSomeTargets(pAtker->GetCenterPos(), 
				uiTargetParty, 
				pLayer->getStageEntities(), 
				pstSkill->m_stCfg.targetcnt(), 
				pstSkill->m_stCfg.castdist(),
				pstTargets);
			//��ʼ׷������
			CCObject *enimy;
			int iIdx = 0;
			CCARRAY_FOREACH(pstTargets, enimy){
				CSkillEffLayer* pstSpawnedSkill = CSkillEffLayer::create();
				pstSpawnedSkill->m_stCfg = pstSkill->m_stCfg;	
				pstSpawnedSkill->LoadRes(pstSkill->m_stCfg.skillid());
				pstSpawnedSkill->setPosition(pAtker->GetCenterPos());
				pstSpawnedSkill->setTarget(dynamic_cast<CQStageEntity*>(enimy));
				pstSpawnedSkill->AddEventListener(pLayer, CSkillEffLayer::HIT_KEY_FRAME_EVENT, eventListner_selector(CBaseFightScene::handleSkillEffAtkFrame));
				pstSpawnedSkill->setFlyState(SKILL_ANIMATION_STATE_PREPARE); //��ʼ׷��
				float fangle;

				if (iIdx % 2 == 0){
					fangle = CC_DEGREES_TO_RADIANS(90 - 60 / pstSkill->m_stCfg.targetcnt() * iIdx);
				}else{
					fangle = CC_DEGREES_TO_RADIANS(90 + 60 / pstSkill->m_stCfg.targetcnt() * iIdx);
				}


				stDstPos = ccpAdd(pAtker->GetTopPos(), ccpForAngle(fangle) * 300);
				if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_DIRECT_FOLLOW){
					pstSpawnedSkill->FlyToPos(stDstPos);
					pstSpawnedSkill->setDstPos(stDstPos);
					pstSpawnedSkill->setOrigPos(pstSpawnedSkill->getPosition());
				}else{
					pstSpawnedSkill->setPosition(dynamic_cast<CQStageEntity*>(enimy)->getPosition());
					pstSpawnedSkill->Fly(false);
				}


				pstSpawnedSkill->setOwnerEntity(pAtker);
				pstSpawnedSkill->setOwnerParty(pAtker->getRoleParty());
				CInstanceUtils::BuildEntityFightInfoBrief(pstSpawnedSkill->m_stOwnerBrief, pAtker);
				pLayer->getSkills()->addObject(pstSpawnedSkill);

				int iNewZorder = CInstanceUtils::GetZOrderByType(SKILL_Z_ORDER, pstSpawnedSkill->getPositionY());
				pLayer->getSceneForeground()->addChild(pstSpawnedSkill, iNewZorder);
				iIdx ++;
			}
			return;
	}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_CLOSE_ATTACK || pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_ACTION_ONLY_ON_ME
		|| pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_DASH || pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_DIRECT_ON_GROUND 
		|| pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_FLY_TO_GROUND || pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_NEXT_ACTION
		|| pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_JUMP_ATK){

			if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_CLOSE_ATTACK){
				CInstanceUtils::CloseSkillAtkEffect(pAtker, pLayer->getStageEntities(), pLayer);
			}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_ACTION_ONLY_ON_ME){
				CInstanceUtils::SkillMakeEffect(pstSkill, pAtker, pAtker, pLayer);
			}else if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_DIRECT_ON_GROUND){  //�ڵ���������̶ֹ�������Ч������һ���̶�Ϊ����λ��
				pLayer->skillRandFlyToGroud(pAtker, pstSkill);
			}else if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_RAND_FLY_TO_GROUND){  //������򼸸��̶��㣬����һ���̶�Ϊ����λ��
				pLayer->skillRandFlyToGroud(pAtker, pstSkill);
			}else if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_NEXT_ACTION){  //�����ͷź󴥷���һ������
				CInstanceUtils::SkillNextAction(pAtker, pstSkill, pLayer);
			}else if (pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_DASH){  //��ײ����
				float dashDis = GetSkillAtkDis(pAtker, pLayer);
				if(dashDis > 0){
					pAtker->ForwardDash(dashDis);
					pLayer->getMoveEntities()->addObject(pAtker);  //���뵽λ�ƽ�ɫ��������
				}
			}else if(pstSkill->m_stCfg.triggertype() == SKILL_TRIGGER_JUMP_ATK){	//��Ծ��������
				CCPoint stJumpTargetPos = GetSkillAtPos(pAtker, stTargePos);
				pAtker->JumpAtkWithAction(stJumpTargetPos, pstSkill->m_stCfg.castaction().c_str());
			}
			return;
	}

	//����һ�����е���Ч
	CSkillEffLayer* pstSkillEffLayer = CSkillEffLayer::create();
	if (!pstSkillEffLayer){
		CC_ASSERT("fail to allocate skill effect layer, OFM?");
	}

	if (pstSkillEffLayer->LoadRes(pstSkill->m_uSkillID)){
		return;
	}

	pstSkillEffLayer->setZalias(pAtker->GetFootPos().y + 1);
	pstSkillEffLayer->setOrigPos(pAtker->GetCenterPos());
	pstSkillEffLayer->setPosition(pAtker->GetCenterPos());
	pstSkillEffLayer->LoadArmRes();

	pstSkillEffLayer->AddEventListener(pLayer, CSkillEffLayer::HIT_KEY_FRAME_EVENT, eventListner_selector(CBaseFightScene::handleSkillEffEvent));
	pstSkillEffLayer->AddEventListener(pLayer, CSkillEffLayer::SPAWN_KEY_FRAME_EVENT, eventListner_selector(CBaseFightScene::handleSkillEffEvent));
	pstSkillEffLayer->AddEventListener(pLayer, CSkillEffLayer::SHAKE_SCREEN_KEY_FRAME, eventListner_selector(CBaseFightScene::handleSceenShakeEvent));
	pstSkillEffLayer->setOwnerEntity(pAtker);
	pstSkillEffLayer->setOwnerParty(pAtker->getRoleParty());
	CInstanceUtils::BuildEntityFightInfoBrief(pstSkillEffLayer->m_stOwnerBrief, pAtker);

	if (pAtker->getScaleX() > 0){
		pstSkillEffLayer->setScaleX(1);
	}else{
		pstSkillEffLayer->setScaleX(-1);
	}

	CCPoint stWorldPos = pLayer->getSceneForeground()->convertToWorldSpace(pAtker->getPosition());  
	CCSize stVisiblesize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint stSkillPos;		//�����ͷŵ�
	CCPoint stDstPos;		//Ŀ�ĵ�
	float fScale = 0;
	float xDis = 0;
	switch (pstSkillEffLayer->m_stCfg.triggertype())
	{
	case SKILL_TRIGGER_AREA:
	case SKILL_TRIGGER_SEPERATE_AREA:
		pstSkillEffLayer->Fly(false);
		pstSkillEffLayer->setPosition(ccpAdd(pAtker->GetFootPos(), ccp(pstSkillEffLayer->m_stCfg.castdist() * pAtker->getScaleX(), 0)));
		pstSkillEffLayer->setFlyState(SKILL_ANIMATION_STATE_FLYING);
		break;
	case SKILL_TRIGGER_FLY_COLIDE:

		pstSkillEffLayer->setDstPos(ccpAdd(pAtker->GetCenterPos(), ccp(pstSkillEffLayer->m_stCfg.castdist() * pAtker->getScaleX(), 0)));		
		pstSkillEffLayer->FlyToPos(pstSkillEffLayer->getDstPos());
		pstSkillEffLayer->setFlyState(SKILL_ANIMATION_STATE_FLYING);

		xDis = (pstSkillEffLayer->getContentSize().width/2)*(pAtker->getScaleX());
		pstSkillEffLayer->setOrigPos(pAtker->getPosition() + pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction()) + ccp(xDis, 0));
		pstSkillEffLayer->setPosition(pAtker->getPosition() + pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction()) + ccp(xDis, 0));
		break;
	case SKILL_TRIGGER_FLY_DIRECT_COLIDE:
		//�����ͷź�ֱ�߷��У���;��ײ����ʧ��δ��ײ�ʹ���
		pstSkillEffLayer->Fly(true);
		pstSkillEffLayer->setFlyState(SKILL_ANIMATION_STATE_FLYING);
		xDis = (pstSkillEffLayer->getContentSize().width/2)*(pAtker->getScaleX());
		stSkillPos = pAtker->getPosition() + pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction()) + ccp(xDis, 0);

		pstSkillEffLayer->setOrigPos(stSkillPos);
		pstSkillEffLayer->setPosition(stSkillPos);

		if (pstSkillEffLayer->getScaleX() > 0){
			stDstPos.x = CCDirector::sharedDirector()->getVisibleSize().width + pstSkillEffLayer->getContentSize().width/2;
			if(stDstPos.x < stSkillPos.x){
				stDstPos.x = stSkillPos.x + pstSkillEffLayer->getContentSize().width/2;
			}
			stDstPos.y = pstSkillEffLayer->getPositionY();
		}else{
			stDstPos.x = -pstSkillEffLayer->getContentSize().width/2;
			stDstPos.y = pstSkillEffLayer->getPositionY();
		}
		pstSkillEffLayer->FlyToPos(stDstPos);

		break;
	case SKILL_TRIGGER_SPAWN_FOLLOW:
		//�ֶ�׷��
		pstSkillEffLayer->setDstPos(ccpAdd(pAtker->GetTopPos(), ccp(0, 200)));		
		pstSkillEffLayer->CastToPos(pstSkillEffLayer->getDstPos());
		pstSkillEffLayer->setFlyState(SKILL_ANIMATION_STATE_PREPARE);
		break;
	case SKILL_TRIGGER_ON_ME:
		pstSkillEffLayer->Fly(false);
		pstSkillEffLayer->setDstPos(pAtker->GetCenterPos());
		pstSkillEffLayer->setPosition(pAtker->GetCenterPos());
		pstSkillEffLayer->setZalias(pAtker->GetFootPos().y - 1);
		break;
	case SKILL_TRIGGER_ON_GUN_DIRECT:	
		//ֱ�ӴӼ����ͷŵ����������Ҫ����Ч���б��
		pstSkillEffLayer->Fly(false);
		stSkillPos = pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction());
		//xDis = (pstSkillEffLayer->getContentSize().width/2)*(pAtker->getScaleX());
		xDis = 0;
		pstSkillEffLayer->setDstPos(pAtker->getPosition() + stSkillPos + ccp(xDis, 0));
		pstSkillEffLayer->setPosition(pAtker->getPosition() + stSkillPos + ccp(xDis, 0));
		pstSkillEffLayer->setZalias(pAtker->GetFootPos().y - 1);
		break;
	case SKILL_TRIGGER_ON_GUN_SPREAD:
		xDis = (pstSkillEffLayer->getContentSize().width/2)*(pAtker->getScaleX());
		stSkillPos = pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction());
		pstSkillEffLayer->Fly(false);
		pstSkillEffLayer->setPosition(pAtker->getPosition() + stSkillPos);
		pstSkillEffLayer->setDstPos(pAtker->getPosition() + stSkillPos);

		stWorldPos = pLayer->getSceneForeground()->convertToWorldSpace(pstSkillEffLayer->getPosition());  
		if (pAtker->getScaleX() > 0){
			fScale = (stVisiblesize.width - stWorldPos.x) / pstSkillEffLayer->getContentSize().width;
		}else{
			fScale = stWorldPos.x / pstSkillEffLayer->getContentSize().width * -1;
		}
		pstSkillEffLayer->setScaleX(fScale);
		break;
	case SKILL_TRIGGER_TRHOWN_TRAP:
		//ֱ�Ӵ�ǹ��Ͷ�����,����·������������
		stSkillPos = pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction());
		if (pAtker->getScaleX() > 0){
			pstSkillEffLayer->setDstPos(ccp(pAtker->getPositionX() + stSkillPos.x + pstSkill->m_stCfg.castdist(), pAtker->getPositionY()));
		}else{
			pstSkillEffLayer->setDstPos(ccp(pAtker->getPositionX() + stSkillPos.x - pstSkill->m_stCfg.castdist(), pAtker->getPositionY()));
		}

		pstSkillEffLayer->Fly(true);
		xDis = (pstSkillEffLayer->getContentSize().width/2)*(pAtker->getScaleX());
		pstSkillEffLayer->setPosition(stSkillPos + pAtker->getPosition());
		pstSkillEffLayer->CalcAcceleration();
		pstSkillEffLayer->setFlyState(SKILL_ANIMATION_STATE_PREPARE);
		break;

	case SKILL_TRIGGER_THROWN_COLIDE:
		//�����߷��У���;��ײ����ʧ��δ��ײ��غ�ը
		stSkillPos = pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction());
		if (pAtker->getScaleX() > 0){
			pstSkillEffLayer->setDstPos(ccp(pAtker->getPositionX() + stSkillPos.x + pstSkill->m_stCfg.castdist(), pAtker->getPositionY()));
		}else{
			pstSkillEffLayer->setDstPos(ccp(pAtker->getPositionX() + stSkillPos.x - pstSkill->m_stCfg.castdist(), pAtker->getPositionY()));
		}
		pstSkillEffLayer->Fly(true);
		xDis = (pstSkillEffLayer->getContentSize().width/2)*(pAtker->getScaleX());
		pstSkillEffLayer->setPosition(stSkillPos + pAtker->getPosition());
		pstSkillEffLayer->setOrigPos(pstSkillEffLayer->getPosition());
		pstSkillEffLayer->CalcAcceleration();
		pstSkillEffLayer->setFlyState(SKILL_ANIMATION_STATE_FLYING);
		break;

	case SKILL_TRIGGER_LEFT_EDGE:
		pstSkillEffLayer->Fly(false);
		stWorldPos = pLayer->getSceneForeground()->convertToNodeSpace(ccp(stVisiblesize.width / 2, 200)); 
		pstSkillEffLayer->setDstPos(stWorldPos);
		pstSkillEffLayer->setPosition(stWorldPos);	

		break;	
	case SKILL_TRIGGER_DIRECT_FLY_TO:
		//����һ���㣬�����¸���Ч���һƬ�����Ŀ��
		pstSkillEffLayer->Fly(true);
		xDis = (pstSkillEffLayer->getContentSize().width/2)*(pAtker->getScaleX());
		pstSkillEffLayer->setOrigPos(pAtker->getPosition() + pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction()) + ccp(xDis, 0));
		pstSkillEffLayer->setPosition(pAtker->getPosition() + pAtker->GetSkillGunPoint(pstSkill->m_stCfg.castaction()) + ccp(xDis, 0));
		pstSkillEffLayer->FlyToPosWithDir(stTargePos);
		pstSkillEffLayer->setFlyState(SKILL_ANIMATION_STATE_PREPARE);
		break;
	default:
		CCLog("error trigger type ... %d", pstSkillEffLayer->m_stCfg.triggertype());
		break;
	}

	int iNewZorder = CInstanceUtils::GetZOrderByType(SKILL_Z_ORDER, pstSkillEffLayer->getZalias());

	//��ɫ����
	if(pstSkillEffLayer->m_stCfg.skillid() >= 20000 && pstSkillEffLayer->m_stCfg.skillid() < 30000){
		iNewZorder = SKILL_Z_ORDER;
	}

	pLayer->getSceneForeground()->addChild(pstSkillEffLayer, iNewZorder);
	pLayer->getSkills()->addObject(pstSkillEffLayer);
}

float CInstanceSkillUtils::GetSkillAtkDis(CQStageEntity* pAtker, CBaseFightScene* pLayer){
	CQSkillCell* pstSkill = pAtker->GetCurSkillCell();
	float dis = pstSkill->m_stCfg.effectx();
	if(pAtker->getScaleX() > 0){
		if(pAtker->getPositionX() + pAtker->getCenterToSides() + dis >= pLayer->getStageRect().getMaxX()){
			dis = pLayer->getStageRect().getMaxX() - pAtker->getPositionX() - pAtker->getCenterToSides();
		}
	}else{
		if(pAtker->getPositionX() - pAtker->getCenterToSides() - dis <= pLayer->getStageRect().getMinX()){
			dis = pAtker->getPositionX() - pLayer->getStageRect().getMinX() - pAtker->getCenterToSides();
		}
	}

	return dis;
}

CCPoint CInstanceSkillUtils::GetSkillAtPos(CQStageEntity* pAtker, CCPoint stTargePos){
	CQSkillCell* pstSkill = pAtker->GetCurSkillCell();
	float disX = pstSkill->m_stCfg.effectx();
	float disY = pstSkill->m_stCfg.effecty();

	CCPoint stPos = pAtker->getPosition() - stTargePos;

	float x = disX;
	float y = disY;
	if(fabs(stPos.x) < disX){
		x = stPos.x;
	}

	if(fabs(stPos.y) < disY){
		y = stPos.y;
	}

	return CCPoint(x, y);
}


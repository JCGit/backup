#include "InstanceFightUtils.h"
#include "InstanceUtils.h"
#include "ConstHelper.h"

bool CInstanceFightUtils::BrushMonsterLogic(CBurshObj stBrushObj){
	unsigned int uType = stBrushObj.pstEnemyObj->type;
	bool isBrush = false;

	switch (uType)
	{
	case APPEAR_NORMAL:
		isBrush = BrushNormalLogic(stBrushObj);
		break;
	case APPEAR_SLEEP:
		isBrush = BrushSleepLogic(stBrushObj);
		break;
	case APPEAR_CAR:
		isBrush = BrushCarLogic(stBrushObj);
		break;
	case APPEAR_AFTER_DIE:
		isBrush = BrushAfterDieLogic(stBrushObj);
		break;
	case APPEAR_WALK_IN_LEFT:
	case APPEAR_WALK_IN_RIGHT:
		isBrush = BrushWalkLogic(stBrushObj);
		break;
	default:
		isBrush = false;
		break;
	}

	return isBrush;
}

bool CInstanceFightUtils::BrushNormalLogic(CBurshObj stBrushObj){
	CBaseFightScene* pLayer = stBrushObj.pLayer;
	CEnemyData* pstObj = stBrushObj.pstEnemyObj;
	float iTime = stBrushObj.iBrushTime;

	if(pLayer->getMonsterNumOnStage() >= ConstHelper(FIGHT_STAGE_MAX_MONSTERS)){
		return false;
	}

	int key = pstObj->key;
	int num = pstObj->num;
	int touchID = pstObj->touchID;
	bool bRepeated = false;		//是否重复刷怪
	bool isCanBrush = false;
	float lastBrushTime;


	if(touchID == 0){  
		if(pstObj->brushTime == 0){
			bRepeated = false;
		}else{
			bRepeated = true;
			lastBrushTime = pstObj->brushTime;
		}

		if(num > 0){
			if(bRepeated == false){  //第一次刷怪
				if(pstObj->time <= (int)ceil(iTime)){
					isCanBrush = true;
				}
			}else{
				if(int(iTime - lastBrushTime) >= pstObj->inter){   //间隔时间刷怪
					isCanBrush = true;
				}
			}
		}
	}else{  //按触发线
		map<int, int>::iterator stIter = pLayer->m_stTouchData.find(touchID);
		if(stIter == pLayer->m_stTouchData.end()){
			CCLOG("[error]::BrushTouchLogic  there is no touch id %d in map", touchID);
			return false;
		}

		if(pstObj->brushTime == 0){  //未触发过
			if(pLayer->getMainHero()->getPositionX() >= stIter->second  && pstObj->time <= (int)ceil(iTime)){
				isCanBrush = true;
			}
		}else{
			lastBrushTime = pstObj->brushTime;
			if(int(iTime - lastBrushTime) >= pstObj->inter && num > 0){   //间隔时间刷怪
				isCanBrush = true;
			}
		}
	}

	if(isCanBrush){
		pLayer->brushOneMonster(pstObj);
		pstObj->num = (--num);
		pstObj->brushTime = iTime;
	}

	return isCanBrush;
}

bool CInstanceFightUtils::BrushAfterDieLogic(CBurshObj stBrushObj){
	CBaseFightScene* pLayer = stBrushObj.pLayer;
	CEnemyData* pstObj = stBrushObj.pstEnemyObj;
	float iTime = stBrushObj.iBrushTime;

	if(pLayer->getMonsterNumOnStage() >= ConstHelper(FIGHT_STAGE_MAX_MONSTERS)){
		return false;
	}

	int touchID = pstObj->touchID;
	bool isCanBrush = false;
	float lastBrushTime;

	if(pLayer->checkMonsterOnStage(pstObj)){  //该刷怪点还有怪在舞台上，不刷
		return false;
	}

	if(touchID == 0){  //直接刷
		if(pstObj->brushTime == 0){
			if(pstObj->time <= (int)ceil(iTime)){
				isCanBrush = true;
			}
		}else{
			lastBrushTime = pstObj->brushTime;
			if(int(iTime - lastBrushTime) >= pstObj->inter){
				isCanBrush = true;
			}
		}
	}else{   //按触发线
		map<int, int>::iterator stIter = pLayer->m_stTouchData.find(touchID);
		if(stIter == pLayer->m_stTouchData.end()){
			CCLOG("[error]::BrushAfterDieLogic  there is no touch id %d in map", touchID);
			return false;
		}

		if(pstObj->brushTime == 0){  //未触发过
			if(pLayer->getMainHero()->getPositionX() >= stIter->second && pstObj->time <= (int)ceil(iTime)){
				isCanBrush = true;
			}
		}else{
			lastBrushTime = pstObj->brushTime;
			if(int(iTime - lastBrushTime) >= pstObj->inter){   //间隔时间刷怪
				isCanBrush = true;
			}
		}
	}

	if(isCanBrush){
		pLayer->brushOneMonster(pstObj);
		pstObj->brushTime = iTime;					//刷怪后时间置为当前时间
	}

	return isCanBrush;
}

bool CInstanceFightUtils::BrushSleepLogic(CBurshObj stBrushObj){
	CBaseFightScene* pLayer = stBrushObj.pLayer;
	CEnemyData* pstObj = stBrushObj.pstEnemyObj;
	float iTime = stBrushObj.iBrushTime;

	int touchID = pstObj->touchID;
	bool isCanBrush = false;

	map<int, int>::iterator stIter = pLayer->m_stTouchData.find(touchID);
	if(stIter == pLayer->m_stTouchData.end()){
		CCLOG("[error]::BrushSleepLogic  there is no touch id %d in map", touchID);
		return false;
	}

	if(pLayer->getMainHero()->getPositionX() < stIter->second || pstObj->time > (int)ceil(iTime)){
		return false;
	}

	CEnemy* pstMonster;
	CCObject* pstObject;
	CCARRAY_FOREACH(pLayer->getSleepMonsters(), pstObject){
		pstMonster = dynamic_cast<CEnemy*>(pstObject);
		int nowNum = pstMonster->getMonsterObj()->num;
		if((pstMonster->getMonsterObj()->touchID == touchID) && pstMonster->getIsActivate() == false && (pstMonster->getMonsterObj()->num > 0)){
			pstMonster->Active();
			pstMonster->getMonsterObj()->num = (--nowNum);
			pstMonster->getMonsterObj()->brushTime = iTime;

			if(pLayer->getSleepMonsters()->indexOfObject(pstMonster) != UINT_MAX){
				pLayer->getSleepMonsters()->removeObject(pstMonster);
			}

			isCanBrush = true;
		}
	}

	return isCanBrush;
}

bool CInstanceFightUtils::BrushCarLogic(CBurshObj stBrushObj){
	CBaseFightScene* pLayer = stBrushObj.pLayer;
	CEnemyData* pstObj = stBrushObj.pstEnemyObj;
	float iTime = stBrushObj.iBrushTime;

	int touchID = pstObj->touchID;

	map<int, int>::iterator stIter = pLayer->m_stTouchData.find(touchID);
	if(stIter == pLayer->m_stTouchData.end()){
		CCLOG("[error]::BrushCarLogic  there is no touch id %d in map", touchID);
		return false;
	}

	if(pLayer->getMonsterNumOnStage() >= ConstHelper(FIGHT_STAGE_MAX_MONSTERS)){
		return false;
	}

	if(pLayer->getMainHero()->getPositionX() < stIter->second || pstObj->brushTime != 0){
		return false;
	}

	float startX = pLayer->getStageRect().getMinX();
	float endX = pLayer->getStageRect().getMaxX();
	float xx = pstObj->x;
	float yy = pLayer->getSceneForeground()->getContentSize().height - pstObj->y;

	CCArmature* pCarArmature = CCArmature::create("jiuhuche");
	pCarArmature->setPosition(ccp(endX + pCarArmature->getContentSize().width/2, yy));
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pCarArmature->getPositionY());
	pLayer->getSceneForeground()->addChild(pCarArmature, iNewZorder);
	pCarArmature->getAnimation()->play("run", -1, -1, 1);

	float carWidth = pCarArmature->getContentSize().width/2;
	float moveInTime = (endX - xx + carWidth)/600.0f;
	float moveOutTime = (xx - startX - carWidth)/600.0f;
	CCActionInterval* moveIn = CCMoveTo::create(moveInTime, ccp(xx - carWidth, yy));
	CCFiniteTimeAction* brushCallFun = CCCallFuncO::create(pLayer, callfuncO_selector(CBaseFightScene::brushFromCar), pstObj);
	CCActionInterval* moveOut = CCMoveTo::create(moveOutTime, ccp(startX - xx - carWidth, yy));
	CCFiniteTimeAction* endCallFun = CCCallFuncO::create(pLayer, callfuncO_selector(CBaseFightScene::carMoveEnd), pCarArmature);
	pCarArmature->runAction(CCSequence::create(moveIn, CCDelayTime::create(0.4f), moveOut, endCallFun, NULL));
	pCarArmature->runAction(CCSequence::create(CCDelayTime::create(moveInTime), brushCallFun, NULL));
	
	pstObj->brushTime = iTime;

	return true;
}

bool CInstanceFightUtils::BrushWalkLogic(CBurshObj stBrushObj){
	CBaseFightScene* pLayer = stBrushObj.pLayer;
	CEnemyData* pstObj = stBrushObj.pstEnemyObj;
	float iTime = stBrushObj.iBrushTime;

	if(pLayer->getMonsterNumOnStage() >= ConstHelper(FIGHT_STAGE_MAX_MONSTERS)){
		return false;
	}

	int touchID = pstObj->touchID;
	int num = pstObj->num;
	bool bRepeated = false;		//是否重复刷怪
	bool isCanBrush = false;
	float lastBrushTime;

	if(touchID == 0){  
		if(pstObj->brushTime == 0){
			bRepeated = false;
		}else{
			bRepeated = true;
			lastBrushTime = pstObj->brushTime;
		}

		if(num > 0){
			if(bRepeated == false){
				if(pstObj->time <= (int)ceil(iTime)){
					isCanBrush = true;
				}
			}else{
				if(int(iTime - lastBrushTime) >= pstObj->inter && num > 0){   //间隔时间刷怪
					isCanBrush = true;
				}
			}
		}
	}else{  //按触发线
		map<int, int>::iterator stIter = pLayer->m_stTouchData.find(touchID);
		if(stIter == pLayer->m_stTouchData.end()){
			CCLOG("[error]::BrushTouchLogic  there is no touch id %d in map", touchID);
			return false;
		}

		if(pstObj->brushTime == 0){  //未触发过
			if(pLayer->getMainHero()->getPositionX() >= stIter->second && pstObj->time <= (int)ceil(iTime)){
				isCanBrush = true;
			}
		}else{
			lastBrushTime = pstObj->brushTime;
			if(int(iTime - lastBrushTime) >= pstObj->inter  && num > 0){   //间隔时间刷怪
				isCanBrush = true;
			}
		}
	}

	if(isCanBrush){
		pLayer->brushOneMonsterWithWalkIn(pstObj);
		pstObj->num = (--num);
		pstObj->brushTime = iTime;					//刷怪后时间置为当前时间
	}

	return isCanBrush;
}

int CInstanceFightUtils::MakeKey(int id, int x, int y){
	return (id & 0xFF) << 16 
		| (x & 0xFF) << 8
		| (y & 0xFF);
}




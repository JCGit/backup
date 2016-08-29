#include "NihilityGoldScene.h"
#include "NihilityUtils.h"
#include "ToolUtils.h"
#include "SceneManager.h"
#include "NihilityControl.h"
#include "GoldCoin.h"
#include "NextStageTipLayer.h"
#include "TextHelper.h"
#include "PopTipLayer.h"
#include "UserObj.h"
#include "ConstHelper.h"
#include "fight/FightControl.h"
#include "fight/FightObj.h"
#include "control/AudioManager.h"
#include "instance/InstanceUtils.h"
#include "instance/InstanceFightUtils.h"
#include "fight/FightWeaponArrLayout.h"

#include "GlobalCfgMaster.h"
#include "NihilityMonsterShowCfgMaster.h"

CNihilityGoldScene::CNihilityGoldScene()
:m_pTipLayer(NULL)
,m_fObjectTime(0)
,m_uCurMonsterNum(0)
,m_fStartWalkTime(0)
,m_uCurLeftMonsterNum(0)
,m_bIsArrivedEdge(false)
,m_bIsAutoWalking(false)
,m_uCurMonsterLev(INIT_MONSTER_LEVEL)
{

}

CNihilityGoldScene::~CNihilityGoldScene(){

}

bool CNihilityGoldScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CNihilityGoldScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	updateCoin();
	updatePackage();
	updateScore();

	m_pScoreUI->setInfo(P_NIHILITY_CTRL->getFloor());

	if(P_USER_OBJ->getUserInfo()->weapon.weaponguid() == TEMP_WEAPON_GUID){   //临时枪械
		m_pArrLayout->showTempWeapon(P_USER_OBJ->getUserInfo()->weapon);
	}
	m_pArrLayout->setSelectByGUID(P_USER_OBJ->getUserInfo()->weapon.weaponguid());
	m_pArrLayout->updateCellsByVec(P_NIHILITY_CTRL->getAllWeaponBulletVal());
	if(m_pArrLayout->getCurCellBulletVal()){
		_mainHero->setBulletTime(m_pArrLayout->getCurCellBulletVal());
	}

	startFight();
}

void CNihilityGoldScene::startUpdate(){
	CBaseFightScene::startUpdate();

	if(P_NIHILITY_CTRL->getbIsAutoFight()){
		_mainHero->ForceIdle();
		_mainHero->setAIType(AI_TYPE_ATTACK_BOSS_CLOSEST);
		_mainHero->SetIsControlledByAI(true);
		addAutoFightLayer();
	}else{
		m_pAutoBtn->setTouchEnabled(true);
	}
}

void CNihilityGoldScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();
}

void CNihilityGoldScene::loadMonsterRes(){
	m_uCurMonsterNum = 0;
	m_uCurLeftMonsterNum = 0;

	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		MonsterCfg stMonsterCfg;
		CNihilityUtils::genMonsterCfg(monsterObj->id, stMonsterCfg);
		loadMonsterResByCfg(&stMonsterCfg, monsterObj->num);
		m_uCurMonsterNum += monsterObj->num;
	}
	m_uCurLeftMonsterNum = m_uCurMonsterNum;
}

void CNihilityGoldScene::initMap(){
	CBaseFightScene::initMap();
	updateMapBlocks();

	walkableRect = CCRect(0, 0, _screenW*FIGHT_STAGE_NUM, m_uWalkHight);
}

void CNihilityGoldScene::initFightData(){
	CBaseFightScene::initFightData();

	m_stMonKeyToTime.clear();
	m_bIsArrivedEdge = false;
	m_bIsAutoWalking = false;
	m_fStartWalkTime = 0;
}

void CNihilityGoldScene::initMapUI(){
	CBaseFightScene::initMapUI();

	if(P_NIHILITY_CTRL->getbIsAutoFight()){
		m_pAutoBtn->setVisible(false);
	}else{
		m_pAutoBtn->setVisible(true);
	}
	m_pAutoBtn->setTouchEnabled(false);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pScoreUI = CFightEndlessScoreUI::create();
	m_pScoreUI->setScale(0.75f);
	m_pScoreUI->setPosition(visibleOrigin + ccp(20, m_pHeroHeadUI->getPositionY() - m_pScoreUI->getSize().height*m_pScoreUI->getScale() + 10));
	m_pUiLayer->addWidget(m_pScoreUI);
}

void CNihilityGoldScene::initMainHero(){
	CBaseFightScene::initMainHero();

	CCPoint m_stInitPos = ccp(300, m_uWalkHight/2);
	_mainHero->SetEnityPosition(m_stInitPos);
	_mainHero->setDesiredPosition(_mainHero->getPosition());
	setViewpointCenter(_mainHero->getPosition());
}

void CNihilityGoldScene::initMonster(){
	
}

void CNihilityGoldScene::update(float delta){
	CBaseFightScene::update(delta);

	checkArrivedEdge();
	checkMonsterOutStage();
	setViewpointCenter(_mainHero->getPosition());

	m_fObjectTime += delta;
}

void CNihilityGoldScene::brushMonsterLogic(float dt){
	if(m_bIsGameOver){
		return;
	}

	unsigned int key;
	int startTime;
	bool bRepeated = false;		//是否重复刷怪
	map<unsigned int, int>::iterator stIter;

	CCObject* pstObj;
	CEnemyData* pstMonsterObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		pstMonsterObj = (CEnemyData*)pstObj;

		if(pstMonsterObj->type == APPEAR_SLEEP || pstMonsterObj->type == APPEAR_AFTER_DIE
			|| pstMonsterObj->touchID != 0){	
			continue;
		}

		key = pstMonsterObj->key;
		stIter = m_stMonKeyToTime.find(key);
		if(stIter == m_stMonKeyToTime.end()){
			m_stMonKeyToTime[key] = int(m_fObjectTime);
			startTime = int(m_fObjectTime);
			bRepeated = false;
		}else{
			startTime = stIter->second;
			bRepeated = true;
		}

		bool isCanBrush = false;
		int nowNum = pstMonsterObj->num;
		if(pstMonsterObj->time <= (int)m_fObjectTime && nowNum > 0){
			if(bRepeated == false){
				isCanBrush = true;
			}else{
				if((int(m_fObjectTime) - startTime) >= pstMonsterObj->inter){   //间隔时间刷怪
					isCanBrush = true;
				}
			}
		}

		if(isCanBrush){
			initMonsterAddToStage(pstMonsterObj);
			pstMonsterObj->num = (--nowNum);
		}
	}
}

void CNihilityGoldScene::initMonsterAddToStage(CEnemyData* pstObj){
	unsigned int key = pstObj->key;
	CEnemy* pstMonster = initOneMonster(pstObj);
	pstMonster->LoadActionRes();
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
	_sceneForegound->addChild(pstMonster, iNewZorder);
	m_stMonKeyToTime[key] = int(m_fObjectTime);

	if(!pstMonster->getBlockSize().equals(CCSizeZero)){
		getBlocksInSize(pstMonster->getBlockSize(), (CCPoint)pstMonster->getPosition(), true);
		updateMapBlocks();
	}

	m_uCurLeftMonsterNum--;   //刷一只少一只
}

void CNihilityGoldScene::handlePassLogic(){
	//舞台没有怪，且怪物已经刷完
	if(m_uCurLeftMonsterNum <= 0 && getMonsterNumOnStage() <= 0 && !m_bIsGameOver){
		m_bIsGameOver = true;

		//金币增加
		unsigned int coinCnt = P_NIHILITY_CTRL->getCoin();
		coinCnt += GLOBAL_CFG_MASTER->GetCfg().nihilitybasegold()*(P_NIHILITY_CTRL->getFloor());
		updateCoin();

		//整层分数
		P_NIHILITY_CTRL->setFloorScore(P_NIHILITY_CTRL->getScore());

		addNextStageTip();
	}
}


void CNihilityGoldScene::handleGiveUpLogic(CCObject* pstObj){
	P_NIHILITY_CTRL->setIsForceQuit(false);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, GIVEUP_FIGHT_EVENT);
	handleGameOver();
}

void CNihilityGoldScene::handleGameOver(){
	m_bIsGameOver = true;
	m_bIsPreOver = true;
	this->unscheduleUpdate();
	P_NIHILITY_CTRL->sendNihilityFightEndRqst(m_uEvolutionCnt);
}

CEnemy* CNihilityGoldScene::initOneMonster(CEnemyData* monsterObj){
	MonsterCfg stCfg;
	//unsigned int normalLev = INIT_MONSTER_LEVEL + P_NIHILITY_CTRL->getFloor()*4 + GOLD_TYPE_LEVEL - 1;
	unsigned int normalLev = 1;		//策划说金币怪数值读1的
	CNihilityUtils::genMonsterCfg(monsterObj->id, stCfg, normalLev);
	CEnemy* pstMonster = CBaseFightScene::initOneMonster(monsterObj, &stCfg);
	pstMonster->setIsCheckOutSize(false);		//不检测出屏
	return pstMonster;
}


void CNihilityGoldScene::addNextStageTip(){
	mainHeroCeaseShoot();
	_mainHero->ForceIdle();
	_hudLayer->Inactive();
	m_pUiLayer->removeAllFireBtn();
	setAtkBtnEnable(false);

	if(m_pTipLayer && m_pTipLayer->getParent() != NULL){
		m_pTipLayer->removeFromParentAndCleanup(true);
		m_pTipLayer = NULL;
	}

	UILayer *pTopLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_TOP));
	m_pTipLayer = CNextStageTipLayer::create();
	m_pTipLayer->setZOrder(100);
	pTopLayer->addWidget(m_pTipLayer);
	m_pTipLayer->startAct();

	CCPoint stDst = ccp(walkableRect.size.width, m_uWalkHight/2);
	_mainHero->WalkTo(stDst);

	m_bIsAutoWalking = true;
	m_bIsWalkNextStage = true;
	m_fStartWalkTime = m_fFightTime;
}

void CNihilityGoldScene::removeStageTip(){
	_hudLayer->Active();
	setAtkBtnEnable(true);

	if(m_pTipLayer && m_pTipLayer->getParent()){
		m_pTipLayer->removeFromParentAndCleanup(true);
		m_pTipLayer = NULL;
	}
}

void CNihilityGoldScene::setAtkBtnEnable(bool isEnable){
	fireBtn->setTouchEnabled(isEnable);
	m_trickBtn->setBtnEnable(isEnable);
	m_skillBtn->setBtnEnable(isEnable);
	m_dodgeBtn->setBtnEnable(isEnable);
	pauseBtn->setTouchEnabled(isEnable);

	if(!P_NIHILITY_CTRL->getbIsAutoFight()){
		m_pAutoBtn->setTouchEnabled(isEnable);
	}
}

void CNihilityGoldScene::checkArrivedEdge() {
	unsigned int wallWidth = 10.0f;
	CCRect rightWall = CCRectMake(walkableRect.size.width - wallWidth, 0, wallWidth, walkableRect.size.height);

	const CCPoint &pos = _mainHero->getPosition();
	const CCSize &size = _mainHero->getContentSize();
	CCRect playerRect = CCRectMake(pos.x - size.width * 0.5, pos.y - size.height * 0.5, size.width, size.height);

	if(!m_bIsArrivedEdge) {
		if((m_fFightTime - m_fStartWalkTime > ConstHelper(FIGHT_STAGE_WALK_TIME)*1.0f/1000 || rightWall.intersectsRect(playerRect)) 
			&& m_bIsGameOver && m_bIsAutoWalking) {

			m_bIsArrivedEdge = true;

			unsigned int preLev = P_NIHILITY_CTRL->getFloor();
			P_NIHILITY_CTRL->setFloor(preLev + 1);

			//进下一层
			P_NIHILITY_CTRL->setNeedRush(0);
			SceneManager::sharedSceneManager()->enterScene(NIHILITY_FIGHT_SCENE);

			//过一层上报一次
			P_NIHILITY_CTRL->sendNihilityMissonReportRqst(m_uEvolutionCnt);

			removeAutoFightLayer();

			if(m_pTipLayer && m_pTipLayer->getParent()){
				m_pTipLayer->removeFromParentAndCleanup(true);
				m_pTipLayer = NULL;
			}
		}
	}
}

void CNihilityGoldScene::checkMonsterOutStage(){
	CCObject* pstObj;
	CQStageEntity* pstEntity;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){
			if(pstEntity->getPositionX() + pstEntity->getCenterToSides() < 0){  //出边界
				_sceneForegound->removeChild(pstEntity, true);
				_stageEntities->removeObject(pstObj);
			}
		}
	}
}

void CNihilityGoldScene::updateScore(){
	m_pScoreUI->setScore(P_NIHILITY_CTRL->getScore());
}

void CNihilityGoldScene::updatePackage(){
	m_pScoreUI->setPackage(P_NIHILITY_CTRL->getAwardFloorCount());
}

void CNihilityGoldScene::updateCoin(){
	m_pScoreUI->setCoin(P_NIHILITY_CTRL->getCoin());
}

void CNihilityGoldScene::handleCoinEndEvent(){
	updateCoin();
}

void CNihilityGoldScene::autoFightHandle(){
	if(!m_pAutoBtn->isEnabled()){
		P_NIHILITY_CTRL->setbIsAutoFight(true);
	}
}

void CNihilityGoldScene::handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleCancelAutoEvent(stEvent, pstEventObj);

	if(m_pAutoBtn->isEnabled()){
		P_NIHILITY_CTRL->setbIsAutoFight(false);
	}
}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CNihilityGoldScene::doHeroDieFail(){
	//猪脚死亡
	P_NIHILITY_CTRL->setIsForceQuit(false);

	handleGameOver();

	if(_stageEntities->indexOfObject(_mainHero) != UINT_MAX){
		_stageEntities->removeObject(_mainHero);
	}
}

void CNihilityGoldScene::handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	unsigned int cfgID = monster->getMonsterObj()->id;
	const NihilityMonsterShowCfg* stCfg = NIHILITY_MONSTER_SHOW_CFG->GetNihilityMonsterShowCfgByID(cfgID);

	if(stCfg->floorbuffid() != 0){
		addFloorBuff(stCfg->floorbuffid(), monster->getRoleParty(), monster->GetFootPos());
	}

	//怪物死亡，数据变化
	handleHeroKillValueChange(stEvent, pstEventObj);

	//金币增加
	unsigned int floor = P_NIHILITY_CTRL->getFloor();
	P_NIHILITY_CTRL->addCoin(stCfg->dropgoldnum()*floor);

	//分数增加
	P_NIHILITY_CTRL->addScore(stCfg->score());
	updateScore();

	if(stCfg->dropgoldnum() > 0){
		addCoinEff(monster, stCfg->dropgoldnum());
	}
}

void CNihilityGoldScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	unsigned int cfgID = pstEntity->getMonsterObj()->id;
	CCPoint stPos = pstEntity->getPosition();
	const NihilityMonsterShowCfg* stCfg = NIHILITY_MONSTER_SHOW_CFG->GetNihilityMonsterShowCfgByID(cfgID);

	//把已经播放完毕死亡动画的死人做掉
	unsigned int mID = pstEntity->m_uID;
	if(pstEntity->getMyState() == kActionStateDie){  
		string str = MONSTER_RES_PATH + stCfg->avatar() + "/" + stCfg->avatar() + JSON_TYPE_NAME;
		updateResToNumByStr(str, false, 1);

		if(pstEntity->getIsBlock()){  //去掉阻挡
			removeBlocksInSize(pstEntity->getBlockSize(), (CCPoint)pstEntity->getPosition(), true);
			updateMapBlocks();
		}

		_sceneForegound->removeChild(pstEntity, true);
		if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
			_stageEntities->removeObject(pstEntity);
		}
	}
}

void CNihilityGoldScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);

	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	if(pstEntity){
		CEnemyData* pstEnemyData = pstEntity->getMonsterObj();

		if(pstEnemyData->type != APPEAR_AFTER_DIE){
			--m_uCurMonsterNum;
		}

		CCLOG("=====the monster be killed, and the number of last monster is ==== %d", m_uCurMonsterNum);
	}
}



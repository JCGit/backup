#include "FightGuideScene.h"
#include "ToolUtils.h"
#include "SceneManager.h"
#include "UserObj.h"
#include "CCColorUtil.h"
#include "FightGuideControl.h"
#include "FightGuideLayout.h"
#include "FightGuideHud.h"
#include "QEnvSet.h"
#include "fight/FightTipLayout.h"
#include "instance/InstanceUtils.h"
#include "evtlog/EvtLog.h"
#include "nihility/TransitionLayer.h"
#include "fight/FightWeaponArrLayout.h"
#include "fight/FightMedicalLayout.h"
#include "fight/FightControl.h"

#include "FightGuideCfgMaster.h"

CFightGuideScene::CFightGuideScene()
:m_pTipImg(NULL)
,m_bIsArrivedEdge(false)
{

}

CFightGuideScene::~CFightGuideScene(){

}

bool CFightGuideScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CFightGuideScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	moveToDst(_mainHero, ccp(300, m_uWalkHight/2));
}

void CFightGuideScene::loadMonsterRes(){
	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		MonsterCfg* monsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(monsterObj->id);
		loadMonsterResByCfg(monsterCfg, monsterObj->num);
	}

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/fight.plist");

	//摇杆特效
	string str = EFFECT_RES_PATH + "ut00043/ut00043.ExportJson";
	m_stRes.insert(str);

	str = EFFECT_RES_PATH + "ut00035/ut00035.ExportJson";
	m_stRes.insert(str);

	str = EFFECT_RES_PATH + "ut00046/ut00046.ExportJson";
	m_stRes.insert(str);
}

void CFightGuideScene::initMap(){
	CBaseFightScene::initMap();

	walkableRect = CCRect(0, 0, _screenW, m_uWalkHight);
}

void CFightGuideScene::initMapUI(){
	CBaseFightScene::initMapUI();

	m_pNextStageTip = CFightTipLayout::create();
	m_pNextStageTip->retain();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pTipImg = ImageView::create();
	m_pTipImg->setPosition(visibleOrigin + ccp(visibleSize.width/2, visibleSize.height*2/3));
	m_pUiLayer->addWidget(m_pTipImg);
	m_pTipImg->setVisible(false);

	//禁用正式摇杆
	_hudLayer->Inactive();
	_hudLayer->setVisible(false);

	m_pGuideHud = CFightGuideHud::create();
	m_pGuideHud->setPosition(visibleOrigin + ccp(150, 150));
	m_pUiLayer->addWidget(m_pGuideHud);

	if(!QEnvSet::beck()){
		pauseBtn->setVisible(false);
		pauseBtn->setTouchEnabled(false);
	}

	setAttackOpBtnVisible(false);

	m_pArrLayout->setEnabled(false);
	m_pChangeWeaponBtn->setEnabled(false);

	CFightMedicalLayout* pMedicalLayout = P_FIGHT_CTRL->getMedicalLayout();
	pMedicalLayout->setEnabled(false);
}

void CFightGuideScene::initMainHero(){
	CBaseFightScene::initMainHero();
	_mainHero->setHit(0);		//必定命中
	_mainHero->setEP(100);
	m_pHeroHeadUI->setEPPercent(_mainHero->getEP(), _mainHero->getMaxEP());
	_mainHero->ForceIdle();

	CCPoint stPlayerPt(walkableRect.getMinX() - (_mainHero->getCenterToSides()*2 + 10), m_uWalkHight/2);
	_mainHero->SetEnityPosition(stPlayerPt);
	_mainHero->setDesiredPosition(_mainHero->getPosition());
	_mainHero->setIsBulletLimit(false);
}

CEnemy* CFightGuideScene::initOneMonster(CEnemyData* monsterObj){
	float posX = walkableRect.getMaxX();
	CCPoint stPos = ccp(posX + m_uBrushNum*60, _sceneForegound->getContentSize().height - monsterObj->y);
	CEnemy* pstMonster = CBaseFightScene::initOneMonster(monsterObj);
	pstMonster->setHP(10);
	pstMonster->setMaxHP(10);
	pstMonster->SetIsControlledByAI(false);
	pstMonster->SetEnityPosition(stPos);
	pstMonster->setDesiredPosition(stPos);
	m_uBrushNum++;
	return pstMonster;
}

void CFightGuideScene::initMonster(){
	m_stCurStageMonsters.clear();
	m_uCurStageMonNum = 0;

	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;

		if(monsterObj->touchID == m_uGuideStep){
			m_uCurStageMonNum += monsterObj->num;
			m_stCurStageMonsters.push_back(monsterObj);
		}
	}

	m_uArriveNum = m_uCurStageMonNum;
}

void CFightGuideScene::initFightData(){
	CBaseFightScene::initFightData();

	m_fObjectTime = 0;
	m_uGuideStep = 1;
	m_uArriveNum = 0;
	m_uBrushNum = 0;
	m_uLastMonUID = 0;

	m_bIsArrivedEdge = false;
	m_bCanBrushMonster = false;
	m_bIsPlayKillEff = false;
	m_bIsAddCoinEff = false;
}

void CFightGuideScene::update(float delta){
	checkArrivedEdge();

	if(m_bIsGameOver){
		return;
	}

	CBaseFightScene::update(delta);

	setViewpointCenter(_mainHero->getPosition());
}

void CFightGuideScene::updateMonsterArrow(){

}

void CFightGuideScene::brushMonsterLogic(float dt){
	if(!m_bCanBrushMonster){
		return;
	}

	CEnemyData* pstMonsterObj;
	CEnemy* pstMonster;
	for(unsigned int i = 0; i < m_stCurStageMonsters.size(); i++){
		pstMonsterObj = m_stCurStageMonsters.at(i);
		bool isCanBrush = false;
		int nowNum = pstMonsterObj->num;
		if(pstMonsterObj->time <= (int)m_fObjectTime && nowNum > 0){
			isCanBrush = true;
		}

		if(isCanBrush){
			pstMonster = initOneMonster(pstMonsterObj);
			pstMonster->LoadActionRes(false);
			int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
			_sceneForegound->addChild(pstMonster, iNewZorder);
			pstMonsterObj->num = (--nowNum);

			moveToDst(pstMonster, ccp(pstMonsterObj->x, pstMonster->getPositionY()));
		}
	}
}

void CFightGuideScene::updateBgViewCenter(float foreX){
	updateBgTextureByRect(_sceneForegound);

	float bgViewX = foreX * BG_RATIO;
	if(_sceneBackground){
		_sceneBackground->setPositionX(bgViewX);
		updateBgTextureByRect(_sceneBackground, true);
	}

	float fgViewX = foreX * FG_RATIO;
	_sceneFrontgound->setPositionX(fgViewX);
	updateBgTextureByRect(_sceneFrontgound);

	if(_sceneMidground){
		float midViewX = foreX * MID_RATIO;
		_sceneMidground->setPositionX(midViewX);
		updateBgTextureByRect(_sceneMidground, true);
	}

	if(_sceneMid2ground){
		float midViewX = foreX * MID2_RATIO;
		_sceneMid2ground->setPositionX(midViewX);
		updateBgTextureByRect(_sceneMid2ground, true);
	}
}

void CFightGuideScene::nextStageTip(){
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	if(m_pNextStageTip->getParent() != NULL){
		m_pNextStageTip->stopAct();
		m_pNextStageTip->removeFromParent();
	}

	m_pUiLayer->addWidget(m_pNextStageTip);

	m_pNextStageTip->setPosition(visibleOrigin + ccp(visibleSize.width/2 - m_pNextStageTip->getSize().width/2, visibleSize.height*3/5));
	m_pNextStageTip->startAct();
}

void CFightGuideScene::cancelTip(){
	if(m_pNextStageTip && m_pNextStageTip->getParent() != NULL){
		m_pNextStageTip->stopAct();
		m_pNextStageTip->removeFromParent();
	}
}

void CFightGuideScene::handleGiveUpLogic(CCObject* pstObj){
	P_FIGHT_GUIDE_CTRL->endGuide();
	SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
}

void CFightGuideScene::moveToDst(CQStageEntity* pstEntity, CCPoint stPos){
	if(!walkableRect.containsPoint(stPos)) {
		CCLOG("fuck... npc outside of walkable rect");
		return;
	}

	if(pstEntity->getPosition().getDistance(stPos) < 40.0f) {
		onArrived("walk_done", pstEntity);
	} else {
		pstEntity->WalkTo(stPos);
		pstEntity->AddEventListener(this, "walk_done", eventListner_selector(CFightGuideScene::onArrived));
	}
}

void CFightGuideScene::onArrived(const string &evt, CCObject *pSender) {
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pSender);
	pstEntity->RemoveEventListener(this, "walk_done", eventListner_selector(CFightGuideScene::onArrived));
	if(pstEntity->getRoleParty() == ROLE_PARTY_FRIEND){  //主角
		if(m_uGuideStep == 1){   //第一步：主角刚开始走到指定位置
			this->scheduleUpdate();
			P_FIGHT_GUIDE_CTRL->startGuide();  //开始指引

			ADD_EVT_LOG(EVT_LOG_DEMO_DIALOG); //进入首场演示战斗场景，出现对话，点击提示
		}
	}else if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){
		if(m_uArriveNum <= 0){  //当前阶段的怪全部跑到位啦,下一步骤
			P_FIGHT_GUIDE_CTRL->nextStep();
		}else{
			m_uArriveNum--;

			if(m_uArriveNum <= 0){
				P_FIGHT_GUIDE_CTRL->nextStep();
			}
		}
	}
}

void CFightGuideScene::handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj){
	walkableRect.setRect(_screenW, 0, _screenW, m_uWalkHight);

	float xx = walkableRect.getMinX() + 300;
	_mainHero->setPosition(ccp(xx, m_uWalkHight/2));
	_mainHero->setDesiredPosition(_mainHero->getPosition());
	_mainHero->ForceIdle();

	float x = walkableRect.getMinX();
	_sceneForegound->setPositionX(x);
	updateBgViewCenter(x);

	m_bIsArrivedEdge = false;
	cancelTip();
	initMonster();
	P_FIGHT_GUIDE_CTRL->nextStep();
}

void CFightGuideScene::handleTransitionComplete(const string& stEvent, CCObject* pstEventObj){
	
}

void CFightGuideScene::setAttackOpBtnVisible(bool bIsVisible){
	fireBtn->setVisible(bIsVisible);
	fireBtn->setTouchEnabled(bIsVisible);
	m_trickBtn->setVisible(bIsVisible);
	m_trickBtn->setBtnEnable(bIsVisible);
	m_skillBtn->setVisible(bIsVisible);
	m_skillBtn->setBtnEnable(bIsVisible);
	m_dodgeBtn->setVisible(bIsVisible);
	m_dodgeBtn->setBtnEnable(bIsVisible);

	if(m_pChangeWeaponBtn->isEnabled()){
		m_pChangeWeaponBtn->setTouchEnabled(bIsVisible);
	}

	m_pGuideHud->setActive(bIsVisible);
}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CFightGuideScene::handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	MonsterCfg* stMonsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(monster->getMonsterObj()->id);

	//怪物死亡，数据变化
	handleHeroKillValueChange(stEvent, pstEventObj);
}

void CFightGuideScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	MonsterCfg* stMonsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(monster->getMonsterObj()->id);

	//把已经播放完毕死亡动画的死人做掉
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	unsigned int mID = pstEntity->m_uID;
	if(pstEntity->getMyState() == kActionStateDie){  
		updateResToNum(pstEntity->getMonsterObj()->id, MONSTER_TYPE, false);

		_sceneForegound->removeChild(pstEntity, true);
		if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
			_stageEntities->removeObject(pstEntity);
		}
	}

	//检查通过条件
	if(getNowStepType() == SKILL_CLEAN_TYPE && m_uCurStageMonNum <= 0 && mID == m_uLastMonUID){
		CToolUtils::setTimeScale(1.0f);    //放慢1倍
		P_FIGHT_GUIDE_CTRL->nextStep();
	}
}

void CFightGuideScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);
	m_uCurStageMonNum--;

	if(m_uCurStageMonNum <= 0){
		m_uLastMonUID = pstEventObj->m_uID;
		mainHeroCeaseShoot();
		setAttackOpBtnVisible(false);
		clearAllBullet();
		_mainHero->ForceIdle();
		removeGuide();
		m_pTipImg->setVisible(false);

		if(getNowStepType() != SKILL_CLEAN_TYPE){
			m_uGuideStep++;
			initMonster();
			if(getNowStepType() == DOWN_FIRE_CLEAN_TYPE){
				nextStageTip();
			}
			P_FIGHT_GUIDE_CTRL->nextStep();
		}else{
			CToolUtils::setTimeScale(0.5f);    //放慢1倍
		}
	}
}

void CFightGuideScene::btnTouchEvent(CCObject *pSender, TouchEventType type){
	CBaseFightScene::btnTouchEvent(pSender, type);

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		if (pSender->m_uID == fireBtn->m_uID){
			if(checkInFireGuide()){
				removeGuide();
			}else if(checkInMoveFireGuide() && m_pGuideHud->getIsMove()){
				removeGuide();
				if(m_pGuideHud->getDirect() == UP_TYPE){
					_mainHero->stateChangeByMove(ccp(0, 1));
				}else if(m_pGuideHud->getDirect() == DOWN_TYPE){
					_mainHero->stateChangeByMove(ccp(0, -1));
				}
			}
		}
		break;
	case TOUCH_EVENT_ENDED:
		if (pSender->m_uID == fireBtn->m_uID){
			if(checkInFireGuide()){
				showFireGuide();
			}else if(checkInMoveFireGuide()){
				showMoveFireGuide(P_FIGHT_GUIDE_CTRL->getDirect());
			}
		}
		break;
	case TOUCH_EVENT_CANCELED:
		if (pSender->m_uID == fireBtn->m_uID){
			if(checkInFireGuide()){
				showFireGuide();
			}else if(checkInMoveFireGuide()){
				showMoveFireGuide(P_FIGHT_GUIDE_CTRL->getDirect());
			}
		}
		break;

	default:
		break;
	}
}

void CFightGuideScene::handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleSkillClickEvent(stEvent, pstEventObj);
	if(getNowStepType() == SKILL_CLEAN_TYPE){
		removeGuide();
	}
}

void CFightGuideScene::unSetMainHeroAsTheOne(){
	if (!m_bIsInTheOneState){
		return;
	}
	_mainHero->setIsTheOne(false);
	resumeAllMonsters();	
	_sceneForegound->removeChild(m_pstBlackScreen, true);
	_sceneForegound->removeChild(m_pstSanguang, true);
	_mainHero->ResumeMovement();
	m_bIsInTheOneState = false;
}


/************************************************************************/
/*                         战斗引导                                     */
/************************************************************************/

unsigned int CFightGuideScene::getNowStepType(){
	int step = P_FIGHT_GUIDE_CTRL->getStepID();
	const FightGuideCfg* stCfg = FIGHT_GUIDE_CFG->GetFightGuideCfgByID(step);

	return stCfg->type();
}

void CFightGuideScene::showFireGuide(){
	m_pTipImg->setVisible(false);

	CCPoint screenOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize stSize = fireBtn->getSize();
	CCPoint anchor = fireBtn->getAnchorPoint();
	CCPoint stPos = fireBtn->getPosition();
	CCPoint stWorldPos = fireBtn->getParent()->convertToWorldSpace(stPos);

	CCPoint stOriPos = ccp(stWorldPos.x - stSize.width * anchor.x - screenOrigin.x, stWorldPos.y - stSize.height * anchor.y - screenOrigin.y);
	CCRect stShowRect = CCRectMake(stOriPos.x, stOriPos.y, stSize.width, stSize.height);
	if(P_FIGHT_GUIDE_CTRL->getIsShowForceGuide()){
		P_FIGHT_GUIDE_CTRL->removeGuide();
	}
	P_FIGHT_GUIDE_CTRL->addGuide();
	P_FIGHT_GUIDE_CTRL->getGuideLayout()->show(stShowRect);
	P_FIGHT_GUIDE_CTRL->getGuideLayout()->showShootTip(ccp(stShowRect.getMidX(), stShowRect.getMidY()));
}

bool CFightGuideScene::checkInFireGuide(){
	bool isInFire = false;
	int step = P_FIGHT_GUIDE_CTRL->getStepID();
	const FightGuideCfg* stCfg = FIGHT_GUIDE_CFG->GetFightGuideCfgByID(step);
	if(stCfg->type() == FIRE_CLEAN_TYPE && m_uCurStageMonNum > 0){
		isInFire = true;
	}

	return isInFire;
}

bool CFightGuideScene::checkInMoveFireGuide(){
	bool isInFire = false;
	int step = P_FIGHT_GUIDE_CTRL->getStepID();
	const FightGuideCfg* stCfg = FIGHT_GUIDE_CFG->GetFightGuideCfgByID(step);
	if(stCfg->type() == UP_FIRE_CLEAN_TYPE && m_uCurStageMonNum > 0){
		isInFire = true;
	}else if(stCfg->type() == DOWN_FIRE_CLEAN_TYPE && m_uCurStageMonNum > 0){
		isInFire = true;
	}

	return isInFire;
}

void CFightGuideScene::removeGuide(){
	m_pUiLayer->removeAllFireBtn();
	P_FIGHT_GUIDE_CTRL->removeGuide();
	int step = P_FIGHT_GUIDE_CTRL->getStepID();
	const FightGuideCfg* stCfg = FIGHT_GUIDE_CFG->GetFightGuideCfgByID(step);
	if(stCfg->pausedesc() != "null"){
		m_pTipImg->setVisible(true);
		string str = "fight/" + stCfg->pausedesc() + ".png";
		m_pTipImg->loadTexture(str.c_str(), UI_TEX_TYPE_PLIST);
	}else{
		m_pTipImg->setVisible(false);
	}
	m_pGuideHud->resetPos();
}

void CFightGuideScene::removeFireBtns(){
	m_pUiLayer->removeAllFireBtn();
}

void CFightGuideScene::showMoveFireGuide(int direct){
	if(!checkInMoveFireGuide()){
		return;
	}

	m_pGuideHud->setDirect(direct);
	m_pGuideHud->setActive(true);
	_mainHero->ForceIdle();
	fireBtn->setVisible(true);
	fireBtn->setTouchEnabled(true);
	m_pTipImg->setVisible(false);
	removeFireBtns();

	CCPoint screenOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize stSize = fireBtn->getSize();
	CCPoint anchor = fireBtn->getAnchorPoint();
	CCPoint stPos = fireBtn->getPosition();
	CCPoint stWorldPos = fireBtn->getParent()->convertToWorldSpace(stPos);

	CCPoint stOriPos = ccp(stWorldPos.x - stSize.width * anchor.x - screenOrigin.x, stWorldPos.y - stSize.height * anchor.y - screenOrigin.y);
	CCRect stShowRect = CCRectMake(stOriPos.x, stOriPos.y, stSize.width, stSize.height);
	if(P_FIGHT_GUIDE_CTRL->getIsShowForceGuide()){
		P_FIGHT_GUIDE_CTRL->removeGuide();
	}
	P_FIGHT_GUIDE_CTRL->addGuide();
	P_FIGHT_GUIDE_CTRL->getGuideLayout()->show(stShowRect);
	P_FIGHT_GUIDE_CTRL->getGuideLayout()->showShootTip(ccp(stShowRect.getMidX(), stShowRect.getMidY()));
	P_FIGHT_GUIDE_CTRL->getGuideLayout()->showFireEff(ccp(stShowRect.getMidX(), stShowRect.getMidY()));

	stSize = m_pGuideHud->getSize();
	stPos = m_pGuideHud->getPosition();
	anchor = m_pGuideHud->getAnchorPoint();
	stWorldPos = m_pGuideHud->getParent()->convertToWorldSpace(stPos);
	stOriPos = ccp(stWorldPos.x - stSize.width*0.5 - screenOrigin.x, stWorldPos.y - stSize.height*0.5 - screenOrigin.y);
	stShowRect = CCRectMake(stOriPos.x, stOriPos.y, stSize.width, stSize.height);
	P_FIGHT_GUIDE_CTRL->getGuideLayout()->show(stShowRect);
	P_FIGHT_GUIDE_CTRL->getGuideLayout()->showEff(ccp(stShowRect.getMidX(), stShowRect.getMidY()));
}

void CFightGuideScene::moveHero(){
	if(!_mainHero->isShooting() || !checkInMoveFireGuide()){
		return;
	}

	if(m_pGuideHud->getDirect() == UP_TYPE){
		_mainHero->stateChangeByMove(ccp(0, 1));
	}else if(m_pGuideHud->getDirect() == DOWN_TYPE){
		_mainHero->stateChangeByMove(ccp(0, -1));
	}

	removeGuide();
}

void CFightGuideScene::showMoveToGuide(){
	CCPoint stMovePos = ccp((_screenW - 500), m_uWalkHight/2);
	//walkableRect = CCRect(0, 0, _screenW*2, m_uWalkHight);
	_mainHero->ForceIdle();
	moveToDst(_mainHero, stMovePos);

	UILayer *pTopLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_TOP));
	CTransitionLayer* pLayer = CTransitionLayer::create();
	CCPoint stPos = CCDirector::sharedDirector()->getVisibleOrigin();
	//pLayer->setPosition(stPos);
	pLayer->setZOrder(100);
	pTopLayer->addWidget(pLayer);
	pLayer->AddEventListener(this, CTransitionLayer::TRANSITION_MID_EVENT, eventListner_selector(CFightGuideScene::handleTransitionMidEvent));
	pLayer->AddEventListener(this, CTransitionLayer::TRANSITION_COMPLETE, eventListner_selector(CFightGuideScene::handleTransitionComplete));
	pLayer->startAct();
}

void CFightGuideScene::showSkillGuide(){
	_mainHero->ForceIdle();
	fireBtn->setVisible(true);
	fireBtn->setTouchEnabled(false);
	m_trickBtn->setBtnEnable(true);
	m_pTipImg->setVisible(false);
	removeFireBtns();

	CCPoint screenOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSize stSize = m_trickBtn->getSize();
	CCPoint anchor = m_trickBtn->getAnchorPoint();
	CCPoint stPos = m_trickBtn->getPosition();
	CCPoint stWorldPos = m_trickBtn->getParent()->convertToWorldSpace(stPos);

	CCPoint stOriPos = ccp(stWorldPos.x - stSize.width * anchor.x - screenOrigin.x, stWorldPos.y - stSize.height * anchor.y - screenOrigin.y);
	CCRect stShowRect = CCRectMake(stOriPos.x, stOriPos.y, stSize.width, stSize.height);
	if(P_FIGHT_GUIDE_CTRL->getIsShowForceGuide()){
		P_FIGHT_GUIDE_CTRL->removeGuide();
	}
	P_FIGHT_GUIDE_CTRL->addGuide();
	P_FIGHT_GUIDE_CTRL->getGuideLayout()->show(stShowRect);
}

void CFightGuideScene::moveToEnd(){
	CCPoint stDst = ccp(_screenW*2, m_uWalkHight/2);
	_mainHero->WalkTo(stDst);

	P_FIGHT_GUIDE_CTRL->enterGuideNormalScene();

	ADD_EVT_LOG(EVT_LOG_DEMO_SKILL_DONE); //完成放技能，点击提示进入下一阶段
}

void CFightGuideScene::checkArrivedEdge() {
	unsigned int wallWidth = 10.0f;
	float fW = walkableRect.getMinX() + walkableRect.size.width;
	CCRect rightWall = CCRectMake(fW - wallWidth, 0, wallWidth, walkableRect.size.height);

	const CCPoint &pos = _mainHero->getPosition();
	const CCSize &size = _mainHero->getContentSize();
	CCRect playerRect = CCRectMake(pos.x - size.width * 0.5, pos.y - size.height * 0.5, size.width, size.height);

	if(!m_bIsArrivedEdge) {
		if(rightWall.intersectsRect(playerRect)) {
			if(!P_FIGHT_GUIDE_CTRL->getIsInGuide()){
				m_bIsArrivedEdge = true;
				//P_FIGHT_GUIDE_CTRL->enterGuideNormalScene();

				//ADD_EVT_LOG(EVT_LOG_DEMO_SKILL_DONE); //完成放技能，点击提示进入下一阶段
			}else if(getNowStepType() == MOVE_AHEAD_TYPE){   //引导向前
				m_bIsArrivedEdge = true;
				//UILayer *pTopLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_TOP));
				//CTransitionLayer* pLayer = CTransitionLayer::create();
				//CCPoint stPos = CCDirector::sharedDirector()->getVisibleOrigin();
				////pLayer->setPosition(stPos);
				//pLayer->setZOrder(100);
				//pTopLayer->addWidget(pLayer);
				//pLayer->AddEventListener(this, CTransitionLayer::TRANSITION_COMPLETE, eventListner_selector(CFightGuideScene::handleTransitionComplete));
				//pLayer->startAct();
			}
		}
	}
}


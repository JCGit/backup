#include "NihilityFightScene.h"
#include "ToolUtils.h"
#include "Macros.pb.h"
#include "NihilityUtils.h"
#include "NihilityControl.h"
#include "SceneManager.h"
#include "UserObj.h"
#include "GoldCoin.h"
#include "NihilityLevelTip.h"
#include "TransitionLayer.h"
#include "NextStageTipLayer.h"
#include "CopterEffLayer.h"
#include "QEnvSet.h"
#include "TextHelper.h"
#include "PopTipLayer.h"
#include "ConstHelper.h"
#include "vlib/CVRand.h"
#include "fight/BossWarning.h"
#include "fight/FightTipLayout.h"
#include "instance/InstanceUtils.h"
#include "instance/InstanceFightUtils.h"
#include "control/AudioManager.h"
#include "instance/InstanceFightUtils.h"
#include "fightendless/FightEndlessScoreUI.h"
#include "fight/FightWeaponArrLayout.h"

#include "GlobalCfgMaster.h"
#include "MonsterLevCfgMaster.h"
#include "NihilityMapLibCfgMaster.h"
#include "NihilityPackageCfgMaster.h"
#include "NihilityMonsterShowCfgMaster.h"
#include "NihilityScoreCfgMaster.h"

#define BOSS_GOLD_COIN 20

CNihilityFightScene::CNihilityFightScene()
:m_stSleepMonsters(NULL)
,m_pBossWarning(NULL)
,m_pTipLayer(NULL)
,m_pCopterEff(NULL)
,m_fObjectTime(0)
,m_fStageTime(0)
,m_uCurLevel(0)
,m_uCurStage(0)
,m_uCurMonsterNum(0)
,m_fStartWalkTime(0)
,m_stInitPos(CCPointZero)
,m_bIsArrivedEdge(false)
,m_bIsAutoWalking(false)
,m_uCurMonsterLev(INIT_MONSTER_LEVEL)
{
	m_stSleepMonsters = CCArray::create();
	m_stSleepMonsters->retain();
}


CNihilityFightScene::~CNihilityFightScene(){
	if (m_stSleepMonsters){
		m_stSleepMonsters->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_stSleepMonsters);
	}

	CC_SAFE_RELEASE_NULL(m_pCopterEff);
}

bool CNihilityFightScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CNihilityFightScene::onEnterTransitionDidFinish(){
	CCLOG("CNihilityFightScene------------------------dump all texture cache begin------------");
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	CCLOG("CNihilityFightScene------------------------dump all texture cache end--------------");

	CBaseFightScene::onEnterTransitionDidFinish();

	m_pAutoBtn->setTouchEnabled(false);
	setAttackOpBtnVisible(false);
	if(P_NIHILITY_CTRL->getNeedRush()){  //扫荡，播放扫荡特效后开始关卡
		//计算金币、宝箱数
		countRushAward();

		//直升机特效
		startCopterEff();
	}else{
		startNihility();
	}

	//更新面板
	updateCoin();
	updateScore();
	updatePackage();

	if(P_USER_OBJ->getUserInfo()->weapon.weaponguid() == TEMP_WEAPON_GUID){   //临时枪械
		m_pArrLayout->showTempWeapon(P_USER_OBJ->getUserInfo()->weapon);
	}
	m_pArrLayout->setSelectByGUID(P_USER_OBJ->getUserInfo()->weapon.weaponguid());
}

void CNihilityFightScene::startCopterEff(){
	if(!m_pCopterEff){
		return;
	}

	m_pCopterEff->initUI();
	m_pCopterEff->AddEventListener(this, CopterEffLayer::COPTER_FLY_OUT_EVENT, eventListner_selector(CNihilityFightScene::handleCopterEffOverEvent));
	CCPoint stPos = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pCopterEff->setPosition(stPos);
	addChild(m_pCopterEff, 10);
	m_pCopterEff->start();

	m_pScoreUI->setInfo(P_NIHILITY_CTRL->getFloor());
}

void CNihilityFightScene::handleCopterEffOverEvent(const string& stEvent, CCObject* pstEventObj){
	m_pCopterEff->removeFromParentAndCleanup(true);
	CC_SAFE_RELEASE_NULL(m_pCopterEff);

	startCopterFly();		//直升机降落
}

void CNihilityFightScene::startCopterFly(){
	_mainHero->ForceIdle();
	_mainHero->setVisible(false);
	CCArmature* pArm = CCArmature::create("ma00053");
	pArm->setScale(1.2f);
	pArm->setPosition(_mainHero->getPosition() + ccp(0, 80));
	pArm->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CNihilityFightScene::copterEffCallFun));
	_sceneForegound->addChild(pArm, _mainHero->getZOrder() - 10);
	pArm->getAnimation()->play("downfly", -1, -1, 0);
}

void CNihilityFightScene::copterEffCallFun(CCArmature* armature, MovementEventType type, const char* nameStr){
	string str = nameStr;
	if(type == COMPLETE){
		if(str == "downfly"){
			_mainHero->setVisible(true);
			armature->getAnimation()->play("upfly", -1, -1, 0);
		}else if(str == "upfly"){  
			//飞出屏幕
			copterFlyOut(armature);
		}
	}
}

void CNihilityFightScene::copterFlyOut(CCArmature* armature){
	armature->getAnimation()->play("standfly", -1, -1, 1);

	float x = _mainHero->getPositionX();
	float y = _winSize.height - _mainHero->getPositionY() + armature->getContentSize().height;
	CCActionInterval *pAction = CCMoveTo::create(1.0f, ccp(x, y));
	CCFiniteTimeAction* pFunc = CCCallFuncO::create(this, callfuncO_selector(CNihilityFightScene::copterFlyEndFun), armature);
	armature->runAction(CCSequence::create(pAction, pFunc, NULL));
}

void CNihilityFightScene::copterFlyEndFun(CCObject* pstObject){
	CCArmature* pArm = dynamic_cast<CCArmature*>(pstObject);
	pArm->removeFromParentAndCleanup(true);
	startNihility();
}

void CNihilityFightScene::startNihility(){
	startFight();

	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint stPoint = CCDirector::sharedDirector()->getVisibleOrigin(); 
	CNihilityLevelTip* pLevelTip = CNihilityLevelTip::create();
	pLevelTip->setPosition(stPoint + ccp(stSize.width/2 - pLevelTip->getSize().width/2, stSize.height*2/3));
	m_pUiLayer->addWidget(pLevelTip);
	pLevelTip->showLevelTip(P_NIHILITY_CTRL->getFloor());

	m_pScoreUI->setInfo(P_NIHILITY_CTRL->getFloor());
}

void CNihilityFightScene::startUpdate(){
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

void CNihilityFightScene::countRushAward(){
	unsigned int maxLev = P_NIHILITY_CTRL->getFloor() - 1;
	for(unsigned int i = 0; i < maxLev; i++){
		if(rollPackage(i + 1)){  //掉落宝箱
			P_NIHILITY_CTRL->addAwarFloor(i + 1);
		}

		//金币
		P_NIHILITY_CTRL->addCoin(GLOBAL_CFG_MASTER->GetCfg().nihilitybasegold()*(i + 1)*3);

		//分数
		P_NIHILITY_CTRL->addScore(NIHILITY_SCORE_CFG->GetNihilityScoreCfgByLev(i + 1)->score());
	}

	P_NIHILITY_CTRL->setFloorScore(P_NIHILITY_CTRL->getScore());
}

void CNihilityFightScene::setMapRes(){
	m_uCurStage = NORMAL_TYPE_LEVEL;
	unsigned int curFloor = P_NIHILITY_CTRL->getFloor();
	const NihilityMapLibCfg* stMapCfg = NIHILITY_MAP_LIB_CFG->GetNihilityMapRandByType(NORMAL_TYPE_LEVEL, P_NIHILITY_CTRL->getFloor());
	//const NihilityMapLibCfg* stMapCfg = NIHILITY_MAP_LIB_CFG->GetNihilityMapLibCfgByID(28);
	m_uNormalID = stMapCfg->id();
	m_stFrontStr = stMapCfg->frontid();
	m_stForeStr = stMapCfg->foreid();
	m_stMidStr = stMapCfg->midid();
	m_stBackStr = stMapCfg->backid();
	m_stMid2Str = stMapCfg->midid2();
	_cfgFileName = stMapCfg->cfgid();
	m_uEliteID = NIHILITY_MAP_LIB_CFG->GetNihilityMapRandByTypeAndRes(ELITE_TYPE_LEVEL, curFloor, m_stForeStr)->id();
	m_uBossID = NIHILITY_MAP_LIB_CFG->GetNihilityMapRandByTypeAndRes(BOSS_TYPE_LEVEL, curFloor, m_stForeStr)->id();
}

void CNihilityFightScene::setWalkHight(){
	const NihilityMapLibCfg* stMapCfg = NIHILITY_MAP_LIB_CFG->GetNihilityMapLibCfgByID(m_uNormalID);
	m_uWalkHight = stMapCfg->midwalkh();
	m_fMidPos = stMapCfg->midpos();
}

void CNihilityFightScene::loadMapBgRes(){
	prepAllStageRes();

	CBaseFightScene::loadMapBgRes();

	string str = EFFECT_RES_PATH + "ut00023/ut00023.ExportJson";
	m_stRes.insert(str);
}

void CNihilityFightScene::prepAllStageRes(){
	//预加载精英、boss关卡资源
	unsigned int uType = BOSS_TYPE_LEVEL;
	unsigned int uCfgID = m_uNormalID;
	for(unsigned int i = 0; i < 2; i++){
		uType = BOSS_TYPE_LEVEL - i;
		if(uType == ELITE_TYPE_LEVEL){
			uCfgID = m_uEliteID;
		}else if(uType == BOSS_TYPE_LEVEL){
			uCfgID = m_uBossID;
		}
		const NihilityMapLibCfg* stCfg = NIHILITY_MAP_LIB_CFG->GetNihilityMapLibCfgByID(uCfgID);
		_cfgFileName = stCfg->cfgid();

		//解析
		analyseMapInfo();

		//加载资源
		loadItemRes();
		loadMonsterRes();
		loadFloorBuffRes();
		loadTrapRes();
	}

	//普通阶段
	const NihilityMapLibCfg* stCfg = NIHILITY_MAP_LIB_CFG->GetNihilityMapLibCfgByID(m_uNormalID);
	_cfgFileName = stCfg->cfgid();
	analyseMapInfo();
}

void CNihilityFightScene::loadItemRes(){
	CBaseFightScene::loadItemRes();
}

void CNihilityFightScene::loadFloorBuffRes(){
	CBaseFightScene::loadFloorBuffRes();
}

void CNihilityFightScene::loadTrapRes(){
	CBaseFightScene::loadTrapRes();
}

void CNihilityFightScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();
}

void CNihilityFightScene::loadMonsterRes(){
	m_uCurMonsterNum = 0;

	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		MonsterCfg stMonsterCfg;
		CNihilityUtils::genMonsterCfg(monsterObj->id, stMonsterCfg);
		loadMonsterResByCfg(&stMonsterCfg, monsterObj->num);

		if(monsterObj->type != APPEAR_AFTER_DIE){  //不计入刷怪数量
			m_uCurMonsterNum += monsterObj->num;
		}
	}

	if(P_NIHILITY_CTRL->getNeedRush()){  //扫荡需要的资源
		loadBrushRes();
	}
}

void CNihilityFightScene::loadBrushRes(){
	string str = MONSTER_RES_PATH + "ma00053" + "/" + "ma00053" + JSON_TYPE_NAME;
	m_stRes.insert(str);

	str = "ui_bg/sky.png";
	m_stRes.insert(str);

	m_pCopterEff = CopterEffLayer::create();
	m_pCopterEff->retain();

	CCObject* pstObj;
	CCARRAY_FOREACH(m_pCopterEff->getMonsterData(), pstObj){
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		MonsterCfg stMonsterCfg;
		CNihilityUtils::genMonsterCfg(monsterObj->id, stMonsterCfg);
		loadMonsterResByCfg(&stMonsterCfg, 10);
	}
}

void CNihilityFightScene::setMonsterNum(){
	m_uCurMonsterNum = 0;

	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		if(monsterObj->type != APPEAR_AFTER_DIE){  //不计入刷怪数量
			m_uCurMonsterNum += monsterObj->num;
		}
	}
}

void CNihilityFightScene::initMap(){
	CBaseFightScene::initMap();

	walkableRect = CCRect(0, 0, _screenW*FIGHT_STAGE_NUM, m_uWalkHight);
}

void CNihilityFightScene::initFightData(){
	CBaseFightScene::initFightData();

	m_bIsArrivedEdge = false;
	m_bIsAutoWalking = false;
	m_fStartWalkTime = 0;
	m_fStageTime = 0;
}

void CNihilityFightScene::initSceneItem(){
	if(_itemData->count() <= 0){
		updateMapBlocks();
		return;
	}

	CCObject* pstObj;
	CItemObj* itemObj;
	CCARRAY_FOREACH(_itemData, pstObj) {
		itemObj = (CItemObj*)pstObj;
		if(itemObj->touchId != 0){  //触发id不为0，表示按触发线触发
			continue;
		}else{
			//添加物件
			addSceneItem(itemObj);
		}
	}

	updateMapBlocks();
}

void CNihilityFightScene::initFloorBuff(){
	CCObject* pstObj;
	CFloorBuffObj* buffObj;
	CCARRAY_FOREACH(_floorBuffData, pstObj) {
		buffObj = (CFloorBuffObj*)pstObj;

		if(buffObj->touchId != 0){  //触发id不为0，表示按触发线触发
			continue;
		}else{
			//添加floorbuff
			if(FLOOR_BUFF_CFG->GetFloorBuffCfgByID(buffObj->id)->bufftype() == BUFF_TYPE_WEAPON){
				addWeaponBuff(buffObj->id, buffObj->party, ccp(buffObj->x, _sceneForegound->getContentSize().height - buffObj->y), true);
			}else{
				addFloorBuff(buffObj->id, buffObj->party, ccp(buffObj->x, _sceneForegound->getContentSize().height - buffObj->y), true);
			}
		}
	}
}

void CNihilityFightScene::initTrap(){
	CCObject* pstObj;
	CTrapObj* trapObj;
	CCARRAY_FOREACH(_trapData, pstObj) {
		trapObj = (CTrapObj*)pstObj;

		//触发id不为0，且不是全屏穿刺型陷阱，表示按触发线触发
		if(trapObj->touchId != 0 && TRAP_CFG_MASTER->GetTrapCfgByID(trapObj->id)->type() != TRAP_TYPE_PUNCTURE){  
			continue;
		}else{
			//添加陷阱
			if(TRAP_CFG_MASTER->GetTrapCfgByID(trapObj->id)->type() == TRAP_TYPE_PUNCTURE){
				addPunctureTrap(trapObj, true);
			}else{
				addSceneTrap(trapObj, true);
			}
		}
	}
}

void CNihilityFightScene::initMapUI(){
	CBaseFightScene::initMapUI();

	if(P_NIHILITY_CTRL->getbIsAutoFight()){
		m_pAutoBtn->setVisible(false);
		m_pAutoBtn->setTouchEnabled(false);
	}else{
		m_pAutoBtn->setVisible(true);
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pScoreUI = CFightEndlessScoreUI::create();
	m_pScoreUI->setScale(0.75f);
	m_pScoreUI->setPosition(visibleOrigin + ccp(20, m_pHeroHeadUI->getPositionY() - m_pScoreUI->getSize().height*m_pScoreUI->getScale() + 10));
	m_pUiLayer->addWidget(m_pScoreUI);
}

void CNihilityFightScene::initMainHero(){
	CBaseFightScene::initMainHero();

	m_stInitPos = ccp(300, m_uWalkHight/2);
	_mainHero->SetEnityPosition(m_stInitPos);
	_mainHero->setDesiredPosition(_mainHero->getPosition());

	setViewpointCenter(_mainHero->getPosition());
}

void CNihilityFightScene::initMonster(){
	m_uCurLevel = P_NIHILITY_CTRL->getFloor();
	//初始化休眠的怪物放在舞台上
	m_stSleepMonsters->removeAllObjects();
	initSleepMonsters();
}

void CNihilityFightScene::initSleepMonsters(){
	CCObject* pstObj;
	CEnemy* pstMonster;
	CEnemyData* pstMonsterObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		pstMonsterObj = (CEnemyData*)pstObj;

		if(pstMonsterObj->type != APPEAR_SLEEP){		
			continue;
		}

		pstMonster = initOneMonster(pstMonsterObj);
		if(pstMonster != NULL){
			pstMonster->LoadActionRes();
			pstMonster->SleepAct();
			int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
			_sceneForegound->addChild(pstMonster, iNewZorder);
			m_stSleepMonsters->addObject(pstMonster);

			if(!pstMonster->getBlockSize().equals(CCSizeZero)){
				getBlocksInSize(pstMonster->getBlockSize(), (CCPoint)pstMonster->getPosition(), true);
				updateMapBlocks();
			}
		}
	}
}

void CNihilityFightScene::update(float delta){
	CBaseFightScene::update(delta);

	checkBrushSceneItem();
	checkArrivedEdge();
	setViewpointCenter(_mainHero->getPosition());
	
	m_fObjectTime += delta;
	m_fStageTime += delta;
}

void CNihilityFightScene::brushMonsterLogic(float dt){
	if(m_bIsGameOver){
		return;
	}

	CCObject* pstObj;
	CEnemyData* pstMonsterObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		pstMonsterObj = (CEnemyData*)pstObj;

		CBurshObj stBrushObj;
		stBrushObj.pLayer = this;
		stBrushObj.pstEnemyObj = pstMonsterObj;
		stBrushObj.iBrushTime = m_fFightTime;
		bool isBrush = CInstanceFightUtils::BrushMonsterLogic(stBrushObj);
	}
}

string CNihilityFightScene::getEmergeByData(CEnemyData* pstObj){
	MonsterCfg stCfg;
	//unsigned int normalLev = INIT_MONSTER_LEVEL + (m_uCurLevel - 1);
	unsigned int normalLev = NIHILITY_SCORE_CFG->GetNihilityScoreCfgByLev(m_uCurLevel)->monsterlev();
	CNihilityUtils::genMonsterCfg(pstObj->id, stCfg, normalLev);
	return stCfg.actions().emerge();
}

void CNihilityFightScene::checkBrushSceneItem(){
	map<int, int>::iterator stIter;
	for(stIter = m_stTouchData.begin(); stIter != m_stTouchData.end(); stIter++){
		int dis = stIter->second;
		if(_mainHero->getPositionX() < dis){
			continue;
		}

		int touchID = stIter->first;
		brushSceneItemByTouch(touchID);
		brushFloorBuffByTouch(touchID);
		brushTrapByTouch(touchID);
	}
}

void CNihilityFightScene::brushSceneItemByTouch(unsigned int touchID){
	CCObject* pstObj;
	CItemObj* itemObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		itemObj = (CItemObj*)pstObj;
		if(itemObj->touchId == touchID){  //添加物件
			addSceneItem(itemObj);
		}
	}
}

void CNihilityFightScene::brushFloorBuffByTouch(unsigned int touchID){
	CCObject* pstObj;
	CFloorBuffObj* buffObj;
	CCARRAY_FOREACH(_floorBuffData, pstObj) {
		buffObj = (CFloorBuffObj*)pstObj;
		if(buffObj->touchId == touchID){  
			//添加floorbuff
			if(FLOOR_BUFF_CFG->GetFloorBuffCfgByID(buffObj->id)->bufftype() == BUFF_TYPE_WEAPON){
				addWeaponBuff(buffObj->id, buffObj->party, ccp(buffObj->x, _sceneForegound->getContentSize().height - buffObj->y));
			}else{
				addFloorBuff(buffObj->id, buffObj->party, ccp(buffObj->x, _sceneForegound->getContentSize().height - buffObj->y));
			}
		}
	}
}

void CNihilityFightScene::brushTrapByTouch(unsigned int touchID){
	CCObject* pstObj;
	CTrapObj* trapObj;
	CCARRAY_FOREACH(_trapData, pstObj) {
		trapObj = (CTrapObj*)pstObj;
		if(trapObj->touchId == touchID && TRAP_CFG_MASTER->GetTrapCfgByID(trapObj->id)->type() != TRAP_TYPE_PUNCTURE){
			addSceneTrap(trapObj);
		}
	}
}

void CNihilityFightScene::handlePassLogic(){
	
}


void CNihilityFightScene::handleGiveUpLogic(CCObject* pstObj){
	P_NIHILITY_CTRL->setIsForceQuit(false);
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, GIVEUP_FIGHT_EVENT);
	handleGameOver();
}

void CNihilityFightScene::handleGameOver(){
	m_bIsGameOver = true;
	m_bIsPreOver = true;
	stopAllMonsters();
	this->unscheduleUpdate();
	this->unschedule(schedule_selector(CNihilityFightScene::updateCoinPos));
	P_NIHILITY_CTRL->setFloor(m_uCurLevel);
	P_NIHILITY_CTRL->sendNihilityFightEndRqst(m_uEvolutionCnt);

	P_NIHILITY_CTRL->getResultPanel()->showLoading();

	removeAutoFightLayer();
}

void CNihilityFightScene::showBossWarning(){
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	if(m_pBossWarning && m_pBossWarning->getParent()){
		m_pBossWarning->stopWarning();
		m_pBossWarning->removeFromParentAndCleanup(true);
		m_pBossWarning = NULL;
	}

	if(m_pBossWarning == NULL){
		m_pBossWarning = CBossWarning::create(); 
		m_pBossWarning->AddEventListener(this, CBossWarning::BOSS_WARNING_COMPLETE, eventListner_selector(CNihilityFightScene::handleBossWarningCompleteEvent));
	}
	m_pBossWarning->setTouchEnabled(false);
	m_pBossWarning->setPosition(visibleOrigin + ccp(stSize.width, stSize.height));
	m_pUiLayer->addWidget(m_pBossWarning);

	m_pBossWarning->showWarning();
}

CEnemy* CNihilityFightScene::initOneMonster(CEnemyData* monsterObj){
	MonsterCfg stCfg;
	//unsigned int normalLev = INIT_MONSTER_LEVEL + (m_uCurLevel - 1);
	unsigned int normalLev = NIHILITY_SCORE_CFG->GetNihilityScoreCfgByLev(m_uCurLevel)->monsterlev();
	CNihilityUtils::genMonsterCfg(monsterObj->id, stCfg, normalLev);
	CEnemy* pstMonster = CBaseFightScene::initOneMonster(monsterObj, &stCfg);
	return pstMonster;
}

bool CNihilityFightScene::checkWinLogic(unsigned int cfgID){
	const NihilityMapLibCfg* stCfg = NIHILITY_MAP_LIB_CFG->GetNihilityMapLibCfgByID(getStageMapID(m_uCurStage));
	unsigned int winType = stCfg->wintype();
	unsigned int winValue1 = stCfg->winvalue1();
	unsigned int winValue2 = stCfg->winvalue2();

	if(winType == WIN_TYPE_CLEAN_ALL){  //杀死所有怪物
		if(m_uCurMonsterNum <= 0){
			m_bIsGameOver = true;
			m_bIsPreOver = true;
			return true;
		}
	}else if(winType == WIN_TYPE_KILL_ONE){  ////消灭某个ID
		if(winValue1 == cfgID){
			m_bIsGameOver = true;
			m_bIsPreOver = true;
			return true;
		}
	}

	return false;
}

void CNihilityFightScene::addNextStageTip(){
	if(m_pTipLayer && m_pTipLayer->getParent() != NULL){
		m_pTipLayer->removeFromParentAndCleanup(true);
		m_pTipLayer = NULL;
	}

	m_pTipLayer = CNextStageTipLayer::create();
	UILayer *pTopLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_TOP));
	CCPoint stPos = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pTipLayer->setPosition(stPos);
	m_pTipLayer->setZOrder(100);
	pTopLayer->addWidget(m_pTipLayer);
	m_pTipLayer->startAct();

	clearPreStage();
	mainHeroCeaseShoot();
	_mainHero->ForceIdle();
	_hudLayer->Inactive();
	m_pUiLayer->removeAllFireBtn();
	setAtkBtnEnable(false);

	CCPoint stDst = ccp(walkableRect.size.width, m_uWalkHight/2);
	_mainHero->WalkTo(stDst);

	m_bIsAutoWalking = true;
	m_bIsWalkNextStage = true;
	m_fStartWalkTime = m_fFightTime;
}

void CNihilityFightScene::removeStageTip(){
	_hudLayer->Active();
	setAtkBtnEnable(true);

	if(m_pTipLayer && m_pTipLayer->getParent()){
		m_pTipLayer->removeFromParentAndCleanup(true);
		m_pTipLayer = NULL;
	}
}

void CNihilityFightScene::setAtkBtnEnable(bool isEnable){
	fireBtn->setTouchEnabled(isEnable);
	m_trickBtn->setBtnEnable(isEnable);
	m_skillBtn->setBtnEnable(isEnable);
	m_dodgeBtn->setBtnEnable(isEnable);
	pauseBtn->setTouchEnabled(isEnable);
}

void CNihilityFightScene::checkArrivedEdge() {
	unsigned int wallWidth = 10.0f;
	CCRect rightWall = CCRectMake(walkableRect.size.width - wallWidth, 0, wallWidth, walkableRect.size.height);

	const CCPoint &pos = _mainHero->getPosition();
	const CCSize &size = _mainHero->getContentSize();
	CCRect playerRect = CCRectMake(pos.x - size.width * 0.5, pos.y - size.height * 0.5, size.width, size.height);

	if(!m_bIsArrivedEdge) {
		if((m_fFightTime - m_fStartWalkTime > ConstHelper(FIGHT_STAGE_WALK_TIME)*1.0f/1000 || rightWall.intersectsRect(playerRect)) 
			&& m_bIsGameOver && m_bIsAutoWalking) {

			m_bIsArrivedEdge = true;
			//金币增加
			P_NIHILITY_CTRL->addCoin(GLOBAL_CFG_MASTER->GetCfg().nihilitybasegold()*m_uCurLevel);

			if(m_uCurStage < BOSS_TYPE_LEVEL){
				nextStage();
			}else{
				P_NIHILITY_CTRL->setFloor(m_uCurLevel);
				CVArray<float, 6> stArr;
				m_pArrLayout->recordAllBulletVal(stArr);
				P_NIHILITY_CTRL->updateAllWeaponBulletVal(stArr);
				CVArray<float, 6> stArr2 = P_NIHILITY_CTRL->getAllWeaponBulletVal();
				SceneManager::sharedSceneManager()->enterScene(NIHILITY_GLOD_SCENE);

				removeAutoFightLayer();
				
				if(m_pTipLayer && m_pTipLayer->getParent()){
					m_pTipLayer->removeFromParentAndCleanup(true);
					m_pTipLayer = NULL;
				}
			}
		}
	}
}

void CNihilityFightScene::nextStage(){
	//过渡场景
	CTransitionLayer* pLayer = CTransitionLayer::create();
	CCPoint stPos = CCDirector::sharedDirector()->getVisibleOrigin();
	UILayer *pTopLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_TOP));
	//pLayer->setPosition(stPos);
	pLayer->setZOrder(101);
	pTopLayer->addWidget(pLayer);
	pLayer->AddEventListener(this, CTransitionLayer::TRANSITION_MID_EVENT, eventListner_selector(CNihilityFightScene::handleTransitionMidEvent));
	pLayer->AddEventListener(this, CTransitionLayer::TRANSITION_COMPLETE, eventListner_selector(CNihilityFightScene::handleTransitionComplete));
	pLayer->startAct();
}

void CNihilityFightScene::handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj){
	//重置
	resetStage();

	m_uCurStage++;
	const NihilityMapLibCfg* stCfg = NIHILITY_MAP_LIB_CFG->GetNihilityMapLibCfgByID(getStageMapID(m_uCurStage));
	_cfgFileName = stCfg->cfgid();
	analyseMapInfo();
	setMonsterNum();
}

void CNihilityFightScene::handleTransitionComplete(const string& stEvent, CCObject* pstEventObj){
	//当前阶段
	initTrap();
	initMonster();
	initSceneItem();
	initFloorBuff();
	updateCoin();
}

void CNihilityFightScene::resetStage(){
	//reset mainhero
	_mainHero->SetEnityPosition(m_stInitPos);
	_mainHero->setDesiredPosition(_mainHero->getPosition());
	setViewpointCenter(_mainHero->getPosition());
	_mainHero->ForceIdle();
	_mainHero->resetScale();

	removeStageTip();
	initFightData();
}

void CNihilityFightScene::clearPreStage(){
	clearAllBuffs();
	clearAllTraps();
	clearAllArrows();
	clearAllMonsters();
	clearAllSceneItems();
	clearAllBullet();
	clearAllSkills();
}

unsigned int CNihilityFightScene::getStageMapID(unsigned int uStageIdx){
	unsigned int uCfgID = m_uNormalID;

	switch (uStageIdx)
	{
	case NORMAL_TYPE_LEVEL:
		uCfgID = m_uNormalID;
		break;
	case ELITE_TYPE_LEVEL:
		uCfgID = m_uEliteID;
		break;
	case BOSS_TYPE_LEVEL:
		uCfgID = m_uBossID;
		break;
	default:
		break;
	}

	return uCfgID;
}

int CNihilityFightScene::rollPackage(unsigned int uLv){
	const NihilityPackageCfg* stCfg = NIHILITY_PACKAGE_CFG_MASTER->GetNihilityPackageCfgByLv(uLv);
	unsigned int uPageCnt = P_NIHILITY_CTRL->getAwardFloorCount();

	unsigned int totalCnt = 0;
	for(unsigned int idx = 0; idx < stCfg->id(); idx++){
		totalCnt += NIHILITY_PACKAGE_CFG_MASTER->GetNihilityPackageCfgByID(idx + 1)->dropcnt();
	}

	if(uPageCnt < totalCnt){
		unsigned int lessLev = stCfg->maxlev() - uLv + 1;
		if(uPageCnt + (stCfg->maxlev() - uLv + 1) <= totalCnt){   //当剩余的层数必掉
			return 1;
		}else{
			unsigned int roll = CRandom::GetRandHaveMax(RANDOM_DOMINATOR);
			unsigned int prob = GLOBAL_CFG_MASTER->GetCfg().nihilitydropprob();

			if(roll <= prob){
				return 1;
			}else{
				return 0;
			}
		}
	}else{
		return 0;
	}
}

void CNihilityFightScene::updateScore(){
	m_pScoreUI->setScore(P_NIHILITY_CTRL->getScore());
}

void CNihilityFightScene::updatePackage(bool bIsShowEff){
	if(bIsShowEff){
		CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
		CCArmature* pArm = CCArmature::create("ut00023");
		pArm->setPosition(visibleOrigin + ccp(stSize.width/2, stSize.height/2));
		//pArm->setPosition(m_pScoreUI->getPosition() + ccp(70, 40));
		pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CNihilityFightScene::handlePackageCallBack));
		pArm->getAnimation()->play("fly", -1, -1, 0);
		m_pUiLayer->addChild(pArm, 100);
	}else{
		m_pScoreUI->setPackage(P_NIHILITY_CTRL->getAwardFloorCount());
	}
}

void CNihilityFightScene::handlePackageCallBack(CCArmature* armature, MovementEventType type, const char* nameStr){
	string strEvt = nameStr;
	if(type == COMPLETE){
		if(strEvt == "fly"){
			armature->getAnimation()->play("fly_feiXing", -1, -1, 1);
			CCMoveTo* pMove = CCMoveTo::create(0.5f, m_pScoreUI->getPosition() + ccp(60, 37));
			CCFiniteTimeAction* endFun = CCCallFuncN::create(this, callfuncN_selector(CNihilityFightScene::packEndFun));
			armature->runAction(CCSequence::create(pMove, endFun, NULL));
		}else if(strEvt == "fly_jieShu"){
			armature->removeFromParentAndCleanup(true);
		}
	}
}

void CNihilityFightScene::packEndFun(CCNode* pNode){
	CCArmature* pArm = dynamic_cast<CCArmature*>(pNode);
	if(pArm){
		pArm->getAnimation()->play("fly_jieShu", -1, -1, 0);
		m_pScoreUI->setPackage(P_NIHILITY_CTRL->getAwardFloorCount());
	}
}

void CNihilityFightScene::updateCoin(){
	m_pScoreUI->setCoin(P_NIHILITY_CTRL->getCoin());
}

//void CNihilityFightScene::btnTouchEvent(CCObject *pSender, TouchEventType type){
//	switch (type)
//	{
//	case TOUCH_EVENT_ENDED:
//		if (pSender->m_uID == fireBtn->m_uID){
//			goldEff(ccp(500, 200));
//		}
//		break;
//	}
//}

void CNihilityFightScene::handleCoinEndEvent(){
	updateCoin();
}

void CNihilityFightScene::autoFightHandle(){
	m_pAutoBtn->setTouchEnabled(false);
	P_NIHILITY_CTRL->setbIsAutoFight(true);
}

void CNihilityFightScene::handleCancelAutoEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleCancelAutoEvent(stEvent, pstEventObj);

	m_pAutoBtn->setTouchEnabled(true);
	P_NIHILITY_CTRL->setbIsAutoFight(false);
}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CNihilityFightScene::handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	if(!P_NIHILITY_CTRL->getNeedRush()){
		startUpdate();
	}
}

void CNihilityFightScene::doHeroDieFail(){
	//猪脚死亡
	P_NIHILITY_CTRL->setIsForceQuit(false);
	handleGameOver();

	if(_stageEntities->indexOfObject(_mainHero) != UINT_MAX){
		_stageEntities->removeObject(_mainHero);
	}
}

void CNihilityFightScene::handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	unsigned int cfgID = monster->getMonsterObj()->id;
	unsigned int roleType = monster->getRoleType();
	const NihilityMonsterShowCfg* stCfg = NIHILITY_MONSTER_SHOW_CFG->GetNihilityMonsterShowCfgByID(cfgID);
	unsigned int dropCoin = stCfg->dropgoldnum();

	if(stCfg->floorbuffid() != 0){
		addFloorBuff(stCfg->floorbuffid(), monster->getRoleParty(), monster->GetFootPos());
	}

	//怪物死亡，数据变化
	handleHeroKillValueChange(stEvent, pstEventObj);

	//分数增加
	P_NIHILITY_CTRL->addScore(stCfg->score());
	updateScore();

	//boss死亡，计算宝箱掉落
	if(monster->getRoleType() == ROLE_TYPE_SENIOR_BOSS){
		bool bIsShowEff = false;
		if(rollPackage(m_uCurLevel)){
			P_NIHILITY_CTRL->addAwarFloor(m_uCurLevel);  //加入索引
			bIsShowEff = true;
		}

		updatePackage(bIsShowEff);

		dropCoin = stCfg->dropgoldnum()*m_uCurLevel*BOSS_TYPE_LEVEL;
	}

	//金币掉落特效
	if(dropCoin != 0){
		addCoinEff(monster, dropCoin);

		//金币增加
		P_NIHILITY_CTRL->addCoin(dropCoin);
	}

	//检查通过条件
	if(checkWinLogic(cfgID)){  //通关,出现提示
		forceOhterMonstersDie();	//其他怪物都死
	}
}

void CNihilityFightScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	unsigned int cfgID = pstEntity->getMonsterObj()->id;
	unsigned int roleType = pstEntity->getRoleType();
	CCPoint stPos = pstEntity->getPosition();
	CCSize stSize = pstEntity->getContentSize();
	const NihilityMonsterShowCfg* stCfg = NIHILITY_MONSTER_SHOW_CFG->GetNihilityMonsterShowCfgByID(cfgID);
    
	//把已经播放完毕死亡动画的死人做掉
	unsigned int mID = pstEntity->m_uID;

	if(pstEntity->getMyState() == kActionStateDie){  
        if(stCfg && pstEntity->getMonsterObj()->type != APPEAR_AFTER_DIE) {
            string str = MONSTER_RES_PATH + stCfg->avatar() + "/" + stCfg->avatar() + JSON_TYPE_NAME;
            updateResToNumByStr(str, false, 1);
        }

		if(pstEntity->getIsBlock()){  //去掉阻挡
			removeBlocksInSize(pstEntity->getBlockSize(), (CCPoint)pstEntity->getPosition(), true);
			updateMapBlocks();
		}

		_sceneForegound->removeChild(pstEntity, true);
		if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
			_stageEntities->removeObject(pstEntity);
		}
	}

	//检查通过条件
	if(checkWinLogic(cfgID)){  //通关,出现提示
		addNextStageTip();
	}
}

void CNihilityFightScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);

	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	if(pstEntity){
		CEnemyData* pstEnemyData = pstEntity->getMonsterObj();

		if(pstEnemyData->type != APPEAR_AFTER_DIE){
			--m_uCurMonsterNum;
		}else{
			pstEnemyData->brushTime = m_fFightTime;
		}

		CCLOG("=====the monster be killed, and the number of last monster is ==== %d", m_uCurMonsterNum);

		//boss死亡，血条消失
		if(pstEntity->getRoleType() == ROLE_TYPE_SENIOR_BOSS || pstEntity->getRoleType() == ROLE_TYPE_JUNIOR_BOSS){
			if(m_pBossHPUI->isVisible()){
				m_pBossHPUI->setVisible(false);
			}

			if(m_pBossWarning && m_pBossWarning->getParent()){  //boss警告去掉
				m_pBossWarning->RemoveEventListener(this, CBossWarning::BOSS_WARNING_COMPLETE, eventListner_selector(CNihilityFightScene::handleBossWarningCompleteEvent));
				m_pBossWarning->stopWarning();
				m_pBossWarning->removeFromParentAndCleanup(true);
				m_pBossWarning = NULL;
			}

			//去掉受伤特效
			CCArmature* pHeroHurtArm = dynamic_cast<CCArmature*>(m_pUiLayer->getChildByTag(CBaseFightScene::HURT_ARM_TAG));
			if(pHeroHurtArm != NULL){
				pHeroHurtArm->stopAllActions();
				m_pUiLayer->removeChildByTag(CBaseFightScene::HURT_ARM_TAG);
			}
		}
	}
}

void CNihilityFightScene::handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleSceneItemFallEvent(stEvent, pstEventObj);
	CSceneItem* item = dynamic_cast<CSceneItem*>(pstEventObj);

	if(item && item->getItemState() == DIE_STATE){
		CItemObj* stInfo = item->getItemObj();
		CCPoint stPos = item->getPosition();
		//物件的掉落
		if(stInfo->dropId != 0){
			if(stInfo->dropType == DROP_TYPE_BUFF){  //地表buff
				//相对与玩家自己
				addFloorBuff(stInfo->dropId, stInfo->dropParty, stPos);
			}else if(stInfo->dropType == DROP_TYPE_WEAPON){
				addWeaponBuff(stInfo->dropId, stInfo->dropParty, stPos);
			}
		}

		if(item->getItemType() == ITEM_TYPE_BLOCK_DESTROY || item->getItemType() == ITEM_TYPE_BLOCK_UNDESTROY){  //阻挡
			removeBlocksInSize(item->getBlockSize(), (CCPoint)item->getPosition(), true);
		}

		updateMapBlocks();
		if(_sceneItems->indexOfObject(item) != UINT_MAX){
			_sceneItems->removeObject(item);
		}

		item->removeFromParentAndCleanup(true);
	}
}

void CNihilityFightScene::handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj){
	CSceneItem* item = dynamic_cast<CSceneItem*>(pstEventObj);

	if(item->getItemType() == ITEM_TYPE_BLOCK_DESTROY || item->getItemType() == ITEM_TYPE_BLOCK_UNDESTROY){  //阻挡
		removeBlocksInSize(item->getBlockSize(), (CCPoint)item->getPosition(), true);
	}

	updateMapBlocks();
	if(_sceneItems->indexOfObject(item) != UINT_MAX){
		_sceneItems->removeObject(item);
	}

	item->removeFromParentAndCleanup(true);
}

void CNihilityFightScene::handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	m_pBossWarning->removeAllChildrenWithCleanup(true);
	m_pBossWarning = NULL;
}


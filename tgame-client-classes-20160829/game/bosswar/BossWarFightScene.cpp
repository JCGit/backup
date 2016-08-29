#include "BossWarFightScene.h"
#include "TimeEx.h"
#include "UserObj.h"
#include "BossWarControl.h"
#include "HeroReviveLayer.h"
#include "BossWarInfoLayout.h"
#include "Global.h"
#include "BossWarWaitLayer.h"
#include "instance/InstanceUtils.h"
#include "nihility/NihilityUtils.h"
#include "fight/FightWeaponArrLayout.h"

#include "MapCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "WorldBossShowLibCfgMaster.h"

CBossWarFightScene::CBossWarFightScene(){
	m_uBossShowID = 0;
	m_uInitBossHp = 0;
	m_uPreBossHp = 0;
	m_bIsUpSkillID = false;
	m_bIsShowWait = false;
	m_pBoss = NULL;
	m_pWaitLayer = NULL;
	m_pReviveLayer = NULL;
	m_pWarInfoLayout = NULL;
	m_stMainHeroInitPos = ccp(300, 200);
	m_stBossInitPos = CCPointZero;
}

CBossWarFightScene::~CBossWarFightScene(){
	CC_SAFE_RELEASE_NULL(m_pWaitLayer);
	CC_SAFE_RELEASE_NULL(m_pReviveLayer);
	CC_SAFE_RELEASE_NULL(m_pBoss);

	P_BOSS_WAR_CTRL->setBossWarScene(NULL);
}

bool CBossWarFightScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CBossWarFightScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	P_BOSS_WAR_CTRL->setBossWarScene(this);

	const CSBossChallengeInfo& stInfo = P_BOSS_WAR_CTRL->getBossChallengeInfo();
	unsigned int uStartTime = stInfo.starttime();
	unsigned int uNowTime = P_GLOBAL->getSvrTimeRelative();
	m_bIsShowWait = (uNowTime < uStartTime);

	m_pBoss->SyncHP(stInfo.bosscurhp());
	m_pBossHPUI->setBossType(m_pBoss->getRoleType(), m_pBoss->getHeadRes());
	m_pBossHPUI->setHPPercent(m_pBoss->getHP(), m_pBoss->getMaxHP());

	m_uInitBossHp = stInfo.bosscurhp();

	if(m_bIsShowWait){
		showWaitLayer();
	}else{
		startFight();
	}
}

void CBossWarFightScene::loadMapBgRes(){
	CBaseMapScene::loadMapBgRes();
}

void CBossWarFightScene::loadItemRes(){
	CBaseFightScene::loadItemRes();
}

void CBossWarFightScene::loadFloorBuffRes(){
	CBaseFightScene::loadFloorBuffRes();
}

void CBossWarFightScene::loadTrapRes(){
	CBaseFightScene::loadTrapRes();
}

void CBossWarFightScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();
}

void CBossWarFightScene::loadMonsterRes(){
	CBaseFightScene::loadMonsterRes();

	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		if(monsterObj->id == 1){
			m_stBossInitPos = ccp(monsterObj->x, monsterObj->y);
		}
	}

	unsigned int uDay = TimeEx::getTime()->tm_wday + 1;
	m_uBossShowID = WORLD_BOSS_SHOW_LIB_CFG->GetWorldBossCfgByDay(uDay)->cfgid();
	MonsterCfg stMonsterCfg;
	CNihilityUtils::genMonsterCfg(m_uBossShowID, stMonsterCfg);
	loadMonsterResByCfg(&stMonsterCfg, 100);
}

void CBossWarFightScene::initSceneItem(){

}

void CBossWarFightScene::initFloorBuff(){

}

void CBossWarFightScene::initTrap(){

}

void CBossWarFightScene::setWalkHight(){
	m_uWalkHight = MAP_CFG_MASTER->GetMapCfgByID(WORLD_BOSS_SCENE_TYPE)->midwalkh(); 
}

void CBossWarFightScene::initMap(){
	CBaseFightScene::initMap();
	walkableRect = CCRect(0, 0, _screenW, m_uWalkHight);
	updateMapBlocks();
}

void CBossWarFightScene::initMapUI(){
	CBaseFightScene::initMapUI();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pWarInfoLayout = CBossWarInfoLayout::create();
	m_pWarInfoLayout->setPosition(visibleOrigin + ccp(20, m_pHeroHeadUI->getPositionY() - m_pWarInfoLayout->getSize().height));
	m_pUiLayer->addWidget(m_pWarInfoLayout);

	m_pBossHPUI->setVisible(true);

	m_pArrLayout->setEnabled(false);
	m_pChangeWeaponBtn->setEnabled(false);
}

void CBossWarFightScene::initMainHero(){
	CBaseFightScene::initMainHero();
	_mainHero->setIsBulletLimit(false);
}

void CBossWarFightScene::initMonster(){
	unsigned int uLv = P_BOSS_WAR_CTRL->getBossLv();
	m_pBoss = initOneDummyBoss(m_uBossShowID, uLv);
	m_pBoss->retain();
	m_pBoss->LoadActionRes(false);
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, m_pBoss->getPositionY());
	_sceneForegound->addChild(m_pBoss, iNewZorder);
	if(!m_pBoss->getBlockSize().equals(CCSizeZero)){
		getBlocksInSize(m_pBoss->getBlockSize(), (CCPoint)m_pBoss->getPosition(), true);
		updateMapBlocks();
	}

	m_pBoss->setScaleX(-1);
	m_pBoss->SetIsControlledByAI(false);
}

CDummyBoss* CBossWarFightScene::initOneDummyBoss(unsigned int uCfgID, unsigned int uLv){
	MonsterCfg monsterCfg;
	CNihilityUtils::genBossCfg(uCfgID, monsterCfg, uLv);

	CDummyBoss* pstMonster = CDummyBoss::create();
	pstMonster->setDesiredPosition(_mainHero->getPosition());
	pstMonster->setNameStr(monsterCfg.monstername());
	pstMonster->setResPath(monsterCfg.avatar());
	pstMonster->setIsNeedPath(monsterCfg.walktype() == 1);
	pstMonster->setAIType(monsterCfg.aiid());
	pstMonster->setHeadRes(monsterCfg.headimg());
	pstMonster->setPatrolArea(monsterCfg.patrolarea());

	CEnemyData* monsterObj = CEnemyData::create();
	monsterObj->x = m_stBossInitPos.x;
	monsterObj->y = m_stBossInitPos.y;
	monsterObj->type = APPEAR_NORMAL;
	pstMonster->setMonsterObj(monsterObj);

	CQStageEntityObj* monsterInfo = CQStageEntityObj::create();
	monsterInfo->setInfoByMonsterCfg(&monsterCfg);
	pstMonster->setObj(monsterInfo);
	
	CCPoint stBossPos = ccp(m_stBossInitPos.x, _sceneForegound->getContentSize().height - m_stBossInitPos.y);
	pstMonster->setCenterToBottom(55);
	pstMonster->setCenterToSides(55);
	pstMonster->SetEnityPosition(stBossPos);
	pstMonster->setDesiredPosition(stBossPos);
	pstMonster->setPosition(stBossPos);
	pstMonster->setHurtActionCD(monsterCfg.hurtactioncd());
	pstMonster->setLevel(monsterCfg.level());

	for(int j = 0; j < monsterCfg.skills_size(); j++){
		CQSkillCell* pstSkill = CQSkillCell::create();	
		pstSkill->m_uSkillID = monsterCfg.skills(j);
		pstMonster->getSkillArray()->addObject(pstSkill);
	}

	pstMonster->AddEventListener(this, CQStageEntity::ATK_ACTION_KEY_FRAME, eventListner_selector(CBossWarFightScene::handleCloseAtkKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::SKILL_CAST_FRAME, eventListner_selector(CBossWarFightScene::handleSkillKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::FIRE_EVENT, eventListner_selector(CBossWarFightScene::handleFireABulletEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_EFF_FRAME, eventListner_selector(CBossWarFightScene::handleDieEffKeyFrameEvent));
	pstMonster->AddEventListener(this, CQStageEntity::SKILL_CAST_IN_ACTION, eventListner_selector(CBossWarFightScene::handleEnemySkillActionEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_MOMENT_EVENT, eventListner_selector(CBossWarFightScene::handleEnemyDieMovementEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_COMPLETE_EVENT, eventListner_selector(CBossWarFightScene::handleEnemyDieCompleteEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DASH_COMPLETE_EVENT, eventListner_selector(CBossWarFightScene::handleEnemyDashCompleteEvent));
	pstMonster->AddEventListener(this, CQStageEntity::DIE_EFF_SOUND_EVENT, eventListner_selector(CBossWarFightScene::handleEnemyDieEffectEvent));
	pstMonster->AddEventListener(this, CDummyBoss::BOSS_DIE_UP_DATA, eventListner_selector(CBossWarFightScene::handleEnemyUpDataEvent));

	pstMonster->AddEventListener(this, ENEMY_HURT_EVENT, eventListner_selector(CBossWarFightScene::handleEnemyHurtEvent));
	pstMonster->AddEventListener(this, HERO_HARMVALUE_CHANGE, eventListner_selector(CBossWarFightScene::handleEnemyHarmValueEvent));
	pstMonster->AddEventListener(this, HERO_EPVALUE_ADD_EVENT, eventListner_selector(CBossWarFightScene::handleHeroEPAddEvent));

	_stageEntities->addObject((CCObject*)pstMonster);

	return pstMonster;
}

void CBossWarFightScene::initFightData(){
	CBaseFightScene::initFightData();

	m_uUpHarmValue = 0;
	m_bIsUpSkillID = false;
}

void CBossWarFightScene::startUpdate(){
	CBaseFightScene::startUpdate();

	m_pBoss->SetIsControlledByAI(true);
}

void CBossWarFightScene::setViewpointCenter(const CCPoint& position){
	CBaseMapScene::setViewpointCenter(position);
}

void CBossWarFightScene::update(float delta){
	if(m_bIsPreOver) {
		return; //活动结束了就不走了
	}

	CBaseFightScene::update(delta);

	CBaseMapScene::setViewpointCenter(_mainHero->getPosition());

	static float totalTime = 0;
	totalTime += delta;
	if(totalTime >= 0.5) {
		//上报一次杀怪信息
		doUploadHitMonster();
		totalTime = 0;
	}
}

void CBossWarFightScene::brushMonsterLogic(float dt){

}

void CBossWarFightScene::handlePassLogic(){

}

void CBossWarFightScene::handleGiveUpLogic(CCObject* pstObj){

}

void CBossWarFightScene::handleGameOver(){

}

void CBossWarFightScene::doUploadHitMonster(){
	if(m_uUpHarmValue == 0){  //伤害为0不上报
		return;
	}

	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
	//上报数据
	unsigned int uSKillID = 0;
	if(m_bIsUpSkillID){
		uSKillID = WEAPON_CFG_MASTER->GetWeaponCfgByID(pUserInfo->weapon.cfgid())->weaponsuperskill();
	}

	CSSingleAttendeeDamageEvent stEvent;
	stEvent.set_damage(m_uUpHarmValue);
	stEvent.set_skillid(uSKillID);
	stEvent.set_uid(pUserInfo->uin);
	stEvent.set_nick(pUserInfo->nick);
	CSBossChallengeBigDamageReport stDamageReport;
	*(stDamageReport.mutable_mydamage()) = stEvent;

	P_BOSS_WAR_CTRL->sendDamageReportRqst(stDamageReport);

	CCLOG("[trace]::you hurt boss with damage === %u", m_uUpHarmValue);

	//清空
	m_bIsUpSkillID = false;
	m_uUpHarmValue = 0;
}

void CBossWarFightScene::showWaitLayer(){
	if(NULL == m_pWaitLayer) {
		m_pWaitLayer = CBossWarWaitLayer::create();
		m_pWaitLayer->retain();
		m_pWaitLayer->AddEventListener(this, CBossWarWaitLayer::WAIT_TIME_DONE_EVENT, eventListner_selector(CBossWarFightScene::onWaitDone));
	}
	if(NULL == m_pWaitLayer->getParent()) {
		m_pWaitLayer->startCountdown();
		m_pWaitLayer->setZOrder(10);
		UILayer *pTopLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_TOP));
		pTopLayer->addWidget(m_pWaitLayer);
	}
}

void CBossWarFightScene::onWaitDone(const string& stEvent, CCObject* pstEventObj){
	startUpdate();
}

void CBossWarFightScene::showReviveLayer(){
	if(NULL == m_pReviveLayer) {
		m_pReviveLayer = CHeroReviveLayer::create();
		m_pReviveLayer->retain();
		m_pReviveLayer->AddEventListener(this, CHeroReviveLayer::HERO_REVIVE_EVENT, eventListner_selector(CBossWarFightScene::onReviveDone));
	}
	if(NULL == m_pReviveLayer->getParent()) {
		m_pReviveLayer->startCountdown();
		m_pReviveLayer->setZOrder(10);
		m_pUiLayer->addWidget(m_pReviveLayer);
	}

	m_pBoss->ForceIdle();
	//m_pBoss->SetIsControlledByAI(false);
}

void CBossWarFightScene::onReviveDone(const std::string& stEvent, CCObject* pstEventObj){
	m_pBoss->SetIsControlledByAI(true);
	_mainHero->setHP(_mainHero->getMaxHP());
	_mainHero->setEP(0);
	_mainHero->Idle();
	_mainHero->setScaleX(1.0f); //面向右
	_mainHero->setShadowVisiable(true);
	_mainHero->setDesiredPosition(m_stMainHeroInitPos);
	m_pHeroHeadUI->setHPPercent(_mainHero->getHP(), _mainHero->getMaxHP());
	m_pHeroHeadUI->setEPPercent(_mainHero->getEP(), _mainHero->getMaxEP());
}

void CBossWarFightScene::doWarOver(){
	if(m_bIsPreOver && m_bIsGameOver){
		unscheduleUpdate();
		return;
	}

	m_bIsPreOver = true;
	m_bIsGameOver = true;

	_mainHero->ForceIdle();
	clearAllBullet();
	clearAllSkills();
	stopAllActions();
	stopAllMonsters();
	unSetMainHeroAsTheOne();

	const CSBossChallengeStateNotification& stInfo = P_BOSS_WAR_CTRL->getStateNotify();
	if(stInfo.has_additionalmsg1()){  //有人击杀boss
		m_pBoss->setHP(0);
		m_pBoss->Die();
	}else{
		m_pBoss->ForceIdle();
	}

	unscheduleUpdate();
}

/************************************************************************/
/*                         协议处理                                     */
/************************************************************************/

void CBossWarFightScene::onBuyReviveCallBack(){
	if(m_pReviveLayer == NULL || m_pReviveLayer->getParent() == NULL){
		return;
	}

	m_pReviveLayer->continueGame();
}

void CBossWarFightScene::doDamageNotify(const CSBossChallengeDamageNotification& stRsp){
	if(m_pBoss == NULL){
		return;
	}

	//更新排行榜 & 内容推送  todo
	m_pWarInfoLayout->updateInfo(stRsp);

	//更新boss血量
	m_pBoss->SyncHP(stRsp.bosshp());
	m_pBossHPUI->setHPPercent(m_pBoss->getHP(), m_pBoss->getMaxHP());

	if(m_uPreBossHp == 0){  //第一次收到伤害同步
		m_uPreBossHp = m_uInitBossHp;
	}

	if(m_uPreBossHp <= stRsp.bosshp()){
		return;
	}

	UINT64_t uDamge = m_uPreBossHp - stRsp.bosshp();
	showDamageEff(uDamge);
	m_uPreBossHp = stRsp.bosshp();
}

void CBossWarFightScene::showDamageEff(UINT64_t uDamge){
	float xPos = 100;
	float yPos = m_pBossHPUI->getSize().height/2 + 10;
	ImageNumber* pDamgeNum = ImageNumber::create(14);
	pDamgeNum->setNum(intToString(uDamge), -2, "minus");
	pDamgeNum->setPosition(ccp(xPos, yPos));
	m_pBossHPUI->addNode(pDamgeNum);
	pDamgeNum->setVisible(false);

	CCActionInterval* actionTo = CCMoveTo::create(0.3f, ccp(xPos, yPos + 30));
	CCActionInterval* scale_act = CCScaleTo::create(0.05f,1.5f, 1.5f);
	CCActionInterval* scale_back = CCScaleTo::create(0.05f, 1.0f, 1.0f);
	CCActionInterval* fade_actby = CCFadeIn::create(0.05f);
	CCActionInterval* fade_actby2 = CCFadeOut::create(0.05f);
	CCCallFuncO *actbackfun = CCCallFuncO::create(this, callfuncO_selector(CBossWarFightScene::showDamageEffCallBack), pDamgeNum);
	pDamgeNum->runAction(CCSequence::create( CCShow::create(),
		fade_actby, scale_act, 
		actionTo, scale_back, fade_actby2, actbackfun, NULL));
}

void CBossWarFightScene::showDamageEffCallBack(CCObject* pstObj){
	CCNode* pImg = dynamic_cast<CCNode*>(pstObj);
	if(pImg){
		pImg->removeFromParentAndCleanup(true);
	}
}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CBossWarFightScene::handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	if(!m_bIsShowWait){
		startUpdate();
	}
}

void CBossWarFightScene::handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj){
	//主角挂了
	showReviveLayer();
}

void CBossWarFightScene::handleEnemyHurtEvent(const string& stEvent, CCObject* pstEventObj){
	m_pBossHPUI->setVisible(true);
	m_pBossHPUI->setHPPercent(m_pBoss->getHP(), m_pBoss->getMaxHP());
}

void CBossWarFightScene::handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj){
	//计入总伤害 
	CCString* strValue = (CCString*)pstEventObj;
	int harmValue = strValue->intValue();
	m_uUpHarmValue += harmValue;
}

void CBossWarFightScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){

}

void CBossWarFightScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){

}

void CBossWarFightScene::handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj){

}

void CBossWarFightScene::handleEnemyUpDataEvent(const string& stEvent, CCObject* pstEventObj){
	doUploadHitMonster();
}

void CBossWarFightScene::handleCloseAtkKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleCloseAtkKeyFrameEvent(stEvent, pstEventObj);
}

void CBossWarFightScene::handleSkillKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleSkillKeyFrameEvent(stEvent, pstEventObj);
}

void CBossWarFightScene::handleFireABulletEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleFireABulletEvent(stEvent, pstEventObj);
}

void CBossWarFightScene::handleDieEffKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleDieEffKeyFrameEvent(stEvent, pstEventObj);
}

void CBossWarFightScene::handleEnemySkillActionEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleEnemySkillActionEvent(stEvent, pstEventObj);
}

void CBossWarFightScene::handleEnemyDashCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleEnemyDashCompleteEvent(stEvent, pstEventObj);
}

void CBossWarFightScene::handleEnemyDieEffectEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleEnemyDieEffectEvent(stEvent, pstEventObj);
}

void CBossWarFightScene::handleHeroEPAddEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroEPAddEvent(stEvent, pstEventObj);
}

void CBossWarFightScene::handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj) {
	CBaseFightScene::handleSkillClickEvent(stEvent, pstEventObj);

	CCString* strValue = (CCString*)pstEventObj;
	int skillIdx = strValue->intValue();
	if(skillIdx == 0){  //大招
		doUploadHitMonster();  //放技能前将伤害伤害上报
		m_bIsUpSkillID = true;
	}
}

void CBossWarFightScene::btnTouchEvent(CCObject *pSender, TouchEventType type){
	if(_mainHero->IsDizzy()){  //眩晕时不能放技能
		return;
	}

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroShoot();
		}
		break;
	case TOUCH_EVENT_ENDED:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroCeaseShoot();
		}else if(pSender->m_uID == pauseBtn->m_uID){
			P_BOSS_WAR_CTRL->getQuitPanel()->open();
		}
		break;
	case TOUCH_EVENT_CANCELED:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroCeaseShoot();
		}
		break;

	default:
		break;
	}
}


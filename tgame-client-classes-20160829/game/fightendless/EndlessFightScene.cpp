#include "EndlessFightScene.h"
#include "vlib/CVRand.h"
#include "FightEndlessControl.h"
#include "ToolUtils.h"
#include "Multiresolution.h"
#include "fight/FightObj.h"
#include "instance/InstanceUtils.h"

#include "GlobalCfgMaster.h"
#include "EndlessCfgMaster.h"
#include "MonsterLevCfgMaster.h"

CEndlessFightScene::CEndlessFightScene(){
	m_stNextMonsters = NULL;
	m_stNextMonsters = CCArray::create();
	m_stNextMonsters->retain();
}

CEndlessFightScene::~CEndlessFightScene(){
	CC_SAFE_RELEASE_NULL(m_stNextMonsters);

	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool CEndlessFightScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CEndlessFightScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	startFightCountDown();
}

void CEndlessFightScene::startFightCountDown(){
	m_pPauseLayer->startCountdown();
	addChild(m_pPauseLayer);
}

void CEndlessFightScene::setWalkHight(){
	//从关卡地图中随机
	const MapCfgSet& cfgSet = MAP_CFG_MASTER->GetCfg();
	unsigned int idx = CRandom::GetRand(cfgSet.mapcfg_size());

	m_stMapCfg = cfgSet.mapcfg(idx);
	m_uWalkHight = m_stMapCfg.midwalkh();
}

void CEndlessFightScene::setMapRes(){
	m_stFrontStr = m_stMapCfg.frontid();
	m_stForeStr = m_stMapCfg.foreid();
	m_stMidStr = m_stMapCfg.midid(); 
	m_stBackStr = m_stMapCfg.backid(); 
}

void CEndlessFightScene::loadMapBgRes(){
	m_uCurStageIdx = 1;
	m_uCurMonsterLev = 1;

	CBaseMapScene::loadMapBgRes();
}

void CEndlessFightScene::loadItemRes(){
	CBaseMapScene::loadItemRes();
}

void CEndlessFightScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();
}

void CEndlessFightScene::loadMonsterRes(){
	prepareMonsterResByStatge(m_uCurStageIdx);
}

void CEndlessFightScene::initMap(){
	CBaseFightScene::initMap();
	walkableRect = CCRect(0, 0, _screenW*2, m_uWalkHight);
}

void CEndlessFightScene::initFightData(){
	CBaseFightScene::initFightData();

	m_fLastTime = 0;
	m_uCurStageIdx = 1;
	m_uCurStageMonNum = 0;
	m_uCurMonsterLev = 1;
	m_uCurTotalNum = 0;
	m_uCurLimitTime = 0;

	m_uCurLeftNum = 0;
	m_uCurRightNum = 0;

	m_uPreInitInter = 0;
	m_uPreInitNum = 0;

	m_uPackageCnt = 0;
	m_uCoin = 0;
	m_uCurScore = 0;

	for(int i = 0; i < m_stMonsterCnt.GetCount(); ++i) {
		m_stMonsterCnt.AddOneItem(0);
	}
}

void CEndlessFightScene::initSceneItem(){
	CItemObj* itemObj;
	for(unsigned int i = 0; i< _itemData->count(); i++){
		itemObj = (CItemObj*)_itemData->objectAtIndex(i);
		addSceneItem(itemObj);
	}

	updateMapBlocks();
}

void CEndlessFightScene::initMapUI(){
	CBaseFightScene::initMapUI();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pPauseLayer = CPauseLayer::create();
	m_pPauseLayer->retain();
	m_pPauseLayer->AddEventListener(this, RESUME_GAME, eventListner_selector(CEndlessFightScene::startEndless));

	m_pScoreUI = CFightEndlessScoreUI::create();
	m_pScoreUI->setPosition(visibleOrigin + ccp(visibleSize.width - m_pScoreUI->getSize().width - 20, m_pPauseLayout->getPositionY() - m_pScoreUI->getSize().height - 20));
	m_pUiLayer->addWidget(m_pScoreUI);

	m_pWaveUI = CFightEndlessWaveUI::create();
	m_pWaveUI->setPosition(visibleOrigin + ccp((visibleSize.width - m_pWaveUI->getSize().width)/2, m_pBossHPUI->getPositionY() - m_pWaveUI->getSize().height + 50));
	m_pUiLayer->addWidget(m_pWaveUI);
	m_pWaveUI->setWave(m_uCurStageIdx);
}

void CEndlessFightScene::initMainHero(){
	CBaseFightScene::initMainHero();

	_mainHero->SetEnityPosition(ccp(walkableRect.size.width/2, m_uWalkHight/2));
	_mainHero->setDesiredPosition(_mainHero->getPosition());

	setViewpointCenter(_mainHero->getPosition());
}

void CEndlessFightScene::initMonster(){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();

	//当前阶段的信息
	int idx = (m_uCurStageIdx - 1)%endlessCfg.roundinfo_size();
	m_uCurTotalNum = endlessCfg.roundinfo(idx).num() + endlessCfg.roundinfo(idx).extranum();  //当前阶段刷的怪总数
	m_uCurLimitTime = endlessCfg.roundinfo(idx).time();
	m_uCurLeftNum = m_uCurTotalNum/2;
	m_uCurRightNum = m_uCurTotalNum - m_uCurLeftNum;
	m_uCurStageMonNum += endlessCfg.roundinfo(idx).num() + endlessCfg.roundinfo(idx).extranum();
	m_uCurMonsterLev += (m_uCurStageIdx - 1)/(endlessCfg.addlev());

	CCLOG("fightEndless now wave == %u, and the monster level == %u", m_uCurStageIdx, m_uCurMonsterLev);

	initMonsterLogic();

	prepareMonsterResByStatge(m_uCurStageIdx + 1);  //预加载下一阶段的怪物
}

void CEndlessFightScene::initMonsterLogic(bool isAddToStage/* = true*/){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
	int idx = (m_uCurStageIdx - 1)%endlessCfg.roundinfo_size();

	if(m_uCurLeftNum == 0 || m_uCurRightNum == 0){
		CCLOG("CEndlessFightScene:: error the monster num is zero");
		return; 
	}

	CCPoint stInitPos = ccp(400, 100);
	//将本阶段的怪全部刷在舞台上
	//先刷左边
	unsigned int yDis = (m_uWalkHight - stInitPos.y*2)/m_uCurLeftNum;
	unsigned int uNum = 0;
	string avatar = "";
	for(unsigned int i = 0; i < m_uCurLeftNum; i++){
		float y = stInitPos.y + yDis*i + CRandom::GetRandHaveMax(50);

		if(i > endlessCfg.roundinfo(idx).num()){  //普通怪刷完了
			if(endlessCfg.roundinfo(idx).extranum() <= 0){
				continue;
			}

			avatar = m_stCurAvatar.second;
		}else{
			avatar = m_stCurAvatar.first;
		}

		initOneMosterByAvatar(avatar, ccp(stInitPos.x, y), m_uCurMonsterLev, isAddToStage);
	}

	uNum += m_uCurLeftNum;

	//再刷右边
	yDis = (m_uWalkHight - stInitPos.y*2)/m_uCurRightNum;
	for(unsigned int i = 0; i < m_uCurRightNum; i++){
		float x = walkableRect.getMaxX() - stInitPos.x;
		float y = stInitPos.y + yDis*i + CRandom::GetRandHaveMax(50);

		if((uNum + i) > endlessCfg.roundinfo(idx).num()){  //普通怪刷完了
			if(endlessCfg.roundinfo(idx).extranum() <= 0){
				continue;
			}

			avatar = m_stCurAvatar.second;
		}else{
			avatar = m_stCurAvatar.first;
		}

		initOneMosterByAvatar(avatar, ccp(x, y), m_uCurMonsterLev, isAddToStage);
	}
}

void CEndlessFightScene::startEndless(const string& stEvent, CCObject* pstEventObj){
	m_fLastTime = 0;
	m_fStageStartTime = 0;
	this->scheduleUpdate();
}

void CEndlessFightScene::update(float delta){
	CBaseFightScene::update(delta);

	m_fLastTime += delta;

	setViewpointCenter(_mainHero->getPosition());

	activeMonsterByTime(delta);

	//在每波之间，间隔一段时间构建一定数量的怪物，但不加入舞台	
	prepareInitMonster(delta);
}

void CEndlessFightScene::activeMonsterByTime(float dt){
	static float fInter = 0;

	if(fInter > 0.5f){
		fInter = 0;
		activeMonster();
	}

	fInter += dt;
}

void CEndlessFightScene::prepareInitMonster(float dt){
	if(m_uPreInitInter == 0 || m_uPreInitNum == 0){
		return;
	}

	static float fPreInter = 0;

	if(fPreInter > (float)m_uPreInitInter){
		fPreInter = 0;
		prepareInitOneMonster(m_uCurStageIdx + 1);
	}

	fPreInter += dt;
}

void CEndlessFightScene::prepareInitOneMonster(unsigned int stageIdx){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
	int idx = (stageIdx - 1)%endlessCfg.roundinfo_size();

	CCPoint stInitPos = ccp(400, 100);
	unsigned int nowInitNum = (m_uCurLeftNum + m_uCurRightNum) - m_uPreInitNum;

	string avatar = "";
	unsigned int yDis;
	float x;
	float y;
	if(nowInitNum < m_uCurLeftNum){
		x = stInitPos.x;
		yDis = (m_uWalkHight - stInitPos.y*2)/m_uCurLeftNum;
		y = stInitPos.y + yDis*nowInitNum + CRandom::GetRandHaveMax(50);
	}else{
		x = walkableRect.getMaxX() - stInitPos.x;
		yDis = (m_uWalkHight - stInitPos.y*2)/m_uCurRightNum;
		y = stInitPos.y + yDis*(nowInitNum - m_uCurLeftNum) + CRandom::GetRandHaveMax(50);
	}

	if(nowInitNum > endlessCfg.roundinfo(idx).num()){  //普通怪刷完了
		avatar = m_stCurAvatar.second;
	}else{
		avatar = m_stCurAvatar.first;
	}

	initOneMosterByAvatar(avatar, ccp(x, y), m_uCurMonsterLev, false);

	m_uPreInitNum--;
}

void CEndlessFightScene::activeMonster(){
	unsigned int activeNum = 0;
	CCObject* pstObj;
	CQStageEntity* pstEntity;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE && !pstEntity->GetIsControlledByAI() && pstEntity->getParent() != NULL){
			pstEntity->SetIsControlledByAI(true);
			activeNum++;

			if(activeNum == 2){
				return;
			}
		}
	}
}

void CEndlessFightScene::updateBgViewCenter(float foreX){
	updateBgTextureByRect(_sceneForegound);

	float bgViewX = foreX * BG_RATIO;
	_sceneBackground->setPositionX(bgViewX);
	updateBgTextureByRect(_sceneBackground);

	float fgViewX = foreX * FG_RATIO;
	_sceneFrontgound->setPositionX(fgViewX);
	updateBgTextureByRect(_sceneFrontgound);

	if(_sceneMidground){
		float midViewX = foreX * MID_RATIO;
		_sceneMidground->setPositionX(midViewX);
		updateBgTextureByRect(_sceneMidground);
	}
}

void CEndlessFightScene::prepareMonsterResByStatge(unsigned int stageIdx){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
	int idx = (stageIdx - 1)%endlessCfg.roundinfo_size();
	MonsterShowCfg* monShowCfg;
	MonsterShowCfg* extraShowCfg;
	string monStr = "";
	string extraStr = "";

	if(endlessCfg.roundinfo(idx).num() > 0){
		monShowCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgRand(ROLE_TYPE_MONSTER);
		monStr = monShowCfg->avatar();

		CCLOG("the normal monster type is %d, avatar is %s", monShowCfg->monstertype(), monStr.c_str());
	}

	if(endlessCfg.roundinfo(idx).extranum() > 0){
		extraShowCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgRand(endlessCfg.roundinfo(idx).extratype());
		extraStr = extraShowCfg->avatar();

		CCLOG("the extra monster type is %d, avatar is %s", extraShowCfg->monstertype(), extraStr.c_str());
	}

	m_stCurAvatar = make_pair(monStr, extraStr);
	m_uPreInitNum = endlessCfg.roundinfo(idx).num() + endlessCfg.roundinfo(idx).extranum();
	m_uPreInitInter = endlessCfg.roundinfo(idx).time()/(m_uPreInitNum + 1);

	CCLOG("the stage idx is ==== %d, the normalStr is %s, the extralStr is %s", stageIdx, monStr.c_str(), extraStr.c_str());

	bool isPreLoad = false;
	if(stageIdx == 1){
		isPreLoad = true;
	}

	MonsterCfg monCfg;
	genMonsterCfg(monStr, monCfg);
	unsigned int num = endlessCfg.roundinfo(idx).num();
	loadMonsterResByCfg(&monCfg, num, isPreLoad);

	if(strcmp(extraStr.c_str(), "") != 0){
		MonsterCfg extraCfg;
		genMonsterCfg(extraStr, extraCfg);

		num = endlessCfg.roundinfo(idx).extranum();
		loadMonsterResByCfg(&extraCfg, num, isPreLoad);
	}
}

void CEndlessFightScene::initOneMosterByAvatar(string avatar, CCPoint stPos, int level /* = 1 */, bool isAddToStage/* = true*/){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
	int idx = (m_uCurStageIdx - 1)%endlessCfg.roundinfo_size();
	MonsterShowCfg* showCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgByShowID(avatar);
	CEnemyData* pstMonsterObj = CEnemyData::create();
	pstMonsterObj->monsterType = showCfg->monstertype();
	pstMonsterObj->x = stPos.x;
	pstMonsterObj->y = _sceneForegound->getContentSize().height - stPos.y;

	if(showCfg->monstertype() == ROLE_TYPE_SENIOR_BOSS){  ////大boss随机掉落
		pstMonsterObj->dropCnt = rollPackage(endlessCfg.roundinfo(idx).extraprob());
	}

	MonsterCfg cfg;
	genMonsterCfg(avatar, cfg, level);

	CEnemy* pstMonster;
	pstMonster = initOneMonster(pstMonsterObj, &cfg);
	pstMonster->setIsBubbleOn(false);
	pstMonster->SetIsControlledByAI(false);   //暂时停掉AI
	pstMonster->LoadActionRes(isAddToStage);

	if(isAddToStage){
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
		_sceneForegound->addChild(pstMonster, iNewZorder);
	}else{
		m_stNextMonsters->addObject(pstMonster);
	}
}

void CEndlessFightScene::updateScore(){
	//当前积分更新
	m_uCurScore = (unsigned int)m_fLastTime*5;
	for(int i = 0; i < m_stMonsterCnt.Size(); i++){
		m_uCurScore += m_stMonsterCnt[i]*m_stMonsterMark[i];
	}

	m_uCurScore = m_uCurScore*(1 + 1.0*P_ENDLESS_CTRL->getScoreAdd()/10000);

	m_pScoreUI->setScore(m_uCurScore);
}

void CEndlessFightScene::handlePassLogic(){
	if(m_uCurStageMonNum <= 0){  //杀死所有怪
		CCLOG("the monster on the stage is killer all");
		nextStageTip();
		nextStage();
		return;
	}

	//超过一个阶段的限制时间
	if((unsigned int)(m_fLastTime) >= m_fStageStartTime + m_uCurLimitTime){  
		CCLOG("the time is out, so next stage");
		nextStageTip();
		nextStage();
		return;
	}
}

void CEndlessFightScene::nextStageTip(){
	m_pWaveUI->startTip();
}

void CEndlessFightScene::nextStage(){
	m_uCurStageIdx++;
	m_fStageStartTime = m_fLastTime;

	m_pWaveUI->setWave(m_uCurStageIdx);

	unsigned int nowCoin = (m_uCurStageIdx - 1)*(GLOBAL_CFG_MASTER->GetCfg().wavecoin());
	nowCoin = nowCoin*(1 + 1.0*P_ENDLESS_CTRL->getCoinAdd()/10000);
	m_pScoreUI->setCoin(nowCoin);

	//initMonster();	

	CCObject* pstObj;
	CCARRAY_FOREACH(m_stNextMonsters, pstObj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE && pstEntity->getParent() == NULL){  //敌方
			int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstEntity->getPositionY());
			pstEntity->Emerge();
			_sceneForegound->addChild(pstEntity, iNewZorder);
		}
	}

	m_stNextMonsters->removeAllObjects();
	m_uPreInitNum = 0;
	m_uPreInitInter = 0;

	prepareMonsterResByStatge(m_uCurStageIdx + 1);
}


void CEndlessFightScene::handleGiveUpLogic(CCObject* pstObj){
	P_FIGHT_OBJ->setIsForceQuit(true);

	handleGameOver();

	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, GIVEUP_FIGHT_EVENT);
}

void CEndlessFightScene::handleGameOver(){
	this->unscheduleUpdate();

	//评分
	m_uMark = (unsigned int)m_fLastTime*5;
	for(int i = 0; i < m_stMonsterCnt.Size(); i++){
		m_uMark += m_stMonsterCnt[i]*m_stMonsterMark[i];
	}

	m_uCoin = (m_uCurStageIdx - 1)*(GLOBAL_CFG_MASTER->GetCfg().wavecoin());

	m_uMark = m_uMark*(1 + 1.0*P_ENDLESS_CTRL->getScoreAdd()/10000);
	m_uCoin = m_uCoin*(1 + 1.0*P_ENDLESS_CTRL->getCoinAdd()/10000);

	P_ENDLESS_CTRL->setScore(m_uMark);
	P_ENDLESS_CTRL->setCoin(m_uCoin);

	P_ENDLESS_CTRL->sendEndlessEndRqst(m_uMark, m_uPackageCnt, m_uCurStageIdx, m_uCoin, m_stMonsterCnt, m_uEvolutionCnt);
}

void CEndlessFightScene::genMonsterCfg(string avatar, MonsterCfg& cfg, int level/* = 1*/){
	MonsterLevCfg* levCfg = MONSTER_LEV_CFG->GetMonsterLevCfgByLev(level);
	MonsterShowCfg* showCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgByShowID(avatar);

	cfg.set_id(0);
	cfg.set_monstername(showCfg->monstername());
	cfg.set_desc(showCfg->desc());
	cfg.set_avatar(avatar);
	cfg.set_camp(ROLE_PARTY_HOSTILE);
	cfg.set_monstertype(showCfg->monstertype());
	cfg.set_level(level);
	cfg.set_aiid(showCfg->aiid());
	cfg.set_patrolarea(showCfg->patrolarea());
	cfg.set_floorbuffid(showCfg->floorbuffid());
	cfg.set_hurtcolor(showCfg->hurtcolor());

	unsigned int attIdx = showCfg->monstertype() - 1; 
	cfg.set_hp(levCfg->monsteratt(attIdx).hp());
	cfg.set_attack(levCfg->monsteratt(attIdx).atk());
	cfg.set_defence(levCfg->monsteratt(attIdx).def());
	cfg.set_atkatt(levCfg->monsteratt(attIdx).atkatt());

	cfg.set_atktype(showCfg->atktype());
	cfg.set_deftype(showCfg->deftype());
	cfg.set_ballistic(showCfg->ballistic());

	for(int i =0; i < showCfg->skills_size(); i++){
		cfg.add_skills(showCfg->skills(i));
	}

	cfg.set_dropid(0);
	cfg.set_minwalkspeed(showCfg->minwalkspeed());
	cfg.set_maxwalkspeed(showCfg->maxwalkspeed());
	cfg.set_highspeed(showCfg->highspeed());
	cfg.set_emergevoice(showCfg->emergevoice());
	cfg.set_hurtvoice(showCfg->hurtvoice());
	cfg.set_dievoice(showCfg->dievoice());
	cfg.set_dieanimation(showCfg->dieanimation());
	cfg.set_ailuafile("");
	cfg.set_atkatttype(showCfg->atkatttype());

	for(int i =0; i < levCfg->defatts_size(); i++){
		DefAttElement* pstDef = cfg.add_defatts();
		pstDef->set_defatt(levCfg->defatts(i).defatt());
		pstDef->set_defatttype(levCfg->defatts(i).defatttype());
	}

	cfg.set_walktype(showCfg->walktype());
	cfg.set_hurtactioncd(showCfg->hurtactioncd());
}

int CEndlessFightScene::rollPackage(unsigned int prob){
	unsigned int roll = CRandom::GetRandHaveMax(10000);

	if(roll <= prob){
		return 1;
	}

	return 0;
}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CEndlessFightScene::handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj){
	//猪脚死亡
	P_FIGHT_OBJ->setIsForceQuit(false);

	handleGameOver();
}

void CEndlessFightScene::handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj){
	//计入总伤害 
	CCString* strValue = (CCString*)pstEventObj;
	int harmValue = strValue->intValue();
	m_uTotalHarmValue += harmValue;
}

void CEndlessFightScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	if(monster->getMonsterObj()->dropCnt != 0){
		//掉落统计
		if(monster->getRoleType() == ROLE_TYPE_SENIOR_BOSS && monster->getMonsterObj()->dropCnt != 0){  //boss
			m_uPackageCnt++;
			m_pScoreUI->setPackage(m_uPackageCnt);
		}
	}

	handleHeroKillValueChange(stEvent, pstEventObj);
}

void CEndlessFightScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	int type = pstEntity->getMonsterObj()->monsterType;

	--m_uCurStageMonNum;
	CCLOG("the wave last monster num == %d", m_uCurStageMonNum);

	//杀怪数据改变
	if(pstEntity->getRoleType() <= 4){
		int preCnt = m_stMonsterCnt[pstEntity->getRoleType() - 1];
		m_stMonsterCnt[pstEntity->getRoleType() - 1] = ++preCnt;
	}
	
	updateScore();

	//boss死亡，血条消失
	if(pstEntity->getRoleType() == ROLE_TYPE_SENIOR_BOSS || pstEntity->getRoleType() == ROLE_TYPE_JUNIOR_BOSS){
		if(m_pBossHPUI->isVisible()){
			m_pBossHPUI->setVisible(false);
		}
			

	}else{
			
	}

	//把已经播放完毕死亡动画的死人做掉
	if(pstEntity->getMyState() == kActionStateDie){  
		_sceneForegound->removeChild(pstEntity, true);
		if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
			_stageEntities->removeObject(pstEntity);
		}
	}
}


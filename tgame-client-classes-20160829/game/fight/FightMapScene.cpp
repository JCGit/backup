#include "FightMapScene.h"
#include "UserObj.h"
#include "role/Enemy.h"
#include "FightObj.h"
#include "SkillEffLayer.h"
#include "FightControl.h"
#include "ToolUtils.h"
#include "LevelControl.h"
#include "LevelObj.h"
#include "GlobalTime.h"
#include "BossWarning.h"
#include "StageTipLayout.h"
#include "FloorBuffCfgMaster.h"
#include "PunctureTrap.h"
#include "QEnvSet.h"
#include "ConstHelper.h"
#include "SceneManager.h"
#include "control/AudioManager.h"
#include "nihility/NextStageTipLayer.h"
#include "nihility/TransitionLayer.h"
#include "instance/AnimationUtils.h"
#include "instance/InstanceUtils.h"
#include "instance/InstanceFightUtils.h"
#include "dailyinstance/DailyInstanceControl.h"

#include "EliteWarMapCfgMaster.h"
#include "HeroCfgMaster.h"
#include "MapCfgMaster.h"
#include "WarMapCfgMaster.h"
#include "MonsterCfgMaster.h"
#include "SkillCfgMaster.h"
#include "BulletCfgMaster.h"
#include "ScoreCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "FightKillNumCfgMaster.h"


void CFightMapScene::release(void) {
    int aaa = this->retainCount();
    CCObject::release();
    int bbb = this->retainCount();
}
void CFightMapScene::retain(void) {
    int aaa = this->retainCount();
    CCObject::retain();
    int bbb = this->retainCount();
}
CCObject* CFightMapScene::autorelease(void) {
    int aaa = this->retainCount();
    return CCObject::autorelease();
}


CFightMapScene::CFightMapScene()
:m_pnextStageTip(NULL)
,m_pTipLayer(NULL)
,m_fStageStartTime(0)
,m_fStartWalkTime(0)
,m_bIsArrivedEdge(false)
{
	
}

CFightMapScene::~CFightMapScene(){
    CCLOG("[trace]=====destroy scene:%s=====", typeid(*this).name());
    P_LEVEL_CTL->releasePanel();
    CC_SAFE_RELEASE_NULL(m_pnextStageTip);
}

bool CFightMapScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CFightMapScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	startFight();
}

void CFightMapScene::onExit() {
    CBaseFightScene::onExit();
}

void CFightMapScene::loadMapBgRes(){
	CBaseMapScene::loadMapBgRes();
}

void CFightMapScene::loadItemRes(){
	CBaseFightScene::loadItemRes();

	//将item按阶段存放
	m_stIDXToItems.clear();

	map<int, vector<CItemObj*> >::iterator stItemID;
	for(unsigned int i = 0; i < _itemData->count(); i++){
		CItemObj* itemObj = (CItemObj*)_itemData->objectAtIndex(i);
		stItemID = m_stIDXToItems.find(itemObj->stage);

		if(stItemID == m_stIDXToItems.end()){
			m_stIDXToItems[itemObj->stage].clear();
			m_stIDXToItems[itemObj->stage].push_back(itemObj);
		}else{
			m_stIDXToItems[itemObj->stage].push_back(itemObj);
		}
	}
}

void CFightMapScene::loadFloorBuffRes(){
	CBaseFightScene::loadFloorBuffRes();

	//将floorbuff按阶段存放
	m_stIDXToFloorBuffs.clear();

	map<int, vector<CFloorBuffObj*> >::iterator stBuffID;
	for(unsigned int i = 0; i < _floorBuffData->count(); i++){
		CFloorBuffObj* buffObj = (CFloorBuffObj*)_floorBuffData->objectAtIndex(i);
		stBuffID = m_stIDXToFloorBuffs.find(buffObj->stage);

		if(stBuffID == m_stIDXToFloorBuffs.end()){
			m_stIDXToFloorBuffs[buffObj->stage].clear();
			m_stIDXToFloorBuffs[buffObj->stage].push_back(buffObj);
		}else{
			m_stIDXToFloorBuffs[buffObj->stage].push_back(buffObj);
		}
	}
}

void CFightMapScene::loadTrapRes(){
	CBaseFightScene::loadTrapRes();

	//将陷阱按阶段存放
	m_stIDXToTraps.clear();

	map<int, vector<CTrapObj*> >::iterator stTrapIter;
	for(unsigned int i = 0; i < _trapData->count(); i++){
		CTrapObj* trapObj = (CTrapObj*)_trapData->objectAtIndex(i);
		stTrapIter = m_stIDXToTraps.find(trapObj->stage);

		if(stTrapIter == m_stIDXToTraps.end()){
			m_stIDXToTraps[trapObj->stage].clear();
			m_stIDXToTraps[trapObj->stage].push_back(trapObj);
		}else{
			m_stIDXToTraps[trapObj->stage].push_back(trapObj);
		}
	}
}

void CFightMapScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();

	//邀请好友助战
	if(P_LEVEL_OBJ->getIsHasFriend()){
		loadHeroResByShowID(P_LEVEL_OBJ->getFriendDetailInfo().showid(), P_LEVEL_OBJ->getFriendDetailInfo().weapon().cfgid());

		//好友枪械
		loadWeaponeResByCfgID(P_LEVEL_OBJ->getFriendDetailInfo().weapon().cfgid());

		//好友进化技能
		loadSkillRes(P_LEVEL_OBJ->getFriendDetailInfo().evolutionskillid());
	}
}

void CFightMapScene::loadMonsterRes(){
	//将怪物按阶段存放
	m_stIDXToMonsters.clear();
	m_stIDXToMonsterNum.clear();

	map<int, vector<CEnemyData*> >::iterator stMonsterID;
	map<int, int>::iterator stMonsterNum;
	for(unsigned int i = 0; i < _monsterData->count(); i++){
		CEnemyData* monsterObj = (CEnemyData*)_monsterData->objectAtIndex(i);

		stMonsterID = m_stIDXToMonsters.find(monsterObj->stage);

		if(stMonsterID == m_stIDXToMonsters.end()){
			m_stIDXToMonsters[monsterObj->stage].clear();
			m_stIDXToMonsters[monsterObj->stage].push_back(monsterObj);
		}else{
			m_stIDXToMonsters[monsterObj->stage].push_back(monsterObj);
		}

		if(monsterObj->type == APPEAR_AFTER_DIE){  //不计入刷怪数量
			continue;
		}
		stMonsterNum = m_stIDXToMonsterNum.find(monsterObj->stage);
		if(stMonsterNum == m_stIDXToMonsterNum.end()){
			m_stIDXToMonsterNum[monsterObj->stage] = monsterObj->num;
		}else{
			int preNum = stMonsterNum->second;
			m_stIDXToMonsterNum[monsterObj->stage] = preNum + monsterObj->num;
		}
	}

	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj) {
		CEnemyData* monsterObj = (CEnemyData*)pstObj;
		MonsterCfg* monsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(monsterObj->id);
		loadMonsterResByCfg(monsterCfg, monsterObj->num);
	}

	m_stRes.insert("monster/jiuhuche/jiuhuche.ExportJson");	//救护车资源
}

void CFightMapScene::initMap(){
	CBaseFightScene::initMap();
	walkableRect = CCRect(0, 0, _screenW*FIGHT_STAGE_NUM, m_uWalkHight);
}

void CFightMapScene::initSceneItem(){
	if(m_stIDXToItems.size() <= 0){
		updateMapBlocks();
		return;
	}

	//当前阶段的item
	map<int, vector<CItemObj*> >::iterator stItemID;
	stItemID = m_stIDXToItems.find(m_uCurStageIdx);
	if(stItemID == m_stIDXToItems.end()){
		updateMapBlocks();
		return;
	}

	vector<CItemObj*> tempArr = stItemID->second;
	CItemObj* itemObj;
	vector<CItemObj*>::iterator stIter;
	for(stIter = tempArr.begin(); stIter != tempArr.end();){
		itemObj = *stIter;

		if(itemObj->touchId != 0){  //触发id不为0，表示按触发线触发
			stIter++;
			continue;
		}else{
			//添加物件
			addSceneItem(itemObj);
			stIter = tempArr.erase(stIter);
		}
	}

	m_stIDXToItems.find(m_uCurStageIdx)->second = tempArr;

	updateMapBlocks();
}

void CFightMapScene::initFloorBuff(){
	//当前阶段的floorbuff
	map<int, vector<CFloorBuffObj*> >::iterator stBuffID;
	stBuffID = m_stIDXToFloorBuffs.find(m_uCurStageIdx);
	if(stBuffID == m_stIDXToFloorBuffs.end()){
		return;
	}

	vector<CFloorBuffObj*> tempArr = stBuffID->second;
	CFloorBuffObj* buffObj;
	vector<CFloorBuffObj*>::iterator stIter;
	for(stIter = tempArr.begin(); stIter != tempArr.end();){
		buffObj = *stIter;

		if(buffObj->touchId != 0){  //触发id不为0，表示按触发线触发
			stIter++;
			continue;
		}else{
			//添加floorbuff
			if(FLOOR_BUFF_CFG->GetFloorBuffCfgByID(buffObj->id)->bufftype() == BUFF_TYPE_WEAPON){
				addWeaponBuff(buffObj->id, buffObj->party, ccp(buffObj->x, _sceneForegound->getContentSize().height - buffObj->y), true);
			}else{
				addFloorBuff(buffObj->id, buffObj->party, ccp(buffObj->x, _sceneForegound->getContentSize().height - buffObj->y), true);
			}
			
			stIter = tempArr.erase(stIter);
		}
	}

	m_stIDXToFloorBuffs.find(m_uCurStageIdx)->second = tempArr;
}

void CFightMapScene::initTrap(){
	//当前阶段的陷阱
	map<int, vector<CTrapObj*> >::iterator stTrapIter;
	stTrapIter = m_stIDXToTraps.find(m_uCurStageIdx);
	if(stTrapIter == m_stIDXToTraps.end()){
		return;
	}

	vector<CTrapObj*> tempArr = stTrapIter->second;
	CTrapObj* trapObj;
	vector<CTrapObj*>::iterator stIter;
	for(stIter = tempArr.begin(); stIter != tempArr.end();){
		trapObj = *stIter;

		//触发id不为0，且不是全屏穿刺型陷阱，表示按触发线触发
		if(trapObj->touchId != 0 && TRAP_CFG_MASTER->GetTrapCfgByID(trapObj->id)->type() != TRAP_TYPE_PUNCTURE){  
			stIter++;
			continue;
		}else{
			//添加陷阱
			if(TRAP_CFG_MASTER->GetTrapCfgByID(trapObj->id)->type() == TRAP_TYPE_PUNCTURE){
				addPunctureTrap(trapObj, true);
			}else{
				addSceneTrap(trapObj, true);
			}

			stIter = tempArr.erase(stIter);
		}
	}

	m_stIDXToTraps.find(m_uCurStageIdx)->second = tempArr;
}

void CFightMapScene::initMapUI(){
	CBaseFightScene::initMapUI();

	m_pnextStageTip = CFightTipLayout::create();
	m_pnextStageTip->retain();
}

void CFightMapScene::initFriend(){
	if(P_LEVEL_OBJ->getIsHasFriend()){
		CCPoint stPlayerPt(300, 100);

		vmsg::CSPlayerDetailInfo& stFriendInfo = P_LEVEL_OBJ->getFriendDetailInfo();
		const string* pHeroStr = HERO_CFG_MASTER->GetResName(stFriendInfo.showid());
		if(NULL == pHeroStr) {
			CCLOG("[error]CBaseFightScene::initFriend, get hero res name fail! cfg id %d", stFriendInfo.showid());
			return;
		}

		const string& heroStr = *pHeroStr;

		CMainHero* pFriend = CMainHero::create();
		string stBodyName;
		int iRet = CMainHero::GetResourceNames(stFriendInfo.weapon().cfgid(), stFriendInfo.showid(), stBodyName);
		if (iRet){
			CCLog("no resource name is found for hero %s", pHeroStr->c_str());
			return;
		}

		pFriend->setShowID(stFriendInfo.showid());
		pFriend->setResPath(stBodyName);
		pFriend->SetIsControlledByAI(true);
		pFriend->setAIType(AI_TYPE_PROTECT_ME);   //设置AI类型
		pFriend->SetEnityPosition(stPlayerPt);
		pFriend->setDesiredPosition(pFriend->getPosition());
		pFriend->setHurtActionCD(HERO_CFG_MASTER->GetHeroCfgByID(stFriendInfo.showid())->stifftime());

		CQStageEntityObj* heroInfo = CQStageEntityObj::create();
		heroInfo->setInfoByDetaiInfo(stFriendInfo);
		pFriend->setObj(heroInfo);

		pFriend->setCenterToBottom(90);
		pFriend->setCenterToSides(60);

		pFriend->AddEventListener(this, CQStageEntity::SKILL_CAST_FRAME, eventListner_selector(CFightMapScene::handleSkillKeyFrameEvent));
		pFriend->AddEventListener(this, CQStageEntity::FIRE_EVENT, eventListner_selector(CFightMapScene::handleFireABulletEvent));
		pFriend->AddEventListener(this, HERO_KILLED_EVENT, eventListner_selector(CFightMapScene::handleFriendDieEvent));
		pFriend->AddEventListener(this, FRIEND_HURT_EVENT, eventListner_selector(CFightMapScene::handleFriendHurtValueEvent));

		//枪械上的技能
		CQSkillCell* pstSkill = CQSkillCell::create();	
		pstSkill->m_uSkillID = WEAPON_CFG_MASTER->GetWeaponCfgByID(heroInfo->weapon.cfgid())->weaponsuperskill();	
		pstSkill->m_uLastCastTime = 0;	
		pFriend->getSkillArray()->addObject(pstSkill);

		CQSkillCell* pstSkill2 = CQSkillCell::create();	
		pstSkill2->m_uSkillID = 2;	
		pstSkill2->m_uLastCastTime = 0;	
		pFriend->getSkillArray()->addObject(pstSkill2);

		pFriend->LoadActionRes();
		pFriend->setLevel(stFriendInfo.level());
		pFriend->getBlood()->setVisible(true);
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pFriend->getPositionY());
		_sceneForegound->addChild(pFriend, iNewZorder);
		_stageEntities->addObject(pFriend);
	}
}

void CFightMapScene::initMonster(){
	m_stBrushAfterDieArr.clear();
	m_stDieMonsterData.clear();
	map<int, vector<CEnemyData*> >::iterator stMonsterID = m_stIDXToMonsters.find(m_uCurStageIdx);
	if(stMonsterID == m_stIDXToMonsters.end()){
		return;
	}

	map<int,int>::iterator stMonsterNum = m_stIDXToMonsterNum.find(m_uCurStageIdx);
	m_uCurStageMonNum = stMonsterNum->second;

	CCLOG("the stage is === %d, and the number of monster is ==== %d", m_uCurStageIdx, m_uCurStageMonNum);

	m_stCurStageMonsters.clear();
	m_stCurStageMonsters = stMonsterID->second;

	//初始化休眠的怪物放在舞台上
	_sleepMonsters->removeAllObjects();
	initSleepMonsters();
}

void CFightMapScene::initSleepMonsters(){
	CEnemyData* pstMonsterObj;
	CEnemy* pstMonster;
	for(unsigned int i = 0; i < m_stCurStageMonsters.size(); i++){
		pstMonsterObj = m_stCurStageMonsters.at(i);
		if(pstMonsterObj->type != APPEAR_SLEEP){		
			continue;
		}

		pstMonster = initOneMonster(pstMonsterObj);
		if(pstMonster != NULL){
			pstMonster->LoadActionRes();
			pstMonster->SleepAct();
			int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
			_sceneForegound->addChild(pstMonster, iNewZorder);
			_sleepMonsters->addObject(pstMonster);

			if(!pstMonster->getBlockSize().equals(CCSizeZero)){
				getBlocksInSize(pstMonster->getBlockSize(), (CCPoint)pstMonster->getPosition(), true);
				updateMapBlocks();
			}
		}
	}
}

void CFightMapScene::initFightData(){
	CBaseFightScene::initFightData();

	if(P_FIGHT_OBJ->getFightType() == NORMAL_FIGHT_TYPE || P_FIGHT_OBJ->getFightType() == DAILY_FIGHT_TYPE){
		WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(P_FIGHT_OBJ->getFightMapID());
		m_uTotalStage = stCfg->stagenum();
		m_uBossID = stCfg->bossid();
		m_uLimiteTime = stCfg->limittime();
		m_uWarLevel = stCfg->level();
		m_uWinType = stCfg->wintype();
		m_uWinValue1 = stCfg->winvalue1();
		m_uWinValue2 = stCfg->winvalue2();
	}else{
		const EliteWarMapCfg* stEliteCfg = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(P_FIGHT_OBJ->getFightMapID());
		m_uTotalStage = stEliteCfg->stagenum();
		m_uBossID = stEliteCfg->bossid();
		m_uLimiteTime = stEliteCfg->limittime();
		m_uWarLevel = stEliteCfg->level();
		m_uWinType = stEliteCfg->wintype();
		m_uWinValue1 = stEliteCfg->winvalue1();
		m_uWinValue2 = stEliteCfg->winvalue2();
	}
	m_uCurStageIdx = 1;
	m_bIsIntroduced = false;
	m_bHasShowStage = false;
	m_bStartedBrush = false;
	m_bIsWinComplete = false;
	m_bIsArrivedEdge = false;
	m_bIsAutoWalking = false;

	m_uCurStageMonNum = 0;
	m_fStartWalkTime = 0;
	m_uLastKillID = 0;
}

void CFightMapScene::update(float delta){
	if(m_bIsPreOver){
		return;
	}

	CBaseFightScene::update(delta);

	setViewpointCenter(_mainHero->getPosition());

	if(!m_bHasShowStage){
		checkShowStage();
	}

	checkBrushSceneItem();
}

void CFightMapScene::checkShowStage(){
	float triggerDis;		//触发刷怪的条件
	map<int, BeginData>::iterator stBeginIter;
	stBeginIter = m_stBeginData.find(m_uCurStageIdx);
	if(stBeginIter == m_stBeginData.end()){
		triggerDis = walkableRect.getMaxX() - _screenW*FIGHT_STAGE_NUM + START_BRUSH_DIS;	
	}else{
		triggerDis = stBeginIter->second.x;
	}

	if(_mainHero->getPositionX() >= triggerDis){
		CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
		unsigned int warId = P_FIGHT_OBJ->getFightMapID();
		CStageTipLayout* pStageTip = CStageTipLayout::create();
		CCPoint stVisPos = CCDirector::sharedDirector()->getVisibleOrigin();
		pStageTip->setPosition(stVisPos + ccp((stSize.width - pStageTip->getSize().width)/2, stSize.height/2));
		m_pUiLayer->addWidget(pStageTip);

		pStageTip->showStageTip(warId, m_uCurStageIdx);

		m_bHasShowStage = true;
	}
}

void CFightMapScene::brushMonsterLogic(float dt){
	if(m_bIsPreOver){  //已经在等待胜利
		return;
	}

	float triggerDis;		//触发刷怪的条件
	map<int, BeginData>::iterator stBeginIter;
	stBeginIter = m_stBeginData.find(m_uCurStageIdx);
	if(stBeginIter == m_stBeginData.end()){
		triggerDis = walkableRect.getMaxX() - _screenW*FIGHT_STAGE_NUM + START_BRUSH_DIS;	
	}else{
		triggerDis = stBeginIter->second.x;
	}

	//已经开始刷怪
	if(m_bStartedBrush){
		brushMonster();
	}else{			//未触发本阶段的刷怪条件
		if(_mainHero->getPositionX() >= triggerDis){
			m_bStartedBrush = true;
		}
	}
}

void CFightMapScene::brushMonster(){

	CEnemyData* pstMonsterObj;
	for(unsigned int i = 0; i < m_stCurStageMonsters.size(); i++){
		pstMonsterObj = m_stCurStageMonsters.at(i);

		CBurshObj stBrushObj;
		stBrushObj.pLayer = this;
		stBrushObj.pstEnemyObj = pstMonsterObj;
		stBrushObj.iBrushTime = m_fFightTime - m_fStageStartTime;
		bool isBrush = CInstanceFightUtils::BrushMonsterLogic(stBrushObj);

		if(isBrush && pstMonsterObj->id == m_uBossID){
			showBossWarning();
		}
	}
}

void CFightMapScene::checkBrushSceneItem(){
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

void CFightMapScene::brushSceneItemByTouch(unsigned int touchID){
	//当前阶段的item
	map<int, vector<CItemObj*> >::iterator stItemID;
	stItemID = m_stIDXToItems.find(m_uCurStageIdx);
	if(stItemID == m_stIDXToItems.end()){
		return;
	}

	vector<CItemObj*> tempArr = stItemID->second;
	CItemObj* itemObj;
	vector<CItemObj*>::iterator stIter;
	for(stIter = tempArr.begin(); stIter != tempArr.end();){
		itemObj = *stIter;

		if(itemObj->touchId == touchID){
			//添加物件
			addSceneItem(itemObj);
			stIter = tempArr.erase(stIter);
		}else{
			stIter++;
			continue;
		}
	}

	m_stIDXToItems.find(m_uCurStageIdx)->second = tempArr;
}

void CFightMapScene::brushFloorBuffByTouch(unsigned int touchID){
	//当前阶段的floorbuff
	map<int, vector<CFloorBuffObj*> >::iterator stBuffID;
	stBuffID = m_stIDXToFloorBuffs.find(m_uCurStageIdx);
	if(stBuffID == m_stIDXToFloorBuffs.end()){
		return;
	}

	vector<CFloorBuffObj*> tempArr = stBuffID->second;
	CFloorBuffObj* buffObj;
	vector<CFloorBuffObj*>::iterator stIter;
	for(stIter = tempArr.begin(); stIter != tempArr.end();){
		buffObj = *stIter;

		if(buffObj->touchId == touchID){  
			//添加floorbuff
			if(FLOOR_BUFF_CFG->GetFloorBuffCfgByID(buffObj->id)->bufftype() == BUFF_TYPE_WEAPON){
				addWeaponBuff(buffObj->id, buffObj->party, ccp(buffObj->x, _sceneForegound->getContentSize().height - buffObj->y));
			}else{
				addFloorBuff(buffObj->id, buffObj->party, ccp(buffObj->x, _sceneForegound->getContentSize().height - buffObj->y));
			}

			stIter = tempArr.erase(stIter);
		}else{
			stIter++;
			continue;
		}
	}

	m_stIDXToFloorBuffs.find(m_uCurStageIdx)->second = tempArr;
}

void CFightMapScene::brushTrapByTouch(unsigned int touchID){
	//当前阶段的陷阱
	map<int, vector<CTrapObj*> >::iterator stTrapIter;
	stTrapIter = m_stIDXToTraps.find(m_uCurStageIdx);
	if(stTrapIter == m_stIDXToTraps.end()){
		return;
	}

	vector<CTrapObj*> tempArr = stTrapIter->second;
	CTrapObj* trapObj;
	vector<CTrapObj*>::iterator stIter;
	for(stIter = tempArr.begin(); stIter != tempArr.end();){
		trapObj = *stIter;

		if(trapObj->touchId == touchID){
			//添加陷阱
			if(TRAP_CFG_MASTER->GetTrapCfgByID(trapObj->id)->type() != TRAP_TYPE_PUNCTURE){
				addSceneTrap(trapObj);
			}
			stIter = tempArr.erase(stIter);
		}else{
			stIter++;
			continue;
		}
	}

	m_stIDXToTraps.find(m_uCurStageIdx)->second = tempArr;
}

void CFightMapScene::updateInstnaceByAI(){
	CBaseFightScene::updateInstnaceByAI();

	checkBossInView();
}

void CFightMapScene::checkBossInView(){
	if(m_bIsIntroduced){
		return;
	}

	CCObject* pstObj;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){  //敌方
			CEnemy* pstEnemy = dynamic_cast<CEnemy*>(pstObj);
			if(pstEnemy->getMonsterObj()->id == m_uBossID){  //boss
				float disW = CCDirector::sharedDirector()->getVisibleSize().width/2 - pstEnemy->getCenterToSides();
				if(fabs(pstEnemy->getPositionX() - _mainHero->getPositionX()) < disW){  //boss首次出现在视野中
					m_bIsIntroduced = true;

					//策划要求暂时屏蔽 2014-9-6
					//CCFiniteTimeAction* showFun = CCCallFunc::create(this, callfunc_selector(CFightMapScene::showBossEmergePanel));
					//this->runAction(CCSequence::create(CCDelayTime::create(0.5f), showFun, NULL));
				}
			}
		}
	}
}

void CFightMapScene::showBossEmergePanel(){
	if(m_bIsPreOver){
		return;
	}

	pauseFight();
	P_LEVEL_CTL->openBossEmergePanelByCfgID(m_uBossID, this, callfunc_selector(CFightMapScene::resumeFight));

	CCLOG("the boss is emerged =======");
}

void CFightMapScene::handlePassLogic(){
	if(m_bIsPreOver){
		return;
	}

	unsigned int uNow = GlobalTime::getTime();
	//检查是否到限定时间
	if((unsigned int)(m_fFightTime) > m_uLimiteTime){ 
		P_FIGHT_OBJ->setIsForceQuit(false);
		handleFailLogic();
		return;
	}

	if(m_uCurStageMonNum > 0){
		checkShowGoTip();
		return;
	}
	
	if(m_uCurStageIdx >= m_uTotalStage){
		return;
	}

	//下一阶段
	if(!m_bIsAutoWalking){
		checkNextStage();
	}
	checkArrivedEdge();
}

void CFightMapScene::checkNextStage(){
	if(m_bIsPreOver || m_uCurStageMonNum > 0 || m_uCurStageIdx >= m_uTotalStage){
		return;
	}

	if(_mainHero->getIsTheOne() && !m_bIsAutoWalking){
		return;
	}

	if(m_bIsGamePause){
		return;
	}

	walkToNextStage();
}

void CFightMapScene::setAtkBtnEnable(bool isEnable){
	fireBtn->setTouchEnabled(isEnable);
	m_trickBtn->setBtnEnable(isEnable);
	m_skillBtn->setBtnEnable(isEnable);
	m_dodgeBtn->setBtnEnable(isEnable);
	pauseBtn->setTouchEnabled(isEnable);
}

void CFightMapScene::walkToNextStage(){
	m_bIsAutoWalking = true;
	m_bIsWalkNextStage = true;
	clearAllSceneItems();
	clearAllTraps();
	clearAllSkills();
	clearAllBullet();
	clearAllBuffs();
	mainHeroCeaseShoot();
	setAtkBtnEnable(false);
	_mainHero->ForceIdle();
	_hudLayer->Inactive();
	m_pUiLayer->removeAllFireBtn();

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

	//走向下一阶段
	float x = walkableRect.getMaxX() + _mainHero->getCenterToSides();
	_mainHero->WalkTo(ccp(x, m_uWalkHight/2));

	if(P_LEVEL_OBJ->getIsHasFriend() && getFriend()){   //暂停好友
		CMainHero* pFriend = dynamic_cast<CMainHero*>(getFriend());
		pFriend->SetIsControlledByAI(false);
		pFriend->stateChangeByAttack(false);
		if(pFriend->getScale() != 1.0f){
			pFriend->setScale(1.0f);
		}
		pFriend->ForceIdle();
		pFriend->WalkTo(ccp(x, m_uWalkHight/2));
	}

	m_fStartWalkTime = m_fFightTime;
}

void CFightMapScene::removeStageTip(){
	_hudLayer->Active();
	setAtkBtnEnable(true);

	if(m_pTipLayer && m_pTipLayer->getParent()){
		m_pTipLayer->removeFromParentAndCleanup(true);
		m_pTipLayer = NULL;
	}
}

void CFightMapScene::checkArrivedEdge() {
	unsigned int wallWidth = 10.0f;
	CCRect rightWall = CCRectMake(getStageRect().size.width - wallWidth, 0, wallWidth, getStageRect().size.height);

	const CCPoint &pos = _mainHero->getPosition();
	const CCSize &size = _mainHero->getContentSize();
	CCRect playerRect = CCRectMake(pos.x - size.width * 0.5, pos.y - size.height * 0.5, size.width, size.height);

	if(!m_bIsArrivedEdge) {
		if((m_fFightTime - m_fStartWalkTime > ConstHelper(FIGHT_STAGE_WALK_TIME)*1.0f/1000 || rightWall.intersectsRect(playerRect)) 
			&& !m_bIsGameOver && m_bIsAutoWalking) {
			m_bIsArrivedEdge = true;
			nextStage();
		}
	}
}

void CFightMapScene::nextStage(){
	//过渡场景
	CTransitionLayer* pLayer = CTransitionLayer::create();
	UILayer *pTopLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_TOP));
	CCPoint stPos = CCDirector::sharedDirector()->getVisibleOrigin();
	//pLayer->setPosition(stPos);
	pLayer->setZOrder(100);
	pTopLayer->addWidget(pLayer);
	pLayer->AddEventListener(this, CTransitionLayer::TRANSITION_MID_EVENT, eventListner_selector(CFightMapScene::handleTransitionMidEvent));
	pLayer->AddEventListener(this, CTransitionLayer::TRANSITION_COMPLETE, eventListner_selector(CFightMapScene::handleTransitionComplete));
	pLayer->startAct();
}

void CFightMapScene::setNextStagePos(){
	m_fStageStartTime = m_fFightTime;		//每阶段开始时间

	float xx = walkableRect.getMinX() + 300;
	_mainHero->SetEnityPosition(ccp(xx, m_uWalkHight/2));
	_mainHero->setDesiredPosition(_mainHero->getPosition());
	_mainHero->ForceIdle();
	_mainHero->resetScale();

	if(P_LEVEL_OBJ->getIsHasFriend() && getFriend()){
		CQStageEntity* pFriend = getFriend();
		pFriend->setPosition(ccp(xx - 100, m_uWalkHight/2));
		pFriend->ForceIdle();
		pFriend->SetIsControlledByAI(true);
	}

	float x = walkableRect.getMinX();
	_sceneForegound->setPositionX(x);
	updateBgViewCenter(x);
}

void CFightMapScene::handleTransitionMidEvent(const string& stEvent, CCObject* pstEventObj){
	m_uCurStageIdx++;

	int rectH = walkableRect.size.height;
	int rectW = FIGHT_STAGE_NUM*_screenW;
	int rectX = (m_uCurStageIdx - 1)*rectW;
	walkableRect.setRect(rectX, 0, rectW, rectH);
	updateMapBlocks();

	setNextStagePos();

	removeStageTip();
}

void CFightMapScene::handleTransitionComplete(const string& stEvent, CCObject* pstEventObj){
	//加载下一关后重置
	m_bHasShowStage = false;
	m_bStartedBrush = false;
	m_bIsArrivedEdge = false;
	m_bIsAutoWalking = false;
	m_bIsWalkNextStage = false;

	//加载本阶段的怪和item
	initMonster();				//初始化本阶段的怪物数据
	initSceneItem();			//构建本阶段的item
	initFloorBuff();			//本阶段的场景buff
	initTrap();					//本阶段的陷阱
}

void CFightMapScene::checkShowGoTip(){
	if(m_uCurStageMonNum <= 0 || m_fFightTime < 3.0f){
		return;
	}

	if(getMonsterNumOnStage() <= 0){  //舞台上没有怪物
		if(!m_pnextStageTip->getIsActing()){
			showGoTip();
		}
	}else{
		if(m_pnextStageTip && m_pnextStageTip->getIsActing()){
			cancelGoTip();
		}
	}
}

bool CFightMapScene::checkWinLogic(unsigned int cfgID){
	if(m_uWinType == WIN_TYPE_CLEAN_ALL){  //杀死所有怪物
		if(m_uCurStageIdx >= m_uTotalStage && m_uCurStageMonNum <= 0){
			return true;
		}
	}else if(m_uWinType == WIN_TYPE_KILL_ONE){  ////消灭某个ID
		if(m_uWinValue1 == cfgID){
			return true;
		}
	}

	return false;
}

void CFightMapScene::handleSuccessLogic(){
	//胜利
	unsigned int uTwoStarTime;
	unsigned int uThreeStarTime;
	if(P_FIGHT_OBJ->getFightType() == NORMAL_FIGHT_TYPE || P_FIGHT_OBJ->getFightType() == DAILY_FIGHT_TYPE){
		WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(P_FIGHT_OBJ->getFightMapID());
		uTwoStarTime = stCfg->starcondition(0);
		uThreeStarTime = stCfg->starcondition(1);
	}else{
		const EliteWarMapCfg* stEliteCfg = ELITE_WAR_MAP_CFG_MASTER->getEliteWarMapCfg(P_FIGHT_OBJ->getFightMapID());
		uTwoStarTime = stEliteCfg->starcondition(0);
		uThreeStarTime = stEliteCfg->starcondition(1);
	}

	if(m_fFightTime > uTwoStarTime){  //1星
		m_uScore = 1;
	}else if(m_fFightTime > uThreeStarTime && m_fFightTime <= uTwoStarTime){				//2星
		m_uScore = 2;
	}else{					//3星
		m_uScore = 3;		
	}

	P_FIGHT_OBJ->setIsForceQuit(false);
	handleGameOver();
}

void CFightMapScene::handleGiveUpLogic(CCObject* pstObj){
    P_FIGHT_OBJ->setIsForceQuit(true);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, GIVEUP_FIGHT_EVENT);
	handleFailLogic();
}

void CFightMapScene::handleFailLogic(){
	if(m_bIsPreOver){
		return;
	}

	m_uScore = 0;
	m_uScorePercent = 0;
	m_uMark = 0;

	handleGameOver();
}

void CFightMapScene::handleGameOver(){
	CToolUtils::setTimeScale(1.0f);
	m_bIsPreOver = true;
	m_bIsGameOver = true;

	P_FIGHT_OBJ->setScore(m_uScore);
	P_FIGHT_OBJ->setMaxKill(m_uMaxKillValue);
	P_FIGHT_OBJ->setTotalTime((unsigned int)(m_fFightTime));
	P_FIGHT_OBJ->setScorePercent(m_uScorePercent);
	P_FIGHT_OBJ->setMark(m_uMark);
	P_FIGHT_OBJ->getCheckInfo().set_evolutionusecnt(m_uEvolutionCnt);
	P_FIGHT_OBJ->getCheckInfo().set_score(m_uScore);

	fireBtn->setTouchEnabled(false);
	fireBtn->setFocused(false);
	_mainHero->Idle();
	unSetMainHeroAsTheOne();
	mainHeroCeaseShoot();
	if(m_uScore != 0){	//胜利
		//清除未消失的怪物
		clearAllMonsters();
	}

	bool bIsShowLoading = true;
	if(P_FIGHT_OBJ->getFightType() == NORMAL_FIGHT_TYPE){
		doNormalWarHideAward();

		P_FIGHT_CTRL->getFightProxy()->sendWarMapEndRqst(P_FIGHT_OBJ->getCheckInfo());
	}else if(P_FIGHT_OBJ->getFightType() == ELITE_FIGHT_TYPE){	//精英副本
		P_FIGHT_CTRL->getFightProxy()->sendEndEliteWarMapRqst(P_FIGHT_OBJ->getFightMapID(), m_uEvolutionCnt, m_uScore);
	}else if(P_FIGHT_OBJ->getFightType() == DAILY_FIGHT_TYPE){  //日常副本
		doDailyInstanceOver();
		bIsShowLoading = false;
	}

	//清除所有技能以及子弹
	clearAllBullet();
	clearAllSkills();

	unscheduleUpdate();
	stopAllActions();
	stopAllMonsters();

	this->unschedule(schedule_selector(CFightMapScene::updateCoinPos));

	//转菊花
	if(bIsShowLoading){
		P_LEVEL_CTL->getResultPanel()->showLoading();
	}
}

void CFightMapScene::doNormalWarHideAward(){
	if(P_LEVEL_OBJ->getIsHasFriend()){		//好友助战没有隐藏奖励
		return;
	}

	WarMapCfg* stCfg = WAR_MAP_CFG->GetWarMapCfgByID(P_FIGHT_OBJ->getFightMapID());
	if(stCfg == NULL){
		return;
	}

	if(stCfg->hidenawards_size() <= 0){   //关卡没有隐藏奖励
		return;
	}

	vector<unsigned int> stHideCells;
	for(int i = 0; i < stCfg->hidenawards_size(); i++){
		if(IsAchieveHideAward(stCfg->hidenawards(i).wintype(), stCfg->hidenawards(i).wincondition())){
			stHideCells.push_back(1);
		}else{
			stHideCells.push_back(0);
		}
	}

	P_FIGHT_OBJ->setWarHideCondition(stHideCells);
}

bool CFightMapScene::IsAchieveHideAward(unsigned int uType, unsigned int uValue){
	switch (uType)
	{
	case WAR_HIDE_HP_TYPE:		//血量百分比，高于条件值可达成条件
		if(_mainHero->getHP()*1.0f/_mainHero->getMaxHP() >= uValue*1.0f/EGAME_DENOM){
			return true;
		}
		break;
	case WAR_HIDE_TIME_TYPE:	//时间条件，低于条件值可达成条件
		if(m_fFightTime < uValue){
			return true;
		}
		break;
	case WAR_HIDE_MEDICAL_TYPE:	//不使用医疗包
		if(m_uUseMedicalCnt <= 0){
			return true;
		}
		break;
	case WAR_HIDE_BULLET_TYPE:	//不使用弹药箱
		if(m_uUseBulletCnt <= 0){
			return true;
		}
		break;
	case WAR_HIDE_RELIVE_TYPE:	//不使用复活币
		if(m_uUseReliveCnt <= 0){
			return true;
		}
		break;
	default:
		return false;
		break;
	}

	return false;
}

void CFightMapScene::doDailyInstanceOver(){
	if(m_uScore == 0){  //直接退出 or 失败直接退出场景
		if(P_FIGHT_OBJ->getIsForceQuit()){
			SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);
		}else{
			P_LEVEL_CTL->getFailPanel()->open();
			P_FIGHT_CTRL->setLastFightFailed(true);
		}
	}else{  //发日常副本成功协议
		P_LEVEL_CTL->getResultPanel()->showLoading();
		DAILY_INSTANCE_CTRL->doWarWin();
	}
}

void CFightMapScene::showGoTip(){
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	if(m_pnextStageTip->getParent() != NULL){
		m_pnextStageTip->stopAct();
		m_pnextStageTip->removeFromParent();
	}

	m_pUiLayer->addWidget(m_pnextStageTip);

	m_pnextStageTip->setPosition(visibleOrigin + ccp(visibleSize.width/2 - m_pnextStageTip->getContentSize().width/2, visibleSize.height*3/5));
	m_pnextStageTip->startAct();
}

void CFightMapScene::cancelGoTip(){
	if(m_pnextStageTip && m_pnextStageTip->getParent() != NULL){
		m_pnextStageTip->stopAct();
		m_pnextStageTip->removeFromParent();
	}
}

void CFightMapScene::setViewpointCenter(const CCPoint& position){
	CBaseMapScene::setViewpointCenter(position);
}

void CFightMapScene::updateBgViewCenter(float foreX){
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

CCRect CFightMapScene::getStageRect() const{
	int rectH = walkableRect.size.height;
	int rectW = FIGHT_STAGE_NUM*_screenW;
	int rectX = (m_uCurStageIdx - 1)*rectW;

	return CCRectMake(rectX, 0, rectW, rectH);
}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CFightMapScene::doHeroDieFail(){
	if(m_bIsPreOver){
		return;
	}

	P_FIGHT_OBJ->setIsForceQuit(false);
	handleFailLogic();
}

void CFightMapScene::handleFriendDieEvent(const string& stEvent, CCObject* pstEventObj){
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);

	 //友军死亡
	_sceneForegound->removeChild(pstEntity, true);
	if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
		_stageEntities->removeObject(pstEntity);
	}
}

void CFightMapScene::handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj){
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);

	if(pstEntity){
		pstEntity->getBlood()->setPercentage(100.0*pstEntity->getHP()/pstEntity->getMaxHP());
	}
}

void CFightMapScene::handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj){
	//计入总伤害 
	CCString* strValue = (CCString*)pstEventObj;
	int harmValue = strValue->intValue();
	m_uTotalHarmValue += harmValue;
}

void CFightMapScene::handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	MonsterCfg* stMonsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(monster->getMonsterObj()->id);

	if(stMonsterCfg->floorbuffid() != 0){
		addFloorBuff(stMonsterCfg->floorbuffid(), monster->getRoleParty(), monster->GetFootPos());
	}

	//怪物死亡，数据变化
	handleHeroKillValueChange(stEvent, pstEventObj);

	//检查通过条件
	if(!m_bIsPreOver && checkWinLogic(monster->getMonsterObj()->id)){  //已经胜利,主角不可被攻击
		m_bIsPreOver = true;
		m_uLastKillID = monster->m_uID;		//记录胜利时候杀死的怪物id
		mainHeroCeaseShoot();
		_mainHero->ForceIdle();
		_mainHero->setIsInvincible(true);
		_mainHero->winAct(true);
		clearAllSkills();
		clearAllBullet();
		clearAllArrows();
		forceOhterMonstersDie();			//其他怪物都死
		setAttackOpBtnVisible(false);		//隐藏技能攻击按钮
		CToolUtils::setTimeScale(0.5f);    //放慢1倍

		//播放胜利特效
		CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
		CCArmature* pWinEff = CCArmature::create("ut00036");
		AudioManager::stopAllEffects();
		AudioManager::playEffect("MissionComplete.ogg");		//音效
		m_pUiLayer->addChild(pWinEff, 2014);
		pWinEff->setPosition(visibleOrigin + ccp(stSize.width/2, stSize.height/2));
		pWinEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CFightMapScene::handleWinEffComplete));
		pWinEff->getAnimation()->play("fly", -1, -1, 0);
	}

	//金币掉落，飘金币
	addCoinEff(monster, stMonsterCfg->dropcoin());
	m_uDropCoin += stMonsterCfg->dropcoin();
}

void CFightMapScene::handleWinEffComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);

		if(m_bIsWinComplete){  //与死亡动作比较，谁后播完，就结束
			handleSuccessLogic();
		}else{
			m_bIsWinComplete = true;
		}
	}	
}

void CFightMapScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	MonsterCfg* stMonsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(pstEntity->getMonsterObj()->id);

	//把已经播放完毕死亡动画的死人做掉
	unsigned int mID = pstEntity->m_uID;
	if(pstEntity->getMyState() == kActionStateDie){  
		if(pstEntity->getMonsterObj()->type != APPEAR_AFTER_DIE){
			updateResToNum(pstEntity->getMonsterObj()->id, MONSTER_TYPE, false);
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
	if(checkWinLogic(stMonsterCfg->id())){
		if(m_uLastKillID == mID){
			if(m_bIsWinComplete){
				handleSuccessLogic();
			}else{
				m_bIsWinComplete = true;
			}
		}
	}
}

void CFightMapScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);

	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	if(pstEntity){
		CEnemyData* pstEnemyData = pstEntity->getMonsterObj();

		if(pstEnemyData->type != APPEAR_AFTER_DIE){
			--m_uCurStageMonNum;
		}else{
			pstEnemyData->brushTime = m_fFightTime;
		}
		
		CCLOG("=====the monster be killed, and the number of last monster is ==== %d", m_uCurStageMonNum);

		//boss死亡，血条消失
		if(pstEntity->getRoleType() == ROLE_TYPE_SENIOR_BOSS || pstEntity->getRoleType() == ROLE_TYPE_JUNIOR_BOSS){
			if(m_pBossHPUI->isVisible()){
				m_pBossHPUI->setVisible(false);
			}

			if(m_pBossWarning && m_pBossWarning->getParent()){  //boss警告去掉
				m_pBossWarning->RemoveEventListener(this, CBossWarning::BOSS_WARNING_COMPLETE, eventListner_selector(CFightMapScene::handleBossWarningCompleteEvent));
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

		if(pstEntity->getMonsterObj()->id == m_uBossID){
			P_FIGHT_OBJ->getCheckInfo().set_bossid(pstEntity->getMonsterObj()->id);
		}

		const google::protobuf::RepeatedPtrField<vmsg::CSKillMonsterInfo>& monsterInfos = P_FIGHT_OBJ->getCheckInfo().monsterinfos();

		for(int i = 0; i < monsterInfos.size(); i++){
			const vmsg::CSKillMonsterInfo& tempInfo = monsterInfos.Get(i);
			if(tempInfo.monsterid() == pstEntity->getMonsterObj()->id){
				unsigned int preCnt = tempInfo.monstercnt();
				P_FIGHT_OBJ->getCheckInfo().mutable_monsterinfos(i)->set_monstercnt(++preCnt);
				break;
			}
		}
	}

	//检测是否到下一阶段
	checkNextStage();
}

void CFightMapScene::handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj){
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

void CFightMapScene::handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj){
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

void CFightMapScene::hanldeWinCompleteFun(const string& stEvent, CCObject* pstEventObj){
	//CBaseFightScene::hanldeWinCompleteFun(stEvent, pstEventObj);

	//P_FIGHT_CTRL->getFightProxy()->sendWarMapEndRqst(P_FIGHT_OBJ->getCheckInfo());
}

void CFightMapScene::btnTouchEvent(CCObject *pSender, TouchEventType type){
	CBaseFightScene::btnTouchEvent(pSender, type);
}

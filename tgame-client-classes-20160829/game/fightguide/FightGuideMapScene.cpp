#include "FightGuideMapScene.h"
#include "UserObj.h"
#include "role/Enemy.h"
#include "fight/SkillEffLayer.h"
#include "instance/InstanceUtils.h"
#include "ToolUtils.h"
#include "GlobalTime.h"
#include "SceneManager.h"
#include "fight/BossWarning.h"
#include "fight/FightTipLayout.h"
#include "fight/StageTipLayout.h"
#include "FloorBuffCfgMaster.h"
#include "fight/PunctureTrap.h"
#include "QEnvSet.h"
#include "control/AudioManager.h"
#include "instance/AnimationUtils.h"
#include "instance//InstanceFightUtils.h"
#include "fightguide/FightGuideControl.h"
#include "fight/FightObj.h"
#include "evtlog/EvtLog.h"
#include "fight/FightWeaponArrLayout.h"
#include "fight/FightMedicalLayout.h"
#include "fight/FightControl.h"

#include "HeroCfgMaster.h"
#include "MonsterCfgMaster.h"
#include "SkillCfgMaster.h"
#include "BulletCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "FightGuideMapCfgMaster.h"


CFightGuideMapScene::CFightGuideMapScene()
:m_pnextStageTip(NULL)
,_sleepMonsters(NULL)
,m_pBossWarning(NULL)
,m_pTipImg(NULL)
,m_stBuffPos(CCPointZero)
{
	_sleepMonsters = CCArray::create();
	_sleepMonsters->retain();
}

CFightGuideMapScene::~CFightGuideMapScene(){
	CC_SAFE_RELEASE_NULL(m_pnextStageTip);

	if (_sleepMonsters){
		_sleepMonsters->removeAllObjects();
		_sleepMonsters->release();
		_sleepMonsters = NULL;
	}
}

bool CFightGuideMapScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CFightGuideMapScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	m_pTipImg = ImageView::create();
	m_pTipImg->setPosition(visibleOrigin + ccp(visibleSize.width/2, visibleSize.height*2/3));
	m_pTipImg->loadTexture("fight/fight_tip_img.png", UI_TEX_TYPE_PLIST);
	addChild(m_pTipImg, 10);

	startFight();
}

void CFightGuideMapScene::startFight(){
	CCPoint stPos = ccp(m_stBuffPos.x, _sceneForegound->getContentSize().height - m_stBuffPos.y);
	moveToDst(_mainHero, stPos);

	setAttackOpBtnVisible(false);
}

void CFightGuideMapScene::moveToDst(CQStageEntity* pstEntity, CCPoint stPos){
	if(!walkableRect.containsPoint(stPos)) {
		CCLOG("fuck... npc outside of walkable rect");
		return;
	}

	if(pstEntity->getPosition().getDistance(stPos) < 40.0f) {
		onArrived("walk_done", pstEntity);
	} else {
		pstEntity->WalkTo(stPos);
		pstEntity->AddEventListener(this, "walk_done", eventListner_selector(CFightGuideMapScene::onArrived));
	}
}

void CFightGuideMapScene::onArrived(const string &evt, CCObject *pSender) {
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pSender);
	pstEntity->RemoveEventListener(this, "walk_done", eventListner_selector(CFightGuideMapScene::onArrived));
	if(pstEntity->getRoleParty() == ROLE_PARTY_FRIEND){  //����
		setAttackOpBtnVisible(true);
		this->scheduleUpdate();

		this->schedule(schedule_selector(CFightGuideMapScene::updateCoinPos), 0.02f);
	}
}

void CFightGuideMapScene::setMapRes(){
	// ���ݳ������ͻ�ȡ��ͼ������
	const FightGuideMapCfg* stCfg = FIGHT_GUIDE_MAP_CFG_MASTER->GetFightGuideMapCfgByID(FIGHT_GUIDE_NORMAL_SCENE_ID);
	_cfgFileName = stCfg->cfgid();  //������
	m_stFrontStr = stCfg->frontid();
	m_stForeStr = stCfg->foreid();
	m_stMidStr = stCfg->midid(); 
	m_stBackStr = stCfg->backid(); 
	m_stMid2Str = stCfg->midid2();
}

void CFightGuideMapScene::setWalkHight(){
	const FightGuideMapCfg* stCfg = FIGHT_GUIDE_MAP_CFG_MASTER->GetFightGuideMapCfgByID(FIGHT_GUIDE_NORMAL_SCENE_ID);
	m_fMidPos = stCfg->midpos();
	m_uWalkHight = stCfg->midwalkh();
}

void CFightGuideMapScene::loadMapBgRes(){
	CBaseMapScene::loadMapBgRes();
}

void CFightGuideMapScene::loadItemRes(){
	CBaseFightScene::loadItemRes();

	//��item���׶δ��
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

void CFightGuideMapScene::loadFloorBuffRes(){
	CBaseFightScene::loadFloorBuffRes();

	//��floorbuff���׶δ��
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

		m_stBuffPos = ccp(buffObj->x, buffObj->y);
	}
}

void CFightGuideMapScene::loadTrapRes(){
	CBaseFightScene::loadTrapRes();

	//�����尴�׶δ��
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

void CFightGuideMapScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();

}

void CFightGuideMapScene::loadMonsterRes(){
	//�����ﰴ�׶δ��
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

		if(monsterObj->type == APPEAR_AFTER_DIE){  //������ˢ������
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

	m_stRes.insert("monster/jiuhuche/jiuhuche.ExportJson");	//�Ȼ�����Դ
}

void CFightGuideMapScene::initMap(){
	CBaseFightScene::initMap();
	walkableRect = CCRect(0, 0, _screenW*FIGHT_STAGE_NUM, m_uWalkHight);
}

void CFightGuideMapScene::initSceneItem(){
	if(m_stIDXToItems.size() <= 0){
		updateMapBlocks();
		return;
	}

	//��ǰ�׶ε�item
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

		if(itemObj->touchId != 0){  //����id��Ϊ0����ʾ�������ߴ���
			stIter++;
			continue;
		}else{
			//������
			addSceneItem(itemObj);
			stIter = tempArr.erase(stIter);
		}
	}

	m_stIDXToItems.find(m_uCurStageIdx)->second = tempArr;

	updateMapBlocks();
}

void CFightGuideMapScene::initFloorBuff(){
	//��ǰ�׶ε�floorbuff
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

		if(buffObj->touchId != 0){  //����id��Ϊ0����ʾ�������ߴ���
			stIter++;
			continue;
		}else{
			//���floorbuff
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

void CFightGuideMapScene::initTrap(){
	//��ǰ�׶ε�����
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

		//����id��Ϊ0���Ҳ���ȫ�����������壬��ʾ�������ߴ���
		if(trapObj->touchId != 0 && TRAP_CFG_MASTER->GetTrapCfgByID(trapObj->id)->type() != TRAP_TYPE_PUNCTURE){  
			stIter++;
			continue;
		}else{
			//�������
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

void CFightGuideMapScene::initMapUI(){
	CBaseFightScene::initMapUI();

	m_pnextStageTip = CFightTipLayout::create();
	m_pnextStageTip->retain();

	m_pArrLayout->setEnabled(false);
	m_pChangeWeaponBtn->setEnabled(false);

	CFightMedicalLayout* pMedicalLayout = P_FIGHT_CTRL->getMedicalLayout();
	pMedicalLayout->setEnabled(false);
}

void CFightGuideMapScene::initMainHero(){
	CBaseFightScene::initMainHero();

	_mainHero->ForceIdle();
	CCPoint stPlayerPt(walkableRect.getMinX() - (_mainHero->getCenterToSides()*2 + 10), m_uWalkHight/2);
	_mainHero->SetEnityPosition(stPlayerPt);
	_mainHero->setDesiredPosition(_mainHero->getPosition());
	_mainHero->setIsBulletLimit(false);
}


void CFightGuideMapScene::initMonster(){
	m_stBrushAfterDieArr.clear();
	m_stDieMonsterData.clear();
	m_stMonKeyToTime.clear();
	map<int, vector<CEnemyData*> >::iterator stMonsterID = m_stIDXToMonsters.find(m_uCurStageIdx);
	if(stMonsterID == m_stIDXToMonsters.end()){
		return;
	}

	map<int,int>::iterator stMonsterNum = m_stIDXToMonsterNum.find(m_uCurStageIdx);
	m_uCurStageMonNum = stMonsterNum->second;

	CCLOG("the stage is === %d, and the number of monster is ==== %d", m_uCurStageIdx, m_uCurStageMonNum);

	m_stCurStageMonsters.clear();
	m_stCurStageMonsters = stMonsterID->second;

	//��ʼ�����ߵĹ��������̨��
	_sleepMonsters->removeAllObjects();
	initSleepMonsters();

	//�Ȼ�����ˢ����Ϣ
	m_stCurCarMonsters.clear();
	CEnemyData* pstMonsterObj;
	for(unsigned int i = 0; i < m_stCurStageMonsters.size(); i++){
		pstMonsterObj = m_stCurStageMonsters.at(i);

		if(pstMonsterObj->type == APPEAR_CAR){
			m_stCurCarMonsters.push_back(pstMonsterObj);
		}
	}
}

void CFightGuideMapScene::initSleepMonsters(){
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

void CFightGuideMapScene::initFightData(){
	CBaseFightScene::initFightData();

	const FightGuideMapCfg* stCfg = FIGHT_GUIDE_MAP_CFG_MASTER->GetFightGuideMapCfgByID(FIGHT_GUIDE_NORMAL_SCENE_ID);
	m_uTotalStage = stCfg->stagenum();
	m_uBossID = stCfg->bossid();
	m_uWinType = stCfg->wintype();
	m_uWinValue1 = stCfg->winvalue1();
	m_uWinValue2 = stCfg->winvalue2();
	m_uCurStageIdx = 1;
	m_bIsIntroduced = false;
	m_bHasShowStage = false;
	m_bPassViewChange = false;
	m_bStartedBrush = false;
	m_bIsPreSuccess = false;
	m_bIsWinComplete = false;
	m_bIsPlayKillEff = false;

	m_uCurStageMonNum = 0;
	m_fStartBrushTime = 0;
	m_uLastKillID = 0;
}

void CFightGuideMapScene::update(float delta){
	if(m_bIsGameOver){
		return;
	}

	CBaseFightScene::update(delta);

	if(m_bPassViewChange){
		passStageViewChange(delta);
	}else{
		setViewpointCenter(_mainHero->getPosition());
	}

	if(!m_bHasShowStage){
		checkShowStage();
	}

	checkBrushSceneItem();
	checkFightTipShow();
}

void CFightGuideMapScene::checkFightTipShow(){
	if(_mainHero->getMyState() != kActionStateIdle && m_pTipImg && m_pTipImg->getParent()){
		m_pTipImg->removeFromParentAndCleanup(true);
		m_pTipImg = NULL;
	}
}

void CFightGuideMapScene::updateSceneBuffs(float dt){
	CBaseFightScene::updateSceneBuffs(dt);
}

void CFightGuideMapScene::checkShowStage(){
	float triggerDis;		//����ˢ�ֵ�����
	map<int, BeginData>::iterator stBeginIter;
	stBeginIter = m_stBeginData.find(m_uCurStageIdx);
	if(stBeginIter == m_stBeginData.end()){
		triggerDis = walkableRect.getMaxX() - _screenW*FIGHT_STAGE_NUM + START_BRUSH_DIS;	
	}else{
		triggerDis = stBeginIter->second.x;
	}

	if(_mainHero->getPositionX() >= triggerDis){
		CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
		unsigned int warId = FIGHT_GUIDE_NORMAL_SCENE_ID;
		CStageTipLayout* pStageTip = CStageTipLayout::create();
		pStageTip->setPosition(ccp((stSize.width - pStageTip->getSize().width)/2, stSize.height/2));
		m_pUiLayer->addWidget(pStageTip);

		pStageTip->showStageTip(warId, m_uCurStageIdx);

		m_bHasShowStage = true;
	}
}

void CFightGuideMapScene::brushMonsterLogic(float dt){
	if(m_bIsPreSuccess){  //�Ѿ��ڵȴ�ʤ��
		return;
	}

	float triggerDis;		//����ˢ�ֵ�����
	map<int, BeginData>::iterator stBeginIter;
	stBeginIter = m_stBeginData.find(m_uCurStageIdx);
	if(stBeginIter == m_stBeginData.end()){
		triggerDis = walkableRect.getMaxX() - _screenW*FIGHT_STAGE_NUM + START_BRUSH_DIS;	
	}else{
		triggerDis = stBeginIter->second.x;
	}

	//�Ѿ���ʼˢ��
	if(m_bStartedBrush){
		m_fStartBrushTime += dt;
		brushMonster();
	}else{			//δ�������׶ε�ˢ������
		if(_mainHero->getPositionX() >= triggerDis){
			m_bStartedBrush = true;
			m_fStartBrushTime = 0;
		}
	}
}

void CFightGuideMapScene::brushMonster(){

	CEnemyData* pstMonsterObj;
	unsigned int key;
	int uNow;
	int startTime;
	bool bRepeated = false;		//�Ƿ��ظ�ˢ��
	map<unsigned int, int>::iterator stIter;
	for(unsigned int i = 0; i < m_stCurStageMonsters.size(); i++){
		uNow = GlobalTime::getTime();
		pstMonsterObj = m_stCurStageMonsters.at(i);

		if(pstMonsterObj->type == APPEAR_SLEEP){		//���ߵĹ����Ѿ�����̨��
			activateMonsterByTouch(pstMonsterObj);
			continue;
		}

		if(pstMonsterObj->type == APPEAR_CAR){			//�Ȼ���������ʽ
			activateCarByTouch(pstMonsterObj);
			continue;
		}

		if(pstMonsterObj->type == APPEAR_AFTER_DIE){   //ˢ�ֵ�ˢ���Ĺ����������ٰ�CDʱ��ˢ��һֻ��
			brushAfterDie(pstMonsterObj);
			continue;
		}

		if(pstMonsterObj->touchID != 0){
			brushMonsterByTouch(pstMonsterObj);
			continue;
		}

		key = pstMonsterObj->key;
		stIter = m_stMonKeyToTime.find(key);
		if(stIter == m_stMonKeyToTime.end()){
			m_stMonKeyToTime[key] = uNow;
			startTime = uNow;
			bRepeated = false;
		}else{
			startTime = stIter->second;
			bRepeated = true;
		}

		bool isCanBrush = false;
		int nowNum = pstMonsterObj->num;
		if(pstMonsterObj->time <= (int)m_fStartBrushTime && nowNum > 0){
			if(bRepeated == false){
				isCanBrush = true;
			}else{
				if(int(uNow - startTime) >= pstMonsterObj->inter){   //���ʱ��ˢ��
					isCanBrush = true;
				}
			}
		}

		if(isCanBrush){
			brushOneMonster(pstMonsterObj);
			pstMonsterObj->num = (--nowNum);
			m_stMonKeyToTime[key] = uNow;	

			if(pstMonsterObj->id == m_uBossID){
				showBossWarning();
			}
		}
	}
}

void CFightGuideMapScene::brushAfterDie(CEnemyData* pstObj){
	if(checkMonsterOnStage(pstObj)){  //��ˢ�ֵ㻹�й�����̨�ϣ���ˢ
		return;
	}

	unsigned int key = pstObj->key;
	map<unsigned int, int>::iterator stKeyIter = m_stMonKeyToTime.find(key);
	map<int, int>::iterator stIter;
	int touchID = pstObj->touchID;
	bool bIsBrush = false;
	int uNow = GlobalTime::getTime();
	int startTime;

	if(touchID == 0){  //ֱ��ˢ
		if(stKeyIter == m_stMonKeyToTime.end()){
			bIsBrush = true;
		}else{
			startTime = stKeyIter->second;
			if(int(uNow - startTime) >= pstObj->inter){
				bIsBrush = true;
			}else{
				bIsBrush = false;
			}
		}
	}else{  //��������
		stIter = m_stTouchData.find(touchID);
		if(stKeyIter == m_stMonKeyToTime.end()){
			if( _mainHero->getPositionX() >= stIter->second){
				bIsBrush = true;
			}else{
				bIsBrush = false;
			}
		}else{
			startTime = stKeyIter->second;
			if(int(uNow - startTime) >= pstObj->inter){
				bIsBrush = true;
			}else{
				bIsBrush = false;
			}
		}
	}

	if(bIsBrush){
		brushOneMonster(pstObj);
		m_stMonKeyToTime[key] = uNow;					//ˢ�ֺ�ʱ����Ϊ��ǰʱ��
	}
}

void CFightGuideMapScene::activateMonsterByTouch(CEnemyData* monsterObj){
	int touchID = monsterObj->touchID;
	map<int, int>::iterator stIter = m_stTouchData.find(touchID);

	if(stIter == m_stTouchData.end()){
		CCLOG("there is no touch id %d in map", touchID);
		return;
	}

	int dis = stIter->second;
	if(_mainHero->getPositionX() < dis){
		return;
	}

	CEnemy* pstMonster;
	CCObject* pstObj;
	CCARRAY_FOREACH(_sleepMonsters, pstObj){
		pstMonster = dynamic_cast<CEnemy*>(pstObj);
		int nowNum = pstMonster->getMonsterObj()->num;
		if((pstMonster->getMonsterObj()->touchID == touchID) && pstMonster->getIsActivate() == false && (pstMonster->getMonsterObj()->num > 0)){
			pstMonster->Active();
			pstMonster->getMonsterObj()->num = (--nowNum);

			if(_sleepMonsters->indexOfObject(pstMonster) != UINT_MAX){
				_sleepMonsters->removeObject(pstMonster);
			}
		}
	}
}

void CFightGuideMapScene::activateCarByTouch(CEnemyData* monsterObj){
	int touchID = monsterObj->touchID;
	map<int, int>::iterator stIter = m_stTouchData.find(touchID);

	if(stIter == m_stTouchData.end()){
		CCLOG("there is no touch id %d in map", touchID);
		return;
	}

	int dis = stIter->second;
	if(_mainHero->getPositionX() < dis){
		return;
	}

	CEnemyData* pstMonsterObj;
	int uNow = 0;
	for(unsigned int i = 0; i < m_stCurCarMonsters.size(); i++){
		pstMonsterObj = m_stCurCarMonsters.at(i);

		if(pstMonsterObj->touchID != touchID){
			continue;
		}

		uNow = GlobalTime::getTime();
		unsigned int key = pstMonsterObj->key;
		map<unsigned int, int>::iterator stKeyIter = m_stMonKeyToTime.find(key);

		if(stKeyIter != m_stMonKeyToTime.end()){		//�Ѿ�������
			return;
		}

		float startX = _screenW*FIGHT_STAGE_NUM*(m_uCurStageIdx - 1);
		float endX = _screenW*FIGHT_STAGE_NUM*m_uCurStageIdx;
		float xx = pstMonsterObj->x;
		float yy = _sceneForegound->getContentSize().height - pstMonsterObj->y;

		CCArmature* pCarArmature = CCArmature::create("jiuhuche");
		pCarArmature->setPosition(ccp(endX + pCarArmature->getContentSize().width/2, yy));
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pCarArmature->getPositionY());
		_sceneForegound->addChild(pCarArmature, iNewZorder);
		pCarArmature->getAnimation()->play("run", -1, -1, 1);

		float carWidth = pCarArmature->getContentSize().width/2;
		float moveInTime = (endX - xx + carWidth)/600.0f;
		float moveOutTime = (xx - startX - carWidth)/600.0f;
		CCActionInterval* moveIn = CCMoveTo::create(moveInTime, ccp(xx - carWidth, yy));
		CCFiniteTimeAction* brushCallFun = CCCallFuncO::create(this, callfuncO_selector(CFightGuideMapScene::brushFromCar), pstMonsterObj);
		CCActionInterval* moveOut = CCMoveTo::create(moveOutTime, ccp(startX - xx - carWidth, yy));
		CCFiniteTimeAction* endCallFun = CCCallFuncO::create(this, callfuncO_selector(CFightGuideMapScene::carMoveEnd), pCarArmature);
		pCarArmature->runAction(CCSequence::create(moveIn, CCDelayTime::create(0.4f), moveOut, endCallFun, NULL));
		pCarArmature->runAction(CCSequence::create(CCDelayTime::create(moveInTime), brushCallFun, NULL));
		m_stMonKeyToTime[key] = uNow;
	}
}

void CFightGuideMapScene::brushFromCar(CCObject* pstObject){
	CEnemyData* pstMonsterObj = dynamic_cast<CEnemyData*>(pstObject);

	CCFiniteTimeAction* brushOneFun = CCCallFuncO::create(this, callfuncO_selector(CFightGuideMapScene::brushFromCarOne), pstMonsterObj);
	this->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(0.2f), brushOneFun, NULL)));
}

void CFightGuideMapScene::brushFromCarOne(CCObject* pstObject){
	CEnemyData* pstMonsterObj = dynamic_cast<CEnemyData*>(pstObject);
	if(pstMonsterObj == NULL || pstMonsterObj->num <= 0){
		this->stopAllActions();
		return;
	}

	int nowNum = pstMonsterObj->num;
	CEnemy* pstMonster = initOneMonster(pstMonsterObj);
	if(pstMonster != NULL){
		pstMonster->LoadActionRes();
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
		_sceneForegound->addChild(pstMonster, iNewZorder);
		pstMonster->Active();

		if(!pstMonster->getBlockSize().equals(CCSizeZero)){
			getBlocksInSize(pstMonster->getBlockSize(), (CCPoint)pstMonster->getPosition(), true);
			updateMapBlocks();
		}
	}

	pstMonsterObj->num = (--nowNum);
}

void CFightGuideMapScene::carMoveEnd(CCObject* pstObject){
	CCArmature* pCarArmature = dynamic_cast<CCArmature*>(pstObject);
	if(pCarArmature != NULL && pCarArmature->getParent() != NULL){
		pCarArmature->removeFromParentAndCleanup(true);
	}
}

void CFightGuideMapScene::brushMonsterByTouch(CEnemyData* monsterObj){
	if(monsterObj->num <= 0){
		return;
	}

	int touchID = monsterObj->touchID;
	map<int, int>::iterator stIter = m_stTouchData.find(touchID);

	if(stIter == m_stTouchData.end()){
		CCLOG("there is no touch id %d in map", touchID);
		return;
	}

	int dis = stIter->second;
	unsigned int key = monsterObj->key;
	map<unsigned int, int>::iterator stKeyIter = m_stMonKeyToTime.find(key);
	int nowNum = monsterObj->num;
	int uNow = GlobalTime::getTime();
	int startTime;

	bool isCanBrush = false;
	if(stKeyIter == m_stMonKeyToTime.end()){		//δ������
		if(_mainHero->getPositionX() >= dis){		//�״δ���
			isCanBrush = true;
		}
	}else{    //�Ѿ�����
		startTime = stKeyIter->second;
		if(int(uNow - startTime) >= monsterObj->inter){
			isCanBrush = true;
		}
	}

	if(isCanBrush){
		brushOneMonster(monsterObj);
		m_stMonKeyToTime[key] = uNow;
		monsterObj->num = (--nowNum);

		if(monsterObj->id == m_uBossID){
			showBossWarning();
		}
	}
}

void CFightGuideMapScene::showBossWarning(){
	CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
	CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
	if(m_pBossWarning && m_pBossWarning->getParent()){
		m_pBossWarning->stopWarning();
		m_pBossWarning->removeFromParentAndCleanup(true);
		m_pBossWarning = NULL;
	}

	if(m_pBossWarning == NULL){
		m_pBossWarning = CBossWarning::create(); 
		m_pBossWarning->AddEventListener(this, CBossWarning::BOSS_WARNING_COMPLETE, eventListner_selector(CFightGuideMapScene::handleBossWarningCompleteEvent));
	}
	m_pBossWarning->setTouchEnabled(false);
	m_pBossWarning->setPosition(visibleOrigin + ccp(stSize.width, stSize.height));
	m_pUiLayer->addWidget(m_pBossWarning);

	m_pBossWarning->showWarning();
}

void CFightGuideMapScene::checkBrushSceneItem(){
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

void CFightGuideMapScene::brushSceneItemByTouch(unsigned int touchID){
	//��ǰ�׶ε�item
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
			//������
			addSceneItem(itemObj);
			stIter = tempArr.erase(stIter);
		}else{
			stIter++;
			continue;
		}
	}

	m_stIDXToItems.find(m_uCurStageIdx)->second = tempArr;
}

void CFightGuideMapScene::brushFloorBuffByTouch(unsigned int touchID){
	//��ǰ�׶ε�floorbuff
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
			//���floorbuff
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

void CFightGuideMapScene::brushTrapByTouch(unsigned int touchID){
	//��ǰ�׶ε�����
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
			//�������
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

void CFightGuideMapScene::updateInstnaceByAI(){
	CBaseFightScene::updateInstnaceByAI();
}

void CFightGuideMapScene::handlePassLogic(){
	if(m_bIsGameOver){
		return;
	}

	if(m_uCurStageMonNum <= 0){
		m_uPassType = WIN_CLEAN_ALL;
	}else{
		checkShowGoTip();
		m_uPassType = WIN_NONE;
		return;
	}


	if(m_uCurStageIdx >= m_uTotalStage){
		return;
	}

	//��һ�׶ε���ʾ
	if(m_pnextStageTip->getParent() == NULL){
		nextStageTip();
	}

	//��һ�����������һ�׶�
	if(_mainHero->getPositionX() > walkableRect.getMaxX() - START_BRUSH_DIS){
		nextStage();
	}
}

void CFightGuideMapScene::checkShowGoTip(){
	if(m_uCurStageMonNum <= 0){
		return;
	}

	if(getMonsterNumOnStage() <= 0){  //��̨��û�й���
		if(!m_pnextStageTip->getIsActing()){
			nextStageTip();
		}
	}else{
		if(m_pnextStageTip && m_pnextStageTip->getIsActing()){
			cancelTip();
		}
	}
}

bool CFightGuideMapScene::checkWinLogic(unsigned int cfgID){
	if(m_uWinType == WIN_TYPE_CLEAN_ALL){  //ɱ�����й���
		if(m_uCurStageIdx >= m_uTotalStage && m_uCurStageMonNum <= 0){
			return true;
		}
	}else if(m_uWinType == WIN_TYPE_KILL_ONE){  ////����ĳ��ID
		if(m_uWinValue1 == cfgID){
			return true;
		}
	}

	return false;
}

void CFightGuideMapScene::handleSuccessLogic(){
	handleGameOver();
}

void CFightGuideMapScene::handleGiveUpLogic(CCObject* pstObj){
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, GIVEUP_FIGHT_EVENT);
	handleFailLogic();
}

void CFightGuideMapScene::handleFailLogic(){
	if(m_bIsGameOver){
		return;
	}

	m_uScore = 0;
	m_uScorePercent = 0;
	m_uMark = 0;

	handleGameOver();
}

void CFightGuideMapScene::handleGameOver(){
	CToolUtils::setTimeScale(1.0f);
	m_bIsGameOver = true;
	m_bIsPreOver = true;

	fireBtn->setTouchEnabled(false);
	fireBtn->setFocused(false);
	_mainHero->Idle();
	mainHeroCeaseShoot();
	if(m_uScore != 0){	//ʤ��
		//���δ��ʧ�Ĺ���
		clearAllMonsters();
	}

	//������м����Լ��ӵ�
	clearAllBullet();
	clearAllSkills();

	unscheduleUpdate();
	stopAllActions();
	stopAllMonsters();
	this->unschedule(schedule_selector(CFightGuideMapScene::updateCoinPos));

	//����ǹе
	P_FIGHT_OBJ->restoreRealWeapon();

	//ֱ���˳�
	SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID);

	ADD_EVT_LOG(EVT_LOG_DEMO_END); //��ɹ���������ָ��������ͬ������
}

void CFightGuideMapScene::nextStage(){
	m_uCurStageIdx++;
	m_bPassViewChange = true;
	m_bHasShowStage = false;

	int rectX = walkableRect.getMinX();
	int rectH = walkableRect.size.height;
	int rectW = walkableRect.size.width + FIGHT_STAGE_NUM*_screenW;
	walkableRect.setRect(rectX, 0, rectW, rectH);

	//������һ�غ�����
	m_bStartedBrush = false;
	m_fStartBrushTime = 0;

	//���ر��׶εĹֺ�item
	initMonster();				//��ʼ�����׶εĹ�������
	initSceneItem();			//�������׶ε�item
	initFloorBuff();			//���׶εĳ���buff
	initTrap();					//���׶ε�����
}

void CFightGuideMapScene::nextStageTip(){
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

void CFightGuideMapScene::delPreStage(){
	int rectX = walkableRect.getMinX() + _screenW*FIGHT_STAGE_NUM;
	int rectH = walkableRect.size.height;
	int rectW = walkableRect.size.width - _screenW*FIGHT_STAGE_NUM;
	walkableRect.setRect(rectX, 0, rectW, rectH);

	CSceneItem* item;
	for(unsigned int i = 0; i < _sceneItems->count(); i++){
		item = dynamic_cast<CSceneItem*>(_sceneItems->objectAtIndex(i));

		if(item->getPositionX() <= (rectX - _screenW)){  //ȥ���Ƴ���ͼ�ڵ�item
			updateResToNum(item->getCfgID(), ITEM_TYPE, false);

			_sceneItems->removeObject(item);
			_sceneForegound->removeChild(item, true);
		}
	}
}

void CFightGuideMapScene::cancelTip(){
	if(m_pnextStageTip && m_pnextStageTip->getParent() != NULL){
		m_pnextStageTip->stopAct();
		m_pnextStageTip->removeFromParent();
	}
}

void CFightGuideMapScene::setViewpointCenter(const CCPoint& position){
	CBaseMapScene::setViewpointCenter(position);

	if(walkableRect.size.width > _screenW*FIGHT_STAGE_NUM){		//�������������1���׶Σ���ʾ����ǰһ�׶�
		if(_sceneForegound->getPositionX() <= -(float)(_screenW*FIGHT_STAGE_NUM*(m_uCurStageIdx - 1))){
			delPreStage();	//ɾ��ǰһ�׶�
		}
	}
}

void CFightGuideMapScene::updateBgViewCenter(float foreX){
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

	if(walkableRect.size.width > _screenW*FIGHT_STAGE_NUM 
		&& _mainHero->getPositionX() > _screenW*FIGHT_STAGE_NUM*(m_uCurStageIdx - 1)){		//�������������1���׶Σ���ʾ����ǰһ�׶�
			cancelTip();
	}
}

void CFightGuideMapScene::passStageViewChange(float dt){
	if(_mainHero->getIsTheOne()){
		return;
	}

	CCPoint pos = _mainHero->getPosition();
	float visibleW = CCDirector::sharedDirector()->getVisibleSize().width;
	if(_sceneForegound->getPositionX() <= visibleW/2 - pos.x){		//��Ļ����Ϊ��һ�׶ο�ʼ
		m_bPassViewChange = false;
		return;
	}

	float disX = pos.x - visibleW/2 + _sceneForegound->getPositionX();
	float moveDis = dt*(_mainHero->getWalkSpeed()*1.5f);
	if(moveDis >= disX){
		moveDis = disX;
	}
	float x = _sceneForegound->getPositionX() - moveDis;
	if(x <= -(walkableRect.getMaxX() - visibleW)){	//���ܳ�����Ļ
		m_bPassViewChange = false;
		x = -(walkableRect.getMaxX() - visibleW);
	}

	CCPoint fgViewPoint = ccp(x, 0);
	_sceneForegound->setPosition(fgViewPoint);

	updateBgViewCenter(fgViewPoint.x);
}

CCRect CFightGuideMapScene::getStageRect() const{
	int rectH = walkableRect.size.height;
	int rectW = FIGHT_STAGE_NUM*_screenW;
	int rectX = (m_uCurStageIdx - 1)*rectW;

	return CCRectMake(rectX, 0, rectW, rectH);
}

void CFightGuideMapScene::updateTimeByKey(unsigned int key){
	map<unsigned int, int>::iterator stIter = m_stMonKeyToTime.find(key);	
	unsigned int uNow = GlobalTime::getTime();
	m_stMonKeyToTime[key] = uNow;
}

/************************************************************************/
/*                         �¼�����                                     */
/************************************************************************/

void CFightGuideMapScene::handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj){

}

void CFightGuideMapScene::handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj){
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);

	//�������
	if(m_bIsGameOver){
		return;
	}

	handleFailLogic();
}

void CFightGuideMapScene::handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj){
	CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstEventObj);

	if(pstEntity){
		pstEntity->getBlood()->setPercentage(100.0*pstEntity->getHP()/pstEntity->getMaxHP());
	}
}

void CFightGuideMapScene::handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj){
	//�������˺� 
	CCString* strValue = (CCString*)pstEventObj;
	int harmValue = strValue->intValue();
	m_uTotalHarmValue += harmValue;
}

void CFightGuideMapScene::handleEnemyDieMovementEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	MonsterCfg* stMonsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(monster->getMonsterObj()->id);

	if(stMonsterCfg->floorbuffid() != 0){
		addFloorBuff(stMonsterCfg->floorbuffid(), monster->getRoleParty(), monster->GetFootPos());
	}

	//��ҵ��䣬Ʈ���
	addCoinEff(monster, stMonsterCfg->dropcoin());

	//�������������ݱ仯
	handleHeroKillValueChange(stEvent, pstEventObj);

	//���ͨ������
	if(!m_bIsPreSuccess && checkWinLogic(monster->getMonsterObj()->id)){  //�Ѿ�ʤ��,���ǲ��ɱ�����
		m_bIsPreSuccess = true;
		m_bIsPreOver = true;
		m_uLastKillID = monster->m_uID;		//��¼ʤ��ʱ��ɱ���Ĺ���id
		mainHeroCeaseShoot();
		_mainHero->ForceIdle();
		_mainHero->setIsInvincible(true);
		_mainHero->winAct(true);
		clearAllSkills();
		clearAllBullet();
		clearAllArrows();
		forceOhterMonstersDie();			//�������ﶼ��
		setAttackOpBtnVisible(false);		//���ؼ��ܹ�����ť
		CToolUtils::setTimeScale(0.5f);    //����1��

		//����ʤ����Ч
		CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
		CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
		CCArmature* pWinEff = CCArmature::create("ut00036");
		AudioManager::playEffect("MissionComplete.ogg");		//��Ч
		m_pUiLayer->addChild(pWinEff, 2014);
		pWinEff->setPosition(visibleOrigin + ccp(stSize.width/2, stSize.height/2));
		pWinEff->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(CFightGuideMapScene::handleWinEffComplete));
		pWinEff->getAnimation()->play("fly", -1, -1, 0);
	}
}

void CFightGuideMapScene::handleWinEffComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);

		if(m_bIsWinComplete){  //�����������Ƚϣ�˭���꣬�ͽ���
			handleSuccessLogic();
		}else{
			m_bIsWinComplete = true;
		}
	}	
}

void CFightGuideMapScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	MonsterCfg* stMonsterCfg = MONSTER_CFG_MASTER->GetMonsterCfgByID(monster->getMonsterObj()->id);

	//���Ѿ��������������������������
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	unsigned int mID = pstEntity->m_uID;
	if(pstEntity->getMyState() == kActionStateDie){  
		updateResToNum(pstEntity->getMonsterObj()->id, MONSTER_TYPE, false);

		if(pstEntity->getIsBlock()){  //ȥ���赲
			removeBlocksInSize(pstEntity->getBlockSize(), (CCPoint)pstEntity->getPosition(), true);
			updateMapBlocks();
		}

		_sceneForegound->removeChild(pstEntity, true);
		if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
			_stageEntities->removeObject(pstEntity);
		}
	}

	//���ͨ������
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

void CFightGuideMapScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);

	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	if(pstEntity){
		CEnemyData* pstEnemyData = pstEntity->getMonsterObj();

		if(pstEnemyData->type != APPEAR_AFTER_DIE){
			--m_uCurStageMonNum;
		}else{
			unsigned int key = pstEnemyData->key;
			m_stDieMonsterData.push_back(key);
			updateTimeByKey(key);
		}

		CCLOG("=====the monster be killed, and the number of last monster is ==== %d", m_uCurStageMonNum);

		//boss������Ѫ����ʧ
		if(pstEntity->getRoleType() == ROLE_TYPE_SENIOR_BOSS || pstEntity->getRoleType() == ROLE_TYPE_JUNIOR_BOSS){
			if(m_pBossHPUI->isVisible()){
				m_pBossHPUI->setVisible(false);
			}

			if(m_pBossWarning && m_pBossWarning->getParent()){  //boss����ȥ��
				m_pBossWarning->RemoveEventListener(this, CBossWarning::BOSS_WARNING_COMPLETE, eventListner_selector(CFightGuideMapScene::handleBossWarningCompleteEvent));
				m_pBossWarning->stopWarning();
				m_pBossWarning->removeFromParentAndCleanup(true);
				m_pBossWarning = NULL;
			}

			//ȥ��������Ч
			CCArmature* pHeroHurtArm = dynamic_cast<CCArmature*>(m_pUiLayer->getChildByTag(CBaseFightScene::HURT_ARM_TAG));
			if(pHeroHurtArm != NULL){
				pHeroHurtArm->stopAllActions();
				m_pUiLayer->removeChildByTag(CBaseFightScene::HURT_ARM_TAG);
			}
		}
	}
}

void CFightGuideMapScene::handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleSceneItemFallEvent(stEvent, pstEventObj);
	CSceneItem* item = dynamic_cast<CSceneItem*>(pstEventObj);

	if(item && item->getItemState() == DIE_STATE){
		CItemObj* stInfo = item->getItemObj();
		CCPoint stPos = item->getPosition();
		//����ĵ���
		if(stInfo->dropId != 0){
			if(stInfo->dropType == DROP_TYPE_BUFF){  //�ر�buff
				//���������Լ�
				addFloorBuff(stInfo->dropId, stInfo->dropParty, stPos);
			}else if(stInfo->dropType == DROP_TYPE_WEAPON){
				addWeaponBuff(stInfo->dropId, stInfo->dropParty, stPos);
			}
		}

		if(item->getItemType() == ITEM_TYPE_BLOCK_DESTROY || item->getItemType() == ITEM_TYPE_BLOCK_UNDESTROY){  //�赲
			removeBlocksInSize(item->getBlockSize(), (CCPoint)item->getPosition(), true);
		}

		updateMapBlocks();
		if(_sceneItems->indexOfObject(item) != UINT_MAX){
			_sceneItems->removeObject(item);
		}

		item->removeFromParentAndCleanup(true);
	}
}

void CFightGuideMapScene::handleSceneItemDisappearEvent(const string& stEvent, CCObject* pstEventObj){
	CSceneItem* item = dynamic_cast<CSceneItem*>(pstEventObj);

	if(item->getItemType() == ITEM_TYPE_BLOCK_DESTROY || item->getItemType() == ITEM_TYPE_BLOCK_UNDESTROY){  //�赲
		removeBlocksInSize(item->getBlockSize(), (CCPoint)item->getPosition(), true);
	}

	updateMapBlocks();
	if(_sceneItems->indexOfObject(item) != UINT_MAX){
		_sceneItems->removeObject(item);
	}

	item->removeFromParentAndCleanup(true);
}

void CFightGuideMapScene::handleBossWarningCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	m_pBossWarning->removeAllChildrenWithCleanup(true);
	m_pBossWarning = NULL;
}

void CFightGuideMapScene::btnTouchEvent(CCObject *pSender, TouchEventType type){
	CBaseFightScene::btnTouchEvent(pSender, type);
}

void CFightGuideMapScene::setAttackOpBtnVisible(bool bIsVisible){
	m_pSkillLayout->setVisible(bIsVisible);
	fireBtn->setTouchEnabled(bIsVisible);
	m_trickBtn->setBtnEnable(bIsVisible);
	m_skillBtn->setBtnEnable(bIsVisible);
	m_dodgeBtn->setBtnEnable(bIsVisible);
	pauseBtn->setEnabled(bIsVisible);

	if(m_pChangeWeaponBtn->isEnabled()){
		m_pChangeWeaponBtn->setTouchEnabled(bIsVisible);
	}

	if(bIsVisible){
		_hudLayer->Active();
	}else{
		_hudLayer->Inactive();
	}
	_hudLayer->setVisible(bIsVisible);

	if(_mainHero->getIsBulletLimit()){
		m_pArrLayout->setEnabled(bIsVisible);
	}
}


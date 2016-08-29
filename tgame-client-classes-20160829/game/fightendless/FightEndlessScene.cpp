#include "FightEndlessScene.h"
#include "fight/FightBossHeadUI.h"
#include "fight/FightHeroHeadUI.h"
#include "UserObj.h"
#include "ToolUtils.h"
#include "vlib/CVRand.h"
#include "FightEndlessControl.h"
#include "GlobalCfgMaster.h"
#include "Multiresolution.h"
#include "fight/FightObj.h"
#include "instance/InstanceUtils.h"
#include "instance/AnimationUtils.h"

#include "HeroCfgMaster.h"
#include "EndlessCfgMaster.h"
#include "MapCfgMaster.h"
#include "MonsterCfgMaster.h"
#include "MonsterLevCfgMaster.h"
#include "SkillCfgMaster.h"
#include "GlobalTime.h"

CFightEndlessScene::CFightEndlessScene(){

}

CFightEndlessScene::~CFightEndlessScene(){
    CC_SAFE_RELEASE_NULL(m_pPauseLayer);

	CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

bool CFightEndlessScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CFightEndlessScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	beginTip();
}

void CFightEndlessScene::setWalkHight(){
	//从关卡地图中随机
	const MapCfgSet& cfgSet = MAP_CFG_MASTER->GetCfg();
	unsigned int idx = CRandom::GetRand(cfgSet.mapcfg_size());

	m_stMapCfg = cfgSet.mapcfg(idx);
	m_uWalkHight = m_stMapCfg.midwalkh();
}

void CFightEndlessScene::setMapRes(){
	m_stFrontStr = m_stMapCfg.frontid();
	m_stForeStr = m_stMapCfg.foreid();
	m_stMidStr = m_stMapCfg.midid(); 
	m_stBackStr = m_stMapCfg.backid(); 
}

void CFightEndlessScene::loadMapBgRes(){
	m_uCurStageIdx = 1;
	m_uCurMonsterLev = 1;

	CBaseMapScene::loadMapBgRes();
}

void CFightEndlessScene::loadItemRes(){
	CBaseMapScene::loadItemRes();
}

void CFightEndlessScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();
}

void CFightEndlessScene::loadMonsterRes(){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
	int idx = (m_uCurStageIdx - 1)%endlessCfg.roundinfo_size();
	MonsterShowCfg* monShowCfg;
	MonsterShowCfg* extraShowCfg;
	string monStr = "";
	string extraStr = "";

	if(endlessCfg.roundinfo(idx).num() > 0){
		monShowCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgRand(ROLE_TYPE_MONSTER);
		monStr = monShowCfg->avatar();

		CCLOG("the normal monster type is %d", monShowCfg->monstertype());
	}

	if(endlessCfg.roundinfo(idx).extranum() > 0){
		extraShowCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgRand(endlessCfg.roundinfo(idx).extratype());
		extraStr = extraShowCfg->avatar();

		CCLOG("the extra monster type is %d", extraShowCfg->monstertype());
	}

	m_stCurAvatar = make_pair(monStr, extraStr);

	CCLOG("the stage idx is ==== %d, the normalStr is %s, the extralStr is %s", m_uCurStageIdx, monStr.c_str(), extraStr.c_str());

	MonsterCfg monCfg;
	genMonsterCfg(monStr, monCfg);
	unsigned int num = endlessCfg.roundinfo(idx).num();
	loadMonsterResByCfg(&monCfg, num);

	if(strcmp(extraStr.c_str(), "") != 0){
		MonsterCfg extraCfg;
		genMonsterCfg(extraStr, extraCfg);

		num = endlessCfg.roundinfo(idx).extranum();
		loadMonsterResByCfg(&extraCfg, num);
	}
}

void CFightEndlessScene::initMap(){
	CBaseFightScene::initMap();
	walkableRect = CCRect(0, 0, _screenW*2, m_uWalkHight);

	//刷怪点信息
	m_stBrushPoint.clear();

	CCObject* pstObj;
	CEnemyData* data;
	float yy;
	CCARRAY_FOREACH(_monsterData, pstObj){
		data = dynamic_cast<CEnemyData*>(pstObj);
		yy = _mapH - data->y;
		if(yy > m_uWalkHight){
			yy = m_uWalkHight - 10;
		}
		m_stBrushPoint.push_back(ccp(data->x, yy));   //y轴需要使用相对下方的位置
	}
}

void CFightEndlessScene::initFightData(){
	CBaseFightScene::initFightData();

	m_bHasTips = false;
	m_bIsGameOver = false;
	m_uCurStageIdx = 1;
	m_uCurMonsterLev = 1;
	m_uHasBrushNum = 0;
	m_uCurLiveMonNum = 0;
	m_uCurStageMonNum = 0;
	m_uCurTotalNum = 0;
	m_fStartBrushTime = 0;
	m_uCurLimitTime = 0;
	m_uPackageCnt = 0;
	m_uCoin = 0;
	m_fScoreTime = 0;

	m_uCurStartTime = GlobalTime::getTime();
	m_stTypeToMonNum.clear();
	
	for(int i = 0; i < m_stMonsterCnt.GetCount(); ++i) {
		m_stMonsterCnt.AddOneItem(0);
	}

	m_uNum = 0;

	m_bIsInBrush = false;
	m_uBrushNum = 0;
	m_uNowHasBrushNum = 0;
	m_iFrameCnt = 0;
	m_iCheckInterval = 0;
}

void CFightEndlessScene::initSceneItem(){
	CItemObj* itemObj;
	for(unsigned int i = 0; i< _itemData->count(); i++){
		itemObj = (CItemObj*)_itemData->objectAtIndex(i);
		//添加物件
		CSceneItem* item = CSceneItem::create();
		item->setCfgID(itemObj->id);
		item->setItemObj(itemObj);
		item->loadRes();
		item->setPosition(ccp(itemObj->x, _sceneForegound->getContentSize().height - itemObj->y));
		item->AddEventListener(this, CSceneItem::SCENE_ITEM_FALLING_EVENT, eventListner_selector(CFightEndlessScene::handleSceneItemFallEvent));
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, item->getPositionY());
		_sceneForegound->addChild(item, iNewZorder);

		if(item->getItemType() == ITEM_TYPE_BLOCK_DESTROY || item->getItemType() == ITEM_TYPE_BLOCK_UNDESTROY){  //阻挡
			getBlocksInSize(item->getBlockSize(), (CCPoint)item->getPosition(), true);
		}

		_sceneItems->addObject(item);
	}

	updateMapBlocks();
}

void CFightEndlessScene::initMapUI(){
	CBaseFightScene::initMapUI();

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	m_pPauseLayer = CPauseLayer::create();
	m_pPauseLayer->retain();
	m_pPauseLayer->AddEventListener(this, RESUME_GAME, eventListner_selector(CFightEndlessScene::startEndless));

	m_pScoreUI = CFightEndlessScoreUI::create();
	m_pScoreUI->setPosition(ccp(visibleSize.width - m_pScoreUI->getSize().width - 20, pauseBtn->getPositionY() - m_pScoreUI->getSize().height - 20));
	m_pUiLayer->addWidget(m_pScoreUI);

	m_pWaveUI = CFightEndlessWaveUI::create();
	m_pWaveUI->setPosition(ccp((visibleSize.width - m_pWaveUI->getSize().width)/2, m_pBossHPUI->getPositionY() - m_pWaveUI->getSize().height + 50));
	m_pUiLayer->addWidget(m_pWaveUI);
	m_pWaveUI->setWave(m_uCurStageIdx);
}

void CFightEndlessScene::initFriend(){

}

void CFightEndlessScene::initMonster(){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();

	//当前阶段怪物数量
	int idx = (m_uCurStageIdx - 1)%endlessCfg.roundinfo_size();
	m_uCurStageMonNum += endlessCfg.roundinfo(idx).num() + endlessCfg.roundinfo(idx).extranum();
	m_uCurTotalNum = endlessCfg.roundinfo(idx).num() + endlessCfg.roundinfo(idx).extranum();
	m_uCurLimitTime = endlessCfg.roundinfo(idx).time();

	prepareNextStage();
}

void CFightEndlessScene::update(float delta){
	CBaseFightScene::update(delta);

	setViewpointCenter(_mainHero->getPosition());

	updateMonsterNum(delta);
}

void CFightEndlessScene::updateMonsterNum(float dt){
	m_fScoreTime += dt;
	if(dt >= 60.0f){
		m_uCurStartTime = 0;

		updateScore();
	}

	unsigned int nowNum = 0;

	CCObject* pstObj;
	CQStageEntity* pstEntity;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){
			nowNum++;
		}
	}

	if(m_uCurLiveMonNum != nowNum){
		m_uCurLiveMonNum = nowNum;
		//m_pScoreUI->setNum(m_uCurLiveMonNum, END_LESS_CFG->GetCfg().limitnum());
	}

	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
	if(m_uCurLiveMonNum >= endlessCfg.limitnum()){  //超出无尽模式限制怪物数量，结束
		m_bIsGameOver = true;
		P_FIGHT_OBJ->setIsForceQuit(false);
		handleGameOver();
	}
}

void CFightEndlessScene::updateScore(){
	//当前积分更新
	m_uCurScore = m_fFightTime*5;
	for(int i = 0; i < m_stMonsterCnt.Size(); i++){
		m_uCurScore += m_stMonsterCnt[i]*stMonsterMark[i];
	}

	m_uCurScore = m_uCurScore*(1 + 1.0*P_ENDLESS_CTRL->getScoreAdd()/10000);

	m_pScoreUI->setScore(m_uCurScore);
}

void CFightEndlessScene::brushMonsterLogic(float dt){
	if(!m_bStartBrush || m_uHasBrushNum >= m_uCurTotalNum){
		return;
	}

	if(m_fStartBrushTime == 0 && m_bIsInBrush == false){
		const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
		int idx = (m_uCurStageIdx - 1)%endlessCfg.roundinfo_size();
		m_uBrushNum =  m_uCurTotalNum - m_uHasBrushNum;

		if(m_uBrushNum >= m_stBrushPoint.size()){  
			m_uBrushNum = m_stBrushPoint.size();
		}
		m_bIsInBrush = true;
		m_uNowHasBrushNum = 0;
		m_iCheckInterval = random_range(10, 30);
	}

	if(m_bIsInBrush){
		m_iFrameCnt++;
		if(m_iFrameCnt >= m_iCheckInterval){
			brushMonster();

			if(m_uNowHasBrushNum >= m_uBrushNum - 1){
				m_bIsInBrush = false;
			}else{
				m_uNowHasBrushNum++;
			}

			m_iFrameCnt = 0;
		}
	}

	if(m_bIsInBrush == false){
		m_fStartBrushTime += dt;
		if(m_fStartBrushTime >= INTER){
			m_fStartBrushTime = 0;
		}

		if(checkMonsterNumOnStage() <= 0){
			m_fStartBrushTime = 0;
		}
	}
}

void CFightEndlessScene::brushMonster(){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
	int idx = (m_uCurStageIdx - 1)%endlessCfg.roundinfo_size();

	CEnemyData* pstMonsterObj;
	CEnemy* pstMonster;
	m_uNum++;
	m_uHasBrushNum++;

	pstMonsterObj = CEnemyData::create();
	string avatar = "";
	if(m_uHasBrushNum > endlessCfg.roundinfo(idx).num()){  //开始刷额外怪
		if(endlessCfg.roundinfo(idx).extranum() <= 0){
			return;
		}
		avatar = m_stCurAvatar.second;
		pstMonsterObj->monsterType = endlessCfg.roundinfo(idx).extratype();

		if(idx == 4){
			CCLOG("the extra brush type is %d", endlessCfg.roundinfo(idx).extratype());
		}

		if(endlessCfg.roundinfo(idx).extratype() == ROLE_TYPE_SENIOR_BOSS){  //大boss随机掉落
			pstMonsterObj->dropCnt = rollPackage(endlessCfg.roundinfo(idx).extraprob());
		}

	}else{
		avatar = m_stCurAvatar.first;
		pstMonsterObj->monsterType = ROLE_TYPE_MONSTER;
	}
	MonsterCfg cfg;
	genMonsterCfg(m_uCurMonsterLev, avatar, cfg);
	MonsterCfg* pstCfg = &cfg;
	pstMonsterObj->x = m_stBrushPoint.at(m_uNowHasBrushNum).x;
	pstMonsterObj->y = _sceneForegound->getContentSize().height - m_stBrushPoint.at(m_uNowHasBrushNum).y;
	pstMonster = initOneMonster(pstMonsterObj, pstCfg);
	pstMonster->setIsBubbleOn(false);
	pstMonster->LoadActionRes();
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
	_sceneForegound->addChild(pstMonster, iNewZorder);
}

void CFightEndlessScene::handlePassLogic(){
	if(m_bIsGameOver){
		return;
	}

	int uNow = GlobalTime::getTime();

	if(m_uCurStageMonNum <= 0){  //杀死所有怪
		if(!m_bHasTips){
			nextStageTip();
		}

		CCLOG("no monster, so brush");
		nextStage();
		return;
	}

	//距离本阶段限制时间还有10s
	if((uNow - m_uCurStartTime) >= m_uCurLimitTime - 10.0f && !m_bHasTips){
		nextStageTip();
	}

	//超过一个阶段的限制时间
	if((unsigned int)(uNow - m_uCurStartTime) >= m_uCurLimitTime){  
		CCLOG("time is up, start next wave");
		nextStage();
		return;
	}
}

void CFightEndlessScene::handleGiveUpLogic(CCObject* pstObj){
	P_FIGHT_OBJ->setIsForceQuit(true);

	handleGameOver();

	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, GIVEUP_FIGHT_EVENT);
}

void CFightEndlessScene::handleGameOver(){
	this->unscheduleUpdate();

	//评分
	m_uMark = (m_fFightTime)*5;
	for(int i = 0; i < m_stMonsterCnt.Size(); i++){
		m_uMark += m_stMonsterCnt[i]*stMonsterMark[i];
	}

	m_uCoin = (m_uCurStageIdx - 1)*(GLOBAL_CFG_MASTER->GetCfg().wavecoin());

	m_uMark = m_uMark*(1 + 1.0*P_ENDLESS_CTRL->getScoreAdd()/10000);
	m_uCoin = m_uCoin*(1 + 1.0*P_ENDLESS_CTRL->getCoinAdd()/10000);

	P_ENDLESS_CTRL->setScore(m_uMark);
	P_ENDLESS_CTRL->setCoin(m_uCoin);

	P_ENDLESS_CTRL->sendEndlessEndRqst(m_uMark, m_uPackageCnt, m_uCurStageIdx, m_uCoin, m_stMonsterCnt, m_uEvolutionCnt);
}

void CFightEndlessScene::nextStage(){
	m_uCurStageIdx++;

	m_pWaveUI->setWave(m_uCurStageIdx);

	unsigned int nowCoin = (m_uCurStageIdx - 1)*(GLOBAL_CFG_MASTER->GetCfg().wavecoin());
	nowCoin = nowCoin*(1 + 1.0*P_ENDLESS_CTRL->getCoinAdd()/10000);
	m_pScoreUI->setCoin(nowCoin);

	m_uCurMonsterLev = (m_uCurStageIdx - 1)/(END_LESS_CFG->GetCfg().addlev()) + 1;

	CCLOG("fightEndless now wave == %u, and the monster level == %u", m_uCurStageIdx, m_uCurMonsterLev);

	//加载下一关后重置
	m_uHasBrushNum = 0;
	m_fStartBrushTime = 0;
	m_uCurStartTime = GlobalTime::getTime();
	m_bHasTips = false;

	m_stCurAvatar = m_stNextAvatar;

	initMonster();	
}

void CFightEndlessScene::updateBgViewCenter(float foreX){
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

void CFightEndlessScene::beginTip(){
	m_pPauseLayer->startCountdown();
	addChild(m_pPauseLayer);
}

void CFightEndlessScene::startEndless(const string& stEvent, CCObject* pstEventObj){
	this->scheduleUpdate();

	m_bStartBrush = true;
	m_uCurStartTime = GlobalTime::getTime();
	m_uHasBrushNum = 0;
	m_fStartBrushTime = 0;
}

void CFightEndlessScene::nextStageTip(){
	m_bHasTips = true;

	m_pWaveUI->startTip();
}

void CFightEndlessScene::prepareNextStage(){
	const EndlessCfg& endlessCfg = END_LESS_CFG->GetCfg();
	int idx = (m_uCurStageIdx)%endlessCfg.roundinfo_size();
	MonsterShowCfg* monShowCfg;
	MonsterShowCfg* extraShowCfg;
	string monStr = "";
	string extraStr = "";

	if(endlessCfg.roundinfo(idx).num() > 0){
		monShowCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgRand(ROLE_TYPE_MONSTER);
		monStr = monShowCfg->avatar();

	}

	if(endlessCfg.roundinfo(idx).extranum() > 0){
		extraShowCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgRand(endlessCfg.roundinfo(idx).extratype());
		extraStr = extraShowCfg->avatar();

		CCLOG("the stage is %d, the extra monster type is %d", m_uCurStageIdx + 1, endlessCfg.roundinfo(idx).extratype());
	}

	m_stNextAvatar = make_pair(monStr, extraStr);

	CCLOG("the preLoad stage idx is ==== %d, the normalStr is %s, the extralStr is %s", m_uCurStageIdx, monStr.c_str(), extraStr.c_str());

	MonsterCfg monCfg;
	genMonsterCfg(monStr, monCfg);
	unsigned int num = endlessCfg.roundinfo(idx).num();
	loadMonsterResByCfg(&monCfg, num, false);

	if(strcmp(extraStr.c_str(), "") != 0){
		MonsterCfg extraCfg;
		genMonsterCfg(extraStr, extraCfg);

		num = endlessCfg.roundinfo(idx).extranum();
		loadMonsterResByCfg(&extraCfg, num, false);
	}
}

void CFightEndlessScene::genMonsterCfg(int level, string avatar, MonsterCfg& cfg){
	MonsterLevCfg* levCfg = MONSTER_LEV_CFG->GetMonsterLevCfgByLev(level);
	MonsterShowCfg* showCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgByShowID(avatar);

	cfg.set_id(0);
	cfg.set_monstername(showCfg->monstername());
	cfg.set_desc(showCfg->desc());
	cfg.set_avatar(avatar);
	cfg.set_camp(ROLE_PARTY_HOSTILE);
	cfg.set_monstertype(showCfg->monstertype());
	cfg.set_level(level);

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

void CFightEndlessScene::genMonsterCfg(string avatar, MonsterCfg& cfg){
	MonsterShowCfg* showCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgByShowID(avatar);

	cfg.set_id(0);
	cfg.set_monstername(showCfg->monstername());
	cfg.set_desc(showCfg->desc());
	cfg.set_avatar(avatar);
	cfg.set_camp(ROLE_PARTY_HOSTILE);
	cfg.set_monstertype(showCfg->monstertype());
	cfg.set_level(1);

	unsigned int attIdx = showCfg->monstertype() - 1; 
	cfg.set_hp(1);
	cfg.set_attack(1);
	cfg.set_defence(1);
	cfg.set_atkatt(1);

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

	cfg.set_walktype(showCfg->walktype());
	cfg.set_hurtactioncd(showCfg->hurtactioncd());
}

int CFightEndlessScene::rollPackage(unsigned int prob){
	unsigned int roll = CRandom::GetRandHaveMax(10000);

	if(roll <= prob){
		return 1;
	}

	return 0;
}

void CFightEndlessScene::cleanup(){

}

int CFightEndlessScene::checkMonsterNumOnStage(){
	int iNum = 0;
	CCObject* pstObj;
	CCARRAY_FOREACH(_stageEntities, pstObj){
		CQStageEntity* pstEntity = dynamic_cast<CQStageEntity*>(pstObj);
		if(pstEntity->getRoleParty() == ROLE_PARTY_HOSTILE){  //敌方
			iNum++;
		}
	}

	return iNum;
}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CFightEndlessScene::handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj){
	//猪脚死亡
	m_bIsGameOver = true;

	P_FIGHT_OBJ->setIsForceQuit(false);

	handleGameOver();
}

void CFightEndlessScene::handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj){
	//计入总伤害 
	CCString* strValue = (CCString*)pstEventObj;
	int harmValue = strValue->intValue();
	m_uTotalHarmValue += harmValue;
}

void CFightEndlessScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);
	if(monster->getMonsterObj()->dropCnt != 0){
		//掉落统计
		if(monster->getRoleType() == ROLE_TYPE_SENIOR_BOSS && monster->getMonsterObj()->dropCnt != 0){  //boss
			m_uPackageCnt++;
			m_pScoreUI->setPackage(m_uPackageCnt);
		}

		//unsigned int itemID = DROP_CFG_MASTER->GetDropCfgByID(dropID)->elementlst().elementid();	//读配置
		//FallingItem::fall(itemID, this->_mainHero, dynamic_cast<CQStageEntity*>(pstEventObj)->getPosition(), callfuncO_selector(CMainHero::pickupItem));
	}

	handleHeroKillValueChange(stEvent, pstEventObj);
}

void CFightEndlessScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);
	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);
	int type = pstEntity->getMonsterObj()->monsterType;
	map<int, int>::iterator stIter = m_stTypeToMonNum.find(type);

	if(stIter == m_stTypeToMonNum.end()){
		m_stTypeToMonNum[type] = 1;
	}else{
		int preNum = stIter->second;
		preNum++;
		m_stTypeToMonNum[type] = preNum;
	}

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
		CCLOG("the boss is died hahahahhah");
		/*if(m_pBossHPUI->getParent()){
			m_pBossHPUI->removeFromParent();
		}*/
			

	}else{
			
	}

	//把已经播放完毕死亡动画的死人做掉
	if(pstEntity->getMyState() == kActionStateDie){  
		//updateResToNum(pstEntity->getMonsterObj()->id, MONSTER_TYPE, false);

		_sceneForegound->removeChild(pstEntity, true);
		if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
			_stageEntities->removeObject(pstEntity);
		}
	}
}

void CFightEndlessScene::handleSceneItemFallEvent(const string& stEvent, CCObject* pstEventObj){
	CSceneItem* item = dynamic_cast<CSceneItem*>(pstEventObj);

	//unsigned int itemID = DROP_CFG_MASTER->GetDropCfgByID(dropID)->elementlst().elementid();	//读配置
	//FallingItem::fall(itemID, this->_mainHero, dynamic_cast<CSceneItem*>(pstEventObj)->getPosition(), callfuncO_selector(CMainHero::pickupItem));
}
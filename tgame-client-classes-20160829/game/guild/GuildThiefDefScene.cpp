#include "GuildThiefDefScene.h"
#include "UserObj.h"

#include "HeroCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "MonsterShowCfgMaster.h"
#include "GuildActCfgMaster.h"
#include "GuildActHpCfgMaster.h"
#include "GlobalTime.h"
CGuildThiefDefScene::CGuildThiefDefScene(){

}

CGuildThiefDefScene::~CGuildThiefDefScene(){

}

bool CGuildThiefDefScene::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!CBaseFightScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void CGuildThiefDefScene::onEnterTransitionDidFinish(){
	CBaseFightScene::onEnterTransitionDidFinish();

	m_pPauseLayer->startCountdown();
	addChild(m_pPauseLayer);
}

void CGuildThiefDefScene::loadMapBgRes(){
	CBaseMapScene::loadMapBgRes();
}

void CGuildThiefDefScene::loadItemRes(){
	
}

void CGuildThiefDefScene::loadHeroRes(){
	CBaseFightScene::loadHeroRes();
}

void CGuildThiefDefScene::loadMonsterRes(){
	string avatar = GUILD_ACT_CFG->GetGuildActCfgByID(1)->avatar();
	genMonsterCfg(P_USER_OBJ->getUserInfo()->level, avatar, m_stMonsterCfg);

	loadMonsterResByCfg(&m_stMonsterCfg, 1000);
}

void CGuildThiefDefScene::initMap(){
	CBaseFightScene::initMap();
	walkableRect = CCRect(0, 0, _screenW, m_uWalkHight);
}

void CGuildThiefDefScene::initFightData(){
	CBaseFightScene::initFightData();
}

void CGuildThiefDefScene::initSceneItem(){
	updateMapBlocks();
}

void CGuildThiefDefScene::initMapUI(){
	CBaseFightScene::initMapUI();

	m_pPauseLayer = CPauseLayer::create();
	m_pPauseLayer->retain();
	m_pPauseLayer->AddEventListener(this, RESUME_GAME, eventListner_selector(CGuildThiefDefScene::startFight));
}

void CGuildThiefDefScene::startFight(const string& stEvent, CCObject* pstEventObj){
	this->scheduleUpdate();

	m_stMonKeyToTime.clear();
	m_stMonKeyToBool.clear();
	CEnemyData* pstMonsterObj;

	for(unsigned int i = 0; i < _monsterData->count(); i++){
		pstMonsterObj = (CEnemyData*)_monsterData->objectAtIndex(i);

		unsigned int key = makeKey(pstMonsterObj->x, pstMonsterObj->y);
		m_stMonKeyToBool[key] = false;
		m_stMonKeyToTime[key] = 0;
	}
}

void CGuildThiefDefScene::initFriend(){

}

void CGuildThiefDefScene::initMonster(){
	CEnemyData* pstMonsterObj;
	CEnemy* pstMonster;
	MonsterCfg* pstCfg = &m_stMonsterCfg;

	for(unsigned int i = 0; i < _monsterData->count(); i++){
		pstMonsterObj = (CEnemyData*)_monsterData->objectAtIndex(i);
		pstMonster = initOneMonster(pstMonsterObj, pstCfg);
		pstMonster->LoadActionRes();
		unsigned int iNewZorder = 200 + 800*(_winSize.height - pstMonster->getPositionY())/_winSize.height;
		_sceneForegound->addChild(pstMonster, iNewZorder);
	}
}

void CGuildThiefDefScene::update(float delta){
	CBaseFightScene::update(delta);

	setViewpointCenter(_mainHero->getPosition());
}

void CGuildThiefDefScene::updateInstnaceByAI(){

}

void CGuildThiefDefScene::brushMonsterLogic(float dt){
	CEnemyData* pstMonsterObj;
	map<unsigned int, bool>::iterator stIter;
	for(unsigned int i = 0; i < _monsterData->count(); i++){
		pstMonsterObj = (CEnemyData*)_monsterData->objectAtIndex(i);
		unsigned int key = makeKey(pstMonsterObj->x, pstMonsterObj->y);
		stIter = m_stMonKeyToBool.find(key);

		if(stIter == m_stMonKeyToBool.end()){
			m_stMonKeyToBool[key] = false;
		}else{
			if(stIter->second){
				unsigned int startTime = m_stMonKeyToTime[key];
				unsigned int uNow = GlobalTime::getTime();
				if((uNow - startTime) >= (unsigned int)pstMonsterObj->inter){
					MonsterCfg* pstCfg = &m_stMonsterCfg;
					CEnemy* pstMonster = initOneMonster(pstMonsterObj, pstCfg);
					pstMonster->LoadActionRes();
					unsigned int iNewZorder = 200 + 800*(_winSize.height - pstMonster->getPositionY())/_winSize.height;
					_sceneForegound->addChild(pstMonster, iNewZorder);

					m_stMonKeyToBool[key] = false;
					m_stMonKeyToTime[key] = uNow;
				}
			}
		}
	}
}

void CGuildThiefDefScene::handlePassLogic(){
	//只有超过时间才结束

	unsigned int uNow = GlobalTime::getTime();
	if((unsigned int)(m_fFightTime) > GUILD_ACT_CFG->GetGuildActCfgByID(1)->time()){ 
		CCLOG("the thief act time is out!!!!!!!!");
		return;
	}
}

void CGuildThiefDefScene::handleGiveUpLogic(CCObject* pstObj){

}

void CGuildThiefDefScene::updateBgViewCenter(float foreX){
	float bgViewX = foreX * BG_RATIO;
	_sceneBackground->setPositionX(bgViewX);

	float fgViewX = foreX * FG_RATIO;
	_sceneFrontgound->setPositionX(fgViewX);

	if(_sceneMidground){
		float midViewX = foreX * MID_RATIO;
		_sceneMidground->setPositionX(midViewX);
	}
}

void CGuildThiefDefScene::genMonsterCfg(int level, string avatar, MonsterCfg& cfg){
	MonsterShowCfg* showCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgByShowID(avatar);
	const GuildActHpCfg* hpCfg = GUILD_HP_CFG->GetGuildActHpCfgByID(P_USER_OBJ->getUserInfo()->level);

	cfg.set_id(0);
	cfg.set_monstername(showCfg->monstername());
	cfg.set_desc(showCfg->desc());
	cfg.set_avatar(avatar);
	cfg.set_camp(ROLE_PARTY_HOSTILE);
	cfg.set_monstertype(showCfg->monstertype());
	cfg.set_level(level);

	if(hpCfg == NULL){
		cfg.set_hp(100*level);
	}else{
		cfg.set_hp(hpCfg->hp());
	}
	
	cfg.set_attack(0);
	cfg.set_defence(0);
	cfg.set_atkatt(0);

	cfg.set_atktype(showCfg->atktype());
	cfg.set_deftype(showCfg->deftype());
	cfg.set_ballistic(showCfg->ballistic());
	cfg.set_dieanimation(showCfg->dieanimation());

	cfg.set_dropid(0);
	cfg.set_minwalkspeed(0);
	cfg.set_maxwalkspeed(0);
	cfg.set_highspeed(0);
	cfg.set_emergevoice(showCfg->emergevoice());
	cfg.set_hurtvoice(showCfg->hurtvoice());
	cfg.set_dievoice(showCfg->dievoice());
	cfg.set_ailuafile("");
	cfg.set_atkatttype(showCfg->atkatttype());
	cfg.set_walktype(showCfg->walktype());
	cfg.set_hurtactioncd(showCfg->hurtactioncd());
}

unsigned int CGuildThiefDefScene::makeKey(int x, int y){
	return (x & 0xFF) << 8 | (y & 0xFF);
}

void CGuildThiefDefScene::cleanup(){

}

/************************************************************************/
/*                         事件处理                                     */
/************************************************************************/

void CGuildThiefDefScene::handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj){
	//猪脚死亡
	
}

void CGuildThiefDefScene::handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj){
	//计入总伤害 
	CCString* strValue = (CCString*)pstEventObj;
	int harmValue = strValue->intValue();
	m_uTotalHarmValue += harmValue;
}

void CGuildThiefDefScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	CEnemy* monster = dynamic_cast<CEnemy*>(pstEventObj);

	//怪物死亡后，才开始计时
	unsigned int key = makeKey(monster->getMonsterObj()->x, monster->getMonsterObj()->y);
	m_stMonKeyToTime[key] = GlobalTime::getTime();
	m_stMonKeyToBool[key] = true;

	if(monster->getMonsterObj()->dropCnt != 0){
		//掉落统计
		//unsigned int itemID = DROP_CFG_MASTER->GetDropCfgByID(dropID)->elementlst().elementid();	//读配置
		//FallingItem::fall(itemID, this->_mainHero, dynamic_cast<CQStageEntity*>(pstEventObj)->getPosition(), callfuncO_selector(CMainHero::pickupItem));
	}

	handleHeroKillValueChange(stEvent, pstEventObj);
}

void CGuildThiefDefScene::handleHeroKillValueChange(const string& stEvent, CCObject* pstEventObj){
	CBaseFightScene::handleHeroKillValueChange(stEvent, pstEventObj);

	CEnemy* pstEntity = dynamic_cast<CEnemy*>(pstEventObj);

	//把已经播放完毕死亡动画的死人做掉
	if(pstEntity->getMyState() == kActionStateDie){  
		_sceneForegound->removeChild(pstEntity, true);
		if(_stageEntities->indexOfObject(pstEntity) != UINT_MAX){
			_stageEntities->removeObject(pstEntity);
		}
	}
}
#include "GuardActScene.h"
#include "MapCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "UserObj.h"
#include "HeroCfgMaster.h"
#include "vlib/CVRand.h"
#include "city/CityControl.h"
#include "city/CityProxy.h"
#include "GuardActRankUI.h"
#include "GuardActPrizeUI.h"
#include "DummyHero.h"
#include "ToolUtils.h"
#include "DummyEnemy.h"
#include "CSSenceOSD.pb.h"
#include "DailyActControl.h"
#include "loading/LoadingScreen.h"
#include "SceneManager.h"
#include "ReviveLayer.h"
#include "CVZMonsterWaveCfgMaster.h"
#include "CVZMonsterCfgMaster.h"
#include "CommonDialog.h"
#include "NounHelper.h"
#include "TextHelper.h"
#include "GlobalTime.h"
#include "instance/InstanceUtils.h"

#define DEFAULT_ROLE_DIRECTION ccp(1, 0)
#define DEFAULT_MONSTER_ID 1

GuardActScene::GuardActScene() {
    m_pRankUI = NULL;
    m_stLastDirection = (CCPointZero);
    m_bIsRoomReady = false; //TODO: 默认为false
    m_pReviveLayer = NULL;
    m_stMainHeroInitPos = ccp(300, 200);
    for(int i = 0; i < PATH_LINE_NUM; ++i) {
        m_stBaseStat[i] = BASE_STAT_LIVE;
    }

	for(int i = 0; i < PATH_LINE_NUM; ++i) {
		m_stBossArm[i] = NULL;
	}

    m_bIsActOver = false;
}

GuardActScene::~GuardActScene() {

    P_DAILYACT_CTRL->setGuardActScene(NULL);

    CC_SAFE_RELEASE_NULL(_sceneForegound);
    CC_SAFE_RELEASE_NULL(_sceneBackground);
    CC_SAFE_RELEASE_NULL(_sceneMidground);
    CC_SAFE_RELEASE_NULL(_sceneFrontgound);

    m_pRolesDict->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pRolesDict);
    CC_SAFE_RELEASE_NULL(m_stHeroRes);

    m_pEnemysDict->removeAllObjects();
    CC_SAFE_RELEASE_NULL(m_pEnemysDict);

    CC_SAFE_RELEASE_NULL(m_pReviveLayer);
}


bool GuardActScene::init(){
    bool bRet = false;

    do {
        CC_BREAK_IF(!CBaseFightScene::init());

        m_pRolesDict = CCDictionary::create();
        m_pRolesDict->retain();

        m_pEnemysDict = CCDictionary::create();
        m_pEnemysDict->retain();
        
        m_stHeroRes = CCArray::create();
        m_stHeroRes->retain();
        
        P_DAILYACT_CTRL->setGuardActScene(this);
        P_CITY_CTRL->getProxy()->sendPlayerChangeSceneRqst(50001); //切场景就会触发参加活动

        bRet = true;
    } while(0);

    return bRet;
}

void  GuardActScene::setRoomReady(){
    m_bIsRoomReady = true;
    if(m_fFightTime > 0) {
        //场景也已经构建好了
        if(m_pReviveLayer && m_pReviveLayer->getParent()) {
            m_pReviveLayer->doWaitingDone();
        }
        P_CITY_CTRL->getProxy()->sendGetPlayersRqst();
    }
    CCLOG("[trace]GuardActScene::setRoomReady,room is ready,scene start time %f", m_fFightTime);
}

void GuardActScene::onEnterTransitionDidFinish(){
    CBaseFightScene::onEnterTransitionDidFinish();

    CCLOG("[trace]GuardActScene::onEnterTransitionDidFinish");
    P_CITY_CTRL->getProxy()->setOSDSceneInf(this);
    if(m_bIsRoomReady) {
        //房间已经准备好了
        P_CITY_CTRL->getProxy()->sendGetPlayersRqst();
    } else {
        showWaitingRoomLayer();
    }
    //TODO: 关卡未开始时等待界面
    CCLOG("[trace]GuardActScene::onEnterTransitionDidFinish,room is ready ? %d", m_bIsRoomReady);
    this->scheduleUpdate();
}


void GuardActScene::update(float delta){
    if(m_bIsActOver) {
        return; //活动结束了就不走了
    }

    CBaseFightScene::update(delta);

    CBaseMapScene::setViewpointCenter(_mainHero->getPosition());

    
    static float totalTime = 0;
    totalTime += delta;
    if(totalTime >= 0.2) {
        //一秒上报一次杀怪信息
        doUploadHitMonster();
        totalTime = 0;
    }

    static int iFrameCnt = 0;
    //10帧上报一次主角血量
    if(iFrameCnt++ >= 60) {
        if(NULL == m_pReviveLayer || NULL == m_pReviveLayer->getParent() ) {
            //复活CD时不上报
            if((unsigned int)_mainHero->getHP() < _mainHero->getMaxHP()) {
                P_DAILYACT_CTRL->getProxy()->sendHitedRqst(_mainHero->getHP()); //TODO: 暂时关掉
            }
            
        }
        doUploadMonsterInfo();

        iFrameCnt = 0;
    }
}


void GuardActScene::setWalkHight(){
    m_uWalkHight = MAP_CFG_MASTER->GetMapCfgByID(50001)->midwalkh(); 
}

void GuardActScene::cleanup(){

}

void GuardActScene::initMap(){
	CBaseFightScene::initMap();
	walkableRect = CCRect(0, 0, _screenW, m_uWalkHight); //设置可行走区域
}

void GuardActScene::loadMonsterRes(){

    //_monsterData记录地图编辑器里的刷怪点信息
    CCLOG("[trace]GuardActScene::loadMonsterRes,monster pos data size=%d", _monsterData->count());

    const CVZMonsterWaveCfgSet& stCfgSet = CVZ_MONSTER_WAVE_CFG_MASTER->GetCfg();
    for(int i = 0; i < stCfgSet.monsterwavecfg_size(); ++i) {
        //怪物资源
        const string& strAvatar = stCfgSet.monsterwavecfg(i).monsteravatar();
        const MonsterShowCfg* pstShowCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgByShowID(strAvatar);
        if(pstShowCfg) {
            loadMonsterResByCfg(pstShowCfg, 100); //TODO: 修改怪物数量,暂时给个大值，退出时清除掉
            CCLOG("[trace]GuardActScene::loadMonsterRes,load wave %d's avatar %s", i+1, strAvatar.c_str());
        }
        
    }


	string str = MONSTER_RES_PATH + "ma00142/ma00142.ExportJson";
	m_stRes.insert(str);

	CCObject* pstObj;
	CCARRAY_FOREACH(_monsterData, pstObj){
		CEnemyData* pstEnemyObj = (CEnemyData*)pstObj; 
		m_stID2Point[pstEnemyObj->id] = ccp(pstEnemyObj->x, pstEnemyObj->y);
	}
}


void GuardActScene::loadItemRes(){

}


void GuardActScene::brushMonsterLogic(float dt){
   
}								

DummyEnemy* GuardActScene::initOneDummyEnemy(const UIN_t uin, const CCPoint& pos, const int level, const int wave){
    DummyEnemy* pstMonster = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(uin));
    if(pstMonster) {
        return pstMonster;
    }
    MonsterCfg monsterCfg;
    int iRet = genMonsterCfg(level, wave, monsterCfg);
    if(0 != iRet) {
        CCLOG("[trace]GuardActScene::initOneDummyEnemy, fail to get cfg,ret %d", iRet);
        return NULL;
    }
    m_uTotalMonsterHp += monsterCfg.hp();

    pstMonster = DummyEnemy::create();
    pstMonster->setUin(uin); //设置Uin 
    pstMonster->setNameStr(monsterCfg.monstername());
    pstMonster->setResPath(monsterCfg.avatar());
    pstMonster->setIsNeedPath(monsterCfg.walktype() == 1);

    CQStageEntityObj* monsterInfo = CQStageEntityObj::create();
    monsterInfo->setInfoByMonsterCfg(&monsterCfg);
    pstMonster->setObj(monsterInfo);

    pstMonster->setCenterToBottom(55);
    pstMonster->setCenterToSides(55);

    pstMonster->SetEnityPosition(pos);
    pstMonster->setDesiredPosition(pos);
    pstMonster->setHurtActionCD(monsterCfg.hurtactioncd());
    pstMonster->setLevel(monsterCfg.level());

    for(int j = 0; j < monsterCfg.skills_size(); j++){
        CQSkillCell* pstSkill = CQSkillCell::create();	
        pstSkill->m_uSkillID = monsterCfg.skills(j);
        pstMonster->getSkillArray()->addObject(pstSkill);
    }

    pstMonster->AddEventListener(this, CQStageEntity::ATK_ACTION_KEY_FRAME, eventListner_selector(GuardActScene::handleCloseAtkKeyFrameEvent));
    pstMonster->AddEventListener(this, CQStageEntity::SKILL_CAST_FRAME, eventListner_selector(GuardActScene::handleSkillKeyFrameEvent));
    pstMonster->AddEventListener(this, CQStageEntity::FIRE_EVENT, eventListner_selector(GuardActScene::handleFireABulletEvent));
    pstMonster->AddEventListener(this, CQStageEntity::DIE_EFF_FRAME, eventListner_selector(GuardActScene::handleDieEffKeyFrameEvent));
    pstMonster->AddEventListener(this, CQStageEntity::DIE_COMPLETE_EVENT, eventListner_selector(GuardActScene::handleEnemyDieCompleteEvent));
	pstMonster->AddEventListener(this, DummyEnemy::ENEMY_DIE_UP_DATA, eventListner_selector(GuardActScene::handleEnemyUpDataEvent));

    pstMonster->AddEventListener(this, ENEMY_HURT_EVENT, eventListner_selector(GuardActScene::handleEnemyHurtEvent));
    pstMonster->AddEventListener(this, HERO_HARMVALUE_CHANGE, eventListner_selector(GuardActScene::handleEnemyHarmValueEvent));
    
    pstMonster->LoadActionRes();
    pstMonster->setAIType(AI_TYPE_LAST_SKILL_ONLY); //设置AI类型为直行
    pstMonster->setScaleX(-1.0); //面向左
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pstMonster->getPositionY());
    _sceneForegound->addChild(pstMonster, iNewZorder); //显示
    _stageEntities->addObject((CCObject*)pstMonster);
    m_pEnemysDict->setObject(pstMonster, uin);

	CCLOG("[trace]GuardActScene::initOneDummyEnemy pos, monster the pos x ==== %f, y ==== %f", pos.x, pos.y);

    return pstMonster; 
}

void GuardActScene::handleEnemyHarmValueEvent(const string& stEvent, CCObject* pstEventObj){
    DummyEnemyHurtEventObj* hurtObj = (DummyEnemyHurtEventObj*)pstEventObj;
    DummyEnemy *pEnemy = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(hurtObj->uEnemyUin));
    int iRemainHP = -1;
    if(pEnemy) {
        iRemainHP = pEnemy->getHP();
    }
    //计算自己的伤害
    CCLOG("enemy harm:%lu,%d,remain %d", hurtObj->uEnemyUin, hurtObj->iHurtVal, iRemainHP);
    if(m_stPlayerHurtData.count(hurtObj->uEnemyUin)) {
        m_stPlayerHurtData[hurtObj->uEnemyUin] += hurtObj->iHurtVal;
    } else {
        m_stPlayerHurtData[hurtObj->uEnemyUin] = hurtObj->iHurtVal;
    }
}

void GuardActScene::handleEnemyDieCompleteEvent(const string& stEvent, CCObject* pstEventObj){
    DummyEnemy *pEnemy = (DummyEnemy*) pstEventObj;
    if(NULL == pEnemy) {
        return;
    }
    const UIN_t uin = pEnemy->getUin();
    CCLOG("[trace]GuardActScene::handleEnemyDieCompleteEvent,monster %lu die,remove it.", uin);

    m_pEnemysDict->removeObjectForKey(uin); //remove
    m_stMyCtrlEnemy.erase(uin);

    //boss死亡，血条消失
    if(pEnemy->getRoleType() == ROLE_TYPE_SENIOR_BOSS || pEnemy->getRoleType() == ROLE_TYPE_JUNIOR_BOSS){
        if(m_pBossHPUI->isVisible()){
            m_pBossHPUI->setVisible(false);
        }
    }

    //把已经播放完毕死亡动画的死人做掉
    if(pEnemy->getMyState() == kActionStateDie) {
        _sceneForegound->removeChild(pEnemy, true);
        if(_stageEntities->indexOfObject(pEnemy) != UINT_MAX){
            _stageEntities->removeObject(pEnemy);
        }
    }
}

void GuardActScene::handleEnemyUpDataEvent(const std::string& stEvent, CCObject* pstEventObj){
	doUploadHitMonster();
}

void GuardActScene::buildMap(){
    CBaseFightScene::buildMap();
    updateMapBlocks();
}

void GuardActScene::updateInstnaceByAI(){
    CBaseFightScene::updateInstnaceByAI();
    
}

void GuardActScene::initMapUI(){
    //重写掉父类方法
    CBaseFightScene::initMapUI();

    //按钮
    CCSize stSize = CCDirector::sharedDirector()->getVisibleSize(); 
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

    m_pPrizeUI = CGuardActPrizeUI::create();
    m_pPrizeUI->setPosition(ccp(visibleOrigin.x, stSize.height - m_pPrizeUI->getSize().height - m_pHeroHeadUI->getSize().height));
    m_pUiLayer->addWidget(m_pPrizeUI);

    m_pRankUI = CGuardActRankUI::create();
    m_pRankUI->setPosition(visibleOrigin + ccp(stSize.width - m_pRankUI->getSize().width - 20,
        m_pPauseLayout->getPositionY() - m_pRankUI->getSize().height - 20));
    m_pUiLayer->addWidget(m_pRankUI); //TODO:
}

void GuardActScene::initMonster(){
	//map<int,CCPoint >::iterator stIter;
	//for(unsigned int i = 0; i < PATH_LINE_NUM; i++){
	//	CCArmature* pArm = CCArmature::create("ma00142");
	//	pArm->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(GuardActScene::haneldBossArmCallBack));
	//	pArm->getAnimation()->play("stand", -1, -1, 1);
	//	float fWidth = pArm->getContentSize().width/4;

	//	stIter = m_stID2Point.find(PATH_LINE_NUM + i);
	//	CCPoint stPos = ccp(fWidth, 500);
	//	if(stIter != m_stID2Point.end()){
	//		stPos = ccp(fWidth, stIter->second.y);
	//	}

	//	pArm->setPosition(ccp(stPos.x, _sceneForegound->getContentSize().height - stPos.y));
	//	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pArm->getPositionY());
	//	_sceneForegound->addChild(pArm, iNewZorder);
	//	m_stBossArm[i] = pArm;
	//}
}

void GuardActScene::haneldBossArmCallBack(CCArmature* armature, MovementEventType type, const char* stName){
	string nameStr = stName;
	if (type == COMPLETE){
		if(nameStr == "die"){
			clearBossArm(armature);
		}
	}
}

void GuardActScene::clearBossArm(CCArmature* armature){
	for(int i = 0; i < PATH_LINE_NUM; i++){
		CCArmature* pArm = m_stBossArm[i];
		if(pArm && pArm->m_uID == armature->m_uID){
			m_stBossArm[i]->removeFromParentAndCleanup(true);
			m_stBossArm[i] = NULL;
			return;
		}
	}
}

DummyHero * GuardActScene::initOneDummyHero(const UIN_t uin, const CCPoint& pos, unsigned int showID, int weaponCfgID) {
    CCPoint stPlayerPt(400, 200); //默认位置
    const int iPosIdx = CRandom::GetRand(PATH_LINE_NUM);

    const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
    const string* pHeroStr = HERO_CFG_MASTER->GetResName(showID); //TODO:
    if(NULL == pHeroStr) {
        CCLOG("[error]GuardActScene::initOneFriend, get hero res name fail! invalid show id %d", showID );
        return NULL;
    }
    const string& heroStr = *pHeroStr;

    DummyHero *hero = NULL;
    hero = DummyHero::create();

    string stBodyName;
    //loadHeroResByShowID(); //TODO:以后load同屏玩家资源用这个吧
    int iRet = CMainHero::GetResourceNames(weaponCfgID, showID, stBodyName);
    if (iRet){
        CCLOG("no resource name is found for hero %s", pHeroStr->c_str());
        return NULL;
    }
    hero->setResPath(stBodyName);
    hero->SetEnityPosition(stPlayerPt);
    hero->setDesiredPosition(hero->getPosition());

    CQStageEntityObj* heroInfo = CQStageEntityObj::create();
    heroInfo->setInfoByUserInfo(pUserInfo);
	heroInfo->weaponCfgID = weaponCfgID;
    hero->setObj(heroInfo);

    hero->setCenterToSides(60);

    hero->AddEventListener(this, CQStageEntity::SKILL_CAST_FRAME, eventListner_selector(GuardActScene::handleSkillKeyFrameEvent));
    hero->AddEventListener(this, CQStageEntity::FIRE_EVENT, eventListner_selector(GuardActScene::handleFireABulletEvent));
    hero->AddEventListener(this, HERO_LOSEVALUE_CHANGE, eventListner_selector(GuardActScene::handleFriendHurtValueEvent));

	if(uin == pUserInfo->uin){
		hero->AddEventListener(this, HERO_EPVALUE_COST_EVENT, eventListner_selector(GuardActScene::handleHeroEPCostEvent));
	}
    
    //枪械上的技能
    CQSkillCell* pstSkill = CQSkillCell::create();	
    pstSkill->m_uSkillID = WEAPON_CFG_MASTER->GetWeaponCfgByID(weaponCfgID)->weaponsuperskill();	
    pstSkill->m_uLastCastTime = 0;	
    hero->getSkillArray()->addObject(pstSkill);

    CQSkillCell* pstSkill2 = CQSkillCell::create();	
    pstSkill2->m_uSkillID = 2;	
    pstSkill2->m_uLastCastTime = 0;	
    hero->getSkillArray()->addObject(pstSkill2);

    //add to stage
    hero->setRoleType(ROLE_TYPE_FRIEND);
    hero->LoadActionRes();
    hero->setLevel(pUserInfo->level);
    
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, hero->getPositionY());
    _sceneForegound->addChild(hero, iNewZorder);
    _stageEntities->addObject(hero); //显示
    m_pRolesDict->setObject(hero, uin);

    return hero;
}

void GuardActScene::handleHeroEmergeCompleteEvent(const string& stEvent, CCObject* pstEventObj){
	
}

//玩家或友军死亡
void GuardActScene::handleHeroKilledEvent(const string& stEvent, CCObject* pstEventObj){
    //不监听友军挂了的事件
    const CMainHero *hero = (CMainHero*)pstEventObj;
    if(hero == _mainHero) {
        //主角挂了
        CCLOG("main hero died, %d" , (hero != NULL));
        showReviveLayer();
    } 
}


//友军掉血
void GuardActScene::handleFriendHurtValueEvent(const string& stEvent, CCObject* pstEventObj){
    //计入损失血量总值 
    CCString* strValue = (CCString*)pstEventObj;
    int hurtValue = strValue->intValue();
    
    CCLOG("hurt val, %d" , hurtValue);
}

void GuardActScene::doUploadHitMonster() {

    if(m_stPlayerHurtData.size() <= 0) {
        return;
    }
    
    vmsg::CSCVZHitMonsterRqst stHitMonsterRqst;

    CCLOG("-------upload data--------");
    stHitMonsterRqst.set_mp(_mainHero->getEP());

    for(auto iter = m_stPlayerHurtData.begin(); iter != m_stPlayerHurtData.end(); ++iter) {
        CCLOG("-monster id %lu,hurt %d", iter->first, (iter->second) );
        vmsg::CSCVZMonsterHurt* oneInfo = stHitMonsterRqst.add_monsterhurt();
        oneInfo->set_monsterid(iter->first);
        oneInfo->set_hurt(iter->second);
    }
    m_stPlayerHurtData.clear(); //上报完置空
    P_DAILYACT_CTRL->getProxy()->sendHitMonsterRqst(stHitMonsterRqst);
    CCLOG("----done----");
}

void GuardActScene::doUploadMonsterInfo() {
    if(m_stMyCtrlEnemy.size() <= 0) {
        return;
    }
    CSCVZMonsters stMonsterInfo;
    for(auto iter = m_stMyCtrlEnemy.begin(); iter != m_stMyCtrlEnemy.end(); ++iter) {
        const UIN_t uin = *iter;
        DummyEnemy* pEnemy = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(uin));
        if(NULL == pEnemy) {
            continue;
        }


        CSCVZMonsterInfo* pOneInfo = stMonsterInfo.add_monsters();
        pOneInfo->set_monsterid(uin);
        pOneInfo->mutable_pos()->set_xpos(pEnemy->getPositionX());
        pOneInfo->mutable_pos()->set_ypos(pEnemy->getPositionY());

        pOneInfo->set_action(pEnemy->getActionStat());
    }
}

///////////////////////////////osd begin///////////////////////////
void GuardActScene::addRoleQueue(const vmsg::CSPlayerPosInScene& stInfo){
	CCPoint pos = ccp(stInfo.scenepoint().xpos(), stInfo.scenepoint().ypos());
	CCPoint direct = ccp(stInfo.direct().xpos(), stInfo.direct().ypos());
	addRole(stInfo.comm(), pos, direct);
}


void GuardActScene::addRole(const vmsg::CSPlayerCommInScene& playerInfo, const CCPoint &pos, const CCPoint &direction) {
    if(!m_bIsRoomReady) {
        return;
    }

	//加载资源
	loadHeroResByShowID(playerInfo.showid(), playerInfo.weaponcfgid(), false);
	loadWeaponeResByCfgID(playerInfo.weaponcfgid(), false);

    CQStageEntity *pRole = NULL;
    const UIN_t uin = playerInfo.uin();

    if(isEnemyUin(uin)) {
        CCLOG("[warn]GuardActScene::addRole, can not add enemy!");
        return;
    }
    //hero
    if(m_pRolesDict->objectForKey(uin) == NULL) { 
        pRole = initOneDummyHero(uin, pos, playerInfo.showid(), playerInfo.weaponcfgid());
    } else {
        pRole = dynamic_cast<DummyHero*>(m_pRolesDict->objectForKey(uin));
    }

    if(NULL == pRole) {
        CCLOG("[warn]GuardActScene::addRole,role is null, uin %lu", uin);
        return;
    }

    if(direction.x >= 0) {
        pRole->setScaleX(1.0f);
    } else {
        pRole->setScaleX(-1.0f);
    }
}

void GuardActScene::removeRole(UIN_t uin) {

    CQStageEntity *pRole = getStageEntity(uin);
    bool isEnemy = isEnemyUin(uin);

    if(pRole != NULL) {
        _sceneForegound->removeChild(pRole);
    }
    if(isEnemy) {
        m_stMyCtrlEnemy.erase(uin);
        m_pEnemysDict->removeObjectForKey(uin);
    } else {
        m_pRolesDict->removeObjectForKey(uin);
    }
}


void GuardActScene::setRolePos(UIN_t uin, const CCPoint &pos, const CCPoint &direction) {
    CQStageEntity *pRole = getStageEntity(uin);
    if(pRole != NULL) {
        pRole->WalkTo(pos); //TODO:是否启用walkto?
        pRole->setDstDirection(direction);
    }
}

void GuardActScene::moveRoleByDirection(UIN_t uin, const CCPoint &startPos, const CCPoint &direction) {
    if(isEnemyUin(uin)) {
        DummyEnemy* pRole = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(uin));
        if(pRole != NULL) {
            pRole->setIsFollowActivate(true);
            pRole->setFollowDstPos(startPos); // 模拟强制拉回
            pRole->WalkWithDirection(direction);
        }
    } else {
        DummyHero* pRole = dynamic_cast<DummyHero*>(m_pRolesDict->objectForKey(uin));
        if(pRole != NULL) {
            //pRole->setPosition(startPos); // 强制拉回
            pRole->setIsFollowActivate(true);
            if(startPos.x == 0 && startPos.y == 0) {
                pRole->setFollowDstPos(pRole->getPosition());
            } else pRole->setFollowDstPos(startPos); // 模拟强制拉回
            
            pRole->stateChangeByMove(direction);
        }
    }
}

void GuardActScene::moveRoleToDestination(UIN_t uin, const CCPoint &endPos, const CCPoint &direction) {
    //DummyHero *pRole = dynamic_cast<DummyHero*>(m_pRolesDict->objectForKey(uin));
    CQStageEntity *pRole = getStageEntity(uin);
    if(pRole != NULL) {
        pRole->WalkTo(endPos);
    }
}
///////////////////////////////osd end/////////////////////////////


//////////////////////////hud layer delegate - begin//////////////////////
void GuardActScene::didChangeDirectionTo(HudLayer *hud, cocos2d::CCPoint direction)
{
    if(m_stLastDirection.equals(CCPointZero) || !m_stLastDirection.equals(direction)) {
        m_stLastDirection = direction;
        direction = direction * 10000;
        _mainHero->stateChangeByMove(direction);
        P_CITY_CTRL->getProxy()->sendPlayerMoveRqst(direction, _mainHero->getPosition());
    }
}

void GuardActScene::isHoldingDirection(HudLayer *hud, cocos2d::CCPoint direction)
{
    _mainHero->stateChangeByMove(direction);
}

void GuardActScene::simpleDPadTouchEnded(HudLayer *hud)
{
    _mainHero->stateChangeByReleaseHud();
    CCPoint direction = _mainHero->getDstDirection() * 10000;
    P_CITY_CTRL->getProxy()->sendPlayerSetPosRqst(direction, _mainHero->getPosition());
}
//////////////////////////hud layer delegate - end//////////////////////

void GuardActScene::mainHeroShoot(){
    if(_mainHero->checkIsDied()) {
        return;
    }
    CBaseFightScene::mainHeroShoot();
    //上报射击状态
    CCPoint direct(_mainHero->getScaleX(), 0);
    P_CITY_CTRL->getProxy()->sendPlayerAtkRqst(true, direct, _mainHero->getPosition());
}

void GuardActScene::mainHeroCeaseShoot(){
    CBaseFightScene::mainHeroCeaseShoot();
    //上报停止射击状态
    CCPoint direct(_mainHero->getScaleX(), 0);
    P_CITY_CTRL->getProxy()->sendPlayerAtkRqst(false, direct, _mainHero->getPosition());
}

void GuardActScene::setPlayerStartAtkStat(const UIN_t uin, const vmsg::CSPlayerStartAtk& stStartAtk){
    CCLOG("[trace]GuardActScene::setPlayerStartAtkStat,uin %lu,start atk", uin);
    DummyHero* pRole = dynamic_cast<DummyHero*>(m_pRolesDict->objectForKey(uin));
    if(pRole) {
        CCPoint direct(stStartAtk.direct().xpos(), stStartAtk.direct().ypos());
        if(direct.x < 0) {
            pRole->setScaleX(-1);
        } else {
            pRole->setScaleX(1);
        }
        //pRole->Idle();
        pRole->stateChangeByReleaseHud();
        pRole->stateChangeByAttack(true);
    }
} 

void GuardActScene::setPlayerStopAtkStat(const UIN_t uin, const vmsg::CSPlayerStopAtk& stStopAtk){
    CCLOG("[trace]GuardActScene::setPlayerStopAtkStat,uin %lu,stop atk", uin);
    DummyHero* pRole = dynamic_cast<DummyHero*>(m_pRolesDict->objectForKey(uin));
    if(pRole) {
        CCPoint direct(stStopAtk.scenepoint().xpos(), stStopAtk.scenepoint().ypos());
        if(direct.x < 0) {
            pRole->setScaleX(-1);
        } else {
            pRole->setScaleX(1);
        }
        pRole->stateChangeByAttack(false);
        pRole->Idle();
    }
}

void GuardActScene::addMonster(const vmsg::CSOSDMonstersInfo& monsterInfo){
    if(!m_bIsRoomReady) {
        return;
    }
    const UIN_t uin = monsterInfo.monsterid();

    CCLOG("[trace]GuardActScene::addMonster,id %d, channel %d,wave %d", 
        monsterInfo.monsterid(), monsterInfo.channel(), monsterInfo.wave());

    if(!isEnemyUin(uin)) {
        CCLOG("[warn]GuardActScene::addMonster, invalid monster uin %lu", uin);
        return;
    } 
    //enemy
    DummyEnemy *pRole = (DummyEnemy*)m_pEnemysDict->objectForKey(uin);
    CCPoint pos(monsterInfo.scenepoint().xpos(), monsterInfo.scenepoint().ypos());

    if(0 == pos.x && 0 == pos.y) {
        //自己放上去
        pos = randMonsterPos(monsterInfo.channel());
    }

    if(NULL == pRole) { 
        pRole = initOneDummyEnemy(uin, pos, monsterInfo.lv(), monsterInfo.wave()); //TODO:设置为CfgID
    }
    if(NULL == pRole) {
        CCLOG("[error]GuardActScene::addMonster, monster is null,uin=%lu,lv=%d,wave=%d", uin, monsterInfo.lv(), monsterInfo.wave());
        return;
    }
    pRole->setPosChannel(monsterInfo.channel());
    pRole->setHP(monsterInfo.hp());
    //pRole->SetIsControlledByAI(false); //设置是否由AI驱动
    if(vmsg::MONSTER_ACTION_SCENE_WALK == monsterInfo.action()) {
        pRole->setIsFollowActivate(true);
        pRole->WalkWithDirection(ccp(-1, 0));
    } else if(vmsg::MONSTER_ACTION_SCENE_HIT == monsterInfo.action()) {
        pRole->Attack();
    } else {
        CCLOG("[error]GuardActScene::addMonster,unknow action %d", monsterInfo.action());
    }
}


void GuardActScene::setPlayerSkillInScene(const UIN_t uin, const vmsg::CSPlayerSkillInScene& stSkillInScene) {
    CCLOG("[trace]GuardActScene::setPlayerSkillInScene,uin %lu,skillID=%d", uin, stSkillInScene.skillid());
    CQStageEntity* pRole = getStageEntity(uin);
    if(NULL == pRole) {
        return;
    }
    CCPoint direct(stSkillInScene.scenepoint().xpos(), stSkillInScene.scenepoint().ypos());
    if(direct.x < 0) {
        pRole->setScaleX(-1);
    } else {
        pRole->setScaleX(1);
    }
    pRole->AttackWithSkillAtIdx(stSkillInScene.skillid()); //同步skill idx
}


CQStageEntity* GuardActScene::getStageEntity(const UIN_t uin){
    CQStageEntity *pRole = NULL;
    if(isEnemyUin(uin)) {
        pRole = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(uin));
    } else {
        pRole = dynamic_cast<DummyHero*>(m_pRolesDict->objectForKey(uin));
    }
    return pRole;
}

void GuardActScene::handlePassLogic(){
    //检查怪物位置判断通关条件
    CCDictElement* pElmt;
    int iIsMyCtrl;
    CCDICT_FOREACH(m_pEnemysDict, pElmt) {
        DummyEnemy* pEnemy = (DummyEnemy*)pElmt->getObject();
        if(pEnemy->getPositionX() <= 150 && !(pEnemy->getIsArriveBase())) { //TODO;
            iIsMyCtrl = m_stMyCtrlEnemy.count(pEnemy->getUin());
            CCLOG("[trace]enemy %lu arrive base %d,isMyCtrl=%d", pEnemy->getUin(), pEnemy->getPosChannel(), iIsMyCtrl);
            pEnemy->setIsArriveBase(true);
            pEnemy->Idle();
            pEnemy->SetIsControlledByAI(false); // 停下AI

            if(iIsMyCtrl) {
                //send destroy base notify
                P_DAILYACT_CTRL->getProxy()->sendDestroyBaseRqst(pEnemy->getPosChannel());
            }
            
        }    
    }

}



void GuardActScene::showReviveLayer(){
    if(NULL == m_pReviveLayer) {
        m_pReviveLayer = CReviveLayer::create();
        m_pReviveLayer->retain();
        m_pReviveLayer->AddEventListener(this, REVIVE_DONE, eventListner_selector(GuardActScene::onReviveDone));
    }
    if(NULL == m_pReviveLayer->getParent()) {
        m_pReviveLayer->startCountdown();
        addChild(m_pReviveLayer);

        P_DAILYACT_CTRL->getProxy()->sendHitedRqst(0); //上报死亡
    }
}


void GuardActScene::onReviveDone(const std::string& stEvent, CCObject* pstEventObj){
    CCLOG("revive done");
    _mainHero->setHP(_mainHero->getMaxHP());
    _mainHero->Idle();
    _mainHero->setScaleX(1.0f); //面向右
    _mainHero->setDesiredPosition(m_stMainHeroInitPos);
	m_pHeroHeadUI->setHPPercent(_mainHero->getHP(), _mainHero->getMaxHP());

    P_DAILYACT_CTRL->getProxy()->sendHitedRqst(_mainHero->getHP());
}


void GuardActScene::showWaitingRoomLayer(){
    if(NULL == m_pReviveLayer) {
        m_pReviveLayer = CReviveLayer::create();
        m_pReviveLayer->retain();
        m_pReviveLayer->AddEventListener(this, REVIVE_DONE, eventListner_selector(GuardActScene::onReviveDone));
    }
    if(NULL == m_pReviveLayer->getParent()) {
        m_pReviveLayer->showWaiting();
        addChild(m_pReviveLayer);
    }
}

void GuardActScene::doControlNotify(const CSCVZControlNotify& stCtrlNotify){
    CSCVZMonsters stInfoRqstLst; //需要上报的怪物状态

    for(int i = 0; i < stCtrlNotify.monsters_size(); ++i) {
        const CSVZControlMonsterInfo& oneInfo = stCtrlNotify.monsters(i);
        m_stMyCtrlEnemy.insert(oneInfo.monsterid()); //记下由我控制
        DummyEnemy *pstMonster = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(oneInfo.monsterid()));
        CCLOG("[trace]GuardActScene::doControlNotify,ctrl monster %d,channel %d,lv=%d,wave=%d", 
            oneInfo.monsterid(), oneInfo.channel(), oneInfo.lv(), oneInfo.wave());
        if(pstMonster) {
            //如果有，则改为AI驱动
            pstMonster->setPosChannel(oneInfo.channel());
            pstMonster->SetIsControlledByAI(true); //设置是否由AI驱动
            pstMonster->setNeedSendSkill(true);

            //上报一下控制的怪物信息
            CSCVZMonsterInfo* pInfoRqst = stInfoRqstLst.add_monsters();
            pInfoRqst->set_monsterid(oneInfo.monsterid());
            pInfoRqst->set_action(MONSTER_ACTION_SCENE_WALK);
            pInfoRqst->mutable_pos()->set_xpos(pstMonster->getPositionX());
            pInfoRqst->mutable_pos()->set_ypos(pstMonster->getPositionY());
            pInfoRqst->set_wave(oneInfo.wave());
            pInfoRqst->set_lv(oneInfo.lv());
            pInfoRqst->set_skillid(DummyEnemy::EMPTY_SKILL_ID);

            continue;
        }

        //如果没有，就刷出来
        CCPoint stPos = randMonsterPos(oneInfo.channel());

        pstMonster = initOneDummyEnemy(oneInfo.monsterid(), stPos, oneInfo.lv(), oneInfo.wave()); 
        if(pstMonster) {
            pstMonster->SetIsControlledByAI(true); //设置是否由AI驱动
            pstMonster->setPosChannel(oneInfo.channel());
            pstMonster->setNeedSendSkill(true);

            CSCVZMonsterInfo* pInfoRqst = stInfoRqstLst.add_monsters();
            pInfoRqst->set_monsterid(oneInfo.monsterid());
            pInfoRqst->set_action(MONSTER_ACTION_SCENE_WALK);
            pInfoRqst->mutable_pos()->set_xpos(pstMonster->getPositionX());
            pInfoRqst->mutable_pos()->set_ypos(pstMonster->getPositionY());
            pInfoRqst->set_wave(oneInfo.wave());
            pInfoRqst->set_lv(oneInfo.lv());
            pInfoRqst->set_skillid(DummyEnemy::EMPTY_SKILL_ID);
        }
        
    }


    //上报一下控制的怪物信息
    if(stInfoRqstLst.monsters_size() > 0) {
        P_DAILYACT_CTRL->getProxy()->sendSubmitMonsterPosRqst(stInfoRqstLst);
    }
    
}

CCPoint GuardActScene::randMonsterPos(const int iChannel){
    //调整一下在本线上的Y坐标，避免刷在一起
    int yPos = CRandom::GetRand(PATH_LINE_HEIGHT - 10); //TODO: 位置后期可以再调整
	map<int,CCPoint >::iterator stIter = m_stID2Point.find(PATH_LINE_NUM + iChannel + 1);
    if(stIter == m_stID2Point.end() || NULL == _sceneForegound) {
        CCLOG("[error]GuardActScene::randMonsterPos,channel=%d", iChannel);
        return ccp(800, 200);
    }
    CCPoint stPos = ccp(stIter->second.x, _sceneForegound->getContentSize().height - stIter->second.y);
    stPos.y = stPos.y + yPos;
    stPos.y = stPos.y <= 0 ? 10 : stPos.y; //TODO:修正一下坐标

    return stPos;
}


void GuardActScene::doHitMonsterNotify(const CSCVZHitMonsterNotify& stHitMonsterNotify){
    for(int i = 0; i < stHitMonsterNotify.monsterhp_size(); ++i) {
        const CSCVZMonsterHP& oneInfo = stHitMonsterNotify.monsterhp(i);
        DummyEnemy *pstMonster = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(oneInfo.monsterid()));
        if(NULL == pstMonster) {
            continue;
        }
        pstMonster->SyncHP(oneInfo.hp());
    }
}

void GuardActScene::doHitedNotify(const CSCVZHitedNotify& stHitedNotify){
    const UIN_t uin = stHitedNotify.uin();
    if(uin == P_USER_OBJ->getUserInfo()->uin) {
        return;
    }
    if(isEnemyUin(uin)) {
        DummyEnemy* pEnemy = dynamic_cast<DummyEnemy*>(m_pRolesDict->objectForKey(uin));
        if(pEnemy) {
            pEnemy->SyncHP(stHitedNotify.hp());
        }
    } else {
        DummyHero* pRole = dynamic_cast<DummyHero*>(m_pRolesDict->objectForKey(uin));
        if(pRole) {
            pRole->SyncHP(stHitedNotify.hp());
        }
    }
}

void GuardActScene::doDestroyBaseNotify(const CSCVZDestroyBaseNotify& stDestroyBaseNotify) {
    CCLOG("[trace]GuardActScene::doDestroyBaseNotify,destroy base");

    for(int i = 0; i < stDestroyBaseNotify.basehp_size(); ++i) {
        int stat = stDestroyBaseNotify.basehp(i);
        if(BASE_STAT_LOSE == stat || BASE_STAT_DIE == stat || BASE_STAT_LIVE == stat) {
            m_stBaseStat[i] = stat;

			if(stat == BASE_STAT_LIVE){   //第一次进入，同步基地
				addBossByIdx(i);
			}else{
				//基地爆炸
				bossArmBoom(i);
			}
        }
        CCLOG("[trace]GuardActScene::doDestroyBaseNotifybase %d,stat %d", i, stat);
    }

    DummyEnemy* pEnemy = NULL;
    for(int i = 0; i < stDestroyBaseNotify.monsrerids_size(); ++i) {
        const int iEnemyUin = stDestroyBaseNotify.monsrerids(i);

        m_stMyCtrlEnemy.erase(iEnemyUin);

        pEnemy = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(iEnemyUin));
        if(pEnemy) {
			pEnemy->ForceIdle();
			pEnemy->setHP(0);
            pEnemy->Die();
        }
        m_pEnemysDict->removeObjectForKey(iEnemyUin);
        CCLOG("[trace]GuardActScene::doDestroyBaseNotify, set enemy %d die", iEnemyUin);
    }
}

void GuardActScene::addBossByIdx(int iIdx){
	if(iIdx >= PATH_LINE_NUM){
		return;
	}

	if(m_stBossArm[iIdx] != NULL){  //基地已经存在
		return;
	}

	string str = MONSTER_RES_PATH + "ma00142/ma00142.ExportJson";
	CToolUtils::loadArmatureRGBA4444(str.c_str());

	CCArmature* pArm = CCArmature::create("ma00142");
	pArm->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(GuardActScene::haneldBossArmCallBack));
	pArm->getAnimation()->play("stand", -1, -1, 1);
	float fWidth = pArm->getContentSize().width/4;

	map<int,CCPoint >::iterator stIter = m_stID2Point.find(PATH_LINE_NUM + iIdx);
	CCPoint stPos = ccp(fWidth, 500);
	if(stIter != m_stID2Point.end()){
		stPos = ccp(fWidth, stIter->second.y);
	}

	pArm->setPosition(ccp(stPos.x, _sceneForegound->getContentSize().height - stPos.y));
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pArm->getPositionY());
	_sceneForegound->addChild(pArm, iNewZorder);
	m_stBossArm[iIdx] = pArm;
}

void GuardActScene::bossArmBoom(int iIdx){
	if(iIdx >= PATH_LINE_NUM){
		return;
	}

	CCArmature* pArm = m_stBossArm[iIdx];
	if(pArm && pArm->getAnimation()->getCurrentMovementID() == "stand"){
		pArm->getAnimation()->play("die", -1, -1, 0);
	}
}

int GuardActScene::genMonsterCfg(int level, int iWave, MonsterCfg& cfg){
    const string* pstrAvatar = CVZ_MONSTER_WAVE_CFG_MASTER->GetAvatar(iWave + 1);
    if(NULL == pstrAvatar) {
        CCLOG("[warn]GuardActScene::genMonsterCfg, fail to find avatar,wave %d", iWave);
        return 1;
    }
    const CVZMonsterCfg* pLvCfg = CVZMONSTER_CFG_MASTER->GetCfg(level);
    if(NULL == pLvCfg) {
        CCLOG("[warn]GuardActScene::genMonsterCfg, fail to find CVZMonsterCfg,level %d", level);
        return 2;
    }
    MonsterShowCfg* showCfg = MONSTER_SHOW_CFG->GetMonsterShowCfgByShowID(*pstrAvatar);
    if(NULL == showCfg) {
        CCLOG("[warn]GuardActScene::genMonsterCfg, fail to find MonsterShowCfg,avatar %s", pstrAvatar->c_str());
        return 3;
    }

    cfg.set_id(0);
    cfg.set_monstername(showCfg->monstername());
    cfg.set_desc(showCfg->desc());
    cfg.set_avatar(*pstrAvatar);
    cfg.set_camp(ROLE_PARTY_HOSTILE);
    cfg.set_monstertype(showCfg->monstertype());
    cfg.set_level(level);
	cfg.set_patrolarea(showCfg->patrolarea());
	cfg.set_floorbuffid(showCfg->floorbuffid());
	cfg.set_hurtcolor(showCfg->hurtcolor());
	cfg.set_initscale(showCfg->initscale());
	cfg.mutable_actions()->set_emerge(showCfg->actions().emerge());

    int attIdx = 0;
    if(showCfg->monstertype() >= 3) {
        attIdx = 1; //boss
    }
    cfg.set_hp(pLvCfg->monsteratt(attIdx).hp());
    cfg.set_attack(pLvCfg->monsteratt(attIdx).atk());
    cfg.set_defence(pLvCfg->monsteratt(attIdx).def());
    cfg.set_atkatt(pLvCfg->monsteratt(attIdx).atkatt());

    cfg.set_atktype(showCfg->atktype());
    cfg.set_deftype(showCfg->deftype());
    cfg.set_ballistic(showCfg->ballistic());

    for(int i =0; i < showCfg->skills_size(); i++){
        cfg.add_skills(showCfg->skills(i));
    }

    cfg.set_dropid(0);
    //cfg.set_walkspeed(showCfg->walkspeed()); //速度
    cfg.set_minwalkspeed(CVZ_MONSTER_WAVE_CFG_MASTER->GetCfgByWave(iWave + 1)->minspeed());
	cfg.set_maxwalkspeed(CVZ_MONSTER_WAVE_CFG_MASTER->GetCfgByWave(iWave + 1)->maxspeed());

    cfg.set_highspeed(showCfg->highspeed());
    cfg.set_emergevoice(showCfg->emergevoice());
    cfg.set_hurtvoice(showCfg->hurtvoice());
    cfg.set_dievoice(showCfg->dievoice());
    cfg.set_dieanimation(showCfg->dieanimation());

    cfg.set_ailuafile("");
    cfg.set_atkatttype(showCfg->atkatttype());

    for(int i =0; i < pLvCfg->defatts_size(); i++){
        DefAttElement* pstDef = cfg.add_defatts();
        pstDef->set_defatt(pLvCfg->defatts(i).defatt());
        pstDef->set_defatttype(pLvCfg->defatts(i).defatttype());
    }

    cfg.set_walktype(showCfg->walktype());
    cfg.set_hurtactioncd(showCfg->hurtactioncd());

    return 0;
}

void GuardActScene::doFail(){
	stopAllMonsters();
    m_bIsActOver = true;
    COMMON_DIALOG->show(TextStr(TEXT_ACT_FAIL), this, NounStr(OK), "", dialogbtneventselector(GuardActScene::doGiveUP));
}
void GuardActScene::doActOver() {
	stopAllMonsters();
    m_bIsActOver = true;
    COMMON_DIALOG->show(TextStr(TEXT_ACT_DONE), this, NounStr(OK), "", dialogbtneventselector(GuardActScene::doGiveUP));
}

void GuardActScene::doTotalContNotify(const int iTotalCont) {
    if(m_pPrizeUI) {
        m_pPrizeUI->updateMyContrNum(iTotalCont);
    }
}

void GuardActScene::doMyContAwardCoinNotify(const int iCoin) {
    if(m_pPrizeUI) {
		m_pPrizeUI->updateMyCoinNum(iCoin);
    }
}

//更新怪物状态和位置
void GuardActScene::doMonstersInfoNotify(const CSCVZMonsters& stMonstersInfo){
    for(int i = 0; i < stMonstersInfo.monsters_size(); ++i) {
        const CSCVZMonsterInfo& oneInfo = stMonstersInfo.monsters(i);
        if(m_stMyCtrlEnemy.count(oneInfo.monsterid())) {
            continue;
        }
        DummyEnemy* pEnemy = dynamic_cast<DummyEnemy*>(m_pEnemysDict->objectForKey(oneInfo.monsterid()));
        if(NULL == pEnemy) { 
            pEnemy = initOneDummyEnemy(oneInfo.monsterid(), 
                ccp(oneInfo.pos().xpos(), oneInfo.pos().ypos()), oneInfo.lv(), oneInfo.wave()); //TODO:设置为CfgID
        } else {
            pEnemy->setPositionX(oneInfo.pos().xpos());
        }
        if(NULL == pEnemy) {
            CCLOG("[error]GuardActScene::doMonstersInfoNotify, monster is null,uin=%u", oneInfo.monsterid());
            return;
        }

		if(pEnemy->IsDead()){
			return;
		}

        //pEnemy->setPosChannel(monsterInfo.channel());
        //pEnemy->setHP(monsterInfo.hp());
        //pEnemy->SetIsControlledByAI(false); //设置是否由AI驱动
        if(vmsg::MONSTER_ACTION_SCENE_WALK == oneInfo.action()) {
            pEnemy->setIsFollowActivate(true);
            pEnemy->WalkWithDirection(ccp(-1, 0));
        } else if(vmsg::MONSTER_ACTION_SCENE_HIT == oneInfo.action()) {
            if(DummyEnemy::EMPTY_SKILL_ID == oneInfo.skillid()) {
                pEnemy->Attack();
            } else {
                pEnemy->AttackWithSkillAtIdx(oneInfo.skillid());
            }
        } else {
            CCLOG("[error]GuardActScene::addMonster,unknow action %d", oneInfo.action());
        }
        pEnemy->setActionStat(oneInfo.action(), oneInfo.pos().xpos());

    }
}

void GuardActScene::doRankInfoNotify(const CSCVZRankInfo& stMonstersInfo) {
    if(m_pRankUI) {
        m_pRankUI->updateRankInfo(stMonstersInfo);
    }
}
void GuardActScene::doWaveTimeNotify(const int iTime) {
    if(m_pRankUI) {
        m_pRankUI->updateTimeTip(iTime);
    }
}

void GuardActScene::handleSkillClickEvent(const string& stEvent, CCObject* pstEventObj) {
    //技能释放
    if (!_mainHero->IsAttackable()){
        //主角处于不可攻击别人的状态，或者正在攻击，或者中了某个buff
        return;
    }
    CCString* strValue = (CCString*)pstEventObj;
    int skillIdx = strValue->intValue();

    if(skillIdx == 2){
        _mainHero->BackDash();
    }else{
        _mainHero->CastSkill(skillIdx);

        if(skillIdx == 1){  //进化技能
            m_uEvolutionCnt++;
        }

        if (skillIdx == 0){
            static unsigned int uWeaponID = 1;
            uWeaponID++;
        }	

        CCPoint direct(getScaleX(), 0);
        P_CITY_CTRL->getProxy()->sendPlayerSkillInSceneRqst(skillIdx, direct, getPosition());
    }
}

void GuardActScene::handleGiveUpLogic(CCObject* pstObj){
    doGiveUP();
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, GIVEUP_FIGHT_EVENT);
}
void GuardActScene::doGiveUP(){
    CCLOG("[trace]GuardActScene::doGiveUP,fail");
    
    this->unscheduleUpdate();
    P_DAILYACT_CTRL->setGuardActScene(NULL);
    SceneManager::sharedSceneManager()->enterScene(P_USER_OBJ->getUserInfo()->sceneID); //回到同屏点
    return;
}

void GuardActScene::btnTouchEvent(CCObject *pSender, TouchEventType type){
	if(_mainHero->IsDizzy()){  //眩晕时不能放技能
		return;
	}

	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroShoot();
		}else if(pSender->m_uID == pauseBtn->m_uID){

		}

		break;
	case TOUCH_EVENT_MOVED:

		break;
	case TOUCH_EVENT_ENDED:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroCeaseShoot();
		}else if(pSender->m_uID == pauseBtn->m_uID){
			COMMON_DIALOG->show(TextStr(TEXT_DAILY_FORCE_QUIT), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(GuardActScene::doGiveUP));
		}
		break;
	case TOUCH_EVENT_CANCELED:
		if (pSender->m_uID == fireBtn->m_uID){
			mainHeroCeaseShoot();
		}else{

		}
		break;

	default:
		break;
	}
}



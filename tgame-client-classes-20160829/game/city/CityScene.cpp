#include "CityScene.h"
#include "cocos-ext.h"
#include "basic/SceneManager.h"
#include "loading/LoadingScreen.h"
#include "basic/HudLayer.h"
#include "vlib/CVRand.h"
#include "instance/InstanceUtils.h"
#include "SceneEffCfgMaster.h"

#include "CityControl.h"
#include "CityProxy.h"
#include "game/task/TaskControl.h"

#include "UserObj.h"
#include "CfgMaster/OSDCfgMaster.h"
#include "CfgMaster/HeroCfgMaster.h"
#include "CfgMaster/StoryCfgMaster.h"
#include "ToolUtils.h"
#include "Macros.pb.h"

#include "game/bag/BagControl.h"
#include "game/bag/BagProxy.h"
#include "fight/LevelControl.h"
#include "CfgMaster/MapCfgMaster.h"

#include "defines.h"
#include "Resources.h"
#include "basic/Multiresolution.h"
#include "game/fight/BgTextureNode.h"
#include "QEnvSet.h"

#include "game/guild/GuildControl.h"
#include "game/guild/GuildListPanel.h"
#include "game/guild/GuildPanel.h"

#include "game/fight/FightControl.h"
#include "game/fight/FightProxy.h"
#include "CfgMaster/WarMapCfgMaster.h"
#include "game/fight/LevelControl.h"

#include "game/evolution/EvolutionControl.h"
#include "game/evolution/EvolutionPanel.h"

#include "fight/FightControl.h"
#include "WarMapCfgMaster.h"

#include "game/task/TaskControl.h"
#include "CfgMaster/BulletCfgMaster.h"
#include "CfgMaster/WeaponCfgMaster.h"
#include "basic/TimeEx.h"
#include "game/chat/AnimSprite.h"

#include "fightendless/FightEndlessControl.h"
#include "rank/RankControl.h"
#include "GlobalTime.h"

#include "AnyEntry.h" // used to test
#include "game/control/AudioManager.h"

#include "plot/PlotControl.h"
#include "CfgMaster/TaskCfgMaster.h"
#include "basic/NounHelper.h"
#include "role/NPCRole.h"
#include "NPCCfgMaster.h"

#include "PopTipLayer.h"

#include "CfgMaster/StoryCfgMaster.h"
#include "CfgMaster/PlayerInitCfgMaster.h"

#include "game/guide/GuideControl.h"
#include "game/vip/VIPControl.h"
#include "basic/GameLayer.h"
#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"
#include "chat/ChatControl.h"
#include "Global.h"
#include "game/warmap/WarMapControl.h"
#include "game/notification/NotificationControl.h"
#include "CityScreenView.h"
#include "game/manual/ManualControl.h"
#include "evtlog/EvtLog.h"
#include "game/refit/RefitControl.h"
#include "game/player/PlayerControl.h"
#include "game/arena/ArenaControl.h"
#include "game/dailyact/DailyActControl.h"
#include "game/qianghun/QianghunControl.h"
#include "game/gen/GenStrengthenControl.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/equiplib/EquipLibControl.h"
#include "game/shop/ShopControl.h"
#include "game/shop/RmbShopPanel.h"
#include "game/act/ActControl.h"
#include "game/login/LoginControl.h"
#include "AutoPopControl.h"

CityScene *CityScene::create() {
	CityScene *pCityScene = new CityScene();

	if(pCityScene && pCityScene->init()) {
		pCityScene->autorelease();
		return pCityScene;
	} else {
		CC_SAFE_DELETE(pCityScene);
		return NULL;
	}
}

CityScene::CityScene()
:m_pPlayer(NULL)
,m_pNpcsDict(NULL)
,m_pAnyEntryBtn(NULL)
,m_pGameLayer(NULL)
,m_stLastDirection(CCPointZero)
,m_bIsArrivedEdge(false)
,m_bIsHasSceneEff(false)
,m_bIsEnterBattlePortal(false)
,m_pstInstanceTransGate(NULL)
{
	
}

CityScene::~CityScene() 
{

    CCLOG("[trace]=====destroy scene:%s=====", typeid(*this).name());
	m_pNpcsDict->removeAllObjects();
	CC_SAFE_RELEASE_NULL(m_pNpcsDict);

    CC_SAFE_RELEASE_NULL(m_pAnyEntryBtn);
	P_GLOBAL->releaseUnusePanel();
}

bool CityScene::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseOSDScene::init());

		m_pNpcsDict = CCDictionary::create();
		m_pNpcsDict->retain();

		CCDirector *pDirector = CCDirector::sharedDirector();
		CCSize screenSize = pDirector->getVisibleSize();
		CCPoint screenOrigin = pDirector->getVisibleOrigin();

		GameLayer *pGameLayer = dynamic_cast<GameLayer*>(this->getLayer(LAYER_ID_GAME));

		if(pGameLayer != NULL) {
			pGameLayer->addHitEventListener(this, hiteventselector(CityScene::gameLayerHitEventHandler));
		}

		P_PLAYER_CTRL;

		P_CHAT_CTRL->init();

		P_PLOT_CTRL->getDialogLayout();

		P_VIP_CTRL;

		P_WAR_MAP_CTRL->init();

		P_REFIT_CTRL->init();

		P_GUILD_CTRL->init();

		UILayer *pWidgetLayer = dynamic_cast<UILayer*>(this->getLayer(LAYER_ID_WIDGET));

		CityScreenView *pCityScreenView = P_CITY_CTRL->getCityScreenView();

		if(pCityScreenView->getParent() != NULL) {
			pCityScreenView->removeFromParent();
		}

		pWidgetLayer->addWidget(pCityScreenView);
		
		bRet = true;
	} while(0);

	return bRet;
}

void CityScene::build() {
	BaseOSDScene::build();

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCSize visibleSize = pDirector->getVisibleSize();
	CCPoint visibleOrigin = pDirector->getVisibleOrigin();

	CCLayer *pGameLayer = getLayer(LAYER_ID_GAME);

	unsigned int uLen = m_pRolesDict->count();

	HudLayer *pHudLayer = dynamic_cast<HudLayer*>(getLayer(LAYER_ID_HUD));
	pHudLayer->setDelegate(this);
	if(pHudLayer->active){
		pHudLayer->Inactive();
		pHudLayer->Active();
	}else{
		pHudLayer->Active();
	}

#ifdef TGAME_SHOW_ANYENTRY
    m_pAnyEntryBtn = UIButton::create();
    //m_pAnyEntryBtn->setScale9Enabled(true);
    //m_pAnyEntryBtn->setSize(CCSizeMake(125, 60));
    m_pAnyEntryBtn->retain();
    m_pAnyEntryBtn->setName("any_entry_btn");
    m_pAnyEntryBtn->setTitleText("AnyEntry");
    m_pAnyEntryBtn->setTitleFontSize(24.0f);
    m_pAnyEntryBtn->loadTextures("share/btn_yellow_up.png", "share/btn_yellow_up.png", 
        "share/btn_yellow_up.png", UI_TEX_TYPE_PLIST);
    m_pAnyEntryBtn->setPosition(ccp(visibleOrigin.x + m_pAnyEntryBtn->getSize().width / 2 + 200, visibleOrigin.y + visibleSize.height - m_pAnyEntryBtn->getSize().height / 2 - 180));
    m_pAnyEntryBtn->addTouchEventListener(this, toucheventselector(CityScene::touchEventHandler));
#endif

}

void CityScene::onEnterTransitionDidFinish() {
	BaseOSDScene::onEnterTransitionDidFinish();

	P_TASK_CTRL->init();
	
	vmsg::CSPlayerCommInScene stHeroInfo;
	stHeroInfo.set_uin(P_USER_OBJ->getUserInfo()->uin);
	stHeroInfo.set_nick(P_USER_OBJ->getUserInfo()->nick);
	stHeroInfo.set_showid(P_USER_OBJ->getUserInfo()->showID);
	stHeroInfo.set_weaponcfgid(P_USER_OBJ->getUserInfo()->weaponCfgID);
	CCPoint stRolePos = ccp(m_stTransitPos.x, DEFAULT_ROLE_POS.y);
	addRole(stHeroInfo, stRolePos, DEFAULT_ROLE_DIRECTION);

	CCObject *pRole = m_pRolesDict->objectForKey(P_USER_OBJ->getUserInfo()->uin);
	m_pPlayer = dynamic_cast<CMainHero*>(pRole);

    P_CITY_CTRL->getProxy()->setOSDSceneInf(this);
	P_CITY_CTRL->getProxy()->sendPlayerChangeSceneRqst(m_uSceneID);
	P_CITY_CTRL->getProxy()->sendPlayerSetPosRqst(DEFAULT_ROLE_DIRECTION, m_pPlayer->getPosition());
	P_CITY_CTRL->getProxy()->sendGetPlayersRqst();

	m_pGameLayer = getLayer(LAYER_ID_GAME);
	P_CITY_CTRL->getCityScreenView()->updateUI();

	UILayer *pWidgetLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_WIDGET));
    if(m_pAnyEntryBtn) {
        pWidgetLayer->addWidget(m_pAnyEntryBtn);
    }
	
	P_LOGIN_CTRL->getProxy()->sendQueryPlayerInfoRqst(P_USER_OBJ->getUserInfo()->uin);
	P_TASK_CTRL->sendQueryTaskRqst();
	P_MANUAL_CTRL->sendQueryManualRqst();
	P_MANUAL_CTRL->sendQueryBuyManualRqst();
	P_TASK_CTRL->sendQueryMilestoneRqst();
	P_EVOLUTION_CTRL->sendQueryEvolutionInfoRqst();
	P_BAG_CTRL->sendQueryBagInfoRqst();
	P_REFIT_CTRL->sendRefitQueryRqst();
	P_ARENA_CTRL->sendArenaQueryRqst();
	P_DAILYACT_CTRL->getProxy()->sendQueryDailyAct();
	P_ACT_CTRL-> sendQueryAllPkgStatusRqst();
	P_QIANGHUN_CTRL->sendQiangHunQueryRqst();
	P_WAR_MAP_CTRL->sendQueryWarMapInfoRqst();

	scheduleUpdate();

	m_uEffStartTime = GlobalTime::getTime();

	unsigned int uTaskNPCID = P_CITY_CTRL->getTaskNPCID();
	if(uTaskNPCID > 0) {
		P_CITY_CTRL->setTaskNPCID(0); // 置0
		moveToNPC(uTaskNPCID);
		
	}

	m_bIsEnterBattlePortal = false;

    POP_TIP_LAYER_MASTER->ClearAllTips(); //reset

}

void CityScene::onExit() {
	BaseOSDScene::onExit();

	P_CITY_CTRL->getProxy()->stopOSD();
	unscheduleUpdate();
	m_bIsArrivedEdge = false;
	m_bIsEnterBattlePortal = false;
    
}


void CityScene::loadHeroRes() {
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
	unsigned int showID = pUserInfo->showID;
	std::string str = HERO_CFG_MASTER->GetHeroCfgByID(showID)->resname();
	string stBodyName;
	//棍哥修改一下，把weaponid加进来
	int iRet = CMainHero::GetResourceNames(pUserInfo->weaponCfgID, showID, stBodyName);
	if (iRet){
		CCLog("no resource name is found for hero %d", showID);
		return;
	}

	string bodyStr = HERO_RES_PATH + stBodyName + "/" + stBodyName + JSON_TYPE_NAME;

	m_stRes.insert(bodyStr);

	loadEffRes();

	m_stTransitPos = DEFAULT_ROLE_POS;
	std::map<int, CCPoint>::const_iterator iter;
	for(iter = m_stNPCData.begin(); iter != m_stNPCData.end(); iter++) {
		if(iter->first == 0){   //传送点
			m_stTransitPos = iter->second;
		}
	}
}

void CityScene::loadEffRes(){
	unsigned int effID = OSD_CFG_MASTER->getOSDCfgByID(getSceneID())->sceneeffectid();
	if(effID == 0){
		m_bIsHasSceneEff = false;
		return;
	}

	m_bIsHasSceneEff = true;

	string effStr = SCENE_EFFECT_CFG->GetSceneEffCfgByID(effID)->effres();
	string str = SCENE_EFF_RES_PATH + effStr + "/" + effStr + JSON_TYPE_NAME;

	m_stRes.insert(str);

	//音效
	if(SCENE_EFFECT_CFG->GetSceneEffCfgByID(effID)->effsound() != "null"){
		str = SOUND_RES_PATH + SCENE_EFFECT_CFG->GetSceneEffCfgByID(effID)->effsound();
		addEffSound(str);
	}

	if(SCENE_EFFECT_CFG->GetSceneEffCfgByID(effID)->nexteffid() != 0){
		effID = SCENE_EFFECT_CFG->GetSceneEffCfgByID(effID)->nexteffid();
		effStr = SCENE_EFFECT_CFG->GetSceneEffCfgByID(effID)->effres();
		str = SCENE_EFF_RES_PATH + effStr + "/" + effStr + JSON_TYPE_NAME;

		m_stRes.insert(str);
	}

	//音效
	if(SCENE_EFFECT_CFG->GetSceneEffCfgByID(effID)->effsound() != "null"){
		str = SOUND_RES_PATH + SCENE_EFFECT_CFG->GetSceneEffCfgByID(effID)->effsound();
		addEffSound(str);
	}
}


void CityScene::loadItemRes(){
	string str = EFFECT_RES_PATH + "ut00076/ut00076" + JSON_TYPE_NAME;
	m_stRes.insert(str);

	str = EFFECT_RES_PATH + "ut00020/ut00020" + JSON_TYPE_NAME;
	m_stRes.insert(str);
}


void CityScene::setWalkHight(){
	m_uWalkHight = OSD_CFG_MASTER->getOSDCfgByID(m_uSceneID)->walkheght();
}

void CityScene::initMap() {
	CBaseMapScene::initMap();
	m_fMidPos = OSD_CFG_MASTER->getOSDCfgByID(getSceneID())->midpos();
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin(); 
	CCSize screenSize = pDirector->getVisibleSize();

	CCSize innerSize = Multiresolution::getInnerSize();

	float scale = screenSize.height / innerSize.height;

	_sceneForegound = CBgTextureNode::create();
	_sceneForegound->retain();

	string str;
	for(int idx = 0; idx < 3; idx++){
		//str = MAP_RES_PATH + m_stForeStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
        //str = "map/mapres/foreres/cb10002/bgfm0.png"
        str = m_stForeStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
		_sceneForegound->AddSubTextureName(str);
	}

	_sceneForegound->ComposeTexture();
	_sceneForegound->setAnchorPoint(ccp(0,0));
	walkableRect = CCRect(0, 0, _sceneForegound->getContentSize().width, m_uWalkHight);

	if(m_stBackStr != "null"){
		_sceneBackground = CBgTextureNode::create();
		_sceneBackground->retain();
		for(unsigned int idx = 0; idx < 3; idx++){
            //string str = MAP_BG_RES_PAHT + m_stBackStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
            str = m_stBackStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
			_sceneBackground->AddSubTextureName(str, true);
		}
		_sceneBackground->ComposeTexture();
		_sceneBackground->setAnchorPoint(ccp(0,0));
		_sceneBackground->setPosition(ccp(0, screenSize.height));
	}

	if(m_stFrontStr != "null"){
		_sceneFrontgound = CBgTextureNode::create();
		_sceneFrontgound->retain();
		for(unsigned int idx = 0; idx < 3; idx++){
            //string str = MAP_QJ_RES_PAHT + m_stFrontStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
            str = m_stFrontStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
			_sceneFrontgound->AddSubTextureName(str);
		}

		_sceneFrontgound->ComposeTexture();
		_sceneFrontgound->setAnchorPoint(ccp(0,0));
	}

	if(m_stMidStr != "null"){  //包含中景层
		_sceneMidground = CBgTextureNode::create();
		_sceneMidground->retain();
		for(int idx = 0; idx < MAP_RES_NUM; idx++){

			string str = m_stMidStr + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
			_sceneMidground->AddSubTextureName(str, true);
		}

		_sceneMidground->ComposeTexture();
		_sceneMidground->setAnchorPoint(ccp(0,0));
		_sceneMidground->setPosition(ccp(0, OPTIMAL_HEIGHT - m_fMidPos));
	}

	if(m_stMid2Str != "null"){  //包含中景层
		_sceneMid2ground = CBgTextureNode::create();
		_sceneMid2ground->retain();
		for(int idx = 0; idx < MAP_RES_NUM; idx++){

			string str = m_stMid2Str + "/bgfm" + to_string(idx) + PNG_TYPE_NAME;
			_sceneMid2ground->AddSubTextureName(str, true);
		}

		_sceneMid2ground->ComposeTexture();
		_sceneMid2ground->setAnchorPoint(ccp(0,0));
		_sceneMid2ground->setPosition(ccp(0, OPTIMAL_HEIGHT - m_fMidPos));
	}

	m_pGameLayer = dynamic_cast<CCLayer*>(getLayer(LAYER_ID_GAME));
    if(NULL == m_pGameLayer) {
        CCLOG("[warn]CityScene::initMap,get game layer fail,now create it again...");
        //m_pGameLayer = addLayer(LAYER_ID_GAME);

    }
	m_pGameLayer->removeAllChildren();
	m_pGameLayer->setAnchorPoint(ccp(0, 0));
	//m_pGameLayer->setScale(scale);
	m_pGameLayer->setPosition(screenOrigin);

	if(_sceneFrontgound){
		m_pGameLayer->addChild(_sceneFrontgound, -2);
	}

	m_pGameLayer->addChild(_sceneForegound, -4);

	if(_sceneMidground){
		m_pGameLayer->addChild(_sceneMidground, -6);
	}

	if(_sceneMid2ground){
		m_pGameLayer->addChild(_sceneMid2ground, -8);
	}

	if(_sceneBackground){
		m_pGameLayer->addChild(_sceneBackground, -10);
	}

	m_pGameLayer->setScale(CCDirector::sharedDirector()->getVisibleSize().height / OPTIMAL_HEIGHT);
	//m_pGameLayer->setScale(scale * 1.5);

	updateMapBlocks();
}

void CityScene::buildMap() {

	// npc
	std::map<int, CCPoint>::const_iterator iter;

	for(iter = m_stNPCData.begin(); iter != m_stNPCData.end(); iter++) {
		if(iter->first == 0){   //传送点
			//添加副本传送点
			CCPoint stPos = iter->second;
			float posY = _sceneForegound->getContentSize().height - stPos.y;
			addInstanceTransGate(ccp(stPos.x, posY));  //这个点要在配置中刷
		}else{
			addNPC(iter->first, iter->second);
		}
	}

	//test 
	CCFileUtils *pFileUtils = CCFileUtils::sharedFileUtils();
	std::string pathRoot = pFileUtils->getWritablePath();

	pathRoot += "tgame";


	/*CCLabelTTF *pTxt = CCLabelTTF::create();
	pTxt->setFontSize(48);
	pTxt->setContentSize(CCSizeMake(200, 200));
	pTxt->setPosition(ccp(300, 300));
	addChild(pTxt);
	pTxt->setString("TestShadow");
	pTxt->enableShadow(CCSizeMake(2,-2), 1.0f, 1.0f);

	CCLabelTTF *pTxt1 = CCLabelTTF::create();
	pTxt1->setFontSize(48);
	pTxt1->setContentSize(CCSizeMake(200, 200));
	pTxt1->setPosition(ccp(300, 400));
	addChild(pTxt1);
	pTxt1->setString("TestStroke");
	pTxt1->enableStroke(ccc3(0xff, 0x00, 0x00), 1.0f);*/

}

void CityScene::update(float dt) {
	BaseOSDScene::update(dt);

	updateRoleToScene(dt);

	setViewpointCenter(m_pPlayer->getPosition());

	if(m_bIsHasSceneEff){
		updateSceneEff(); 
	}

	// 可行走区域检测
	CCDictElement *pElmt = NULL;
	CQStageEntity *pRole = NULL;

	CCDICT_FOREACH(m_pRolesDict, pElmt) {
		pRole = dynamic_cast<CQStageEntity*>(pElmt->getObject());
		
		if(pRole != NULL && pRole->getIsPlayingMovement()) {
			checkRolePos(pRole);
		}
	}

	checkTransitGate();

	checkArrivedEdge();
}

void CityScene::updateRoleToScene(float dt){
	static float fInter = 0;

	if(fInter >= 1.0f){  //每隔1s添加一个同屏人物
		fInter = 0;
		addRoleToSceneInter();
	}

	fInter += dt;
}

void CityScene::addRoleToSceneInter(){
	if(m_stRolesQueue.size() <= 0){
		return;
	}

	map<UIN_t, vmsg::CSPlayerPosInScene>::iterator stIter = m_stRolesQueue.begin();

	if(stIter == m_stRolesQueue.end()){
		return;
	}

	UIN_t uin = stIter->first;

	if(m_pRolesDict->objectForKey(uin) != NULL){	//舞台上已经有该人物
		m_stRolesQueue.erase(stIter);
		return;
	}

	CCPoint pos = ccp(stIter->second.scenepoint().xpos(), stIter->second.scenepoint().ypos());
	CCPoint direct = ccp(stIter->second.direct().xpos(), stIter->second.direct().ypos());
	addRole(stIter->second.comm(), pos, direct);
	m_stRolesQueue.erase(stIter);
}

void CityScene::updateSceneEff(){
	if(_sceneBackground == NULL){
		return;
	}

	int uNow = GlobalTime::getTime();
	if(uNow - m_uEffStartTime >= EFFECT_INTER){  //超过间隔时间
		m_uEffStartTime = uNow;		//不管是否触发都重置时间
		unsigned int uRand = CRandom::GetRandHaveMax(100);

		if(uRand <= 75){  //一般的概率触发
			CSceneEffect* m_pEffect = CSceneEffect::create();
			m_pEffect->LoadRes(1);

			CCPoint stWorldPos = _sceneForegound->convertToWorldSpace(ccp(1480, 400));
			CCPoint stBackPos = _sceneBackground->convertToNodeSpace(stWorldPos);

			m_pEffect->setPosition(stBackPos);
			_sceneBackground->addChild(m_pEffect);
			if(m_pEffect->getEffType() == EFFECT_TYPE_YUNSHI){
				m_pEffect->AddEventListener(this, CSceneEffect::SHAKE_SCREEN_KEY_FRAME, eventListner_selector(CityScene::handleShakeKeyFrameEvent));
				m_pEffect->AddEventListener(this, CSceneEffect::DUST_PLAY_KEY_FRAME, eventListner_selector(CityScene::handleDustKeyFrameEvent));
				m_pEffect->AddEventListener(this, CSceneEffect::SCENE_EFFECT_SOUND_FRAME, eventListner_selector(CityScene::handleEffSoundKeyFrameEvent)); //播放声音的
			}

			m_pEffect->play(false);
		}
	}
}

void CityScene::handleShakeKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	CInstanceUtils::ShakeScene(this, 1, 10);
}

void CityScene::handleDustKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	CSceneEffect* pstParentEff = dynamic_cast<CSceneEffect*>(pstEventObj);
	CSceneEffect* pstEff = CSceneEffect::create();
	pstEff->LoadRes(pstParentEff->getNextEffID());

	pstEff->setPosition(m_pPlayer->getPosition() + ccp(-50, 50));
	_sceneForegound->addChild(pstEff, m_pPlayer->getZOrder() + 5);

	pstEff->play(false);
}

void CityScene::handleEffSoundKeyFrameEvent(const string& stEvent, CCObject* pstEventObj){
	CSceneEffect* pstEff = dynamic_cast<CSceneEffect*>(pstEventObj);
	if (pstEff->m_stCfg.effsound().length() && pstEff->m_stCfg.effsound() != "null"){
		AudioManager::playEffect(pstEff->m_stCfg.effsound().c_str());
	}
}


void CityScene::updateBgViewCenter(float foreX) {
	float posX = 0.0f;
	float posY = 0.0f;
	
	if(m_pPlayer->getPositionX() - m_pPlayer->getCenterToSides() <= walkableRect.getMinX()) {
		posX = walkableRect.getMinX() + m_pPlayer->getCenterToSides();
	} else if(m_pPlayer->getPositionX() + m_pPlayer->getCenterToSides() >= walkableRect.getMaxX()) {
		posX = walkableRect.getMaxX() - m_pPlayer->getCenterToSides();
	} else {
		posX = m_pPlayer->getPositionX();
	}

	if(m_pPlayer->getPositionY() <= walkableRect.getMinY()) {
		posY = walkableRect.getMinY();
	} else if(m_pPlayer->getPositionY() >= walkableRect.getMaxY()) {
		posY = walkableRect.getMaxY();
	} else {
		posY = m_pPlayer->getPositionY();
	}

	m_pPlayer->setPosition(ccp(posX, posY));

	updateBgTextureByRect(_sceneForegound);

	if(_sceneBackground){
		float bgViewX = foreX * BG_RATIO;
		_sceneBackground->setPositionX(bgViewX);
		updateBgTextureByRect(_sceneBackground, true);
	}

	if(_sceneFrontgound){
		float fgViewX = foreX * FG_RATIO;
		_sceneFrontgound->setPositionX(fgViewX);
		updateBgTextureByRect(_sceneFrontgound);
	}

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

void CityScene::didChangeDirectionTo(HudLayer *hud, cocos2d::CCPoint direction)
{
    if(NULL == m_pPlayer) {
        return;
    }
	if(m_stLastDirection.equals(CCPointZero) || !m_stLastDirection.equals(direction)) {
		m_stLastDirection = direction;
		direction = direction * 10000;
		m_pPlayer->stateChangeByMove(direction);
		P_CITY_CTRL->getProxy()->sendPlayerMoveRqst(direction, m_pPlayer->getPosition());
		//CCLOG("osd move send %s", TimeEx::getTimeStr().c_str());
	}
}

void CityScene::isHoldingDirection(HudLayer *hud, cocos2d::CCPoint direction)
{
    if(NULL == m_pPlayer) {
        return;
    }
	m_pPlayer->stateChangeByMove(direction);
}

void CityScene::simpleDPadTouchEnded(HudLayer *hud)
{
    if(NULL == m_pPlayer) {
        return;
    }
	if(!m_bIsArrivedEdge) {
		m_pPlayer->stateChangeByReleaseHud();
		CCPoint direction = m_pPlayer->getDstDirection() * 10000;
		P_CITY_CTRL->getProxy()->sendPlayerSetPosRqst(direction, m_pPlayer->getPosition());
		//CCLOG("osd move end===== %s", TimeEx::getTimeStr().c_str());
	}
	
}

void CityScene::cleanRes(){
	BaseOSDScene::cleanRes();

}

NPCRole* CityScene::getNPC(unsigned int uNPCID) {
	return dynamic_cast<NPCRole*>(m_pNpcsDict->objectForKey(uNPCID));
}

void CityScene::addNPC(unsigned int uNPCID, const CCPoint &pos) {
	NPCRole *pNPC = NULL;

	if(m_pNpcsDict->objectForKey(uNPCID) == NULL) { // 找不到
		pNPC = createNPC(uNPCID);
		pNPC->updateTaskState();
		m_pNpcsDict->setObject(pNPC, uNPCID);
		float posY = _sceneForegound->getContentSize().height - pos.y;
		pNPC->setPosition(ccp(pos.x, posY));
		int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, pNPC->getPositionY()) - 30;
		_sceneForegound->addChild(pNPC, iNewZorder);
	}
}

void CityScene::removeNPC(unsigned int uNPCID) {
	NPCRole *pNPC = dynamic_cast<NPCRole*>(m_pNpcsDict->objectForKey(uNPCID));

	if(pNPC != NULL) {
		m_pNpcsDict->removeObjectForKey(uNPCID);
		_sceneForegound->removeChild(pNPC);
		pNPC->release();
	}
}

void CityScene::moveToNPC(unsigned int uNPCID) {
	NPCRole *pNPC = dynamic_cast<NPCRole*>(m_pNpcsDict->objectForKey(uNPCID));

	if(pNPC != NULL) { // npc在场景中
		const CCPoint &npcPos = pNPC->getPosition();
		const CCPoint &playerPos = m_pPlayer->getPosition();

		if(!walkableRect.containsPoint(npcPos)) {
			CCLOG("fuck... npc outside of walkable rect");
		}
		
		if(playerPos.getDistance(npcPos) < 40.0f) {
			onArrivedNPC("walk_done", m_pPlayer);
		} else {
			moveRoleToDestination(P_USER_OBJ->getUserInfo()->uin, npcPos, ccp(0, 0));
			m_pPlayer->AddEventListener(this, "walk_done", eventListner_selector(CityScene::onArrivedNPC));
		}
		
	}
}

void CityScene::onArrivedNPC(const std::string &evt, CCObject *pSender) {
	bool isArriveNPC = false;
	unsigned int uNPCID = 0;
	CCDictElement *pElmt = NULL;

	CCDICT_FOREACH(m_pNpcsDict, pElmt) {
		NPCRole *pNPC = dynamic_cast<NPCRole*>(pElmt->getObject());

		float distance = m_pPlayer->getPosition().getDistance(pNPC->getPosition());

		if(distance <= 40.0f) {
			uNPCID = pElmt->getIntKey();
			P_CITY_CTRL->setArriveNPCID(uNPCID);
			isArriveNPC = true;
			break;
		}
	}

	if(isArriveNPC) {
		if(P_TASK_CTRL->isTaskNPC(uNPCID)) {
			//P_TASK_CTRL->sendTalkWithNPC(uNPCID);
			// 检查任务等级限制
			const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();
			const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(stTask.taskid());
			unsigned int uTaskStat = stTask.taskstat();

			if(pTaskCfg == NULL) {
				return;
			}

			if(uTaskStat == vmsg::TASK_STAT_ORG && P_USER_OBJ->getUserInfo()->level < pTaskCfg->level()) {
				char text[100];
				snprintf(text, 100, TextStr(TEXT_TASK_LEVEL_UNREACHED), pTaskCfg->level());
				COMMON_DIALOG->show(text);
			} else {
				if(P_GUIDE_CTRL->isGuideDuring() && P_GUIDE_CTRL->getGuideID() != GUIDE_ID_FIRST_TASK) {
					return;
				}
				showPlotDialog();
			}
		}
	}
}

void CityScene::updateNPCTaskState(unsigned int uNPCID){
	NPCRole *pNPC = dynamic_cast<NPCRole*>(m_pNpcsDict->objectForKey(uNPCID));

	if(pNPC != NULL) { // npc在场景中
		pNPC->updateTaskState();
	}
}

void CityScene::checkRolePos(CQStageEntity *pRole) {
	float posX = 0.0f;
	float posY = 0.0f;

	if(pRole->getPositionX() - pRole->getCenterToSides() <= walkableRect.getMinX()) {
		posX = walkableRect.getMinX() + pRole->getCenterToSides();
	} else if(pRole->getPositionX() + pRole->getCenterToSides() >= walkableRect.getMaxX()) {
		posX = walkableRect.getMaxX() - pRole->getCenterToSides();
	} else {
		posX = pRole->getPositionX();
	}

	if(pRole->getPositionY() <= walkableRect.getMinY()) {
		posY = walkableRect.getMinY();
	} else if(pRole->getPositionY() >= walkableRect.getMaxY()) {
		posY = walkableRect.getMaxY();
	} else {
		posY = pRole->getPositionY();
	}

	pRole->setPosition(ccp(posX, posY));

}

NPCRole* CityScene::createNPC(unsigned int uNPCID) {
	NPCCfg* stCfg = NPC_CFG_MASTER->getNPCCfgByID(uNPCID);
	string str = NPC_RES_PATH + stCfg->showid() + "/" + stCfg->showid() + JSON_TYPE_NAME;

	addResToList(str);

	NPCRole *pNPC = NPCRole::create();
	pNPC->setCfgID(uNPCID);
	pNPC->LoadActionRes();
	return pNPC;
}

void CityScene::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pAnyEntryBtn->getName()) == 0) {
				AnyEntry::exec(); // 测试代码写到AnyEntry.h
			}
			break;
	}
}

void CityScene::dialogEventHandler(CCObject *pSender, unsigned int uStoryID) {
	DialogLayout *pDialogLayout = dynamic_cast<DialogLayout*>(pSender);

	const StoryCfg *pStoryCfg = STORY_CFG_MASTER->getStoryCfg(uStoryID);
	const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();

	if(pStoryCfg != NULL) {
		std::string url = "";

		switch(pStoryCfg->storytype()) {
			case STORY_RECV_TASK:
				if(stTask.taskstat() == vmsg::TASK_STAT_ORG) {
					P_TASK_CTRL->sendRecvTaskRqst();
				}
				break;
			case STORY_COMMIT_TASK:
				if(stTask.taskstat() == vmsg::TASK_STAT_FINISH) {
					P_TASK_CTRL->sendCommitTaskRqst();
				}
				break;
			case STORY_GUIDE_STEP:
				P_GUIDE_CTRL->nextStep();
				break;
			default:;
		}
	}

}

void CityScene::gameLayerHitEventHandler(CCObject *pSender, CCPoint pos) {
	pos = _sceneForegound->convertToNodeSpace(pos);
	CCLOG("pos(%f, %f)", pos.x, pos.y);
	
	unsigned int uNPCID = 0;
	CCDictElement *pElmt = NULL;

	CCDICT_FOREACH(m_pNpcsDict, pElmt) {
		NPCRole *pNPC = dynamic_cast<NPCRole*>(pElmt->getObject());
		CCPoint npcPos = pNPC->getPosition();
		CCPoint npcAnchor = pNPC->getAnchorPoint();
		CCSize npcSize = pNPC->getContentSize();
		CCRect npcRect = CCRectMake(npcPos.x - npcSize.width * 0.5, npcPos.y - npcSize.height * npcAnchor.y, npcSize.width, npcSize.height);

		if(npcRect.containsPoint(pos)) {
			uNPCID = pElmt->getIntKey();
			moveToNPC(uNPCID);
			break;
		}
	}

	if(uNPCID == 0) {
		const CCSize &playerSize = m_pPlayer->getContentSize();
		float minX = walkableRect.getMinX() + playerSize.width / 2;
		float maxX = walkableRect.getMaxX() - playerSize.width / 2;
		float minY = walkableRect.getMinY();
		float maxY = walkableRect.getMaxY();
		float posX = 0.0f;
		float posY = 0.0f;

		if(pos.x > maxX) {
			posX = maxX;
		} else if(pos.x < minX) {
			posX = minX;
		} else {
			posX = pos.x;
		}

		if(pos.y > maxY) {
			posY = maxY;
		} else if(pos.y < minY) {
			posY = minY;
		} else {
			posY = pos.y;
		}

		moveRoleToDestination(P_USER_OBJ->getUserInfo()->uin, ccp(posX, posY), ccp(0, 0));
	}
}

void CityScene::addInstanceTransGate(const CCPoint& stPos){
	m_pstInstanceTransGate = CCArmature::create("ut00076");
	m_pstInstanceTransGate->setPosition(stPos);
	m_pstInstanceTransGate->getAnimation()->play("fly", -1, -1, 1);
	int iNewZorder = CInstanceUtils::GetZOrderByType(ROLE_Z_ORDER, stPos.y) - 200;
	_sceneForegound->addChild(m_pstInstanceTransGate, iNewZorder);
}

void CityScene::checkArrivedEdge() {
	CCRect leftWall = CCRectMake(0, 0, WALL_WIDTH, walkableRect.size.height);
	CCRect rightWall = CCRectMake(walkableRect.size.width - WALL_WIDTH, 0, WALL_WIDTH, walkableRect.size.height);

	const CCPoint &pos = m_pPlayer->getPosition();
	const CCSize &size = m_pPlayer->getContentSize();
	CCRect playerRect = CCRectMake(pos.x - size.width * 0.5, pos.y - size.height * 0.5, size.width, size.height);

	if(!m_bIsArrivedEdge) {
		if(leftWall.intersectsRect(playerRect)) {
			m_bIsArrivedEdge = true;
			prevCity();
		} else if(rightWall.intersectsRect(playerRect)) {
			m_bIsArrivedEdge = true;
			nextCity();
		}
	}
}

void CityScene::prevCity() {
	OSDCfg *pOSDCfg = OSD_CFG_MASTER->getPrevOSDCfg(m_uSceneID);

	if(pOSDCfg != NULL && pOSDCfg->sceneid() != m_uSceneID) {
		SceneManager::sharedSceneManager()->enterScene(pOSDCfg->sceneid());
		P_USER_OBJ->getMutableUserInfo()->sceneID = pOSDCfg->sceneid();
		P_CITY_CTRL->getProxy()->sendPlayerChangeSceneRqst(pOSDCfg->sceneid());
	} else {
		m_bIsArrivedEdge = false;
	}
}

void CityScene::nextCity() {
	OSDCfg *pOSDCfg = OSD_CFG_MASTER->getNextOSDCfg(m_uSceneID);

	if(pOSDCfg != NULL && pOSDCfg->sceneid() != m_uSceneID && P_CITY_CTRL->isSceneOpen(pOSDCfg->sceneid())) {
		SceneManager::sharedSceneManager()->enterScene(pOSDCfg->sceneid());
		P_USER_OBJ->getMutableUserInfo()->sceneID = pOSDCfg->sceneid();
		P_CITY_CTRL->getProxy()->sendPlayerChangeSceneRqst(pOSDCfg->sceneid());
	} else {
		m_bIsArrivedEdge = false;
	}
}

void CityScene::checkTransitGate(){
	if(m_pstInstanceTransGate != NULL) {
		if(fabs(m_pPlayer->getPositionX() - m_pstInstanceTransGate->getPositionX()) < 50 
		&& (m_pPlayer->getPositionY() >= m_pstInstanceTransGate->getPositionY() 
		&& m_pPlayer->getPositionY() <= m_pstInstanceTransGate->getPositionY() + 30)) {
				//主角正在传送点，判断是否需要打开传送面板
				if(!m_bIsEnterBattlePortal) {
					m_bIsEnterBattlePortal = true;
					//P_LEVEL_CTL->getChoosePanel()->open();
					if(P_GUIDE_CTRL->isGuideDuring() && P_GUIDE_CTRL->getGuideID() != GUIDE_ID_FIRST_TASK) {
						return;
					}

					P_WAR_MAP_CTRL->getWarMapPanel()->open();
				}

		}else{
			m_bIsEnterBattlePortal = false;
		}
	}
	
}


void CityScene::setActEntryLayoutStat(const bool bIsShow) {
    
   /* UILayer *pUILayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_UI));
    if(NULL == pUILayer) {
        CCLOG("[error]CityScene::showActEntryLayout, ui layer is null");
        return;
    }

    if(bIsShow) {
        if(m_pActEntryLayout->getParent() == NULL) {
            pUILayer->addWidget(m_pActEntryLayout);
        } else if(m_pActEntryLayout->getParent() != pUILayer) {
            m_pActEntryLayout->removeFromParent();
            pUILayer->addChild(m_pActEntryLayout);
        }
    } else {
        m_pActEntryLayout->removeFromParent();
    }
*/
}

void CityScene::moveToBattlePortal() {
	if(m_bIsEnterBattlePortal){
		m_bIsEnterBattlePortal = false;
    }

    P_WAR_MAP_CTRL->getWarMapPanel(); //自动寻路过去的

	const CCPoint &portalPos = m_pstInstanceTransGate->getPosition();
	moveRoleToDestination(P_USER_OBJ->getUserInfo()->uin, portalPos, ccp(0, 0));
}

void CityScene::moveToScene(unsigned int uSceneID) {
	OSDCfg *pOSDCfg = NULL;

	pOSDCfg = OSD_CFG_MASTER->getNextOSDCfg(m_uSceneID);

	if(pOSDCfg != NULL && pOSDCfg->sceneid() == uSceneID) {
		// move to right edge of this scene, and then enter next scene
		CCPoint endPos = ccp(walkableRect.size.width - WALL_WIDTH / 2, walkableRect.getMinY() + walkableRect.size.height / 2);
		moveRoleToDestination(P_USER_OBJ->getUserInfo()->uin, endPos, ccp(0, 0));
		return;
	}

	pOSDCfg = OSD_CFG_MASTER->getPrevOSDCfg(m_uSceneID);

	if(pOSDCfg != NULL && pOSDCfg->sceneid() == uSceneID) {
		// move to left edge of this scene, and then enter prev scene
		CCPoint endPos = ccp(walkableRect.getMinX() + WALL_WIDTH / 2, walkableRect.getMinY() + walkableRect.size.height / 2);
		moveRoleToDestination(P_USER_OBJ->getUserInfo()->uin, endPos, ccp(0, 0));
		return;
	}

}

void CityScene::showPlotDialog() {
	const vmsg::CSTask &stTask = P_TASK_CTRL->getCurTask();
	if(!stTask.has_taskid()) {
		return;
	}
    const int iTaskID = stTask.taskid();

    const TaskCfg *pTaskCfg = TASK_CFG_MASTER->getTaskCfgByID(stTask.taskid());
    if(NULL == pTaskCfg) {
        CCLOG("[warn]CityScene::showPlotDialog, fail to get task cfg,id %d", stTask.taskid() );
        return;
    }
    unsigned int uStoryID = 0;
    bool bIsForGetTask = false; //是否指引接取任务
    if(stTask.taskstat() == vmsg::TASK_STAT_ORG) {
        uStoryID = pTaskCfg->recvstoryid();
        bIsForGetTask = true;
    } else if(stTask.taskstat() == vmsg::TASK_STAT_FINISH) {
        uStoryID = pTaskCfg->commitstoryid();
    }
	CCLOG("show story=%d", uStoryID);
    P_PLOT_CTRL->setStoryID(uStoryID);
    P_PLOT_CTRL->getDialogLayout()->addDialogEventListener(this, dialog_selector(CityScene::dialogEventHandler));
    P_PLOT_CTRL->showDialog();

    /*
    //TODO:新手引导开启后再添加
    //记录事件ID
    int iEvtID = 0;
    switch(uStoryID) {
    case 1:
        break;
    }

    if(iEvtID > 0) {

        ADD_EVT_LOG(iEvtID); //游戏事件日志
    }
    */
}

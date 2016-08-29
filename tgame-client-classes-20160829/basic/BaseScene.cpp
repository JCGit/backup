#include "BaseScene.h"

#include "CfgMaster/SceneCfgMaster.h"
#include "control/AudioManager.h"
#include "set/SetSysControl.h"

#include "HudLayer.h"
#include "UIBoxLayer.h"
#include "GameLayer.h"

BaseScene::BaseScene()
:m_uSceneID(0)
,m_uSceneType(0)
,m_strSceneName("")
{
	//setScale(720/480);  //调整自己的大小，场景上的所有资源都是按照480来输出了
	m_stRes.clear();
}

BaseScene::~BaseScene()
{
}

bool BaseScene::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!CCScene::init());

		bRet = true;
	} while(0);

	return bRet;
}

void BaseScene::onExit() {
	CCScene::onExit();

	unscheduleUpdate();
}

void BaseScene::addLayer(ELayerID eLayerID) {
	if(getChildByTag(eLayerID) == NULL) { // 不重复添加
		CCLayer *pLayer = createLayer(eLayerID);
		int nZOrder = getLayerZOrder(eLayerID);
		pLayer->setTouchPriority(100-nZOrder); // priority越小越先响应事件
		addChild(pLayer, nZOrder, eLayerID);

		
	}
}

void BaseScene::removeLayer(ELayerID eLayerID) {
	CCLayer *pLayer = getLayer(eLayerID);

	if(pLayer != NULL) { // 有才移除
		removeChild(pLayer);
	}
}

CCLayer* BaseScene::getLayer(ELayerID eLayerID) {
	CCLayer *pLayer = NULL;
	CCNode *pNode = getChildByTag(eLayerID);

	if(pNode != NULL) {
		pLayer = dynamic_cast<CCLayer*>(pNode);
	} else {
		CCLOG("[warn]BaseScene::getLayer,fail to get layer id %d,this=[%s]", eLayerID, typeid(*this).name());
	}

	return pLayer;
}

int BaseScene::getLayerZOrder(ELayerID eLayerID) {
	int nZOrder = 0;
	
	if(eLayerID == LAYER_ID_GAME) {
		nZOrder = 0;
	} else if(eLayerID == LAYER_ID_WIDGET) {
		nZOrder = 1;
	} else if(eLayerID == LAYER_ID_HUD) {
		nZOrder = 2;
	} else if(eLayerID == LAYER_ID_FIGHT) {
		nZOrder = 3;
	} else if(eLayerID == LAYER_ID_UI) {
		nZOrder = 4;
	} else if(eLayerID == LAYER_ID_POP) {
		nZOrder = 5;
	} else if(eLayerID == LAYER_ID_TOP) {
		nZOrder = 6;
	}

	return nZOrder;
}

unsigned int BaseScene::getSceneID() const {
	return m_uSceneID;
}

void BaseScene::loadCfg(unsigned int uSceneID){
	SceneCfg *pSceneCfg = SCENE_CFG_MASTER->getSceneCfgByID(uSceneID);

	m_uSceneID = pSceneCfg->sceneid();
	m_uSceneType = pSceneCfg->scenetype();
	m_strSceneName = pSceneCfg->scenename();
}

unsigned int BaseScene::getSceneType() const {
	return m_uSceneType;
}

void BaseScene::loadRes() {
	
}

void BaseScene::build() {

}

void BaseScene::cleanRes(){

}

void BaseScene::addResToList(string str){
	if(m_stRes.find(str) == m_stRes.end()){
		m_stRes.insert(str);
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(str.c_str());
	}
}

void BaseScene::addEffSound(string str){
	if(m_stEffSoudnRes.find(str) == m_stEffSoudnRes.end()){
		m_stEffSoudnRes.insert(str);
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(str.c_str());
		AudioManager::preloadEffect(str.c_str());
	}
}


void BaseScene::cleanUI() {
    
	UIBoxLayer *pUILayer = dynamic_cast<UIBoxLayer*>(getLayer(LAYER_ID_UI));

	if(pUILayer != NULL) {
		pUILayer->removeAllBoxes();
	}
}

CCLayer* BaseScene::createLayer(ELayerID eLayerID) {
	CCLayer* pLayer = NULL;

	if(eLayerID == LAYER_ID_HUD) {
		CCDirector *pDirector = CCDirector::sharedDirector();
		CCSize visibleSize = pDirector->getVisibleSize();
		CCPoint visibleOrigin = pDirector->getVisibleOrigin();

        //摇杆层
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/city.plist");
		CCSprite *spRockerBG=CCSprite::create();//摇杆背景
        spRockerBG->initWithSpriteFrameName("city/bg_rocker_down.png");

		/*float scale = visibleSize.height / (3 * spRockerBG->getContentSize().height);
		spRocker->setScale(scale);
		spRockerBG->setScale(scale); */
		float dy = 150;

		//是否移动的
		bool bIsHudMove = !P_SET_CTRL->isSet(SetSysControl::HUD_ROCKER_STILL_TYPE);
		HudLayer *pHudLayer = HudLayer::HudLayerWithCenter(ccp(visibleOrigin.x + dy, visibleOrigin.y + dy), spRockerBG->getContentSize().height/2, bIsHudMove);//创建摇杆
		pLayer = pHudLayer;
	} else if(eLayerID == LAYER_ID_UI) {
		UIButton *pBackBtn = UIButton::create();
		pBackBtn->setName("back_btn");
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/share.plist");
		pBackBtn->loadTextures("share/btn_back1_up.png", "share/btn_back1_down.png", "", UI_TEX_TYPE_PLIST);
		pLayer = UIBoxLayer::create(pBackBtn);
	} else if(eLayerID == LAYER_ID_GAME) {
		pLayer = GameLayer::create();
		
	} else if(eLayerID == LAYER_ID_WIDGET || eLayerID == LAYER_ID_POP || eLayerID == LAYER_ID_TOP || eLayerID == LAYER_ID_FIGHT) {
		pLayer = UILayer::create();
	} else {
		pLayer = CCLayer::create();
	}
    pLayer->setAccelerometerEnabled(false); //neil mod
	return pLayer;
}

void BaseScene::disableHudLayer() {
    CCLayer* pLayer = getLayer(LAYER_ID_HUD);
    if(NULL == pLayer) {
        return;
    }
    HudLayer* pHud = dynamic_cast<HudLayer*>(pLayer);
    if(pHud) {
        pHud->Inactive();
    }
    removeLayer(LAYER_ID_HUD);
}

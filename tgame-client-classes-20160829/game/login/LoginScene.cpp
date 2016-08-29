#include "LoginScene.h"
#include "Resources.h"
#include "SceneManager.h"
#include "LoginControl.h"
#include "LoginProxy.h"
#include "QEnvSet.h"
#include "control/AudioManager.h"
#include "update/UpdateResScreen.h"
#include "LoginScreenView.h"

#include "CfgMaster/PlayerInitCfgMaster.h"
#include "CfgMaster/HeroCfgMaster.h"
#include "game/role/MainHero.h"
#include "GameInf.h"
#include "ChannelInf.h"


LoginScene::LoginScene()
:m_pBgStart(NULL)
{
    m_pUpdateResScreen = NULL;
}

LoginScene::~LoginScene()
{
	CC_SAFE_RELEASE_NULL(m_pBgStart);
}

LoginScene *LoginScene::create() {
	LoginScene *pRet = new LoginScene();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool LoginScene::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseScene::init());

		addLayer(LAYER_ID_WIDGET);
		addLayer(LAYER_ID_UI);
        addLayer(LAYER_ID_POP);
        addLayer(LAYER_ID_TOP);

		bRet = true;
	} while(0);

	return bRet;
}

void LoginScene::loadRes() {
	BaseScene::loadRes();

	build();
	//SceneManager::sharedSceneManager()->enterScene(this);
}

void LoginScene::build() {
	BaseScene::build();

	//P_LOGIN_CTRL->init();

	CCDirector *pDirector = CCDirector::sharedDirector();
	CCSize visibleSize = pDirector->getVisibleSize();
	CCPoint centerPos = pDirector->getVisibleOrigin() + ccp(visibleSize.width / 2, visibleSize.height / 2);

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00026/ut00026.ExportJson"); //用RGBA8888格式的
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/denglujiemian/denglujiemian.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/denglujiemianqianjin/denglujiemianqianjin.ExportJson");

    //neilwu,mod,do pre load common res
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/start.plist");

}

void LoginScene::onEnterTransitionDidFinish() {
	BaseScene::onEnterTransitionDidFinish();

#ifdef TGAME_USE_AUTO_UPDATE
    m_pUpdateResScreen = UpdateResScreen::create();
    addChild(m_pUpdateResScreen);
#else
    doUpdateResDone();
#endif
    
	std::string stSoundPath = "login_bg_sound.mp3";
	AudioManager::playBackgroundMusic(stSoundPath.c_str(), true);

	/*// 加载枪械资源
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("guns/guns.plist");

	// 加载角色资源
	const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();

	for(int i = 0; i < stPlayerInitCfgSet.playerinitcfg_size(); i++) {
		const PlayerInitCfg &stPlayerInitCfg = stPlayerInitCfgSet.playerinitcfg(i);
		const HeroCfg *pHeroCfg = HERO_CFG_MASTER->GetHeroCfgByID(stPlayerInitCfg.showid());

		if(pHeroCfg != NULL) {
			// 获取资源路径
			unsigned int uShowID = pHeroCfg->showid();
			unsigned int uWeaponCfgID = 58; // 登录指定一把枪
			string bodyName;
			int iRet = CMainHero::GetResourceNames(uWeaponCfgID, uShowID, bodyName);

			if(iRet != 0) {
				CCLOG("Fail to get hero res! ShowID=%u WeaponCfgID=%u", uShowID, uWeaponCfgID);
				return;
			}

			string bodyPath = HERO_RES_PATH + bodyName + "/" + bodyName + JSON_TYPE_NAME;

			// 加载资源
			CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(bodyPath.c_str());
		}
	}*/
}

void LoginScene::onExit() {
	BaseScene::onExit();

    //AudioManager::stopBackgroundMusic(true);
    P_LOGIN_CTRL->finalize();
}

void LoginScene::doUpdateResDone() {
    if(m_pUpdateResScreen) {
        removeChild(m_pUpdateResScreen);
        m_pUpdateResScreen = NULL;
    }

	UILayer *pWidgetLayer = dynamic_cast<UILayer*>(getLayer(LAYER_ID_WIDGET));
	pWidgetLayer->addWidget(P_LOGIN_CTRL->getLoginScreenView());


    P_LOGIN_CTRL->getProxy()->sendGetSvrListHttpRqst(P_GAME_INF->getDirURL());

    
}

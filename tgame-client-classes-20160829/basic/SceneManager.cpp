#include "SceneManager.h"
#include "loading/LoadingScreen.h"
#include "loading/LoadingScene.h"
#include "BaseScene.h"
#include "defines.h"
#include "game/login/LoginScene.h"
#include "game/city/CityScene.h"
#include "game/fight/FightMapScene.h"
#include "fightendless/FightEndlessScene.h"
#include "SceneCfgMaster.h"
#include "dailyact/GuardActScene.h"
#include "guild/GuildThiefDefScene.h"
#include "guild/GuildBalkCleanScene.h"
#include "arena/ArenaFightScene.h"
#include "game/entry/EntryScene.h"
#include "fightendless/EndlessFightScene.h"
#include "nihility/NihilityFightScene.h"
#include "nihility/NihilityGoldScene.h"
#include "fightguide/FightGuideScene.h"
#include "fightguide/FightGuideMapScene.h"
#include "fightguide/FightGuideControl.h"
#include "control/AudioManager.h"
#include "bosswar/BossWarFightScene.h"
//#include "update/UpdateResScene.h"

#include "ToolUtils.h"

static SceneManager *s_pSharedSceneManager = NULL;

using namespace vmsg;

SceneManager *SceneManager::sharedSceneManager() {
	if(s_pSharedSceneManager == NULL) {
		s_pSharedSceneManager = new SceneManager();

		if(s_pSharedSceneManager == NULL || !s_pSharedSceneManager->init()) {
			CC_SAFE_DELETE(s_pSharedSceneManager);
		}
	}

	return s_pSharedSceneManager;
}

void SceneManager::destroyInstance() {
	CC_SAFE_DELETE(s_pSharedSceneManager);
}

SceneManager::SceneManager()
:m_pCurScene(NULL)
,m_pEnteringScene(NULL)
,m_bIsPushEnteringScene(false)
{
    m_bNeedDoLoadNextRes = false;
    m_stSceneLst = CCArray::create();
    m_stSceneLst->retain();
}

SceneManager::~SceneManager() {
	m_pCurScene = NULL;
	m_pEnteringScene = NULL;
    CC_SAFE_RELEASE_NULL(m_stSceneLst);
}

bool SceneManager::init() {
	bool bRet = false;

	do {
		

		bRet = true;
	} while(0);

	return bRet;
}

BaseScene* SceneManager::getCurScene() {
	return m_pCurScene;
}
const std::vector<unsigned int>& SceneManager::getSceneEnterOrderList() const {
	return m_stSceneEnterOrderList;
}

BaseScene* SceneManager::createScene(unsigned int uSceneID){
	BaseScene *pScene = NULL;

	// 获取场景配置
	SceneCfg *pSceneCfg = SCENE_CFG_MASTER->getSceneCfgByID(uSceneID);
	const unsigned int uSceneType = pSceneCfg->scenetype();

	if(uSceneType == SCENE_LOGIN) { // 登录场景
		pScene = LoginScene::create();
	} else if(uSceneType == SCENE_OSD) { // 同屏场景
		pScene = CityScene::create();
	} else if(uSceneType == SCENE_ENTRY) { // 入口场景
		pScene = EntryScene::create();
	} else if(uSceneType == SCENE_BATTLE) { // 战斗场景
		if(uSceneID == FIGHT_GUIDE_SCENE_ID){
			pScene = CFightGuideScene::create();
		}else if(uSceneID == FIGHT_GUIDE_NORMAL_SCENE_ID){
			pScene = CFightGuideMapScene::create();
		}else{
			pScene = CFightMapScene::create();
		}
	} else if(uSceneType == SCENE_ENDLESS){  //无尽模式
		pScene = CEndlessFightScene::create();
	} else if(uSceneType == SCENE_CVZ) {	//日常活动，小浣熊大战僵尸
        pScene = GuardActScene::create();
	} else if(uSceneType == SCENE_THIEF_DEF) {		//帮派活动，小偷保卫战
		pScene = CGuildThiefDefScene::create();
	} else if(uSceneType == SCENE_BALK_CLEAN) {		//帮派活动，障碍清除
		pScene = CGuildBalkCleanScene::create();
	} else if(uSceneType == SCENE_ARENA){			//竞技场战斗场景
		pScene = CArenaFightScene::create();
	} else if(uSceneType == SCENE_NOTHINGWAR){		//虚无知地场景
		pScene = CNihilityFightScene::create();
	} else if(uSceneType == SCENE_COINWAR){			//金币场景
		pScene = CNihilityGoldScene::create();
	} else if(uSceneType == SCENE_WORLD_BOSS){			//boss战
		pScene = CBossWarFightScene::create();
	}  /*else if(uSceneType == SCENE_UPDATE_RES) { //在线更新资源
        pScene = UpdateResScene::create(); //neilwu mod,加载面板移动到LoginScene了
    }*/

	CCAssert(pScene != NULL, "create a scene fail in SceneManager::createScene");

	pScene->loadCfg(uSceneID);

	return pScene;
}

void SceneManager::enterScene(unsigned int uSceneID, bool bIsPush) {
    
	BaseScene *pScene = NULL;

	CCDirector *pDirector = CCDirector::sharedDirector();
	BaseScene *pRunningScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());

	// 要进入的场景即为当前场景
	if(pRunningScene != NULL && uSceneID == pRunningScene->getSceneID()) {
		return;
	}

	// 要进入的场景已push，直接pop
	if(isSceneInStack(uSceneID)) {
		int level = 1; // 根场景的level
		int c = m_stSceneIDStackList.size();

		while (c > 1)
		{
			if(m_stSceneIDStackList.at(c - 1) == uSceneID) {
				level = c;
				break;
			} else {
				m_stSceneIDStackList.pop_back();
			}

			c--;
		}

		pDirector->popToSceneStackLevel(level);

		if(level > 1) {
			m_stSceneIDStackList.pop_back();
		}
	} else {
		pScene = createScene(uSceneID);

		if(isSceneResCached(uSceneID)) {
			pScene->build();
			enterScene(pScene, bIsPush);
		} else {
			if(pRunningScene != NULL) {
                if(LoadingScreen::getInstance()->isOpen()) {
                    LoadingScreen::getInstance()->close();
                }
                CCLayer *pLayer = pRunningScene->getLayer(LAYER_ID_UI);
                UILayer *pUILayer = dynamic_cast<UILayer*>(pLayer);

                if(pUILayer != NULL) {
                    pRunningScene->cleanUI();
                    pRunningScene->disableHudLayer();
                    LoadingScreen::getInstance()->open();
                    //pDirector->drawScene();
                    CCLOG("[trace]SceneManager::enterScene,drawScene");
                }
                CCLOG("[trace]SceneManager::enterScene,loading in scene %d", pRunningScene->getSceneID());
			}else{
				//pDirector->runWithScene(LoadingScene::sharedLoadingScene());
			}

            m_pEnteringScene = pScene;
            m_bIsPushEnteringScene = bIsPush;
            pScene->loadRes();


            m_bNeedDoLoadNextRes = true;
            m_stSceneLst->addObject(pScene);

            /*
            //对比2个场景的资源，从缓存中去掉上一个场景中不会重用的资源
            if(pRunningScene != NULL){
                compareRes(pRunningScene, pScene);				//资源
                compareEffSoundRes(pRunningScene, pScene);		//音效
                if(LoadingScreen::getInstance()->isOpen()) {
                    LoadingScreen::getInstance()->close();
                }
            }

            SceneManager::sharedSceneManager()->enterScene(pScene);
            */
		}
	}
}


void SceneManager::prepareNextSceneRes() {
    if(!m_bNeedDoLoadNextRes) {
        return;
    }
    bool bIsAllDone = m_pEnteringScene->loadNextRes();
    if(bIsAllDone) {
        //对比2个场景的资源，从缓存中去掉上一个场景中不会重用的资源
        BaseScene *pRunningScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
        if(pRunningScene != NULL){
            compareRes(pRunningScene, m_pEnteringScene);				//资源
            compareEffSoundRes(pRunningScene, m_pEnteringScene);		//音效
            if(LoadingScreen::getInstance()->isOpen()) {
                LoadingScreen::getInstance()->close();
            }
        }

        SceneManager::sharedSceneManager()->enterScene(m_pEnteringScene);
        m_stSceneLst->removeAllObjects();
        m_bNeedDoLoadNextRes = false;
    } 
}

void SceneManager::enterScene(BaseScene *pScene, bool isClean, bool bIsPush) {
	if (NULL == pScene)
	{
		CCLOG("[Error]SceneManager::enterScene, scene is null.");
		return;
	}

	CCDirector *pDirector = CCDirector::sharedDirector();

	if(m_pEnteringScene != NULL && m_pEnteringScene->getSceneID() == pScene->getSceneID()) {
		bIsPush = m_bIsPushEnteringScene;
		m_pEnteringScene = NULL;
		m_bIsPushEnteringScene = false;
	}

	if(pDirector->getRunningScene() != NULL) {
		if(bIsPush) {
			pDirector->pushScene(pScene);
		} else {
			pDirector->replaceScene(pScene);
			if(m_stSceneIDStackList.size() > 0){
				m_stSceneIDStackList.pop_back();
			}
		}
	} else {
		pDirector->runWithScene(pScene);
	}

	//清除上个场景不重用的资源
	cleanPreSceneRes(isClean);
	
	m_stSceneIDStackList.push_back(pScene->getSceneID());
	m_pCurScene = pScene;
	m_stSceneEnterOrderList.push_back(pScene->getSceneID());
}

unsigned int SceneManager::backToPrevScene(){
	if(m_stSceneEnterOrderList.size() <= 0){
		CCLOG("[Error]SceneManager::backToPrevScene, scene order list's size is zero.");
		return 0;
	}

	m_stSceneEnterOrderList.pop_back(); // 移除当前场景ID
	unsigned int prevSceneID = m_stSceneEnterOrderList.back();
	enterScene(prevSceneID);
	m_stSceneEnterOrderList.pop_back(); // 返回操作，取消添加场景ID

	return prevSceneID;
}

void SceneManager::addSceneIDToResCachedList(unsigned int uSceneID) {
	m_stSceneResCachedList.push_back(uSceneID);
}

void SceneManager::removeSceneIDFromResCachedList(unsigned int uSceneID) {
	std::vector<unsigned int>::iterator iter;

	for(iter = m_stSceneResCachedList.begin(); iter != m_stSceneResCachedList.end(); iter++) {
		if(*iter == uSceneID) {
			m_stSceneResCachedList.erase(iter);
			break;
		}
	}
}

bool SceneManager::isSceneInStack(unsigned int uSceneID) {
	bool bRet = false;
	std::vector<unsigned int>::const_iterator iter;

	for(iter = m_stSceneIDStackList.begin(); iter != m_stSceneIDStackList.end(); iter++) {
		if(*iter == uSceneID) {
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool SceneManager::isSceneResCached(unsigned int uSceneID) {
	bool bRet = false;
	std::vector<unsigned int>::const_iterator iter;

	for(iter = m_stSceneResCachedList.begin(); iter != m_stSceneResCachedList.end(); iter++) {
		if(*iter == uSceneID) {
			bRet = true;
			break;
		}
	}

	return bRet;
}

void SceneManager::compareRes(BaseScene* sceneA, BaseScene* sceneB){
	set<string> stPreSceneRes = sceneA->getResList();
	set<string> stNextSceneRes = sceneB->getResList();

	m_stNeedCleanRes.clear();
	set<string>::iterator stIter;
	for(stIter = stPreSceneRes.begin(); stIter != stPreSceneRes.end(); stIter++){
		set<string>::iterator nextIter = stNextSceneRes.find(*stIter);

		//资源不重用
		if(nextIter == stNextSceneRes.end()){
			CCLOG("[trace]SceneManager::compareRes,the resoure is no use need to clean %s", (*stIter).c_str());
			m_stNeedCleanRes.insert(*stIter);
		}
	}
}

void SceneManager::compareEffSoundRes(BaseScene* sceneA, BaseScene* sceneB){
	set<string> stPreSceneRes = sceneA->getEffSoundList();
	set<string> stNextSceneRes = sceneB->getEffSoundList();

	m_stNeedCleanEffSoundRes.clear();
	set<string>::iterator stIter;
	for(stIter = stPreSceneRes.begin(); stIter != stPreSceneRes.end(); stIter++){
		set<string>::iterator nextIter = stNextSceneRes.find(*stIter);

		//资源不重用
		if(nextIter == stNextSceneRes.end()){
			CCLOG("[trace]SceneManager::compareRes,the effect sound is no use need to clean %s", (*stIter).c_str());

			m_stNeedCleanEffSoundRes.insert(*stIter);
		}
	}
}

void SceneManager::cleanPreSceneRes(bool isClean){
	if(isClean){
		set<string>::iterator stIter;
		for(stIter = m_stNeedCleanRes.begin(); stIter != m_stNeedCleanRes.end(); stIter++){
			string strPath = *stIter;
			string typeStr = CToolUtils::getResType(strPath);

			if(strcmp(typeStr.c_str(), JSON_TYPE_NAME.c_str()) == 0){   //动画资源
				//CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(strPath.c_str());
                CToolUtils::removeArmatureFile(strPath.c_str());
			} else if(strcmp(typeStr.c_str(), PNG_TYPE_NAME.c_str()) == 0){  //图片资源
				CCTextureCache::sharedTextureCache()->removeTextureForKey(strPath.c_str());
            } else if(strcmp(typeStr.c_str(), PLIST_TYPE_NAME.c_str()) == 0){  //图片资源
                //CCTextureCache::sharedTextureCache()->removeTextureForKey(strPath.c_str());
                //CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(strPath.c_str());
                CToolUtils::removePlistTexFile(strPath.c_str());
            } else {
                CCLOG("[warn]SceneManager::cleanPreSceneRes,ignore clean %s", strPath.c_str());
            }
		}

		for(stIter = m_stNeedCleanEffSoundRes.begin(); stIter != m_stNeedCleanEffSoundRes.end(); stIter++){
			string strPath = *stIter;
			AudioManager::unloadEffect(strPath.c_str());
		}
	}

    /* 
    //2014-6-16,neil add，主角资源会被清理掉orz
    //将引用计数为1的精灵帧从缓存中清除,按顺序先清理动画缓存，然后清理精灵帧，最后是纹理
    CCAnimationCache::purgeSharedAnimationCache();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames(); 
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    */
}
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

	// ��ȡ��������
	SceneCfg *pSceneCfg = SCENE_CFG_MASTER->getSceneCfgByID(uSceneID);
	const unsigned int uSceneType = pSceneCfg->scenetype();

	if(uSceneType == SCENE_LOGIN) { // ��¼����
		pScene = LoginScene::create();
	} else if(uSceneType == SCENE_OSD) { // ͬ������
		pScene = CityScene::create();
	} else if(uSceneType == SCENE_ENTRY) { // ��ڳ���
		pScene = EntryScene::create();
	} else if(uSceneType == SCENE_BATTLE) { // ս������
		if(uSceneID == FIGHT_GUIDE_SCENE_ID){
			pScene = CFightGuideScene::create();
		}else if(uSceneID == FIGHT_GUIDE_NORMAL_SCENE_ID){
			pScene = CFightGuideMapScene::create();
		}else{
			pScene = CFightMapScene::create();
		}
	} else if(uSceneType == SCENE_ENDLESS){  //�޾�ģʽ
		pScene = CEndlessFightScene::create();
	} else if(uSceneType == SCENE_CVZ) {	//�ճ����С��ܴ�ս��ʬ
        pScene = GuardActScene::create();
	} else if(uSceneType == SCENE_THIEF_DEF) {		//���ɻ��С͵����ս
		pScene = CGuildThiefDefScene::create();
	} else if(uSceneType == SCENE_BALK_CLEAN) {		//���ɻ���ϰ����
		pScene = CGuildBalkCleanScene::create();
	} else if(uSceneType == SCENE_ARENA){			//������ս������
		pScene = CArenaFightScene::create();
	} else if(uSceneType == SCENE_NOTHINGWAR){		//����֪�س���
		pScene = CNihilityFightScene::create();
	} else if(uSceneType == SCENE_COINWAR){			//��ҳ���
		pScene = CNihilityGoldScene::create();
	} else if(uSceneType == SCENE_WORLD_BOSS){			//bossս
		pScene = CBossWarFightScene::create();
	}  /*else if(uSceneType == SCENE_UPDATE_RES) { //���߸�����Դ
        pScene = UpdateResScene::create(); //neilwu mod,��������ƶ���LoginScene��
    }*/

	CCAssert(pScene != NULL, "create a scene fail in SceneManager::createScene");

	pScene->loadCfg(uSceneID);

	return pScene;
}

void SceneManager::enterScene(unsigned int uSceneID, bool bIsPush) {
    
	BaseScene *pScene = NULL;

	CCDirector *pDirector = CCDirector::sharedDirector();
	BaseScene *pRunningScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());

	// Ҫ����ĳ�����Ϊ��ǰ����
	if(pRunningScene != NULL && uSceneID == pRunningScene->getSceneID()) {
		return;
	}

	// Ҫ����ĳ�����push��ֱ��pop
	if(isSceneInStack(uSceneID)) {
		int level = 1; // ��������level
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
            //�Ա�2����������Դ���ӻ�����ȥ����һ�������в������õ���Դ
            if(pRunningScene != NULL){
                compareRes(pRunningScene, pScene);				//��Դ
                compareEffSoundRes(pRunningScene, pScene);		//��Ч
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
        //�Ա�2����������Դ���ӻ�����ȥ����һ�������в������õ���Դ
        BaseScene *pRunningScene = dynamic_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
        if(pRunningScene != NULL){
            compareRes(pRunningScene, m_pEnteringScene);				//��Դ
            compareEffSoundRes(pRunningScene, m_pEnteringScene);		//��Ч
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

	//����ϸ����������õ���Դ
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

	m_stSceneEnterOrderList.pop_back(); // �Ƴ���ǰ����ID
	unsigned int prevSceneID = m_stSceneEnterOrderList.back();
	enterScene(prevSceneID);
	m_stSceneEnterOrderList.pop_back(); // ���ز�����ȡ����ӳ���ID

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

		//��Դ������
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

		//��Դ������
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

			if(strcmp(typeStr.c_str(), JSON_TYPE_NAME.c_str()) == 0){   //������Դ
				//CCArmatureDataManager::sharedArmatureDataManager()->removeArmatureFileInfo(strPath.c_str());
                CToolUtils::removeArmatureFile(strPath.c_str());
			} else if(strcmp(typeStr.c_str(), PNG_TYPE_NAME.c_str()) == 0){  //ͼƬ��Դ
				CCTextureCache::sharedTextureCache()->removeTextureForKey(strPath.c_str());
            } else if(strcmp(typeStr.c_str(), PLIST_TYPE_NAME.c_str()) == 0){  //ͼƬ��Դ
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
    //2014-6-16,neil add��������Դ�ᱻ�����orz
    //�����ü���Ϊ1�ľ���֡�ӻ��������,��˳�������������棬Ȼ��������֡�����������
    CCAnimationCache::purgeSharedAnimationCache();
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames(); 
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    */
}
#include "MovieView.h"
#include "BaseScene.h"
#include "ToolUtils.h"
#include "control/AudioManager.h"

MovieView::MovieView()
:m_pDarkLayer(NULL)
{
    m_pArmature = NULL;
    m_pTargetObj = NULL;
    m_pCallbackFunc = NULL;
    m_pEndTip = NULL;
}

MovieView::~MovieView()
{

}

MovieView* MovieView::create() {
	MovieView *pRet = new MovieView();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool MovieView::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UILayout::init());

		CCDirector *pDirector=CCDirector::sharedDirector();
		CCSize screenSize = pDirector->getVisibleSize();
		CCPoint screenOrigin = pDirector->getVisibleOrigin();

		setSize(screenSize);
		setPosition(screenOrigin);
        
		m_pDarkLayer = CCLayerColor::create(ccc4(0x00, 0x00, 0x00, 255)); //纯黑背景
		addNode(m_pDarkLayer);
				
		bRet = true;
	} while(0);

	return bRet;
}

void MovieView::onEnter() {
	UILayout::onEnter();
	setTouchEnabled(true);
    CCLOG("[trace]MovieView::onEnter");
    //CToolUtils::loadArmatureRGBA4444("effect/diyizhang1/diyizhang1.ExportJson");
    //CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/diyizhang1/diyizhang1.ExportJson");

    m_stSceneBgSound = AudioManager::getNowBgSound();
    AudioManager::playBackgroundMusic("movie_bg_enter_game.mp3", true);
}

void MovieView::onExit() {
	UILayout::onExit();
    setTouchEnabled(false);
    CCLOG("[trace]MovieView::onExit");

    if(m_pEndTip) {
        m_pEndTip->removeFromParentAndCleanup(true);
    }
    m_pArmature = NULL;
    m_pTargetObj = NULL;
    m_pCallbackFunc = NULL;
    m_pEndTip = NULL;

    AudioManager::playBackgroundMusic(m_stSceneBgSound.c_str(), true);
    CToolUtils::removeArmatureFile("effect/diyizhang1/diyizhang1.ExportJson");
    CCTextureCache::sharedTextureCache()->removeTextureForKey("ui_bg/movie_click_tip.png");
}

bool MovieView::hitTest(const CCPoint &pt) {
	bool bRet = true; // 默认其上的任何点都不可穿透

	if(m_pEndTip && m_pEndTip->getParent()) {
        endPlay();
    }
	return bRet;
}


void MovieView::initArmature() {
    CCDirector *pDirector = CCDirector::sharedDirector();
    BaseScene *pScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
    if(NULL == pScene) {
        return;
    }
    UILayer *pTopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_TOP));
    if(NULL == pTopLayer) {
        return;
    }
    if(NULL == this->getParent()) {
        pTopLayer->addWidget(this);
    }
    


    if(NULL == m_pArmature) {
        m_pArmature = CCArmature::create("diyizhang1");
    }
    if(m_pArmature->getParent()) {
        return;
    }

    const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
    //m_pArmature->setAnchorPoint(ccp(0.0f, 0.0f));
    m_pArmature->setPosition(ccp(globalSize.width*0.5, globalSize.height*0.5)); //屏幕中位置
    //m_pArmature->setPosition(ccp(0, 0));

    m_pArmature->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(MovieView::onOneFrameDone));
    addNode(m_pArmature);

}

void MovieView::playMovie(const int iMovieType, CCObject* pTargetObj, SEL_MovieDoneCallFunc pCallbackFunc) {
    m_stNowMovieInfo.iNextIdx = 0;
    m_stNowMovieInfo.stAniLst.clear(); //reset
    m_pTargetObj = pTargetObj;
    m_pCallbackFunc = pCallbackFunc;

    switch (iMovieType)
    {
    case MOVIE_ENTER_GAME:
        m_stNowMovieInfo.stAniLst.push_back("Animation1"); 
        break;
    default:
        CCLOG("[warn]MovieView::playMovie,unknow type %d", iMovieType);
        break;
    }
    initArmature();
    playNextFrame();
}

 void MovieView::playNextFrame() {
     if(m_stNowMovieInfo.iNextIdx == m_stNowMovieInfo.stAniLst.size()) {
         showEndTip();
         return;
     }
     m_pArmature->getAnimation()->play(m_stNowMovieInfo.stAniLst[m_stNowMovieInfo.iNextIdx].c_str(), -1, -1, 0); //not loop
 }

 void MovieView::onOneFrameDone(CCArmature * armature, MovementEventType type, const char * name) {
     if (COMPLETE == type){
         m_stNowMovieInfo.iNextIdx++;
         this->scheduleOnce(schedule_selector(MovieView::onFrameDelayDone), 0.8f);
         CCLOG("[trace]MovieView::onOneFrameDone,done %s", name);
     } else if(LOOP_COMPLETE == type) {
     }

 }

 void MovieView::onFrameDelayDone(float dt) {
    playNextFrame();
 }

 void MovieView::endPlay() {
     CCLOG("[trace]MovieView::endPlay");
     removeNode(m_pArmature);
     //先调用回调，再清除
     if(m_pTargetObj && m_pCallbackFunc) {
         (m_pTargetObj->*m_pCallbackFunc)();
     }

     CCDirector *pDirector = CCDirector::sharedDirector();
     BaseScene *pScene = dynamic_cast<BaseScene*>(pDirector->getRunningScene());
     if(NULL == pScene) {
         return;
     }
     UILayer *pTopLayer = dynamic_cast<UILayer*>(pScene->getLayer(LAYER_ID_TOP));
     if(NULL == pTopLayer) {
         return;
     }
     pTopLayer->removeWidget(this);
 }

 void MovieView::showEndTip() {
     if(NULL == m_pEndTip) {
         m_pEndTip = UIImageView::create();
         m_pEndTip->loadTexture("ui_bg/movie_click_tip.png");
         const CCSize globalSize = CCDirector::sharedDirector()->getVisibleSize(); 
         m_pEndTip->setAnchorPoint(ccp(1.0f, 0.0f));
         m_pEndTip->setPosition( ccp(globalSize.width - 20, 20) );
     }

     if(NULL == m_pEndTip->getParent()) {
         addChild(m_pEndTip, 5);
     }
 }


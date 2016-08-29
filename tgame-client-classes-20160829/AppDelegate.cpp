#include "AppDelegate.h"
#include "basic/SceneManager.h"
#include "basic/BaseMapScene.h"
#include "QNetMgr.h"
#include "defines.h"
#include "vlib/base/CVLog.h"
#include "SkillCfgMaster.h"
#include "EffectSoundCfgMaster.h"
#include "QEnvSet.h"
#include "CfgLoader.h"
#include "login/HeartBeat.h"
#include "login/LoginScene.h"
#include "GlobalTime.h"
#include "basic/Multiresolution.h"
#include "control/PanelSoundLoader.h"
#include "game/guide/GuideControl.h"
#include "control/AudioManager.h"

#include "support/zip_support/ZipUtils.h"
#include "GameInf.h"
//#include "vld.h"
#include "evtlog/EvtLog.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    m_bIsBoot = true;
}

AppDelegate::~AppDelegate() 
{
    google::protobuf::ShutdownProtobufLibrary();
	CocosDenshion::SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
	Multiresolution::setDesignResolutionSize(1280.0, 720.0);
	CCSize innerSize = Multiresolution::getInnerSize();
	CCLOG("inner size w=%f h=%f", innerSize.width, innerSize.height);
	CCSize winSize = pDirector->getWinSize();
	CCLOG("win size w=%f h=%f scale=%f", winSize.width, winSize.height, pDirector->getContentScaleFactor());

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 45);

#ifdef TGAME_SHOW_FPS
    pDirector->setDisplayStats(true);
#else
    pDirector->setDisplayStats(false);
#endif


    InitLog(); //初始化日志系统
    
    ADD_EVT_LOG(EVT_LOG_BOOT_GAME); //游戏事件日志

	ZipUtils::ccSetPvrEncryptionKey(0x080b5d31,0x0195f81c,0xddfe3166,0x332189c8); 

	P_GUIDE_CTRL->init();
    
    LoadCfg();

    //面板音效
    CPanelSoundLoader stSoundLoader;
    stSoundLoader.LoadAllPanelSound();

	if(QEnvSet::beck()){
		SceneManager::sharedSceneManager()->enterScene(1);
    } else if(QEnvSet::taddy()) {
		SceneManager::sharedSceneManager()->enterScene(1);
    } else if (QEnvSet::linc()){
		SceneManager::sharedSceneManager()->enterScene(1);
    } else if (QEnvSet::neil()) {
        SceneManager::sharedSceneManager()->enterScene(1); //login
    } else {
        SceneManager::sharedSceneManager()->enterScene(1);        
    }

	CCDirector::sharedDirector()->setGameLoopCallbackFuc(AppDelegate::mainLoopCallback);
    m_bIsBoot = false;

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
	//CCDirector::sharedDirector()->pause();

    // if you use SimpleAudioEngine, it must be pause
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	AudioManager::pauseBackgroundMusic();
	AudioManager::pauseAllEffects();

    //游戏处于后台状态就会调用
    CCLOG("[trace]AppDelegate::applicationDidEnterBackground");
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    
    CCLOG("[trace]AppDelegate::applicationWillEnterForeground,begin");
    //CCTextureCache::sharedTextureCache()->reloadAllTextures(); //android jni/main.cpp里调用过了
    /*
    GlobalTime::updateTime();
    int iPreTime = GlobalTime::getTime();
    sleep(5);
    GlobalTime::updateTime();
    int iNowTime = GlobalTime::getTime();
    CCLOG("[trace]AppDelegate::applicationWillEnterForeground,pretime %d, now time %d", iPreTime, iNowTime);
    */
    CCDirector::sharedDirector()->startAnimation();
	//CCDirector::sharedDirector()->resume();

    // if you use SimpleAudioEngine, it must resume here
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	AudioManager::resumeBackgroundMusic();
	AudioManager::resumeAllEffects();

    if(!m_bIsBoot) {
        CV_INFO(("AppDelegate::applicationWillEnterForeground"));
    } else {
        CCLOG("[trace]AppDelegate::applicationWillEnterForeground");
    }
    //初始化启动时，不检查此选项
    if(!m_bIsBoot && !P_NET_MGR->IsConnect()) {
        //SceneManager::sharedSceneManager()->enterScene(1);
        //CCLog("[trace]AppDelegate::applicationWillEnterForeground,enter login scene.");
        //GameInf::DoConnectionClose();
    }
}

void AppDelegate::mainLoopCallback()
{
    static int iCnt = 0;
    if (iCnt++ >= SOCKET_RECV_INTERVAL)
    {
        iCnt = 0;
        //CCLog("[trace]AppDelegate::mainLoopCallback,do recv."); 
        P_NET_MGR->DoRecv();
        //CCLog("[trace]AppDelegate::mainLoopCallback,after recv."); 
    }

    static int iTimeCnt = 0;
    if (iTimeCnt++ >= 40)
    {
        iTimeCnt = 0;
        //unsigned int iPreTime = GlobalTime::getTime();
        GlobalTime::updateTime();
        //unsigned int iNowTime = GlobalTime::getTime();
        //CCLOG("time=%d", GlobalTime::getTime());
        //int iDeltaTime = iNowTime - iPreTime; //秒
    }

    if(P_NET_MGR->IsConnect()) {
        CCDirector* pDirector = CCDirector::sharedDirector();
        static int iHeartCnt = 0;
        int interval = 60.0/pDirector->getAnimationInterval();
        if(iHeartCnt++ > interval){
            iHeartCnt = 0;
            P_HEART_BEAT->startHeart();
        }
    }
	

    SceneManager::sharedSceneManager()->prepareNextSceneRes();
}


void AppDelegate::InitLog()
{
    CCLog("[trace]AppDelegate::InitLog,start initing...");
    int iLogFileNum = 1;
    int iLogFileSize = 1024000;

    string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    CCLog("[trace]AppDelegate::InitLog,writable path=[%s]", path.c_str() );

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // In android, every programe has a director under /data/data.
    // The path is /data/data/ + start activity package name.
    // You can save application specific data here.
    path.append("tgame.info.log");
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // You can save file in anywhere if you have the permision.
    path.append("tgame.info.log");
#endif


    const char* pInfoLogName = path.c_str();
    const char* pErrorLogName = path.c_str();


    CV_NLOG->Init(ENamed, NULL, pInfoLogName, iLogFileSize, iLogFileNum, true);
    CV_ELOG->Init(ENamed, NULL, pErrorLogName, iLogFileSize, iLogFileNum, false);

    // 设置filter
    string str("time|pid|newline|type|fileline");
    CV_NLOG->SetFormatByStr(str.c_str());
    CV_ELOG->SetFormatByStr(str.c_str());

    // 设置filter
    str = "fatal|error|warning|info|trace";
    CV_NLOG->SetFilterByStr(str.c_str());
    CV_ELOG->SetFilterByStr(str.c_str());

    CV_INFO(("LogFormat=%d LogFilter=%d", CV_NLOG->GetFormat(), CV_NLOG->GetFilter()));
}


void AppDelegate::CheckResPath() {
    //添加本地资源搜索路径
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
    //win下m_writableFolder最后带着 反斜杠
    path += "\\data\\Resources\\";
#else  
    path += "/data/Resources/"; 
#endif
    
    CCFileUtils::sharedFileUtils()->addSearchPath(path.c_str()); //先把更新路径加进去
    CCLOG("[trace]AppDelegate::CheckResPath,writable path=%s", path.c_str() );
}


bool AppDelegate::LoadCfg() {

    CCDirector* pDirector = CCDirector::sharedDirector();
#ifdef TGAME_USE_AUTO_UPDATE
    CheckResPath(); //添加本地资源搜索路径
    //neilwu, 使用在线更新后，这里仅仅 stCfgLoader.LoadInitCfg();
    CCfgLoader stCfgLoader;
    int iRet = 0;
    iRet = stCfgLoader.LoadInitCfg(); //只在内网做在线更新
    if (iRet != 0){
        CCLOG("[error]AppDelegate,fail to init config!");
        pDirector->end();
        CCAssert(0, "fail to init config!");
        return false;
    }
#else
    CCfgLoader stCfgLoader;
    if (stCfgLoader.LoadAllCfg() != 0){
        CCLOG("[error]AppDelegate,fail to init config!");
        pDirector->end();
        CCAssert(0, "fail to init config!");
        return false;
    }
#endif
    return true;
}


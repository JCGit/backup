
#include "stdafx.h"

#include "GamePrecedure.h"

#include "StateMachine.h"
#include "LoadingFrame.h"
#include "LogoFrame.h"
#include "MainFrame.h"
#include "cocos2d.h"
#include "CCScheduler.h"
#include "Language.h"
#include "CCBManager.h"
#include "PacketManager.h"
#include "TimeCalculator.h"
#include "SeverConsts.h"
#include "CharacterConsts.h"

#include "DataTableManager.h"
#include "waitingManager.h"
#include "SoundManager.h"
#include "RestrictedWord.h"
//#include "ThreadSocket.h"
#include "inifile.h"
#include "CustomPage.h"
#include "GamePlatform.h"
#include "GamePlatformInfo.h"
#include "buyingCheck.h"
#include "LoginPacket.h"

#include "script_support/CCScriptSupport.h"
#include "CCLuaEngine.h"
#include "Gamelua.h"
#include "pb.h"
#include "libOS.h"
#include "GameNotification.h"

#include "MessageBoxPage.h"
#include "GamePacketManager.h"
#include "ConfirmPage.h"
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "..\..\cocos2dx\platform\android\jni\JniHelper.h"
#include "../../cocos2dx/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif
//
using namespace cocos2d;

#define PRELOAD(name, count) \
	{static int preloadCount = 0; \
	int preloadMax = count; \
	if(preloadCount<preloadMax){ \
	CCBContainer* ccb = CCBManager::Get()->createAndLoad(name); \
	ccb->retain(); \
	ccblist.push_back(ccb); \
	preloadCount++;}}

//--begin xinzheng 2013-12-12
static ConfigFile* st_pImgsetCfg = NULL;
//--end

static float fLoginJiShiSeconds = 0.f;

static std::list<CCBContainer*> ccblist;

void GamePrecedure::init()
{
	if(mInitialized)
        return;

	GamePlatformInfo::getInstance()->registerPlatform();
	PacketManager::Get()->registerPacketHandler(ASSEMBLE_FINISH_S,this);
	TimeCalculator::Get()->init();
	Language::Get()->init("Lang/chinese.lang");

	m_pScheduler = CCDirector::sharedDirector()->getScheduler();
	m_pScheduler->retain();
	m_pScheduler->scheduleUpdateForTarget(this, 0, false);
    
	buyingCheck::Get()->init();

    mInitialized = true;

	mReadTables = false;

	CCLog("*************GamePrecedure::init finished****************");
}

void GamePrecedure::enterLogoMovie()
{
	if(!mLogoFrame)
	{
		mLoadFrameCcbiPreLoad = CCBManager::Get()->createAndLoad("LoadingFrame.ccbi");
		mLogoFrame = new LogoFrame;
	}

	if(!m_pStateMachine)
		m_pStateMachine = new StateMachine<GamePrecedure>(this);

	m_pStateMachine->ChangeState(mLogoFrame);
}

void GamePrecedure::reEnterLoading()
{
	mInLoadingScene = true;
	fLoginJiShiSeconds = 0.f;	
	mLoginPacketsReceivedCount = 0;
	mIsReEnterLoadingFrame = true;

	LoginPacket::Get()->setEnabled(false);
	PacketManager::Get()->disconnect();
	GamePrecedure::Get()->setLoginAssemblyFinished(false);
	/*
	MainFrame* mainFrame = dynamic_cast<MainFrame*>(CCBManager::Get()->getPage("MainFrame"));
	if (mainFrame)
	{
		CCLOG("mainFrame frame mScene retainCount:%d",mainFrame->getRootSceneNode()->retainCount());
		gameSnapshot();
	}
	*/
	//重登陆时，暂时注释掉了清空所有登录信息，后期有可能会加，by zhenhui 2014/7/30
	//ServerDateManager::Get()->clearLoginAllHandler();
	g_AppDelegate->purgeCachedData();
	//
	reset();
	enterLoading();
}

void GamePrecedure::enterLoading()
{
	if(!mLoadingFrame)
    {
        //mLoadingFrame =  LoadingFrame::create();
		mLoadingFrame = dynamic_cast<LoadingFrame*>(CCBManager::Get()->getPage("LoadingFrame"));
        mLoadingFrame->retain();
    }
    
	if(!m_pStateMachine)
		m_pStateMachine = new StateMachine<GamePrecedure>(this);
	
    m_pStateMachine->ChangeState(mLoadingFrame);
    
    if(mMainFrame)
	{		
		mMainFrame->release();
		mMainFrame = 0;
	}
}


void GamePrecedure::enterMainMenu()
{
	gotoMainScene = true;
    
}

void GamePrecedure::enterInstruction(int id)
{
}

void GamePrecedure::enterGame()
{
}

void GamePrecedure::exitGame()
{
	if(m_pStateMachine)
	{
		//m_pStateMachine->CurrentState()->Exit(this);
		delete m_pStateMachine;
		m_pStateMachine = 0;
	}
	if (mLoadingFrame)
	{
		mLoadingFrame->release();
		if (mLoadFrameCcbiPreLoad)
		{
			mLoadFrameCcbiPreLoad->release();
			mLoadFrameCcbiPreLoad = NULL;
		}
		mLoadingFrame = NULL;
	}
	if(mMainFrame)
	{
		mMainFrame->release();
		mMainFrame = 0;
	}
	PacketManager::Get()->disconnect();
	if(m_pScheduler)
	{
		m_pScheduler->unscheduleUpdateForTarget(this);
		m_pScheduler->release();
		m_pScheduler = 0;
	}
}

void GamePrecedure::update( float dt )
{
    if ( m_pBulletinMgr )
    {
        m_pBulletinMgr->update(dt);
    }
    
	mFrameTime = dt;
	mTotalTime += dt;
	static float durationTime=0;
	durationTime+=dt;
	if(durationTime>1.0f)
	{
		durationTime -=1.0f;
		++mServerTime;
	}
	m_pStateMachine->Update();
	PacketManager::Get()->update(dt);
	MessageManager::Get()->update();
	waitingManager::Get()->update(dt);
	TimeCalculator::Get()->update();
	LoginPacket::Get()->update(dt);	

	// 处理logoframe
	if (mLogoFrame)
	{
		if (mLogoFrame->isLogoFinished())
		{
			enterLoading();
			delete(mLogoFrame);
			mLogoFrame = 0;
		}
	}


	//--begin xinzheng 2013-11-26
	//if (mInLoadingScene)//reRequestPackages

	
	////may be not need to send package
	//{
	//	static int stCount = 0;
	//	++stCount;
	//	std::list<REQUEST_PACKAGE>::iterator it = mRequestPackages.begin();
	//	for (; it != mRequestPackages.end(); ++it)
	//	{
	//		if ((*it).bfirstsent == false)
	//		{
	//			if (stCount%5 == 0)
	//			{
	//				(*it).bfirstsent = true;
	//				//登录之后发包，暂时被注释掉，改为服务器直接推送 by zhenhui 2014/7/30
	//				//PacketManager::Get()->sendPakcet((*it).opcode,(*it).message);
	//				//delete it->second.message;//不能delete，_failedPakcage会用到
	//				++mLoginPacketsReceivedCount;
	//				break;
	//			}
	//			else
	//				break;//需要break保持顺序
	//		}
	//	}
	//}
	//--end
	//--begin xinzheng 2013-12-7
	if (mInLoadingScene)
	{		
		if (mStartLoginJiShi)
		{
			fLoginJiShiSeconds += dt;
			if (fLoginJiShiSeconds > 12.f)
			{
				mStartLoginJiShi = false;
				mLoadingFrame->showEnter();
//				libOS::getInstance()->setWaiting(false);
                mLoadingFrame->hidLoadingAniPage();
				//
				//阻止ontimeout、sendfailed、receivefailed、packeterror等_failedPakcage 发起reconnect
				//超时后有赖玩家手动重新点击进入游戏
				LoginPacket::Get()->setEnabled(false);
				PacketManager::Get()->disconnect();
				//
			}
			else
			{
				int percent = 90 + mLoginPacketsReceivedCount*70 / (mLoginRequestPacketsTotalCount);
				CCLOGERROR("LoginingPackets:%d%%	%f		%d", percent, fLoginJiShiSeconds, mLoginPacketsReceivedCount);
				mLoadingFrame->showLoginingInPercent(percent);
//				libOS::getInstance()->setWaiting(true);
                mLoadingFrame->showLoadingAniPage();
			}
		}
		else
		{
			fLoginJiShiSeconds = 0.f;
		}
	}
	//--end
	static bool sbgo = false;
	
	bool preparePacketDone = false;
	//m_loginPacketAssemblySuccess  代表收到了服务器登录收尾的AssemblyFinish包
	//mHasMainRole 表示新手还没有角色时，如果没有角色，需要进入选角色页面
	preparePacketDone =(m_loginPacketAssemblySuccess | (!mHasMainRole));

	if(gotoMainScene && preparePacketDone && mReadTables && sbgo && mInitLuaDone)
	{
		PacketManager::Get()->removePacketHandler(this);
		LoginPacket::Get()->setEnabled(true);

		while(!ccblist.empty())
		{
			ccblist.front()->release();
			ccblist.pop_front();
		}

		gotoMainScene = false;
		mInLoadingScene = false;
		mStartLoginJiShi = false;
		mGotHeartBeatTime = -10.f;//阻止第一次不应该的生效

		SeverConsts::Get()->exitServerConst();

		if(!mMainFrame)
			mMainFrame = dynamic_cast<MainFrame*>(CCBManager::Get()->getPage("MainFrame"));//MainFrame::create();
		if(mMainFrame)mMainFrame->retain();

		if(!m_pStateMachine)
			m_pStateMachine = new StateMachine<GamePrecedure>(this);
		m_pStateMachine->ChangeState(mMainFrame);


		libOS::getInstance()->setWaiting(false);

		/*modify by zhaozhen for close all music*/

#ifndef _CLOSE_MUSIC
		//bg music is played after the mainframe loading completely modify by xinghui
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
		SoundManager::Get()->playGeneralMusic();
#endif


		if(mLoadingFrame)
		{
			mLoadingFrame->release();
			if (mLoadFrameCcbiPreLoad)
			{
				mLoadFrameCcbiPreLoad->release();
				mLoadFrameCcbiPreLoad = NULL;
			}
			mLoadingFrame = NULL;
		}

		g_AppDelegate->purgeCachedData();
	}
	else if(isInLoadingScene() && SeverConsts::Get()->checkUpdateInfo() == SeverConsts::CS_OK)
	{
		//
		//--begin xinzheng 2013-12-12
		static CCSpriteFrameCache * frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
		static ConfigFile cfg;
		if (st_pImgsetCfg == NULL)
		{
			cfg.load("Imageset.txt");
			st_pImgsetCfg = &cfg;
			frameCache->purgeSpriteFramesAndFileNames();
		}
		static ConfigFile::SettingsMapIterator itr = cfg.getSettingsMapIterator();
		if(itr.hasMoreElements())
		{
			std::string filename = itr.getNext();
			static int ic = 0;
			CCLOG("Imageset%d:%s", ++ic, filename.c_str());
			if(!filename.empty())
				frameCache->addSpriteFramesNameWithFile(filename.c_str());
		}
		else
		{
			sbgo = true;
		}
		//--end
		static int stCount = 0;
		++stCount;
        long mem = libOS::getInstance()->avalibleMemory();//取不准
        if(mem > 3&& sbgo /*&& mStartLoginJiShi*/)
		{
			if (stCount % 3 == 0)
			{
				/*
					得考虑ccbi加载的texture的大小，首次引用texture触发load，大图耗时
					尽量均匀分布耗时，注意不要某一帧特别耗时
				*/
				PRELOAD("MainFrame.ccbi",1);
			}
			else if (stCount % 5 == 0)
			{
				PRELOAD("BattlePage.ccbi",1);//8
			}
			else if (stCount % 7 == 0)
			{
				PRELOAD("MainScene.ccbi",1);//8
				PRELOAD("OfflineBattleAccountPopUp.ccbi",1);
				PRELOAD("GiftPopUp.ccbi",1);
			}
			else if (stCount % 8 == 0)
			{
				PRELOAD("ArenaPage.ccbi",1);//8
			}
			else if (stCount % 11 ==0)
			{
				PRELOAD("EquipmentPage.ccbi",1);
				PRELOAD("EquipmentInfoPopUp.ccbi",1);
			}
        }
		//
    }
	else if(!isInLoadingScene())
	{
		cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
		if(pEngine)
		{
			pEngine->executeGlobalFunctionByName("GamePrecedure_update",this,"GamePrecedure");
		}
		//
		if (mGotHeartBeatTime > 10.f)
		{
			GamePacketManager::Get()->setNeedReConnect();
			mGotHeartBeatTime = 0;
		}
		//
		if(mHeartbeatTime > 10.f)//心跳包
		{
			mHeartbeatTime = 0;
			HPHeartBeat info;
			//info.set_version(1);
			time_t nowstamp = time(0);
			info.set_timestamp(nowstamp);
			PacketManager::Get()->sendPakcet(HEART_BEAT,&info, false);
			mGotHeartBeatTime = 0;
		}
		else
			mHeartbeatTime += dt;
		//
		mGotHeartBeatTime += dt;
		//
	}
}

void GamePrecedure::initLuaEnv(){
	
	//将初始化lua 的时机提前 by zhenhui 2014/8/8
	/*
			初始化和加载lua环境，这个很耗时！！！
		*/
		CCLOG("initLuas");
		if(!cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine())
		{
			cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
			cocos2d::CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
			pEngine->start();
			tolua_Gamelua_open(pEngine->getLuaStack()->getLuaState());
			luaopen_pb(pEngine->getLuaStack()->getLuaState());

			pEngine->executeString("require \"main\"");

			pEngine->executeGlobalFunctionByName("GamePrecedure_preEnterMainMenu",this,"GamePrecedure");

			//修改lua堆栈大小
			int stack_size = 32;
			while (lua_checkstack(pEngine->getLuaStack()->getLuaState(), stack_size))
				stack_size <<= 1;

			CCLog("lua stack size: %d", stack_size >> 1);
		}

		mInitLuaDone = true;
		//有可能需要在这里加入初始化lua 结束指令，发给后端，代表前段已经做好了准备工作，再由他们发assembly 消息。 by zhenhui 2014/8/8

}


float GamePrecedure::getFrameTime()
{
	return mFrameTime;
}

float GamePrecedure::getTotalTime()
{
	return mTotalTime;
}

static time_t s_gotoBackgroundTime = 0;

void GamePrecedure::enterForeGround()
{
    std::string privateLogin = VaribleManager::Get()->getSetting("privatePause","","false");
    if(privateLogin!="true")
        libPlatformManager::getPlatform()->gamePause();
    
    time_t t = time(0);

	if(!GamePrecedure::Get()->getUserKickedByServer())
	{
		CCLOG("GamePrecedure::enterForeGround->LoginPacket::forceSentPacket");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//if (getIsOnTempShortPauseJNI())
		{
			LoginPacket::Get()->setEnabled(true);
			LoginPacket::Get()->forceSentPacket();
		}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		// win32 不需要断线，方便debug
#else
		LoginPacket::Get()->setEnabled(true);
		LoginPacket::Get()->forceSentPacket();
#endif
		

		cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
		if(pEngine)
		{
			pEngine->executeGlobalFunctionByName("GamePrecedure_enterForeground",this,"GamePrecedure");
		}
	}
	else
	{
		MSG_BOX_LAN("@UserKickoutMsg");
	}
}

void GamePrecedure::preEnterMainMenu()
{
	CCLOG("preEnterMainMenu readTables");
	//
	readTables();
	//
	CCLog("init lua environment");
	//提前lua初始化位置，by zhenhui 
	initLuaEnv();
	//ServerDateManager::Get()->clearLoginAllHandler();
	//
	/*std::list<REQUEST_PACKAGE>::iterator it = mRequestPackages.begin();
	for(; it != mRequestPackages.end(); ++it)
	{
		delete (*it).message;
	}
	mRequestPackages.clear();*/
	//
	//requestPackages();
	mStartLoginJiShi = true;
	mLoginPacketsReceivedCount = 0;
	mLoginRequestPacketsTotalCount = AFTER_LOGIN_PACKET_NUM;
	//
	CCLog("preEnterMainMenu end");
}

void GamePrecedure::readTables()
{
	CCLog("readTables begin");
	if (mReadTables)
		return;
	CCLog("Language begin");
	Language::Get()->clear();
	Language::Get()->init("Lang/chinese.lang");
	CCLog("Language end");

	//modified by dylan at 20131108 remove fnt cache
	//cocos2d::FNTConfigRemoveCache();

	CCLog("VaribleManager begin");
    VaribleManager::Get()->reload();//构造函数load，因为playLoadingMusic先用它了，内更新后这里reload它
	unsigned int max_texture_cache_bytes = StringConverter::parseUnsignedInt(VaribleManager::Get()->getSetting("MaxTexCacheBytes", "", "131072000"), 131072000);//125*1024*1024
    CCTextureCache::sharedTextureCache()->setMaxCacheByteSizeLimit(max_texture_cache_bytes);
	g_AppDelegate->setMaxCacheByteSizeLimit(max_texture_cache_bytes);
	CCLog("VaribleManager end");

	//CCLog("ToolTableManager begin");
	//ToolTableManager::Get()->init("Tools.txt");//非构造函数init
	//ToolTableManager::Get()->initIncludeOther();//see ToolTableManager
	//CCLog("ToolTableManager end");

	//CStageConfigManager::Get()->Load("chapters.txt", "stages.txt", "story.txt");
	//CSkillConfigManager::Get()->Load("Skills.txt", "buff.txt");
	//CFavorConfigManager::Get()->Load("disciplefavor.txt", "disciplefavorachiv.txt");
	
	mReadTables = true;//
	CCLog("readTables end");
}
//
//void GamePrecedure::requestPackages()
//{
//	LoginPacket::Get()->setEnabled(false);
//
//	//开始
//	OPAssembleFinish* ass = new OPAssembleFinish;	
//	ass->set_status(0);
//	if (!_requestPackage(OPCODE_ASSEMBLE_FINISH_C,OPCODE_ASSEMBLE_FINISH_S,ass))
//	{
//		delete ass;
//	}
//
//	OPUserBasicInfo *info = new OPUserBasicInfo;
//	info->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_BASIC_INFO_C,OPCODE_GET_USER_BASIC_INFORET_S,info))
//	{
//		delete info;
//	}
//
//	OPCheckNotice *noticeInfo=new OPCheckNotice;
//	noticeInfo->set_version(1);
//	
//	if(!_requestPackage(OPCODE_NOTICE_INFO_C,OPCODE_NOTICE_INFO_S,noticeInfo))
//	{
//		delete noticeInfo;
//	}
//	
//	
//	OPUserDiscipleInfo* discipleInfo = new OPUserDiscipleInfo;//clear
//	discipleInfo->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_DISCIPLE_INFO_C, OPCODE_GET_USER_DISCIPLE_INFORET_S, discipleInfo))
//	{
//		delete discipleInfo;
//	}
//
//	OPUserToolInfo* userToolInfo = new OPUserToolInfo;//clear
//	userToolInfo->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_TOOL_INFO_C, OPCODE_GET_USER_TOOL_INFORET_S, userToolInfo))
//	{
//		delete userToolInfo;
//	}
//
//	OPUserSoulInfo* userSoulInfo = new OPUserSoulInfo;//clear
//	userSoulInfo->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_SOUL_INFO_C, OPCODE_GET_USER_SOUL_INFORET_S ,userSoulInfo))
//	{
//		delete userSoulInfo;
//	}
//
//	OPUserCareerInfo* career = new OPUserCareerInfo;//clear
//	career->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_CAREER_INFO_C,OPCODE_GET_USER_CAREER_INFORET_S,career))
//	{
//		delete career;
//	}
//
//	OPShopList* shop = new OPShopList;
//	shop->set_version(1);
//	
//	if ( ServerDateManager::Get()->getUserBasicInfo().unReadCount )
//	{
//		ServerDateManager::Get()->clearSysMsgCount();
//	}
//	OPUserSysMsgInfo* sysMsgInfo = new OPUserSysMsgInfo;
//	sysMsgInfo->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_SYSMSG_INFO_C,OPCODE_GET_USER_SYSMSG_INFORET_S,sysMsgInfo))
//	{
//		delete sysMsgInfo;
//	}
//    
//#ifdef PROJECTAPPSTORE
//    shop->set_platname("ios_appstore");
//#endif
//
//#ifdef PROJECTAPPSTORETW
//	shop->set_platname("ios_tw");
//#endif
//    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	const PlatformRoleItem* item=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
//	if(item)
//	{
//		shop->set_platname(item->name);
//	}
//	else
//	{
//		shop->set_platname(libPlatformManager::getPlatform()->getClientChannel());
//	}
//#else
//	//shop->set_platname(libPlatformManager::getInstance()->getPlatform()->getClientChannel());
//#endif
//
//	OPUserEquipleInfo* equip = new OPUserEquipleInfo;//clear
//	equip->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_EQUIP_INFO_C,OPCODE_GET_USER_EQUIP_INFORET_S, equip))
//	{
//		delete equip;
//	}
//
//	OPUserTreasureInfo* treasure = new OPUserTreasureInfo;//clear
//	treasure->set_version(1);
//	if (!_requestPackage(OPCODE_TREASURE_INFO_LIST_C,OPCODE_TREASURE_INFO_LIST_S, treasure))
//	{
//		delete treasure;
//	}
//
//	OPUserDragonSoulInfo* dragonSoul = new OPUserDragonSoulInfo;//clear
//	dragonSoul->set_version(1);
//	if (!_requestPackage(OPCODE_GET_ALL_DRAGON_SOULS_C,OPCODE_GET_ALL_DRAGON_SOULS_S, dragonSoul))
//	{
//		delete dragonSoul;
//	}
//
//
//	OPUserBattleArray* userBattleArray = new OPUserBattleArray;//clear
//	userBattleArray->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_BATTLE_ARRAY_INFO_C,OPCODE_GET_USER_BATTLE_ARRAY_INFORET_S, userBattleArray))
//	{
//		delete userBattleArray;
//	}
//
//	//取消gameprecedure中发送图鉴包，改为每次进入图鉴页面时发送包。 by zhenhui 2014/2/26
//	//OPHandbookInfo* handbookInfo = new OPHandbookInfo;//clear
//	//handbookInfo->set_version(1);
//	//if (!_requestPackage(OPCODE_HANDBOOK_INFO_C,OPCODE_HANDBOOK_INFORET_S, handbookInfo))
//	//{
//	//	delete handbookInfo;
//	//}
//
//	OPUserMarketInfo* marketInfo = new OPUserMarketInfo;//already clear
//	marketInfo->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_MARKET_INFO_C,OPCODE_GET_USER_MARKET_INFORET_S,marketInfo))
//	{
//		delete marketInfo;
//	}
//
//	OPAdventureInfo* adventureInfo = new OPAdventureInfo;
//	adventureInfo->set_version(1);
//	if (!_requestPackage(OPCODE_GET_ADVENTURE_INFO_C,OPCODE_GET_ADVENTURE_INFORET_S,adventureInfo))
//	{
//		delete adventureInfo;
//	}
//	
//	//结束
//	OPAssembleFinish* assF = new OPAssembleFinish;	
//	ass->set_status(1);
//	if (!_requestPackage(OPCODE_ASSEMBLE_FINISH_C,OPCODE_ASSEMBLE_FINISH_S,assF))
//	{
//		delete assF;
//	}
//
//	//buyingCheck逻辑未实现完，且应该放到进入mainframe后择机发起
//	//buyingCheck::Get()->check();//zhengxin 2013-11-27
//}
//
//void GamePrecedure::reRequestPackages()
//{
//	OPUserBasicInfo *info = new OPUserBasicInfo;
//	info->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_BASIC_INFO_C,OPCODE_GET_USER_BASIC_INFORET_S,info))
//	{
//		delete info;
//	}
//
//	OPCheckNotice *noticeInfo=new OPCheckNotice;
//	noticeInfo->set_version(1);
//
//	if(!_requestPackage(OPCODE_NOTICE_INFO_C,OPCODE_NOTICE_INFO_S,noticeInfo))
//	{
//		delete noticeInfo;
//	}
//
//	OPShopList* shop = new OPShopList;
//	shop->set_version(1);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	const PlatformRoleItem* item=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
//	if(item)
//	{
//		shop->set_platname(item->name);
//	}
//	else
//	{
//		shop->set_platname(libPlatformManager::getPlatform()->getClientChannel());
//	}
//#endif
//#ifdef PROJECTAPPSTORE
//	shop->set_platname("ios_appstore");
//#endif
//
//	OPUserMarketInfo* marketInfo = new OPUserMarketInfo;
//	marketInfo->set_version(1);
//	if (!_requestPackage(OPCODE_GET_USER_MARKET_INFO_C,OPCODE_GET_USER_MARKET_INFORET_S,marketInfo))
//	{
//		delete marketInfo;
//	}
//
//	OPAdventureInfo* adventureInfo = new OPAdventureInfo;
//	adventureInfo->set_version(1);
//	if (!_requestPackage(OPCODE_GET_ADVENTURE_INFO_C,OPCODE_GET_ADVENTURE_INFORET_S,adventureInfo))
//	{
//		delete adventureInfo;
//	}
//
//}
const std::string GamePrecedure::gameSnapshot()
{
	CCSize size = CCEGLView::sharedOpenGLView()->getFrameSize();//CCDirector::sharedDirector()->getWinSize();////;//getWinSizeInPixels
	CCRenderTexture* in_texture = CCRenderTexture::create((int)size.width, (int)size.height, kCCTexture2DPixelFormat_RGBA8888);

	CCLOG("GamePrecedure::gameSnapshot width,height: %d,%d", (int)size.width, (int)size.height);

	in_texture->keepBegin();
	if (this->mInLoadingScene)
	{
		if (mLoadingFrame && mLoadingFrame->getRootSceneNode())
		{
			mLoadingFrame->getRootSceneNode()->visit();
		}
	}
	else
	{		
		if (mMainFrame && mMainFrame->getRootSceneNode())
		{
			mMainFrame->getRootSceneNode()->visit();
			
		}
	}
	in_texture->keepEnd();
	std::string file_path = "/share.png";
	bool b_result = in_texture->saveToFile(file_path.c_str(), kCCImageFormatPNG);
	
	if (b_result)
	{
		CCLOG("GamePrecedure::gameSnapshot success to %s", (CCFileUtils::sharedFileUtils()->getWritablePath() + file_path).c_str());
	}
	else
	{
		CCLOG("GamePrecedure::gameSnapshot failed, %s", (CCFileUtils::sharedFileUtils()->getWritablePath() + file_path).c_str());
	}
	
	return CCFileUtils::sharedFileUtils()->getWritablePath() + file_path;
}

void GamePrecedure::unInit()
{
	//
	//return;//zhengxin 2013-12-07 readTables 构造函数模式之后
	//
	if (mReadTables)
	{
		DiscipleTableManager::Get()->Free();
		//
		SkillTableManager::Get()->Free();
		//
		EquipTableManager::Get()->Free();
		//
		YuanfenTableManager::Get()->Free();
		//
		UserPropertyTableManager::Get()->Free();
		//
		ToolTableManager::Get()->Free();
		//
		DisciplesLevelParamManager::Get()->Free();
		//
		EquipLevelParamManager::Get()->Free();
		//
		HelpTableManager::Get()->Free();
		//
		VIPPrivilegeTableManager::Get()->Free();
		//
		AboutTableManager::Get()->Free();
		//
		AdventureTableManager::Get()->Free();
		//
		PlayerGradeTableManager::Get()->Free();
		//
		FragmentBookTableManager::Get()->Free();
		//
		AnnouncementTableManager::Get()->Free();
		//
		ToolTableManager::Get()->Free();
		//
		NewActivePromptTableManager::Get()->Free();
		//
		ArenaRewardTableManager::Get()->Free();
		//
		FightEndRewardTableManager::Get()->Free();
		//
		RestrictedWord::Get()->Free();
		
		//
		VipDataTableManager::Get()->Free();
		//
		ActivityPopTableManager::Get()->Free();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//PlatformNameManager::Get()->Free();
		//BBSConfigManager::Get()->Free();
//#endif
		//
		//
		TableReaderManager::Get()->Free();
		//
		buyingCheck::Get()->Free();
	}
	//
	mReadTables = false;
	//
	TimeCalculator::Get()->Free();
	//
	SeverConsts::Get()->Free();
	//
	waitingManager::Get()->Free();
	//
	LoginPacket::Get()->Free();
	//
}

GamePrecedure::~GamePrecedure()
{
	//this->unInit();//zhengxin 2013-12-07 readTables 构造函数模式之后
}

//bool GamePrecedure::_requestPackage( int opcode, int opcoderet, ::google::protobuf::Message* message )
//{
//	REQUEST_PACKAGE pkt;
//	pkt.opcode = opcode;
//	pkt.opcoderet = opcoderet;
//	pkt.message = message;
//	pkt.bfirstsent = false;
//
//	PacketManager::Get()->registerPacketHandler(opcoderet,this);
//
//	//	
//	std::list<REQUEST_PACKAGE>::iterator it = mRequestPackages.begin();
//	std::list<REQUEST_PACKAGE>::iterator end = mRequestPackages.end();
//	for ( ; it != end; ++it)
//	{
//		if ((*it).opcode == opcode)
//		{
//			return false;
//		}
//	}
//	mRequestPackages.push_back(pkt);
//	return true;
//}

void GamePrecedure::_gotPackage( int opcoderet )
{
	//std::list<REQUEST_PACKAGE>::iterator it = mRequestPackages.begin();
	//std::list<REQUEST_PACKAGE>::iterator end = mRequestPackages.end();
	//for ( ; it != end; ++it)
	//{
	//	if ((*it).opcoderet == opcoderet)
	//	{
	//		delete (*it).message;
	//		mRequestPackages.erase(it);
	//		++mLoginPacketsReceivedCount;
	//		break;;
	//	}
	//}


	//收包做一个计数，用来计算登录百分比
	++mLoginPacketsReceivedCount;
	//收到Assembly finish 回包后，重新注册error report handler 和 setEnable finish  by zhenhui 2014/7/30
	//if (opcoderet == ASSEMBLE_FINISH_S)
	//{
	//	m_loginPacketAssemblySuccess = true;
	//	mLoginPacketsReceivedCount =0;
	//	waitingManager::Get()->registerErrReportHandler();
	//	LoginPacket::Get()->setEnabled(true);
	//}
}

void GamePrecedure::_failedPakcage( int opcode, bool isSendFaild )
{

	//GamePrecedure 登录之后收包失败后暂时不做处理，后期有可能要做 by zhenhui 2014/7/30
	//if(!isSendFaild)//opcode is opcode ret
	//{		
	//	std::list<REQUEST_PACKAGE>::iterator it = mRequestPackages.begin();
	//	std::list<REQUEST_PACKAGE>::iterator end = mRequestPackages.end();
	//	for ( ; it != end; ++it)
	//	{
	//		if ((*it).opcode == opcode)
	//		{
	//			PacketManager::Get()->sendPakcet((*it).opcode, (*it).message);
	//			CCLOGERROR("LoginingPackets failed receive and resend one packet,opcode:%d",opcode);
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	LoginPacket::Get()->setEnabled(true);

	//	std::list<REQUEST_PACKAGE>::iterator it = mRequestPackages.begin();
	//	std::list<REQUEST_PACKAGE>::iterator end = mRequestPackages.end();
	//	for ( ; it != end; ++it)
	//	{
	//		if ((*it).opcode == opcode)
	//		{
	//			PacketManager::Get()->sendPakcet((*it).opcode, (*it).message);
	//			CCLOGERROR("LoginingPackets failed send and resend one packet,opcode:%d",opcode);
	//		}
	//	}
	//}
}

void GamePrecedure::enterBackGround()
{
	CCLOG("GamePrecedure::enterBackGround, enter the back ground");
	//如果登录数据包没有收到完成包，退出
	//if(m_loginPacketAssemblySuccess == false)
	//	exit(0);

	//暂时注释掉发送吃鱼回复体力等通知 by zhenhui 2014/7/30
	//GameNotification::Get()->addNotification();
    
    s_gotoBackgroundTime = time(0);

	cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
	if(pEngine)
	{
		pEngine->executeGlobalFunctionByName("GamePrecedure_enterBackGround",this,"GamePrecedure");
	}
    
}

bool GamePrecedure::isInLoadingSceneAndNeedExit()
{
    if(mLoadingFrame)
    {
        return !mLoadingFrame->isUpdateDown();
    }
    return false;
}

void GamePrecedure::onConnectFailed( std::string ip, int port )
{
	mStartLoginJiShi = false;
	if (mLoadingFrame)
	{
		mLoadingFrame->showEnter();
	}
}

void GamePrecedure::showLoadingEnter(){
	mStartLoginJiShi = false;
	if (mLoadingFrame)
	{
		mLoadingFrame->showEnter();
	}

}

LoadingFrame* GamePrecedure::getLoadingFrame()
{
	if (mLoadingFrame)
	{
		return mLoadingFrame;
	}

	return NULL;
}

void GamePrecedure::reset()
{
	mServerID = -1;
	mUin = mPlayerUin = "";
}

void GamePrecedure::showBulletin()
{
    if ( !m_pBulletinMgr )
    {
        m_pBulletinMgr = new BulletinManager();
    }
    m_pBulletinMgr->showBulletin();
}

void GamePrecedure::closeBulletin()
{
    if ( m_pBulletinMgr )
    {
        m_pBulletinMgr->closeBulletin();
        delete m_pBulletinMgr;
        m_pBulletinMgr = NULL;
    }
}

void ReEnterLoading( bool boo )
{
	if (boo)
	{
		GamePrecedure::Get()->reEnterLoading();
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
{
	JNIEXPORT jstring JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeGameSnapshot() {
		JNIEnv * env = 0;

		if (JniHelper::getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK || ! env) {
			return 0;
		}
		std::string file_path = GamePrecedure::Get()->gameSnapshot();
		return env->NewStringUTF(file_path.c_str());
	}

	JNIEXPORT jboolean JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeHasEnterMainFrame(JNIEnv * env, jobject obj) {

		return (!GamePrecedure::Get()->isInLoadingScene());

	}

	JNIEXPORT jint JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeGetServerId(JNIEnv * env, jobject obj) {

		return GamePrecedure::Get()->getServerID();

	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeReEnterLoadingFrame(JNIEnv * env, jobject obj) {

		GamePrecedure::Get()->reEnterLoading();

	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeAskLogoutFromMainFrameToLoadingFrame(JNIEnv * env, jobject obj) {

		//cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine()
		//	->executeGlobalFunctionByName("askLogoutFromMainFrameToLoadingFrame", NULL, "askLogoutFromMainFrameToLoadingFrame()");
		std::string title = Language::Get()->getString("@LogOffTitle");
		std::string message = Language::Get()->getString("@LogOffMSG");

		ShowConfirmPage( message , title ,ReEnterLoading );
	}
}



#endif


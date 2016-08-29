
#include "stdafx.h"

#include "AppDelegate.h"
#include "GamePrecedure.h"
#include "SimpleAudioEngine.h"
//#include "ThreadSocket.h"
#include "SoundManager.h"
#include "CCBManager.h"
#include "CCBScriptContainer.h"
#include "TimeCalculator.h"
#include <time.h>
#include "ServerDateManager.h"
#include "com4loves.h"
#include "SeverConsts.h"
//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <sys/sysinfo.h>//for sysinfo
#include <jni.h>
#include "..\..\cocos2dx\platform\android\jni\JniHelper.h"
#include "../../cocos2dx/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#endif
//

#define  MAX_TEXTURE_MSIZE (125*1024*1024)


//asuming game is portrait, and the non-retina pad's height is 1024, retina pad is 2048
#define CC_IS_IPAD_PORTRAIT() (CCDirector::sharedDirector()->getWinSize().height==2048||CCDirector::sharedDirector()->getWinSize().height==1024)

static time_t timestamp = time(0);//see this file

//
USING_NS_CC;

AppDelegate* g_AppDelegate = 0;

static float s_recycle_frequence = 5.f;
#define FREQUENCE_MAX  5.f

#ifdef WIN32
void accelerometerKeyHook(UINT message, WPARAM wParam, LPARAM lParam)
{	
	if (message == WM_KEYUP)
	{
		//Ctrl Pressed
		if (GetKeyState(VK_CONTROL) < 0)
		{
			//Ctrl + 'T' To Dump TextureCache
			if (wParam == 'T')
				CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
			if (wParam == 'Q')
				g_AppDelegate->purgeCachedData();
			if (wParam == 'U')
			{
				//load all ccbi in "ccbi" folder to check the ccbi's resource validation, for example, file lost, multiple name found and so on by zhenhui
				ResManager::Get()->checkCCBIResource();

				cocos2d::CCLuaEngine* pEngine = cocos2d::CCLuaEngine::defaultEngine();
				if(pEngine)
				{
					pEngine->executeGlobalFunction("checkAllConfigFile");
				}
			}
			if (wParam == 'O')
			{
				CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFramesPerFrame();
				CCTextureCache::sharedTextureCache()->removeUnusedTexturesPerFrame();
			}
		}
	}
}
#endif

AppDelegate::AppDelegate()
:needPurgeCache(false),needRecycleTex(false)
,delayRecycleSeconds(5.f),max_cache_bytes(MAX_TEXTURE_MSIZE)
{
	CCLOG("cocos2d-x app delegate");

	g_AppDelegate = this;
}

AppDelegate::~AppDelegate() 
{
}

void registerRecycleTex(const std::string& texName)
{
	g_AppDelegate->registerRecycleTex(texName);
}

bool checkDeviceInIOSPlist(std::string modelName){
	if (CCFileUtils::sharedFileUtils()->isFileExist("shader/ios-devices.plist"))
	{
		std::transform(modelName.begin(),modelName.end(),modelName.begin(),tolower);
		//
		CCDictionary* iosDic = CCDictionary::createWithContentsOfFileThreadSafe("shader/ios-devices.plist");
		iosDic->autorelease();
		if (iosDic) {
			CCObject* flagObject = iosDic->objectForKey(modelName);
			if (flagObject) {
				CCString* sResFlag = dynamic_cast<CCString*>(flagObject);//check if value of flag is "true"
				if (sResFlag->compare("true")==0) {
					return true;
				}else{
					CCLOG("@checkDeviceInIOSPlist, the flag is not true");
					return false;
				}
			}else{
				return false;
			}

		}else{
			return false;
		}
	}
	return false;
}


//add by hui
bool checkDeviceInAndroidPlist(int resWidth, int resHeight,std::string sManufactory, std::string sModelName){

	if (CCFileUtils::sharedFileUtils()->isFileExist("shader/android-devices.plist"))
	{
		std::transform(sManufactory.begin(),sManufactory.end(),sManufactory.begin(),tolower);
		std::transform(sModelName.begin(),sModelName.end(),sModelName.begin(),tolower);
		//
		CCDictionary* androidDic = CCDictionary::createWithContentsOfFileThreadSafe("shader/android-devices.plist");
		androidDic->autorelease();
		if (androidDic) {
			//Big flag no.1: check gpu names. not use currently.
			CCObject* gpusObject = androidDic->objectForKey("gpus");
			if (gpusObject) {
				CCDictionary* gpusDic = dynamic_cast<CCDictionary*>(gpusObject);
			}else{
				CCLOG("@checkDeviceInAndroidPlist, not find gpus in dic");
				return  false;
			}

			bool bInResolutionDic = false;
			//Big flag no.2: check resolutions 
			CCObject* resolutionsObject = androidDic->objectForKey("resolutions");
			if (resolutionsObject) {
				CCDictionary* resolutionsDic = dynamic_cast<CCDictionary*>(resolutionsObject);//get the res dic
				std::string sResWidth = StringConverter::toString(resWidth);
				std::string sResHeight = StringConverter::toString(resHeight);
				std::string sResolution = sResWidth+"*"+sResHeight;
				CCObject* resFlag = resolutionsDic->objectForKey(sResolution);//check if resolution exist.
				if (resFlag) {
					CCString* sResFlag = dynamic_cast<CCString*>(resFlag);//check if value of resolution is "true"
					if (sResFlag->compare("true")==0) {
						bInResolutionDic = true;
					}else{
						CCLOG("@checkDeviceInAndroidPlist, the flag of resolution(%s) is not true",sResolution.c_str());
						//return false;
					}
				}else{
					CCLOG("@checkDeviceInAndroidPlist, the resolution(%s) not found in dic",sResolution.c_str());
					//return false;
				}

			}else{
				CCLOG("@checkDeviceInAndroidPlist, resolution dic not found");
				return  false;
			}

			//if(bInResolutionDic){
				CCObject* devicesObject = androidDic->objectForKey("devices");
				if (devicesObject) {
					CCDictionary* devicesDic = dynamic_cast<CCDictionary*>(devicesObject);//get the devices dic
					{
						CCDictElement* pElement = NULL;
						CCDICT_FOREACH(devicesDic, pElement)
						{
							if (sManufactory.find(pElement->getStrKey()) != std::string::npos)
							{
								CCDictionary* manuDic = dynamic_cast<CCDictionary*>(pElement->getObject());
								CCDictElement* pElement1 = NULL;
								CCDICT_FOREACH(manuDic, pElement1)
								{
									if (sModelName.find(pElement1->getStrKey()) != std::string::npos)
									{
										CCString* sdeviceFlag = dynamic_cast<CCString*>(pElement1->getObject());
										if (sdeviceFlag && sdeviceFlag->compare("true")==0) {
										
											CCLOG("@checkDeviceInAndroidPlist, found model name %s in dic",sModelName.c_str());
											return true;
										}
										else
										{
											CCLOG("@checkDeviceInAndroidPlist, model name %s in dic is not equal to true",sModelName.c_str());
											return false;
										}
									}
								}
							}
						}
					}
					/*
					CCObject* manuObject = devicesDic->objectForKey(sManufactory);
					if (manuObject) {
						CCDictionary* manuDic = dynamic_cast<CCDictionary*>(manuObject);
						CCObject* modelFlagObject = manuDic->objectForKey(sModelName);
						if (modelFlagObject) {
							CCString* sdeviceFlag = dynamic_cast<CCString*>(modelFlagObject);//check if value of device flag is "true"
							if (sdeviceFlag->compare("true")==0) {
								CCLOG("@checkDeviceInAndroidPlist, found model name %s in dic",sModelName.c_str());
								return true;
							}else{
								CCLOG("@checkDeviceInAndroidPlist, model name %s in dic is not equal to true",sModelName.c_str());
								return false;
							}
						}else{
							CCLOG("@checkDeviceInAndroidPlist, Not find the model name %s in dic",sModelName.c_str());
							return false;
						}


					}else{
						CCLOG("@checkDeviceInAndroidPlist, Not find the manufactory %s in dic",sManufactory.c_str());
						return false;
					}
					*/
				}else{
					return false;
				}

			//}



		}else{
			CCLOG("@checkDeviceInAndroidPlist, shader/android-devices.plist load failed");
			return  false;
		}

	}
	return false;
}

bool AppDelegate::applicationDidFinishLaunching()
{
	//add the additional search path by hui.
	SeverConsts::Get()->initSearchPath();
	SeverConsts::Get()->setOriginalSearchPath();
	SeverConsts::Get()->setAdditionalSearchPath();

	// initialize director
	CCDirector *pDirector = CCDirector::sharedDirector();
	pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	pDirector->setProjection(kCCDirectorProjection2D);
	CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();

	/*
	480*854		320*569.333
	320*480		320*480
	640*1136	320*568
	768*1024	360*480
	800*1280	320*512
	480*800		320*533.333
	720*1280	320*568.889
	738*1152	320*480
	*/
	//CCSize designSize = CCSizeMake(320, 480);
	//float asp = screenSize.height/screenSize.width;
	//if(asp >1.5f)
	//	designSize.height = 320*asp;
	//else
	//	designSize.width = 480/asp;

	//新的屏幕适配，固定宽度，适配高度。
	CCSize designSize = CCSizeMake(640, 960);
	float asp = screenSize.height/screenSize.width;
	designSize.height = 640*asp;

	//CCSize designSize = CCSize(screenSize.width/2.0f,screenSize.height/2.0f);//CCSizeMake(320, 568);

	if (screenSize.height > 960)
	{
		//CCFileUtils::sharedFileUtils()->setResourceDirectory("iphonehd");
		CCFileUtils::sharedFileUtils()->addSearchResolutionsOrder("iphonehd/");
		//pDirector->setContentScaleFactor(screenSize.height/designSize.height);
	}
 	else
 	{
 		CCFileUtils::sharedFileUtils()->addSearchResolutionsOrder("iphone/");
 	}

	if (CCFileUtils::sharedFileUtils()->isFileExist("shader/palette.sha"))
	{
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename("shader/palette.sha").c_str())->getCString();	
		if (strstr(fragSource, "main()"))
		{
			bool bUsePaletteFlag = false;

			if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS){
				std::string platformInfo = libOS::getInstance()->getPlatformInfo();
				int pos = platformInfo.find('_');
				std::string iosDeviceName = platformInfo.substr(0,pos);
				bUsePaletteFlag = checkDeviceInIOSPlist(iosDeviceName);
			}else if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				//
				unsigned int warning_ram_mb = StringConverter::parseUnsignedInt(VaribleManager::Get()->getSetting("WarningRamMb", "", "768"), 768);//768Mb
				//
				struct sysinfo info = {0};
				unsigned long ram_mb = 2000;
				int err = ::sysinfo(&info);
				//LOGD("avalibleMemory (freeram,totalram,freehigh,totalhigh):(%d,%d,%d,%d)", info.freeram/1048576,info.totalram/1048576,info.freehigh/1048576,info.totalhigh/1048576);
				if (err == 0)
					ram_mb = info.totalram/1048576;
				//
				if (ram_mb <= warning_ram_mb)
					bUsePaletteFlag = true;
				else
				{
					std::string platformInfo = libOS::getInstance()->getPlatformInfo();
					bUsePaletteFlag = checkDeviceInAndroidPlist(screenSize.width,screenSize.height,platformInfo,platformInfo);
				}
#endif
			}
			else if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			{
				bUsePaletteFlag = true;
			}
			//
			if (bUsePaletteFlag)
				CCTextureCache::sharedTextureCache()->initPaletteGLProgram(ccPositionTextureColor_vert, fragSource);
			//
		}
		fragSource = NULL;
	}

	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionNoBorder  );
	//CCEGLView::sharedOpenGLView()->setViewPortInPoints(screenSize.width,screenSize.height,1,1);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	// turn on display FPS
	pDirector->setDisplayStats(true);
#endif
	

	// set FPS. the default value is 1.0/60 if you don't call this
#if 1
	pDirector->setAnimationInterval(1.0 / 30);
#endif

	//调度器优先更新
	pDirector->getScheduler()->scheduleUpdateForTarget(this, -1, false);

#ifdef WIN32
	//设置键盘事件回调
	CCEGLView::sharedOpenGLView()->setAccelerometerKeyHook(accelerometerKeyHook);
#endif
    
	GamePrecedure::Get()->init();

	//GamePrecedure::Get()->enterMainMenu();
	// 安卓平台的logomovie实在java层实现的，所以直接进入loading状态
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	GamePrecedure::Get()->enterLoading();
//#else
//	GamePrecedure::Get()->enterLogoMovie();
//#endif
	return true;
}

void AppDelegate::update(float deltaTime)
{
	if (needRecycleTex)
	{
		if (recycleTex.size() > 0)
		{
			s_recycle_frequence -= deltaTime;
			//if (s_recycle_frequence < 0.f)
			//{
				/*
				std::map<std::string, std::string>::iterator it = recycleTex.begin();
				for (;it != recycleTex.end();it++)	
				{
					//remove可能引起资源泄漏，从管理器除名的资源，可能引用计数得不到正确释放，重用时管理器又重新load，就发生泄漏了
					CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrameByName(it->second.c_str());
					CCTextureCache::sharedTextureCache()->removeTextureForKey(it->second.c_str());
				}
				*/
				clearRecycleTex();
				if (!needPurgeCache)
				{
					time_t start, end;
					start = clock();
					CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFramesPerFrame();
					CCTextureCache::sharedTextureCache()->removeUnusedTexturesPerFrame();
					end = clock();
					CCLOG("remove textures use time:%d",(int)(end-start));
				}
				CCLOG("AppDelegate Recycle Texture");
				needRecycleTex = false;
				s_recycle_frequence = FREQUENCE_MAX;
				//内存过高检测
				checkMemoryWarning();
			//}
		}
		else
		{
			needRecycleTex = false;
			//static float stLastCheck = 0.f;
			//内存过高检测
			checkMemoryWarning();
		}
	}

	if (needPurgeCache)
	{
		delayRecycleSeconds -= deltaTime;
		if (delayRecycleSeconds < 0)
		{
			delayRecycleSeconds = 5.f;
			needPurgeCache = false;
			purgeCachedData();
		}
	}
}

void AppDelegate::notifyPurgeCached()
{
	needPurgeCache = true;
}

void  AppDelegate::notifyRecycleTex()
{
	needRecycleTex = true;
}

void AppDelegate::registerRecycleTex(const std::string& texName)
{
	recycleTex[texName] = texName;
}

void AppDelegate::clearRecycleTex()
{
	recycleTex.clear();
}

//纹理占用内存过高, 深度回收, add by daijunhua 2013.11.25
void AppDelegate::checkMemoryWarning(bool forceInst)
{
	if(CCTextureCache::sharedTextureCache()->getTextrueTotalBytes() >= max_cache_bytes/*MAX_TEXTURE_MSIZE*/ 
		|| libOS::getInstance()->avalibleMemory() < 30)
	{
		if (forceInst)
		{
			needPurgeCache = false;
			purgeCachedData();
		}
		else
		{
			needPurgeCache = true;
		}
		//
		needRecycleTex = false;

		int leftMemory = libOS::getInstance()->avalibleMemory();

		CCLOG("AppDelegate Purge Cached Data, current left memory is %d",leftMemory);
	}	
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    if(GamePrecedure::Get()->isInLoadingSceneAndNeedExit())
	{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	#else
        exit(0);
	#endif
	}
    if (libOS::getInstance()->IsInPlayMovie())
    {
        exit(0);
    }
	//
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//if (getIsOnTempShortPauseJNI() == false)
	{
		PacketManager::Get()->disconnect();
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// win32 不需要断线，方便debug
#else
	PacketManager::Get()->disconnect();
#endif
		
	//
	CCDirector::sharedDirector()->stopAnimation();
	
	/*modify by zhaozhen for close all music*/

#ifndef _CLOSE_MUSIC
	SoundManager::Get()->appGotoBackground();
#endif

    if(!GamePrecedure::Get()->isInLoadingScene())
	{
        GamePrecedure::Get()->enterBackGround();
    }
	if(TimeCalculator::getInstance()->hasKey("ExitGameTime"))
	{
		if(TimeCalculator::getInstance()->getTimeLeft("ExitGameTime")<=0)
		{
			//exit(0);
		}
	}
	//--begin xinzheng 2013-7-18
	{
		
		time_t nowstamp = time(0);
		struct tm* timetm = localtime(&timestamp);
		struct tm savetm = {0};
		memcpy(&savetm, timetm, sizeof(struct tm));
		/*
			localtime返回同一个struct tm实例的指针
		*/
		struct tm* nowtm = localtime(&nowstamp);
		if (nowtm->tm_yday > savetm.tm_yday)
		{
			//exit(0);
		}
#if _DEBUG
		//test
		if (nowtm->tm_yday == savetm.tm_yday && (nowtm->tm_hour == savetm.tm_hour) && (nowtm->tm_min-savetm.tm_min) > 1)
		{
			//exit(0);
		}
#endif
		//
		timestamp = nowstamp;
	}
	//--end

	purgeCachedData();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    if(!GamePrecedure::Get()->isInLoadingScene())
        GamePrecedure::Get()->enterForeGround();
    
	CCDirector::sharedDirector()->startAnimation();

	/*modify by zhaozhen for close all music*/
#ifndef _CLOSE_MUSIC
	SoundManager::Get()->appResumeBackground();
#endif
}

void AppDelegate::purgeCachedData( void )
{
	//CCAnimationCache::purgeSharedAnimationCache();

	//CCBScriptContainer::purgeCachedData();

	//CCBManager::Get()->purgeCachedData();

	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFramesPerFrame();

	CCTextureCache::sharedTextureCache()->removeUnusedTexturesPerFrame();

//#ifdef _DEBUG
//	CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
//#endif
}

void AppDelegate::applicationWillGoToExit()
{
	//--begin xinzheng 2013-9-23
	/*UserBasicInfo& userBasicInfo = ServerDateManager::Get()->getUserBasicInfo();
	if (!userBasicInfo.name.empty())
	{
		//name有效->已登录->退出时向账号服务器刷新
		bool putSvr = false;
		if (VaribleManager::Get()->getSetting("PushSvrID") == "1")
			putSvr = true;
		com4loves::updateServerInfo(ServerDateManager::Get()->getSeverID(), 
			userBasicInfo.name, userBasicInfo.playerId, userBasicInfo.level, 
			userBasicInfo.viplevel, userBasicInfo.silvercoins, userBasicInfo.goldcoins, putSvr);
	}*/
	//--end
	//
	if (GamePrecedure::Get()->isInLoadingScene())
	{
		GamePrecedure::Get()->exitGame();
	}
	//
	//
	CCBManager::Get()->Free();
	//
	MessageManager::Get()->Free();
	//
	PacketManager::Get()->Free();
	//
	
	//CurlDownload::Get()->Free();
	//

	//modify by zhaozhen for close all music
#ifndef _CLOSE_MUSIC
	SoundManager::Get()->Free();
#endif

	//
	Language::Get()->Free();
	//
	GamePrecedure::Get()->Free();
	//
	//ThreadSocket::Get()->Free();
	
	//
	purgeCachedData();
	//
}

void AppDelegate::setMaxCacheByteSizeLimit( unsigned int maxbytes )
{
	max_cache_bytes = maxbytes;
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {

	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeGameDestroy(JNIEnv*  env, jobject thiz) {
		CCApplication::sharedApplication()->applicationWillGoToExit();
	}
	/*
		这个Android Cocos2dxActivity onLowMemory 会控制时间间隔触发
	*/
	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativePurgeCachedData(JNIEnv*  env, jobject thiz) {
		
		CCApplication::sharedApplication()->purgeCachedData();

		//CCDirector::sharedDirector()->purgeCachedData();

		//CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();

	}
}
#endif

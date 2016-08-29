
#include "stdafx.h"

#include "LogoFrame.h"
#include "AppDelegate.h"
#include "SoundManager.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "libOS.h"
#include <list>
#include <vector>
#include "DataTableManager.h"

USING_NS_CC;
USING_NS_CC_EXT;
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/* interface from State */
//===============================================================================================
void LogoFrame::Enter(GamePrecedure*)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#ifndef _CLOSE_MUSIC
	//bg music is played after the mainframe loading completely modify by xinghui
		SoundManager::getInstance()->setMusicOn(false);	
#endif

	libOS::getInstance()->setIsInPlayMovie(true);
	libOS::getInstance()->registerListener(this);	
	libPlatformManager::getPlatform()->registerListener(this);
	libOS::getInstance()->playMovie("gamelogo", false);
#else
	m_bLogoFinished = true;
#endif
}
//===============================================================================================
void LogoFrame::Execute(GamePrecedure*)
{

}

//===============================================================================================
void LogoFrame::Exit(GamePrecedure*)
{
}

/* interface from libOSListener */
//===============================================================================================
void LogoFrame::onPlayMovieEndMessage()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	libOS::getInstance()->removeListener(this);
	libPlatformManager::getPlatform()->removeListener(this);
	libOS::getInstance()->setIsInPlayMovie(false);
#ifndef _CLOSE_MUSIC
	//modify by zhaozhen for close all music
	SoundManager::getInstance()->setMusicOn(true);
#endif

#endif
	m_bLogoFinished = true;
}

/* interface from platformListener */
//===============================================================================================
void LogoFrame::onPlayMovieEnd()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	libOS::getInstance()->removeListener(this);
	libPlatformManager::getPlatform()->removeListener(this);
	libOS::getInstance()->setIsInPlayMovie(false);
	
#ifndef _CLOSE_MUSIC
	//modify by zhaozhen for close all music
	SoundManager::getInstance()->setMusicOn(true);
#endif
#endif
	m_bLogoFinished = true;
}


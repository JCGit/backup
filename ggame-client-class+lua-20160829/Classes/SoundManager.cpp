
#include "stdafx.h"

#include "SoundManager.h"
#include "GameMessages.h"
#include "BlackBoard.h"

#include "SimpleAudioEngine.h"
#include "DataTableManager.h"
#define generalmusic "general.mp3"
#define battlemusic "battle.mp3"
#define clicksound "click.mp3"
#define loadingmusic "loadingMusic.mp3"

SoundManager::SoundManager(void)
	:mMusicOn(true),mEffectOn(true)
{
}


SoundManager::~SoundManager(void)
{
}

void SoundManager::playLoadingMusic()
{
	std::string stepKey = "SetttingsMusic";
	bool _localMusic=cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(stepKey.c_str(),1)==1?true:false;
	if(_localMusic && (StringConverter::parseInt(VaribleManager::getInstance()->getSetting("musicAndSoundOn")) == 1))
	{
		mPlayMusic=VaribleManager::Get()->getSetting("LoadingMusic");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(mPlayMusic.c_str(), true);
	}
}
void SoundManager::playGeneralMusic()
{
    if(mMusicOn && mPlayMusic!=generalmusic && (StringConverter::parseInt(VaribleManager::getInstance()->getSetting("musicAndSoundOn")) == 1))
	{
		mPlayMusic=VaribleManager::Get()->getSetting("GeneralMusic");
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(mPlayMusic.c_str(), true);
	}
	mPlayMusic = generalmusic;
}
void SoundManager::playFightMusic(std::string music)
{
    if(mMusicOn && mPlayMusic != music && (StringConverter::parseInt(VaribleManager::getInstance()->getSetting("musicAndSoundOn")) == 1))
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(music.c_str(), true);
	}
	mPlayMusic = music;
}

void SoundManager::playMusic( std::string music )
{
	//添加播放音乐开关，by zhaolu 2014年9月10日
	if(mMusicOn && (StringConverter::parseInt(VaribleManager::getInstance()->getSetting("musicAndSoundOn")) == 1))
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(music.c_str(), true);
	mPlayMusic = music;
}

//modify by daimeiqi at 20131202 播放活动背景音乐
void SoundManager::playAdventureMusic(unsigned int itemId)
{
	const AdventureItem* item=AdventureTableManager::Get()->getAdventureItemByID(itemId);
	if(!item)
		return;
	std::string musicName=item->music;
	if(musicName=="")
		return;
	if(mMusicOn && mPlayMusic !=musicName && (StringConverter::parseInt(VaribleManager::getInstance()->getSetting("musicAndSoundOn")) == 1))
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(musicName.c_str(), true);
	mPlayMusic=musicName;
}

void SoundManager::onReceiveMassage( const GameMessage * message )
{
	if(mEffectOn && (StringConverter::parseInt(VaribleManager::getInstance()->getSetting("musicAndSoundOn")) == 1))
	{
		MsgButtonPressed* msg = dynamic_cast<MsgButtonPressed*>(message->clone());
		if (!msg)
		{
			return;
		}
		SoundEffectItem* item = SoundEffectTableManager::Get()->getSoundEffectById(msg->tag);
		if (item)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(item->file.c_str(),false);
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( clicksound , false);
		}
		delete msg;
	}
}

int SoundManager::playEffectByID( int musicID )
{
	//
	if(mEffectOn && (StringConverter::parseInt(VaribleManager::getInstance()->getSetting("musicAndSoundOn")) == 1))
	{
		if(!BlackBoard::Get()->isSamSungi9100Audio)
		{
			SoundEffectItem* item = SoundEffectTableManager::Get()->getSoundEffectById(musicID);
			if (item)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(item->file.c_str(),false);
			}
			else
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( clicksound , false);
			}
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

void SoundManager::init()
{
    MessageManager::Get()->regisiterMessageHandler(MSG_BUTTON_PRESSED, this);
    
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(loadingmusic);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(generalmusic);
	if(!BlackBoard::Get()->isSamSungi9100Audio)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(clicksound);
	}
	
}

void SoundManager::setMusicOn( bool isOn)
{
	if(mMusicOn != isOn)
	{
		mMusicOn = isOn;
		if(isOn)
		{
			if(mPauseMusic == mPlayMusic)
				CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
			else
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(mPlayMusic.c_str(), true);
		}
		else
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
			mPauseMusic = mPlayMusic;
		}
		std::string stepKey = "SetttingsMusic";
		cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(stepKey.c_str(),mMusicOn?1:0);
		cocos2d::CCUserDefault::sharedUserDefault()->flush();
	}
}

void SoundManager::setEffectOn( bool isOn)
{
	mEffectOn = isOn;
	if(!isOn)
	{
		if(!BlackBoard::Get()->isSamSungi9100Audio)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
		}
	}
	std::string stepKey = "SetttingsEffect";
	cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(stepKey.c_str(),mEffectOn?1:0);
	cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

int SoundManager::playEffect( std::string effect )
{
	if (effect.empty())
		return -1;
	if (effect == "none")
		return -1;

	//
	if(mEffectOn && (StringConverter::parseInt(VaribleManager::getInstance()->getSetting("musicAndSoundOn")) == 1))
	{
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect( effect.c_str());
		if(!BlackBoard::Get()->isSamSungi9100Audio)
		{
			return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( effect.c_str() , false);
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

void SoundManager::appGotoBackground()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	if(!BlackBoard::Get()->isSamSungi9100Audio)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	}
}	

void SoundManager::appResumeBackground()
{
	if(mMusicOn)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	if(mEffectOn)
	{
		if(!BlackBoard::Get()->isSamSungi9100Audio)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
		}
	}
}

SoundManager* SoundManager::getInstance()
{
	return SoundManager::Get();
}

void SoundManager::stopAllEffect()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void SoundManager::stopMusic()
{
	mPlayMusic = "";
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

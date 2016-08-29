#pragma once
#include "Singleton.h"
#include "MessageManager.h"
#include <string>

class SoundManager
	: public Singleton<SoundManager>
	, public MessageHandler
{
public:
	SoundManager(void);
	~SoundManager(void);
	void init();
	void setMusicOn(bool);
	void setEffectOn(bool);

	void playLoadingMusic();
	void playGeneralMusic();
	void playFightMusic(std::string music);
	void stopMusic();
	void playMusic(std::string music);

	int playEffectByID( int musicID );

	int playEffect(std::string effect);
	void stopAllEffect();
	//modify by daimeiqi at 20131202 ≤•∑≈ªÓ∂Ø±≥æ∞“Ù¿÷
	void playAdventureMusic(unsigned int itemId);
	virtual void onReceiveMassage(const GameMessage * message);

	void appGotoBackground();
	void appResumeBackground();

	static SoundManager* getInstance();
private:
	bool mMusicOn;
	bool mEffectOn;
	std::string mPauseMusic;
	std::string mPlayMusic;
};


#include "AudioManager.h"
#include "SimpleAudioEngine.h"
#include "defines.h"
#include "Resources.h"
#include "QEnvSet.h"
#include "cocos2d.h"

USING_NS_CC;

map<string, int > AudioManager::m_stAudioPathMap;
string AudioManager::m_strNowBgSound = "";

void checkEffSoundForIOS(string& strFile) {
    //ios 下把ogg转为mp3
    //Android	Supports .ogg best, not so good for .wav format.
    //iOS	.mp3, .wav, .caf
    //Windows Desktop	.mid and .wav only
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(strFile.find(".mp3") != string::npos) {
        return;
    }
    int pos =  strFile.rfind(".");
    if(pos != string::npos){
        strFile = strFile.replace(pos, (strFile.length() - pos) , ".mp3");
    }
#endif
}


bool AudioManager::isSoundTypeOK(const string& strFile) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    if(m_strNowBgSound.find(".wav") == string::npos) {
        CCLOG("[warn]AudioManager::playBackgroundMusic, invalid sound type:%s", strFile.c_str() );
        return false;
    }
#endif
    return true;
}

void AudioManager::playBackgroundMusic(const char* pszFilePath, bool bLoop){
	m_strNowBgSound = pszFilePath;
	if(!isBackgroundOn()){
		return;
	}
    if(!isSoundTypeOK(m_strNowBgSound)) {
        return;
    }
	string stSoundUrl = BG_SOUND_RES_PATH;

	stSoundUrl.append(pszFilePath);
	if(IS_AUDIO_OPEN && QEnvSet::beck() == false) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(stSoundUrl.c_str(), bLoop);
        m_stAudioPathMap[stSoundUrl] = AUDIO_BG;
	}
}

void AudioManager::playBackgroundMusic(const char* pszFilePath) {
	m_strNowBgSound = pszFilePath;
	if(!isBackgroundOn()){
		return;
	}

    if(!isSoundTypeOK(m_strNowBgSound)) {
        return;
    }

	string stSoundUrl = BG_SOUND_RES_PATH;

	stSoundUrl.append(pszFilePath);
	if(IS_AUDIO_OPEN && QEnvSet::beck() == false) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(stSoundUrl.c_str(), false);
        m_stAudioPathMap[stSoundUrl] = AUDIO_BG;
	}
}

void AudioManager::stopBackgroundMusic(bool bReleaseData){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(bReleaseData);
}

void AudioManager::pauseBackgroundMusic(){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AudioManager::resumeBackgroundMusic(){
	if(!isBackgroundOn()){
		return;
	}

	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void AudioManager::stopBackgroundMusic(){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(false);
}

bool AudioManager::isBackgroundOn(){
	return CCUserDefault::sharedUserDefault()->getBoolForKey("is_tgame_bg_sound", true);
}


void AudioManager::preloadEffect(const char* pszFilePath){
    if(!isSoundTypeOK(string(pszFilePath))) {
        return;
    }
    string file(pszFilePath);
    checkEffSoundForIOS(file);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(file.c_str());
}

void AudioManager::unloadEffect(const char* pszFilePath){
    if(!isSoundTypeOK(string(pszFilePath))) {
        return;
    }
    string file(pszFilePath);
    checkEffSoundForIOS(file);
	CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect(file.c_str());
}

unsigned int AudioManager::playEffect(const char* pszFilePath, bool bLoop){
	if(!isEffectOn()){
		return 0;
	}
    if(!isSoundTypeOK(string(pszFilePath))) {
        return 0;
    }
	string stSoundUrl = SOUND_RES_PATH;

    stSoundUrl.append(pszFilePath);
    checkEffSoundForIOS(stSoundUrl);
	unsigned int uiEffID = 0;
	if(IS_AUDIO_OPEN) {
		uiEffID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(stSoundUrl.c_str(), bLoop);
        m_stAudioPathMap[stSoundUrl] = AUDIO_EFFECT;
		
	}
	return uiEffID;
}

unsigned int AudioManager::playEffect(const char* pszFilePath){
	if(!isEffectOn()){
		return 0;
	}
    if(!isSoundTypeOK(string(pszFilePath))) {
        return 0;
    }
	string stSoundUrl = SOUND_RES_PATH;
	
    stSoundUrl.append(pszFilePath);
    checkEffSoundForIOS(stSoundUrl);
	unsigned int uiEffID = 0;
	if(IS_AUDIO_OPEN) {
		uiEffID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(stSoundUrl.c_str(), false);
		m_stAudioPathMap[stSoundUrl] = AUDIO_EFFECT;
	}
	return 0;
}

void AudioManager::stopEffect(unsigned int nSoundId){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(nSoundId);
}

void AudioManager::resumeEffect(unsigned int nSoundId){
	if(!isEffectOn()){
		return;
	}

	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeEffect(nSoundId);
}

void AudioManager::pauseAllEffects(){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

void AudioManager::resumeAllEffects(){
	if(!isEffectOn()){
		return;
	}

	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void AudioManager::stopAllEffects(){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

bool AudioManager::isEffectOn(){
	return CCUserDefault::sharedUserDefault()->getBoolForKey("is_tgame_sound_effect", true);
}

AudioManager::AudioManager(){
	
}

AudioManager::~AudioManager()
{
}

void AudioManager::dumpAllAudio() {
    CCLOG("-------------AudioManager::dumpAllAudio---begin-----------");
    for(auto it = m_stAudioPathMap.begin(); it != m_stAudioPathMap.end(); ++it) {
        CCLOG("audio=%s,type=%d", it->first.c_str(), it->second);
    }
    CCLOG("total audio size %d", m_stAudioPathMap.size());
    CCLOG("-------------AudioManager::dumpAllAudio---end-------------");
}

string AudioManager::getNowBgSound() {
    return m_strNowBgSound;
}



#ifndef AudioManager_h__
#define AudioManager_h__

#include "cocos-ext.h"
#include "cocos2d.h"
#include <map>
#include <string>
using namespace std;

class AudioManager
{
public:
    enum {
        AUDIO_BG = 1, //背景音乐
        AUDIO_EFFECT = 2, //音效
    };
	AudioManager();
	~AudioManager();

	static void playBackgroundMusic(const char* pszFilePath, bool bLoop);

	static void playBackgroundMusic(const char* pszFilePath);

	static void stopBackgroundMusic(bool bReleaseData);

	static void pauseBackgroundMusic();

	static void resumeBackgroundMusic();

	static void stopBackgroundMusic();

	static bool isBackgroundOn();

	static void preloadEffect(const char* pszFilePath);		//包含文件夹名

	static void unloadEffect(const char* pszFilePath);

	static unsigned int playEffect(const char* pszFilePath, bool bLoop);	//只需音效文件名

	static unsigned int playEffect(const char* pszFilePath);

	static void stopEffect(unsigned int nSoundId);

	static void resumeEffect(unsigned int nSoundId);

	static void pauseAllEffects();

	static void resumeAllEffects();

    static void stopAllEffects();

	static bool isEffectOn();

    static void dumpAllAudio(); //just for debug
    static string getNowBgSound();
private:
    static bool isSoundTypeOK(const string& strFile);
private:
	static map<string, int > m_stAudioPathMap; //<path, audio type>
    static string m_strNowBgSound;
};

#endif // AudioManager_h__

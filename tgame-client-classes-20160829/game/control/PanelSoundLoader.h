#ifndef PanelSoundLoader_h__
#define PanelSoundLoader_h__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Resources.h"
#include "control/AudioManager.h"

#include "PanelSoundCfgMaster.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class CPanelSoundLoader
{
public:
	CPanelSoundLoader(){}
	~CPanelSoundLoader(){}


	int LoadAllPanelSound(){
		m_stSoundLst.clear();
		PanelSoundCfgSet m_stCfg = PANEL_SOUND_CFG->GetCfg();
		string soundStr = "";
		for(int idx = 0; idx < m_stCfg.panelsoundcfg_size(); idx++){
			soundStr = m_stCfg.panelsoundcfg(idx).opensound();
			m_stSoundLst.insert(soundStr);

			soundStr = m_stCfg.panelsoundcfg(idx).closesound();
			m_stSoundLst.insert(soundStr);
		}

		set<string>::iterator stIter;
		for(stIter = m_stSoundLst.begin(); stIter != m_stSoundLst.end(); stIter++){
			string fullPath = SOUND_RES_PATH + *stIter;
			//CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(fullPath.c_str());
			AudioManager::preloadEffect(fullPath.c_str());
		}

		return 0;
	}

private:
	set<string> m_stSoundLst;
};

#endif // PanelSoundLoader_h__

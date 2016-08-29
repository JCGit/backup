#pragma once
#include "lib91.h"
#include "libPP.h"
#include "libTB.h"
#include "libITools.h"
#include "libAppStore.h"
#include "libYouai.h"
#include "libKY.h"
#include "libAs.h"
#include "libUHaima.h"
#include "libXY.h"
#include "libJunhai.h"
#include "libDefault.h"

class GamePlatformInfo
{
	static GamePlatformInfo *m_sInstance;
	std::string platFormName;
	std::string platVersionName;
    SDK_CONFIG_STU config;
public:
	
	void init(bool isRegPlat=false);
	void registerPlatform();
	std::string getPlatFormName() { return platFormName;};
	std::string getPlatVersionName() { return platVersionName;}
	static GamePlatformInfo* getInstance();
    SDK_CONFIG_STU getPlatFromconfig() { return config;}
};
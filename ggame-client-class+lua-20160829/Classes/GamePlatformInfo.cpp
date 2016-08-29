#include "GamePlatformInfo.h"
#include "libPlatform.h"
#include "platform/CCPlatformConfig.h"

GamePlatformInfo * GamePlatformInfo::m_sInstance = 0;

GamePlatformInfo* GamePlatformInfo::getInstance()
{
	if(!m_sInstance)
	{
		m_sInstance = new GamePlatformInfo;
		m_sInstance->init();
	}
	return m_sInstance;
}

void GamePlatformInfo::init(bool isRegPlat)
{
    config.com4lovesconfig.appid = "xwlgj";
    config.com4lovesconfig.sdkappid = "21";
    config.com4lovesconfig.sdkappkey = "xwlgj_ios";
#ifdef PROJECT_EXPERIENCE_IOS
		if(isRegPlat) AUTO_REGISTER_PLATFORM(lib91);
		platVersionName="version_experience_ios.cfg";
		platFormName="lib91";
#else
#ifdef PROJECT91
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(lib91);
		platVersionName="version_ios_all.cfg";
		platFormName="lib91";
        
        
        config.com4lovesconfig.channelid = "0";
        config.com4lovesconfig.platformid = "ios_91";
        
        
        config.platformconfig.appid = "115606";//115601
        config.platformconfig.appkey = "85da6ab16bde4088798ea977f9bf4c227446a88f145ec157";//45a22c2a03c74e685106b5c567bf2370447c3f2c347e9c7c
        config.platformconfig.uidPrefix = "91_";

        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "91";
        config.platformconfig.moneyName = "91豆";
	}
#endif

#ifdef PROJECTCMGE
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libCmge);
		platVersionName="version_ios_all.cfg";
		platFormName="libCmge";
	}
#endif
    
#ifdef PROJECTUC
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libUC);
		platVersionName="version_ios_all.cfg";
		platFormName="libUC";
	}
#endif
    
#ifdef PROJECTPP
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libPP);
		platVersionName="version_ios_all.cfg";
		platFormName="libPP";
        
        config.com4lovesconfig.channelid = "0";
        config.com4lovesconfig.platformid = "ios_pp";
        
        
        config.platformconfig.appid = "4293";//115601
        config.platformconfig.appkey = "f55b314daff2e50c0e168fd548e62b00";//45a22c2a03c74e685106b5c567bf2370447c3f2c347e9c7c
        config.platformconfig.uidPrefix = "PPUSR_";
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "PP";
        config.platformconfig.moneyName = "PP币";
	}
#endif
    
#ifdef PROJECTPPZB
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libPPZB);
		platVersionName="version_ios_pp.cfg";
		platFormName="libPPZB";
	}
#endif

#ifdef PROJECT37WANWAN
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(lib37wanwan);
		platVersionName="version_ios_all.cfg";
		platFormName="lib37wanwan";
	}
#endif
    
#ifdef PROJECT49APP
	{
        if(isRegPlat) AUTO_REGISTER_PLATFORM(lib49app);
        platVersionName="version_ios_all.cfg";
        platFormName="lib49app";
	}
#endif
    
#ifdef PROJECTTB
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libTB);
		platVersionName="version_ios_all.cfg";
		platFormName="libTB";
        
        config.com4lovesconfig.channelid = "0";
        config.com4lovesconfig.platformid = "ios_tb";
        
        config.platformconfig.appid = "140838";
        config.platformconfig.appkey = "GdTA4Naxm@JhWj7QGSqANCZx0zJWti7F";
        config.platformconfig.uidPrefix = "TBUSR_";
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "TB";
        config.platformconfig.moneyName = "推币";
	}
#endif

#ifdef PROJECTITools
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libITools);
		platVersionName="version_ios_all.cfg";
		platFormName="libITools";
        
        config.com4lovesconfig.channelid = "0";
        config.com4lovesconfig.platformid = "ios_ito";
        
        config.platformconfig.appid = "438";
        config.platformconfig.appkey = "4981333892356B97E80EEDDC385071D8";
        config.platformconfig.uidPrefix = "ITOUSR_";
        config.platformconfig.bbsurl = "";
        
        config.platformconfig.clientChannel = "ITO";
        config.platformconfig.moneyName = "RMB";
	}
#endif

#ifdef PROJECT91Debug
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(lib91Debug);
		platVersionName="version_debug.cfg";
		platFormName="lib91Debug";
	}
#endif

#ifdef PROJECT91Quasi
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(lib91Quasi);
		platVersionName="version_ios_all.cfg";
		platFormName="lib91Quasi";
	}
#endif

#ifdef PROJECTAPPSTORE
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libAppStore);
		platVersionName="version_ios_all.cfg";
		platFormName="libAppStore";
        
        config.com4lovesconfig.appid = "xwlgj";
        config.com4lovesconfig.sdkappid = "22";
        config.com4lovesconfig.sdkappkey = "xwlgj_app";
        config.com4lovesconfig.channelid = "902";
        config.com4lovesconfig.platformid = "ios_appstore";
	}
#endif

#ifdef PROJECTAPPSTORETW
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libAppStore);
		platVersionName="version_ios_appstore_tw.cfg";
		platFormName="libAppStore";
	}
#endif



#ifdef PROJECTYOUAI
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libYouai);
        platVersionName="version_ios_all.cfg";
        platFormName="libYouai";
        
        config.com4lovesconfig.channelid = "1000";
        config.com4lovesconfig.platformid = "ios_youai";
        
        config.platformconfig.appid = "qmwow.com4loves.com";
        config.platformconfig.appkey = "xwlgjioseliqioqockqcnmwte";
        config.platformconfig.uidPrefix = "";
        
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "youai";
        config.platformconfig.moneyName = "RMB";
	}
#endif

#ifdef PROJECTJUNHAI
    {
        if(isRegPlat) AUTO_REGISTER_PLATFORM(libJunhai);
        platVersionName="version_ios_all.cfg";
        platFormName="libJunhai";
        
        config.com4lovesconfig.channelid = "1153";
        config.com4lovesconfig.platformid = "ios_youai";
        
        config.platformconfig.appid = "qmwow.com4loves.com";
        config.platformconfig.appkey = "xwlgjioseliqioqockqcnmwte";
        config.platformconfig.uidPrefix = "";
        
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "youai";
        config.platformconfig.moneyName = "RMB";
    }
#endif

#ifdef PROJECTAG
    {
        if(isRegPlat) AUTO_REGISTER_PLATFORM(libYouai);
        platVersionName="version_ios_all.cfg";
        platFormName="libYouai";
        
        config.com4lovesconfig.channelid = "909";
        config.com4lovesconfig.platformid = "ios_youai";
        
        config.platformconfig.appid = "qmwow.com4loves.com";
        config.platformconfig.appkey = "xwlgjioseliqioqockqcnmwte";
        config.platformconfig.uidPrefix = "";
        
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "youai";
        config.platformconfig.moneyName = "RMB";
    }
#endif
    
#ifdef PROJECTAG2
    {
        if(isRegPlat) AUTO_REGISTER_PLATFORM(libYouai);
        platVersionName="version_ios_all.cfg";
        platFormName="libYouai";
        
        config.com4lovesconfig.channelid = "1179";
        config.com4lovesconfig.platformid = "youai";
        
        config.platformconfig.appid = "qmwow.com4loves.com";
        config.platformconfig.appkey = "xwlgjioseliqioqockqcnmwte";
        config.platformconfig.uidPrefix = "";
        
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "youai";
        config.platformconfig.moneyName = "RMB";
    }
#endif
    
#ifdef PROJECTYP
    {
        if(isRegPlat) AUTO_REGISTER_PLATFORM(libYouai);
        platVersionName="version_ios_all.cfg";
        platFormName="libYouai";
        
        config.com4lovesconfig.channelid = "1089";
        config.com4lovesconfig.platformid = "ios_youai";
        
        config.platformconfig.appid = "qmwow.com4loves.com";
        config.platformconfig.appkey = "xwlgjioseliqioqockqcnmwte";
        config.platformconfig.uidPrefix = "";
        
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "youai";
        config.platformconfig.moneyName = "RMB";
    }
#endif
    
#ifdef PROJECTHL
    {
        if(isRegPlat) AUTO_REGISTER_PLATFORM(libYouai);
        platVersionName="version_ios_all.cfg";
        platFormName="libYouai";
        
        config.com4lovesconfig.channelid = "1180";
        config.com4lovesconfig.platformid = "ios_youai";
        
        config.platformconfig.appid = "qmwow.com4loves.com";
        config.platformconfig.appkey = "xwlgjioseliqioqockqcnmwte";
        config.platformconfig.uidPrefix = "";
        
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "youai";
        config.platformconfig.moneyName = "RMB";
    }
#endif
    
#ifdef PROJECTKY
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libKY);
		platVersionName="version_ios_all.cfg";
		platFormName="libKY";
        
        config.com4lovesconfig.channelid = "901";
        config.com4lovesconfig.platformid = "ios_ky";

        
        config.platformconfig.appid = "2373fd9fcd8fd32d3f5cdd8824a4256e";
        config.platformconfig.appkey = "Z1b8QiK5HohimgBEAKd27rG4e6Q0qImp";
        config.platformconfig.uidPrefix = "KYUSR_";
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "KY";
        config.platformconfig.moneyName = "RMB";
        config.platformconfig.appScheme = "6129";
	}
#endif

#ifdef PROJECTDownJoy
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libDownJoy);
		platVersionName="version_ios_all.cfg";
		platFormName="libDownJoy";
	}
#endif

#ifdef PROJECTUHAIMA
    if(isRegPlat) AUTO_REGISTER_PLATFORM(libUHaima);
    platVersionName="version_ios_all.cfg";
    platFormName="libUHaima";
    
    config.com4lovesconfig.channelid = "1130";
    config.com4lovesconfig.platformid = "ios_haima";
    
    config.platformconfig.appid = "3000910897";
    config.platformconfig.appkey = "NjBGNDVEMDUyMzUxM0VDMjIxMDU4OTNFMDY3MDBDMDMyOUM5QzAwRk9UTXpOekl5TVRVek9UZzRNRE01TVRBek55c3hOelExTnprek9USXpNemMwT1RNNE1EWXpOak00TVRVeE1EQTVOemN6TURZME5qa3dNVGM9";
    config.platformconfig.uidPrefix = "HAIMAUSR_";
    config.platformconfig.bbsurl = "";
    config.platformconfig.clientChannel = "haima";
    config.platformconfig.moneyName = "RMB";
#endif
    
#ifdef PROJECTXY
    {
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libXY);
		platVersionName="version_ios_all.cfg";
		platFormName="libXY";
        
        config.com4lovesconfig.channelid = "1123";//xy:1123,xy1:1193,xy2:1194
        config.com4lovesconfig.platformid = "ios_xy";
        
        config.platformconfig.appid = "100001449";//xy:100001449;xy3:100002405
        config.platformconfig.appkey = "0nbgXBRKtwatWw7lAYBIVdkXCgYhhOS2";//xy:0nbgXBRKtwatWw7lAYBIVdkXCgYhhOS2;xy3:mb4osQ39PXncw43xq3s1QnxtH99Z6nDi
        config.platformconfig.uidPrefix = "XYUSR_";
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "xy";
        config.platformconfig.moneyName = "RMB";
	}
#endif
    
#ifdef PROJECTXY3
    {
        if(isRegPlat) AUTO_REGISTER_PLATFORM(libXY);
        platVersionName="version_ios_all.cfg";
        platFormName="libXY";
        
        config.com4lovesconfig.channelid = "1123";//xy:1123,xy1:1193,xy2:1194
        config.com4lovesconfig.platformid = "ios_xy3";
        
        config.platformconfig.appid = "100002405";//xy:100001449;xy3:100002405
        config.platformconfig.appkey = "mb4osQ39PXncw43xq3s1QnxtH99Z6nDi";//xy:0nbgXBRKtwatWw7lAYBIVdkXCgYhhOS2;xy3:mb4osQ39PXncw43xq3s1QnxtH99Z6nDi
        config.platformconfig.uidPrefix = "XYUSR_";
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "xy3";
        config.platformconfig.moneyName = "RMB";
    }
#endif
    
    
#ifdef PROJECT44755
    {
		if(isRegPlat) AUTO_REGISTER_PLATFORM(lib44755);
		platVersionName="version_ios_all.cfg";
		platFormName="lib44755";
	}
#endif

#ifdef PROJECTAPPSTOREDEBUG
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libAppStore);
		platVersionName="version_ios_appstore_debug.cfg";
		platFormName="libAppStore";
	}
#endif
    
#ifdef PROJECTAS
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(libAs);
		platVersionName="version_ios_all.cfg";
		platFormName="libAs";
        
        config.com4lovesconfig.channelid = "123";
        config.com4lovesconfig.platformid = "ios_as";
        
        config.platformconfig.appid = "225";
        config.platformconfig.appkey = "b3e68abf68574c739556163fca085f21";
        config.platformconfig.uidPrefix = "ASUSR_";
        config.platformconfig.bbsurl = "";
        config.platformconfig.clientChannel = "as";
        config.platformconfig.moneyName = "RMB";
	}
#endif

    
#ifdef PROJECTDEFAULT
    
    if (isRegPlat) AUTO_REGISTER_PLATFORM(libDefault);
    
    platVersionName = "version_ios_all.cfg";
    platFormName = "libDefault";
    
#endif
    
    
    
#ifdef WIN32
	{
		if(isRegPlat) AUTO_REGISTER_PLATFORM(lib91);
		platVersionName="version_win32.cfg";
		platFormName="lib91";
	}
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	{
		 if(isRegPlat) AUTO_REGISTER_PLATFORM(lib91);
		 platVersionName="version_android.cfg";
		 platFormName="lib91";
	}
#endif
    
    
    
#endif

	if(isRegPlat)
	{
		libPlatformManager::getInstance()->setPlatform(platFormName);
	}
}

void GamePlatformInfo::registerPlatform()
{
	init(true);
}
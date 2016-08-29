
#include "stdafx.h"

#include "LoadingFrame.h"
#include "AppDelegate.h"
#include "GameMessages.h"
#include "Language.h"
#include "SeverConsts.h"
#include "GameMaths.h"
#include "ServerDateManager.h"
#include "SoundManager.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "ActiveCodePage.h"
#include "BlackBoard.h"
#include "TimeCalculator.h"
#include "DataTableManager.h"
#include <list>
#include <vector>
//#include "com4loves.h"
#include "Base64.h"
#include "md5.h"
#include "LoginPacket.h"
#include "waitingManager.h"
#include "LoadingAniPage.h"
#include "GamePlatformInfo.h"
//#include "SeverConsts.h"
#include "GamePlatformInfo.h"
#include "SimpleAudioEngine.h"
#include "ScriptMathToLua.h"
USING_NS_CC;
USING_NS_CC_EXT;
//////////////////////////////////////////////////////////////////////////

class ReadTablesAtLoadingFrame : public ThreadTask
{
public:
	virtual int run()
	{
		GamePrecedure::Get()->readTables();
		return 0;
	}
};


//////////////////////////////////////////////////////////////////////////

REGISTER_PAGE("LoadingFrame",LoadingFrame);

#define CONNECT_DIRECT_ENABLE 0
#define CONNECT_DIRECT_ADDRESS "127.0.0.1"
#define CONNECT_DIRECT_PORT 25524


LoadingFrame::LoadingFrame(void)
	:mIsFirstLoginNotServerIDInfo(false)
	,mIsNeedShowSeversOnLogin(0),mTryUserLoginSprite(NULL),mUserLoginSprite(NULL),
	mSendLogin(false),mScene(NULL),mSelectedSeverID(0),mIsServerListBuild(false)
{
}


LoadingFrame::~LoadingFrame(void)
{
	mSendLogin=false;
}

void LoadingFrame::onShowOrHidBtn(bool v,const std::string btnName)
{

	//隐藏或者显示按钮


	if (btnName == "m_Guest")
	{
		CCNode* pGuest = this->getCCNodeFromCCB("m_Guest");
		if (pGuest)
		{
			pGuest->setVisible(v);
		}

		CCNode* pLable_mGuest = this->getCCNodeFromCCB("lable_mGuest");
		if (pLable_mGuest)
		{
			pLable_mGuest->setVisible(v);
		}
	}else if (btnName == "m_Binding")
	{

		CCNode* pBind = this->getCCNodeFromCCB("m_Binding");
		if (pBind)
		{
			pBind->setVisible(v);
		}

		CCNode* pLable_mbinding = this->getCCNodeFromCCB("lable_mbinding");
		if (pLable_mbinding)
		{
			pLable_mbinding->setVisible(v);
		}
	}
	else if (btnName == "mEntergameButton1")
	{
		CCNode* pFacebookEnter = this->getCCNodeFromCCB("mEntergameButton1");
		if (pFacebookEnter)
		{
			pFacebookEnter->setVisible(v);
		}

		CCNode* plable_mfacebook = this->getCCNodeFromCCB("lable_mfacebook");
		if (plable_mfacebook)
		{
			plable_mfacebook->setVisible(v);
		}
	}
	else if (btnName == "mEntergameButton")
	{
		CCNode* pEnter = this->getCCNodeFromCCB("mEntergameButton");
		if (pEnter)
		{
			pEnter->setVisible(v);
		}

		CCNode* plable_mstartgame = this->getCCNodeFromCCB("lable_mstartgame");
		if (plable_mstartgame)
		{
			plable_mstartgame->setVisible(v);
		}
	}
	else if (btnName == "m_Logout")
	{
		CCNode* pEnter = this->getCCNodeFromCCB("m_Logout");
		if (pEnter)
		{
			pEnter->setVisible(v);
		}

		CCNode* plable_mstartgame = this->getCCNodeFromCCB("lable_mLogout");
		if (plable_mstartgame)
		{
			plable_mstartgame->setVisible(v);
		}
	}
		
}

void LoadingFrame::Enter( GamePrecedure* )
{

	

	CCLOG("enter loading frame");
	if (VaribleManager::Get()->getSetting("OpenAnalytics") == "1")
	{
        //modify by dylan at 20140105 ios use flurry 
		//libOS::getInstance()->initAnalytics("P9X5XZSV5Z762TH334BW");
        //for mxhzw ios MTA appid IPI6IR1NR34Q,flurry appkey GBQ4Q5T9Q74MW3FWDXNF
		/*std::string _analytiscAppId="GBQ4Q5T9Q74MW3FWDXNF";
		const std::string projectName=VaribleManager::Get()->getSetting("projectName","","dragonball");
		if(projectName!="dragonball")
		{
			_analytiscAppId="P9X5XZSV5Z762TH334BW";
		}
		libOS::getInstance()->initAnalytics(_analytiscAppId);*/
	}
	//
    mIsUpdateDown = false;
	mLogined = false;
	m91checked = false;
    mNetWorkNotWorkMsgShown = false;
	mSelectedSeverID = -1;//初始化
	//
	checkstate = SeverConsts::CS_NOT_STARTED;
	updatestate = SeverConsts::US_NOT_STARTED;
    
	MessageManager::Get()->regisiterMessageHandler(MSG_LOADINGFRAME_ENTERGAME,this);
	//
	libOS::getInstance()->registerListener(this);

	//读配置文件判断，是否协议压缩 by zhenhui 2014/8/23
	bool isCompress = StringConverter::parseBool(VaribleManager::Get()->getSetting("isPacketCompress", "", "true"),true);
	PacketManager::Get()->setCompress(isCompress);
	/*
		创建与（apk|ipa）loader程序的衔接
	*/
	libPlatformManager::getPlatform()->registerListener(this);
    CCLOG("init severConst...");
    SeverConsts::Get()->init(GamePlatformInfo::getInstance()->getPlatVersionName());
    CCLOG("init lib91...");
	/*
		这个Platform()->init在Android无作用
	*/
    
	//--zhengxin 2014-08-10
	if (GamePrecedure::Get()->isReEnterLoadingFrame())
	{
		libPlatformManager::getPlatform()->logout();
	}
	//
//    std::string privateLogin = VaribleManager::Get()->getSetting("privateLogin","","false");
//    if(privateLogin != "true")
//        libPlatformManager::getPlatform()->init();
//    else
//        libPlatformManager::getPlatform()->init(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (!GamePrecedure::Get()->isReEnterLoadingFrame())
        libPlatformManager::getPlatform()->initWithConfigure(GamePlatformInfo::getInstance()->getPlatFromconfig());
#else
    libPlatformManager::getPlatform()->initWithConfigure(GamePlatformInfo::getInstance()->getPlatFromconfig());
#endif

    //lib91::getInstance()->showInputbox(true);
    //lib91::getInstance()->openURL("http://www.baidu.com");
    //libOS::getInstance()->setWaiting(true);//
    
    CCLOG("init lib91 done...");
	
	mScene = CCScene::create();
	mScene->retain();

	mScene->addChild(this);

    CCLOG("load ccbi file");
	
	load();
    CCB_FUNC(this, "mEnterButton", CCNode, setVisible(false));
    CCB_FUNC(this, "mLoginNode", CCNode, setVisible(false));
    CCLabelBMFont* eb = dynamic_cast<CCLabelBMFont*>(getVariable("mSeverName1"));
    if(eb)eb->setString("");
	
	CCLabelTTF* eb1 = dynamic_cast<CCLabelTTF*>(getVariable("mLoginName"));
	if(eb1)
	{
		eb1->setString("");
	}
	else
	{
		CCLabelBMFont* eb2 = dynamic_cast<CCLabelBMFont*>(getVariable("mLoginName"));
		if(eb2)eb2->setString("");
	}
	mTryUserLoginSprite = CCSprite::create(VaribleManager::Get()->getSetting("LoadingFrameTryUserLoginSprite","","loadingScene/u_tryloginbutten.png").c_str());
	mTryUserLoginSprite->retain();
	mUserLoginSprite = CCSprite::create(VaribleManager::Get()->getSetting("LoadingFrameUserLoginSprite","","loadingScene/u_StartButton.png").c_str());
	mUserLoginSprite->retain();
    
	showUpdateChecking();
	
	CCB_FUNC(this,"mVer",cocos2d::CCLabelBMFont,setVisible(false));
	CCB_FUNC(this,"mVer",cocos2d::CCLabelBMFont,setString(""));
    CCLOG("************start running LoadingFrame scene**************");
	
	CCDirector::sharedDirector()->setDepthTest(true);
	// run
	if(cocos2d::CCDirector::sharedDirector()->getRunningScene())
		cocos2d::CCDirector::sharedDirector()->replaceScene(mScene);
	else
		cocos2d::CCDirector::sharedDirector()->runWithScene(mScene);
    
	std::string loadingSceneMsg=Language::Get()->getString("@loading2");
	

	const PlatformRoleItem* item=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
	if(item)
	{
		loadingSceneMsg=item->loadinScenceMsg;
	}

	CCB_FUNC(this,"mLoadingScenceMsg",CCLabelBMFont,setString(loadingSceneMsg.c_str()));

    CCLOG("************loading frame enter done**************");
	if (GamePrecedure::Get()->isReEnterLoadingFrame())
	{
		onUpdate(NULL, true, "");//重进这种情况，刷新一下
	}
	else
	{
#ifndef _CLOSE_MUSIC
		SoundManager::Get()->init();
#endif
	}
#ifndef _CLOSE_MUSIC
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	SoundManager::Get()->playLoadingMusic();
#endif
	//


# ifdef Macro_Platform_Thai
	this->onShowOrHidBtn(false,"m_Binding");
	this->onShowOrHidBtn(false,"mEntergameButton");
	this->onShowOrHidBtn(false,"m_Logout");
#endif



}

void LoadingFrame::showAnnounce()
{
	static bool allShow = StringConverter::parseBool(VaribleManager::Get()->getSetting("ShowAnnounceSmallExit"), true);
	if ( !allShow )
	{
		static unsigned int showCount = 0;
		if ( showCount > 0 )
		{
			return;
		}
		++showCount;
	}

	BasePage* page = CCBManager::Get()->getPage("AnnouncePage");
	if ( page && !page->getParent() )
	{
		page->load();
		State<MainFrame>* sta = dynamic_cast<State<MainFrame>* >(page);
		if ( sta )
		{
			sta->Enter(NULL);
		}
		addChild(page);
		page->setPosition(ccp(0, 0));
	}

	
}

void LoadingFrame::clearAnnounce()
{
	BasePage* page = CCBManager::Get()->getPage("AnnouncePage");
	if ( page )
	{
		page->unload();
		State<MainFrame>* sta = dynamic_cast<State<MainFrame>* >(page);
		if ( sta )
		{
			sta->Exit(NULL);
		}
	}
}

void LoadingFrame::Exit( GamePrecedure* )
{	
	mIsServerListBuild = false;
	libOS::getInstance()->removeListener(this);
	clearAnnounce();
	/*
		解除与loader的衔接
	*/
    libPlatformManager::getPlatform()->removeListener(this);
	PacketManager::Get()->removePacketHandler(this);
	MessageManager::Get()->removeMessageHandler(this);
	//
	mTryUserLoginSprite->removeFromParent();
	mTryUserLoginSprite->release();
	mTryUserLoginSprite = NULL;
	mUserLoginSprite->removeFromParent();
	mUserLoginSprite->release();
	mUserLoginSprite = NULL;
	//
	unload();
	mScene->removeAllChildrenWithCleanup(true);
	mScene->release();
	CCLOG("loading frame mScene retainCount:%d",mScene->retainCount());
	mScene = NULL;
	//
	mThread.shutdown();
}

void LoadingFrame::Recycle()
{

}

void LoadingFrame::Execute( GamePrecedure* gp)
{
    static bool _isFirstIn=true;
	if(_isFirstIn)
	{
		_isFirstIn=false;
	}
    
	SeverConsts::Get()->update(gp->getFrameTime());

    bool enterShown = false;
    if(getVariable("mLoginNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mLoginNode"));
		enterShown = sever->isVisible();
	}
    
	//static SeverConsts::CHECK_STATE checkstate = SeverConsts::CS_NOT_STARTED ;
	
	if(checkstate!=SeverConsts::Get()->checkUpdateInfo())
	{
		checkstate = SeverConsts::Get()->checkUpdateInfo();

		if(	checkstate == SeverConsts::CS_NEED_UPDATE &&
			SeverConsts::Get()->checkUpdateState() == SeverConsts::US_NOT_STARTED)
		{
			std::string strMsg;
			float fsizem = SeverConsts::Get()->getNeedUpdateTotalBytes();
			char szTmp[256] = {0};
			bool isQuietUpdate = false;
			isQuietUpdate = SeverConsts::Get()->isQuietUpdateActive();
			
			if (fsizem < 1024u )
				sprintf(szTmp, "(1Kb)");
			else if (fsizem < 1024u*1024u)
				sprintf(szTmp, "(%dKb)", (int)fsizem/1024);
			else
				sprintf(szTmp, "(%fM)", fsizem/1024.f/1024.f);
			//
            if(libOS::getInstance()->getNetWork() == ReachableViaWWAN)
			{
				strMsg = Language::Get()->getString("@LOADINGFRAME_NeedUpdate3G");
			}
            else
			{
				strMsg = Language::Get()->getString("@LOADINGFRAME_NeedUpdate");
			}

			std::string strUpdateMsg = SeverConsts::Get()->getNeedUpdateMsg();
			if (!strUpdateMsg.empty())
			{
				strUpdateMsg.append("\n");
				strUpdateMsg.append(strMsg);
			}
			else
				strUpdateMsg = strMsg;
			//如果不是静默下载，弹出消息框，点击确认下载，否则，直接下载资源
			if (isQuietUpdate == false)
			{
				libOS::getInstance()->showMessagebox(strUpdateMsg, 100);
			}else{
				SeverConsts::Get()->updateResources();
			}
			
		}
		else if(checkstate == SeverConsts::CS_NEED_STORE_UPDATE)
		{
			//modify by dylan merge Platform webkit differentiation  
				unsigned int isUsePrivateBigVersionUpdate=1;
#ifdef WIN32
				isUsePrivateBigVersionUpdate=1;
#else
				const PlatformRoleItem* item=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
				if(item)
				{
					isUsePrivateBigVersionUpdate=item->isUsePrivateBigVersionUpdate;
				}
#endif
			if(isUsePrivateBigVersionUpdate==1)
			{
				std::string strMsg = SeverConsts::Get()->getDirectDownloadMsg();
				strMsg.append("\n");
				//
				if(libOS::getInstance()->getNetWork() == ReachableViaWWAN)
				{
					strMsg.append(Language::Get()->getString("@LOADINGFRAME_NeedUpdate3G"));
				}
				else
				{
					strMsg.append(Language::Get()->getString("@LOADINGFRAME_NeedUpdateVersion"));
				}

				libOS::getInstance()->showMessagebox(strMsg,110);

			}
            else
            {
                #ifdef PROJECTKY
                    libPlatformManager::getPlatform()->login();
                #endif
            }
			SeverConsts::Get()->clearVersion();
		}
		else if(checkstate == SeverConsts::CS_FAILED)
		{
			showUpdateCheckingFailed();
		}
		else if ( checkstate == SeverConsts::CS_OK )
		{
			showAnnounce();
		}
	}
    
	//static SeverConsts::UPDATE_STATE updatestate = SeverConsts::US_NOT_STARTED;

	if( updatestate!=SeverConsts::Get()->checkUpdateState())
	{
		updatestate = SeverConsts::Get()->checkUpdateState();
		if(updatestate == SeverConsts::US_CHECKING)
		{
			showUpdateFileChecking();
		}
		else if(updatestate == SeverConsts::US_DOWNLOADING)
		{
			bool isQuietUpdate = SeverConsts::Get()->isQuietUpdateActive();
			if (isQuietUpdate == true)
			{
				showUpdateFileChecking();
			}else{
				showUpdateUpdating();
			}
			
		}
		else if(updatestate == SeverConsts::US_FAILED)
		{
			showUpdateFailed();
		}
	}

    if(checkstate == SeverConsts::CS_OK && m91checked && !enterShown && !mIsUpdateDown)
	{
		//
		if (updatestate == SeverConsts::US_OK)
		{
			
			bool isQuietUpdate = SeverConsts::Get()->isQuietUpdateActive();
			if (isQuietUpdate == false)
			{
				showPersent(1.0f);//需要更新，更新完成
			}
			showAnnounce();
		}
		//
		showUpdateDone();//mIsUpdateDown = true;
		//--zhengxin 2014-08-10
		if (updatestate == SeverConsts::US_OK)
		{
			if (GamePrecedure::Get()->isReEnterLoadingFrame())
			{
				//todo弹出对话框强制重启
				libOS::getInstance()->requestRestart();
			}
		}
		//
	}
    
	if(updatestate == SeverConsts::US_DOWNLOADING)
	{
		unsigned long total = SeverConsts::Get()->getUpdateTotalCount();
		unsigned long done = SeverConsts::Get()->getUpdatedCount();
		if(total>0)
		{
			float per = (float)done/(float)total;
			std::string _sizeTip="";
			std::list<std::string> replaceList;
			replaceList.push_back(StringConverter::toString(done/1024));
			replaceList.push_back(StringConverter::toString(total/1024));
			showPersent(per,GameMaths::replaceStringWithStringList(Language::Get()->getString("@UpdateSizeInfo"),&replaceList));
		}
		
	}
	if (mIsNeedShowSeversOnLogin > 0)
	{
		--mIsNeedShowSeversOnLogin;
		if (mIsNeedShowSeversOnLogin <= 0)
		{
			mIsNeedShowSeversOnLogin=0;
			showSevers(true);
		}
	}

	CCNode* node = dynamic_cast<CCNode*>(getVariable("mNodeFront"));
	if (node)
	{
		LoadingAniPage* page = dynamic_cast<LoadingAniPage*>(node->getChildByTag(LoadinAniPageTag));
		if (page && !page->getIsInWaiting())
		{
			page->runLoadingAni();
		}
	}
}



void LoadingFrame::onReceiveMassage( const GameMessage * message )
{
	if(message->getTypeId() == MSG_LOADINGFRAME_ENTERGAME)
	{
		GamePrecedure::Get()->enterMainMenu();
	}
}


void LoadingFrame::onLogin(libPlatform* lib, bool success, const std::string& log)
{
    if(success)
    {
		std::string uin = lib->loginUin();
        
		libOS::getInstance()->initUserID(uin);

        {
            mLogined = true;
            showEnter();
        }
        
		/*
        static int loginCount = 0;
        if(loginCount == 0)
        {
            GamePrecedure::Get()->showBulletin();
            loginCount = 1;
        }
		*/
        
        /**
        const std::string address = SeverConsts::Get()->getAnnouncement();
        if(!address.empty())
        {
            static int loginCount = 0;
            if(loginCount == 0)
			{
//modify by dylan merge Platform webkit differentiation  
				bool _webkitOpenStatus=true;
#ifdef WIN32
				_webkitOpenStatus=true;
#else
				const PlatformRoleItem* item=PlatformRoleTableManager::Get()->getPlatformRoleByName(libPlatformManager::getPlatform()->getClientChannel());
				if(item)
				{
					_webkitOpenStatus=item->webkitOpenStatus;
				}
#endif

                if(_webkitOpenStatus)
                {//itools sdk hide webkit notice because has bug!
                    
                    ++loginCount;
                    BulletinBoardPage* page = BulletinBoardPage::getInstance();
                    page->init(120, 70, 300, 500, this);
                    page->show(address);
                }
                
            }
        }
         */
        int newServerId = getDefaultSeverID();
		{
			SeverConsts::SEVERLIST::const_iterator it = SeverConsts::Get()->getSeverList().find(newServerId);
			if(it == SeverConsts::Get()->getSeverList().end())
				newServerId = SeverConsts::Get()->getSeverDefaultID();
		}
        if(newServerId!=mSelectedSeverID)
        {
            mSelectedSeverID = newServerId;
            updateSeverName();
        }
        if(getVariable("mSeverNode"))
        {
            CCNode* sever = dynamic_cast<CCNode*>(getVariable("mSeverNode"));
            if(sever &&mIsFirstLoginNotServerIDInfo)
            {
				mIsFirstLoginNotServerIDInfo = false;
            }
        }
	}else{
		CCLog("libPlatformManager showEnter false");
		mLogined = false;
		showEnter();
	}
}

void LoadingFrame::onPlatformLogout(libPlatform*)
{
    if(!libPlatformManager::getPlatform()->getLogined())
    {
        CCLabelTTF* eb = dynamic_cast<CCLabelTTF*>(getVariable("mLoginName"));
		if(eb)
			eb->setString("Login");
		else
		{
			CCLabelBMFont* eb2 = dynamic_cast<CCLabelBMFont*>(getVariable("mLoginName"));
			if(eb2)eb2->setString("Login");
		}
        CCMessageBox(Language::Get()->getString("@LOADINGFRAME_HAVENOT_LOGIN").c_str(),"");
    }
};

void LoadingFrame::onUpdate(libPlatform*,bool ok, std::string msg)
{
    if(!ok)
    {
		CCLog("LoadingFrame::onUpdate failed");
        m91checked = false;
        libOS::getInstance()->showMessagebox(msg);
    }
    else
    {
		if (!m91checked)
		{
			CCLog("LoadingFrame::onUpdate success");
			m91checked = true;
			SeverConsts::Get()->start();
		}
    }
}

void LoadingFrame::onMessageboxEnter(int tag)
{
    if(tag == 100)
    {
        SeverConsts::Get()->updateResources();
    }
	else if (tag == 110)
	{
		//xinzheng 2013-07-15
#if defined(ANDROID)
		libOS::getInstance()->openURL(SeverConsts::Get()->getDirectDownloadUrl());
#else
        if(VaribleManager::Get()->getSetting("useInsideAppUpdate")!="true")
		{
			//modified by dylan merge ios version_xx.cfg and sever_xx.php at 20131108
			if(!SeverConsts::Get()->getDirectDownloadUrl().empty()&&SeverConsts::Get()->getDirectDownloadUrl().length()>5)
			{
				libOS::getInstance()->openURL(SeverConsts::Get()->getDirectDownloadUrl());
			}
			else
			{
				libOS::getInstance()->openURL(SeverConsts::Get()->getInStoreUpdateAddress());
			}
		}
        else
        {
			libPlatformManager::getInstance()->getPlatform()->updateApp();
		}
#endif
		//
	}
	else if(tag==Err_CheckingFailed)
	{
		exit(0);
	}
	else if(tag==Err_UpdateFailed)
	{
		exit(0);
	}
	else if(tag==Err_ConnectFailed)
	{
		//exit(0);//zhengxin 2013-12-24 not exit
	}
	else if(tag==Err_ErrMsgReport)
	{
		mSendLogin=false;
		if(!waitingManager::Get()->getWaiting())
		{
			showEnter();
		}
	}
	else if(tag == 120)
	{
		mNetWorkNotWorkMsgShown = false;
	}
}
void LoadingFrame::onMenuItemAction( const std::string& itemName ,cocos2d::CCObject* sender)
{
	if(itemName == "onSever")
	{
		showSevers(true);
	}
	if(itemName == "onClose")
	{
		showSevers(false);
	}else if (itemName == "onBinding")
	{


		libPlatformManager::getPlatform()->onBinding();
	}
	else if (itemName == "onGuest")
	{
		libPlatformManager::getPlatform()->onGuest();
	}else if (itemName == "onLogout")
	{
		libPlatformManager::getPlatform()->elogout();
	}
	//else if (itemName == "onEnter1")
	//{
	//	libPlatformManager::getPlatform()->login();
	//}
	else if(itemName =="onEnter" || itemName == "onEnter1")
	{
		GamePrecedure::Get()->setInLoadingScene();
		//CCSpriteFrameCache * frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
		//frameCache->addSpriteFramesNameWithFile("Imagesetfile/LoadingAni.plist");

#ifdef PROJECT91Quasi
#else

        if(libPlatformManager::getPlatform()->loginUin().empty() || !libPlatformManager::getPlatform()->getLogined())
        {
			//get user name error.
            libPlatformManager::getPlatform()->login();
            return;
        }
        if(mSelectedSeverID<0)
        {
            showSevers(true);
            return;
        }
#endif

#ifdef WIN32
#endif
		
		SeverConsts::SEVERLIST::const_iterator it = SeverConsts::Get()->getSeverList().find(mSelectedSeverID);
		if(it!=SeverConsts::Get()->getSeverList().end())
		{
#if CONNECT_DIRECT_ENABLE == 1
			PacketManager::Get()->init(CONNECT_DIRECT_ADDRESS,25524);
#else
			PacketManager::Get()->init(it->second->address,it->second->port);
#endif 
			showEnterAndLoginingIn();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && defined(Macro_AndroidTestJiHuoMa)
			PacketManager::Get()->registerPacketHandler(OPCODE_ACTIVE_CODERET_S,this);
#endif
			//ServerDateManager::Get()->setSeverID(mSelectedSeverID);
			PacketManager::Get()->registerPacketHandler(LOGIN_S,this);
			PacketManager::Get()->registerPacketHandler(ERROR_CODE,this);
			
			//modify by dylan at 20131112 login interface Base64 And MD5
			HPLogin loginPack;
			GamePrecedure::Get()->setServerID(mSelectedSeverID);
			GamePrecedure::Get()->setAlertServerUpdating(false);
			std::string uin = libPlatformManager::getPlatform()->loginUin();
            
#ifdef PROJECT91Quasi
            if(gPuid != "")
			{
                uin = gPuid;
				GamePrecedure::Get()->setLoginUin(uin);
            }
#endif
			std::string platformInfo = libPlatformManager::getPlatform()->getPlatformInfo()+"#"+StringConverter::toString(libPlatformManager::getPlatform()->getPlatformId());
            std::string deviceID = libOS::getInstance()->getDeviceID();
			 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			// Samsung i9100's audio driver has a bug , it can not accept too much effects concurrently.
			if(((platformInfo.find("samsung")!=std::string::npos)||(platformInfo.find("SAMSUNG")!=std::string::npos))&&(platformInfo.find("9100")!=std::string::npos))
			{
				BlackBoard::Get()->isSamSungi9100Audio = true;				
			}
#endif
			GamePrecedure::Get()->setUin(uin);			

			
			if(uin.empty()) uin="none";
			if(deviceID.empty()) deviceID="none";
			if(!platformInfo.empty())
				loginPack.set_platform(platformInfo);
#ifdef PROJECT91Quasi
            //add by xinghui
            if (uin.empty()) 
			{
                libOS::getInstance()->showInputbox(true);
            }
			else
            {
#endif
			loginPack.set_puid(uin);
			loginPack.set_deviceid(deviceID);
			
			loginPack.set_version(SeverConsts::Get()->getVersion());

			mSendLogin=true;
			LoginPacket::Get()->setEnabled(false);
			PacketManager::Get()->sendPakcet(LOGIN_C,&loginPack);
			//caculate the time to kill game when player enter background
			if(VaribleManager::Get()->getSetting("ExitGameTime")!="")
			{
				TimeCalculator::getInstance()->createTimeCalcultor("ExitGameTime",StringConverter::parseInt(VaribleManager::Get()->getSetting("ExitGameTime")));
			}
#ifdef PROJECT91Quasi
            }
#endif
		}
		else
		{
			mSelectedSeverID = -1;
			showSevers(true);
			return;
		}
	}
    else if(itemName =="onLogin")
	{
		CCLOG("onLogin clicked");
        mLogined = false;
#ifdef PROJECT91Quasi
        
        libOS::getInstance()->showInputbox(true);
#else
        libPlatformManager::getPlatform()->switchUsers();
#endif
	}
	CCLOG("LoadingFrame::onMenuItemAction %s: ", itemName.c_str());
}
void LoadingFrame::onInputboxEnter(const std::string& content)
{
    gPuid = content;
    CCLabelTTF* eb1 = dynamic_cast<CCLabelTTF*>(getVariable("mLoginName"));
    if (eb1) {
        eb1->setString(gPuid.c_str());
    }else
    {
        CCLabelBMFont* eb2 = dynamic_cast<CCLabelBMFont*>(getVariable("mLoginName"));
        if (eb2) {
            eb2->setString(gPuid.c_str());
        }
    }
    
//#ifdef PROJECT91Quasi
//    //add by xinghui
//    CCLabelTTF* eb1 = dynamic_cast<CCLabelTTF*>(getVariable("mLoginName"));
//    if (eb1) {
//        eb1->setString(gPuid.c_str());
//    }else
//    {
//        CCLabelBMFont* eb2 = dynamic_cast<CCLabelBMFont*>(getVariable("mLoginName"));
//        if (eb2) {
//            eb2->setString(gPuid.c_str());
//        }
//    }
//#endif
}


void LoadingFrame::onMenuItemAction( const std::string& itemName, cocos2d::CCObject* sender, int tag )
{
	mSelectedSeverID = tag;
	_updateLocalSeverId();//add by dylan at 20130711 HZW-1623
	updateSeverName();

	showSevers(false);
}

void LoadingFrame::_updateLocalSeverId()
{
	std::string uinkey = "DefaultSeverID";// + libPlatformManager::getPlatform()->loginUin();
	cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(uinkey.c_str(),mSelectedSeverID);
	cocos2d::CCUserDefault::sharedUserDefault()->flush();
}

void LoadingFrame::load( void )
{
	loadCcbiFile("LoadingFrame.ccbi");
}

void LoadingFrame::showSevers(bool _show)
{
	if(getVariable("mSeverNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mSeverNode"));
		if(sever) 
		{
			
			sever->setVisible(_show);
			if (mIsServerListBuild)
			{
				return;
			}

			if(_show && getVariable("mSeverRecentList"))
			{
				CCScrollView* severlist = dynamic_cast<CCScrollView*>(getVariable("mSeverRecentList"));

				CCNode* attachNode = CCNode::create();

				CCBContainer * node = CCBContainer::create();
				node->loadCcbiFile("LoadingFrameSeverRecentList.ccbi");
				float singleButtonHeight=0;
				int count = 0;
				int noteCount = 0;

				if(node->getVariable("mRecent"))
				{
                    count = 0;//com4loves::getServerInfoCount();
					int trueCount = count;
                   
                    std::vector<int> *servers = new std::vector<int>();
                    
                    if(count == 0)
                        count = 1;
                    
                    std::string uinkey = "DefaultSeverID";// + libPlatformManager::getPlatform()->loginUin();
					int defid = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(uinkey.c_str(),SeverConsts::Get()->getSeverDefaultID());
                    
                    for(int i=0; i<count;++i)
                    {
						//add by dylan at 20131217,recentList max record 6 HZW-2236
						if(noteCount>6) break;
                        int serverIDtoforRecent = defid;
                        if(trueCount>0)
						{
                            int numToFind = 0;//com4loves::getServerUserByIndex(i);
							std::vector<int>::iterator iter = std::find(servers->begin(),servers->end(),numToFind);
							if (iter==servers->end()) 
							{
								serverIDtoforRecent=numToFind;
								servers->push_back(numToFind);
							}
							else
							{
								continue;
							}
						}
                            
                                                
                        SeverConsts::SEVERLIST::const_iterator it = SeverConsts::Get()->getSeverList().find(serverIDtoforRecent);
                        if(it==SeverConsts::Get()->getSeverList().end())
                            continue;
                                              
                        CCNode* recentNode = dynamic_cast<CCNode*>(node->getVariable("mRecent"));
                        if(noteCount%2==1)
                           recentNode = dynamic_cast<CCNode*>(node->getVariable("mRecent2"));
                        
                        if(recentNode && it!=SeverConsts::Get()->getSeverList().end())
                        {
                            CCBContainer* button = CCBContainer::create();
                            button->setListener(this,it->second->id);
                            button->loadCcbiFile("LoadingFrameSever.ccbi");
                            if(button->getVariable("mSeverName"))
                            {
                                CCLabelTTF* str = dynamic_cast<CCLabelTTF*>(button->getVariable("mSeverName"));
                                if(str)	str->setString(it->second->name.c_str());
                            }

                            CCNode* fullSever = 0;
                            CCNode* newSever = 0;
                            if(button->getVariable("mSeverFull"))
                                fullSever = dynamic_cast<CCNode*>(button->getVariable("mSeverFull"));
                            if(button->getVariable("mSeverNew"))
                                newSever = dynamic_cast<CCNode*>(button->getVariable("mSeverNew"));
                            
                            if(fullSever) fullSever->setVisible(it->second->state == SeverConsts::SS_FULL);
                            if(newSever)  newSever->setVisible(it->second->state == SeverConsts::SS_NEW);
                            singleButtonHeight = button->getContentSize().height;
							
							button->setPositionY(-(singleButtonHeight)*(noteCount-noteCount%2)*0.5f);//(i-i%2)*0.5f

                            recentNode->addChild(button);
                        }
						noteCount++;
                    }
                    delete servers;
                }
				attachNode->addChild(node);
				
                //count--;
                float offpos1 = singleButtonHeight * ((noteCount + 1)/2 - 1);//count
                if(offpos1<severlist->getContentSize().height - singleButtonHeight)
                    offpos1 = severlist->getContentSize().height - singleButtonHeight;             
				//count++;
				node->setPosition(0,offpos1);

				CCSize size;				
				size.width  = node->getContentSize().width;
                size.height = (noteCount+noteCount%2)*0.5f*singleButtonHeight;//count
				//size.height = singleButtonHeight * ((count+1)/2);
				//float offpos = severlist->getContentSize().height-size.height;
                if(severlist->getContentSize().height>size.height)
                    size.height = severlist->getContentSize().height;
				
				attachNode->setContentSize(size);
				severlist->setContainer(attachNode);
				severlist->setContentSize(size);
				severlist->setContentOffset(ccp(0,severlist->getViewSize().height-size.height));
			}

			if(_show && getVariable("mSeverList"))
			{
				CCScrollView* severlist = dynamic_cast<CCScrollView*>(getVariable("mSeverList"));
				//modified by zhenhui 2014/6/4 for the penetrating bug of scrollview
				CCTouchHandler* pHandler = CCDirector::sharedDirector()->getTouchDispatcher()->findHandler(severlist);
				if (pHandler)
				{
					CCTargetedTouchHandler* pTh = dynamic_cast<CCTargetedTouchHandler*>(pHandler);
					if (pTh)
					{
						pTh->setSwallowsTouches(true);
					}
				}

				//引入Facade，避免区服过多卡顿
				CCReViScrollViewFacade* m_pScrollViewFacade = new CCReViScrollViewFacade(severlist);
				m_pScrollViewFacade->init(4, 3);
				int iCount = 0;
				int iMaxNode = m_pScrollViewFacade->getMaxDynamicControledItemViewsNum();
				CCBContainer* oneItem = CCBContainer::create();
				oneItem->loadCcbiFile("LoadingFrameSever.ccbi");
				float singleButtonHeight = oneItem->getContentSize().height;
				float singleNodeWidth = 0;
				unsigned int totalSize = SeverConsts::Get()->getSeverList().size();
				if (totalSize%2 == 0)
				{
					totalSize = totalSize/2;
				}
				else
				{
					totalSize = totalSize/2 + 1;
				}
				for(int i=0;i<totalSize;i++)
				{
					CCReViSvItemData* pItemData = new CCReViSvItemData();
					pItemData->m_iIdx = totalSize - iCount;
					//countID--;
					float offpos1 = singleButtonHeight * (iCount);

					pItemData->m_ptPosition = ccp(0, offpos1);

					if (iCount < iMaxNode) 
					{
						ServerListContent * node = new ServerListContent(totalSize -iCount,this);
						node->initItemView();
						CCSize originSize = node->getContainerNode()->getContentSize();
						originSize.height = singleButtonHeight;
						node->getContainerNode()->setContentSize(originSize);
						
						singleNodeWidth = node->getContainerNode()->getContentSize().width;


						m_pScrollViewFacade->addItem(pItemData,node);
						
					}
					else
					{
						m_pScrollViewFacade->addItem(pItemData);
					}
					iCount++;
				}

				CCSize size;
				size.width = singleNodeWidth;
				size.height = (iCount)*singleButtonHeight;
				if(severlist->getContentSize().height>size.height)
					size.height = severlist->getContentSize().height;
				CCLog("contentheight:%f",severlist->getContentSize().height);
				severlist->setContentSize(size);
				severlist->setContentOffset(ccp(0,severlist->getViewSize().height-severlist->getContentSize().height * severlist->getScaleY()));
				m_pScrollViewFacade->setDynamicItemsStartPosition(iCount-1);
				severlist->forceRecaculateChildren();
				ScriptMathToLua::setSwallowsTouches(severlist);
				mIsServerListBuild = true;
			}
		}
	}
}

std::list<SeverConsts::SEVER_ATTRIBUTE> ServerListContent::mOrderlist;
void ServerListContent::initItemView()
{
	mContentContainer = CCBManager::Get()->createAndLoad(getCcbiFileName());
	mContentContainer->setListener(mListener);
	mContentContainer->retain();
	this->m_pItemNode = mContentContainer;

	if (mOrderlist.size() != SeverConsts::Get()->getSeverList().size())
	{
		mOrderlist.clear();
		SeverConsts::SEVERLIST::const_reverse_iterator it = SeverConsts::Get()->getSeverList().rbegin();
		for(;it!=SeverConsts::Get()->getSeverList().rend();++it)
		{
			mOrderlist.push_back(*it->second);
		}
		mOrderlist.sort();
	}
}

void ServerListContent::refreshItemView(cocos2d::extension::CCReViSvItemData* pItem)
{
	int index = pItem->m_iIdx - 1;
	unsigned int perLine = 1;
	std::list<SeverConsts::SEVER_ATTRIBUTE>::iterator itOrdered = mOrderlist.begin();
	if (mOrderlist.size() > (index*2))
	{
		if(mOrderlist.size() >= index*2 + 2)
		{
			perLine = 2;
		}
		for (int i=0;i<index*2;i++)
		{
			itOrdered++;
		}
	}
	else
	{
		return;
	}
	int countID = 0;
	if(mContentContainer->getVariable("mAll1") && mContentContainer->getVariable("mAll2"))
	{
		CCNode* allNode1 = dynamic_cast<CCNode*>(mContentContainer->getVariable("mAll1"));
		CCNode* allNode2 = dynamic_cast<CCNode*>(mContentContainer->getVariable("mAll2"));

		allNode2->removeAllChildren();
		allNode1->removeAllChildren();

		for (int i=0;i<perLine;i++)
		{
			if(itOrdered!=mOrderlist.end())
			{
				CCBContainer* button = CCBContainer::create();
				button->setListener(mListener,itOrdered->id);
				button->loadCcbiFile("LoadingFrameSever.ccbi");
				if(button->getVariable("mSeverName"))
				{
					CCLabelTTF* str = dynamic_cast<CCLabelTTF*>(button->getVariable("mSeverName"));
					if(str)	str->setString(itOrdered->name.c_str());
				}
				CCNode* fullSever = 0;
				CCNode* newSever = 0;
				if(button->getVariable("mSeverFull"))
					fullSever = dynamic_cast<CCNode*>(button->getVariable("mSeverFull"));
				if(button->getVariable("mSeverNew"))
					newSever = dynamic_cast<CCNode*>(button->getVariable("mSeverNew"));

				if(fullSever) fullSever->setVisible(itOrdered->state == SeverConsts::SS_FULL);
				if(newSever) newSever->setVisible(itOrdered->state == SeverConsts::SS_NEW);

				
				if(countID %2 ==0)
				{
					allNode1->addChild(button);
				}
				else
				{
					allNode2->addChild(button);
				}
				countID++;
				itOrdered++;
			}
		}

	}
}
void LoadingFrame::showUpdateChecking()
{
	if(getVariable("mPostUpdateNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mPostUpdateNode"));
		if(sever) sever->setVisible(false);
	}
	if(getVariable("mPersentageNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mPersentageNode"));
		if(sever) sever->setVisible(false);
	}
	if(getVariable("mWaitingNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mWaitingNode"));
		if(sever) sever->setVisible(true);
	}
	if(getVariable("mUpdateDescription"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
		if(sever)
		{
			sever->setString(Language::Get()->getString("@UpdateDescriptionChecking").c_str(),true);
		}
	}
}
void LoadingFrame::showUpdateCheckingFailed()
{
	if(getVariable("mUpdateDescription"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
		if(sever)
		{
			sever->setString(Language::Get()->getString("@UpdateDescriptionCheckingFailed").c_str(),true);
		}
	}

	if(libOS::getInstance()->getNetWork()==NotReachable)
	{
		libOS::getInstance()->showMessagebox(Language::Get()->getString("@NoNetWork"),Err_CheckingFailed);
	}
	else
	{
		libOS::getInstance()->showMessagebox(Language::Get()->getString("@UpdateDescriptionCheckingFailed"),Err_CheckingFailed);
	}
}
void LoadingFrame::showUpdateFileChecking()
{

	if(getVariable("mUpdateDescription"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
		if(sever)
		{
			sever->setString(Language::Get()->getString("@UpdateDescriptionUpdatingCheck").c_str(),true);
		}
	}
}

void LoadingFrame::showUpdateUpdating()
{
	showPersent(0);
	if(getVariable("mPersentageNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mPersentageNode"));
		if(sever) sever->setVisible(true);
	}
	
	if(getVariable("mUpdateDescription"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
		if(sever)
		{
			sever->setString(Language::Get()->getString("@UpdateDescriptionUpdating").c_str(),true);
		}
	}
}
void LoadingFrame::showUpdateFailed()
{
	std::list<std::string> replaceList;
	replaceList.push_back(SeverConsts::Get()->getFailedName());
	std::string _errStr=GameMaths::replaceStringWithStringList(Language::Get()->getString("@UpdateDescriptionupdateFailed"),&replaceList);
	if(getVariable("mUpdateDescription"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
		if(sever)
		{
			sever->setString(_errStr.c_str(),true);
		}
	}
	if(libOS::getInstance()->getNetWork()==NotReachable)
	{
		libOS::getInstance()->showMessagebox(Language::Get()->getString("@NoNetWork"),Err_UpdateFailed);
	}
	else
	{
		libOS::getInstance()->showMessagebox(_errStr,Err_UpdateFailed);
	}
}
void LoadingFrame::showUpdateDone()
{
	//if (mIsUpdateDown)
	//	return;
	//

	//逻辑有点乱，showUpdateDone执行2遍才达到效果：登录转菊花，按钮不可点，登录可能失败，菊花消失，继续可点击

	if (!mIsUpdateDown)
	{
		libOS::getInstance()->setWaiting(false);
	}

    mIsUpdateDown = true;
    
	showSevers(false);

	if(getVariable("mPostUpdateNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mPostUpdateNode"));
		if(sever) sever->setVisible(true);
	}
	if(getVariable("mPersentageNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mPersentageNode"));
		if(sever) sever->setVisible(false);
	}
	if(getVariable("mWaitingNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mWaitingNode"));
		if(sever) sever->setVisible(false);
	}
	
	if(getVariable("mLoginNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mLoginNode"));
		if(sever) sever->setVisible(true);
	}
    if(getVariable("mEnterButton"))
	{
		CCNode* eb = dynamic_cast<CCNode*>(getVariable("mEnterButton"));
		if(eb)
			eb->setVisible(true);
	}
	
#ifdef PROJECT91Quasi
    
#else


	if(!mLogined){
#ifdef WIN32
		//如果是win32，直接从userDefault中取出最近一次登录puid，省去输入inputBox操作 by zhenhui 2014/5/20
		std::string puidKey = "LastLoginPUID";
		std::string strPuid = cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey(puidKey.c_str(), "");	
		//如果xml中没有值，则输入puid，否则，直接直接赋值并发送onlogin消息
		if (strPuid.length()>0)
		{
			lib91::setLoginName(strPuid);
			libPlatformManager::getPlatform()->_boardcastLoginResult(true,"");
		}else{
			libPlatformManager::getPlatform()->login();
		}
#else
		libPlatformManager::getPlatform()->login();
#endif
	}
#endif
	//
}
void LoadingFrame::showEnter()
{
	//平台登录结果回来了，可选区 进入游戏
	CCLOG("LoadingFrame::showEnter");
	if(getVariable("mLoginNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mLoginNode"));
		if(sever) sever->setVisible(true);
		if (sever && sever->getParent())
		{
			sever->getParent()->setVisible(true);
		}
	}
	if(getVariable("mEnterButton"))
	{
		CCNode* eb = dynamic_cast<CCNode*>(getVariable("mEnterButton"));
		if(eb)
			eb->setVisible(true);
	}
	if(getVariable("mUpdateDescription"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
		if(sever)
		{
			sever->setString("",true);
		}
	}
	if(getVariable("mWaitingNode"))
	{
		//登录进度条挂在这个节点的
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mWaitingNode"));
		if(sever) sever->setVisible(false);
	}
	//
	updateLoginName();
}

void LoadingFrame::showPersent( float persentage,std::string sizeTip )
{
	if(getVariable("mPersentage"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mPersentage"));
		if(persentage>1.0f)
			persentage = 1.0f;
		if(persentage<0.06f)
			persentage = 0.06f;

		if(sever) 
			sever->setScaleX(persentage);
	}
	if(getVariable("mPersentageTxt"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mPersentageTxt"));
		char perTxt[64];
		sprintf(perTxt,"%d%%   %s",(int)(persentage*100),sizeTip.c_str());
		if(sever) 
			sever->setString(perTxt);
	}
}

void LoadingFrame::showEnterAndLoginingIn()
{
	if(getVariable("mPostUpdateNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mPostUpdateNode"));
		if(sever) sever->setVisible(false);
	}
	if(getVariable("mPersentageNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mPersentageNode"));
		if(sever) sever->setVisible(false);
	}
	if(getVariable("mWaitingNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mWaitingNode"));
		if(sever) sever->setVisible(true);
	}
	if(getVariable("mLoginNode"))
	{
		CCNode* sever = dynamic_cast<CCNode*>(getVariable("mLoginNode"));
		//if(sever) sever->setVisible(false);
		if (sever && sever->getParent())
		{
			sever->getParent()->setVisible(false);
		}
	}
	if(getVariable("mUpdateDescription"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
		if(sever)
		{
			sever->setString(Language::Get()->getString("@UpdateDescriptionupLoginingIn").c_str(),true);
		}
	}
}

int LoadingFrame::getDefaultSeverID()
{
	std::string projectNameKey="xwlgj";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	projectNameKey="xwlgj_android";
#else
	projectNameKey="xwlgj_ios";
#endif
	const std::string projectName=VaribleManager::Get()->getSetting(projectNameKey,"","xwlgj");
    const std::string uin = libPlatformManager::getPlatform()->loginUin();
	std::map<std::string,std::string> strMap;
	strMap.insert(std::pair<std::string,std::string>("gameid",projectName.c_str()));
	strMap.insert(std::pair<std::string,std::string>("puid", uin));
	libOS::getInstance()->analyticsLogEvent("getDefaultSeverID-refreshServerInfo", strMap ,true);
    bool getSvr = false;
	if (!VaribleManager::Get()->getSetting("OpenAnalytics").empty())
		getSvr = true;
//	com4loves::refreshServerInfo(projectName.c_str(),uin, getSvr);
    libOS::getInstance()->analyticsLogEndTimeEvent("getDefaultSeverID-refreshServerInfo");
    int id = 0;//com4loves::getServerInfoCount()>0?com4loves::getServerUserByIndex(0):-1;
	if (id <=0 )
	{
		std::string uinkey = "DefaultSeverID";// + uin;
		int itmp = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(uinkey.c_str(), -1);
		if (itmp == -1)
		{
			mIsFirstLoginNotServerIDInfo = true;
			libOS::getInstance()->analyticsLogEvent("getDefaultSeverID-mIsFirstLoginNotServerIDInfo");
			itmp = SeverConsts::Get()->getSeverDefaultID();
			return itmp;
		}
		else
		{
			return itmp;
		}
	}
	else
		return id;
}

void LoadingFrame::updateSeverName()
{
	if(getVariable("mSeverName1") && mSelectedSeverID!=-1)
	{
		CCLabelBMFont* eb = dynamic_cast<CCLabelBMFont*>(getVariable("mSeverName1"));
		SeverConsts::SEVERLIST::const_iterator it = SeverConsts::Get()->getSeverList().find(mSelectedSeverID);
		if(eb && it!=SeverConsts::Get()->getSeverList().end())
			eb->setString(it->second->name.c_str());
	}
}

void LoadingFrame::updateLoginName()
{
    std::string nickName = libPlatformManager::getPlatform()->nickName();
#ifdef    WIN32
	//Win32下，不限制nickName字符串长度
#else
	nickName=GameMaths::stringCutWidthLen(nickName,7);
#endif
	CCLog("libPlatformManager nickName:%s", nickName.c_str());
	if (getVariable("mLoginName") && nickName.empty())
	{
		CCLog("libPlatformManager setString("")");
		CCLabelBMFont* eb = dynamic_cast<CCLabelBMFont*>(getVariable("mLoginName"));
		eb->setString("");
		return;
		
	} 

	CCLOG("LoadingFrame::updateLoginName:%s ", nickName.c_str());
	CCNode* node = dynamic_cast<CCNode*>(getVariable("mStartTexNode"));
	if(getVariable("mLoginName") && !nickName.empty())
	{

		CCLabelTTF* eb = dynamic_cast<CCLabelTTF*>(getVariable("mLoginName"));
		CCMenuItemImage* enter = dynamic_cast<CCMenuItemImage*>(getVariable("mentergamebutton"));
		if ( node )
		{
			node->setVisible(true);
		}
		if(eb)
        {
            if (libPlatformManager::getPlatform()->getIsTryUser()) 
			{    
				//modify by dylan 搜狗平台游戏账号展现nickName 20131127
				bool isShowNickName=false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				if(libPlatformManager::getPlatform()->getClientChannel()=="Android_SouGou")
				{
					isShowNickName=true;
				}
#endif
				if(!isShowNickName) nickName=Language::Get()->getString("@tryUserLogin");
                eb->setString(nickName.c_str());
				if (enter)
				{
					if(node)
					{
						node->setVisible(false);
					}
					enter->setNormalImage(mTryUserLoginSprite);
				}
            }
			else 
			{
                eb->setString(nickName.c_str());
				if (enter)
				{
					enter->setNormalImage(mUserLoginSprite);
				}
            }
        }
		else
		{
			CCLabelBMFont* eb2 = dynamic_cast<CCLabelBMFont*>(getVariable("mLoginName"));
			if(eb2)
			{
				if (libPlatformManager::getPlatform()->getIsTryUser()) 
				{
					//modify by dylan 搜狗平台游戏账号展现nickName 20131127
					bool isShowNickName=false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
					if(libPlatformManager::getPlatform()->getClientChannel()=="Android_SouGou")
					{
						isShowNickName=true;
					}
#endif
					if(!isShowNickName) nickName=Language::Get()->getString("@tryUserLogin");
					eb2->setString(nickName.c_str());
					if (enter)
					{
						if(node)
						{
							node->setVisible(false);
						}
						enter->setNormalImage(mTryUserLoginSprite);
					}
				}
				else 
				{
					eb2->setString(nickName.c_str());
					if (enter)
					{
						enter->setNormalImage(mUserLoginSprite);
					}
				}
			}
		}
	}
}

void LoadingFrame::onReceivePacket( const int opcode, const ::google::protobuf::Message* packet )
{
	static int siNowSvrID = 0;
	if (siNowSvrID == 0)
	{
		siNowSvrID = mSelectedSeverID;
	}
	if(opcode==LOGIN_S)
	{
		const HPLoginRet * logret = dynamic_cast<const HPLoginRet*>(packet);
		CCLOG("**************User LoginPacket Received *************************");

		//status =0 means log in success 
		if(logret )
		{
			if (logret->has_playerid())
			{
				if (logret->playerid()>0)
				{
					if (siNowSvrID != mSelectedSeverID || 
						(ServerDateManager::Get()->mLoginInfo.m_iPlayerID > 0 && 
							ServerDateManager::Get()->mLoginInfo.m_iPlayerID != logret->playerid()))
					{
						siNowSvrID = mSelectedSeverID;
						//发生了换角色或者换区重新登录
						ServerDateManager::Get()->reset(true);
					}
					//
					if (ServerDateManager::Get()->mLoginInfo.m_iPlayerID > 0)
					{
						//大于0说明不是大退新登，是重新登，重新登就reset
						ServerDateManager::Get()->reset(false);
					}
					ServerDateManager::Get()->mLoginInfo.m_iPlayerID = logret->playerid();
				}
				else
				{
					CCAssert(false, "logret->playerid()>0");
					return;
				}
			}
			if (logret->has_roleitemid())
			{
				ServerDateManager::Get()->mLoginInfo.m_iRoleItemID= logret->roleitemid();
			}
			else
			{
				ServerDateManager::Get()->mLoginInfo.m_iRoleItemID= 0;//允许从MainFrame退回LoadingFrame就需要这样
			}
			if (logret->has_timestamp())
			{
				if (logret->timestamp()>0)
				{
					ServerDateManager::Get()->mLoginInfo.m_iTimeStamp= logret->timestamp();
					GamePrecedure::Get()->setServerTime(logret->timestamp());
				}
			}
			SeverConsts::SEVERLIST::const_iterator it = SeverConsts::Get()->getSeverList().find(mSelectedSeverID);
			//如果没有角色，代表需要进入选人页面
			if (ServerDateManager::Get()->mLoginInfo.m_iRoleItemID == 0)
			{
				GamePrecedure::Get()->setHasMainRole(false);
			}

			GamePrecedure::Get()->preEnterMainMenu();
			_updateLocalSeverId();
			MsgLoadingFrameEnterGame enterGameMsg;
			MessageManager::Get()->sendMessage(&enterGameMsg);
			PacketManager::Get()->removePacketHandler(this);
			GamePrecedure::Get()->setAlertServerUpdating(true);
		}
		else
		{
			showEnter();
		}
	}
	else if(opcode==ERROR_CODE)
	{
		const HPErrorCode * errMsgRet = dynamic_cast<const HPErrorCode*>(packet);
		if(errMsgRet->hpcode()==LOGIN_S&&mSendLogin)
		{
			PacketManager::Get()->removePacketHandler(this);
			std::string _errMsgLang="";
			if(!errMsgRet->has_errmsg())
			{
				char msg[256];
				sprintf(msg, "@ErrorReport_%d", errMsgRet->errcode());
				_errMsgLang=Language::Get()->getString(msg)+"["+StringConverter::toString(errMsgRet->hpcode())+"|"+StringConverter::toString(errMsgRet->errcode())+"]";
			}
			else
			{
				_errMsgLang=errMsgRet->errmsg();
			}
			libOS::getInstance()->showMessagebox(_errMsgLang,Err_ErrMsgReport);
		}
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) && defined(Macro_AndroidTestJiHuoMa)
	else if(opcode == OPCODE_ACTIVE_CODERET_S)
	{
		const OPActiveCodeRet * ret = dynamic_cast<const OPActiveCodeRet*>(packet);
		if(ret->status()==1)
		{
			mScene->removeChildByTag(999,true);
			std::string out = Language::Get()->getString("@ActiveCodeSuccess");
			CCMessageBox(out.c_str(),Language::Get()->getString("@LoadingFrameLogretStatsTitle").c_str());
		}
		else if(ret->status()==2)
		{
			std::string out = Language::Get()->getString("@ActiveCodeBeenUsed");
			CCMessageBox(out.c_str(),Language::Get()->getString("@LoadingFrameLogretStatsTitle").c_str());
		}
		else
		{
			std::string out = Language::Get()->getString("@WrongActiveCode");
			CCMessageBox(out.c_str(),Language::Get()->getString("@LoadingFrameLogretStatsTitle").c_str());
		}
	}
#endif
}

void LoadingFrame::onConnectFailed( std::string ip, int port )
{
	waitingManager::Get()->endWaiting();
	waitingManager::Get()->clearReasones();//手动触发超时，结束菊花
	showEnter();
	std::string out = Language::Get()->getString("@LoadingFrameSeversConnectionFailed");
	if(libOS::getInstance()->getNetWork()==NotReachable)
	{
		libOS::getInstance()->showMessagebox(Language::Get()->getString("@NoNetWork"),Err_ConnectFailed);
	}
	else
	{
		if(SeverConsts::Get()->isServerInUpdatingCode())
		{
			libOS::getInstance()->showMessagebox(SeverConsts::Get()->getServerInUpdateMsg(),Err_ConnectFailed);
		}
		else
		{
			libOS::getInstance()->showMessagebox(out,Err_ConnectFailed);
		}
	}
	PacketManager::Get()->removePacketHandler(this); //modify by dylan onMenuItemAction method register 
}

CCScene* LoadingFrame::getRootSceneNode()
{
	return mScene;
}

void LoadingFrame::onLoadingTimeOut()
{
    //BulletinBoardPage::getInstance()->close();
}

void LoadingFrame::onBtnAction()
{
    //BulletinBoardPage::getInstance()->close();
}

void LoadingFrame::showLoginingInPercent( int pct )
{
	showEnterAndLoginingIn();

	if(getVariable("mUpdateDescription"))
	{
		CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
		if(sever)
		{
			sever->setVisible(true);
			char sztmp[16] = {0};
			sprintf(sztmp,"%d%%",pct);
			std::string strpct = Language::Get()->getString("@UpdateDescriptionupLoginingIn");
			strpct.append(sztmp);
			sever->setString(strpct.c_str(),true);
		}
	}
}

void LoadingFrame::hideLoginingInPercent()
{
	CCLabelBMFont* sever = dynamic_cast<CCLabelBMFont*>(getVariable("mUpdateDescription"));
	if (sever)
	{
		sever->setVisible(false);
	}
}

void LoadingFrame::showLoadingAniPage()
{
	LoadingAniPage* page = dynamic_cast<LoadingAniPage*>(CCBManager::Get()->getPage("LoadingAniPage"));
	if (!page)
	{
		return;
	}
	CCNode* child = this->getChildByTag(LoadinAniPageTag);
	if (child)
	{
		return;
	}
	page->load();
	page->removeFromParent();
	cocos2d::CCLayer* layer = CCLayer::create();
	layer->setContentSize(CCDirector::sharedDirector()->getOpenGLView()->getVisibleSize());
	layer->setPosition(0,0);
	layer->setAnchorPoint(ccp(0,0));
	layer->setTouchEnabled(true);
	layer->setTouchMode(cocos2d::kCCTouchesOneByOne);
	page->addChild(layer, -1);
	page->setTag(LoadinAniPageTag);
	CCNode* node = dynamic_cast<CCNode*>(getVariable("mNodeFront"));
	if (node)
	{
		node->addChild(page);
	}
}

void LoadingFrame::hidLoadingAniPage()
{
	LoadingAniPage* page = dynamic_cast<LoadingAniPage*>(CCBManager::Get()->getPage("LoadingAniPage"));
	if (!page)
	{
		return;
	}

	page->stopLoadingAni();
	page->unload();
	page->removeFromParentAndCleanup(true);
}

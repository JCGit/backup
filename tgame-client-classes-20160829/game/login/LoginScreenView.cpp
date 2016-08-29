#include "LoginScreenView.h"
#include "QEnvSet.h"
#include "basic/QNetMgr.h"
#include "game/UserObj.h"
#include "LoginControl.h"
#include "LoginObj.h"
#include "ChooseSvrPanel.h"
#include "ChooseRolePanel.h"
#include "LoginProxy.h"
#include "LoadingLayer.h"
#include "TextHelper.h"

#include "PlayerInitCfgMaster.h"
#include "HeroCfgMaster.h"
#include "Resources.h"
#include "basic/ToolUtils.h"
#include "ChannelInf.h"
#include "evtlog/EvtLog.h"
#include "NounHelper.h"
#include "game/role/MainHero.h"
#include "basic/CommonDialog.h"
#include "ChannelInfoCallback.h"
#include "FaceBookSDK.h"

#ifdef TGAME_IN_APPLE_APP_STORE
#include "IOSiApGameInf.h"
#endif // TGAME_IN_APPLE_APP_STORE


bool LoginScreenView::m_bIsNeedAutoLoginSDK = true;

LoginScreenView::LoginScreenView()
:m_pBgImg(NULL)
,m_pSettingBtn(NULL)
,m_pNoticeBtn(NULL)
,m_pSvrNameValTxt(NULL)
,m_pSvrListBtn(NULL)
,m_pLoginBtn(NULL)
,m_pOpenIDInputTxt(NULL)
{
    m_pLoadingLayer = NULL;
}

LoginScreenView::~LoginScreenView()
{
    CC_SAFE_RELEASE_NULL(m_pLoadingLayer);
	//CToolUtils::removeArmatureFile("effect/ut00045/ut00045.ExportJson");
	CToolUtils::removeArmatureFile("effect/ut00057/ut00057.ExportJson");
	CToolUtils::removeArmatureFile("effect/ut00061/ut00061.ExportJson");
	CToolUtils::removePlistTexFile("ui_bg/bg_login.plist");
    releaseRoleRes();
}

LoginScreenView* LoginScreenView::create() {
	LoginScreenView *pRet = new LoginScreenView();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool LoginScreenView::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		bRet = true;
	} while(0);

	return bRet;
}

void LoginScreenView::onEnter() {
	BaseLayout::onEnter();

	if(!isUIInited()) {
		initUI();
	}

	m_pLoginBtn->setTouchEnabled(false); // 取得dir info后更新

#ifdef TGAME_IN_APP_SDK
    //进入后，自动调用sdk登陆平台
    CCLOG("[trace]LoginScreenView::onEnter, IsNeedAutoLoginSDK %d, isExitQQSDK %d", m_bIsNeedAutoLoginSDK, ChannelInfoCallback::isExitQQmsdk());
    if(m_bIsNeedAutoLoginSDK) {
        delayGoSdk();
        m_bIsNeedAutoLoginSDK = false;
    } 
#endif

	P_LOGIN_CTRL->getNoticePanel()->open();
}

void LoginScreenView::initUI() {
	CCDirector *pDirector = CCDirector::sharedDirector();
	CCPoint screenOrigin = pDirector->getVisibleOrigin();
	CCSize screenSize = pDirector->getVisibleSize();

	initLayoutFromJsonFile("ui_v2/LoginScreenUI.ExportJson");
	m_pRootLayout->setSize(screenSize);
	m_pRootLayout->setPosition(screenOrigin);
	addChild(m_pRootLayout);

    m_pGameLogoImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("game_logo_img"));
    CCLOG("[trace]LoginScreenView::initUI,channel name [%s]", ChannelInf::GetChannelName().c_str() );
    if("qqmsdk" == ChannelInf::GetChannelName() ) {
        //腾讯的用特兰战地
        m_pGameLogoImg->loadTexture("ui_bg/logo_terran.png");
    }


	CToolUtils::loadSpriteFrameRGBA4444("ui_bg/bg_login.plist");
	m_pBgImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_img"));
	/*CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00045/ut00045.ExportJson");
	CCArmature *pBgArmture = CCArmature::create("ut00045");
	m_pBgImg->addNode(pBgArmture);
	pBgArmture->getAnimation()->play("fly", -1, -1, 1);*/
	m_pBgImg->loadTexture("bg_login.png", UI_TEX_TYPE_PLIST);
	float scaleX = screenSize.width / m_pBgImg->getSize().width;
	float scaleY = screenSize.height / m_pBgImg->getSize().height;
	float scale = scaleX > scaleY ? scaleX : scaleY;
	m_pBgImg->setScale(scale);

	m_pSettingBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("setting_btn"));
	m_pSettingBtn->addTouchEventListener(this, toucheventselector(LoginScreenView::touchEventHandler));
	m_pSettingBtn->setEnabled(false);

	m_pNoticeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("notice_btn"));
	m_pNoticeBtn->addTouchEventListener(this, toucheventselector(LoginScreenView::touchEventHandler));

	UILayout *pSvrInfoLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("svr_info_layout"));
	m_pSvrNameValTxt = dynamic_cast<UILabel*>(pSvrInfoLayout->getChildByName("svr_name_val_txt"));
		
	m_pSvrListBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("svr_list_btn"));
	m_pSvrListBtn->addTouchEventListener(this, toucheventselector(LoginScreenView::touchEventHandler));

    m_pVerTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("ver_txt"));
    std::string strVersion = NounStr(TXT_NOW_VERSION) + string("1.4.0");
    m_pVerTxt->setText(strVersion);

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00061/ut00061.ExportJson");
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00057/ut00057.ExportJson");
	m_pLoginBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("login_btn"));
	m_pLoginBtn->addTouchEventListener(this, toucheventselector(LoginScreenView::touchEventHandler));

	CCArmature *pArm = CCArmature::create("ut00057");
	m_pLoginBtn->addNode(pArm);
	pArm->getAnimation()->play("fly", -1, -1, 1);

    m_pOpenIDInputTxt = dynamic_cast<UITextField*>(m_pRootLayout->getChildByName("open_id_input_txt"));

    if(QEnvSet::beck() || QEnvSet::neil() ) {
        m_pOpenIDInputTxt->setPlaceHolder(gGlobalOpenID);
    }

	m_pFaceBookLoginBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("facebook_login_btn"));
	m_pFaceBookLoginBtn->addTouchEventListener(this, toucheventselector(LoginScreenView::touchEventHandler));

	m_pGuestLoginBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("guest_login"));
	m_pGuestLoginBtn->addTouchEventListener(this, toucheventselector(LoginScreenView::touchEventHandler));

	m_pLoginBtn->setEnabled(true);
	m_pFaceBookLoginBtn->setEnabled(false);
	m_pGuestLoginBtn->setEnabled(false);

#ifdef TGAME_IN_APP_SDK
    //bg_open_id_img open_id_txt open_id_input_txt
    //隐藏
    UIImageView* pBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_open_id_img")); 
    pBg->setVisible(false);

    UILabel* pOpenidTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("open_id_txt"));
    
    pOpenidTipTxt->setVisible(false);
    m_pOpenIDInputTxt->setVisible(false);
	m_pOpenIDInputTxt->setTouchEnabled(false);

	m_pLoginBtn->setEnabled(false);
	m_pFaceBookLoginBtn->setEnabled(true);
	m_pGuestLoginBtn->setEnabled(true);

#endif // TGAME_IN_APP_SDK


	m_bUIInited = true;
}

void LoginScreenView::updateUI() {
	m_pSvrNameValTxt->setText(P_LOGIN_OBJ->getCurSvrInfo().name);
	m_pLoginBtn->setTouchEnabled(true);

	const DirSvrInfo &stDirSvrInfo = P_LOGIN_OBJ->getDirSvrInfo();

	if(stDirSvrInfo.isstop) {
		COMMON_DIALOG->show(stDirSvrInfo.notification);
	}
}

void LoginScreenView::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    
    switch(type) {
    	case TOUCH_EVENT_ENDED:
            if(std::strcmp(pszWidgetName, m_pSettingBtn->getName()) == 0) {
            	
			} else if(std::strcmp(pszWidgetName, m_pNoticeBtn->getName()) == 0) {
				P_LOGIN_CTRL->getNoticePanel()->open();
			} else if(std::strcmp(pszWidgetName, m_pSvrListBtn->getName()) == 0) {
            	P_LOGIN_CTRL->getChooseSvrPanel()->open();
            } else if(std::strcmp(pszWidgetName, m_pLoginBtn->getName()) == 0) {
                doLoginIn();
			} else if(std::strcmp(pszWidgetName, m_pFaceBookLoginBtn->getName()) == 0) {  //FaceBook登陆
				doLoginSDK(1);
			} else if(std::strcmp(pszWidgetName, m_pGuestLoginBtn->getName()) == 0) {		//游客登陆
				doLoginSDK(2);
			}
    		break;
    	default:;
    }
}

void LoginScreenView::scheduleLoginAfterConnected(float dt) {
    ADD_EVT_LOG(EVT_LOG_CHOOSE_SVR); //游戏事件日志
#ifdef TGAME_IN_APPLE_APP_STORE
    //apple app store
    P_LOGIN_CTRL->getProxy()->sendAppleGuestLoginRqst(P_USER_OBJ->getUserInfo()->loginInfo); //给后端验证
#else
    P_LOGIN_CTRL->getProxy()->sendLoginRqst(P_USER_OBJ->getUserInfo()->loginInfo); //给后端验证
#endif
    P_LOGIN_OBJ->updateRecentLoginSvrInfo();
}

void LoginScreenView::scheduleLoadRoleRes(float dt) {
    
    //2014-10-10,neilwu mod,从city切过来时有时序问题，所以角色资源在此加载.可以保证进scene不卡
    /*const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();
    for(int i = 0; i < stPlayerInitCfgSet.playerinitcfg_size(); i++) {
        const PlayerInitCfg &stPlayerInitCfg = stPlayerInitCfgSet.playerinitcfg(i);

        const HeroCfg *pHeroCfg = HERO_CFG_MASTER->GetHeroCfgByID(stPlayerInitCfg.showid());

        if(pHeroCfg != NULL) {
            std::string url = HERO_RES_PATH + pHeroCfg->resname() + "_denglu/" + pHeroCfg->resname() + "_denglu.ExportJson";
            CCLOG("[trace]load res %s", url.c_str());
            //CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(url.c_str());
            //neilwu mod,这里使用异步加载试试，减少动画卡顿，出错了再改回去
            CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync(url.c_str(), NULL, NULL);
        }
    }*/

	// 2015-1-12 modified by taddy, 二师兄说登录不用特定资源了
	// 加载枪械资源
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("guns/guns.plist");

	// 加载角色资源
	const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();

	for(int i = 0; i < stPlayerInitCfgSet.playerinitcfg_size(); i++) {
		const PlayerInitCfg &stPlayerInitCfg = stPlayerInitCfgSet.playerinitcfg(i);
		const HeroCfg *pHeroCfg = HERO_CFG_MASTER->GetHeroCfgByID(stPlayerInitCfg.showid());

		if(pHeroCfg != NULL) {
			// 获取资源路径
			unsigned int uShowID = pHeroCfg->showid();
			unsigned int uWeaponCfgID = 58; // 登录指定一把枪
			string bodyName;
			int iRet = CMainHero::GetResourceNames(uWeaponCfgID, uShowID, bodyName);

			if(iRet != 0) {
				CCLOG("Fail to get hero res! ShowID=%u WeaponCfgID=%u", uShowID, uWeaponCfgID);
				return;
			}

			string bodyPath = HERO_RES_PATH + bodyName + "/" + bodyName + JSON_TYPE_NAME;

			// 加载资源
			CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(bodyPath.c_str());
		}
	}
}

void LoginScreenView::releaseRoleRes() {
    /*
    const PlayerInitCfgSet &stPlayerInitCfgSet = PLAYER_INIT_CFG->GetCfg();
    for(int i = 0; i < stPlayerInitCfgSet.playerinitcfg_size(); i++) {
        const PlayerInitCfg &stPlayerInitCfg = stPlayerInitCfgSet.playerinitcfg(i);

        const HeroCfg *pHeroCfg = HERO_CFG_MASTER->GetHeroCfgByID(stPlayerInitCfg.showid());

        if(pHeroCfg != NULL) {
            std::string url = HERO_RES_PATH + pHeroCfg->resname() + "_denglu/" + pHeroCfg->resname() + "_denglu.ExportJson";
            CToolUtils::removeArmatureFile(url.c_str());
        }
    }
    */
}


void LoginScreenView::closeLoading() {
    if(m_pLoadingLayer) {
        m_pLoadingLayer->removeFromParentAndCleanup(true);
    }
}

void LoginScreenView::doLoginSDK(int iRetCode){

#ifdef TGAME_IN_APP_SDK
	CCLOG("[facebook]=====LoginScreenView::doLoginSDK, iRetCode === %d", iRetCode);

	FaceBookSDK::onLogin(iRetCode);
#endif

}

void LoginScreenView::delayGoSdk() {
    CCLOG("[trace]LoginScreenView::delayGoSdk");

	bool bIsSessionValid = FaceBookSDK::isSessionValid();
	CCLOG("[facebook]=====LoginScreenView::delayGoSdk, bIsSessionValid === %d", bIsSessionValid);

	//检测是否以前登陆过
	if(bIsSessionValid){  //已经登陆过
		if(FaceBookSDK::getUserType() == "0"){  //游客登陆过
			m_pFaceBookLoginBtn->setEnabled(false);
			m_pGuestLoginBtn->setEnabled(true);
		}else if(FaceBookSDK::getUserType() == "2"){  //FB登陆过
			m_pFaceBookLoginBtn->setEnabled(true);
			m_pGuestLoginBtn->setEnabled(false);
		}
	}else{
		m_pFaceBookLoginBtn->setEnabled(true);
		m_pGuestLoginBtn->setEnabled(true);
	}
}


void LoginScreenView::doLoginIn() {

#ifdef TGAME_IN_APP_SDK
    /*
    CCLOG("[trace]LoginScreenView::doLoginIn,go to sdk");
    P_LOGIN_CTRL->getChooseSvrPanel()->showLoading();
    //延时是为了显示转菊花
    this->scheduleOnce(schedule_selector(LoginScreenView::delayGoSdk), 0.3);
    */
    //2015-1-27,修改为自动登陆sdk
    ChannelInfoCallback::doGameLogin();

	//FaceBookSDK::

    return;
#endif

    const char* pStr = m_pOpenIDInputTxt->getStringValue();
    std::string openID(pStr);

	if(QEnvSet::taddy()) {
		//openID = "U193205707399967845520fb4e350cd2uc";
	}

	const DirSvrInfo &stDirSvrInfo = P_LOGIN_OBJ->getDirSvrInfo();

	if(stDirSvrInfo.isstop) {
		const string &whitelist = stDirSvrInfo.whitelist;
		string::size_type startPos = 0;
		startPos = whitelist.find(openID);

		if(startPos == string::npos
		|| !(startPos + openID.size() == whitelist.size() || whitelist.substr(startPos + openID.size(), 1) == ",")
		|| !(startPos == 0 || whitelist.substr(startPos - 1, 1) == ",")) {
			COMMON_DIALOG->show(stDirSvrInfo.notification);
			return;
		}
	}

    if(QEnvSet::beck() || QEnvSet::neil() ) {
        if(openID.size() <= 0) {
            openID = gGlobalOpenID; //测试时使用gGlobalOpenID，免得输入open id
        }
    }
    //openID = "U191506787399969614314bb6b55d654uc";
    if(openID.size() <= 0) {
        CCLOG("[trace]LoginScreenView::doLoginIn,openid is empty");        
        return;
    }

    onLoginInfoDone(openID);

	if(m_pLoginBtn->getNodeByTag(101)){
		m_pLoginBtn->removeNodeByTag(101);
	}

	CCArmature *pArm = CCArmature::create("ut00061");
	m_pLoginBtn->addNode(pArm, 0, 101);
	pArm->getAnimation()->play("fly", -1, -1, 0);
	pArm->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(LoginScreenView::handleBtnEffComplete));
    
}

void LoginScreenView::handleBtnEffComplete(CCArmature * armature, MovementEventType type, const char * name){
	if (type == COMPLETE){
		armature->removeFromParentAndCleanup(true);
	}
}

void LoginScreenView::onLoginInfoDone(const string& strLoginInfo) {
    P_USER_OBJ->getMutableUserInfo()->loginInfo = strLoginInfo;
    CCLOG("[trace]LoginPanel::onOpenIDDone,set LoginInfo [%s]", strLoginInfo.c_str());

	const SvrInfo& srvInfo = P_LOGIN_OBJ->getCurSvrInfo();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	// 确认是否开服
	eSvrStatus estat = (eSvrStatus)srvInfo.iFlag;
	if (estat == ESS_NOT_SERVICES)
	{
		CVTime vtime(srvInfo.szTime);
		char _szTime[256] = { 0, };
		snprintf(_szTime, 256, TextStr(TEXT_SVR_NOT_SERVICES),
			vtime.GetMonth(), vtime.GetDay(), vtime.GetHour(), vtime.GetMinute());
		COMMON_DIALOG->show(_szTime);
		return;
	}
	else if (estat == ESS_STOP_SERVICES)
	{
		COMMON_DIALOG->show(TextStr(TEXT_SVR_STOP_SERVICES));
		return;
	}
#endif

	P_NET_MGR->InitNetConnection(srvInfo.ip, 9000);

    if(m_pLoadingLayer) {
        m_pLoadingLayer->removeFromParentAndCleanup(true);
    } else {
        m_pLoadingLayer = CLoadingLayer::create();
        m_pLoadingLayer->retain();
    }
    m_pLoadingLayer->showLoading();
    this->scheduleOnce(schedule_selector(LoginScreenView::scheduleLoginAfterConnected), 1);
    this->scheduleOnce(schedule_selector(LoginScreenView::scheduleLoadRoleRes), 0.2f);
}


void LoginScreenView::doAppleQQAuthLogin() {

#ifdef TGAME_IN_APPLE_APP_STORE
    CCLOG("[trace]LoginPanel::doAppleQQAuthLogin");
    IOS_IAP_GAME_INF->tencentOAuthInf->doTencentOAuth();
#endif // TGAME_IN_APPLE_APP_STORE

}

void LoginScreenView::doAppleGuestLogin() {

#ifdef TGAME_IN_APPLE_APP_STORE
    CCLOG("[trace]LoginPanel::doAppleGuestLogin");
    //获取ios设备identifierForVendor
    //identifierForVendor对供应商来说是唯一的一个值，
    //也就是说，由同一个公司发行的的app在相同的设备上运行的时候都会有这个相同的标识符。
    //然而，如果用户删除了这个供应商的app然后再重新安装的话，这个标识符就会不一致。
    const std::string strIOSID = IOS_IAP_GAME_INF->tencentOAuthInf->getIdentifierForVendor();
    if(strIOSID.size() <= 0) {
        return;
    }
    char buff[300] = {0};
    snprintf(buff, 300, "{\"channel\":\"appstore\",\"token\":\"%s\"}", strIOSID.c_str() );
    onLoginInfoDone(std::string(buff));
#endif // TGAME_IN_APPLE_APP_STORE

}



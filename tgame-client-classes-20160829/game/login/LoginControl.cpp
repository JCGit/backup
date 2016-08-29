#include "LoginControl.h"

#include "CfgMaster/PlayerInitCfgMaster.h"
#include "CfgMaster/HeroCfgMaster.h"

#include "basic/ToolUtils.h"
#include "weaponlib/WeaponLibControl.h"
#include "game/bag/BagControl.h"
#include "game/manual/ManualControl.h"
#include "game/mail/MailControl.h"
#include "game/UserObj.h"
#include "game/friend/FriendControl.h"
#include "dailyact/DailyActControl.h"
#include "LoginScene.h"
#include "GameInf.h"
#include "game/lottery/LotteryControl.h"
#include "game/act/ActControl.h"

LoginControl::LoginControl()
:m_pLoginProxy(NULL)
,m_pLoginScreenView(NULL)
,m_pChooseSvrPanel(NULL)
,m_pChooseRolePanel(NULL)
,m_pCreateRolePanel(NULL)
,m_pTencentLoginPanel(NULL)
,m_pNoticePanel(NULL)
,m_pGameExitPanel(NULL)
{
}

LoginControl::~LoginControl()
{
    CC_SAFE_DELETE(m_pLoginProxy);
    CC_SAFE_RELEASE_NULL(m_pLoginScreenView);
    CC_SAFE_RELEASE_NULL(m_pChooseSvrPanel);
    CC_SAFE_RELEASE_NULL(m_pChooseRolePanel);
    CC_SAFE_RELEASE_NULL(m_pCreateRolePanel);
	CC_SAFE_RELEASE_NULL(m_pTencentLoginPanel);
	CC_SAFE_RELEASE_NULL(m_pNoticePanel);
	CC_SAFE_RELEASE_NULL(m_pGameExitPanel);
}

LoginProxy* LoginControl::getProxy() {
	if(m_pLoginProxy == NULL) {
		m_pLoginProxy = new LoginProxy();
	}

	return m_pLoginProxy;
}

LoginScreenView* LoginControl::getLoginScreenView() {
    if(m_pLoginScreenView == NULL) {
        m_pLoginScreenView = LoginScreenView::create();
        m_pLoginScreenView->retain();
    }
	return m_pLoginScreenView;
}

ChooseSvrPanel* LoginControl::getChooseSvrPanel() {
    if(m_pChooseSvrPanel == NULL) {
        m_pChooseSvrPanel = ChooseSvrPanel::create();
        m_pChooseSvrPanel->retain();
    }
	return m_pChooseSvrPanel;
}

ChooseRolePanel* LoginControl::getChooseRolePanel() {
    if(m_pChooseRolePanel == NULL) {
        m_pChooseRolePanel = ChooseRolePanel::create();
        m_pChooseRolePanel->retain();
    }

	return m_pChooseRolePanel;
}

CreateRolePanel* LoginControl::getCreateRolePanel() {
    if(m_pCreateRolePanel == NULL) {
        m_pCreateRolePanel = CreateRolePanel::create();
        m_pCreateRolePanel->retain();
    }

    return m_pCreateRolePanel;
}

TencentLoginPanel* LoginControl::getTencentLoginPanel() {
	if(m_pTencentLoginPanel == NULL) {
		m_pTencentLoginPanel = TencentLoginPanel::create();
		m_pTencentLoginPanel->retain();
	}

	return m_pTencentLoginPanel;
}

NoticePanel* LoginControl::getNoticePanel() {
	if(m_pNoticePanel == NULL) {
		m_pNoticePanel = NoticePanel::create();
		CC_SAFE_RETAIN(m_pNoticePanel);
	}

	return m_pNoticePanel;
}

GameExitPanel* LoginControl::getGameExitPanel(){
	if(m_pGameExitPanel == NULL) {
		m_pGameExitPanel = GameExitPanel::create();
		CC_SAFE_RETAIN(m_pGameExitPanel);
	}

	return m_pGameExitPanel;
}

void LoginControl::finalize() {
    //LoginScene::onExit()
    CC_SAFE_RELEASE_NULL(m_pChooseSvrPanel);
    CC_SAFE_RELEASE_NULL(m_pChooseRolePanel);
    CC_SAFE_RELEASE_NULL(m_pCreateRolePanel);
	CC_SAFE_RELEASE_NULL(m_pTencentLoginPanel);
    CC_SAFE_RELEASE_NULL(m_pLoginScreenView); //最后释放


    //neil mod,登陆成功后把这几个给释放了吧
    CToolUtils::removePlistTexFile("ui_v2/start.plist");
    CToolUtils::removeArmatureFile("effect/ut00026/ut00026.ExportJson");;
	CToolUtils::removeArmatureFile("effect/denglujiemian/denglujiemian.ExportJson");
	CToolUtils::removeArmatureFile("effect/denglujiemianqianjin/denglujiemianqianjin.ExportJson");
}

void LoginControl::onLoginGetData() {
    CCLOG("[trace]LoginControl::onLoginGetData,prepare some data.");
    
	getProxy()->sendQueryPlayerInfoRqst(P_USER_OBJ->getUserInfo()->uin);
	getProxy()->sendQueryGoldInfoRqst();
	getProxy()->sendQueryPlayerDetailInfoRqst();
    //getProxy()->sendUmengDeviceTokenRqst();

    P_BAG_CTRL->sendQueryBagInfoRqst();
	P_MAIL_CTRL->getProxy()->sendQueryMail();
    P_FRIEND_CTRL->getProxy()->sendQueryFriendRqst(); //注册函数监听
    P_DAILYACT_CTRL->getProxy()->sendQueryDailyAct(); //初始化监听
    P_WEAPON_CTRL->sendWeaponBagRqst(); //拉一下枪械背包
	P_ACT_CTRL-> sendQueryAllPkgStatusRqst();//初始化活动监听
    //P_WEAPON_LOT_CTRL->onLoginCheckTime();
	P_LOTTERY_CTRL->onLoginCheckTime();
    
	//将枪的图片加载到cache中
	//CToolUtils::loadSpriteFrameRGBA4444("guns/guns.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("guns/guns.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/other.plist");
}

void LoginControl::sendSetSettingParamRqst(std::string str){
	getProxy()->sendSetSettingParamRqst(str);
}



void LoginControl::forceCloseLoginPanels() {
    CCLOG("[trace]LoginControl::forceCloseLoginPanels");
    if(m_pChooseSvrPanel != NULL) {
        m_pChooseSvrPanel->close();
    }
    if(m_pChooseRolePanel != NULL) {
        m_pChooseRolePanel->close();
    }
    if(m_pCreateRolePanel != NULL) {
        m_pCreateRolePanel->close();
    }

	if(m_pTencentLoginPanel != NULL) {
		m_pTencentLoginPanel->close();
	}
}

void LoginControl::doAfterSDKInited() {
    CCLOG("[trace]LoginControl::doAfterSDKInited");
    if(m_pLoginScreenView){
        getProxy()->sendGetSvrListHttpRqst(P_GAME_INF->getDirURL());
    } else {
        CCLOG("[trace]LoginControl::doAfterSDKInited, login screen view not ready,do nothing");
    }
}

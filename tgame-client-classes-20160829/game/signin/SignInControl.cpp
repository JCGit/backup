#include "SignInControl.h"
#include "SignInPanel.h"
#include "UserObj.h"
#include "city/CityControl.h"
#include "ModuleID.pb.h"
#include "game/notification/NotificationControl.h"

SignInControl::SignInControl()
{
    m_pSignInPanel = NULL;
    m_bNeedAutoOpen = true;
}

SignInControl::~SignInControl()
{
	finalizePanel();
}

SignInPanel* SignInControl::getSignInPanel() {
	if(m_pSignInPanel == NULL) {
		m_pSignInPanel = SignInPanel::create();
		m_pSignInPanel->retain(); // release at SignInControl::~SignInControl()
	}

	return m_pSignInPanel;
}





void SignInControl::finalizePanel() {
    CC_SAFE_RELEASE_NULL(m_pSignInPanel);
}



void SignInControl::updateSignInRsp(bool bIsRsp) {
    if(m_pSignInPanel && m_pSignInPanel->isOpen() ) {
        m_pSignInPanel->updateUI();
    }
	notifySignIn();

    if(bIsRsp) {
        //显示奖励信息
        m_pSignInPanel->popAwardTip();
    }
}

void SignInControl::closeSigninPanel() {
    if(m_pSignInPanel && m_pSignInPanel->isOpen() ) {
        m_pSignInPanel->close();
    }
}

void SignInControl::checkNeedAutoOpen() {
    if(!m_bNeedAutoOpen) {
        return;
    }
    //check module open?
    if(!P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_SIGNIN)) {
        return;
    }

    //check can reg
    if(P_USER_OBJ->getUserInfo()->stDailyRegInfo.canreg() <= 0) {
        return;
    }
    m_bNeedAutoOpen = false; //只提醒一次

    getSignInPanel()->open();

}

void SignInControl::notifySignIn()
{
	if (isCanSignIn())
	{
		PUSH_NOTIFICATION(ENTRY_SIGN_IN,NOTIFY_TAG_SIGNIN,1);
	}else
	{
		PUSH_NOTIFICATION(ENTRY_SIGN_IN,NOTIFY_TAG_SIGNIN,0);
	}
}
bool SignInControl::isCanSignIn()
{
	return P_USER_OBJ->getUserInfo()->stDailyRegInfo.canreg();
}

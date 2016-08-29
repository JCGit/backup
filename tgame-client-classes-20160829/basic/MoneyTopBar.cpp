#include "MoneyTopBar.h"
#include "UserObj.h"
#include "defines.h"
#include "Constant.h"
#include "vip/VIPControl.h"
#include "vip/BuyCoinPanel.h"
#include "GlobalCfgMaster.h"
#include "manual/ManualControl.h"
#include "tip/TipControl.h"
#include "TextHelper.h"


MoneyTopBar::MoneyTopBar(){
    m_pRootLayout = NULL;
}

MoneyTopBar::~MoneyTopBar(){
    CC_SAFE_RELEASE_NULL(m_pRootLayout);
}



bool MoneyTopBar::init() {
    bool iRet = false;
    do {
        CC_BREAK_IF(!BaseLayout::init());
        initLayoutFromJsonFile("ui_v2/money_top_bar.ExportJson");
        addChild(m_pRootLayout);

        m_pBtnCoin = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_coin"));
        m_pBtnGold = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_gold"));
        m_pBtnMannual = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("btn_mannual"));

        m_pTxtCoin = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("txt_coin"));
        m_pTxtGold = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("txt_gold"));
        m_pTxtMannual = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("txt_mannual"));

        m_pBtnCoin->addTouchEventListener(this, toucheventselector(MoneyTopBar::touchEventHandler));
        m_pBtnGold->addTouchEventListener(this, toucheventselector(MoneyTopBar::touchEventHandler));
        m_pBtnMannual->addTouchEventListener(this, toucheventselector(MoneyTopBar::touchEventHandler));

        iRet = true;
    } while (0);

    return iRet;
    
}

void MoneyTopBar::setVisible(bool visible) {
    if(visible) {
        //update data
        updateUI();
    }
}
void MoneyTopBar::updateUI() {
    const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();
    m_pTxtCoin->setText(intToString(pUserInfo->coin) );
    m_pTxtGold->setText(intToString(pUserInfo->gold) );

    const unsigned int uRemainManual = pUserInfo->remainManual;
    m_pTxtMannual->setText(intToString(uRemainManual) + "/" + intToString(MAX_MANUAL_VAL));
}

void MoneyTopBar::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, "btn_coin") == 0) {
            P_VIP_CTRL->getBuyCoinPanel()->open();
		} else if(std::strcmp(pszWidgetName, "btn_gold") == 0) {
            //P_SIGN_IN_CTRL->getSignInPanel()->open(); //just for test
        } else if(std::strcmp(pszWidgetName, "btn_mannual") == 0) {
			P_TIP_CTRL->LowManualBuyTip(this, dialogbtneventselector(MoneyTopBar::buyManual));
        }
        //CCLOG("%s", pszWidgetName);
		break;
	default:
		break;
	}
}
void MoneyTopBar::buyManual(){
    P_MANUAL_CTRL->sendBuyManualRqst();
}


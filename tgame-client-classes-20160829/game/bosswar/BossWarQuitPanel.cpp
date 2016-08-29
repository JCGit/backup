#include "BossWarQuitPanel.h"
#include "BossWarControl.h"

CBossWarQuitPanel::CBossWarQuitPanel(){

}

CBossWarQuitPanel::~CBossWarQuitPanel(){

}

bool CBossWarQuitPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/boss_war_quit_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		setUseBackForClose(false);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CBossWarQuitPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pSureBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("sure_btn"));
	m_pCancelBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("cancel_btn"));

	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CBossWarQuitPanel::touchEventHandler));
	m_pSureBtn->addTouchEventListener(this, toucheventselector(CBossWarQuitPanel::touchEventHandler));
	m_pCancelBtn->addTouchEventListener(this, toucheventselector(CBossWarQuitPanel::touchEventHandler));
}

void CBossWarQuitPanel::updateUI(){

}

void CBossWarQuitPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();
		}else if(strcmp(pszWidgetName, "cancel_btn") == 0) {
			this->close();
		}else if(strcmp(pszWidgetName, "sure_btn") == 0) {
			this->showLoading();
			P_BOSS_WAR_CTRL->sendAttendOrLeaveRqst(0);
		}
		break;
	default:

		break;
	}
}

void CBossWarQuitPanel::open(){
	BasePanel::open();
}

void CBossWarQuitPanel::close(){
	BasePanel::close();
}


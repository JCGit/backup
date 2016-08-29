#include "DailyActEntryPanel.h"


DailyActEntryPanel::DailyActEntryPanel()
:m_pActNameTxt(NULL)
,m_pRemainTimesValTxt(NULL)
,m_pCoinValTxt(NULL)
,m_pExpValTxt(NULL)
,m_pPropValTxt(NULL)
,m_pExitBtn(NULL)
,m_pGotoBtn(NULL)
{
}

DailyActEntryPanel::~DailyActEntryPanel()
{
}

DailyActEntryPanel* DailyActEntryPanel::create() {
	DailyActEntryPanel *pRet = new DailyActEntryPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool DailyActEntryPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui/DailyActEntryPanelUI.json");
		addChild(m_pRootLayout);

		m_pActNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_name_txt"));

		m_pRemainTimesValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("remain_times_val_txt"));

		m_pCoinValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_val_txt"));

		m_pExpValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("exp_val_txt"));

		m_pPropValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("prop_val_txt"));

		m_pExitBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("exit_btn"));
		m_pExitBtn->addTouchEventListener(this, toucheventselector(DailyActEntryPanel::touchEventHandler));

		m_pGotoBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("goto_btn"));
		m_pGotoBtn->addTouchEventListener(this, toucheventselector(DailyActEntryPanel::touchEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void DailyActEntryPanel::updateUI() {

}

void DailyActEntryPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, "exit_btn") == 0) {
			close();
		} else if(std::strcmp(pszWidgetName, "goto_btn") == 0) {

		}
		break;
	default:
		break;
	}
}

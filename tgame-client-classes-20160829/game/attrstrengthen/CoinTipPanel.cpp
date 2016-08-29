#include "CoinTipPanel.h"

CoinTipPanel::CoinTipPanel()
:m_pCloseBtn(NULL)
,m_pGotoBtn(NULL)
{
}

CoinTipPanel::~CoinTipPanel()
{
}

CoinTipPanel* CoinTipPanel::create() {
	CoinTipPanel *pRet = new CoinTipPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CoinTipPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui/CoinTipPanelUI.json");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CoinTipPanel::touchEventHandler));

		m_pGotoBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("goto_btn"));
		m_pGotoBtn->addTouchEventListener(this, toucheventselector(CoinTipPanel::touchEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void CoinTipPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			} else if(std::strcmp(pszWidgetName, m_pGotoBtn->getName()) == 0) {
				close();

			}
			break;
		default:
			break;
	}
}

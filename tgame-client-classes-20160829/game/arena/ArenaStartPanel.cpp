#include "ArenaStartPanel.h"

const string CArenaStartPanel::START_PVP = "start_pvp";				//¿ªÊ¼pvp

CArenaStartPanel::CArenaStartPanel(){

}

CArenaStartPanel::~CArenaStartPanel(){

}

bool CArenaStartPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/pvp_enter_panel_ui.ExportJson");
		addChild(m_pRootLayout);
		setOpenWithBgImg(false);
		setUseBackForClose(false);

		m_pTipImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("tip_img"));
		m_pRootLayout->setTouchEnabled(false);

		addTouchEventListener(this, toucheventselector(CArenaStartPanel::touchEventHandler));

		float dur = 0.6f;
		m_pTipImg->setOpacity(0x00);
		CCActionInterval* pFadein = CCFadeIn::create(dur);
		CCActionInterval* pFadeout = CCFadeOut::create(dur);
		m_pTipImg->runAction(CCRepeatForever::create(CCSequence::create(pFadein, pFadeout, NULL)));

		bRet = true;
	} while (0);

	return bRet;
}

void CArenaStartPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, getName()) == 0) {
			DispatchEvent(START_PVP, this);	
			this->close();
		}
		break;
	default:

		break;
	}
}


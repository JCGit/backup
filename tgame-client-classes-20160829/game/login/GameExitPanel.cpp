#include "GameExitPanel.h"
#include "GameInf.h"
#include "CommonDialog.h"

GameExitPanel::GameExitPanel(){

}

GameExitPanel::~GameExitPanel(){

}

bool GameExitPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(false);

		initLayoutFromJsonFile("ui_v2/common_dialog_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pYesBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("yes_btn"));
		m_pYesBtn->addTouchEventListener(this, toucheventselector(GameExitPanel::touchEventHandler));

		m_pNoBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("no_btn"));
		m_pNoBtn->addTouchEventListener(this, toucheventselector(GameExitPanel::touchEventHandler));

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(GameExitPanel::touchEventHandler));

		m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));
		m_pDescTxt->setText("");

		m_pNoBtn->setEnabled(false);

		bRet = true;
	} while(0);

	return bRet;
}

void GameExitPanel::show(string strDesc){
	if(!this->isOpen()){
		this->open();
	}

	m_pYesBtn->setPosition(ccp(m_pDescTxt->getPositionX(), m_pYesBtn->getPositionY()));
	m_pDescTxt->setText(strDesc);
}

void GameExitPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	if (TOUCH_EVENT_ENDED == type) {
		UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
		const char *pszWidgetName = pWidget->getName();
		if(std::strcmp(pszWidgetName, "yes_btn") == 0) {
			close(); 
		}else if(std::strcmp(pszWidgetName, "close_btn") == 0) {
			close();
		}
	}

}

void GameExitPanel::open(){
	BasePanel::open();
}

void GameExitPanel::close(){
	BasePanel::close();

	COMMON_DIALOG->closeLoading();
	P_GAME_INF->onClickClose();
}


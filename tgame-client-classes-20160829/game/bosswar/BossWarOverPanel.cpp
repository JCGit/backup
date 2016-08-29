#include "BossWarOverPanel.h"
#include "BossWarControl.h"

CBossWarOverPanel::CBossWarOverPanel(){

}

CBossWarOverPanel::~CBossWarOverPanel(){

}

bool CBossWarOverPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/boss_war_over_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(false);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CBossWarOverPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pQuitBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("quit_btn"));

	m_pWinImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("win_img"));
	m_pNameTxt = dynamic_cast<Label*>(m_pWinImg->getChildByName("name_txt"));

	m_pFailImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("fail_img"));
	m_pFailImg->setVisible(false);

	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CBossWarOverPanel::touchEventHandler));
	m_pQuitBtn->addTouchEventListener(this, toucheventselector(CBossWarOverPanel::touchEventHandler));
}

void CBossWarOverPanel::updateUI(){
	const CSBossChallengeStateNotification& stInfo = P_BOSS_WAR_CTRL->getStateNotify();
	if(stInfo.state() != BOSS_CHALLENGE_OVER){  //Î´½áÊø
		this->close();
		return;
	}

	bool bIsWin = stInfo.has_additionalmsg1();

	m_pWinImg->setEnabled(bIsWin);
	m_pWinImg->setVisible(bIsWin);
	m_pFailImg->setEnabled(!bIsWin);
	m_pFailImg->setVisible(!bIsWin);

	if(bIsWin){
		m_pNameTxt->setText(stInfo.additionalmsg1());
	}
}

void CBossWarOverPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			doOver();
		}else if(strcmp(pszWidgetName, "quit_btn") == 0) {
			doOver();
		}
		break;
	default:

		break;
	}
}

void CBossWarOverPanel::doOver(){
	this->showLoading();
	P_BOSS_WAR_CTRL->doAttendOrLeave(0);
}

void CBossWarOverPanel::open(){
	BasePanel::open();
}

void CBossWarOverPanel::close(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	BasePanel::close();
}


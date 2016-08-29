#include "CDKPanel.h"
#include "ActControl.h"

CDKPanel::CDKPanel(){

}

CDKPanel::~CDKPanel(){

}

bool CDKPanel::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/cdk_exchange_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CDKPanel::touchEventHandler));

		m_pSureBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("sure_btn"));
		m_pSureBtn->addTouchEventListener(this, toucheventselector(CDKPanel::touchEventHandler));

		m_pCancelBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("cancel_btn"));
		m_pCancelBtn->addTouchEventListener(this, toucheventselector(CDKPanel::touchEventHandler));

		m_pInputTxt = dynamic_cast<TextField*>(m_pRootLayout->getChildByName("input_txt"));
		m_pTipTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("tip_txt"));
		m_pTipTxt->setVisible(false);

		bRet = true;
	} while(0);

	return bRet;
}

void CDKPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
			close();
		} else if(std::strcmp(pszWidgetName, m_pSureBtn->getName()) == 0) {
			sureFun();
		} else if(std::strcmp(pszWidgetName, m_pCancelBtn->getName()) == 0) {
			close();
		} 
		break;
	default:;
	}
}

void CDKPanel::sureFun(){
	m_pTipTxt->setVisible(false);
	string inputStr = m_pInputTxt->getStringValue();
	if(inputStr == ""){
		if(m_pTipTxt->numberOfRunningActions()){
			m_pTipTxt->stopAllActions();
		}
		m_pTipTxt->setVisible(true);
		CCBlink* pBlink = CCBlink::create(3.0f, 3);
		CCFiniteTimeAction* pFun = CCCallFunc::create(this, callfunc_selector(CDKPanel::hideTip));
		m_pTipTxt->runAction(CCSequence::create(pBlink, pFun, NULL));
		return;
	}

	string sendStr = trimSpace(inputStr);
	P_ACT_CTRL->sendClaimCouponRqst(sendStr);

	m_pInputTxt->setText("");

	close();
}

void CDKPanel::hideTip(){
	m_pTipTxt->setVisible(false);
}

string CDKPanel::trimSpace(string& str){
	string trimStr = "";
	int len = str.length();

	for(int i = 0; i < len; i++){
		if(str[i] != ' '){
			trimStr += str[i];
		}
	}

	return trimStr;
}

void CDKPanel::close(){
	BasePanel::close();

	m_pTipTxt->setVisible(false);
}


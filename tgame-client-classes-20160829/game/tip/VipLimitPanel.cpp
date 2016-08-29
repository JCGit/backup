#include "VipLimitPanel.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"

CVipLimitPanel::CVipLimitPanel(){

}

CVipLimitPanel::~CVipLimitPanel(){

}

bool CVipLimitPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/vip_limit_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CVipLimitPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CVipLimitPanel::touchEventHandler));

	m_pChargeBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("charge_btn"));
	m_pChargeBtn->addTouchEventListener(this, toucheventselector(CVipLimitPanel::touchEventHandler));

	m_pCancelBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("cancel_btn"));
	m_pCancelBtn->addTouchEventListener(this, toucheventselector(CVipLimitPanel::touchEventHandler));

	m_pDescTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("desc_txt"));
}

void CVipLimitPanel::updateUI(string descStr){
	m_pDescTxt->setText(descStr);
}

void CVipLimitPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();
		}else if(strcmp(pszWidgetName, "charge_btn") == 0) {
			P_SHOP_CTRL->getRmbShopPanel()->open();
			this->close();
		}else if(strcmp(pszWidgetName, "cancel_btn") == 0) {
			this->close();
		}
		break;
	default:

		break;
	}
}

void CVipLimitPanel::open(){
	BasePanel::open();
}

void CVipLimitPanel::close(){
	BasePanel::close();
}


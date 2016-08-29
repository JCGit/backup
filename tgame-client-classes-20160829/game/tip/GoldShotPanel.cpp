#include "GoldShotPanel.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"
#include "game/vip/VIPControl.h"
#include "game/warmap/WarMapControl.h"

CGoldShotPanel::CGoldShotPanel(){
	
}

CGoldShotPanel::~CGoldShotPanel(){

}

bool CGoldShotPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/gold_shot_panel_ui.ExportJson");
		addChild(m_pRootLayout);


		m_pSureBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("sure_btn"));
		m_pCancelBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("cancel_btn"));
		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));

		m_pSureBtn->addTouchEventListener(this, toucheventselector(CGoldShotPanel::touchEventHandler));
		m_pCancelBtn->addTouchEventListener(this, toucheventselector(CGoldShotPanel::touchEventHandler));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CGoldShotPanel::touchEventHandler));

		bRet = true;
	} while(0);

	return bRet;
}

void CGoldShotPanel::close() {
	BasePanel::close();

	P_WAR_MAP_CTRL->setEnterWarManualNotEnough(false);
}

void CGoldShotPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, "sure_btn") == 0) {
			bool isEnterWarManualNotEnough = P_WAR_MAP_CTRL->isEnterWarManualNotEnough();
			this->close(); //先关闭，然后回掉

			if(isEnterWarManualNotEnough) {
				P_VIP_CTRL->getVIPPanel()->open();
			} else {
				P_SHOP_CTRL->getRmbShopPanel()->open();
			}
		}else if(std::strcmp(pszWidgetName, "cancel_btn") == 0) {
			this->close();
		}else if(std::strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();
		}
		break;
	default:
		break;
	}
}



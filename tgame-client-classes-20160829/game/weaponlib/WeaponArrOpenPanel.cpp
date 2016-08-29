#include "WeaponArrOpenPanel.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"
#include "WeaponLibControl.h"
#include "WeaponArrayOpenCfgMaster.h"

CWeaponArrOpenPanel::CWeaponArrOpenPanel(){
	m_uArrIdx = 0;
}

CWeaponArrOpenPanel::~CWeaponArrOpenPanel(){

}

bool CWeaponArrOpenPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/weapon_arr_open_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CWeaponArrOpenPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CWeaponArrOpenPanel::touchEventHandler));

	m_pChargeBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("charge_btn"));
	m_pChargeBtn->addTouchEventListener(this, toucheventselector(CWeaponArrOpenPanel::touchEventHandler));

	m_pCancelBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("cancel_btn"));
	m_pCancelBtn->addTouchEventListener(this, toucheventselector(CWeaponArrOpenPanel::touchEventHandler));

	m_pDescTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("desc_txt"));
}

void CWeaponArrOpenPanel::setArrIdx(int iIdx){
	m_uArrIdx = iIdx;

	updateUI();
}

void CWeaponArrOpenPanel::updateUI(){
	const WeaponArrayOpenCfg* pCfg = WEAPON_ARR_OPEN_CFG_MASTER->GetWeaponArrayOpenCfgByID(m_uArrIdx);

	if(pCfg){
		m_pDescTxt->setText(pCfg->desc());
	}
}

void CWeaponArrOpenPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
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
			if(P_WEAPON_CTRL->getArrayPanel()->isOpen()){
				P_WEAPON_CTRL->getArrayPanel()->close();
			}
		}else if(strcmp(pszWidgetName, "cancel_btn") == 0) {
			this->close();
		}
		break;
	default:

		break;
	}
}

void CWeaponArrOpenPanel::open(){
	BasePanel::open();
}

void CWeaponArrOpenPanel::close(){
	BasePanel::close();
}


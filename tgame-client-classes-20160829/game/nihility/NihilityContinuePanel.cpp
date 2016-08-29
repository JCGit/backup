#include "NihilityContinuePanel.h"
#include "NihilityControl.h"
#include "UserObj.h"
#include "CommonDialog.h"
#include "bag/BagControl.h"
#include "firearms/FireArmsObj.h"
#include "tip/TipControl.h"
#include "fight/FightObj.h"
#include "fight/FightControl.h"
#include "NounHelper.h"
#include "manual/ManualControl.h"

#include "GlobalCfgMaster.h"

CNihilityContinuePanel::CNihilityContinuePanel(){

}

CNihilityContinuePanel::~CNihilityContinuePanel(){

}

bool CNihilityContinuePanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/nihility_continue_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CNihilityContinuePanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pSureBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("sure_btn"));
	m_pCancelBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("cancel_btn"));

	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CNihilityContinuePanel::touchEventHandler));
	m_pSureBtn->addTouchEventListener(this, toucheventselector(CNihilityContinuePanel::touchEventHandler));
	m_pCancelBtn->addTouchEventListener(this, toucheventselector(CNihilityContinuePanel::touchEventHandler));
}

void CNihilityContinuePanel::updateUI(){

}

void CNihilityContinuePanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();
			P_NIHILITY_CTRL->getGenPanel()->open();
			P_NIHILITY_CTRL->setbIsContinue(false);
		}else if(strcmp(pszWidgetName, "cancel_btn") == 0) {
			this->close();
			P_NIHILITY_CTRL->getGenPanel()->open();
			P_NIHILITY_CTRL->setbIsContinue(false);
		}else if(strcmp(pszWidgetName, "sure_btn") == 0) {
			continueFight();
		}
		break;
	default:

		break;
	}
}

void CNihilityContinuePanel::continueFight(){
	if(P_USER_OBJ->checkManual(GLOBAL_CFG_MASTER->GetCfg().infinitemanual()) != 0){
		COMMON_DIALOG->show(TextStr(TEXT_MANUAL_BUY_TIP), this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(CNihilityContinuePanel::buyManualTip));
		return;
	}

	if(P_ARM_OBJ->isWeaponBagFull())  {
		COMMON_DIALOG->show(TextStr(WEAPON_BAG_FULL));
		return;
	}

	if(P_BAG_CTRL->isFull()){
		COMMON_DIALOG->show(TextStr(TEXT_BAG_FULL_OP_TIP), NULL, NounStr(OK));
		return;
	}


	this->showLoading();
	P_NIHILITY_CTRL->resetNihility();
	P_FIGHT_OBJ->setFightType(NIHILITY_FIGHT_TYPE);
	P_FIGHT_CTRL->getFightProxy()->sendQueryPlayerFightInfo();
}

void CNihilityContinuePanel::buyManualTip(){
	P_TIP_CTRL->LowManualBuyTip(this, dialogbtneventselector(CNihilityContinuePanel::buyManual));
}

void CNihilityContinuePanel::buyManual() {
	P_MANUAL_CTRL->sendBuyManualRqst();
}

void CNihilityContinuePanel::open(){
	BasePanel::open();
}

void CNihilityContinuePanel::close(){
	BasePanel::close();

	this->closeLoading();
}


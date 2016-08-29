#include "RelivePanel.h"
#include "bag/BagControl.h"
#include "defines.h"
#include "TextHelper.h"
#include "FightControl.h"
#include "FightObj.h"
#include "tip/TipControl.h"

#include "GlobalCfgMaster.h"

const string CRelivePanel::HERO_RELIVE_EVENT = "hero_relive_event";
const string CRelivePanel::HERO_CANCEL_RELIVE_EVENT = "hero_cancel_relive_event";

CRelivePanel::CRelivePanel(){
	m_pTarget = NULL;
	m_pEventSelector = NULL;

	m_bIsCostGold = true;
	m_uCostGold = 0;
}

CRelivePanel::~CRelivePanel(){

}

bool CRelivePanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/relive_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(false);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CRelivePanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CRelivePanel::touchEventHandler));

	m_pReliveBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("relive_btn"));
	m_pReliveBtn->addTouchEventListener(this, toucheventselector(CRelivePanel::touchEventHandler));

	m_pCancelBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("cancel_btn"));
	m_pCancelBtn->addTouchEventListener(this, toucheventselector(CRelivePanel::touchEventHandler));

	m_pCostItemLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("cost_item_layout"));
	m_pCostGoldLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("cost_gold_layout"));

	m_pBtnGoldImg = dynamic_cast<ImageView*>(m_pReliveBtn->getChildByName("gold_icon"));
	m_pBtnLabelImg = dynamic_cast<ImageView*>(m_pReliveBtn->getChildByName("relive_big_img"));
	m_pBtnReliveIcon = dynamic_cast<ImageView*>(m_pBtnGoldImg->getChildByName("img"));

	m_pBtnCostTxt = dynamic_cast<Label*>(m_pBtnGoldImg->getChildByName("cost_txt"));
	m_pCostItemRightTxt = dynamic_cast<Label*>(m_pCostItemLayout->getChildByName("right_label"));
	m_pCostGoldNumTxt = dynamic_cast<Label*>(m_pCostGoldLayout->getChildByName("cost_txt"));
	m_pCostGoldRightTxt = dynamic_cast<Label*>(m_pCostGoldLayout->getChildByName("right_label"));
}

void CRelivePanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	unsigned int uReliveCfgID = vmsg::ITEM_RELIVE_ID;		//复活币ID 
	unsigned int uReliveNum = P_BAG_CTRL->getItemCnt(uReliveCfgID);

	if(uReliveNum <= 0){
		m_bIsCostGold = true;
		m_uCostGold = P_FIGHT_OBJ->getReliveCost();

		if(m_uCostGold == 0){  //使用首次复活消耗
			m_uCostGold = GLOBAL_CFG_MASTER->GetCfg().relive1stcost();
		}
	}else{
		m_bIsCostGold = false;
	}

	if(!m_bIsCostGold){  //消耗道具
		m_pCostItemLayout->setVisible(true);
		m_pCostGoldLayout->setVisible(false);
		m_pBtnGoldImg->setVisible(false);
		m_pBtnLabelImg->setVisible(true);

		char str[150];
		snprintf(str, 150, TextStr(TEXT_RELIVE_COST_ITEM_TIP), uReliveNum);
		m_pCostItemRightTxt->setText(str);
	}else{			//消耗星钻
		m_pCostItemLayout->setVisible(false);
		m_pCostGoldLayout->setVisible(true);
		m_pBtnGoldImg->setVisible(true);
		m_pBtnLabelImg->setVisible(false);

		float fInter = 23;
		if(m_uCostGold < 1000){
			fInter = 28;
		}else{
			fInter = 23;
		}
		m_pBtnCostTxt->setText(intToString(m_uCostGold));
		m_pBtnReliveIcon->setPositionX(m_pBtnCostTxt->getPositionX() + m_pBtnCostTxt->getSize().width + fInter);

		m_pCostGoldNumTxt->setText("X" + intToString(m_uCostGold));
		m_pCostGoldRightTxt->setPositionX(m_pCostGoldNumTxt->getPositionX() + m_pCostGoldNumTxt->getSize().width + 13);
	}
}

void CRelivePanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0 || strcmp(pszWidgetName, "cancel_btn") == 0) {
			this->close();
			DispatchEvent(HERO_CANCEL_RELIVE_EVENT, this);
		}else if(strcmp(pszWidgetName, "relive_btn") == 0) {
			doRelive();
		}
		break;
	default:

		break;
	}
}

void CRelivePanel::doRelive(){
	if(m_bIsCostGold){  //使用星钻
		P_TIP_CTRL->GoldCostTip(m_uCostGold, this, dialogbtneventselector(CRelivePanel::reliveFun), TextStr(TEXT_RELIVE_COST_GOLD_TIP));
	}else{
		unsigned int uReliveCfgID = vmsg::ITEM_RELIVE_ID;		//复活币ID 
		unsigned int uBagIdx = P_BAG_CTRL->getIdxByItemCfgID(uReliveCfgID);
		P_FIGHT_CTRL->getFightProxy()->sendReliveRqst(1, uReliveCfgID, uBagIdx);
	}
}

void CRelivePanel::reliveFun(){
	P_FIGHT_CTRL->getFightProxy()->sendReliveRqst(0);
}

void CRelivePanel::doReliveDone(){
	this->close();
	DispatchEvent(HERO_RELIVE_EVENT, this);
}

void CRelivePanel::openWithCallFun(CCObject* target, SEL_CallFunc callbackFun){
	m_pTarget = target;
	m_pEventSelector = callbackFun;

	open();
}

void CRelivePanel::open(){
	BasePanel::open();

	P_BAG_CTRL->sendQueryBagInfoRqst();

	this->showLoading();
}

void CRelivePanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}

	if(m_pTarget && m_pEventSelector){
		(m_pTarget->*m_pEventSelector)();
	} 

	m_pTarget = NULL;
	m_pEventSelector = NULL;

	m_bIsCostGold = true;
	m_uCostGold = 0;
}


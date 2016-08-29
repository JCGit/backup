#include "FillBulletPanel.h"
#include "bag/BagControl.h"
#include "defines.h"
#include "TextHelper.h"
#include "FightControl.h"
#include "FightObj.h"
#include "tip/TipControl.h"

#include "GlobalCfgMaster.h"

const string CFillBulletPanel::FILL_BULLET_SUCCESS_EVENT = "fill_bullet_success_event";
const string CFillBulletPanel::CANCEL_FILL_BULLET_EVENT = "cancel_fill_bullet_event";

CFillBulletPanel::CFillBulletPanel(){
	m_pTarget = NULL;
	m_pEventSelector = NULL;

	m_bIsCostGold = true;
	m_uCostGold = 0;
}

CFillBulletPanel::~CFillBulletPanel(){

}

bool CFillBulletPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/fill_bullet_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(false);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CFillBulletPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CFillBulletPanel::touchEventHandler));

	m_pFillBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("fill_btn"));
	m_pFillBtn->addTouchEventListener(this, toucheventselector(CFillBulletPanel::touchEventHandler));

	m_pCancelBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("cancel_btn"));
	m_pCancelBtn->addTouchEventListener(this, toucheventselector(CFillBulletPanel::touchEventHandler));

	m_pCostItemLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("cost_item_layout"));
	m_pCostGoldLayout = dynamic_cast<Layout*>(m_pRootLayout->getChildByName("cost_gold_layout"));

	m_pBtnGoldImg = dynamic_cast<ImageView*>(m_pFillBtn->getChildByName("gold_icon"));
	m_pBtnLabelImg = dynamic_cast<ImageView*>(m_pFillBtn->getChildByName("fill_big_img"));
	m_pBtnFillIcon = dynamic_cast<ImageView*>(m_pBtnGoldImg->getChildByName("img"));

	m_pBtnCostTxt = dynamic_cast<Label*>(m_pBtnGoldImg->getChildByName("cost_txt"));
	m_pCostItemRightTxt = dynamic_cast<Label*>(m_pCostItemLayout->getChildByName("right_label"));
	m_pCostGoldNumTxt = dynamic_cast<Label*>(m_pCostGoldLayout->getChildByName("cost_txt"));
	m_pCostGoldRightTxt = dynamic_cast<Label*>(m_pCostGoldLayout->getChildByName("right_label"));
}

void CFillBulletPanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	unsigned int uFillBulletCfgID = vmsg::ITEM_ARMO_ID;		//µ¯Ò©ÏäID
	unsigned int uFillBulletNum = P_BAG_CTRL->getItemCnt(uFillBulletCfgID);

	if(uFillBulletNum <= 0){
		m_bIsCostGold = true;
		m_uCostGold = P_FIGHT_OBJ->getFillBulletCost();

		if(m_uCostGold == 0){  //Ê¹ÓÃÊ×´Î¹ºÂòµ¯Ò©ÏûºÄ
			m_uCostGold = GLOBAL_CFG_MASTER->GetCfg().bulletbasecost();
		}
	}else{
		m_bIsCostGold = false;
	}

	if(!m_bIsCostGold){  //ÏûºÄµÀ¾ß
		m_pCostItemLayout->setVisible(true);
		m_pCostGoldLayout->setVisible(false);
		m_pBtnGoldImg->setVisible(false);
		m_pBtnLabelImg->setVisible(true);

		char str[150];
		snprintf(str, 150, TextStr(TEXT_FILL_BULLET_COST_ITEM_TIP), uFillBulletNum);
		m_pCostItemRightTxt->setText(str);
	}else{			//ÏûºÄÐÇ×ê
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
		m_pBtnFillIcon->setPositionX(m_pBtnCostTxt->getPositionX() + m_pBtnCostTxt->getSize().width + fInter);

		m_pCostGoldNumTxt->setText("X" + intToString(m_uCostGold));
		m_pCostGoldRightTxt->setPositionX(m_pCostGoldNumTxt->getPositionX() + m_pCostGoldNumTxt->getSize().width + 13);
	}
}

void CFillBulletPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0 || strcmp(pszWidgetName, "cancel_btn") == 0) {
			this->close();
			DispatchEvent(CANCEL_FILL_BULLET_EVENT, this);
		}else if(strcmp(pszWidgetName, "fill_btn") == 0) {
			doBuyBullet();
		}
		break;
	default:

		break;
	}
}

void CFillBulletPanel::doBuyBullet(){
	if(m_bIsCostGold){  //Ê¹ÓÃÐÇ×ê
		P_TIP_CTRL->GoldCostTip(m_uCostGold, this, dialogbtneventselector(CFillBulletPanel::buyBulletFun), TextStr(TEXT_FILL_BULLET_COST_GOLD_TIP));
	}else{
		unsigned int uBulletCfgID = vmsg::ITEM_ARMO_ID;		//µ¯Ò©ÏäID 
		unsigned int uBagIdx = P_BAG_CTRL->getIdxByItemCfgID(uBulletCfgID);
		P_FIGHT_CTRL->getFightProxy()->sendBuyBulletRqst(1, uBulletCfgID, uBagIdx);
	}
}

void CFillBulletPanel::buyBulletFun(){
	P_FIGHT_CTRL->getFightProxy()->sendBuyBulletRqst(0);
}

void CFillBulletPanel::doFillBulletDone(){
	this->close();
	DispatchEvent(FILL_BULLET_SUCCESS_EVENT, this);
}

void CFillBulletPanel::openWithCallFun(CCObject* target, SEL_CallFunc callbackFun){
	m_pTarget = target;
	m_pEventSelector = callbackFun;

	open();
}

void CFillBulletPanel::open(){
	BasePanel::open();

	P_BAG_CTRL->sendQueryBagInfoRqst();

	this->showLoading();
}

void CFillBulletPanel::close(){
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


#include "StrengthenItem.h"
#include "NounHelper.h"
#include "defines.h"
#include "FightEndlessControl.h"
#include "TextHelper.h"
#include "UserObj.h"
#include "CommonDialog.h"
#include "tip/TipControl.h"

CStrengthenItem::CStrengthenItem(){

}

CStrengthenItem::~CStrengthenItem(){

}

CStrengthenItem* CStrengthenItem::create(UILayout *pRefLayout){
	CStrengthenItem *pRet = new CStrengthenItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CStrengthenItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

bool CStrengthenItem::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/strengthen_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CStrengthenItem::initUI(){
	UIImageView *pImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_bg"));
	m_pCell = IconCell::create();
	m_pCell->showNumTxt();
	m_pCell->setInfo(THING_TYPE_GOLD, 0, 0);
	pImg->addChild(m_pCell);

	m_pDesTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));
	m_pBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("cost_btn"));
	m_pBtnTxt = dynamic_cast<UILabel*>(m_pBtn->getChildByName("btn_txt"));

	m_pBtn->addTouchEventListener(this, toucheventselector(CStrengthenItem::touchEventHandler));
}

void CStrengthenItem::setObj(unsigned int idx, const vmsg::CSGen& info){
	m_uIDX = idx;
	m_stInfo = info;

	m_pBtnTxt->setText(to_string(info.costcoin()));

	if(idx == 0){
		m_pDesTxt->setText(TextStr(ENDLESS_RAND_GEN));
		return;
	}

	m_pDesTxt->setText(P_ENDLESS_CTRL->getDescByGenInfo(info));
	if(info.cnt() > 0){
		m_pCell->setNum(info.cnt(), ccYELLOW);
	}else{
		m_pCell->setNum(0, ccYELLOW);
	}
}

const vmsg::CSGen& CStrengthenItem::getObj(){
	return m_stInfo;
}

unsigned int CStrengthenItem::getIDX(){
	return m_uIDX;
}

void CStrengthenItem::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "cost_btn") == 0) {
            if(!P_TIP_CTRL->CoinCostTip(m_stInfo.costcoin(), this)){
				return;
			}
			
			P_ENDLESS_CTRL->sendInfiniteBuyGenRqst(m_uIDX);
		}
		break;
	default:

		break;
	}
}

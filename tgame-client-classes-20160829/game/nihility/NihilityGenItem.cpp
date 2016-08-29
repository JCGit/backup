#include "NihilityGenItem.h"
#include "defines.h"
#include "Resources.h"
#include "tip/TipControl.h"
#include "NihilityControl.h"

#include "GenStrengthenCfgMaster.h"

CNihilityGenItem::CNihilityGenItem(){
	m_uIDX = 0;
}

CNihilityGenItem::~CNihilityGenItem(){

}

CNihilityGenItem* CNihilityGenItem::create(UILayout *pRefLayout){
	CNihilityGenItem *pRet = new CNihilityGenItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CNihilityGenItem::initWithLayout(UILayout *pRefLayout) {
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

bool CNihilityGenItem::init(){
	bool bRet = false;

	do{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/nihility_gen_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	}while(0);

	return bRet;
}

void CNihilityGenItem::initUI(){
	m_pIcon = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("icon_img"));
	m_pNameTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("name_txt"));
	m_pDescTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("desc_txt"));
	m_pBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("buy_btn"));
	m_pCostTxt = dynamic_cast<Label*>(m_pBtn->getChildByName("coin_txt"));
	m_pNumTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("num_txt"));

	m_pBtn->addTouchEventListener(this, toucheventselector(CNihilityGenItem::touchEventHandler));
}

void CNihilityGenItem::setObj(unsigned int idx, const vmsg::CSGen& info){
	m_uIDX = idx;
	m_stInfo = info;

	const GenStrengthenCfg* stCfg = GEN_STRENGTHEN_CFG_MASTER->GetGenStrengthenCfgByID(m_uIDX);

	m_pNameTxt->setText(stCfg->name());
	m_pDescTxt->setText(stCfg->desc());
	m_pCostTxt->setText(intToString(stCfg->costcoin()));
	m_pNumTxt->setVisible(true);

	if(idx == 0){
		m_pNumTxt->setVisible(false);
	}else{
		if(info.cnt() > 0){
			m_pNumTxt->setText(intToString(info.cnt()));
		}else{
			m_pNumTxt->setVisible(false);
		}
	}

	string str = ICON_PATH + stCfg->icon() + ".png";
	m_pIcon->loadTexture(str.c_str());
}

void CNihilityGenItem::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "buy_btn") == 0) {
			if(!P_TIP_CTRL->CoinCostTip(m_stInfo.costcoin(), this)){
				return;
			}
			GUIDE_TOUCH(pWidget);
			P_NIHILITY_CTRL->sendInfiniteBuyGenRqst(m_uIDX);
		}
		break;
	default:

		break;
	}
}

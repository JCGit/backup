#include "SvrActListItem.h"
#include "shop/RmbShopPanel.h"
#include "shop/ShopControl.h"
#include "DailyActControl.h"

#include "DailyActCfgMaster.h"

CSvrActListItem::CSvrActListItem(){

}

CSvrActListItem::~CSvrActListItem(){

}

CSvrActListItem* CSvrActListItem::create(UILayout *pRefLayout) {
	CSvrActListItem *pRet = new CSvrActListItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CSvrActListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/svr_act_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(false);

	return bRet;
}

bool CSvrActListItem::initWithLayout(UILayout *pRefLayout) {
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

void CSvrActListItem::initUI(){
	m_pIconImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("icon"));
	m_pTitleImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("title_img"));
	m_pActTimeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_time_txt"));
	m_pActPrizeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_txt"));

	m_pDummyBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("dummy_btn"));
	m_pDummyBtn->addTouchEventListener(this, toucheventselector(CSvrActListItem::touchEventHandler));
}

void CSvrActListItem::setActID(unsigned int uID){
	m_uActID = uID;

	const DailyActCfg* stCfg = DAILY_ACT_CFG_MASTER->GetDailyActCfgByID(m_uActID);
	string iconStr = "icon/" + stCfg->acticon();
	m_pIconImg->loadTexture(iconStr.c_str());

	string resStr = "daily_act_ui/" + stCfg->titleimg();
	m_pTitleImg->loadTexture(resStr.c_str(), UI_TEX_TYPE_PLIST);

	m_pActTimeTxt->setText(stCfg->timedesc());
	m_pActPrizeTxt->setText(stCfg->awarddesc());
}

void CSvrActListItem::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	switch(type) {
	case TOUCH_EVENT_ENDED:
		if(std::strcmp(pszWidgetName, "dummy_btn") == 0) {
			P_SHOP_CTRL->getRmbShopPanel()->open();
			P_DAILYACT_CTRL->getDailyActPanel()->close();
		} 
		break;
	default:
		break;
	}
}


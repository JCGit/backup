#include "SweepCardShotPanel.h"
#include "shop/ShopControl.h"
#include "shop/RmbShopPanel.h"
#include "TextHelper.h"
#include "ItemUtils.h"
#include "login/UserGoldControl.h"

using namespace vmsg;

CSweepCardShotPanel::CSweepCardShotPanel(){
	
}

CSweepCardShotPanel::~CSweepCardShotPanel(){

}

bool CSweepCardShotPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/sweep_card_shot_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CSweepCardShotPanel::initUI(){
	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CSweepCardShotPanel::touchEventHandler));

	m_pChallengeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("challenge_btn"));
	m_pChallengeBtn->addTouchEventListener(this, toucheventselector(CSweepCardShotPanel::touchEventHandler));

	m_pChargeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("charge_btn"));
	m_pChargeBtn->addTouchEventListener(this, toucheventselector(CSweepCardShotPanel::touchEventHandler));

	m_pBuyBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("buy_btn"));
	m_pBuyBtn->addTouchEventListener(this, toucheventselector(CSweepCardShotPanel::touchEventHandler));

	m_pChallengeBtn->loadTextureNormal("icon/entry/ie00028up.png");
	m_pChargeBtn->loadTextureNormal("icon/entry/ie00009up.png");
}

void CSweepCardShotPanel::updateUI(const vmsg::CSQueryRMBStoreItemRsp& stRsp){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	m_stInfo = stRsp;
}

void CSweepCardShotPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0 || strcmp(pszWidgetName, "challenge_btn") == 0) {
			this->close();
		}else if(strcmp(pszWidgetName, "close_btn") == 0) {
			P_SHOP_CTRL->getRmbShopPanel()->open();
			this->close();
		}else if(strcmp(pszWidgetName, "buy_btn") == 0) {
			doBuy();
		}
		break;
	default:

		break;
	}
}

void CSweepCardShotPanel::doBuy(){
#ifdef TGAME_IN_APPLE_APP_STORE
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) 
	//ios
	CCLog("[trace]RmbShopListItemOne::onTouch,ios,produceid=[%s]", m_stItemInfo.channelproductid().c_str());
	P_SHOP_CTRL->setRMBBuyLoading(true);
	P_USER_GOLD_CTRL->GetProxy()->sendAppAuthedBuyRqst(m_stItemInfo.channelproductid(), 1);
	return;
#endif
#endif
	BuyProductInfo stBuyInfo;
	stBuyInfo.strProductID = m_stInfo.productinfo().channelproductid();
	stBuyInfo.iProductCount = 1; //m_stItemInfo.elmtcnt();
	stBuyInfo.iRealPayMoney = m_stInfo.productinfo().price() * 100; //×¢Òâµ¥Î»»»Ëã!!!!
	stBuyInfo.iSinglePrice = m_stInfo.productinfo().price() * 100;
	stBuyInfo.strProductName = ItemUtils::getItemName(m_stInfo.productinfo().elmttype(), m_stInfo.productinfo().elmtcfgid());
	P_USER_GOLD_CTRL->startBuy(stBuyInfo);
}

void CSweepCardShotPanel::open(){
	BasePanel::open();

	//Ö±½Ó¹ºÂòÉ¨µ´¿¨
	P_USER_GOLD_CTRL->GetProxy()->sendQueryRMBStoreItemRqst(vmsg::RMB_STORE_ITEM_SWEEP_ID);

	this->showLoading();
}

void CSweepCardShotPanel::close(){
	BasePanel::close();

	if(this->getIsInLoading()){
		this->closeLoading();
	}
}


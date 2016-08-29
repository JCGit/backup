#include "ItemLackPanel.h"

#include "defines.h"

#include "basic/ItemUtils.h"
#include "basic/TextHelper.h"

#include "game/login/UserGoldControl.h"
#include "game/shop/ShopControl.h"
#include "game/shop/RmbShopPanel.h"

ItemLackPanel::ItemLackPanel()
:m_pCloseBtn(NULL)
,m_pItemImg(NULL)
,m_pCntTxt(NULL)
,m_pDescTxt(NULL)
,m_pMoreBtn(NULL)
,m_pBuyBtn(NULL)
,m_bIsPaying(false)
{
}

ItemLackPanel::~ItemLackPanel()
{
}

ItemLackPanel* ItemLackPanel::create() {
	ItemLackPanel *pRet = new ItemLackPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ItemLackPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

bool ItemLackPanel::isPaying() {
	return m_bIsPaying;
}

void ItemLackPanel::promptBuyItem(unsigned int uProductID) {
	m_bIsPaying = true;
	P_USER_GOLD_CTRL->GetProxy()->sendQueryRMBStoreItemRqst(uProductID);
}

void ItemLackPanel::showProductInfo(const vmsg::CSRMBStoreProductInfo& stProductInfo) {
	m_bIsPaying = false;
	open();

	showLoading();

	if(!m_bUIInited) {
		initUI();
	}

	m_stProductInfo = stProductInfo;

	string iconURL = ItemUtils::getItemIcon(m_stProductInfo.elmttype(), m_stProductInfo.elmtcfgid());
	m_pItemImg->loadTexture(iconURL.c_str());

	m_pCntTxt->setText(string("x") + intToString(m_stProductInfo.elmtcnt()));

	string desc = ItemUtils::getItemDesc(m_stProductInfo.elmttype(), m_stProductInfo.elmtcfgid());
	char text[100];
	snprintf(text, 100, TextStr(TEXT_BUY_EXP_ITEM), desc.c_str(), m_stProductInfo.price());
	m_pDescTxt->setText(text);

	char title[100];
	snprintf(title, 100, TextStr(TEXT_RMB_BUY), m_stProductInfo.price());
	m_pBuyBtn->setTitleText(title);

	closeLoading();
}

void ItemLackPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/ItemLackPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(ItemLackPanel::touchEventHandler));

	m_pItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_img"));

	m_pCntTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("cnt_txt"));

	m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));

	m_pMoreBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("more_btn"));
	m_pMoreBtn->addTouchEventListener(this, toucheventselector(ItemLackPanel::touchEventHandler));

	m_pBuyBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("buy_btn"));
	m_pBuyBtn->addTouchEventListener(this, toucheventselector(ItemLackPanel::touchEventHandler));

	m_bUIInited = true;
}    

void ItemLackPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			} else if(pSender == m_pMoreBtn) {
				P_SHOP_CTRL->getRmbShopPanel()->open();
				P_SHOP_CTRL->getRmbShopPanel()->switchTab(RmbShopPanel::SHOP_TAB_ITEM);
			} else if(pSender == m_pBuyBtn) {
				buyItem();
			}
			break;
		default:;
	}
}

void ItemLackPanel::buyItem() {
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
	stBuyInfo.strProductID = m_stProductInfo.channelproductid();
	stBuyInfo.iProductCount = m_stProductInfo.elmtcnt();
	stBuyInfo.iRealPayMoney = m_stProductInfo.price() * 100; //×¢Òâµ¥Î»»»Ëã!!!!
	stBuyInfo.iSinglePrice = m_stProductInfo.price() * 100;
	stBuyInfo.strProductName = ItemUtils::getItemName(m_stProductInfo.elmttype(), m_stProductInfo.elmtcfgid());
	P_USER_GOLD_CTRL->startBuy(stBuyInfo);
}

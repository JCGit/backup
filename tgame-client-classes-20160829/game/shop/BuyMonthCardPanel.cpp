#include "BuyMonthCardPanel.h"

#include "ModuleID.pb.h"

#include "basic/ItemUtils.h"

#include "game/UserObj.h"
#include "game/Global.h"
#include "game/city/AutoPopControl.h"
#include "game/city/CityControl.h"
#include "game/login/UserGoldControl.h"

#include "ShopControl.h"

BuyMonthCardPanel::BuyMonthCardPanel()
:m_pCloseBtn(NULL)
,m_pBuyBtn(NULL)
{
}

BuyMonthCardPanel::~BuyMonthCardPanel()
{
}

BuyMonthCardPanel* BuyMonthCardPanel::create() {
	BuyMonthCardPanel *pRet = new BuyMonthCardPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BuyMonthCardPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		bRet = true;
	} while(0);

	return bRet;
}

void BuyMonthCardPanel::open() {
	BasePanel::open();

	showLoading();
	P_USER_GOLD_CTRL->GetProxy()->sendQueryRMBStoreItemRqst(vmsg::RMB_STORE_MONTHCARD_ID);

	if(!m_bUIInited) {
		initUI();
	}
}

void BuyMonthCardPanel::close() {
	BasePanel::close();

	P_AUTO_POP_CTRL->onClosePanel(this);
}

bool BuyMonthCardPanel::isMeetAutoOpen() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_MONTHCARD));

		// month card 
		const vmsg::CSMonthCardInfo &stMonthCardInfo = P_USER_OBJ->getUserInfo()->stMonthCardInfo;
		unsigned int uSvrTime = P_GLOBAL->getSvrTime();
		unsigned int uEndTime = stMonthCardInfo.endtime();
		CC_BREAK_IF(uEndTime > uSvrTime); // 月卡没过期
		
		bRet = true;
	} while(0);

	return bRet;
}

void BuyMonthCardPanel::updateProductInfo(const vmsg::CSRMBStoreProductInfo& stProductInfo) {
	m_stProductInfo = stProductInfo;
	closeLoading();
}

void BuyMonthCardPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/BuyMonthCardPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(BuyMonthCardPanel::touchEventHandler));

	m_pBuyBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("buy_btn"));
	m_pBuyBtn->addTouchEventListener(this, toucheventselector(BuyMonthCardPanel::touchEventHandler));

	m_bUIInited = true;
}

void BuyMonthCardPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(pSender == m_pCloseBtn) {
				close();
			} else if(pSender == m_pBuyBtn) {
				buyMonthCard();
			}
			break;
		default:;
	}
}

void BuyMonthCardPanel::buyMonthCard() {
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
	stBuyInfo.iProductCount = 1;
	stBuyInfo.iRealPayMoney = m_stProductInfo.price() * 100; //注意单位换算!!!!
	stBuyInfo.iSinglePrice = m_stProductInfo.price() * 100;
	stBuyInfo.strProductName = ItemUtils::getItemName(m_stProductInfo.elmttype(), m_stProductInfo.elmtcfgid());
	P_USER_GOLD_CTRL->startBuy(stBuyInfo);
}

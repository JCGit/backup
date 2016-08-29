#include "RestrictShopPanel.h"
#include "RestrictShopItem.h"
#include "defines.h"
#include "TimeEx.h"
#include "ShopControl.h"
#include "Global.h"

CRestrictShopPanel::CRestrictShopPanel(){
	m_uTimeCount = 0;
}

CRestrictShopPanel::~CRestrictShopPanel(){
	for(int i = 0; i < MAX_ITEM_NUM; ++i) {
		CRestrictShopItem* pItem = m_stItemLst[i];
		pItem->release();
	}
}

bool CRestrictShopPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);

		initLayoutFromJsonFile("ui_v2/restrict_shop_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void CRestrictShopPanel::initUI(){
	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CRestrictShopPanel::touchEventHandler));

	m_pTimeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_txt"));

	//init item cell
	for(int i = 0; i < MAX_ITEM_NUM; ++i) {
		CRestrictShopItem* pItem = CRestrictShopItem::create();
		pItem->retain();
		pItem->initLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName(string("item_" + intToString(i)).c_str() )) );
		m_stItemLst[i] = pItem;
	}
}

void CRestrictShopPanel::open() {
	BasePanel::open();
	
	P_SHOP_CTRL->sendRestrictShopQueryRqst();

	this->showLoading();
}

void CRestrictShopPanel::close() {
	BasePanel::close();
	this->unschedule(schedule_selector(CRestrictShopPanel::timeCountDown));
}

void CRestrictShopPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();			
		}		
		break;
	default:

		break;
	}
}

void CRestrictShopPanel::updateUI(const CSPanicBuyingStoreQueryRsp& stInfo, bool bIsUpdateTime/* = true*/){
	if(!this->isOpen()) {
		return;
	}
	this->closeLoading();

	if(bIsUpdateTime){
		m_uTimeCount = stInfo.nextrefreshtime() - P_GLOBAL->getSvrTimeRelative();
		m_pTimeTxt->setText(TimeEx::getStringTimeForInt(m_uTimeCount));
		this->unschedule(schedule_selector(CRestrictShopPanel::timeCountDown));
		this->schedule(schedule_selector(CRestrictShopPanel::timeCountDown), 1.0f);
	}

	for(int i = 0; i < stInfo.items_size(); ++i) {
		const CSPanicBuyingStoreItem& oneItem = stInfo.items(i);
		if(i >= MAX_ITEM_NUM) {
			break;
		}
		CRestrictShopItem* pItem = m_stItemLst[i];
		pItem->setInfo(oneItem, i);
	}
}

void CRestrictShopPanel::timeCountDown(float dt){
	if(m_uTimeCount <= 0){
		this->unschedule(schedule_selector(CRestrictShopPanel::timeCountDown));
		P_SHOP_CTRL->sendRestrictShopQueryRqst();
		P_SHOP_CTRL->getRestrictItemInfoPanel()->close();
		if(this->isOpen()) {
			showLoading();
		}
		return;
	}

	m_uTimeCount--;
	m_pTimeTxt->setText(TimeEx::getStringTimeForInt(m_uTimeCount));
}


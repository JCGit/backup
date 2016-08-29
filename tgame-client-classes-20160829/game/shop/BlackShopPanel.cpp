#include "BlackShopPanel.h"
#include "BlackShopItem.h"
#include "defines.h"
#include "TimeEx.h"
#include "ShopControl.h"
#include "frag/FragControl.h"
#include "tip/TipControl.h"

BlackShopPanel::BlackShopPanel()
{
    m_uTimeCount = 0;
}

BlackShopPanel::~BlackShopPanel()
{
    for(int i = 0; i < MAX_ITEM_NUM; ++i) {
        BlackShopItem* pItem = m_stItemLst[i];
        pItem->release();
    }
}

BlackShopPanel* BlackShopPanel::create() {
	BlackShopPanel *pRet = new BlackShopPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BlackShopPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);

		initLayoutFromJsonFile("ui_v2/black_shop_ui.ExportJson");
		addChild(m_pRootLayout);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(BlackShopPanel::touchEventHandler));

        m_pRefreshBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("refresh_btn"));
        m_pRefreshBtn->addTouchEventListener(this, toucheventselector(BlackShopPanel::touchEventHandler));

        m_pTimeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_txt"));

        //init item cell
        for(int i = 0; i < MAX_ITEM_NUM; ++i) {
            BlackShopItem* pItem = BlackShopItem::create();
            pItem->retain();
            pItem->initLayout(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName(string("item_" + intToString(i)).c_str() )) );
            m_stItemLst[i] = pItem;
        }

		bRet = true;
	} while(0);

	return bRet;
}

void BlackShopPanel::open() {
    BasePanel::open();
    P_SHOP_CTRL->getProxy()->sendBlackShopQueryRqst();
    P_FRAG_CTRL->getProxy()->sendQueryDebrisBagRqst(); //²éÑ¯Ò»ÏÂ
    this->showLoading();
}

void BlackShopPanel::close() {
    BasePanel::close();
    this->unschedule(schedule_selector(BlackShopPanel::timeCountDown));
}

void BlackShopPanel::updateUI(const CSBlackMarketInfo& stInfo) {
	
    if(!this->isOpen()) {
        return;
    }
    this->closeLoading();

    m_uResetCost = stInfo.resetcost();
    m_uTimeCount = stInfo.remaintime();
    m_uRemainResetCnt = stInfo.remainresetcnt();

    /*bool bHaveResetCnt = (m_uRemainResetCnt > 0);
    m_pRefreshBtn->setVisible(bHaveResetCnt);
    m_pRefreshBtn->setEnabled(bHaveResetCnt);*/

    m_pTimeTxt->setText(TimeEx::getStringTimeForInt(m_uTimeCount));
    this->unschedule(schedule_selector(BlackShopPanel::timeCountDown));
    this->schedule(schedule_selector(BlackShopPanel::timeCountDown), 1.0f);
    
    for(int i = 0; i < stInfo.products_size(); ++i) {
        const CSBlackProductInfo& oneItem = stInfo.products(i);
        if(i >= MAX_ITEM_NUM) {
            break;
        }
        BlackShopItem* pItem = m_stItemLst[i];
        pItem->setInfo(oneItem, i);
    }
}


void BlackShopPanel::timeCountDown(float dt){
    if(m_uTimeCount <= 0){
        this->unschedule(schedule_selector(BlackShopPanel::timeCountDown));
        P_SHOP_CTRL->getProxy()->sendBlackShopQueryRqst();
        P_SHOP_CTRL->getBlackShopItemInfoPanel()->close();
        if(this->isOpen()) {
            showLoading();
        }
        return;
    }

    m_uTimeCount--;
    m_pTimeTxt->setText(TimeEx::getStringTimeForInt(m_uTimeCount));
}


void BlackShopPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    string strWidgetName = pWidget->getName();

    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strcmp(pszWidgetName, "close_btn") == 0) {
            this->close();			
        }else if(strcmp(pszWidgetName, "refresh_btn") == 0) {
            if(m_uRemainResetCnt > 0) {
				P_TIP_CTRL->GoldCostTip(m_uResetCost, this, dialogbtneventselector(BlackShopPanel::refreshFun), TextStr(TEXT_REFRESH_COST_TIP));
            } else {
				P_TIP_CTRL->getPromptPayPanel()->openWithVIPModule(vmsg::VIP_MODULE_BLACKSTORERESET);
			}
        }		
        break;
    default:

        break;
    }
}

void BlackShopPanel::refreshFun(){
    P_SHOP_CTRL->getProxy()->sendBlackShopResetRqst();
    
}


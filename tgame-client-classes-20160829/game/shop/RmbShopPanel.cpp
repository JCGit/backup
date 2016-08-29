#include "RmbShopPanel.h"
#include "RmbShopListItem.h"
#include "ImageNumber.h"
#include "UserObj.h"
#include "defines.h"
#include "VIPLvCfgMaster.h"
#include "login/UserGoldControl.h"
#include "vip/VIPControl.h"
#include "FaceBookSDK.h"

RmbShopPanel::RmbShopPanel()
{
	m_uCellCnt = 0;
	m_uIdx = 0;
}

RmbShopPanel::~RmbShopPanel()
{
}

RmbShopPanel* RmbShopPanel::create() {
	RmbShopPanel *pRet = new RmbShopPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

void RmbShopPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();

    switch(type) {
    case TOUCH_EVENT_ENDED:
        if(std::strcmp(pszWidgetName, "close_btn") == 0) {
            close();
        } else if(std::strcmp(pszWidgetName, "vip_btn") == 0) {
            P_VIP_CTRL->getVIPPanel()->open();  
        } else if(std::strcmp(pszWidgetName, "tab_charge") == 0){
			/*m_pChargeBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
			m_pChargeBtn->setTitleFontSize(24);
			m_pChargeBtn->setTitleColor(ccc3(0xd0, 0xff, 0xb0));*/

			showTabByIdx(SHOP_TAB_GOLD);
		} else if(std::strcmp(pszWidgetName, "tab_item") == 0){
			/*m_pItemBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
			m_pItemBtn->setTitleFontSize(24);
			m_pItemBtn->setTitleColor(ccc3(0xd0, 0xff, 0xb0));*/

			showTabByIdx(SHOP_TAB_ITEM);
		}
        break;
    default:;
    }
}

bool RmbShopPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);
        setShowMoneyTopBar(true);

		initLayoutFromJsonFile("ui_v2/rmb_shop_panel.ExportJson");
		addChild(m_pRootLayout);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(RmbShopPanel::touchEventHandler));

        m_pVipBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("vip_btn"));
        m_pVipBtn->addTouchEventListener(this, toucheventselector(RmbShopPanel::touchEventHandler));

		m_pChargeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_charge"));
		m_pChargeBtn->addTouchEventListener(this, toucheventselector(RmbShopPanel::touchEventHandler));

		m_pItemBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("tab_item"));
		m_pItemBtn->addTouchEventListener(this, toucheventselector(RmbShopPanel::touchEventHandler));

        m_pRemainGoldTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("remain_gold_txt"));
        m_pGoldTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("gold_tip_txt"));

        m_pNextVipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("next_vip_txt"));

        m_pGoldBar = dynamic_cast<UILoadingBar*>(m_pRootLayout->getChildByName("gold_bar"));
        m_pGoldBarValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("gold_bar_val_txt"));

        m_pGoldTipLabel0 = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("gold_tip_label_0"));
        
		m_pTableView = CCTableView::create(this, CCSizeMake(670, 351));
		m_pTableView->setDirection(kCCScrollViewDirectionVertical);
		m_pTableView->setPosition(ccp(30, 10));
		m_pTableView->setDelegate(this);
		m_pTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

        UIImageView* pBgInner = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_inner"));
		pBgInner->addNode(m_pTableView);

        m_pVIPTxt = ImageNumber::create(25);
        m_pVIPTxt->setAnchorPoint(ccp(1.0f, 0.5f));
        CCPoint pt = m_pGoldBar->getPosition();
        pt.x = pt.x - m_pGoldBar->getSize().width/2 - 30;
		pt.y = pt.y + 10;
        m_pVIPTxt->setPosition(pt);
        m_pRootLayout->addNode(m_pVIPTxt);
        m_pVIPTxt->setNum("0", 0.0f, "vip");

		bRet = true;
	} while(0);

	return bRet;
}

void RmbShopPanel::updateUI(const vmsg::CSQueryRMBStoreRsp& stRsp) {
    if(!this->isOpen()) {
        return;
    }
    if(this->getIsInLoading()) {
        this->closeLoading();
    }

    const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();

	const unsigned int vipLv = pUserInfo->vipLv;
    
	VIPLvCfg *pCurVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(vipLv);

	const VIPLvCfgSet &stVIPLvCfgSet = VIP_LV_CFG_MASTER->GetCfg();
	unsigned int uMaxVIPLv = stVIPLvCfgSet.viplvcfg(stVIPLvCfgSet.viplvcfg_size() - 1).viplv();

	const unsigned int uTotalGold = P_USER_OBJ->getUserInfo()->totalGoldByMoney;
	bool isTopVIP = (vipLv == uMaxVIPLv);

    m_pVIPTxt->setNum(intToString(vipLv), 0.0f, "vip");

	if(!isTopVIP) {
		VIPLvCfg *pNextVIPLvCfg = VIP_LV_CFG_MASTER->getVIPLvCfg(vipLv + 1);
		unsigned int uTotalNeedGold = pNextVIPLvCfg->needgold();
		unsigned int uNowNeedGold = pNextVIPLvCfg->needgold() - uTotalGold;
		m_pRemainGoldTxt->setText(intToString(uNowNeedGold)); //再充值uNowNeedGold 星钻

        m_pGoldTipTxt->setPositionX(m_pRemainGoldTxt->getPositionX() + m_pRemainGoldTxt->getSize().width + 5);

        m_pNextVipTxt->setText("vip" + intToString(vipLv+1));
        m_pNextVipTxt->setPositionX(m_pGoldTipTxt->getPositionX() + m_pGoldTipTxt->getSize().width + 5);

        m_pGoldBar->setPercent(100 * uTotalGold / uTotalNeedGold);
        m_pGoldBarValTxt->setText(intToString(uTotalGold) + "/" + intToString(uTotalNeedGold));
	} else {
		m_pGoldBar->setPercent(100);
	}
	m_pGoldBarValTxt->setVisible(!isTopVIP);
	m_pGoldTipLabel0->setVisible(!isTopVIP);
	m_pRemainGoldTxt->setVisible(!isTopVIP);
    m_pNextVipTxt->setVisible(!isTopVIP);
	m_pGoldTipTxt->setVisible(!isTopVIP);

    //update item
    m_stRmbShopRsp = stRsp;
    m_stBoughtIDSet.clear();
    for(int i = 0; i < stRsp.boughtids_size(); ++i) {
        m_stBoughtIDSet.insert(stRsp.boughtids(i));
    }

	showTabByIdx(m_uIdx == 0 ? SHOP_TAB_GOLD : m_uIdx);
}

void RmbShopPanel::showTabByIdx(unsigned int iIdx){
	if(iIdx <= 0 || iIdx > 2){
		return;
	}

	//iIdx: 1、充值星钻 2、购买道具
	m_uIdx = iIdx;
	m_stItemLst.clear();

	for(int i = 0; i < m_stRmbShopRsp.products_size(); i++){
		if(m_stRmbShopRsp.products(i).tabid() == iIdx){
			m_stItemLst.push_back(m_stRmbShopRsp.products(i));
		}
	}

	if(m_uIdx == SHOP_TAB_GOLD) {
		m_pChargeBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
		m_pChargeBtn->setTitleFontSize(24);
		m_pChargeBtn->setTitleColor(ccc3(0xd0, 0xff, 0xb0));
	} else {
		m_pChargeBtn->setBrightStyle(BRIGHT_NORMAL);
		m_pChargeBtn->setTitleFontSize(22);
		m_pChargeBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
	}

	if(m_uIdx == SHOP_TAB_ITEM) {
		m_pItemBtn->setBrightStyle(BRIGHT_HIGHLIGHT);
		m_pItemBtn->setTitleFontSize(24);
		m_pItemBtn->setTitleColor(ccc3(0xd0, 0xff, 0xb0));
	} else {
		m_pItemBtn->setBrightStyle(BRIGHT_NORMAL);
		m_pItemBtn->setTitleFontSize(22);
		m_pItemBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
	}

	/*if(iIdx == 1){
		m_pItemBtn->setBrightStyle(BRIGHT_NORMAL);
		m_pItemBtn->setTitleFontSize(22);
		m_pItemBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
	}else{
		m_pChargeBtn->setBrightStyle(BRIGHT_NORMAL);
		m_pChargeBtn->setTitleFontSize(22);
		m_pChargeBtn->setTitleColor(ccc3(0x7b, 0xbc, 0xe1));
	}*/

	m_uCellCnt = ceil(m_stItemLst.size() / 2.0f);
	m_pTableView->reloadData();
}

CCSize RmbShopPanel::cellSizeForTable(CCTableView *table){
	return CCSizeMake(table->getContentSize().width, 117);
}

unsigned int RmbShopPanel::numberOfCellsInTableView(CCTableView *table){
	return m_uCellCnt;
}

CCTableViewCell* RmbShopPanel::tableCellAtIndex(CCTableView *table, unsigned int idx){
	CCTableViewCell* cell = table->dequeueCell();
	RmbShopListItem* pItem = NULL;

	if(!cell){
		cell = new CCTableViewCell();
		cell->autorelease();

		pItem = RmbShopListItem::create();

		cell->addChild(pItem, 0, RMB_SHOP_ITEM_TAG);
	}else{
		pItem = dynamic_cast<RmbShopListItem*>(cell->getChildByTag(RMB_SHOP_ITEM_TAG));
	}

    //因为每行有2个
    int iArrayIdx = idx * 2;
    const int iMaxNum = m_stItemLst.size();
    bool bHasBought = false;
    if(iArrayIdx < iMaxNum) {
        bHasBought = (m_stBoughtIDSet.count( m_stItemLst.at(iArrayIdx).id() ) > 0) ;
        pItem->updateUI(m_stItemLst.at(iArrayIdx), true, bHasBought);

        iArrayIdx++;
        if(iArrayIdx < iMaxNum) {
            bHasBought = (m_stBoughtIDSet.count( m_stItemLst.at(iArrayIdx).id() ) > 0) ;
            pItem->updateUI(m_stItemLst.at(iArrayIdx), false, bHasBought );
        } else {
            pItem->setTwoEmpty();
        }
    }

	return cell;
}

void RmbShopPanel::tableCellTouched(CCTableView* table, CCTableViewCell* cell){

    bool bIsTop = isPanelOnTop();
    if(!bIsTop) {
        return; //为了屏蔽事件顺序
    }

    RmbShopListItem* curItem = NULL;
    curItem = dynamic_cast<RmbShopListItem*>(cell->getChildByTag(RMB_SHOP_ITEM_TAG));
    if(curItem) {
        curItem->onItemTouched(m_stGlobalTouchPt);
    }

}

void RmbShopPanel::switchTab(ShopTab tab) {
	showTabByIdx(tab);
}

void RmbShopPanel::close(){
    BasePanel::close();

	m_uIdx = 0;
}

void RmbShopPanel::open(){

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	BasePanel::open();
#endif
    

	//直接调用SDK，不打开面板
	FaceBookSDK::onPay(100);

    //showLoading();
    P_USER_GOLD_CTRL->GetProxy()->sendQueryRMBStoreRqst();
}


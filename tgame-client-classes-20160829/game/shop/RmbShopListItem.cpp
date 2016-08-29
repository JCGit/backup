#include "RmbShopListItem.h"
#include "ShopControl.h"
#include "defines.h"
#include "UserObj.h"

#include "basic/ItemUtils.h"
#include "basic/CommonDialog.h"
#include "basic/TextHelper.h"
#include "NounHelper.h"
#include "login/UserGoldControl.h"
#include "ItemUtils.h"

RmbShopListItem::RmbShopListItem()
{
}

RmbShopListItem::~RmbShopListItem()
{
}

RmbShopListItem* RmbShopListItem::create(UILayout *pRefLayout) {
	RmbShopListItem *pRet = new RmbShopListItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}


RmbShopListItem* RmbShopListItem::create() {
    RmbShopListItem *pRet = new RmbShopListItem();

    if(pRet != NULL && pRet->initLayout()) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }

    return pRet;
}
bool RmbShopListItem::initLayout() {
    bool bRet = false;

    do {
        CC_BREAK_IF(!BaseLayout::init());

        initLayoutFromJsonFile("ui_v2/rmb_shop_list_item.ExportJson");
        addChild(m_pRootLayout);
        initUI();

        bRet = true;
    } while(0);

    return bRet;
}

void RmbShopListItem::initUI() {
    m_stItemOne.init( dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("item_0")) );
    m_stItemTwo.init( dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("item_1")) );
}

bool RmbShopListItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);
        initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void RmbShopListItem::updateUI(const CSRMBStoreProductInfo& stItemInfo, const bool bIsFirst, const bool bHasBought) {
	if(bIsFirst) {
        m_stItemOne.setInfo(stItemInfo, bHasBought);
    } else {
        m_stItemTwo.setInfo(stItemInfo, bHasBought);
    }
}

void RmbShopListItem::setTwoEmpty() {
    m_stItemTwo.setEmpty();
}

void RmbShopListItem::onItemTouched(const CCPoint& stGlobalPt) {
    UILayout* pLayout = m_stItemOne.getRefLayout();
    if(pLayout && pLayout->hitTest(stGlobalPt)) {
        m_stItemOne.onTouch();
        return;
    }

    pLayout = m_stItemTwo.getRefLayout();
    if(pLayout && pLayout->isVisible() && pLayout->hitTest(stGlobalPt)) {
        m_stItemTwo.onTouch();
        return;
    }

}

///////////////////////////////////////////////////////////////////////////
void RmbShopListItemOne::init(UILayout* pRefImg) {
    if(NULL == pRefImg) {
        CCLOG("[warn]RmbShopListItemOne::init, ref img is null");
        return;
    }
    m_pRefLayout = pRefImg;
    
    m_pCellBg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("cell_bg"));
    m_pCellImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("cell_img"));
	m_pDiscountImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("discount_img"));
	m_pAwardTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("award_txt"));
    m_pGoldNumTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("gold_num_txt"));
    m_pRmbNumTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("rmb_num_txt"));
    m_pRmbNameTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("rmb_name_txt"));
    m_pDescTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("desc_txt"));
}



void RmbShopListItemOne::setInfo(const CSRMBStoreProductInfo& stItemInfo, const bool bHasBought) {
    m_stItemInfo = stItemInfo;
    m_pRefLayout->setVisible(true);

	string goldStr = intToString(stItemInfo.elmtcnt());
	string itemName = ItemUtils::getItemName(stItemInfo.elmttype(), stItemInfo.elmtcfgid());

	if(stItemInfo.elmtcnt() > 1) {
		m_pGoldNumTxt->setText(goldStr + itemName);
	} else {
		m_pGoldNumTxt->setText(itemName);
	}

    m_pRmbNumTxt->setText(intToString(stItemInfo.price()));

	string iconURL = "";

    char str[100] = {0};

    if(!bHasBought) {
        //首次充值
        if(stItemInfo.firstextracnt() > 0) {
            snprintf(str, 100, TextStr(TEXT_RMB_FIRST_GIFT), stItemInfo.firstextracnt() );
        }
        
    } else {
        if( stItemInfo.extracnt() > 0) {
            snprintf(str, 100, TextStr(TEXT_RMB_EXTRA_GIFT), stItemInfo.extracnt());
        }
    }

	bool bIsShowTip = true;
    if(vmsg::THING_TYPE_MONTHCARD == stItemInfo.elmttype()) {
        //int iDay = P_USER_OBJ->getUserInfo()->stMonthCardInfo.totaldays();
        int iGold = P_USER_OBJ->getUserInfo()->stMonthCardInfo.gold();
        snprintf(str, 100, TextStr(TEXT_MONTH_CARD_TIP), iGold);
		iconURL = string("icon/") + stItemInfo.icon() + ".png";
		m_pDescTxt->setText(str);
		bIsShowTip = true;
    } else if(vmsg::THING_TYPE_ITEM == stItemInfo.elmttype()) {
		iconURL = string("item/") + stItemInfo.icon() + ".png";
		m_pDescTxt->setText(ItemUtils::getItemDesc(stItemInfo.elmttype(), stItemInfo.elmtcfgid()));
		bIsShowTip = false;
	} else {
		iconURL = string("icon/") + stItemInfo.icon() + ".png";
		m_pDescTxt->setText(str);
		bIsShowTip = true;
	}

    m_pCellImg->loadTexture(iconURL.c_str());

	if(bIsShowTip){
		m_pDiscountImg->setVisible(false);
		if(stItemInfo.extraitemcnt() > 0){
			m_pAwardTxt->setVisible(true);

			string extraName = TextStr(TEXT_VIP_EXTRA_AWARD);
			extraName += ItemUtils::getItemName(stItemInfo.extraitemtype(), stItemInfo.extraitemid());
			extraName += "x" + intToString(stItemInfo.extraitemcnt());
			m_pAwardTxt->setText(extraName);

			m_pDescTxt->setPositionY(47);
		}else{
			m_pAwardTxt->setVisible(false);
			m_pDescTxt->setPositionY(31);
		}
	}else{
		m_pDiscountImg->setVisible(true);
		m_pAwardTxt->setVisible(false);
		m_pDescTxt->setPositionY(31);
	}
}

void RmbShopListItemOne::setEmpty() {
    m_pRefLayout->setVisible(false);
}

void RmbShopListItemOne::onTouch() {
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
    stBuyInfo.strProductID = m_stItemInfo.channelproductid();
    stBuyInfo.iProductCount = 1; //m_stItemInfo.elmtcnt();
    stBuyInfo.iRealPayMoney = m_stItemInfo.price() * 100; //注意单位换算!!!!
    stBuyInfo.iSinglePrice = m_stItemInfo.price() * 100;
    stBuyInfo.strProductName = ItemUtils::getItemName(m_stItemInfo.elmttype(), m_stItemInfo.elmtcfgid());
    P_SHOP_CTRL->setRMBBuyLoading(true);
    P_USER_GOLD_CTRL->startBuy(stBuyInfo);

}

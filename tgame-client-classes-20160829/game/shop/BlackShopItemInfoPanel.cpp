#include "BlackShopItemInfoPanel.h"
#include "IconCell.h"
#include "ShopControl.h"
#include "UserObj.h"
#include "bag/BagControl.h"
#include "firearms/FireArmsObj.h"
#include "defines.h"
#include "ItemUtils.h"
#include "CCColorUtil.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "DebrisCfgMaster.h"
#include "frag/FragControl.h"
#include "ToolUtils.h"
#include "firearms/FireArmsObj.h"
#include "tip/TipControl.h"



BlackShopItemInfoPanel::BlackShopItemInfoPanel()
{
}

BlackShopItemInfoPanel::~BlackShopItemInfoPanel()
{
}

BlackShopItemInfoPanel* BlackShopItemInfoPanel::create() {
	BlackShopItemInfoPanel *pRet = new BlackShopItemInfoPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool BlackShopItemInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

        setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/black_shop_item_ui.ExportJson");
		addChild(m_pRootLayout);

        m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->addTouchEventListener(this, toucheventselector(BlackShopItemInfoPanel::touchEventHandler));

        m_pBuyBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("buy_btn"));
        m_pBuyBtn->addTouchEventListener(this, toucheventselector(BlackShopItemInfoPanel::touchEventHandler));

        m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
        m_pNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_txt"));
        m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));
        m_pGoldIcon = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("gold_icon"));
        m_pGoldTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("gold_txt"));


        m_pCellBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_bg"));

        m_pIconCell = IconCell::create();
        m_pIconCell->showNumTxt();

        m_pCellBg->addChild(m_pIconCell);

		bRet = true;
	} while(0);

	return bRet;
}

void BlackShopItemInfoPanel::open() {
    BasePanel::open();
}

void BlackShopItemInfoPanel::close() {
    BasePanel::close();
    if(COMMON_DIALOG->isOpen()) {
        COMMON_DIALOG->close();
    }
}


void BlackShopItemInfoPanel::setInfo(const CSBlackProductInfo& oneItem, const int iIdx) {
	if(!this->isOpen()) {
        return;
    }
    m_iItemIdx = iIdx;
    m_iGoldCnt = oneItem.currcnt();
    m_iGoldType = oneItem.currtype();
    m_bHasBought = (oneItem.hasbought() > 0);
    m_iProductType = oneItem.producttype();

    int iHaveNum = 0;
    bool bIsDebris = false;

    CToolUtils::adjustIconBgSize(oneItem.producttype(), oneItem.productcfgid(), m_pCellBg);
    CToolUtils::adjustFragIcon(oneItem.producttype(), m_pCellBg);

    if(vmsg::THING_TYPE_WEAPON == oneItem.producttype()) {
        iHaveNum = P_ARM_OBJ->getWeaponNumByCfgID(oneItem.productcfgid());
    } else if(vmsg::THING_TYPE_DEBRIS == oneItem.producttype()) {
        bIsDebris = true;
        iHaveNum = P_FRAG_CTRL->getNumByCfgID(oneItem.productcfgid());
    } else {
        if(vmsg::THING_TYPE_EQUIP == oneItem.producttype() ) {
            iHaveNum = P_BAG_CTRL->getEquipCnt(oneItem.productcfgid(), 0);
        } else {
            iHaveNum = P_BAG_CTRL->getItemCnt(oneItem.productcfgid());
        }
    }

    
    m_pNumTxt->setText(intToString(iHaveNum));
    m_pIconCell->setInfo(oneItem.producttype(), oneItem.productcnt(), oneItem.productcfgid() );

    m_pNameTxt->setText(ItemUtils::getItemName(oneItem.producttype(), oneItem.productcfgid()));


    bool bIsMoneyEnough = false;
    if(vmsg::THING_TYPE_COIN == oneItem.currtype()) {
        m_pGoldIcon->loadTexture("share/icon_coin.png", UI_TEX_TYPE_PLIST);
        bIsMoneyEnough = (0 == P_USER_OBJ->checkCoin(oneItem.currcnt()) );
    } else {
        m_pGoldIcon->loadTexture("share/icon_gold.png", UI_TEX_TYPE_PLIST);
        bIsMoneyEnough = (0 == P_USER_OBJ->checkGold(oneItem.currcnt()) );
    }
    if(bIsMoneyEnough) {
        CCColorUtil::setLabelColor(m_pGoldTxt, ccGREEN);
    } else {
        CCColorUtil::setLabelColor(m_pGoldTxt, ccRED);
    }

    m_pGoldTxt->setText(intToString(oneItem.currcnt()));

    m_pDescTxt->setText(ItemUtils::getItemDesc(oneItem.producttype(), oneItem.productcfgid()));
}



void BlackShopItemInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();
    string strWidgetName = pWidget->getName();

    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(strcmp(pszWidgetName, "close_btn") == 0) {
            this->close();			
        }else if(strcmp(pszWidgetName, "buy_btn") == 0) {
            doBuy();
        }		
        break;
    default:

        break;
    }
}

void BlackShopItemInfoPanel::doBuy() {
    if(m_bHasBought) {
        close();
        return;
    }
    if(vmsg::THING_TYPE_WEAPON == m_iProductType && P_ARM_OBJ->isWeaponBagFull())  {
        COMMON_DIALOG->show(TextStr(WEAPON_BAG_FULL));
        return;
    }

    if(vmsg::THING_TYPE_COIN == m_iGoldType) {

        if(P_TIP_CTRL->CoinCostTip(m_iGoldCnt, this)) {
            char tip[100] = {0};
            snprintf(tip, 100, TextStr(TEXT_CONFIRM_COIN_COST), m_iGoldCnt);
            COMMON_DIALOG->show(tip, this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(BlackShopItemInfoPanel::onBuy));
        } 
    } else {
		P_TIP_CTRL->GoldCostTip(m_iGoldCnt, this, dialogbtneventselector(BlackShopItemInfoPanel::onBuy));
    }
}

void BlackShopItemInfoPanel::onBuy() {
    
    P_SHOP_CTRL->getProxy()->sendBlackShopBuyRqst(m_iItemIdx);
    close();
    if(P_SHOP_CTRL->getBlackShopPanel()->isOpen()) {
        P_SHOP_CTRL->getBlackShopPanel()->showLoading();
    }
}

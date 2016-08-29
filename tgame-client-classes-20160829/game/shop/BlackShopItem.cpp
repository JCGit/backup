#include "BlackShopItem.h"
#include "ShopControl.h"
#include "ItemUtils.h"
#include "defines.h"
#include "UserObj.h"
#include "CCColorUtil.h"
#include "DebrisCfgMaster.h"
#include "IconCell.h"
#include "ToolUtils.h"



BlackShopItem::BlackShopItem()
{
    m_pRefLayout = NULL;
    m_pCellBg = NULL;
}

BlackShopItem::~BlackShopItem()
{
}


void BlackShopItem::initLayout(UILayout* pRefLayout) {
    if(NULL == pRefLayout) {
        return;
    }
    m_pRefLayout = pRefLayout;

    m_pRefLayout->addTouchEventListener(this, toucheventselector(BlackShopItem::touchEventHandler) );

    m_pNameTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("name_txt"));
    m_pCellBg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("cell_bg"));
    m_pGoldIcon = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("gold_icon"));
    m_pGoldTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("gold_txt"));

    m_pIconCell = IconCell::create();
    m_pIconCell->showNumTxt();
    m_pCellBg->addChild(m_pIconCell);
}


void BlackShopItem::touchEventHandler(CCObject *pSender, TouchEventType type){
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    switch (type) {
    case TOUCH_EVENT_ENDED:
        if(m_stItemInfo.hasbought()) {
            break;
        }
        P_SHOP_CTRL->getBlackShopItemInfoPanel()->open();
        P_SHOP_CTRL->getBlackShopItemInfoPanel()->setInfo(m_stItemInfo, m_iItemIndex);
        break;
    default:

        break;
    }
}

void BlackShopItem::setInfo(const CSBlackProductInfo& oneItem, const int iIdx) {
	
    bool bIsDebris = (vmsg::THING_TYPE_DEBRIS == oneItem.producttype() );

    CToolUtils::adjustIconBgSize(oneItem.producttype(), oneItem.productcfgid(), m_pCellBg);
    CToolUtils::adjustFragIcon(oneItem.producttype(), m_pCellBg);

    m_pIconCell->setInfo(oneItem.producttype(), oneItem.productcnt(), oneItem.productcfgid() );

    m_stItemInfo = oneItem;
    m_iItemIndex = iIdx;

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

    //ÊÇ·ñ¹ºÂò¹ý
    bool bIsBought = (oneItem.hasbought() > 0);
    UIImageView* pBoughtImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByTag(TAG_IMG_BOUGHT));
    if(bIsBought) {
        if(NULL == pBoughtImg) {
            pBoughtImg = UIImageView::create();
            pBoughtImg->loadTexture("other/icon_bought.png", UI_TEX_TYPE_PLIST);

            CCPoint pt = m_pCellBg->getPosition();
            pt.x = pt.x + m_pCellBg->getSize().width*0.5 - 25;
            pt.y = pt.y - m_pCellBg->getSize().height*0.5 + 10;
            pBoughtImg->setPosition(pt);

        }
        if(NULL == pBoughtImg->getParent()) {
            m_pRefLayout->addChild(pBoughtImg, 1, TAG_IMG_BOUGHT);
        }
    } else {
        if(pBoughtImg) {
            m_pRefLayout->removeChildByTag(TAG_IMG_BOUGHT, true);
        }
    }
}

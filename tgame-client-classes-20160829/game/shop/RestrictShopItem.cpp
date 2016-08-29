#include "RestrictShopItem.h"
#include "IconCell.h"
#include "ToolUtils.h"
#include "CCColorUtil.h"
#include "ItemUtils.h"
#include "UserObj.h"
#include "defines.h"
#include "ClientSvrMsg.pb.h"
#include "ShopControl.h"
#include "CommonDialog.h"
#include "TextHelper.h"

CRestrictShopItem::CRestrictShopItem(){

}

CRestrictShopItem::~CRestrictShopItem(){

}

void CRestrictShopItem::initLayout(UILayout* pRefLayout){
	if(NULL == pRefLayout) {
		return;
	}
	m_pRefLayout = pRefLayout;

	m_pRefLayout->addTouchEventListener(this, toucheventselector(CRestrictShopItem::touchEventHandler) );

	m_pNameTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("name_txt"));
	m_pCellBg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("cell_bg"));
	m_pGoldIcon = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("gold_icon"));
	m_pStateImg = dynamic_cast<UIImageView*>(m_pRefLayout->getChildByName("state_img"));
	m_pGoldTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("gold_txt"));
	m_pNumTxt = dynamic_cast<UILabel*>(m_pRefLayout->getChildByName("num_txt"));

	m_pIconCell = IconCell::create();
	m_pIconCell->showNumTxt();
	m_pCellBg->addChild(m_pIconCell);
}

void CRestrictShopItem::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(m_stItemInfo.mybuyingstatus()) {
			COMMON_DIALOG->show(TextStr(TEXT_RESTRICT_BUY_TIP));
			break;
		}

		if(m_stItemInfo.totalbuycnt() - m_stItemInfo.totalboughtcnt() <= 0){
			COMMON_DIALOG->show(TextStr(TEXT_RESTRICT_SELL_OUT_TIP));
			break;
		}

		P_SHOP_CTRL->getRestrictItemInfoPanel()->open();
		P_SHOP_CTRL->getRestrictItemInfoPanel()->setInfo(m_stItemInfo, m_iItemIndex);
		break;
	default:

		break;
	}
}

void CRestrictShopItem::setInfo(const CSPanicBuyingStoreItem& oneItem, const int iIdx){
	CToolUtils::adjustIconBgSize(oneItem.type(), oneItem.cfgid(), m_pCellBg);
	CToolUtils::adjustFragIcon(oneItem.type(), m_pCellBg);

	m_pIconCell->setInfo(oneItem.type(), oneItem.eachbuyingcnt(), oneItem.cfgid());

	m_stItemInfo = oneItem;
	m_iItemIndex = iIdx;

	m_pNameTxt->setText(ItemUtils::getItemName(oneItem.type(), oneItem.cfgid()));
	m_pNumTxt->setText(intToString(oneItem.totalbuycnt() - oneItem.totalboughtcnt()));

	bool bIsBought = (oneItem.mybuyingstatus() > 0);		//ÊÇ·ñ¹ºÂò¹ý
	bool bIsSellOut = ((oneItem.totalbuycnt() - oneItem.totalboughtcnt()) <= 0);

	if(bIsSellOut){
		CCColorUtil::setLabelColor(m_pNumTxt, ccRED);
	}else {
		CCColorUtil::setLabelColor(m_pNumTxt, ccWHITE);
	}

	bool bIsMoneyEnough = false;
	if(vmsg::THING_TYPE_COIN == oneItem.consumingtype()) {
		m_pGoldIcon->loadTexture("share/icon_coin.png", UI_TEX_TYPE_PLIST);
		bIsMoneyEnough = (0 == P_USER_OBJ->checkCoin(oneItem.price()) );
	} else {
		m_pGoldIcon->loadTexture("share/icon_gold.png", UI_TEX_TYPE_PLIST);
		bIsMoneyEnough = (0 == P_USER_OBJ->checkGold(oneItem.price()) );
	}
	if(bIsMoneyEnough) {
		CCColorUtil::setLabelColor(m_pGoldTxt, ccGREEN);
	} else {
		CCColorUtil::setLabelColor(m_pGoldTxt, ccRED);
	}
	m_pGoldTxt->setText(intToString(oneItem.price()));

	if(!bIsBought && !bIsSellOut){
		m_pStateImg->setVisible(false);
	}else{
		m_pStateImg->setVisible(true);

		if(bIsBought){
			m_pStateImg->loadTexture("other/icon_bought.png", UI_TEX_TYPE_PLIST);
		}else{
			if(bIsSellOut){
				m_pStateImg->loadTexture("other/sell_out_img.png", UI_TEX_TYPE_PLIST);
			}
		}
	}
}


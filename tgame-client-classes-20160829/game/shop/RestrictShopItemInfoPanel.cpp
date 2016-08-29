#include "RestrictShopItemInfoPanel.h"
#include "IconCell.h"
#include "ToolUtils.h"
#include "defines.h"
#include "CommonDialog.h"
#include "ClientSvrMsg.pb.h"
#include "UserObj.h"
#include "ItemUtils.h"
#include "CCColorUtil.h"
#include "TextHelper.h"
#include "NounHelper.h"
#include "ShopControl.h"
#include "tip/TipControl.h"
#include "firearms/FireArmsObj.h"

CRestrictShopItemInfoPanel::CRestrictShopItemInfoPanel(){
	m_bIsSellOut = false;
	m_bHasBought = false;
}

CRestrictShopItemInfoPanel::~CRestrictShopItemInfoPanel(){

}

bool CRestrictShopItemInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/black_shop_item_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CRestrictShopItemInfoPanel::touchEventHandler));

		m_pBuyBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("buy_btn"));
		m_pBuyBtn->addTouchEventListener(this, toucheventselector(CRestrictShopItemInfoPanel::touchEventHandler));

		m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
		m_pNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_txt"));
		m_pNumLabel = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_label"));
		m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));
		m_pGoldIcon = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("gold_icon"));
		m_pGoldTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("gold_txt"));

		m_pNumLabel->setText(NounStr(NOUN_STOCK_TXT));

		m_pCellBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_bg"));

		m_pIconCell = IconCell::create();
		m_pIconCell->showNumTxt();

		m_pCellBg->addChild(m_pIconCell);

		bRet = true;
	} while(0);

	return bRet;
}

void CRestrictShopItemInfoPanel::open() {
	BasePanel::open();
}

void CRestrictShopItemInfoPanel::close() {
	BasePanel::close();
	if(COMMON_DIALOG->isOpen()) {
		COMMON_DIALOG->close();
	}
}

void CRestrictShopItemInfoPanel::setInfo(const CSPanicBuyingStoreItem& oneItem, const int iIdx){
	if(!this->isOpen()) {
		return;
	}
	m_iItemIdx = iIdx;
	m_iGoldCnt = oneItem.price();
	m_iGoldType = oneItem.consumingtype();
	m_bHasBought = (oneItem.mybuyingstatus() > 0);
	m_bIsSellOut = (oneItem.totalbuycnt() - oneItem.totalboughtcnt() <= 0);
	m_iProductType = oneItem.type();

	CToolUtils::adjustIconBgSize(oneItem.type(), oneItem.cfgid(), m_pCellBg);
	CToolUtils::adjustFragIcon(oneItem.type(), m_pCellBg);

	m_pNumTxt->setText(intToString(oneItem.totalbuycnt() - oneItem.totalboughtcnt()));
	m_pIconCell->setInfo(oneItem.type(), oneItem.eachbuyingcnt(), oneItem.cfgid() );
	m_pNameTxt->setText(ItemUtils::getItemName(oneItem.type(), oneItem.cfgid()));

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
	m_pDescTxt->setText(ItemUtils::getItemDesc(oneItem.type(), oneItem.cfgid()));
}

void CRestrictShopItemInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
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

void CRestrictShopItemInfoPanel::doBuy() {
	if(m_bHasBought || m_bIsSellOut) {
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
			COMMON_DIALOG->show(tip, this, NounStr(OK), NounStr(CANCEL), dialogbtneventselector(CRestrictShopItemInfoPanel::onBuy));
		} 
	} else {
		P_TIP_CTRL->GoldCostTip(m_iGoldCnt, this, dialogbtneventselector(CRestrictShopItemInfoPanel::onBuy));
	}
}

void CRestrictShopItemInfoPanel::onBuy() {

	P_SHOP_CTRL->sendRestrictShopBuyRqst(m_iItemIdx);
	close();
	if(P_SHOP_CTRL->getRestrictItemInfoPanel()->isOpen()) {
		P_SHOP_CTRL->getRestrictItemInfoPanel()->showLoading();
	}
}


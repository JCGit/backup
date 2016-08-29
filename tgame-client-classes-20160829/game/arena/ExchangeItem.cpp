#include "ExchangeItem.h"
#include "ItemUtils.h"
#include "defines.h"
#include "ArenaControl.h"
#include "CCColorUtil.h"
#include "ToolUtils.h"

using namespace std;

CExchangeItem::CExchangeItem(){

}

CExchangeItem::~CExchangeItem(){

}

bool CExchangeItem::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/exchange_item_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pItemBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("item_bg"));
		m_pGetImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("get_img"));
		m_pCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_txt"));
        m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
		m_pGetImg->setVisible(false);

		m_pItem = IconCell::create();
		m_pItem->setZOrder(2);
		m_pItem->showNumTxt(true);
		m_pItem->setAnchorPoint(ccp(0.5f, 0.5f));
		m_pItemBg->addChild(m_pItem);

		m_pItemBg->setTouchEnabled(true);
		m_pItemBg->addTouchEventListener(this, toucheventselector(CExchangeItem::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void CExchangeItem::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "item_bg") == 0) {
			P_ARENA_CTRL->openExchangeInfoPanel(m_stInfo.itemid(), m_stInfo.itemtype(), m_stInfo.itemcnt(), m_uIdx, m_stInfo.arenamoney());
		}
		break;
	default:

		break;
	}
}

void CExchangeItem::setInfo(const CSArenaStroeItem& stInfo, unsigned int idx){
	m_stInfo = stInfo;
	m_uIdx = idx;

	updateUI();
}

void CExchangeItem::updateUI(){
	m_pItem->setInfo(m_stInfo.itemtype(), m_stInfo.itemcnt(), m_stInfo.itemid());
	m_pCoinTxt->setText(intToString(m_stInfo.arenamoney()));
    m_pNameTxt->setText(ItemUtils::getItemName(m_stInfo.itemtype(), m_stInfo.itemid()) );

    CToolUtils::adjustIconBgSize(m_stInfo.itemtype(), m_stInfo.itemid(), m_pItemBg);
    CToolUtils::adjustFragIcon(m_stInfo.itemtype(), m_pItemBg);

	if(m_stInfo.itembought()){
        //ÒÑ¾­¶Ò»»
		m_pItemBg->setTouchEnabled(false);
        m_pGetImg->setVisible(true);
	}else{
		m_pItemBg->setTouchEnabled(true);
        m_pGetImg->setVisible(false);
	}
}


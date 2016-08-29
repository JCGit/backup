#include "ExchangeInfoPanel.h"
#include "ArenaControl.h"
#include "ItemUtils.h"
#include "NounHelper.h"
#include "ToolUtils.h"
#include "CommonDialog.h"
#include "TextHelper.h"
#include "frag/FragControl.h"
#include "bag/BagControl.h"
#include "defines.h"
#include "ItemCfgMaster.h"

using namespace std;

CExchangeInfoPanel::CExchangeInfoPanel(){

}

CExchangeInfoPanel::~CExchangeInfoPanel(){

}

bool CExchangeInfoPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/exchange_info_panel_ui.ExportJson");
		addChild(m_pRootLayout);

		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pExchangeBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("exchange_btn"));

		m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
		m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));
        m_pBagNumTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("bag_num_txt"));
        m_pArenaCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_txt"));

		m_pIconBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("icon_bg"));
		m_pItem = IconCell::create();
		m_pItem->setZOrder(2);
		m_pItem->showNumTxt(true);
		m_pIconBg->addChild(m_pItem);

		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CExchangeInfoPanel::touchEventHandler));
		m_pExchangeBtn->addTouchEventListener(this, toucheventselector(CExchangeInfoPanel::touchEventHandler));

		bRet = true;
	} while (0);

	return bRet;
}

void CExchangeInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();			
		}else if(strcmp(pszWidgetName, "exchange_btn") == 0) {
			if(m_uCoin > P_ARENA_CTRL->getArenaObj()->getArenaInfo().arenamoney()){  //¹¦Ñ«²»×ã
				COMMON_DIALOG->show(TextStr(ARENA_COIN_NOT_ENOUGH_TXT))	;
				return;
			}
			P_ARENA_CTRL->sendStoreBuyRqst(m_uIdx, m_uCfgID);
			this->close();
		}
		break;
	default:

		break;
	}
}

void CExchangeInfoPanel::setInfo(unsigned int cfgID, unsigned int cfgType, unsigned int cfgCnt, unsigned int idx, unsigned int coin){
	m_uCfgID = cfgID;
	m_uCfgType = cfgType;
	m_uCfgCnt = cfgCnt;
	m_uIdx = idx;
	m_uCoin = coin;

	updateUI();
}

void CExchangeInfoPanel::updateUI(){
	m_pItem->setInfo(m_uCfgType, m_uCfgCnt, m_uCfgID);
	m_pNameTxt->setText(ItemUtils::getItemName(m_uCfgType, m_uCfgID));

	CToolUtils::adjustIconBgSize(m_uCfgType, m_uCfgID, m_pIconBg);
	CToolUtils::adjustFragIcon(m_uCfgType, m_pIconBg);

	string descStr = ItemUtils::getItemDesc(m_uCfgType, m_uCfgID);
    m_pDescTxt->setText(descStr);

    m_pArenaCoinTxt->setText(intToString(m_uCoin));
    if(vmsg::THING_TYPE_ITEM == m_uCfgType || vmsg::THING_TYPE_DEBRIS == m_uCfgType) {
        int iNum = 0;
		if(vmsg::THING_TYPE_ITEM == m_uCfgType){
			iNum = P_BAG_CTRL->getItemCnt(m_uCfgID);
		}else{
			iNum = P_FRAG_CTRL->getNumByCfgID(m_uCfgID);
		}
        char str[50] = {0};
        snprintf(str, 50, TextStr(ARENA_EXCHANGE_NUM_TIP), iNum );

        m_pBagNumTxt->setText(str);
    } else {
        m_pBagNumTxt->setText("");
    }
}
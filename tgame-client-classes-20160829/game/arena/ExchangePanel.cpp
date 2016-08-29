#include "ExchangePanel.h"
#include "ExchangeItem.h"
#include "ArenaControl.h"
#include "defines.h"
#include "TimeEx.h"
#include "GlobalTime.h"
#include "Constant.h"
#include "TextHelper.h"
#include "tip/TipControl.h"
#include "game/UserObj.h"

#include "GlobalCfgMaster.h"
#include "VIPLvCfgMaster.h"

using namespace vmsg;
using namespace std;

#define MAX_STORE_NUM 6
#define COL_CELL_NUM 3

CExchangePanel::CExchangePanel(){
	m_stItemLst = NULL;
	m_uTimeCount = 0;
}

CExchangePanel::~CExchangePanel(){
	CC_SAFE_RELEASE_NULL(m_stItemLst);
}

bool CExchangePanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/exchange_panel_ui.ExportJson");
		addChild(m_pRootLayout);

        setUseBackForClose(false);
        setShowMoneyTopBar(true);

		m_pRefreshBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("refresh_btn"));
		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));

		m_pTimeTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("time_txt"));
		m_pCoinTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("coin_txt"));
		
		m_pItemLayout = dynamic_cast<UIPanel*>(m_pRootLayout->getChildByName("item_layout"));

		m_pRefreshBtn->addTouchEventListener(this, toucheventselector(CExchangePanel::touchEventHandler));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(CExchangePanel::touchEventHandler));

		m_stItemLst = CCArray::create();
		m_stItemLst->retain();
        /*
		for(unsigned int i = 0; i < MAX_STORE_NUM; i++){
			CExchangeItem* pItem = CExchangeItem::create();
			string itemName = "item_" + intToString(i);
			pItem->setName(itemName.c_str());
			float posX = (i%COL_CELL_NUM)*(1 + pItem->getSize().width);
			float posY = 240 - (i/COL_CELL_NUM)*(10 + pItem->getSize().height);
			pItem->setPosition(ccp(posX, posY));
			m_pItemLayout->addChild(pItem);

			m_stItemLst->addObject(pItem);
		}
        */
		bRet = true;
	} while (0);

	return bRet;
}

void CExchangePanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();			
		}else if(strcmp(pszWidgetName, "refresh_btn") == 0) {
			const CSArenaStroeInfo& stStoreInfo = P_ARENA_CTRL->getArenaObj()->getStoreInfo();

			if(stStoreInfo.refreshcnt() >= stStoreInfo.maxrefreshcnt()){
				P_TIP_CTRL->getPromptPayPanel()->openWithVIPModule(vmsg::VIP_MODULE_ARENASTORERESET);
			} else {
				P_TIP_CTRL->GoldCostTip(ARENA_STORE_REFRESH_GOLD, this, dialogbtneventselector(CExchangePanel::refreshFun), TextStr(TEXT_REFRESH_COST_TIP));
			}
		}		
		break;
	default:

		break;
	}
}

void CExchangePanel::refreshFun(){
	P_ARENA_CTRL->sendStoreRefreshRqst();	
}

void CExchangePanel::updateUI(){
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const CSArenaStroeInfo& stStoreInfo = P_ARENA_CTRL->getArenaObj()->getStoreInfo();

	//É¾³ý¶àÓàµÄÔªËØ
	while(int(m_stItemLst->count()) > (unsigned int)stStoreInfo.itemlst_size() ){
		CExchangeItem* pItem = dynamic_cast<CExchangeItem*>(m_stItemLst->lastObject());
		m_stItemLst->removeLastObject();
		pItem->removeFromParent();
	}

	CExchangeItem* pItem = NULL;
	for(int i = 0; i < stStoreInfo.itemlst_size(); i++){
		pItem = dynamic_cast<CExchangeItem*>(m_stItemLst->objectAtIndex(i));
		if(pItem == NULL){
			pItem = CExchangeItem::create();
            pItem->setAnchorPoint(ccp(0.0f, 0.0f));
			string itemName = "item_" + intToString(i);
			pItem->setName(itemName.c_str());

            float posX = (i%COL_CELL_NUM)*(30 + pItem->getSize().width);
            float posY = 200 - (i/COL_CELL_NUM)*(10 + pItem->getSize().height);

			pItem->setPosition(ccp(posX, posY));
			pItem->setTouchEnabled(true);
			m_pItemLayout->addChild(pItem);

			pItem->addTouchEventListener(this, toucheventselector(CExchangePanel::touchEventHandler));

			m_stItemLst->addObject(pItem);
		}
		pItem->setInfo(stStoreInfo.itemlst(i), i);
	}

	m_pCoinTxt->setText(intToString(stStoreInfo.arenamoney()));

	unsigned int refreshTime = GLOBAL_CFG_MASTER->GetCfg().arenastorerefreshtime();
	m_uTimeCount = refreshTime - (GlobalTime::getTime() - stStoreInfo.lastrefreshtime());
	m_pTimeTxt->setText(TimeEx::getStringTimeForInt(m_uTimeCount));

    this->unschedule(schedule_selector(CExchangePanel::timeCountDown));
	this->schedule(schedule_selector(CExchangePanel::timeCountDown), 1.0f);
}

void CExchangePanel::timeCountDown(float dt){
	if(m_uTimeCount <= 0){
		this->unschedule(schedule_selector(CExchangePanel::timeCountDown));
		P_ARENA_CTRL->sendStoreQueryRqst();
		return;
	}

	m_uTimeCount--;
	m_pTimeTxt->setText(TimeEx::getStringTimeForInt(m_uTimeCount));
}

void CExchangePanel::open(){
	BasePanel::open();

	P_ARENA_CTRL->sendStoreQueryRqst();

	this->showLoading();
}

void CExchangePanel::close(){
	BasePanel::close();

	this->unschedule(schedule_selector(CExchangePanel::timeCountDown));
}
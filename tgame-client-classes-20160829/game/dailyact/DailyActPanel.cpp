#include "DailyActPanel.h"
#include "DailyActListItem.h"
#include "SvrActListItem.h"
#include "DailyActControl.h"

DailyActPanel::DailyActPanel()
:m_pItemDemo(NULL)
,m_stDailyActItems(NULL)
{
	
}

DailyActPanel::~DailyActPanel()
{
	CC_SAFE_RELEASE_NULL(m_pItemDemo);

	if(m_stDailyActItems){
		m_stDailyActItems->removeAllObjects();
		CC_SAFE_RELEASE_NULL(m_stDailyActItems);
	}
}

bool DailyActPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/daily_act_ui.ExportJson");
		addChild(m_pRootLayout);
		
		m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(DailyActPanel::touchEventHandler));

		m_pActList = dynamic_cast<ListView*>(m_pRootLayout->getChildByName("act_list"));

		DailyActListItem* pDemo = DailyActListItem::create();
		m_pItemDemo = pDemo->getRootLayout();
		m_pItemDemo->retain();

		m_stDailyActItems = CCArray::create();
		m_stDailyActItems->retain();

		bRet = true;
	} while(0);

	return bRet;
}

void DailyActPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, "close_btn") == 0) {
				close();
			} 
			break;
		default:
			break;
	}
}


void DailyActPanel::open() {
    BasePanel::open();

	P_DAILYACT_CTRL->getProxy()->sendQueryDailyAct();
	this->showLoading();
}

void DailyActPanel::close(){
	BasePanel::close();

	this->closeLoading();

	m_pActList->removeAllItems();
	m_stDailyActItems->removeAllObjects();
}

void DailyActPanel::updateUI() {
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const CSActQueryRsp& stMsg = P_DAILYACT_CTRL->getDailyActInfo();

	m_pActList->removeAllItems();
	m_stDailyActItems->removeAllObjects();

	DailyActListItem* pItem;
	for(int idx = 0; idx < stMsg.actinfos_size(); idx++){
		pItem = DailyActListItem::create(m_pItemDemo);
		m_pActList->pushBackCustomItem(pItem);
		pItem->setActInfo(stMsg.actinfos(idx));
		m_stDailyActItems->addObject(pItem);
	}
}

void DailyActPanel::updateOneItem(const CSActStatChange& stInfo){
	const CSActQueryRsp& stMsg = P_DAILYACT_CTRL->getDailyActInfo();

	DailyActListItem* pItem;
	for(unsigned int idx = 0; idx < m_stDailyActItems->count(); idx++){
		pItem = dynamic_cast<DailyActListItem*>(m_stDailyActItems->objectAtIndex(idx));
		if(pItem && pItem->getActInfo().actid() == stInfo.actid()){
			CSActStatChange stActInfo;
			pItem->updateStat(stInfo.actstat());
		}
	}
}

void DailyActPanel::updateWorldBossActItem(unsigned int uMark){
	DailyActListItem* pItem;
	for(unsigned int idx = 0; idx < m_stDailyActItems->count(); idx++){
		pItem = dynamic_cast<DailyActListItem*>(m_stDailyActItems->objectAtIndex(idx));
		if(pItem && pItem->getActInfo().actid() == ACT_ID_WORLD_BOSS){
			pItem->updateWorldBossTishen(uMark);
			return;
		}
	}
}


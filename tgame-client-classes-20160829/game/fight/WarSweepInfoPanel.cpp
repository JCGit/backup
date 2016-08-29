#include "WarSweepInfoPanel.h"

#include "WarSweepInfoItem.h"
#include "LevelObj.h"
#include "LevelControl.h"

WarSweepInfoPanel::WarSweepInfoPanel()
:m_pCloseBtn(NULL)
,m_pSweepInfoListView(NULL)
,m_pSweepStopTipTxt(NULL)
,m_nSweepIdx(0)
{
}

WarSweepInfoPanel::~WarSweepInfoPanel()
{
}

WarSweepInfoPanel* WarSweepInfoPanel::create() {
	WarSweepInfoPanel *pRet = new WarSweepInfoPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool WarSweepInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setClickOutToClose(false);
		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void WarSweepInfoPanel::close() {
	BasePanel::close();
	m_pSweepStopTipTxt->setVisible(false);
	m_pSweepInfoListView->removeAllItems();
}

void WarSweepInfoPanel::updateUI() {
	m_pCloseBtn->setEnabled(false);
	schedule(schedule_selector(WarSweepInfoPanel::sweepUpdate), 0.8f);
}

void WarSweepInfoPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/EliteSweepInfoPanelUI.ExportJson");
	addChild(m_pRootLayout);

	m_pCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(WarSweepInfoPanel::touchEventHandler));

	m_pSweepInfoListView = dynamic_cast<UIListView*>(m_pRootLayout->getChildByName("sweep_info_list"));

	m_pSweepStopTipTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("bag_full_tip_txt"));
	m_pSweepStopTipTxt->setVisible(false);
}

void WarSweepInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pCloseBtn->getName()) == 0) {
				close();
			}
			break;
		default:;
	}
}

void WarSweepInfoPanel::sweepUpdate(float dt) {
	const vmsg::CSSweepEliteWarRsp &stEliteSweepInfo = P_LEVEL_OBJ->getEliteSweepInfo();
	
	if(m_nSweepIdx >= stEliteSweepInfo.awardlist_size()) { // 扫荡完成
		const vmsg::CSEliteAward &stFirstAwardInfo = stEliteSweepInfo.awardlist(0);
		int nStartLevel = stFirstAwardInfo.elitewarmapid() % 5 == 0 ? 5 : stFirstAwardInfo.elitewarmapid() % 5;
		m_pSweepStopTipTxt->setVisible(stEliteSweepInfo.awardlist_size() <= (5 - nStartLevel)); // 为true时，背包已满，还有没有扫荡的关卡
		unschedule(schedule_selector(WarSweepInfoPanel::sweepUpdate));
		m_nSweepIdx = 0;
		m_pCloseBtn->setEnabled(true);
	} else {
		const vmsg::CSEliteAward &stAwardInfo = stEliteSweepInfo.awardlist(m_nSweepIdx);

		WarSweepInfoItem *pItem = WarSweepInfoItem::create();
		m_pSweepInfoListView->pushBackCustomItem(pItem);
		pItem->updateUI(stAwardInfo);
		m_pSweepInfoListView->sortAllChildren();
		m_pSweepInfoListView->scrollToBottom(0.5f, true);

		m_nSweepIdx++;
	}
}

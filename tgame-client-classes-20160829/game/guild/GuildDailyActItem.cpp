#include "GuildDailyActItem.h"

GuildDailyActItem::GuildDailyActItem()
:m_pActNameTxt(NULL)
,m_pRemainTimesValTxt(NULL)
,m_pGotoBtn(NULL)
,m_bSelected(NULL)
{
}

GuildDailyActItem::~GuildDailyActItem()
{
}

GuildDailyActItem* GuildDailyActItem::create() {
	GuildDailyActItem *pRet = new GuildDailyActItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildDailyActItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/GuildDailyActItemUI.ExportJson");
		addChild(m_pRootLayout);

		m_pActNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_name_txt"));

		m_pRemainTimesValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("remain_times_val_txt"));

		m_pGotoBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("goto_btn"));
		m_pGotoBtn->addTouchEventListener(this, toucheventselector(GuildDailyActItem::touchEventHandler));

		selected(false);

		bRet = true;
	} while(0);

	return bRet;
}

void GuildDailyActItem::updateUI() {

}

bool GuildDailyActItem::isSelected() {
	return m_bSelected;
}

void GuildDailyActItem::selected(bool bSelected) {
	m_bSelected = bSelected;


}

void GuildDailyActItem::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pGotoBtn->getName()) == 0) {

			}
			break;
		default:;
	}
}

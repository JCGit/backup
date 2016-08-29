#include "GuildJoinReplyItem.h"

#include "defines.h"

GuildJoinReplyItem::GuildJoinReplyItem()
{
}

GuildJoinReplyItem::~GuildJoinReplyItem()
{
}

GuildJoinReplyItem* GuildJoinReplyItem::create() {
	GuildJoinReplyItem *pRet = new GuildJoinReplyItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildJoinReplyItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

bool GuildJoinReplyItem::isSelected() {
	return m_bSelected;
}

void GuildJoinReplyItem::selected(bool bSelected) {
	m_bSelected = bSelected;

	m_pCheckboxBtn->setSelectedState(m_bSelected);
}

void GuildJoinReplyItem::setInfo(const vmsg::CSGuildReq &stGuildRqst) {
	m_pNameTxt->setText(stGuildRqst.name());
	m_pLevelTxt->setText(string("Lv.") + intToString(stGuildRqst.level()));
}

void GuildJoinReplyItem::initUI() {
	initLayoutFromJsonFile("ui_v2/GuildJoinReplyItemUI.ExportJson");
	addChild(m_pRootLayout);

	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

	m_pLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));

	m_pCheckboxBtn = dynamic_cast<UICheckBox*>(m_pRootLayout->getChildByName("checkbox_btn"));
}

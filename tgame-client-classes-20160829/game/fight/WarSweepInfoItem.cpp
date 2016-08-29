#include "WarSweepInfoItem.h"

#include "defines.h"

#include "basic/ItemUtils.h"
#include "basic/TextHelper.h"

WarSweepInfoItem::WarSweepInfoItem()
{
}

WarSweepInfoItem::~WarSweepInfoItem()
{
}

WarSweepInfoItem* WarSweepInfoItem::create() {
	WarSweepInfoItem *pRet = new WarSweepInfoItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool WarSweepInfoItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/EliteSweepInfoItemUI.ExportJson");
		addChild(m_pRootLayout);

		m_pWarMapNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("war_map_name_txt"));

		m_pAwardDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_desc_txt"));

		bRet = true;
	} while(0);

	return bRet;
}

void WarSweepInfoItem::updateUI(const vmsg::CSEliteAward &stAwardInfo) {
	char text[100];
	snprintf(text, 100, TextStr(SWEEP_WAR), stAwardInfo.elitewarmapid() % 5 == 0 ? 5 : stAwardInfo.elitewarmapid() % 5);
	m_pWarMapNameTxt->setText(text);

	string desc = "";

	for(int i = 0; i < stAwardInfo.award_size(); i++) {
		const vmsg::CSAwardElement &stAwardElmt = stAwardInfo.award(i);
		desc += ItemUtils::getItemName(stAwardElmt.elementtype(), stAwardElmt.elementid()) + " x" + intToString(stAwardElmt.elementcnt()) + "    ";
	}

	m_pAwardDescTxt->setText(desc);
}

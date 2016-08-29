#include "GuildDailyActCell.h"


GuildDailyActCell::GuildDailyActCell()
:m_pActTitleTxt(NULL)
,m_pRemainTimesTxt(NULL)
,m_pRemainTimesValTxt(NULL)
,m_pAwardTxt(NULL)
,m_pAwardValTxt(NULL)
,m_pBgSelectedImg(NULL)
,m_bSelected(false)
{
}

GuildDailyActCell::~GuildDailyActCell()
{
}

GuildDailyActCell* GuildDailyActCell::create() {
	GuildDailyActCell *pRet = new GuildDailyActCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildDailyActCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui/GuildDailyActCellUI.json");
		addChild(m_pRootLayout);

		m_pActTitleTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_title_txt"));

		m_pRemainTimesTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("times_txt"));

		m_pRemainTimesValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("times_val_txt"));

		m_pAwardTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_txt"));

		m_pAwardValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("award_val_txt"));

		m_pBgSelectedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_selected_img"));

		selected(true);
		selected(false);

		bRet = true;
	} while(0);

	return bRet;
}

void GuildDailyActCell::updateUI() {

}

bool GuildDailyActCell::isSelected() {
	return m_bSelected;
}

void GuildDailyActCell::selected(bool bSelected) {
	if(m_bSelected != bSelected) {
		m_bSelected = bSelected;
		m_pBgSelectedImg->setVisible(m_bSelected);
	}
}

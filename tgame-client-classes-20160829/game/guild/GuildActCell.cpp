#include "GuildActCell.h"


GuildActCell::GuildActCell()
:m_pBgSelectedImg(NULL)
,m_pIconImg(NULL)
,m_pActNameTxt(NULL)
,m_bSelected(false)
{
}

GuildActCell::~GuildActCell()
{
}

GuildActCell* GuildActCell::create() {
	GuildActCell *pRet = new GuildActCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool GuildActCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui/GuildActCellUI.json");
		addChild(m_pRootLayout);

		m_pBgSelectedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_selected_img"));

		m_pIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_cell_img"));

		m_pActNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("act_name_txt"));

		selected(true);
		selected(false);

		bRet = true;
	} while(0);

	return bRet;
}

void GuildActCell::updateUI() {

}

bool GuildActCell::isSelected() {
	return m_bSelected;
}

void GuildActCell::selected(bool bSelected) {
	if(m_bSelected != bSelected) {
		m_bSelected = bSelected;
		m_pBgSelectedImg->setVisible(m_bSelected);
	}
}

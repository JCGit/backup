#include "PartnerListItem.h"

PartnerListItem::PartnerListItem()
:m_pBgSelectedImg(NULL)
,m_pNameTxt(NULL)
,m_pLevelTxt(NULL)
,m_bSelected(false)
{
}

PartnerListItem::~PartnerListItem()
{
}

PartnerListItem* PartnerListItem::create() {
	PartnerListItem *pRet = new PartnerListItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool PartnerListItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui/PartnerListItemUI.json");
		addChild(m_pRootLayout);

		m_pBgSelectedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_selected_img"));

		m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));

		m_pLevelTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("level_txt"));

		selected(true);
		selected(false);

		bRet = true;
	} while(0);

	return bRet;
}

void PartnerListItem::updateUI() {
	/*m_pNameTxt->setText("");
	m_pLevelTxt->setText("");*/
}

bool PartnerListItem::isSelected() {
	return m_bSelected;
}

void PartnerListItem::selected(bool bSelected) {
	if(m_bSelected != bSelected) {
		m_bSelected = bSelected;
		m_pBgSelectedImg->setVisible(m_bSelected);
	}
}

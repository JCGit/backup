#include "CategoryListItem.h"

CategoryListItem::CategoryListItem()
:m_pBgSelectedImg(NULL)
,m_pCategoryTxt(NULL)
,m_bSelected(false)
,m_bHotFlag(false)
,m_uType(0)
,m_uDetailType(0)
{
}

CategoryListItem::~CategoryListItem()
{
}

CategoryListItem* CategoryListItem::create(UILayout *pRefLayout) {
	CategoryListItem *pRet = new CategoryListItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CategoryListItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		m_pBgSelectedImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_selected_img"));
        m_pBgNormalImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_normal_img")); //bg_normal_img

		m_pCategoryTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("category_txt"));

		bRet = true;
	} while(0);

	return bRet;
}

bool CategoryListItem::isSelected() {
	return m_bSelected;
}

void CategoryListItem::selected(bool bSelected) {
	m_bSelected = bSelected;

	m_pBgSelectedImg->setVisible(m_bSelected);
    m_pBgNormalImg->setVisible(!m_bSelected);
}

void CategoryListItem::setCategory(const std::string &category) {
	m_pCategoryTxt->setText(category);
}

bool CategoryListItem::getHotFlag() {
	return m_bHotFlag;
}

void CategoryListItem::setHotFlag(bool bFlag) {
	m_bHotFlag = bFlag;
}

unsigned int CategoryListItem::getType() {
	return m_uType;
}

void CategoryListItem::setType(unsigned int uType) {
	m_uType = uType;
}

unsigned int CategoryListItem::getDetailType() {
	return m_uDetailType;
}

void CategoryListItem::setDetailType(unsigned int uDetailType) {
	m_uDetailType = uDetailType;
}

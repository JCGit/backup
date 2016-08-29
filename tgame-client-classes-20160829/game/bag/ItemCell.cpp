#include "ItemCell.h"

#include "CfgMaster/ItemCfgMaster.h"
#include "CfgMaster/EquipmentCfgMaster.h"

ItemCell::ItemCell()
:m_pDefaultImg(NULL)
,m_pItemImg(NULL)
{
}

ItemCell::~ItemCell()
{
	
}

ItemCell* ItemCell::create() {
	ItemCell *pRet = new ItemCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

ItemCell* ItemCell::create(UILayout *pRefLayout) {
	ItemCell *pRet = new ItemCell();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool ItemCell::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);
		
		m_pDefaultImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("default_icon_img"));

		m_pItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_icon_img"));

		bRet = true;
	} while(0);

	return bRet;
}

bool ItemCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		m_pDefaultImg = UIImageView::create();
		addChild(m_pDefaultImg);

		m_pItemImg = UIImageView::create();
		addChild(m_pItemImg);
		
		bRet = true;
	} while(0);

	return bRet;
}

std::string ItemCell::getDescription() const {
	return "ItemCell";
}

UIImageView* ItemCell::getDefaultImg() {
	return m_pDefaultImg;
}

UIImageView* ItemCell::getItemImg() {
	return m_pItemImg;
}

const vmsg::CSItemInfo& ItemCell::getItemInfo() {
	return m_stItemInfo;
}

void ItemCell::updateItemInfo(const vmsg::CSItemInfo& stItemInfo) {
	m_stItemInfo = stItemInfo;
	
	std::string strIconURL = "item/";
	strIconURL.append(ITEM_CFG_MASTER->GetItemCfgByID(m_stItemInfo.itemcfgid())->itemicon());
	strIconURL.append(".png");

	m_pDefaultImg->setVisible(false);
	m_pItemImg->setVisible(true);
	m_pItemImg->loadTexture(strIconURL.c_str());
}

const vmsg::CSEquipInfo& ItemCell::getEquipInfo() {
	return m_stEquipInfo;
}

void ItemCell::updateEquipInfo(const vmsg::CSEquipInfo& stEquipInfo) {
	m_stEquipInfo = stEquipInfo;

	std::string strIconURL = "item/";
	strIconURL.append(EQUIPMENT_CFG_MASTER->GetEquipCfgByID(m_stEquipInfo.equipid())->icon());
	strIconURL.append(".png");

	m_pDefaultImg->setVisible(false);
	m_pItemImg->setVisible(true);
	m_pItemImg->loadTexture(strIconURL.c_str());
}

void ItemCell::clearItem() {
	if(m_pDefaultImg != NULL) {
		m_pDefaultImg->setVisible(true);
	}

	m_pItemImg->setVisible(false);
}

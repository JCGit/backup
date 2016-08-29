#include "DropItem.h"

#include "ClientSvrMsg.pb.h"

#include "CfgMaster/ItemDropTipCfgMaster.h"

#include "RefitMaterialCell.h"

#define CHILD_CELL_TAG 101

DropItem::DropItem()
:m_pItemImg(NULL)
,m_pDescTxt(NULL)
,m_uWarType(0)
,m_uWarID(0)
{
}

DropItem::~DropItem()
{
}

DropItem* DropItem::create() {
	DropItem *pRet = new DropItem();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool DropItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/DropItemUI.ExportJson");
		addChild(m_pRootLayout);

		m_pItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_img"));
		RefitMaterialCell *pCell = RefitMaterialCell::create();
		pCell->setStat(RefitMaterialCell::CELL_STAT_UNKNOWN);
		m_pItemImg->addChild(pCell, 0, CHILD_CELL_TAG);

		m_pDescTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("desc_txt"));

		bRet = true;
	} while(0);

	return bRet;
}

void DropItem::setInfo(unsigned int uThingType, unsigned int uThingID, unsigned int uWarType, unsigned int uWarID) {
	m_uWarType = uWarType;
	m_uWarID = uWarID;
	
	RefitMaterialCell *pCell = dynamic_cast<RefitMaterialCell*>(m_pItemImg->getChildByTag(CHILD_CELL_TAG));
	pCell->setItemID(uThingID);
	
	string desc = ITEM_DROP_TIP_MASTER->getWarName(uWarType, uWarID);
	m_pDescTxt->setText(desc);
}

unsigned int DropItem::getWarType() {
	return m_uWarType;
}

unsigned int DropItem::getWarID() {
	return m_uWarID;
}

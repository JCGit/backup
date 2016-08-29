#include "RefitMaterialCell.h"

#include "defines.h"
#include "Resources.h"

#include "ClientSvrMsg.pb.h"

#include "CfgMaster/DotaItemCfgMaster.h"

#include "basic/CCColorUtil.h"

RefitMaterialCell::RefitMaterialCell()
:m_stat(CELL_STAT_UNKNOWN)
,m_uItemID(0)
,m_pIconImg(NULL)
,m_pFrameImg(NULL)
,m_pIconAddImg(NULL)
{
}

RefitMaterialCell::~RefitMaterialCell()
{
}

RefitMaterialCell* RefitMaterialCell::create() {
	RefitMaterialCell *pRet = new RefitMaterialCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RefitMaterialCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!Widget::init());

		ignoreContentAdaptWithSize(false);

		m_pIconImg = UIImageView::create();
		m_pIconImg->loadTexture("share/bg_item_cell.png", UI_TEX_TYPE_PLIST);
		addChild(m_pIconImg);

		m_pFrameImg = UIImageView::create();
		m_pFrameImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
		addChild(m_pFrameImg);

		m_pIconAddImg = UIImageView::create();
		m_pIconAddImg->loadTexture("share/icon_add.png", UI_TEX_TYPE_PLIST);
		addChild(m_pIconAddImg);

		setSize(m_pFrameImg->getSize());
		setStat(m_stat);

		bRet = true;
	} while(0);

	return bRet;
}

void RefitMaterialCell::setSize(const CCSize &size) { // setScale会导致触摸事件不准确，故作此处理
	if(_size.equals(CCSizeZero)) {
		Widget::setSize(size);
	} else {
		float scaleX = size.width / _size.width;
		float scaleY = size.height / _size.height;
		Widget::setSize(size);
		m_pIconImg->setScaleX(scaleX);
		m_pIconImg->setScaleY(scaleY);
		m_pFrameImg->setScaleX(scaleX);
		m_pFrameImg->setScaleY(scaleY);
		m_pIconAddImg->setScaleX(scaleX);
		m_pIconAddImg->setScaleY(scaleY);
	}
}

unsigned int RefitMaterialCell::getItemID() {
	return m_uItemID;
}

void RefitMaterialCell::setItemID(unsigned int uDotaItemID) {
	m_uItemID = uDotaItemID;

	const DotaItemCfg *pDotaItemCfg = DOTA_ITEM_CFG_MASTER->getDotaItemCfg(uDotaItemID);

	if(pDotaItemCfg != NULL) {
		string iconURL = ITEM_RES_PATH + pDotaItemCfg->dotaitemicon() + ".png";
		m_pIconImg->loadTexture(iconURL.c_str());
	} else {
		m_pIconImg->loadTexture("share/bg_item_cell.png", UI_TEX_TYPE_PLIST);
	}
	
}

unsigned int RefitMaterialCell::getStat() {
	return m_stat;
}

void RefitMaterialCell::setStat(CellStat stat) {
	m_stat = stat;

	// icon
	if(m_stat == CELL_STAT_UNKNOWN) {
		m_pIconImg->loadTexture("share/bg_item_cell.png", UI_TEX_TYPE_PLIST);
	} else {
		setItemID(m_uItemID);
	}

	if(m_stat == CELL_STAT_EMPTY || m_stat == CELL_STAT_ADD) {
		CCColorUtil::addGray(m_pIconImg->getVirtualRenderer());
	} else {
		CCColorUtil::removeGray(m_pIconImg->getVirtualRenderer());
	}

	// frame
	if(m_stat == CELL_STAT_ADDED) {
		m_pFrameImg->loadTexture("share/item_golden_frame.png", UI_TEX_TYPE_PLIST);
	} else {
		m_pFrameImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
	}

	m_pIconAddImg->setEnabled(m_stat == CELL_STAT_ADD);

}

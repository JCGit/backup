#include "IconFrameCell.h"

#include "defines.h"

#include "ClientSvrMsg.pb.h"

#include "CfgMaster/DebrisCfgMaster.h"

#include "basic/ItemUtils.h"

IconFrameCell::IconFrameCell()
:m_pIconImg(NULL)
,m_pFrameImg(NULL)
,m_pFlagImg(NULL)
,m_pCntTxt(NULL)
,m_uType(0)
,m_uCfgID(0)
,m_uCnt(0)
{
}

IconFrameCell::~IconFrameCell()
{
}

IconFrameCell* IconFrameCell::create() {
	IconFrameCell *pRet = new IconFrameCell();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool IconFrameCell::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!UIWidget::init());

		ignoreContentAdaptWithSize(false);

		m_pIconImg = UIImageView::create();
		m_pIconImg->loadTexture("share/bg_item_cell.png", UI_TEX_TYPE_PLIST);
		addChild(m_pIconImg);

		m_pFrameImg = UIImageView::create();
		m_pFrameImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
		addChild(m_pFrameImg);

		setSize(m_pFrameImg->getSize());

		bRet = true;
	} while(0);

	return bRet;
}

UIImageView* IconFrameCell::getIconImg() {
	return m_pIconImg;
}

UIImageView* IconFrameCell::getFrameImg() {
	return m_pFrameImg;
}

unsigned int IconFrameCell::getType() {
	return m_uType;
}

unsigned int IconFrameCell::getCfgID() {
	return m_uCfgID;
}

unsigned int IconFrameCell::getCnt() {
	return m_uCnt;
}

void IconFrameCell::setInfo(unsigned int uType, unsigned int uCfgID, unsigned int uCnt /* = 0 */) {
	m_uType = uType;
	m_uCfgID = uCfgID;
	m_uCnt = uCnt;

	string url = ItemUtils::getItemIcon(uType, uCfgID);
	m_pIconImg->loadTexture(url.c_str());

	unsigned int uFrameType = uType;

	if(uType == vmsg::THING_TYPE_DEBRIS) {
		const DebrisCfg *pDebrisCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(uCfgID);
		uFrameType = pDebrisCfg != NULL ? pDebrisCfg->composetype() : uType;
	}

	if(uFrameType == vmsg::THING_TYPE_WEAPON) {
		m_pFrameImg->loadTexture("share/weapon_frame.png", UI_TEX_TYPE_PLIST);
		setSize(m_pFrameImg->getSize());
	} else {
		unsigned int uColor = ItemUtils::getItemColor(uType, uCfgID);

		switch(uColor) {
			case vmsg::EQUIP_GRADE_WHITE:
				m_pFrameImg->loadTexture("share/item_frame_white.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_GREEN:
				m_pFrameImg->loadTexture("share/item_frame_green.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_BLUE:
				m_pFrameImg->loadTexture("share/item_frame_blue.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_PURPLE:
				m_pFrameImg->loadTexture("share/item_frame_purple.png", UI_TEX_TYPE_PLIST);
				break;
			case vmsg::EQUIP_GRADE_ORANGE:
				m_pFrameImg->loadTexture("share/item_frame_orange.png", UI_TEX_TYPE_PLIST);
				break;
			default:
				m_pFrameImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
		}

		setSize(m_pFrameImg->getSize());
	}

	if(uType == vmsg::THING_TYPE_DEBRIS) { // ËéÆ¬
		if(m_pFlagImg == NULL) {
			m_pFlagImg = UIImageView::create();
			m_pFlagImg->loadTexture("other/icon_frag.png", UI_TEX_TYPE_PLIST);
			m_pFlagImg->setPosition(CCPoint(m_pFlagImg->getSize() / 2) - CCPoint(m_pIconImg->getSize() / 2));
			addChild(m_pFlagImg);
		} else {
			m_pFlagImg->setVisible(true);
			m_pFlagImg->setEnabled(true);
		}
	} else {
		if(m_pFlagImg != NULL) {
			m_pFlagImg->setVisible(false);
			m_pFlagImg->setEnabled(false);
		}
	}

	if(uCnt > 0 && m_pCntTxt == NULL) {
		m_pCntTxt = UILabel::create();
		m_pCntTxt->setColor(ccc3(0x32, 0xff, 0x05));
		m_pCntTxt->setFontName("fonts/04b_03b.ttf");
		m_pCntTxt->setFontSize(24);
		m_pCntTxt->setTextHorizontalAlignment(kCCTextAlignmentRight); //ÓÒ¶ÔÆë
		m_pCntTxt->setText("");
		m_pCntTxt->setAnchorPoint(ccp(1, 0.5));
		dynamic_cast<CCLabelTTF*>(m_pCntTxt->getVirtualRenderer())->enableShadow(CCSizeMake(2, 2), 0.4f, 1.0f);
		addChild(m_pCntTxt);
	}

	if(m_pCntTxt != NULL) {
		m_pCntTxt->setEnabled(uCnt > 0);
		m_pCntTxt->setText(intToString(uCnt));

		if(uFrameType == vmsg::THING_TYPE_WEAPON) {
			m_pCntTxt->setPosition(ccp(76, -26));
		} else {
			m_pCntTxt->setPosition(ccp(36, -26));
		}
	}

}

void IconFrameCell::clear() {
	unsigned int uFrameType = m_uType;

	if(m_uType == vmsg::THING_TYPE_DEBRIS) {
		const DebrisCfg *pDebrisCfg = DEBRIS_CFG_MASTER->GetDebrisCfgByID(m_uCfgID);
		uFrameType = pDebrisCfg != NULL ? pDebrisCfg->composetype() : m_uType;
	}

	if(uFrameType == vmsg::THING_TYPE_WEAPON) {
		m_pIconImg->loadTexture("share/bg_weapon_cell.png", UI_TEX_TYPE_PLIST);
		m_pFrameImg->loadTexture("share/weapon_frame.png", UI_TEX_TYPE_PLIST);
		setSize(m_pFrameImg->getSize());
	} else {
		m_pIconImg->loadTexture("share/bg_item_cell.png", UI_TEX_TYPE_PLIST);
		m_pFrameImg->loadTexture("share/item_frame.png", UI_TEX_TYPE_PLIST);
		setSize(m_pFrameImg->getSize());
	}

	if(m_pCntTxt != NULL) {
		m_pCntTxt->setEnabled(false);
	}

	m_uType = 0;
	m_uCfgID = 0;
	m_uCnt = 0;
}

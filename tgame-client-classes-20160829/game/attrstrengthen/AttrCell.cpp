#include "AttrCell.h"

#include "defines.h"

#include "ClientSvrMsg.pb.h"
#include "Macros.pb.h"

#include "CfgMaster/AttrStrengthenCfgMaster.h"

#include "basic/NounHelper.h"

#include "AttrStrengthenControl.h"

AttrCell::AttrCell()
:m_pAttrIconImg(NULL)
,m_uAttrType(0)
,m_uMagicType(0)
,m_uLevel(0)
,m_bSelected(false)
{
}

AttrCell::~AttrCell()
{
}

AttrCell* AttrCell::create(UILayout *pRefLayout) {
	AttrCell *pRet = new AttrCell();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool AttrCell::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		m_pAttrIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("attr_icon_img"));

		selected(m_bSelected);

		bRet = true;
	} while(0);

	return bRet;
}

void AttrCell::setType(unsigned int uAttrType, unsigned int uMagicType) {
	m_uAttrType = uAttrType;
	m_uMagicType = uMagicType;

	const char *pszIconURL = "";

	switch(m_uMagicType) {
		case ICE_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_ice_atk.png" : "attr_strengthen/icon_ice_def.png";
			break;
		case FLAME_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_fire_atk.png" : "attr_strengthen/icon_fire_def.png";
			break;
		case POISON_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_poison_atk.png" : "attr_strengthen/icon_poison_def.png";
			break;
		case BIOTECH_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_biochemistry_atk.png" : "attr_strengthen/icon_biochemistry_def.png";
			break;
		case LIGHTNING_MAGIC:
			pszIconURL = m_uAttrType == vmsg::MAGIC_ATK ? "attr_strengthen/icon_thunder_atk.png" : "attr_strengthen/icon_thunder_def.png";
			break;
		default:;
	}

	m_pAttrIconImg->loadTexture(pszIconURL, UI_TEX_TYPE_PLIST);
}

unsigned int AttrCell::getAttrType() {
	return m_uAttrType;
}

unsigned int AttrCell::getMagicType() {
	return m_uMagicType;
}

unsigned int AttrCell::getLevel() {
	return m_uLevel;
}

void AttrCell::updateUI(unsigned int uLevel) {
	m_uLevel = uLevel;
	/*m_pAttrLevelValTxt->setText(std::string("Lv.") + intToString(m_uLevel));
	m_pAttrLevelValTxt->setVisible(m_uLevel > 0);*/

}

bool AttrCell::isSelected() {
	return m_bSelected;
}

void AttrCell::selected(bool bSelected) {
	m_bSelected = bSelected;

	setTouchEnabled(m_bSelected);
	setScale(m_bSelected ? 1.2f : 1.0f);
	
	if(m_bSelected) {
		setSize(CCSizeMake(180, 200) * 1.2f);
	} else {
		setSize(CCSizeMake(180, 200));
	}
}

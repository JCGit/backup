#include "EquipCell.h"

#include "Resources.h"

#include "CfgMaster/EquipmentCfgMaster.h"
#include "CfgMaster/WeaponCfgMaster.h"

#include "ClientSvrMsg.pb.h"

EquipCell::EquipCell()
:m_pBgItemImg(NULL)
,m_pItemImg(NULL)
{
}

EquipCell::~EquipCell()
{
}

EquipCell* EquipCell::create(UILayout *pRefLayout) {
	EquipCell *pRet = new EquipCell();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool EquipCell::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);

		m_stWeapon.Clear();
		m_stEquipInfo.Clear();

		m_pBgItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_item_img"));

		m_pItemImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("item_img"));

		bRet = true;
	} while(0);

	return bRet;
}

const vmsg::CSEquipInfo& EquipCell::getInfo() {
	return m_stEquipInfo;
}

void EquipCell::setWeapon(const vmsg::CSWeapon &stWeapon) {
	m_stWeapon = stWeapon;

	std::string iconURL = std::string("weaponicon/") + WEAPON_CFG_MASTER->GetMutableWeaponCfgByID(m_stWeapon.cfgid())->weaponicon() + ".png";
	m_pItemImg->loadTexture(iconURL.c_str());
}

void EquipCell::setInfo(const vmsg::CSEquipInfo &stEquipInfo) {
	m_stEquipInfo = stEquipInfo;

	std::string iconURL = ITEM_RES_PATH + EQUIPMENT_CFG_MASTER->GetEquipCfgByID(m_stEquipInfo.equipid())->icon() + ".png";
	m_pItemImg->loadTexture(iconURL.c_str());
}

void EquipCell::clearInfo() {
	if(m_stWeapon.has_weaponguid()) {
		const char *pszIconURL = "share/icon_weapon_default.png";

		m_pItemImg->loadTexture(pszIconURL, UI_TEX_TYPE_PLIST);
	}

	m_stWeapon.Clear();

	if(m_stEquipInfo.has_equipid()) {
		unsigned int uPart = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(m_stEquipInfo.equipid())->part();

		const char *pszIconURL = "";

		if(uPart == vmsg::EQUIP_POS_ARMOR) {
			pszIconURL = "share/icon_armor_default.png";
		} else if(uPart == vmsg::EQUIP_POS_RING) {
			pszIconURL = "share/icon_amulet_default.png";
		} else if(uPart == vmsg::EQUIP_POS_SHOE) {
			pszIconURL = "share/icon_shoes_default.png";
		} else if(uPart == vmsg::EQUIP_POS_CLOTH) {
			pszIconURL = "share/icon_cloak_default.png";
		}

		m_pItemImg->loadTexture(pszIconURL, UI_TEX_TYPE_PLIST);
	}
	
	m_stEquipInfo.Clear(); 
}

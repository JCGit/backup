#include "WeaponSelItem.h"
#include "defines.h"
#include "firearms/FireArmsObj.h"
#include "Resources.h"
#include "UserObj.h"
#include "WeaponUtils.h"
#include "fight/FightObj.h"
#include "Global.h"

#include "WeaponCfgMaster.h"
#include "SkillCfgMaster.h"
#include "WeaponTypeCfgMaster.h"
#include "WeaponStarStepCfgMaster.h"

WeaponSelItem::WeaponSelItem(){
	
}

WeaponSelItem::~WeaponSelItem(){
	
}

WeaponSelItem* WeaponSelItem::create(UILayout *pRefLayout) {
	WeaponSelItem *pRet = new WeaponSelItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool WeaponSelItem::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/weapon_arr_choose_item_ui.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

bool WeaponSelItem::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUICopy(pRefLayout);
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void WeaponSelItem::initUI(){
	m_pCellBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_bg"));
	m_pIconImg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_img"));
	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
	m_pLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("lv_txt"));

	m_pStarBar = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png");
	m_pStarBar->setAnchorPoint(ccp(0.5f, 0.0f));
    m_pStarBar->setStarMargin(1);
	m_pStarBar->setStarNum(7);
	m_pStarBar->setPosition(ccp(315, 24));
    m_pRootLayout->addChild(m_pStarBar, 10);

}

void WeaponSelItem::setInfo(const vmsg::CSWeapon& stInfo){
	m_stInfo = stInfo;

	updateUI();
}

void WeaponSelItem::updateUI(){
	const WeaponCfg* cfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_stInfo.cfgid());
	vector<vmsg::CSWeapon> m_stAllWeapon = P_ARM_OBJ->getAllWeaponBag();

	const int maxLev = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(cfg->initstar(), cfg->initstep())->needlevel();

	m_pNameTxt->setText(cfg->weaponname());
	m_pLvTxt->setText("Lv." + intToString(m_stInfo.level()) + "/" + intToString(maxLev));
	m_pStarBar->setStarNum(cfg->initstar());

	string iconStr = ICON_WEAPON_PATH + cfg->weaponicon() + PNG_TYPE_NAME;
	m_pIconImg->loadTexture(iconStr.c_str());
}

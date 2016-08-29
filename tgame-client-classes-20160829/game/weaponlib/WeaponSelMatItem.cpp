#include "WeaponSelMatItem.h"
#include "firearms/FireArmsObj.h"
#include "defines.h"
#include "basic/TextHelper.h"
#include "WeaponUtils.h"
#include "Resources.h"
#include "ItemUtils.h"

#include "ItemCfgMaster.h"
#include "WeaponCfgMaster.h"
#include "SkillCfgMaster.h"
#include "WeaponTypeCfgMaster.h"
#include "WeaponExpItemCfgMaster.h"
#include "ClientSvrMsg.pb.h"
#include "WeaponCfgMaster.h"
#include "ToolUtils.h"
#include "GlobalCfgMaster.h"

WeaponSelMatItem::WeaponSelMatItem(){
	m_uType = 0;
	m_uItemCfgID = 0;
    _selected = false;
    _isEnable = false;
    m_pStarBar = NULL;
}

WeaponSelMatItem::~WeaponSelMatItem(){
    CC_SAFE_RELEASE_NULL(m_pStarBar);
}

WeaponSelMatItem* WeaponSelMatItem::create(UILayout *pRefLayout) {
	WeaponSelMatItem *pRet = new WeaponSelMatItem();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool WeaponSelMatItem::init(){
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/weapon_sel_mat_item.ExportJson");
		addChild(m_pRootLayout);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

bool WeaponSelMatItem::initWithLayout(UILayout *pRefLayout) {
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

void WeaponSelMatItem::initUI(){
	m_pCellBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("cell_bg"));

	m_pNameTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("name_txt"));
    m_pExpTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("exp_txt"));
    m_pSelectBtn = dynamic_cast<UICheckBox*>(m_pRootLayout->getChildByName("sel_btn"));
    
    m_pCellIcon = UIImageView::create();
    m_pCellIcon->setPosition(m_pCellBg->getPosition());
    addChild(m_pCellIcon);

    
}


void WeaponSelMatItem::setType(unsigned int var){
    m_uType = var;

    m_stWeaponInfo.Clear();
    m_uItemCfgID = 0;
    if(m_uType == vmsg::THING_TYPE_ITEM) {
        m_pCellBg->setSize(CCSizeMake(80, 80));
        if(m_pStarBar && m_pStarBar->getParent()) {
            m_pStarBar->removeFromParent();
        }
    } else if(m_uType == vmsg::THING_TYPE_WEAPON ) {
        m_pCellBg->setSize(CCSizeMake(160, 80));
        if(NULL == m_pStarBar) {
            m_pStarBar = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png");
            m_pStarBar->retain(); //retain
            m_pStarBar->setAnchorPoint(ccp(0.5f, 0.5f));
            CCPoint pt = m_pNameTxt->getPosition();
            pt.y -= 50;
            m_pStarBar->setPosition(pt);
            m_pStarBar->setStarMargin(1);
            m_pStarBar->setStarNum(7);
        }
        if(NULL == m_pStarBar->getParent()) {
            addChild(m_pStarBar);
        }
    }
}

void WeaponSelMatItem::setSelected(bool var){
    //Ç¹ÐµÕ¼2¸ö
    bool iStat = var;
    /*
    if(!var && P_ARM_OBJ->canAddUpMat(m_uType) ) {
        iStat = false;
    }*/

	_selected = iStat;
    
	if(m_pSelectBtn->isVisible()){
		m_pSelectBtn->setSelectedState(_selected);
	}
    
}

bool WeaponSelMatItem::getSelected(){
	return _selected;
}

void WeaponSelMatItem::setIsEnable(bool var){
	_isEnable = var;

	m_pSelectBtn->setVisible(_isEnable);
}

bool WeaponSelMatItem::getIsEnable(){
	return _isEnable;
}

void WeaponSelMatItem::setItemCfgID(unsigned int var){
	m_uItemCfgID = var;

	updateMaterial();
}

void WeaponSelMatItem::setWeaponInfo(const CSWeapon& stInfo){
	m_stWeaponInfo = stInfo;

	updateWeapon();
}

void WeaponSelMatItem::updateMaterial(){
	const ItemCfg* itemCfg = ITEM_CFG_MASTER->GetItemCfgByID(m_uItemCfgID);
	m_pNameTxt->setText(itemCfg->itemname());
	m_pExpTxt->setText(intToString(WEAPON_EXP_CFG->GetWeaponExpItemCfgByID(m_uItemCfgID)->weaponexp()));

    CToolUtils::adjustIconBgSize(m_uType, m_uItemCfgID, m_pCellBg);
	m_pCellIcon->loadTexture(ItemUtils::getItemIcon(vmsg::THING_TYPE_ITEM, m_uItemCfgID).c_str());
}

void WeaponSelMatItem::updateWeapon(){
	const WeaponCfg* cfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_stWeaponInfo.cfgid());
	const SkillCfg* skillCfg = SKILL_CFG_MASTER->GetSkillCfgByID(cfg->weaponsuperskill());
    
    m_pNameTxt->setText(cfg->weaponname() + " Lv." + intToString(m_stWeaponInfo.level()) );

    int iExp = m_stWeaponInfo.totalexp()  * GLOBAL_CFG_MASTER->GetCfg().weaponeatradio() / 10000.0;
    iExp += WEAPON_CFG_MASTER->GetWeaponCfgByID(m_stWeaponInfo.cfgid())->baseexp();
    m_pExpTxt->setText(intToString(iExp)); 

    CToolUtils::adjustIconBgSize(m_uType, m_uItemCfgID, m_pCellBg);

    string iconStr = ICON_WEAPON_PATH + cfg->weaponicon() + PNG_TYPE_NAME;
    m_pCellIcon->loadTexture(iconStr.c_str());
    m_pStarBar->setStarNum(cfg->initstar());
}


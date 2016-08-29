#include "WeaponAdvDonePanel.h"
#include "WeaponUtils.h"
#include "WeaponStarStepCfgMaster.h"
#include "defines.h"
#include "Resources.h"

WeaponAdvDonePanel::WeaponAdvDonePanel()
{
    m_iFromWeaponCfgID = 0;
    m_iFromWeaponLv = 0;
    m_iFromWeaponColor = 0;

    m_iToWeaponCfgID = 0;
    m_iToWeaponLv = 0;
    m_iToWeaponColor = 0;
}

WeaponAdvDonePanel::~WeaponAdvDonePanel()
{

}

bool WeaponAdvDonePanel::init() 
{
	bool bRet = false;

	do 
    {
		CC_BREAK_IF(!BasePanel::init());
        setClickOutToClose(true);

		initLayoutFromJsonFile("ui_v2/weapon_lv_up_done_panel.ExportJson");
		addChild(m_pRootLayout);
		
		m_pCloseBtn = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("close_btn"));
        m_pCloseBtn->setTouchEnabled(true);
		m_pCloseBtn->addTouchEventListener(this, toucheventselector(WeaponAdvDonePanel::touchEventHandler));

        m_pDoCloseBtn = dynamic_cast<UIButton*>(m_pRootLayout->getChildByName("do_close"));
        m_pDoCloseBtn->addTouchEventListener(this, toucheventselector(WeaponAdvDonePanel::touchEventHandler));

        m_pFromWeaponBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_0"));
        m_pFromAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_val_txt_0"));
        m_pFromAtkAttrValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_attr_val_txt_0"));
        m_pFromMaxLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("max_lv_txt_0"));

        m_pFromWeaponIcon = UIImageView::create();
        m_pFromWeaponBg->addChild(m_pFromWeaponIcon);

        m_pToWeaponBg = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("icon_1"));
        m_pToAtkValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_val_txt_1"));
        m_pToAtkAttrValTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("atk_attr_val_txt_1"));
        m_pToMaxLvTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("max_lv_txt_1"));

        m_pToWeaponIcon = UIImageView::create();
        m_pToWeaponBg->addChild(m_pToWeaponIcon);

		bRet = true;
	} 
    while(0);

	return bRet;
}

void WeaponAdvDonePanel::open()
{
    if(m_iFromWeaponCfgID <= 0 || m_iToWeaponCfgID <= 0) {
        return;
    }
    BasePanel::open();
    doUpdateUI();
}


void WeaponAdvDonePanel::close()
{
    BasePanel::close();

}

CCNode* WeaponAdvDonePanel::getGuideNodeByName(const std::string &name) {
	UIButton *pRet = NULL;

	if(name == "do_close_btn") {
		if(m_pDoCloseBtn != NULL && m_pDoCloseBtn->isRunning() && m_pDoCloseBtn->isVisible()) {
			pRet = m_pDoCloseBtn;
		}
	}

	return pRet;
}

void WeaponAdvDonePanel::touchEventHandler(CCObject *pSender, TouchEventType type) 
{
    UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
    const char *pszWidgetName = pWidget->getName();

    switch(type) {
    case TOUCH_EVENT_ENDED:
        if(std::strcmp(pszWidgetName, "close_btn") == 0) {
            close();
        } else if(std::strcmp(pszWidgetName, "do_close") == 0) {
			GUIDE_TOUCH(pWidget);
            close();
        }
        break;
    default:
        break;
    }
}


void WeaponAdvDonePanel::setOneWeaponInfo(const bool bIsFromEquip, const int iCfgID, const int iLv, const int iColor) {
    //neilwu,comment,在点击进阶按钮后就把这个设置把，以后直接打开面板来显示就行了
    if(bIsFromEquip) {
        m_iFromWeaponCfgID = iCfgID;
        m_iFromWeaponLv = iLv;
        m_iFromWeaponColor = iColor;
    } else {
        m_iToWeaponCfgID = iCfgID;
        m_iToWeaponLv = iLv;
        m_iToWeaponColor = iColor;
    }
}

void WeaponAdvDonePanel::doUpdateUI() {
    if(m_iFromWeaponCfgID <= 0 || m_iToWeaponCfgID <= 0) {
        close();
        return;
    }
    const WeaponCfg* pFromWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_iFromWeaponCfgID);
    const WeaponCfg* pToWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_iToWeaponCfgID);
    if(!pFromWeaponCfg || !pToWeaponCfg) {
        close();
        return;
    }

    string iconStr = ICON_WEAPON_PATH + pFromWeaponCfg->weaponicon() + PNG_TYPE_NAME;
	unsigned int atkAddCoe = WeaponUtils::getWeaponAddCoe(m_iFromWeaponCfgID, m_iFromWeaponLv, m_iFromWeaponColor);
    m_pFromWeaponIcon->loadTexture(iconStr.c_str());
    m_pFromAtkValTxt->setText(intToString(WeaponUtils::getWeaponAtk(m_iFromWeaponCfgID, m_iFromWeaponLv, m_iFromWeaponColor ) + atkAddCoe));
    m_pFromAtkAttrValTxt->setText(intToString(WeaponUtils::getWeaponAtkAttr(m_iFromWeaponCfgID, m_iFromWeaponLv, m_iFromWeaponColor )));
    const unsigned int uFromEquipMaxLv = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pFromWeaponCfg->initstar(), 
        pFromWeaponCfg->initstep())->needlevel();
    m_pFromMaxLvTxt->setText(intToString(uFromEquipMaxLv));

    iconStr = ICON_WEAPON_PATH + pToWeaponCfg->weaponicon() + PNG_TYPE_NAME;
	atkAddCoe = WeaponUtils::getWeaponAddCoe(m_iToWeaponCfgID, m_iToWeaponLv, m_iToWeaponColor);
    m_pToWeaponIcon->loadTexture(iconStr.c_str());
    m_pToAtkValTxt->setText(intToString(WeaponUtils::getWeaponAtk(m_iToWeaponCfgID, m_iToWeaponLv, m_iToWeaponColor ) + atkAddCoe));
    m_pToAtkAttrValTxt->setText(intToString(WeaponUtils::getWeaponAtkAttr(m_iToWeaponCfgID, m_iToWeaponLv, m_iToWeaponColor )));
    const unsigned int uToEquipMaxLv = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(pToWeaponCfg->initstar(), 
        pToWeaponCfg->initstep())->needlevel();
    m_pToMaxLvTxt->setText(intToString(uToEquipMaxLv));

	GUIDE_SHOW(GUIDE_UI_WEAPON_STEP_DONE);
}


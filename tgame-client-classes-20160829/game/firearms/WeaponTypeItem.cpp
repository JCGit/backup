#include "WeaponTypeItem.h"
#include "defines.h"

const string CWeaponTypeItem::WEAPON_TAB_BTN_CLICK = "weapon_tab_btn_click";

CWeaponTypeItem::CWeaponTypeItem(){

}

CWeaponTypeItem::~CWeaponTypeItem(){

}

bool CWeaponTypeItem::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutFromJsonFile("ui_v2/weapon_lib_type_btn.ExportJson"); 
		addChild(m_pRootLayout);

		m_pNum = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("num_txt"));
		m_pTabBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("tab_bg"));
        m_pTabTxt = dynamic_cast<UILabel*>(m_pRootLayout->getChildByName("tab_txt"));
        m_pBgNum = dynamic_cast<UIImageView*>(m_pRootLayout->getChildByName("bg_num"));

        m_pNum->setVisible(false);
        m_pBgNum->setVisible(false);

		bRet = true;
	} while(0);

	return bRet;
}

void CWeaponTypeItem::setTxt(const std::string& text){
	m_pTabTxt->setText(text);
}

void CWeaponTypeItem::setNum(unsigned int num){
	if(num > 0){
		m_pNum->setText(intToString(num));
	}

    m_pNum->setVisible( (num > 0) );
    m_pBgNum->setVisible((num > 0));
}

void CWeaponTypeItem::setSelected(bool var){
    _selected = var;
    /*
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_tp/weapon_lib_ui.plist");
	if(_selected){
		m_pTabBg->loadTexture("weapon_lib_ui/weapon_tab_select.png", UI_TEX_TYPE_PLIST);
	}else{
		m_pTabBg->loadTexture("weapon_lib_ui/weapon_tab_normal.png", UI_TEX_TYPE_PLIST);
	}
    */
    if(_selected){
        m_pTabBg->loadTexture("share/btn_tab_down.png", UI_TEX_TYPE_PLIST);
        m_pTabTxt->setColor(ccc3(0xff, 0xfa, 0xae) );
        m_pTabTxt->setFontSize(24);
    }else{
        m_pTabBg->loadTexture("share/btn_tab_up.png", UI_TEX_TYPE_PLIST);
        m_pTabTxt->setColor(ccc3(0x7b, 0xbc, 0xe1) );
        m_pTabTxt->setFontSize(22);
    }
}

bool CWeaponTypeItem::getSelected(){
	return _selected;
}

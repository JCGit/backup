#include "WeaponArrCell.h"
#include "ClientSvrMsg.pb.h"
#include "ItemUtils.h"
#include "WeaponLibControl.h"
#include "firearms/FireArmsObj.h"
#include "game/guide/GuideControl.h"

CWeaponArrCell::CWeaponArrCell(){

}

CWeaponArrCell::~CWeaponArrCell(){

}

CWeaponArrCell* CWeaponArrCell::create(UILayout *pRefLayout) {
	CWeaponArrCell *pRet = new CWeaponArrCell();

	if(pRet != NULL && pRet->initWithLayout(pRefLayout)) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool CWeaponArrCell::initWithLayout(UILayout *pRefLayout) {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BaseLayout::init());

		initLayoutWithUI(pRefLayout);
		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CWeaponArrCell::initUI(){
	m_pWeaponBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("weapon_bg"));
	m_pFrameBg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("frame_bg"));
	m_pIconBg = dynamic_cast<ImageView*>(m_pFrameBg->getChildByName("icon_bg"));
	m_pWeaponImg = dynamic_cast<ImageView*>(m_pFrameBg->getChildByName("weapon_icon"));

	m_pWeaponBg->setTouchEnabled(true);
	m_pWeaponBg->addTouchEventListener(this, toucheventselector(CWeaponArrCell::touchEventHandler));
}

void CWeaponArrCell::setInfo(const CSInbattleWeaponElmt& stElmt){
	m_stInfo = stElmt;

	updateUI();
}

void CWeaponArrCell::updateUI(){
	if(m_stInfo.state() == WEAPON_ARRAY_CELL_LOCKED){  //未开启
		m_pFrameBg->setVisible(false);
		m_pWeaponBg->loadTexture("weapon_arr/weapon_lock_bg.png", UI_TEX_TYPE_PLIST);
	}else{  //已经开启
		m_pFrameBg->setVisible(true);
		if(m_stInfo.weapon().weaponguid() != 0){  //已经装备
			string iconUrl = ItemUtils::getItemIcon(THING_TYPE_WEAPON, m_stInfo.weapon().cfgid());
			m_pWeaponImg->setVisible(true);
			m_pWeaponImg->loadTexture(iconUrl.c_str());
			m_pFrameBg->loadTexture("weapon_arr/weapon_bg_img_select.png", UI_TEX_TYPE_PLIST);
			m_pIconBg->loadTexture("weapon_arr/equip_weapon_icon_bg.png", UI_TEX_TYPE_PLIST);
			m_pWeaponBg->loadTexture("weapon_arr/weapon_equip_bg.png", UI_TEX_TYPE_PLIST);
		}else{
			m_pWeaponImg->setVisible(false);
			m_pFrameBg->loadTexture("weapon_arr/weapon_bg_img.png", UI_TEX_TYPE_PLIST);
			m_pIconBg->loadTexture("weapon_arr/change_txt_img.png", UI_TEX_TYPE_PLIST);
			m_pWeaponBg->loadTexture("weapon_arr/weapon_choose_bg.png", UI_TEX_TYPE_PLIST);
		}
	}
}

void CWeaponArrCell::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "weapon_bg") == 0) {
			GUIDE_TOUCH(pWidget);

			if(m_stInfo.state() == WEAPON_ARRAY_CELL_LOCKED){  //未开启
				P_WEAPON_CTRL->getArrOpenPanel()->open();
				P_WEAPON_CTRL->getArrOpenPanel()->setArrIdx(m_stInfo.arrayidx() + 1);
			}else{
				if(m_stInfo.weapon().weaponguid() != 0){  //阵位上有武器，打开枪械信息面板
					P_WEAPON_CTRL->getArrInfoPanel()->setInfo(m_stInfo.weapon(), m_stInfo.arrayidx());
					P_WEAPON_CTRL->getArrInfoPanel()->open();
				}else{
					P_ARM_OBJ->setOldWeapon(m_stInfo.weapon().weaponguid(), m_stInfo.arrayidx());
					P_WEAPON_CTRL->getWeaponSelPanel()->setSelType(WeaponSelPanel::DEPUTY_WEAPON_TYPE);
					P_WEAPON_CTRL->getWeaponSelPanel()->open();
				}
			}
		}
		break;
	default:

		break;
	}
}


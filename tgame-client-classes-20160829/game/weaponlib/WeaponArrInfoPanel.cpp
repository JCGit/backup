#include "WeaponArrInfoPanel.h"
#include "defines.h"
#include "ItemUtils.h"
#include "WeaponUtils.h"
#include "ClientSvrMsg.pb.h"
#include "WeaponLibControl.h"
#include "firearms/FireArmsObj.h"
#include "firearms/StarProgressBar.h"

#include "WeaponCfgMaster.h"
#include "WeaponStarStepCfgMaster.h"

CWeaponArrInfoPanel::CWeaponArrInfoPanel(){
	m_iIDx = -1;
}

CWeaponArrInfoPanel::~CWeaponArrInfoPanel(){

}

CCNode* CWeaponArrInfoPanel::getGuideNodeByName(const std::string &name)
{
	CCNode* pRet = NULL;
	if (name == "change_weapon_btn")
	{
		pRet = m_pChangeImg;
	}
	return pRet;
}

bool CWeaponArrInfoPanel::init(){
	bool bRet = false;

	do 
	{
		CC_BREAK_IF(!BasePanel::init());

		initLayoutFromJsonFile("ui_v2/weapon_arr_info_ui.ExportJson");
		addChild(m_pRootLayout);

		setClickOutToClose(true);

		initUI();

		bRet = true;
	} while (0);

	return bRet;
}

void CWeaponArrInfoPanel::initUI(){
	m_pCloseBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("close_btn"));
	m_pCloseBtn->addTouchEventListener(this, toucheventselector(CWeaponArrInfoPanel::touchEventHandler));

	m_pChangeBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("change_btn"));
	m_pChangeBtn->addTouchEventListener(this, toucheventselector(CWeaponArrInfoPanel::touchEventHandler));
	m_pChangeImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("img_change"));


	m_pUnloadBtn = dynamic_cast<Button*>(m_pRootLayout->getChildByName("unload_btn"));
	m_pUnloadBtn->addTouchEventListener(this, toucheventselector(CWeaponArrInfoPanel::touchEventHandler));

	m_pIconImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("weapon_icon"));
	m_pUseImg = dynamic_cast<ImageView*>(m_pRootLayout->getChildByName("use_img"));

	m_pNameTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("name_txt"));
	m_pLevTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("lev_txt"));
	m_pAtkTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("atk_txt"));
	m_pAtkSpeedTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("atk_speed_txt"));
	m_pHitTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("hit_txt"));
	m_pBulletTxt = dynamic_cast<Label*>(m_pRootLayout->getChildByName("bullet_txt"));

	m_pStarBar = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png");
	m_pStarBar->setAnchorPoint(ccp(0.0f, 0.0f));
	float xx = m_pIconImg->getPositionX() + m_pIconImg->getSize().width/2 + 50; 
	float yy = m_pNameTxt->getPositionY() - 60;
	m_pStarBar->setPosition(ccp(xx, yy));
	m_pStarBar->setStarMargin(8);
	m_pStarBar->setStarNum(7);
	m_pRootLayout->addChild(m_pStarBar);
}

void CWeaponArrInfoPanel::setInfo(const CSWeapon& stInfo, int iIdx){
	m_stInfo = stInfo;
	m_iIDx = iIdx;

	updateUI();
}

void CWeaponArrInfoPanel::updateUI(){
	const WeaponCfg* weaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(m_stInfo.cfgid());
	if(NULL == weaponCfg) {
		return;
	}

	const int maxLev = WEAPON_STAR_CFG->GetWeaponStarStepCfgByStarAndStep(weaponCfg->initstar(), weaponCfg->initstep())->needlevel();

	m_pNameTxt->setText(weaponCfg->weaponname());
	m_pLevTxt->setText(intToString(m_stInfo.level()) + "/" + intToString(maxLev));
	m_pAtkSpeedTxt->setText(intToString(weaponCfg->shotspeed()));
	m_pHitTxt->setText(intToString(weaponCfg->hit()));
	m_pBulletTxt->setText(intToString(weaponCfg->bulletshowval()));

	int iAddCoe = WeaponUtils::getWeaponAddCoe(m_stInfo.cfgid(), m_stInfo.level(), m_stInfo.quality());
	iAddCoe += WeaponUtils::getWeaponAtk(m_stInfo.cfgid(), m_stInfo.level(), m_stInfo.quality());
	m_pAtkTxt->setText(intToString(iAddCoe));

	string urlStr = ItemUtils::getItemIcon(THING_TYPE_WEAPON, m_stInfo.cfgid());
	m_pIconImg->loadTexture(urlStr.c_str());

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui_v2/other.plist");
	urlStr = "other/main_weapon_use_img.png";
	if(m_stInfo.weaponguid() == P_ARM_OBJ->getWeaponBag().playerweapon().weaponguid()){  //主武器
		urlStr = "other/main_weapon_use_img.png";
	}else{
		urlStr = "other/deputy_weapon_use_img.png";
	}
	m_pUseImg->loadTexture(urlStr.c_str(), UI_TEX_TYPE_PLIST);
	m_pStarBar->setStarNum(weaponCfg->initstar());

	if(m_iIDx == -1){
		m_pUnloadBtn->setEnabled(false);
	}else{
		m_pUnloadBtn->setEnabled(true);
	}
	GUIDE_SHOW(GUIDE_UI_CHANGEWEAPON_INFO);
}

void CWeaponArrInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type){
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();
	string strWidgetName = pWidget->getName();

	switch (type) {
	case TOUCH_EVENT_ENDED:
		if(strcmp(pszWidgetName, "close_btn") == 0) {
			this->close();
		}else if(strcmp(pszWidgetName, "change_btn") == 0) {
			GUIDE_TOUCH(m_pChangeImg);
			doClickDone();
		}else if(strcmp(pszWidgetName, "unload_btn") == 0) {
			doUnloadDone();
		}
		break;
	default:

		break;
	}
}

void CWeaponArrInfoPanel::doClickDone(){
	if(m_iIDx == -1){   //主武器
		P_WEAPON_CTRL->getWeaponSelPanel()->setSelType(WeaponSelPanel::MAIN_WEAPON_TYPE);
		P_WEAPON_CTRL->getWeaponSelPanel()->open();
	}else{	//副武器
		P_ARM_OBJ->setOldWeapon(m_stInfo.weaponguid(), m_iIDx);
		P_WEAPON_CTRL->getWeaponSelPanel()->setSelType(WeaponSelPanel::DEPUTY_WEAPON_TYPE);
		P_WEAPON_CTRL->getWeaponSelPanel()->open();
	}

	this->close();
}

void CWeaponArrInfoPanel::doUnloadDone(){
	if(m_iIDx == -1){   //主武器不能卸下
		return;
	}

	P_ARM_OBJ->setOldWeapon(m_stInfo.weaponguid(), m_iIDx);

	const vmsg::CSInbattleWeaponElmtBrief& stOldWeapon = P_ARM_OBJ->getOldWeapon();

	vmsg::CSInbattleWeaponElmtBrief stNewWeapon;
	stNewWeapon.set_weaponguid(0);
	stNewWeapon.set_bagidx(-1);
	stNewWeapon.set_arrayidx(-1);

	P_WEAPON_CTRL->sendChangeWeaponInBattleRqst(stOldWeapon, stNewWeapon);

	this->close();
}

void CWeaponArrInfoPanel::open(){
	BasePanel::open();
}

void CWeaponArrInfoPanel::close(){
	BasePanel::close();
}


#include "OtherRoleInfoPanel.h"

#include "defines.h"
#include "Constant.h"
#include "Resources.h"

#include "basic/NounHelper.h"
#include "basic/ImageNumber.h"
#include "basic/ToolUtils.h"

#include "CfgMaster/HeroCfgMaster.h"
#include "CfgMaster/EquipmentCfgMaster.h"
#include "CfgMaster/WeaponCfgMaster.h"
#include "CfgMaster/WeaponRefitAdditionCfgMaster.h"
#include "CfgMaster/ExpCfgMaster.h"

#include "ClientSvrMsg.pb.h"
#include "CSEquipItemMsg.pb.h"
#include "ModuleID.pb.h"
#include "CSPlayerMsg.pb.h"

#include "game/UserObj.h"
#include "game/role/MainHero.h"
#include "game/city/CityControl.h"
#include "game/firearms/StarProgressBar.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/login/LoginObj.h"

#include "StarCell.h"
#include "EquipCell.h"
#include "BagControl.h"
#include "EquipInfoPanel.h"
#include "ItemUtils.h"
#include "friend/FriendControl.h"
#include "TextHelper.h"
#include "CommonDialog.h"
#include "WeaponUtils.h"

OtherRoleInfoPanel::OtherRoleInfoPanel()
:m_pBackBtn(NULL)
,m_pInfoRoleNameTxt(NULL)
,m_pRoleLayout(NULL)
,m_pRole(NULL)
,m_pWeaponCell(NULL)
,m_pWeaponStarProgress(NULL)
,m_pArmorCell(NULL)
,m_pAmuletCell(NULL)
,m_pShoesCell(NULL)
,m_pCloakCell(NULL)
,m_pInfoLevelValTxt(NULL)
,m_pInfoHpValTxt(NULL)
,m_pInfoAttackValTxt(NULL)
,m_pInfoDefenseValTxt(NULL)
,m_pInfoHitValTxt(NULL)
,m_pInfoHpAddValTxt(NULL)
,m_pInfoAttackAddValTxt(NULL)
,m_pInfoDefenseAddValTxt(NULL)
,m_pInfoHitAddValTxt(NULL)
,m_pOptionBtn(NULL)
//detail
,m_pRoleNameTxt(NULL)
,m_pLevelValTxt(NULL)
,m_pHpValTxt(NULL)
,m_pExpProgressTxt(NULL)
,m_pExpProgress(NULL)
,m_pFightPowerValTxt(NULL)
,m_pUINValTxt(NULL)
,m_pDefValTxt(NULL)
,m_pAtkValTxt(NULL)
,m_pHitValTxt(NULL)
,m_pDefTypeValTxt(NULL)
,m_pAtkTypeValTxt(NULL)
,m_pThunderAtkValTxt(NULL)
,m_pPoisonAtkValTxt(NULL)
,m_pFireAtkValTxt(NULL)
,m_pIceAtkValTxt(NULL)
,m_pBiochemistryAtkValTxt(NULL)
,m_pThunderDefValTxt(NULL)
,m_pPoisonDefValTxt(NULL)
,m_pFireDefValTxt(NULL)
,m_pIceDefValTxt(NULL)
,m_pBiochemistryDefValTxt(NULL)
{
}

OtherRoleInfoPanel::~OtherRoleInfoPanel()
{
}

OtherRoleInfoPanel* OtherRoleInfoPanel::create() {
	OtherRoleInfoPanel *pRet = new OtherRoleInfoPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool OtherRoleInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);

		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void OtherRoleInfoPanel::open() {
	BasePanel::open();
	//registerWithTouchDispatcher();
	CToolUtils::loadArmatureRGBA4444("effect/ut00059/ut00059.ExportJson");
	showLoading();
}

void OtherRoleInfoPanel::close(){
	BasePanel::close();

	CToolUtils::removeArmatureFile("effect/ut00059/ut00059.ExportJson");
	setTouchEnabled(false);
	// CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	if(this->getIsInLoading()){
		this->closeLoading();
	}

	if(m_pRole && m_pRole->getParent()){
		m_pRole->removeFromParentAndCleanup(true);
		m_pRole = NULL;
	}
}

void OtherRoleInfoPanel::onExit() {
	BasePanel::onExit();
	
	if(m_pRole && m_pRole->getParent()){
		m_pRole->removeFromParentAndCleanup(true);
		m_pRole = NULL;
	}
}

void OtherRoleInfoPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/OtherRoleInfoPanelUI.ExportJson");
	addChild(m_pRootLayout);
	
	UILayout *pInfoLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("role_info_layout"));

	m_pBackBtn = dynamic_cast<UIButton*>(pInfoLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(OtherRoleInfoPanel::touchEventHandler));
	
	m_pInfoRoleNameTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("role_name_txt"));

	m_pRoleLayout = dynamic_cast<UILayout*>(pInfoLayout->getChildByName("role_layout"));
	m_pRoleLayout->setTouchEnabled(false);

	m_pWeaponCell = EquipCell::create(dynamic_cast<UILayout*>(pInfoLayout->getChildByName("weapon_cell")));
	m_pWeaponCell->addTouchEventListener(this, toucheventselector(OtherRoleInfoPanel::touchEventHandler));
	m_pWeaponCell->setName("weapon_cell");
	m_pWeaponCell->setTouchEnabled(false);
	
	m_pWeaponStarProgress = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png");
	m_pWeaponStarProgress->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pWeaponStarProgress->setZOrder(2);
	m_pWeaponStarProgress->setPosition(ccp(m_pWeaponCell->getSize().width / 2, m_pWeaponCell->getSize().height));
	m_pWeaponStarProgress->setStarMargin(1);
	m_pWeaponStarProgress->setStarNum(7);
	m_pWeaponStarProgress->setScale(0.8f);
	m_pWeaponCell->addChild(m_pWeaponStarProgress);

	m_pArmorCell = EquipCell::create(dynamic_cast<UILayout*>(pInfoLayout->getChildByName("armor_cell")));
	m_pArmorCell->addTouchEventListener(this, toucheventselector(OtherRoleInfoPanel::touchEventHandler));
	m_pArmorCell->setName("armor_cell");

	m_pAmuletCell = EquipCell::create(dynamic_cast<UILayout*>(pInfoLayout->getChildByName("amulet_cell")));
	m_pAmuletCell->addTouchEventListener(this, toucheventselector(OtherRoleInfoPanel::touchEventHandler));
	m_pAmuletCell->setName("amulet_cell");

	m_pShoesCell = EquipCell::create(dynamic_cast<UILayout*>(pInfoLayout->getChildByName("shoes_cell")));
	m_pShoesCell->addTouchEventListener(this, toucheventselector(OtherRoleInfoPanel::touchEventHandler));
	m_pShoesCell->setName("shoes_cell");

	m_pCloakCell = EquipCell::create(dynamic_cast<UILayout*>(pInfoLayout->getChildByName("cloak_cell")));
	m_pCloakCell->addTouchEventListener(this, toucheventselector(OtherRoleInfoPanel::touchEventHandler));
	m_pCloakCell->setName("cloak_cell");

	m_pInfoLevelValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("level_val_txt"));

	m_pInfoHpValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("hp_val_txt"));

	m_pInfoAttackValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("atk_val_txt"));

	m_pInfoDefenseValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("def_val_txt"));

	m_pInfoHitValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("hit_val_txt"));

	m_pInfoHpAddValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("hp_add_val_txt"));

	m_pInfoAttackAddValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("atk_add_val_txt"));

	m_pInfoDefenseAddValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("def_add_val_txt"));

	m_pInfoHitAddValTxt = dynamic_cast<UILabel*>(pInfoLayout->getChildByName("hit_add_val_txt"));

	m_pOptionBtn = dynamic_cast<UIButton*>(pInfoLayout->getChildByName("bag_toggle_btn"));
	m_pOptionBtn->addTouchEventListener(this, toucheventselector(OtherRoleInfoPanel::touchEventHandler));
	m_pOptionBtn->setTitleText(NounStr(NOUN_BE_FRIEND));

	UILayout *pDetailLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("detail_layout"));

	m_pRoleNameTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("role_name_txt"));

	m_pVIPTxt = ImageNumber::create(25);
	m_pVIPTxt->setAnchorPoint(ccp(0.0f, 0.0f));
	m_pVIPTxt->setPosition(240, 538);
	pDetailLayout->addNode(m_pVIPTxt);
	m_pVIPTxt->setNum("0", 0.0f, "vip");

	m_pLevelValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("level_val_txt"));

	m_pHpValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("hp_val_txt"));

	m_pExpProgress = dynamic_cast<UILoadingBar*>(pDetailLayout->getChildByName("exp_progress"));

	m_pExpProgressTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("exp_progress_txt"));

	m_pFightPowerValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("fight_power_val_txt"));
	CCLabelTTF *pLabel = dynamic_cast<CCLabelTTF*>(m_pFightPowerValTxt->getVirtualRenderer());
	pLabel->enableStroke(ccc3(0xff, 0x60, 0x00), 0.8f);

	m_pUINValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("uid_val_txt"));

	m_pDefValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("def_val_txt"));

	m_pDefTypeValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("def_type_val_txt"));

	m_pAtkValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("atk_val_txt"));

	m_pAtkTypeValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("atk_type_val_txt"));

	m_pHitValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("hit_val_txt"));

	m_pThunderAtkValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("thunder_atk_val_txt"));

	m_pThunderDefValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("thunder_def_val_txt"));

	m_pPoisonAtkValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("poison_atk_val_txt"));

	m_pPoisonDefValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("poison_def_val_txt"));

	m_pFireAtkValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("fire_atk_val_txt"));

	m_pFireDefValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("fire_def_val_txt"));

	m_pIceAtkValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("ice_atk_val_txt"));

	m_pIceDefValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("ice_def_val_txt"));

	m_pBiochemistryAtkValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("biochemistry_atk_val_txt"));

	m_pBiochemistryDefValTxt = dynamic_cast<UILabel*>(pDetailLayout->getChildByName("biochemistry_def_val_txt"));

}

void OtherRoleInfoPanel::updateUI() {
	if(!isOpen()) {
		return;
	}

	if(this->getIsInLoading()){
		this->closeLoading();
	}

	const vmsg::CSPlayerInfo &stPlayerInfo = P_LOGIN_OBJ->getOtherPlayerInfo();
	const vmsg::CSPlayerDetailInfo &stPlayerDetailInfo = P_LOGIN_OBJ->getOtherPlayerDetailInfo();

	m_pInfoRoleNameTxt->setText(stPlayerInfo.nick());

	m_pInfoLevelValTxt->setText(intToString(stPlayerInfo.level()));

	m_pWeaponCell->clearInfo();
	m_pArmorCell->clearInfo();
	m_pAmuletCell->clearInfo();
	m_pShoesCell->clearInfo();
	m_pCloakCell->clearInfo();

	unsigned int starLevel = 0;
	unsigned int hpAddVal = 0;
	unsigned int atkAddVal = 0;
	unsigned int defAddVal = 0;
	unsigned int moveSpeedAddVal = 0;
	unsigned int uStrengthenAttrVal = 0;

	const vmsg::CSWeapon &stWeapon =  stPlayerDetailInfo.weapon();
	const WeaponCfg *pWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());

	m_pWeaponCell->setWeapon(stWeapon);
	m_pWeaponStarProgress->setStarNum(pWeaponCfg->initstar());

    atkAddVal += pWeaponCfg->atk() + pWeaponCfg->atkadd() * (stWeapon.level() - 1);
    unsigned int colorAdd = WEAPONREFITADDITION_CFG_MASTER->GetCfg(stWeapon.quality())->multi();
    atkAddVal *= (float)colorAdd / 10000;
    atkAddVal = (1.0*atkAddVal*pWeaponCfg->shotspeed())/1270.0f;

	for(int i = 0; i < stPlayerDetailInfo.equiplst_size(); i++) {
		const vmsg::CSEquipInfo &stEquipInfo = stPlayerDetailInfo.equiplst(i);
		const EquipmentCfg *pEquipCfg = EQUIPMENT_CFG_MASTER->GetEquipCfgByID(stEquipInfo.equipid());
		unsigned int part = pEquipCfg->part();

		if(part == vmsg::EQUIP_POS_ARMOR) {
			m_pArmorCell->setInfo(stEquipInfo);
		} else if(part == vmsg::EQUIP_POS_RING) {
			m_pAmuletCell->setInfo(stEquipInfo);
		} else if(part == vmsg::EQUIP_POS_SHOE) {
			m_pShoesCell->setInfo(stEquipInfo);
		} else if(part == vmsg::EQUIP_POS_CLOTH) {
			m_pCloakCell->setInfo(stEquipInfo);
		}
		uStrengthenAttrVal = ItemUtils::getEquipAttrVal(stEquipInfo.equipid(), stEquipInfo.level(), stEquipInfo.quality() );  //该公式算出的最终总属性

		if(pEquipCfg->attr().attrid() == vmsg::ATTR_HP) {
			hpAddVal += uStrengthenAttrVal;
		} else if(pEquipCfg->attr().attrid() == vmsg::ATTR_ATK) {
			atkAddVal += uStrengthenAttrVal;
		} else if(pEquipCfg->attr().attrid() == vmsg::ATTR_DEF) {
			defAddVal += uStrengthenAttrVal;
		} else if(pEquipCfg->attr().attrid() == vmsg::ATTR_MOVESPEED) {
			moveSpeedAddVal += uStrengthenAttrVal;
		}
	}

	m_pInfoHpValTxt->setText(intToString(stPlayerDetailInfo.hp() - hpAddVal));
	m_pInfoAttackValTxt->setText(intToString(stPlayerDetailInfo.atk() - atkAddVal));
	m_pInfoDefenseValTxt->setText(intToString(stPlayerDetailInfo.def() - defAddVal));
	m_pInfoHitValTxt->setText(intToString((unsigned int)(floor(stPlayerDetailInfo.hitwithoutinitval() * 2.3f + 0.5f))));

	m_pInfoHpAddValTxt->setText(std::string("+") + intToString(hpAddVal));
	m_pInfoHpAddValTxt->setPositionX(m_pInfoHpValTxt->getPositionX() + m_pInfoHpValTxt->getSize().width + 16.0f);
	m_pInfoHpAddValTxt->setVisible(hpAddVal > 0);

	m_pInfoAttackAddValTxt->setText(std::string("+") + intToString(atkAddVal));
	m_pInfoAttackAddValTxt->setPositionX(m_pInfoAttackValTxt->getPositionX() + m_pInfoAttackValTxt->getSize().width + 16.0f);
	m_pInfoAttackAddValTxt->setVisible(atkAddVal > 0);

	m_pInfoDefenseAddValTxt->setText(std::string("+") + intToString(defAddVal));
	m_pInfoDefenseAddValTxt->setPositionX(m_pInfoDefenseValTxt->getPositionX() + m_pInfoDefenseValTxt->getSize().width + 16.0f);
	m_pInfoDefenseAddValTxt->setVisible(defAddVal > 0);

	m_pInfoHitAddValTxt->setText(std::string("+") + intToString(0));
	m_pInfoHitAddValTxt->setPositionX(m_pInfoHitValTxt->getPositionX() + m_pInfoHitValTxt->getSize().width + 16.0f);
	m_pInfoHitAddValTxt->setVisible(false);

	updateRole();
	updateDetailInfo();

    if(P_FRIEND_CTRL->isMyFriend( stPlayerInfo.uin() ) && P_FRIEND_CTRL->getFriendPanelUI()->isOpen() ) {
        m_pOptionBtn->setTitleText(NounStr(NOUN_DEL_FRIEND));
    } else {
        m_pOptionBtn->setTitleText(NounStr(NOUN_BE_FRIEND));
    }
}

void OtherRoleInfoPanel::updateRole() {
	const vmsg::CSPlayerInfo &stPlayerInfo = P_LOGIN_OBJ->getOtherPlayerInfo();
	unsigned int uShowID = stPlayerInfo.showid();
	unsigned int uWeaponCfgID = stPlayerInfo.weaponcfgid();

	// 获取资源路径
	string bodyName;
	int iRet = CMainHero::GetResourceNames(uWeaponCfgID, uShowID, bodyName);

	if(iRet != 0) {
		CCLOG("Fail to get hero res! ShowID=%u WeaponCfgID=%u", uShowID, uWeaponCfgID);
		return;
	}

	string bodyPath = HERO_RES_PATH + bodyName + "/" + bodyName + JSON_TYPE_NAME;

	// 加载资源
	CCArmatureDataManager *pArmatureDataManager = CCArmatureDataManager::sharedArmatureDataManager();
	pArmatureDataManager->addArmatureFileInfo(bodyPath.c_str());

	if(m_pRole != NULL) {
		m_pRole->removeFromParent();
		m_pRole = NULL;
	} 

	UIImageView *pRoleShadowImg = dynamic_cast<UIImageView*>(m_pRoleLayout->getChildByName("role_shadow_img"));

	m_pRole = CMainHero::create();
	m_pRole->setPosition(pRoleShadowImg->getPosition());
	m_pRole->setShowID(uShowID);
	m_pRole->setResPath(bodyName);
	m_pRole->LoadActionRes();
	m_pRole->setHP(1);
	m_pRole->getBlood()->setVisible(false);
    m_pRole->getArmature()->setScale(1.6f);
	m_pRole->Idle();
	m_pRole->setShadowVisiable(false);
	m_pRoleLayout->addNode(m_pRole);
	m_pRole->SwitchWeapon(uWeaponCfgID, false);

	if(m_pRoleLayout->getNodeByTag(101)){
		m_pRoleLayout->removeNodeByTag(101);
	}

	CCArmature *pArm = CCArmature::create("ut00059");
	pArm->setPosition(pRoleShadowImg->getPosition());
	m_pRoleLayout->addNode(pArm, 0, 101);
	pArm->getAnimation()->play("fly", -1, -1, 1);
}

void OtherRoleInfoPanel::updateDetailInfo() {
	const vmsg::CSPlayerInfo &stPlayerInfo = P_LOGIN_OBJ->getOtherPlayerInfo();
	const vmsg::CSPlayerDetailInfo &stPlayerDetailInfo = P_LOGIN_OBJ->getOtherPlayerDetailInfo();

	if(stPlayerInfo.has_uin()) {
		m_pRoleNameTxt->setText(stPlayerInfo.nick());
		m_pVIPTxt->setNum(intToString(stPlayerInfo.viplv()), 0.0f, "vip");
		m_pLevelValTxt->setText(intToString(stPlayerInfo.level()));

		UINT64_t uExp = stPlayerInfo.exp();
		UINT64_t uMaxExp = EXP_CFG_MASTER->getExpCfgByID(stPlayerInfo.level())->exp();
		m_pExpProgress->setPercent(100 * uExp / uMaxExp);
		m_pUINValTxt->setText(intToString((unsigned int)stPlayerInfo.uin()));
	}

	if(stPlayerDetailInfo.has_hp()) {
		m_pHpValTxt->setText(intToString(stPlayerDetailInfo.hp()));
		m_pFightPowerValTxt->setText(intToString(stPlayerDetailInfo.fightpower()));

		m_pDefValTxt->setText(intToString(stPlayerDetailInfo.def()));
		m_pAtkValTxt->setText(intToString(stPlayerDetailInfo.atk()));
		m_pHitValTxt->setText(intToString((unsigned int)(floor(stPlayerDetailInfo.hitwithoutinitval() * 2.3f + 0.5f))));

		std::string atkTypeName = "";

		if(stPlayerDetailInfo.atktype() == CLOSE_PANITRATE) {
			atkTypeName = NounStr(NOUN_CLOSE_PANITRATE);
		} else if(stPlayerDetailInfo.atktype() == CLOSE_BLUNT) {
			atkTypeName = NounStr(NOUN_CLOSE_BLUNT);
		} else if(stPlayerDetailInfo.atktype() == CLOSE_MAGIC) {
			atkTypeName = NounStr(NOUN_CLOSE_MAGIC);
		} else if(stPlayerDetailInfo.atktype() == CLOSE_ORDINARY) {
			atkTypeName = NounStr(NOUN_CLOSE_ORDINARY);
		} else if(stPlayerDetailInfo.atktype() == REMOGE_MAGIC) {
			atkTypeName = NounStr(NOUN_REMOTE_MAGIC);
		} else if(stPlayerDetailInfo.atktype() == REMOTE_PANITRATE) {
			atkTypeName = NounStr(NOUN_REMOTE_PANITRATE);
		} else if(stPlayerDetailInfo.atktype() == REMOTE_ORDINARY) {
			atkTypeName = NounStr(NOUN_REMOTE_ORDINARY);
		}  

		m_pAtkTypeValTxt->setText(atkTypeName);
		m_pAtkTypeValTxt->setVisible(atkTypeName != "");

		std::string defTypeName = "";

		if(stPlayerDetailInfo.deftype() == HEARY_ARMOR_DEF) {
			defTypeName = NounStr(HEARY_ARMOR_DEF_STR);
		} else if(stPlayerDetailInfo.deftype() == LIGHT_ARMOR_DEF) {
			defTypeName = NounStr(LIGHT_ARMOR_DEF_STR);
		} else if(stPlayerDetailInfo.deftype() == NO_ARMOR_DEF) {
			defTypeName = NounStr(NONE_ARMOR_DEF_STR);
		}

		m_pDefTypeValTxt->setText(defTypeName);
		m_pDefTypeValTxt->setVisible(defTypeName != "");

		for(int i = 0; i < stPlayerDetailInfo.atkattrs_size(); i++) {
			const vmsg::CSPlayerAttr &attr = stPlayerDetailInfo.atkattrs(i);

			switch(attr.attrtype()) {
				case ICE_MAGIC:
					m_pIceAtkValTxt->setText(intToString(attr.attrval()));
					break;
				case FLAME_MAGIC:
					m_pFireAtkValTxt->setText(intToString(attr.attrval()));
					break;
				case POISON_MAGIC:
					m_pPoisonAtkValTxt->setText(intToString(attr.attrval()));
					break;
				case BIOTECH_MAGIC:
					m_pBiochemistryAtkValTxt->setText(intToString(attr.attrval()));
					break;
				case LIGHTNING_MAGIC:
					m_pThunderAtkValTxt->setText(intToString(attr.attrval()));
					break;
				default:;
			}
		}

		for(int i = 0; i < stPlayerDetailInfo.defattrs_size(); i++) {
			const vmsg::CSPlayerAttr &attr = stPlayerDetailInfo.defattrs(i);

			switch(attr.attrtype()) {
				case ICE_MAGIC:
					m_pIceDefValTxt->setText(intToString(attr.attrval()));
					break;
				case FLAME_MAGIC:
					m_pFireDefValTxt->setText(intToString(attr.attrval()));
					break;
				case POISON_MAGIC:
					m_pPoisonDefValTxt->setText(intToString(attr.attrval()));
					break;
				case BIOTECH_MAGIC:
					m_pBiochemistryDefValTxt->setText(intToString(attr.attrval()));
					break;
				case LIGHTNING_MAGIC:
					m_pThunderDefValTxt->setText(intToString(attr.attrval()));
					break;
				default:;
			}
		}

	}
}

void OtherRoleInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(std::strcmp(pszWidgetName, m_pOptionBtn->getName()) == 0) {
				if(NounStr(NOUN_BE_FRIEND) == m_pOptionBtn->getTitleText()) {
                    const vmsg::CSPlayerInfo &stPlayerInfo = P_LOGIN_OBJ->getOtherPlayerInfo();
                    P_FRIEND_CTRL->getProxy()->sendAddReqRqst(stPlayerInfo.nick());
                    COMMON_DIALOG->show(TextStr(TEXT_ADD_FRIEND_TIP));
					//CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
				} else if(NounStr(NOUN_DEL_FRIEND) == m_pOptionBtn->getTitleText() ) {
                    const vmsg::CSPlayerInfo &stPlayerInfo = P_LOGIN_OBJ->getOtherPlayerInfo();
                    if(P_FRIEND_CTRL->getFriendPanelUI()->isOpen()) {
                        P_FRIEND_CTRL->getFriendPanelUI()->showLoading();
                    }
                    P_FRIEND_CTRL->getProxy()->sendDelRqst(stPlayerInfo.uin());
                    close();
                }
			} else if(strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				close();
			}
			break;
		default:;
	}
}

void OtherRoleInfoPanel::registerWithTouchDispatcher(){  
	//使用int最小值做最高优先级,并且吞掉事件true  
	setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool OtherRoleInfoPanel::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) { 
	
	CCPoint pTouchPoint = pTouch->getLocation();
	UILayout *pInfoLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("role_info_layout"));
	CCArray* pBtnArray = pInfoLayout->getChildren();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(pBtnArray, pObj)
	{
		Widget* pWidget = dynamic_cast<Widget*>(pObj);
		if (pWidget)
		{
			if (pWidget->hitTest(pTouchPoint))
			{
				pWidget->setFocused(true);
			}
		}
	}
	return true;  
}  

void OtherRoleInfoPanel::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {  
}  


void OtherRoleInfoPanel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
	CCPoint pTouchPoint = pTouch->getLocation();
	UILayout *pInfoLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("role_info_layout"));
	CCArray* pBtnArray = pInfoLayout->getChildren();
	CCObject* pObj = NULL;
	CCARRAY_FOREACH(pBtnArray, pObj)
	{
		Widget* pWidget = dynamic_cast<Widget*>(pObj);
		if (pWidget)
		{
			pWidget->setFocused(false);
			if (pWidget->hitTest(pTouchPoint))
			{
				touchEventHandler(pWidget,TOUCH_EVENT_ENDED);
			}
		}
	}
	
}  
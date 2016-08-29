#include "RoleInfoPanel.h"

#include "Resources.h"

#include "basic/NounHelper.h"
#include "basic/ToolUtils.h"
#include "basic/WeaponUtils.h"

#include "CfgMaster/HeroCfgMaster.h"
#include "CfgMaster/EquipmentCfgMaster.h"
#include "CfgMaster/WeaponCfgMaster.h"
#include "CfgMaster/WeaponRefitAdditionCfgMaster.h"

#include "ClientSvrMsg.pb.h"
#include "CSEquipItemMsg.pb.h"
#include "ModuleID.pb.h"

#include "game/UserObj.h"
#include "game/role/MainHero.h"
#include "game/city/CityControl.h"
#include "game/firearms/StarProgressBar.h"
#include "game/weaponlib/WeaponLibControl.h"
#include "game/login/LoginControl.h"

#include "EquipCell.h"
#include "BagControl.h"
#include "ItemUtils.h"
#include "BagLayout.h"
#include "DetailInfoLayout.h"
#include "EquipInfoPanel.h"
#include "QEnvSet.h"

RoleInfoPanel::RoleInfoPanel()
:m_pBackBtn(NULL)
,m_pRoleNameTxt(NULL)
,m_pRoleLayout(NULL)
,m_pRole(NULL)
,m_pWeaponCell(NULL)
,m_pWeaponStarProgress(NULL)
,m_pArmorCell(NULL)
,m_pAmuletCell(NULL)
,m_pShoesCell(NULL)
,m_pCloakCell(NULL)
,m_pLevelValTxt(NULL)
,m_pHpValTxt(NULL)
,m_pAtkValTxt(NULL)
,m_pDefValTxt(NULL)
,m_pHitValTxt(NULL)
,m_pHpAddValTxt(NULL)
,m_pAtkAddValTxt(NULL)
,m_pDefAddValTxt(NULL)
,m_pHitAddValTxt(NULL)
,m_pBagOrDetailBtn(NULL)
,m_pBagLayout(NULL)
,m_pDetailInfoLayout(NULL)
{
}

RoleInfoPanel::~RoleInfoPanel()
{
}

RoleInfoPanel* RoleInfoPanel::create() {
	RoleInfoPanel *pRet = new RoleInfoPanel();

	if(pRet != NULL && pRet->init()) {
		pRet->autorelease();
	} else {
		CC_SAFE_DELETE(pRet);
	}

	return pRet;
}

bool RoleInfoPanel::init() {
	bool bRet = false;

	do {
		CC_BREAK_IF(!BasePanel::init());

		setOpenWithBgImg(true);
		
		initUI();

		bRet = true;
	} while(0);

	return bRet;
}

void RoleInfoPanel::open() {
	BasePanel::open();

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("effect/ut00059/ut00059.ExportJson");

	P_LOGIN_CTRL->getProxy()->sendQueryPlayerInfoRqst(P_USER_OBJ->getUserInfo()->uin);
	P_LOGIN_CTRL->getProxy()->sendQueryPlayerDetailInfoRqst();

	if(P_CITY_CTRL->isModuleOpen(vmsg::MODULE_ID_BAG)) {
		m_pBagLayout->setEnabled(true);
		m_pDetailInfoLayout->setEnabled(false);
		m_pBagOrDetailBtn->setTitleText(NounStr(NOUN_DETAIL_PROP));
		m_pBagOrDetailBtn->setEnabled(true);
		P_BAG_CTRL->sendQueryBagInfoRqst();
		m_pBagLayout->gotoTabPage(BagLayout::BAG_TAB_ALL);
	} else {
		if(QEnvSet::beck()){
			m_pBagLayout->setEnabled(true);
			m_pDetailInfoLayout->setEnabled(false);
			m_pBagOrDetailBtn->setTitleText(NounStr(NOUN_DETAIL_PROP));
			m_pBagOrDetailBtn->setEnabled(true);
			P_BAG_CTRL->sendQueryBagInfoRqst();
			m_pBagLayout->gotoTabPage(BagLayout::BAG_TAB_ALL);
		}else{
			m_pBagLayout->setEnabled(false);
			m_pDetailInfoLayout->setEnabled(true);
			m_pBagOrDetailBtn->setTitleText(NounStr(NOUN_BAG));
			m_pBagOrDetailBtn->setEnabled(false);
		}
	}

	P_WEAPON_CTRL->sendWeaponBagRqst();
	//updateUI();
}

void RoleInfoPanel::close() {
	BasePanel::close();


	if(m_pRole && m_pRole->getParent()){
		m_pRole->removeFromParentAndCleanup(true);
		m_pRole = NULL;
	}
}

CCNode* RoleInfoPanel::getGuideNodeByName(const std::string &name) {
	CCNode *pRet = NULL;

	if(name == "equip_item_0") {
		pRet = m_pBagLayout->getGuideBagCell(101);
	}else if (name == "equip_item_1")
	{
		pRet = m_pBagLayout->getGuideBagCell(201);
	}else if(name == "back_btn") {
		if(m_pBackBtn != NULL && m_pBackBtn->isRunning() && m_pBackBtn->isTouchEnabled()) {
			pRet = m_pBackBtn;
		}
	} else if (name == "equip_tab")
	{
		pRet = m_pBagLayout->getEquipTab();
	}

	return pRet;
}

void RoleInfoPanel::initUI() {
	initLayoutFromJsonFile("ui_v2/RoleInfoPanelUI.ExportJson");
	addChild(m_pRootLayout);

	UILayout *pRoleInfoLayout = dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("role_info_layout"));

	m_pBackBtn = dynamic_cast<UIButton*>(pRoleInfoLayout->getChildByName("back_btn"));
	m_pBackBtn->addTouchEventListener(this, toucheventselector(RoleInfoPanel::touchEventHandler));

	m_pRoleNameTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("role_name_txt"));

	m_pRoleLayout = dynamic_cast<UILayout*>(pRoleInfoLayout->getChildByName("role_layout"));
	m_pRoleLayout->setTouchEnabled(false);

	m_pWeaponCell = EquipCell::create(dynamic_cast<UILayout*>(pRoleInfoLayout->getChildByName("weapon_cell")));
	m_pWeaponCell->addTouchEventListener(this, toucheventselector(RoleInfoPanel::touchEventHandler));
	m_pWeaponCell->setName("weapon_cell");

	m_pWeaponStarProgress = CStarProgressBar::create("share/little_star_dark.png", "share/little_star_light.png", "ui_v2/share.png");
	m_pWeaponStarProgress->setAnchorPoint(ccp(0.5f, 0.5f));
	m_pWeaponStarProgress->setZOrder(2);
	m_pWeaponStarProgress->setPosition(ccp(m_pWeaponCell->getSize().width / 2, m_pWeaponCell->getSize().height - 8));
	m_pWeaponStarProgress->setStarMargin(1);
	m_pWeaponStarProgress->setStarNum(7);
	m_pWeaponStarProgress->setScale(0.8f);
	m_pWeaponCell->addChild(m_pWeaponStarProgress);

	m_pArmorCell = EquipCell::create(dynamic_cast<UILayout*>(pRoleInfoLayout->getChildByName("armor_cell")));
	m_pArmorCell->addTouchEventListener(this, toucheventselector(RoleInfoPanel::touchEventHandler));
	m_pArmorCell->setName("armor_cell");

	m_pAmuletCell = EquipCell::create(dynamic_cast<UILayout*>(pRoleInfoLayout->getChildByName("amulet_cell")));
	m_pAmuletCell->addTouchEventListener(this, toucheventselector(RoleInfoPanel::touchEventHandler));
	m_pAmuletCell->setName("amulet_cell");

	m_pShoesCell = EquipCell::create(dynamic_cast<UILayout*>(pRoleInfoLayout->getChildByName("shoes_cell")));
	m_pShoesCell->addTouchEventListener(this, toucheventselector(RoleInfoPanel::touchEventHandler));
	m_pShoesCell->setName("shoes_cell");

	m_pCloakCell = EquipCell::create(dynamic_cast<UILayout*>(pRoleInfoLayout->getChildByName("cloak_cell")));
	m_pCloakCell->addTouchEventListener(this, toucheventselector(RoleInfoPanel::touchEventHandler));
	m_pCloakCell->setName("cloak_cell");

	m_pLevelValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("level_val_txt"));

	m_pHpValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("hp_val_txt"));

	m_pAtkValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("atk_val_txt"));

	m_pDefValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("def_val_txt"));

	m_pHitValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("hit_val_txt"));

	m_pHpAddValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("hp_add_val_txt"));
	m_pHpAddValTxt->setEnabled(false);

	m_pAtkAddValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("atk_add_val_txt"));
	m_pAtkAddValTxt->setEnabled(false);

	m_pDefAddValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("def_add_val_txt"));
	m_pDefAddValTxt->setEnabled(false);

	m_pHitAddValTxt = dynamic_cast<UILabel*>(pRoleInfoLayout->getChildByName("hit_add_val_txt"));
	m_pHitAddValTxt->setEnabled(false);

	m_pBagOrDetailBtn = dynamic_cast<UIButton*>(pRoleInfoLayout->getChildByName("bag_toggle_btn"));
	m_pBagOrDetailBtn->addTouchEventListener(this, toucheventselector(RoleInfoPanel::touchEventHandler));
	m_pBagOrDetailBtn->setTitleText(NounStr(NOUN_DETAIL_PROP));

	m_pBagLayout = BagLayout::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("bag_layout")));

	m_pDetailInfoLayout = DetailInfoLayout::create(dynamic_cast<UILayout*>(m_pRootLayout->getChildByName("detail_layout")));
}

void RoleInfoPanel::updateUI() {
	if(!isOpen()) {
		return;
	}

	if(this->getIsInLoading()){
		this->closeLoading();
	}

	updateRole();

	updateRoleInfo();

	updateDetailInfo();

	updateBag();
}

void RoleInfoPanel::updateRoleInfo() {
	const UserInfo *pUserInfo = P_USER_OBJ->getUserInfo();
	m_pRoleNameTxt->setText(pUserInfo->nick);

	m_pLevelValTxt->setText(intToString(pUserInfo->level));
	//m_pLevelValTxt->setText(string("test") + intToString(12345));

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

	const vmsg::CSWeapon &stWeapon =  pUserInfo->weapon;
	const WeaponCfg *pWeaponCfg = WEAPON_CFG_MASTER->GetWeaponCfgByID(stWeapon.cfgid());

	m_pWeaponCell->setWeapon(stWeapon);
	m_pWeaponStarProgress->setStarNum(pWeaponCfg->initstar());

	atkAddVal += pWeaponCfg->atk() + pWeaponCfg->atkadd() * (stWeapon.level() - 1);
	unsigned int colorAdd = WEAPONREFITADDITION_CFG_MASTER->GetCfg(stWeapon.quality())->multi();
	atkAddVal *= (float)colorAdd / 10000;
	atkAddVal = (1.0*atkAddVal*pWeaponCfg->shotspeed())/1270.0f;


	for(unsigned int i = 0; i < pUserInfo->stEquipLst.size(); i++) {
		const vmsg::CSEquipInfo &stEquipInfo = pUserInfo->stEquipLst[i];
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

		//uStrengthenAttrVal = 0;//EQUIP_REFINE_COST_CFG_MASTER->GetColorAttrAdd(stEquipInfo.refinelv(), pEquipCfg->part(), pEquipCfg->color()); //强化加属性
		uStrengthenAttrVal = ItemUtils::getEquipAttrVal(stEquipInfo.equipid(), stEquipInfo.level(),  stEquipInfo.quality() );  //该公式算出的最终总属性

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

	// 大坑说显示总值
	unsigned int weaponAddCoe = WeaponUtils::getWeaponAddCoe(pUserInfo->weapon.cfgid(), pUserInfo->weapon.level(), pUserInfo->weapon.quality());
	m_pHpValTxt->setText(intToString(pUserInfo->hp));
	m_pAtkValTxt->setText(intToString(pUserInfo->atk + weaponAddCoe));
	m_pDefValTxt->setText(intToString(pUserInfo->def));
	m_pHitValTxt->setText(intToString((unsigned int)(floor(pUserInfo->hitWithOutInitVal * 2.3f + 0.5f))));

	/*m_pHpValTxt->setText(intToString(pUserInfo->hp - hpAddVal));
	m_pAtkValTxt->setText(intToString(pUserInfo->atk - atkAddVal));
	m_pDefValTxt->setText(intToString(pUserInfo->def - defAddVal));
	m_pHitValTxt->setText(intToString((unsigned int)(std::floor(pUserInfo->hitWithOutInitVal * 2.3f + 0.5f))));*/

	/*m_pHpAddValTxt->setText(std::string("+") + intToString(hpAddVal));
	m_pHpAddValTxt->setPositionX(m_pHpValTxt->getPositionX() + m_pHpValTxt->getSize().width + 16.0f);
	m_pHpAddValTxt->setVisible(hpAddVal > 0);

	m_pAtkAddValTxt->setText(std::string("+") + intToString(atkAddVal));
	m_pAtkAddValTxt->setPositionX(m_pAtkValTxt->getPositionX() + m_pAtkValTxt->getSize().width + 16.0f);
	m_pAtkAddValTxt->setVisible(atkAddVal > 0);

	m_pDefAddValTxt->setText(std::string("+") + intToString(defAddVal));
	m_pDefAddValTxt->setPositionX(m_pDefValTxt->getPositionX() + m_pDefValTxt->getSize().width + 16.0f);
	m_pDefAddValTxt->setVisible(defAddVal > 0);

	m_pHitAddValTxt->setText(std::string("+") + intToString(0));
	m_pHitAddValTxt->setPositionX(m_pHitValTxt->getPositionX() + m_pHitValTxt->getSize().width + 16.0f);
	m_pHitAddValTxt->setVisible(false);*/
}

void RoleInfoPanel::updateDetailInfo() {
	if(m_pDetailInfoLayout != NULL) {
		m_pDetailInfoLayout->updateUI();
	}
}

void RoleInfoPanel::updateBag() {
	if(m_pBagLayout != NULL) {
		m_pBagLayout->updateUI();
	}
}

void RoleInfoPanel::updateRole() {
	const UserInfo* pUserInfo = P_USER_OBJ->getUserInfo();	//玩家信息
	unsigned int uShowID = pUserInfo->showID;
	unsigned int uWeaponCfgID = pUserInfo->weapon.cfgid();

	// 获取资源路径
	string bodyName;
	int iRet = CMainHero::GetResourceNames(uWeaponCfgID, uShowID, bodyName);

	if(iRet != 0) {
		CCLOG("Fail to get hero res! ShowID=%u WeaponCfgID=%u", uShowID, uWeaponCfgID);
		return;
	}

	if(m_pRole != NULL) {
		m_pRole->removeFromParent();
	} 

	UIImageView *pRoleShadowImg = dynamic_cast<UIImageView*>(m_pRoleLayout->getChildByName("role_shadow_img"));

	m_pRole = CMainHero::create();
	m_pRole->setPosition(pRoleShadowImg->getPosition());
	m_pRole->setShowID(uShowID);
	m_pRole->setResPath(bodyName);
	m_pRole->LoadActionRes();
	m_pRole->setHP(1);
	m_pRole->getBlood()->setVisible(false);
	m_pRole->Idle();
    m_pRole->setShadowVisiable(false);
	m_pRoleLayout->addNode(m_pRole, 2);
	m_pRole->SwitchWeapon(uWeaponCfgID, false);
	m_pRole->setScale(1.6f);

	if(m_pRoleLayout->getNodeByTag(101)){
		m_pRoleLayout->removeNodeByTag(101);
	}

	CCArmature *pArm = CCArmature::create("ut00059");
    if(pArm) {
        pArm->setPosition(pRoleShadowImg->getPosition());
        m_pRoleLayout->addNode(pArm, 0, 101);
        pArm->getAnimation()->play("fly", -1, -1, 1);
    }
	
}

void RoleInfoPanel::touchEventHandler(CCObject *pSender, TouchEventType type) {
	UIWidget *pWidget = dynamic_cast<UIWidget*>(pSender);
	const char *pszWidgetName = pWidget->getName();

	switch(type) {
		case TOUCH_EVENT_ENDED:
			if(strcmp(pszWidgetName, m_pBackBtn->getName()) == 0) {
				GUIDE_TOUCH(pWidget);
				close();
			} else if(std::strcmp(pszWidgetName, m_pBagOrDetailBtn->getName()) == 0) {
				if(NounStr(NOUN_DETAIL_PROP) == m_pBagOrDetailBtn->getTitleText()) {
					m_pBagLayout->setEnabled(false);
					m_pDetailInfoLayout->setEnabled(true);
					m_pDetailInfoLayout->updateUI();
					m_pBagOrDetailBtn->setTitleText(NounStr(NOUN_BAG));
				} else if(NounStr(NOUN_BAG) == m_pBagOrDetailBtn->getTitleText()) {
					m_pBagLayout->setEnabled(true);
					m_pDetailInfoLayout->setEnabled(false);
					m_pDetailInfoLayout->updateUI();
					m_pBagOrDetailBtn->setTitleText(NounStr(NOUN_DETAIL_PROP));
				}
			} else if(std::strcmp(pszWidgetName, m_pArmorCell->getName()) == 0
				|| std::strcmp(pszWidgetName, m_pAmuletCell->getName()) == 0
				|| std::strcmp(pszWidgetName, m_pShoesCell->getName()) == 0
				|| std::strcmp(pszWidgetName, m_pCloakCell->getName()) == 0) {
					EquipCell *pCell = dynamic_cast<EquipCell*>(pWidget);

					const vmsg::CSEquipInfo &stEquipInfo = pCell->getInfo();

					if(stEquipInfo.has_equipguid()) {
						P_BAG_CTRL->getEquipInfoPanel()->open();
						P_BAG_CTRL->getEquipInfoPanel()->updateInfo(stEquipInfo, EquipInfoPanel::EQUIP_ON_ROLE);
					}
					
			} else if(std::strcmp(pszWidgetName, m_pWeaponCell->getName()) == 0) {
				
				const vmsg::CSWeapon &stWeapon =  P_USER_OBJ->getUserInfo()->weapon;
				P_WEAPON_CTRL->getWeaponInfoPanel()->openInSeq(this);
				P_WEAPON_CTRL->getWeaponInfoPanel()->updateWeaponInfo(stWeapon.cfgid(), stWeapon.weaponguid());
			}
			break;
		default:;
	}
}
